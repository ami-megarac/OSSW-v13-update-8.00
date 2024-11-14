-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: perl
Binary: perl-base, perl-doc, perl-debug, libperl5.32, libperl-dev, perl-modules-5.32, perl
Architecture: any all
Version: 5.32.1-4+deb11u4
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
 1a9f3ec534dceb86f90304bcce61fed77438ebe3 170428 perl_5.32.1-4+deb11u4.debian.tar.xz
Checksums-Sha256:
 1d179b41283f12ad83f9758430f6ddc49bdf20db5c396aeae7e51ebb4e4afd29 871331 perl_5.32.1.orig-regen-configure.tar.gz
 57cc47c735c8300a8ce2fa0643507b44c4ae59012bfdad0121313db639e02309 12610988 perl_5.32.1.orig.tar.xz
 3fd8c90c0b5a8d4efeb2f2cb03d4c83d9fff90e223d3d1d385754a44cf59f597 170428 perl_5.32.1-4+deb11u4.debian.tar.xz
Files:
 377ac050e8c90f3361fec97d91e96469 871331 perl_5.32.1.orig-regen-configure.tar.gz
 7f104064b906ad8c7329ca5e409a32d7 12610988 perl_5.32.1.orig.tar.xz
 518675f386a529d452a35ea6d2346d1a 170428 perl_5.32.1-4+deb11u4.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCgAdFiEERpy6p3b9sfzUdbME05pJnDwhpVIFAmcVh4IACgkQ05pJnDwh
pVIi/g//WpHoy0Wdu+rBFRrEihkd1S4wEBfQx7Rubc7AgqvIdHgfe5RZDXiV4x72
m1hbhmDuLSlHeccHXpByrGUk8WEssBIaekRDDC4r6wqh3abFr6FR7YsN8zEClESU
BOLpoxuxX0BPo8NBiCo2Bmr7egpZCHNz2/PLxIZv0Q7w+ti15NYFAww9pDvJUmpa
QvgOzkajcggcR81cyRsfgKaKnIYAWj3fWFhXRlKVRbkezaTgELf1Pud8E/1HA2Av
pNrGxdnaX6uZ4qphhv2QISv5vxfs5AQWuG28ZjMUVMkTOzC0SY6ohuPXZ+ePCIfy
i69mGU7HmoNrIt9528aEbbJOJDOdUYFUNDET6T6QwagqpVkBBkEwpTvnyYU7npf9
+ZoHyZQFygGYX9puUYidwMBlgBhqm86K/flRPPvaq8NxtMfU2vyRuyVWLstUzz+6
LCwC8Zjw2PSHovuvCCy+b/cuF1kGPgBHaUa5WgI0g3Fnsb4QOLBP+qCmG5u5J/93
m/NPfRH6rNVVAynVuv5wEQiefZXuHC/SA5/Kc4VfwaI6y+5IHEdvKFSmox3d5Og3
E7Ew5TO3wk1h+jx93Jt/fv1+KKGekFv1XwVGvst2uGHYi+yrIjRwU6pOytNtcmbH
n8SfA/4mDlDZy7uF8UwJ0ebNTUbWcHzO9/xtefJBfUmNmxSnSN4=
=JCaF
-----END PGP SIGNATURE-----
