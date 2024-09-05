-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: libselinux
Binary: selinux-utils, libselinux1, libselinux1-dev, libselinux1-udeb, ruby-selinux, python3-selinux
Architecture: linux-any
Version: 3.1-3
Maintainer: Debian SELinux maintainers <selinux-devel@lists.alioth.debian.org>
Uploaders: Laurent Bigonville <bigon@debian.org>, Russell Coker <russell@coker.com.au>
Homepage: https://selinuxproject.org
Standards-Version: 4.5.1
Vcs-Browser: https://salsa.debian.org/selinux-team/libselinux
Vcs-Git: https://salsa.debian.org/selinux-team/libselinux.git
Testsuite: autopkgtest
Testsuite-Triggers: build-essential, pkg-config, python3-all
Build-Depends: debhelper-compat (= 13), dh-python <!nopython>, file, gem2deb (>= 0.5.0~) <!noruby>, libsepol1-dev (>= 3.1), libpcre2-dev, pkg-config, python3-all-dev <!nopython>, swig <!nopython> <!noruby>
Package-List:
 libselinux1 deb libs optional arch=linux-any
 libselinux1-dev deb libdevel optional arch=linux-any
 libselinux1-udeb udeb debian-installer optional arch=linux-any
 python3-selinux deb python optional arch=linux-any profile=!nopython
 ruby-selinux deb ruby optional arch=linux-any profile=!noruby
 selinux-utils deb admin optional arch=linux-any
Checksums-Sha1:
 f687894176f0b69a4e0e14b936685c72cb41a084 204703 libselinux_3.1.orig.tar.gz
 9a409aeb6eae48fdf720aff83dad97eaf766be82 24176 libselinux_3.1-3.debian.tar.xz
Checksums-Sha256:
 ea5dcbb4d859e3f999c26a13c630da2f16dff9462e3cc8cb7b458ac157d112e7 204703 libselinux_3.1.orig.tar.gz
 7170ab6914f0d2e93de169da312df961f799f5d58cc0a4c552e3f8a7882f3c81 24176 libselinux_3.1-3.debian.tar.xz
Files:
 693680c021feb69a4b258b0370021461 204703 libselinux_3.1.orig.tar.gz
 963bf4bb91c9abfa0684f497e32d2983 24176 libselinux_3.1-3.debian.tar.xz
Ruby-Versions: all

-----BEGIN PGP SIGNATURE-----

iQFFBAEBCAAvFiEEmRrdqQAhuF2x31DwH8WJHrqwQ9UFAmAiVk4RHGJpZ29uQGRl
Ymlhbi5vcmcACgkQH8WJHrqwQ9WRoAf/WrbLKtTSG9BEmrOXBXZKzE1r+nGznF5r
X7IVlNOkqpB/HzPKU8pdVlWCLRW9F5XMTseqmycL7FW25p08iG/EX5FkpJDuc5Hb
LBpnhwei8SJlCCgMGd82flq322cYQDNEBrjie3V+NOd20cUsEZotGY0gd0zdtK7j
5tr05oqSS6s/sKcixsQvWGLSCrVThZVXwxHuLMo7oMr5SJMY1J6sEhThdeIw2WzA
YRr6hj6jqTW51yLGOD6FiTRWl4gcroMPLGspNzz+fslJ4dBaQbE28t5SDvR1Vn83
QNoKr+OQuFH3RVDrXPIC5UeWtW79yLjTFwYF0YXPXEIbywKt0QrSOQ==
=srKh
-----END PGP SIGNATURE-----
