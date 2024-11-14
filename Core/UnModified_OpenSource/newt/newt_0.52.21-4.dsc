-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: newt
Binary: libnewt-dev, libnewt-pic, libnewt0.52-udeb, newt-tcl, python3-newt, libnewt0.52, whiptail
Architecture: any
Version: 0.52.21-4
Maintainer: Alastair McKinstry <mckinstry@debian.org>
Homepage: https://pagure.io/newt
Standards-Version: 4.4.1.0
Vcs-Browser: https://salsa.debian.org:/mckinstry/newt.git
Vcs-Git: https://salsa.debian.org:/mckinstry/newt.git
Build-Depends: debhelper-compat (= 12), docbook-utils, automake (>= 1.16), libslang2-dev (>= 2.0.4-2), libpopt-dev, gettext, libfribidi-dev, tcl8.6-dev, dh-python <!stage1>, python3-all-dev:any <!stage1>, libpython3-all-dev <!stage1>
Package-List:
 libnewt-dev deb libdevel optional arch=any
 libnewt-pic deb libdevel optional arch=any
 libnewt0.52 deb libs optional arch=any
 libnewt0.52-udeb udeb debian-installer optional arch=any
 newt-tcl deb interpreters optional arch=any
 python3-newt deb python optional arch=any profile=!stage1
 whiptail deb utils optional arch=any
Checksums-Sha1:
 6c5995432fce9976328eb4799059cf5c540f935b 112160 newt_0.52.21.orig.tar.xz
 f3fcf2f036808e3da3a08b15fe18ff4dbc1eff3a 37920 newt_0.52.21-4.debian.tar.xz
Checksums-Sha256:
 1099418091a7f66b7164c9d2960e233619af5a299d6fdd91a4f58534a9773d13 112160 newt_0.52.21.orig.tar.xz
 163f2f58bf4d0ac8a0907a1c2530a02d7c178b88c53fb98ee69d4b33bc86187d 37920 newt_0.52.21-4.debian.tar.xz
Files:
 452999475bf543db9c270911a7f6ddd3 112160 newt_0.52.21.orig.tar.xz
 6b089a6d785f2e1941cc7d24b7f42f38 37920 newt_0.52.21-4.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCAAdFiEEgjg86RZbNHx4cIGiy+a7Tl2a06UFAl36AUAACgkQy+a7Tl2a
06W3vg//dRhOAZVjZQEjNmPE1ogb4YefBjWkVE8rZNg1cMqPXF3QNXctA1Nk+KfF
MGf7gXBnrPG2bQTdYkLFjLg/byrwnm2HhhruKl6nPDYxz9Djt4gpsFI3+gRhIn6k
vopbx0YAlXb+7oaGgB1Sx9Z++12INfPMIiDRkCdvOG/oNp7yTw42BsgnhvEMwTwS
ZPyZ3jbLZ8He4yMrp1Ate7dvHJyhzbTT5U7NUc8bGaOCDQzPqWpEsJLHQLYJB2vw
vgI72L5Nln+5pdxdCW/Qvsu+U3MvnLDJZO/Wnjhkixj1uwBylR9Amn8UCOm5wtVj
W2xrPjWEA7JvCmVYqZkOpibzvEPoqPsbrSTfpCfUGhLNA8vuFVSUvUKSHib+Y4TK
hotts16OWOHAG/H7h2/HJmyzo3vzTOZKfS+OW8V2nAImaXFNRz8i5LTs9bxQgrgB
NYJ6ef1c93qhZW3tyrBUSQfkvqS4FHNfzgsi91f4EOZ7VxLkb/amylxM3CwT3hxA
vyxkXX0mUJOltLa2RNdolUbJ+aVY21UJyKegVJdWC4ZqUiE1YyEug2mimIneMC2r
Lt9qY2RM3E+SjgL235Wo3ahjuX7+VvKLL/rIEAtGxaum9ZxGsY1ViGarKQOeZNuc
f1CQqZ0lT1k8+jH5QHW1rONTFV2EBnxyFSeZyDNT+X2vNJcNiS0=
=MSzG
-----END PGP SIGNATURE-----
