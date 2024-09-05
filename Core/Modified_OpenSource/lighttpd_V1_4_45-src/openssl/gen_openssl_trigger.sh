#!/bin/sh
# /etc/init.d/gen_openssl_trigger.sh Triggering gen_openssl certificate generation file
#Runlevel : 3 = S29
# Check the file is available in conf then execute the script

FILE=/etc/ssl/gen_openssl.sh
ROOT_CA_FILE=/conf/rootCACert.pem
CORE_FEATURES="/etc/core_features"
if [ -f "$FILE" ]; then
    STATUS="/etc/ssl/gen_openssl.sh exist"
    if [ -f "$ROOT_CA_FILE" ]; then
	STATUS="ROOT_CA_FILE exist"
    else
	STATUS="ROOT_CA_FILE not exist, generating key file starts"
#	cd /conf
	bash /etc/ssl/gen_openssl.sh
    fi
else 
    echo "$FILE does not exist"
fi

isfeatureenabled()
{
        feature="$1"

        if [ -f $CORE_FEATURES ]
        then
            line=`cat $CORE_FEATURES | grep -w $feature`
            if [ "$line" == "" ]; then
                    return 0
            fi
            return 1
        fi

        return 0
}

isfeatureenabled CONFIG_SPX_FEATURE_REDFISH_ENABLED
        if [ $? == 1 ]; then
                bash /etc/ssl/dynamic_certificate.sh
        fi
