-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: pypy3
Binary: pypy3, pypy3-tk, pypy3-lib, pypy3-lib-testsuite, pypy3-doc, pypy3-dev
Architecture: any all
Version: 7.3.5+dfsg-2+deb11u2
Maintainer: Stefano Rivera <stefanor@debian.org>
Homepage: https://www.pypy.org/
Standards-Version: 4.5.1
Vcs-Browser: https://salsa.debian.org/debian/pypy3
Vcs-Git: https://salsa.debian.org/debian/pypy3.git
Testsuite: autopkgtest
Testsuite-Triggers: build-essential, python-pip-whl, python3-setuptools, shunit2, virtualenv
Build-Depends: debhelper (>= 9.20141010~), dh-exec, dpkg-dev (>= 1.17.14~), libbz2-dev, libexpat1-dev, libffi-dev, libgdbm-dev, liblzma-dev, libncurses-dev, libsqlite3-dev, libssl-dev | libssl1.0-dev, netbase, pkg-config, procps, pypy [any-amd64 any-i386 arm64 armhf ppc64 ppc64el s390x], python-pip-whl, python2, python3-dev, python3-sphinx (>= 1.0.7+dfsg), tcl-dev, tk-dev, valgrind [amd64 arm64 armhf i386 mips mips64el mipsel powerpc ppc64 ppc64el s390x], zlib1g-dev
Build-Depends-Indep: graphviz
Package-List:
 pypy3 deb python optional arch=any
 pypy3-dev deb python optional arch=all
 pypy3-doc deb doc optional arch=all
 pypy3-lib deb python optional arch=any
 pypy3-lib-testsuite deb python optional arch=all
 pypy3-tk deb python optional arch=any
Checksums-Sha1:
 cdbfcdbe0de3a82197db830d51831312466375fe 15063080 pypy3_7.3.5+dfsg.orig.tar.xz
 f3778ab46c1b30904ebf35686dcf26841275137d 49872 pypy3_7.3.5+dfsg-2+deb11u2.debian.tar.xz
Checksums-Sha256:
 73e1580c9e3c406e183ec84879e76d04ba184e962dee6adc62df146685b93f0d 15063080 pypy3_7.3.5+dfsg.orig.tar.xz
 3acba1e200732bea5e0fbebd2f6def50e3928782643c0b8c28cdd01288e84736 49872 pypy3_7.3.5+dfsg-2+deb11u2.debian.tar.xz
Files:
 58ab7a168ad61cdb4a67615e0b76ab8e 15063080 pypy3_7.3.5+dfsg.orig.tar.xz
 a38bb10a0aa516b0339364788af74568 49872 pypy3_7.3.5+dfsg-2+deb11u2.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iIoEARYKADIWIQTumtb5BSD6EfafSCRHew2wJjpU2AUCY2F2hRQcc3RlZmFub3JA
ZGViaWFuLm9yZwAKCRBHew2wJjpU2D0CAP9nNkmg54wPrZPAt0Zj0D5LjH7iQhJX
cI8y3mZs5vK7DAD/VVSwbD9m4qJ5C5WzOx1uw0+vsvSkp2QKCGhrM5k1dAI=
=Y9Me
-----END PGP SIGNATURE-----
