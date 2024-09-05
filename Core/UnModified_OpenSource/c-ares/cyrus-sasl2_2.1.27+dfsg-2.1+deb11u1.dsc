-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: cyrus-sasl2
Binary: sasl2-bin, cyrus-sasl2-doc, libsasl2-2, libsasl2-modules, libsasl2-modules-db, libsasl2-modules-ldap, libsasl2-modules-otp, libsasl2-modules-sql, libsasl2-modules-gssapi-mit, libsasl2-dev, libsasl2-modules-gssapi-heimdal
Architecture: any all
Version: 2.1.27+dfsg-2.1+deb11u1
Maintainer: Debian Cyrus Team <team+cyrus@tracker.debian.org>
Uploaders: Fabian Fagerholm <fabbe@debian.org>, Roberto C. Sanchez <roberto@debian.org>, Ondřej Surý <ondrej@debian.org>, Adam Conrad <adconrad@0c3.net>
Homepage: https://www.cyrusimap.org/sasl/
Standards-Version: 4.3.0.1
Vcs-Browser: https://salsa.debian.org/debian/cyrus-sasl2
Vcs-Git: https://salsa.debian.org/debian/cyrus-sasl2.git
Build-Depends: chrpath, debhelper-compat (= 12), default-libmysqlclient-dev <!pkg.cyrus-sasl2.nosql> | libmysqlclient-dev <!pkg.cyrus-sasl2.nosql>, docbook-to-man, groff-base, heimdal-multidev <!pkg.cyrus-sasl2.nogssapi>, krb5-multidev <!pkg.cyrus-sasl2.nogssapi>, libdb-dev, libkrb5-dev <!pkg.cyrus-sasl2.nogssapi>, libldap2-dev <!pkg.cyrus-sasl2.noldap>, libpam0g-dev, libpod-pom-view-restructured-perl, libpq-dev <!pkg.cyrus-sasl2.nosql>, libsqlite3-dev, libssl-dev, po-debconf, python3-sphinx, quilt
Build-Conflicts: heimdal-dev
Package-List:
 cyrus-sasl2-doc deb doc optional arch=all
 libsasl2-2 deb libs standard arch=any
 libsasl2-dev deb libdevel optional arch=any
 libsasl2-modules deb libs optional arch=any
 libsasl2-modules-db deb libs standard arch=any
 libsasl2-modules-gssapi-heimdal deb libs optional arch=any profile=!pkg.cyrus-sasl2.nogssapi
 libsasl2-modules-gssapi-mit deb libs optional arch=any profile=!pkg.cyrus-sasl2.nogssapi
 libsasl2-modules-ldap deb libs optional arch=any profile=!pkg.cyrus-sasl2.noldap
 libsasl2-modules-otp deb libs optional arch=any
 libsasl2-modules-sql deb libs optional arch=any profile=!pkg.cyrus-sasl2.nosql
 sasl2-bin deb utils optional arch=any
Checksums-Sha1:
 6da3baff1685e96b93b46cdd47e13ecc34a632df 2058596 cyrus-sasl2_2.1.27+dfsg.orig.tar.xz
 83f257eaa104f1ed74b51dc7cd6dcddb8293cb7c 102488 cyrus-sasl2_2.1.27+dfsg-2.1+deb11u1.debian.tar.xz
Checksums-Sha256:
 108b0c691c423837264f05abb559ea76c3dfdd91246555e8abe87c129a6e37cd 2058596 cyrus-sasl2_2.1.27+dfsg.orig.tar.xz
 70b7a55776febf987363f5bb58322d6f03186215374a2eb0b6203980924f8680 102488 cyrus-sasl2_2.1.27+dfsg-2.1+deb11u1.debian.tar.xz
Files:
 ce30955361d1cdde3c31d0ee742e338d 2058596 cyrus-sasl2_2.1.27+dfsg.orig.tar.xz
 d5938fe70f9d42dc1fd696bdbd849ba4 102488 cyrus-sasl2_2.1.27+dfsg-2.1+deb11u1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQKmBAEBCgCQFiEERkRAmAjBceBVMd3uBUy48xNDz0QFAmIYlE9fFIAAAAAALgAo
aXNzdWVyLWZwckBub3RhdGlvbnMub3BlbnBncC5maWZ0aGhvcnNlbWFuLm5ldDQ2
NDQ0MDk4MDhDMTcxRTA1NTMxRERFRTA1NENCOEYzMTM0M0NGNDQSHGNhcm5pbEBk
ZWJpYW4ub3JnAAoJEAVMuPMTQ89E4GkP/j7vusIi4AttCHOpnlVhT4yp3J1yCG6Y
DRHVvDaR84Hnz17JCPKen+iGCFlnTIw5UgaLfsi4stKzWpZiHk210cchTnvQ0x0K
3r48OSd6i84nh9KMfGX8DBXx7Ca9lARRnpCqzyNbPvUM6TFLTfv4Dv9bXkb92VcP
BDHPhYt09Ji0/vkOodLWreUOnHKPfZpluGE3uqFoCfM5U0+GjAyXNVxbIV/owX4C
GWqMRbv1b9+d8C7zDNej/BT++0R4qmVgw3Lf1VqMilB1TkJRxKCdIZ29Swuq+ehN
KM1A8YquJwNomVo+uum8WniQdYKalV99cDf+hbIPutHxQU5TqHFWMMMGLjL+u0jy
uWje/MDIuFhnZf6bboPuP6PVSt7RDGuUB9C1wfeHWAtffC9S2k4eWs7hxNcQ+LGF
GUfoyNB61n6DFX0L6bzaD6sB9k7z+JRTHY6nPiCaGSIS//LiP/KABbEh32WNLryE
YvG9kv2woPEFK/GlPC+n3yH4sqehifmCgXFo0QT8HrvQly303pyacd7vu6I8ovcr
H+bCBHwJGAMyHDCCzd7+0bbPPeSUuKwjRviEYdX94Fu8LYQTzPPAlHAVFB6h8EGz
bEV8L87Krn/lrPXHZA8a+Oqw7qPOWlHtrPO1PY7C0PJNqViDEsIJq8d04DmypI8q
vF56YcjQqrSJ
=52yU
-----END PGP SIGNATURE-----
