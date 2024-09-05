/*------------------------------------------------------------------
 * safe_config.h -- Safe C Lib configs
 * @configure_input@
 *
 * August 2017, Reini Urban
 *
 * Copyright (c) 2017 by Reini Urban
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *------------------------------------------------------------------
 */

#ifndef __SAFE_LIB_CONFIG_H__
#define __SAFE_LIB_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __MINGW32__
# if defined __MINGW64_VERSION_MAJOR && defined __MINGW64_VERSION_MINOR
#   define HAVE_MINGW64  /* mingw-w64 (either 32 or 64bit) */
# else
#   define HAVE_MINGW32  /* old mingw */
# endif
#endif

/*
 * Safe Lib specific configuration values.
 */

/*
 * We depart from the C11 standard and allow memory and string
 * operations to have different max sizes. See the respective
 * safe_mem_lib.h or safe_str_lib.h files.
 */

#ifndef RSIZE_MAX_MEM
/* maximum buffer length. default: 256UL << 20 (256MB) */
#ifndef SPX_BMC
#define RSIZE_MAX_MEM @RSIZE_MAX_MEM@
#else
#define RSIZE_MAX_MEM (256UL << 20)
#endif
#endif

#ifndef RSIZE_MAX_STR
/* maximum string length. default: 4UL << 10 (4KB) */
#ifndef SPX_BMC
#define RSIZE_MAX_STR @RSIZE_MAX_STR@
#else
#define RSIZE_MAX_STR (4UL << 10)
#endif
#endif

/* Null out the remaining part of a string buffer if it is not completely used */
#ifndef SPX_BMC
@INSERT_NULLSLACK@
#else
#define SAFECLIB_STR_NULL_SLACK 1
#endif

/* Disable the C11 invoke_safe_{str,mem}_constraint_handler callbacks on errors,
   for performance, smaller size and less flexibility. */
#ifndef SPX_BMC
@INSERT_CONSTRAINT_HANDLER@
#else
#undef SAFECLIB_DISABLE_CONSTRAINT_HANDLER
#endif

/* Define to include some additional unsafe C11 functions:
 * tmpnam_s
 */
#ifndef SPX_BMC
@INSERT_UNSAFE@
#else
#define SAFECLIB_ENABLE_UNSAFE 1
#endif

/* Define to disable additional functions not defined
 * in the C11 appendix K specification
 */
#ifndef SPX_BMC
@INSERT_EXTS@
#else
#undef SAFECLIB_DISABLE_EXTENSIONS
#endif

/* Define to disable new multibyte and wchar support.
 * E.g. for the linux kernel
 */
#ifndef SPX_BMC
@INSERT_DISABLE_WCHAR@
#else
#undef SAFECLIB_DISABLE_WCHAR
#endif

/* Define to disable linking with dllimport, only relevant to windows.
 * Defined with a static libsafec.
 */
#ifndef SPX_BMC
@INSERT_DISABLE_DLLIMPORT@
#else
#undef DISABLE_DLLIMPORT
#endif

/*
 * Defined by your compiler when __builtin_object_size() is available
 * for compile-time dmax checks. Override when you use a different compiler.
 * Available since: clang-3.3+, gcc-4.1+.
 */
#ifndef HAVE___BUILTIN_OBJECT_SIZE
#ifndef SPX_BMC
@INSERT_OBJECT_SIZE@
#else
#define HAVE___BUILTIN_OBJECT_SIZE 1
#endif
#endif

/*
 * Defined by your compiler when __builtin_constant_p() is available
 * for compile-time checks. Override when you use a different compiler.
 * Available since: clang-3.1+, gcc-3.4+.
 */
#ifndef HAVE___BUILTIN_CONSTANT_P
#ifndef SPX_BMC
@INSERT_CONSTANT_P@
#else
#define HAVE___BUILTIN_CONSTANT_P 1
#endif
#endif

/*
 * Defined by --enable-warn-dmax, to enable dmax checks against 
 *  __builtin_object_size(dest)
 */
#ifndef SPX_BMC
@INSERT_WARN_DMAX@
#else
#undef HAVE_WARN_DMAX
#endif

/*
 * Defined by --enable-error-dmax to make HAVE_WARN_DMAX fatal
 */
#ifndef SPX_BMC
@INSERT_ERROR_DMAX@
#else
#undef HAVE_ERROR_DMAX
#endif

/*
 * Set if libsafec3 was compiled with C99 support.
 */
#ifndef SPX_BMC
@INSERT_SAFECLIB_HAVE_C99@
#else
#define SAFECLIB_HAVE_C99 1
#endif

/*
 * The spec does not call out a maximum len for the strtok src
 * string (the max delims size), so one is defined here.
 */
#ifndef STRTOK_DELIM_MAX_LEN
#define  STRTOK_DELIM_MAX_LEN  16
#endif

#ifdef __cplusplus
}
#endif

#endif /* __SAFE_LIB_CONFIG_H__ */
