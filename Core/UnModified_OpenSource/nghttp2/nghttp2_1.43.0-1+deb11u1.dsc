-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: nghttp2
Binary: libnghttp2-dev, libnghttp2-doc, libnghttp2-14, nghttp2-client, nghttp2-proxy, nghttp2-server, nghttp2
Architecture: any all
Version: 1.43.0-1+deb11u1
Maintainer: Tomasz Buchert <tomasz@debian.org>
Uploaders: Ondřej Surý <ondrej@debian.org>
Homepage: https://nghttp2.org/
Standards-Version: 4.5.1
Vcs-Browser: https://salsa.debian.org/debian/nghttp2
Vcs-Git: https://salsa.debian.org/debian/nghttp2.git
Testsuite: autopkgtest
Testsuite-Triggers: nginx
Build-Depends: debhelper (>= 13), debhelper-compat (= 13), dpkg-dev (>= 1.17.14), libc-ares-dev, libcunit1-dev <!nocheck>, libev-dev, libevent-dev, libjansson-dev, libjemalloc-dev [!hurd-i386], libssl-dev, libsystemd-dev, libxml2-dev, pkg-config, zlib1g-dev
Build-Depends-Indep: python3-sphinx
Package-List:
 libnghttp2-14 deb libs optional arch=any
 libnghttp2-dev deb libdevel optional arch=any
 libnghttp2-doc deb doc optional arch=all
 nghttp2 deb httpd optional arch=all
 nghttp2-client deb httpd optional arch=any
 nghttp2-proxy deb httpd optional arch=any
 nghttp2-server deb httpd optional arch=any
Checksums-Sha1:
 cf95821d4f5afe5b69911eb98fb1f4681c8c86d0 4521786 nghttp2_1.43.0.orig.tar.bz2
 4a07939c82b2e6893fe19c681b9a8f5b99105880 21832 nghttp2_1.43.0-1+deb11u1.debian.tar.xz
Checksums-Sha256:
 556f24653397c71ebb8270b3c5e5507f0893e6eac2c6eeda6be2ecf6e1f50f62 4521786 nghttp2_1.43.0.orig.tar.bz2
 b391becc0718fdc415d2f966d577a864f996068e828ce803600d34b0bf03baf2 21832 nghttp2_1.43.0-1+deb11u1.debian.tar.xz
Files:
 d7d7d01fd9c5d30c2960ae4349e6b6b7 4521786 nghttp2_1.43.0.orig.tar.bz2
 0161fdba3fd82d13ae21ae43e8b42513 21832 nghttp2_1.43.0-1+deb11u1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCgAdFiEEtuYvPRKsOElcDakFEMKTtsN8TjYFAmVguKkACgkQEMKTtsN8
TjY/mg/+M3xHKZ1ugCBmHOo+nHnWl9eodQQjAJt3zcexHd0yIG8g4yDaB0EQsx+3
8Rshla/x1CqFCCwN9uEkXOfmRgPcxplven8JxPeoqFQx1kpG0lSh92IybPooq0h5
sGAU8T5Y+Xge7qlJIC6EE2m4laQwoQZr0DsmR/x5n4Hf470iFUJEXwaUDfCm2FaY
Z9zIHjKtAC9gWP+os7ahkYZJ0SWpsnX8S2xCW9lp6cIvDTmVpPkRFYrxH/w+moBx
l5huOzXf4DOutuR2biyXZvlA3UT2NeEMyBu+9094lRoDZaDekzjWTrNlNaVzNlwJ
TG49W865qA9wieG+aD4biR/Tfw4HVgHlngDHSAhz8tEYpu1E8/8s32bSulbmui3G
jxTvkxQIgYw7+BxGm6SzAfKpvItxx+kd2ZU+M9BR3Y5yi5pE11DIGg1jQKqMQjy2
cWfRJAbalXvEeLG6APuxjWznwMZe12L1zKhSUTQyu9FpHBmNC1cN4dxafuFnQyHA
xESbXoz9m9A+Tjp2WOjIdoqcyxSJaSSjHWNKtjjmHUVVPHm9bP6LCUE1gXhg757j
wQdSdDIoGk73jxGlhuQkz78smbh6i3fNAwVA5mORP/FaJa9jFygQpume5Os7MJWU
HBtdZCsTjMOmkDeXbHX4vQhMLZdPgIfmYTW8FNzPQVkI0tjsEUs=
=bIwE
-----END PGP SIGNATURE-----
