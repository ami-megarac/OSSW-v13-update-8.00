-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: python-cffi
Binary: python3-cffi, python3-cffi-backend, python3-cffi-backend-dbg, python-cffi-doc
Architecture: any all
Version: 1.14.5-1
Maintainer: Debian Python Team <team+python@tracker.debian.org>
Uploaders: Stefano Rivera <stefanor@debian.org>
Homepage: https://cffi.readthedocs.org/
Standards-Version: 4.5.1
Vcs-Browser: https://salsa.debian.org/python-team/packages/python-cffi
Vcs-Git: https://salsa.debian.org/python-team/packages/python-cffi.git
Testsuite: autopkgtest
Testsuite-Triggers: build-essential, python3-all-dbg, python3-all-dev, python3-py, python3-pytest, python3-setuptools, virtualenv
Build-Depends: debhelper-compat (= 12), dh-python, libffi-dev, python3-all-dbg, python3-all-dev, python3-py, python3-pycparser, python3-pytest, python3-setuptools, python3-sphinx, virtualenv
Package-List:
 python-cffi-doc deb doc optional arch=all
 python3-cffi deb python optional arch=all
 python3-cffi-backend deb python optional arch=any
 python3-cffi-backend-dbg deb debug optional arch=any
Checksums-Sha1:
 21cdeccd7b7b121d35eae1b8e91d78f9ec83da98 475055 python-cffi_1.14.5.orig.tar.gz
 69ffecb1cdc1f84f2b60645786f299587f01b424 6880 python-cffi_1.14.5-1.debian.tar.xz
Checksums-Sha256:
 fd78e5fee591709f32ef6edb9a015b4aa1a5022598e36227500c8f4e02328d9c 475055 python-cffi_1.14.5.orig.tar.gz
 f63edaa6b42d79dd8c3ddcca080b3f3b4daf809b594c445a7d83f93988e4a79e 6880 python-cffi_1.14.5-1.debian.tar.xz
Files:
 272cb183bf0365530e3c0d8f446cd89d 475055 python-cffi_1.14.5.orig.tar.gz
 cf065a6eadfd0b15088d4d6a373e303c 6880 python-cffi_1.14.5-1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iHUEARYKAB0WIQTumtb5BSD6EfafSCRHew2wJjpU2AUCYCbQ1gAKCRBHew2wJjpU
2HoJAQCguGObb99d5zxpHZweItqs9qH8CBtV7JvfaHeRN17gQQEAmVv7VSsx2QeL
aONSTkPYOJZ0SGsi27x+BlCJFmR/Og8=
=9jQa
-----END PGP SIGNATURE-----
