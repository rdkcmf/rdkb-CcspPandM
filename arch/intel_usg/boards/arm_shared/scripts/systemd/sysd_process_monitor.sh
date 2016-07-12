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
		if [ "$SNMP_PID" == "" ]; then
			echo "RDKB_PROCESS_CRASHED : snmpsubagent_process is not running, restarting it"
			cd snmp/
			sh run_subagent.sh /var/tmp/cm_snmp_ma &
			cd ..
		fi

		# Checking Home Security's PID
		HOMESEC_PID=`pidof CcspHomeSecurity`
		if [ "$HOMESEC_PID" == "" ]; then
			echo "RDKB_PROCESS_CRASHED : HomeSecurity_process is not running, restarting it"
			$BINPATH/CcspHomeSecurity 8081&
		fi

			# Checking dropbear PID
		DROPBEAR_PID=`pidof dropbear`
		if [ "$DROPBEAR_PID" == "" ]; then
			echo "RDKB_PROCESS_CRASHED : dropbear_process is not running, restarting it"
			sh /etc/utopia/service.d/service_sshd.sh sshd-restart &
		fi
	
		# Checking webpa PID
		WEBPA_PID=`pidof webpa`
		if [ "$WEBPA_PID" == "" ]; then
               if [ -f /etc/os-release ];then
                    while [ ! -f /tmp/crash_reboot ]
                    do
                        sleep 2
                    done
                    if [ -f /tmp/crash_reboot ];then rm -rf /tmp/crash_reboot; fi
                fi
			ENABLEWEBPA=`cat /nvram/webpa_cfg.json | grep -r EnablePa | awk '{print $2}' | sed 's|[\"\",]||g'`
			echo "ENABLEWEBPA is $ENABLEWEBPA"
			if [ "$ENABLEWEBPA" == "true" ];then
			echo "RDKB_PROCESS_CRASHED : WebPA_process is not running, trying to restart it"
			#We'll set the reason only if webpa reconnect is not due to DNS resolve
			syscfg get X_RDKCENTRAL-COM_LastReconnectReason | grep "Dns_Res_webpa_reconnect"
			if [ $? != 0 ]; then
				echo "setting reconnect reason from sysd_process_monitor.sh"
				syscfg set X_RDKCENTRAL-COM_LastReconnectReason WebPa_crash
				result=`echo $?`
				if [ "$result" != "0" ]
				then
				    echo "SET for Reconnect Reason failed"
				fi
				syscfg commit
				result=`echo $?`
				if [ "$result" != "0" ]
				then
				    echo "Commit for Reconnect Reason failed"
				fi
			fi
				$BINPATH/webpa -subsys $Subsys
			else
				echo "EnablePa is false in config file. Hence not initializng WebPA.."
			fi

		fi
		
		# Checking WiFi Agent PID
		WiFi_PID=`pidof CcspWifiSsp`
		if [ "$WiFi_PID" == "" ]; then
			echo "WiFi process is not running, restarting it"
			cd /wifi
			export LD_LIBRARY_PATH=$PWD:.:$PWD/../../lib:$PWD/../../.:/lib:/usr/lib:$LD_LIBRARY_PATH
			dmcli eRT setv Device.LogAgent.WifiLogMsg string "RDK_LOG_ERROR,RDKB_PROCESS_CRASHED : WiFiAgent_process is not running, need restart"
			sh /etc/ath/fast_down.sh
			$BINPATH/CcspWifiSsp -subsys $Subsys &
			cd ..
			sleep 60
		else
			WIFI_RESTART=0
			APUP_PID=`pidof apup`
			if [ "$APUP_PID" == "" ]; then
                         
				HOSTAPD_PID=`pidof hostapd`
				if [ "$HOSTAPD_PID" == "" ]; then
	        		WIFI_RESTART=1
					echo "Hostapd process is not running, restarting WiFi"
					dmcli eRT setv Device.LogAgent.WifiLogMsg string "RDK_LOG_ERROR,RDKB_PROCESS_CRASHED : Hostapd_process is not running, restarting WiFi"
					#dmcli eRT setv Device.X_CISCO_COM_DeviceControl.RebootDevice string Wifi
				fi
                        
				check_ap_enable5=`cfg -e | grep AP_ENABLE_2=1 | cut -d"=" -f2`
				check_radio_enable5=`cfg -e | grep AP_RADIO_ENABLED=1 | cut -d"=" -f2`
				check_interface_up5=`ifconfig | grep ath1`
				check_ap_enable2=`cfg -e | grep AP_ENABLE=1 | cut -d"=" -f2`
				check_radio_enable2=`cfg -e | grep AP_RADIO_ENABLED_2=1 | cut -d"=" -f2`
				check_interface_up2=`ifconfig | grep ath0`
				check_interface_iw2=`iwconfig ath0 | grep Access | awk '{print $6}'`
				check_interface_iw5=`iwconfig ath1 | grep Access | awk '{print $6}'`
                        

				if [ "$check_ap_enable2" == "1" ] && [ "$check_radio_enable2" == "1" ] && [ "$check_interface_iw2" == "Not-Associated" ];then				
					echo "ath0 is Not-Associated, restarting WiFi"
					dmcli eRT setv Device.LogAgent.WifiLogMsg string "[RKDB_PLATFORM_ERROR]: ath0 is Not-Associated, restarting WiFi"
					WIFI_RESTART=1
					#dmcli eRT setv Device.X_CISCO_COM_DeviceControl.RebootDevice string Wifi
				fi

				if [ "$check_ap_enable5" == "1" ] && [ "$check_radio_enable5" == "1" ] && [ "$check_interface_iw5" == "Not-Associated" ];then				
					echo "ath1 is Not-Associated, restarting WiFi"
					dmcli eRT setv Device.LogAgent.WifiLogMsg string "[RKDB_PLATFORM_ERROR]: ath1 is Not-Associated, restarting WiFi"
					WIFI_RESTART=1
					#dmcli eRT setv Device.X_CISCO_COM_DeviceControl.RebootDevice string Wifi
				fi


				if [ "$check_ap_enable5" == "1" ] && [ "$check_radio_enable5" == "1" ] && [ "$check_interface_up5" == "" ]; then
					echo "ath1 is down, restarting WiFi"
					dmcli eRT setv Device.LogAgent.WifiLogMsg string "[RKDB_PLATFORM_ERROR]: ath1 is down, restarting WiFi"
					WIFI_RESTART=1
					#dmcli eRT setv Device.X_CISCO_COM_DeviceControl.RebootDevice string Wifi
				fi
		
				if [ "$check_ap_enable2" == "1" ] && [ "$check_radio_enable2" == "1" ] && [ "$check_interface_up2" == "" ]; then
					echo "ath0 is down, restarting WiFi"
					dmcli eRT setv Device.LogAgent.WifiLogMsg string "[RKDB_PLATFORM_ERROR]: ath0 is down, restarting WiFi"
					WIFI_RESTART=1
					#dmcli eRT setv Device.X_CISCO_COM_DeviceControl.RebootDevice string Wifi
				fi

				if [ "$WIFI_RESTART" == "1" ]; then
					dmcli eRT setv Device.X_CISCO_COM_DeviceControl.RebootDevice string Wifi
					sleep 60
				fi
			fi #if [ "$APUP_PID" == "" ]; then
		fi #if [ "$WiFi_PID" == "" ]; then

		# Checking Harvester PID
		Harvester_PID=`pidof harvester`
		if [ "$Harvester_PID" == "" ]; then
			cd /harvester
			echo "Harvester process is not running, restarting it"
			$BINPATH/harvester &
			cd ..
		fi
		
		# Checking lighttpd PID
		LIGHTTPD_PID=`pidof lighttpd`
		if [ "$LIGHTTPD_PID" == "" ]; then
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
								echo "SET succeeded"
								sh /etc/calc_random_time_to_reboot_dev.sh "" &
							elif [ "$rebootNeededforbrlan1" -eq 1 ]
							then
								echo "rebootNeededforbrlan1"
								echo "RDKB_REBOOT : brlan1 interface is not up, rebooting the device."
								echo "Setting last reboot reason"
								dmcli eRT setv Device.DeviceInfo.X_RDKCENTRAL-COM_LastRebootReason string brlan1_down
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


