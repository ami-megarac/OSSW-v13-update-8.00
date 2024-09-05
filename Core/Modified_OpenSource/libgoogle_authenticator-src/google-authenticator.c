// Helper program to generate a new secret for use in two-factor
// authentication.
//
// Copyright 2010 Google Inc.
// Author: Markus Gutschke
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "config.h"

#include <assert.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "base32.h"
#include "hmac.h"
#include "sha1.h"

#include "safesystem.h"

#define SECRET "/.google_authenticator"
#define SECRET_PATH "/conf/google_otp/"
#define SECRET_BITS 128                          // Must be divisible by eight
#define VERIFICATION_CODE_MODULUS (1000 * 1000)  // Six digits
#define SCRATCHCODES 5                           // Default number of initial scratchcodes
#define MAX_SCRATCHCODES 10                      // Max number of initial scratchcodes
#define SCRATCHCODE_LENGTH 8                     // Eight digits per scratchcode
#define BYTES_PER_SCRATCHCODE 4                  // 32bit of randomness is enough
#define BITS_PER_BASE32_CHAR 5                   // Base32 expands space by 8/5

#define MAX_URL_LEN 1024  // Base32 expands space by 8/5

static int generateCode(const char *key, unsigned long tm) {
    uint8_t challenge[8];
    int i;
    for (i = 8; i--; tm >>= 8) {
        challenge[i] = tm;
    }

    // Estimated number of bytes needed to represent the decoded secret. Because
    // of white-space and separators, this is an upper bound of the real number,
    // which we later get as a return-value from base32_decode()
    int secretLen = (strlen(key) + 7) / 8 * BITS_PER_BASE32_CHAR;

    // Sanity check, that our secret will fixed into a reasonably-sized static
    // array.
    if (secretLen <= 0 || secretLen > 100) {
        return -1;
    }

    // Decode secret from Base32 to a binary representation, and check that we
    // have at least one byte's worth of secret data.
    uint8_t secret[100];
    if ((secretLen = base32_decode((const uint8_t *)key, secret, secretLen)) < 1) {
        return -1;
    }

    // Compute the HMAC_SHA1 of the secret and the challenge.
    uint8_t hash[SHA1_DIGEST_LENGTH];
    hmac_sha1(secret, secretLen, challenge, 8, hash, SHA1_DIGEST_LENGTH);

    // Pick the offset where to sample our hash value for the actual verification
    // code.
    const int offset = hash[SHA1_DIGEST_LENGTH - 1] & 0xF;

    // Compute the truncated hash in a byte-order independent loop.
    unsigned int truncatedHash = 0;
    for (i = 0; i < 4; ++i) {
        truncatedHash <<= 8;
        truncatedHash |= hash[offset + i];
    }

    // Truncate to a smaller number of digits.
    truncatedHash &= 0x7FFFFFFF;
    truncatedHash %= VERIFICATION_CODE_MODULUS;

    return truncatedHash;
}

// return the user name in heap-allocated buffer.
// Caller frees.
static const char *getUserName(uid_t uid) {
    struct passwd pwbuf, *pw;
    char *buf;
#ifdef _SC_GETPW_R_SIZE_MAX
    int len = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (len <= 0) {
        len = 4096;
    }
#else
    int len = 4096;
#endif
    buf = malloc(len);
    char *user;
    if (getpwuid_r(uid, &pwbuf, buf, len, &pw) || !pw) {
        user = malloc(32);
        snprintf(user, 32, "%d", uid);
    } else {
        user = strdup(pw->pw_name);
        if (!user) {
            perror("malloc()");
            _exit(1);
        }
    }
    free(buf);
    return user;
}

static const char *urlEncode(const char *s) {
    const size_t size = 3 * strlen(s) + 1;
    if (size > 10000) {
        // Anything "too big" is too suspect to let through.
        fprintf(stderr, "Error: Generated URL would be unreasonably large.\n");
        exit(1);
    }
    char *ret = malloc(size);
    char *d = ret;
    do {
        switch (*s) {
            case '%':
            case '&':
            case '?':
            case '=':
            encode:
                snprintf(d, size - (d - ret), "%%%02X", (unsigned char)*s);
                d += 3;
                break;
            default:
                if ((*s && *s <= ' ') || *s >= '\x7F') {
                    goto encode;
                }
                *d++ = *s;
                break;
        }
    } while (*s++);
    char *newret = realloc(ret, strlen(ret) + 1);
    if (newret) {
        ret = newret;
    }
    return ret;
}

