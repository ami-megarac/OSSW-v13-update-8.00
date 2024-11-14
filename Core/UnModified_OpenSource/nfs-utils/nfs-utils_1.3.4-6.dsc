-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: nfs-utils
Binary: nfs-kernel-server, nfs-common
Architecture: any
Version: 1:1.3.4-6
Maintainer: Debian kernel team <debian-kernel@lists.debian.org>
Uploaders: Ben Hutchings <ben@decadent.org.uk>, Steve Langasek <vorlon@debian.org>, Salvatore Bonaccorso <carnil@debian.org>
Homepage: https://linux-nfs.org/
Standards-Version: 4.2.1
Vcs-Browser: https://salsa.debian.org/kernel-team/nfs-utils
Vcs-Git: https://salsa.debian.org/kernel-team/nfs-utils.git
Testsuite: autopkgtest
Build-Depends: debhelper (>= 9.20160709), libwrap0-dev, libevent-dev, libnfsidmap-dev (>= 0.24), libkrb5-dev, libblkid-dev, libkeyutils-dev, pkg-config, libldap2-dev, libcap-dev, libtirpc-dev (>= 1.0.2), libdevmapper-dev, dh-autoreconf, libmount-dev, libsqlite3-dev
Package-List:
 nfs-common deb net optional arch=any
 nfs-kernel-server deb net optional arch=any
Checksums-Sha1:
 54707d8e171d9c8048a20aeb1181e4582e935b82 602240 nfs-utils_1.3.4.orig.tar.xz
 394ed54e18da92b693e3a1e260d68657d8883f33 52140 nfs-utils_1.3.4-6.debian.tar.xz
Checksums-Sha256:
 b42a5bc0a8d80d04650030ceb9a11f08f4acfbcb1ee297f657fb94e339c45975 602240 nfs-utils_1.3.4.orig.tar.xz
 015bd80fbe41497d1db8b021085f23ba3f7b49a0d47d168861e387221624196b 52140 nfs-utils_1.3.4-6.debian.tar.xz
Files:
 54e4119043ec8507a2a0e054cf2889a4 602240 nfs-utils_1.3.4.orig.tar.xz
 06302be9388b60cd77536c77e4e57a33 52140 nfs-utils_1.3.4-6.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQKmBAEBCgCQFiEERkRAmAjBceBVMd3uBUy48xNDz0QFAmDZwTlfFIAAAAAALgAo
aXNzdWVyLWZwckBub3RhdGlvbnMub3BlbnBncC5maWZ0aGhvcnNlbWFuLm5ldDQ2
NDQ0MDk4MDhDMTcxRTA1NTMxRERFRTA1NENCOEYzMTM0M0NGNDQSHGNhcm5pbEBk
ZWJpYW4ub3JnAAoJEAVMuPMTQ89EJd0P/2lhq2Avpc4gyMTDBQtT0jqaA+dmc16a
ZeqviL1TR20N4Mb4V4nqYFPm/H1dH+Dpyw3x6OHTpBxyBAFGEzBZZKQyGGtMUPOU
dPJKzQIcLR1RchDC7XTYD6nefgoH7FasABdyvJ67zWVvCJSArlA+18rUqdBI0RnX
2MzTI4PKeUHmsKX3CQcfa4JpNSoub892pTaQXYU30H4rScj9doCEQkroQJXLymox
gy9nS7lsZrDGrib/e6I9/Zx2V2ZukBrkNDF7wnSK48nuLljA+YVDS147wXoph/KA
AZ8DcMYql6IEQuJqUCTFzrp1YWrefJhEcO0uLcbHDrDniGhp0dssN7qM6hFjcJpe
hD4468mRyhNOGSklREdDjyhbyT8p7TKe5YlEMnvEXIwS3uSZDMc9f7PzJc0VMrLr
E4k5ycgL8s1gGBlWbDXXr1hZVyjtexvFgdqWWJYlG1vPnpmazily8a43wBRDht0u
wHUHBRrlnlGy1PhIkHSYmUVZCKNByrH0U9xG/MNcmUDfjwfKt0to/jEwBgLIj6MU
TUvPRdlqZDr7pgFePrWYolwpG3FGKCvP+16zKDpO2C7USxOFX04/pLO79Y1iDc4F
5UHPKeHE2j0ERr6T6jCMvPL72Nphu87piuZma9cXSvv3ogqPqsh3055othQPLBSe
3ggs8fqVSzWE
=gJEf
-----END PGP SIGNATURE-----
