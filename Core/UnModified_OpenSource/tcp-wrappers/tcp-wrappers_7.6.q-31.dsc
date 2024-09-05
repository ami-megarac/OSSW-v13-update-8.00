-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: tcp-wrappers
Binary: tcpd, libwrap0, libwrap0-dev
Architecture: any
Version: 7.6.q-31
Maintainer: Marco d'Itri <md@linux.it>
Standards-Version: 4.5.0.3
Vcs-Browser: https://salsa.debian.org/md/tcp-wrappers
Vcs-Git: https://salsa.debian.org/md/tcp-wrappers.git
Build-Depends: debhelper-compat (= 12), libnsl-dev | libc6-dev (<< 2.31-4)
Package-List:
 libwrap0 deb libs optional arch=any
 libwrap0-dev deb libdevel optional arch=any
 tcpd deb net optional arch=any
Checksums-Sha1:
 61689ec85b80f4ca0560aef3473eccd9e9e80481 99438 tcp-wrappers_7.6.q.orig.tar.gz
 557ff48ff18bb5c4183babd6f9a20ee7104d895d 36256 tcp-wrappers_7.6.q-31.debian.tar.xz
Checksums-Sha256:
 9543d7adedf78a6de0b221ccbbd1952e08b5138717f4ade814039bb489a4315d 99438 tcp-wrappers_7.6.q.orig.tar.gz
 20fec7826bde1262007e6dd43f365b5802bacc283942d93b78385ea634dc594a 36256 tcp-wrappers_7.6.q-31.debian.tar.xz
Files:
 e6fa25f71226d090f34de3f6b122fb5a 99438 tcp-wrappers_7.6.q.orig.tar.gz
 8b730ce70c719c7b0e49b9abdf72975e 36256 tcp-wrappers_7.6.q-31.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iHUEARYIAB0WIQQnKUXNg20437dCfobLPsM64d7XgQUCX6yQ2gAKCRDLPsM64d7X
gSYJAQDrxZkYivjJdhOwiVFcXMKQ7y+X18v/LtZsP9BKwUcKbAEA1/aOaPNrcV8c
Utc3UwAli5LpIB1F71DhiuycP+DFTA4=
=qjs8
-----END PGP SIGNATURE-----
