-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: libxcrypt
Binary: libcrypt1, libcrypt-dev, libcrypt1-udeb, libxcrypt-source
Architecture: any all
Version: 1:4.4.18-4
Maintainer: Marco d'Itri <md@linux.it>
Standards-Version: 4.5.0.3
Vcs-Browser: https://salsa.debian.org/md/libxcrypt
Vcs-Git: https://salsa.debian.org/md/libxcrypt.git
Testsuite: autopkgtest
Build-Depends: debhelper-compat (= 12), debhelper (>= 12.7~), autoconf, automake, libtool, pkg-config
Package-List:
 libcrypt-dev deb libdevel optional arch=any
 libcrypt1 deb libs optional arch=any protected=yes
 libcrypt1-udeb udeb debian-installer optional arch=any
 libxcrypt-source deb devel optional arch=all
Checksums-Sha1:
 37c40940b64ee9cbc921d7ac3cc13d90d54fdcbf 397776 libxcrypt_4.4.18.orig.tar.xz
 76661337026bd7886836ecf3d65baeec60015d61 7560 libxcrypt_4.4.18-4.debian.tar.xz
Checksums-Sha256:
 4cd2a06e98519d57a5572ee8885b6cc23c70a559d234c161d3f22c487edaa3fa 397776 libxcrypt_4.4.18.orig.tar.xz
 6c99b888c57e1411d870fa81d057e30444aa801ed430aa3126d31996e187dd84 7560 libxcrypt_4.4.18-4.debian.tar.xz
Files:
 b3074c8e7cee43dee5a182657119e249 397776 libxcrypt_4.4.18.orig.tar.xz
 35b9262d0209b2ccd1e889e4fa3d4374 7560 libxcrypt_4.4.18-4.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iHUEARYIAB0WIQQnKUXNg20437dCfobLPsM64d7XgQUCYHzVMwAKCRDLPsM64d7X
gXijAQCCIqXhRcVJt6bdFlzXadRgzG4ZXXc35D9rfkr7IpA0KQD/QKaBU4telUQz
hdGNRNVmSjZXyxgtM82Tu2w22nT8lw4=
=SPAg
-----END PGP SIGNATURE-----
