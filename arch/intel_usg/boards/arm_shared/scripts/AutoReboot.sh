#!/bin/sh
##########################################################################
# If not stated otherwise in this file or this component's LICENSE
# file the following copyright and licenses apply:
#
# Copyright 2021 RDK Management
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
##########################################################################


source /etc/utopia/service.d/log_capture_path.sh
source /etc/utopia/service.d/log_env_var.sh
source /etc/RebootCondition.sh

BIN_PATH=/usr/bin
FW_START="/nvram/.FirmwareUpgradeStartTime"
FW_END="/nvram/.FirmwareUpgradeEndTime"
REBOOT_WAIT="/tmp/.waitingreboot"
reb_window=0
DOWNLOAD_INPROGRESS="/tmp/.downloadingfw"
deferReboot="/tmp/.deferringreboot"
ABORT_REBOOT="/tmp/AbortReboot"
abortReboot_count=0


calcRandTime()
{
    ExtractcurrentTime
    if [ "$start_time" -gt "$end_time" ]
    then
        start_time=$(($start_time-86400))
    fi
 
    random_time=`awk -v min=$start_time -v max=$end_time 'BEGIN{srand(); print int(min+rand()*(max-min+1))}'`

    if [ $random_time -le 0 ]
    then
        random_time=$((random_time+86400))
    fi
    random_time_in_sec=$random_time

    if [ $curr_time_in_sec -le $random_time_in_sec ]
    then
        sec_to_sleep=$((random_time_in_sec-curr_time_in_sec))
    else
        sec_to_12=$((86400-curr_time_in_sec))
        sec_to_sleep=$((sec_to_12+random_time_in_sec))
    fi

    time=$(( `date +%s`+$sec_to_sleep ))
    date_final=`date -d @${time} +"%T"`

    echo_t "Action on $date_final" 

    echo_t "AutoReboot : SLEEPING FOR  $sec_to_sleep seconds"
    sleep $sec_to_sleep
    echo_t "AutoReboot : got up after $sec_to_sleep seconds"
}

# Check if it is still in maintenance window
checkMaintenanceWindow()
{
    ExtractcurrentTime
    reb_window=0
    if [ $start_time -lt $end_time ] && [ $curr_time_in_sec -ge $start_time ] && [ $curr_time_in_sec -lt $end_time ]
    then
        reb_window=1
    elif [ $start_time -gt $end_time ] && [[ $curr_time_in_sec -lt $end_time || $curr_time_in_sec -ge $start_time ]]
    then
        reb_window=1
    else
        reb_window=0
    fi
}

reboot_device_success=0
AutoReboot=false
AutoReboot=`dmcli eRT getv Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.AutoReboot.Enable | grep value | cut -f3 -d : | cut -f2 -d" "`
ConfiguredDays=`dmcli eRT getv Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.AutoReboot.UpTime | grep value | cut -f3 -d : | cut -f2 -d" "`


while [ $reboot_device_success -eq 0 ]; do 
    if [ "$AutoReboot" == "true" ];then
        uptime=$(cut -d. -f1 /proc/uptime)
        uptime_days=$((uptime/86400))  
        # Check if still within reboot window
        if [ $uptime_days -lt $ConfiguredDays ]
        then
            echo_t "Auto Reboot will try in next Maintienance window as uptime: $uptime_days is lesss than configured: $ConfiguredDays"
            http_reboot_ready_stat=1
            calcRandTime
        else
            checkMaintenanceWindow

            if [ $reb_window -eq 1 ]; then
                echo_t "Auto Reboot in current Maintienance window"
                reboot_now=1
            else
                echo_t "Not within current maintenance window for reboot.Rebooting in the next window"            
                reboot_now=0
            fi

            # If we are not supposed to reboot now, calculate random time
            # to reboot in next maintenance window 
            if [ $reboot_now -eq 0 ];then
                calcRandTime
            fi    

            # Check the Reboot status
            # Continously check reboot status every 10 seconds  
            # till the end of the maintenace window until the reboot status is OK
            $BIN_PATH/XconfHttpDl http_reboot_status
            http_reboot_ready_stat=$?

            while [ $http_reboot_ready_stat -eq 1 ]
            do     
                sleep 10
                checkMaintenanceWindow

                if [ $reb_window -eq 1 ]
                then
                    #We're still within the reboot window 
                    $BIN_PATH/XconfHttpDl http_reboot_status
                    http_reboot_ready_stat=$?

                else
                    #If we're out of the reboot window, exit while loop
                    break
                fi
            done
                 
        fi 
  
    
        # The reboot ready status changed to OK within the maintenance window,proceed
        if [ $http_reboot_ready_stat -eq 0 ];then
            if [ $abortReboot_count -lt 5 ];then
                #Wait for Notification to propogate
                deferfw=`dmcli eRT getv Device.DeviceInfo.X_RDKCENTRAL-COM_xOpsDeviceMgmt.RPC.DeferFWDownloadReboot | grep value | cut -d ":" -f 3 | tr -d ' ' `
                echo_t "AutoReboot : Sleeping for $deferfw seconds before reboot"
                touch $deferReboot 
                dmcli eRT setv Device.DeviceInfo.X_RDKCENTRAL-COM_xOpsDeviceMgmt.RPC.RebootPendingNotification uint $deferfw
                sleep $deferfw
            else
                echo_t "AutoReboot: Abort Count reached maximum limit $abortReboot_count"
            fi
            if [ ! -e "$ABORT_REBOOT" ]
            then    
                if [ -f $REBOOT_WAIT ]
                then
                    echo_t "Waiting for sowftware upgrade reboot,so no need to do auto reboot"
                    exit
                else
                    echo_t "setting LastRebootReason as AutoReboot"
                    dmcli eRT setv Device.DeviceInfo.X_RDKCENTRAL-COM_LastRebootReason string AutoReboot
                    reboot_device_success=1
                    echo_t "AutoReboot take log back up and reboot"
                    sh /rdklogger/backupLogs.sh &
                fi
            else
                reboot_device_success=0
                echo_t "AutoReboot aborted by the user"
                abortReboot_count=$((abortReboot_count+1))
                echo_t "AutoReboot : Abort Count is  $abortReboot_count"
                rm -rf $ABORT_REBOOT
                rm -rf $deferReboot
                while [ 1 ]
                do
                    checkMaintenanceWindow

                    if [ $reb_window -eq 1 ]
                    then
                    #We're still within the maintenance window, sleeping for 2 hr to come out of maintenance window
                    sleep 7200
                   else
                   #If we're out of the maintenance window, exit while loop
                    break
                    fi
                done
            fi

        # The reboot ready status didn't change to OK within the maintenance window 
        else
            reboot_device_success=0
            # Goto start of Reboot Manager again  
     
        fi
    else
        Removecron
        exit
     fi               
done # While loop for reboot manager
