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
#######################################################################################

# This script is used to check if the unit has got network access.
# Query the response code of google service. If we get 204, then that means
# we are not in DNS redirection mode.
# Eg Command: curl -w '%{http_code}\n' http://clients3.google.com/generate_204 --connect-timeout 10 -m 10

WAN_INTERFACE="erouter0"
REVERT_FLAG="/nvram/reverted"
EROUTER_IP=""
EROUTER_IP6=""
HAS_IP=0
URL_1="http://clients3.google.com/generate_204"
URL_2="www.google.com"
URL_3="www.amazon.com"
URL_4="www.facebook.com"
CURL_PATH="/fss/gw"
RESPONSE="/var/tmp/networkresponse.txt"
RESPONSE_1="/var/tmp/url_1_response.txt"
RESPONSE_2="/var/tmp/url_2_response.txt"
RESPONSE_3="/var/tmp/url_3_response.txt"
RESPONSE_4="/var/tmp/url_4_response.txt"
SERVER6_CONF="/etc/dibbler/server.conf"
responseCode_1=0
responseCode_2=0
responseCode_3=0
responseCode_4=0
responseCode=0
superResponse=0
gotResponse=0
v4Count=0
v6Count=0

export PATH=$PATH:/fss/gw
source /etc/utopia/service.d/log_capture_path.sh
ATOM_RPC_IP=`cat /etc/device.properties | grep ATOM_ARPING_IP | cut -f 2 -d"="`
BOX_TYPE=`cat /etc/device.properties | grep BOX_TYPE | cut -f 2 -d"="`

if [ "$BOX_TYPE" = "XB3" ]; then
	LAST_REBOOT_REASON=`syscfg get X_RDKCENTRAL-COM_LastRebootReason`
	echo_t "Network Response: LastRebootReason is $LAST_REBOOT_REASON"
	if [ "$LAST_REBOOT_REASON" = "factory-reset" ]
	then
	    echo_t "Network Response: Removing onboard files from ATOM on factory reset"
		rpcclient $ATOM_RPC_IP "rm /nvram/.device_onboarded"
		rpcclient $ATOM_RPC_IP "rm /nvram/DISABLE_ONBOARD_LOGGING"
	fi
fi

echo_t "Network Response: Checking erouter0 ip address"

# This loop will wait till the erouter0 interface gets IP
while [ $HAS_IP -ne 1 ]
do
	sleep 2
	#Check if WAN got an IP
	EROUTER_IP=`ifconfig $WAN_INTERFACE | grep "inet addr" | cut -d":" -f2 | cut -d" " -f1`

	#If WAN has v4 just get out of the loop
	if [ "$EROUTER_IP" != "" ] 
	then
		HAS_IP=1
	fi
	
done

restartLanServices()
{
    # Check if dnsmasq is up
    CHECK_DNSMASQ=`pidof dnsmasq`
    if [ "$CHECK_DNSMASQ" != "" ]
    then
        echo_t "Network Response: DHCP Server is already running , set an event to restart it"
        sysevent set dhcp_server-restart
    fi
	
    # Check if zebra is up
    CHECK_ZEBRA=`pidof zebra`
    if [ "$CHECK_ZEBRA" != "" ]
    then
        echo_t "Network Response: ZebraService is already running ,restarting it..."
        sysevent set zebra-restart
    fi
    
    echo_t "Network Response: Restarting v6Service"
    # Modify DNS server option in dibbler configuration
    if [ -e $SERVER6_CONF ]
    then
	Uncommented_line=`cat $SERVER6_CONF | grep dns-server | sed -e 's/.//'`
	#cat $SERVER6_CONF | grep dns-server | sed -e 's/.//' > $SERVER6_BKUP
	sed "/dns-server/c \
	\ \ \ \ $Uncommented_line"  $SERVER6_CONF > $SERVER6_BKUP
	cp -f $SERVER6_BKUP $SERVER6_CONF
	rm $SERVER6_BKUP
	cat $SERVER6_CONF
    else
       echo_t "No dibbler6 configuration available...."
    fi

    dibbler-server stop
    dibbler-server start

    sysevent set firewall-restart
    uptime=`cat /proc/uptime | awk '{ print $1 }' | cut -d"." -f1`
    if [ -e "/usr/bin/onboarding_log" ]; then
        /usr/bin/onboarding_log "RDKB_FIREWALL_RESTART:$uptime"
    fi
}


