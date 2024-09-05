#!/bin/sh
#This script will rename the JSON files and compress the files into /conf

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

dump_dir_file_name="/etc/crashdump/crashdumppath"
if [ -f $dump_dir_file_name ]; then
	target=$(grep "crashdump_dir" $dump_dir_file_name)
	crashdump_dir=$(echo $target | awk -F= '{print $2}')
	if [ -z $crashdump_dir ] || [ $crashdump_dir = "" ]; then
		crashdump_dir="/home/crashdump"
	fi
fi

#rename and compress the JSON files, only if a new debug file is available
if [ -f "/var/crashdump/json/crashdump.json" ]; then
	#crashdump.json to sysdebug1.json
	#sysdebug1.json to sysdebug2.json
	if [ -f "/var/crashdump/json/sysdebug2.json" ]; then
		rm /var/crashdump/json/sysdebug2.json
	fi
	
	if [ -f "/var/crashdump/json/sysdebug1.json" ]; then
		mv /var/crashdump/json/sysdebug1.json /var/crashdump/json/sysdebug2.json
	fi

	mv /var/crashdump/json/crashdump.json /var/crashdump/json/sysdebug1.json
	chmod 600 /var/crashdump/json/sysdebug1.json

	#compress the crashdump JSON files and put it in crashdump directory
	if [ -f "/var/crashdump/json/sysdebug1.json" ]; then
		tar -czvf $crashdump_dir/sysdebug1.gz -C /var/crashdump/json sysdebug1.json
	fi
	if [ -f "/var/crashdump/json/sysdebug2.json" ]; then
		tar -czvf $crashdump_dir/sysdebug2.gz -C /var/crashdump/json sysdebug2.json
	fi
fi

#rename and compress the JSON files for BAFI, only if a new debug file is available
if [ -f "/var/crashdump/json/BAFI.json" ]; then
	#BAFI.json to bafi_decoded1.json
        #bafi_decoded1.json to bafi_decoded2.json
    if [ -f "/var/crashdump/json/bafi_decoded2.json" ]; then
		rm /var/crashdump/json/bafi_decoded2.json
    fi

	if [ -f "/var/crashdump/json/bafi_decoded1.json" ]; then
        mv /var/crashdump/json/bafi_decoded1.json /var/crashdump/json/bafi_decoded2.json
    fi

    mv /var/crashdump/json/BAFI.json /var/crashdump/json/bafi_decoded1.json
    chmod 600 /var/crashdump/json/bafi_decoded1.json
    
	#compress the BAFI JSON files and put it in crashdump directory
	if [ -f "/var/crashdump/json/bafi_decoded1.json" ]; then
		tar -czvf $crashdump_dir/bafi_decoded.1.gz -C /var/crashdump/json bafi_decoded1.json
	fi
        if [ -f "/var/crashdump/json/bafi_decoded2.json" ]; then
		tar -czvf $crashdump_dir/bafi_decoded.2.gz -C /var/crashdump/json bafi_decoded2.json
	fi
fi


exit 0
