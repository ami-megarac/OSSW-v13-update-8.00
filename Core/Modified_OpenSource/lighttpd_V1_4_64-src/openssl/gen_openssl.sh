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

DEFAULT_KEY_FILE="/conf/certs/privkey.pem"
DEFAULT_CERT_FILE="/conf/certs/server.pem"
DEFAULT_DHPARAM_FILE="/conf/certs/dhparam.pem"
CONF_ROOTCACERT_FILE="/conf/rootCACert.pem"
OPENSSL_CONF_FILE="/conf/openssl.cnf"
MULTI_HOSTNAME_CSR="/conf/multiple_hostname.csr"
MULTI_HOSTNAME_CASIGNED_FILE="/conf/multiple_hostname_casigned.pem"
CONF_COMBINED_SSL_FILE="/conf/server.pem"
CONF_INTERMEDIATE_FILE="/conf/intermediate.pem"

/usr/bin/openssl genrsa -out $DEFAULT_KEY_FILE 2048
if [ $? == 0 ]; then
    STATUS="Generating privkey.pem successfully"
else
    echo "Generating privkey.pem failed"
fi
if [ -f "$DEFAULT_KEY_FILE" ]; then
    #echo "$DEFAULT_KEY_FILE exist"
    /usr/bin/openssl req -x509 -sha256 -new -nodes -config $OPENSSL_CONF_FILE -key $DEFAULT_KEY_FILE -days $valid_days -out $CONF_ROOTCACERT_FILE -subj /C="$country_name"/ST="$state_name"/L="$locality_name"/O="$organization_name"/OU="$organizational_unit_name"/CN="$common_name"/emailAddress="$email_address" >> /dev/null 2>&1
    if [ $? == 0 ]; then
        STATUS="Generating rootCACert.pem successfully"
    else
    echo "Generating rootCACert.pem failed"
    fi
else
    echo "$DEFAULT_KEY_FILE does not exist"
fi
#Print the generated rootCACert.pem certificate
#openssl x509 -noout -text -in /conf/rootCACert.pem
if [ -f "$DEFAULT_KEY_FILE" ]; then
    #echo "$DEFAULT_KEY_FILE exist"
    /usr/bin/openssl req -new -key $DEFAULT_KEY_FILE -out $MULTI_HOSTNAME_CSR -config $OPENSSL_CONF_FILE  -subj /C="$csr_country_name"/ST="$csr_state_name"/L="$csr_locality_name"/O="$csr_organization_name"/OU="$csr_organizational_unit_name"/CN="$csr_common_name"/emailAddress="$csr_email_address" >> /dev/null 2>&1
    if [ $? == 0 ]; then
        STATUS="Creating multiple_hostname.csr certificate successfully"
    else
        echo "Creating multiple_hostname.csr certificate failed"
    fi
else
    echo "$DEFAULT_KEY_FILE does not exist"
fi
if [ -f "/conf/multiple_hostname.csr" ]; then
     #echo "multiple_hostname.csr exist"
    /usr/bin/openssl x509 -req -in $MULTI_HOSTNAME_CSR -CA $CONF_ROOTCACERT_FILE -CAkey $DEFAULT_KEY_FILE -CAcreateserial -out $MULTI_HOSTNAME_CASIGNED_FILE -days $valid_days -extfile /conf/openssl.cnf -extensions v3_req >> /dev/null 2>&1
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
if [ -f $MULTI_HOSTNAME_CASIGNED_FILE ]; then
    #echo "multiple_hostname_casigned.pem exist"
    chmod 0600 $MULTI_HOSTNAME_CASIGNED_FILE
    mv $MULTI_HOSTNAME_CASIGNED_FILE  $DEFAULT_CERT_FILE
    cat $DEFAULT_CERT_FILE $CONF_ROOTCACERT_FILE $DEFAULT_KEY_FILE $DEFAULT_DHPARAM_FILE > $CONF_COMBINED_SSL_FILE
    cp $CONF_ROOTCACERT_FILE $CONF_INTERMEDIATE_FILE
    chmod 0600 $CONF_COMBINED_SSL_FILE

    chmod 0600 $CONF_ROOTCACERT_FILE

    if [ $? == 0 ]; then
        echo "SSL certificates generated successfully"
    else
        echo "multiple_hostname_casigned.pem copied as privkey.pem in /conf/cert path FAILED"
    fi
else
    echo "multiple_hostname_casigned.pem does not exist"
fi
