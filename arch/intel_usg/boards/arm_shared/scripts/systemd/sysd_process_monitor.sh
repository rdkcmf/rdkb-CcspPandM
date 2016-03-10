#! /bin/sh

SELFHEAL_ENABLE=`syscfg get selfheal_enable`
if [ "$SELFHEAL_ENABLE" == "false" ]
then
	export LD_LIBRARY_PATH=$PWD:.:$PWD/../../lib:$PWD/../../.:/lib:/usr/lib:$LD_LIBRARY_PATH
	export DBUS_SYSTEM_BUS_ADDRESS=unix:path=/var/run/dbus/system_bus_socket

	source /etc/utopia/service.d/log_env_var.sh
	source /etc/utopia/service.d/log_capture_path.sh
	loop=1

	while [ $loop -eq 1 ]
	do
		sleep 300

		cd /usr/ccsp/
	
		# Checking snmp subagent PID
		SNMP_PID=`pidof snmp_subagnet`
		if [ "$SNMP_PID" = "" ]; then
			echo "RDKB_PROCESS_CRASHED : snmpsubagent_process is not running, restarting it"
			cd snmp/
			sh run_subagent.sh /var/tmp/cm_snmp_ma &
			cd ..
		fi

		# Checking Home Security's PID
		HOMESEC_PID=`pidof CcspHomeSecurity`
		if [ "$HOMESEC_PID" = "" ]; then
			echo "RDKB_PROCESS_CRASHED : HomeSecurity_process is not running, restarting it"
			CcspHomeSecurity 8081&
		fi

			# Checking dropbear PID
		DROPBEAR_PID=`pidof dropbear`
		if [ "$DROPBEAR_PID" = "" ]; then
			echo "RDKB_PROCESS_CRASHED : dropbear_process is not running, restarting it"
			sh /etc/utopia/service.d/service_sshd.sh sshd-restart &
		fi


		#hotspotfd -n erouter0 -e 1
		#hotspot_arpd -q 0
		#dhcp_snooperd -q 1 -n 2 -e 1
		HOTSPOT_ENABLE=`dmcli eRT getv Device.DeviceInfo.X_COMCAST_COM_xfinitywifiEnable | grep value | cut -f3 -d : | cut -f2 -d" "`
		if [ "$HOTSPOT_ENABLE" = "true" ]
		then
			BASEQUEUE=1
			keepalive_args="-n `sysevent get wan_ifname` -e 1"
			HOTSPOTDAEMON_PID=`pidof hotspotfd`
			if [ "$HOTSPOTDAEMON_PID" = "" ]; then
				echo "RDKB_PROCESS_CRASHED : HotSpot_process is not running, restarting it"
					hotspotfd $keepalive_args  > /dev/null &
			fi    
			DHCP_SNOOPERD_PID=`pidof dhcp_snooperd`
			if [ "$DHCP_SNOOPERD_PID" = "" ]; then
				echo "RDKB_PROCESS_CRASHED : DhcpSnooper_process is not running, restarting it"
					dhcp_snooperd -q $BASEQUEUE -n 2 -e 1  > /dev/null &
			fi 
			DHCP_ARP_PID=`pidof hotspot_arpd`
			if [ "$DHCP_ARP_PID" = "" ]; then
				echo "RDKB_PROCESS_CRASHED : DhcpArp_process is not running, restarting it"
					hotspot_arpd -q 0  > /dev/null &
			fi
		fi

	
		# Checking webpa PID
		WEBPA_PID=`pidof webpa`
		if [ "$WEBPA_PID" = "" ]; then
			ENABLEWEBPA=`cat /nvram/webpa_cfg.json | grep -r EnablePa | awk '{print $2}' | sed 's|[\"\",]||g'`
			echo "ENABLEWEBPA is $ENABLEWEBPA"
			if [ "$ENABLEWEBPA" = "true" ];then
			echo "RDKB_PROCESS_CRASHED : WebPA_process is not running, trying to restart it"
				#cd webpa/
				/usr/bin/webpa -subsys $Subsys
				cd ..
			else
				echo "EnablePa is false in config file. Hence not initializng WebPA.."
			fi

		fi
fi

done
