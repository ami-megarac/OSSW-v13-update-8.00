-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: libgpg-error
Binary: libgpg-error-dev, libgpg-error0, gpgrt-tools, libgpg-error0-udeb, libgpg-error-mingw-w64-dev, libgpg-error-l10n
Architecture: any all
Version: 1.38-2
Maintainer: Debian GnuPG Maintainers <pkg-gnupg-maint@lists.alioth.debian.org>
Uploaders:  Daniel Kahn Gillmor <dkg@fifthhorseman.net>,
Homepage: https://www.gnupg.org/related_software/libgpg-error/
Standards-Version: 4.5.0
Vcs-Browser: https://salsa.debian.org/debian/libgpg-error
Vcs-Git: https://salsa.debian.org/debian/libgpg-error.git -b debian/master
Testsuite: autopkgtest
Testsuite-Triggers: build-essential, gcc-mingw-w64-i686, gcc-mingw-w64-x86-64, pkg-config, wine32, wine64
Build-Depends: automake (>= 1.14), debhelper-compat (= 13), gettext (>= 0.19.3), texinfo
Build-Depends-Indep: mingw-w64
Package-List:
 gpgrt-tools deb devel optional arch=any
 libgpg-error-dev deb libdevel optional arch=any
 libgpg-error-l10n deb localization optional arch=all
 libgpg-error-mingw-w64-dev deb libdevel optional arch=all
 libgpg-error0 deb libs optional arch=any
 libgpg-error0-udeb udeb debian-installer optional arch=any
Checksums-Sha1:
 cb50ade98f8ba088f8bb9b910ca8f00abaac5d0a 957637 libgpg-error_1.38.orig.tar.bz2
 c0b6fd4820eed0341e90ea9a75634ee54f6e7f85 488 libgpg-error_1.38.orig.tar.bz2.asc
 c799fbc370cab4dcfe61da4af7e2c35fa9db4e8c 19544 libgpg-error_1.38-2.debian.tar.xz
Checksums-Sha256:
 d8988275aa69d7149f931c10442e9e34c0242674249e171592b430ff7b3afd02 957637 libgpg-error_1.38.orig.tar.bz2
 d80eb927d85e19e96d8de17552f8f48b517ae7acac7685404e8027475c5b4330 488 libgpg-error_1.38.orig.tar.bz2.asc
 824bcb278ead676c20f174bd551b1cc44a294137fabe6a1d892667882f3b4ba2 19544 libgpg-error_1.38-2.debian.tar.xz
Files:
 f164ce3400c820907965fdc53e43acfc 957637 libgpg-error_1.38.orig.tar.bz2
 7a39bb84595b2e63a431e1bf5a16f4b6 488 libgpg-error_1.38.orig.tar.bz2.asc
 b643ddb252b1d6be5313c18702237c12 19544 libgpg-error_1.38-2.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iHUEARYIAB0WIQQsv6x2UaqQJzY+dXHEDyVUMvKBDwUCXvzkqwAKCRDEDyVUMvKB
D9oDAQCvs8AwvixC9WFBx12OFlBczgZMKwOOJxV4JGn2RZDJtwD/eoPLmd9bsp25
icZpRGtTR4bYFP19BpN1PKlkbLCavQg=
=e57W
-----END PGP SIGNATURE-----
