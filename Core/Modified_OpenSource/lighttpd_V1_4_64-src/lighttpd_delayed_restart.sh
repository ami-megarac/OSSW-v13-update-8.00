#!/bin/bash

# shell script to restart lighttpd after a delay 

echo "Restarting lighttpd in $1 seconds..."
sleep $1
echo "Restarting lighttpd NOW!"
/etc/init.d/lighttpd.sh restart
