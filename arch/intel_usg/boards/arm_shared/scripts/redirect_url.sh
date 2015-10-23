#!/bin/sh

source /etc/utopia/service.d/log_capture_path.sh
REVERTED_FLAG="/nvram/reverted"
syscfg set redirection_flag true 
syscfg commit
DHCPDONE=`sysevent get captiveportaldhcp`

# We need to restart DHCP restart and firewall only in case of WiFi factory restore.
# /nvram/reverted will be created only when user changes WiFI and password
if [ -e "$REVERTED_FLAG" ]
then
	echo "Redirect URL : Removing reverted flag"
	rm -f $REVERTED_FLAG

	echo "Redirect URL : Restarting dnsmasq daemon for redirection"
	cat /var/dnsmasq.conf
	sysevent set dhcp_server-stop
	# Let's make sure dhcp server restarts properly
	sleep 1
	sysevent set dhcp_server-start
	
	#Restart Firewall. 
	#This is required as have to put DNS redirection rules in case of WiFi factory restore/Factory restore.
	sysevent set firewall-restart
fi
