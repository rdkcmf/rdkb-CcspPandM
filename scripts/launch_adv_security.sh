#!/bin/bash
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
################################################################################

source /etc/device.properties

export RUNTIME_BIN_DIR="/usr/ccsp/advsec"
export RUNTIME_LIB_PATH="/usr/lib"
if [ "$DEVICE_MODEL" = "TCHXB3" ]; then
    export RUNTIME_BIN_DIR="/tmp/cujo_dnld/usr/ccsp/advsec"
    export RUNTIME_LIB_PATH="/tmp/cujo_dnld/usr/lib"
fi
CCSP_ADVSEC_INITIALIZING=/tmp/ccsp_advsec_initializing
CCSP_ADVSEC_INITIALIZED_SYSD=/tmp/advsec_initialized_sysd
ADVSEC_AGENT_SHUTDOWN=/tmp/advsec_agent_shutdown
ADVSEC_CONFIG_PARAMS_PATH="/tmp/advsec_config_params"

launch_device_finger_print()
{
echo "Advanced Security Launch Status" $1
bridge_mode=`syscfg get bridge_mode`
if [ "$1" = "-enable" ]
then
    if [ "$bridge_mode" = "2" ]; then
        echo "Advsec: Device is in Bridge Mode, do not launch agent!"
        touch $ADVSEC_AGENT_SHUTDOWN
        exit 0
    else
        if [ -f $ADVSEC_AGENT_SHUTDOWN ]; then
            rm $ADVSEC_AGENT_SHUTDOWN
        fi
    fi

    if [ -f $CCSP_ADVSEC_INITIALIZING ]; then
        echo "Ccsp Advanced Security is already being initialized"
        exit 0
    fi
    touch $CCSP_ADVSEC_INITIALIZING
    if [ "$DEVICE_MODEL" = "TCHXB3" ]; then
    export PATH=$PATH:$RUNTIME_BIN_DIR
    export LD_LIBRARY_PATH=/lib:/usr/lib:$RUNTIME_LIB_PATH
        if [ -e $RUNTIME_BIN_DIR ]; then
                cd $RUNTIME_BIN_DIR
                ./CcspAdvSecuritySsp -subsys eRT. &
        else
                /usr/sbin/cujo_download.sh &
        fi
    else
        if [ "$BOX_TYPE" = "XB3" ]; then
                cd $RUNTIME_BIN_DIR
                ./CcspAdvSecuritySsp -subsys eRT. &
        else
                echo "Device_Finger_Printing_enabled:true"
		touch $CCSP_ADVSEC_INITIALIZED_SYSD
                systemctl start CcspAdvSecuritySsp.service
	fi
    fi
    rm $CCSP_ADVSEC_INITIALIZING
elif [ "$1" = "-disable" ]
then
    $RUNTIME_BIN_DIR/start_adv_security.sh -disable
    if [ "$BOX_TYPE" = "XB3" ]; then
        killall CcspAdvSecuritySsp
    else
        echo "Device_Finger_Printing_enabled:false"
	rm $CCSP_ADVSEC_INITIALIZED_SYSD
        systemctl stop CcspAdvSecuritySsp.service
    fi

    if [ -d "${ADVSEC_CONFIG_PARAMS_PATH}" ]; then
        rm -rf ${ADVSEC_CONFIG_PARAMS_PATH}
    fi

    if [ -f $CCSP_ADVSEC_INITIALIZING ]; then
        rm $CCSP_ADVSEC_INITIALIZING
    fi
    if [ "$bridge_mode" = "2" ]; then
        touch $ADVSEC_AGENT_SHUTDOWN
    else
        if [ -f $ADVSEC_AGENT_SHUTDOWN ]; then
            rm $ADVSEC_AGENT_SHUTDOWN
        fi
    fi
fi
}

enable_sysd_start()
{
    bridge_mode=`syscfg get bridge_mode`
    if [ "$bridge_mode" != "2" ]; then
        touch $CCSP_ADVSEC_INITIALIZED_SYSD
    fi
}

if [ "$1" = "-enable" ] || [ "$1" = "-disable" ]
then
    launch_device_finger_print $1
elif [ "$1" = "-sysd" ]
then
    enable_sysd_start
fi
