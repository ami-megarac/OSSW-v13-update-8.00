-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: openmpi
Binary: openmpi-bin, libopenmpi-dev, libopenmpi3, openmpi-common, openmpi-doc
Architecture: any all
Version: 4.1.0-10
Maintainer: Alastair McKinstry <mckinstry@debian.org>
Uploaders: Manuel Prinz <manuel@debian.org>
Homepage: https://www.open-mpi.org/
Standards-Version: 4.5.1
Vcs-Browser: https://salsa.debian.org/hpc-team/openmpi
Vcs-Git: https://salsa.debian.org/hpc-team/openmpi.git -b debian/latest
Testsuite: autopkgtest
Testsuite-Triggers: gcc, gfortran
Build-Depends: dpkg-dev (>= 1.16.1~), dh-sequence-fortran-mod, debhelper-compat (= 13), libevent-dev, libpmix-dev, libucx-dev [amd64 arm64 ppc64el], zlib1g-dev, gfortran, libhwloc-dev, pkg-config, libibverbs-dev (>= 1.1.7) [!kfreebsd-i386 !kfreebsd-amd64 !hurd-i386], libfabric-dev [!kfreebsd-i386 !kfreebsd-amd64 !hurd-i386], libnuma-dev [!kfreebsd-i386 !kfreebsd-amd64 !hurd-i386], libpsm-infinipath1-dev [amd64 i386], libpsm2-dev [amd64], default-jdk [!hppa !hurd-i386] <!stage1>, flex
Build-Conflicts: libmpich-dev, libopenmpi-dev
Package-List:
 libopenmpi-dev deb libdevel optional arch=any
 libopenmpi3 deb libs optional arch=any
 openmpi-bin deb net optional arch=any
 openmpi-common deb net optional arch=all
 openmpi-doc deb doc optional arch=all
Checksums-Sha1:
 e81eec53a4397494eb6450d6306a67dde47e0a53 6725344 openmpi_4.1.0.orig.tar.xz
 04c328e4b9cf63490b599e42e37761d4be0e118a 69288 openmpi_4.1.0-10.debian.tar.xz
Checksums-Sha256:
 2c49d569d7c0c95ad49054aefb78ba7b1e235d3547314fc4802b1eb793665103 6725344 openmpi_4.1.0.orig.tar.xz
 aa3354333b2228ac44fbf8dce90fd39fad7cfdac4017720903194252a69da671 69288 openmpi_4.1.0-10.debian.tar.xz
Files:
 6b2b2fdc2fc433453fdcf8efab5d5fa3 6725344 openmpi_4.1.0.orig.tar.xz
 71f31345ce1985754418268f4100233b 69288 openmpi_4.1.0-10.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCAAdFiEEgjg86RZbNHx4cIGiy+a7Tl2a06UFAmCvaUUACgkQy+a7Tl2a
06WMoA//WiVXGcaeAqgCb9yD0xctZOm3e80GscZjfCPtSxgmOqfCcLh6A+Fz4FcJ
R3WSHQcPgOduDXq+4lLH1IA9xuj8WIU3TFrNBEVTnlV56CE2KwUcokXdDl0kchP5
M+qkNmLjYP+19saIJQE1d7qQnApEaNpxwYd8ObVbx2arBRkgmFjvjjYBkzUsTGLx
grDcmcB+xNrXexW5Y28utNrkuCGnUa2S4dqz9b/nBHSQzsW58Tts8AGLCOKC8ep4
5ePVfHz5fdEHdAZjUkw2VmOuQ4ipG0F5+l5IYD20waLbG7YQx23A+AtpASeCc9DZ
uo2Rc5lqtImz9wNup/tjn8zwDFTrHmZxDPU/TqXvO41TcJTzay7itgtzPcrsuZ5c
zmxLGFoC/KN1fKCsTtk+scQu9nopujkfxvDgeY4yRxJxICPPV4FkntXeiLsIH+cE
h6CS1oHFVaOd664vOJPEsgItIs4zfe23lTE3xR+RyxdVHFARQZHGL/xoM2jzbMN8
3HFjJuP4JJf+Mifzy3xbm/klMO3YvcC41VEjwNZ4/xldlo8OWE0hFa0yyxpv/tl/
dZcD61heq6SoPaVvjAW3V8sHSzOJnfJRIPE3Hqk9PlQxWqjeqKfz9njtcEc6wxM5
fg391pfbXOgb6E5CkkvgPorwIeozszxOy9OoFhNKQlTNi27HPXE=
=gfuG
-----END PGP SIGNATURE-----
