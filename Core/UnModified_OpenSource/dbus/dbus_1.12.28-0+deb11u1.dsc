-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: dbus
Binary: dbus, dbus-1-doc, dbus-tests, dbus-udeb, dbus-user-session, dbus-x11, libdbus-1-3, libdbus-1-3-udeb, libdbus-1-dev
Architecture: any all
Version: 1.12.28-0+deb11u1
Maintainer: Utopia Maintenance Team <pkg-utopia-maintainers@lists.alioth.debian.org>
Uploaders:  Sjoerd Simons <sjoerd@debian.org>, Sebastian Dröge <slomo@debian.org>, Michael Biebl <biebl@debian.org>, Loic Minier <lool@dooz.org>, Simon McVittie <smcv@debian.org>,
Homepage: https://dbus.freedesktop.org/
Standards-Version: 4.5.1
Vcs-Browser: https://salsa.debian.org/utopia-team/dbus
Vcs-Git: https://salsa.debian.org/utopia-team/dbus.git -b debian/bullseye
Testsuite: autopkgtest
Testsuite-Triggers: apparmor, build-essential, gnome-desktop-testing, init, xauth, xvfb
Build-Depends: autoconf-archive (>= 20150224), automake, debhelper-compat (= 12), dh-exec, libapparmor-dev [linux-any], libaudit-dev [linux-any], libcap-ng-dev [linux-any], libexpat-dev, libglib2.0-dev <!nocheck> <!noinsttest>, libnss-wrapper <!nocheck>, libselinux1-dev [linux-any], libsystemd-dev [linux-any], libx11-dev, pkg-config, python3 <!nocheck> <!noinsttest>, python3-dbus <!nocheck> <!noinsttest>, python3-gi <!nocheck> <!noinsttest>, valgrind [amd64 arm64 armhf i386 mips64 mips64el mips mipsel powerpc ppc64 ppc64el s390x], xmlto <!nodoc>
Build-Depends-Indep: doxygen <!nodoc>, ducktype <!nodoc>, xsltproc <!nodoc>, yelp-tools <!nodoc>
Package-List:
 dbus deb admin standard arch=any
 dbus-1-doc deb doc optional arch=all profile=!nodoc
 dbus-tests deb misc optional arch=any profile=!noinsttest
 dbus-udeb udeb debian-installer optional arch=any profile=!noudeb
 dbus-user-session deb admin optional arch=linux-any
 dbus-x11 deb x11 optional arch=any
 libdbus-1-3 deb libs optional arch=any
 libdbus-1-3-udeb udeb debian-installer optional arch=any profile=!noudeb
 libdbus-1-dev deb libdevel optional arch=any
Checksums-Sha1:
 b367eab7a052f9079ed3c6bdfc5db95031df6ee4 2122182 dbus_1.12.28.orig.tar.gz
 4b2a2fd2909cd72dddbca9202938095437484378 833 dbus_1.12.28.orig.tar.gz.asc
 101580ee5ba8eacbd288c4f35eeec82606fd74fb 58556 dbus_1.12.28-0+deb11u1.debian.tar.xz
Checksums-Sha256:
 9da1e3f2b73f75eec0a9e4509d64be43909d1f2853fe809528a0a53984d76420 2122182 dbus_1.12.28.orig.tar.gz
 3f6c19d8c063459682d49d4bf74fc0d13290664ae966f612f118f4d4a73ddaab 833 dbus_1.12.28.orig.tar.gz.asc
 b07222a653b330e8f81e3642209f2cb7fb1dba3f8207c755b146d657767fbb48 58556 dbus_1.12.28-0+deb11u1.debian.tar.xz
Files:
 28d92a7a576f7feec7ddb3bb87b28b43 2122182 dbus_1.12.28.orig.tar.gz
 fdb93fff82091ad4527217ecc9b02c5b 833 dbus_1.12.28.orig.tar.gz.asc
 d1ca004b05e34caafe9ba9a5c20ca849 58556 dbus_1.12.28-0+deb11u1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCAAdFiEENuxaZEik9e95vv6Y4FrhR4+BTE8FAmSNzoQACgkQ4FrhR4+B
TE9wCg/+OAQtFZKnpeXp4FZZlxphVLsD1fcmbrBNKmP4zo/Y+dQq1k7onNfJt1Vq
XDtbZfAu64fj7Y0wrGRA20L6Nl5c2mPmZ8FiNAIGMmymBQRXXC4aKRgJ1k28b1z6
WB7pBzR8ofy9yj8HKqkX/fxoSeN9bv4D1UxlCzNE2PhQvLh2GNTEItEOhY24QPTe
d+2Xw/7LYOfDQ8rMROScq3rqw4nO6bw3BfcV+QVWabQNCHdgL3LR01sbUzQtVfsM
8N0uUJZEDIcOXcAyz6LSe5RmkufSOD1D5lNgHtlYkSj7ERvch5t9hwlxNS5lgL83
TpPP9/Fyjsh5rgeaahzulEATbUoZqEoiV2XUjxEbd2r3mQ1PLTS3n+/ZD/4WLyD0
+it+7yvT85qeXuseorI9UTA33v96velJ2j1oWdsQkV43dwBlIUNsnJxOVIOzepkb
aQoEr5RzMKXlS/NXb0uliqyRi1Ro/UVfyIbQF6RNJe0y1Kw1kvLeoKRiFwL3YPg9
9vuiOUURa4YT2S/K7MlTe6hJ3OsA1Qw63+3sqf5veCuankVFSgFN0utmf6IhqTZk
2kXqeo0lPzHlw6ouc8q5R1Wr+pGLwrz6qcUhfAvRjN2mj2T/IMxuKif25/8acaCV
YmL//dgE6BiJSUV9SqP6jlArvRcMNFE+iQSqtf4fVXJ9aG02ewI=
=mg4s
-----END PGP SIGNATURE-----
