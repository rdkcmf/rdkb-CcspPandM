#! /bin/sh
######################################################################################
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

export LD_LIBRARY_PATH=$PWD:.:$PWD/../../lib:$PWD/../../.:/lib:/usr/lib:$LD_LIBRARY_PATH
export DBUS_SYSTEM_BUS_ADDRESS=unix:path=/var/run/dbus/system_bus_socket

source /fss/gw/etc/utopia/service.d/log_env_var.sh
source /lib/rdk/t2Shared_api.sh

SUBSYSLOCATION="/fss/gw/usr/ccsp"
rebootNeeded=0
rebootNeededPAM=0
rebootNeededPSM=0
rebootNeededCR=0
rebootNeededTR69=0
rebootNeededforbrlan1=0
rebootNeededforbrlan0=0

if [ -f $SUBSYSLOCATION/cp_subsys_ert ]; then
	Subsys="eRT."
elif [ -e $SUBSYSLOCATION/cp_subsys_emg ]; then
	Subsys="eMG."
else
	Subsys=""
fi

LIGHTTPD_CONF="/var/lighttpd.conf"

setRebootreason()
{

        echo_t "Setting rebootReason to $1 and rebootCounter to $2"
        if [ -e "/usr/bin/onboarding_log" ]; then
	    /usr/bin/onboarding_log "Device reboot due to reason $1"
	fi
        syscfg set X_RDKCENTRAL-COM_LastRebootReason $1
        result=`echo $?`
        if [ "$result" != "0" ]
        then
            echo_t "SET for Reboot Reason failed"
        fi
        syscfg commit
        result=`echo $?`
        if [ "$result" != "0" ]
        then
            echo_t "Commit for Reboot Reason failed"
        fi

        syscfg set X_RDKCENTRAL-COM_LastRebootCounter $2
        result=`echo $?`
        if [ "$result" != "0" ]
        then
            echo_t "SET for Reboot Counter failed"
        fi
        syscfg commit
        result=`echo $?`
        if [ "$result" != "0" ]
        then
            echo_t "Commit for Reboot Counter failed"
        fi
}

crashed=0
source /etc/utopia/service.d/log_capture_path.sh
loop=1

while [ $loop -eq 1 ]
do
	sleep 300

	cd /fss/gw/usr/ccsp/

	# Checking PandM's PID
	PAM_PID=`pidof CcspPandMSsp`
	if [ "$PAM_PID" = "" ]; then
                if [ -d /tmp/.uploadCoredumps.lock.d ];then
                    while [ ! -f /tmp/crash_reboot ]
		    do
			sleep 2
		    done
                    if [ -f /tmp/crash_reboot ];then rm -rf /tmp/crash_reboot; fi
                fi
		if [ "$rebootNeededPAM" -eq 0 ]; then
		echo_t "RDKB_PROCESS_CRASHED : PAM_process is not running, need to reboot the unit now"
		
		echo_t "Setting Last reboot reason"
		reason="PAM_crash"
		rebootCount=1
		
		setRebootreason $reason $rebootCount
		echo_t "SET succeeded"
		rebootNeededPAM=1
		rebootNeeded=1
    		touch $HAVECRASH
	         /fss/gw/rdklogger/backupLogs.sh "true" "PAM"
		#/fss/gw/rdklogger/backupLogs.sh
		fi
	fi

	# Checking PSM's PID
	PSM_PID=`pidof PsmSsp`
	if [ "$PSM_PID" = "" ]; then
                if [ -d /tmp/.uploadCoredumps.lock.d ];then
                    while [ ! -f /tmp/crash_reboot ]
                    do
                        sleep 2
                    done
                    if [ -f /tmp/crash_reboot ];then rm -rf /tmp/crash_reboot; fi
                fi
		if [ "$rebootNeededPSM" -eq 0 ]; then
		echo_t "RDKB_PROCESS_CRASHED : PSM_process is not running, need to reboot the unit"
		rebootNeededPSM=1
		rebootNeeded=1
		#/fss/gw/rdklogger/backupLogs.sh
		fi
	fi

	# Checking CR's PID
	CR_PID=`pidof CcspCrSsp`
	if [ "$CR_PID" = "" ]; then
                if [ -d /tmp/.uploadCoredumps.lock.d ];then
                    while [ ! -f /tmp/crash_reboot ]
                    do
                        sleep 2
                    done
                    if [ -f /tmp/crash_reboot ];then rm -rf /tmp/crash_reboot; fi
                fi
		if [ "$rebootNeededCR" -eq 0 ]; then
		echo_t "RDKB_PROCESS_CRASHED : CR_process is not running, need to reboot the unit"
		rebootNeeded=1
		rebootNeededCR=1
		#/fss/gw/rdklogger/backupLogs.sh
		fi
	fi

	
	# Checking MTA's PID
	MTA_PID=`pidof CcspMtaAgentSsp`
	if [ "$MTA_PID" = "" ]; then
		echo_t "RDKB_PROCESS_CRASHED : MTA_process is not running, restarting it"
		#cd mta/
		/usr/bin/CcspMtaAgentSsp -subsys $Subsys
		cd ..
	fi

	# Checking CM's PID
	CM_PID=`pidof CcspCMAgentSsp`
	if [ "$CM_PID" = "" ]; then
		echo_t "RDKB_PROCESS_CRASHED : CM_process is not running, restarting it"
		#cd cm/
		/usr/bin/CcspCMAgentSsp -subsys $Subsys
		cd ..
	fi

	# Checking RebootManager's PID
