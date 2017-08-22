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
	echo_t "Redirect URL : Removing reverted flag"
	rm -f $REVERTED_FLAG
fi

# We need not have to restart dnsmasq if this event is set
# We have set this event immediately after activation while doing 
# DHCP server rnd lighttpd restarts in network_response.sh
dhcp_activation=`sysevent get dhcp_after_activation`
if [ "$dhcp_activation" != "flagged" ]
then
   echo_t "Redirect URL : Restart lighttpd"
   /etc/webgui.sh &
   echo_t "Redirect URL : Restarting dnsmasq daemon for redirection"
   cat /var/dnsmasq.conf
   sysevent set dhcp_server-stop
   # Let's make sure dhcp server restarts properly
   sleep 1
   sysevent set dhcp_server-start
else
   # Set a different value so that we will restart DHCP server in case 
   # of a WiFi factory reset
   sysevent set dhcp_after_activation completed
fi
	
# Restart Firewall. 
#This is required as have to put DNS redirection rules in case of 
# WiFi factory restore/Factory restore.
echo_t "Redirect URL : Restarting firewall"
sysevent set firewall-restart

