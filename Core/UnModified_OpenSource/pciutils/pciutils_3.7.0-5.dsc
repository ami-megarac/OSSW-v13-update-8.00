-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: pciutils
Binary: pciutils, libpci-dev, libpci3, pciutils-udeb, libpci3-udeb
Architecture: any
Version: 1:3.7.0-5
Maintainer: Guillem Jover <guillem@debian.org>
Homepage: https://mj.ucw.cz/sw/pciutils/
Standards-Version: 4.5.1
Vcs-Browser: https://git.hadrons.org/cgit/debian/pkgs/pciutils.git
Vcs-Git: https://git.hadrons.org/git/debian/pkgs/pciutils.git
Testsuite: autopkgtest
Testsuite-Triggers: build-essential, pkg-config
Build-Depends: debhelper-compat (= 13), pci.ids (>= 0.0~2019.11.10-2), pkg-config, libkmod-dev [linux-any], libudev-dev (>= 196) [linux-any], zlib1g-dev
Package-List:
 libpci-dev deb libdevel optional arch=any
 libpci3 deb libs optional arch=any
 libpci3-udeb udeb debian-installer optional arch=any
 pciutils deb admin standard arch=any
 pciutils-udeb udeb debian-installer optional arch=any
Checksums-Sha1:
 faf65ed0452c85414206f04d2ed40a7b504fb911 516418 pciutils_3.7.0.orig.tar.gz
 e351221add098847b067d347b703134548d4d66f 833 pciutils_3.7.0.orig.tar.gz.asc
 875346ee61c2625d7d5ef6aade9ddb2e2cbc495c 21428 pciutils_3.7.0-5.debian.tar.xz
Checksums-Sha256:
 08c27e01030d1fcc700d02bc2ea66c638f58a3d150e45e58852aa82ad4160d84 516418 pciutils_3.7.0.orig.tar.gz
 f1c3ef27541bba787c1a12d96580cf4df0c169783b12eea5ca36717970eeba6b 833 pciutils_3.7.0.orig.tar.gz.asc
 910b5955d54e42ee4073534f60716c4f5ca2ac4470b99b2be02a98ccfb18239e 21428 pciutils_3.7.0-5.debian.tar.xz
Files:
 51554c538b5a57b61123326e14ea28a1 516418 pciutils_3.7.0.orig.tar.gz
 7f714d692ae5f197b6bd2cdaec1a909b 833 pciutils_3.7.0.orig.tar.gz.asc
 75dc1fc7a2e08541578ad3e6372f3ccb 21428 pciutils_3.7.0-5.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCgAdFiEETz509DYFDBD1aWV0uXK/PqSuV6MFAl/k1QcACgkQuXK/PqSu
V6OUIg//SLkotXOc++Cpan2rOn0fppRx5zT4DDrdeOruHIWlta1hN0Q8bFWYMIie
8WVTcFp2/WQzJJL7fvFvoW7R56PQ02pekp9H4RnThR6B7NxH3MIJZYvYiJow4MRz
0XBRG32FHW0yjtba4DU9kG/yGU/qIqm3xjHjyrw1VyW24HczTrUR1MsBVqz92cFZ
WuA1casidascOHYoFy4k1tA5RIInWBu7ah7AEeCn36pjoaaZOQuXlFQiDqNNrX1H
d9YCrHwZ6ImSxGkZRGO6xtoj9LADL2EfQ3xUvN34s4/lVAXeifa0AO6k9QO5V1DO
zV4gQk7TZRTKyy/V0izuqxNhujmlX4jdmbl8QaiVsxzoGLhix6noyyBkDg2Jf/Ag
EkmDRttfmywmy9wiH5JvqhVQe5gvU5FcywvVKtNAdPyyYCIXlQBxWEKOT6WWkOiz
Yd446qzSTUVLYclVZOxYdlkCJVAol/D0wWkmgk5uTVDHaC0cwkt8iudAAImZwraZ
VWohTH3WnIQBLOXvipRiJTzHycSuqrP3G0JraGhrLpvg3IHxohiU38myBrwdM+Dw
8vLtdrpIHUPz9yuIBUwMmaPLvtT8I+pjG/0EVmPsQaX3Qkz+Sp12cQhPSUQE/Lgz
PDD61f9VS4q+EGoslvyglN6ueUFVmKFg30VxSalm0fIwueSUfLY=
=/4M/
-----END PGP SIGNATURE-----
