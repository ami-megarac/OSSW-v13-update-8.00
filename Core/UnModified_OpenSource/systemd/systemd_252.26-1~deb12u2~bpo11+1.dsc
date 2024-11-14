-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: systemd
Binary: systemd, systemd-sysv, systemd-container, systemd-journal-remote, systemd-coredump, systemd-timesyncd, systemd-tests, libpam-systemd, libnss-myhostname, libnss-mymachines, libnss-resolve, libnss-systemd, libsystemd0, libsystemd-dev, libsystemd-shared, udev, libudev1, libudev-dev, udev-udeb, libudev1-udeb, systemd-standalone-sysusers, systemd-standalone-tmpfiles, systemd-oomd, systemd-userdbd, systemd-homed, systemd-boot, systemd-boot-efi, systemd-resolved
Architecture: linux-any
Version: 252.26-1~deb12u2~bpo11+1
Maintainer: Debian systemd Maintainers <pkg-systemd-maintainers@lists.alioth.debian.org>
Uploaders: Michael Biebl <biebl@debian.org>, Marco d'Itri <md@linux.it>, Sjoerd Simons <sjoerd@debian.org>, Martin Pitt <mpitt@debian.org>, Felipe Sateler <fsateler@debian.org>, Luca Boccassi <bluca@debian.org>,
Homepage: https://www.freedesktop.org/wiki/Software/systemd
Standards-Version: 4.6.2
Vcs-Browser: https://salsa.debian.org/systemd-team/systemd
Vcs-Git: https://salsa.debian.org/systemd-team/systemd.git
Testsuite: autopkgtest
Testsuite-Triggers: acl, apparmor, attr, bind9-dnsutils, bind9-host, bsdutils, build-essential, busybox-static, cron, cryptsetup-bin, cryptsetup-initramfs, dbus-user-session, dmeventd, dnsmasq-base, dosfstools, e2fsprogs, erofs-utils, evemu-tools, fdisk, gcc, gdm3, iproute2, iputils-ping, isc-dhcp-client, jq, kbd, knot, knot-dnssecutils, knot-dnsutils, less, libc-dev, libc6-dev, libcap2-bin, libdw-dev, libelf-dev, libfido2-dev, liblz4-tool, libp11-kit-dev, libqrencode-dev, libssl-dev, libtss2-dev, locales, locales-all, make, mtools, net-tools, netcat-openbsd, netlabel-tools, network-manager, openssl, perl, pkg-config, plymouth, policykit-1, polkitd, psmisc, python3, python3-colorama, python3-pexpect, qemu-system-arm, qemu-system-ppc, qemu-system-s390x, qemu-system-x86, quota, rsyslog, screen, seabios, socat, squashfs-tools, strace, stress, tree, vim-tiny, xkb-data, xserver-xorg, xserver-xorg-video-dummy, xz-utils, zstd
Build-Depends: debhelper-compat (= 13), dh-sequence-installnss, dh-sequence-installsysusers, pkg-config, xsltproc <!nodoc>, docbook-xsl <!nodoc>, docbook-xml <!nodoc>, meson (>= 0.53.2), gettext, gperf, gnu-efi [amd64 i386 arm64 armhf riscv64], libcap-dev, libcrypt-dev, libpam0g-dev, libapparmor-dev (>= 2.13) <!stage1>, libidn2-dev <!stage1>, libiptc-dev <!stage1>, libaudit-dev <!stage1>, libdbus-1-dev (>= 1.3.2) <!nocheck> <!noinsttest>, libcryptsetup-dev (>= 2:1.6.0) <!stage1>, libselinux1-dev (>= 2.1.9), libacl1-dev, liblzma-dev, liblz4-dev (>= 1.3.0), liblz4-tool <!nocheck>, libbz2-dev <!stage1>, zlib1g-dev <!stage1> | libz-dev <!stage1>, libcurl4-openssl-dev <!stage1>, libmicrohttpd-dev <!stage1>, libpcre2-dev <!stage1>, libgcrypt20-dev, libkmod-dev (>= 15), libblkid-dev (>= 2.24), libmount-dev (>= 2.30), libfdisk-dev (>= 2.33), libseccomp-dev (>= 2.3.1) [amd64 arm64 armel armhf i386 mips mipsel mips64 mips64el x32 powerpc ppc64 ppc64el riscv64 s390x], libdw-dev (>= 0.158) <!stage1>, libpolkit-gobject-1-dev <!stage1>, libzstd-dev (>= 1.4.0), libtss2-dev <!stage1>, libfido2-dev <!stage1>, libp11-kit-dev <!stage1>, libssl-dev <!stage1>, libqrencode-dev <!stage1>, linux-base <!nocheck>, acl <!nocheck>, python3:native, python3-jinja2:native, python3-lxml:native, python3-pyparsing <!nocheck>, python3-evdev:native <!nocheck>, tzdata <!nocheck>, libcap2-bin <!nocheck>, iproute2 <!nocheck>, zstd <!nocheck>, gawk <!nocheck>, fdisk <!nocheck>
Package-List:
 libnss-myhostname deb admin optional arch=linux-any
 libnss-mymachines deb admin optional arch=linux-any
 libnss-resolve deb admin optional arch=linux-any
 libnss-systemd deb admin standard arch=linux-any
 libpam-systemd deb admin standard arch=linux-any
 libsystemd-dev deb libdevel optional arch=linux-any
 libsystemd-shared deb libs optional arch=linux-any
 libsystemd0 deb libs optional arch=linux-any
 libudev-dev deb libdevel optional arch=linux-any
 libudev1 deb libs optional arch=linux-any
 libudev1-udeb udeb debian-installer optional arch=linux-any profile=!noudeb
 systemd deb admin important arch=linux-any
 systemd-boot deb admin optional arch=amd64,i386,arm64,armhf,riscv64
 systemd-boot-efi deb admin optional arch=amd64,i386,arm64,armhf,riscv64
 systemd-container deb admin optional arch=linux-any profile=!stage1
 systemd-coredump deb admin optional arch=linux-any profile=!stage1
 systemd-homed deb admin optional arch=linux-any profile=!stage1
 systemd-journal-remote deb admin optional arch=linux-any profile=!stage1
 systemd-oomd deb admin optional arch=linux-any
 systemd-resolved deb admin optional arch=linux-any
 systemd-standalone-sysusers deb admin optional arch=linux-any
 systemd-standalone-tmpfiles deb admin optional arch=linux-any
 systemd-sysv deb admin important arch=linux-any
 systemd-tests deb admin optional arch=linux-any profile=!noinsttest
 systemd-timesyncd deb admin standard arch=linux-any
 systemd-userdbd deb admin optional arch=linux-any profile=!stage1
 udev deb admin important arch=linux-any
 udev-udeb udeb debian-installer optional arch=linux-any profile=!noudeb
