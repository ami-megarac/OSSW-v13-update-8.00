-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: e2fsprogs
Binary: fuse2fs, logsave, e2fsck-static, e2fsprogs-l10n, libcom-err2, comerr-dev, libss2, ss-dev, e2fsprogs-udeb, libext2fs2, libext2fs-dev, e2fsprogs
Architecture: any all
Version: 1.46.2-2
Maintainer: Theodore Y. Ts'o <tytso@mit.edu>
Homepage: http://e2fsprogs.sourceforge.net
Standards-Version: 4.5.1
Vcs-Browser: https://git.kernel.org/pub/scm/fs/ext2/e2fsprogs.git
Vcs-Git: https://git.kernel.org/pub/scm/fs/ext2/e2fsprogs.git -b debian/master
Testsuite: autopkgtest
Testsuite-Triggers: fuse3
Build-Depends: gettext, texinfo, pkg-config, libfuse-dev [linux-any kfreebsd-any] <!pkg.e2fsprogs.no-fuse2fs>, debhelper (>= 12.0), dh-exec, libblkid-dev, uuid-dev, m4, udev [linux-any], systemd [linux-any], cron [linux-any]
Package-List:
 comerr-dev deb libdevel optional arch=any
 e2fsck-static deb admin optional arch=any profile=!pkg.e2fsprogs.no-static
 e2fsprogs deb admin required arch=any
 e2fsprogs-l10n deb localization optional arch=all
 e2fsprogs-udeb udeb debian-installer optional arch=any profile=!noudeb
 fuse2fs deb admin optional arch=linux-any,kfreebsd-any profile=!pkg.e2fsprogs.no-fuse2fs
 libcom-err2 deb libs optional arch=any
 libext2fs-dev deb libdevel optional arch=any
 libext2fs2 deb libs optional arch=any
 libss2 deb libs optional arch=any
 logsave deb admin optional arch=any
 ss-dev deb libdevel optional arch=any
Checksums-Sha1:
 cfaf65ecdfb71cbb424d4cce13b436da4a079dff 9496954 e2fsprogs_1.46.2.orig.tar.gz
 feafeda9c3804e870726e9812df44bdbb64a6146 488 e2fsprogs_1.46.2.orig.tar.gz.asc
 c8b88204ae05dab2d80b75d71c5f7fa140f048ab 92624 e2fsprogs_1.46.2-2.debian.tar.xz
Checksums-Sha256:
 f79f26b4f65bdc059fca12e1ec6a3040c3ce1a503fb70eb915bee71903815cd5 9496954 e2fsprogs_1.46.2.orig.tar.gz
 948552550f23a9e0223cecb51b5b85258c9d94895a20bce1180fce770628a55f 488 e2fsprogs_1.46.2.orig.tar.gz.asc
 dc67d61815c524922e7461040d732bd245cf0196f7cc8a91ea7911a87b38f737 92624 e2fsprogs_1.46.2-2.debian.tar.xz
Files:
 e8ef5fa3b72557be5e9fe564a25da6eb 9496954 e2fsprogs_1.46.2.orig.tar.gz
 cc53a1867cb8b23f1c74453ddf9110d9 488 e2fsprogs_1.46.2.orig.tar.gz.asc
 663e84e9d724a60103ceb5248bbd0b07 92624 e2fsprogs_1.46.2-2.debian.tar.xz
Dgit: 5697567d7592522ba76381cde80dbda57246c1b4 debian archive/debian/1.46.2-2 https://git.dgit.debian.org/e2fsprogs

-----BEGIN PGP SIGNATURE-----

iQEzBAEBCAAdFiEEK2m5VNv+CHkogTfJ8vlZVpUNgaMFAmC+zYwACgkQ8vlZVpUN
gaMbiQf/bAKNrlZJ+hb7R4c8cbxtHfM0W7G9Nk+Vt96MTNbJMVZbnnQwdyBDuzaW
UJk59vRq1UrTpVNRwm0Aeb+EmEfDS+Ol0VOoAhBMas6vFOv3yr6UdH8Ph3HGrdWw
T/sE0v0pXeURn9WxMvCZEXDwaH4x+cR28DGRzZQAmK3RwB2zB0lffl2mXCAoVJ3g
SUmx07pQFy1WEa9oo36+N6AajF7tlYwLoybX81FCeQzlCUQgMT+EklyZokQAjLoX
lkhWfAT3y28FmzgJgKsQ9+hb1DExsbTHuwiGtFWgnHWJTYO+otvnO9l65Efa7Ijw
jpvWzw5Dw4laTHQ07u1qVZyd7GX2hg==
=S8eA
-----END PGP SIGNATURE-----
