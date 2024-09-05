-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: mariadb-10.5
Binary: libmariadb-dev, libmariadb-dev-compat, libmariadb3, libmariadbd19, libmariadbd-dev, mariadb-common, mariadb-client-core-10.5, mariadb-client-10.5, mariadb-server-core-10.5, mariadb-server-10.5, mariadb-server, mariadb-client, mariadb-backup, mariadb-plugin-connect, mariadb-plugin-s3, mariadb-plugin-rocksdb, mariadb-plugin-oqgraph, mariadb-plugin-mroonga, mariadb-plugin-spider, mariadb-plugin-gssapi-server, mariadb-plugin-gssapi-client, mariadb-plugin-cracklib-password-check, mariadb-test, mariadb-test-data
Architecture: any all
Version: 1:10.5.23-0+deb11u1
Maintainer: Debian MySQL Maintainers <pkg-mysql-maint@lists.alioth.debian.org>
Uploaders: Otto Kekäläinen <otto@debian.org>
Homepage: https://mariadb.org/
Standards-Version: 4.5.0
Vcs-Browser: https://salsa.debian.org/mariadb-team/mariadb-10.5
Vcs-Git: https://salsa.debian.org/mariadb-team/mariadb-10.5.git
Testsuite: autopkgtest
Testsuite-Triggers: eatmydata
Build-Depends: bison, cmake, cracklib-runtime <!nocheck>, debhelper (>= 10), dh-exec, gdb <!nocheck>, libaio-dev [linux-any], libboost-dev, libcrack2-dev (>= 2.9.0), libcurl4-openssl-dev | libcurl4-dev, libedit-dev, libedit-dev:native, libjemalloc-dev [linux-any], libjudy-dev, libkrb5-dev, liblz4-dev, libncurses5-dev (>= 5.0-6~), libncurses5-dev:native (>= 5.0-6~), libnuma-dev [linux-any], libpam0g-dev, libpcre2-dev, libsnappy-dev, libssl-dev, libssl-dev:native, libsystemd-dev [linux-any], libxml2-dev, libzstd-dev (>= 1.3.3), lsb-release, perl:any, po-debconf, psmisc, unixodbc-dev, uuid-dev, zlib1g-dev (>= 1:1.1.3-5~)
Package-List:
 libmariadb-dev deb libdevel optional arch=any
 libmariadb-dev-compat deb libdevel optional arch=any
 libmariadb3 deb libs optional arch=any
 libmariadbd-dev deb libdevel optional arch=any
 libmariadbd19 deb libs optional arch=any
 mariadb-backup deb database optional arch=any
 mariadb-client deb database optional arch=all
 mariadb-client-10.5 deb database optional arch=any
 mariadb-client-core-10.5 deb database optional arch=any
 mariadb-common deb database optional arch=all
 mariadb-plugin-connect deb database optional arch=any
 mariadb-plugin-cracklib-password-check deb database optional arch=any
 mariadb-plugin-gssapi-client deb database optional arch=any
 mariadb-plugin-gssapi-server deb database optional arch=any
 mariadb-plugin-mroonga deb database optional arch=any-alpha,any-amd64,any-arm,any-arm64,any-i386,any-ia64,any-mips64el,any-mips64r6el,any-mipsel,any-mipsr6el,any-nios2,any-powerpcel,any-ppc64el,any-sh3,any-sh4,any-tilegx
 mariadb-plugin-oqgraph deb database optional arch=any
 mariadb-plugin-rocksdb deb database optional arch=amd64,arm64,mips64el,ppc64el
 mariadb-plugin-s3 deb database optional arch=any
 mariadb-plugin-spider deb database optional arch=any
 mariadb-server deb database optional arch=all
 mariadb-server-10.5 deb database optional arch=any
 mariadb-server-core-10.5 deb database optional arch=any
 mariadb-test deb database optional arch=any
 mariadb-test-data deb database optional arch=all
Checksums-Sha1:
 10abdd7453eca557df81c4b929a1715dff6ece6a 114829503 mariadb-10.5_10.5.23.orig.tar.gz
 da5ee3541869a676e6ad3186eccdaae77d6527bb 833 mariadb-10.5_10.5.23.orig.tar.gz.asc
 26bdc9ab2cf8cd913f96a5f9995c37534e9cb379 229380 mariadb-10.5_10.5.23-0+deb11u1.debian.tar.xz
Checksums-Sha256:
 3f7c33b208cb799db014a874f56718eadff22ee95781fa360aa6bc696109acfe 114829503 mariadb-10.5_10.5.23.orig.tar.gz
 47f9451521033ff561aa8fff8d4302bf2cfae229c0754c997ed3ca00b64d23fe 833 mariadb-10.5_10.5.23.orig.tar.gz.asc
 45946cf3a93adbb0c9e4cd0bcd9a90142ad0c006f75eb9529a2f02e7fc011714 229380 mariadb-10.5_10.5.23-0+deb11u1.debian.tar.xz
Files:
 cc89da1cb931bc920f3b0492cb7f1657 114829503 mariadb-10.5_10.5.23.orig.tar.gz
 d43d87ec9250868c4cd633edfd89c1b3 833 mariadb-10.5_10.5.23.orig.tar.gz.asc
 5b14b9b8ad1944c0407c7135cdf91042 229380 mariadb-10.5_10.5.23-0+deb11u1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCgAdFiEEmbRSsR88dMO0U+RvvthEn87o2ogFAmWP/Y4ACgkQvthEn87o
2ohQ+BAAwD2ThXeCHivvQ5YCct11njVQ/YeO+z9/TFOo1l3DgwzHhY3j+tln9a0P
h85eAeDK8xPGSoYSauvBowQsEsFznrly1YMNRTrxtY0sCrzpM54mH3+4aZHmJWFI
rf8bT8Tx5sxaiIVCPEv/f3JYh2yhqmbSGFiDLSUYw3RU04kfDMIqVnPevaRsgmpq
kcEX/nSv8JBRA0dcctKWQ9LyeYIkfpHfSVInxzY7HjWhbMHxMNwZoG+yazZNNWd0
h0ty4+Es30tWiEEEkva765SwKGVLK/aFfUHFggVvYBWuQ/kAL2xdF9USs/Z3otry
UeMfbK1FsVnVzCDCa8iQPdgvC+nVl8BjaRTlEPb9ThvZfmAzLVwiHxThfYuOWLuU
mTO+SavUTqj28pDVTcOFwXqgpvz6grKazhJ6msxKjFNM2as3nHnIGVaUr8KOGJA3
iwBFi0GgP0oWWGE4XZjuE2BkVh4Jwps0+DSVk3chLj4lsTVERmPhpI5WgBfiX6dd
S43dGfNUSSVLKY4IbJZHq90bdSrOiA1yteVFrI7xboZ8hS0xmpiXdPqgD1XwkF3F
tshh3vWBWEy7n/QX1KUKly7FAF5fGIxcpeknVYSGa4MSjcqZI0aFQuwcgMvrne2q
W7BZJ/7ymTsZ8LaEsRA50RGN9/mEkh/zRq+LsC+y/xkHLle223Q=
=hj4J
-----END PGP SIGNATURE-----
