#!/bin/bash
##########################################################################
# If not stated otherwise in this file or this component's Licenses.txt
# file the following copyright and licenses apply:
#
# Copyright 2015 RDK Management
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

#######################################################################
#   Copyright [2014] [Cisco Systems, Inc.]
# 
#   Licensed under the Apache License, Version 2.0 (the \"License\");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
# 
#       http://www.apache.org/licenses/LICENSE-2.0
# 
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an \"AS IS\" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#######################################################################


EMAIL_READY="/var/.email_ready"
SEND_FW="/var/.email_fwlog"
SEND_PC="/var/.email_pclog"
SEND_AW="/var/.email_awlog"
SEND_ALL="/var/.email_alllog"

FW_LOG_CACHE="/var/.fw_log_cache"
PC_LOG_CACHE="/var/.pc_log_cache"
AW_LOG_CACHE="/var/.aw_log_cache"

FWLOG_PATTERN="UTOPIA: FW"
PCLOG_PATTERN="Blocked_"
AWLOG_PATTERN="\.warn\ |\.alert\ "

FW_LOG="/var/log/kernel"
PC_LOG="/var/log/kernel"
AW_LOG="/var/log/messages"

FW_LAST_LOG=${FWLOG_PATTERN}
PC_LAST_LOG=${PCLOG_PATTERN}
AW_LAST_LOG=${AWLOG_PATTERN}

VERSION_FILE="/etc/versions"

if [ ! -f $VERSION_FILE ]; then
    PRODUCT_NAME="Docsis Gateway"
elif [ `head -n1 $VERSION_FILE | grep -wc "dpc3939"` -eq 1 ]; then
    PRODUCT_NAME="dpc3939"
elif [ `head -n1 $VERSION_FILE | grep -wc "dpc3939b"` -eq 1 ]; then
    PRODUCT_NAME="dpc3939b"
elif [ `head -n1 $VERSION_FILE | grep -wc "dpc3848"` -eq 1 ]; then
    PRODUCT_NAME="dpc3848"
elif [ `head -n1 $VERSION_FILE | grep -wc "dpc3848"` -eq 1 ]; then
    PRODUCT_NAME="dpc3848"
else
    PRODUCT_NAME="Docsis Gateway"
fi

