-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: openssh
Binary: openssh-client, openssh-server, openssh-sftp-server, openssh-tests, ssh, ssh-askpass-gnome, openssh-client-udeb, openssh-server-udeb
Architecture: any all
Version: 1:8.4p1-5+deb11u3
Maintainer: Debian OpenSSH Maintainers <debian-ssh@lists.debian.org>
Uploaders: Colin Watson <cjwatson@debian.org>, Matthew Vernon <matthew@debian.org>,
Homepage: http://www.openssh.com/
Standards-Version: 4.1.0
Vcs-Browser: https://salsa.debian.org/ssh-team/openssh
Vcs-Git: https://salsa.debian.org/ssh-team/openssh.git
Testsuite: autopkgtest
Testsuite-Triggers: devscripts, haveged, openssl, putty-tools, python3-twisted, sudo, sysvinit-utils
Build-Depends: autotools-dev, debhelper (>= 9.20160709~), debhelper-compat (= 9), dh-autoreconf, dh-exec, dh-runit (>= 2.8.8), dpkg-dev (>= 1.16.1~), libaudit-dev [linux-any], libedit-dev, libfido2-dev (>= 1.5.0) [linux-any], libgtk-3-dev <!pkg.openssh.nognome>, libkrb5-dev | heimdal-dev, libpam0g-dev | libpam-dev, libselinux1-dev [linux-any], libssl-dev (>= 1.1.0g), libsystemd-dev [linux-any], libwrap0-dev | libwrap-dev, pkg-config, zlib1g-dev (>= 1:1.2.3)
Package-List:
 openssh-client deb net standard arch=any
 openssh-client-udeb udeb debian-installer optional arch=any profile=!noudeb
 openssh-server deb net optional arch=any
 openssh-server-udeb udeb debian-installer optional arch=any profile=!noudeb
 openssh-sftp-server deb net optional arch=any
 openssh-tests deb net optional arch=any
 ssh deb net optional arch=all
 ssh-askpass-gnome deb gnome optional arch=any profile=!pkg.openssh.nognome
Checksums-Sha1:
 69305059e10a60693ebe6f17731f962c9577535c 1742201 openssh_8.4p1.orig.tar.gz
 323573568682eac265e1f69206bc98149a8e423e 683 openssh_8.4p1.orig.tar.gz.asc
 d38cba955daa0185b9f6a0cb7152591de23f2ff6 186600 openssh_8.4p1-5+deb11u3.debian.tar.xz
Checksums-Sha256:
 5a01d22e407eb1c05ba8a8f7c654d388a13e9f226e4ed33bd38748dafa1d2b24 1742201 openssh_8.4p1.orig.tar.gz
 ccd9dd484651ce4cc926228f6e1b46afaf0c5ab98a866217fa0ef1074370ea2b 683 openssh_8.4p1.orig.tar.gz.asc
 f460cc974def7a03753f6d3e5248265aa01deca7e2ba5e29979677487e89cd41 186600 openssh_8.4p1-5+deb11u3.debian.tar.xz
Files:
 8f897870404c088e4aa7d1c1c58b526b 1742201 openssh_8.4p1.orig.tar.gz
 715c219a524631139bafa8a351cf44e7 683 openssh_8.4p1.orig.tar.gz.asc
 90e3da465d87838658dd0182fef0ac37 186600 openssh_8.4p1-5+deb11u3.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCAAdFiEErApP8SYRtvzPAcEROTWH2X2GUAsFAmWEY3cACgkQOTWH2X2G
UAvioxAAo7aXxIacXiN2PEhQJWV23mI6QPVoWJTLDUtiVVeDOex3bIGjufw/s13D
LcpKgCIJr2txbjN8XepjO5UG4J/QUKXIM2LGQxIQPMH+Z1ip30TAziMGz2ZgcM4A
70mgL1r8pMsHkfpJlkeudjQEcBQ9BDTz2DgYRV8ITBO35nN+QmsbW7Y6zTlJQfaE
/LyuUbBjVAnCnSTF1YFl9LCPac1B9xfIZ1V4GajiqVKYCbPtOi+y9EObVyWab82W
sBWoI5LbMR6nSp2zkRxJsVpU51WOMQA41ZjwmllzIR20g/J4PMAjeT9L0k1bKmPm
NWEslWzRESqmghT2JpmmXpvx8ml89kPBdl+iytvmOiZpvHdRE2qXNglch05mIRpT
Wbh3SR/Xxc/uXoihOiZQ8EkL+AKBPF89IuQS1V/Hql+cb745o03P3+CV3hH24BUC
Oh7lV0LYIfF16Vm8KscVpjNEAgde8JHnfDV92ITlZDJn2k9KuYQvnAytT3RZzWOI
Q5tYSAlpmT13Ny3Jf+0+Jrs6KBh7OKR34qutwXtwmCbrMzc5j3SXhliWThZO7W3x
Qk8jTJq7diO8hYusaHyDY85eOn9A1XI/nQCAPM/9VWPcgpKrbe7ap1XRSa7zPpbt
AtHAJt4yMkJH/puIugo8yXH2Vja2LXXtbqaLnxhAtqSQtePeSYA=
=GHEO
-----END PGP SIGNATURE-----
