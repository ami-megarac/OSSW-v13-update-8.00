-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: rsyslog
Binary: rsyslog, rsyslog-mysql, rsyslog-pgsql, rsyslog-mongodb, rsyslog-elasticsearch, rsyslog-kafka, rsyslog-gssapi, rsyslog-gnutls, rsyslog-openssl, rsyslog-relp, rsyslog-czmq, rsyslog-hiredis, rsyslog-snmp, rsyslog-kubernetes
Architecture: any
Version: 8.2302.0-1~bpo11+1
Maintainer: Michael Biebl <biebl@debian.org>
Homepage: https://www.rsyslog.com/
Standards-Version: 4.6.2
Vcs-Browser: https://salsa.debian.org/debian/rsyslog
Vcs-Git: https://salsa.debian.org/debian/rsyslog.git
Build-Depends: debhelper-compat (= 13), dh-exec, autoconf-archive, zlib1g-dev, libzstd-dev (>= 1.4.0), libhiredis-dev, default-libmysqlclient-dev, libpq-dev, libmongoc-dev, libcurl4-openssl-dev, librdkafka-dev (>= 0.9.1), libkrb5-dev, libgnutls28-dev, libssl-dev, librelp-dev (>= 1.4.0), libestr-dev (>= 0.1.9), liblognorm-dev (>= 2.0.3), libfastjson-dev (>= 0.99.8), libczmq-dev (>= 3.0.2), libsnmp-dev, uuid-dev, libsystemd-dev (>= 209) [linux-any], pkg-config, bison, flex, procps <!nocheck>, python3 <!nocheck>, libfaketime <!nocheck>, systemd [linux-any] <!nocheck>, lsof [linux-any] <!nocheck>, logrotate <!nocheck>, iproute2 [linux-any] <!nocheck>, curl <!nocheck>
Package-List:
 rsyslog deb admin optional arch=any
 rsyslog-czmq deb admin optional arch=any
 rsyslog-elasticsearch deb admin optional arch=any
 rsyslog-gnutls deb admin optional arch=any
 rsyslog-gssapi deb admin optional arch=any
 rsyslog-hiredis deb admin optional arch=any
 rsyslog-kafka deb admin optional arch=any
 rsyslog-kubernetes deb admin optional arch=any
 rsyslog-mongodb deb admin optional arch=any
 rsyslog-mysql deb admin optional arch=any
 rsyslog-openssl deb admin optional arch=any
 rsyslog-pgsql deb admin optional arch=any
 rsyslog-relp deb admin optional arch=any
 rsyslog-snmp deb admin optional arch=any
Checksums-Sha1:
 8d3b57c0c8f38e618ee2e49790dcfc26b27c40d5 3273655 rsyslog_8.2302.0.orig.tar.gz
 c44ff9025ec4480d525e6ace9d4ae40fb0bb7207 29120 rsyslog_8.2302.0-1~bpo11+1.debian.tar.xz
Checksums-Sha256:
 25415f85b662615ce3c83077d53758029e8743cb5929044bfd3564e3d626a3b9 3273655 rsyslog_8.2302.0.orig.tar.gz
 75b336cccb5270e9f3d4e978c4e72a94a71deb966ea32d2b8a8b3feb227b5b32 29120 rsyslog_8.2302.0-1~bpo11+1.debian.tar.xz
Files:
 2ab62877bbf45050251e2e2ba80eaf79 3273655 rsyslog_8.2302.0.orig.tar.gz
 0ba4af6d8fd328c1048c79ab6c003406 29120 rsyslog_8.2302.0-1~bpo11+1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCAAdFiEECbOsLssWnJBDRcxUauHfDWCPItwFAmP2iMAACgkQauHfDWCP
ItzXZQ/8DR97km7CwnC/q9j9SN/YHAgU4DWW3L20SxjAmve+9cLUxeA54zrxgELW
zsgX/SGLmNOwxJwqpuWw26YxVRjzm/WL2o2HOnfR+fe3Z1YcljS8OqREN05MXi3k
X+En8dvULIboZ5yNykVX9LZN9GMidaUrB6JwD98MlYjw8XKE4tkNSeB7KaAxIOCJ
rd1LyJ0r8Gg/5gGdX9viBrAywCK/EOolV3Lk4HVsPKkjWQWsFMYET95PdjIHprqJ
3VY+EV+Y23PI+AFYwDVoTaiflWUW5mNQdbEMXWyTm+pmTojr6m9d6u8VWkgaW8Sf
S2xYqlsexxf42ATn4KNvbKjxR7GRpZauxJbahs1zLj5lctcVMbhXjPMBBnPiVJep
W2BZZ/eAqNkNXdBFi+BrJesSu5KRzWcZIJHgXmKgcekvik3ZstL8/OPLVWQCt7vu
5TjAa/SNW0ZK52ySBpBVfblXIbpqieEkBMKOl+aYs4Td1SnMkGn0KRa5UteOLBfN
r32cz3yBNpQ2v72GdXlzqnr0vZl6xvUaQlsLM1xaIbH90yftpGdop/4Veo5Njvro
acG/j0oY0L5KIiDgEXJzZBiCDv9K0r27RoQgXprMzF8SBgYcv1WRcPBN2zQmyXRJ
mvl3jKTvfnOMk2z3zzHl9XDxfwM8DJdzFgchU2kNb3FfCWlIs4k=
=CZR2
-----END PGP SIGNATURE-----
