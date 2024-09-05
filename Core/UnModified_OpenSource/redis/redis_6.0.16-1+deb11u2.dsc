-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: redis
Binary: redis, redis-sentinel, redis-server, redis-tools
Architecture: any all
Version: 5:6.0.16-1+deb11u2
Maintainer: Chris Lamb <lamby@debian.org>
Homepage: https://redis.io/
Standards-Version: 4.5.1
Vcs-Browser: https://salsa.debian.org/lamby/pkg-redis
Vcs-Git: https://salsa.debian.org/lamby/pkg-redis.git -b debian/sid
Testsuite: autopkgtest
Build-Depends: debhelper-compat (= 13), dpkg-dev (>= 1.17.14), libhiredis-dev (>= 0.14.0), libjemalloc-dev [linux-any], liblua5.1-dev, liblzf-dev, libssl-dev, libsystemd-dev, lua-bitop-dev, lua-cjson-dev, openssl <!nocheck>, pkg-config, procps <!nocheck>, tcl <!nocheck>, tcl-tls <!nocheck>
Package-List:
 redis deb database optional arch=all
 redis-sentinel deb database optional arch=any
 redis-server deb database optional arch=any
 redis-tools deb database optional arch=any
Checksums-Sha1:
 381b94558450b967c0f6fa1e66497523f3c5da76 2307243 redis_6.0.16.orig.tar.gz
 cd9e181eef1125b7c7eb9cd1198791ac0a9f24ae 29764 redis_6.0.16-1+deb11u2.debian.tar.xz
Checksums-Sha256:
 8bea58a468bb67bedc92d8c2e44c170e42e6ea02527cbc5d233e92e8d78d1b99 2307243 redis_6.0.16.orig.tar.gz
 11dd7e429818fa7a4595849bc3223f4a23055d4e83b474e54102e5eeb922ad0d 29764 redis_6.0.16-1+deb11u2.debian.tar.xz
Files:
 cc0f506796970cf1454ee898e2bf7698 2307243 redis_6.0.16.orig.tar.gz
 33786e9d93c01b16352b2b25372a69b4 29764 redis_6.0.16-1+deb11u2.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCAAdFiEEwv5L0nHBObhsUz5GHpU+J9QxHlgFAmINc7kACgkQHpU+J9Qx
HlhgdxAAopW2ilYoig7ORV4VK+3ohb3Q3Z0Cf6k1S/WDrdrNify6iE21n/rbFWKd
lWVBmTGtH0AiyQVcVsb/p03I+Ruy7w3VmS+wav8fS+uQaJw54ZjRLrkNNTN4+o/l
BaLYE4tOoSkXMDpFgHQQkZOhSaksutK5nLP2UndLdnWFznbagBY+Sm3yeXaNBrQZ
rKngUQ6eG9CYbl8ZTCXK3/mIPv/XJzc5dM+gjaRJM0kfBhlokSkkiV3CaX+VFEuN
LlKTskQ33332NxzQ8Ol3FArWXJFgFbQ98DHhDjQd6dwshgnqpZ6ZbrIkVQTKeTI2
nwNxyLmodn69YCvJY3X0DrgMFwhSRTARe8KLHGuu2N+a6phiADcbAObXUQJngaS9
tT6LlfI/UDrrHtTN6ql2zceTrGKI757wwWgJPRDWlTykTzY6mFrJtdcagk4nb3bg
+twZW4nJvL6FTdz3zWmx5/2yhTBjsxy8LlnI0D98nEpeFU45xsOD6CjzJdU0iOGw
eQmqwhVeviifoG9dt101D6XlVlMcRvih/bAaHRA5nPEShg/lpnbyRrSKnLZmmDKg
OIfQ1H4yulDlacP2s7PcL2ExpFRrwXVZRPD9uqAUViesNsCKkIzoGclme1kcvfRG
bZ7yHSowgd8cZ1Q9YCxxuG61PNThe59fYXFqio741D6+OhknNJ8=
=Sbx7
-----END PGP SIGNATURE-----
