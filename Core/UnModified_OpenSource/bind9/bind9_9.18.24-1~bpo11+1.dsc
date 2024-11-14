-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: bind9
Binary: bind9, bind9utils, bind9-utils, bind9-doc, bind9-host, bind9-libs, bind9-dev, dnsutils, bind9-dnsutils
Architecture: any all
Version: 1:9.18.24-1~bpo11+1
Maintainer: Debian DNS Team <team+dns@tracker.debian.org>
Uploaders: Ondřej Surý <ondrej@debian.org>, Bernhard Schmidt <berni@debian.org>
Homepage: https://www.isc.org/downloads/bind/
Standards-Version: 4.6.2
Vcs-Browser: https://salsa.debian.org/dns-team/bind9
Vcs-Git: https://salsa.debian.org/dns-team/bind9.git -b debian/9.18
Testsuite: autopkgtest
Build-Depends: bison, debhelper-compat (= 12), dh-apparmor, dh-exec, libcap2-dev [linux-any], libcmocka-dev, libdb-dev, libedit-dev, libfstrm-dev, libidn2-dev, libjemalloc-dev, libjson-c-dev, libkrb5-dev, liblmdb-dev, libltdl-dev, libmaxminddb-dev (>= 1.3.0), libnghttp2-dev, libprotobuf-c-dev, libssl-dev, libsystemd-dev [linux-any], libtool, libuv1-dev (>= 1.40.0), libxml2-dev, pkg-config, protobuf-c-compiler, python3, zlib1g-dev
Build-Depends-Indep: fonts-freefont-otf, latexmk, python3-sphinx, python3-sphinx-rtd-theme, texlive-fonts-extra, texlive-latex-recommended, texlive-xetex, xindy
Package-List:
 bind9 deb net optional arch=any
 bind9-dev deb devel optional arch=any
 bind9-dnsutils deb net standard arch=any
 bind9-doc deb doc optional arch=all
 bind9-host deb net standard arch=any
 bind9-libs deb libs standard arch=any
 bind9-utils deb net optional arch=any
 bind9utils deb oldlibs optional arch=all
 dnsutils deb oldlibs optional arch=all
Checksums-Sha1:
 e5bfeb64e3d118c5b4e21ae615f2b9c3ea5339ff 5515528 bind9_9.18.24.orig.tar.xz
 a0c8cb98326345552e993c5a1d720ae39490efea 60832 bind9_9.18.24-1~bpo11+1.debian.tar.xz
Checksums-Sha256:
 709d73023c9115ddad3bab65b6c8c79a590196d0d114f5d0ca2533dbd52ddf66 5515528 bind9_9.18.24.orig.tar.xz
 b42069ab340d2c9a3b8e6288f8081ffa8299f89ffad955e258f8f638b74e7200 60832 bind9_9.18.24-1~bpo11+1.debian.tar.xz
Files:
 c791cb32069dbfb6d555ee682309ab09 5515528 bind9_9.18.24.orig.tar.xz
 c37e0137a7a5308e9820cbbff4855027 60832 bind9_9.18.24-1~bpo11+1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQJFBAEBCgAvFiEE1uAexRal3873GVbTd1B55bhQvJMFAmXNMl8RHGJlcm5pQGRl
Ymlhbi5vcmcACgkQd1B55bhQvJPV9A//XT6Q7CKgbIwRsNkbhX5gy6bUKBkgVnZF
STaJXISJTwWU7kx0p9/Ljnd3ioLEBR81V3JMXo5VxHesmxmsvxqfEjGvvl2qSXkw
P3HmVO1PONPFI+MOoy+TEAirABpQ1gchDCgcuyB1+wUBdqycsQFqb5mMH20xpyRD
AYfj3Ii5HVGJc+ZNy+BHvQzSxURpn1yAeSm39wmJFaTCUjY54d5PiclSfJRku371
j+Xq8QUbyHgKD4lThm4jPp+WoD7y9LZPFFLBMKORRVldojEVnuUmNMc9jONby8o1
NuDSklYW5SokyksnTW28a4px4BXCeqo0OBygeDXYr/FebyTmeXTZNJ28z/RmDZo6
Jysrw4/OdEJ4xp4g5VjGT+64Hnpj+6MxjG2Nlj6eZMu+O9lwBw+/YxEnncgQQJms
IecxIQHnSCuPtYVQOECv+iSMQ/tzgLy4INaUfpii0bGUsnU06JIP1I9buWxA9Xvg
fHcXv8sAK5yHsLTA88CLIN1Kn//5VimtmktpicGh6FxazBJBaoN7zwKWqSPccixW
9ULokBhHLjuybbLUihOew/rrGD8fh2aLiHq0+BG+6tcNZ3qhqPWlcrxdDynqgSPJ
I8ZeDttXBD+TXmjHUMEPf/2O+ePIehODPecJZwJ2jlTGyZAIQuasczgE9pjZ3HK6
SPUxuDa7FDQ=
=VFti
-----END PGP SIGNATURE-----
