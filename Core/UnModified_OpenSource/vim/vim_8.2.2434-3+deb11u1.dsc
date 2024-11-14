-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: vim
Binary: vim-common, vim-gui-common, vim-runtime, vim-doc, vim-tiny, vim, vim-gtk, vim-gtk3, vim-nox, vim-athena, xxd
Architecture: any all
Version: 2:8.2.2434-3+deb11u1
Maintainer: Debian Vim Maintainers <team+vim@tracker.debian.org>
Uploaders:  James McCoy <jamessan@debian.org>,
Homepage: https://www.vim.org/
Standards-Version: 4.5.0
Vcs-Browser: https://salsa.debian.org/vim-team/vim
Vcs-Git: https://salsa.debian.org/vim-team/vim.git
Testsuite: autopkgtest
Testsuite-Triggers: cscope, procps
Build-Depends: autoconf, cscope <!nocheck>, debhelper-compat (= 12), libacl1-dev, libcanberra-dev, libgpmg1-dev [linux-any], libgtk-3-dev, liblua5.2-dev, libperl-dev, libselinux1-dev [linux-any], libncurses-dev, libtool-bin <!nocheck>, libxaw7-dev, libxpm-dev, libxt-dev, lua5.2, procps <!nocheck>, python3-dev, ruby [!alpha !ia64] <!pkg.vim.noruby>, ruby-dev [!alpha !ia64] <!pkg.vim.noruby>, tcl-dev
Build-Depends-Indep: docbook-utils, docbook-xml, ghostscript, pdf2svg
Build-Conflicts: autoconf2.13
Package-List:
 vim deb editors optional arch=any
 vim-athena deb editors optional arch=any
 vim-common deb editors important arch=all
 vim-doc deb doc optional arch=all
 vim-gtk deb oldlibs optional arch=all
 vim-gtk3 deb editors optional arch=any
 vim-gui-common deb editors optional arch=all
 vim-nox deb editors optional arch=any
 vim-runtime deb editors optional arch=all
 vim-tiny deb editors important arch=any
 xxd deb editors optional arch=any
Checksums-Sha1:
 06c86291e6ce501b7eb197f7b5a5ab6124a174dc 15412030 vim_8.2.2434.orig.tar.gz
 6b2a6af6da4606979765cba51f934558e56bb154 200476 vim_8.2.2434-3+deb11u1.debian.tar.xz
Checksums-Sha256:
 dec2f91bb4f877b2bfdf283ccc3d94f99f436a225bd8a9b5e3d422f95c56d702 15412030 vim_8.2.2434.orig.tar.gz
 d2dae7246169762d88942a5de7e58bb109ec5af293eafdda271c36f91cc293a4 200476 vim_8.2.2434-3+deb11u1.debian.tar.xz
Files:
 502e2de3d0b5049a1c2a5a519dd2c842 15412030 vim_8.2.2434.orig.tar.gz
 ab0f59aae9b0bbefc8a4bcdfa5fe8e9a 200476 vim_8.2.2434-3+deb11u1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQKTBAEBCgB9FiEEkb+/TWlWvV33ty0j3+aRrjMbo9sFAmGqWCRfFIAAAAAALgAo
aXNzdWVyLWZwckBub3RhdGlvbnMub3BlbnBncC5maWZ0aGhvcnNlbWFuLm5ldDkx
QkZCRjRENjk1NkJENURGN0I3MkQyM0RGRTY5MUFFMzMxQkEzREIACgkQ3+aRrjMb
o9uJfA/+NVzgFmSAZb6mCriJD082xpzBx0PyqpmBMr/7AM0xSQpVMIOGI3pzlcYO
RpdffEELpwo0jOl6tYWJIwubasQdca57uNC3L/ZEyL+lgs8dfhJ8prT6XOuqEv/6
UXIJ+gQo32gQjdNyw+SClcdoFtAXXjvGq3YMjkktiT/EAc15R+DZ6hmESe/3QJU3
6HniuTVuukEUMBOPCoHC01l/j0YsoIO3DXuQmlYHmkz0XAkYZwAsxIzoTG2sATZN
n39VqSt9mXIAsRp9juky3vJ/aSJW//yQ6WQQP1gxL9aG35AtF1WcPErNHyaRg9zo
FGEEoQ7J60jFxiItJ+ASVcJRCeEAdh20RYZ5dtPGSE05zsfje4r470TGAJwGmbAM
c/k+LbaoPVolgRG8uM1DSTSLXRD3lTMDy83WE1dKZ+R37yiAr2K2eHG9O3k931O7
MZWBFvHIWj+xj5Xl7n+eLgVGIn9Ag1VLM6JBANvi+1XNzZtdMYxo0NFJGgJ063tF
qA98EAFoG1FmbSef1aBGWGaTuBJ9KPxHiXWCnacf5FPle3DyeVLtSPHBwk/lkTPs
h145TdEM1jEKYO0oTdvN36yZdCskyMEkzBK+xlhX9KwLGnM159Oe/cbRwF/O1qZx
MxIWHg3RXVGjJKSgpEBxwx11qW5DQK5Ihg2ibdcIbYJuDQYElz8=
=Q0G3
-----END PGP SIGNATURE-----
