/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2015 RDK Management
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
#include "platform_hal.h"
#include "secure_wrapper.h"

#include "cosa_x_comcast_com_gre_apis.h"
#include <utctx.h>
#include <utctx_api.h>
#include <utapi.h>
#include <utapi_util.h>
#include <pthread.h>
#include <syscfg/syscfg.h>

#define _ERROR_ "NOT SUPPORTED"

#define CAPTIVEPORTAL_EANBLE     "CaptivePortal_Enable"

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

ANSC_STATUS
COSAGetParamValueByPathName
    (
        void*                      bus_handle,
        parameterValStruct_t       *val,
        ULONG                      *parameterValueLength
    );

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
    ULONG size = sizeof(outdata);
    outdata[0] = '\0';

    varStruct.parameterName = (char*)pathName;
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
    UNREFERENCED_PARAMETER(hContext);
#ifndef _ENABLE_EPON_SUPPORT_ 
	return Local_CosaDmlGetParamValueByPathName("Device.X_CISCO_COM_CableModem.MACAddress", pValue, pulSize);
#else
	return Local_CosaDmlGetParamValueByPathName("Device.DPoE.Mac_address",pValue, pulSize);
#endif      
}
/*X_COMCAST-COM_EWAN_CMMAC*/
ANSC_STATUS
CosaDmlDiGetEwanCMMacAddress
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
     platform_hal_GetBaseMacAddress(pValue);
    *pulSize = AnscSizeOfString(pValue);
    return ANSC_STATUS_SUCCESS;
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
    UNREFERENCED_PARAMETER(hContext);
#ifdef FEATURE_RDKB_XDSL_PPP_MANAGER
    char wanPhyName[32] = {0};
    char out_value[32] = {0};

    syscfg_init();

    if (syscfg_get(NULL, "wan_physical_ifname", out_value, sizeof(out_value)) == 0)
    {
       strncpy(wanPhyName, out_value, sizeof(wanPhyName));
       CcspTraceInfo(("%s %d - WanPhyName=%s \n", __FUNCTION__,__LINE__, wanPhyName));
    }
    else
    {
       strncpy(wanPhyName, "erouter0", sizeof(wanPhyName));
       CcspTraceInfo(("%s %d - WanPhyName=%s \n", __FUNCTION__,__LINE__, wanPhyName));
    }
    s_get_interface_mac(wanPhyName, pValue, 18);
#else	
    s_get_interface_mac("erouter0", pValue, 18);
#endif    
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
    UNREFERENCED_PARAMETER(hContext);
#if !defined (_HUB4_PRODUCT_REQ_)
    return Local_CosaDmlGetParamValueByPathName("Device.X_CISCO_COM_MTA.MACAddress", pValue, pulSize);
#else
    UNREFERENCED_PARAMETER(pValue);
    UNREFERENCED_PARAMETER(pulSize);
    return ANSC_STATUS_FAILURE;
#endif
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
    UNREFERENCED_PARAMETER(hContext);
	unsigned int UIntIP = (unsigned int)CosaUtilGetIfAddr("erouter0");
#if defined (_XB6_PRODUCT_REQ_) ||  defined (_COSA_BCM_ARM_)
	sprintf(pValue, "%d.%d.%d.%d",(UIntIP & 0xff),((UIntIP >> 8) & 0xff),((UIntIP >> 16) & 0xff),(UIntIP >> 24));
#else
	sprintf(pValue, "%d.%d.%d.%d", (UIntIP >> 24),((UIntIP >> 16) & 0xff),((UIntIP >> 8) & 0xff),(UIntIP & 0xff));
#endif
	*pulSize = AnscSizeOfString(pValue);

    return ANSC_STATUS_SUCCESS;
}

/*X_COMCAST-COM_WAN_IPv6*/
ANSC_STATUS
CosaDmlDiGetRouterIPv6Address
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pulSize);
	ipv6_addr_info_t * p_v6addr = NULL;
    int  v6addr_num = 0, i, l_iIpV6AddrLen;

#if defined(_HUB4_PRODUCT_REQ_)
	CosaUtilGetIpv6AddrInfo("brlan0", &p_v6addr, &v6addr_num);
