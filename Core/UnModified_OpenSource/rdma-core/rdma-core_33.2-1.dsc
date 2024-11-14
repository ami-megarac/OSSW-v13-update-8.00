-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: rdma-core
Binary: rdma-core, ibacm, ibverbs-providers, ibverbs-utils, libibverbs-dev, libibverbs1, libibumad-dev, libibumad3, librdmacm-dev, librdmacm1, rdmacm-utils, srptools, python3-pyverbs, infiniband-diags, libibmad5, libibmad-dev, libibnetdisc5, libibnetdisc-dev
Architecture: linux-any
Version: 33.2-1
Maintainer: Benjamin Drung <benjamin.drung@ionos.com>
Homepage: https://github.com/linux-rdma/rdma-core
Standards-Version: 4.5.1
Vcs-Browser: https://github.com/linux-rdma/rdma-core
Vcs-Git: https://github.com/linux-rdma/rdma-core.git -b debian/master
Build-Depends: cmake (>= 2.8.11), cython3, debhelper-compat (= 13), dh-python, dpkg-dev (>= 1.17), libnl-3-dev, libnl-route-3-dev, libsystemd-dev, libudev-dev, ninja-build, pandoc, pkg-config, python3-dev, python3-docutils, valgrind [amd64 arm64 armhf i386 mips mips64el mipsel powerpc ppc64 ppc64el s390x]
Package-List:
 ibacm deb net optional arch=linux-any
 ibverbs-providers deb net optional arch=linux-any
 ibverbs-utils deb net optional arch=linux-any
 infiniband-diags deb net optional arch=linux-any
 libibmad-dev deb libdevel optional arch=linux-any
 libibmad5 deb libs optional arch=linux-any
 libibnetdisc-dev deb libdevel optional arch=linux-any
 libibnetdisc5 deb libs optional arch=linux-any
 libibumad-dev deb libdevel optional arch=linux-any
 libibumad3 deb libs optional arch=linux-any
 libibverbs-dev deb libdevel optional arch=linux-any
 libibverbs1 deb libs optional arch=linux-any
 librdmacm-dev deb libdevel optional arch=linux-any
 librdmacm1 deb libs optional arch=linux-any
 python3-pyverbs deb python optional arch=linux-any
 rdma-core deb net optional arch=linux-any
 rdmacm-utils deb net optional arch=linux-any
 srptools deb net optional arch=linux-any
Checksums-Sha1:
 c18ed44a87d32e577c260540d3ad1cf6d8b7b849 1759965 rdma-core_33.2.orig.tar.gz
 e95578b628680f473701deccf0962bbdcf4c5efd 22580 rdma-core_33.2-1.debian.tar.xz
Checksums-Sha256:
 bc1ed3c785edc0932d7ea0e06401b6c8821d811bd199e0dcef220c5bbd6a28d6 1759965 rdma-core_33.2.orig.tar.gz
 bfb6283a2fd771b319f8aee27caf8e702a77c31378996300fc57465bb5f7cebb 22580 rdma-core_33.2-1.debian.tar.xz
Files:
 ccbc7a8532a2c63185bcc194e5c33a29 1759965 rdma-core_33.2.orig.tar.gz
 12ca431f14c443bfa623f218b7b83cf5 22580 rdma-core_33.2-1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCgAdFiEE5/q3CzlQJ15towl13YzVpd6MfnoFAmC4n9UACgkQ3YzVpd6M
fnoLpxAAkNAFskwFcrAvmkVdfNGA7CmXqCFOdPwl/w9j9guzD14qMWO+xwUf70mR
5036diKI7tuLY2rzgH0z/0uZRt+SSoUQx9bAhBAVHZSD6xbJq1MQgEU9plO7oUgi
siC+HeIRLRUCpuA8FDPXPUkkt8xGBDOvGWdBLrdFvzREoNPNiuxDAi6fMIorVZy8
tlnBdPUjDlqb4UkyE7ypboQjlGcebg5finmz5DlUhAT/A33YRMOdulRhv5j7cgf/
IRpAdD5Nm2uKqKYHXG75UMxhMPpMHd55G3NcmZy3u+zosAgKIulbOCYB5+3RHhsj
P5QhM/S3yQtervIkbUSvgDLn6IiZqOPHvnQAvMZNTse9+D8YER6R7Q4sc/SlTcLM
WJCFhJubbsGXqiwhdJCZGOD+YXCeQI1yibEU1YFXibTbqjijzlL7rjyLSZE6aF/E
8cOT100GLL3Lg9dsSnqqeL79NQpR3i3Je5umVM29G5Ry325STy4IrXUW+uDEGUZ6
ms8Syoq9hcQA+sMxD7gyhLXDiAqU/pF4srggFuSMg49EqG9kT9gxqWGlLxph9n7D
g5Q8YfEZRRU87bFI5PLOEobgu19ysCXrs/1Z+NhkjEmnAgZ52gVqiknKRwOVUk40
EZLotAEtNbnpFXJ8X0ES3e7ZAhG1JZBUb2wah2bZMjsy0vfRwcA=
=V/4D
-----END PGP SIGNATURE-----
