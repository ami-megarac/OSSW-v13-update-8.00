#!/bin/sh
#/conf/gen_openssl: Creating open ssl certificate
# Generate a set of SSL Keys
#openssl.cnf is must in /conf path
export RANDFILE="/conf/.rnd"
SSLCONFFILE=/conf/sslconf.ini
if [ -f "$SSLCONFFILE" ]; then
	#echo "$SSLCONFFILE exist"
	source $SSLCONFFILE
	#value is empty store default value
	country_name=${country_name:-"US"}
	state_name=${state_name:-"Georgia"}
    locality_name=${locality_name:-"Norcross"}
    organization_name=${organization_name:-"American Megatrends International LLC (AMI)"}
    organizational_unit_name=${organizational_unit_name:-"Service Processors"}
    common_name=${common_name:-"megarac.com"}
	email_address=${email_address:-"support@amidefault.com"}
	valid_days=${valid_days:-3650}
    csr_country_name=${csr_country_name:-"US"}
    csr_state_name=${csr_state_name:-"Georgia"}
    csr_locality_name=${csr_locality_name:-"Duluth"}
    csr_organization_name=${csr_organization_name:-"AMI"}
    csr_organizational_unit_name=${csr_organizational_unit_name:-"MEGARAC"}
    csr_common_name=${csr_common_name:-"ami.com"}
    csr_email_address=${csr_email_address:-"megarac@ami.com"}
else
	#echo "$SSLCONFFILE not exist"
	country_name="US"
	state_name="Georgia"
	locality_name="Norcross"
	organization_name="American Megatrends International LLC (AMI)"
	organizational_unit_name="Service Processors"
	common_name="megarac.com"
	email_address="support@ami.com"
	valid_days=3650
	csr_country_name="US"
       	csr_state_name="Georgia"
	csr_locality_name="Duluth"
	csr_organization_name="AMI"
	csr_organizational_unit_name="MEGARAC"
	csr_common_name="ami.com"
	csr_email_address="megarac@ami.com"
fi
ROOT_CA_FILE=/conf/server.pem
if [ -f "$ROOT_CA_FILE" ]; then
    #echo "$ROOT_CA_FILE exist"
    /usr/bin/openssl req -x509 -sha256 -new -nodes -config /conf/openssl.cnf -key $ROOT_CA_FILE -days $valid_days -out /conf/rootCACert.pem -subj /C="$country_name"/ST="$state_name"/L="$locality_name"/O="$organization_name"/OU="$organizational_unit_name"/CN="$common_name"/emailAddress="$email_address" >> /dev/null 2>&1
    if [ $? == 0 ]; then
        STATUS="Generating rootCACert.pem successfully"
    else
    echo "Generating rootCACert.pem failed"
    fi
else
    echo "$ROOT_CA_FILE does not exist"
fi
#Print the generated rootCACert.pem certificate
#openssl x509 -noout -text -in /conf/rootCACert.pem
if [ -f "$ROOT_CA_FILE" ]; then
    #echo "$ROOT_CA_FILE exist"
    /usr/bin/openssl req -new -key $ROOT_CA_FILE -out /conf/multiple_hostname.csr -config /conf/openssl.cnf  -subj /C="$csr_country_name"/ST="$csr_state_name"/L="$csr_locality_name"/O="$csr_organization_name"/OU="$csr_organizational_unit_name"/CN="$csr_common_name"/emailAddress="$csr_email_address" >> /dev/null 2>&1
    if [ $? == 0 ]; then
        STATUS="Creating multiple_hostname.csr certificate successfully"
    else
        echo "Creating multiple_hostname.csr certificate failed"
    fi
else
    echo "$ROOT_CA_FILE does not exist"
fi
if [ -f "/conf/multiple_hostname.csr" ]; then
     #echo "multiple_hostname.csr exist"
    /usr/bin/openssl x509 -req -in /conf/multiple_hostname.csr -CA /conf/rootCACert.pem -CAkey $ROOT_CA_FILE -CAcreateserial -out /conf/multiple_hostname_casigned.pem -days $valid_days -extfile /conf/openssl.cnf -extensions v3_req >> /dev/null 2>&1
    if [ $? == 0 ]; then
        STATUS="Server certificate and sign it with root ca successfully"
    else
        echo "Server certificate and sign it with root ca failed"
    fi
else
    echo "multiple_hostname.csr does not exist"
fi
# if [ -f "/conf/rootCACert.pem" ]; then
#     # echo "rootCACert.pem exist"
#     cp /conf/rootCACert.pem /conf/certs/privkey.pem
#     if [ $? == 0 ]; then
#         STATUS="rootCACert.pem copied as privkey.pem in /conf/cert path successfully"
#     else
#         echo "rootCACert.pem copied as privkey.pem in /conf/cert path FAILED"
#     fi
# else
#     echo "rootCACert.pem does not exist"
# fi
if [ -f "/conf/multiple_hostname_casigned.pem" ]; then
    #echo "multiple_hostname_casigned.pem exist"
    chmod 0600 /conf/multiple_hostname_casigned.pem
    mv /conf/multiple_hostname_casigned.pem  /conf/certs/server.pem
    cat /conf/certs/server.pem /conf/rootCACert.pem /conf/certs/privkey.pem > /conf/server.pem
    cp /conf/rootCACert.pem /conf/intermediate.pem
    chmod 0600 /conf/server.pem

    chmod 0600 /conf/rootCACert.pem
    chmod 0600 /etc/defconfig/server.pem

    if [ $? == 0 ]; then
        echo "SSL certificates generated successfully"
    else
        echo "multiple_hostname_casigned.pem copied as privkey.pem in /conf/cert path FAILED"
    fi
else
    echo "multiple_hostname_casigned.pem does not exist"
fi
