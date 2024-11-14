-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: sysvinit
Binary: sysvinit-core, sysvinit-utils, sysv-rc, initscripts, bootlogd
Architecture: any all
Version: 2.96-7+deb11u1
Maintainer: Debian sysvinit maintainers <debian-init-diversity@chiark.greenend.org.uk>
Uploaders:  Ian Jackson <ijackson@chiark.greenend.org.uk>, Benda Xu <heroxbd@gentoo.org>, Adam Borowski <kilobyte@angband.pl>, Vincenzo (KatolaZ) Nicosia <katolaz@freaknet.org>,
Homepage: https://savannah.nongnu.org/projects/sysvinit
Standards-Version: 4.5.0
Vcs-Browser: https://salsa.debian.org/debian/sysvinit
Vcs-Git: https://salsa.debian.org/debian/sysvinit.git
Build-Depends: debhelper-compat (= 12), libcrypt-dev, libselinux1-dev [linux-any], po-debconf
Package-List:
 bootlogd deb admin optional arch=any
 initscripts deb admin optional arch=all
 sysv-rc deb admin optional arch=all
 sysvinit-core deb admin optional arch=any
 sysvinit-utils deb admin required arch=any essential=yes
Checksums-Sha1:
 ae032bad8558108487dd04742635e96e62626e9d 147834 sysvinit_2.96.orig.tar.gz
 827f0be86d09c4a00ba171dfbf9b18409ad9f085 129936 sysvinit_2.96-7+deb11u1.debian.tar.xz
Checksums-Sha256:
 1275620f767c85bb2d7e5b9542579ae097f3eb542065ff30a70efc95b6e84c64 147834 sysvinit_2.96.orig.tar.gz
 5ce9a0b9735a66c536c77c65e17d4190338e0c0416ba235c26c9cf12d3cfbeb3 129936 sysvinit_2.96-7+deb11u1.debian.tar.xz
Files:
 7ed8956c3204157ec9fab6f6f7ed68d1 147834 sysvinit_2.96.orig.tar.gz
 b016b8ce181644ea7c000ffb1f35ff65 129936 sysvinit_2.96-7+deb11u1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCAAdFiEEUGwVpCsK9aCoVCPu0opFvzKH1kkFAmIx0Z4ACgkQ0opFvzKH
1kndAw//QQV8CniXe1Io4I4sbjCq/6VmPRHd/iS9x+mRsGYWCeLlXs+KOCsaeaA6
agHo7SL3HDQuwSmvJhXzVSohLON5OFjZoUyBwejRuE9mV7A867CcRUiqZZcwu3MQ
+NcphX4xkenaLQlpXPhZphXDiqrvP3lHr0HCv498ChwQMWdx9QJtSdFlGyisQI7N
vT4z2/0pCscDKg5AWyxsfglIWJGMvbWwY+O+Ge/e9szKX9dwJKKzoQRFlflw+gIy
VsOGvGrd/12krEoRrP6QRdXMOyghQpqwhOwgc0UeQp+9j3nNRhv576hAZ7+KPxK0
v4Udu+vx1VDQKtosTMdUcA8OuewDxbA6JriQ2f0nSxcwamkOcswXbmi6GCivJiCh
z4hw9GWR8lAU6AdFN8jWyDpFvkYC5SDc7YZYYxaLzjm9DSyr7qS3AWnEZrPtJYdQ
a+n2oOUOBenYfuBerYwFiSI7qujujKQcXjl/TzFilLeFSkjwo+Mc+LyMRFb/3qnY
e0J44BQQ5j7KSSY6VCn4amPZ5Dn2jm+RY+fmdVy8a8eLHJYo7AbMGLxoagtiwwOQ
jIfm31EsRee4VK833uwNOUdtBKfyZTKRldANAlMoNav4tmKvsGZ7/vHnXWr1MDNC
eI8rFf4nIrEnFGfJH1cjqK0u8zRJ4lq2pG98zL2s1y5fxEZGKBA=
=03lm
-----END PGP SIGNATURE-----
