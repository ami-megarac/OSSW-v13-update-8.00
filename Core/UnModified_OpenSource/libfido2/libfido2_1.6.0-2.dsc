-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: libfido2
Binary: libfido2-1, libfido2-dev, libfido2-doc, fido2-tools
Architecture: any all
Version: 1.6.0-2
Maintainer: Debian Authentication Maintainers <pkg-auth-maintainers@lists.alioth.debian.org>
Uploaders: Colin Watson <cjwatson@debian.org>, nicoo <nicoo@debian.org>
Homepage: https://developers.yubico.com/libfido2/
Standards-Version: 4.5.0
Vcs-Browser: https://salsa.debian.org/auth-team/libfido2
Vcs-Git: https://salsa.debian.org/auth-team/libfido2.git
Build-Depends: debhelper-compat (= 12), pkg-config, cmake (>= 3.14), mandoc, libcbor-dev, libssl-dev, libudev-dev
Package-List:
 fido2-tools deb utils optional arch=any
 libfido2-1 deb libs optional arch=any
 libfido2-dev deb libdevel optional arch=any
 libfido2-doc deb doc optional arch=all
Checksums-Sha1:
 6f57e9a0554b458f3840f963025dc99340e1ed43 413904 libfido2_1.6.0.orig.tar.gz
 97dd36bda859b0601eb0ffded491974d043e5c35 488 libfido2_1.6.0.orig.tar.gz.asc
 78b1c6bd3e75a52fcf24c584861c4c5691c95209 72700 libfido2_1.6.0-2.debian.tar.xz
Checksums-Sha256:
 6aed47aafd22be49c38f9281fb88ccd08c98678d9b8c39cdc87d1bb3ea2c63e4 413904 libfido2_1.6.0.orig.tar.gz
 3340543e15edc67a38850be6777448db33245b5f9f686903d24563a2c7c0d969 488 libfido2_1.6.0.orig.tar.gz.asc
 a08adb41d42e64206c5f70f1f2c1510debb37256fe3b92681a0bdc4a73e6d8d2 72700 libfido2_1.6.0-2.debian.tar.xz
Files:
 cdf2acf529bb47705c6ed8fda2a806f2 413904 libfido2_1.6.0.orig.tar.gz
 8484ff1b482ea87adb99cf8fe15881f2 488 libfido2_1.6.0.orig.tar.gz.asc
 dbdb64688ca26f6ef7220d7a7c07ee44 72700 libfido2_1.6.0-2.debian.tar.xz
Dgit: 50362cb2e80c3699b0a7561991fc5d4031c13cc7 debian archive/debian/1.6.0-2 https://git.dgit.debian.org/libfido2

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCAAdFiEErApP8SYRtvzPAcEROTWH2X2GUAsFAmADbNUACgkQOTWH2X2G
UAtuFw/9HA0rCVU4qyg56vBAKgTL2tlFauAiXH+BBq5wdm/w7D85C0koZTQsxAvR
oq1/sd7VuK4fSo1vczalS79/Qdhq1CVX5okmdoUJS4RKo7bNKTYdZD12Bor6P4Pz
1WzDFzpsjI2omb7tldFJUhxhRqhWHIi/H2g4eGVwlSJSXyviU89Z1HcX0jImPrhW
7oo3Kf+HBwzNUpIne5jNjHju2zT1e/cDxAvGmasEsflrhCYAGsxnU8rgKV/NOtr4
yQStN3o4C4otCm5xc72IlmJM4R7bA2xZJk3lSoswtgpssif20FuTmaXJK24EO7bl
iI+CRqKdPAsLadLrVzZWxyGunsz4BshAg18hSVV56XAQuuluHXDBcKdKZ5pKgwpY
8FkKA/pT+zg/JODfG+NRVEXADv2rSF/bYuAioigD94OHxtCZryJqlpSq15kqhD42
9+2Clx3iL8JPjpy4RZNP4DWWMDPgureVvqBsb8rCvcwexICel78vPwl7rhSMOF/p
p1rlWRx28fp1qw6Xrchmjky+4kK5ZdU0EAF96poDXNRpoM7seJeUQ9bS4FlEvr9m
2CrDwIOOtX80HSxorPEMBMlDWv7m22Smk9VTA0RT0NdnmW6P10sEScJs4pngCTXe
tcpw/rBvZFPHYybMIg4IX2nuKNE2erxiRcrpZDZUyUR7g/bSkek=
=MUgg
-----END PGP SIGNATURE-----
