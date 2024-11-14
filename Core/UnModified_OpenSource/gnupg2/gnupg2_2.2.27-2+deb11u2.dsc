-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: gnupg2
Binary: gpgconf, gnupg-agent, gpg-agent, gpg-wks-server, gpg-wks-client, scdaemon, gpgsm, gpg, gnupg, gnupg2, gpgv, gpgv2, dirmngr, gpgv-udeb, gpgv-static, gpgv-win32, gnupg-l10n, gnupg-utils
Architecture: any all
Version: 2.2.27-2+deb11u2
Maintainer: Debian GnuPG Maintainers <pkg-gnupg-maint@lists.alioth.debian.org>
Uploaders:  Eric Dorland <eric@debian.org>, Daniel Kahn Gillmor <dkg@fifthhorseman.net>, Christoph Biedl <debian.axhn@manchmal.in-ulm.de>,
Homepage: https://www.gnupg.org/
Standards-Version: 4.5.1
Vcs-Browser: https://salsa.debian.org/debian/gnupg2
Vcs-Git: https://salsa.debian.org/debian/gnupg2.git -b debian/bullseye
Testsuite: autopkgtest
Testsuite-Triggers: debian-archive-keyring, diffutils, gnupg1, wine32
Build-Depends: automake, autopoint, debhelper-compat (= 13), file, gettext, ghostscript, gpgrt-tools, imagemagick, libassuan-dev (>= 2.5.0), libbz2-dev, libcurl4-gnutls-dev, libgcrypt20-dev (>= 1.8.0), libgnutls28-dev (>= 3.0), libgpg-error-dev (>= 1.35), libksba-dev (>= 1.3.5), libldap2-dev, libnpth0-dev (>= 1.2), libreadline-dev, librsvg2-bin, libsqlite3-dev, libusb-1.0-0-dev [!hurd-any], openssh-client <!nocheck>, pkg-config, texinfo, transfig, zlib1g-dev | libz-dev
Build-Depends-Indep: binutils-multiarch [!amd64 !i386], libassuan-mingw-w64-dev (>= 2.5.0), libgcrypt-mingw-w64-dev (>= 1.8.0), libgpg-error-mingw-w64-dev (>= 1.26-2~), libksba-mingw-w64-dev (>= 1.3.5), libnpth-mingw-w64-dev (>= 1.2), libz-mingw-w64-dev, mingw-w64
Package-List:
 dirmngr deb utils optional arch=any
 gnupg deb utils optional arch=all
 gnupg-agent deb oldlibs optional arch=all
 gnupg-l10n deb localization optional arch=all
 gnupg-utils deb utils optional arch=any
 gnupg2 deb oldlibs optional arch=all
 gpg deb utils optional arch=any
 gpg-agent deb utils optional arch=any
 gpg-wks-client deb utils optional arch=any
 gpg-wks-server deb utils optional arch=any
 gpgconf deb utils optional arch=any
 gpgsm deb utils optional arch=any
 gpgv deb utils important arch=any
 gpgv-static deb utils optional arch=any
 gpgv-udeb udeb debian-installer optional arch=any
 gpgv-win32 deb utils optional arch=all
 gpgv2 deb oldlibs optional arch=all
 scdaemon deb utils optional arch=any
Checksums-Sha1:
 d928d4bd0808ffb8fe20d1161501401d5d389458 7191555 gnupg2_2.2.27.orig.tar.bz2
 7fc979ac1633b07f7ccb2fa06150402935726b05 119 gnupg2_2.2.27.orig.tar.bz2.asc
 6db567be004ab69ce5f8496e2d62060a90363157 63960 gnupg2_2.2.27-2+deb11u2.debian.tar.xz
Checksums-Sha256:
 34e60009014ea16402069136e0a5f63d9b65f90096244975db5cea74b3d02399 7191555 gnupg2_2.2.27.orig.tar.bz2
 2b44fd82da223cb629062b9c8840d92698c003be8531fc393c38f97b28cae2a4 119 gnupg2_2.2.27.orig.tar.bz2.asc
 b35c6a717d7f79cfd1a7468436721ca9c9211f70d10216e22523478094670a7b 63960 gnupg2_2.2.27-2+deb11u2.debian.tar.xz
Files:
 a9c002b5356103c97412955a1956ae0c 7191555 gnupg2_2.2.27.orig.tar.bz2
 3a7ebb524a333b41032765eb651ea032 119 gnupg2_2.2.27.orig.tar.bz2.asc
 1062344355df19f937226cba648c4098 63960 gnupg2_2.2.27-2+deb11u2.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iHUEARYIAB0WIQQttUkcnfDcj0MoY88+nXFzcd5WXAUCYr8HvQAKCRA+nXFzcd5W
XBddAQD+QzLzJf3asUkkWtaS31e+9tmcl0ocse8X5fSXa2Rb/gEA90mQ6eBSLapY
1GLddZybDNrY751orb2XrS0/m8meIAk=
=Rono
-----END PGP SIGNATURE-----
