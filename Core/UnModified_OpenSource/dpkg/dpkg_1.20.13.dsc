-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (native)
Source: dpkg
Binary: dpkg, libdpkg-dev, dpkg-dev, libdpkg-perl, dselect
Architecture: any all
Version: 1.20.13
Maintainer: Dpkg Developers <debian-dpkg@lists.debian.org>
Uploaders: Guillem Jover <guillem@debian.org>
Homepage: https://wiki.debian.org/Teams/Dpkg
Standards-Version: 4.5.1
Vcs-Browser: https://git.dpkg.org/cgit/dpkg/dpkg.git
Vcs-Git: https://git.dpkg.org/git/dpkg/dpkg.git
Testsuite: autopkgtest
Testsuite-Triggers: autoconf, build-essential, eatmydata, file, pkg-config
Build-Depends: debhelper-compat (= 13), pkg-config, gettext (>= 0.19.7), po4a (>= 0.59), zlib1g-dev, libbz2-dev, liblzma-dev, libselinux1-dev [linux-any], libncurses-dev (>= 6.1+20180210) | libncursesw5-dev, bzip2 <!nocheck>, xz-utils <!nocheck>
Package-List:
 dpkg deb admin required arch=any essential=yes
 dpkg-dev deb utils optional arch=all
 dselect deb admin optional arch=any
 libdpkg-dev deb libdevel optional arch=any
 libdpkg-perl deb perl optional arch=all
Checksums-Sha1:
 1e26d87ee9c7c893ac04f095c022a364f6b8964b 5009848 dpkg_1.20.13.tar.xz
Checksums-Sha256:
 d2aa90535d750b20d1f40827344d74bb552ee5d40d3c24603496fd438f15424d 5009848 dpkg_1.20.13.tar.xz
Files:
 8af062f98701aab8081013c15698d4aa 5009848 dpkg_1.20.13.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCgAdFiEETz509DYFDBD1aWV0uXK/PqSuV6MFAmUCLqsACgkQuXK/PqSu
V6Nh3hAAgDNYQjGwUjIjsBgPE4uWBvbrEfVxGt5CAQ+ih0SVCs3eOI5Y+SW2C/wf
y1wecJl/MQMAZr7PU++jeu+7PcU1PBMrAQMfcOtarTwik5CNFw5xL5VKhFUCfjdd
VYvkOmxLXON9DDCzj1N5nq06KZGRMd0ZJekIwC1el6hjkVf1Ecw/0PqpNKOT/DVZ
rYr1iU2DBYqXqhk9w57uV9DzhKsV3MbrdiGaFJ7IgDdcNnajIy8qUcLaGGh3O/7a
qh9III/84D8yDfqTbCEWazVNkh5rsiXfDBSKYkCXruL0II8kLz2DmYi8DrR89PYO
qaYaPkUaOUsAGquw3Qs379u/s0kwhuPuHyyUFvr2Xte1ZARLRy4m7wElM6y9U9jV
nm4Ry65yo2CegjdAN+uhRYoaxy+6UAZO8sDcCbk+0GkN8+yZcaVVv7b4LCDSgJ1E
VKZZogsCLYwnCjd2Gq1KYtW3jaqxTRut9/koZJUGRQEAH14ukgzvRy5MPMgVLIvM
EN+nTVCqdDurwar73JXtT6gqjEJJ3EqdSqKvBo29/dRuiFvNRlmP8s6tqHdo83KH
RyaZO/k1mGLQydUa87as/2V4xGtUot8G6GCR2luHt58vK8x+mt54X+wh+Z/wKvpN
AYxbULIn1fsdzvNOJ2KcCy5z4ekK+J7pxwkR3+1ECVy3ru7WAQM=
=WVtr
-----END PGP SIGNATURE-----
