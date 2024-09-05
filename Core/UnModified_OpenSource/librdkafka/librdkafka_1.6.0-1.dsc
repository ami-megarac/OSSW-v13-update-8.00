-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: librdkafka
Binary: librdkafka1, librdkafka++1, librdkafka-dev
Architecture: any
Version: 1.6.0-1
Maintainer: Faidon Liambotis <paravoid@debian.org>
Uploaders:  Christos Trochalakis <ctrochalakis@debian.org>, ChangZhuo Chen (陳昌倬) <czchen@debian.org>,
Homepage: https://github.com/edenhill/librdkafka
Standards-Version: 4.5.1
Vcs-Browser: https://salsa.debian.org/kafka-team/librdkafka
Vcs-Git: https://salsa.debian.org/kafka-team/librdkafka.git
Testsuite: autopkgtest
Testsuite-Triggers: build-essential, pkg-config
Build-Depends: debhelper-compat (= 13), liblz4-dev, libsasl2-dev, libssl-dev, libzstd-dev, python3, rapidjson-dev, zlib1g-dev
Package-List:
 librdkafka++1 deb libs optional arch=any
 librdkafka-dev deb libdevel optional arch=any
 librdkafka1 deb libs optional arch=any
Checksums-Sha1:
 8dc42ccc094048b02bd34fe44c652965dedc6b24 2909665 librdkafka_1.6.0.orig.tar.gz
 6e248bcf77487e06ab000572871a7caa05b5b398 10640 librdkafka_1.6.0-1.debian.tar.xz
Checksums-Sha256:
 3130cbd391ef683dc9acf9f83fe82ff93b8730a1a34d0518e93c250929be9f6b 2909665 librdkafka_1.6.0.orig.tar.gz
 25dc07b79bb1394a6e8f13573ac72427e361a7bfb8ca82d1459ac2bf91672ea7 10640 librdkafka_1.6.0-1.debian.tar.xz
Files:
 464166edf1fa5aa3dc327d7e6333d5f7 2909665 librdkafka_1.6.0.orig.tar.gz
 7bb9879e320e7de3d2931dc9110dd1a7 10640 librdkafka_1.6.0-1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCgAdFiEEqVksUhy5BAd9ZZgAnQteWx7sjw4FAmAgt2oACgkQnQteWx7s
jw6EOxAAlL7ZyzMojl8QCO/9xIDN2i+JBrVRESCkTfw25BMZBlgijtTVlwzXDGxe
v8U+Q9Vqad3LtA1DJ8s0sogTJjPxhjEXnfzi5y2pJld29pUCx1w7cCDkDea38fmU
93cZ/QGb5Z3ZhYRW6GMaaUhdu44EQvlERutqECd8CBaU8GtOvJ8LH5pNtW+1GMSf
QEZfge8SgvrigLYizTVYAGmDGNY38FiIHBqP751nAVvjlo6RcUIa15uGpMXJPzMz
GtavZ5Mioh6EefqdJCceQyv20dF0V2eejLWj9auFrNrvmrZzJYZVRQ5HxkML3dH9
NJUNPFI5Sm2MbiIPsO7lPb1Y7SHbpxERdXrw8x9FRlQDMka4w7WEF2byr5n0xXGd
ITAfpPPm9eSO+7hwiXnaHxZW5Kiu/thJ6DN1PdYzha/O0Hf/XTQwJttPDtzHr/Hm
Iuet5cQQWNywEByQrAzdMnHNT5fSfmBTtl906wlhNfqwlwr130cP+dmeFio6MCQw
5tchDyCqs0JGDb1/YFeQ18c3V8PQoE3BBeJsYBCYRaHPInqcDiqXoiE4KJNyU2Qv
JjP8uCqqvOmgmffswJThbfqibcFXMP7GkN/ET2xp61xupUPv/uNV5db/SCgIvbgm
y+3FnU+s69uggG1V2bVQgU+D1a5JMTOitX2cpK0pvLW4SVoLcz0=
=vZpE
-----END PGP SIGNATURE-----
