Steps to Apply patch and Build Net-snmp binary

1) Unzip the net-snmp-5.8.zip file using bellow command

   unzip net-snmp-5.8.zip

   extracted file sample : net-snmp-5.8

2) use the following commands and apply the patches

   patch net-snmp-5.8/snmplib/snmpusm.c snmpusm.patch
   
   patch net-snmp-5.8/snmplib/snmp_api.c snmp_api.patch
   
   patch net-snmp-5.8/agent/snmp_agent.c snmp_agent.patch

   patch -p0 < 001-Fixed_CVE-2019-20892.patch

   patch -p0 < 002-Fixed_CVE-2020-15861.patch
   
   patch -p0 < 003-Fixed_CVE-2020-15862.patch
   
   patch -p0 < 004-encrypted_password_snmpusm_c.patch

   patch -p0 < 005-encrypted_password_snmpusm_h.patch
   
3) Use the following command to configure

./configure --build=arm-linux-gnueabi --prefix=/usr --includedir=${prefix}/include --mandir=${prefix}/share/man --infodir=${prefix}/share/info --sysconfdir=/etc --localstatedir=/var --libdir=${prefix}/lib/arm-linux-gnueabi --libexecdir=${prefix}/lib/arm-linux-gnueabi --disable-maintainer-mode --disable-dependency-tracking --prefix=/usr --sysconfdir=/etc --mandir=/usr/share/man --with-persistent-directory=/var/lib/snmp --enable-shared --enable-ipv6 --with-logfile=none --without-rpm --with-libwrap --with-openssl --without-dmalloc --without-efence --without-rsaref --with-sys-contact=root --with-sys-location=Unknown --with-out-mib-modules= --enable-mfd-rewrites --with-mnttab=/etc/mtab --with-mibdirs=$HOME/.snmp/mibs:/usr/share/snmp/mibs:/usr/share/snmp/mibs/iana:/usr/share/snmp/mibs/ietf:/usr/share/mibs/site:/usr/share/snmp/mibs:/usr/share/mibs/iana:/usr/share/mibs/ietf:/usr/share/mibs/netsnmp --with-mysql --with-defaults

4) Run "Make all" command to compile and build netsnmp binaries

5) Strip the required libraries and binaries using following command

   strip --strip-unneeded <lib/bin name>
