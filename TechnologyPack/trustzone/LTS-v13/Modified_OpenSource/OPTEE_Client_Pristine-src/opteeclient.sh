#!/bin/sh
# /etc/init.d/opteeclient: initiate optee client process
#
# chkconfig: 2345 10 90
#
#Runlevel : S = S21
#Runlevel : 4 = K49
#Runlevel : 6 = K49
#Runlevel : 7 = K49
#Runlevel : 8 = K49
# Restart the service on warm reboot
#Runlevel : 9 = K49
#Runlevel : 9 = S21

PATH=/bin:/usr/bin:/sbin:/usr/sbin
OPTEECLIENTDAEMON_NAME="/usr/sbin/tee-supplicant"
PIDFILE="/var/run/tee-supplicant.pid"

ACTION=$1

#Load the sepcificied driver if not loaded already
load_drivers()
{
	lsmod | grep $1 >/dev/null
	if [ $? == 1 ]
        then
  		echo -n "Loading " $1  " driver... "
	        modprobe $1
	       	echo "Done"
	fi
}

load_optee_drivers(){

	load_drivers "aspeed-swintr"
	load_drivers "tee"
	load_drivers "optee"
}

# Options for start/stop the optee client daemons
case "$ACTION" in
  start)
    load_optee_drivers	   
    echo -n "Starting optee client : tee-supplicant"
    if [ -x /usr//sbin/tee-supplicant ]; then
    	$OPTEECLIENTDAEMON_NAME -d
    fi
    echo "."
    echo -n
    ;;
  stop)
    echo -n "Stopping optee client : tee-supplicant"
    killall -15 tee-supplicant
    echo "."
    echo -n
    ;;
  *)
    echo "Usage: /etc/init.d/opteeclient {start|stop}"
    echo "."
    echo -n
    exit 1
esac
  
exit 0

