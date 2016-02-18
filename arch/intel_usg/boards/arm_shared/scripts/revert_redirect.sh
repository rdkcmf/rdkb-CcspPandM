#!/bin/sh

source /etc/utopia/service.d/log_capture_path.sh

SERVER6_CONF="/etc/dibbler/server.conf"
SERVER6_BKUP="/nvram/server_bkup.conf"
Uncommented_line=""
REVERTED_FLAG="/nvram/reverted"

echo "Revert Redirect : Reverting back the changes made for redirecting the URL's"
syscfg set redirection_flag false

# Check if command execution was success
# If not, set syscfg value once again
result=`echo $?`
if [ "$result" != "0" ]
then
	echo "Revert Redirect : Setting redirection_flag to false failed, try resetting it.."
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

echo "Revert Redirect : Indicate revert rediretion is triggered"
touch $REVERTED_FLAG

echo "Revert Redirect : Reverting dns configuration changes and restarting DHCP servers"
sysevent set dhcp_server-stop
# Let's make sure dhcp server restarts properly
sleep 1
sysevent set dhcp_server-start

#Changes for EMS starts here
echo "Obtaining the SSID and Password for 2.4 and 5 GHz"
#echo "Getting Device.WiFi.SSID.1.SSID.."
SSID_1=`dmcli eRT getv Device.WiFi.SSID.1.SSID | grep string | awk '{print $5}'`
#echo $SSID_1
#echo "Getting Device.WiFi.AccessPoint.1.Security.KeyPassphrase.."
Password_1=`dmcli eRT getv Device.WiFi.AccessPoint.1.Security.X_COMCAST-COM_KeyPassphrase | grep string | awk '{print $5}'`
#echo $Password_1
#echo "Getting Device.WiFi.SSID.2.SSID.."
SSID_2=`dmcli eRT getv Device.WiFi.SSID.2.SSID | grep string | awk '{print $5}'`
#echo $SSID_2
#echo "Getting Device.WiFi.AccessPoint.2.Security.KeyPassphrase.."
Password_2=`dmcli eRT getv Device.WiFi.AccessPoint.2.Security.X_COMCAST-COM_KeyPassphrase | grep string | awk '{print $5}'`
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

echo "Exporting path for curl.."
export PATH=/fss/gw:$PATH

defaultMobileNumber="0000000000"
#echo "Default Mob No = $defaultMobileNumber"
#echo "Mobile_Number = $Mobile_Number"
if [ "$Mobile_Number" != "$defaultMobileNumber" ]
then
	echo "Revert Redirect : Triggering EMS service using curl.."
	#curl --data "SSID_1=$SSID_1&Password_1=$Password_1&Mobile_Number=$Mobile_Number&SSID_2=$SSID_1&Password_2=$Password_2&CM_MAC=$CM_MAC&CM_IP=$CM_IP" http://96.119.182.89:8080/captiveportalEMS

	# If SSID for 2.4 and 5 are same, then SMS to be send is single band template
	# Server will check for this value to take decision on template
	if [ "$SSID_1" = "$SSID_2" ]
	then
		SSID_2="default#@!$"
	fi
	
	curl -v --cacert /nvram/cacert.pem --interface erouter0 --data "SSID_1=$SSID_1&Password_1=$Password_1&Mobile_Number=$Mobile_Number&SSID_2=$SSID_2&Password_2=$Password_2&CM_MAC=$CM_MAC&CM_IP=$CM_IP" $Server_URL
	
	echo "Revert Redirect : SMS Execution complete.."
	`dmcli eRT setv Device.DeviceInfo.X_COMCAST-COM_EMS_MobileNumber string 0000000000`

else
	echo "Revert Redirect : SMS option is not opted for.."
fi

sleep 5
#Ends here

#Modify DNS server option in dibbler configuration
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
	echo "No dibbler6 configuration available...."
fi

echo "Revert Redirect : Restarting 6servers..."
dibbler-server stop
dibbler-server start

#Restart Firewall
#Make sure all DNS redirection rules are removed
echo "Revert Redirect : Restarting firewall"
sysevent set firewall-restart

# Restart Zebra to populate RDNSS
sysevent set zebra-restart

# Check syscfg value once again to reconfirm
redirectVal=`syscfg get redirection_flag`
if [ "$redirectVal" = "true" ]
then
    echo "Revert Redirect : Setting redirection_flag to false failed even after retry, trying to reset it again.."
    syscfg set redirection_flag false
    syscfg commit
fi
		
sysevent set captiveportaldhcp reverted

exit 0


