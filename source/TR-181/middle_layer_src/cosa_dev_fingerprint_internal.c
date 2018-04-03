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

    module: cosa_adv_security_internal.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implements back-end apis for the COSA Data Model Library

**************************************************************************/

#include "cosa_dev_fingerprint_internal.h"
#include "cosa_dev_fingerprint_dml.h"
#include "ccsp_psm_helper.h"


extern ANSC_HANDLE bus_handle;
extern char g_Subsystem[32];

static char *g_DeviceFingerPrintEnabled = "eRT.com.cisco.spvtg.ccsp.advsecurity.Device.DeviceInfo.X_RDKCENTRAL-COM_DeviceFingerPrint.Enable";

ANSC_STATUS CosaGetNVRamULONG(char* setting, ULONG *value);
ANSC_STATUS CosaSetNVRamULONG(char* setting, ULONG value);

ANSC_HANDLE
CosaDeviceFingerprintCreate
    (
        VOID
    )
{
    PCOSA_DATAMODEL_FPAGENT       pMyObject    = (PCOSA_DATAMODEL_FPAGENT)NULL;
    ULONG                   psmValue = 0;
    int                     retPsmGet  = CCSP_SUCCESS;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PCOSA_DATAMODEL_FPAGENT)AnscAllocateMemory(sizeof(COSA_DATAMODEL_FPAGENT));

    if ( !pMyObject )
    {
    	CcspTraceInfo(("%s exit ERROR \n", __FUNCTION__));
        return  (ANSC_HANDLE)NULL;
    }

    retPsmGet = CosaGetNVRamULONG(g_DeviceFingerPrintEnabled, &psmValue);
    if(!psmValue)
    {
        fprintf(stderr,"\nDevice_Finger_Printing_enabled:false\n");
        fprintf(stderr,"\nADV_SECURITY_SAFE_BROWSING_DISABLE\n");
        fprintf(stderr,"\nADV_SECURITY_SOFTFLOWD_DISABLE\n");
#ifdef _XB6_PRODUCT_REQ_
        system("/usr/ccsp/advsec/print_console_xb6.sh \"Device_Finger_Printing_enabled:false\" &");
        system("/usr/ccsp/advsec/print_console_xb6.sh \"ADV_SECURITY_SAFE_BROWSING_DISABLE\" &");
        system("/usr/ccsp/advsec/print_console_xb6.sh \"ADV_SECURITY_SOFTFLOWD_DISABLE\" &");
#endif //_XB6_PRODUCT_REQ_
    }

    pMyObject->bEnable = psmValue; 

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

ANSC_STATUS CosaGetNVRamULONG(char* setting, ULONG* value)
{
    char *strValue = NULL;
    int retPsmGet = CCSP_SUCCESS;

    retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, setting, NULL, &strValue);
    if (retPsmGet == CCSP_SUCCESS) {
        *value = _ansc_atoi(strValue);
        ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(strValue);
    }
    return retPsmGet;
}

ANSC_STATUS CosaSetNVRamULONG(char* setting, ULONG value)
{
    int retPsmSet = CCSP_SUCCESS;
    char psmValue[32] = {};

    sprintf(psmValue,"%d",value);
    retPsmSet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, setting, ccsp_string, psmValue);
    if (retPsmSet != CCSP_SUCCESS)
        {
        CcspTraceInfo(("%s PSM_Set_Record_Value2 returned ERROR [%d] while setting [%s] Value [%d]\n",__FUNCTION__, retPsmSet, setting, value));
        }
    else
        {
        CcspTraceInfo(("%s PSM_Set_Record_Value2 returned SUCCESS[%d] while Setting [%s] Value [%d]\n",__FUNCTION__, retPsmSet, setting, value));
        }
    return retPsmSet;
}

ANSC_STATUS CosaAdvSecInit(ANSC_HANDLE hThisObject)
{
    ANSC_STATUS  returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_FPAGENT            pMyObject    = (PCOSA_DATAMODEL_FPAGENT)hThisObject;

    char cmd[128];
    memset(cmd, 0, sizeof(cmd));
    /*We need to set this before the script since in XB6,
      there is a check in ExecStartPre for PSM to be enabled */
    CosaSetNVRamULONG(g_DeviceFingerPrintEnabled, 1);
    AnscCopyString(cmd, "/usr/ccsp/pam/launch_adv_security.sh -enable &");
    returnStatus = WEXITSTATUS(system(cmd));
    if ( returnStatus == ANSC_STATUS_SUCCESS )
	{
        pMyObject->bEnable = TRUE;
        fprintf(stderr,"Device_Finger_Printing_enabled:true\n");
	}
    return returnStatus;
}

ANSC_STATUS CosaAdvSecDeInit(ANSC_HANDLE hThisObject)
{
    ANSC_STATUS  returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_FPAGENT            pMyObject    = (PCOSA_DATAMODEL_FPAGENT)hThisObject;
    char cmd[128];
    memset(cmd, 0, sizeof(cmd));
    AnscCopyString(cmd, "/usr/ccsp/pam/launch_adv_security.sh -disable &");
    returnStatus = WEXITSTATUS(system(cmd));
    if ( returnStatus == ANSC_STATUS_SUCCESS )
	{
        pMyObject->bEnable = FALSE;

        //ignore nvram set failure
        CosaSetNVRamULONG(g_DeviceFingerPrintEnabled, 0);
        fprintf(stderr,"Device_Finger_Printing_enabled:false\n");
	}
    return returnStatus;
}

