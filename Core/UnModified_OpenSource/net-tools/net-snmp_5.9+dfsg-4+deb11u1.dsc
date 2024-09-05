-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: net-snmp
Binary: snmpd, snmptrapd, snmp, libsnmp-base, libsnmp40, libnetsnmptrapd40, libsnmp-dev, libsnmp-perl, tkmib
Architecture: any all
Version: 5.9+dfsg-4+deb11u1
Maintainer: Net-SNMP Packaging Team <pkg-net-snmp-devel@lists.alioth.debian.org>
Uploaders: Craig Small <csmall@debian.org>, Thomas Anders <tanders@users.sourceforge.net>, Noah Meyerhans <noahm@debian.org>
Homepage: http://net-snmp.sourceforge.net/
Standards-Version: 4.4.1
Vcs-Browser: https://salsa.debian.org/debian/net-snmp
Vcs-Git: https://salsa.debian.org/debian/net-snmp.git
Testsuite: autopkgtest
Build-Depends: debhelper-compat (= 12), libtool, libwrap0-dev, libssl-dev, perl:any (>= 5.8), perl-xs-dev, autoconf, automake, debianutils (>= 1.13.1), bash (>= 2.05), findutils (>= 4.1.20), procps, pkg-config, libbsd-dev [kfreebsd-i386 kfreebsd-amd64], libkvm-dev [kfreebsd-i386 kfreebsd-amd64], libsensors-dev [!hurd-i386 !kfreebsd-i386 !kfreebsd-amd64], default-libmysqlclient-dev, libpci-dev
Build-Conflicts: libsnmp-dev
Package-List:
 libnetsnmptrapd40 deb libs optional arch=any
 libsnmp-base deb libs optional arch=all
 libsnmp-dev deb libdevel optional arch=any
 libsnmp-perl deb perl optional arch=any
 libsnmp40 deb libs optional arch=any
 snmp deb net optional arch=any
 snmpd deb net optional arch=any
 snmptrapd deb net optional arch=any
 tkmib deb net optional arch=all
Checksums-Sha1:
 6f4e33a72ff64b78e2593585cb7c73e649066a9f 3513780 net-snmp_5.9+dfsg.orig.tar.xz
 be115560971f50c8e0a9aa741812eba92c46cda8 69952 net-snmp_5.9+dfsg-4+deb11u1.debian.tar.xz
Checksums-Sha256:
 a153d0e867ded287285335e5055e67e0177959315f02ae6995401ddf9b10c8e8 3513780 net-snmp_5.9+dfsg.orig.tar.xz
 3f2e8d4ac4c6ae7550c101923c169d6bddc9e1e01053ed6bc7ad86b807426dff 69952 net-snmp_5.9+dfsg-4+deb11u1.debian.tar.xz
Files:
 6c2d346ce3320e8999500497e9bacc99 3513780 net-snmp_5.9+dfsg.orig.tar.xz
 a3e626b1ed5adc26430e1727d81641df 69952 net-snmp_5.9+dfsg-4+deb11u1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCgAdFiEEXT3w9TizJ8CqeneiAiFmwP88hOMFAmL6MVkACgkQAiFmwP88
hOOVbQ/+L2mgsXvkw/FmXeECnDuegxSuxnS1Mvygpp7pAIgjZ2uzYs22bWuDDnpk
PGvMJ4p1ZbYqgTgWUuyhE3oDTyIlaRkI8PzhS1alBO7v1rxw4qZpLGlZjgjjvPs0
KRxj6/uOZTPXslinz/tfCSO1UkEWsTEFxUyFI9FFcT/2rP5YULiQXEWsR9WrtAZe
BwaXDEuoi5jyW/MsyokQsMHgdDHQHmgE1w/kNlE/am39Whuy11fCXxel83hjNwpn
I56JdhJbZ4KrIqbuu13GTLHeyOmbd3IS7QgK2mKsMhvidMciS6DiRaakZXMbv255
3WW58iOmRSds3xHWv7YoEIf/605S9IiwhJ8Zq6maGwm4J8jqKuo1GP0IXWv+IxU/
Xl4icIgaunTJ7H58Y6F47pCezTSKF03n5Lob1NVjYAtEr2EaqHvFGm4UzvOWYdf3
QZY+BqhpEgPYwA8Q0YBBlG7S1Jy5ml1f/aUJmjqQbvbVQLnmKjxhdh5KtP8D+H7k
xlh4IFGqDPZcssMloAVlql+tpzIbiPa7/mp0l2eNE2Hz14qvmX5C+RmJmn+ORBOQ
7ep0syZaVsHoAdAa0OwbqrnGSJu9XltyMvbJ72s1wZioAOj+ZOriQem52DkS4pnY
EI6ybXs89L3Wfl73xwyp2puZjDZ+PUEXxp5rK+cg1MaBDZr8RcY=
=XqYm
-----END PGP SIGNATURE-----
