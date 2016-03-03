#! /bin/sh

export LD_LIBRARY_PATH=$PWD:.:$PWD/../../lib:$PWD/../../.:/lib:/usr/lib:$LD_LIBRARY_PATH
export DBUS_SYSTEM_BUS_ADDRESS=unix:path=/var/run/dbus/system_bus_socket

source /fss/gw/etc/utopia/service.d/log_env_var.sh

SUBSYSLOCATION="/fss/gw/usr/ccsp"
rebootNeeded=0
rebootNeededPAM=0
rebootNeededPSM=0
rebootNeededCR=0
rebootNeededTR69=0

if [ -f $SUBSYSLOCATION/cp_subsys_ert ]; then
	Subsys="eRT."
elif [ -e $SUBSYSLOCATION/cp_subsys_emg ]; then
	Subsys="eMG."
else
	Subsys=""
fi

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
		if [ "$rebootNeededPAM" -eq 0 ]; then
		echo "RDKB_PROCESS_CRASHED : PAM_process is not running, need to reboot the unit now"
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
		if [ "$rebootNeededPSM" -eq 0 ]; then
		echo "RDKB_PROCESS_CRASHED : PSM_process is not running, need to reboot the unit"
		rebootNeededPSM=1
		rebootNeeded=1
		#/fss/gw/rdklogger/backupLogs.sh
		fi
	fi

	# Checking CR's PID
	CR_PID=`pidof CcspCrSsp`
	if [ "$CR_PID" = "" ]; then
		if [ "$rebootNeededCR" -eq 0 ]; then
		echo "RDKB_PROCESS_CRASHED : CR_process is not running, need to reboot the unit"
		rebootNeeded=1
		rebootNeededCR=1
		#/fss/gw/rdklogger/backupLogs.sh
		fi
	fi

	
	# Checking MTA's PID
	MTA_PID=`pidof CcspMtaAgentSsp`
	if [ "$MTA_PID" = "" ]; then
		echo "RDKB_PROCESS_CRASHED : MTA_process is not running, restarting it"
		#cd mta/
		/usr/bin/CcspMtaAgentSsp -subsys $Subsys
		cd ..
	fi

	# Checking CM's PID
	CM_PID=`pidof CcspCMAgentSsp`
	if [ "$CM_PID" = "" ]; then
		echo "RDKB_PROCESS_CRASHED : CM_process is not running, restarting it"
		#cd cm/
		/usr/bin/CcspCMAgentSsp -subsys $Subsys
		cd ..
	fi

	# Checking WEBController's PID
	WEBC_PID=`pidof CcspWecbController`
	if [ "$WEBC_PID" = "" ]; then
		echo "RDKB_PROCESS_CRASHED : WECBController_process is not running, restarting it"
		#cd wecb/
		/usr/bin/CcspWecbController -subsys $Subsys
		cd ..
	fi

	# Checking RebootManager's PID
#	Rm_PID=`pidof CcspRmSsp`
#	if [ "$Rm_PID" = "" ]; then
#		echo "RDKB_PROCESS_CRASHED : RebootManager_process is not running, restarting it"
		#cd rm/
#		/usr/bin/CcspRmSsp -subsys $Subsys
#	fi

	# Checking TR69's PID
	TR69_PID=`pidof CcspTr069PaSsp`
	if [ "$TR69_PID" = "" ]; then
		if [ "$rebootNeededTR69" -eq 0 ]; then
		echo "RDKB_PROCESS_CRASHED : TR69_process is not running, need to reboot the unit"
		rebootNeeded=1
		rebootNeededTR69=1
		#/fss/gw/rdklogger/backupLogs.sh
		fi
	fi

	# Checking Test adn Daignostic's PID
	TandD_PID=`pidof CcspTandDSsp`
	if [ "$TandD_PID" = "" ]; then
		echo "RDKB_PROCESS_CRASHED : TandD_process is not running, restarting it"
		#cd tad/
		/usr/bin/CcspTandDSsp -subsys $Subsys
		cd ..
	fi

	# Checking Lan Manager PID
	LM_PID=`pidof CcspLMLite`
	if [ "$LM_PID" = "" ]; then
		echo "RDKB_PROCESS_CRASHED : LanManager_process is not running, restarting it"
		#cd lm/
		/usr/bin/CcspLMLite &
		cd ..
	
	fi

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

	#cd /
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
					if [ "$rebootNeededPSM" -eq 1 ]
					then
						echo "rebootNeededPSM"
			                	sh /etc/calc_random_time_to_reboot_dev.sh "PSM" &

					elif [ "$rebootNeededCR" -eq 1 ]
					then
						echo "rebootNeededCR"
			                	sh /etc/calc_random_time_to_reboot_dev.sh "CR" &
					else 
						echo "rebootNeededTR69"
			                	sh /etc/calc_random_time_to_reboot_dev.sh "TR69" &
					fi
					touch $FLAG_REBOOT
#				fi
				else
					echo "Already waiting for reboot"
				fi					
	     		fi
   		fi

	fi
done
