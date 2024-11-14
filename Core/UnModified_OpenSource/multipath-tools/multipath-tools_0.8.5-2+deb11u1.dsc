-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: multipath-tools
Binary: multipath-tools, kpartx, multipath-tools-boot, multipath-udeb, kpartx-udeb
Architecture: linux-any all
Version: 0.8.5-2+deb11u1
Maintainer: Debian DM Multipath Team <team+linux-blocks@tracker.debian.org>
Uploaders: Guido Günther <agx@sigxcpu.org>, Ritesh Raj Sarraf <rrs@debian.org>, Chris Hofstaedtler <zeha@debian.org>
Homepage: http://christophe.varoqui.free.fr/
Standards-Version: 3.9.8
Vcs-Browser: https://salsa.debian.org/linux-blocks-team/multipath-tools
Vcs-Git: https://salsa.debian.org/linux-blocks-team/multipath-tools.git
Testsuite: autopkgtest
Testsuite-Triggers: fio, gdisk, lsscsi, open-iscsi, qemu-utils, tgt
Build-Depends: debhelper-compat (= 9), po-debconf, libdevmapper-dev, libreadline-dev, libaio-dev, libudev-dev, libsystemd-dev, systemd, liburcu-dev, pkg-config, libjson-c-dev
Package-List:
 kpartx deb admin optional arch=linux-any
 kpartx-udeb udeb debian-installer optional arch=linux-any
 multipath-tools deb admin optional arch=linux-any
 multipath-tools-boot deb admin optional arch=all
 multipath-udeb udeb debian-installer optional arch=linux-any
Checksums-Sha1:
 58886428511e57390cc7b2d71aac3920e2dd0461 494654 multipath-tools_0.8.5.orig.tar.gz
 64ec5631d4147090a50b06fe1c0ae5b812679f5d 43920 multipath-tools_0.8.5-2+deb11u1.debian.tar.xz
Checksums-Sha256:
 0549ebe39fd3950ec49c16304da408c2bfe1b18d386d8e6647b5bcb60363c8aa 494654 multipath-tools_0.8.5.orig.tar.gz
 0a6895e1e24e86f9a0e4865c3d70589a16ac654e68fa65d0c86447a47e2293e1 43920 multipath-tools_0.8.5-2+deb11u1.debian.tar.xz
Files:
 353e8ff3211d368c17c9b465a468c7ed 494654 multipath-tools_0.8.5.orig.tar.gz
 6cea518f56a77fdab875fe102061f3a8 43920 multipath-tools_0.8.5-2+deb11u1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCAAdFiEE/d0M/zhkJ3YwohhskWT6HRe9XTYFAmP+bC4ACgkQkWT6HRe9
XTZO3hAAhMcRav29BJVYdwQFgvI0AxDIJqYsyvhN2kRDBNX2ICK/7kdS7AcU5ejy
ZJZXy1JkyCeKLSIRIC5cKWIlHSyXEpVzm5d+QrduRf1HNNb66qSe9L3gqYVw6ydh
JJQGt75eQHOyold4BRn00/hWl6xfrARSm6hlKVzEm1NWhDJyvlD9/poZfg6M0Hi/
KhSS1vsJqru37qXl/M0PxgM8iJHblDHsDsqzF/qGJExBvxdQVnTiPmHvTSZDZVCK
9vq9DVnm97iI2tAR1WWqnZb+SHtUpLYYUqs91sGsEXr8030V3HlE5se+bQfVkw6N
1zyfHOMzhL/wJPK+TkEOUkhZnKuqDY0ynP0yyLe+hVwKPgMQIQ83NpyCS7NV0r2S
s1qwZPJBkFLtpZFsewO7sFy3n5LEJHPoQg1625S+TqSiahzmmmThtFVVEiw8eZ9z
IBmJJbXEYtWAHuG4wzixgVtV7eEe3b5m5cXaSqIrtd+jgZlDgVW/3X885BVzjymB
4bno9QWn1GOQqhZ+oFZnvyc+vtcUof98bHCPWXrAonk5Yt68Jk4RA8Sq+Pb1KiJs
YAbXmk9gLV1Y8KRfSqo35Ra2/QMYPhKPLRqOvZmAT7wSJdTdPytMNR4BcByn9Rp4
VNdOwbo11ZiLfQJ2x6/2J0vVeVPNad+kKIfRdPqPzavD1XOcRTI=
=SebD
-----END PGP SIGNATURE-----
