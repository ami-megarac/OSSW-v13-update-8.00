-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: cmake
Binary: cmake, cmake-data, cmake-curses-gui, cmake-qt-gui, cmake-doc
Architecture: any all
Version: 3.25.1-1~bpo11+1
Maintainer: Debian CMake Team <pkg-cmake-team@lists.alioth.debian.org>
Uploaders:  Lisandro Damián Nicanor Pérez Meyer <lisandro@debian.org>, Felix Geyer <fgeyer@debian.org>, Timo Röhling <roehling@debian.org>,
Homepage: https://cmake.org/
Description: cross-platform, open-source make system
 CMake is used to control the software compilation process using
 simple platform and compiler independent configuration files. CMake
 generates native makefiles and workspaces that can be used in the
 compiler environment of your choice. CMake is quite sophisticated: it
 is possible to support complex environments requiring system
 configuration, pre-processor generation, code generation, and template
 instantiation.
Standards-Version: 4.6.1
Vcs-Browser: https://salsa.debian.org/cmake-team/cmake
Vcs-Git: https://salsa.debian.org/cmake-team/cmake.git
Testsuite: autopkgtest
Testsuite-Triggers: @builddeps@, bison, cvs, default-jdk, doxygen, flex, gettext, git, hspell, icoutils, imagemagick, libarchive-dev, libarmadillo-dev, libasound2-dev, libboost-dev, libbz2-dev, libcups2-dev, libcurl4-openssl-dev, libexpat1-dev, libfreetype6-dev, libgif-dev, libgnutls28-dev, libgtk2.0-dev, liblzma-dev, libmagick++-dev, libmagickcore-dev, libmagickwand-dev, libopenscenegraph-dev, libphysfs-dev, libprotobuf-dev, libtiff5-dev, libxml2-dev, libxslt1-dev, mercurial, ninja-build, perl, pkg-config, protobuf-compiler, python3-dev, ruby-dev, subversion, swig, zlib1g-dev
Build-Depends: debhelper-compat (= 13), cmake <cross>, freebsd-glue [kfreebsd-any], libarchive-dev (>= 3.3.3) <!pkg.cmake.bootstrap>, libcurl4-openssl-dev <!pkg.cmake.bootstrap> | libcurl-ssl-dev <!pkg.cmake.bootstrap>, libexpat1-dev <!pkg.cmake.bootstrap>, libjsoncpp-dev <!pkg.cmake.bootstrap>, libncurses5-dev <!pkg.cmake.bootstrap !pkg.cmake.nogui>, librhash-dev <!pkg.cmake.bootstrap>, libssl-dev <pkg.cmake.bootstrap>, libuv1-dev (>= 1.10) <!pkg.cmake.bootstrap>, procps [!hurd-any], python3-sphinx:native, python3-sphinxcontrib.qthelp:native, qtbase5-dev <!pkg.cmake.bootstrap !pkg.cmake.nogui>, zlib1g-dev <!pkg.cmake.bootstrap>
Build-Depends-Indep: dh-sequence-sphinxdoc
Package-List:
 cmake deb devel optional arch=any
 cmake-curses-gui deb devel optional arch=any profile=!pkg.cmake.bootstrap,!pkg.cmake.nogui
 cmake-data deb devel optional arch=all
 cmake-doc deb doc optional arch=all profile=!pkg.cmake.bootstrap
 cmake-qt-gui deb devel optional arch=any profile=!pkg.cmake.bootstrap,!pkg.cmake.nogui
Checksums-Sha1:
 6cc13d2b3d2c8fecfb2486e0e521d42ab45188d0 10561054 cmake_3.25.1.orig.tar.gz
 5a064aa668657ff72de99d04f75c4cf9a46e4a10 32792 cmake_3.25.1-1~bpo11+1.debian.tar.xz
Checksums-Sha256:
 1c511d09516af493694ed9baf13c55947a36389674d657a2d5e0ccedc6b291d8 10561054 cmake_3.25.1.orig.tar.gz
 c4856f8ffb00170f46aad50cea557f1b92e00225ede494e0181fea5fc7c71981 32792 cmake_3.25.1-1~bpo11+1.debian.tar.xz
Files:
 521a03b1c36412463eda0348bebf4765 10561054 cmake_3.25.1.orig.tar.gz
 ae39ccdc02b464d581d782da092cfb53 32792 cmake_3.25.1-1~bpo11+1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQHIBAEBCgAyFiEEJvtDgpxjkjCIVtam+C8H+466LVkFAmOLQfEUHHJvZWhsaW5n
QGRlYmlhbi5vcmcACgkQ+C8H+466LVmJPQwAuokl6KaJLxoBQsNR9bWr/4FQr+6g
J984JKV6ZTpN64siiL3M+Hk6isgLpWKRk+6VpYmiWbrZLZ+udXssK6ddUt4Fwmd9
/aaXsanxC0wYnC7cSphd7CfyPou6FtyNtVrS3ISq9aq4u74NagOolC7dApH/N4xl
DRqp4cHs/TAFy0aZvKlL3d1IqIWrBZfgsaciP3oqfCYi99eAUo60DP0y9/pAg7Um
7/Y9dBhYd36rAoKW4G4cgKvLdbwD0+ZY/B43m9eRM23ZZGSAkf1lcgspsT3Gchv4
0Cn+KrWCvjaoWFHVwePWeIn7UAN/3eoX55ipEXCEz6arpvqCR5JI22nxHtvrsRVc
jgvrALvlTsdJk+ntu0UeWL4pplMObxC5KJmZHnV/JRfGXjGe9SWY80mCC6tPD0bx
npenAaaA44UjT4KXFVbMEz++73V/xbIZpdlWvH/s6VTuZGWHy0r/hPARoBd3DTyM
sCF09Pv+tbfcM1enqU5B0Acz951c64iCWnU8
=HW4H
-----END PGP SIGNATURE-----
