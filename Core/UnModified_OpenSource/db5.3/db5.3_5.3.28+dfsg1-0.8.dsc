-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: db5.3
Binary: db5.3-doc, libdb5.3-dev, libdb5.3, db5.3-util, db5.3-sql-util, libdb5.3++, libdb5.3++-dev, libdb5.3-tcl, libdb5.3-dbg, libdb5.3-java-jni, libdb5.3-java, libdb5.3-java-dev, libdb5.3-sql-dev, libdb5.3-sql, libdb5.3-stl-dev, libdb5.3-stl
Architecture: any all
Version: 5.3.28+dfsg1-0.8
Maintainer: Debian Berkeley DB Team <team+bdb@tracker.debian.org>
Uploaders: Ondřej Surý <ondrej@debian.org>
Homepage: http://www.oracle.com/technetwork/database/database-technologies/berkeleydb/overview/index.html
Standards-Version: 3.9.6
Vcs-Browser: https://salsa.debian.org/debian/db5.3
Vcs-Git: https://salsa.debian.org/debian/db5.3.git
Build-Depends: debhelper (>= 10), autotools-dev, dh-autoreconf, tcl <cross !pkg.db5.3.notcl>, tcl-dev <!pkg.db5.3.notcl>, procps [!hurd-i386] <!nocheck>, javahelper <!nojava>, default-jdk <!nojava>
Package-List:
 db5.3-doc deb doc optional arch=all
 db5.3-sql-util deb database extra arch=any
 db5.3-util deb database optional arch=any
 libdb5.3 deb libs standard arch=any
 libdb5.3++ deb libs optional arch=any
 libdb5.3++-dev deb libdevel extra arch=any
 libdb5.3-dbg deb debug extra arch=any
 libdb5.3-dev deb libdevel extra arch=any
 libdb5.3-java deb java optional arch=all profile=!nojava
 libdb5.3-java-dev deb libdevel optional arch=any profile=!nojava
 libdb5.3-java-jni deb java optional arch=any profile=!nojava
 libdb5.3-sql deb libs extra arch=any profile=!pkg.db5.3.nosql
 libdb5.3-sql-dev deb libdevel extra arch=any
 libdb5.3-stl deb libs extra arch=any
 libdb5.3-stl-dev deb libdevel extra arch=any
 libdb5.3-tcl deb interpreters extra arch=any profile=!pkg.db5.3.notcl
Checksums-Sha1:
 98d30e5ba942cc4a818ac29270ac72a3ffc2c374 19723860 db5.3_5.3.28+dfsg1.orig.tar.xz
 0b3a6ae6161c1b40470c9e1425edfe25a778c5d3 30748 db5.3_5.3.28+dfsg1-0.8.debian.tar.xz
Checksums-Sha256:
 b19bf3dd8ce74b95a7b215be9a7c8489e8e8f18da60d64d6340a06e75f497749 19723860 db5.3_5.3.28+dfsg1.orig.tar.xz
 073c0c87283bf5e606f3ce6d1814315b40b9685c943601ae3fd81e2da4e612d4 30748 db5.3_5.3.28+dfsg1-0.8.debian.tar.xz
Files:
 1dd7f0f45b985b661dc3c6edbd646d80 19723860 db5.3_5.3.28+dfsg1.orig.tar.xz
 627b4582a70909265c7efcebfd0874b0 30748 db5.3_5.3.28+dfsg1-0.8.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCgAdFiEEc7KZy9TurdzAF+h6W//cwljmlDMFAmAdenEACgkQW//cwljm
lDN4Qg/9E0/F2meTYulBmZtXullhaWtoyj3rmnXKzUswYjetgKBrhAG1jZX/0exX
yAvYyeNtiXcgzIVJwPI9nysF4w/9SACfuDGzv541o6haFMkXhkdkVZdfV96Yk7XU
fTnFI08fosy129ZOlXAb0/me7GG3GKwGrXPvyfqkP2U9Ind6PaBacENi9MhnuGQd
NqHqfoLvo3SqO7WnPfa4l56+OrO2OMfLr9JK1LxF+KTPHyajASCv+MUDstibD+md
RTGzd5lDYrcpSGGSIx6Bw6OeXnoSAYxwCvlDHUvKfvAR999/mNzVll4sC6/mVVD4
w0ykWksAwluDDtLLhdrNSgIkE2eWuyhXQ9rRHGaLKTsrNwnqVy2xSiMW9RnjV7L9
w04iekiV588Qe3VQ6kYeKgBSKEKAv9/X99D08jUv2wiHxvsPsersVLdfMA/E2eCw
25kA14qrPiyzzDLXPWMUwH0p7Prgrnuc8E0YXygoEG8uqjhGB7RzRKF/DKyJNSrM
mzsFnI3u8VTiVSmoRAC5lRz7VFIJaXjWaO8hHN9iEm2bBGFn70AVfrR8yeOtl3QZ
4mWEkGJGdw8uxSRXWvoMdJLXb8LXBDNsGgQsonTpDNFJxQBw60pHmdyyWbHHe3b1
Q8dly8Oqe3g4xSzNnp8cywfud0tFhWrO1+e/FZAAFcaKFKmJe1Y=
=CTLi
-----END PGP SIGNATURE-----
