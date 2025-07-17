STEPS TO BUILD Wide_DHCPV6:

1)Extract the wide-dhcpv6 client tar file.

2)Change the directory to source directory(wide-dhcpv6-20080615) and apply the patch using the below command.

				 patch -p1 < <Patch files in order>
				
3)Run the ./configure script file to generate the Makefile.

4)After generating the makefile, it needs modification to compile with corss compiler. Please refer the Makefile.sample in this package.

    a) cp Makefile.sample which name Makefile into your src and replace sysroot and libcpath inside as following:
           Workspace = <Workspace>     #Specify the target path of spx fully Build workspace
           example:
               Workspace = /home/user/ast2500evb/ws
  
    b) Check for the sysroot and libcpath are in correct path:
            SYSROOT = <WORKSPACE>/tools/arm-soft-linux-gnueabi/arm-soft-linux-gnueabi/sysroot
            LIBCPATH= <WORKSPACE>/Build/target/usr/lib/arm-linux-gnueabi

    c) Change crose compile:
            CROSS_COMPILE=arm-linux-gnueabi-
		
5)Give Softlinks to the cross compile library in the build machine:
        ln -s <WORKSPACE>/Build/target/lib/arm-linux-gnueabi /lib/arm-linux-gnueabi
        ln -s <WORKSPACE>/Build/target/usr/lib/arm-linux-gnueabi/ /usr/lib/arm-linux-gnueabi

6)Now proceed to make.
         make dhcp6c
