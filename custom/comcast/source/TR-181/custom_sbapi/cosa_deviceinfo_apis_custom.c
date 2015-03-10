/**********************************************************************
   Copyright [2014] [Cisco Systems, Inc.]
 
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
 
       http://www.apache.org/licenses/LICENSE-2.0
 
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
**********************************************************************/

/**************************************************************************

    module: cosa_deviceinfo_api_custom.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    copyright:

        Cisco Systems, Inc.
        All Rights Reserved.

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for custom DeviceInfo Data
        Models

    -------------------------------------------------------------------

    environment:

        platform dependent

    -------------------------------------------------------------------

    author:

        Ding Hua

    -------------------------------------------------------------------

    revision:

        07/07/2014    initial revision.

**************************************************************************/

#include "cosa_deviceinfo_apis.h"
#include "cosa_deviceinfo_apis_custom.h"
#include "dml_tr181_custom_cfg.h" 

#include "ccsp_psm_helper.h"            // for PSM_Get_Record_Value2
#include "dmsb_tr181_psm_definitions.h" // for DMSB_TR181_PSM_DeviceInfo_Root/ProductClass
 
#include <utctx.h>
#include <utctx_api.h>
#include <utapi.h>
#include <utapi_util.h>
#define _ERROR_ "NOT SUPPORTED"

extern void* g_pDslhDmlAgent;

static int
PsmGet(const char *param, char *value, int size)
{
    char *val = NULL;

    if (PSM_Get_Record_Value2(g_MessageBusHandle, g_GetSubsystemPrefix(g_pDslhDmlAgent),
                (char *)param, NULL, &val) != CCSP_SUCCESS)
        return -1;
    
    if(val) {
        snprintf(value, size, "%s", val);
        ((CCSP_MESSAGE_BUS_INFO *)g_MessageBusHandle)->freefunc(val);
    }
    else return -1;

    return 0;
}

#ifdef CONFIG_VENDOR_CUSTOMER_COMCAST

extern ANSC_HANDLE bus_handle;

static ANSC_STATUS
Local_CosaDmlGetParamValueByPathName
    (
        const char *pathName,
        char *pValue,
        PULONG pulSize
    )
{
    ANSC_STATUS retval = ANSC_STATUS_FAILURE;
    parameterValStruct_t varStruct;
    char outdata[80];
    int size = sizeof(outdata);

    varStruct.parameterName = pathName;
    varStruct.parameterValue = outdata;

    retval = COSAGetParamValueByPathName(bus_handle, &varStruct, &size);
    
    if ( retval != ANSC_STATUS_SUCCESS) 
    {
        return ANSC_STATUS_FAILURE;
    }
    else 
    {
        AnscCopyString(pValue, outdata);
        *pulSize = AnscSizeOfString(pValue);
        return ANSC_STATUS_SUCCESS;
    }
}

/*X_COMCAST-COM_CM_MAC*/
ANSC_STATUS
CosaDmlDiGetCMMacAddress
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
    return Local_CosaDmlGetParamValueByPathName("Device.X_CISCO_COM_CableModem.MACAddress", pValue, pulSize);
}

/*X_COMCAST-COM_WAN_MAC*/
ANSC_STATUS
CosaDmlDiGetRouterMacAddress
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
    s_get_interface_mac("erouter0", pValue, 18);
    *pulSize = AnscSizeOfString(pValue);

    return ANSC_STATUS_SUCCESS;
}

/*X_COMCAST-COM_MTA_MAC*/
ANSC_STATUS
CosaDmlDiGetMTAMacAddress
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
    return Local_CosaDmlGetParamValueByPathName("Device.X_CISCO_COM_MTA.MACAddress", pValue, pulSize);
}

/*X_COMCAST-COM_WAN_IP*/
ANSC_STATUS
CosaDmlDiGetRouterIPAddress
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
	unsigned int UIntIP = (unsigned int)CosaUtilGetIfAddr("erouter0");
	sprintf(pValue, "%d.%d.%d.%d", (UIntIP >> 24),((UIntIP >> 16) & 0xff),((UIntIP >> 8) & 0xff),(UIntIP & 0xff));
	*pulSize = AnscSizeOfString(pValue);

    return ANSC_STATUS_SUCCESS;
}

