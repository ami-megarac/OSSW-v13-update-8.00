-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: elfutils
Binary: elfutils, libelf1, libelf-dev, libdw-dev, libdw1, libasm1, libasm-dev, libdebuginfod1, libdebuginfod-dev, debuginfod, libdebuginfod-common
Architecture: any all
Version: 0.187-1~bpo11+1
Maintainer: Debian Elfutils Maintainers <debian-gcc@lists.debian.org>
Uploaders: Kurt Roeckx <kurt@roeckx.be>, Matthias Klose <doko@debian.org>, Sergio Durigan Junior <sergiodj@debian.org>,
Homepage: https://sourceware.org/elfutils/
Standards-Version: 4.5.1
Vcs-Browser: https://salsa.debian.org/toolchain-team/elfutils
Vcs-Git: https://salsa.debian.org/toolchain-team/elfutils.git
Build-Depends: debhelper (>= 11), autoconf, automake, lsb-release, bzip2, zlib1g-dev, zlib1g-dev:native, libbz2-dev, liblzma-dev, m4, gettext, po-debconf, gawk, dpkg-dev (>= 1.16.1~), gcc-multilib [any-amd64 sparc64] <!nocheck>, libc6-dbg [powerpc powerpcspe ppc64 ppc64el armel armhf arm64 sparc64 riscv64], flex, bison, pkg-config, libarchive-dev <!pkg.elfutils.nodebuginfod>, libmicrohttpd-dev <!pkg.elfutils.nodebuginfod>, libcurl4-gnutls-dev <!pkg.elfutils.nodebuginfod>, libsqlite3-dev <!pkg.elfutils.nodebuginfod>
Build-Conflicts: autoconf2.13
Package-List:
 debuginfod deb devel optional arch=any profile=!pkg.elfutils.nodebuginfod
 elfutils deb utils optional arch=any
 libasm-dev deb libdevel optional arch=any
 libasm1 deb libs optional arch=any
 libdebuginfod-common deb devel optional arch=all profile=!pkg.elfutils.nodebuginfod
 libdebuginfod-dev deb libdevel optional arch=any profile=!pkg.elfutils.nodebuginfod
 libdebuginfod1 deb libs optional arch=any profile=!pkg.elfutils.nodebuginfod
 libdw-dev deb libdevel optional arch=any
 libdw1 deb libs optional arch=any
 libelf-dev deb libdevel optional arch=any
 libelf1 deb libs optional arch=any
Checksums-Sha1:
 2c529212fff4b54e890cb3cf33f93b5f39eda5ab 9240221 elfutils_0.187.orig.tar.bz2
 0ea10dd33bb854402df43b8501f7ea74c0d9fde2 37840 elfutils_0.187-1~bpo11+1.debian.tar.xz
Checksums-Sha256:
 e70b0dfbe610f90c4d1fe0d71af142a4e25c3c4ef9ebab8d2d72b65159d454c8 9240221 elfutils_0.187.orig.tar.bz2
 74a6aa269ff9061e9cb1ce832ca4a437df5aca290b1a44e039a2c568018d4fff 37840 elfutils_0.187-1~bpo11+1.debian.tar.xz
Files:
 cc04f07b53a71616b22553c0a458cf4b 9240221 elfutils_0.187.orig.tar.bz2
 0797864f92a65929654b50fe1cde2965 37840 elfutils_0.187-1~bpo11+1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQJIBAEBCgAyFiEEI3pUsQKHKL8A7zH00Ot2KGX8XjYFAmLUQfAUHHNlcmdpb2Rq
QGRlYmlhbi5vcmcACgkQ0Ot2KGX8XjaYQg/9GVl+rA0JUgCh+NVedISHhEY83qnK
pzeMcZxwpGi+Q433osVPBaRiN5n+/2kFo7jGFlVSOGqSNvywDkKSa7RAe5Ryptad
QyQdHHgbZu8f/MpY6nCLULWw8CLfDruQ7uWuBF05RJD5yvH1bmFFA9KmV2jNaXgK
8gvVJKbzn8kUnMXraWotu25y1b03dDb5WeV9jX5VTMbUzTaQ1fY81W7YYDEukx+2
CRqa8ssmxphNwBR4fPKan/HkcphSENJA+SyrEBmJ5WWfhRZ60nOKcZyEHt6hZORu
Tu2O8V6F5D+3oyyqYGWhG6WW9HHzN9rsUnq0Ox5Ij+zdLB1A9AqE+ILBU3wqOCew
yUQCHwIrxUjSEWZcZAc9tdgUaUSWVvdJLE881FXXVcoqaPNTT/p7INo7Iza5Pgty
whm5iuRsLxUQ7eCy7/Ptbe6gXNsuo8vYlNGMVoEMesDVHg20XOu5xHV8m9i5iuBj
BRsljQyrsKO0hh+nOlRLBhE4TmQWUo2Xjxtu5rX16yT8YElj7nVLr22O7/J7BBeF
c7NNeXj7NldDK+cv4Bqmlu+gl6LZqxzZ3CXp39I+3DkJOiGvsxpCPnWU+QGINCI1
LdzgT6UHMCyx46SyoTJGwcYswNyNLcZW68zw+ay57ORS7uF7oL9Mg+NJDaZ44rLW
soqJC0fQRyzZzQ0=
=lhsC
-----END PGP SIGNATURE-----
