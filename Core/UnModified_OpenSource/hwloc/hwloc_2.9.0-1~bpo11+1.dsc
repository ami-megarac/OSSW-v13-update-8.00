-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: hwloc
Binary: hwloc, hwloc-nox, libhwloc-dev, libhwloc15, libhwloc-plugins, libhwloc-common, libhwloc-doc
Architecture: any all
Version: 2.9.0-1~bpo11+1
Maintainer: Samuel Thibault <sthibault@debian.org>
Homepage: https://www.open-mpi.org/projects/hwloc/
Standards-Version: 4.6.0
Vcs-Browser: https://salsa.debian.org/debian/hwloc
Vcs-Git: https://salsa.debian.org/debian/hwloc.git
Testsuite: autopkgtest
Testsuite-Triggers: debhelper, dh-autoreconf, libltdl-dev, libxml2-dev, libxml2-utils
Build-Depends: debhelper-compat (= 12), dh-exec, libltdl-dev [!gnu-any-any], valgrind [amd64 arm64 armhf i386 mips64el mipsel ppc64el s390x powerpc ppc64], libcairo2-dev, libx11-dev, libxml2-dev, libxml2-utils <!nocheck>, libncurses5-dev, libnuma-dev [linux-any] <!nocheck>, libxnvctrl-dev, libpciaccess-dev, libudev-dev [linux-any], pkg-config, ocl-icd-opencl-dev [!hurd-i386] | opencl-dev, opencl-headers, autoconf (>= 2.63), dpkg-dev (>= 1.16)
Build-Depends-Indep: doxygen-latex, transfig
Build-Conflicts: autoconf2.13
Package-List:
 hwloc deb admin optional arch=any
 hwloc-nox deb admin optional arch=any
 libhwloc-common deb libs optional arch=all
 libhwloc-dev deb libdevel optional arch=any
 libhwloc-doc deb doc optional arch=all
 libhwloc-plugins deb libs optional arch=any
 libhwloc15 deb libs optional arch=any
Checksums-Sha1:
 63d266428c0fd50a7546de6c3d1eac981f5b942f 3250228 hwloc_2.9.0.orig.tar.bz2
 7b72e518ead59a7d37f0f50ad53e8eaeac216b25 13372 hwloc_2.9.0-1~bpo11+1.debian.tar.xz
Checksums-Sha256:
 b3fb1a43a14d8fbcd8e976de5ede2ea7ce1d768ed173adb33c7ceaaf098dc970 3250228 hwloc_2.9.0.orig.tar.bz2
 25206acd3b6772bbd0860e8d3406891e295a05655c9bba66b46aca41f28834d9 13372 hwloc_2.9.0-1~bpo11+1.debian.tar.xz
Files:
 b550165a2793444777d2354a31e49899 3250228 hwloc_2.9.0.orig.tar.bz2
 53eb1116a30f63d10be523d33fb1c23e 13372 hwloc_2.9.0-1~bpo11+1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQJJBAEBCgAzFiEEZTSF1IMOAGwT71n/aHTOWK4tfj8FAmOhhwkVHHN0aGliYXVs
dEBkZWJpYW4ub3JnAAoJEGh0zliuLX4/EmYP/1Vbex1Aw39NzyAlSsroq5DRa9f4
Ep//Ra7UKY7eWl6e8c/Ajvr8Hw9GdYyqw2HrNvl17P5R7LAl83mpSflRUz+hlB0y
ap+ARKZV92od55HfNHtQM5lzrW5LdHXsRW367f+r7UY1RdCVHBJye4D9PgSqcY1i
9GCwPIlA4rteJBlOzuuW7ErhzU5L3gYN9FV3puHg3hzLOKAy3pXVtwHAMk/sc0wU
IW1bm6iKFLqWijXPPn8oPTlU0ZgHN1SITjHAMw0YgGzhsGkYlqO7qxBlvcl0LUbH
OxTRNyQbLIlYoldnmkuGwtxFhE8cyN+Fn0BIPtT+HEoty2VeVXzeqp5dNNSZD4rd
ZEEjABJ6ycDiaFbYShapRLrysCJWeQHnJW2d9YywlMUT8vqG4yDCT5hKQRAQAKUJ
+DE1GRfgvqtAzbISZ5ddocLrpMTZJQa04tOzlCSIaAQK/ur435P0uiWGK0wXjiyC
ogNO/Hin3GRjLGOpjI4JjOI2G55DFuCbntYXwrMJQ+VVfmCBlVrORQmE3Is5WmQT
9+EXuzQIr8UOpE/RX9ZFTFC78K2z6CkRXh0/Sf46POBZ74RsZXmBAgqZlawZivyw
IQfW99/0lVyfIwX3m6Jf8VWxglEfoQ6ciEL7JEsG8UCvyUD+VrB5tIBSxrkL+HJh
PzShh9bOd1YE6t3W
=s6E4
-----END PGP SIGNATURE-----