# Check if we are in captive portal mode.
# If unit is in captive portal mode, then restart all necessary services
# to get the corresponding configuration updated.
REDIRECTION_FLAG=`syscfg get redirection_flag`
echo_t "REDIRECTION_FLAG got is : $REDIRECTION_FLAG"
if [ "$REDIRECTION_FLAG" = "true" ] || [ "$BOX_TYPE" = "TCCBR" ] || [ "$BOX_TYPE" = "CFG3" ]
then
	# gotResponse will be marked 1 when any of the required responses are available.
	# We still have to depend on 204 response and response file /var/tmp/networkresponse.txt to minimize code changes
	# determining CP mode.
	# This loop will wait till we get a success response from any one of the URLs
	while [ $gotResponse -ne 1 ]
	do

		# Remove all response files if it already exists
		if [ -e $RESPONSE ] && [ -f "/tmp/.gotnetworkresponse" ]
		then
			responseCode=`cat $RESPONSE`
			rm -rf $RESPONSE				
		fi

		if [ -e $RESPONSE_1 ]
		then
			rm -rf $RESPONSE_1
		fi

		if [ -e $RESPONSE_2 ]
		then
			rm -rf $RESPONSE_2
		fi

		if [ -e $RESPONSE_3 ]
		then
			rm -rf $RESPONSE_3
		fi

		if [ -e $RESPONSE_4 ]
		then
			rm -rf $RESPONSE_4
		fi
	
	
	# RDKB-2752 : Comment out querying multiple URLs
	#if [ $gotResponse -eq 0 ] 
	#then
	#	curl -sL -w '%{http_code}\n' --interface $WAN_INTERFACE $URL_2 --connect-timeout 10 -m 10 -o /dev/null > $RESPONSE_2 

		echo_t "Network Response: Running curl commands to check network access"

		# If any of the service returned success code, mark device is activated	
		# Return codes can be 200 or 302
		# 302 means : URL is redirected by the server
		# 200 means : Successful GET

		if [ $responseCode -eq 204 ]
		then
			echo_t "Network Response: Already $RESPONSE has been available with 204 response code."
			echo $responseCode > $RESPONSE_1
		else
		   # Check v4Count. This variable will be incremented only when we have IPv6 for erouter0
		   # Try curl command for 3 times in v4 mode. If all 3 fails, try curl command for 3 times in v6 mode
                   # only if erouter0 has an IPv6.
		   if [ $v4Count -lt 3 ]
		   then
		      echo_t "Network Response: Executing command for ipv4"
		      curl -4 -w '%{http_code}\n' --interface $WAN_INTERFACE $URL_1 --connect-timeout 30 -m 30 > $RESPONSE_1
		      has_ipv6=`ifconfig $WAN_INTERFACE | grep inet6 | grep Global`
		      if [ "$has_ipv6" != "" ]
		      then
		         echo_t "Network Response: Increment count as we have ipv6 ip"
		         v4Count=`expr $v4Count + 1`
		      fi
		   else
		      # We will come into this else condition, only if erouter0 has IPv6 and 
		      # curl command failed 3 times for IPv4
		      if [ $v6Count -lt 3 ]
		      then
		         echo_t "Network Response: Executing command for ipv6"
		         curl -6 -w '%{http_code}\n' --interface $WAN_INTERFACE $URL_1 --connect-timeout 30 -m 30 > $RESPONSE_1
		         v6Count=`expr $v6Count + 1`
		      else
		         v4Count=0
		         v6Count=0
		      fi
		   fi

		fi
			
		if [ -e $RESPONSE_1 ]
		then
                        # Touching this file and will be checked from webgui.sh
                        touch /tmp/.gotnetworkresponse

			responseCode_1=`cat $RESPONSE_1`
			echo_t "Network Response: Response code received is $responseCode_1"
			if [ "$responseCode_1" = "" ]
			then
				echo_t "Network Response: Responsefile for $URL_1 was empty.."
				responseCode_1=0
			fi

			if [ "$responseCode_1" = "204" ]
			then
				echo_t "Network Response: Got success response with URL1"
				gotResponse=1
			else
				if [ -f "/etc/ONBOARD_LOGGING_ENABLE" ]; then
				    echo_t "Network Response: Enabling onboard logs backup"
					touch /tmp/backup_onboardlogs
				fi
			fi
		fi	
	
		if [ $gotResponse -eq 1 ] 
		then
			#superResponse=204
			echo 204 > $RESPONSE
			echo_t "Network Response: Got 204. Move on.."

			#if [ "$BOX_TYPE" = "XB6" ]; then
			#	white=0
			#	solid=0
			#	/usr/bin/SetLED $white $solid		#Set LED to Solid White
			#fi

			isUnitActivated=`syscfg get unit_activated`
			if [ "$isUnitActivated" = "" ]
			then
			    echo_t "Network Response: unit_activated is NULL. Retry"
			    isUnitActivated=`syscfg get unit_activated`
			    if [ "$isUnitActivated" = "" ]
			    then
			       echo_t "Network Response: unit_activated is NULL after retry. Fallback to 0"
			       isUnitActivated=0
			    fi
			fi

			if [ $isUnitActivated -eq 0 ]
			then
				# Set syscfg parameter to indicate unit is activated
				syscfg set unit_activated 1
				syscfg commit
				uptime=`cat /proc/uptime | awk '{ print $1 }' | cut -d"." -f1`
				echo_t "Exit_Captive_Mode:$uptime"
				if [ -e "/usr/bin/onboarding_log" ]; then
					/usr/bin/onboarding_log "Exit_Captive_Mode:$uptime"
				fi
				echo_t "Network Response: Restart DHCP server"
				sysevent set dhcp_server-stop
				# Let's make sure dhcp server restarts properly
				sleep 1
				sysevent set dhcp_server-start
				echo_t "Network Response: Restart Firewall"
				sysevent set firewall-restart
				uptime=`cat /proc/uptime | awk '{ print $1 }' | cut -d"." -f1`
				if [ -e "/usr/bin/onboarding_log" ]; then
					/usr/bin/onboarding_log "RDKB_FIREWALL_RESTART:$uptime"
				fi
				# We will flag that DHCP server is restarted after activation
				# This event is checked in redirect_url.sh
				echo_t "Network Response: Set sysevent to indicate dhcp started"
				sysevent set dhcp_after_activation flagged
			fi

			if [ -e "$REVERT_FLAG" ]
			then
				echo_t "Network Response: Reverted flag should not be present in case of default state"	
				rm -f $REVERT_FLAG
			fi
			break;
		else
				echo_t "Network Response: Didnt recieve success response..should retry.."

    			if [ "$BOX_TYPE" = "XB6" ]; then
        			white=0
        			blink=1
					interval=1
        			/usr/bin/SetLED $white $blink $interval     #Set LED to Blink White per second
    			fi

				unitActivated=`syscfg get unit_activated`
				if [ "$unitActivated" != 0 ]
				then
				    # As we haven't received 204 response, indicate unit is not activated
				    syscfg set unit_activated 0
				    syscfg commit
				    uptime=`cat /proc/uptime | awk '{ print $1 }' | cut -d"." -f1`
				    echo_t "Enter_Captive_Mode:$uptime"
				    if [ -e "/usr/bin/onboarding_log" ]; then
				        /usr/bin/onboarding_log "Enter_Captive_Mode:$uptime"
				    fi
				    # Remove /var/tmp/networkresponse.txt file.
				    # LAN services should not be in captive portal configuration. 
				    # LAN services depend on 204 response to configure captive portal.
				    # By default, we have put 204 into /var/tmp/networkresponse.txt from utopia_init.sh
				    echo_t "Network Response: Removing networkresponse.txt file before restarting services.."
				    rm -rf $RESPONSE
				    restartLanServices
				fi
                        
				sleep 5
		fi

	done
