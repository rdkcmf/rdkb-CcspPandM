#! /bin/sh
######################################################################################
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
######################################################################################

source /etc/utopia/service.d/log_capture_path.sh

WHITELIST_URL=$1
echo_t "whitelisting address= $WHITELIST_URL"

RESOLV_CONF=/etc/resolv.conf

# nested dmcli get is causing issue when we issue dmcli set
#WiFiConfigure=`dmcli eRT getv Device.DeviceInfo.X_RDKCENTRAL-COM_ConfigureWiFi | grep bool, | awk '{print $5}'`

# New flag /nvram/reverted which will indicate whether WiFi changes are done and 
# captive portal redirection is reverted
if [ ! -e "/nvram/reverted" ]
then
	ishttp=`echo $WHITELIST_URL | grep //`
    echo_t "Whitelist Script : ishttp is $ishttp"

    if [ "$ishttp" != "" ]
    then
        WHITELIST_URL=`echo $WHITELIST_URL | cut -f2 -d":" | cut -f3 -d"/"`
        echo_t "Whitelist Script :  url is $WHITELIST_URL"
    fi
	
    # Commenting the code to receive the ipv4 dns server address from the resolv_bkup.conf file for situations
    # where erouter0 has an ipv6 address and calls whitelist and later gets an ipv4 address  
	# isIPv4=`ifconfig erouter0 | grep inet | grep -v inet6`
	# if [ "$isIPv4" = "" ]
	# then
    #    	 isIPv6=`ifconfig erouter0 | grep inet6`
    #    	 if [ "$isIPv6" != "" ]
	#	     then
    
    # nServer6=`cat /var/tmp/resolv_bkup.conf | grep nameserver | grep ":" | head -1 | cut -d" " -f2`
    # echo "Whitelist Script :  nServer6 is  $nServer6"
	#        fi
	# else	

    nServer4=`cat $RESOLV_CONF | grep nameserver | grep "\." | head -1 | cut -d" " -f2`
    echo_t "Whitelist Script : nServer4 is $nServer4"

	# fi

	if [ "$nServer4" != "" ]
	then    
            echo_t "Whitelist Script : Whitelisitng server with nServer4"
    		echo "server=/$WHITELIST_URL/$nServer4" >> /var/dnsmasq.conf
	fi
	# if [ "$nServer6" != "" ]
	# then
    #        echo "Whitelist url : Whitelisitng server with nServer6"
    #        echo "server=/$WHITELIST_URL/$nServer6" >> /var/dnsmasq.conf
	# fi


	echo_t "Restarting dnsmasq daemon"
#	sysevent set dhcp_server-stop
	# Let's make sure dhcp server restarts properly
#	sleep 1
#	sysevent set dhcp_server-start
	echo_t "Restarting dnsmasq daemon with lan_not_restart"
	sysevent set dhcp_server-restart lan_not_restart
fi
