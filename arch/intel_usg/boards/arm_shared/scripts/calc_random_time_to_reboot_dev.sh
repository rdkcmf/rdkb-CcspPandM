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

calcRandTimetoReboot $1

