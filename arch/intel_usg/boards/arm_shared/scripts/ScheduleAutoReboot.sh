#! /bin/sh
source /etc/log_timestamp.sh
source /etc/utopia/service.d/log_capture_path.sh
source /etc/utopia/service.d/log_env_var.sh
CRONTAB_DIR="/var/spool/cron/crontabs/"
CRONTAB_FILE=$CRONTAB_DIR"root"
CRONFILE_BK="/tmp/cron_tab$$.txt"
FW_START="/nvram/.FirmwareUpgradeStartTime"
FW_END="/nvram/.FirmwareUpgradeEndTime"
source /etc/RebootCondition.sh

FILE_LOCK="/tmp/AutoReboot.lock"
MAX_RETRY_COUNT=10
count=0

#Only one process should schedule cron at a time
while : ; do
    if [ $count -lt $MAX_RETRY_COUNT ]; then
        if [ -f $FILE_LOCK ]; then
            echo_t "[ScheduleAutoReboot.sh]:another instance is running"
            sleep 1;
            count=$((count+1))
            echo_t "Retry count = $count"
            continue;
        else
            # Creating lock to allow one process at a time
            touch $FILE_LOCK
            break;
        fi
    else
        echo_t "[ScheduleAutoReboot.sh]: Exiting, another instance is running and max retry reached"
        exit 1
    fi
done

if [ "$2" == "0" ]
then
        Removecron
        rm -f $FILE_LOCK
        exit
else
        Rebootday=$1
fi

calcRebootExecTime()
{        # Extract maintenance window start and end time
        ExtractMaintenanceTime
        #Get local time off set
        time_offset=`dmcli eRT getv Device.Time.TimeOffset | grep "value:" | cut -d ":" -f 3 | tr -d ' '`
        if [ "x$time_offset" == "x" ];then
            echo_t "[ScheduleAutoReboot.sh] time offset is obtained as null"
            time_offset=-18000
        fi

        #Maintence start and end time in local
        main_start_time=$((start_time-time_offset))
        main_end_time=$((end_time-time_offset))

        #calculate random time in sec
        rand_time_in_sec=`awk -v min=$main_start_time -v max=$main_end_time -v seed="$(date +%N)" 'BEGIN{srand(seed);print int(min+rand()*(max-min+1))}'`

        # To avoid cron to be set beyond 24 hr clock limit
        if [ $rand_time_in_sec -ge 86400 ]
        then
                rand_time_in_sec=$((rand_time_in_sec-86400))
                echo_t "[ScheduleAutoReboot.sh] Random time in sec exceed 24 hr limit.setting it correct limit"
        elif [ $rand_time_in_sec -le 0 ]
        then
            rand_time_in_sec=$((rand_time_in_sec+86400))
            echo_t "[ScheduleAutoReboot.sh] Random time in sec negative. Setting it correct limit"
        fi

        #conversion of random generated time to HH:MM:SS format
        #calculate random second
        rand_time=$rand_time_in_sec
        rand_sec=$((rand_time%60))

        #calculate random minute
        rand_time=$((rand_time/60))
        rand_min=$((rand_time%60))

        #calculate random hour
        rand_time=$((rand_time/60))
        rand_hr=$((rand_time%60))

        echo_t "[ScheduleAutoReboot.sh]start_time: $start_time, end_time: $end_time"
        echo_t "[ScheduleAutoReboot.sh]time_offset: $time_offset"
        echo_t "[ScheduleAutoReboot.sh]main_start_time: $main_start_time , main_end_time= $main_end_time"
        echo_t "[ScheduleAutoReboot.sh]rand_time_in_sec: $rand_time_in_sec ,rand_hr: $rand_hr ,rand_min: $rand_min ,rand_sec: $rand_sec"
}

ScheduleCron()
{       
        days_in_sec=$((Rebootday*86400))        
        date_part="$(( `date +%s`+$days_in_sec ))"
        date_final=`date -d @${date_part} +"%d"`
        echo_t "Auto Reboot is scheduled at date: $date_final"
        # Dump existing cron jobs to a file & add new job
        crontab -l -c $CRONTAB_DIR > $CRONFILE_BK
        existing_cron=$(grep "AutoReboot.sh" $CRONFILE_BK)
        cronPattern="$rand_min $rand_hr * * *"
        if [ "$Rebootday" = "0" ]; then
                echo_t "Auto Reboot in next Maintienance window $cronPattern"     
        else           
                cronPattern="$rand_min $rand_hr $date_final * *"
                echo_t "[ScheduleAutoReboot.sh] AutoReboot scheduled cron $cronPattern"  
        fi

        if [ -z "$existing_cron" ]; then
            echo "$cronPattern /etc/AutoReboot.sh" >> $CRONFILE_BK
            echo_t "[ScheduleAutoReboot.sh] Auto Reboot adding the cron job"
        else
           sed -i '/AutoReboot.sh/d' $CRONFILE_BK
           echo "$cronPattern /etc/AutoReboot.sh" >> $CRONFILE_BK
           echo_t "[ScheduleAutoReboot.sh] Auto Reboot updating the cron job"

        fi
        crontab $CRONFILE_BK -c $CRONTAB_DIR
        rm -rf $CRONFILE_BK
}

#calculate ane schedule cron job

calcRebootExecTime
if [ -f $CRONTAB_FILE ]
then
	 ScheduleCron
	 echo_t "[ScheduleAutoReboot.sh] Auto Reboot cron job scheduled"
fi
rm -f $FILE_LOCK
