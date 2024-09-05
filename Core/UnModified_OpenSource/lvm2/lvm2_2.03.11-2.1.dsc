-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: lvm2
Binary: lvm2, lvm2-udeb, lvm2-dbusd, lvm2-lockd, libdevmapper-dev, libdevmapper1.02.1, libdevmapper1.02.1-udeb, dmsetup, dmsetup-udeb, libdevmapper-event1.02.1, dmeventd, liblvm2cmd2.03, liblvm2-dev
Architecture: linux-any all
Version: 2.03.11-2.1
Maintainer: Debian LVM Team <team+lvm@tracker.debian.org>
Uploaders: Bastian Blank <waldi@debian.org>
Homepage: https://sourceware.org/lvm2/
Standards-Version: 4.1.1
Vcs-Browser: https://salsa.debian.org/lvm-team/lvm2
Vcs-Git: https://salsa.debian.org/lvm-team/lvm2.git
Build-Depends: debhelper-compat (= 13), autoconf-archive, automake, libaio-dev, libblkid-dev, pkg-config, systemd
Build-Depends-Arch: libcmap-dev, libcorosync-common-dev, libcpg-dev, libdlm-dev (>> 2), libdlmcontrol-dev, libedit-dev, libquorum-dev, libsanlock-dev, libselinux1-dev, libsystemd-dev, libudev-dev
Build-Depends-Indep: dh-sequence-python3, python3-dev, python3-dbus, python3-pyudev
Package-List:
 dmeventd deb admin optional arch=linux-any
 dmsetup deb admin optional arch=linux-any
 dmsetup-udeb udeb debian-installer optional arch=linux-any
 libdevmapper-dev deb libdevel optional arch=linux-any
 libdevmapper-event1.02.1 deb libs optional arch=linux-any
 libdevmapper1.02.1 deb libs optional arch=linux-any
 libdevmapper1.02.1-udeb udeb debian-installer optional arch=linux-any
 liblvm2-dev deb libdevel optional arch=linux-any
 liblvm2cmd2.03 deb libs optional arch=linux-any
 lvm2 deb admin optional arch=linux-any
 lvm2-dbusd deb admin optional arch=all
 lvm2-lockd deb admin optional arch=linux-any
 lvm2-udeb udeb debian-installer optional arch=linux-any
Checksums-Sha1:
 131a5943a49f141d67fbd75ff4d6577c884fe132 1699012 lvm2_2.03.11.orig.tar.xz
 f00490baab52b83f334f6ca98021c41c28c083dd 32208 lvm2_2.03.11-2.1.debian.tar.xz
Checksums-Sha256:
 7ef41edc65c4b807c5667ac7e9c371016d0db2a641812b334571acc0e025d86c 1699012 lvm2_2.03.11.orig.tar.xz
 7b7b8aaffdefdc99a8ae2f25928c2f0bfd0451c639db5c44c42e62f56c089cca 32208 lvm2_2.03.11-2.1.debian.tar.xz
Files:
 7abb38e01b740dd7cbbe3d2aac93f1bc 1699012 lvm2_2.03.11.orig.tar.xz
 5c106b27d86b5460e4077d3892a13513 32208 lvm2_2.03.11-2.1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQJKBAEBCAA0FiEE9GM4QTXwpzdLbkGwrEC7WJcE6vwFAmA0KRwWHGl2by5kZWRl
Y2tlckB1Z2VudC5iZQAKCRCsQLtYlwTq/NODEACi6pMxtJYEI6/jTPKfnvc9D48Z
GV1MiCLNAeWZ3CwSqBWwqwj8p1SEZFoJMFX++LySbrpgY5mnChk5bVP9sHNyYRMj
c/XBuUwOlwEes5N4OCfUmcV3kdQSEHmyAaX4C5jTdar6sF3Llw8yxPyTugY5WjXz
sDnxEt9TmJJGzyojOtTIwiKQPZE6nHTQ8TBbFBkVRm0kgpiJzPc6GhQGHV55J+OW
BPssQhwr8DaxIYWE7O8NzlpLYZUjzsOgmBrNNauxazlfTxlYHjBVWt+6sJQq21bo
O3F6vu68jtmngAd9OdPDWXnNZLq9xmTxPSwyTparoxTi6UH6u5qyuIc4KonUXlk6
zmDDyXhL9LdAYMxV19P1ladeTPxW2O8C2nXzzxGL5Xlwz37tP8ptKvwlSt+9CIyr
H8GVlhEK8qt8+YBos8ddzsL8qgAQUKLaOVD/nwGNTy16ferc734sCQt7Jcp8b1L8
z4Uz+avHOLENP5UzvvAhpSj3Ow9oiTMo8Q/7IS2E74ygdc2oXdjr5yBSa1LmU7Ca
5Ge4PL03eGI1CUrsVTZK5mWxm+nrPygxBb0/Ri7HkEcnvwn3oQ2ZhtRQKlFeI64E
k9E9dDpyMRD0HwThBhHcXRGnHnhz73rN/P5Lu8vDUXaFduAQJp9tajJkh/rvRCdE
YLvtoqYNVGgLvHjSLw==
=iNWq
-----END PGP SIGNATURE-----
