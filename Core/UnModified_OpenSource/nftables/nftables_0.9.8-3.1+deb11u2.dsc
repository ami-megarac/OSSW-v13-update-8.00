-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: nftables
Binary: nftables, libnftables-dev, libnftables1, python3-nftables
Architecture: linux-any
Version: 0.9.8-3.1+deb11u2
Maintainer: Debian Netfilter Packaging Team <pkg-netfilter-team@lists.alioth.debian.org>
Uploaders: Arturo Borrero Gonzalez <arturo@debian.org>, Jeremy Sowden <jeremy@azazel.net>
Homepage: https://www.netfilter.org/
Standards-Version: 4.5.1
Vcs-Browser: https://salsa.debian.org/pkg-netfilter-team/pkg-nftables
Vcs-Git: https://salsa.debian.org/pkg-netfilter-team/pkg-nftables.git
Testsuite: autopkgtest
Testsuite-Triggers: kmod, systemd
Build-Depends: asciidoc-base, automake, bison, debhelper-compat (= 13), dh-python, docbook-xsl, flex, libgmp-dev, libjansson-dev, libmnl-dev (>= 1.0.4), libnftnl-dev (>= 1.1.9), libeditreadline-dev, libtool (>= 2.2.6), libxtables-dev (>= 1.6.1), python3-all
Package-List:
 libnftables-dev deb libdevel optional arch=linux-any
 libnftables1 deb libs optional arch=linux-any
 nftables deb net important arch=linux-any
 python3-nftables deb python optional arch=linux-any
Checksums-Sha1:
 c15ac5552959c8358975f6b3e15757841c6904c8 879516 nftables_0.9.8.orig.tar.bz2
 92f2d96c99447bbd4292ef97b5e499e510a731b3 27940 nftables_0.9.8-3.1+deb11u2.debian.tar.xz
Checksums-Sha256:
 60fc004656dae4fefc4901c939c9d64120b4dedb49738e420a9a34989f108fe4 879516 nftables_0.9.8.orig.tar.bz2
 f25d1c2a6142a58f403dc5880c25d543057e9c63bde1477548395d15ceef9385 27940 nftables_0.9.8-3.1+deb11u2.debian.tar.xz
Files:
 77bf0bd43e65e92212fc73139a2e47fc 879516 nftables_0.9.8.orig.tar.bz2
 8144c8a2a0ad5421148e514ed401a2d6 27940 nftables_0.9.8-3.1+deb11u2.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCgAdFiEE3ZhhqyPcMzOJLgepaOcTmB0VFfgFAmWT2PwACgkQaOcTmB0V
Ffi4Rg/8D6HWkE/3MXRvwD1o8xkNqtCeLSS41bIvGNHITfsWNPbtAOz4dGqnCJLs
BRvzUdRsPklPtQUotwdxUQW6qa+syjKgP9OG1aHIWxJ6TBuLCr0JhltAe1Rq7KGc
22IO7kLZHKsCP4SD8epauhOsEnZnbIBrb2cW1paAtw4HvpEkVRno2CtZ680m7NP+
iRUtoa9DExXJEKZNgMxOyfgjsybV3cVIwik98O0PVpW7YAuCWg22RtyuHHAUnorl
SuG2C1RSFqyy/oUg7aIC/sSAgf9qZMDLWklU94Cxptj0ryWcVZztF6v31V4eHoOJ
cXvaDN0HjIwctxo34p1MUQZgwsd7IhcjQPQon32ChHv302hhawNlIfaevSFgT9Vo
mxuuIbmFRoAalkcBpCHQu8C6lagEKD2NNwiIM5UUzTWX8ax1iU1IOoCRRUvTGky/
x1gIPIYpCoSI8O1BlCyrH7MTc9p0QkKnpVLKic9J2Krl9EJOwyJpBJ0WOPtxqWGi
4mUgG51fZPi1Ace/XoywcVd0eoePVw9iImhNj+Fiu6piqfmzOP3Qyf7SZjIL8IXT
GC9vXTfxiYpYtUB3siy4LWng6fhyCj0vkyp8XG03R7IYb/4gOuyCjphiH21f9+Rq
r+X1ucBMxuwSF/Bakj4Z0lHr7mSEGr66x0AzE1YOozWPs0qKK60=
=XUEC
-----END PGP SIGNATURE-----
