-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: twisted
Binary: python3-twisted, python3-twisted-bin, python3-twisted-bin-dbg, twisted-doc
Architecture: any all
Version: 20.3.0-7+deb11u1
Maintainer: Debian Python Team <team+python@tracker.debian.org>
Uploaders: Matthias Klose <doko@debian.org>
Homepage: https://twistedmatrix.com/
Standards-Version: 4.5.0
Vcs-Browser: https://salsa.debian.org/python-team/packages/twisted
Vcs-Git: https://salsa.debian.org/python-team/packages/twisted.git
Testsuite: autopkgtest
Testsuite-Triggers: python3-hamcrest, python3-setuptools
Build-Depends: debhelper-compat (= 13), dh-python, patch, python2-doc, python3-all-dev, python3-all-dbg, python3-hamcrest, python3-zope.interface (>= 4.0.2), python3-setuptools, python3-incremental, python3-constantly, python3-automat (>= 0.6.0), python3-hyperlink, python3-doc
Build-Depends-Indep: python3-sphinx <!nodoc>
Package-List:
 python3-twisted deb python optional arch=all
 python3-twisted-bin deb python optional arch=any
 python3-twisted-bin-dbg deb debug optional arch=any
 twisted-doc deb doc optional arch=all profile=!nodoc
Checksums-Sha1:
 915f782b902aca3ea5547ef333089961101e0871 3127793 twisted_20.3.0.orig.tar.bz2
 57921a411ec3276d3ad929af13a0f7ce8a25372b 43572 twisted_20.3.0-7+deb11u1.debian.tar.xz
Checksums-Sha256:
 d72c55b5d56e176563b91d11952d13b01af8725c623e498db5507b6614fc1e10 3127793 twisted_20.3.0.orig.tar.bz2
 60f42ddde10c7e8f01b32254579e78d254a53a8ef77b42fe76eb562a0bd6a4aa 43572 twisted_20.3.0-7+deb11u1.debian.tar.xz
Files:
 fc16d575730db7d0cddd09fc35af3eea 3127793 twisted_20.3.0.orig.tar.bz2
 ab9da6d93ae431fc8b0b4c06dbeca196 43572 twisted_20.3.0-7+deb11u1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iIoEARYKADIWIQTumtb5BSD6EfafSCRHew2wJjpU2AUCYnPchRQcc3RlZmFub3JA
ZGViaWFuLm9yZwAKCRBHew2wJjpU2OiGAQDN+BAhFYOBWvk70/+A3OKVoF7DYTSb
0feqcUDrgzHB/QEA1ewDEQJgqJZ7DWf0MiFWZw7NmfOUUMVigf8jq2NX4gY=
=ZISQ
-----END PGP SIGNATURE-----
