-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: libsemanage
Binary: libsemanage-common, libsemanage1, libsemanage1-dev, ruby-semanage, python3-semanage, semanage-utils
Architecture: linux-any all
Version: 3.1-1
Maintainer: Debian SELinux maintainers <selinux-devel@lists.alioth.debian.org>
Uploaders: Laurent Bigonville <bigon@debian.org>, Russell Coker <russell@coker.com.au>
Homepage: http://userspace.selinuxproject.org/
Standards-Version: 4.5.0
Vcs-Browser: https://salsa.debian.org/selinux-team/libsemanage
Vcs-Git: https://salsa.debian.org/selinux-team/libsemanage.git
Build-Depends: bison, debhelper-compat (= 12), dh-python <!nopython>, file, flex, gem2deb (>= 0.5.0~) <!noruby>, libaudit-dev, libbz2-dev, libcunit1-dev <!nocheck>, libselinux1-dev (>= 3.1), libsepol1-dev (>= 3.1), pkg-config, python3-all-dev <!nopython>, secilc (>= 3.1) <!nocheck>, swig <!nopython> <!noruby>
Package-List:
 libsemanage-common deb libs optional arch=all
 libsemanage1 deb libs optional arch=linux-any
 libsemanage1-dev deb libdevel optional arch=linux-any
 python3-semanage deb python optional arch=linux-any profile=!nopython
 ruby-semanage deb ruby optional arch=linux-any profile=!noruby
 semanage-utils deb admin optional arch=linux-any
Checksums-Sha1:
 2f0cd4a40f9b43d76c5f363518fc0751b8559197 179601 libsemanage_3.1.orig.tar.gz
 77d58bb4428461b5a303e2c861a3b2218a0bff2d 17556 libsemanage_3.1-1.debian.tar.xz
Checksums-Sha256:
 22d6c75526e40d1781c30bcf29abf97171bdfe6780923f11c8e1c76a75a21ff8 179601 libsemanage_3.1.orig.tar.gz
 185b151158faaaf3d8f9ff939f29efd3eb5dbb050d01a87d3fde6cf40e778648 17556 libsemanage_3.1-1.debian.tar.xz
Files:
 d16eee8c1dc8cf43f59957d575d6bd29 179601 libsemanage_3.1.orig.tar.gz
 67791347424d5a1fad68db22faf0f63a 17556 libsemanage_3.1-1.debian.tar.xz
Ruby-Versions: all

-----BEGIN PGP SIGNATURE-----

iQFFBAEBCAAvFiEEmRrdqQAhuF2x31DwH8WJHrqwQ9UFAl8Ot+IRHGJpZ29uQGRl
Ymlhbi5vcmcACgkQH8WJHrqwQ9WnrAgAgMH2y/iXoezXzyzwAPc99z3Y/O0JL6Qy
uyy9/s4dg8BkREIaQrT1fG/Kxn2jIw64txN06Y70zdEwgxCx43GIzneZvjA1sSZE
fIYxyarcCnqFU9hSyAyTjZ5lsLkQBbX+WArRqTdd5DG/6TpKiGdlg92kg60jr+lY
L4qZBHDf1RGR1cGf4jzakrnbgMbXy14wywYbvzFpmKLlkfUMJe7MjSutCe2IDwhC
7fyqwwSJsqVS+O4mEHeQoJLeyJohnL4GFldadMwYniKvqE5n7dJ6Hw7pkF08KxON
HxrCQcu//wr2jfsgywG0Pt0XXvRZFgDqu3BDHeFCttPrOjVRvYtr/A==
=SvwV
-----END PGP SIGNATURE-----
