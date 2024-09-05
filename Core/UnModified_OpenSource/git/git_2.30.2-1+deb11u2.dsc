-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: git
Binary: git, git-man, git-doc, git-cvs, git-svn, git-mediawiki, git-email, git-daemon-run, git-daemon-sysvinit, git-gui, gitk, git-el, gitweb, git-all
Architecture: any all
Version: 1:2.30.2-1+deb11u2
Maintainer: Jonathan Nieder <jrnieder@gmail.com>
Uploaders: Anders Kaseorg <andersk@mit.edu>
Homepage: https://git-scm.com/
Standards-Version: 4.3.0.1
Vcs-Browser: https://repo.or.cz/w/git/debian.git/
Vcs-Git: https://repo.or.cz/r/git/debian.git/
Build-Depends: libz-dev, gettext, libpcre2-dev | libpcre3-dev, libcurl4-gnutls-dev, libexpat1-dev, subversion, libsvn-perl, libyaml-perl, tcl, python3, libhttp-date-perl | libtime-parsedate-perl, libcgi-pm-perl, liberror-perl, libmailtools-perl, cvs, cvsps, libdbd-sqlite3-perl, unzip, libio-pty-perl, debhelper-compat (= 10), dh-exec (>= 0.7), dh-apache2, dpkg-dev (>= 1.16.2~)
Build-Depends-Indep: asciidoc (>= 8.6.10), xmlto, docbook-xsl
Package-List:
 git deb vcs optional arch=any
 git-all deb vcs optional arch=all
 git-cvs deb vcs optional arch=all
 git-daemon-run deb vcs optional arch=all
 git-daemon-sysvinit deb vcs optional arch=all
 git-doc deb doc optional arch=all
 git-el deb vcs optional arch=all
 git-email deb vcs optional arch=all
 git-gui deb vcs optional arch=all
 git-man deb doc optional arch=all
 git-mediawiki deb vcs optional arch=all
 git-svn deb vcs optional arch=all
 gitk deb vcs optional arch=all
 gitweb deb vcs optional arch=all
Checksums-Sha1:
 76b3118428fe59dd95bf4fa918244a8396f32bea 6329820 git_2.30.2.orig.tar.xz
 edacf69b3db1153a083dde7c4f4e58d567a9290b 698392 git_2.30.2-1+deb11u2.debian.tar.xz
Checksums-Sha256:
 41f7d90c71f9476cd387673fcb10ce09ccbed67332436a4cc58d7af32c355faa 6329820 git_2.30.2.orig.tar.xz
 ca456459c18359a34008db6f68cd0f7bf470e3e4eb7093a982b67c51277a0f58 698392 git_2.30.2-1+deb11u2.debian.tar.xz
Files:
 53f3e1424598cd24eaf78588bcf90816 6329820 git_2.30.2.orig.tar.xz
 311b49f3834c1333bc55c3bc2742200e 698392 git_2.30.2-1+deb11u2.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQEzBAEBCAAdFiEEhhz+aYQl/Bp4OTA7O1LKKgqv2VQFAmP16EoACgkQO1LKKgqv
2VRG4Qf+IVP3/7LADEDbYrCRaf1latv5ObAvZVsZjD8GIq4NtLwl96/m8ie7nQYJ
/XurYIZorQ5oMVsc58XJMKGkt2GJ3MWmC9YFmTOYjmdH0iToecvOBxYFWzF/USFy
8Zc2fbVHtG/zOGC8sv4vxhX8tIqoJlzYVgruQrWiMDpj4GTsjmZNFx2wO4USBJk5
9Ls/fP/UwV5p4C1PLB26y/RFOknoclAb24bLBsKwm3OrnW73C1CNHcqszHEGNUDN
0pECZRFYi+odG76HYrnWCK3sPtcJ+GjvvsjPl4yCfqjD3UwNaGLqDZ2qiMlgHCXV
NxM3I/z00OkukmI/21DcMJxUHHVHCQ==
=g/5u
-----END PGP SIGNATURE-----
