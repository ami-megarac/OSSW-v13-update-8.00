-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: audit
Binary: auditd, libauparse0, libauparse-dev, libaudit1, libaudit-common, libaudit-dev, python3-audit, golang-redhat-audit-dev, audispd-plugins
Architecture: linux-any all
Version: 1:3.0-2
Maintainer: Laurent Bigonville <bigon@debian.org>
Homepage: https://people.redhat.com/sgrubb/audit/
Standards-Version: 4.5.1
Vcs-Browser: https://salsa.debian.org/debian/audit
Vcs-Git: https://salsa.debian.org/debian/audit.git
Testsuite: autopkgtest
Testsuite-Triggers: build-essential, pkg-config, python3-all
Build-Depends: debhelper-compat (= 12), dh-python <!nopython>, dpkg-dev (>= 1.16.1~), intltool, libcap-ng-dev, libkrb5-dev, libldap2-dev <!pkg.audit.noldap>, libwrap0-dev, linux-libc-dev (>= 5.9~), python3-all-dev:any <!nopython>, libpython3-all-dev <!nopython>, swig
Build-Depends-Indep: golang-go
Package-List:
 audispd-plugins deb admin optional arch=linux-any profile=!pkg.audit.noldap
 auditd deb admin optional arch=linux-any
 golang-redhat-audit-dev deb devel optional arch=all
 libaudit-common deb libs optional arch=all
 libaudit-dev deb libdevel optional arch=linux-any
 libaudit1 deb libs optional arch=linux-any
 libauparse-dev deb libdevel optional arch=linux-any
 libauparse0 deb libs optional arch=linux-any
 python3-audit deb python optional arch=linux-any profile=!nopython
Checksums-Sha1:
 57077653bf6e56062a31e5836cb50533b728ac20 1109442 audit_3.0.orig.tar.gz
 d1148539c643079bf025c923437d537c62b147bb 18640 audit_3.0-2.debian.tar.xz
Checksums-Sha256:
 bd31826823b912b6fe271d2d979ed879e9fc393cab1e2f7c4e1af258231765b8 1109442 audit_3.0.orig.tar.gz
 10193fa9823eb66dfb1220fb109b8b8e01f3f720c5a1630e9015d92aa7a8ce3a 18640 audit_3.0-2.debian.tar.xz
Files:
 46d94d176e3e8c8b5b6999ae076b99d3 1109442 audit_3.0.orig.tar.gz
 15347d49dc0d258083f760f8ac148222 18640 audit_3.0-2.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQFFBAEBCAAvFiEEmRrdqQAhuF2x31DwH8WJHrqwQ9UFAl/1z28RHGJpZ29uQGRl
Ymlhbi5vcmcACgkQH8WJHrqwQ9Xs5Qf/ZnLOWRj9k3Hpcw2BqgIk0mSsGokJW0Du
73WuApAAB9z7q8aWP39TGYtJdwU1XvQaRWKTRp37RbSegnz1KZ/ZqgVPm74DauZ/
sOgFmeb5tuZkT/lQcG4cnRicMKauRzpn/A6pfjcn/tYvnEM77aOUQbhJRdJAcHgl
pISNFsXEe7k8ygZZxzIkUil1VFrl/LJFQ1YT4ILG9FUirFF1dSiADIdvWDoJgnYQ
/MWYZSvu2RAtft5WLvYBJwF+TVssf/tvMCPn/yLwwDhAbLvRIwfnfphKQIwEIOSt
v74ntxp7KCu7hnl5JG89deOU3MfA7DO0W+tciENoGQ1HSzgw7lC0rA==
=EogJ
-----END PGP SIGNATURE-----
