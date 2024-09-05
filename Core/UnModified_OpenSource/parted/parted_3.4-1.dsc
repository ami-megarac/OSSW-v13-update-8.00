-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: parted
Binary: parted, parted-udeb, libparted2, libparted-fs-resize0, libparted2-udeb, libparted-fs-resize0-udeb, libparted-i18n, libparted-dev, parted-doc
Architecture: any all
Version: 3.4-1
Maintainer: Parted Maintainer Team <parted-maintainers@alioth-lists.debian.net>
Uploaders: Bastian Blank <waldi@debian.org>, Colin Watson <cjwatson@debian.org>
Homepage: http://www.gnu.org/software/parted
Standards-Version: 3.9.8
Vcs-Browser: https://salsa.debian.org/parted-team/parted
Vcs-Git: https://salsa.debian.org/parted-team/parted.git
Build-Depends: dpkg-dev (>= 1.15.7~), debhelper (>= 9.20150501~), debhelper-compat (= 9), dh-exec, libncurses-dev | libncurses5-dev, libreadline-dev | libreadline6-dev, libdevmapper-dev (>= 2:1.02.39) [linux-any], uuid-dev, gettext, texinfo (>= 4.2), debianutils (>= 1.13.1), libblkid-dev, pkg-config, check, dh-autoreconf, autoconf (>= 2.64), automake (>= 1:1.11.6), autopoint, gperf
Package-List:
 libparted-dev deb libdevel optional arch=any
 libparted-fs-resize0 deb libs optional arch=any
 libparted-fs-resize0-udeb udeb debian-installer optional arch=any
 libparted-i18n deb localization optional arch=all
 libparted2 deb libs optional arch=any
 libparted2-udeb udeb debian-installer optional arch=any
 parted deb admin optional arch=any
 parted-doc deb doc optional arch=all
 parted-udeb udeb debian-installer optional arch=any
Checksums-Sha1:
 903c58fab429d3b62aa324033a3e41b0b96ad810 1860300 parted_3.4.orig.tar.xz
 71266bb4dc9883728972466fae61fbc9e7d20b6f 508 parted_3.4.orig.tar.xz.asc
 d39ddc9b14c05fc1f11dbe4d8cb4f5b18462989d 56140 parted_3.4-1.debian.tar.xz
Checksums-Sha256:
 e1298022472da5589b7f2be1d5ee3c1b66ec3d96dfbad03dc642afd009da5342 1860300 parted_3.4.orig.tar.xz
 d830f6d27ef3e11723e9cafa3f4f3b6aaa8cb00ec6ebbfa9cf1e0cf991913257 508 parted_3.4.orig.tar.xz.asc
 0859660452fadbc4e0d841aa4b060d58f764b95911c1247dcb739ed9dc536862 56140 parted_3.4-1.debian.tar.xz
Files:
 357d19387c6e7bc4a8a90fe2d015fe80 1860300 parted_3.4.orig.tar.xz
 2148025ea942dd50b38bd896c5a27a63 508 parted_3.4.orig.tar.xz.asc
 f4149907c7c0c064ecc231273ef788de 56140 parted_3.4-1.debian.tar.xz
Dgit: 6469e72d3289f4a69c00aadaeac06a55752d4ba4 debian archive/debian/3.4-1 https://git.dgit.debian.org/parted

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCAAdFiEErApP8SYRtvzPAcEROTWH2X2GUAsFAmAUpIYACgkQOTWH2X2G
UAvAIRAAhMNPwTPQsR/sloOUUXDtmFqgg8jSLD5RGA05fbzdEbNOWBgSIPDx+G2v
MCC4pO1uWEUQh4cy8TMxUEFOerhN15B0W34v35RuRkIS5jFaVMwtPU8MKJW+KAW7
sJovocj89Z0bialUA2g33a9bN8AHT2GfYJKe6udlggC7WLHOBQdZsEF56gaBDgat
AxjjUAgxE1nZ1EccTACWbD55H8jNZ39CBb68IFgepOZWU9hVY+qlmohVdz3eySq/
LEPpOBvCVPjVskoKb6ysqoCYIpqRdA44CHgoNot1XZDyBlwDjt6tU7lhFqeGDPKV
xQI8YqLtYhAmOhFp6seCMNKDYb08jojkpkWHvvSSHIomPtzIbq7gntkrH3m9J4fZ
5SrOUTKg1AB6BCcWXGFz0ES8QHA65yIioEZFH+zAccROdqqfqXtwqPLVP7SI2nne
y3vWb5TSTPhg/N8+yDP8Gm4IBmsH26eW5As9NZ0m4i4IgzjzBD3t8l57I+Azny3/
60hopckXpy1EwSXL/l6MmA5MLYFiBtN1Fp+tZzaLCnkCfptkxqdkDW2jdSq9NmgW
WmeW5I0wa6iBFNm989237g/LiauEqZijPIdEZBy5DE0hJK+jmrixtdXxy8f1uG0a
dlxox2k+LG12topvkMk1+pMpEE/h1E5qwInv9lwRAIA0k7XpwHo=
=2yfU
-----END PGP SIGNATURE-----