#else
	CosaUtilGetIpv6AddrInfo("erouter0", &p_v6addr, &v6addr_num);
#endif
    for(i = 0; i < v6addr_num; i++ )
    {
#if defined(_HUB4_PRODUCT_REQ_)
        if((p_v6addr[i].scope == IPV6_ADDR_SCOPE_GLOBAL) && (strncmp(p_v6addr[i].v6addr, "fd", 2) != 0) && (strncmp(p_v6addr[i].v6addr, "fc", 2) != 0))
#else
        if(p_v6addr[i].scope == IPV6_ADDR_SCOPE_GLOBAL)
#endif
        {
			l_iIpV6AddrLen = strlen(p_v6addr[i].v6addr);
			strncpy(pValue, p_v6addr[i].v6addr, l_iIpV6AddrLen);
			pValue[l_iIpV6AddrLen] = '\0';
        }
    }
	if(p_v6addr)
        free(p_v6addr);

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
    UNREFERENCED_PARAMETER(hContext);
#if !defined (_HUB4_PRODUCT_REQ_)
    return Local_CosaDmlGetParamValueByPathName("Device.X_CISCO_COM_MTA.IPAddress", pValue, pulSize);
#else
    UNREFERENCED_PARAMETER(pValue);
    UNREFERENCED_PARAMETER(pulSize);
    return ANSC_STATUS_FAILURE;
#endif
}

/*X_COMCAST-COM_MTA_IPV6*/
ANSC_STATUS
CosaDmlDiGetMTAIPV6Address
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hContext);
    return Local_CosaDmlGetParamValueByPathName("Device.X_CISCO_COM_MTA_V6.IPV6Address", pValue, pulSize);
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
    UNREFERENCED_PARAMETER(hContext);
#ifndef _ENABLE_EPON_SUPPORT_
    ANSC_STATUS retStatus;
    retStatus = Local_CosaDmlGetParamValueByPathName("Device.X_CISCO_COM_CableModem.IPv6Address", pValue, pulSize);
    if(!(*pulSize))
        return Local_CosaDmlGetParamValueByPathName("Device.X_CISCO_COM_CableModem.IPAddress", pValue, pulSize);

    return retStatus;
#else
    if ( pValue )
    {
        strcpy(pValue, "0.0.0.0");
    }
    UNREFERENCED_PARAMETER(pulSize);
    return ANSC_STATUS_SUCCESS;
#endif
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

    val[0].parameterName  = (char*)param;
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


