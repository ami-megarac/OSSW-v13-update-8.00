-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: sdbus-cpp
Binary: libsdbus-c++-bin, libsdbus-c++-dev, libsdbus-c++-doc, libsdbus-c++0
Architecture: linux-any all
Version: 0.8.3-4
Maintainer: Shengjing Zhu <zhsj@debian.org>
Homepage: https://github.com/Kistler-Group/sdbus-cpp
Standards-Version: 4.5.1
Vcs-Browser: https://salsa.debian.org/zhsj/sdbus-cpp
Vcs-Git: https://salsa.debian.org/zhsj/sdbus-cpp.git
Build-Depends: cmake, dbus, debhelper-compat (= 13), googletest (>= 1.10.0~), libexpat1-dev, libsystemd-dev, pkg-config
Build-Depends-Indep: doxygen
Package-List:
 libsdbus-c++-bin deb utils optional arch=linux-any
 libsdbus-c++-dev deb libdevel optional arch=linux-any
 libsdbus-c++-doc deb doc optional arch=all
 libsdbus-c++0 deb libs optional arch=linux-any
Checksums-Sha1:
 003700d9710ab8563681186d5ec244e48bc8ed94 190124 sdbus-cpp_0.8.3.orig.tar.gz
 844bfbb3cd0da8151710683be02830625e45fb05 2940 sdbus-cpp_0.8.3-4.debian.tar.xz
Checksums-Sha256:
 0fd575ae0f463773dd9141242d1133731e2b780fd6526650ce992ba711d88628 190124 sdbus-cpp_0.8.3.orig.tar.gz
 8b4786b48605e6190901639c2f8cf7985c907d3798eb9174043237f4b7852757 2940 sdbus-cpp_0.8.3-4.debian.tar.xz
Files:
 ade438a7ea3ce50ae15ad3721f683197 190124 sdbus-cpp_0.8.3.orig.tar.gz
 e03f171151ea4eebfef0f2445a36c04e 2940 sdbus-cpp_0.8.3-4.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iIYEARYIAC4WIQTiXc95jUQrjt9HgU3EhUo4GOCwFgUCYC7ANxAcemhzakBkZWJp
YW4ub3JnAAoJEMSFSjgY4LAWqV4BAK8EOk3NC7/SqmAj9adggF1HvNscJP3rro6O
WKHAcL2qAQDCJ0rqp0PYJdInusBGhvVaIDMxeYhmhDaV4EUuwFj4Dw==
=O81H
-----END PGP SIGNATURE-----
