-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: gdb
Binary: gdb, gdb-multiarch, gdbserver, gdb-source, gdb-minimal
Architecture: any all
Version: 10.1-1.7
Maintainer: Héctor Orón Martínez <zumbi@debian.org>
Uploaders: Riku Voipio <riku.voipio@linaro.org>, Sergio Durigan Junior <sergiodj@debian.org>
Homepage: https://www.gnu.org/s/gdb/
Standards-Version: 4.5.1
Vcs-Browser: https://salsa.debian.org/gdb-team/gdb
Vcs-Git: https://salsa.debian.org/gdb-team/gdb.git
Build-Depends: debhelper (>= 10), lsb-release, xz-utils, autoconf, libtool, gettext, bison, dejagnu, flex, procps, gobjc, mig [hurd-any], libutil-freebsd-dev [kfreebsd-any], texinfo (>= 4.7-2.2), texlive-base <!nodoc>, libexpat1-dev, libncurses5-dev, libreadline-dev, zlib1g-dev, liblzma-dev, libbabeltrace-dev, libipt-dev [amd64 i386 x32], libsource-highlight-dev, libxxhash-dev, libmpfr-dev, pkg-config, python3-dev, libkvm-dev [kfreebsd-any], libunwind-dev [ia64], libdebuginfod-dev [linux-any]
Package-List:
 gdb deb devel optional arch=any
 gdb-minimal deb devel optional arch=any
 gdb-multiarch deb devel optional arch=any
 gdb-source deb devel optional arch=all
 gdbserver deb devel optional arch=amd64,armel,armhf,arm64,i386,ia64,m32r,m68k,mips,mipsel,mips64el,powerpc,powerpcspe,ppc64,ppc64el,riscv64,s390,s390x,x32
Checksums-Sha1:
 b517c0d09cb66a547b2a07eb61ced6c47789c37f 22045412 gdb_10.1.orig.tar.xz
 91620c68977b871de37a2bef23981222e248236f 195 gdb_10.1.orig.tar.xz.asc
 2225b023f600c0a38cc6964cb0f32ac0d955004f 285872 gdb_10.1-1.7.debian.tar.xz
Checksums-Sha256:
 d9108690ec1b818d3d4b21243a550083b8b82e36db07063bd89c4f1f73f5e1e7 22045412 gdb_10.1.orig.tar.xz
 141a58ec02dda554ddb9b22675b728d227f40b59069b15ceeee9bc3aaf8841e3 195 gdb_10.1.orig.tar.xz.asc
 c54a774c8a75a3908724ab648be07dba8174eda7b8cd3de263bd97154516d8dd 285872 gdb_10.1-1.7.debian.tar.xz
Files:
 6e67db8c8f80fac5fb251e211b22cbef 22045412 gdb_10.1.orig.tar.xz
 6a27ceb026deb9bd2f1550ebc3674971 195 gdb_10.1.orig.tar.xz.asc
 89cc77538322d6f5e2a8fb2f56b2e77d 285872 gdb_10.1-1.7.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQJEBAEBCAAuFiEE1WVxuIqLuvFAv2PWvX6qYHePpvUFAl/xpa4QHGRva29AZGVi
aWFuLm9yZwAKCRC9fqpgd4+m9a0DEACG9/3ZHSjbNas5wqbE65I9FHCYCt6W/5Yf
nqpU8pPC4Tv1Xr5zATsbEFlF5cC6hUkPUZ6zw9MRiPKiqbKhYbHfwz+QARua3f7g
PK7CH7ydSCCUkBKKoZvhUpad2oCvuInzGtkbYKJixlPbMEe57MD6A5vUBMIUIs0t
JTz/601Q6CgjsZ1LluSbxQ/w0CpYMqEK3/yyrKIfDsEdEEeL0Hby4vaAdJ7ipw1i
XwpfcMbanRWpOcVX+srJ9VrBwE0B/MMRouswVgrfzzIA01cibG5t4QJ8AIQCV0Bz
3tJShDaw4qUDzZ0SUbeYJ5QKVbTjuO3v0hcLJIrNxmWCHx5cD62iLld7Q7HRR/2T
cGSkPnJpY3/+E99jR6Ho2AXOKajpkWJIZYrX0Jpi/BXHfTJ/jbX8bLLlvloMVaHr
kunvNXnyIyLDgA6KjliQ+pw40nCx6De3kr8SAhD3FeVMDv6GVLkCy8tQiV1G9OCE
wN5uV0ViWqmIo1MEnrn7afO9mk6MyfKqBHA57LZKpaTJw6D5Ja3YTTv5RWsl/n9r
zocPXJ6cOe20/uc7tFR2BQllzM3JgYxy5A/sXaELH84EmnMydv0mkmMaRPKhTyO0
uTlyw8ErI2QFU/Zzmjr6oSeyUPIha9YHlIQiXbDFB3NwcZhMEOo6l/R7Px/4gng1
fGZL6bLrdA==
=6XwJ
-----END PGP SIGNATURE-----
