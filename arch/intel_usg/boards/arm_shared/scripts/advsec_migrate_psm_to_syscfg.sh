#!/bin/sh

#This script should be removed once migartion from PSM to SysCfg DB is complete for Advance Secuurity Finger Print service.

PSM_DB=/nvram/bbhm_cur_cfg.xml
ADVSEC_FP_RECORD="eRT.com.cisco.spvtg.ccsp.advsecurity.Device.DeviceInfo.X_RDKCENTRAL-COM_DeviceFingerPrint.Enable"
ADVSEC_FP_SYSCFG=Advsecurity_DeviceFingerPrint

check_psm_setting()
{
	if [ -f $PSM_DB ]; then
		_res=`grep $ADVSEC_FP_RECORD $PSM_DB | grep "type=\"astr\">1</Record>"`
		if [ "$?" = "0" ]; then
			echo "true"
		else
			echo "false"
		fi
	fi
}

load_syscfg_setting()
{
	if [ "$( check_psm_setting )" = "true" ]; then
		_res=`syscfg set $ADVSEC_FP_SYSCFG 1`
		_res=`syscfg commit`
	fi
}

reset_psm_setting()
{
	if [ "$( check_psm_setting )" = "true" ]; then
		_res=`psmcli set eRT.com.cisco.spvtg.ccsp.advsecurity.Device.DeviceInfo.X_RDKCENTRAL-COM_DeviceFingerPrint.Enable 0`
	fi
}

$1

