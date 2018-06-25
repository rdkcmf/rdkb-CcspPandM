#! /bin/sh
####################################################################################
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

#export LD_LIBRARY_PATH=$PWD:.:$PWD/../../lib:$PWD/../../.:/lib:/usr/lib:$LD_LIBRARY_PATH
#export DBUS_SYSTEM_BUS_ADDRESS=unix:path=/var/run/dbus/system_bus_socket

#source /fss/gw/etc/utopia/service.d/log_env_var.sh
source /etc/utopia/service.d/log_capture_path.sh

#loop=1

#while [ $loop -eq 1 ]
#do
#       sleep 10
	echo_t "Obtaining the MoCA status.."
	moca_status=`dmcli eRT getv Device.MoCA.Interface.1.Status | grep string | awk '{print $5}'`
	echo_t "MoCA is $moca_status"
#done
        echo_t "Checking for MoCA Enable.."
	moca_enable=`dmcli eRT getv Device.MoCA.Interface.1.Enable | grep bool | awk '{print $5}'`
	if [ $moca_enable == "true" ];then 
        echo_t "MoCA is Enabled"
        else 
        echo_t "MoCA is Disabled"
        fi
        
	echo_t "Obtaining the MoCA CurrentOperFreq.."
	moca_freq=`dmcli eRT getv Device.MoCA.Interface.1.CurrentOperFreq | grep uint | awk '{print $5}'`
	echo_t "MoCA_FREQ:$moca_freq"
	
	echo_t "Obtaining the MoCA PreferredNC.."
	moca_pnc=`dmcli eRT getv Device.MoCA.Interface.1.PreferredNC | grep bool | awk '{print $5}'`
	echo_t "MoCA_PNC:$moca_pnc"
	
	echo_t "Obtaining the MoCA ChannelScanning.."
	moca_scan=`dmcli eRT getv Device.MoCA.Interface.1.X_CISCO_COM_ChannelScanning | grep bool | awk '{print $5}'`
	echo_t "MoCA_SCAN:$moca_scan"

	echo_t "Obtaining the MoCA FreqCurrentMaskSetting.."
	moca_freq_mask=`dmcli eRT getv Device.MoCA.Interface.1.FreqCurrentMaskSetting | grep string | awk '{print $5}'`
	echo_t "MOCA_FREQ_MASK:$moca_freq_mask"

	echo_t "Obtaining the MoCA PrivacyEnabled.."
	moca_privacy=`dmcli eRT getv Device.MoCA.Interface.1.PrivacyEnabled | grep bool | awk '{print $5}'`
	echo_t "MOCA_PRIVACY:$moca_privacy"
