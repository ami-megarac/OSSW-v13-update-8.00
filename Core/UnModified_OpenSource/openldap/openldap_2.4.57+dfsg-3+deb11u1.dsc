-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: openldap
Binary: slapd, slapd-contrib, slapd-smbk5pwd, ldap-utils, libldap-2.4-2, libldap-common, libldap2-dev, slapi-dev
Architecture: any all
Version: 2.4.57+dfsg-3+deb11u1
Maintainer: Debian OpenLDAP Maintainers <pkg-openldap-devel@lists.alioth.debian.org>
Uploaders: Steve Langasek <vorlon@debian.org>, Torsten Landschoff <torsten@debian.org>, Ryan Tandy <ryan@nardis.ca>
Homepage: https://www.openldap.org/
Standards-Version: 4.5.0
Vcs-Browser: https://salsa.debian.org/openldap-team/openldap
Vcs-Git: https://salsa.debian.org/openldap-team/openldap.git
Testsuite: autopkgtest
Build-Depends: debhelper (>= 10), dpkg-dev (>= 1.17.14), groff-base, heimdal-multidev (>= 7.4.0.dfsg.1-1~) <!pkg.openldap.noslapd>, libargon2-dev <!pkg.openldap.noslapd>, libdb5.3-dev <!pkg.openldap.noslapd>, libgnutls28-dev, libltdl-dev <!pkg.openldap.noslapd>, libperl-dev (>= 5.8.0) <!pkg.openldap.noslapd>, libsasl2-dev, libwrap0-dev <!pkg.openldap.noslapd>, nettle-dev <!pkg.openldap.noslapd>, perl:any, po-debconf, unixodbc-dev <!pkg.openldap.noslapd>
Build-Conflicts: autoconf2.13, bind-dev, libbind-dev
Package-List:
 ldap-utils deb net optional arch=any
 libldap-2.4-2 deb libs optional arch=any
 libldap-common deb libs optional arch=all
 libldap2-dev deb libdevel optional arch=any
 slapd deb net optional arch=any profile=!pkg.openldap.noslapd
 slapd-contrib deb net optional arch=any profile=!pkg.openldap.noslapd
 slapd-smbk5pwd deb oldlibs optional arch=all profile=!pkg.openldap.noslapd
 slapi-dev deb libdevel optional arch=any profile=!pkg.openldap.noslapd
Checksums-Sha1:
 c7c27b4b187e0ce627fb1750c28ecf0842d5f6af 5054318 openldap_2.4.57+dfsg.orig.tar.gz
 1f5f16831a414688c9fc987623e34e9d63221709 170324 openldap_2.4.57+dfsg-3+deb11u1.debian.tar.xz
Checksums-Sha256:
 009cc88733eaf41a21607e073a19bce53d7d6ed90a5c280e80880978c4e91db7 5054318 openldap_2.4.57+dfsg.orig.tar.gz
 2c1cbc8431c570ece2b8e6e0e13936c2f65a270e6473ecf5374ae5ba67147835 170324 openldap_2.4.57+dfsg-3+deb11u1.debian.tar.xz
Files:
 3d2f24e84664e373b095ca84aebc95ae 5054318 openldap_2.4.57+dfsg.orig.tar.gz
 7bf7558997284158f273e4ef0b3644c2 170324 openldap_2.4.57+dfsg-3+deb11u1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQKmBAEBCgCQFiEERkRAmAjBceBVMd3uBUy48xNDz0QFAmKCr59fFIAAAAAALgAo
aXNzdWVyLWZwckBub3RhdGlvbnMub3BlbnBncC5maWZ0aGhvcnNlbWFuLm5ldDQ2
NDQ0MDk4MDhDMTcxRTA1NTMxRERFRTA1NENCOEYzMTM0M0NGNDQSHGNhcm5pbEBk
ZWJpYW4ub3JnAAoJEAVMuPMTQ89EXd4P/RB0L7tgtuFWrfiEg+uD0YovrbQRz7n4
PFIEf0r5nkUSHnPARmHPNgO4g3vATFyELXVtWi7DuCMG8nYwMDHIz6bUbdk1T2Na
uCq/TPTiayc5m9rs3tggSowfQMnNiKp192ViWHDD2jhjb0x8/YOXPjqF63kDkSar
vy9ely2g6YMZz14+YEyEsuFoegAMPy1YeaQxGj3x5lQBynLgBxSrmCkv4gniCOmJ
tf4/2wCQz4LNejvRyldJR9eOzh8iC0TJBVy1cGS7hb5AQXACMM6yfyv5XC6RhU3N
NcUougwduQwZhO/pREZ4KBbNFB5JVXrFHdQJucZjOS+r7B0US3a7i0UjZFU17+Nz
0ADQuCSIQmFgg+fo6xZigjVMq5e5IKgZR/EvcyRj+4bU80ub+ZiO3oRG3Q5PhZFM
vnaHCWgscssITemNT/Dp4L1MDbYcjGe9jKpkqL+JgM160DIxMVu9RdhODqqhWe7Q
MhrIZK8goOBeukfaDfaqZqqYlZUCgPCwLFv4qOcDp7pvABGcPidPdhB6YEuR/48X
N5AjOz2u29dBSqPDj7sshgrh2BXDm+I71rZJwqRCPlrVaNY5gcNZrxp/UsMsVJTt
gGIZLag9dSzHB/P4usy/JTfYDTv/zBOYTVrHD8ufTrJ0tWLwtniUAeJ70qZ+Cqbj
kusphTOH3nOB
=qbEI
-----END PGP SIGNATURE-----
