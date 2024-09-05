-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: boost1.74
Binary: libboost1.74-dev, libboost1.74-tools-dev, libboost1.74-all-dev, libboost1.74-doc, libboost-atomic1.74.0, libboost-atomic1.74-dev, libboost-chrono1.74.0, libboost-chrono1.74-dev, libboost-container1.74.0, libboost-container1.74-dev, libboost-context1.74.0, libboost-context1.74-dev, libboost-contract1.74.0, libboost-contract1.74-dev, libboost-coroutine1.74.0, libboost-coroutine1.74-dev, libboost-date-time1.74.0, libboost-date-time1.74-dev, libboost-exception1.74-dev, libboost-fiber1.74.0, libboost-fiber1.74-dev, libboost-filesystem1.74.0, libboost-filesystem1.74-dev, libboost-graph1.74.0, libboost-graph1.74-dev, libboost-graph-parallel1.74.0, libboost-graph-parallel1.74-dev, libboost-iostreams1.74.0, libboost-iostreams1.74-dev, libboost-locale1.74.0, libboost-locale1.74-dev, libboost-log1.74.0, libboost-log1.74-dev, libboost-math1.74.0, libboost-math1.74-dev, libboost-mpi1.74.0, libboost-mpi1.74-dev, libboost-mpi-python1.74.0, libboost-mpi-python1.74-dev,
 libboost-nowide1.74.0, libboost-nowide1.74-dev, libboost-numpy1.74.0, libboost-numpy1.74-dev, libboost-program-options1.74.0, libboost-program-options1.74-dev, libboost-python1.74.0, libboost-python1.74-dev, libboost-random1.74.0, libboost-random1.74-dev, libboost-regex1.74.0, libboost-regex1.74-dev, libboost-serialization1.74.0, libboost-serialization1.74-dev, libboost-stacktrace1.74.0, libboost-stacktrace1.74-dev, libboost-system1.74.0, libboost-system1.74-dev, libboost-test1.74.0, libboost-test1.74-dev, libboost-thread1.74.0, libboost-thread1.74-dev, libboost-timer1.74.0, libboost-timer1.74-dev, libboost-type-erasure1.74.0, libboost-type-erasure1.74-dev, libboost-wave1.74.0,
 libboost-wave1.74-dev
