-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: fuse
Binary: fuse, libfuse2, libfuse-dev, fuse-udeb, libfuse2-udeb
Architecture: linux-any kfreebsd-any
Version: 2.9.9-5
Maintainer: Laszlo Boszormenyi (GCS) <gcs@debian.org>
Homepage: https://github.com/libfuse/libfuse/wiki
Standards-Version: 4.4.0
Build-Depends: debhelper-compat (= 11), libselinux-dev [linux-any], gettext
Package-List:
 fuse deb utils optional arch=linux-any
 fuse-udeb udeb debian-installer optional arch=linux-any profile=!noudeb
 libfuse-dev deb libdevel optional arch=linux-any,kfreebsd-any
 libfuse2 deb libs optional arch=linux-any,kfreebsd-any
 libfuse2-udeb udeb debian-installer optional arch=linux-any,kfreebsd-any
Checksums-Sha1:
 943ba651b14bc4a3c6fd959ed4b8c04f4a59032d 1813177 fuse_2.9.9.orig.tar.gz
 b5d3a07106bfd03de062f39f12c96fb1d22f7cb3 1012 fuse_2.9.9.orig.tar.gz.asc
 53353b925a699ded8608144706d96a3ccc8be256 22844 fuse_2.9.9-5.debian.tar.xz
Checksums-Sha256:
 d0e69d5d608cc22ff4843791ad097f554dd32540ddc9bed7638cc6fea7c1b4b5 1813177 fuse_2.9.9.orig.tar.gz
 2306ebb33ecc560701f1b569cff3b1cee3dd1c02b46462a9f2c5ba0e5c263a51 1012 fuse_2.9.9.orig.tar.gz.asc
 333552602e8ec558e92c613f656b197c9cb06cda10e3ddbd650879ad8464c1be 22844 fuse_2.9.9-5.debian.tar.xz
Files:
 8000410aadc9231fd48495f7642f3312 1813177 fuse_2.9.9.orig.tar.gz
 8e617f77c41a15537bcb5094eb11a9de 1012 fuse_2.9.9.orig.tar.gz.asc
 4d7f1950156e04d09c0be4107d92cdbf 22844 fuse_2.9.9-5.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCgAdFiEEfYh9yLp7u6e4NeO63OMQ54ZMyL8FAmA5JP4ACgkQ3OMQ54ZM
yL92bA/9FMqG0dWSkSw1Ef6Y1OjYtWWJeVHowU7eOfA9xiNXMbOkNylnoTJhPxrS
OAy0HsT5SSa1Gy6irTLUwis52Aw6xgUTiorqScpIqGOhGl4m69Aea1TafRDn3b/G
Y5GbWMPbEJUsSKYvChz4dMuRu5rnz5AnXZfHPzUvNcoU3pwcK9hwtlEcvj+MWUlW
UI70FjLIfdjzWuVkrmmw7ynt2IdmbmLO+t1wRZbsCTgAnB5jG9Sm6PBQSX069HBm
aMVeB1WC57vQ9T+TXO4SR9uzRrxRQFbzpvs0SNR2sYEytoJtRgfG6hSoh56YCJnH
3UXolMH6B7DNfnvZkwzzjerJ9jEWw6pjZ49QXaE+vll8TFQJ6O3tnZWAuLwZsxVs
imu2ZeXZlV/UrRDQ8QnJWH07M8hSKUjHlcJKlZ56mwX1X2KEaGBQ3a7dPKLVPEQb
ijE7B6U4iwXIr9r1oexeB/n5XqiX3k6+2sKjhvRp/xurr0kpQxkqxxir6Iwz9LaY
BjD8SW3bUi0AdHdGnqOZOQhtjqLUWh6yizGdo4e3QTzDOOpbkcTZdHesBvQgJyn5
Q7ap6pF9iKHOyLggcsS4PPpMAqYkgM5K3deCGkWLE1n1zQ5ZpXa3wm/DdUNuAy0k
pbxTnYUkFKEm1vPDnfE4rVESt7IEIHcHc2UasZeW14Uxt3YQqOI=
=JxPw
-----END PGP SIGNATURE-----
