/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2016 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

/**************************************************************************

    module: cosa_dev_fingerprint_internal.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implements back-end apis for the COSA Data Model Library

**************************************************************************/

#include "cosa_dev_fingerprint_internal.h"
#include "cosa_dev_fingerprint_dml.h"
#include "ccsp_psm_helper.h"
#include "cosa_advsec_utils.h"

extern ANSC_HANDLE bus_handle;
extern char g_Subsystem[32];

static char *g_DeviceFingerPrintEnabled = "Advsecurity_DeviceFingerPrint";

ANSC_HANDLE
CosaDeviceFingerprintCreate
    (
        VOID
    )
{
    PCOSA_DATAMODEL_FPAGENT       pMyObject    = (PCOSA_DATAMODEL_FPAGENT)NULL;
    ULONG                   syscfgValue = 0;
    int                     retGet  = CCSP_SUCCESS;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PCOSA_DATAMODEL_FPAGENT)AnscAllocateMemory(sizeof(COSA_DATAMODEL_FPAGENT));

    if ( !pMyObject )
    {
    	CcspTraceInfo(("%s exit ERROR \n", __FUNCTION__));
        return  (ANSC_HANDLE)NULL;
    }

    retGet = CosaGetSysCfgUlong(g_DeviceFingerPrintEnabled, &syscfgValue);
#if !defined(DUAL_CORE_XB3) && !defined(_HUB4_PRODUCT_REQ_)
    if(!syscfgValue)
    {
	if(access("/usr/ccsp/advsec/print_console_xb6.sh",F_OK) != -1)
        {
                system("/usr/ccsp/advsec/print_console_xb6.sh \"Device_Finger_Printing_enabled:false\" &");
                system("/usr/ccsp/advsec/print_console_xb6.sh \"ADV_SECURITY_SAFE_BROWSING_DISABLE\" &");
                system("/usr/ccsp/advsec/print_console_xb6.sh \"ADV_SECURITY_SOFTFLOWD_DISABLE\" &");
        }
    }
    else
    {
        system("/usr/ccsp/pam/launch_adv_security.sh -sysd &");
    }
#endif //DUAL_CORE_XB3_

    pMyObject->bEnable = syscfgValue;

    return  (ANSC_HANDLE)pMyObject;
}

ANSC_STATUS
CosaDeviceFingerprintRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_FPAGENT            pMyObject    = (PCOSA_DATAMODEL_FPAGENT)hThisObject;

    /* Remove self */
    AnscFreeMemory((ANSC_HANDLE)pMyObject);
    CcspTraceInfo(("%s EXIT \n", __FUNCTION__));

    return returnStatus;
}

ANSC_STATUS CosaAdvSecInit(ANSC_HANDLE hThisObject)
{
    ANSC_STATUS  		returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_FPAGENT	pMyObject    = (PCOSA_DATAMODEL_FPAGENT)hThisObject;
    char cmd[128];

    memset(cmd, 0, sizeof(cmd));

    returnStatus = CosaSetSysCfgUlong(g_DeviceFingerPrintEnabled, 1);
    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        AnscCopyString(cmd, "/usr/ccsp/pam/launch_adv_security.sh -enable &");
        system(cmd);
        pMyObject->bEnable = TRUE;
        fprintf(stderr,"Device_Finger_Printing_enabled:true\n");
    }
    return returnStatus;
}

ANSC_STATUS CosaAdvSecDeInit(ANSC_HANDLE hThisObject)
{
    ANSC_STATUS			returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_FPAGENT	pMyObject    = (PCOSA_DATAMODEL_FPAGENT)hThisObject;
    char cmd[128];

    memset(cmd, 0, sizeof(cmd));

    returnStatus = CosaSetSysCfgUlong(g_DeviceFingerPrintEnabled, 0);
    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        AnscCopyString(cmd, "/usr/ccsp/pam/launch_adv_security.sh -disable &");
        system(cmd);
        pMyObject->bEnable = FALSE;
        fprintf(stderr,"Device_Finger_Printing_enabled:false\n");
    }
    return returnStatus;
}

BOOL is_device_finger_printing_enabled()
{
    ULONG                   value = 0;

    CosaGetSysCfgUlong(g_DeviceFingerPrintEnabled, &value);
    return value;
}