else
	# We are here as Db value indicate that the unit is already configured with personalized
	# WiFi settings. So we mark the device as activated rather querying any service.
	echo_t "Network Response: WiFi is already configured. Hence marking as got response 204"
	echo 204 > $RESPONSE
	#Set syscfg parameter to indicate unit is activated
	syscfg set unit_activated 1
	syscfg commit

    if [ "$BOX_TYPE" = "XB6" ]; then
        white=0
        solid=0
        /usr/bin/SetLED $white $solid       #Set LED to Solid White
    fi

	if [ ! -e "$REVERT_FLAG" ]
	then
		echo_t "Network Response: WiFi is already configured. Set reverted flag in nvram"	
		touch $REVERT_FLAG
	fi
fi

# to update device onboarding state.
if [ -f "/etc/ONBOARD_LOGGING_ENABLE" ]; then
    NETWORKRESPONSEVALUE=`cat /var/tmp/networkresponse.txt`
    unitActivated=`syscfg get unit_activated`
    echo_t "Network Response: Updating device onboarding state"
    if [ "$unitActivated" = "1" ] && [ "$NETWORKRESPONSEVALUE" = "204" ]
    then
        echo_t "Network Response: Device is detected as onboarded"
		if [ -f "/nvram/.device_onboarded" ]
		then
			echo_t "Network Response: /nvram/.device_onboarded is already present"
		else
		    echo_t "Network Response: Creating /nvram/.device_onboarded file"
		    touch /nvram/.device_onboarded
		    if [ -e "/usr/bin/rpcclient" ] && [ "$BOX_TYPE" == "XB3" ]; then
		        rpcclient $ATOM_RPC_IP "/bin/touch /nvram/.device_onboarded"
		    fi
		fi
    else
        echo_t "Network Response: Device is detected as not onboarded"
        if [ -f "/nvram/.device_onboarded" ]
        then
            rm -rf /nvram/.device_onboarded
            if [ -e "/usr/bin/rpcclient" ] && [ "$BOX_TYPE" == "XB3" ]; then
                rpcclient $ATOM_RPC_IP "rm /nvram/.device_onboarded"
            fi
        fi
    fi
