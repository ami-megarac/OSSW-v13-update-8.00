-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: git
Binary: git, git-man, git-doc, git-cvs, git-svn, git-mediawiki, git-email, git-daemon-run, git-daemon-sysvinit, git-gui, gitk, git-el, gitweb, git-all
Architecture: any all
Version: 1:2.30.2-1+deb11u3
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
 5ca907e08b960d5a0171d29670afe4814bcee975 711960 git_2.30.2-1+deb11u3.debian.tar.xz
Checksums-Sha256:
 41f7d90c71f9476cd387673fcb10ce09ccbed67332436a4cc58d7af32c355faa 6329820 git_2.30.2.orig.tar.xz
 0cfa77116c7174aa5c7cf3ee243132e7d899e5129c791c12b5298e085f3cbe52 711960 git_2.30.2-1+deb11u3.debian.tar.xz
Files:
 53f3e1424598cd24eaf78588bcf90816 6329820 git_2.30.2.orig.tar.xz
 133cda1eeff3534440690c1fa41262db 711960 git_2.30.2-1+deb11u3.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQJNBAEBCgA3FiEEm5FwB64DDjbk/CSLaVt65L8GYkAFAmbW2NMZHHNwd2hpdHRv
bkBzcHdoaXR0b24ubmFtZQAKCRBpW3rkvwZiQE5XEACAu53ucd6VJIHpJeDB0oIu
7zQJMXjYchbu0rEamZXIU6ET+M2Y5xGr1oT7RpWlWjuWWgoHGowgKiYI8XvIbi4e
wZ4tBih7JA6n5uF4vQjyXyWtaR1dSyPrR+82j7M/6AqWLm5N9s+CoBimXD14Op/0
rTgmCAXnCx9LDAVJq6k/Ioh2BXZHg9Lj8P8DQ2h5U0YXMLVtFKEExchJPc9ewUL6
D1qxwnTGCIWaX3JGDYIbb4mldlxBWsRPgoa7B9CBzLRZsY0g91CrOHonf5ALF2eQ
3jnBpXVN2DY/zsjxF4Sur495EO06CX3K0D0qVTGCGQ25alcSNoIguVSvWxGowLzr
K3emVmK5OQGXgxyJZlmyeysiMyO3ZJOYuzM/8tH5c6TWy3Iev6+kar4+kLzPJ5ad
p9jQPfaNtAuEFpjxJqBrfs9S3+Yj0+o6FrGL966GStb5rFKYZNE0zJlaktZzNzTk
5wHZshFbeLxFgRk7GyQzTiH+E0D2erybOgxBZHZBT5eP2aBtGbXPABdHy2yz8FTn
4OYMN8m9/SS5W0gZf5Kr/NgXkMZy4mb6IDQv3iPGW9CZH+t9is5gJv50Jk8OzJgn
Y/CezAXcICWiH3kVWkkxJxnHg3qLrg/aXEALvZ/Tx2h2AxLWCw2MfSxSGBJb/vbD
be4Jq1xGcOOJLXPbEywKwA==
=6RSK
-----END PGP SIGNATURE-----
