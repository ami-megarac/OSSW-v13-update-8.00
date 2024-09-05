-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: cups
Binary: libcups2, cups, cups-bsd, cups-client, cups-common, cups-core-drivers, cups-daemon, cups-ipp-utils, cups-ppdc, cups-server-common, libcups2-dev, libcupsimage2, libcupsimage2-dev
Architecture: any all
Version: 2.3.3op2-3+deb11u6
Maintainer: Debian Printing Team <debian-printing@lists.debian.org>
Uploaders:  Didier Raboud <odyx@debian.org>, Till Kamppeter <till.kamppeter@gmail.com>,
Homepage: https://github.com/OpenPrinting/cups/
Standards-Version: 4.5.1
Vcs-Browser: https://salsa.debian.org/printing-team/cups
Vcs-Git: https://salsa.debian.org/printing-team/cups.git
Testsuite: autopkgtest
Build-Depends: autoconf, automake, debhelper-compat (= 13), dh-strip-nondeterminism, libavahi-client-dev, libavahi-common-dev, libdbus-1-dev, libgnutls28-dev, libkrb5-dev, libpam0g-dev, libpaper-dev, libsystemd-dev [linux-any], libtool, libusb-1.0-0-dev [!hurd-any], patch, pkg-config, po-debconf, po4a, zlib1g-dev
Build-Depends-Arch: dh-apparmor
Build-Conflicts: libgmp-dev (<< 2:6)
Package-List:
 cups deb net optional arch=any
 cups-bsd deb net optional arch=any
 cups-client deb net optional arch=any
 cups-common deb net optional arch=all
 cups-core-drivers deb net optional arch=any
 cups-daemon deb net optional arch=any
 cups-ipp-utils deb net optional arch=any
 cups-ppdc deb utils optional arch=any
 cups-server-common deb net optional arch=all
 libcups2 deb libs optional arch=any
 libcups2-dev deb libdevel optional arch=any
 libcupsimage2 deb libs optional arch=any
 libcupsimage2-dev deb libdevel optional arch=any
Checksums-Sha1:
 23108e0f6ca7d8caa1a6a6224f5322e21ba0a27d 7993205 cups_2.3.3op2.orig.tar.gz
 84ef20cb866516c32579e9499186a44379143d3f 833 cups_2.3.3op2.orig.tar.gz.asc
 e431d18cd5e4ce7fdf36ab0027886a5d9dda43fe 348436 cups_2.3.3op2-3+deb11u6.debian.tar.xz
Checksums-Sha256:
 deb3575bbe79c0ae963402787f265bfcf8d804a71fc2c94318a74efec86f96df 7993205 cups_2.3.3op2.orig.tar.gz
 ac9e7e3415d727e9882cf0f65ee38459cbcd602cb49cf392e0a65a172fe3bdaf 833 cups_2.3.3op2.orig.tar.gz.asc
 efbc81277010c49ed1830f2e2a40e147f3ecb082e31fb86b428bc6f573f699c8 348436 cups_2.3.3op2-3+deb11u6.debian.tar.xz
Files:
 524266752cf243c968921547b18ee594 7993205 cups_2.3.3op2.orig.tar.gz
 71e38a6689d78e15e00eed9426c370f0 833 cups_2.3.3op2.orig.tar.gz.asc
 9fba994f9ab983d68ed812de139948f5 348436 cups_2.3.3op2-3+deb11u6.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQKnBAEBCgCRFiEEYgH7/9u94Hgi6ruWlvysDTh7WEcFAmUe0o1fFIAAAAAALgAo
aXNzdWVyLWZwckBub3RhdGlvbnMub3BlbnBncC5maWZ0aGhvcnNlbWFuLm5ldDYy
MDFGQkZGREJCREUwNzgyMkVBQkI5Njk2RkNBQzBEMzg3QjU4NDcTHGRlYmlhbkBh
bHRlaG9sei5kZQAKCRCW/KwNOHtYRyL5D/4xCQe+PC4QlQaIKq9gSZYUuCgApP97
AoTdq/QC4hs5HB+N8ULOjZEI2i03pHzKsGfTmkCCoCE3c3YGRU8FQMpGLi0lKFzO
0BNGgS7KziCi3u8y7cIlWaTMphysEJaZJ2eW4sC4Ll3AecjXq6KUatm3nBAPok6N
pUasANQEWof6BroDK43STeMo2feGlX1080hn4wE1yfrWCaDDHLQKgGhvN+vkdfef
4/Knoqq9KWM8+BC0d2asM7iK0zjy9qzGodb644Dg+ifH00g8OlUkVQDF8D/U0qY9
8cXvIuKv60K8aTl6AtlJRJBeBQ7bjdveASnSlszlB+FLEq7doJYeCp/dK/1oJInM
KELwepSWm7e23oXcDCcfglEqFbmx5M0iEBCu3spRPPXoXZChyfLEwAiSv+bCp6wF
KELDcvlHbeBRVoC3xMzQvFoCvnE2++Sn0xijsoyZmseoHQKDedr3AbyWmsoqT5Ky
Hd1UlhmgCoaTSHIiboGOOhhOf6RfXr0alMAaqhewhlzAJaV8KvXHWjPpKCVlp9+7
HO4sPGeU2BHh/IJQWpzh8+CnWLpx/6se7+nnGxqY3zn1dzMEmutgYQ3YL5wx9WP3
RGOwgFuDHadXCRcisrPPWTqItRUryAxYMjeJK/NuwTnQ5WUUa6EZqQjj4ARBoUZa
BEJ4/zdGx8YaGw==
=MX4a
-----END PGP SIGNATURE-----
