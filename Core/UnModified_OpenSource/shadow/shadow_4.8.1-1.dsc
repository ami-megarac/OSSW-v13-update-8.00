-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: shadow
Binary: passwd, login, uidmap
Architecture: any
Version: 1:4.8.1-1
Maintainer: Shadow package maintainers <pkg-shadow-devel@lists.alioth.debian.org>
Uploaders: Balint Reczey <rbalint@ubuntu.com>, Serge Hallyn <serge@hallyn.com>
Homepage: https://github.com/shadow-maint/shadow
Standards-Version: 3.9.5
Vcs-Browser: https://salsa.debian.org/debian/shadow
Vcs-Git: https://salsa.debian.org/debian/shadow -b master
Build-Depends: debhelper-compat (= 12), gettext, libpam0g-dev, quilt, xsltproc, docbook-xsl, docbook-xml, libxml2-utils, libselinux1-dev [linux-any], libsemanage1-dev [linux-any], itstool, bison, libaudit-dev [linux-any]
Package-List:
 login deb admin required arch=any essential=yes
 passwd deb admin required arch=any
 uidmap deb admin optional arch=any
Checksums-Sha1:
 63457a0ba58dc4e81b2663b839dc6c89d3343f12 1611196 shadow_4.8.1.orig.tar.xz
 cc1c30e6ed8b02163e3fd0c8afc433ccc3dc4cc0 74752 shadow_4.8.1-1.debian.tar.xz
Checksums-Sha256:
 a3ad4630bdc41372f02a647278a8c3514844295d36eefe68ece6c3a641c1ae62 1611196 shadow_4.8.1.orig.tar.xz
 fdbccadc28fcca744f365e0529f3828d0c82bc3513b28976dca7308b40ea4773 74752 shadow_4.8.1-1.debian.tar.xz
Files:
 4b05eff8a427cf50e615bda324b5bc45 1611196 shadow_4.8.1.orig.tar.xz
 672c764f21617fd8de737e9bf350773c 74752 shadow_4.8.1-1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCgAdFiEEI/PvTgXX55rLQUfDg6KBkWslS0UFAl49g3QACgkQg6KBkWsl
S0WrIQ/+Ph5u1lF92NMPpSHqycaXmykauuyVK3z8SbmwujLmoYXh8QSVoALPe/P7
0Ut9sER4PhIp/WEHm3ZPs+4xFRWNjNetpaBGKC7GLT2GduZShl2QbNFMTiqtKocs
qL7pQYss5J8n+0MS8ridjRErodvEV+LME6X3hapNTA3NbQdMAhge87NG3XO40Uly
9bxcvY7Hz5A8MpVZpjFi2Zlw30gXoQXTrRsIsleOWJoGCS8bLNUpYhwKyER+Y3VS
Fh0nQPlj2GytCeAPY4CNbOpLXwM2JeZxytPQDSgvgJG/Pdl58nItuwEQ/ntJI385
xGSrp/xjLKHu2ZbCvw7mC3kKsE6+xf1rtJFHMZp4Fja5W8NjxfKYPBh5AOQGdsMb
/cGvP5nLW/1RohZ3p1lUkmDXRuQiBoK60VYp3/jt2PHccckLMMpp8gEbn/q0rNuJ
ranGQnrYL422DnZVpF0v7+bqBHM6SKqk0hdhFuWfMAqf5UaKBpiJDNY05DfZ0RM+
wSCLd+pYZw51JHIWR873Zp5r+bjkc6zDWLp3m9XUwx8oCSRo4Ha8iahsLQc1AAtU
RuTLufcM3bf3wMZ/fEGY5OHzZJDtStpRSbJgKo27C65EP39dFGs7RJAZ7PiRYjJT
CyuaI/10K05J2zV6LstG5C5aCoOTdbEOqtGtWNMvKpTpu9NKh84=
=qYKd
-----END PGP SIGNATURE-----
