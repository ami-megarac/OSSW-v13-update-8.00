#!/bin/sh
# /etc/init.d/gen_openssl_trigger.sh Triggering gen_openssl certificate generation file
#Runlevel : S = S29
#Runlevel : 3 = S29
# Check the file is available in conf then execute the script

FIPS_FILE=/usr/lib/arm-linux-gnueabihf/ossl-modules/fips.so
FILE=/etc/ssl/gen_openssl.sh
ROOT_CA_FILE=/conf/rootCACert.pem
CORE_FEATURES="/etc/core_features"


if [ -f "$FIPS_FILE" ]; then
    echo "------Running fipsinstall------"
    openssl fipsinstall -out /conf/fipsmodule.cnf -module /usr/lib/arm-linux-gnueabihf/ossl-modules/fips.so
    cp -f /usr/lib/arm-linux-gnueabihf/ossl-modules/openssl.cnf /conf/openssl.cnf
    cp -f /usr/lib/arm-linux-gnueabihf/ossl-modules/ssh_server_config /conf/ssh_server_config
fi

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


