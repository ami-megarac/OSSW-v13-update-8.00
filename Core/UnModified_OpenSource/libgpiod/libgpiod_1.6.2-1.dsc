-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: libgpiod
Binary: libgpiod-dev, libgpiod-doc, libgpiod2, gpiod, python3-libgpiod
Architecture: linux-any all
Version: 1.6.2-1
Maintainer: SZ Lin (林上智) <szlin@debian.org>
Homepage: https://git.kernel.org/pub/scm/libs/libgpiod/libgpiod.git
Standards-Version: 4.5.1
Vcs-Browser: https://salsa.debian.org/debian/libgpiod
Vcs-Git: https://salsa.debian.org/debian/libgpiod.git
Build-Depends: debhelper-compat (= 13), autoconf-archive, automake, dh-sequence-python3 <!nopython>, help2man <!cross>, pkg-config, m4, python3-dev:any <!nopython>, libpython3-dev <!nopython>, python3-setuptools <!nopython>, linux-libc-dev (>= 4.8)
Build-Depends-Indep: doxygen
Package-List:
 gpiod deb misc optional arch=linux-any
 libgpiod-dev deb libdevel optional arch=linux-any
 libgpiod-doc deb doc optional arch=all
 libgpiod2 deb libs optional arch=linux-any
 python3-libgpiod deb python optional arch=linux-any profile=!nopython
Checksums-Sha1:
 b97d76ef7682db503896f968e3cddbe812dcad14 326072 libgpiod_1.6.2.orig.tar.xz
 8d508c26f39401a194bbf79875c4769332fbd7d5 8500 libgpiod_1.6.2-1.debian.tar.xz
Checksums-Sha256:
 c601e71846f5ab140c83bc757fdd62a4fda24a9cee39cc5e99c96ec2bf1b06a9 326072 libgpiod_1.6.2.orig.tar.xz
 6287dfdc58dee1e40210b2a91797c31488c63263f7015e6e2d2e9ad2baeb0415 8500 libgpiod_1.6.2-1.debian.tar.xz
Files:
 4765470becb619fead3cdaeac61b9a77 326072 libgpiod_1.6.2.orig.tar.xz
 3c2539d01a6bf681f59cbbc3adafe6da 8500 libgpiod_1.6.2-1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCgAdFiEEF4+DOLMUAeME/ES6qVmzipVh8/kFAl/XFy0ACgkQqVmzipVh
8/kIAQ/+IDE0qPCVNTx2s966KVqEPoqyRL6FMyRBJjEJ0u3fNnOZ9mu6F6hCDZwF
riYX0z/JvpX5BX2u5ZA/VwTi/upp+cuIzR71Ue+8hVA0E1ZJ5R+1KbklZSXn5vkz
VO0ZDjjVh70g5jQAXhSaOBgP0T+MEjxpFCn+jmyoo7BYQ7xEDMzIonbi0tGJyu8r
bAd/1H29UcZJGERb2guC/7niCgM4heXwkyD9CQ5pqdy1Q03A+2fuGt14oYbGX2VY
+xJpcv4A7zVfKhSUUhPFHiD4jiJ3I1tUaYCQojXfDpJi6/XGQDRvgnkaloG+UNlk
aNZg4p2hQhHRveQH/ItwREzoQH+MgXBQitR13VPIrJTmTL5LU//U7JLjiDfN8Rkq
VWSJVfK2kZaxQTvld/DXKzoAHD9tVt1iqhoPhMqdPOKHEizsXB9wZfx5zJMWYxpu
iKfddCMfHT1lEb7CL45sWun8TjJJ/t2jAL2WN83pQ7M/9DhY9fCPRozvwSbLpZwQ
xLCfAYkloiFpc+KBVFY/hrOrtUuDo5t6f13T7+xQ018rkE1ernUVbyi/Rq+w1RkM
5wM2e1Xb/xzoLOn/ttq594exNz8tY0pFIFhLdBX/FIEjsaX/sNvWnuMlZWplOY4G
1+5xKJj5gKNQPGuPs51xSUrJsaum9zVBbuR9ATbmCP1LB/06w3g=
=Bwc8
-----END PGP SIGNATURE-----
