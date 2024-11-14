-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: six
Binary: python-six, python3-six, pypy-six, python-six-doc
Architecture: all
Version: 1.16.0-2
Maintainer: Debian Python Team <team+python@tracker.debian.org>
Uploaders: Colin Watson <cjwatson@debian.org>
Homepage: https://pypi.python.org/pypi/six
Standards-Version: 4.5.0
Vcs-Browser: https://salsa.debian.org/python-team/packages/six
Vcs-Git: https://salsa.debian.org/python-team/packages/six.git
Testsuite: autopkgtest-pkg-python
Build-Depends: debhelper-compat (= 12), dh-python, pypy, pypy-setuptools, python2 (>= 2.6.6-3~), python-all, python-setuptools, python3 (>= 3.1.2-8~), python3-all, python3-pytest, python3-setuptools, python3-sphinx
Package-List:
 pypy-six deb python optional arch=all
 python-six deb python optional arch=all
 python-six-doc deb doc optional arch=all
 python3-six deb python optional arch=all
Checksums-Sha1:
 06fa0bb50f2a4e2917fd14c21e9d2d5508ce0163 34041 six_1.16.0.orig.tar.gz
 e25d4abd800c3cc72c4834406dde46e0e927871f 4892 six_1.16.0-2.debian.tar.xz
Checksums-Sha256:
 1e61c37477a1626458e36f7b1d82aa5c9b094fa4802892072e49de9c60c4c926 34041 six_1.16.0.orig.tar.gz
 35c7fe199852a2be2575d04d787adabeeb157a72a473ce0295bc7f40df6f26a6 4892 six_1.16.0-2.debian.tar.xz
Files:
 a7c927740e4964dd29b72cebfc1429bb 34041 six_1.16.0.orig.tar.gz
 af4193ede3c3dffa12a245d1bf55ae34 4892 six_1.16.0-2.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iHUEARYKAB0WIQTumtb5BSD6EfafSCRHew2wJjpU2AUCYQArXQAKCRBHew2wJjpU
2OeyAQC6fu6jyB7ym3dGHeO6KEdFy24RCHK350c4gw0tqmIVsQEA9Xv8+quY66g7
yg+oD/RdMgjki9vLFSJNcY2FBIYmpAw=
=q+T9
-----END PGP SIGNATURE-----
