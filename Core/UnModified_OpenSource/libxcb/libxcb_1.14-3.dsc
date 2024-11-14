-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 1.0
Source: libxcb
Binary: libxcb1, libxcb1-udeb, libxcb1-dev, libxcb-doc, libxcb-composite0, libxcb-composite0-dev, libxcb-damage0, libxcb-damage0-dev, libxcb-dpms0, libxcb-dpms0-dev, libxcb-glx0, libxcb-glx0-dev, libxcb-randr0, libxcb-randr0-dev, libxcb-record0, libxcb-record0-dev, libxcb-render0, libxcb-render0-dev, libxcb-res0, libxcb-res0-dev, libxcb-screensaver0, libxcb-screensaver0-dev, libxcb-shape0, libxcb-shape0-dev, libxcb-shm0, libxcb-shm0-dev, libxcb-sync1, libxcb-sync-dev, libxcb-xf86dri0, libxcb-xf86dri0-dev, libxcb-xfixes0, libxcb-xfixes0-dev, libxcb-xinerama0, libxcb-xinerama0-dev, libxcb-xinput0, libxcb-xinput-dev, libxcb-xtest0, libxcb-xtest0-dev, libxcb-xv0, libxcb-xv0-dev, libxcb-xvmc0, libxcb-xvmc0-dev, libxcb-dri2-0, libxcb-dri2-0-dev, libxcb-present0, libxcb-present-dev, libxcb-dri3-0, libxcb-dri3-dev, libxcb-xkb1, libxcb-xkb-dev
Architecture: any all
Version: 1.14-3
Maintainer: Debian X Strike Force <debian-x@lists.debian.org>
Uploaders:  Julien Cristau <jcristau@debian.org>,
Homepage: https://xcb.freedesktop.org
Standards-Version: 4.5.0
Vcs-Browser: https://salsa.debian.org/xorg-team/lib/libxcb
Vcs-Git: https://salsa.debian.org/xorg-team/lib/libxcb.git
Testsuite: autopkgtest
Testsuite-Triggers: build-essential, pkg-config, xauth, xvfb
Build-Depends: libxau-dev (>= 1:1.0.5-2), libxdmcp-dev (>= 1:1.0.3-2), xcb-proto (>= 1.14), xcb-proto (<< 2.0), libpthread-stubs0-dev (>= 0.1), debhelper-compat (= 12), pkg-config, xutils-dev, xsltproc (>= 1.1.19), check (>= 0.9.4-2) <!nocheck>, python3-xcbgen (>= 1.14), libtool, automake, python3:native, dctrl-tools
Build-Depends-Indep: doxygen, graphviz
Package-List:
 libxcb-composite0 deb libs optional arch=any
 libxcb-composite0-dev deb libdevel optional arch=any
 libxcb-damage0 deb libs optional arch=any
 libxcb-damage0-dev deb libdevel optional arch=any
 libxcb-doc deb doc optional arch=all
 libxcb-dpms0 deb libs optional arch=any
 libxcb-dpms0-dev deb libdevel optional arch=any
 libxcb-dri2-0 deb libs optional arch=any
 libxcb-dri2-0-dev deb libdevel optional arch=any
 libxcb-dri3-0 deb libs optional arch=any
 libxcb-dri3-dev deb libdevel optional arch=any
 libxcb-glx0 deb libs optional arch=any
 libxcb-glx0-dev deb libdevel optional arch=any
 libxcb-present-dev deb libdevel optional arch=any
 libxcb-present0 deb libs optional arch=any
 libxcb-randr0 deb libs optional arch=any
 libxcb-randr0-dev deb libdevel optional arch=any
 libxcb-record0 deb libs optional arch=any
 libxcb-record0-dev deb libdevel optional arch=any
 libxcb-render0 deb libs optional arch=any
 libxcb-render0-dev deb libdevel optional arch=any
 libxcb-res0 deb libs optional arch=any
 libxcb-res0-dev deb libdevel optional arch=any
 libxcb-screensaver0 deb libs optional arch=any
 libxcb-screensaver0-dev deb libdevel optional arch=any
 libxcb-shape0 deb libs optional arch=any
 libxcb-shape0-dev deb libdevel optional arch=any
 libxcb-shm0 deb libs optional arch=any
 libxcb-shm0-dev deb libdevel optional arch=any
 libxcb-sync-dev deb libdevel optional arch=any
 libxcb-sync1 deb libs optional arch=any
 libxcb-xf86dri0 deb libs optional arch=any
 libxcb-xf86dri0-dev deb libdevel optional arch=any
 libxcb-xfixes0 deb libs optional arch=any
 libxcb-xfixes0-dev deb libdevel optional arch=any
 libxcb-xinerama0 deb libs optional arch=any
 libxcb-xinerama0-dev deb libdevel optional arch=any
 libxcb-xinput-dev deb libdevel optional arch=any
 libxcb-xinput0 deb libs optional arch=any
 libxcb-xkb-dev deb libdevel optional arch=any
 libxcb-xkb1 deb libs optional arch=any
 libxcb-xtest0 deb libs optional arch=any
 libxcb-xtest0-dev deb libdevel optional arch=any
 libxcb-xv0 deb libs optional arch=any
 libxcb-xv0-dev deb libdevel optional arch=any
 libxcb-xvmc0 deb libs optional arch=any
 libxcb-xvmc0-dev deb libdevel optional arch=any
 libxcb1 deb libs optional arch=any
 libxcb1-dev deb libdevel optional arch=any
 libxcb1-udeb udeb debian-installer optional arch=any
