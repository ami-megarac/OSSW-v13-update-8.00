-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: iptables
Binary: iptables, libxtables12, libxtables-dev, libiptc0, libiptc-dev, libip4tc2, libip4tc-dev, libip6tc2, libip6tc-dev
Architecture: linux-any
Version: 1.8.7-1
Maintainer: Debian Netfilter Packaging Team <pkg-netfilter-team@lists.alioth.debian.org>
Uploaders: Arturo Borrero Gonzalez <arturo@debian.org>, Alberto Molina Coballes <alb.molina@gmail.com>, Laurence J. Lane <ljlane@debian.org>
Homepage: https://www.netfilter.org/
Standards-Version: 4.5.0
Vcs-Browser: https://salsa.debian.org/pkg-netfilter-team/pkg-iptables
Vcs-Git: https://salsa.debian.org/pkg-netfilter-team/pkg-iptables.git
Testsuite: autopkgtest
Build-Depends: autoconf, automake, bison, debhelper-compat (= 13), flex, libmnl-dev, libnetfilter-conntrack-dev, libnetfilter-conntrack3, libnfnetlink-dev, libnftnl-dev (>= 1.1.6), libtool (>= 2.2.6)
Package-List:
 iptables deb net optional arch=linux-any
 libip4tc-dev deb libdevel optional arch=linux-any
 libip4tc2 deb libs optional arch=linux-any
 libip6tc-dev deb libdevel optional arch=linux-any
 libip6tc2 deb libs optional arch=linux-any
 libiptc-dev deb libdevel optional arch=linux-any
 libiptc0 deb oldlibs optional arch=linux-any
 libxtables-dev deb libdevel optional arch=linux-any
 libxtables12 deb libs optional arch=linux-any
Checksums-Sha1:
 05ef75415cb7cb7641f51d51e74f3ea29cc31ab1 717862 iptables_1.8.7.orig.tar.bz2
 291f8c2baf6075dea68095ddbeec4acf2d3034f2 23744 iptables_1.8.7-1.debian.tar.xz
Checksums-Sha256:
 c109c96bb04998cd44156622d36f8e04b140701ec60531a10668cfdff5e8d8f0 717862 iptables_1.8.7.orig.tar.bz2
 d885495f8d1bf7706c70668adff6891c44527cad5506ece9d1944ce245d1dd69 23744 iptables_1.8.7-1.debian.tar.xz
Files:
 602ba7e937c72fbb7b1c2b71c3b0004b 717862 iptables_1.8.7.orig.tar.bz2
 bacfd4f463240af61c4a50298e5f7560 23744 iptables_1.8.7-1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCgAdFiEE3ZhhqyPcMzOJLgepaOcTmB0VFfgFAmAEfPQACgkQaOcTmB0V
FfiWaQ/8DsGGcmmYzaJI1cfZidqVbEVn6TUJO80U6OsTFrX/pl35o8FH83tyDTyO
8MOMnIh0Q+g5zAuhUmiqseILHGtiANGuE3xA3+dxgrVEB32CWYsVBEIdhlZGadmb
R7r6PJ0X8wVMDrUzIpxhJ5jd35fRVXnsg8R6/sQ69ct35Ja7bc/qTGdlsZTR2uM2
75giuPf5FH0/V3AsfHLOiaW5IOYfcdJxuunvHulCBxPpOhG/ztk+KLpIjLKLBixv
sO/CEbGO68YhmV+AxNWsebvsKATr9RWK3ykkxQqDtjqfi8HGwyGPb7LrzCzrfwaI
HUAmNYm46jrjyy90CTDpg/TpchhBwhqhytmRy+QGmEYwjouZS5Ef5sTm/olQLYb2
SErkf7B7UgCEBVzn9Nxq6byzWcL8zZrfMwQ5EUWSxZhMLedHVrQw97za8Bq7gBV6
D8jOVrkAjk8vEn6Srhw9QScuSEVeu4oMeknzxYP6a2TSeIqJjDtaCKH4zcdo9/YL
D97rkF4j0Ie+d5GOvp5J9pVNe/rsVn2ZETpT21rb+CQXmdoXVSu1bow5/535/k8z
KUXC5rZoRFfXZsXdzki/0E47InYEhmIfLUdyLNkICzwViJWIhmAiolkfhrcbl9Af
zWo/Wd7ITcXyFLOfgcpmFKMm3w3ZIgaRYNQiPoZKbxucF78sFOc=
=G309
-----END PGP SIGNATURE-----
