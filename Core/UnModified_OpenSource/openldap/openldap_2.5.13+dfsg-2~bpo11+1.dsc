-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: openldap
Binary: slapd, slapd-contrib, slapd-smbk5pwd, ldap-utils, libldap-2.5-0, libldap-common, libldap-dev, libldap2-dev, slapi-dev
Architecture: any all
Version: 2.5.13+dfsg-2~bpo11+1
Maintainer: Debian OpenLDAP Maintainers <pkg-openldap-devel@lists.alioth.debian.org>
Uploaders: Steve Langasek <vorlon@debian.org>, Torsten Landschoff <torsten@debian.org>, Ryan Tandy <ryan@nardis.ca>, Sergio Durigan Junior <sergiodj@debian.org>
Homepage: https://www.openldap.org/
Standards-Version: 4.6.0
Vcs-Browser: https://salsa.debian.org/openldap-team/openldap
Vcs-Git: https://salsa.debian.org/openldap-team/openldap.git
Testsuite: autopkgtest
Testsuite-Triggers: heimdal-kdc, samba, schema2ldif
Build-Depends: debhelper-compat (= 12), dpkg-dev (>= 1.17.14), groff-base, heimdal-multidev (>= 7.4.0.dfsg.1-1~) <!pkg.openldap.noslapd>, libargon2-dev <!pkg.openldap.noslapd>, libgnutls28-dev, libltdl-dev <!pkg.openldap.noslapd>, libperl-dev (>= 5.8.0) <!pkg.openldap.noslapd>, libsasl2-dev, libwrap0-dev <!pkg.openldap.noslapd>, nettle-dev <!pkg.openldap.noslapd>, openssl <!nocheck>, perl:any, pkg-config (>= 0.29), po-debconf, unixodbc-dev <!pkg.openldap.noslapd>
Build-Conflicts: autoconf2.13, bind-dev, libbind-dev
Package-List:
 ldap-utils deb net optional arch=any
 libldap-2.5-0 deb libs optional arch=any
 libldap-common deb libs optional arch=all
 libldap-dev deb libdevel optional arch=any
 libldap2-dev deb oldlibs optional arch=all
 slapd deb net optional arch=any profile=!pkg.openldap.noslapd
 slapd-contrib deb net optional arch=any profile=!pkg.openldap.noslapd
 slapd-smbk5pwd deb oldlibs optional arch=all profile=!pkg.openldap.noslapd
 slapi-dev deb libdevel optional arch=any profile=!pkg.openldap.noslapd
Checksums-Sha1:
 ebccbf4ba733bcf1d2bc41c3ecef4528a7de7878 3727704 openldap_2.5.13+dfsg.orig.tar.xz
 8707d9ca226cd65ead5e32914405362bb7e12a03 164564 openldap_2.5.13+dfsg-2~bpo11+1.debian.tar.xz
Checksums-Sha256:
 1d95c400a3eae6730246614ef16883de3dbd1b14b01a1ebe3a9aa1ccad2c13ec 3727704 openldap_2.5.13+dfsg.orig.tar.xz
 49e313eb058208961604a8595031bf1001174d4600d1624287f7877671df7f40 164564 openldap_2.5.13+dfsg-2~bpo11+1.debian.tar.xz
Files:
 a0e79d20e2861f906f08574c530299af 3727704 openldap_2.5.13+dfsg.orig.tar.xz
 64172d400592fc6283584ce35bf54b08 164564 openldap_2.5.13+dfsg-2~bpo11+1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQJIBAEBCgAyFiEEI3pUsQKHKL8A7zH00Ot2KGX8XjYFAmNutP0UHHNlcmdpb2Rq
QGRlYmlhbi5vcmcACgkQ0Ot2KGX8XjYzMA/9G2f8k8Pdp6tVDiQBGxy3+RDIQxz6
szPYwtTnyTjHCvSDaPnWX4GEaLfYcA8SSSjlq/ETvkyiLnsm0FZA5zbikuyanm+J
jNItfXF+8iBp2FESH7FCFqy0tOYryztUAdyaCMNx8BM9EEjAxTn3H3PQZJkR87/4
L27RcIyt3k4rPHpulMXyn+vSB4C1sdwlLtGsOS/ow0wP3NxZqdsQpa9E+biRyUxF
5eouShM+Q+RhBlC1ohhq2IeBlY8ioDmoPT4hoBommfRPRkXnIUDnVKnppyKsBwGE
EMTFQ0OpOrMQtrbHb0+lp6FCC/Ya2iMtFT4oUp4unBJ/yahn74LxW9U8Eklxc8b8
8LLZnxYdDWCzenbgDW51xLiFUhIWHyJ2jHl0n5aX/rduykYvn/t+8iySRBcZe8Od
7z8hbmIeIKOuTvajLfhXlhMAz048O/1AZIEOa26bxvBGTmGYSoedmt8zHuFVoPO4
EYsk7uF7aknUOMF4qMoIZ71fMldwZMxlJo64r21TZ9X4IL6MgNEXZReU+I4nCgZb
t5MFXahhI7zdu+/ak4hKeoQLoyO7tXhGeoW27mWwEdMKP1OEp7wk84Ev0nDqjFA/
8eUnR+jsB9So2JWEaRuRJDGrSNfzhaKS0+uUcfZuBtIZuoxS9R7lzQe3RlCqkA2q
S4/kTqpb6Zbylrw=
=se4i
-----END PGP SIGNATURE-----
