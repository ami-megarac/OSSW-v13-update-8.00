-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: valgrind
Binary: valgrind, valgrind-dbg, valgrind-mpi
Architecture: amd64 arm64 armhf i386 mips mipsel mips64 mips64el powerpc ppc64 ppc64el s390x x32
Version: 1:3.16.1-1
Maintainer: Alessandro Ghedini <ghedo@debian.org>
Homepage: http://www.valgrind.org/
Standards-Version: 4.5.0
Vcs-Browser: https://salsa.debian.org/debian/valgrind
Vcs-Git: https://salsa.debian.org/debian/valgrind.git
Testsuite: autopkgtest
Build-Depends: debhelper-compat (= 13), gdb, gcc-multilib [amd64], libc6-dev-i386 [amd64], mpi-default-dev, pkg-config, docbook, docbook-xsl, docbook-xml, xsltproc
Package-List:
 valgrind deb devel optional arch=amd64,arm64,armhf,i386,mips,mipsel,mips64,mips64el,powerpc,ppc64,ppc64el,s390x,x32
 valgrind-dbg deb debug optional arch=amd64,arm64,armhf,i386,mips,mipsel,mips64,mips64el,powerpc,ppc64,ppc64el,s390x,x32
 valgrind-mpi deb devel optional arch=amd64,arm64,armhf,i386,mips,mipsel,mips64,mips64el,powerpc,ppc64,ppc64el,s390x,x32
Checksums-Sha1:
 192bfe53802abb7f23edbb8aad61158a60c34f8a 16262776 valgrind_3.16.1.orig.tar.bz2
 8e8686acabe0de46deb15d484fdb966831933e48 27772 valgrind_3.16.1-1.debian.tar.xz
Checksums-Sha256:
 c91f3a2f7b02db0f3bc99479861656154d241d2fdb265614ba918cc6720a33ca 16262776 valgrind_3.16.1.orig.tar.bz2
 39702edd1a060141efa2509641d4dfa6ef3b7ee090e586a54ea3f6a8d9288c46 27772 valgrind_3.16.1-1.debian.tar.xz
Files:
 d1b153f1ab17cf1f311705e7a83ef589 16262776 valgrind_3.16.1.orig.tar.bz2
 82ce21d5759b7abea484672617032b51 27772 valgrind_3.16.1-1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQJFBAEBCgAvFiEEBsId305pBx+F583DbwzL4CFiRygFAl742RcRHGdoZWRvQGRl
Ymlhbi5vcmcACgkQbwzL4CFiRyg1jhAAvDf3DH0qllwNaU8iwp6h+OOv/nD5YVtR
HG0JjAU5oIVS3/i+gt3nxcH5bye9i6F40sTqzIbIgdSzJZCOUeVDQr/6cVKBvGXJ
1XQMfL7LQ2ZTlHAQBHf31sE6O362Kr+3hu3TrePmYnULQMI3rlg2ajq/gkO9nCys
IQlHqgF2sndxQ/3OdUWV8ahnPJsjzYl7YNNa1XLOM3pYr+0qjgAXoc4QP+fQojt8
KrXu4XiPkA8aWyyrLquGErnIAY5DOlPCniiaMbD30ymqaizUAn3mOOe0ufvixMGf
cql3M6c2kbb+mOjvCG1/Ei/bUgKEkXAdcINRQxIpZ/05VVh0ujva7pkUW93mueWh
6MnoXFlFJJt9zSGz71RK83r5mnhXm4jr2JiUQbuXkNToUm0BH45k9FiDJAseqLJY
OGuYYwH81/v9fQtr0w90emeamDlWrXCTJhBqI3S5PSCIM3Rhktza6ih2Qc4ulxW8
VzlwHvTfvIllxYRuDaXfCqtFGmEBRtLOFcLXfTDJHL4xCJ0Aur8zoGIJ9jAglj7W
IXmrIKxruB7HQfz8uZy4aYilPAuYAzIilVgptAz9qlSt5ELSZM5W5x3Rq5qaNcw3
BS7vSPaG1n99R+z327b7w6celhlP9WzRskrvVnT6hm480FUd0ejlumEzgA/kawnm
zb1nWcgYxwQ=
=Xoc/
-----END PGP SIGNATURE-----