Checksums-Sha1:
 478a423fc34cc0aea218b156e3ce5ba40da987a6 12128004 systemd_252.26.orig.tar.gz
 99ec5aa63dfb7286dbbdf8bed55dd631235dee53 171664 systemd_252.26-1~deb12u2~bpo11+1.debian.tar.xz
Checksums-Sha256:
 050eae6163c810713d4f92e5ad607aeebcbd2946bbf8bfd8788f07f90e38e335 12128004 systemd_252.26.orig.tar.gz
 afe00a42ebcb7c1a098fdd32bf7d6c93e484e9291777798e2365e39759500487 171664 systemd_252.26-1~deb12u2~bpo11+1.debian.tar.xz
Files:
 b9f4af2bf9571234bf11180c46b02957 12128004 systemd_252.26.orig.tar.gz
 039585d7c6005ff98e8f35647fa7816f 171664 systemd_252.26-1~deb12u2~bpo11+1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQJFBAEBCgAvFiEErCSqx93EIPGOymuRKGv37813JB4FAmaAABERHGJsdWNhQGRl
Ymlhbi5vcmcACgkQKGv37813JB4C8w//ZjR0SxQTqA9RLKvsq5Sa/TVVYNc3eUKP
sFI0BvNEvQEKmKUw5qZ8yvNPyBHDn37Ocv0SrvwOHxSmfoYVzdJCKakrXGfxmz03
lJR4jCQGrz+tUuzBQDkvUxGQTiJ7jrtM0+/wxrdvRnl3t3fL7GE8b6gFlRbuhCRb
nGLpE8mJHpYaEPW9Mwres7fxAJGhfZhOL+vw+qVTH2wy3yzlGCDGoVpuHs3jDlh8
xHx6H9DFQFCtECv+0Y6nWQAnFxg0tqZhhZ8XVdO0Ex8rMctwUFSUSyzZRB/Bbcjt
EDAmvCY2WhJYQbCquKgW7WhchP0rRl0liMuTlA3PeldXyac3MLq4H7fu60hS6wLc
imsrmA4lD9GqeU7OpijfsQ3DATrN0sC9y3hdIm4gHAKj28AJDxZmsVq7JQ3Fdd87
iZS/pC9o8PyWQdcsRqJiW6tsRDj3oSdcRWzF3ELr67WG2HqcyjBcPX1VHc0r7zij
2IuFbO9kEQz7J6me1+mG+/RcY4esekOjf53u4O9dmXjS5gI4FDPZ9arSpa6a1j3M
SHCXtZ3E/oKqMJiioEBggE0FqItqZEP/D8GaBsIVQpxnXLMz08xZGtwP7QcyLo7i
sjiq+FEwPdGOeqV9Zeojc4nmlKTudF4qN83xi0oL0NMJdD7cvUQb2OtVDg4ToOAg
0LdfMqLLiMI=
=hSK5
-----END PGP SIGNATURE-----
