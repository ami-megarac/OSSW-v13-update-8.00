-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: nginx
Binary: nginx, nginx-doc, nginx-common, nginx-core, nginx-full, nginx-light, nginx-extras, libnginx-mod-http-geoip, libnginx-mod-http-geoip2, libnginx-mod-http-image-filter, libnginx-mod-http-xslt-filter, libnginx-mod-mail, libnginx-mod-stream, libnginx-mod-stream-geoip, libnginx-mod-stream-geoip2, libnginx-mod-http-perl, libnginx-mod-http-auth-pam, libnginx-mod-http-lua, libnginx-mod-http-ndk, libnginx-mod-nchan, libnginx-mod-http-echo, libnginx-mod-http-upstream-fair, libnginx-mod-http-headers-more-filter, libnginx-mod-http-cache-purge, libnginx-mod-http-fancyindex, libnginx-mod-http-uploadprogress, libnginx-mod-http-subs-filter, libnginx-mod-http-dav-ext, libnginx-mod-rtmp
Architecture: any all
Version: 1.18.0-6.1+deb11u3
Maintainer: Debian Nginx Maintainers <pkg-nginx-maintainers@alioth-lists.debian.net>
Uploaders: Christos Trochalakis <ctrochalakis@debian.org>, Ondřej Nový <onovy@debian.org>,
Homepage: https://nginx.net
Standards-Version: 4.5.0
Vcs-Browser: https://salsa.debian.org/nginx-team/nginx
Vcs-Git: https://salsa.debian.org/nginx-team/nginx.git
Testsuite: autopkgtest
Testsuite-Triggers: curl, ssl-cert
Build-Depends: debhelper-compat (= 13), dpkg-dev (>= 1.15.5), libexpat-dev, libgd-dev, libgeoip-dev, libhiredis-dev, liblua5.1-0-dev [!i386 !amd64 !kfreebsd-i386 !armel !armhf !powerpc !powerpcspe !mips !mipsel], libluajit-5.1-dev [i386 amd64 kfreebsd-i386 armel armhf powerpc powerpcspe mips mipsel], libmaxminddb-dev, libmhash-dev, libpam0g-dev, libpcre3-dev, libperl-dev, libssl-dev, libxslt1-dev, po-debconf, quilt, zlib1g-dev
Package-List:
 libnginx-mod-http-auth-pam deb httpd optional arch=any
 libnginx-mod-http-cache-purge deb httpd optional arch=any
 libnginx-mod-http-dav-ext deb httpd optional arch=any
 libnginx-mod-http-echo deb httpd optional arch=any
 libnginx-mod-http-fancyindex deb httpd optional arch=any
 libnginx-mod-http-geoip deb httpd optional arch=any
 libnginx-mod-http-geoip2 deb httpd optional arch=any
 libnginx-mod-http-headers-more-filter deb httpd optional arch=any
 libnginx-mod-http-image-filter deb httpd optional arch=any
 libnginx-mod-http-lua deb httpd optional arch=any
 libnginx-mod-http-ndk deb httpd optional arch=any
 libnginx-mod-http-perl deb httpd optional arch=any
 libnginx-mod-http-subs-filter deb httpd optional arch=any
 libnginx-mod-http-uploadprogress deb httpd optional arch=any
 libnginx-mod-http-upstream-fair deb httpd optional arch=any
 libnginx-mod-http-xslt-filter deb httpd optional arch=any
 libnginx-mod-mail deb httpd optional arch=any
 libnginx-mod-nchan deb httpd optional arch=any
 libnginx-mod-rtmp deb httpd optional arch=any
 libnginx-mod-stream deb httpd optional arch=any
 libnginx-mod-stream-geoip deb httpd optional arch=any
 libnginx-mod-stream-geoip2 deb httpd optional arch=any
 nginx deb httpd optional arch=all
 nginx-common deb httpd optional arch=all
 nginx-core deb httpd optional arch=any
 nginx-doc deb doc optional arch=all
 nginx-extras deb httpd optional arch=any
 nginx-full deb httpd optional arch=all
 nginx-light deb httpd optional arch=any
Checksums-Sha1:
 47b2c5ccd12e2a7088b03d629ff6b9ab18215180 1039530 nginx_1.18.0.orig.tar.gz
 57512088d69fb5c4ca97582410fb4cd9fabb1346 455 nginx_1.18.0.orig.tar.gz.asc
 437228defebd97d575d5f7756e425b1a0c5dbb18 1041528 nginx_1.18.0-6.1+deb11u3.debian.tar.xz
Checksums-Sha256:
 4c373e7ab5bf91d34a4f11a0c9496561061ba5eee6020db272a17a7228d35f99 1039530 nginx_1.18.0.orig.tar.gz
 71083832dbcedb7cbbe3b849805a6f87dfe7a3b7c3de7997b8c12ffc716f1014 455 nginx_1.18.0.orig.tar.gz.asc
 7da3a9b9a8b94544a84106180fae42c52f7104bce772a20b5c107bd14ffb4fc4 1041528 nginx_1.18.0-6.1+deb11u3.debian.tar.xz
Files:
 b2d33d24d89b8b1f87ff5d251aa27eb8 1039530 nginx_1.18.0.orig.tar.gz
 3f0d891d096a9f169ef9a183e6a76b1f 455 nginx_1.18.0.orig.tar.gz.asc
 23483eba005461e13fd05db42936708e 1041528 nginx_1.18.0-6.1+deb11u3.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCgAdFiEEtuYvPRKsOElcDakFEMKTtsN8TjYFAmNuS2EACgkQEMKTtsN8
Tjbr5A/+I37ss6qk2mHWOq2vRL6/PbOT0FKGN4cYceBX+CBZACkJtVRfLZOil4OC
CQNsL7uW4XrWlp19ywdDIJ3V8jdzk+peGn80RUPcgWsySYARnlbL2iYYjKAgvCiy
+07/v5vAnUxpu7SflCbeLry7XGXGMc+ExUp+GikgqQorESu08fnYTXVmpaLirv6t
VlVzTlYxAJAR+ehTqA+z01eyK739s3t5kQm3EAmdStErG2S0eu7aq0QsPO9rskLX
suys4SC+UPoOPnX9zmivJvNhQxVT4c3LDnauN9c575lD/TtH7XTzXHf6eVt+2iJB
ThpzjQQzlU9M/+nm/d/3798QglU2dfyDEOnDz9tBGfUQ9Lj0dsNqnTl9DqOtXgfF
edzfkBQhtWYzl43YJSSAnSj5cOVT3Vl67rlyXyCtYntCh981Un7IJLjBfeQg8FR5
/d46G9Esli9lRlhEJaBqk00bZ/xcpvVDZ5rEf/+3xJxDXMmDTUDq/1jiIGIBqAwO
xw/wKLIrrdJ9qrT5pBkbzKfTu5se0Rv1tpB5OIHijL9ahEsyGoWyl8XPOZ3kHoQh
WRtOMvSmM+TfBfyfMXv79Dfjqe37iMwWIPSUH9Y3ghETTAYdbsBRvT4iqIpIOA4T
k8Xb8/yFCnt0ko3wXrple1q1Np7vU1tMXv4uC2ZsV0Z98i7lKNM=
=2goo
-----END PGP SIGNATURE-----
