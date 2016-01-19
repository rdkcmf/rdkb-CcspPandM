#!/bin/sh

source /fss/gw/etc/utopia/service.d/log_env_var.sh

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
     /fss/gw/rdklogger/backupLogs.sh "true" "$process_crashed_is"
}

calcRandTimetoReboot $1

