#! /bin/sh
###################################################################################
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
##################################################################################

echo "Inside restart service script"
SERVER6_CONF="/etc/dibbler/server.conf"
SERVER6_BKUP="/nvram/server_bkup.conf"
WIFIUNCONFIGURED=`syscfg get redirection_flag`
# Restart dhcp server
        sysevent set dhcp_server-restart


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


