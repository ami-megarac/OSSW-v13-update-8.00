-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: strace
Binary: strace, strace64, strace-udeb
Architecture: linux-any
Version: 5.10-1
Maintainer: Steve McIntyre <93sam@debian.org>
Homepage: https://strace.io
Standards-Version: 4.1.3
Vcs-Browser: https://salsa.debian.org/debian/strace
Vcs-Git: https://salsa.debian.org/debian/strace.git
Build-Depends: libc6-dev (>= 2.2.2) [!alpha !ia64], libc6.1-dev (>= 2.2.2) [alpha ia64], gcc-multilib [amd64 i386 powerpc ppc64 s390 sparc sparc64 x32], debhelper (>= 10.0.0), gawk, libunwind-dev [amd64], libiberty-dev [amd64], libbluetooth-dev
Package-List:
 strace deb utils optional arch=linux-any
 strace-udeb udeb debian-installer optional arch=linux-any
 strace64 deb utils optional arch=i386,powerpc,s390,sparc
Checksums-Sha1:
 8b5ced312f379806406a0ee6551fbe373c55e738 1859688 strace_5.10.orig.tar.xz
 74507e04ec1febf74754a99082d01506ce48fbdd 20492 strace_5.10-1.debian.tar.xz
Checksums-Sha256:
 fe3982ea4cd9aeb3b4ba35f6279f0b577a37175d3282be24b9a5537b56b8f01c 1859688 strace_5.10.orig.tar.xz
 756fd86c8b56a3756b2448ff814085fa44647c0b989453272660c9f0198a2086 20492 strace_5.10-1.debian.tar.xz
Files:
 5f9f87f9afa282e7512f67a129287d6c 1859688 strace_5.10.orig.tar.xz
 87340085d05970052b46b42f2dacd7b1 20492 strace_5.10-1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQJFBAEBCAAvFiEEzrtSMB1hfpEDkP4WWHl5VzRCaE4FAl/vPfkRHDkzc2FtQGRl
Ymlhbi5vcmcACgkQWHl5VzRCaE4B4RAAwwSpDV3RcKzGh+H8SADi3rc8CINrgGJi
fk15xZDbvohs1207RjazSBXl4GF22lFF1dNrOwEe+QSgLe7GB8g6MS6D3qu2npiR
gps38kiNdEmwgcCWWT0mYfxJNDVEsFEOoPVA/Jgl3Jhxr8vHUflH/RPlTLlU2R58
EE7MXkL3BMOFQGqzcppe0x2Y+NPeIuORFaEVr8Q7vznnfrKZoBFa+ciqoQSKIpjv
nXJN9gWaF3zjX0jWegkz+k2g/Y6/9GhrTsJt8JhQr3a+AZReeH9YuPfcfc2bSiTx
MQht5U5dAVvocqviio8HS/kEeX8E88j62cQ1OzaDuBGGYf7UrSHVKbhobSVqJr/j
G8pfzX5dRK8FwafOPQYZjfDPg+GWNSkoRl5KxBfSxexsFBhnl+sTCE94wys3GH/n
czHZtxbujRoAu8uEUSz4A2crBhs8msw1NzaSZ9h/C9UCT8Ha06j+5/RKMLIIvkhr
58+l5pTguFzSoWnX0W2J9RCI9EiE9TAT/kuBdibab2fXCZLShh6nKjOX2TFA+2g2
4xnsGdKx4a2jnZfNpzW3xKufljk0p9MFvVWU1gLdYCEBdok1ImGhoFkwx33kKB2n
E3ZyKDnpoyXO3codOSI26uYktFRN/kUEnCIOcUhkR1BnqNZhm5rJHdsvu455KrAT
WilQTtiTijE=
=cJaU
-----END PGP SIGNATURE-----
