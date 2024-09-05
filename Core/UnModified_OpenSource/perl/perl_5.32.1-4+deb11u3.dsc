-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: perl
Binary: perl-base, perl-doc, perl-debug, libperl5.32, libperl-dev, perl-modules-5.32, perl
Architecture: any all
Version: 5.32.1-4+deb11u3
Maintainer: Niko Tyni <ntyni@debian.org>
Uploaders: Dominic Hargreaves <dom@earth.li>
Homepage: http://dev.perl.org/perl5/
Standards-Version: 3.9.8
Vcs-Browser: https://salsa.debian.org/perl-team/interpreter/perl
Vcs-Git: https://salsa.debian.org/perl-team/interpreter/perl.git
Testsuite: autopkgtest
Testsuite-Triggers: build-essential, cpio, db-util, dist, dpkg-dev, gdbmtool, libapt-pkg-perl, libdpkg-perl, pkg-perl-autopkgtest
Build-Depends: file, cpio, libdb-dev, libgdbm-dev (>= 1.18-3), libgdbm-compat-dev, netbase <!nocheck>, procps [!hurd-any] <!nocheck>, debhelper-compat (= 13), zlib1g-dev | libz-dev, libbz2-dev, dpkg-dev (>= 1.17.14), dist (>= 3.5-236), libc6-dev (>= 2.19-9) [s390x]
Package-List:
 libperl-dev deb libdevel optional arch=any
 libperl5.32 deb libs optional arch=any
 perl deb perl standard arch=any
 perl-base deb perl required arch=any essential=yes
 perl-debug deb devel optional arch=any
 perl-doc deb doc optional arch=all
 perl-modules-5.32 deb libs standard arch=all
Checksums-Sha1:
 54a6edeb0ad3b9a0afe089d85e5e743b730b18c5 871331 perl_5.32.1.orig-regen-configure.tar.gz
 1fb4f710d139da1e1a3e1fa4eaba201fcaa8e18e 12610988 perl_5.32.1.orig.tar.xz
 335383a549989bcc89b727b7ab6731e5e03425b3 167388 perl_5.32.1-4+deb11u3.debian.tar.xz
Checksums-Sha256:
 1d179b41283f12ad83f9758430f6ddc49bdf20db5c396aeae7e51ebb4e4afd29 871331 perl_5.32.1.orig-regen-configure.tar.gz
 57cc47c735c8300a8ce2fa0643507b44c4ae59012bfdad0121313db639e02309 12610988 perl_5.32.1.orig.tar.xz
 a875b28e158180906d77df06dea08b8e724f91b31249fe7d205fc3f2cb520ddf 167388 perl_5.32.1-4+deb11u3.debian.tar.xz
Files:
 377ac050e8c90f3361fec97d91e96469 871331 perl_5.32.1.orig-regen-configure.tar.gz
 7f104064b906ad8c7329ca5e409a32d7 12610988 perl_5.32.1.orig.tar.xz
 741773ddb9b9d629e197e4d81c3f4f9f 167388 perl_5.32.1-4+deb11u3.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQJFBAEBCgAvFiEEdqKOQsmBHZHoj7peLsD/s7cwGx8FAmVjaJwRHG50eW5pQGRl
Ymlhbi5vcmcACgkQLsD/s7cwGx9UUBAAmJuWq1lPKDiAqAdhZhC1u886sCRkDJsU
TRwnk7j4eRFFZJo30z/P6za+fZ6moPVvAt/6hUczXLX2GjDh0TnzuNqtVdEYtYTo
Qj4eBH5FPEJYet4k3J3UmhvwkJzTQ+fQ9BGyjJyO8vOUi+eUvuDUGRi8XcFZbsg+
aGvF/gtnr7lnDlw3MdSmrsI9u9xG2C2cKFWCzmrhJ0Pmh1IvcI2/IcKSN4E3LsFH
JgVjTi4TfCJh9dqdQaG0t9FqEFxJpzoXkwPiOPQ7NgAMjpcvjWeArTbyshxZeLoj
Z5U0Z4iVQqe8gMf3FjbbzL3FmGaaoj1r7hGoThQ1ba5LiBG4wydtk73TIxwXlNWQ
xRU8oypALYjX8lyJDvlk8utb1tPs3Zgd1bVvH4+Td1WvOMOmj8HIvYmQ/F/3yhGd
XMDjP4wBDD0Z6UnJHHhbGKol80pzIXG6N6bEjwK5p/Xp4jhfVZG7OtRPpQOb50yT
3+BD03hQifZoFXx7XRXUF1irDRaG3GwBkh+Paccb3OO/8SzZiKDqheEw4HeAfJtn
jo1w7HjEK9GIaN1Z8+Bnr1b4rcrvjiOuWgGKojvWxL/qFRUqrLpemwS2etz7pfdG
bfnyVp9VqnecmJesF1Ux8kqSj4b6ecMGShQ2yOfeEhG+td5LsJQRlXCtyZNjK2xG
gYmqbtL2UMQ=
=EUHx
-----END PGP SIGNATURE-----
