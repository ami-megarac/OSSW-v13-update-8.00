-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: freetype
Binary: libfreetype6, libfreetype-dev, libfreetype6-dev, freetype2-demos, freetype2-doc, libfreetype6-udeb
Architecture: any all
Version: 2.10.4+dfsg-1+deb11u1
Maintainer: Hugh McMaster <hugh.mcmaster@outlook.com>
Uploaders: Anthony Fok <foka@debian.org>, Keith Packard <keithp@keithp.com>
Homepage: https://www.freetype.org
Standards-Version: 4.5.1
Vcs-Browser: https://salsa.debian.org/debian/freetype
Vcs-Git: https://salsa.debian.org/debian/freetype.git
Testsuite: autopkgtest
Testsuite-Triggers: build-essential, pkg-config
Build-Depends: debhelper-compat (= 13), autoconf, bzip2, gettext, libbrotli-dev, libjs-jquery, libpng-dev, libtool, libx11-dev <!stage1>, zlib1g-dev | libz-dev, pkg-config, x11proto-core-dev <!stage1>
Package-List:
 freetype2-demos deb utils optional arch=any profile=!stage1
 freetype2-doc deb doc optional arch=all
 libfreetype-dev deb libdevel optional arch=any
 libfreetype6 deb libs optional arch=any
 libfreetype6-dev deb oldlibs optional arch=any
 libfreetype6-udeb udeb debian-installer optional arch=any
Checksums-Sha1:
 d16eef3cb775532995db5826c4f4f6dbe883cc5b 236712 freetype_2.10.4+dfsg.orig-ft2demos.tar.xz
 9ee8535dc25a1112ec49ec24c1e342a01a08b1be 195 freetype_2.10.4+dfsg.orig-ft2demos.tar.xz.asc
 9c86a3225cabc659914095c5f97b4844001bb733 2079084 freetype_2.10.4+dfsg.orig-ft2docs.tar.xz
 a5a931498eae366af724962ef2a1d9b4923f674e 195 freetype_2.10.4+dfsg.orig-ft2docs.tar.xz.asc
 b9f4a2de4d5702910b1df7247c68a977c407e3f9 2259340 freetype_2.10.4+dfsg.orig.tar.xz
 c15cdadbe91eb16627811264ef772495d77da163 117572 freetype_2.10.4+dfsg-1+deb11u1.debian.tar.xz
Checksums-Sha256:
 3f873ebe4fb387da3859149459f9be95320ce1fd56b50f8fdb9d2a8492887083 236712 freetype_2.10.4+dfsg.orig-ft2demos.tar.xz
 38d5b9a5aa11ecf8c6d4c983ef48b3ce2288fdf93d44719df2598b9d415c8061 195 freetype_2.10.4+dfsg.orig-ft2demos.tar.xz.asc
 cca1c19d1efa911bb685d919b5b0fe1279b0699bf8eb6a3d3bf9f02784758212 2079084 freetype_2.10.4+dfsg.orig-ft2docs.tar.xz
 29fca9ff0e1cdc57ad5707b17f629eeaa216eb334f6082f1b05fb0fe35e14ff3 195 freetype_2.10.4+dfsg.orig-ft2docs.tar.xz.asc
 db0c0938b3b75cf314775baa75198098e41583b3aaa4804b454f183ce45120a9 2259340 freetype_2.10.4+dfsg.orig.tar.xz
 f8845df5a24d816934ef30b46b8561072df3f7973ea65654674d69810aab9c3b 117572 freetype_2.10.4+dfsg-1+deb11u1.debian.tar.xz
Files:
 6e0772fe3b0f945bb6a4140e14c20eb2 236712 freetype_2.10.4+dfsg.orig-ft2demos.tar.xz
 a860c686671cde927e5eb79ecc87085a 195 freetype_2.10.4+dfsg.orig-ft2demos.tar.xz.asc
 a389bc390ca8009c758094e71a8fb18d 2079084 freetype_2.10.4+dfsg.orig-ft2docs.tar.xz
 13b191ee4f0b2f62e94d09a2e88503b9 195 freetype_2.10.4+dfsg.orig-ft2docs.tar.xz.asc
 35e2d6bd7134a9aeba82ea2149272128 2259340 freetype_2.10.4+dfsg.orig.tar.xz
 2d245a36a612a09abe52ebfdaabbaf13 117572 freetype_2.10.4+dfsg-1+deb11u1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQJOBAEBCgA4FiEEOiCBPKV5RoaMUVIRWsYQdMXoG8QFAmKocrIaHGh1Z2gubWNt
YXN0ZXJAb3V0bG9vay5jb20ACgkQWsYQdMXoG8RVcBAAy9T/GR4pVa2di6nfLbUt
1bMclH03pxgyqgl+le/oAQjnC1G3SmFeKYDLMomDI7jh/zG6BDJKLi+Lo4UI5Y9B
299AIJ32lAQBU/V+cGF+vc14OSarcu0xALAul+Xl1Zd9TkzRoR+HNsXi2giXPW/G
NquoapbbdNtzRWOwJjX5nWtx+KuajrppdT2TTs2xO11GmyU9UyWR3eqWpwUPnusv
w68mwmcTNw7v9j5YU0e4lT8YVHG+jhpBHmXtaVWcqmj95RsjtQyQz/Ew47TNwse3
YuNX7RKOYryisCkmZjjDywwKGbZh+0WNMhDObqmiokAntRpc8Xi8Xxm5/aXnpFRA
MBgDb885BvcmXfw6AplAG1HNgTxl+Dw9Fmij34arXwwSJC5SzngFGN481TfB6eGo
D3C8LX+r5A9osDDrJZk6r6n/g0Jy/QrzgKPn5VQ6S47SfgRWHOIE/apTnUrprUjJ
/M3y5EEa0QTXKpHtEz2UUVJ+RsuO0GG86vBsgVJnLOUcN9B3zrHAUqi4uxZdRMFK
VFWqND0G6BTdRPnnN9LaYJ82wNjSirK8RkUfIRIB5XPIvDUoIowGFqORCkVAT34E
/XUL9Plq8n6CNuSUlzRxAs2vZ0l+Pyawi+6YvK7zhuK81twiryMcpEBmXliCgVds
XJMI3JHOffGP4XaXDxlaFV8=
=Aidh
-----END PGP SIGNATURE-----