Architecture: any all
Version: 1.74.0-9
Maintainer: Debian Boost Team <team+boost@tracker.debian.org>
Uploaders:  Steve M. Robbins <smr@debian.org>, Giovanni Mascellani <gio@debian.org>, Anton Gladky <gladk@debian.org>
Homepage: https://www.boost.org/
Standards-Version: 4.5.1
Vcs-Browser: https://salsa.debian.org/debian/boost
Vcs-Git: https://salsa.debian.org/debian/boost.git
Testsuite: autopkgtest
Testsuite-Triggers: build-essential, clang, cmake, html2text, libboost-atomic1.71-dev, libboost-chrono1.71-dev, libboost-container1.71-dev, libboost-context1.71-dev, libboost-coroutine1.71-dev, libboost-date-time1.71-dev, libboost-exception1.71-dev, libboost-fiber1.71-dev, libboost-filesystem1.71-dev, libboost-graph-parallel1.71-dev, libboost-graph1.71-dev, libboost-iostreams1.71-dev, libboost-locale1.71-dev, libboost-log1.71-dev, libboost-math1.71-dev, libboost-mpi-python1.71-dev, libboost-mpi1.71-dev, libboost-program-options1.71-dev, libboost-python1.71-dev, libboost-random1.71-dev, libboost-regex1.71-dev, libboost-serialization1.71-dev, libboost-stacktrace1.71-dev, libboost-system1.71-dev, libboost-test1.71-dev, libboost-thread1.71-dev, libboost-timer1.71-dev, libboost-type-erasure1.71-dev, libboost-wave1.71-dev, libgraph-easy-perl, mpi-default-dev, python3-minimal
Build-Depends: debhelper-compat (= 12), dpkg-dev (>= 1.16.1~), dctrl-tools, chrpath, zlib1g-dev, libbz2-dev, liblzma-dev, libzstd-dev, libicu-dev (>= 63.1), mpi-default-dev, bison, flex, docbook-to-man, help2man, xsltproc, doxygen, docbook-xsl, docbook-xml, texlive-latex-base, ghostscript, dh-python, g++ (>= 4:5-0), python3, python3-all-dev (>= 3.1), python3-numpy
Build-Depends-Indep: graphviz
Build-Conflicts: libopenmpi-dev (= 1.3.2-2)
Package-List:
 libboost-atomic1.74-dev deb libdevel optional arch=any
 libboost-atomic1.74.0 deb libs optional arch=any
 libboost-chrono1.74-dev deb libdevel optional arch=any
 libboost-chrono1.74.0 deb libs optional arch=any
 libboost-container1.74-dev deb libdevel optional arch=any
 libboost-container1.74.0 deb libs optional arch=any
 libboost-context1.74-dev deb libdevel optional arch=i386,hurd-i386,kfreebsd-i386,amd64,kfreebsd-amd64,armel,armhf,arm64,mips,mipsel,mips64el,powerpc,ppc64el,riscv64,s390x
 libboost-context1.74.0 deb libs optional arch=i386,hurd-i386,kfreebsd-i386,amd64,kfreebsd-amd64,armel,armhf,arm64,mips,mipsel,mips64el,powerpc,ppc64el,riscv64,s390x
 libboost-contract1.74-dev deb libdevel optional arch=any
 libboost-contract1.74.0 deb libs optional arch=any
 libboost-coroutine1.74-dev deb libdevel optional arch=i386,hurd-i386,kfreebsd-i386,amd64,kfreebsd-amd64,armel,armhf,arm64,mips,mipsel,mips64el,powerpc,ppc64el,riscv64,s390x
 libboost-coroutine1.74.0 deb libs optional arch=i386,hurd-i386,kfreebsd-i386,amd64,kfreebsd-amd64,armel,armhf,arm64,mips,mipsel,mips64el,powerpc,ppc64el,riscv64,s390x
 libboost-date-time1.74-dev deb libdevel optional arch=any
 libboost-date-time1.74.0 deb libs optional arch=any
 libboost-exception1.74-dev deb libdevel optional arch=any
 libboost-fiber1.74-dev deb libdevel optional arch=i386,hurd-i386,kfreebsd-i386,amd64,kfreebsd-amd64,armel,armhf,arm64,mips,mipsel,mips64el,powerpc,ppc64el,riscv64,s390x
 libboost-fiber1.74.0 deb libs optional arch=i386,hurd-i386,kfreebsd-i386,amd64,kfreebsd-amd64,armel,armhf,arm64,mips,mipsel,mips64el,powerpc,ppc64el,riscv64,s390x
 libboost-filesystem1.74-dev deb libdevel optional arch=any
 libboost-filesystem1.74.0 deb libs optional arch=any
 libboost-graph-parallel1.74-dev deb libdevel optional arch=any
 libboost-graph-parallel1.74.0 deb libs optional arch=any
 libboost-graph1.74-dev deb libdevel optional arch=any
 libboost-graph1.74.0 deb libs optional arch=any
 libboost-iostreams1.74-dev deb libdevel optional arch=any
 libboost-iostreams1.74.0 deb libs optional arch=any
 libboost-locale1.74-dev deb libdevel optional arch=any
 libboost-locale1.74.0 deb libs optional arch=any
 libboost-log1.74-dev deb libdevel optional arch=any
 libboost-log1.74.0 deb libs optional arch=any
 libboost-math1.74-dev deb libdevel optional arch=any
 libboost-math1.74.0 deb libs optional arch=any
 libboost-mpi-python1.74-dev deb libdevel optional arch=any
 libboost-mpi-python1.74.0 deb libs optional arch=any
 libboost-mpi1.74-dev deb libdevel optional arch=any
 libboost-mpi1.74.0 deb libs optional arch=any
 libboost-nowide1.74-dev deb libdevel optional arch=any
 libboost-nowide1.74.0 deb libs optional arch=any
 libboost-numpy1.74-dev deb libdevel optional arch=any
 libboost-numpy1.74.0 deb libs optional arch=any
 libboost-program-options1.74-dev deb libdevel optional arch=any
 libboost-program-options1.74.0 deb libs optional arch=any
 libboost-python1.74-dev deb libdevel optional arch=any
 libboost-python1.74.0 deb libs optional arch=any
 libboost-random1.74-dev deb libdevel optional arch=any
 libboost-random1.74.0 deb libs optional arch=any
 libboost-regex1.74-dev deb libdevel optional arch=any
 libboost-regex1.74.0 deb libs optional arch=any
 libboost-serialization1.74-dev deb libdevel optional arch=any
 libboost-serialization1.74.0 deb libs optional arch=any
 libboost-stacktrace1.74-dev deb libdevel optional arch=any
 libboost-stacktrace1.74.0 deb libs optional arch=any
 libboost-system1.74-dev deb libdevel optional arch=any
 libboost-system1.74.0 deb libs optional arch=any
 libboost-test1.74-dev deb libdevel optional arch=any
 libboost-test1.74.0 deb libs optional arch=any
 libboost-thread1.74-dev deb libdevel optional arch=any
 libboost-thread1.74.0 deb libs optional arch=any
 libboost-timer1.74-dev deb libdevel optional arch=any
 libboost-timer1.74.0 deb libs optional arch=any
 libboost-type-erasure1.74-dev deb libdevel optional arch=any
 libboost-type-erasure1.74.0 deb libs optional arch=any
 libboost-wave1.74-dev deb libdevel optional arch=any
 libboost-wave1.74.0 deb libs optional arch=any
 libboost1.74-all-dev deb libdevel optional arch=any
 libboost1.74-dev deb libdevel optional arch=any
 libboost1.74-doc deb doc optional arch=all
 libboost1.74-tools-dev deb libdevel optional arch=any
