-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: elfutils
Binary: elfutils, libelf1, libelf-dev, libdw-dev, libdw1, libasm1, libasm-dev, libdebuginfod1, libdebuginfod-dev, debuginfod
Architecture: any
Version: 0.183-1
Maintainer: Debian Elfutils Maintainers <debian-gcc@lists.debian.org>
Uploaders: Kurt Roeckx <kurt@roeckx.be>, Matthias Klose <doko@debian.org>
Homepage: https://sourceware.org/elfutils/
Standards-Version: 4.5.1
Vcs-Browser: https://salsa.debian.org/toolchain-team/elfutils
Vcs-Git: https://salsa.debian.org/toolchain-team/elfutils.git
Build-Depends: debhelper (>= 11), autoconf, automake, bzip2, zlib1g-dev, zlib1g-dev:native, libbz2-dev, liblzma-dev, m4, gettext, gawk, dpkg-dev (>= 1.16.1~), gcc-multilib [any-amd64 sparc64] <!nocheck>, libc6-dbg [powerpc powerpcspe ppc64 ppc64el armel armhf arm64 sparc64 riscv64], flex, bison, pkg-config, libarchive-dev <!pkg.elfutils.nodebuginfod>, libmicrohttpd-dev <!pkg.elfutils.nodebuginfod>, libcurl4-gnutls-dev <!pkg.elfutils.nodebuginfod>, libsqlite3-dev <!pkg.elfutils.nodebuginfod>
Build-Conflicts: autoconf2.13
Package-List:
 debuginfod deb devel optional arch=any profile=!pkg.elfutils.nodebuginfod
 elfutils deb utils optional arch=any
 libasm-dev deb libdevel optional arch=any
 libasm1 deb libs optional arch=any
 libdebuginfod-dev deb libdevel optional arch=any profile=!pkg.elfutils.nodebuginfod
 libdebuginfod1 deb libs optional arch=any profile=!pkg.elfutils.nodebuginfod
 libdw-dev deb libdevel optional arch=any
 libdw1 deb libs optional arch=any
 libelf-dev deb libdevel optional arch=any
 libelf1 deb libs optional arch=any
Checksums-Sha1:
 20227ae4cc7474de505ddf0f1f0b1b24ce5198e7 9109254 elfutils_0.183.orig.tar.bz2
 0bc19533073f537019d48d2816057dd0ad6046b4 33396 elfutils_0.183-1.debian.tar.xz
Checksums-Sha256:
 c3637c208d309d58714a51e61e63f1958808fead882e9b607506a29e5474f2c5 9109254 elfutils_0.183.orig.tar.bz2
 e64d82677a9f12344210f2294f8738ce649fbac0ebec20f2354f39d8bd4698ce 33396 elfutils_0.183-1.debian.tar.xz
Files:
 6f58aa1b9af1a5681b1cbf63e0da2d67 9109254 elfutils_0.183.orig.tar.bz2
 735202b8088003db97b428c5c2f834aa 33396 elfutils_0.183-1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQJEBAEBCAAuFiEE1WVxuIqLuvFAv2PWvX6qYHePpvUFAmAhBpwQHGRva29AZGVi
aWFuLm9yZwAKCRC9fqpgd4+m9RFRD/0f4DWyXQ02V29PP/0NDua10YyOSq+xxOOt
gJ43LKaPvTPD8Et4/0o8zAw78E3Dj6hrENpzUNKF3oGsTSova1S7/cB7nXWp4vsA
uYwi8UnVZwGO6CTTT1TzH0BQR6MWlpaX7Bcewr4ynSmXuR+mFjWlT1q0P5xNvGz7
V61JiP7Xez1le40TRuWgurskWzNG+9IsJRQR/Jzl7SFSNKit79hInX+ZFUwds9B5
RQON8gW4hhgGDc+4m7qaQVHKG4Synw2luAjgbQT9ZvKib/SXyOPg6dj1CaQmEMv6
oJ6fRGqtHZGSlpUNkPVzJ/iddLP150T7quvydOY9BMEYmqz8zEnPEfgU+OLTk/rx
cgMCv6bXQvHH/oFisQCyOtBIXDV0TVPvYPDp+TvJJUU8488MFpzDIZhAokgQzILY
Drwu3HrFK08bSbtjJQqO1fSdJ0NXApSVFfd6oauDcrSpxYv6FBLpeaZhPa4SbTaY
MH9hc1tEVWedU2q1UeDTSZGYKaKnUi3CtbWI40SCyBrxrIecTZ2wf3ubUuq1so3b
S5HPD9/WdjVOAD4RYQuzCrFwjylVzF7XRHuC9m3GxIglh41kyMbdvJnb/riSVFnM
zBm4OvALsY3ter3fDg15JoxLNo1WHahCLy0AeTgpRRdva3iLJ/2FEGwbX1t6rTNJ
AFjayTpHVg==
=32XA
-----END PGP SIGNATURE-----
