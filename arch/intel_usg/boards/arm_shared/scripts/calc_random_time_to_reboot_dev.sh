#!/bin/sh
##################################################################################
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
################################################################################
#
source /etc/utopia/service.d/log_env_var.sh
source /etc/device.properties

SyseventdCrashed="/rdklogs/syseventd_crashed"
calcRandTimetoReboot()
{
    rand_hr=0
    rand_min=0
    rand_sec=0
    process_crashed_is=$1
    # Calculate random min
    rand_min=`awk -v min=0 -v max=59 -v seed="$(date +%N)" 'BEGIN{srand(seed);print int(min+rand()*(max-min+1))}'`

    # Calculate random second
    rand_sec=`awk -v min=0 -v max=59 -v seed="$(date +%N)" 'BEGIN{srand(seed);print int(min+rand()*(max-min+1))}'`
        
    # Calculate random hour
    rand_hr=`awk -v min=0 -v max=0 -v seed="$(date +%N)" 'BEGIN{srand(seed);print int(min+rand()*(max-min+1))}'`

        echo "Time Generated : $rand_hr hr $rand_min min $rand_sec sec"
	
    min_to_sleep=$(($rand_hr*60 + $rand_min))
    sec_to_sleep=$(($min_to_sleep*60 + $rand_sec))
    sleep $sec_to_sleep;
    touch $HAVECRASH
    rm -rf $SyseventdCrashed
     /rdklogger/backupLogs.sh "true" "$process_crashed_is"
}

calcRandTimetoReboot_CBR()
{
    process_crashed_is=$1

    # Extract maintenance window start and end time
    start_time=`dmcli eRT getv Device.DeviceInfo.X_RDKCENTRAL-COM_MaintenanceWindow.FirmwareUpgradeStartTime | grep "value:" | cut -d ":" -f 3 | tr -d ' '`
    end_time=`dmcli eRT getv Device.DeviceInfo.X_RDKCENTRAL-COM_MaintenanceWindow.FirmwareUpgradeEndTime | grep "value:" | cut -d ":" -f 3 | tr -d ' '`

    if [ "$start_time" -eq "$end_time" ]
    then
        echo "Start time can not be equal to end time"
        echo "Resetting values to default"
        dmcli eRT setv Device.DeviceInfo.X_RDKCENTRAL-COM_MaintenanceWindow.FirmwareUpgradeStartTime string "0"
        dmcli eRT setv Device.DeviceInfo.X_RDKCENTRAL-COM_MaintenanceWindow.FirmwareUpgradeEndTime string "10800"
        start_time=0
        end_time=10800
    fi

    echo "Firmware upgrade start time : $start_time"
    echo "Firmware upgrade end time : $end_time"

    if [ "$start_time" -gt "$end_time" ]
    then
        start_time=$(($start_time-86400))
    fi

    #Calculate random value
    random_time=`awk -v min=$start_time -v max=$end_time 'BEGIN{srand(); print int(min+rand()*(max-min+1))}'`

    if [ $random_time -le 0 ]
    then
        random_time=$((random_time+86400))
    fi
    random_time_in_sec=$random_time

    # Calculate random second
    rand_sec=$((random_time%60))

    # Calculate random min
    random_time=$((random_time/60))
    rand_min=$((random_time%60))

    # Calculate random hour
    random_time=$((random_time/60))
    rand_hr=$((random_time%60))

    echo "Time Generated : $rand_hr hr $rand_min min $rand_sec sec"

    # Get current time
    if [ "$UTC_ENABLE" == "true" ]
    then
        cur_hr="10#"`LTime H`
        cur_min="10#"`LTime M`
        cur_sec="10#"`date +"%S"`
    else
        cur_hr="10#"`date +"%H"`
        cur_min="10#"`date +"%M"`
        cur_sec="10#"`date +"%S"`
    fi

    curr_hr_in_sec=$((cur_hr*60*60))
    curr_min_in_sec=$((cur_min*60))
    curr_time_in_sec=$((curr_hr_in_sec+curr_min_in_sec+cur_sec))

    if [ $curr_time_in_sec -le $random_time_in_sec ]
    then
        sec_to_sleep=$((random_time_in_sec-curr_time_in_sec))
    else
        sec_to_12=$((86400-curr_time_in_sec))
        sec_to_sleep=$((sec_to_12+random_time_in_sec))
    fi

    time=$(( `date +%s`+$sec_to_sleep ))
    date_final=`date -d @${time} +"%T"`

    echo "Action on $date_final"

    echo "SLEEPING FOR $sec_to_sleep seconds"

    sleep $sec_to_sleep
    
    echo "Got up after $sec_to_sleep seconds"  

    touch $HAVECRASH
    rm -rf $SyseventdCrashed
    /rdklogger/backupLogs.sh "true" "$process_crashed_is"
}

#TCCBR-2704
if [ "x$BOX_TYPE" == "xTCCBR" ]
then
    calcRandTimetoReboot_CBR $1
else
    calcRandTimetoReboot $1
fi