void *XfinityWifiThread
    (
        void *arg
    )
{
    BOOL *pvalue = (BOOL*)arg;
    BOOL value = FALSE;
    
    if (!pvalue)
        return NULL;
    value = *pvalue;
    if (FALSE == value)
    {
        PsmSet(HOTSPOT_PSM_EANBLE, "0");
        //SSIDs 5 and 6 case
        if (DmSetBool("Device.WiFi.SSID.5.Enable", value) != ANSC_STATUS_SUCCESS) {
            fprintf(stderr, "%s: set WiFi.SSID.5 Disable error\n", __FUNCTION__);
        } else {
            fprintf(stderr, "%s: set WiFi.SSID.5 Disable OK\n", __FUNCTION__);
        }

        if (DmSetBool("Device.WiFi.SSID.6.Enable", value) != ANSC_STATUS_SUCCESS) {
            fprintf(stderr, "%s: set WiFi.SSID.6 Disable error\n", __FUNCTION__);
        } else {
            fprintf(stderr, "%s: set WiFi.SSID.6 Disable OK\n", __FUNCTION__);
        }
    	
        //SSIDs 9 and 10 case
        if (DmSetBool("Device.WiFi.SSID.9.Enable", value) != ANSC_STATUS_SUCCESS) {
            fprintf(stderr, "%s: set WiFi.SSID.9 Disable error\n", __FUNCTION__);
        } else {
            fprintf(stderr, "%s: set WiFi.SSID.9 Disable OK\n", __FUNCTION__);
        }
	
        if (DmSetBool("Device.WiFi.SSID.10.Enable", value) != ANSC_STATUS_SUCCESS) {
            fprintf(stderr, "%s: set WiFi.SSID.10 Disable error\n", __FUNCTION__);
        } else {
            fprintf(stderr, "%s: set WiFi.SSID.10 Diable OK\n", __FUNCTION__);
        }

        
        if (DmSetBool("Device.WiFi.SSID.16.Enable", value) != ANSC_STATUS_SUCCESS) {
            fprintf(stderr, "%s: set WiFi.SSID.16 Disable error\n", __FUNCTION__);
        } else {
            fprintf(stderr, "%s: set WiFi.SSID.16 Diable OK\n", __FUNCTION__);
        }
    }
    else
    {
         PsmSet(HOTSPOT_PSM_EANBLE, "1");
    }
        /*Removing webconfig files*/
        v_secure_system("rm /nvram/hotspot.json");

        v_secure_system("rm /nvram/hotspot_blob");
	
	if (g_SetParamValueBool("Device.X_COMCAST-COM_GRE.Tunnel.1.Enable", value) != ANSC_STATUS_SUCCESS) {
		fprintf(stderr, "%s: set X_COMCAST-COM_GRE.Tunnel.1.Enable error\n", __FUNCTION__);
        AnscTraceError(("%s: set X_COMCAST-COM_GRE.Tunnel.1.Enable error\n", __FUNCTION__));
        AnscFreeMemory(pvalue);
		return NULL;
    } else {
        AnscTraceWarning(("%s: set X_COMCAST-COM_GRE.Tunnel.1.Enable OK\n", __FUNCTION__));
    }
	if (g_SetParamValueBool("Device.X_COMCAST-COM_GRE.Tunnel.1.Interface.1.Enable", value) != ANSC_STATUS_SUCCESS) {
        AnscTraceError(("%s: set X_COMCAST-COM_GRE.Tunnel.1.Interface.1.Enable error\n", __FUNCTION__));
    } else {
        AnscTraceWarning(("%s: set X_COMCAST-COM_GRE.Tunnel.1.Interface.1.Enable OK\n", __FUNCTION__));
    }
	if (g_SetParamValueBool("Device.X_COMCAST-COM_GRE.Tunnel.1.Interface.2.Enable", value) != ANSC_STATUS_SUCCESS) {
        AnscTraceError(("%s: set X_COMCAST-COM_GRE.Tunnel.1.Interface.2.Enable error\n", __FUNCTION__));
    } else {
        AnscTraceWarning(("%s: set X_COMCAST-COM_GRE.Tunnel.1.Interface.2.Enable OK\n", __FUNCTION__));
    }
	//zqiu<<
    if (value) {
         //Update circuit ID here
        hotspot_update_circuit_ids(1,1); 
    }
    
    AnscFreeMemory(pvalue);
    pvalue = NULL;
    return NULL;
}

ANSC_STATUS
CosaDmlDiSetXfinityWiFiEnable
    (
        BOOL value
    )
{
    pthread_t thread_xfinity_wifi = 0;
    BOOL *pValue = NULL;
    
    /*CID: 61742 Wrong sizeof argument*/
    pValue =  AnscAllocateMemory(sizeof(BOOL));    
    if (pValue != NULL)
    {
           *pValue = value;
            pthread_create
            (
             &thread_xfinity_wifi,
             NULL,
             XfinityWifiThread,
             (void*)pValue
            );
            return ANSC_STATUS_SUCCESS;
     }
     else
     {
            return ANSC_STATUS_FAILURE;
     }
}
#endif

ANSC_STATUS
CosaDmlGetCaptivePortalEnable
    (
        BOOL *pValue
    )
{
	char buf[5];
        /* CID: 58774 Array compared against 0*/
        if(!syscfg_get( NULL, CAPTIVEPORTAL_EANBLE , buf, sizeof(buf)))
	{
    		if (strcmp(buf,"true") == 0)
		{
			CcspTraceWarning(("CaptivePortal: Captive Portal switch is enabled...\n"));		
    		       *pValue = true;
		}
    		else
		{
			CcspTraceWarning(("CaptivePortal: Captive Portal switch is disabled...\n"));		
    		        *pValue = false;	
		}
           return ANSC_STATUS_SUCCESS;

    	} else {
           CcspTraceWarning(("CaptivePortal: syscfg get error\n"));
           return ANSC_STATUS_FAILURE;
        }
}

