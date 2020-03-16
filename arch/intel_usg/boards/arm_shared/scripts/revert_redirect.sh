#!/bin/sh
##################################################################################
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
#####################################################################################

source /lib/rdk/t2Shared_api.sh

source /etc/utopia/service.d/log_capture_path.sh

if [ -f /etc/device.properties ]
then
    source /etc/device.properties
fi

SERVER6_CONF="/etc/dibbler/server.conf"
SERVER6_BKUP="/nvram/server_bkup.conf"
Uncommented_line=""
REVERTED_FLAG="/nvram/reverted"
WAN_INTERFACE="erouter0"
v4Count=0
v6Count=0
response=
if [ "$1" != "rfcp" ]
then
    echo_t "Revert Redirect : Reverting back the changes made for redirecting the URL's"
    syscfg set redirection_flag false

    # Check if command execution was success
    # If not, set syscfg value once again
    result=`echo $?`
    if [ "$result" != "0" ]
    then
        echo_t "Revert Redirect : Setting redirection_flag to false failed, try resetting it.."
        syscfg set redirection_flag false
    fi

    syscfg commit

    # Check if command execution was success
    # If not, commit syscfg value once again
    result=`echo $?`
    if [ "$result" != "0" ]
    then
        syscfg commit
    fi

    echo_t "Revert Redirect : Indicate revert rediretion is triggered"
    touch $REVERTED_FLAG
    uptime=`cat /proc/uptime | awk '{ print $1 }' | cut -d"." -f1`
    echo_t "Exit_WiFi_Personalization_captive_mode:$uptime"
    t2ValNotify "btime_wcpexit_split" $uptime
    if [ -e "/usr/bin/onboarding_log" ]; then
        /usr/bin/onboarding_log "Exit_WiFi_Personalization_captive_mode:$uptime"
    fi
fi
echo_t "Revert Redirect : Reverting dns configuration changes and restarting DHCP servers"
sysevent set dhcp_server-stop
# Let's make sure dhcp server restarts properly
sleep 1
sysevent set dhcp_server-start