Checksums-Sha1:
 2e81412a00ccbc16551a57418e1e137dd559253b 60536256 boost1.74_1.74.0.orig.tar.xz
 235973436bfc007a0f604962bbb28752d2578db0 367784 boost1.74_1.74.0-9.debian.tar.xz
Checksums-Sha256:
 2467be4af625b5ae4b3c93fc7af196a09eba39c11a7338cd9e8b356fa44d2f45 60536256 boost1.74_1.74.0.orig.tar.xz
 10ff1892cd8194f38316a63c2236a44cb7681ba17301cbc0a2fa6f482335c1f9 367784 boost1.74_1.74.0-9.debian.tar.xz
Files:
 f89e4e193730416fb06979ab1bd0e720 60536256 boost1.74_1.74.0.orig.tar.xz
 238a5e2463145406dc30f791a47e2a83 367784 boost1.74_1.74.0-9.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCgAdFiEEu71F6oGKuG/2fnKF0+Fzg8+n/wYFAmBMeWQACgkQ0+Fzg8+n
/wY0dA/+LLPAUrpBzNpsgqCIo+Ldn9aOF0KR+oJ0FoxFkn4FEO/dnsBeI0Bv01hi
uXOmT3nSCHoQ4G45YvtvO/dPmhlBz+RoD9Oly1FHqplVC9pjp4emXlXHOFbL0X2A
VCikTn/Tpg9hKcY4+GXeiKPKWSaBVG6bRzMLMH+TQQPQTsrv8ArkumfXKXDxoxLu
R+r8iZrIStiKvH85B5ryb1U4DjVM931x32zmw9B2B6WVweF5dB/HUrh1R3Zl8skW
fCiJTI/zx4EeqmHJe6zAVKz9wR5Uz5RBNQ9opj9nGvOV8xhwwesxee2tpAfl3y7O
qMzgCPW0ce45+lfYlC7LNTcQhGXUfyHZWXSbqTydoYiSmdxwAkuP7nv2l6uXhJuo
e6v7L71P+ICKyehOGZPiE/Fe8gJWp+FPr4SZ9ATj3Ro9Stj9RyxrTGKHIGd16MrY
LQ5wD7bC9Vpo20eUiz82Opbq5JQDS8LlmxIuavJMp1+fozU00qz3zK96Sjfp5uor
iS5Xb0KalKc8fd4dpQ13iCVRghZRJHMsehmnbkNp57jV79/8puctY5J1tpIPf3dr
ybbzvBiamhqC2lHzi7ZQiAsC3oiBXPyYewyli6R8FkEbgF8BqKJEBakVg+4yLgB/
DhrwaI4+nBA3RLvf7YXuxJLx8s0eEEvBhsdLXC15xHUNBVB4JSk=
=WwEs
-----END PGP SIGNATURE-----