fi

# In factory default condition, we should check if all LAN configurations are up.
# If they are up, restart it.
# This check is needed because when WAN comes up later than LAN we should make all 
# configurations in sync

CAPTIVEPORTAL_ENABLED=`syscfg get CaptivePortal_Enable`
echo_t "Network Response : CaptivePortal enabled val is $CAPTIVEPORTAL_ENABLED"


if [ "$REDIRECTION_FLAG" = "true" ] && [ "$CAPTIVEPORTAL_ENABLED" == "true" ] 
then
        #Check if lighttpd daemon is up
        CHECK_LIGHTTPD=`pidof lighttpd`
        if [ "$CHECK_LIGHTTPD" != "" ]
        then
                echo_t "Network Response: Check ConfigureWiFi parameter is in sync or not"
                SET_CONFIGURE_FLAG=`psmcli get eRT.com.cisco.spvtg.ccsp.Device.WiFi.NotifyWiFiChanges`

                #Read the http response value
                NETWORKRESPONSEVALUE=`cat /var/tmp/networkresponse.txt`

                iter=0
                max_iter=2
                while [ "$SET_CONFIGURE_FLAG" = "" ] && [ "$iter" -le $max_iter ]
                do
                   iter=$((iter+1))
                   echo_t "$iter"
                   SET_CONFIGURE_FLAG=`psmcli get eRT.com.cisco.spvtg.ccsp.Device.WiFi.NotifyWiFiChanges`
                done
                echo_t "Network Response: NotifyWiFiChanges is $SET_CONFIGURE_FLAG"
                echo_t "Network Response: Redirection_flag value is $REDIRECTION_FLAG"

                if [ "$NETWORKRESPONSEVALUE" = "204" ] && [ "$SET_CONFIGURE_FLAG" = "true" ]
                then
                    count=0
                    MaxCount=30
                    while : ; do

                        if [ -f "/tmp/pam_initialized" ]
                        then
                           if [ ! -f "/tmp/.configurewifidone" ]
                           then
                              echo_t "Network Response: WiFi is not configured,setting ConfigureWiFi to true"
                              output=`dmcli eRT setvalues Device.DeviceInfo.X_RDKCENTRAL-COM_ConfigureWiFi bool TRUE`
                              check_success=`echo $output | grep  "Execution succeed."`
                              if [ "$check_success" != "" ]
                              then
                                 echo_t "Network Response : Setting ConfigureWiFi to true is success"
				uptime=`cat /proc/uptime | awk '{ print $1 }' | cut -d"." -f1`
				 echo_t "Enter_WiFi_Personalization_captive_mode:$uptime"
				 if [ -e "/usr/bin/onboarding_log" ]; then
				   /usr/bin/onboarding_log "Enter_WiFi_Personalization_captive_mode:$uptime"
				 fi
                                 touch /tmp/.configurewifidone
                              fi
                           fi
                           break
                        else
                           sleep 5
                           count=$((count+1))
                           if [ "$count" -ge "$MaxCount" ]
                           then
                              echo_t "Network Response : Max wait for PandM reached , breaking the loop"
                              break
                           fi
                        fi
                    done
                else
                    echo_t "Network Response: WiFi is already personalized setting redirection_flag to false"
                    #We reached here as redirection_flag is "true". But WiFi is configured already as per notification status.
                    #Set syscfg value to false now.
                    syscfg set redirection_flag false
                    syscfg commit
                    echo_t "Network Response: ConfigureWiFi is false.Set reverted flag in nvram"
                    if [ ! -e "$REVERT_FLAG" ]
                    then
                       touch $REVERT_FLAG
                    fi
                fi

        fi
fi