Checksums-Sha1:
 f0d7b99c8ae1fbe8a6ec9c8cf3faa21090b11b12 640322 libxcb_1.14.orig.tar.gz
 64c513a764fc6f622a7a8d179245f321fde9ffc1 26583 libxcb_1.14-3.diff.gz
Checksums-Sha256:
 2c7fcddd1da34d9b238c9caeda20d3bd7486456fc50b3cc6567185dbd5b0ad02 640322 libxcb_1.14.orig.tar.gz
 aed546fff9cf733c52188ad4f0d869a5ee8ffec52b54a6fa8f666a87adda82a3 26583 libxcb_1.14-3.diff.gz
Files:
 8d1285dec5a474236f67f899f99bc147 640322 libxcb_1.14.orig.tar.gz
 401edc6f113abc94967f11449951d636 26583 libxcb_1.14-3.diff.gz

-----BEGIN PGP SIGNATURE-----

iQJIBAEBCgAyFiEEVXgdqzTmGgnvuIvhnbAjVVb4z60FAmAYI0cUHGpjcmlzdGF1
QGRlYmlhbi5vcmcACgkQnbAjVVb4z63QOw//e62k8ImZyTu8vjngChSSA7d+OPOi
i0Oe9xy/eKWydDgrpfq6ksQcLhTSoB4Sng5G3KpZSHKVQGvT+3J3MlGD0BMIwC/b
sm1QfYc/OMiGNR4qCkgqKSdVt8kWtXLITnrETN5ump3W68tgrhk5VxcDXDITP/ym
dfaMuA8oa0mV/DoDz6VSN266y4CG8JULpmvoExO5cCy/Z4kuEQOtREbcqrJdBRYo
pG44zxcjIzUjsIjsG6oPPcW13wFT2cj6LMlsvZsl3fEJobrzIhO6PnGwQJ8RjumY
T9ovjcl9NI8+BuMXnvoAxAYu5WpBDphkhV0ymrQQw1T76d9BOOMovcYd2a+xPNXz
X6BSSsGmJ6xuurVxd211DetAS3T52ssyYBzZ2Acnc4XaUQQ8EQpJkIog4FHHC1Dt
POAvoorRdcSelIEuaYSIUqQEHp/YHUrcOsDhTATMNZgqBzItyDelWfGq8XoJ7HhL
dt9aFJYrzqZaff9GUd9j5m2w25AxsGVKK7RUluzQi7J+MAfM1yKTytkMIX7fCJZu
C5W4REZhlWuv1IJcZcWDiLjonnkRq0hNG6VyRqvHNkaD6jcnXet0cwduKgITN6Iz
2MeEUD/tbMMQExdkcWC0YwA6nArYrc0NoU9HsBqVdunol7BCGzlZIqnVEIFJCUMq
2sfVzW2KVSUwSWQ=
=7OEv
-----END PGP SIGNATURE-----