if [ "$1" != "rfcp" ]
    then
    #Changes for EMS starts here
    echo_t "Obtaining the SSID and Password for 2.4 and 5 GHz"
    #echo "Getting Device.WiFi.SSID.1.SSID.."
    SSID_1=`dmcli eRT getv Device.WiFi.SSID.1.SSID | grep string | cut -d":" -f3- | cut -d" " -f2-`
    #echo $SSID_1
    #echo "Getting Device.WiFi.AccessPoint.1.Security.KeyPassphrase.."
    Password_1=`dmcli eRT getv Device.WiFi.AccessPoint.1.Security.X_COMCAST-COM_KeyPassphrase | grep string | cut -d":" -f3- | cut -d" " -f2-`
    #echo $Password_1
    #echo "Getting Device.WiFi.SSID.2.SSID.."
    SSID_2=`dmcli eRT getv Device.WiFi.SSID.2.SSID | grep string | cut -d":" -f3- | cut -d" " -f2-`
    #echo $SSID_2
    #echo "Getting Device.WiFi.AccessPoint.2.Security.KeyPassphrase.."
    Password_2=`dmcli eRT getv Device.WiFi.AccessPoint.2.Security.X_COMCAST-COM_KeyPassphrase | grep string | cut -d":" -f3- | cut -d" " -f2-`
    #echo $Password_2
    #echo "Getting Device.X_CISCO_COM_CableModem.MACAddress.."
    CM_MAC=`dmcli eRT getv Device.X_CISCO_COM_CableModem.MACAddress | grep string | awk '{print $5}'`
    #echo $CM_MAC
    #echo "Getting Device.X_CISCO_COM_CableModem.IPAddress.."
    CM_IP=`dmcli eRT getv Device.X_CISCO_COM_CableModem.IPAddress | grep string | awk '{print $5}'`

    if [ "$CM_IP" = "0.0.0.0" ]
    then
        CM_IP=`dmcli eRT getv Device.X_CISCO_COM_CableModem.IPv6Address | grep string | awk '{print $5}'`
    fi
    #echo $CM_IP
    #echo "Getting Mobile Number.."
    Mobile_Number=`dmcli eRT getv Device.DeviceInfo.X_COMCAST-COM_EMS_MobileNumber | grep string | awk '{print $5}'`
    #echo $Mobile_Number
    #echo "Getting Server IP address.."
    Server_URL=`dmcli eRT getv Device.DeviceInfo.X_COMCAST-COM_EMS_ServerURL | grep string | awk '{print $5}'`
    #echo $Server_URL

    echo_t "Exporting path for curl.."
    export PATH=/fss/gw:$PATH

    defaultMobileNumber="0000000000"
    #echo "Default Mob No = $defaultMobileNumber"
    #echo "Mobile_Number = $Mobile_Number"
    if [ "$Mobile_Number" != "" ] && [ "$Mobile_Number" != "$defaultMobileNumber" ]
    then
        echo_t "Revert Redirect : Triggering EMS service using curl.."
        #curl --data "SSID_1=$SSID_1&Password_1=$Password_1&Mobile_Number=$Mobile_Number&SSID_2=$SSID_1&Password_2=$Password_2&CM_MAC=$CM_MAC&CM_IP=$CM_IP" http://96.119.182.89:8080/captiveportalEMS

        # If SSID for 2.4 and 5 are same, then SMS to be send is single band template
        # Server will check for this value to take decision on template
        if [ "$SSID_1" = "$SSID_2" ]
        then
            SSID_2="default#@!$"
        fi
    while :
    do 
               # Check v4Count. This variable will be incremented only when we have IPv6 for erouter0
               # Try curl command for 3 times in v4 mode. If all 3 fails, try curl command for 3 times in v6 mode
                       # only if erouter0 has an IPv6.
               if [ $v4Count -lt 3 ]
               then
                  echo_t "Revert Redirect: Executing command for ipv4"
                  curl -4 -v --interface erouter0 --data "SSID_1=$SSID_1&Password_1=$Password_1&Mobile_Number=$Mobile_Number&SSID_2=$SSID_2&Password_2=$Password_2&CM_MAC=$CM_MAC&CM_IP=$CM_IP" $Server_URL
                  response=`echo $?`
                  v4Count=`expr $v4Count + 1`
               else
                  has_ipv6=`ifconfig $WAN_INTERFACE | grep inet6 | grep Global`
                  if [ "$has_ipv6" == "" ]
                  then
                  echo_t "Revert Redirect: Ipv6 not found"		
                  break
                  fi
                  # We will come into this else condition, only if erouter0 has IPv6 and 
                  # curl command failed 3 times for IPv4
                  if [ $v6Count -lt 3 ]
                  then
                     echo_t "Revert Redirect: Executing command for ipv6"
                curl -6 -v --interface erouter0 --data "SSID_1=$SSID_1&Password_1=$Password_1&Mobile_Number=$Mobile_Number&SSID_2=$SSID_2&Password_2=$Password_2&CM_MAC=$CM_MAC&CM_IP=$CM_IP" $Server_URL
                response=`echo $?`
                     v6Count=`expr $v6Count + 1`
                  else
                     v4Count=0
                     v6Count=0
                     echo_t "Revert Redirect: Reseting counters"
                     break
                  fi
               fi
                  if [ $response -eq 0 ] 
                  then
                  echo_t "Revert Redirect:Response is $response"
                    break
                  fi
    done
                  echo_t "Revert Redirect : SMS Execution complete.."
                  dmcli eRT setv Device.DeviceInfo.X_COMCAST-COM_EMS_MobileNumber string 0000000000
    else
        echo_t "Revert Redirect : SMS option is not opted for.."
            echo_t "Mobile number:$Mobile_Number"
    fi

    sleep 5
    #Ends here
fi
#Modify DNS server option in dibbler configuration
if [ -e $SERVER6_CONF ]
then
	#Intel Proposed RDKB Generic Bug Fix from XB6 SDK
	sed -i 's/^.*option dns\-server/option dns\-server/' $SERVER6_CONF
	cat $SERVER6_CONF
else
	echo_t "No dibbler6 configuration available...."
fi

echo_t "Revert Redirect : Restarting 6servers..."
dibbler-server stop
dibbler-server start

#Restart Firewall
#Make sure all DNS redirection rules are removed
echo_t "Revert Redirect : Restarting firewall"
sysevent set firewall-restart

# Restart Zebra to populate RDNSS
sysevent set zebra-restart

if [ "$1" != "rfcp" ]
then
    # Check syscfg value once again to reconfirm
    redirectVal=`syscfg get redirection_flag`
    if [ "$redirectVal" = "true" ]
    then
        echo_t "Revert Redirect : Setting redirection_flag to false failed even after retry, trying to reset it again.."
        syscfg set redirection_flag false
        syscfg commit
    fi
            
    sysevent set captiveportaldhcp reverted

fi

exit 0


