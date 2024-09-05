-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: apparmor
Binary: apparmor, apparmor-utils, apparmor-profiles, libapparmor-dev, libapparmor1, libapparmor-perl, libapache2-mod-apparmor, libpam-apparmor, apparmor-notify, python3-libapparmor, python3-apparmor, dh-apparmor, apparmor-easyprof
Architecture: linux-any all
Version: 2.13.6-10
Maintainer: Debian AppArmor Team <pkg-apparmor-team@lists.alioth.debian.org>
Uploaders: intrigeri <intrigeri@debian.org>
Homepage: https://apparmor.net/
Standards-Version: 4.5.1
Vcs-Browser: https://salsa.debian.org/apparmor-team/apparmor/tree/debian/master
Vcs-Git: https://salsa.debian.org/apparmor-team/apparmor.git -b debian/master
Testsuite: autopkgtest
Testsuite-Triggers: @builddeps@, apparmor-profiles-extra, bind9, cups-browsed, cups-daemon, evince, haveged, libreoffice-common, libvirt-daemon-system, linux-image-amd64, linux-image-generic, man-db, ntp, onioncircuits, tcpdump, tor
Build-Depends: apache2-dev, autoconf, automake, bison, bzip2, chrpath, debhelper-compat (= 13), dejagnu <!nocheck>, dh-apache2, dh-python, flex, liblocale-gettext-perl <!nocheck>, libpam-dev, libtool, perl, pkg-config, po-debconf, python3, python3-all, python3-all-dev, swig
Package-List:
 apparmor deb admin optional arch=linux-any
 apparmor-easyprof deb admin optional arch=all
 apparmor-notify deb admin optional arch=all
 apparmor-profiles deb admin optional arch=all
 apparmor-utils deb admin optional arch=linux-any
 dh-apparmor deb devel optional arch=all
 libapache2-mod-apparmor deb httpd optional arch=linux-any
 libapparmor-dev deb libdevel optional arch=linux-any
 libapparmor-perl deb perl optional arch=linux-any
 libapparmor1 deb libs optional arch=linux-any
 libpam-apparmor deb admin optional arch=linux-any
 python3-apparmor deb python optional arch=linux-any
 python3-libapparmor deb python optional arch=linux-any
Checksums-Sha1:
 403235a3923efd51a574ac7f833d5518894bc91b 7399438 apparmor_2.13.6.orig.tar.gz
 ae1e68dd97710a2a63f1d70ba06fb9d530e0d8e1 870 apparmor_2.13.6.orig.tar.gz.asc
 fc20504890e46b20faf028285cbd827fb6889135 109296 apparmor_2.13.6-10.debian.tar.xz
Checksums-Sha256:
 4426b8b73f368889aea69cbc224ae666d4145b9520a2866cc2a915908e87ba8f 7399438 apparmor_2.13.6.orig.tar.gz
 74f180f2108043842a64671239aa408062eae3681b60602801246d4e6f63eccf 870 apparmor_2.13.6.orig.tar.gz.asc
 e62ffde6e5aa39d2858a1c730ea4034ad933ee1f00af8a42fb256b0f15f389a8 109296 apparmor_2.13.6-10.debian.tar.xz
Files:
 377545626db2278ef5d1319b68611c84 7399438 apparmor_2.13.6.orig.tar.gz
 be7031dff39357b76824e4b7ea8b09bc 870 apparmor_2.13.6.orig.tar.gz.asc
 59e9969ccc8746f1e342f837aed76cef 109296 apparmor_2.13.6-10.debian.tar.xz
Dgit: 43161bd09d4b877a40c179806d79ad70b1be6960 debian archive/debian/2.13.6-10 https://git.dgit.debian.org/apparmor

-----BEGIN PGP SIGNATURE-----

iIsEARYKADMWIQRhtDRcZu/HkP7YWcafj6cvaVTDowUCYGgT2hUcaW50cmlnZXJp
QGRlYmlhbi5vcmcACgkQn4+nL2lUw6PEiQD/bXCuRxCVaIumELVPY1vHR42zbzzb
se1Uil+duM/NLlAA/1ujDMYRVIsNkU9+sY7Hz8DEFiF08lEFv/Ot3lmLhUkL
=jxMq
-----END PGP SIGNATURE-----
