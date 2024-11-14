-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: gnutls28
Binary: libgnutls28-dev, libgnutls30, gnutls-bin, gnutls-doc, libgnutlsxx28, libgnutls-openssl27, libgnutls-dane0, guile-gnutls
Architecture: any all
Version: 3.7.1-5+deb11u5
Maintainer: Debian GnuTLS Maintainers <pkg-gnutls-maint@lists.alioth.debian.org>
Uploaders:  Andreas Metzler <ametzler@debian.org>, Eric Dorland <eric@debian.org>, James Westby <jw+debian@jameswestby.net>, Simon Josefsson <simon@josefsson.org>
Homepage: https://www.gnutls.org/
Standards-Version: 4.5.1
Vcs-Browser: https://salsa.debian.org/gnutls-team/gnutls
Vcs-Git: https://salsa.debian.org/gnutls-team/gnutls.git
Testsuite: autopkgtest
Testsuite-Triggers: build-essential, ca-certificates, datefudge, freebsd-net-tools, net-tools, openssl, softhsm2
Build-Depends: autogen (>= 1:5.16-0), bison, ca-certificates <!nocheck>, chrpath, datefudge <!nocheck>, debhelper-compat (= 13), freebsd-net-tools [kfreebsd-i386 kfreebsd-amd64] <!nocheck>, gperf, guile-2.2-dev <!noguile>, libcmocka-dev <!nocheck>, libgmp-dev (>= 2:6), libidn2-dev, libopts25-dev, libp11-kit-dev (>= 0.23.10), libssl-dev <!nocheck>, libtasn1-6-dev (>= 4.9), libunbound-dev (>= 1.5.10-1), libunistring-dev (>= 0.9.7), net-tools [!kfreebsd-i386 !kfreebsd-amd64] <!nocheck>, nettle-dev (>= 3.6), openssl <!nocheck>, pkg-config, softhsm2 <!nocheck>
Build-Depends-Indep: gtk-doc-tools, texinfo (>= 4.8), texlive-plain-generic, texlive-latex-base
Build-Conflicts: libgnutls-dev
Package-List:
 gnutls-bin deb net optional arch=any
 gnutls-doc deb doc optional arch=all
 guile-gnutls deb lisp optional arch=any profile=!noguile
 libgnutls-dane0 deb libs optional arch=any
 libgnutls-openssl27 deb libs optional arch=any
 libgnutls28-dev deb libdevel optional arch=any
 libgnutls30 deb libs optional arch=any
 libgnutlsxx28 deb libs optional arch=any
Checksums-Sha1:
 5de5d25534ee5910ea9ee6aaeeb6af1af4350c1e 6038388 gnutls28_3.7.1.orig.tar.xz
 8c2c3aabe289987bbe51ddc1ad4a42558683ca66 854 gnutls28_3.7.1.orig.tar.xz.asc
 84ef766db919cac520c54dd4b5c1a80391f480ac 100560 gnutls28_3.7.1-5+deb11u5.debian.tar.xz
Checksums-Sha256:
 3777d7963eca5e06eb315686163b7b3f5045e2baac5e54e038ace9835e5cac6f 6038388 gnutls28_3.7.1.orig.tar.xz
 13a683b12602c169a7ad7827ab0e3f35c8fa1f98675d0073cf7d54a8cd635582 854 gnutls28_3.7.1.orig.tar.xz.asc
 b50f57600d68d03fe88f116db6a103647d4f361c7f1909c23ce75cf4454567ec 100560 gnutls28_3.7.1-5+deb11u5.debian.tar.xz
Files:
 278e1f50d79cd13727733adbf01fde8f 6038388 gnutls28_3.7.1.orig.tar.xz
 590c9d64f7d8ee77671cdb9547f5edaf 854 gnutls28_3.7.1.orig.tar.xz.asc
 065ebd3f5c3e47d873d0984dc6bea098 100560 gnutls28_3.7.1-5+deb11u5.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCgAdFiEE0uCSA5741Jbt9PpepU8BhUOCFIQFAmWrqB0ACgkQpU8BhUOC
FIQK3Q//TKrIq4KsaqHi6Vtoht1UjcBZzCWVXiPgRUkwvnT9jhTHr6+0lPNVGDqN
3cUSN/1lkYs6CWlSZto1YQgLbs1fn/a5wBVzQXNA5eP1EUuOv1vnho7DGxmnGtbL
wAaKjpvNKfxsFC9kO9BloglXfz1WTJvL9A2tCk42ala9KBxdRB3sAjCMNGCC+JPy
p2FEEJMV5UvHKL9BrCsojwbuiFwzeNTRnCVMxCTKGXVQREPaY1X335OU1cFXDq3r
k8O5V5OvgSCNxIj1joENB87VVrR8MUp60su9+ih2JyyEAnALhn29txsf9QzQEjT7
spNRtG8pgDynzx8I+z3XF8RgGosQbMCy+cDyyME82QlEcpaX4CWAjl2DkbHS3tS5
t9lePMO3P9wj9dUyKmCwzlCVkNaOXwV0mOMYluasuBSQZ1/6OavCTyRekJoiforK
YuuQousrST3DoTkxFrYOXEWPXhjeRNsOgcJwDUeNgjey8YajrvThXb4hZXidxgde
QjdW/jHhnM0X82nBFuepXQugChAHuf3TDR7ELnTSpVESnJnVm83D9dKJLMmx+wOY
jTemx9P4xNgZJ9hogl4o5AHs4tDGg6JsToXClk1qzlQss5za4aa63D2e0f7aQ3KP
ixOeX77H/BmvQGj8DG4td9dteyp0IbmycTyM+3YSA9QBy6I7GNA=
=gBsI
-----END PGP SIGNATURE-----
