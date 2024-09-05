-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: hiredis
Binary: libhiredis0.14, libhiredis-dev
Architecture: any
Version: 0.14.1-1
Maintainer: Chris Lamb <lamby@debian.org>
Homepage: https://github.com/redis/hiredis
Standards-Version: 4.5.0
Vcs-Browser: https://salsa.debian.org/lamby/pkg-hiredis
Vcs-Git: https://salsa.debian.org/lamby/pkg-hiredis.git
Testsuite: autopkgtest
Testsuite-Triggers: gcc, libc-dev, pkg-config
Build-Depends: debhelper-compat (= 12), procps <!nocheck>, redis-server [linux-any kfreebsd-any] <!nocheck>
Package-List:
 libhiredis-dev deb libdevel optional arch=any
 libhiredis0.14 deb libs optional arch=any
Checksums-Sha1:
 a422493d601aa36cd14504a6791e7609093af8ed 64372 hiredis_0.14.1.orig.tar.gz
 e09f855cc54ee576641741b28f2d91d6351998bd 8872 hiredis_0.14.1-1.debian.tar.xz
Checksums-Sha256:
 2663b2aed9fd430507e30fc5e63274ee40cdd1a296026e22eafd7d99b01c8913 64372 hiredis_0.14.1.orig.tar.gz
 581d935f9039e1cce32a3b2141b2b2363f7abd5f2aa1faffd51a7966ffaadcac 8872 hiredis_0.14.1-1.debian.tar.xz
Files:
 3e1c541f9df28becb82a611e63e3e939 64372 hiredis_0.14.1.orig.tar.gz
 d0ef92c6f301ac5023f7953cf7816ff0 8872 hiredis_0.14.1-1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCAAdFiEEwv5L0nHBObhsUz5GHpU+J9QxHlgFAl57ZVMACgkQHpU+J9Qx
HlhxfA//aRXNUL/REHMoXqW02HIPp6JZrab3fHn1SJc3WFUO0Vq98+4Fv/GRBBq+
uTvbv9EHQU8+0SFII9S1aHj6TWQ07IDQXKKjHWFUcKc50znyt41kV+Nshk0hLAhD
DhXsJI7B6UfO188BC6KVzsjI0OuhMGuyVhr83MrwhjWxNKn3ibLrHW53YfQRraAI
R0GbeU6Bfk3PaEYJcG9H3I9kE2FRw9dbxoTMAtRsHIUrH3XTZFxk4P+X/zsIJ4Ho
BgcZQYP653Hs3sRhtL1tOiKVb/yJ0wjjeiwfQnftM/YeoLCRZhifc24hKckmPwve
w651wgxyE691G58M8qNzNcTD/wmJcd/vIiZuxmzGNtb3KlkrrMFPbXfuV4jwkwLn
rzdJxFQzF74tEpLGXjIObmby0hqujtshIAZPw/eQ7x4Lsk4hdThiG6kanYiw7KkN
Xrk/zqVYyepwn497M0Lz/2bNPXwRz+ts+nJ9zOJ1RdUfRVFmFuKxmARRxA4qfpXC
omKP0tHmB45UXLLFJ9nJU5GymOG0UvB1nWXpmuhXJWkMrEbQGwsPpIoAlrjdiJtt
UdP9uFeQaVNXeIDpbbHR+VgznREZY/AiPIG8goB5GUxi4UsFh4/rNbvamNVIWZDg
Phco1LAnpWKDPpYFBsMetY+v76AibPGsPmpmqLiRJKhKa5XoRX0=
=7ijB
-----END PGP SIGNATURE-----
