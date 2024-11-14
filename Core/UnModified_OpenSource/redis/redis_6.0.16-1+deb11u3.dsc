-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: redis
Binary: redis, redis-sentinel, redis-server, redis-tools
Architecture: any all
Version: 5:6.0.16-1+deb11u3
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
 11992e69cf876d4f0913e2f3f73e1904a4ff7aef 38220 redis_6.0.16-1+deb11u3.debian.tar.xz
Checksums-Sha256:
 8bea58a468bb67bedc92d8c2e44c170e42e6ea02527cbc5d233e92e8d78d1b99 2307243 redis_6.0.16.orig.tar.gz
 d0aad99a43604aef2408762eb22f7c222ca6ff8d4bd2c68cc867ca7b36af7506 38220 redis_6.0.16-1+deb11u3.debian.tar.xz
Files:
 cc0f506796970cf1454ee898e2bf7698 2307243 redis_6.0.16.orig.tar.gz
 91ba05e3b1e56c1d9b6bbe7661b7b9b0 38220 redis_6.0.16-1+deb11u3.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCAAdFiEEwv5L0nHBObhsUz5GHpU+J9QxHlgFAmbgOpoACgkQHpU+J9Qx
Hlgo1w//TtZW6tEg3CZ5vWZncsv/RzDBwydMV8V7ayAZTyHJcsS61ibs9WrL4Daf
qh2CfamjE2G6ZUWtsSQmOwwjvWJTgcBwB/h78XgPgkzYOcGcW2PbBDgbGD4XNQTI
+HB0TLp/kLo2MEkiDlCcXWxX5jHi6emUctIif0SrxdxqQHjTNf4Yd5d9DNt1CEWm
/AmoOSYoXbH3QxBeIRXjHMG1l4IAzuuDP4AwAbdm8m5UDXXcOq2BqniPba7vFHWP
mWhra96r/QHD1a5BXvM16CyHS6LvgKtvI5hOsNPKL5Y44XcUriKimlKsclroBFWM
T7K4A5+CE6+FtJTwmK9APZKk8Yx+9Fq7zSzNJneR8kwMLMbJomDN3ie7Kso5x8Ui
3oxUt9+gUWlkNrBdeJ0Kqu8KtSIEh7FT1OLjB6AhNB42K6RpmcXiwJyyW94xtYA7
HA3e+SysJJ+JIHd2hJn1cpPvL5yUwuEErZHxYKUBgFiDUirR+8xxHya+poNOZs0M
n6n6g/X4eXoMT3CN3bG83C1t+2NTIDP4XZFKtJLIlKaogWjuQlW9YmNcMyYg0pm5
Yk8FW13O9H8eDCI7Z/gjDyp8hL3Jej79i4CSRuz1V7B8+ZFv92hJ84LJWwyk0Nfy
0iY12f9dIYJC4DfzXi2/wNsa74gtcYiZz/24wNPH5x0tdR09DK0=
=I5TS
-----END PGP SIGNATURE-----
