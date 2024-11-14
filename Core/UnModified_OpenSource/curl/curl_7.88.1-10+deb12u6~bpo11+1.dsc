-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: curl
Binary: curl, libcurl4, libcurl3-gnutls, libcurl3-nss, libcurl4-openssl-dev, libcurl4-gnutls-dev, libcurl4-nss-dev, libcurl4-doc
Architecture: any all
Version: 7.88.1-10+deb12u6~bpo11+1
Maintainer: Alessandro Ghedini <ghedo@debian.org>
Uploaders: Samuel Henrique <samueloph@debian.org>, Sergio Durigan Junior <sergiodj@debian.org>
Homepage: https://curl.se/
Standards-Version: 4.6.2
Vcs-Browser: https://salsa.debian.org/debian/curl
Vcs-Git: https://salsa.debian.org/debian/curl.git
Testsuite: autopkgtest
Testsuite-Triggers: @builddeps@, gcc, libc-dev, libcurl-dev, libldap-dev, pkgconf, slapd
Build-Depends: debhelper-compat (= 13), autoconf, automake, ca-certificates, dh-exec, groff-base, libbrotli-dev, libgnutls28-dev, libidn2-dev, libkrb5-dev, libldap2-dev, libnghttp2-dev, libnss3-dev, libpsl-dev, librtmp-dev, libssh-dev, libssh2-1-dev, libssl-dev, libtool, libzstd-dev, locales-all <!nocheck>, openssh-server <!nocheck>, python3:native <!nocheck>, python3-impacket <!nocheck>, gnutls-bin [amd64 arm64 armel armhf i386 mips64el mipsel s390x powerpc ppc64 riscv64] <!nocheck>, quilt, stunnel4 <!nocheck>, zlib1g-dev, nss-plugin-pem <!nocheck>
Build-Conflicts: autoconf2.13, automake1.4
Package-List:
 curl deb web optional arch=any
 libcurl3-gnutls deb libs optional arch=any
 libcurl3-nss deb libs optional arch=any
 libcurl4 deb libs optional arch=any
 libcurl4-doc deb doc optional arch=all
 libcurl4-gnutls-dev deb libdevel optional arch=any
 libcurl4-nss-dev deb libdevel optional arch=any
 libcurl4-openssl-dev deb libdevel optional arch=any
Checksums-Sha1:
 6ae5229c36badb822641bb14958e7d227c57611d 4343562 curl_7.88.1.orig.tar.gz
 9222035242431a3ef31d33a2ca3d881bcf4572fe 488 curl_7.88.1.orig.tar.gz.asc
 1bb5e4fa97fd7704ed6107a4dd32135f3e132805 68484 curl_7.88.1-10+deb12u6~bpo11+1.debian.tar.xz
Checksums-Sha256:
 cdb38b72e36bc5d33d5b8810f8018ece1baa29a8f215b4495e495ded82bbf3c7 4343562 curl_7.88.1.orig.tar.gz
 7a5a55d7123149a1b357f298cf895bd0a601e3a2807005ef6c95f3752803485f 488 curl_7.88.1.orig.tar.gz.asc
 ff9db6c5abe0f82bdd4c861b16915b9fbdd26f61b63ad9e02632a10cf5f4def2 68484 curl_7.88.1-10+deb12u6~bpo11+1.debian.tar.xz
Files:
 1211d641ae670cebce361ab6a7c6acff 4343562 curl_7.88.1.orig.tar.gz
 08b846caa2ce56ccb4b4caa268b30dc2 488 curl_7.88.1.orig.tar.gz.asc
 02f1c55b73e0c648b399fce525dfd8d7 68484 curl_7.88.1-10+deb12u6~bpo11+1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCgAdFiEEBdtqg34QX0sdAsVfu6n6rcz7RwcFAmY/U3EACgkQu6n6rcz7
RwfzIw//ZntrTnUQSD4uH593DUhE/KAMQXVCjzs17KdEAJvdf8qemYoG1E/aW8Ic
18y8GfUOnisf4BLJQsOE2kVIHXqAvPR9LIohi5MgIYoX0NIAj91frK15l8g3yIgl
8UBkadPGtJM/iMLPCvkkd01nTJUBPxflEzyBLkcWrVm7t2uU4IBPyGHre2VpNlNj
bLq4zxQrAAuQyBQjTpi07d/TZtKyqlQuc7uhSz5uBAtrNLLcoI/nnt8u4MS7XeII
I5U0CTaagH/w8aDTLSqQlTFRUTtwCrMPVqyF0JJq7kl3OgAuZJnsLRCcJYMIMpJO
HGuZf33n7Eh2IIizsi23LVlryHnmkJ3pR74QFLchSeAiEI7sn7b98Ems4ftQYj2p
45XwBLPwtqwsxUNWvJKPgmVEHHuVQlkiksPgUSlyhIdTYhk7hxXYYihpTolo2j3y
hOdv7xour2gxuIYtT6uxWdJBELbU8oZu4Emydh4+/vZLUD1uDEreGuPPiCcx+gtZ
6AvDBQUeF+9kIoAT5MbkxgbjQyYhQo47ihWTnoiVhsI/8pwU4I31DBhotJfuTQcf
J1j0LHj78bq5ZhlpyeNRMESXRAaNsygfU/bVaI6Pa27e88Jvrzzz++uXgkLZBD2r
tSGWTp9Gyf9mZRlfBuSovcPkGBLeXE5yTYnOlzbZ7qqY/GJdgWQ=
=XJZn
-----END PGP SIGNATURE-----
