-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: python-cryptography
Binary: python3-cryptography, python-cryptography-doc
Architecture: any all
Version: 3.3.2-1
Maintainer: Tristan Seligmann <mithrandi@debian.org>
Uploaders:  Debian Python Team <team+python@tracker.debian.org>, Sandro Tosi <morph@debian.org>,
Homepage: https://cryptography.io/
Standards-Version: 4.5.0
Vcs-Browser: https://salsa.debian.org/python-team/packages/python-cryptography
Vcs-Git: https://salsa.debian.org/python-team/packages/python-cryptography.git
Testsuite: autopkgtest
Testsuite-Triggers: python3-all, python3-cffi, python3-cryptography-vectors, python3-hypothesis, python3-iso8601, python3-pretend, python3-pyasn1-modules, python3-pytest, python3-tz
Build-Depends: debhelper-compat (= 13), dh-sequence-python3, dh-sequence-sphinxdoc <!nodoc>, dpkg-dev (>= 1.17.14), libssl-dev, python3-all-dev, python3-asn1crypto (>= 0.21.0~), python3-cffi (>= 1.7~), python3-cryptography-vectors (<< 3.3.3~) <!nocheck>, python3-cryptography-vectors (>= 3.3.2~) <!nocheck>, python3-doc <!nodoc>, python3-hypothesis <!nocheck>, python3-idna (>= 2.0~), python3-iso8601 <!nocheck>, python3-pretend <!nocheck>, python3-pytest (>= 2.9.0) <!nocheck>, python3-setuptools (>= 11.3), python3-six (>= 1.4.1~), python3-sphinx <!nodoc>, python3-sphinx-rtd-theme <!nodoc>, python3-tz <!nocheck>
Package-List:
 python-cryptography-doc deb doc optional arch=all profile=!nodoc
 python3-cryptography deb python optional arch=any
Checksums-Sha1:
 eb8be696115458f9368432525e9cae11d0f6bebf 539883 python-cryptography_3.3.2.orig.tar.gz
 212cc382d2fed2a70b00e0fb2dd72f3b59de7c63 488 python-cryptography_3.3.2.orig.tar.gz.asc
 71e630a030948d7ba5cdd411971a9a726fc1a8a1 11852 python-cryptography_3.3.2-1.debian.tar.xz
Checksums-Sha256:
 5a60d3780149e13b7a6ff7ad6526b38846354d11a15e21068e57073e29e19bed 539883 python-cryptography_3.3.2.orig.tar.gz
 4e662a721607429b6d233c140d0884e367b7d9de6867de92e90ded52f9267930 488 python-cryptography_3.3.2.orig.tar.gz.asc
 91ade8b576bbfe3596624204d9660e711e84c697592d47f232083e08eb80c828 11852 python-cryptography_3.3.2-1.debian.tar.xz
Files:
 e2ce2ec8a63965fad351f36ed70fde4b 539883 python-cryptography_3.3.2.orig.tar.gz
 cbcb0cccb5b2b5879764251ae4f8dcb0 488 python-cryptography_3.3.2.orig.tar.gz.asc
 181e641cf7ffcc59c189c0cb4241970b 11852 python-cryptography_3.3.2-1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQGTBAEBCgB9FiEEXAZWhXVRbQoz/6ejwImQ+x9jeJMFAmAlBAdfFIAAAAAALgAo
aXNzdWVyLWZwckBub3RhdGlvbnMub3BlbnBncC5maWZ0aGhvcnNlbWFuLm5ldDVD
MDY1Njg1NzU1MTZEMEEzM0ZGQTdBM0MwODk5MEZCMUY2Mzc4OTMACgkQwImQ+x9j
eJNPiQf9HNQYDFzLEcXiaHP/ZMiiqJHEcSy2leMwKSNQVpPJwjzVICpOPrn8bcoH
DqH9VgGLcJHblG2gPp6yTPJWoE0+Jsyg35osfF+yHLy+DM8LxLlnFLZvIlZd9gjz
5tl2aypTfIpPPmsggVU1IVlY/+MCEMqZds+SccCPmPyc9cBwoPpt3tLlItwPO7gR
phHKQEGwuQMfQXLka/zvTL/A4/wONVYueSpjtmk5weCbX+Nit6iEHVZn99rR1Q75
Vs8oeFvhLDECIm0iIJ4/WHyPrQd6yFVSv0BTo/r2dMjIjg0dgNGmAItcKKlDqs4a
1uFB/XS0VhUExhB4AP2pE/ZIxRIKgQ==
=8aKr
-----END PGP SIGNATURE-----
