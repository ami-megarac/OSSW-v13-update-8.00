-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (native)
Source: cdebconf
Binary: cdebconf, cdebconf-gtk, libdebconfclient0, libdebconfclient0-dev, cdebconf-udeb, cdebconf-priority, libdebconfclient0-udeb, cdebconf-text-udeb, cdebconf-newt-udeb, cdebconf-gtk-udeb
Architecture: any all
Version: 0.260
Maintainer: Debian Install System Team <debian-boot@lists.debian.org>
Uploaders:  Colin Watson <cjwatson@debian.org>, Cyril Brulebois <kibi@debian.org>
Standards-Version: 3.9.7
Vcs-Browser: https://salsa.debian.org/installer-team/cdebconf
Vcs-Git: https://salsa.debian.org/installer-team/cdebconf.git
Build-Depends: debhelper-compat (= 12), po-debconf (>= 0.5.0), libslang2-dev, libnewt-dev, libtextwrap-dev (>= 0.1-5), libdebian-installer4-dev (>= 0.41) | libdebian-installer-dev, libglib2.0-dev (>= 2.50), libgtk2.0-dev (>= 2.24) <!pkg.cdebconf.nogtk>, libcairo2-dev (>= 1.8.10-3) <!pkg.cdebconf.nogtk>, libselinux1-dev (>= 2.3) [linux-any] | libselinux-dev [linux-any], dh-autoreconf, dh-exec
Package-List:
 cdebconf deb utils optional arch=any
 cdebconf-gtk deb admin optional arch=any profile=!pkg.cdebconf.nogtk
 cdebconf-gtk-udeb udeb debian-installer optional arch=any profile=!pkg.cdebconf.nogtk
 cdebconf-newt-udeb udeb debian-installer optional arch=any
 cdebconf-priority udeb debian-installer standard arch=all
 cdebconf-text-udeb udeb debian-installer optional arch=any
 cdebconf-udeb udeb debian-installer standard arch=any
 libdebconfclient0 deb libs optional arch=any
 libdebconfclient0-dev deb libdevel optional arch=any
 libdebconfclient0-udeb udeb debian-installer optional arch=any
Checksums-Sha1:
 40910377652daa3fed7797486772903f58daba26 279824 cdebconf_0.260.tar.xz
Checksums-Sha256:
 ac8a9d7449c76eeaa8ed4ef0bbbf4c16b1b816b9905690c732dea2f341ac079b 279824 cdebconf_0.260.tar.xz
Files:
 04c15f7e5620f953e3e1c7a84c51b53f 279824 cdebconf_0.260.tar.xz

-----BEGIN PGP SIGNATURE-----

iQJJBAEBCgAzFiEESWrG6BRCSzSFCDUpWfGHyhVusHYFAmDmrN4VHGh3YW5zaW5n
QG1haWxib3gub3JnAAoJEFnxh8oVbrB21VsQAJJV4Rk2ThfyVa+/C9LXPY66+Nlw
pPh4HpIagGEjGrWDuAodXMsH313YH38z6oLXzq4e+1VS3Mo/UPyg6IpLjB9iMqJA
URnOUM3w6EbyAhokIfQGwwDiHIT3QY/3WCr1LrdGmQNDrRNLqy10kfeIyHUwIUc2
gYD08+Gz33sz3pq1QNRXEcntFf43CRgMHZMhAIE54zMFfOeSzTB8COCU282mc8AB
f52ZHzDaaDF5uHOyexPxQRL+QBIw7utB+h+WA1SkmstAwPkGsQ5aGLzk37PABg03
SL872Dc6+lU+4WplAfOgR9ptelS4l9f12vS1hRF1VSyRvroy0TwaIoPxfyxYn89Y
7nLWWWgDJAnvYNTAOj8Fz3HMBJzLRdNoZXBPOvF93VqGeb8uxhQEr4xFl8sh+gGJ
YeYNmKDu75h4K2NQ47fjuFKAN8S7DWpSjYKPC6Hp6z0dqily0T9jy8LUexCLufLM
bE129T8HMSYV2tp/KIoCzzbG/RkGUe8z3gmAPS/K4yyfiTYVgzmpExAnWmCkwrA0
DsisPb5ANzY0EwH5GjwGgr6Hc/j13WYLh0ODwgT2q/UXr/Ku+tGn/WDAy1lWKi0n
d+9+RrNFNWTESaZYAjK+ky+u3XFufcWstT4y3maDZo/yMOBd+wUSr1PiVEZO8FmP
vpJowWqEJ35WzTTk
=kBw9
-----END PGP SIGNATURE-----
