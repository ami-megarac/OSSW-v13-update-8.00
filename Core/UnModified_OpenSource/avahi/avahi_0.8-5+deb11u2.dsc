-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: avahi
Binary: avahi-daemon, avahi-dnsconfd, avahi-autoipd, python3-avahi, avahi-utils, avahi-discover, libavahi-common3, libavahi-common-data, libavahi-common-dev, libavahi-core7, libavahi-core-dev, libavahi-client3, libavahi-client-dev, libavahi-glib1, libavahi-glib-dev, libavahi-gobject0, libavahi-gobject-dev, libavahi-compat-libdnssd1, libavahi-compat-libdnssd-dev, libavahi-ui-gtk3-0, libavahi-ui-gtk3-dev, avahi-ui-utils, gir1.2-avahi-0.6
Architecture: any all
Version: 0.8-5+deb11u2
Maintainer: Utopia Maintenance Team <pkg-utopia-maintainers@lists.alioth.debian.org>
Uploaders: Sjoerd Simons <sjoerd@debian.org>, Sebastian Dröge <slomo@debian.org>, Loic Minier <lool@dooz.org>, Michael Biebl <biebl@debian.org>
Homepage: http://avahi.org/
Standards-Version: 4.5.0
Vcs-Browser: https://salsa.debian.org/utopia-team/avahi
Vcs-Git: https://salsa.debian.org/utopia-team/avahi.git
Build-Depends: debhelper-compat (= 12), dh-python, pkg-config, libcap-dev (>= 1:2.16) [linux-any], libgdbm-dev, libglib2.0-dev (>= 2.4), libgtk-3-dev <!pkg.avahi.nogui>, libexpat-dev, libdaemon-dev (>= 0.11), libdbus-1-dev (>= 0.60), python3, python3-gdbm, python3-dbus <!nopython>, python3-gi <!nopython>, python-gi-dev <!nopython>, gobject-introspection, libgirepository1.0-dev, xmltoman, intltool (>= 0.35.0)
Package-List:
 avahi-autoipd deb net optional arch=linux-any
 avahi-daemon deb net optional arch=any
 avahi-discover deb net optional arch=all profile=!nopython,!pkg.avahi.nogui
 avahi-dnsconfd deb net optional arch=any
 avahi-ui-utils deb utils optional arch=any profile=!pkg.avahi.nogui
 avahi-utils deb net optional arch=any
 gir1.2-avahi-0.6 deb introspection optional arch=any
 libavahi-client-dev deb libdevel optional arch=any
 libavahi-client3 deb libs optional arch=any
 libavahi-common-data deb libs optional arch=any
 libavahi-common-dev deb libdevel optional arch=any
 libavahi-common3 deb libs optional arch=any
 libavahi-compat-libdnssd-dev deb libdevel optional arch=any
 libavahi-compat-libdnssd1 deb libs optional arch=any
 libavahi-core-dev deb libdevel optional arch=any
 libavahi-core7 deb libs optional arch=any
 libavahi-glib-dev deb libdevel optional arch=any
 libavahi-glib1 deb libs optional arch=any
 libavahi-gobject-dev deb libdevel optional arch=any
 libavahi-gobject0 deb libs optional arch=any
 libavahi-ui-gtk3-0 deb libs optional arch=any profile=!pkg.avahi.nogui
 libavahi-ui-gtk3-dev deb libdevel optional arch=any profile=!pkg.avahi.nogui
 python3-avahi deb python optional arch=any profile=!nopython
Checksums-Sha1:
 969a50ae18c8d8e2288435a75666dd076e69852a 1591458 avahi_0.8.orig.tar.gz
 265091880adabf0e355c1e9f62dea829fdd8c9c1 37092 avahi_0.8-5+deb11u2.debian.tar.xz
Checksums-Sha256:
 060309d7a333d38d951bc27598c677af1796934dbd98e1024e7ad8de798fedda 1591458 avahi_0.8.orig.tar.gz
 f3e78ee15b344828821e9c40545f87832731e5d164f9839ddbe6487a1e63d9c6 37092 avahi_0.8-5+deb11u2.debian.tar.xz
Files:
 229c6aa30674fc43c202b22c5f8c2be7 1591458 avahi_0.8.orig.tar.gz
 6872ecf32c746a78b7d500e570676c43 37092 avahi_0.8-5+deb11u2.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCAAdFiEECbOsLssWnJBDRcxUauHfDWCPItwFAmPelZsACgkQauHfDWCP
ItyiGA//fzDWvFWgpq2fA/5EtK4+OnGaTwnVEteHMvfA8kSYxoVES17EahW0fKf3
WRkUrIXpWnlHf5xj7HjHhc8N3XxTToSRJbpOGMrZqNf5eVBlt3mccE6TRVC5g4cW
Cgy+ginEB6yIWnqJxsUBdit+OynDU/qzSl4wBjPgGCdQUMIpVUhvsZAw7/J9575l
EMADXzCwZpTfC4399/4/DoNSzcI2nUMJ9xPw4zqcWYxXclAZfwCnVR+7TnJoaNss
SJrh0k0qoaqeJgicrYw+HuzxU5owcrMfM+NfBE8OPCI7puMwo3R7YbfeEG9VtOa+
Yc8cf7yRrzwQE/ijv2GegvGyD7wxijhBrTCdMO/LEq59nm8uP6TsDIm3AhRzvtAn
k5KmXoxL+CbP24mnjegqFD1VLthlkz8cckCY4iQovnhE4OJ8GDnjbmfbTS3JYWK9
yspN1M1h+6J5ZvErSnzAPFNfNfi6f0zX1r1JUbIjfV5oCEeE+LtFEVrsHKlyemiq
RzOoF6/5mZhLSAOq125Hu/crCVuAdhhtm+BYxEGbPNLJ/1+mHiYmPhgmOk9TliM3
ZcoIzPdxP+SGswFQ1qJn8gGNZQViWYHntkyhR0mj/KJIZs/sqPjMrTLmFJ+kGCgB
I/dJY5ukOsIsP6I13b6T69IAUY1OIY9ivT+AwAZiLovRxUnc9GM=
=KMK3
-----END PGP SIGNATURE-----