ANSC_STATUS
CosaDmlSetCaptivePortalEnable
    (
        BOOL value
    )
{

	char buf[10];
	memset(buf,0,sizeof(buf));
	if (value)
	{
		strcpy(buf,"true");
		CcspTraceWarning(("CaptivePortal: Enabling Captive Portal switch ...\n"));		
	}
	else
	{
		CcspTraceWarning(("CaptivePortal: Disabling Captive Portal switch ...\n"));		
		strcpy(buf,"false");
	}
	if (syscfg_set(NULL, CAPTIVEPORTAL_EANBLE , buf) != 0) {
                     CcspTraceWarning(("syscfg_set failed to enable/disable captive portal\n"));
		     return ANSC_STATUS_FAILURE;
             } else {

                    if (syscfg_commit() != 0) {
                            CcspTraceWarning(("syscfg_commit failed\n"));
		     return ANSC_STATUS_FAILURE;
                    }
	  }

    v_secure_system("sh /etc/restart_services.sh %s",buf);
    /*commonSyseventSet("dhcp-server-restart", "");
    commonSyseventSet("firewall-restart", "");
    commonSyseventSet("zebra-restart", ""); */

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
	CosaDmlDiGetCloudCapable
	(
     	BOOL *pValue
	)
{
	int rc;
    char buf[5];

	memset(buf, 0, sizeof(buf));
	rc = syscfg_get( NULL, "cloud_capable_flag", buf, sizeof(buf));
    if( rc == 0 )
    {
        if (strcmp(buf,"1") == 0)
            *pValue = TRUE;
        else
    		*pValue = FALSE;
    }
	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlGetCloudUIReachableStatus
    (
		CHAR *pCloudPersonalizationURL,
        BOOL *pValue
    )
{
	ANSC_STATUS returnStatus   = ANSC_STATUS_SUCCESS;
	BOOL 		bProcessFuther = TRUE;
	/* Received Param Validation */
	if( NULL == pCloudPersonalizationURL )
	{
		*pValue        = FALSE;
		returnStatus   = ANSC_STATUS_FAILURE;
		bProcessFuther = FALSE;		
	}

	if( bProcessFuther )
	{
		FILE  *fp;
                char   retBuff[256]   = { 0 };
		int    curlResponse   = 0;
                
                fp = v_secure_popen( "r", "curl --connect-timeout 10 --interface erouter0 --write-out %%{http_code} --silent --output /dev/null '%s'", pCloudPersonalizationURL );

		if( NULL != fp )
		{
			fgets( retBuff, sizeof(retBuff), fp );
                        v_secure_pclose(fp);
			curlResponse = atoi(retBuff);

			CcspTraceInfo(("URL[ %s ] curlResponse[ %d ]\n",
									pCloudPersonalizationURL,
									curlResponse));
		
			if( ( curlResponse >= 200 ) && \
				( curlResponse < 400 ) 
			  )
			{
				*pValue = TRUE;
			}
			else
			{
				*pValue = FALSE;
			}
		}
		else
		{
			*pValue = FALSE;
		}
	}
	
    return returnStatus;
}

#if defined(INTEL_PUMA7) || defined(_XB6_PRODUCT_REQ_)
ANSC_STATUS
CosaDmlSetLED
    (
    	int color,
    	int state,
    	int interval
    )
{
    LEDMGMT_PARAMS ledMgmt;
    memset(&ledMgmt, 0, sizeof(LEDMGMT_PARAMS));

	ledMgmt.LedColor = color;
	ledMgmt.State	 = state;
	ledMgmt.Interval = interval;

	if(RETURN_ERR == platform_hal_setLed(&ledMgmt)) {
		CcspTraceWarning(("platform_hal_setLed failed\n"));
		return ANSC_STATUS_FAILURE;
	}

    return ANSC_STATUS_SUCCESS;	
}
#endif

