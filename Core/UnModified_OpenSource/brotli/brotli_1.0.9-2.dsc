-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: brotli
Binary: python3-brotli, brotli, libbrotli-dev, libbrotli1
Architecture: any
Version: 1.0.9-2
Maintainer: Tomasz Buchert <tomasz@debian.org>
Uploaders: Ondřej Surý <ondrej@debian.org>
Homepage: https://github.com/google/brotli
Standards-Version: 4.5.0
Vcs-Browser: https://salsa.debian.org/debian/brotli
Vcs-Git: https://salsa.debian.org/debian/brotli.git
Testsuite: autopkgtest
Testsuite-Triggers: python3-all
Build-Depends: cmake, debhelper (>= 13), debhelper-compat (= 13), dh-python <!nopython>, libpython3-all-dev <!nopython>, python3-all-dev:any <!nopython>, python3-setuptools <!nopython>, python3:any <!nopython>
Package-List:
 brotli deb utils optional arch=any
 libbrotli-dev deb libdevel optional arch=any
 libbrotli1 deb libs optional arch=any
 python3-brotli deb python optional arch=any profile=!nopython
Checksums-Sha1:
 ddfefdf2593b3f03eec221a7f4ceaa710e5a2e6b 486984 brotli_1.0.9.orig.tar.gz
 892f0dfdc63ec3a3717fab41bf854e9f2de21f10 5552 brotli_1.0.9-2.debian.tar.xz
Checksums-Sha256:
 f9e8d81d0405ba66d181529af42a3354f838c939095ff99930da6aa9cdf6fe46 486984 brotli_1.0.9.orig.tar.gz
 ab81b1db852c8d01e0fa5b0b650bb486f32a232b35336828423af50af6fecca0 5552 brotli_1.0.9-2.debian.tar.xz
Files:
 c2274f0c7af8470ad514637c35bcee7d 486984 brotli_1.0.9.orig.tar.gz
 4ded8882473ee36703b794bdd08ef5a2 5552 brotli_1.0.9-2.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCAAdFiEEkpeKbhleSSGCX3/w808JdE6fXdkFAl9XP+8ACgkQ808JdE6f
XdnMvQ/9E7KmXHnVxCct4pwauLQnav63xvcF1ZOZXPIph2DnGa2wmn25x8nnesnI
At3F0/KRE67KJVGmp31NmkJQo49/PQBrLAQc3ttDHZ39Os/eYbzf6Rrld6sOklQf
qOvVhg9PoMuC0RyaQ7rxtHcWVFdOyjw40IX0J/nPzPs18lUJ6r5DK3tYyK/0EsXE
r24KldIPcgzvg4x5WVRb4xnz7sp+1i3+6/iKbXpsbPI3cJdPkC1hrWPJghcdFBMA
Sf62LQjGe1mYGzzSL8QoVKNTkOlHKmtyYhid6LFwxOq9/JIPz4O3Hy+5qPr1+b8d
N47xMGdHnu27o3djBhTIQBiaeHIosv/Bs9lmdzfBLRgjpXLg7K7eZX6COnCd6B3e
GDlSIL3dxN7qifBNMOyiJEWotWo2g1Hau3ulAttn7morBwK4k98MBg4WZ1z1fe9x
9a5coPPxQNTaSf1Tm4ii6b0h34lI9s0yNJDBZmmHDvmT1XiZfAaw+ydqZ0T67Kk6
E9qJjWmPfYb/RKmUkfLYJlJZAhfcT9Hc+ZTKU5i9CG8UiCB3a/vdv4tTUoNvuVvr
2zrTfkUPGji8NZZuzJaDzV3h1H7J3u0BEI6cybWu9PqWAof8XAl/WeAdvKcNGoTe
0x74mh+1fhiEyfA5g11cSfAscTsuGlDKY6bFCIwr8A6yBz+x/ac=
=Jfjv
-----END PGP SIGNATURE-----
