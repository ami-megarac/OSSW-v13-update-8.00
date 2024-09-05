-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: openssl
Binary: openssl, libssl1.1, libcrypto1.1-udeb, libssl1.1-udeb, libssl-dev, libssl-doc
Architecture: any all
Version: 1.1.1w-0+deb11u1
Maintainer: Debian OpenSSL Team <pkg-openssl-devel@lists.alioth.debian.org>
Uploaders: Christoph Martin <christoph.martin@uni-mainz.de>, Kurt Roeckx <kurt@roeckx.be>, Sebastian Andrzej Siewior <sebastian@breakpoint.cc>
Homepage: https://www.openssl.org/
Standards-Version: 4.5.0
Vcs-Browser: https://salsa.debian.org/debian/openssl
Vcs-Git: https://salsa.debian.org/debian/openssl.git
Testsuite: autopkgtest
Testsuite-Triggers: perl
Build-Depends: debhelper-compat (= 12), m4, bc, dpkg-dev (>= 1.15.7)
Package-List:
 libcrypto1.1-udeb udeb debian-installer optional arch=any
 libssl-dev deb libdevel optional arch=any
 libssl-doc deb doc optional arch=all
 libssl1.1 deb libs optional arch=any
 libssl1.1-udeb udeb debian-installer optional arch=any
 openssl deb utils optional arch=any
Checksums-Sha1:
 76fbf3ca4370e12894a408ef75718f32cdab9671 9893384 openssl_1.1.1w.orig.tar.gz
 bca277263dfe1d951360eefda6c5eb01edc36462 833 openssl_1.1.1w.orig.tar.gz.asc
 8620bc969d88ef1e7fe5abd41dfaf2dc4ce707b7 84088 openssl_1.1.1w-0+deb11u1.debian.tar.xz
Checksums-Sha256:
 cf3098950cb4d853ad95c0841f1f9c6d3dc102dccfcacd521d93925208b76ac8 9893384 openssl_1.1.1w.orig.tar.gz
 5e5c3694cda755231c0438ac3c96af00d7510abd7b916d79c51bb979567040db 833 openssl_1.1.1w.orig.tar.gz.asc
 022636c62f183b6157b1a1c65a1365690b6e3db5b0db38a5258ec8f097a05998 84088 openssl_1.1.1w-0+deb11u1.debian.tar.xz
Files:
 3f76825f195e52d4b10c70040681a275 9893384 openssl_1.1.1w.orig.tar.gz
 89d454a3aab2163ba2dda93510ea8089 833 openssl_1.1.1w.orig.tar.gz.asc
 89522257a05d623342fccf7b463db6b2 84088 openssl_1.1.1w-0+deb11u1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCgAdFiEEZCVGlf/wqkRmzBnme5boFiqM9dEFAmUazpMACgkQe5boFiqM
9dFSag/+NduqhU/dYHnCSJFKUuZ5ZIRUrU2s9SRYJBJ2mcCzwuGYaWRBOvWrRqFA
5L4JGP85dvf3TpbHYhFgnvKgs+mZTCOtCmSsyrkv6ZHSyAms1lES3i6wBjKTGj+v
ZV6zfzAoA6nQigKxktHDm53iIZVRCNLlC1tLXyWxpmr7PsgXrBoIXklWl61F+81X
ZjA2pky71N3v2Opus89a3jJl4x17ziTkZ9972sLGrhgX+hB8jbCnHcQkJKFD7i57
6cpIgytt4/DZyyTNsdFkrzMdGW2B32enHlEHJ4hWLK6Q9CtTggkGNzdh/wNexZae
0J8kpfuF52SThBbTvR5mzeftymygUoM2dY2MEZ159Hu2+05ZD0hKIs1IHpXQgcvw
FRaWokrDwQgKzZqrBXS72t/HvDKeoUDL/QdBeLAgiOUO+h/OcmFHsOQUThuvxsv3
CnpPFKZxkhPJOAIQe4wsy//SbFeop4ZmLd/vOGNMVL0wmKQcjdGbsD0jaX+4QeOp
nLj1UJjMKs9APKprlNVDOqpV+Ee17ZwYLQd5t0MbClpmGhg9xUoxBQQSApbvlWvB
tdxIt7QSXpi9JopyN4fJxOu942TJ5/7grWe+lmuOFf2dC3M4KlPsbbNVwwhEX3Ix
hjl16cemhjHFRWRKyNMcON9+bu0RBDHbnUMsGgWP8915+HJZ9Rw=
=18Cc
-----END PGP SIGNATURE-----
