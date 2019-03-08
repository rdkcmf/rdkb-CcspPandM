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

#This script should be removed once migartion from PSM to SysCfg DB is complete for Advance Secuurity Finger Print service.

PSM_DB=/nvram/bbhm_cur_cfg.xml
ADVSEC_FP_RECORD="eRT.com.cisco.spvtg.ccsp.advsecurity.Device.DeviceInfo.X_RDKCENTRAL-COM_DeviceFingerPrint.Enable"
ADVSEC_FP_SYSCFG=Advsecurity_DeviceFingerPrint
ADVSEC_MIGRATE_SYSCFG=Advsecurity_MigrateToSysDB

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
	_res=`syscfg get $ADVSEC_MIGRATE_SYSCFG`
	if [ "$_res" != "1" ]; then
		if [ "$( check_psm_setting )" = "true" ]; then
			_res=`syscfg set $ADVSEC_FP_SYSCFG 1`
			_res=`syscfg set $ADVSEC_MIGRATE_SYSCFG 1`
			_res=`syscfg commit`
		fi
	fi
}

$1

