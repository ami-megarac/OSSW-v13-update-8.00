-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: gdbm
Binary: libgdbm6, gdbm-l10n, libgdbm-dev, gdbmtool, libgdbm-compat4, libgdbm-compat-dev
Architecture: any all
Version: 1.19-2
Maintainer: Dmitry Bogatov <KAction@debian.org>
Homepage: https://gnu.org/software/gdbm
Standards-Version: 4.2.1
Vcs-Browser: https://salsa.debian.org/debian/gdbm
Vcs-Git: https://salsa.debian.org/debian/gdbm.git
Testsuite: autopkgtest
Testsuite-Triggers: @builddeps@, build-essential
Build-Depends: texinfo, debhelper-compat (= 11), dh-exec, dietlibc-dev (>= 0.34~cvs20160606-3) [alpha amd64 arm64 armeb armel armhf hppa i386 mips mipsel mips64el powerpc powerpcspe ppc64 ppc64el s390x sparc64 x32] <!pkg.gdbm.nodietlibc>, libreadline-dev, bison
Package-List:
 gdbm-l10n deb localization optional arch=all
 gdbmtool deb utils optional arch=any
 libgdbm-compat-dev deb libdevel optional arch=any
 libgdbm-compat4 deb libs optional arch=any
 libgdbm-dev deb libdevel optional arch=any
 libgdbm6 deb libs optional arch=any
Checksums-Sha1:
 2b6cc7453341904432b457fb8a2450e5f2aaf331 967861 gdbm_1.19.orig.tar.gz
 91c84182b9f7c3db9a18371d55b6a3888584b29f 181 gdbm_1.19.orig.tar.gz.asc
 601e83c0b7b7efa9a4bd2f0a2be72d8a1fa35957 16228 gdbm_1.19-2.debian.tar.xz
Checksums-Sha256:
 37ed12214122b972e18a0d94995039e57748191939ef74115b1d41d8811364bc 967861 gdbm_1.19.orig.tar.gz
 8f4e0502073a7a22972f1edc84c70151033257d879402ac85176d3ebc984b2b8 181 gdbm_1.19.orig.tar.gz.asc
 c49d2faaa340acc8e94277dab0e0bf4ac10d49d9c374d6841883ac868edb5014 16228 gdbm_1.19-2.debian.tar.xz
Files:
 aeb29c6a90350a4c959cd1df38cd0a7e 967861 gdbm_1.19.orig.tar.gz
 9912185cbc609abdd9b1b1f0d3dfed29 181 gdbm_1.19.orig.tar.gz.asc
 8a3e9911d3e52798472fa04c51e24def 16228 gdbm_1.19-2.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCAAdFiEEkpeKbhleSSGCX3/w808JdE6fXdkFAmAFw9MACgkQ808JdE6f
Xdk8eQ//cbyAbr+taAgSbMtkXP/fPsW6zzZFsQwkZ0ML536GBddbmiuw8ZHyu308
Iivnk0tuo59xkRWM+dO2kC6VlriryJfztz08sXnSrAWBJzFqmOQMMCOxvEaGmbmk
l/YC7af/WKEBwmQCF7i+ohKRgpftSBeeqNYEFxdaqVTnpw38txnkvtBIkwI4H3QG
Igc2Niq3OqGlTPpEkq6CM24RxcgzqsHhnwGuCzHKkUdPRZFFjgi7qO+fFOxwJl2y
Qes/dq7kkFWtHkd2m0ul6czFJyAlY2H1io8FwaaJcVq0O6lB8PSGG0MmZSI3gEK3
dASnrNw4qIMd26v1K367FsyF/+0INOkc+ZerpOunrI4AlBa6UnH4hYMe5vIzT59Y
8hCncmsN5OP/qvD1s/tRMBhKIsbe2pqbgDSSAlAhDxYoUA5bWq2h9pf9ciqcctJ/
WwTdOldv0m7xtb6yR3YE0lsPtqe8NPoH9IBTX3MoicjtPEae4c+udQxkmIhDh4Kv
vzXXjY885CZEfs3BpGCl8tAQ7JAZRMj8JzVH/eFM1//Yk8B6BhmKwxXZ0SAZ0z+c
Tgodi6ozbE8jrVVdjfl+1bocp80QltE5eY8GuedCBVSkEK8yAqAl6PyTGKZLLzNF
QPHdKsphDwWZlJtglBAUOvo27qwDzPHyPjfBhOdFls+bwFgl88A=
=2P0u
-----END PGP SIGNATURE-----
