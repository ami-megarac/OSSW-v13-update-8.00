#Runlevel : 3 = S90
#Runlevel : 6 = K90
#Runlevel : 7 = K90
# Restart the service on warm reboot
#Runlevel : 9 = K90
#Runlevel : 9 = S90

PATH=/bin:/usr/bin:/sbin:/usr/sbin

BKUPCONF_DIR="bkupconf"
check_bkupconf() {
      # Check if there is bkupconf in fstab
      VAL=`df | grep "/$BKUPCONF_DIR" | awk '{ printf $6 }'`
      if [ "$VAL" == "" ]; then
          return 0
      else
          #Found bkupconf
          return 1
      fi
  }
lighttpd_file="/conf/lighttpd.conf"
if  test -f "$lighttpd_file" 
then 
    echo "Lighttpd configurations exists"
else 
    echo "Copying Lighttpd configurations"
    cp /etc/defconfig/lighttpd.conf /conf/
    check_bkupconf
    if [ $? == 1 ]; then
      #If there is bkupconf in fstab, copy lighttpd.conf to /bkupconf/
      cp /etc/defconfig/lighttpd.conf /bkupconf/
    fi
fi

lighttpd_start() {
if [ -x /usr/local/sbin/lighttpd ]; then
    if ! `test -e /var/run/lighttpd.pid`; then
        /usr/local/sbin/lighttpd -f /conf/lighttpd.conf -m /usr/local/lib
        echo "Starting lighttpd"
    else
        /bin/ps ax | grep /usr/local/sbin/lighttpd | grep -v grep > /dev/null
        if [ $? == 1 ];then
           rm -f /var/run/lighttpd.pid
           /usr/local/sbin/lighttpd -f /conf/lighttpd.conf -m /usr/local/lib
        else
            echo "Lighttpd is already running!"
        fi
    fi
fi
}

lighttpd_stop() {
echo "Stopping lighttpd"
killall -15 lighttpd
}

lighttpd_restart() {
lighttpd_stop
sleep 3
lighttpd_start
}
test -f /var/tmp/licstat/lighttpd_nolicense && exit 0
case "$1" in
'start')
lighttpd_start
;;
'stop')
lighttpd_stop
;;
'restart')
lighttpd_restart
;;
*)
echo "usage $0 start|stop|restart"
;;
esac 
