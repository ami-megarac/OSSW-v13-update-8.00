-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: libseccomp
Binary: libseccomp-dev, libseccomp2, seccomp, python3-seccomp
Architecture: linux-any
Version: 2.5.1-1+deb11u1
Maintainer: Kees Cook <kees@debian.org>
Uploaders: Luca Bruno <lucab@debian.org>, Felix Geyer <fgeyer@debian.org>
Homepage: https://github.com/seccomp/libseccomp
Standards-Version: 3.9.7
Vcs-Browser: https://salsa.debian.org/debian/libseccomp
Vcs-Git: https://salsa.debian.org/debian/libseccomp.git
Testsuite: autopkgtest
Testsuite-Triggers: build-essential
Build-Depends: debhelper-compat (= 12), linux-libc-dev, dh-python <!nopython>, python3-all-dev:any <!nopython>, libpython3-all-dev <!nopython>, cython3:native <!nopython>, valgrind [amd64 arm64 armhf i386 mips mips64 powerpc ppc64 ppc64el s390x] <!nocheck>, gperf
Package-List:
 libseccomp-dev deb libdevel optional arch=linux-any
 libseccomp2 deb libs optional arch=linux-any
 python3-seccomp deb python optional arch=linux-any profile=!nopython
 seccomp deb utils optional arch=linux-any
Checksums-Sha1:
 6069b720219400d8369f567af30cd2ad06ad938e 638811 libseccomp_2.5.1.orig.tar.gz
 2a2e3196065512d210841f622abb77917aac3f0a 833 libseccomp_2.5.1.orig.tar.gz.asc
 c5548f4c2298c0ab9478a1a040f9782d14f5f6c4 19524 libseccomp_2.5.1-1+deb11u1.debian.tar.xz
Checksums-Sha256:
 ee307e383c77aa7995abc5ada544d51c9723ae399768a97667d4cdb3c3a30d55 638811 libseccomp_2.5.1.orig.tar.gz
 14d45c86e5ceed5ac5511c3ebf70a4dca128b7584b314dc8a551c779ea225d2e 833 libseccomp_2.5.1.orig.tar.gz.asc
 a09ef7c0b9b6464f426b78a7b978d8566da53667c1a234234ffd2cc600543200 19524 libseccomp_2.5.1-1+deb11u1.debian.tar.xz
Files:
 59f5563c532d3fa1df9db0516b36b1cd 638811 libseccomp_2.5.1.orig.tar.gz
 a70bd435a38d8d3dee746f9be1efa0fe 833 libseccomp_2.5.1.orig.tar.gz.asc
 2d525e62b28e5fe051b89c2080ce5d78 19524 libseccomp_2.5.1-1+deb11u1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCgAdFiEEFkxwUS95KUdnZKtW/iLG/YMTXUUFAmGtQ0QACgkQ/iLG/YMT
XUXW5RAAlrOaCybX68zoke5GZAbKOYsRordH5p1D0lOUvkZmJeMEeHKbeOxF0nEg
KY7elLxw07S87lcsF+kOsQd8F1uFk//zGqhIEOwoA8lw+JT/VlJfqJkgoEpdDshP
KkM2hSAPlxW/5D6Z2Cxy7JLDJ1N4nw641QeP/qUNU4kMtorjDPUPV4XYOWHKCDmQ
RGfIlR6Bk1fn/viDCbQPhe1bc+6YHBMc/hBiDo3GuFNN3ddFn+LwhP3H8nagmLq0
bUm5HxI+pUagWJRvitQOCSc9Jz3Vy3w6L6YRPpfcrzn9j7HAPozaNL8VxDKnRR34
ujy19pptp7JqEDUMBWLl6I9PX3Z7iyKhkCcNoG/DhcNHWgNMZ1NBeo6KlJgXqhc+
PFE+Wb8OInfX2/WnvK2xetuKFGD+4swIHD6x4ZZnSerAqSjT4E/sV/rAbwrarKYV
uzjLv3CW/YE+0YFi3O/kS53zH+IUXnSPphtg1/Z66ex1lQlcMLXbYhEa6Zs9qDFl
C9seWfi3Ruf0V2yTSMVWNbnhndU4QdBiRUkRbyu1v3b1bz+InOWeIl3CS5FE7LaB
ql6H24HzE+LyjeEVXvTS4PGh/7kHX64+cy0fC+rO31+xMMCB6LDoOH35+UlnjRPT
ZhWPctfJD3FFoSIlO+0NkGSDCaprHEeh+dwJ04Bx2vdmK6IGzsI=
=fhuH
-----END PGP SIGNATURE-----
