-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: binutils
Binary: binutils-for-host, binutils-for-build, binutils, libctf-nobfd0, libctf-nobfd0-dbg, libctf0, libctf0-dbg, libbinutils, libbinutils-dbg, binutils-dev, binutils-multiarch, binutils-multiarch-dbg, binutils-multiarch-dev, binutils-hppa64-linux-gnu, binutils-hppa64-linux-gnu-dbg, binutils-doc, binutils-source, binutils-common, binutils-x86-64-linux-gnu, binutils-x86-64-linux-gnu-dbg, binutils-i686-linux-gnu, binutils-i686-linux-gnu-dbg, binutils-aarch64-linux-gnu, binutils-aarch64-linux-gnu-dbg, binutils-arm-linux-gnueabihf, binutils-arm-linux-gnueabihf-dbg, binutils-arm-linux-gnueabi, binutils-arm-linux-gnueabi-dbg, binutils-powerpc64le-linux-gnu, binutils-powerpc64le-linux-gnu-dbg, binutils-s390x-linux-gnu, binutils-s390x-linux-gnu-dbg, binutils-alpha-linux-gnu, binutils-alpha-linux-gnu-dbg, binutils-hppa-linux-gnu, binutils-hppa-linux-gnu-dbg, binutils-ia64-linux-gnu, binutils-ia64-linux-gnu-dbg, binutils-m68k-linux-gnu, binutils-m68k-linux-gnu-dbg,
 binutils-powerpc-linux-gnu, binutils-powerpc-linux-gnu-dbg, binutils-powerpc64-linux-gnu, binutils-powerpc64-linux-gnu-dbg, binutils-riscv64-linux-gnu, binutils-riscv64-linux-gnu-dbg, binutils-sh4-linux-gnu, binutils-sh4-linux-gnu-dbg, binutils-sparc64-linux-gnu, binutils-sparc64-linux-gnu-dbg, binutils-x86-64-linux-gnux32, binutils-x86-64-linux-gnux32-dbg, binutils-i686-gnu, binutils-i686-gnu-dbg, binutils-x86-64-kfreebsd-gnu, binutils-x86-64-kfreebsd-gnu-dbg, binutils-i686-kfreebsd-gnu, binutils-i686-kfreebsd-gnu-dbg, binutils-mips-linux-gnu, binutils-mips-linux-gnu-dbg, binutils-mipsel-linux-gnu, binutils-mipsel-linux-gnu-dbg, binutils-mips64-linux-gnuabi64, binutils-mips64-linux-gnuabi64-dbg, binutils-mips64el-linux-gnuabi64, binutils-mips64el-linux-gnuabi64-dbg, binutils-mips64-linux-gnuabin32, binutils-mips64-linux-gnuabin32-dbg, binutils-mips64el-linux-gnuabin32, binutils-mips64el-linux-gnuabin32-dbg, binutils-mipsisa64r6-linux-gnuabin32,
 binutils-mipsisa64r6-linux-gnuabin32-dbg, binutils-mipsisa64r6el-linux-gnuabin32, binutils-mipsisa64r6el-linux-gnuabin32-dbg, binutils-mipsisa32r6-linux-gnu, binutils-mipsisa32r6-linux-gnu-dbg, binutils-mipsisa32r6el-linux-gnu, binutils-mipsisa32r6el-linux-gnu-dbg, binutils-mipsisa64r6-linux-gnuabi64, binutils-mipsisa64r6-linux-gnuabi64-dbg, binutils-mipsisa64r6el-linux-gnuabi64,
 binutils-mipsisa64r6el-linux-gnuabi64-dbg