static const char *getURL(const char *secret, const char *label, char **encoderURL,
                          const int use_totp, const char *issuer) {
    const char *encodedLabel = urlEncode(label);
    char *url;
    const char totp = use_totp ? 't' : 'h';
    if (asprintf(&url, "otpauth://%cotp/%s?secret=%s", totp, encodedLabel, secret) < 0) {
        fprintf(stderr, "String allocation failed, probably running out of memory.\n");
        _exit(1);
    }

    if (issuer != NULL && strlen(issuer) > 0) {
        // Append to URL &issuer=<issuer>
        const char *encodedIssuer = urlEncode(issuer);
        char *newUrl;
        if (asprintf(&newUrl, "%s&issuer=%s", url, encodedIssuer) < 0) {
            fprintf(stderr,
                    "String allocation failed, probably running out of memory.\n");
            _exit(1);
        }
        free((void *)encodedIssuer);
        free(url);
        url = newUrl;
    }

    if (encoderURL) {
        // Show a QR code.
        const char *encoder =
            "https://www.google.com/chart?chs=200x200&"
            "chld=M|0&cht=qr&chl=";
        const char *encodedURL = urlEncode(url);

        *encoderURL =
            strcat(strcpy(malloc(strlen(encoder) + strlen(encodedURL) + 1), encoder),
                   encodedURL);
        free((void *)encodedURL);
    }
    free((void *)encodedLabel);
    return url;
}

// Display to the user what they need to provision their app.
static void displayEnrollInfo(const char *secret, const char *label, const int use_totp,
                              const char *issuer) {
    char *encoderURL;
    const char *url = getURL(secret, label, &encoderURL, use_totp, issuer);
    printf(
        "Warning: pasting the following URL into your browser exposes the OTP secret to "
        "Google:\n  %s\n",
        encoderURL);

    free((char *)url);
    free(encoderURL);
}

/*
static char *addOption(char *buf, size_t nbuf, const char *option) {
    assert(strlen(buf) + strlen(option) < nbuf);
    char *scratchCodes = strchr(buf, '\n');
    assert(scratchCodes);
    scratchCodes++;
    memmove(scratchCodes + strlen(option), scratchCodes,
            strlen(scratchCodes) + 1);
    memcpy(scratchCodes, option, strlen(option));
    return buf;
}
*/

