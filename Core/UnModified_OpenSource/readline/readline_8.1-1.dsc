-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: readline
Binary: libreadline8, lib64readline8, readline-common, libreadline-dev, lib64readline-dev, rlfe, lib32readline8, lib32readline-dev, readline-doc
Architecture: any all
Version: 8.1-1
Maintainer: Matthias Klose <doko@debian.org>
Standards-Version: 4.5.1
Build-Depends: debhelper (>= 11), libncurses-dev, lib32ncurses-dev [amd64 ppc64], lib64ncurses-dev [i386 powerpc sparc s390], mawk | awk, texinfo, gcc-multilib [amd64 i386 kfreebsd-amd64 powerpc ppc64 s390 sparc]
Package-List:
 lib32readline-dev deb libdevel optional arch=amd64,ppc64
 lib32readline8 deb libs optional arch=amd64,ppc64
 lib64readline-dev deb libdevel optional arch=i386,powerpc,s390,sparc
 lib64readline8 deb libs optional arch=i386,powerpc,s390,sparc
 libreadline-dev deb libdevel optional arch=any
 libreadline8 deb libs optional arch=any
 readline-common deb utils optional arch=all
 readline-doc deb doc optional arch=all
 rlfe deb utils optional arch=any profile=!cross
Checksums-Sha1:
 48c5e0c3a212449e206e21ba82856accac842c6f 2993288 readline_8.1.orig.tar.gz
 fb373c8ff5004a45ec9e7efbac8d00e0a74b916d 29220 readline_8.1-1.debian.tar.xz
Checksums-Sha256:
 f8ceb4ee131e3232226a17f51b164afc46cd0b9e6cef344be87c65962cb82b02 2993288 readline_8.1.orig.tar.gz
 852267a95aeec23b267c838469fee346e83a29e7a08071178dc87682591cffbf 29220 readline_8.1-1.debian.tar.xz
Files:
 e9557dd5b1409f5d7b37ef717c64518e 2993288 readline_8.1.orig.tar.gz
 139411bd8fb351302b86df09d45f9f0a 29220 readline_8.1-1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQJEBAEBCAAuFiEE1WVxuIqLuvFAv2PWvX6qYHePpvUFAl/PJQIQHGRva29AZGVi
aWFuLm9yZwAKCRC9fqpgd4+m9Ye9EACmvlYqPz8H2jYl1MdY2Y0Y9jiWg+0UiQ7N
7wcObYG8F8tFhdXnwfx5Fyrq27m/uPoZbwdNVldPqDxG3lOB4GTvVnIj/kzdwghH
2/D0n8WmgaAIsPattl2f0zCuKvmWL7Kpy+nYNXGDOpbtwzzqD8K5GL1Z9EYwTa2H
hEXEBrVKf/7BqyP3dtzSS71M8uhGiTNhQ7Vsjdvax8TSMYy0D49gScp87jXMFB4A
UCZTDg6yrSXwyaWmc85QI1v0EWvY1UYCot7ozicx4sidvZ8DLQTB1CQ+4lfpAE6w
c539xwxlMCgPO3/f2abKpWOOyCL6uRTRiqsXQriEgKMNVs8CuSutm6GbwLVQCUTS
RjDE1wQY/oh91rkpXfUJKHvxvA2lcABxVkmJ+seEUSDVOMHtkq3Z0WqWCJMygjZF
BKfI5lTx971z5JzgxdHoP9B75jDHknRUvI74NC7RcDuAASb3DsahtOk59L9uEgmQ
CmDmTkIFjtW0DtQYBSj36zlkRPqOLTUdoP4/Fhtsg+sG4FSNNSAEVHQ5jy2GBLs7
BEPa23pZ0O+7S5QNN/x9UU3IYjcqYv/n89eAa9OmRPfVmO1gszsiQEtO+5lFnuW9
7xdLP7e+6KCXiyAEqfVzoeaQyB91kcEMc7QG24YiOBgU3gx7nskL9QVV6wV+EXDb
f+9c7R084w==
=Pno5
-----END PGP SIGNATURE-----
