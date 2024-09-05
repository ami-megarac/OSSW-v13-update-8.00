-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: python3.9
Binary: python3.9, python3.9-venv, libpython3.9-stdlib, python3.9-minimal, libpython3.9-minimal, libpython3.9, python3.9-examples, python3.9-dev, libpython3.9-dev, libpython3.9-testsuite, idle-python3.9, python3.9-doc, python3.9-dbg, libpython3.9-dbg, python3.9-full
Architecture: any all
Version: 3.9.2-1
Maintainer: Matthias Klose <doko@debian.org>
Standards-Version: 4.5.1
Vcs-Browser: https://salsa.debian.org/cpython-team/python3/tree/python3.9
Vcs-Git: https://salsa.debian.org/cpython-team/python3.git -b python3.9
Testsuite: autopkgtest
Testsuite-Triggers: build-essential, python3-setuptools, shunit2, virtualenv
Build-Depends: debhelper (>= 11), dpkg-dev (>= 1.17.11), quilt, autoconf, lsb-release, sharutils, libreadline-dev, libncursesw5-dev (>= 5.3), zlib1g-dev, libbz2-dev, liblzma-dev, libgdbm-dev, libdb-dev, tk-dev, blt-dev (>= 2.4z), libssl-dev, libexpat1-dev, libmpdec-dev (>= 2.5.1~), libbluetooth-dev [linux-any] <!pkg.python3.9.nobluetooth>, locales-all, libsqlite3-dev, libffi-dev (>= 3.0.5) [!or1k !avr32], libgpm2 [linux-any], media-types | mime-support, netbase, bzip2, time, python3:any, python3.9:any <cross>, net-tools, xvfb <!nocheck>, xauth <!nocheck>, systemtap-sdt-dev
Build-Depends-Indep: python3-sphinx, python3-docs-theme, texinfo
Build-Conflicts: git
Package-List:
 idle-python3.9 deb python optional arch=all
 libpython3.9 deb libs optional arch=any
 libpython3.9-dbg deb debug optional arch=any
 libpython3.9-dev deb libdevel optional arch=any
 libpython3.9-minimal deb python optional arch=any
 libpython3.9-stdlib deb python optional arch=any
 libpython3.9-testsuite deb libdevel optional arch=all
 python3.9 deb python optional arch=any
 python3.9-dbg deb debug optional arch=any
 python3.9-dev deb python optional arch=any
 python3.9-doc deb doc optional arch=all
 python3.9-examples deb python optional arch=all
 python3.9-full deb python optional arch=any
 python3.9-minimal deb python optional arch=any
 python3.9-venv deb python optional arch=any
Checksums-Sha1:
 110ca5bca7989f9558a54ee6762e6774a4b9644a 18889164 python3.9_3.9.2.orig.tar.xz
 de26dff3e34ff621d6d42e8b82da83cc67771a3d 211484 python3.9_3.9.2-1.debian.tar.xz
Checksums-Sha256:
 3c2034c54f811448f516668dce09d24008a0716c3a794dd8639b5388cbde247d 18889164 python3.9_3.9.2.orig.tar.xz
 b8b9e1710ca5dc5b0f0d9734494024ea11f560e36c34eb9191ccb0605798a490 211484 python3.9_3.9.2-1.debian.tar.xz
Files:
 f0dc9000312abeb16de4eccce9a870ab 18889164 python3.9_3.9.2.orig.tar.xz
 3b623d731925bc5a87408c4ffecec792 211484 python3.9_3.9.2-1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQJEBAEBCAAuFiEE1WVxuIqLuvFAv2PWvX6qYHePpvUFAmA7zWsQHGRva29AZGVi
aWFuLm9yZwAKCRC9fqpgd4+m9Yy8EACEO12hlph5mP1WRKe1CrjbumxPhkhh8r3T
6CJWZ5tgpDw5FTOHfg3royE1QGc1Na4+Iz2i1AONG0ksRCpEEkD9XWZ5AOxL9t9n
uiNgp5fzy9Docr8/NJiL/lhUhLrdP0idFFQGP7pDJ0kSkYRN4ORBApHSBfRHPWYn
BdF1yFDzracxVQC34Zew+EAsqBdrHzi0aFaxrtRgDxJlM3XkpYMIDkl6ktQMJ25e
VCVDbj+1Xs4vXSm52fALwTeR5ACJnHz85GAQ0PIbG65qitA3aMbK83c0926vpzLA
HmjwLzhR3Qp2RR5GeKV45VEvzkHwd0KGQ5YJzSJ54hVFTKINFNc8a0y0TJ4guAvT
T1OuJshNqRAFHEDgdVz6fiLP8NnCeHGT+Cd/LyqvXPKTSW5pQqqjY7KEHcMnSlPP
IJUmMhU3KxC6chmbcFhDt/kk7g3uBOmBZkECq8flsNJetfIKkbl119jYPvNDzsOL
J7H6RctjjCnQx4xErerOpZp+gWOxgJ+FgtN2buw6r/v0Iq6hKRH5Ii/E2FJAUrRF
cHH0KAVlgiSYk0wGFXBumnaM0aelZFsJ5fcuv/HUntMnqRI7AW5q1RjJYLPryhz9
r9TCH2RWXk8TBe1tkl7L4cuLR98X5GAg4s9uXwEnc6CXiMOGHg0YEj8gQdWogXRj
AdpLby3NNQ==
=o89v
-----END PGP SIGNATURE-----
