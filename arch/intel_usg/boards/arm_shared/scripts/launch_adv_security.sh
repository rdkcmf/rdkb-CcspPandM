#!/bin/bash

source /etc/device.properties

export RUNTIME_BIN_DIR="/usr/ccsp/advsec"
export RUNTIME_LIB_PATH="/usr/lib"
if [ "$DEVICE_MODEL" = "TCHXB3" ]; then
    export RUNTIME_BIN_DIR="/tmp/cujo_dnld/usr/ccsp/advsec"
    export RUNTIME_LIB_PATH="/tmp/cujo_dnld/usr/lib"
fi
CCSP_ADVSEC_INITIALIZING=/tmp/ccsp_advsec_initializing
CCSP_ADVSEC_INITIALIZED_SYSD=/tmp/advsec_initialized_sysd

launch_device_finger_print()
{
echo "Advanced Security Launch Status" $1

if [ "$1" = "-enable" ]
then
    if [ -f $CCSP_ADVSEC_INITIALIZING ]; then
        echo "Ccsp Advanced Security is already being initialized"
        exit 0
    fi
    touch $CCSP_ADVSEC_INITIALIZING
    if [ "$DEVICE_MODEL" = "TCHXB3" ]; then
    export PATH=$PATH:$RUNTIME_BIN_DIR
    export LD_LIBRARY_PATH=/lib:/usr/lib:$RUNTIME_LIB_PATH
        cd $RUNTIME_BIN_DIR
        ./CcspAdvSecuritySsp -subsys eRT. &
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
    if [ -f $CCSP_ADVSEC_INITIALIZING ]; then
        rm $CCSP_ADVSEC_INITIALIZING
    fi
fi
}

if [ "$1" = "-enable" ] || [ "$1" = "-disable" ]
then
    launch_device_finger_print $1
fi
