-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: cmake
Binary: cmake, cmake-data, cmake-curses-gui, cmake-qt-gui, cmake-doc
Architecture: any all
Version: 3.18.4-2+deb11u1
Maintainer: Debian CMake Team <pkg-cmake-team@lists.alioth.debian.org>
Uploaders: Lisandro Damián Nicanor Pérez Meyer <lisandro@debian.org>, Felix Geyer <fgeyer@debian.org>
Homepage: https://cmake.org/
Standards-Version: 4.1.3
Vcs-Browser: https://salsa.debian.org/cmake-team/cmake
Vcs-Git: https://salsa.debian.org/cmake-team/cmake.git
Testsuite: autopkgtest
Testsuite-Triggers: @builddeps@, bison, cvs, default-jdk, doxygen, flex, gettext, git, hspell, icoutils, imagemagick, libarchive-dev, libarmadillo-dev, libasound2-dev, libboost-dev, libbz2-dev, libcups2-dev, libcurl4-openssl-dev, libexpat1-dev, libfreetype6-dev, libgif-dev, libgnutls28-dev, libgtk2.0-dev, liblzma-dev, libmagick++-dev, libmagickcore-dev, libmagickwand-dev, libopenscenegraph-dev, libphysfs-dev, libprotobuf-dev, libtiff5-dev, libxml2-dev, libxslt1-dev, mercurial, ninja-build, perl, pkg-config, protobuf-compiler, python3-dev, ruby-dev, subversion, swig, zlib1g-dev
Build-Depends: debhelper-compat (= 12), freebsd-glue [kfreebsd-any], libarchive-dev (>= 3.3.3), libcurl4-openssl-dev | libcurl-ssl-dev, libexpat1-dev, libjsoncpp-dev, libncurses5-dev, librhash-dev, libuv1-dev (>= 1.10), procps [!hurd-any], python3-sphinx, python3-sphinxcontrib.qthelp | python3-sphinx (<< 2.0), qtbase5-dev <!stage1>, zlib1g-dev
Package-List:
 cmake deb devel optional arch=any
 cmake-curses-gui deb devel optional arch=any
 cmake-data deb devel optional arch=all
 cmake-doc deb doc optional arch=all
 cmake-qt-gui deb devel optional arch=any profile=!stage1
Checksums-Sha1:
 73ab5348c881f1a53c250b66848b6ee101c9fe1f 8976659 cmake_3.18.4.orig.tar.gz
 0b392e25524a199076d4f36ee5cda0739bce859b 29760 cmake_3.18.4-2+deb11u1.debian.tar.xz
Checksums-Sha256:
 597c61358e6a92ecbfad42a9b5321ddd801fc7e7eca08441307c9138382d4f77 8976659 cmake_3.18.4.orig.tar.gz
 61057001f141a646afb2456fe2414374f685da027f7da2cbdc2add394b748538 29760 cmake_3.18.4-2+deb11u1.debian.tar.xz
Files:
 0380beaee1c39a22455db02651abe7be 8976659 cmake_3.18.4.orig.tar.gz
 41c3c43c298add4dc11d45bc93542f4c 29760 cmake_3.18.4-2+deb11u1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQHIBAEBCgAyFiEEJvtDgpxjkjCIVtam+C8H+466LVkFAmFl6mUUHHJvZWhsaW5n
QGRlYmlhbi5vcmcACgkQ+C8H+466LVndJAv/ZqNkddT24syQIBT8TgAg96jLB1Zw
k7EZcSUzYkQbz6/GKZxCmPxMHC8jHIxm0dz7xFnUuhgezeK0cDGBg1X7LFqdhkt4
jopuQli6kxLLb6KPYNWDQkdrycIkmr5WbfR2wd+rtVU0TBb93kGRnQXLv5H20Jo9
UpgSlwZOVwBBYdZWLK9bVm/6FNhv8Bl9072NPs2u9o/VHfG/w3FDBdR2hGF3W1yL
ykgq+xoy7EBnXsne8rrR4Q/DHvQVOZs3FPjpj3YztYK7v6AIXDDKVJXRuQwwfUg+
AUp2uC3POz1/OYm3Rv6LWu4/vKBlArkn+JAKZAI9GdhmStbS6TGsSdSVUUllZ43H
myZQIV+O5apxE5RP+wkONd6sH3fpBRz19JJUOsVZ4mrUTjivtSBPoFYmxTsqvieK
JVEw9okpjwGw+uYJgHy3UG7uNgiwcfuE93F4WLO4ZcP5lBvPOn/mrflwStJKH9nj
N7EJkJleKgC9oJqGA74uJHVR6ARRYCLlfQKU
=eZPG
-----END PGP SIGNATURE-----
