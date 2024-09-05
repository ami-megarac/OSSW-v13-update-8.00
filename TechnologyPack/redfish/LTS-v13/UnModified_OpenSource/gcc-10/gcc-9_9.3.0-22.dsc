-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: gcc-9
Binary: gcc-9-base, libgcc-9-dev, lib64gcc-9-dev, lib32gcc-9-dev, libn32gcc-9-dev, libx32gcc-9-dev, gcc-9, gcc-9-multilib, gcc-9-test-results, gcc-9-plugin-dev, gcc-9-hppa64-linux-gnu, cpp-9, gcc-9-locales, g++-9, g++-9-multilib, libasan5, lib32asan5, lib64asan5, libx32asan5, libgccjit-9-doc, libgccjit-9-dev, gobjc++-9, gobjc++-9-multilib, gobjc-9, gobjc-9-multilib, libobjc-9-dev, lib64objc-9-dev, lib32objc-9-dev, libn32objc-9-dev, libx32objc-9-dev, gfortran-9, gfortran-9-multilib, libgfortran-9-dev, lib64gfortran-9-dev, lib32gfortran-9-dev, libn32gfortran-9-dev, libx32gfortran-9-dev, gccgo-9, gccgo-9-multilib, libgo-9-dev, lib64go-9-dev, lib32go-9-dev, libn32go-9-dev, libx32go-9-dev, libgo14, lib64go14, lib32go14, libn32go14, libx32go14, libstdc++-9-dev, libstdc++-9-pic, libstdc++6-9-dbg, lib32stdc++-9-dev, lib32stdc++6-9-dbg, lib64stdc++-9-dev, lib64stdc++6-9-dbg, libn32stdc++-9-dev, libn32stdc++6-9-dbg, libx32stdc++-9-dev, libx32stdc++6-9-dbg, libstdc++-9-doc, gnat-9,
 libgnat-9, libgnatvsn9-dev, libgnatvsn9, gdc-9, gdc-9-multilib, libgphobos-9-dev, lib64gphobos-9-dev, lib32gphobos-9-dev, libn32gphobos-9-dev, libx32gphobos-9-dev, libgphobos76, lib64gphobos76, lib32gphobos76, libn32gphobos76, libx32gphobos76, gm2-9, libgm2-9-dev, libgm2-0, gccbrig-9, libhsail-rt-9-dev, gcc-9-offload-nvptx, libgomp-plugin-nvptx1, libgomp-plugin-hsa1,
 gcc-9-source
