-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: pam
Binary: libpam0g, libpam-modules, libpam-modules-bin, libpam-runtime, libpam0g-dev, libpam-cracklib, libpam-doc
Architecture: any all
Version: 1.4.0-9+deb11u1
Maintainer: Steve Langasek <vorlon@debian.org>
Uploaders: Sam Hartman <hartmans@debian.org>
Homepage: http://www.linux-pam.org/
Standards-Version: 4.3.0
Vcs-Browser: https://salsa.debian.org/vorlon/pam
Vcs-Git: https://salsa.debian.org/vorlon/pam.git
Build-Depends: libcrack2-dev (>= 2.8), bzip2, debhelper (>= 9), quilt (>= 0.48-1), flex, libdb-dev, libselinux1-dev [linux-any], po-debconf, dh-autoreconf, autopoint, libaudit-dev [linux-any] <!stage1>, pkg-config, libfl-dev, libfl-dev:native, docbook-xsl, docbook-xml, xsltproc, libxml2-utils, w3m
Build-Conflicts: libdb4.2-dev, libxcrypt-dev
Build-Conflicts-Indep: fop
Package-List:
 libpam-cracklib deb admin optional arch=any
 libpam-doc deb doc optional arch=all
 libpam-modules deb admin required arch=any
 libpam-modules-bin deb admin required arch=any
 libpam-runtime deb admin required arch=all
 libpam0g deb libs optional arch=any
 libpam0g-dev deb libdevel optional arch=any
Checksums-Sha1:
 e26c6594c14680da42ea2875b60664ec159670bf 988908 pam_1.4.0.orig.tar.xz
 3f17c765d15b61d11ee215b6aabf03413502af24 120148 pam_1.4.0-9+deb11u1.debian.tar.xz
Checksums-Sha256:
 cd6d928c51e64139be3bdb38692c68183a509b83d4f2c221024ccd4bcddfd034 988908 pam_1.4.0.orig.tar.xz
 bcaaad9423c3ab32c5c4f9e363595a84fe3c535aa9568e42e560028a4e33dfcf 120148 pam_1.4.0-9+deb11u1.debian.tar.xz
Files:
 39fca0523bccec6af4b63b5322276c84 988908 pam_1.4.0.orig.tar.xz
 0a4e504123f7bf90edce22edba13ae06 120148 pam_1.4.0-9+deb11u1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iHUEARYIAB0WIQSj2jRwbAdKzGY/4uAsbEw8qDeGdAUCYVEGbgAKCRAsbEw8qDeG
dMEfAP4spj/EK7TB6Rg71d6OP05rpdsIVbc5M3p7ceDX3mdWVAEA14/IYf29enlr
NTNI+SZTx5zQC5gDBtqcIcHPo7r4sAo=
=cZYT
-----END PGP SIGNATURE-----
