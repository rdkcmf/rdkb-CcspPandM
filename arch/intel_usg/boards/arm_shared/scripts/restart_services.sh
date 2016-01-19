#! /bin/sh

echo "Inside restart service script"
SERVER6_CONF="/etc/dibbler/server.conf"
SERVER6_BKUP="/nvram/server_bkup.conf"
WIFIUNCONFIGURED=`syscfg get redirection_flag`
# Restart dhcp server
        sysevent set dhcp_server-stop
        # Let's make sure dhcp server restarts properly
        sleep 1
        sysevent set dhcp_server-start


# Restart firewall
	sysevent set firewall-restart

# Restart Zebra to populate RDNSS
	sysevent set zebra-restart

if [ -e $SERVER6_CONF ]
then

	if [ "$1" == "true" ] && [ "$WIFIUNCONFIGURED" == "true" ]
	then
		sed -e '/dns-server/s/^/#/g' -i $SERVER6_CONF 
	else
		Uncommented_line=`cat $SERVER6_CONF | grep dns-server | sed -e 's/.//'`
		#cat $SERVER6_CONF | grep dns-server | sed -e 's/.//' > $SERVER6_BKUP
		sed "/dns-server/c \
		\ \ \ \ $Uncommented_line"  $SERVER6_CONF > $SERVER6_BKUP
		cp -f $SERVER6_BKUP $SERVER6_CONF
		rm $SERVER6_BKUP
	
	fi

else
	echo "No dibbler6 configuration available...."	

fi

	dibbler-server stop
	dibbler-server start


