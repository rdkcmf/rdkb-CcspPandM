#! /bin/sh

#export LD_LIBRARY_PATH=$PWD:.:$PWD/../../lib:$PWD/../../.:/lib:/usr/lib:$LD_LIBRARY_PATH
#export DBUS_SYSTEM_BUS_ADDRESS=unix:path=/var/run/dbus/system_bus_socket

#source /fss/gw/etc/utopia/service.d/log_env_var.sh
source /etc/utopia/service.d/log_capture_path.sh

#loop=1

#while [ $loop -eq 1 ]
#do
#       sleep 10
	echo "Obtaining the MoCA status.."
	moca_status=`dmcli eRT getv Device.MoCA.Interface.1.Status | grep string | awk '{print $5}'`
	echo "MoCA is $moca_status"
#done
