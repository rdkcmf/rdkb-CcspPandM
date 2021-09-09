#!/bin/sh
#######################################################################################
# If not stated otherwise in this file or this component's Licenses.txt file the
# following copyright and licenses apply:

#  Copyright 2018 RDK Management

# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at

# http://www.apache.org/licenses/LICENSE-2.0

# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
########################################################################################

source /etc/utopia/service.d/log_capture_path.sh
REVERTED_FLAG="/nvram/reverted"
syscfg set redirection_flag true 
syscfg commit
DHCPDONE=`sysevent get captiveportaldhcp`
SERVER6_CONF="/etc/dibbler/server.conf"
MIGRATED_FLAG="/nvram/migratedfrom_nonrdkb"
SERVER6_BKUP="/nvram/server_bkup.conf"

if [ "$1" != "rfcp" ]
then
   # We need to restart DHCP restart and firewall only in case of WiFi factory restore.
   # /nvram/reverted will be created only when user changes WiFI and password
   if [ -e "$REVERTED_FLAG" ] || [ -e "$MIGRATED_FLAG" ]
   then
	echo_t "Redirect URL : Removing reverted flag"
	rm -f $REVERTED_FLAG
   fi

   # We need not have to restart dnsmasq if this event is set
   # We have set this event immediately after activation while doing 
   # DHCP server restart in network_response.sh
   dhcp_activation=`sysevent get dhcp_after_activation`
   if [ "$dhcp_activation" != "flagged" ]
   then
     echo_t "Redirect URL : Restarting dnsmasq daemon for redirection"
     cat /var/dnsmasq.conf
     sysevent set dhcp_server-restart
   else
     # Set a different value so that we will restart DHCP server in case 
     # of a WiFi factory reset
     sysevent set dhcp_after_activation completed
   fi
fi
	
# Restart Firewall. 
#This is required as have to put DNS redirection rules in case of 
# WiFi factory restore/Factory restore.
echo_t "Redirect URL : Restarting firewall"
sysevent set firewall-restart

if [ -e "/usr/bin/onboarding_log" ]; then
    uptime=$(cut -d. -f1 /proc/uptime)
    /usr/bin/onboarding_log "RDKB_FIREWALL_RESTART:$uptime"
fi

if [ "$1" = "rfcp" ]
then
   echo_t "RF CP dnsmasq restart"
   cat /var/dnsmasq.conf
   sysevent set dhcp_server-restart

   sysevent set zebra-restart
fi

echo_t "Redirect URL : Restarting dibblerServer"
# Modify DNS server option in dibbler configuration
if [ -e $SERVER6_CONF ]
then
    sed -e '/dns-server/s/^/#/g' -i $SERVER6_CONF 
    cat $SERVER6_CONF
else
    echo_t "Redirect URL : No dibbler6 configuration available...."
fi

dibbler-server stop
dibbler-server start
if [ "$1" != "rfcp" ]
then
  if [ "$DEVICE_MODEL" = "TCHXB3" ]
  then
     if [ -e "$MIGRATED_FLAG" ]
     then
        sysevent set zebra-restart
     fi
  fi
fi
