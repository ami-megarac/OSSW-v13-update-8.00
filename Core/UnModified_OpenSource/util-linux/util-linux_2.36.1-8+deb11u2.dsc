-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: util-linux
Binary: util-linux, util-linux-locales, mount, bsdutils, bsdextrautils, fdisk, fdisk-udeb, libblkid1, libblkid1-udeb, libblkid-dev, libfdisk1, libfdisk1-udeb, libfdisk-dev, libmount1, libmount1-udeb, libmount-dev, libsmartcols1, libsmartcols1-udeb, libsmartcols-dev, libuuid1, uuid-runtime, libuuid1-udeb, uuid-dev, util-linux-udeb, rfkill, eject, eject-udeb
Architecture: any all
Version: 2.36.1-8+deb11u2
Maintainer: util-linux packagers <util-linux@packages.debian.org>
Uploaders: Chris Hofstaedtler <zeha@debian.org>
Homepage: http://www.kernel.org/pub/linux/utils/util-linux/
Standards-Version: 4.5.1
Vcs-Browser: https://salsa.debian.org/debian/util-linux
Vcs-Git: https://salsa.debian.org/debian/util-linux.git
Testsuite: autopkgtest
Testsuite-Triggers: bash, bc, bsdmainutils, build-essential, dpkg, grep, pkg-config
Build-Depends: bc <!stage1 !nocheck>, bison, debhelper-compat (= 13), dh-exec, gettext, libaudit-dev [linux-any] <!stage1>, libcap-ng-dev [linux-any] <!stage1>, libncurses5-dev, libncursesw5-dev, libpam0g-dev <!stage1>, libselinux1-dev (>= 2.6-3~) [linux-any], libsystemd-dev [linux-any] <!stage1>, libtool, libudev-dev [linux-any] <!stage1>, netbase <!stage1 !nocheck>, pkg-config, po-debconf, socat <!stage1 !nocheck>, systemd [linux-any] <!stage1>, zlib1g-dev
Build-Conflicts: libcryptsetup-dev
Package-List:
 bsdextrautils deb utils optional arch=any profile=!stage1
 bsdutils deb utils required arch=any profile=!stage1 essential=yes
 eject deb utils optional arch=linux-any
 eject-udeb udeb debian-installer optional arch=linux-any
 fdisk deb utils important arch=any profile=!stage1
 fdisk-udeb udeb debian-installer optional arch=hurd-any,linux-any profile=!stage1
 libblkid-dev deb libdevel optional arch=any
 libblkid1 deb libs optional arch=any
 libblkid1-udeb udeb debian-installer optional arch=any
 libfdisk-dev deb libdevel optional arch=any
 libfdisk1 deb libs optional arch=any
 libfdisk1-udeb udeb debian-installer optional arch=any
 libmount-dev deb libdevel optional arch=linux-any
 libmount1 deb libs optional arch=any
 libmount1-udeb udeb debian-installer optional arch=linux-any
 libsmartcols-dev deb libdevel optional arch=any
 libsmartcols1 deb libs optional arch=any
 libsmartcols1-udeb udeb debian-installer optional arch=any
 libuuid1 deb libs optional arch=any
 libuuid1-udeb udeb debian-installer optional arch=any
 mount deb admin required arch=linux-any profile=!stage1
 rfkill deb utils optional arch=linux-any profile=!stage1
 util-linux deb utils required arch=any profile=!stage1 essential=yes
 util-linux-locales deb localization optional arch=all profile=!stage1
 util-linux-udeb udeb debian-installer optional arch=any profile=!stage1
 uuid-dev deb libdevel optional arch=any
 uuid-runtime deb utils optional arch=any profile=!stage1
Checksums-Sha1:
 d542136a8875ec672aa4f065c16a5fce1808ca42 5231880 util-linux_2.36.1.orig.tar.xz
 472c45dcb65e1ddc4687e808e3ebe5a308f8fbae 106524 util-linux_2.36.1-8+deb11u2.debian.tar.xz
Checksums-Sha256:
 09fac242172cd8ec27f0739d8d192402c69417617091d8c6e974841568f37eed 5231880 util-linux_2.36.1.orig.tar.xz
 0b07fbb79e1dab3be2568295664af950a7c3e589ce7821febf2ab0a3ccbe4862 106524 util-linux_2.36.1-8+deb11u2.debian.tar.xz
Files:
 b0b702979d47043d9c4d8ba93be21e20 5231880 util-linux_2.36.1.orig.tar.xz
 b88fa53fa6422a3a64743166a23384e5 106524 util-linux_2.36.1-8+deb11u2.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCAAdFiEEfRrP+tnggGycTNOSXBPW25MFLgMFAmYFQ5AACgkQXBPW25MF
LgMnOA/8Dnfgso3OpFuE2f3xp1KFZOA2Lp1O/QrcMaSOiAkT5c2ZzyK/AWZIz2eA
fnkmUSJ5ZEd04YnRGWDu2O2L1qF6FyfnZIUARcYG8o02hLqkAdNGYYqjzFPws84f
SNrfwrUlqCNG2gefS17ChgEhoTRUa6pnnDYYrAEJoEXaezSWIs0F5sQPrDSrXkgo
IpFhpbjpsPzafU/sMzrgr6tkvuw5Asl9JOC27ckGXgxq+OLPzrWfDLsxRX4wImLc
SVW9syrAIUsaZzQp9yWtnRaEIhb58WybBrHw36wdlCsKOzeTlSwstEsKvAUiUvWT
n7K9ntJEeWIz9Hvlff3QfTBSNS2orQ+yAr31pMUsB+Sg3AbpfIWofPa9yhS0wVJ2
bUJklGgTXqeHJmKqhqXD6xyo1fcUrvfd8qXIvM+02qZq46L1sFUseIgWrjhkNCBb
OQKd8eCcorpkE3BH0FtRpOOzbDPOxqvU4CIouf+hj8AJcY8tSf67HwlWPgfYCN/T
evKPqBc6rzP7bTvqNVW8MsEVptWkAx5qoo6xo30tNgAHzfUslkv3clIkpu0vxyOq
i5AONOQIpydQRogpcYU7jJG3b+0wKAz7PQV7B/sth9wJ4ZVzi6J4yyOvpYqDhKZ4
vekhVSHWOA48QY3igNKFV31nFKbU5Sr1IDiu3xDTebWZhxbF6N8=
=CpMX
-----END PGP SIGNATURE-----
