-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: procps
Binary: procps, libprocps8, libprocps-dev
Architecture: any
Version: 2:3.3.17-5
Maintainer: Craig Small <csmall@debian.org>
Homepage: https://gitlab.com/procps-ng/procps
Standards-Version: 4.1.3
Vcs-Browser: https://salsa.debian.org/debian/procps
Vcs-Git: https://salsa.debian.org/debian/procps.git
Testsuite: autopkgtest
Build-Depends: debhelper-compat (= 13), dh-exec (>= 0.3), libncurses5-dev, libncursesw5-dev, dejagnu, libnuma-dev [amd64 i386 ia64 mips mipsel mips64 mips64el powerpc ppc64el], pkg-config, libsystemd-dev (>= 209) [linux-any]
Package-List:
 libprocps-dev deb libdevel optional arch=any
 libprocps8 deb libs optional arch=any
 procps deb admin important arch=any
Checksums-Sha1:
 a52952e8bc6aaab812176c00d25adc4d4e1552e2 1008428 procps_3.3.17.orig.tar.xz
 b06e5b4609c3253a9c903dcf7d426208648bcc21 28608 procps_3.3.17-5.debian.tar.xz
Checksums-Sha256:
 4518b3e7aafd34ec07d0063d250fd474999b20b200218c3ae56f5d2113f141b4 1008428 procps_3.3.17.orig.tar.xz
 e6b5f9ef22eca9f03f79dc79b4c389249368216df8702a8cc380e10f29eda8c9 28608 procps_3.3.17-5.debian.tar.xz
Files:
 d60613e88c2f442ebd462b5a75313d56 1008428 procps_3.3.17.orig.tar.xz
 46ea1b4b1fe71652a402adbcb15150c0 28608 procps_3.3.17-5.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCgAdFiEEXT3w9TizJ8CqeneiAiFmwP88hOMFAmBsC9cACgkQAiFmwP88
hOM4Ow/+IZZcbd4c9/7dEfopWAfmeD37y7uaFAyo3lVs7/rJmPGR3YhesjxoFotd
w6nxsD+qlpE74DQLoT/+BzXsoQNfa9UJEIrF8TjeHv4lb92vtOZuJYqGoR0xCKYF
yBzvW23WbwdU0kgZMxGPxDkfcQUbrzy3bTXou/+eeUQW1bQX+OTA8zYOFZx3VaYH
wzjBdHh4ptTC4zW/yDbYQj8Z2nQeqfOYa2fINMpQEq2UUempNSo1wFBy9tcnOlXR
C4vH+p+EIOzQzc0WkYcXw394qvfK27/rdcfhdfFocfUy8jVnHtocvLYTOH1Uli77
8svePIlbI2Msy5JJ1G8usm4MKqj/9rFStpC+LY8v0hQsWo4gl+EiKC2OzvliSBAV
/sQ1rXj1yyufzACmI2Sb3MLplWrsteXYR+FkRxyyq8aayV+yeU2wq/lHOlagZ8P1
x1ea1qC3EmDXTqSmqSROvxHhqJLQNQqcrmHcRFLAcAXOVom6dehuOk2nQaobUmMM
QDQ5oxbPbvWL3o3UymAG2CkL6S1i6m5q15G7nJrDgg29MgDEBbsgXRF0TAfmseCH
gg7BqvbnJNRJu7a3J+vXsJVSQERgrSJCgoKmyOxQDd0sQbqD4GWo6i0lyMeDc2YM
MBFA0DwPmZzYoQKztJRRUzRwXMiqhmtuIgEtbmsXPk6xw7U0wzU=
=A41a
-----END PGP SIGNATURE-----
