-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: fontconfig
Binary: fontconfig, fontconfig-config, fontconfig-udeb, libfontconfig-dev, libfontconfig1-dev, libfontconfig1, libfontconfig-doc
Architecture: any all
Version: 2.13.1-4.2
Maintainer: Debian freedesktop.org maintainers <pkg-freedesktop-maintainers@lists.alioth.debian.org>
Uploaders: Keith Packard <keithp@debian.org>, Josselin Mouette <joss@debian.org>, Emilio Pozuelo Monfort <pochu@debian.org>,
Homepage: https://www.freedesktop.org/wiki/Software/fontconfig/
Standards-Version: 4.2.1
Vcs-Browser: https://salsa.debian.org/freedesktop-team/fontconfig
Vcs-Git: https://salsa.debian.org/freedesktop-team/fontconfig.git
Build-Depends: debhelper-compat (= 11), libfreetype6-dev (>= 2.8.1), libexpat1-dev, uuid-dev, pkg-config, gperf, po-debconf
Build-Depends-Indep: docbook <!nodoc>, docbook-utils <!nodoc>, texlive-formats-extra <!nodoc>
Package-List:
 fontconfig deb fonts optional arch=any
 fontconfig-config deb fonts optional arch=all
 fontconfig-udeb udeb debian-installer optional arch=any
 libfontconfig-dev deb libdevel optional arch=any
 libfontconfig-doc deb doc optional arch=all profile=!nodoc
 libfontconfig1 deb libs optional arch=any
 libfontconfig1-dev deb oldlibs optional arch=any
Checksums-Sha1:
 75612356ef4f801735b49baf987f8942b4a7a5e1 1723639 fontconfig_2.13.1.orig.tar.bz2
 48f32f66d2523a7bb13b46e1c25c27e60355bd3e 55124 fontconfig_2.13.1-4.2.debian.tar.xz
Checksums-Sha256:
 f655dd2a986d7aa97e052261b36aa67b0a64989496361eca8d604e6414006741 1723639 fontconfig_2.13.1.orig.tar.bz2
 f1ec69a2a0affd86189d3b75ced77b30bbcbc3a6fc0508490e570d4786464b58 55124 fontconfig_2.13.1-4.2.debian.tar.xz
Files:
 36cdea1058ef13cbbfdabe6cb019dc1c 1723639 fontconfig_2.13.1.orig.tar.bz2
 b6faf59f882b8981b20d72435d5647b0 55124 fontconfig_2.13.1-4.2.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQJIBAEBCgAyFiEEVXgdqzTmGgnvuIvhnbAjVVb4z60FAl6+eB4UHGpjcmlzdGF1
QGRlYmlhbi5vcmcACgkQnbAjVVb4z62ZSQ/9EDsQGlrV/tHWcWkxqg1Y3XcoXcaU
I68KeOZkMsHOQobwJxbO3+Z/mSSxLfdQAnW3f/LVmJTb01UvTsRwC1tUMZgQjdzb
G3BcSyHM2qkuCUKfoXTGvooSfNq5RFrREAb5X2IOK3kCB+2NCbefAo8zAB5HdFS3
8ohAeC+YERWUlTHJZhPNRC4mawHWrpm4ZqSiv7tD5nIULPDQ1XE0x9eBuUUTlOX/
S2qtbkLND5l55lfih9X8LY7SsGhnWIlh+QEaPanC7sghU0Jb2HaYEbH8GHyckVph
fJK9bQZUacswtWDcxObADP1vdK5lHpqsO21mQq7rmYJ49nUX/B2mPzPAVyz+8aaF
xiG8sMyj+Mr/8ByhJqOTc3JzKmdl6xlOYrI4tJlAECkTGWnjUjxohEgkv4ui1Quk
dzKKr3iNT11Jp39znj7dY/c73ody8iplYyKg4wjafED3kGNJXnlRN2DFu/bftId9
xctQgmcjnbbe3ZJPbufpCHhdx+bsOpBaa7AdtPMsrK4iF0UZXmLBue7H0FrxBI4g
nd6BEAALskb2tUVA80dfmIz2+Nivilr/htd3hjfsTfMYFdSE9XOAcQA24gGG7NZi
epMw0VNLQtXA1GNTUVB+ZjBv6InGE2BsqHYxj9TNeE3OD4Qx0+a1gfNM7/N9Hla8
lktui2yXj0jRojc=
=19T+
-----END PGP SIGNATURE-----
