-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (native)
Source: apt
Binary: apt, libapt-pkg6.0, apt-doc, libapt-pkg-dev, libapt-pkg-doc, apt-utils, apt-transport-https
Architecture: any all
Version: 2.2.4
Maintainer: APT Development Team <deity@lists.debian.org>
Uploaders: Michael Vogt <mvo@debian.org>, Julian Andres Klode <jak@debian.org>, David Kalnischkies <donkult@debian.org>
Standards-Version: 4.1.1
Vcs-Browser: https://salsa.debian.org/apt-team/apt
Vcs-Git: https://salsa.debian.org/apt-team/apt.git
Testsuite: autopkgtest
Testsuite-Triggers: @builddeps@, aptitude, db-util, dpkg, fakeroot, gnupg, gnupg1, gnupg2, gpgv, gpgv1, gpgv2, libfile-fcntllock-perl, lsof, pkg-config, python3-apt, stunnel4, valgrind, wget
Build-Depends: cmake (>= 3.4), debhelper-compat (= 12), docbook-xml, docbook-xsl, dpkg-dev (>= 1.17.14), gettext (>= 0.12), googletest <!nocheck> | libgtest-dev <!nocheck>, libbz2-dev, libdb-dev, libgnutls28-dev (>= 3.4.6), libgcrypt20-dev, liblz4-dev (>= 0.0~r126), liblzma-dev, libseccomp-dev (>= 2.4.2) [amd64 arm64 armel armhf i386 mips mips64el mipsel ppc64el s390x hppa powerpc powerpcspe ppc64 x32], libsystemd-dev [linux-any], libudev-dev [linux-any], libxxhash-dev (>= 0.8), libzstd-dev (>= 1.0), ninja-build, pkg-config, po4a (>= 0.34-2), triehash, xsltproc, zlib1g-dev
Build-Depends-Indep: doxygen, graphviz, w3m
Package-List:
 apt deb admin important arch=any
 apt-doc deb doc optional arch=all
 apt-transport-https deb oldlibs optional arch=all
 apt-utils deb admin important arch=any
 libapt-pkg-dev deb libdevel optional arch=any
 libapt-pkg-doc deb doc optional arch=all
 libapt-pkg6.0 deb libs optional arch=any
Checksums-Sha1:
 4eff4bbd27abf86746c2ceb718b4eda11f1b321d 2197424 apt_2.2.4.tar.xz
Checksums-Sha256:
 6eecd04a4979bd2040b22a14571c15d342c4e1802b2023acb5aa19649b1f64ea 2197424 apt_2.2.4.tar.xz
Files:
 8a1f8bb7a5f85f81dadadb989e3b8ed0 2197424 apt_2.2.4.tar.xz

-----BEGIN PGP SIGNATURE-----

iQJDBAEBCgAtFiEET7WIqEwt3nmnTHeHb6RY3R2wP3EFAmDB0ykPHGpha0BkZWJp
YW4ub3JnAAoJEG+kWN0dsD9xKlEP/1NehzZWVCS71J9ys4Amlz1jPh5+kkU6YpAj
iHdmOeCWSXKXjikKoK8i+Kwrz4HcaQwsRiE8qwv9R8wwYkAlmwd2B1RcO5YhBmQZ
HrG1S/gQkBchGIHNeM0J8WfrH+lU4Z/zlgPg0PzJwwatUgZ8Msp4P4l5BFD0o6F2
LsYnTfx3GUcIZZjvmLOTdvSZ71AlGxkVgFOKN2LM0iDsqfKFudyzCnVhYxYuw1aQ
ltUChj1IgIAvkZWfgfu7cpt/iIlMOU2hAW44oaYJyJ6liW30H5+FPQvuU/lKQrKp
hN0Vwb7bYUyiNYu703wBQQpbkYwKHm3vA/YyA0+Uj6Uf80ezDXPRd6rqu2vzskS8
LD+PJ6VEIf5GmKpbwEtgRxR+Zfy3zrh5HS8880cW2HpFIpq0d+2j1/cOiY/A5A3y
Nb/+c3U8LLHpBx7XnhRn7yotK3SfruNR0rc8E8XAeXfe9vRyeIA/MTuAHpNaaxoy
8dQ9ntiYAzX0gz5P6Uoz+3FV0wB5dlxNNO2740umjJrQShHWIVg5xwBOpaPoBy7z
/Gq8vZRiqlyXehVsu57PiCDddOGUIfpnaltzVOSXfHqkIYtipwvuBw8ee5HEgGBI
W1wprCmkuCpAUajlIJwNk/7hkFsp1HeKW9IJ0gsEvL+NSyPTF/fE+5Ut1jLoErZ9
OcKRfh2D
=OgrZ
-----END PGP SIGNATURE-----