int create_secret(char *username, int channel_num, char *url, int *scratch_code, int *scratch_num) {

    uint8_t buf[SECRET_BITS / 8 + MAX_SCRATCHCODES * BYTES_PER_SCRATCHCODE];
    static const char totp[] = "\" TOTP_AUTH\n";
    // static const char hotp[] = "\" HOTP_COUNTER 0\n";
    // static const char disallow[]  = "\" DISALLOW_REUSE\n";
    static const char window[] = "\" WINDOW_SIZE 3\n"; // default for HOTP is 17\n";
    // static const char ratelimit[] = "\" RATE_LIMIT 3 30\n";
    char secret[(SECRET_BITS + BITS_PER_BASE32_CHAR - 1) / BITS_PER_BASE32_CHAR +
                1 /* newline */ +
                sizeof(totp) +
                // sizeof(hotp) +  // hotp and totp are mutually exclusive.
                // sizeof(disallow) +
                sizeof(window) +
                // sizeof(ratelimit) + 5 + // NN MMM (total of five digits)
                SCRATCHCODE_LENGTH * (MAX_SCRATCHCODES + 1 /* newline */) +
                1 /* NUL termination character */];
    int quiet = 1; /* 0/1 -- disable/enalbe quite mode */
    // int r_limit = 0, r_time = 0; /*-r , --rate-limit=N  Limit logins to N per every M
    // seconds ; -R,  --rate-time=M Limit logins to N per every M seconds ; Must set -r
    // when setting -R, and vice versa ; -r requires an argument in the range 1..10 ; -R
    // requires an argument in the range 15..600*/

    // channel path
    char channel[8];
    char secret_ch[sizeof(SECRET_PATH) + sizeof(channel)];
    memset(secret_ch, 0, sizeof(secret_ch));
    char secret_fn[sizeof(SECRET_PATH) + sizeof(channel) + 
                   strlen(username) +
                   sizeof(SECRET)]; /* -s, --secret=<file> Specify a non-standard file location */
    char *label = NULL; /* -l, --label=<label> Override the default label in \"otpauth://\" URL */
    char *issuer =
        NULL; /* -i, --issuer=<issuer> Override the default issuer in \"otpauth://\" URL */
    // int window_size = 0; /* Set window of concurrently valid codes, -w requires an
    // argument in the range 1..21*/
    int emergency_codes =
        SCRATCHCODES; /* fprintf(stderr, "-e requires an argument in the range 0..%d\n", MAX_SCRATCHCODES);
                         -e, --emergency-codes=N Number of emergency codes to generate */
    // HOTP = 0; TOTP = 1
    int use_totp = 1;

    char s[1024];
    struct stat st = {0};

    memset(secret_fn, 0, sizeof(secret_fn));

    snprintf(channel, sizeof(channel), "ch%d/", channel_num);

    snprintf(secret_fn, sizeof(SECRET_PATH) + sizeof(channel) + strlen(username) + sizeof(SECRET), "%s%s%s%s",
             SECRET_PATH, channel, username, SECRET);
    // snprintf(secret_fn, sizeof(SECRET_PATH) + 5 + sizeof(SECRET), "%s%s%s",
    // SECRET_PATH, username, SECRET);

    if (!label) {
        const uid_t uid = getuid();
        const char *user = getUserName(uid);
        char hostname[128] = {0};
        if (gethostname(hostname, sizeof(hostname) - 1)) {
            strcpy(hostname, "unix");
        }
        label =
            strcat(strcat(strcpy(malloc(strlen(user) + strlen(hostname) + 2), user), "@"),
                   hostname);
        free((char *)user);
    }

    if (!issuer) {
        char hostname[128] = {0};
        if (gethostname(hostname, sizeof(hostname) - 1)) {
            strcpy(hostname, "unix");
        }

        issuer = strdup(username);
    }

    // Not const because 'fd' is reused. TODO.
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0) {
        perror("Failed to open \"/dev/urandom\"");
        free(label);
        free(issuer);
        return 1;
    }
    if (read(fd, buf, sizeof(buf)) != sizeof(buf)) {
    urandom_failure:
        perror("Failed to read from \"/dev/urandom\"");
        if (label) { free(label); }
        if (issuer) { free(issuer); }
        close(fd);
        return 1;
    }

    base32_encode(buf, SECRET_BITS / 8, (uint8_t *)secret, sizeof(secret));

    if (!quiet) {
        displayEnrollInfo(secret, label, use_totp, issuer);
        printf("Your new secret key is: %s\n", secret);

        const unsigned long tm = 1;
        printf("Your verification code for code %lu is %06d\n", tm,
               generateCode(secret, tm));
        printf("Your emergency scratch codes are:\n");
    }
    char *encoderURL;
    const char *s_url = getURL(secret, label, &encoderURL, use_totp, issuer);

    // strncpy(url, s_url, 1024);
    if (snprintf(url, MAX_URL_LEN, "%s", s_url) >= MAX_URL_LEN) {
        free((char *)s_url);
        free(encoderURL);
        free(label);
        free(issuer);
        close(fd);
        return -1;
    }

    free((char *)s_url);
    free(encoderURL);

    free(label);
    label = NULL;
    free(issuer);
    issuer = NULL;
    strcat(secret, "\n");
    strcat(secret, totp);
    // strcat(secret, hotp);
    strcat(secret, window);
    int i;
    for (i = 0; i < emergency_codes; ++i) {
    new_scratch_code:;
        int scratch = 0;
        int j;
        for (j = 0; j < BYTES_PER_SCRATCHCODE; ++j) {
            scratch =
                256 * scratch + buf[SECRET_BITS / 8 + BYTES_PER_SCRATCHCODE * i + j];
        }
        int modulus = 1;
        for (j = 0; j < SCRATCHCODE_LENGTH; j++) {
            modulus *= 10;
        }
        scratch = (scratch & 0x7FFFFFFF) % modulus;
        if (scratch < modulus / 10) {
            // Make sure that scratch codes are always exactly eight digits. If they
            // start with a sequence of zeros, just generate a new scratch code.
            if (read(fd, buf + (SECRET_BITS / 8 + BYTES_PER_SCRATCHCODE * i),
                     BYTES_PER_SCRATCHCODE) != BYTES_PER_SCRATCHCODE) {
                goto urandom_failure;
            }
            goto new_scratch_code;
        }
        if (!quiet) {
            printf("  %08d\n", scratch);
        }
        snprintf(strrchr(secret, '\000'), sizeof(secret) - strlen(secret), "%08d\n",
                 scratch);

        scratch_code[i] = scratch;
        (*scratch_num)++;
        // memcpy(*scratch_code+i, secret, sizeof(secret));
        // printf("%x\n",*scratch_code[0]);
    }
    close(fd);

    const int size = strlen(secret_fn) + 3;
    char *tmp_fn = malloc(size);
    if (!tmp_fn) {
        perror("malloc()");
        _exit(1);
    }
    snprintf(tmp_fn, size, "%s~", secret_fn);

    /*
    // Add optional flags.
    // Counter based.
    memset(s, 0, sizeof(s));
    snprintf(s, sizeof s, "\" WINDOW_SIZE %d\n", window_size);
    addOption(secret, sizeof(secret), s);
    */

    /*
    if (r_limit > 0 && r_time > 0) {
        char s[80];
        snprintf(s, sizeof s, "\" RATE_LIMIT %d %d\n", r_limit, r_time);
        addOption(secret, sizeof(secret), s);
    }
    */

    if (stat(SECRET_PATH, &st) == -1) {
        if (mkdir(SECRET_PATH, 0777) == -1)
        {
            perror("mkdir");
            goto errout;
        }
    }

    snprintf(secret_ch, sizeof(SECRET_PATH) + sizeof(channel), "%s%s", SECRET_PATH, channel);
    if (stat(secret_ch, &st) == -1) {
        if (mkdir(secret_ch, 0777) == -1)
        {
            perror("mkdir");
            goto errout;
        }
    }

    memset(s, 0, sizeof(s));
    snprintf(s, sizeof(SECRET_PATH) + sizeof(channel) + strlen(username), "%s%s%s", SECRET_PATH, channel, username);
    if (stat(s, &st) == -1) {
        if (mkdir(s, 0777) == -1)
        {
            perror("mkdir");
            goto errout;
        }
    }
    
    if (system("chmod -R 777 /conf/google_otp") == -1)
    {
        perror("system");
        goto errout;
    }

    fd = open(tmp_fn, O_WRONLY | O_EXCL | O_CREAT | O_NOFOLLOW | O_TRUNC, 0644);
    if (fd < 0) {
        fprintf(stderr, "Failed to create \"%s\" (%s)", secret_fn, strerror(errno));
        goto errout;
    }

    if (write(fd, secret, strlen(secret)) != (ssize_t)strlen(secret) ||
        rename(tmp_fn, secret_fn)) {
        perror("Failed to write new secret");
        unlink(secret_fn);
        goto errout;
    }
    printf(s, sizeof s, "updated \"%s\" file", secret_fn);

    free(tmp_fn);
    // free(secret_fn);
    close(fd);

    return 0;

errout:
    if (fd >= 0) {
        /* Reason: After the other "close" will not reach here. */
        /* coverity[double_close: FALSE] */
        close(fd);
    }
    // free(secret_fn);
    free(tmp_fn);
    return 1;
}
