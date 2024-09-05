-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: libxml2
Binary: libxml2, libxml2-utils, libxml2-dev, libxml2-doc, python3-libxml2, python3-libxml2-dbg
Architecture: any all
Version: 2.9.10+dfsg-6.7+deb11u4
Maintainer: Debian XML/SGML Group <debian-xml-sgml-pkgs@lists.alioth.debian.org>
Uploaders:  Aron Xu <aron@debian.org>, YunQiang Su <wzssyqa@gmail.com>,
Homepage: http://xmlsoft.org
Standards-Version: 4.5.0
Vcs-Browser: https://salsa.debian.org/xml-sgml-team/libxml2
Vcs-Git: https://salsa.debian.org/xml-sgml-team/libxml2.git
Testsuite: autopkgtest
Testsuite-Triggers: build-essential, pkg-config
Build-Depends: debhelper-compat (= 13)
Build-Depends-Arch: dh-sequence-python3 <!nopython>, libicu-dev, liblzma-dev, libpython3-all-dbg <!nopython>, libpython3-all-dev <!nopython>, pkg-config, python3-all-dbg:any <!nopython>, python3-all-dev:any (>= 3.5) <!nopython>, rename, zlib1g-dev | libz-dev
Build-Depends-Indep: pkg-config
Package-List:
 libxml2 deb libs optional arch=any
 libxml2-dev deb libdevel optional arch=any
 libxml2-doc deb doc optional arch=all
 libxml2-utils deb text optional arch=any
 python3-libxml2 deb python optional arch=any profile=!nopython
 python3-libxml2-dbg deb debug optional arch=any profile=!nopython
Checksums-Sha1:
 2578c0817feae47d78c4f987c7a2a32f87d89517 2503560 libxml2_2.9.10+dfsg.orig.tar.xz
 c81ab44e7161a6eea076731546170f3597b9f7fe 41904 libxml2_2.9.10+dfsg-6.7+deb11u4.debian.tar.xz
Checksums-Sha256:
 65ee7a2f5e100c64ddf7beb92297c9b2a30b994a76cd1fab67470cf22db6b7d0 2503560 libxml2_2.9.10+dfsg.orig.tar.xz
 62037376765cc8e5961771d758b8efa9fddce3cce6fd67c31bc111ddff656654 41904 libxml2_2.9.10+dfsg-6.7+deb11u4.debian.tar.xz
Files:
 4fb60521425df67f453b3c1ff0efbc1c 2503560 libxml2_2.9.10+dfsg.orig.tar.xz
 d14f4789d2783a8a502a53409381f3a1 41904 libxml2_2.9.10+dfsg-6.7+deb11u4.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQKmBAEBCgCQFiEERkRAmAjBceBVMd3uBUy48xNDz0QFAmQ688lfFIAAAAAALgAo
aXNzdWVyLWZwckBub3RhdGlvbnMub3BlbnBncC5maWZ0aGhvcnNlbWFuLm5ldDQ2
NDQ0MDk4MDhDMTcxRTA1NTMxRERFRTA1NENCOEYzMTM0M0NGNDQSHGNhcm5pbEBk
ZWJpYW4ub3JnAAoJEAVMuPMTQ89E6NgP/1y3IjNwxDxbfIoCQ6r2/xC2vKPBa5wp
dCj5k3fIBGHPcuz7u0xiCvTwpYM9q7Z6M+aXpAHA8EKrnz/VzXg4QXOcyi/JeGze
nZVj8LSRMuR4NL0LmvxRTvt9xeNz9ZSgRwT0DDQ75ap5iyAMdQrTkKaKjPr0zyme
DoVc87wzQ9Ci25wtnCLr766uMi6/2Rsb0qrKd7qw8RRIbc2uWNgSoJxRCKe8E63n
qp9ZLR3vHkbk1NgcTvxiXRrRyb43X5BgIz2jBcK4DR781bDVnNUizp4sUXHiRLIg
Ad1OvM/T2FYJ2g73gYxdMGalBqV0fUe7kKfbZbit95s0m8YHW10iuMdTEH2Vf1DV
7vFOMK3PBZey0xYdW7hOV+1pnnPp3YCI6jFgYQllkhGumBj+87o1Lpo+WzM1oIfg
6eBwLJADh5zvlFAAvHfKEzA+nT21o2ZT7Ev+rxTn91lfFwoLVFxd6EQb05ZQqVch
jHhPNzZ4AzXyvstR9eDMe2E9IReFfzQR53Fnpx6DG4eSnMvlnxJPY/5E4PE2VS2G
i/YUeHT8k+mLbM82AZqWd2YkjOt65BmhIGWZWNSXvyYcKwPs55MtHbq853VIw1U5
NgrLu9hLegoPj6NnlX6L6EN3vKJljbLedAsRqztJcbdg2sIt9B5vBVDmR5B62pmW
1pAxXYUg6ZvC
=XJSR
-----END PGP SIGNATURE-----