if [ $# -eq 0 ]; then
    sleepInterval=12
else
    sleepInterval=$1
fi

rm -rf "${FW_LOG_CACHE}"
rm -rf "${PC_LOG_CACHE}"
rm -rf "${AW_LOG_CACHE}"

while [ 1 ]; do

    ##wait until email notification SB APIs are ready
    while [ ! -e "${EMAIL_READY}" ];do
        sleep 5
    done
   
    ##send interval
    sleep $sleepInterval

    psCnt=`ps | grep -F "email_notification_monitor.sh" | wc -l`                
    while [ $psCnt -gt 3 ]; do                                                  
        sleep 3                                                                 
        psCnt=`ps | grep -F "email_notification_monitor.sh" | wc -l`            
    done
    
    ##-------------------------FW LOGS--------------------------------------------------
    ##collect firewall logs only if its GUI option is set to yes
    if [ -e "${SEND_FW}" -o -e "${SEND_ALL}" ]; then
        
        match=`grep -F -c "${FW_LAST_LOG}" ${FW_LOG}`

        if [ $match -ne 0 ]; then
            ##1. if ${FW_LAST_LOG} is FWLOG_PATTERN, then append all the entries in the log file
            ##2. if ${FW_LAST_LOG} is the last line of logs which has been sent, then append all the entries AFTER this line 
            if [ "${FW_LAST_LOG}" != "${FWLOG_PATTERN}" ]; then
                awk -v pat="${FW_LAST_LOG}" 'f; $0 == pat {f=1}' "${FW_LOG}" | grep -F "${FWLOG_PATTERN}" >> ${FW_LOG_CACHE}

            else
                grep -F "${FWLOG_PATTERN}" "${FW_LOG}" >> ${FW_LOG_CACHE} 
            fi
        elif [ "${FW_LAST_LOG}" != "${FWLOG_PATTERN}" ]; then
            ##if no entries are found
            ##1. Maybe after the last email has been sent, ${FW_LAST_LOG} is flushed in the log file. We need to find the new entries by ${FWLOG_PATTERN}
            FW_LAST_LOG="${FWLOG_PATTERN}"
            
            match2=`grep -F -c "${FW_LAST_LOG}" ${FW_LOG}`

            if [ $match2 -ne 0 ]; then
                ##1.1 if new entries are found by ${FWLOG_PATTERN}, append them to log cache file.
                grep -F "${FWLOG_PATTERN}" "${FW_LOG}" >> ${FW_LOG_CACHE} 
            fi
        fi
    
        ##if found new log entries
        ##1. save the last line for tracking
        ##2. send logs 
        ##3. clean log cache file
        if [ -s "${FW_LOG_CACHE}" ]; then
            FW_LAST_LOG=`tail -n 1 ${FW_LOG_CACHE}`;

            (((cat /var/ssmtp_msg; echo "Subject: $PRODUCT_NAME Firewall Logs"; echo; echo; uuencode ${FW_LOG_CACHE} USGv2_Firewall_Logs_`date +%Y_%m_%d_%H_%M_%S`.txt) | ssmtp `awk -F ': ' 'NR==1{print $2}' /var/ssmtp_msg`) && (echo -n > ${FW_LOG_CACHE}))
        fi
    fi
   
    ##Not using array here since we may want to apply different log sending strategy for different log types. Repeat same code block.
    ##-------------------------PC LOGS--------------------------------------------------
    ##collect parental control logs only if its GUI option is set to yes
    if [ -e "${SEND_PC}" -o -e "${SEND_ALL}" ]; then
        
        match=`grep -F -c "${PC_LAST_LOG}" ${PC_LOG}`

        if [ $match -ne 0 ]; then
            ##1. if ${PC_LAST_LOG} is PCLOG_PATTERN, then append all the entries in the log file
            ##2. if ${PC_LAST_LOG} is the last line of logs which has been sent, then append all the entries AFTER this line 
            if [ "${PC_LAST_LOG}" != "${PCLOG_PATTERN}" ]; then
                awk -v pat="${PC_LAST_LOG}" 'f; $0 == pat {f=1}' "${PC_LOG}" | grep -F "${PCLOG_PATTERN}" >> ${PC_LOG_CACHE}
            else
                grep -F "${PCLOG_PATTERN}" "${PC_LOG}" >> ${PC_LOG_CACHE} 
            fi
        elif [ "${PC_LAST_LOG}" != "${PCLOG_PATTERN}" ]; then
            ##if no entries are found
            ##1. Maybe after the last email has been sent, ${PC_LAST_LOG} is flushed in the log file. We need to find the new entries by ${PCLOG_PATTERN}
            PC_LAST_LOG="${PCLOG_PATTERN}"
            
            match2=`grep -F -c "${PC_LAST_LOG}" ${PC_LOG}`

            if [ $match2 -ne 0 ]; then
                ##1.1 if new entries are found by ${PCLOG_PATTERN}, append them to log cache file.
                grep -F "${PCLOG_PATTERN}" "${PC_LOG}" >> ${PC_LOG_CACHE} 
            fi
        fi
    
        ##if found new log entries
        ##1. save the last line for tracking
        ##2. send logs 
        ##3. clean log cache file
        if [ -s "${PC_LOG_CACHE}" ]; then
            PC_LAST_LOG=`tail -n 1 ${PC_LOG_CACHE}`;

            BLKURL_CNT=`syscfg get ManagedSiteBlockCount`
            INDEX=1

            while [ $INDEX -le $BLKURL_CNT ]; do
                SITE=`syscfg get pcms_$INDEX site`
                METHOD=`syscfg get pcms_$INDEX method`
                
                if [ $METHOD == "URL" ]; then
                    sed -i 's,LOG_SiteBlocked_'"${INDEX}"'_DROP,Block URL '"$SITE"' ,g' "${PC_LOG_CACHE}"
                else
                    sed -i 's,LOG_SiteBlocked_'"${INDEX}"'_DROP,Block Keyword '"$SITE"' ,g' "${PC_LOG_CACHE}"
                fi                
                let INDEX=$INDEX+1
            done
            
            BLKSERV_CNT=`syscfg get ManagedServiceBlockCount`
            INDEX=1

            while [ $INDEX -le $BLKSERV_CNT ]; do
                SERV=`syscfg get pcmse_$INDEX desc`
                sed -i "s/LOG_ServiceBlocked_${INDEX}_DROP/Block Service $SERV /g" ${PC_LOG_CACHE}
                let INDEX=$INDEX+1
            done

            (((cat /var/ssmtp_msg; echo "Subject: $PRODUCT_NAME Parental Control Logs"; echo; echo; uuencode ${PC_LOG_CACHE} USGv2_ParentalControl_Logs_`date +%Y_%m_%d_%H_%M_%S`.txt) | ssmtp `awk -F ': ' 'NR==1{print $2}' /var/ssmtp_msg`) && (echo -n > ${PC_LOG_CACHE}))
        fi
    fi

    ##-------------------------AW LOGS--------------------------------------------------
    ##collect alerts and warnings logs only if its GUI option is set to yes
    if [ -e "${SEND_AW}" -o -e "${SEND_ALL}" ]; then

        if [ "${AW_LAST_LOG}" = "${AWLOG_PATTERN}" ]; then
            match=`grep -E -c "${AW_LAST_LOG}" ${AW_LOG}`
        else
            match=`grep -F -c "${AW_LAST_LOG}" ${AW_LOG}`
        fi

        if [ $match -ne 0 ]; then
            ##1. if ${AW_LAST_LOG} is AWLOG_PATTERN, then append all the entries in the log file
            ##2. if ${AW_LAST_LOG} is the last line of logs which has been sent, then append all the entries AFTER this line 
            if [ "${AW_LAST_LOG}" != "${AWLOG_PATTERN}" ]; then
                awk -v pat="${AW_LAST_LOG}" 'f; $0 == pat {f=1}' "${AW_LOG}" | grep -E "${AWLOG_PATTERN}" >> ${AW_LOG_CACHE}
            else
                grep -E "${AWLOG_PATTERN}" "${AW_LOG}" >> ${AW_LOG_CACHE} 
            fi
        elif [ "${AW_LAST_LOG}" != "${AWLOG_PATTERN}" ]; then
            ##if no entries are found
            ##1. Maybe after the last email has been sent, ${AW_LAST_LOG} is flushed in the log file. We need to find the new entries by ${AWLOG_PATTERN}
            AW_LAST_LOG="${AWLOG_PATTERN}"
            
            match2=`grep -E -c "${AW_LAST_LOG}" ${AW_LOG}`

            if [ $match2 -ne 0 ]; then
                ##1.1 if new entries are found by ${AWLOG_PATTERN}, append them to log cache file.
                grep -E "${AWLOG_PATTERN}" "${AW_LOG}" >> ${AW_LOG_CACHE} 
            fi
        fi
    
        ##if found new log entries
        ##1. save the last line for tracking
        ##2. send logs 
        ##3. clean log cache file
        if [ -s "${AW_LOG_CACHE}" ]; then
            AW_LAST_LOG=`tail -n 1 ${AW_LOG_CACHE}`;

            (((cat /var/ssmtp_msg; echo "Subject: $PRODUCT_NAME Alerts and Warnings Logs"; echo; echo; uuencode ${AW_LOG_CACHE} USGv2_AlertsOrWarnings_Logs_`date +%Y_%m_%d_%H_%M_%S`.txt) | ssmtp `awk -F ': ' 'NR==1{print $2}' /var/ssmtp_msg`) && (echo -n > ${AW_LOG_CACHE}))
        fi
    fi

done
