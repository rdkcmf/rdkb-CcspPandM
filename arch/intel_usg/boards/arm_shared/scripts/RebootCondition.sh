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


FW_START="/nvram/.FirmwareUpgradeStartTime"
FW_END="/nvram/.FirmwareUpgradeEndTime"

ExtractMaintenanceTime()
{
 # Extract maintenance window start and end time
    if [ -f "$FW_START" ] && [ -f "$FW_END" ]
    then
      start_time=`cat $FW_START`
      end_time=`cat $FW_END`
    else
           start_time=3600
           end_time=14400
    fi

    if [ "$start_time" = "$end_time" ]
    then
        start_time=3600
        end_time=14400
        echo "$start_time" > $FW_START
        echo "$end_time" > $FW_END
    fi
}


ExtractcurrentTime()
{
    ExtractMaintenanceTime
    echo_t "RebootCondition : Firmware upgrade start time : $start_time"
    echo_t "RebootCondition : Firmware upgrade end time : $end_time"
    # Get current time
    if [ "$UTC_ENABLE" == "true" ]
    then
        cur_hr=`LTime H | sed 's/^0*//'`
        cur_min=`LTime M | sed 's/^0*//'`
        cur_sec=`date +"%S" | sed 's/^0*//'`
    else
        cur_hr=`date +"%H" | sed 's/^0*//'`
        cur_min=`date +"%M" | sed 's/^0*//'`
        cur_sec=`date +"%S" | sed 's/^0*//'`
    fi
    echo_t "RebootCondition : Current Local Time: $cur_hr hr $cur_min min $cur_sec sec"

    curr_hr_in_sec=$((cur_hr*60*60))
    curr_min_in_sec=$((cur_min*60))
    curr_time_in_sec=$((curr_hr_in_sec+curr_min_in_sec+cur_sec))
    echo_t "RebootCondition: Current Time in secs: $curr_time_in_sec sec"
}

Removecron()
{
    echo_t "AutoReboot parameter is set to false. Removing the AutoReboot crontab"
    CRONTAB_DIR="/var/spool/cron/crontabs/"
    CRONFILE_BK="/tmp/cron_tab$$.txt"
    crontab -l -c $CRONTAB_DIR > $CRONFILE_BK
    sed -i "/AutoReboot.sh/d" $CRONFILE_BK
    crontab $CRONFILE_BK -c $CRONTAB_DIR
    rm -rf $CRONFILE_BK
}
