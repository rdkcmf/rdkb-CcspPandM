#! /bin/sh

export LD_LIBRARY_PATH=$PWD:.:$PWD/../../lib:$PWD/../../.:/lib:/usr/lib:$LD_LIBRARY_PATH
export DBUS_SYSTEM_BUS_ADDRESS=unix:path=/var/run/dbus/system_bus_socket

SUBSYSLOCATION="/fss/gw/usr/ccsp"
rebootNeeded=0
FLAG_REBOOT="/var/tmp/waitingreboot"

if [ -f $SUBSYSLOCATION/cp_subsys_ert ]; then
	Subsys="eRT."
elif [ -e $SUBSYSLOCATION/cp_subsys_emg ]; then
	Subsys="eMG."
else
	Subsys=""
fi

echo "Entering newWhile $Subsys"
crashed=0
source /etc/utopia/service.d/log_capture_path.sh
loop=1
countForMem=0
while [ $loop -eq 1 ]
do
	sleep 300
    countForMem=$((countForMem+1))
	cd /fss/gw/usr/ccsp/

	# Checking PandM's PID
	PAM_PID=`pidof CcspPandMSsp`
	if [ "$PAM_PID" = "" ]; then
		echo "RDKB_PROCESS_CRASHED : PAM_process is not running, need to reboot the unit"
		rebootNeeded=1
		#/fss/gw/rdklogger/backupLogs.sh
	fi

	# Checking PSM's PID
	PSM_PID=`pidof PsmSsp`
	if [ "$PSM_PID" = "" ]; then
		echo "RDKB_PROCESS_CRASHED : PSM_process is not running, need to reboot the unit"
		rebootNeeded=1
		#/fss/gw/rdklogger/backupLogs.sh
	fi

	# Checking CR's PID
	CR_PID=`pidof CcspCrSsp`
	if [ "$CR_PID" = "" ]; then
		echo "RDKB_PROCESS_CRASHED : CR_process is not running, need to reboot the unit"
		rebootNeeded=1
		#/fss/gw/rdklogger/backupLogs.sh
	fi

	
	# Checking MTA's PID
	MTA_PID=`pidof CcspMtaAgentSsp`
	if [ "$MTA_PID" = "" ]; then
		echo "RDKB_PROCESS_CRASHED : MTA_process is not running, restarting it"
		cd mta/
		./CcspMtaAgentSsp -subsys $Subsys
		cd ..
	fi

	# Checking CM's PID
	CM_PID=`pidof CcspCMAgentSsp`
	if [ "$CM_PID" = "" ]; then
		echo "RDKB_PROCESS_CRASHED : CM_process is not running, restarting it"
		cd cm/
		./CcspCMAgentSsp -subsys $Subsys
		cd ..
	fi

	# Checking WEBController's PID
	WEBC_PID=`pidof CcspWecbController`
	if [ "$WEBC_PID" = "" ]; then
		echo "RDKB_PROCESS_CRASHED : WECBController_process is not running, restarting it"
		cd wecb/
		./CcspWecbController -subsys $Subsys
		cd ..
	fi

	# Checking RebootManager's PID
	Rm_PID=`pidof CcspRmSsp`
	if [ "$Rm_PID" = "" ]; then
		echo "RDKB_PROCESS_CRASHED : RebootManager_process is not running, restarting it"
		cd rm/
		./CcspRmSsp -subsys $Subsys
	fi

	# Checking TR69's PID
	TR69_PID=`pidof CcspTr069PaSsp`
	if [ "$TR69_PID" = "" ]; then
		echo "RDKB_PROCESS_CRASHED : TR69_process is not running, need to reboot the unit"
		rebootNeeded=1
		#/fss/gw/rdklogger/backupLogs.sh
	fi

	# Checking Test adn Daignostic's PID
	TandD_PID=`pidof CcspTandDSsp`
	if [ "$TandD_PID" = "" ]; then
		echo "RDKB_PROCESS_CRASHED : TandD_process is not running, restarting it"
		cd tad/
		./CcspTandDSsp -subsys $Subsys
		cd ..
	fi

	# Checking Lan Manager PID
	LM_PID=`pidof CcspLMLite`
	if [ "$LM_PID" = "" ]; then
		echo "RDKB_PROCESS_CRASHED : LanManager_process is not running, restarting it"
		cd lm/
		./CcspLMLite &
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

	#hotspotfd -n erouter0 -e 1
	#hotspot_arpd -q 0
	#dhcp_snooperd -q 1 -n 2 -e 1
	HOTSPOT_ENABLE=`dmcli eRT getv Device.DeviceInfo.X_COMCAST_COM_xfinitywifiEnable | grep value | cut -f3 -d : | cut -f2 -d" "`
	if [ $HOTSPOT_ENABLE = "true" ]
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
			cd webpa/
			./webpa -subsys $Subsys
			cd ..
		else
			echo "EnablePa is false in config file. Hence not initializng WebPA.."
		fi
	
	fi

	if [ $countForMem -eq 12 ]
    then
       	countForMem=0

		#Checking for Total in system
		#TOTALMEM=`free | awk 'FNR == 2 {print $2}'`
		TOTALMEM=`dmcli eRT getv Device.DeviceInfo.MemoryStatus.Total | grep value | cut -f3  -d":" | cut -f2 -d" "`
		echo "RDKB_SYS_MEM_INFO_HAL : Total memory in system is $TOTALMEM MB"

		#Checking for memory used in system
		#USEDMEM=`free | awk 'FNR == 2 {print $3}'`
		USEDMEM=`dmcli eRT getv Device.DeviceInfo.MemoryStatus.Used | grep value | cut -f3  -d":" | cut -f2 -d" "`
		echo "RDKB_SYS_MEM_INFO_HAL : Used memory in system is $USEDMEM MB"

		#Checking for memory availble in system
		#FREEMEM=`free | awk 'FNR == 2 {print $4}'`
		FREEMEM=`dmcli eRT getv Device.DeviceInfo.MemoryStatus.Free | grep value | cut -f3  -d":" | cut -f2 -d" "`
		echo "RDKB_SYS_MEM_INFO_HAL : Free memory in system is $FREEMEM MB"

		totalMemSys=`free | awk 'FNR == 2 {print $2}'`
		usedMemSys=`free | awk 'FNR == 2 {print $3}'`
		freeMemSys=`free | awk 'FNR == 2 {print $3}'`
		echo "RDKB_SYS_MEM_INFO_SYS : Total memory in system is $totalMemSys"
		echo "RDKB_SYS_MEM_INFO_SYS : Used memory in system is $usedMemSys"
		echo "RDKB_SYS_MEM_INFO_SYS : Free memory in system is $freeMemSys"
		
		
   fi

	#cd /
	if [ $rebootNeeded -eq 1 ]
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
                	sh /etc/calc_random_time_to_reboot_dev.sh &
					touch $FLAG_REBOOT
#				fi
				else
					echo "Already waiting for reboot"
				fi					
	     		fi
   		fi

	fi
done
