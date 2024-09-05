-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: rsyslog
Binary: rsyslog, rsyslog-mysql, rsyslog-pgsql, rsyslog-mongodb, rsyslog-elasticsearch, rsyslog-kafka, rsyslog-gssapi, rsyslog-gnutls, rsyslog-openssl, rsyslog-relp, rsyslog-czmq, rsyslog-hiredis
Architecture: any
Version: 8.2102.0-2+deb11u1
Maintainer: Michael Biebl <biebl@debian.org>
Homepage: https://www.rsyslog.com/
Standards-Version: 4.5.1
Vcs-Browser: https://salsa.debian.org/debian/rsyslog
Vcs-Git: https://salsa.debian.org/debian/rsyslog.git
Build-Depends: debhelper-compat (= 13), dh-exec, autoconf-archive, zlib1g-dev, libhiredis-dev, default-libmysqlclient-dev, libpq-dev, libmongoc-dev, libcurl4-gnutls-dev, librdkafka-dev (>= 0.9.1), libkrb5-dev, libgnutls28-dev, libssl-dev, librelp-dev (>= 1.4.0), libestr-dev (>= 0.1.9), liblognorm-dev (>= 2.0.3), libfastjson-dev (>= 0.99.8), libczmq-dev (>= 3.0.2), uuid-dev, libsystemd-dev (>= 209) [linux-any], pkg-config, bison, flex, procps <!nocheck>, python3 <!nocheck>, libfaketime <!nocheck>, systemd [linux-any] <!nocheck>, lsof [linux-any] <!nocheck>, logrotate <!nocheck>, iproute2 <!nocheck>, curl <!nocheck>
Package-List:
 rsyslog deb admin important arch=any
 rsyslog-czmq deb admin optional arch=any
 rsyslog-elasticsearch deb admin optional arch=any
 rsyslog-gnutls deb admin optional arch=any
 rsyslog-gssapi deb admin optional arch=any
 rsyslog-hiredis deb admin optional arch=any
 rsyslog-kafka deb admin optional arch=any
 rsyslog-mongodb deb admin optional arch=any
 rsyslog-mysql deb admin optional arch=any
 rsyslog-openssl deb admin optional arch=any
 rsyslog-pgsql deb admin optional arch=any
 rsyslog-relp deb admin optional arch=any
Checksums-Sha1:
 fdda78ed808e7a0dca03ead9227a0a5d913a050f 3123684 rsyslog_8.2102.0.orig.tar.gz
 8392d443c5fc4ea6e2064a93c9bc595ac45f6ab4 30620 rsyslog_8.2102.0-2+deb11u1.debian.tar.xz
Checksums-Sha256:
 94ee0d0312c2edea737665594cbe4a9475e4e3b593e12b5b8ae3a743ac9c72a7 3123684 rsyslog_8.2102.0.orig.tar.gz
 a8af4719b549b006bfe8be7278c3fb743037db8b8c85715c1b0da5e492dee73a 30620 rsyslog_8.2102.0-2+deb11u1.debian.tar.xz
Files:
 1f6150dfd2ef38db37c2165e98d2f2b1 3123684 rsyslog_8.2102.0.orig.tar.gz
 1526ed39ebbeb52e3f3f89d1bd0ebee2 30620 rsyslog_8.2102.0-2+deb11u1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCAAdFiEECbOsLssWnJBDRcxUauHfDWCPItwFAmKIHI8ACgkQauHfDWCP
ItxZLg/9EWzbA7DAV/RQqey+WHUnCSWXi6E32t0SJ6zND0Z8vLm1iCW9CbLhSKc1
Zm7k7OWb73MwarT/8a0CfvwUTEp0OLmqcW5hdxRfsZtrQkSKBJX9P7fmi4QhWKgt
KXggC+J9yHkOq+w7jV9znMvvEzcEDXroWvD1O9bWju9lv6P9wg+vmaTWo6pegPqB
1Vr7Q0prvDeLzULVez4j6WIDKQjL1/IMRkdxyDj2cRd38j3eyXfTxHcXkIU0s8V7
75QM8sg73D6Hz0euPt9J/tFrCQs1eEneG9YqhxLUmkq7Pc/gqP5/T2ro2N0KZtMX
B+/abavRdiEqNwBzHE3sDa6HG7G/tlk/GwXNsGkvhLwh3XMcRK8+UzgLIbs2CtRN
V8SZrvI1n6NX/zMlXcpJFPGnt3eTbEJ/AQfFi0a2Qt/k3sl5T1Rlz0qcHZCDgsZC
CBk/gt86dBCWjlKIXbboxZdWEiHBZix6Vj0ygDHlwDFou6wJXVfdkSxwwVY3U+RB
nQH0i9Xi7034GMUM3MN9DwJZ4XaEVwOEoy1Rps9+HdZ5X+cBSZgIAOe5XDPiL2UY
uQQg6uepF9F9eg8hOgE6zb1qaiy5Z6FbrMPl9YXcVJnEsRfs2jJYGRrz+KZsbsaj
m5WHfJA32+45BjZwqbrwFR7SJLRWV3vQsZv4c6JEWuuUIdnoIEY=
=VMgz
-----END PGP SIGNATURE-----
