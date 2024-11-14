-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: glib2.0
Binary: libglib2.0-0, libglib2.0-tests, libglib2.0-udeb, libglib2.0-bin, libglib2.0-dev, libglib2.0-dev-bin, libglib2.0-data, libglib2.0-doc
Architecture: any all
Version: 2.66.8-1+deb11u4
Maintainer: Debian GNOME Maintainers <pkg-gnome-maintainers@lists.alioth.debian.org>
Uploaders: Iain Lane <laney@debian.org>, Simon McVittie <smcv@debian.org>
Homepage: https://wiki.gnome.org/Projects/GLib
Standards-Version: 4.5.1
Vcs-Browser: https://salsa.debian.org/gnome-team/glib
Vcs-Git: https://salsa.debian.org/gnome-team/glib.git
Testsuite: autopkgtest
Testsuite-Triggers: build-essential, dbus, dbus-x11, dpkg-dev, gnome-desktop-testing, xauth, xvfb
Build-Depends: dbus <!nocheck>, debhelper-compat (= 12), desktop-file-utils <!nocheck>, dh-exec, dh-python, docbook-xml, docbook-xsl, dpkg-dev (>= 1.17.14), gettext, gnome-pkg-tools (>= 0.11), libdbus-1-dev (>= 1.2.14) <!nocheck> <!noinsttest>, libelf-dev (>= 0.142), libffi-dev (>= 3.3), libgamin-dev [hurd-any] | libfam-dev [hurd-any], libmount-dev (>= 2.35.2-7~) [linux-any], libpcre3-dev (>= 1:8.35), libselinux1-dev [linux-any], libxml2-utils, linux-libc-dev [linux-any], meson (>= 0.52.0), pkg-config (>= 0.16.0), python3-dbus <!nocheck>, python3-distutils, python3-gi <!nocheck>, python3:any (>= 2.7.5-5~), shared-mime-info <!nocheck>, tzdata <!nocheck>, xsltproc, xterm <!nocheck>, zlib1g-dev
Build-Depends-Indep: gtk-doc-tools (>= 1.32.1)
Package-List:
 libglib2.0-0 deb libs optional arch=any
 libglib2.0-bin deb misc optional arch=any
 libglib2.0-data deb libs optional arch=all
 libglib2.0-dev deb libdevel optional arch=any
 libglib2.0-dev-bin deb libdevel optional arch=any
 libglib2.0-doc deb doc optional arch=all
 libglib2.0-tests deb libs optional arch=any profile=!noinsttest
 libglib2.0-udeb udeb debian-installer optional arch=any profile=!noudeb
Checksums-Sha1:
 668795cd3c2698a28987fb38d632ff7642ddf377 4845548 glib2.0_2.66.8.orig.tar.xz
 5302a8bef7e1dd807b87ff84642214aad1fb1e91 188516 glib2.0_2.66.8-1+deb11u4.debian.tar.xz
Checksums-Sha256:
 97bc87dd91365589af5cbbfea2574833aea7a1b71840fd365ecd2852c76b9c8b 4845548 glib2.0_2.66.8.orig.tar.xz
 cfa47700ccca4f73e1ccf26a11051bf3cc4f28e57bfd39fa04cc0c0f2d65e38b 188516 glib2.0_2.66.8-1+deb11u4.debian.tar.xz
Files:
 705dd46a43d339e8aea19e946e71c32a 4845548 glib2.0_2.66.8.orig.tar.xz
 3a4722268cff7b062ca6b9da3fa27d31 188516 glib2.0_2.66.8-1+deb11u4.debian.tar.xz
Dgit: 0a0e8c97a3a784aee59fdc98869a6fcbebf36d08 debian archive/debian/2.66.8-1+deb11u4 https://git.dgit.debian.org/glib2.0

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCAAdFiEENuxaZEik9e95vv6Y4FrhR4+BTE8FAmZEhFIACgkQ4FrhR4+B
TE/tQg/9GrSBhUA73vc7tfJhXb+Qss4p+pdrI1sQRsarWwK4/ETCAokppGa730Ha
3JgyyT1SFF7JCa6xjHovGZGs6z365zg5cWRnfREx/PlpnN3UHCcfSltTbI+dNNON
ZCvuwU1aIGNDl1/6Jt6BqPCuFIz3JEhhD1wkcvPoXlxA6Z3OecEAeK69zLLrxf/8
Gj8PoLwIas0Uzzy/LrFXhNb8qwH5T7ZW7H2OOKL98EY8GNMdkD4ZMnmZloaL7Et1
T6mHthevH4FE0MPo6aICYJmoGD7izE2b/yeQavHUe09UFTXjmlcJXmcv39hT/P05
3n/GbyNMBUJR+JfKTrlNfQVwoQJqIkG/YyHkjB6G0ba/YtiPURgYazbtLAfabNcp
4Z4nsmjFpC40UbC9gxKmPY3PI478uMQTD9+ZLihzzMyxEuBNgwU+ut8bwNAWfk7M
ceDPtH2/yh9/yafKY7Ia1iny+qiOue2ZdASey5yrQnTGac+wQDodF8fbw20QbYut
Y4GL0HL23Z/gcD+12gMdhbTozpc7p1N2MLT0kK8OSrV2Wpwd4OzazL805S8Fdqb5
KoPcSHUeQQTFJ7Lz/LKU4Owvqh+9IKRwEo0RJuFZRtRlMxXx7P7A663dnNs96LFI
xqUwa2XKvD1z22JDyxU4k/To4l/4PaT8rb85hLxHBjbtHmSqR8c=
=Xl6M
-----END PGP SIGNATURE-----