#	Rm_PID=`pidof CcspRmSsp`
#	if [ "$Rm_PID" = "" ]; then
#		echo_t "RDKB_PROCESS_CRASHED : RebootManager_process is not running, restarting it"
		#cd rm/
#		/usr/bin/CcspRmSsp -subsys $Subsys
#	fi

	# Checking TR69's PID
	TR69_PID=`pidof CcspTr069PaSsp`
	if [ "$TR69_PID" = "" ]; then
                if [ -d /tmp/.uploadCoredumps.lock.d ];then
                    while [ ! -f /tmp/crash_reboot ]
                    do
                        sleep 2
                    done
                    if [ -f /tmp/crash_reboot ];then rm -rf /tmp/crash_reboot; fi
                fi
		if [ "$rebootNeededTR69" -eq 0 ]; then
		echo_t "RDKB_PROCESS_CRASHED : TR69_process is not running, need to reboot the unit"
		rebootNeeded=1
		rebootNeededTR69=1
		#/fss/gw/rdklogger/backupLogs.sh
		fi
	fi

	# Checking Test adn Daignostic's PID
	TandD_PID=`pidof CcspTandDSsp`
	if [ "$TandD_PID" = "" ]; then
		echo_t "RDKB_PROCESS_CRASHED : TandD_process is not running, restarting it"
		#cd tad/
		/usr/bin/CcspTandDSsp -subsys $Subsys
		cd ..
	fi

	# Checking Lan Manager PID
	LM_PID=`pidof CcspLMLite`
	if [ "$LM_PID" = "" ]; then
		echo_t "RDKB_PROCESS_CRASHED : LanManager_process is not running, restarting it"
        	t2CountNotify "SYS_SH_LM_restart"
		#cd lm/
		/usr/bin/CcspLMLite &
		cd ..
	
	fi

	# Checking snmp subagent PID
	if [ -f "/etc/SNMP_PA_ENABLE" ]; then
		SNMP_PID=`pidof snmp_subagent`
		if [ "$SNMP_PID" = "" ]; then
			echo_t "RDKB_PROCESS_CRASHED : snmpsubagent_process is not running, restarting it"
			t2CountNotify "SYS_INFO_snmpsubagent_restart"
			cd snmp/
			sh run_subagent.sh /var/tmp/cm_snmp_ma &
			cd ..
		fi
	fi
	# Checking Home Security's PID
	HOMESEC_PID=`pidof CcspHomeSecurity`
	if [ "$HOMESEC_PID" = "" ]; then
		echo_t "RDKB_PROCESS_CRASHED : HomeSecurity_process is not running, restarting it"
		CcspHomeSecurity 8081&
	fi

        # Checking dropbear PID
	DROPBEAR_PID=`pidof dropbear`
	if [ "$DROPBEAR_PID" = "" ]; then
		echo_t "RDKB_PROCESS_CRASHED : dropbear_process is not running, restarting it"
        	t2CountNotify "SYS_SH_DhcpArp_restart"
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
			echo_t "RDKB_PROCESS_CRASHED : HotSpot_process is not running, restarting it"
        		hotspotfd $keepalive_args  > /dev/null &
   		fi    
   		DHCP_SNOOPERD_PID=`pidof dhcp_snooperd`
   		if [ "$DHCP_SNOOPERD_PID" = "" ]; then
			echo_t "RDKB_PROCESS_CRASHED : DhcpSnooper_process is not running, restarting it"
			t2CountNotify "SYS_SH_DhcpSnooper_restart"
        		dhcp_snooperd -q $BASEQUEUE -n 2 -e 1  > /dev/null &
   		fi 
		DHCP_ARP_PID=`pidof hotspot_arpd`
   		if [ "$DHCP_ARP_PID" = "" ]; then
			echo_t "RDKB_PROCESS_CRASHED : DhcpArp_process is not running, restarting it"
        		t2CountNotify "SYS_SH_DhcpArp_restart"
			hotspot_arpd -q 0  > /dev/null &
   		fi
	fi

	
	# Checking webpa PID
	WEBPA_PID=`pidof webpa`
	if [ "$WEBPA_PID" = "" ]; then
                if [ -d /tmp/.uploadCoredumps.lock.d ];then
                    while [ ! -f /tmp/crash_reboot ]
                    do
                        sleep 2
                    done
                    if [ -f /tmp/crash_reboot ];then rm -rf /tmp/crash_reboot; fi
                fi
		ENABLEWEBPA=`cat /nvram/webpa_cfg.json | grep -r EnablePa | awk '{print $2}' | sed 's|[\"\",]||g'`
		echo_t "ENABLEWEBPA is $ENABLEWEBPA"
		if [ "$ENABLEWEBPA" = "true" ];then
		echo_t "RDKB_PROCESS_CRASHED : WebPA_process is not running, trying to restart it"
		t2CountNotify "SYS_SH_WebPA_restart"
		#We'll set the reason only if webpa reconnect is not due to DNS resolve
		syscfg get X_RDKCENTRAL-COM_LastReconnectReason | grep "Dns_Res_webpa_reconnect"
		if [ $? != 0 ]; then
			
			echo_t "setting reconnect reason from process_monitor.sh"
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
			
			#cd webpa/
			/usr/bin/webpa -subsys $Subsys
			cd ..
		else
			echo_t "EnablePa is false in config file. Hence not initializng WebPA.."
		fi
	
	fi

	# Checking lighttpd PID
	LIGHTTPD_PID=`pidof lighttpd`
	if [ "$LIGHTTPD_PID" = "" ]; then
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
			echo_t "Setting Last reboot reason"
			dmcli eRT setv Device.DeviceInfo.X_RDKCENTRAL-COM_LastRebootReason string brlan0_down
			echo_t "SET succeeded"
			rebootNeededforbrlan0=1
			rebootNeeded=1
	      		/fss/gw/rdklogger/backupLogs.sh "true" ""
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

	#cd /
	if [ "$rebootNeeded" -eq 1 ]
	then
		cur_hr=`date +"%H"`
       		cur_min=`date +"%M"`
		if [ $cur_hr -ge 02 ] && [ $cur_hr -le 03 ]
		then
  	    		if [ $cur_hr -eq 03 ] && [ $cur_min -ne 00 ]
	     		then
		   			echo_t "Maintanance window for the current day is over , unit will be rebooted in next Maintanance window "
	     		else
				#Check if we have already flagged reboot is needed
				if [ ! -e $FLAG_REBOOT ]
				then
					if [ "$rebootNeededPSM" -eq 1 ]
					then
						echo_t "rebootNeededPSM"
							echo_t "Setting last reboot reason"
						    dmcli eRT setv Device.DeviceInfo.X_RDKCENTRAL-COM_LastRebootReason string Psm_crash
						    t2CountNotify "SYS_ERROR_PSMCrash_reboot"
						    echo_t "SET succeeded"	
			                	sh /etc/calc_random_time_to_reboot_dev.sh "PSM" &

					elif [ "$rebootNeededCR" -eq 1 ]
					then
						echo_t "rebootNeededCR"
						  echo_t "Setting Last reboot reason"
		                    reason="CR_crash"
		                    rebootCount=1
		                    setRebootreason $reason $rebootCount
		                    echo_t "SET succeeded"
			                	sh /etc/calc_random_time_to_reboot_dev.sh "CR" &

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

					else 
						echo_t "rebootNeededTR69"
								echo_t "Setting last reboot reason"
								dmcli eRT setv Device.DeviceInfo.X_RDKCENTRAL-COM_LastRebootReason string Tr069_crash
								echo_t "SET succeeded"
			                	sh /etc/calc_random_time_to_reboot_dev.sh "TR69" &
					fi
					touch $FLAG_REBOOT
#				fi
				else
					echo_t "Already waiting for reboot"
				fi					
	     		fi
   		fi

	fi
done