Architecture: any all
Version: 2.35.2-2
Maintainer: Matthias Klose <doko@debian.org>
Uploaders: James Troup <binutils@elmo.tasta.io>
Homepage: https://www.gnu.org/software/binutils/
Standards-Version: 4.5.1
Vcs-Browser: //salsa.debian.org/toolchain-team/binutils/tree/binutils-2.35
Vcs-Git: https://salsa.debian.org/toolchain-team/binutils.git
Testsuite: autopkgtest
Testsuite-Triggers: autoconf, bison, build-essential, chrpath, debugedit, dejagnu, dwz, fakeroot, file, flex, gettext, libstdc++-dev, lsb-release, procps, python3, quilt, texinfo, xz-utils, zlib1g-dev
Build-Depends: autoconf (>= 2.64), dpkg-dev (>= 1.19.0.5), bison, flex, gettext, texinfo, dejagnu, quilt, chrpath, dwz, debugedit (>= 4.16), python3:any, file, xz-utils, lsb-release, zlib1g-dev, procps, g++-aarch64-linux-gnu [amd64 i386 x32] <!nocheck>, g++-arm-linux-gnueabi [amd64 arm64 i386 x32] <!nocheck>, g++-arm-linux-gnueabihf [amd64 arm64 i386 x32] <!nocheck>, g++-powerpc64le-linux-gnu [amd64 arm64 i386 ppc64 x32] <!nocheck>, g++-s390x-linux-gnu [amd64 arm64 i386 ppc64el x32] <!nocheck>, g++-alpha-linux-gnu [amd64 i386 x32] <!nocheck>, g++-hppa-linux-gnu [amd64 i386 x32] <!nocheck>, g++-m68k-linux-gnu [amd64 i386 x32] <!nocheck>, g++-powerpc-linux-gnu [amd64 i386 ppc64el x32] <!nocheck>, g++-powerpc64-linux-gnu [amd64 i386 x32] <!nocheck>, g++-riscv64-linux-gnu [amd64 arm64 i386 ppc64el x32] <!nocheck>, g++-sh4-linux-gnu [amd64 i386 x32] <!nocheck>, g++-sparc64-linux-gnu [amd64 i386 x32] <!nocheck>, g++-i686-linux-gnu [amd64 arm64 ppc64el x32] <!nocheck>, g++-x86-64-linux-gnu [arm64 i386 ppc64el] <!nocheck>, g++-x86-64-linux-gnux32 [amd64 arm64 i386 ppc64el] <!nocheck>
Build-Conflicts: libelf-dev
Package-List:
 binutils deb devel optional arch=any
 binutils-aarch64-linux-gnu deb devel optional arch=arm64,amd64,i386,x32,ppc64el
 binutils-aarch64-linux-gnu-dbg deb debug optional arch=arm64,amd64,i386,x32,ppc64el
 binutils-alpha-linux-gnu deb devel optional arch=alpha,amd64,i386,x32
 binutils-alpha-linux-gnu-dbg deb debug optional arch=alpha,amd64,i386,x32
 binutils-arm-linux-gnueabi deb devel optional arch=armel,amd64,i386,x32,arm64,ppc64el
 binutils-arm-linux-gnueabi-dbg deb debug optional arch=armel,amd64,i386,x32,arm64,ppc64el
 binutils-arm-linux-gnueabihf deb devel optional arch=armhf,amd64,i386,x32,arm64,ppc64el
 binutils-arm-linux-gnueabihf-dbg deb debug optional arch=armhf,amd64,i386,x32,arm64,ppc64el
 binutils-common deb devel optional arch=any
 binutils-dev deb devel optional arch=any
 binutils-doc deb doc optional arch=all
 binutils-for-build deb devel optional arch=all
 binutils-for-host deb devel optional arch=any
 binutils-hppa-linux-gnu deb devel optional arch=hppa,amd64,i386,x32
 binutils-hppa-linux-gnu-dbg deb debug optional arch=hppa,amd64,i386,x32
 binutils-hppa64-linux-gnu deb devel optional arch=amd64,i386,x32,hppa
 binutils-hppa64-linux-gnu-dbg deb debug optional arch=amd64,i386,x32,hppa
 binutils-i686-gnu deb devel optional arch=hurd-i386,amd64,i386,x32
 binutils-i686-gnu-dbg deb debug optional arch=hurd-i386,amd64,i386,x32
 binutils-i686-kfreebsd-gnu deb devel optional arch=kfreebsd-i386,amd64,i386,x32
 binutils-i686-kfreebsd-gnu-dbg deb debug optional arch=kfreebsd-i386,amd64,i386,x32
 binutils-i686-linux-gnu deb devel optional arch=i386,amd64,arm64,ppc64el,x32
 binutils-i686-linux-gnu-dbg deb debug optional arch=i386,amd64,arm64,ppc64el,x32
 binutils-ia64-linux-gnu deb devel optional arch=ia64,amd64,i386,x32
 binutils-ia64-linux-gnu-dbg deb debug optional arch=ia64,amd64,i386,x32
 binutils-m68k-linux-gnu deb devel optional arch=m68k,amd64,i386,x32
 binutils-m68k-linux-gnu-dbg deb debug optional arch=m68k,amd64,i386,x32
 binutils-mips-linux-gnu deb devel optional arch=mips
 binutils-mips-linux-gnu-dbg deb debug optional arch=mips
 binutils-mips64-linux-gnuabi64 deb devel optional arch=mips64
 binutils-mips64-linux-gnuabi64-dbg deb debug optional arch=mips64
 binutils-mips64-linux-gnuabin32 deb devel optional arch=mipsn32
 binutils-mips64-linux-gnuabin32-dbg deb debug optional arch=mipsn32
 binutils-mips64el-linux-gnuabi64 deb devel optional arch=mips64el
 binutils-mips64el-linux-gnuabi64-dbg deb debug optional arch=mips64el
 binutils-mips64el-linux-gnuabin32 deb devel optional arch=mipsn32el
 binutils-mips64el-linux-gnuabin32-dbg deb debug optional arch=mipsn32el
 binutils-mipsel-linux-gnu deb devel optional arch=mipsel
 binutils-mipsel-linux-gnu-dbg deb debug optional arch=mipsel
 binutils-mipsisa32r6-linux-gnu deb devel optional arch=mipsr6
 binutils-mipsisa32r6-linux-gnu-dbg deb debug optional arch=mipsr6
 binutils-mipsisa32r6el-linux-gnu deb devel optional arch=mipsr6el
 binutils-mipsisa32r6el-linux-gnu-dbg deb debug optional arch=mipsr6el
 binutils-mipsisa64r6-linux-gnuabi64 deb devel optional arch=mips64r6
 binutils-mipsisa64r6-linux-gnuabi64-dbg deb debug optional arch=mips64r6
 binutils-mipsisa64r6-linux-gnuabin32 deb devel optional arch=mipsn32r6
 binutils-mipsisa64r6-linux-gnuabin32-dbg deb debug optional arch=mipsn32r6
 binutils-mipsisa64r6el-linux-gnuabi64 deb devel optional arch=mips64r6el
 binutils-mipsisa64r6el-linux-gnuabi64-dbg deb debug optional arch=mips64r6el
 binutils-mipsisa64r6el-linux-gnuabin32 deb devel optional arch=mipsn32r6el
 binutils-mipsisa64r6el-linux-gnuabin32-dbg deb debug optional arch=mipsn32r6el
 binutils-multiarch deb devel optional arch=any
 binutils-multiarch-dbg deb debug optional arch=any
 binutils-multiarch-dev deb devel optional arch=any
 binutils-powerpc-linux-gnu deb devel optional arch=powerpc,amd64,i386,x32,ppc64el
 binutils-powerpc-linux-gnu-dbg deb debug optional arch=powerpc,amd64,i386,x32,ppc64el
 binutils-powerpc64-linux-gnu deb devel optional arch=ppc64,amd64,i386,x32,ppc64el
 binutils-powerpc64-linux-gnu-dbg deb debug optional arch=ppc64,amd64,i386,x32,ppc64el
 binutils-powerpc64le-linux-gnu deb devel optional arch=ppc64el,amd64,i386,x32,ppc64,arm64
 binutils-powerpc64le-linux-gnu-dbg deb debug optional arch=ppc64el,amd64,i386,x32,ppc64,arm64
 binutils-riscv64-linux-gnu deb devel optional arch=riscv64,amd64,i386,x32,arm64,ppc64el
 binutils-riscv64-linux-gnu-dbg deb debug optional arch=riscv64,amd64,i386,x32,arm64,ppc64el
 binutils-s390x-linux-gnu deb devel optional arch=s390x,amd64,i386,x32,arm64,ppc64el
 binutils-s390x-linux-gnu-dbg deb debug optional arch=s390x,amd64,i386,x32,arm64,ppc64el
 binutils-sh4-linux-gnu deb devel optional arch=sh4,amd64,i386,x32
 binutils-sh4-linux-gnu-dbg deb debug optional arch=sh4,amd64,i386,x32
 binutils-source deb devel optional arch=all
 binutils-sparc64-linux-gnu deb devel optional arch=sparc64,amd64,i386,x32
 binutils-sparc64-linux-gnu-dbg deb debug optional arch=sparc64,amd64,i386,x32
 binutils-x86-64-kfreebsd-gnu deb devel optional arch=kfreebsd-amd64,amd64,i386,x32
 binutils-x86-64-kfreebsd-gnu-dbg deb debug optional arch=kfreebsd-amd64,amd64,i386,x32
 binutils-x86-64-linux-gnu deb devel optional arch=amd64,arm64,i386,ppc64el,x32
 binutils-x86-64-linux-gnu-dbg deb debug optional arch=amd64,arm64,i386,ppc64el,x32
 binutils-x86-64-linux-gnux32 deb devel optional arch=x32,amd64,arm64,i386,ppc64el
 binutils-x86-64-linux-gnux32-dbg deb debug optional arch=x32,amd64,arm64,i386,ppc64el
 libbinutils deb devel optional arch=any
 libbinutils-dbg deb debug optional arch=any
 libctf-nobfd0 deb devel optional arch=any
 libctf-nobfd0-dbg deb debug optional arch=any
 libctf0 deb devel optional arch=any
 libctf0-dbg deb debug optional arch=any