/*X_COMCAST-COM_MTA_IP*/
ANSC_STATUS
CosaDmlDiGetMTAIPAddress
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
    return Local_CosaDmlGetParamValueByPathName("Device.X_CISCO_COM_MTA.IPAddress", pValue, pulSize);
}

/*X_COMCAST-COM_CM_IP*/
ANSC_STATUS
CosaDmlDiGetCMIPAddress
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
    return Local_CosaDmlGetParamValueByPathName("Device.X_CISCO_COM_CableModem.IPAddress", pValue, pulSize);
}

#endif


#ifdef CONFIG_CISCO_HOTSPOT

ANSC_STATUS
CosaDmlDiGetXfinityWiFiCapable
    (
        BOOL *pValue
    )
{
    *pValue = TRUE;
    return ANSC_STATUS_SUCCESS;
}

#define HOTSPOT_PSM_EANBLE     "dmsb.hotspot.enable"

static int
PsmSet(const char *param, const char *value)
{
    if (PSM_Set_Record_Value2(g_MessageBusHandle, g_GetSubsystemPrefix(g_pDslhDmlAgent),
                (char *)param, ccsp_string, (char *)value) != CCSP_SUCCESS)
        return -1;
    return 0;
}

static int 
DmSetBool(const char *param, BOOL value)
{
    parameterValStruct_t val[1];
    char crname[256], *fault = NULL;
    int err;

    val[0].parameterName  = param;
    val[0].parameterValue = (value ? "true" : "false");
    val[0].type           = ccsp_boolean;

    snprintf(crname, sizeof(crname), "%s%s", g_GetSubsystemPrefix(g_pDslhDmlAgent), CCSP_DBUS_INTERFACE_CR);

    if ((err = CcspBaseIf_SetRemoteParameterValue(g_MessageBusHandle, 
                crname, param, g_GetSubsystemPrefix(g_pDslhDmlAgent), 0, 0xFFFF, val, 1, 1, &fault)) != CCSP_SUCCESS)

    if (fault)
        AnscFreeMemory(fault);

    return (err == CCSP_SUCCESS) ? 0 : -1;
}

ANSC_STATUS
CosaDmlDiGetXfinityWiFiEnable
    (
        BOOL *pValue
    )
{
    char val[64];

    if (PsmGet(HOTSPOT_PSM_EANBLE, val, sizeof(val)) != 0)
        return ANSC_STATUS_FAILURE;

    *pValue = (atoi(val) == 1) ? TRUE : FALSE;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDiSetXfinityWiFiEnable
    (
        BOOL value
    )
{
#if 0
    if (DmSetBool("Device.WiFi.SSID.5.Enable", value) != ANSC_STATUS_SUCCESS) {
        AnscTraceError(("%s: set WiFi SSID Enable error\n", __FUNCTION__));
    } else {
        AnscTraceWarning(("%s: set WiFi SSID Enable OK\n", __FUNCTION__));
    }

    if (DmSetBool("Device.WiFi.Radio.1.X_CISCO_COM_ApplySetting", value) != ANSC_STATUS_SUCCESS) {
        AnscTraceError(("%s: set WiFi ApplySetting error\n", __FUNCTION__));
    } else {
        AnscTraceWarning(("%s: set WiFi ApplySetting OK\n", __FUNCTION__));
    }
#endif
    if (g_SetParamValueBool("Device.X_COMCAST_COM_GRE.Interface.1.Enable", value) != ANSC_STATUS_SUCCESS) {
        AnscTraceError(("%s: set WiFi ApplySetting error\n", __FUNCTION__));
    } else {
        AnscTraceWarning(("%s: set WiFi ApplySetting OK\n", __FUNCTION__));
    }

    if (value)
        PsmSet(HOTSPOT_PSM_EANBLE, "1");
    else
        PsmSet(HOTSPOT_PSM_EANBLE, "0");

    return ANSC_STATUS_SUCCESS;
}
#endif