Architecture: any all
Version: 9.3.0-22
Maintainer: Debian GCC Maintainers <debian-gcc@lists.debian.org>
Uploaders: Matthias Klose <doko@debian.org>
Homepage: http://gcc.gnu.org/
Standards-Version: 4.5.1
Vcs-Browser: https://salsa.debian.org/toolchain-team/gcc/tree/gcc-9-debian
Vcs-Git: https://salsa.debian.org/toolchain-team/gcc.git -b gcc-9-debian
Testsuite: autopkgtest
Testsuite-Triggers: apt, libc-dev, libc6-dev, python3-minimal
Build-Depends: debhelper (>= 9.20141010), dpkg-dev (>= 1.17.14), g++-multilib [amd64 i386 kfreebsd-amd64 mips mips64 mips64el mips64r6 mips64r6el mipsel mipsn32 mipsn32el mipsn32r6 mipsn32r6el mipsr6 mipsr6el powerpc ppc64 s390 s390x sparc sparc64 x32] <!cross>, libc6.1-dev (>= 2.13-5) [alpha ia64] | libc0.3-dev (>= 2.13-5) [hurd-i386] | libc0.1-dev (>= 2.13-5) [kfreebsd-i386 kfreebsd-amd64] | libc6-dev (>= 2.13-5), libc6-dev (>= 2.13-31) [armel armhf], libc6-dev-amd64 [i386 x32], libc6-dev-sparc64 [sparc], libc6-dev-sparc [sparc64], libc6-dev-s390 [s390x], libc6-dev-s390x [s390], libc6-dev-i386 [amd64 x32], libc6-dev-powerpc [ppc64], libc6-dev-ppc64 [powerpc], libc0.1-dev-i386 [kfreebsd-amd64], lib32gcc-s1 [amd64 ppc64 kfreebsd-amd64 mipsn32 mipsn32el mips64 mips64el mipsn32r6 mipsn32r6el mips64r6 mips64r6el s390x sparc64 x32], libn32gcc-s1 [mips mipsel mips64 mips64el mipsr6 mipsr6el mips64r6 mips64r6el], lib64gcc-s1 [i386 mips mipsel mipsn32 mipsn32el mipsr6 mipsr6el mipsn32r6 mipsn32r6el powerpc sparc s390 x32], libc6-dev-mips64 [mips mipsel mipsn32 mipsn32el mipsr6 mipsr6el mipsn32r6 mipsn32r6el], libc6-dev-mipsn32 [mips mipsel mips64 mips64el mipsr6 mipsr6el mips64r6 mips64r6el], libc6-dev-mips32 [mipsn32 mipsn32el mips64 mips64el mipsn32r6 mipsn32r6el mips64r6 mips64r6el], libc6-dev-x32 [amd64 i386], libx32gcc-s1 [amd64 i386], libc6.1-dbg [alpha ia64] | libc0.3-dbg [hurd-i386] | libc0.1-dbg [kfreebsd-i386 kfreebsd-amd64] | libc6-dbg, kfreebsd-kernel-headers (>= 0.84) [kfreebsd-any], linux-libc-dev [m68k], m4, libtool, autoconf2.69, gcc-10-base, dwz, libunwind8-dev [ia64], libatomic-ops-dev [ia64], gawk, lzma, xz-utils, patchutils, zlib1g-dev, systemtap-sdt-dev [linux-any kfreebsd-any hurd-any], binutils:native (>= 2.33.1), binutils-hppa64-linux-gnu:native (>= 2.33.1) [hppa amd64 i386 x32], gperf (>= 3.0.1), bison (>= 1:2.3), flex, gettext, gdb:native [!riscv64], nvptx-tools [amd64 ppc64el], texinfo (>= 4.3), locales-all, sharutils, procps, gnat-9:native [!m32r !sh3 !sh3eb !sh4eb !m68k], g++-9:native, netbase, libpth-dev, python3:any, libisl-dev (>= 0.20), libmpc-dev (>= 1.0), libmpfr-dev (>= 3.0.0-9~), libgmp-dev (>= 2:5.0.1~), lib32z1-dev [amd64 kfreebsd-amd64], lib64z1-dev [i386], dejagnu [!m68k] <!nocheck>, coreutils (>= 2.26) | realpath (>= 1.9.12), chrpath, lsb-release, quilt, pkg-config, libgc-dev, g++-9-alpha-linux-gnu [alpha] <cross>, gobjc-9-alpha-linux-gnu [alpha] <cross>, gfortran-9-alpha-linux-gnu [alpha] <cross>, gdc-9-alpha-linux-gnu [alpha] <cross>, gccgo-9-alpha-linux-gnu [alpha] <cross>, gnat-9-alpha-linux-gnu [alpha] <cross>, gm2-9-alpha-linux-gnu [alpha] <cross>, g++-9-x86-64-linux-gnu [amd64] <cross>, gobjc-9-x86-64-linux-gnu [amd64] <cross>, gfortran-9-x86-64-linux-gnu [amd64] <cross>, gdc-9-x86-64-linux-gnu [amd64] <cross>, gccgo-9-x86-64-linux-gnu [amd64] <cross>, gnat-9-x86-64-linux-gnu [amd64] <cross>, gm2-9-x86-64-linux-gnu [amd64] <cross>, g++-9-arm-linux-gnueabi [armel] <cross>, gobjc-9-arm-linux-gnueabi [armel] <cross>, gfortran-9-arm-linux-gnueabi [armel] <cross>, gdc-9-arm-linux-gnueabi [armel] <cross>, gccgo-9-arm-linux-gnueabi [armel] <cross>, gnat-9-arm-linux-gnueabi [armel] <cross>, gm2-9-arm-linux-gnueabi [armel] <cross>, g++-9-arm-linux-gnueabihf [armhf] <cross>, gobjc-9-arm-linux-gnueabihf [armhf] <cross>, gfortran-9-arm-linux-gnueabihf [armhf] <cross>, gdc-9-arm-linux-gnueabihf [armhf] <cross>, gccgo-9-arm-linux-gnueabihf [armhf] <cross>, gnat-9-arm-linux-gnueabihf [armhf] <cross>, gm2-9-arm-linux-gnueabihf [armhf] <cross>, g++-9-aarch64-linux-gnu [arm64] <cross>, gobjc-9-aarch64-linux-gnu [arm64] <cross>, gfortran-9-aarch64-linux-gnu [arm64] <cross>, gdc-9-aarch64-linux-gnu [arm64] <cross>, gccgo-9-aarch64-linux-gnu [arm64] <cross>, gnat-9-aarch64-linux-gnu [arm64] <cross>, gm2-9-aarch64-linux-gnu [arm64] <cross>, g++-9-i686-linux-gnu [i386] <cross>, gobjc-9-i686-linux-gnu [i386] <cross>, gfortran-9-i686-linux-gnu [i386] <cross>, gdc-9-i686-linux-gnu [i386] <cross>, gccgo-9-i686-linux-gnu [i386] <cross>, gnat-9-i686-linux-gnu [i386] <cross>, gm2-9-i686-linux-gnu [i386] <cross>, g++-9-mipsel-linux-gnu [mipsel] <cross>, gobjc-9-mipsel-linux-gnu [mipsel] <cross>, gfortran-9-mipsel-linux-gnu [mipsel] <cross>, gdc-9-mipsel-linux-gnu [mipsel] <cross>, gccgo-9-mipsel-linux-gnu [mipsel] <cross>, gnat-9-mipsel-linux-gnu [mipsel] <cross>, gm2-9-mipsel-linux-gnu [mipsel] <cross>, g++-9-mips64-linux-gnuabi64 [mips64] <cross>, gobjc-9-mips64-linux-gnuabi64 [mips64] <cross>, gfortran-9-mips64-linux-gnuabi64 [mips64] <cross>, gdc-9-mips64-linux-gnuabi64 [mips64] <cross>, gccgo-9-mips64-linux-gnuabi64 [mips64] <cross>, gnat-9-mips64-linux-gnuabi64 [mips64] <cross>, gm2-9-mips64-linux-gnuabi64 [mips64] <cross>, g++-9-mips64el-linux-gnuabi64 [mips64el] <cross>, gobjc-9-mips64el-linux-gnuabi64 [mips64el] <cross>, gfortran-9-mips64el-linux-gnuabi64 [mips64el] <cross>, gdc-9-mips64el-linux-gnuabi64 [mips64el] <cross>, gccgo-9-mips64el-linux-gnuabi64 [mips64el] <cross>, gnat-9-mips64el-linux-gnuabi64 [mips64el] <cross>, gm2-9-mips64el-linux-gnuabi64 [mips64el] <cross>, g++-9-mips64-linux-gnuabin32 [mipsn32] <cross>, gobjc-9-mips64-linux-gnuabin32 [mipsn32] <cross>, gfortran-9-mips64-linux-gnuabin32 [mipsn32] <cross>, gdc-9-mips64-linux-gnuabin32 [mipsn32] <cross>, gccgo-9-mips64-linux-gnuabin32 [mipsn32] <cross>, gnat-9-mips64-linux-gnuabin32 [mipsn32] <cross>, gm2-9-mips64-linux-gnuabin32 [mipsn32] <cross>, g++-9-powerpc-linux-gnu [powerpc] <cross>, gobjc-9-powerpc-linux-gnu [powerpc] <cross>, gfortran-9-powerpc-linux-gnu [powerpc] <cross>, gdc-9-powerpc-linux-gnu [powerpc] <cross>, gccgo-9-powerpc-linux-gnu [powerpc] <cross>, gnat-9-powerpc-linux-gnu [powerpc] <cross>, g++-9-powerpc64-linux-gnu [ppc64] <cross>, gobjc-9-powerpc64-linux-gnu [ppc64] <cross>, gfortran-9-powerpc64-linux-gnu [ppc64] <cross>, gdc-9-powerpc64-linux-gnu [ppc64] <cross>, gccgo-9-powerpc64-linux-gnu [ppc64] <cross>, gnat-9-powerpc64-linux-gnu [ppc64] <cross>, g++-9-powerpc64le-linux-gnu [ppc64el] <cross>, gobjc-9-powerpc64le-linux-gnu [ppc64el] <cross>, gfortran-9-powerpc64le-linux-gnu [ppc64el] <cross>, gdc-9-powerpc64le-linux-gnu [ppc64el] <cross>, gccgo-9-powerpc64le-linux-gnu [ppc64el] <cross>, gnat-9-powerpc64le-linux-gnu [ppc64el] <cross>, gm2-9-powerpc64le-linux-gnu [ppc64el] <cross>, g++-9-m68k-linux-gnu [m68k] <cross>, gobjc-9-m68k-linux-gnu [m68k] <cross>, gfortran-9-m68k-linux-gnu [m68k] <cross>, gdc-9-m68k-linux-gnu [m68k] <cross>, gm2-9-m68k-linux-gnu [m68k] <cross>, g++-9-riscv64-linux-gnu [riscv64] <cross>, gobjc-9-riscv64-linux-gnu [riscv64] <cross>, gfortran-9-riscv64-linux-gnu [riscv64] <cross>, gdc-9-riscv64-linux-gnu [riscv64] <cross>, gccgo-9-riscv64-linux-gnu [riscv64] <cross>, gnat-9-riscv64-linux-gnu [riscv64] <cross>, gm2-9-riscv64-linux-gnu [riscv64] <cross>, g++-9-sh4-linux-gnu [sh4] <cross>, gobjc-9-sh4-linux-gnu [sh4] <cross>, gfortran-9-sh4-linux-gnu [sh4] <cross>, gnat-9-sh4-linux-gnu [sh4] <cross>, g++-9-sparc64-linux-gnu [sparc64] <cross>, gobjc-9-sparc64-linux-gnu [sparc64] <cross>, gfortran-9-sparc64-linux-gnu [sparc64] <cross>, gdc-9-sparc64-linux-gnu [sparc64] <cross>, gccgo-9-sparc64-linux-gnu [sparc64] <cross>, gnat-9-sparc64-linux-gnu [sparc64] <cross>, gm2-9-sparc64-linux-gnu [sparc64] <cross>, g++-9-s390x-linux-gnu [s390x] <cross>, gobjc-9-s390x-linux-gnu [s390x] <cross>, gfortran-9-s390x-linux-gnu [s390x] <cross>, gdc-9-s390x-linux-gnu [s390x] <cross>, gccgo-9-s390x-linux-gnu [s390x] <cross>, gnat-9-s390x-linux-gnu [s390x] <cross>, gm2-9-s390x-linux-gnu [s390x] <cross>, g++-9-x86-64-linux-gnux32 [x32] <cross>, gobjc-9-x86-64-linux-gnux32 [x32] <cross>, gfortran-9-x86-64-linux-gnux32 [x32] <cross>, gdc-9-x86-64-linux-gnux32 [x32] <cross>, gccgo-9-x86-64-linux-gnux32 [x32] <cross>, gnat-9-x86-64-linux-gnux32 [x32] <cross>, gm2-9-x86-64-linux-gnux32 [x32] <cross>, g++-9-mips64el-linux-gnuabin32 [mipsn32el] <cross>, gobjc-9-mips64el-linux-gnuabin32 [mipsn32el] <cross>, gfortran-9-mips64el-linux-gnuabin32 [mipsn32el] <cross>, gdc-9-mips64el-linux-gnuabin32 [mipsn32el] <cross>, gccgo-9-mips64el-linux-gnuabin32 [mipsn32el] <cross>, gnat-9-mips64el-linux-gnuabin32 [mipsn32el] <cross>, gm2-9-mips64el-linux-gnuabin32 [mipsn32el] <cross>, g++-9-mipsisa32r6-linux-gnu [mipsr6] <cross>, gobjc-9-mipsisa32r6-linux-gnu [mipsr6] <cross>, gfortran-9-mipsisa32r6-linux-gnu [mipsr6] <cross>, gdc-9-mipsisa32r6-linux-gnu [mipsr6] <cross>, gccgo-9-mipsisa32r6-linux-gnu [mipsr6] <cross>, gnat-9-mipsisa32r6-linux-gnu [mipsr6] <cross>, gm2-9-mipsisa32r6-linux-gnu [mipsr6] <cross>, g++-9-mipsisa32r6el-linux-gnu [mipsr6el] <cross>, gobjc-9-mipsisa32r6el-linux-gnu [mipsr6el] <cross>, gfortran-9-mipsisa32r6el-linux-gnu [mipsr6el] <cross>, gdc-9-mipsisa32r6el-linux-gnu [mipsr6el] <cross>, gccgo-9-mipsisa32r6el-linux-gnu [mipsr6el] <cross>, gnat-9-mipsisa32r6el-linux-gnu [mipsr6el] <cross>, gm2-9-mipsisa32r6el-linux-gnu [mipsr6el] <cross>, g++-9-mipsisa64r6-linux-gnuabi64 [mips64r6] <cross>, gobjc-9-mipsisa64r6-linux-gnuabi64 [mips64r6] <cross>, gfortran-9-mipsisa64r6-linux-gnuabi64 [mips64r6] <cross>, gdc-9-mipsisa64r6-linux-gnuabi64 [mips64r6] <cross>, gccgo-9-mipsisa64r6-linux-gnuabi64 [mips64r6] <cross>, gnat-9-mipsisa64r6-linux-gnuabi64 [mips64r6] <cross>, gm2-9-mipsisa64r6-linux-gnuabi64 [mips64r6] <cross>, g++-9-mipsisa64r6el-linux-gnuabi64 [mips64r6el] <cross>, gobjc-9-mipsisa64r6el-linux-gnuabi64 [mips64r6el] <cross>, gfortran-9-mipsisa64r6el-linux-gnuabi64 [mips64r6el] <cross>, gdc-9-mipsisa64r6el-linux-gnuabi64 [mips64r6el] <cross>, gccgo-9-mipsisa64r6el-linux-gnuabi64 [mips64r6el] <cross>, gnat-9-mipsisa64r6el-linux-gnuabi64 [mips64r6el] <cross>, gm2-9-mipsisa64r6el-linux-gnuabi64 [mips64r6el] <cross>, g++-9-mipsisa64r6-linux-gnuabin32 [mipsn32r6] <cross>, gobjc-9-mipsisa64r6-linux-gnuabin32 [mipsn32r6] <cross>, gfortran-9-mipsisa64r6-linux-gnuabin32 [mipsn32r6] <cross>, gdc-9-mipsisa64r6-linux-gnuabin32 [mipsn32r6] <cross>, gccgo-9-mipsisa64r6-linux-gnuabin32 [mipsn32r6] <cross>, gnat-9-mipsisa64r6-linux-gnuabin32 [mipsn32r6] <cross>, gm2-9-mipsisa64r6-linux-gnuabin32 [mipsn32r6] <cross>, g++-9-mipsisa64r6el-linux-gnuabin32 [mipsn32r6el] <cross>, gobjc-9-mipsisa64r6el-linux-gnuabin32 [mipsn32r6el] <cross>, gfortran-9-mipsisa64r6el-linux-gnuabin32 [mipsn32r6el] <cross>, gdc-9-mipsisa64r6el-linux-gnuabin32 [mipsn32r6el] <cross>, gccgo-9-mipsisa64r6el-linux-gnuabin32 [mipsn32r6el] <cross>, gnat-9-mipsisa64r6el-linux-gnuabin32 [mipsn32r6el] <cross>, gm2-9-mipsisa64r6el-linux-gnuabin32 [mipsn32r6el] <cross>
Build-Depends-Indep: doxygen (>= 1.7.2), graphviz (>= 2.2), ghostscript, texlive-latex-base, xsltproc, libxml2-utils, docbook-xsl-ns
Package-List:
 cpp-9 deb interpreters optional arch=any
 g++-9 deb devel optional arch=any
 g++-9-multilib deb devel optional arch=amd64,i386,kfreebsd-amd64,mips,mips64,mips64el,mips64r6,mips64r6el,mipsel,mipsn32,mipsn32el,mipsn32r6,mipsn32r6el,mipsr6,mipsr6el,powerpc,ppc64,s390,s390x,sparc,sparc64,x32
 gcc-9 deb devel optional arch=any
 gcc-9-base deb libs required arch=any
 gcc-9-hppa64-linux-gnu deb devel optional arch=hppa,amd64,i386,x32
 gcc-9-locales deb devel optional arch=all
 gcc-9-multilib deb devel optional arch=amd64,i386,kfreebsd-amd64,mips,mips64,mips64el,mips64r6,mips64r6el,mipsel,mipsn32,mipsn32el,mipsn32r6,mipsn32r6el,mipsr6,mipsr6el,powerpc,ppc64,s390,s390x,sparc,sparc64,x32
 gcc-9-offload-nvptx deb devel optional arch=amd64,ppc64el
 gcc-9-plugin-dev deb devel optional arch=any
 gcc-9-source deb devel optional arch=all
 gcc-9-test-results deb devel optional arch=any
 gccbrig-9 deb devel optional arch=any
 gccgo-9 deb devel optional arch=any
 gccgo-9-multilib deb devel optional arch=amd64,i386,kfreebsd-amd64,mips,mips64,mips64el,mips64r6,mips64r6el,mipsel,mipsn32,mipsn32el,mipsn32r6,mipsn32r6el,mipsr6,mipsr6el,powerpc,ppc64,s390,s390x,sparc,sparc64,x32
 gdc-9 deb devel optional arch=any
 gdc-9-multilib deb devel optional arch=any
 gfortran-9 deb devel optional arch=any
 gfortran-9-multilib deb devel optional arch=amd64,i386,kfreebsd-amd64,mips,mips64,mips64el,mips64r6,mips64r6el,mipsel,mipsn32,mipsn32el,mipsn32r6,mipsn32r6el,mipsr6,mipsr6el,powerpc,ppc64,s390,s390x,sparc,sparc64,x32
 gm2-9 deb devel optional arch=any
 gnat-9 deb devel optional arch=any
 gobjc++-9 deb devel optional arch=any
 gobjc++-9-multilib deb devel optional arch=amd64,i386,kfreebsd-amd64,mips,mips64,mips64el,mips64r6,mips64r6el,mipsel,mipsn32,mipsn32el,mipsn32r6,mipsn32r6el,mipsr6,mipsr6el,powerpc,ppc64,s390,s390x,sparc,sparc64,x32
 gobjc-9 deb devel optional arch=any
 gobjc-9-multilib deb devel optional arch=amd64,i386,kfreebsd-amd64,mips,mips64,mips64el,mips64r6,mips64r6el,mipsel,mipsn32,mipsn32el,mipsn32r6,mipsn32r6el,mipsr6,mipsr6el,powerpc,ppc64,s390,s390x,sparc,sparc64,x32
 lib32asan5 deb libs optional arch=amd64,ppc64,kfreebsd-amd64,s390x,sparc64,x32,mipsn32,mipsn32el,mips64,mips64el,mipsn32r6,mipsn32r6el,mips64r6,mips64r6el
 lib32gcc-9-dev deb libdevel optional arch=amd64,ppc64,kfreebsd-amd64,s390x,sparc64,x32,mipsn32,mipsn32el,mips64,mips64el,mipsn32r6,mipsn32r6el,mips64r6,mips64r6el
 lib32gfortran-9-dev deb libdevel optional arch=amd64,ppc64,kfreebsd-amd64,s390x,sparc64,x32,mipsn32,mipsn32el,mips64,mips64el,mipsn32r6,mipsn32r6el,mips64r6,mips64r6el
 lib32go-9-dev deb libdevel optional arch=amd64,ppc64,kfreebsd-amd64,s390x,sparc64,x32,mipsn32,mipsn32el,mips64,mips64el,mipsn32r6,mipsn32r6el,mips64r6,mips64r6el
 lib32go14 deb libs optional arch=amd64,ppc64,kfreebsd-amd64,s390x,sparc64,x32,mipsn32,mipsn32el,mips64,mips64el,mipsn32r6,mipsn32r6el,mips64r6,mips64r6el
 lib32gphobos-9-dev deb libdevel optional arch=amd64,ppc64,kfreebsd-amd64,s390x,sparc64,x32,mipsn32,mipsn32el,mips64,mips64el,mipsn32r6,mipsn32r6el,mips64r6,mips64r6el
 lib32gphobos76 deb libs optional arch=amd64,ppc64,kfreebsd-amd64,s390x,sparc64,x32,mipsn32,mipsn32el,mips64,mips64el,mipsn32r6,mipsn32r6el,mips64r6,mips64r6el
 lib32objc-9-dev deb libdevel optional arch=amd64,ppc64,kfreebsd-amd64,s390x,sparc64,x32,mipsn32,mipsn32el,mips64,mips64el,mipsn32r6,mipsn32r6el,mips64r6,mips64r6el
 lib32stdc++-9-dev deb libdevel optional arch=amd64,ppc64,kfreebsd-amd64,s390x,sparc64,x32,mipsn32,mipsn32el,mips64,mips64el,mipsn32r6,mipsn32r6el,mips64r6,mips64r6el
 lib32stdc++6-9-dbg deb debug optional arch=amd64,ppc64,kfreebsd-amd64,s390x,sparc64,x32,mipsn32,mipsn32el,mips64,mips64el,mipsn32r6,mipsn32r6el,mips64r6,mips64r6el
 lib64asan5 deb libs optional arch=i386,powerpc,sparc,s390,mips,mipsel,mipsn32,mipsn32el,mipsr6,mipsr6el,mipsn32r6,mipsn32r6el,x32
 lib64gcc-9-dev deb libdevel optional arch=i386,powerpc,sparc,s390,mips,mipsel,mipsn32,mipsn32el,mipsr6,mipsr6el,mipsn32r6,mipsn32r6el,x32
 lib64gfortran-9-dev deb libdevel optional arch=i386,powerpc,sparc,s390,mips,mipsel,mipsn32,mipsn32el,mipsr6,mipsr6el,mipsn32r6,mipsn32r6el,x32
 lib64go-9-dev deb libdevel optional arch=i386,powerpc,sparc,s390,mips,mipsel,mipsn32,mipsn32el,mipsr6,mipsr6el,mipsn32r6,mipsn32r6el,x32
 lib64go14 deb libs optional arch=i386,powerpc,sparc,s390,mips,mipsel,mipsn32,mipsn32el,mipsr6,mipsr6el,mipsn32r6,mipsn32r6el,x32
 lib64gphobos-9-dev deb libdevel optional arch=i386,powerpc,sparc,s390,mips,mipsel,mipsn32,mipsn32el,mipsr6,mipsr6el,mipsn32r6,mipsn32r6el,x32
 lib64gphobos76 deb libs optional arch=i386,powerpc,sparc,s390,mips,mipsel,mipsn32,mipsn32el,mipsr6,mipsr6el,mipsn32r6,mipsn32r6el,x32
 lib64objc-9-dev deb libdevel optional arch=i386,powerpc,sparc,s390,mips,mipsel,mipsn32,mipsn32el,mipsr6,mipsr6el,mipsn32r6,mipsn32r6el,x32
 lib64stdc++-9-dev deb libdevel optional arch=i386,powerpc,sparc,s390,mips,mipsel,mipsn32,mipsn32el,mipsr6,mipsr6el,mipsn32r6,mipsn32r6el,x32
 lib64stdc++6-9-dbg deb debug optional arch=i386,powerpc,sparc,s390,mips,mipsel,mipsn32,mipsn32el,mipsr6,mipsr6el,mipsn32r6,mipsn32r6el,x32
 libasan5 deb libs optional arch=any
 libgcc-9-dev deb libdevel optional arch=any
 libgccjit-9-dev deb libdevel optional arch=any
 libgccjit-9-doc deb doc optional arch=all
 libgfortran-9-dev deb libdevel optional arch=any
 libgm2-0 deb libs optional arch=any
 libgm2-9-dev deb libdevel optional arch=any
 libgnat-9 deb libs optional arch=any
 libgnatvsn9 deb libs optional arch=any
 libgnatvsn9-dev deb libdevel optional arch=any
 libgo-9-dev deb libdevel optional arch=any
 libgo14 deb libs optional arch=any
 libgomp-plugin-hsa1 deb libs optional arch=amd64
 libgomp-plugin-nvptx1 deb libs optional arch=amd64,ppc64el
 libgphobos-9-dev deb libdevel optional arch=amd64,arm64,armel,armhf,i386,x32,kfreebsd-amd64,kfreebsd-i386,hppa,mips,mips64,mipsel,mips64el,mipsn32,mipsn32el,mipsr6,mipsr6el,mipsn32r6,mipsn32r6el,mips64r6,mips64r6el,riscv64,s390x
 libgphobos76 deb libs optional arch=amd64,arm64,armel,armhf,i386,x32,kfreebsd-amd64,kfreebsd-i386,hppa,mips,mips64,mipsel,mips64el,mipsn32,mipsn32el,mipsr6,mipsr6el,mipsn32r6,mipsn32r6el,mips64r6,mips64r6el,riscv64,s390x
 libhsail-rt-9-dev deb libdevel optional arch=any
 libn32gcc-9-dev deb libdevel optional arch=mips,mipsel,mips64,mips64el,mipsr6,mipsr6el,mips64r6,mips64r6el
 libn32gfortran-9-dev deb libdevel optional arch=mips,mipsel,mips64,mips64el,mipsr6,mipsr6el,mips64r6,mips64r6el
 libn32go-9-dev deb libdevel optional arch=mips,mipsel,mips64,mips64el,mipsr6,mipsr6el,mips64r6,mips64r6el
 libn32go14 deb libs optional arch=mips,mipsel,mips64,mips64el,mipsr6,mipsr6el,mips64r6,mips64r6el
 libn32gphobos-9-dev deb libdevel optional arch=mips,mipsel,mips64,mips64el,mipsr6,mipsr6el,mips64r6,mips64r6el
 libn32gphobos76 deb libs optional arch=mips,mipsel,mips64,mips64el,mipsr6,mipsr6el,mips64r6,mips64r6el
 libn32objc-9-dev deb libdevel optional arch=mips,mipsel,mips64,mips64el,mipsr6,mipsr6el,mips64r6,mips64r6el
 libn32stdc++-9-dev deb libdevel optional arch=mips,mipsel,mips64,mips64el,mipsr6,mipsr6el,mips64r6,mips64r6el
 libn32stdc++6-9-dbg deb debug optional arch=mips,mipsel,mips64,mips64el,mipsr6,mipsr6el,mips64r6,mips64r6el
 libobjc-9-dev deb libdevel optional arch=any
 libstdc++-9-dev deb libdevel optional arch=any
 libstdc++-9-doc deb doc optional arch=all
 libstdc++-9-pic deb libdevel optional arch=any
 libstdc++6-9-dbg deb debug optional arch=any
 libx32asan5 deb libs optional arch=amd64,i386
 libx32gcc-9-dev deb libdevel optional arch=amd64,i386
 libx32gfortran-9-dev deb libdevel optional arch=amd64,i386
 libx32go-9-dev deb libdevel optional arch=amd64,i386
 libx32go14 deb libs optional arch=amd64,i386
 libx32gphobos-9-dev deb libdevel optional arch=amd64,i386
 libx32gphobos76 deb libs optional arch=amd64,i386
 libx32objc-9-dev deb libdevel optional arch=amd64,i386
 libx32stdc++-9-dev deb libdevel optional arch=amd64,i386
 libx32stdc++6-9-dbg deb debug optional arch=amd64,i386
