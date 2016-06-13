#! /bin/sh

BINPATH="/usr/bin"

SELFHEAL_ENABLE=`syscfg get selfheal_enable`
if [ "$SELFHEAL_ENABLE" == "false" ]
then
	export LD_LIBRARY_PATH=$PWD:.:$PWD/../../lib:$PWD/../../.:/lib:/usr/lib:$LD_LIBRARY_PATH
	export DBUS_SYSTEM_BUS_ADDRESS=unix:path=/var/run/dbus/system_bus_socket

	source /etc/utopia/service.d/log_env_var.sh
	rebootNeeded=0
	rebootNeededforbrlan1=0
	rebootNeededforbrlan0=0
	LIGHTTPD_CONF="/var/lighttpd.conf"

	crashed=0
	
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
			$BINPATH/CcspHomeSecurity 8081&
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
					$BINPATH/hotspotfd $keepalive_args  > /dev/null &
			fi    
			DHCP_ARP_PID=`pidof hotspot_arpd`
			if [ "$DHCP_ARP_PID" = "" ]; then
				echo "RDKB_PROCESS_CRASHED : DhcpArp_process is not running, restarting it"
					$BINPATH/hotspot_arpd -q 0  > /dev/null &
			fi
		fi
	
		# Checking webpa PID
		WEBPA_PID=`pidof webpa`
		if [ "$WEBPA_PID" = "" ]; then
               if [ -f /etc/os-release ];then
                    while [ ! -f /tmp/crash_reboot ]
                    do
                        sleep 2
                    done
                    if [ -f /tmp/crash_reboot ];then rm -rf /tmp/crash_reboot; fi
                fi
			ENABLEWEBPA=`cat /nvram/webpa_cfg.json | grep -r EnablePa | awk '{print $2}' | sed 's|[\"\",]||g'`
			echo "ENABLEWEBPA is $ENABLEWEBPA"
			if [ "$ENABLEWEBPA" = "true" ];then
			echo "RDKB_PROCESS_CRASHED : WebPA_process is not running, trying to restart it"
				$BINPATH/webpa -subsys $Subsys
			else
				echo "EnablePa is false in config file. Hence not initializng WebPA.."
			fi

		fi
		
		# Checking WiFi Agent PID
		WiFi_PID=`pidof CcspWifiSsp`
		if [ "$WiFi_PID" = "" ]; then
			echo "WiFi process is not running, restarting it"
			dmcli eRT setv Device.LogAgent.WifiLogMsg string "RDK_LOG_ERROR,RDKB_PROCESS_CRASHED : WiFiAgent_process is not running, need restart"
			$BINPATH/CcspWifiSsp -subsys $Subsys &
		fi
		
		# Checking Harvester PID
		Harvester_PID=`pidof harvester`
		if [ "$Harvester_PID" = "" ]; then
			echo "Harvester process is not running, restarting it"
			$BINPATH/harvester &
		fi
		
		# Checking lighttpd PID
		LIGHTTPD_PID=`pidof lighttpd`
		if [ "$LIGHTTPD_PID" = "" ]; then
			echo "RDKB_PROCESS_CRASHED : lighttpd is not running, restarting it"
			lighttpd -f $LIGHTTPD_CONF
		fi

		ifconfig | grep brlan1
		if [ $? == 1 ]; then
			if [ "$rebootNeededforbrlan1" -eq 0 ]; then
				echo "[RKDB_PLATFORM_ERROR] : brlan1 interface is not up, need to reboot the unit." 
				rebootNeededforbrlan1=1
				rebootNeeded=1
			fi
		fi

		ifconfig | grep brlan0
		if [ $? == 1 ]; then
			if [ "$rebootNeededforbrlan0" -eq 0 ]; then
				echo "[RKDB_PLATFORM_ERROR] : brlan0 interface is not up" 
				echo "RDKB_REBOOT : brlan0 interface is not up, rebooting the device."
				rebootNeededforbrlan0=1
				rebootNeeded=1
					/fss/gw/rdklogger/backupLogs.sh "true" ""
			fi
		fi

		dmcli eRT getv Device.WiFi.SSID.2.Status | grep Up
		if [ $? == 1 ]; then
			echo "[RKDB_PLATFORM_ERROR] : 5G private SSID (ath1) is off, resetting WiFi now"
			dmcli eRT setv Device.X_CISCO_COM_DeviceControl.RebootDevice string Wifi
		fi

		iptables-save -t nat | grep "A PREROUTING -i"
		if [ $? == 1 ]; then
			echo "[RDKB_PLATFORM_ERROR] : iptable corrupted. Restarting firewall"
			sysevent set firewall-restart
		fi

		if [ "$rebootNeeded" -eq 1 ]
		then
			cur_hr=`date +"%H"`
				cur_min=`date +"%M"`
			if [ $cur_hr -ge 02 ] && [ $cur_hr -le 03 ]
			then
					if [ $cur_hr -eq 03 ] && [ $cur_min -ne 00 ]
					then
						echo "Maintanance window for the current day is over , unit will be rebooted in next Maintanance window "
					else
						#Check if we have already flagged reboot is needed
						if [ ! -e $FLAG_REBOOT ]
						then
							if [ "$rebootNeededforbrlan0" -eq 1 ]
							then
								echo "rebootNeededforbrlan0"
								echo "RDKB_REBOOT : brlan0 interface is not up, rebooting the device."
								echo "Setting last reboot reason"
								dmcli eRT setv Device.DeviceInfo.X_RDKCENTRAL-COM_LastRebootReason string brlan0_down
								dmcli eRT setv Device.DeviceInfo.X_RDKCENTRAL-COM_LastRebootCounter int 1
								echo "SET succeeded"
								sh /etc/calc_random_time_to_reboot_dev.sh "" &
							elif [ "$rebootNeededforbrlan1" -eq 1 ]
							then
								echo "rebootNeededforbrlan1"
								echo "RDKB_REBOOT : brlan1 interface is not up, rebooting the device."
								echo "Setting last reboot reason"
								dmcli eRT setv Device.DeviceInfo.X_RDKCENTRAL-COM_LastRebootReason string brlan1_down
								dmcli eRT setv Device.DeviceInfo.X_RDKCENTRAL-COM_LastRebootCounter int 1
								echo "SET succeeded"
								sh /etc/calc_random_time_to_reboot_dev.sh "" &
							fi
							touch $FLAG_REBOOT
						else
							echo "Already waiting for reboot"
						fi					
					fi
			fi
		fi
	done
fi
