#!/bin/sh
# /etc/init.d/crashdump: start crashdump monitoring service
#
#Runlevel : S = S98z
#Runlevel : 6 = K98z
#Runlevel : 7 = K98z
#Runlevel : 8 = K98z
# Restart the service on warm reboot
#Runlevel : 9 = K98z
#Runlevel : 9 = S98z

file_name="/conf/crashdump/crashdumpinfo"
isCrashDumpEnabled()
{
	if [ -f $file_name ]; then

		target=$(grep "Status" $file_name)
		status=$(echo $target| cut -d'=' -f 2)
		if [ $status = "Enabled" ]; then
			return 1
		else	
	  		return 0
		fi
	fi
}

dump_dir_file_name="/etc/crashdump/crashdumppath"
if [ -f $dump_dir_file_name ]; then
	target=$(grep "crashdump_dir" $dump_dir_file_name)
	crashdump_dir=$(echo $target | awk -F= '{print $2}')
	if [ -z $crashdump_dir ] || [ $crashdump_dir = "" ]; then
		crashdump_dir="/home/crashdump"
	fi
fi

feature_file_name="/etc/core_features"
isFeatureEnabled()
{
	if [ -f $feature_file_name ]; then
		feature=$(grep "$1" $feature_file_name)
		if [ "$feature" == "$1" ]; then
			return 1
		else
			return 0
		fi
	fi
}

extract_existing_json()
{
	#extract compressed JSON files at boot time
	if [ ! -d "/var/crashdump/json" ]; then
		mkdir -p /var/crashdump/json
	fi
	
	if [ -f $crashdump_dir/sysdebug1.gz ]; then
		tar -xzvf $crashdump_dir/sysdebug1.gz -C /var/crashdump/json/
	fi
	
	if [ -f $crashdump_dir/sysdebug2.gz ]; then
		tar -xzvf $crashdump_dir/sysdebug2.gz  -C /var/crashdump/json/
	fi

	if [ -f $crashdump_dir/bafi_decoded.1.gz ]; then
		tar -xzvf $crashdump_dir/bafi_decoded.1.gz -C /var/crashdump/json/
	fi

	if [ -f $crashdump_dir/bafi_decoded.2.gz ]; then
		tar -xzvf $crashdump_dir/bafi_decoded.2.gz  -C /var/crashdump/json/
	fi

}

start_application()
{
	#start the crashdump monitoring application
	if [ -e /usr/local/bin/Crashdump ]
	then
		isCrashDumpEnabled
		enablestatus=$?
		crashdumprunning=`ps x | grep Crash | grep -v grep | wc -l`
		#SERVICE DISABLED
		if [ $enablestatus != 1 ]; then 
			#CRASHDUMP RUNNING
			if [ $crashdumprunning == 1 ]; then
				killall -9 Crashdump
				echo -e "Crashdump monitoring stopped! "
			fi
		#SERVICE ENABLED
		else
			#CRASHDUMP NOT RUNNING
			if [ $crashdumprunning != 1 ]; then
				echo -e "Crashdump monitoring started! "
				/usr/local/bin/Crashdump &
			#CRASHDUMP ALREADY RUNNING
			else	
				echo -e "Crashdump monitoring already running! "
			fi
		fi
	fi
}

decode_existing_json()
{
	isFeatureEnabled CONFIG_SPX_FEATURE_GLOBAL_AUTONOMOUS_CRASH_DUMP_DECODE_SUPPORT
	featureenabled=$?
	if [ $featureenabled == 1 ]; then
		#extract the python library and cscripts
		/etc/init.d/extractpython27.sh
	
		#decode the existing JSON files
		/etc/init.d/crashdump_decode.sh &
	fi
}

stop_application()
{
    killall -9 Crashdump
}

case "$1" in
    start)
        echo -e "Starting Crashdump monitoring application"
        extract_existing_json
        start_application
        decode_existing_json
        echo "."
        ;;
    stop)
        echo -e "Stopping Crashdump monitoring application"
        stop_application
        echo "."
        ;;
    *)
        echo "Usage: /etc/init.d/crashdump.sh {start|stop}"
        exit 1
esac

exit 0