Checksums-Sha1:
 e571c860cceaf89ef069c1b6b5ba87a907ea0fbd 88686943 gcc-9_9.3.0.orig.tar.gz
 34ad9b848576db51e3e7f0c375b6571ea4409ac3 904252 gcc-9_9.3.0-22.debian.tar.xz
Checksums-Sha256:
 824044ffa96eb337bb1c1d4cf6a82691d0290d6f42e1d13362eea855458de060 88686943 gcc-9_9.3.0.orig.tar.gz
 68d55260456847880c71831b69c19cb81e9d1abf09274ab77ab6c081e177d94d 904252 gcc-9_9.3.0-22.debian.tar.xz
Files:
 e749b397634522b971e49557e8002ef7 88686943 gcc-9_9.3.0.orig.tar.gz
 82fc3ce28521f73a2e7ddd07ff503017 904252 gcc-9_9.3.0-22.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQJEBAEBCAAuFiEE1WVxuIqLuvFAv2PWvX6qYHePpvUFAmAgKP0QHGRva29AZGVi
aWFuLm9yZwAKCRC9fqpgd4+m9XmxEADW6q6O80JNwRBIH7OYJYeBODq5F+LYmqls
s4muZ263r613BICZtH8jCfPpQRtpaVgMQHiXxTYC8u1PhYLs9mNISJZeAyePM6DL
pt1cOwCI/mN9m92dBzJV1EWvAu2Om1YLndIYjeXgcLijE6CGCtsAxWKhOjyysvJH
/r1fIi+/0B+McOTZbnP4qOsBZqQYSnPt00zFhH/l7YnnahwT/NJqwmdFwEZhI+ld
sZnVJCruDw5pfk/IdKUneKUyVL8/0Hc+eczgavZjh2D23u622i2OREkECiKTp5tI
k8DMWd9AnN5G9BlJGuhtb0iMgVMbE8UbGl+1D44ELAS5q/2hLybJ4b3cOBF8JSMY
PIsxoHnBeIpoipR7+TLkuJMgsTfp1DWPUyvD3TBpp5RWBL8jhjzAzBheEGqiX2rU
VbHqn2ww0KdyirrNMD45TcfNsdWRsFoArrI54C/SaCbpsKE61trho5C8W6H3o/DZ
wpXJucXp6TzqUNuBlQgIDxQrAerj5q+EGnuXjbS8gc6rA5DxCZw87y2r3hFQt2xD
SxsCwjwNJmlGqLzttnEnuPnOyDOGOMvHJxURfYwfG0bFgJEku/rY8/K+e5cu3jO8
L6l7y3//0DAExFCXr/7MYZwfaLZbp+ZmfPdHd6AdHqP6NfaXIHompU6WvCO0iNVa
e5O1/A8NEw==
=ZHmT
-----END PGP SIGNATURE-----
