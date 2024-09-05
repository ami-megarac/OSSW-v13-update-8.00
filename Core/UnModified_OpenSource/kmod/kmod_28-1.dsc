-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: kmod
Binary: kmod, libkmod2, libkmod-dev, kmod-udeb, libkmod2-udeb
Architecture: linux-any
Version: 28-1
Maintainer: Marco d'Itri <md@linux.it>
Standards-Version: 4.5.1.0
Vcs-Browser: https://salsa.debian.org/md/kmod
Vcs-Git: https://salsa.debian.org/md/kmod.git
Build-Depends: debhelper-compat (= 12), liblzma-dev, libssl-dev, xsltproc, autoconf, automake, libtool, gtk-doc-tools
Package-List:
 kmod deb admin important arch=linux-any
 kmod-udeb udeb debian-installer important arch=linux-any
 libkmod-dev deb libdevel optional arch=linux-any
 libkmod2 deb libs optional arch=linux-any
 libkmod2-udeb udeb debian-installer important arch=linux-any
Checksums-Sha1:
 0e53311431fe7e12c3179eefefb47eaef52a4b0b 251476 kmod_28.orig.tar.xz
 16e4a49aa762ebf655b62a4841a85e99b630dbf2 8512 kmod_28-1.debian.tar.xz
Checksums-Sha256:
 02a4368b1d073e382bcfbbc58536dccb894045f670ebf68dd5a763eea3101c9c 251476 kmod_28.orig.tar.xz
 35f77764f4e597ce193bde5645aaa19a1540ae1552b0ccb48cf335e06aeaed4f 8512 kmod_28-1.debian.tar.xz
Files:
 0f6a07b57b7837e1c79524ba2a405f4d 251476 kmod_28.orig.tar.xz
 74d66646365736161a7b9ee9d68ff4e7 8512 kmod_28-1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iHUEARYIAB0WIQQnKUXNg20437dCfobLPsM64d7XgQUCX/e5kQAKCRDLPsM64d7X
gb35AQC2O5E94A6AIX/jDvvb1OXocLLsT0zadf8p+x/0MhpdQwD8CbMiUc/kZR2v
JaeRVDIqHsHEn99H0uRGPSYHYTrHDgg=
=VOrh
-----END PGP SIGNATURE-----
