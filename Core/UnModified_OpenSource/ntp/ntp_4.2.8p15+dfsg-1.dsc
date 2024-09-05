-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: ntp
Binary: ntp, ntpdate, ntp-doc, sntp
Architecture: any all
Version: 1:4.2.8p15+dfsg-1
Maintainer: Debian NTP Team <ntp@packages.debian.org>
Uploaders: Kurt Roeckx <kurt@roeckx.be>, Bernhard Schmidt <berni@debian.org>
Homepage: http://support.ntp.org/
Standards-Version: 4.1.3.0
Vcs-Browser: https://salsa.debian.org/pkg-ntp-team/ntp
Vcs-Git: https://salsa.debian.org/pkg-ntp-team/ntp.git
Build-Depends: autogen, debhelper (>= 10), dh-apparmor, libcap2-dev [linux-any], libedit-dev, libevent-dev, libopts25-dev, libssl-dev, pkg-config, pps-tools [linux-any]
Build-Conflicts: libavahi-compat-libdnssd-dev, libwww-dev, libwww-ssl-dev
Package-List:
 ntp deb net optional arch=any
 ntp-doc deb doc optional arch=all
 ntpdate deb net optional arch=any
 sntp deb net optional arch=any
Checksums-Sha1:
 9995db1022105be36b80a5ff57cacd31cba7b7ce 4491056 ntp_4.2.8p15+dfsg.orig.tar.xz
 d2d1a51aeccf00be29057aa6766324f8b2d66f8e 47040 ntp_4.2.8p15+dfsg-1.debian.tar.xz
Checksums-Sha256:
 6783a98bc497d661adf8606f23f274d41e32e24115443ff5b9ab899a386be19d 4491056 ntp_4.2.8p15+dfsg.orig.tar.xz
 65c1f0875fa19ac82833d0f721c33e1655c275a0f863eceab5ff655d429c8a59 47040 ntp_4.2.8p15+dfsg-1.debian.tar.xz
Files:
 c1c557036197188a22ec285fa53149d8 4491056 ntp_4.2.8p15+dfsg.orig.tar.xz
 896c9b6679f379d8967b012a0cc792c2 47040 ntp_4.2.8p15+dfsg-1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQJFBAEBCgAvFiEE1uAexRal3873GVbTd1B55bhQvJMFAl9rOhERHGJlcm5pQGRl
Ymlhbi5vcmcACgkQd1B55bhQvJO6DhAApSBsz12PeJD20X5JtUQ9iL8kC+nYovby
7CnCkx30d+4pj4mHe0WjmLvyNpuZcmHb1bXyZwZVNAy9CGRb/3f74Nf3JUX1z6nr
AwZzpXxmF0AfnuVSOdMkWk5w65ggLl02cfL03WxLVGBzvUZ4o/uN1NMt4BvBEGNp
RTNHm5pSSoBAmQdyDNUkvFtPbkGAgrUnLwlO3tAabgontelN6tGqRMDRUsOErblS
iZitUF1vQ8SwyCqnGyQPJnxEeiZ1Fw5N8vRjSddgOie5eTr+oKxj5zLrNPVbTl4L
0xN9X2Uhnii4XZW2sVbZtbbi6YK6JMPyfOSPaw1CF0KaHGzIbGJTnz7pIqPjt9DW
ikqmkZRAwIqBuWo2MAO/DgeutKquYIP3uUbpqqKjd0olSApu9QClP8pGPpfGjzH2
nxMFz4WAfDEM6l7vTMTsehAHvMdCkJ0I2YHEi6zdeLQi3ane2J4A3syuUosqZ6w9
oZRt+lUWIaywnhUZ7J8Ms0s2CGV4rvus/PIYFoZ0xtsKecnDpf2e+uxyBifNJDzX
O+hbsjU74JuDMPkYnTW6ZK+mOQrM6ni+HzJmwz7lUcfuc6540CGwK5/mro+FXCh8
8f4tfLoMBfPjYNsj5XpKTHFilVTgYdOK1QMlBgUezAaYjFyYlNVsL57b9+4+DhP5
Cyb94gbznAg=
=B+tU
-----END PGP SIGNATURE-----