Checksums-Sha1:
 90c2c4815e7e4f651a6bc410ec4a116d2a4b6482 23514376 binutils_2.35.2.orig.tar.xz
 90e45c950ce3261d72777d0fd8076b1c2f13bb10 101124 binutils_2.35.2-2.debian.tar.xz
Checksums-Sha256:
 2643d99d7aba8557319a4b018f6bcae58677fc9bc853d4c2cd2eb571867b75e7 23514376 binutils_2.35.2.orig.tar.xz
 96f56b4d5259be49ce4a2f27057892356f7b0aed499742cb8a07f9f62b10dcb5 101124 binutils_2.35.2-2.debian.tar.xz
Files:
 c79158e329a60b3018f0ea8572adfb8c 23514376 binutils_2.35.2.orig.tar.xz
 d519f51a23c1265151601d0ac88c8948 101124 binutils_2.35.2-2.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQJEBAEBCAAuFiEE1WVxuIqLuvFAv2PWvX6qYHePpvUFAmAxMDsQHGRva29AZGVi
aWFuLm9yZwAKCRC9fqpgd4+m9dH2EACaXvDySVZhXcZcEVde1W7pmgHfTVbEcmNI
G/Ac7BEDClnA6F3ID0+IMdlMtDR8CZnRmfPxh8bF/hvdYXBRGI0D/MBQSWVnLr7k
F78UV0kwkA8+MnXjWtwh8asmaLPkGNc2/3fXOMD7G8YxVfHEhb0MhbID1WqZYtNi
rjmIQvm7X8RzAvHTdFObz5nD38cNVR84wzXAUKcad45JCjmuzl6h5PRgi1QzaDYp
RKVVjniLdrudzkxGiIMkvKDtIAq9YsPBZL42K+MXJe0IJ3jBzAx4enZQ6Q+HR+Oo
psv9nMFMLW8jgR0vwRGqrC7RY7/CHGZUTSkby18eiFugSkyU7+pq3TRDm//b5DGW
BOa1d9ePvIGoH/pMktaWfPEH9S3sHRum4/PvOEDIoWqR5knYzgrY1XZTONlS3jWg
SUlxRxbZ1kyg/JsYRNg0CEv/zyBY8fsjMf2zV0gEtgzNMAduxtz+ghUawOx6hbzC
jGmI4wvZSGuzhF1AN+CYLOI9HD7CNLsxQehH6lIft3F/5+nksljKRNi/ezuWI5MK
RJzQgyRdYVw8L2PZCtI4BGP2nhE+17w0JtjXwHW0Fz0GGgxjOFVsGwdn1vTNGeLh
i/AizclIIRE0S7AUkWBlXDDxdaSS6leDXSyHPY0sOlavlqglAleiB7LqNu5HX31F
PCIn7mATaw==
=Netp
-----END PGP SIGNATURE-----
