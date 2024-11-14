-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: e2fsprogs
Binary: fuse2fs, logsave, e2fsck-static, e2fsprogs-l10n, libcom-err2, comerr-dev, libss2, ss-dev, e2fsprogs-udeb, libext2fs2, libext2fs-dev, e2fsprogs
Architecture: any all
Version: 1.46.2-2+deb11u1
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
 dda1bf6393c1962c47dc6e8066d56e805fef4f79 93408 e2fsprogs_1.46.2-2+deb11u1.debian.tar.xz
Checksums-Sha256:
 f79f26b4f65bdc059fca12e1ec6a3040c3ce1a503fb70eb915bee71903815cd5 9496954 e2fsprogs_1.46.2.orig.tar.gz
 948552550f23a9e0223cecb51b5b85258c9d94895a20bce1180fce770628a55f 488 e2fsprogs_1.46.2.orig.tar.gz.asc
 48b50e91479754043fe442c0bdd4ba7cddc8e5b2ff0b93ca001b1a6b05067b80 93408 e2fsprogs_1.46.2-2+deb11u1.debian.tar.xz
Files:
 e8ef5fa3b72557be5e9fe564a25da6eb 9496954 e2fsprogs_1.46.2.orig.tar.gz
 cc53a1867cb8b23f1c74453ddf9110d9 488 e2fsprogs_1.46.2.orig.tar.gz.asc
 aa1cc92536351fb2609e03a529c9def9 93408 e2fsprogs_1.46.2-2+deb11u1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCgAdFiEEOvp1f6xuoR0v9F3wiNJCh6LYmLEFAmb/49AACgkQiNJCh6LY
mLFz4BAAts/IUPIKOU65DPSMOPtD/8ZidfCuikyv6wHf33JUQ91M/jLXjmAdlt+o
eSG6ln9RxI4JRsFzCQjlx29BY8Ww9A55UImVltdzfNk++HxEISDE6WCfGHKHd6gD
fUg0H+5RE/3NkUSX/YYSBchS4vxdHBghKTGAJ4hNVuFdbfw1KCWcyTDG2xaFW4+h
obghn0CfsgMFJl0yjjUrvmnkHNKex96+A7X3IkAJKGug0DPZUJOTn5GzoawErzzO
3/OxeeGC/BTZGuIDoQLao9PB9rCx3b85CxAmslTNhzUhvixGLXOVuEsMRwy8Cuam
McFJqlATZuL7LbjO761va+J1PcLN/AcXL9gtLoZthQweq2f/POSe2Hc3Thc1HYp5
rhHireVwIa/rCnJEfdteyP6rnsDo3NN5JEX96JkVwGDiyR1iH9AykIH/CK88v4c/
us+ufsrKU/6dIAWc22uM8lOrg4hzf8alCuj9s4xP9WU0KvgS4xUkEngB4+lbsIQx
S4jGZflOgcsPdgV2SLSq3it1zVcia33+86wJBRpYc2Yn/ul7bK3gjH0KoIN5rchE
B0pYWUTHwkmJ+inV+RsqxwRV0Z/A96BNa0/nE2GGU81eM4qQgwB7e3NiWZY6yocz
8ln4He5SmZDSgcO+j4W/XEZ9kJmN0FubGDMz99aofaDnLiShGLQ=
=2DtY
-----END PGP SIGNATURE-----
