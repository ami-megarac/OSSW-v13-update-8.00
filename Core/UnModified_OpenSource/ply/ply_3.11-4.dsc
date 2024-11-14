-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: ply
Binary: python-ply, python3-ply, python-ply-doc
Architecture: all
Version: 3.11-4
Maintainer: JCF Ploemen (jcfp) <linux@jcf.pm>
Uploaders: Debian Python Modules Team <python-modules-team@lists.alioth.debian.org>
Homepage: https://www.dabeaz.com/ply/
Standards-Version: 4.5.0
Vcs-Browser: https://salsa.debian.org/python-team/modules/ply
Vcs-Git: https://salsa.debian.org/python-team/modules/ply.git
Testsuite: autopkgtest
Testsuite-Triggers: python-six, python2, python3-all, python3-six
Build-Depends: debhelper-compat (= 12), dh-python, python2.7, python-six, python3-all, python3-six, perl
Build-Depends-Indep: python-setuptools, python3-setuptools
Package-List:
 python-ply deb python optional arch=all
 python-ply-doc deb doc optional arch=all
 python3-ply deb python optional arch=all
Checksums-Sha1:
 10a555a32095991fbc7f7ed10c677a14e21fad1d 159130 ply_3.11.orig.tar.gz
 4da4eb4bbaf472929108ce48d0eda633fa0702d1 8136 ply_3.11-4.debian.tar.xz
Checksums-Sha256:
 00c7c1aaa88358b9c765b6d3000c6eec0ba42abca5351b095321aef446081da3 159130 ply_3.11.orig.tar.gz
 3ba67d28e70b429794cdb8f98153f043a3cb16a9c2fc8ec6d291a9d5c9af4c11 8136 ply_3.11-4.debian.tar.xz
Files:
 6465f602e656455affcd7c5734c638f8 159130 ply_3.11.orig.tar.gz
 dd161a8d3f63d1b914568d609038f9f5 8136 ply_3.11-4.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iIoEARYKADIWIQTumtb5BSD6EfafSCRHew2wJjpU2AUCXyoJBhQcc3RlZmFub3JA
ZGViaWFuLm9yZwAKCRBHew2wJjpU2DPyAQCP1OZOXrBwmLruBc7qu30IVAkRVh84
XN7sDZYTs+yzqgEAhvbX9aiwubbv1n657Dx8G5et+BlxJzHSCFOUhvdyiQQ=
=WV+n
-----END PGP SIGNATURE-----
