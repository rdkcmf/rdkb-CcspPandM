#! /bin/sh
#######################################################################################
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

if [ -f /etc/device.properties ]
then
    source /etc/device.properties
fi

BINPATH="/usr/bin"

source /lib/rdk/t2Shared_api.sh

SELFHEAL_ENABLE=`syscfg get selfheal_enable`
if [ "$SELFHEAL_ENABLE" == "false" ]
then
	export LD_LIBRARY_PATH=$PWD:.:$PWD/../../lib:$PWD/../../.:/lib:/usr/lib:$LD_LIBRARY_PATH
	export DBUS_SYSTEM_BUS_ADDRESS=unix:path=/var/run/dbus/system_bus_socket

	source /etc/utopia/service.d/log_env_var.sh
	batteryMode=0
	rebootNeeded=0
	rebootNeededforbrlan1=0
	rebootNeededforbrlan0=0
	LIGHTTPD_CONF="/var/lighttpd.conf"

	crashed=0
	
	source /etc/utopia/service.d/log_capture_path.sh
	loop=1
	Subsys="eRT."
	time=0
	newline="
"
	while [ $loop -eq 1 ]
	do
		sleep 300
		
		if [ -f  /usr/bin/Selfhealutil ]
		then
			Selfhealutil power_mode
			batteryMode=$?
			echo "[RKDB_PLATFORM] : batteryMode is  $batteryMode"
		fi	
     	   
		if [ $batteryMode = 0 ]
		then

			cd /usr/ccsp/

			# Checking snmp subagent PID
			if [ -f "/etc/SNMP_PA_ENABLE" ]; then
				SNMP_PID=`pidof snmp_subagent`
				if [ "$SNMP_PID" == "" ]; then
					echo_t "RDKB_PROCESS_CRASHED : snmpsubagent_process is not running, restarting it"
					t2CountNotify "SYS_INFO_snmpsubagent_restart"
					cd snmp/
					sh run_subagent.sh $CCSP_SNMP_AGENT_COM &
					cd ..
				fi
			fi
			# Checking Home Security's PID
			HOMESEC_PID=`pidof CcspHomeSecurity`
			if [ "$HOMESEC_PID" == "" ]; then
				echo_t "RDKB_PROCESS_CRASHED : HomeSecurity_process is not running, restarting it"
				$BINPATH/CcspHomeSecurity 8081&
			fi

			# Checking dropbear PID
			DROPBEAR_PID=`pidof dropbear`
			if [ "$DROPBEAR_PID" == "" ]; then
				echo_t "RDKB_PROCESS_CRASHED : dropbear_process is not running, restarting it"
        			t2CountNotify "SYS_SH_DhcpArp_restart"
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
				echo_t "ENABLEWEBPA is $ENABLEWEBPA"
				if [ "$ENABLEWEBPA" == "true" ];then
				echo_t "RDKB_PROCESS_CRASHED : WebPA_process is not running, trying to restart it"
				t2CountNotify "SYS_SH_WebPA_restart"
				#We'll set the reason only if webpa reconnect is not due to DNS resolve
				syscfg get X_RDKCENTRAL-COM_LastReconnectReason | grep "Dns_Res_webpa_reconnect"
				if [ $? != 0 ]; then
					echo_t "setting reconnect reason from sysd_process_monitor.sh"
					syscfg set X_RDKCENTRAL-COM_LastReconnectReason WebPa_crash
					result=`echo $?`
					if [ "$result" != "0" ]
					then
						echo_t "SET for Reconnect Reason failed"
					fi
					syscfg commit
					result=`echo $?`
					if [ "$result" != "0" ]
					then
						echo_t "Commit for Reconnect Reason failed"
					fi
				fi
					$BINPATH/webpa -subsys $Subsys
				else
					echo_t "EnablePa is false in config file. Hence not initializng WebPA.."
				fi

			fi

			# Checking WiFi Agent PID
			WiFi_PID=`pidof CcspWifiSsp`
			if [ "$WiFi_PID" == "" ]; then
				echo_t "WiFi process is not running, restarting it"
				cd /wifi
				export LD_LIBRARY_PATH=$PWD:.:$PWD/../../lib:$PWD/../../.:/lib:/usr/lib:$LD_LIBRARY_PATH
				dmcli eRT setv Device.LogAgent.WifiLogMsg string "RDK_LOG_ERROR,RDKB_PROCESS_CRASHED : WiFiAgent_process is not running, need restart $newline"
				$BINPATH/CcspWifiSsp -subsys $Subsys &
				cd ..
			fi #if [ "$WiFi_PID" == "" ]; then

			# Checking lighttpd PID
			LIGHTTPD_PID=`pidof lighttpd`
			if [ "$LIGHTTPD_PID" == "" ]; then
				echo_t "RDKB_PROCESS_CRASHED : lighttpd is not running, restarting it"
				lighttpd -f $LIGHTTPD_CONF
			fi

			ifconfig | grep brlan1
			if [ $? == 1 ]; then
				if [ "$rebootNeededforbrlan1" -eq 0 ]; then
					echo_t "[RKDB_PLATFORM_ERROR] : brlan1 interface is not up, need to reboot the unit." 
					rebootNeededforbrlan1=1
					rebootNeeded=1
				fi
			fi

			ifconfig | grep brlan0
			if [ $? == 1 ]; then
				if [ "$rebootNeededforbrlan0" -eq 0 ]; then
					echo_t "[RKDB_PLATFORM_ERROR] : brlan0 interface is not up" 
					echo_t "RDKB_REBOOT : brlan0 interface is not up, rebooting the device."
					if [ -e "/usr/bin/onboarding_log" ]; then
					    /usr/bin/onboarding_log "RDKB_REBOOT : brlan0 interface is not up, rebooting the device."
					fi
					rebootNeededforbrlan0=1
					rebootNeeded=1
					/rdklogger/backupLogs.sh "true" ""
				fi
			fi

			dmcli eRT getv Device.WiFi.SSID.2.Status | grep Up
			if [ $? == 1 ]; then
				echo_t "[RKDB_PLATFORM_ERROR] : 5G private SSID (ath1) is off, resetting WiFi now"
				t2CountNotify "WIFI_SH_5G_wifi_reset"
				dmcli eRT setv Device.X_CISCO_COM_DeviceControl.RebootDevice string Wifi
			fi

			iptables-save -t nat | grep "A PREROUTING -i"
			if [ $? == 1 ]; then
				echo_t "[RDKB_PLATFORM_ERROR] : iptable corrupted. Restarting firewall"
				sysevent set firewall-restart
			fi

			if [ "$rebootNeeded" -eq 1 ]
			then
				cur_hr=`LTime H`
				cur_min=`LTime M`
				if [ $cur_hr -ge 02 ] && [ $cur_hr -le 03 ]
				then
						if [ $cur_hr -eq 03 ] && [ $cur_min -ne 00 ]
						then
							echo_t "Maintanance window for the current day is over , unit will be rebooted in next Maintanance window "
						else
							#Check if we have already flagged reboot is needed
							if [ ! -e $FLAG_REBOOT ]
							then
								if [ "$rebootNeededforbrlan0" -eq 1 ]
								then
									echo_t "rebootNeededforbrlan0"
									echo_t "RDKB_REBOOT : brlan0 interface is not up, rebooting the device."
									if [ -e "/usr/bin/onboarding_log" ];then
									    /usr/bin/onboarding_log "RDKB_REBOOT : brlan0 interface is not up, rebooting the device."
									fi
									echo_t "Setting last reboot reason"
									dmcli eRT setv Device.DeviceInfo.X_RDKCENTRAL-COM_LastRebootReason string brlan0_down
									echo_t "SET succeeded"
									sh /etc/calc_random_time_to_reboot_dev.sh "" &
								elif [ "$rebootNeededforbrlan1" -eq 1 ]
								then
									echo_t "rebootNeededforbrlan1"
									echo_t "RDKB_REBOOT : brlan1 interface is not up, rebooting the device."
									if [ -e "/usr/bin/onboarding_log" ]; then
									    /usr/bin/onboarding_log "RDKB_REBOOT : brlan1 interface is not up, rebooting the device."
									fi
									echo_t "Setting last reboot reason"
									dmcli eRT setv Device.DeviceInfo.X_RDKCENTRAL-COM_LastRebootReason string brlan1_down
									echo_t "SET succeeded"
									sh /etc/calc_random_time_to_reboot_dev.sh "" &
								fi
								touch $FLAG_REBOOT
							else
								echo_t "Already waiting for reboot"
							fi					
						fi
				fi
			fi #if [ "$rebootNeeded" -eq 1 ]
		fi #if [ $batteryMode = 0 ]
	done
fi


