-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: dbus-python
Binary: python-dbus-dev, python-dbus-doc, python3-dbus, python3-dbus-dbg, python3-dbus-tests
Architecture: any all
Version: 1.2.16-5
Maintainer: Utopia Maintenance Team <pkg-utopia-maintainers@lists.alioth.debian.org>
Uploaders: Sjoerd Simons <sjoerd@debian.org>, Sebastian Dröge <slomo@debian.org>, Simon McVittie <smcv@debian.org>, Loic Minier <lool@dooz.org>,
Homepage: https://www.freedesktop.org/wiki/Software/DBusBindings#Python
Standards-Version: 4.5.1
Vcs-Browser: https://salsa.debian.org/debian/dbus-python
Vcs-Git: https://salsa.debian.org/debian/dbus-python.git
Testsuite: autopkgtest, autopkgtest-pkg-python
Testsuite-Triggers: build-essential, dbus, gnome-desktop-testing, libglib2.0-dev, python3-all-dbg, python3-all-dev, python3-gi, python3-tap
Build-Depends: autoconf, autoconf-archive, automake, dbus (>= 1.8), debhelper-compat (= 12), dh-python, dh-sequence-python3, dpkg-dev (>= 1.16.1), libdbus-1-dev (>= 1.8), libglib2.0-dev (>= 2.40), python3-all-dbg, python3-all-dev, python3-gi, python3-tap <!nocheck>
Build-Depends-Indep: python3-sphinx, python3-sphinx-rtd-theme
Package-List:
 python-dbus-dev deb python optional arch=all
 python-dbus-doc deb doc optional arch=all
 python3-dbus deb python optional arch=any
 python3-dbus-dbg deb debug optional arch=any
 python3-dbus-tests deb python optional arch=any
Checksums-Sha1:
 de05308c75baa2ce5434de73d60428c005ac0cc1 576701 dbus-python_1.2.16.orig.tar.gz
 4bbf4355c8f6f614b0598f95341259eae9d297e7 833 dbus-python_1.2.16.orig.tar.gz.asc
 beba401c9ecfbff90c8bbc7c39ce482d80bca549 34088 dbus-python_1.2.16-5.debian.tar.xz
Checksums-Sha256:
 11238f1d86c995d8aed2e22f04a1e3779f0d70e587caffeab4857f3c662ed5a4 576701 dbus-python_1.2.16.orig.tar.gz
 0fcfcb9844226c5cde1690b74b3c094d802ea735392d3a8829f1b5993837e86c 833 dbus-python_1.2.16.orig.tar.gz.asc
 9d8a99e3a6c2f366772efe15a6a64cf3d83bf8bc5b581259a69b2a429f8a9bd7 34088 dbus-python_1.2.16-5.debian.tar.xz
Files:
 51a45c973d82bedff033a4b57d69d5d8 576701 dbus-python_1.2.16.orig.tar.gz
 5833899712853bf194c024b34d4ddac3 833 dbus-python_1.2.16.orig.tar.gz.asc
 d6a46805f22de543c9a621c56fb33d18 34088 dbus-python_1.2.16-5.debian.tar.xz
Dgit: fbc1ece7ca8a98ef8e27b227dbfc78f05f59c325 debian archive/debian/1.2.16-5 https://git.dgit.debian.org/dbus-python

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCAAdFiEENuxaZEik9e95vv6Y4FrhR4+BTE8FAmAW4OUACgkQ4FrhR4+B
TE//NxAAsL0t8hyoYdoTeHEitc7pzqix8RhvUcNz0L0zzdkWQGexjat4oBi7EbHv
TEzD1ER6paImfOSXEAEci944zXmhboN0EfdL5zCa4WnqTqJ3lo3rpzHQXftz1Xs8
a9/5qSBLjybSNIpUMMQXjRmFYlrk2sfj8x/Yezc4S5uz4EmHD7uLPnkOq9IjR/Gt
0gBqiL8oXUqgogOOK7MWTC9hSm9NJTh9BQxVimFgv95uHlvBJfjmdz0P4KW9RVJz
RJZVQHw/zT5XlEcVMgHnSsbziWyEtvnwNvzeZfkU9wjtUlIss0QXZh+wcJv01GMs
s30vKT+hEY34cHx7yYeZ7yAlTRNC9Thig1IoHnvbZ3Rbg64Aq18rYf+bOBaG2oon
A4lASIBO3BQQWS6/9UumJVOpttkIIh0pUSbt71JeAUWpXMfxTR4AZzZBclBRVLW2
JacVtT3ZvaNQVOxuhluCkuGlhYp+SeLv3+huhZQT+CvmGOGroOgIcZukauVQn1Fa
aGH5pfU4h5/WlpxBwzHEZcjxz2CAgqY7iz/KsijPC4C8es+VksyhPJeITovdi5zK
AMO52zlxum9DhXeTgNNvuMl2lqtJ/q6rYOMetmJpdFsQrA0L+FsyYc6DWGDnUDw8
QRxdyZ4TMeAe5EchqVg6geYykHp1NHrcQYhpasprb0XsoA0sL7c=
=Xz30
-----END PGP SIGNATURE-----
