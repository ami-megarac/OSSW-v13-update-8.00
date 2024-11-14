-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: gobject-introspection
Binary: libgirepository-1.0-1, libgirepository1.0-dev, libgirepository1.0-doc, gobject-introspection, gir1.2-glib-2.0, gir1.2-freedesktop
Architecture: any all
Version: 1.66.1-1
Maintainer: Debian GNOME Maintainers <pkg-gnome-maintainers@lists.alioth.debian.org>
Uploaders: Iain Lane <laney@debian.org>, Jeremy Bicha <jbicha@debian.org>, Laurent Bigonville <bigon@debian.org>, Tim Lunn <tim@feathertop.org>
Homepage: https://wiki.gnome.org/GObjectIntrospection
Standards-Version: 4.5.0
Vcs-Browser: https://salsa.debian.org/gnome-team/gobject-introspection
Vcs-Git: https://salsa.debian.org/gnome-team/gobject-introspection.git
Testsuite: autopkgtest
Testsuite-Triggers: build-essential, file, gir1.2-gtk-3.0, libcairo2-dev
Build-Depends: debhelper-compat (= 12), dh-sequence-gnome, dh-sequence-python3, meson (>= 0.50.1), python3-dev, python3-distutils, pkg-config, flex, gtk-doc-tools (>= 1.19), bison, libglib2.0-dev (>= 2.62.4-2~), libcairo2-dev, libffi-dev (>= 3.3), libtool, python3-mako, python3-markdown
Build-Depends-Indep: libglib2.0-doc
Package-List:
 gir1.2-freedesktop deb introspection optional arch=any
 gir1.2-glib-2.0 deb introspection optional arch=any
 gobject-introspection deb devel optional arch=any
 libgirepository-1.0-1 deb libs optional arch=any
 libgirepository1.0-dev deb libdevel optional arch=any
 libgirepository1.0-doc deb doc optional arch=all
Checksums-Sha1:
 c1a9aa497bbe0b639f9ea4505c5fadfd765f9c98 1012784 gobject-introspection_1.66.1.orig.tar.xz
 e356f32df5aa5bfb3a127e607720d34936ef3e7b 23656 gobject-introspection_1.66.1-1.debian.tar.xz
Checksums-Sha256:
 dd44a55ee5f426ea22b6b89624708f9e8d53f5cc94e5485c15c87cb30e06161d 1012784 gobject-introspection_1.66.1.orig.tar.xz
 9962d8a3e3330d53877d37c846cfdae17083302eacaa81fc009ff81c7c0401cf 23656 gobject-introspection_1.66.1-1.debian.tar.xz
Files:
 f43d661d39ff52d33622cb1854aaaf76 1012784 gobject-introspection_1.66.1.orig.tar.xz
 7daa925b6eff7361902661fe412d5c94 23656 gobject-introspection_1.66.1-1.debian.tar.xz
Dgit: 4e19215e43a3b4d29aff4c4d2d4a93266682311d debian archive/debian/1.66.1-1 https://git.dgit.debian.org/gobject-introspection

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCAAdFiEENuxaZEik9e95vv6Y4FrhR4+BTE8FAl+ETV8ACgkQ4FrhR4+B
TE/GNw//fJ3L2OuigsPbzdNnVrFWVtQrVsNjcpPyEOZtnWNUZu8ojCnQuy/P1IFH
QnEq9alM8kosLan2l82+Z+esKuGHCNh9+RqC3xIXl6ke8o+aMerpKM6Tw2jpY0d5
tRnGz5njJjUh4QhiccEJ8HDw1J7TSlHQMMTRif3Z6YGOPXICjnPksr96nX+Brf7N
YbeXd91/VkcRJsPqk639bonj/l3RAqRHBU8Hd38Jyi1LZF3GruNCWGZUxn1XAV99
OtNPtMCDcwUHQ1Q9KdnwEe9ISOzXx/+CX4V6YtDYC7TxIra14VGBWTPTlDolLE7H
2psqb5MbTmK3Z5yIK/DR3a2j4l8vCLBTNW28b1Ia+jEcpcM5Sp1xKckALkSnN6Wu
t32e5be0CJh9nItHmn3Tic6xEhc8X1p3SSfyPMOA1BF2PDLOCB8fIErTOsfqaa75
07Bgk9s0kA6b/JIvP65mZRmDv0nDYL/c5GU0Fus/3EXFiS7n0YuaZFbOTEQiaRw8
1YTpinLOAiWnSIy7JFeAIJ9t0jKJqAtdu7XxPcDXyU4B64nbLgBalb6cZVzicUHN
JPctQyHUFF2NqdGK7NZbix6Z14mCxUhtdqiRMRAyuyf7asAItjP/WmcvaxEioL6L
xK32vzM5bDJd0uyAO42FNsCC5wHSMqa4DIUsttC9e7fZCwjdu+s=
=qSIJ
-----END PGP SIGNATURE-----
