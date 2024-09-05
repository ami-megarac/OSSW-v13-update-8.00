-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: ncurses
Binary: libtinfo6, libtinfo6-udeb, libncurses6, libncurses-dev, libtinfo-dev, libncurses5-dev, libncursesw6, libncursesw5-dev, lib64tinfo6, lib64ncurses6, lib64ncursesw6, lib64ncurses-dev, lib32tinfo6, lib32ncurses6, lib32ncursesw6, lib32ncurses-dev, ncurses-bin, ncurses-base, ncurses-term, ncurses-examples, ncurses-doc, libtinfo5, libncurses5, libncursesw5
Architecture: any all
Version: 6.2+20201114-2+deb11u2
Maintainer: Craig Small <csmall@debian.org>
Uploaders: Sven Joachim <svenjoac@gmx.de>
Homepage: https://invisible-island.net/ncurses/
Standards-Version: 4.5.1
Vcs-Browser: https://salsa.debian.org/debian/ncurses
Vcs-Git: https://salsa.debian.org/debian/ncurses.git
Build-Depends: debhelper-compat (= 13), libgpm-dev [linux-any], pkg-config, autoconf-dickey (>= 2.52+20200111)
Build-Depends-Arch: g++-multilib [amd64 i386 powerpc ppc64 s390 sparc] <!nobiarch>
Package-List:
 lib32ncurses-dev deb libdevel optional arch=amd64,ppc64 profile=!nobiarch
 lib32ncurses6 deb libs optional arch=amd64,ppc64 profile=!nobiarch
 lib32ncursesw6 deb libs optional arch=amd64,ppc64 profile=!nobiarch
 lib32tinfo6 deb libs optional arch=amd64,ppc64 profile=!nobiarch
 lib64ncurses-dev deb libdevel optional arch=i386,powerpc,sparc,s390 profile=!nobiarch
 lib64ncurses6 deb libs optional arch=i386,powerpc,sparc,s390 profile=!nobiarch
 lib64ncursesw6 deb libs optional arch=i386,powerpc,sparc,s390 profile=!nobiarch
 lib64tinfo6 deb libs optional arch=i386,powerpc,sparc,s390 profile=!nobiarch
 libncurses-dev deb libdevel optional arch=any
 libncurses5 deb oldlibs optional arch=any profile=!pkg.ncurses.nolegacy
 libncurses5-dev deb oldlibs optional arch=any
 libncurses6 deb libs optional arch=any
 libncursesw5 deb oldlibs optional arch=any profile=!pkg.ncurses.nolegacy
 libncursesw5-dev deb oldlibs optional arch=any
 libncursesw6 deb libs optional arch=any
 libtinfo-dev deb oldlibs optional arch=any
 libtinfo5 deb oldlibs optional arch=any profile=!pkg.ncurses.nolegacy
 libtinfo6 deb libs optional arch=any
 libtinfo6-udeb udeb debian-installer optional arch=any profile=!noudeb
 ncurses-base deb misc required arch=all essential=yes
 ncurses-bin deb utils required arch=any essential=yes
 ncurses-doc deb doc optional arch=all
 ncurses-examples deb misc optional arch=any profile=!pkg.ncurses.noexamples
 ncurses-term deb misc standard arch=all
Checksums-Sha1:
 fe4bfc4c178c5211802ba28c47bdcc326e251d29 3539796 ncurses_6.2+20201114.orig.tar.gz
 6f79837a2d40983ed4472f4fce13e22bd3115289 265 ncurses_6.2+20201114.orig.tar.gz.asc
 fec49aa8392ce5fabbd46ddd3fc523c4c60105da 54056 ncurses_6.2+20201114-2+deb11u2.debian.tar.xz
Checksums-Sha256:
 aa3f8cfaff2a2b78f184274ec43d9da910c864e4b4d80fc47b5b48cba9154cd2 3539796 ncurses_6.2+20201114.orig.tar.gz
 91615d9d5575f9e974e78c6aca55e1885f42d1b2600cebec407be4471bb7a27d 265 ncurses_6.2+20201114.orig.tar.gz.asc
 c0f89212206e4c9dc5e141e2588ba82ea76051a04fccc5dc84075a358b3a6704 54056 ncurses_6.2+20201114-2+deb11u2.debian.tar.xz
Files:
 c522f09793230ef1ed3b6dff5bbc0740 3539796 ncurses_6.2+20201114.orig.tar.gz
 774e49e6e5e47f3e6bba8aa9dd52094f 265 ncurses_6.2+20201114.orig.tar.gz.asc
 afa5ce292444d35fa3e006d0ff741c08 54056 ncurses_6.2+20201114-2+deb11u2.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCgAdFiEEKF8heKgv5Jai5p4QOxBucY1rMawFAmRw+6oACgkQOxBucY1r
Maz7Gw//UoiOWKJZm8rOCOUhWW+UU5+BxSbOhlQYwEhNsEj4X8qtYbzVfVEDAVBh
3fJglIkoThZt2WOEw28IMJgWo5wByeJMRNHPgWRFLXmKtj/v7H6bsSe3vMy1pWaJ
LDGcG+HZ4mdeZjW6wkXiXIrgVDtA037blPKCeg8mR20E2qrkD0W1BIWkagdxSJAz
hR5uUk8eevGGMgVhSUFnt8lPsY73RJpzoVNX82iKD2vmGioQjpaiD1kAV/X56RPy
/uvJrBIMt8ZTB8Y2GM9ghe2wse5Kuru52lAmPXfHPpe60XZ3a6cjUDlJjTnk0p2P
tZWqjDuw7RvxOII1ehHm+VzG4Pe4d24XWgcyb4WLuOt3ObGNl8IEiPC6bn99KkVX
D4Cylr2Rw5phHnd5WoMvLx/T9Eth1cx3zuL6Zq1ZMfFU3l4/vt1GgN2gFY91uYZ0
/bkgUoace4gBPWxZHmGt55Ky3oaVRSZ2+FBFfjiefVMSvrC7LDTEAnmcdZqj2F4A
m2rISE3Xtu0P0rgtHGlY63isveIsys72dYYdLkQBrtzgTalj8wPzUvryed6EzKoC
k1P1a//nQUzyLme8zABNxZ/FzCw1xrSg2/VzuE/R66KIwWZ6bp7lrSiRLIjgasyj
8OZRarr4RVEhpwmuuUNzm82iqvhvxy89DZ/WEc0JGY8S1o0ZSMY=
=7sH9
-----END PGP SIGNATURE-----
