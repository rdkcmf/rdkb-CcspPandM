#!/bin/sh
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

source /etc/utopia/service.d/log_capture_path.sh

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

# Check if we are in captive portal mode.
# If unit is in captive portal mode, then restart all necessary services
# to get the corresponding configuration updated.
REDIRECTION_FLAG=`syscfg get redirection_flag`
echo_t "REDIRECTION_FLAG got is : $REDIRECTION_FLAG"
if [ "$REDIRECTION_FLAG" = "true" ]
then
	# gotResponse will be marked 1 when any of the required responses are available.
	# We still have to depend on 204 response and response file /var/tmp/networkresponse.txt to minimize code changes
	# determining CP mode.
	# This loop will wait till we get a success response from any one of the URLs
	while [ $gotResponse -ne 1 ]
	do

		# Remove all response files if it already exists
		if [ -e $RESPONSE ]
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

		export PATH=$PATH:/fss/gw
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
			curl -w '%{http_code}\n' --interface $WAN_INTERFACE $URL_1 --connect-timeout 30 -m 30 > $RESPONSE_1 	
		fi
			
		if [ -e $RESPONSE_1 ]
		then
			responseCode_1=`cat $RESPONSE_1`
			if [ "$responseCode_1" = "" ]
			then
				echo_t "Network Response: Responsefile for $URL_1 was empty.."
				responseCode_1=0
			fi

			if [ "$responseCode_1" = "204" ]
			then
				echo_t "Network Response: Got success response with URL1"
				gotResponse=1
			fi
		fi
	
		# RDKB-2752 : Comment out querying multiple URLs
		#if [ $gotResponse -eq 0 ] 
		#then
		#	curl -sL -w '%{http_code}\n' --interface $WAN_INTERFACE $URL_2 --connect-timeout 10 -m 10 -o /dev/null > $RESPONSE_2 
	
		#	if [ -e $RESPONSE_2 ]
		#	then
		#		responseCode_2=`cat $RESPONSE_2`
		#		if [ "$responseCode_2" = "" ]
		#		then
		#			echo "Network Response: Responsefile for $URL_2 was empty.."
		#			responseCode_2=0
		#		fi
		#	fi
	
		#	if [ $responseCode_2 -eq 200 ] || [ $responseCode_2 -eq 302 ]
		#	then
		#		echo "Network Response: Got success response with URL2"
		#		gotResponse=1
		#	fi
		#fi

	

		#if [ $gotResponse -eq 0 ] 
		#then
		#	curl -sL -w '%{http_code}\n' --interface $WAN_INTERFACE $URL_3 --connect-timeout 10 -m 10 -o /dev/null > $RESPONSE_3 

		#	if [ -e $RESPONSE_3 ]
		#	then
		#		responseCode_3=`cat $RESPONSE_3`
		#		if [ "$responseCode_3" = "" ]
		#		then
		#			echo "Network Response: Responsefile for $URL_3 was empty.."
		#			responseCode_3=0
		#		fi
		#	fi
			
		#	if [ $responseCode_3 -eq 200 ] || [ $responseCode_3 -eq 302 ]
		#	then
		#		echo "Network Response: Got success response with URL3"
		#		gotResponse=1
		#	fi
		#fi

	

		#if [ $gotResponse -eq 0 ] 
		#then
		#	curl -sL -w '%{http_code}\n' --interface $WAN_INTERFACE $URL_4 --connect-timeout 10 -m 10 -o /dev/null > $RESPONSE_4 
	
		#	if [ -e $RESPONSE_4 ]
		#	then
		#		responseCode_4=`cat $RESPONSE_4`
		#		if [ "$responseCode_4" = "" ]
		#		then
		#			echo "Network Response: Responsefile for $URL_4 was empty.."
		#			responseCode_4=0
		#		fi
		#	fi
		
		#	if [ $responseCode_4 -eq 302 ] || [ $responseCode_4 -eq 200 ]
		#	then
		#		echo "Network Response: Got success response with URL4"
		#		gotResponse=1
		#	fi
		#fi
	
	
		if [ $gotResponse -eq 1 ] 
		then
			#superResponse=204
			echo 204 > $RESPONSE
			echo_t "Network Response: Got 204. Move on.."
			if [ -e "$REVERT_FLAG" ]
			then
				echo_t "Network Response: Reverted flag should not be present in case of default state"	
				rm -f $REVERT_FLAG
			fi
			break;
		else
			echo_t "Network Response: Didnt recieve success response..should retry.."
			sleep 5
		fi

	done
else
	# We are here as Db value indicate that the unit is already configured with personalized
	# WiFi settings. So we mark the device as activated rather querying any service.
	echo_t "Network Response: WiFi is already configured. Hence marking as got response 204"
	echo 204 > $RESPONSE

	if [ ! -e "$REVERT_FLAG" ]
	then
		echo_t "Network Response: WiFi is already configured. Set reverted flag in nvram"	
		touch $REVERT_FLAG
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

	# Check if lighttpd daemon is up
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

				echo_t "Network Response: Restarting v6Service"
				#Modify DNS server option in dibbler configuration
				if [ -e $SERVER6_CONF ]
				then
					#cat $SERVER6_CONF | grep dns-server | sed -e 's/.//' > $SERVER6_BKUP
					sed -e '/dns-server/s/^/#/g' -i $SERVER6_CONF 
					cat $SERVER6_CONF
				else
					echo_t "No dibbler6 configuration available...."
				fi

				dibbler-server stop
				dibbler-server start

				echo_t "Network Response: WiFi is not configured,setting ConfigureWiFi to true"	
				dmcli eRT setvalues Device.DeviceInfo.X_RDKCENTRAL-COM_ConfigureWiFi bool TRUE
			else
				echo_t "Network Response: WiFi is already personalized setting redirection_flag to false"
				# We reached here as redirection_flag is "true". But WiFi is configured already as per notification status.
				# Set syscfg value to false now.
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

