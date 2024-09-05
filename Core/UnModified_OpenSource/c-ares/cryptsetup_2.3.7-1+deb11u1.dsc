-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: cryptsetup
Binary: cryptsetup, cryptsetup-bin, cryptsetup-initramfs, cryptsetup-run, libcryptsetup12, libcryptsetup-dev, cryptsetup-udeb, libcryptsetup12-udeb
Architecture: linux-any all
Version: 2:2.3.7-1+deb11u1
Maintainer: Debian Cryptsetup Team <pkg-cryptsetup-devel@alioth-lists.debian.net>
Uploaders: Jonas Meurer <jonas@freesources.org>, Guilhem Moulin <guilhem@debian.org>
Homepage: https://gitlab.com/cryptsetup/cryptsetup
Standards-Version: 4.5.1
Vcs-Browser: https://salsa.debian.org/cryptsetup-team/cryptsetup
Vcs-Git: https://salsa.debian.org/cryptsetup-team/cryptsetup.git -b debian/latest
Build-Depends: autoconf, automake (>= 1:1.12), autopoint, debhelper-compat (= 13), dh-strip-nondeterminism, docbook-xml, docbook-xsl, gettext, jq <!nocheck>, libargon2-dev, libblkid-dev, libdevmapper-dev, libjson-c-dev, libpopt-dev, libselinux1-dev, libsepol1-dev, libssl-dev, libtool, pkg-config, po-debconf, procps <!nocheck>, uuid-dev, xsltproc, xxd <!nocheck>
Package-List:
 cryptsetup deb admin optional arch=linux-any
 cryptsetup-bin deb admin optional arch=linux-any
 cryptsetup-initramfs deb admin optional arch=all
 cryptsetup-run deb oldlibs optional arch=all
 cryptsetup-udeb udeb debian-installer optional arch=linux-any
 libcryptsetup-dev deb libdevel optional arch=linux-any
 libcryptsetup12 deb libs optional arch=linux-any
 libcryptsetup12-udeb udeb debian-installer optional arch=linux-any
Checksums-Sha1:
 a2eec22f8355334ecb532ce76c5c8e2ffabb846f 10852556 cryptsetup_2.3.7.orig.tar.xz
 d52eba11a31a546d3cbf2d5a5d01732433083b58 115516 cryptsetup_2.3.7-1+deb11u1.debian.tar.xz
Checksums-Sha256:
 545808e126c84aee06c18a9ebd1ac6ff9ca0ced4632e6bb5d3ee5cf4e048771e 10852556 cryptsetup_2.3.7.orig.tar.xz
 43ee3e3674a7faf92a694299c66bc9245b4936d0d586d6924b00b9f7ecb5e042 115516 cryptsetup_2.3.7-1+deb11u1.debian.tar.xz
Files:
 de3f6d111c94ed64a1738c2ffa518d3f 10852556 cryptsetup_2.3.7.orig.tar.xz
 46b117f59fecb8538c169f650fba4aa5 115516 cryptsetup_2.3.7-1+deb11u1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCgAdFiEERpy6p3b9sfzUdbME05pJnDwhpVIFAmIDsg8ACgkQ05pJnDwh
pVKwbQ//a26niNkDONmeXDGt7cU35JsgfxgiLULvaQ7OmVyBbAQM9ctYF8wdZILk
jxdt2ZDkY9OCg9a0lUJog/wNkqsdJvxArRYrJMeffRs+dmmMZe3ubZW7F9CSRrsh
uHVUnnYrGODu3MqSxfUnfV0KeiduddnViDA3tBgh8BAclrs5WTFMb0FDITdffUGe
/bFN6zusasWAsoMGOcbs5it6B0lNv8sjx3tK983+3CePkHXovkDAFXgPBY/vRlrb
j2b8DYbRPQFZN6W6wKOCxzsDbtrL97IVuI91/QaVNoJasPB6UuWoz6CHzhVoYler
FOw45d28PpKSrFPNJJ8HOHwzZF9FbYtQNyrIcPVxCJFTGFwvuIqUeaO9XymbjBf/
9TQ18QbW0eefZyEMtvw1tgHCmD2ja10he4q/HIywbttm/ER2ErhPkeWsI/mo3z9D
vEimBHbZQ2YVya8trQUkVeICUoLuDwt5ooaZd9I70sxnDnpOula9eJIKBzyoM2KS
GXmdATKm/si8wzkY/UgURj8kMFlym0tZ8ih9IU6YEkQsSnbVpbdHD6/ox0/I0xRH
4zYFRB+Q817kH+wWZ4NIeIN15XbdTUrEa+/mQh8fPHfArxUy7av8mHcsvsblDjG9
J5UGh6L0HPvMTGHCuQlS4ZWu3+1QAoolOgpK5+TXDPoBhe/i5eE=
=vDZo
-----END PGP SIGNATURE-----
