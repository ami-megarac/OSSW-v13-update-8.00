-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: pmix
Binary: libpmix-dev, libpmix2, libpmix-bin, python3-pmix
Architecture: any
Version: 4.0.0-4.1+deb11u1
Maintainer: Alastair McKinstry <mckinstry@debian.org>
Homepage: https://github.com/pmix/pmix
Standards-Version: 4.5.1
Vcs-Browser: https://salsa.debian.org:/science-team/pmix.git
Vcs-Git: https://salsa.debian.org:/science-team/pmix.git -b debian/latest
Build-Depends: debhelper-compat (= 13), dh-sequence-python3, flex, python3-all-dev, cython3, python3-distutils, pandoc, libpsm-infinipath1-dev [amd64 i386], libpsm2-dev [amd64], libhwloc-dev, zlib1g-dev, libevent-dev, libjansson-dev, libltdl-dev, libmunge-dev [!hurd-i386]
Package-List:
 libpmix-bin deb net optional arch=any
 libpmix-dev deb libdevel optional arch=any
 libpmix2 deb libs optional arch=any
 python3-pmix deb net optional arch=any
Checksums-Sha1:
 2f0078824ea5ff508f14dfdf38f05e30385fffa9 806668 pmix_4.0.0.orig.tar.xz
 28d47e4a41952d2b6e62a645b412162a7a1e1c46 10312 pmix_4.0.0-4.1+deb11u1.debian.tar.xz
Checksums-Sha256:
 b11eb90a88cd387515b96b8ab8e2f035eeef24f51fa6f15a9ca5cc89778eb1f7 806668 pmix_4.0.0.orig.tar.xz
 af67a0e52dc1f478a12def44e7e3a5cfc143ae4fcae26ea9f973d64a06cfd6aa 10312 pmix_4.0.0-4.1+deb11u1.debian.tar.xz
Files:
 efb717846526fc6cdd1e8f338ac4bebb 806668 pmix_4.0.0.orig.tar.xz
 df5580dc91f6cdfccf9190a2b1b549f7 10312 pmix_4.0.0-4.1+deb11u1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQKmBAEBCgCQFiEERkRAmAjBceBVMd3uBUy48xNDz0QFAmU9XKdfFIAAAAAALgAo
aXNzdWVyLWZwckBub3RhdGlvbnMub3BlbnBncC5maWZ0aGhvcnNlbWFuLm5ldDQ2
NDQ0MDk4MDhDMTcxRTA1NTMxRERFRTA1NENCOEYzMTM0M0NGNDQSHGNhcm5pbEBk
ZWJpYW4ub3JnAAoJEAVMuPMTQ89E47UP/R9W9Z5eTBBtrDFB8rBPPuZMNRhhencA
jUf1yoF/8aFwo57GqlwR+yYmk/AErbeEatdpCoZRN7B/fZ5mGBHCO7dw9AtH7krv
9/AwzRIaxIijDBugmWsRSb5lMCll9J3fiAUdHVkxekmjEkZk8ArM20XdIOh5jkcJ
KOtXbqKat6QfOiz4E/1dyG5y1O8NoCoX/mCWZPdiJQNK8M0khutG/cMaby1npHDP
e6Bgg1EMpS+A52Onao6XoUJXwJTFMShEjTB9MKpONx8SVtTv8O5jga/Asj1zu1bj
3csy84HK3V/8kxboeK38dRAzgaLZTAqwLb3YO6zS74ciRWqsUJaScsalmDpWByJ5
Fblif4j4K4v3LD5nIBOAy089/HHd64P/t5tpVrYuUJ+FZ7eT9YXaWasIXTdX1HXf
wnoYSjwye/10zQ5YTSRFGBMICqs8Iz/Hh1fCcXjhtN+4ZhUdtj8Ej68EhTtjWmGA
WuK4UP5xr8286Pv+otERAlqY4MM2dZ1KKopA/AZmwiJ0ZZgwrOKg8wZnBL4JdYng
J39Tvbn+iII7SBFEl6QsJJjAjtO30fNRFJ1rlcsN2T+6nrpkD6GJYonQfIRejvcl
gPs8THlyWFHwaKqiegV+5uAvqfk+8IjF3VlmKzsrLvnCYyO5OZk1g0NqFn5mjOlv
R2006eDNRein
=kjgF
-----END PGP SIGNATURE-----
