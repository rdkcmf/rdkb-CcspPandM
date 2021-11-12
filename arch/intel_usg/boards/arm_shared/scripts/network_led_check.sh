#!/bin/sh

source /etc/utopia/service.d/log_capture_path.sh
source /etc/device.properties

WAN_INTERFACE="erouter0"
url="http://clients3.google.com/generate_204"
bootresponse="/var/tmp/bootresponse.txt"
response=""
v4Count=0
v6Count=0
activated=0
gotresponse=0
iteration=1
delay=5
ert_ipv6=""
ert_ipv4=""

#If script is already running, then latest one takes priority.
#Kill previous script and proceed
for pid in $(pidof network_led_check.sh); do
    if [ $pid != $$ ]; then
        echo_t "Process is already running with PID $pid"
        kill -9 $pid
    fi
done

   # This script is called only when WAN is up. So no need to check cases of no IP.
   # Such cases/delay in WAN ip assignment should be handled in network_response.sh
   # network_led_check.sh is intended to control the white/blinking white LEDS in case 
   # of walledgarden bootfile

getIpStatus()
{
  ert_ipv4=`ifconfig $WAN_INTERFACE | grep "inet addr" | cut -d":" -f2 | cut -d" " -f1`
  ert_ipv6=`ifconfig $WAN_INTERFACE | grep inet6 | grep Global | cut -f1 -d/ | sed -r 's/\s+//g'`
  echo_t "network_led_status: ert_ipv4=$ert_ipv4 , ert_ipv6:$ert_ipv6"
}

getIpStatus

while [ $activated -ne 1 ]
do
   if [ $v4Count -lt 3 ] && [ "$ert_ipv4" != "" ]
   then
      echo_t "network_led_check: Executing curl command for ipv4"
      curl -4 -w '%{http_code}\n' --interface $WAN_INTERFACE $url --connect-timeout 30 -m 30 > $bootresponse

      if [ "$ert_ipv6" = "" ] && [ "$v4Count" = "0" ]
      then
         last_erouter_mode=`syscfg get last_erouter_mode`
         ipv6_status_check=$(sysevent get ipv6-status)
         if [ "$last_erouter_mode" = "1" ] && [ "x$ipv6_status_check" = "x" ];then
             echo_t "network_led_check: CDV device, not having ipv6  exiting !!"
             exit 0
         fi
      fi
      echo_t "network_led_check: Increment count as we have ipv6 ip"                                     
      v4Count=`expr $v4Count + 1`
   else
      if [ $v6Count -lt 3 ] && [ "$ert_ipv6" != "" ]
      then
        echo_t "network_led_check: Executing curl command for ipv6"
        curl -6 -w '%{http_code}\n' --interface $WAN_INTERFACE $url --connect-timeout 30 -m 30 > $bootresponse
        v6Count=`expr $v6Count + 1`
      else
        v4Count=0
        v6Count=0
        if [ $iteration -eq 1 ]
        then
           getIpStatus
           iteration=`expr $iteration + 1`
        else
           # Tried 6 ipv4 and 6 ipv6 curl queries.
           # Exit from script
           echo_t "network_led_check: Exit as retries done"
           exit 0
        fi
      fi
   fi

   if [ -f $bootresponse ]
   then
      response=`cat $bootresponse`
      echo_t "network_led_check: Response received is $response"
      if [ "$response" = "204" ]
      then
          activated=1
      else
          activated=0
      fi
  else
     echo_t "network_led_check: No response received"
     activated=0
  fi


  if [ $activated -eq 1 ]
  then
     echo_t "network_led_check: activated is 1"
     if [ "$BOX_TYPE" = "XB6" ]; then
             white=0
             solid=0
             /usr/bin/SetLED $white $solid       #Set LED to Solid White
     fi
  else
        echo_t "network_led_check: activated is 0"
        white=0                                                                          
        blink=1 
        if [ "$BOX_TYPE" = "XB6" ]; then
          interval=1
        fi

        if [ "$MODEL_NUM" = "CGA4332COM" ]; then
          interval=5
        fi
        /usr/bin/SetLED $white $blink $interval     #Set LED to Blink White

        # If both IPs are NULL then unit will end up in RED LED state controlled by WEBPA
        if [ "$ert_ipv4" = "" ] && [ "$ert_ipv6" = "" ]
        then
           echo_t "network_led_check: WAN ipv4 and ipv6 are NULL"                     
           # Put an additional delay before next retry
           # as script will exit after retry
           sleep 10
        
        fi
  fi
  sleep $delay
done

if [ -f $bootresponse ]
then
   rm -rf $bootresponse
fi

exit 0
