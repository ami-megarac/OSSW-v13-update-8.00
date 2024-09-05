-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: samba
Binary: samba, samba-libs, samba-common, samba-common-bin, smbclient, samba-testsuite, registry-tools, samba-dev, python3-samba, samba-dsdb-modules, samba-vfs-modules, libsmbclient, libsmbclient-dev, winbind, libpam-winbind, libnss-winbind, libwbclient0, libwbclient-dev, ctdb
Architecture: any all
Version: 2:4.13.13+dfsg-1~deb11u6
Maintainer: Debian Samba Maintainers <pkg-samba-maint@lists.alioth.debian.org>
Uploaders: Steve Langasek <vorlon@debian.org>, Jelmer Vernooĳ <jelmer@debian.org>, Mathieu Parent <sathieu@debian.org>, Andrew Bartlett <abartlet+debian@catalyst.net.nz>
Homepage: http://www.samba.org
Standards-Version: 4.5.0
Vcs-Browser: https://salsa.debian.org/samba-team/samba
Vcs-Git: https://salsa.debian.org/samba-team/samba.git
Testsuite: autopkgtest
Testsuite-Triggers: cifs-utils, coreutils, passwd, systemd
Build-Depends: bison, debhelper-compat (= 13), dh-exec, dh-python, docbook-xml, docbook-xsl, flex, libacl1-dev, libarchive-dev, libavahi-client-dev, libavahi-common-dev, libblkid-dev, libbsd-dev, libcap-dev [linux-any], libcephfs-dev [amd64 arm64 armel armhf i386 mips64el mipsel ppc64el ppc64 s390x x32], libcmocka-dev (>= 1.1.3), libcups2-dev, libdbus-1-dev, libglusterfs-dev [linux-any], libgnutls28-dev, libgpgme11-dev, libicu-dev, libjansson-dev, libldap2-dev, libldb-dev (>= 2:2.2.3-2~deb11u2~), libncurses5-dev, libpam0g-dev, libparse-yapp-perl, libpcap-dev [hurd-i386 kfreebsd-any], libpopt-dev, librados-dev [amd64 arm64 armel armhf i386 mips64el mipsel ppc64el ppc64 s390x x32], libreadline-dev, libsystemd-dev [linux-any], libtalloc-dev (>= 2.3.1~), libtasn1-6-dev (>= 3.8), libtasn1-bin, libtdb-dev (>= 1.4.3~), libtevent-dev (>= 0.10.2~), liburing-dev [linux-any], perl, pkg-config, po-debconf, python3-dev, python3-dnspython, python3-etcd, python3-ldb (>= 2:2.2.3~), python3-ldb-dev (>= 2:2.2.3~), python3-talloc-dev (>= 2.3.1~), python3-tdb (>= 1.4.3~), python3-testtools, python3, xfslibs-dev [linux-any], xsltproc, zlib1g-dev (>= 1:1.2.3)
Build-Conflicts: libtracker-miner-2.0-dev, libtracker-sparql-2.0-dev
Package-List:
 ctdb deb net optional arch=any
 libnss-winbind deb admin optional arch=any
 libpam-winbind deb admin optional arch=any
 libsmbclient deb libs optional arch=any
 libsmbclient-dev deb libdevel optional arch=any
 libwbclient-dev deb libdevel optional arch=any
 libwbclient0 deb libs optional arch=any
 python3-samba deb python optional arch=any
 registry-tools deb net optional arch=any
 samba deb net optional arch=any
 samba-common deb net optional arch=all
 samba-common-bin deb net optional arch=any
 samba-dev deb devel optional arch=any
 samba-dsdb-modules deb libs optional arch=any
 samba-libs deb libs optional arch=any
 samba-testsuite deb net optional arch=any
 samba-vfs-modules deb net optional arch=any
 smbclient deb net optional arch=any
 winbind deb net optional arch=any
Checksums-Sha1:
 eb322235696de71ccf6d847d21d4277e51ac77dd 11787668 samba_4.13.13+dfsg.orig.tar.xz
 cc02d6a311e34e6e5f9f0e5e74a1f2871f06b25f 552112 samba_4.13.13+dfsg-1~deb11u6.debian.tar.xz
Checksums-Sha256:
 c4747c211a2050e583d706cf380d48f5d9c1021536a9229fd6ba69e461545c46 11787668 samba_4.13.13+dfsg.orig.tar.xz
 2c8ea1a78096eb431e2f458dd58054a1b428a42003e2088845e40dc2f553018e 552112 samba_4.13.13+dfsg-1~deb11u6.debian.tar.xz
Files:
 c272fc38655e965733ba1a9e3aa52019 11787668 samba_4.13.13+dfsg.orig.tar.xz
 bd5c01f0cda8f5f328026c07713f41fa 552112 samba_4.13.13+dfsg-1~deb11u6.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iHUEARYIAB0WIQRZVjztY8b+Ty43oH1itBCJKh26HQUCZf2f2wAKCRBitBCJKh26
HaU4AP4uazOo86V5GFOTVnYgUicaPllOPCdYJVPkbZOk8YoilwEAyinnElMgM8GP
9vYGt+RluyQ+cLUAslRXHvZujSpIOwo=
=B5Rz
-----END PGP SIGNATURE-----
