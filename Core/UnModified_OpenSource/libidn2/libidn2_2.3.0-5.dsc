-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: libidn2
Binary: libidn2-0, libidn2-doc, libidn2-dev, libidn2-0-dev, idn2
Architecture: any all
Version: 2.3.0-5
Maintainer: Debian Libidn team <help-libidn@gnu.org>
Uploaders: Simon Josefsson <simon@josefsson.org>, Ondřej Surý <ondrej@debian.org>
Homepage: https://www.gnu.org/software/libidn/#libidn2
Standards-Version: 4.5.1
Vcs-Browser: https://salsa.debian.org/debian/libidn2
Vcs-Git: https://salsa.debian.org/debian/libidn2.git
Testsuite: autopkgtest
Testsuite-Triggers: build-essential, man, pkg-config
Build-Depends: debhelper-compat (= 13), gengetopt, libunistring-dev, pkg-config, ronn | ruby-ronn (<< 0.7.3-5.1~)
Build-Depends-Indep: dblatex, gtk-doc-tools, libglib2.0-dev, texinfo, texlive
Package-List:
 idn2 deb devel optional arch=any
 libidn2-0 deb libs optional arch=any
 libidn2-0-dev deb oldlibs optional arch=any
 libidn2-dev deb libdevel optional arch=any
 libidn2-doc deb doc optional arch=all
Checksums-Sha1:
 53f54fd3c305d871ed5716406c0695fad5c34c45 2164993 libidn2_2.3.0.orig.tar.gz
 3286936fc4445de6f0ac1390d7fd5c82e554124c 11276 libidn2_2.3.0-5.debian.tar.xz
Checksums-Sha256:
 e1cb1db3d2e249a6a3eb6f0946777c2e892d5c5dc7bd91c74394fc3a01cab8b5 2164993 libidn2_2.3.0.orig.tar.gz
 e061b97d035e374bc6a948a514c26ad7d1bda31c8147cc8db02e604c82865a15 11276 libidn2_2.3.0-5.debian.tar.xz
Files:
 01c5084995295e519f95978ae9785ee0 2164993 libidn2_2.3.0.orig.tar.gz
 e9f0aeac4f69efb598422a17a215cddc 11276 libidn2_2.3.0-5.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQFIBAEBCgAyFiEEmUFc4ZBdDlWp+IAmhgt/uzL4EZ0FAmABcpAUHHNpbW9uQGpv
c2Vmc3Nvbi5vcmcACgkQhgt/uzL4EZ0ahAf/QcvhNkalLU/AMxIU/kFf5fFa4Uv2
9S7sh6jGdP1VhXhpmnMPeGyqxqUwqFobtPkvVx2vwlXmpzKzrHTXfmqwyRVMoSsR
3hhpowUo3Sf0f1IKwOqLHi2/pud/hsl6p8xIuE6Er1jlUpfHoF3pCRzfs9uH/5xP
+kU6GjB+WobBiH8KBYVtxIsvGGbZIS3dXkQT8vOlMLI7i6cuNUnhXLkuY9/lKSbK
VKndkl3+JyyU+mgyXa0JiJZnmM8jWO1CtaeH7W9rP51ATaMONApwxwSpORvDAGLi
JSeGne3lOp7X1wUOKdX/AfVeTFLUl5hm3jqGaiFT+viPyT0n7fBuiVZF0A==
=b3QT
-----END PGP SIGNATURE-----
