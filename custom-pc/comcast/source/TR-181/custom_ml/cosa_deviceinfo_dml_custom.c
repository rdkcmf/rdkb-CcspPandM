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

    module: cosa_deviceinfo_apis_custom.h

    For Data Model Implementation,
    Common Component Software Platform (CCSP)

    ---------------------------------------------------------------

    description:

        The prototypes of custom data model APIs are defined here

    ---------------------------------------------------------------

    environment:

        Maybe platform independent, but customer specific

    ---------------------------------------------------------------

    author:

        Ding Hua

    ---------------------------------------------------------------

    revision:

        05/08/2014  initial revision.

**************************************************************************/

#include "dml_tr181_custom_cfg.h" 
#include "cosa_deviceinfo_dml.h"
#include "cosa_deviceinfo_apis_custom.h"
#define SYSCFG_FILE  "/nvram/syscfg.db"
//For PSM Access
extern ANSC_HANDLE bus_handle;
extern char g_Subsystem[32];
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DeviceInfo_GetParamBoolValue_Custom
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL*                       pBool
            );

    description:

        This function is called to retrieve Boolean parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL*                       pBool
                The buffer of returned boolean value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
DeviceInfo_GetParamBoolValue_Custom
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    char *param_value = NULL;

#ifdef CONFIG_INTERNET2P0
    if (strcmp(ParamName, "X_RDKCENTRAL-COM_ConfigureWiFi") == 0)
    {
        PSM_Get_Record_Value2(bus_handle,g_Subsystem, "Device.DeviceInfo.X_RDKCENTRAL-COM_ConfigureWiFi", NULL, &param_value);
	if(strcmp(param_value,"true") == 0)
		pMyObject->bWiFiConfigued = TRUE;
	else
		pMyObject->bWiFiConfigued = FALSE;
       *pBool = pMyObject->bWiFiConfigued;
	return TRUE;
    }
    if (strcmp(ParamName, "X_RDKCENTRAL-COM_WiFiNeedsPersonalization") == 0)
    {
	    char buf[5];
        /*CID: 63071 Array compared against 0*/
        if(!syscfg_get( NULL, "redirection_flag", buf, sizeof(buf)))
    	{
    		if (strcmp(buf,"true") == 0)
    		        *pBool = TRUE;
    		    else
    		        *pBool = FALSE;
    	} else {
            return FALSE;
        }
	    return TRUE;
    }
	    if (strcmp(ParamName, "X_RDKCENTRAL-COM_CaptivePortalEnable") == 0)
    {
 //      *pBool = pMyObject->bCaptivePortalEnable;
        if (CosaDmlGetCaptivePortalEnable(&pMyObject->bCaptivePortalEnable) != ANSC_STATUS_SUCCESS)
            return FALSE;
       *pBool = pMyObject->bCaptivePortalEnable;
	return TRUE;
    }
    if (strcmp(ParamName, "X_RDKCENTRAL-COM_CloudUICapable") == 0)
    {

	    *pBool = pMyObject->bCloudCapable;
	     return TRUE;

    }
    if (strcmp(ParamName, "X_RDKCENTRAL-COM_CloudUIEnable") == 0)
    {
	*pBool = pMyObject->bCloudEnable;
	return TRUE;
    }
#endif
 
#ifdef CONFIG_CISCO_HOTSPOT
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "X_COMCAST-COM_xfinitywifiCapableCPE") == 0)
    {
        if (CosaDmlDiGetXfinityWiFiCapable(pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }
    if (strcmp(ParamName, "X_COMCAST_COM_xfinitywifiEnable") == 0)
    {
         if (CosaDmlDiGetXfinityWiFiEnable(pBool) != ANSC_STATUS_SUCCESS)
             return FALSE;
			// printf("%s : bxfinitywifiEnable value is : %d\n",__FUNCTION__,pMyObject->bxfinitywifiEnable);
			//*pBool = pMyObject->bxfinitywifiEnable;
        return TRUE;
    }
#endif

    if (strcmp(ParamName, "X_COMCAST-COM_rdkbPlatformCapable") == 0)
    {
       *pBool = TRUE;
	    return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        DeviceInfo_GetParamStringValue_Custom
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pValue,
                ULONG*                      pUlSize
            );

    description:

        This function is called to retrieve string parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue,
                The string value buffer;

                ULONG*                      pUlSize
                The buffer of length of string value;
                Usually size of 1023 will be used.
                If it's not big enough, put required size here and return 1;

    return:     0 if succeeded;
                1 if short of buffer size; (*pUlSize = required size)
                -1 if not supported.

**********************************************************************/
ULONG
DeviceInfo_GetParamStringValue_Custom
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

#ifdef CONFIG_INTERNET2P0   
    if (strcmp(ParamName, "X_RDKCENTRAL-COM_CloudUIWebURL") == 0)
    { 
	syscfg_get(NULL, "redirection_url", pMyObject->WebURL, sizeof(pMyObject->WebURL));
	AnscCopyString(pValue, pMyObject->WebURL);
        return 0;
    } 
#endif

#ifdef CONFIG_VENDOR_CUSTOMER_COMCAST
	if (strcmp(ParamName, "X_COMCAST-COM_CM_MAC") == 0)
	{
	   CosaDmlDiGetRouterMacAddress(NULL, pValue,pulSize);//Emulator doesn't have support of cable modem
	   return 0;
	}

	if (strcmp(ParamName, "X_COMCAST-COM_WAN_MAC") == 0)
	{
	   CosaDmlDiGetRouterMacAddress(NULL, pValue,pulSize);
	   return 0;
	}

	if (strcmp(ParamName, "X_COMCAST-COM_AP_MAC") == 0)
	{
	   CosaDmlDiGetRouterMacAddress(NULL, pValue,pulSize);
	   return 0;
	}

	if (strcmp(ParamName, "X_COMCAST-COM_MTA_MAC") == 0)
	{
	   CosaDmlDiGetMTAMacAddress(NULL, pValue,pulSize);
	   return 0;
	}

	if (strcmp(ParamName, "X_COMCAST-COM_CM_IP") == 0)
	{
	   CosaDmlDiGetRouterIPAddress(NULL, pValue,pulSize);
	   return 0;
	}

	if (strcmp(ParamName, "X_COMCAST-COM_WAN_IP") == 0)
	{
	   CosaDmlDiGetRouterIPAddress(NULL, pValue,pulSize);
	   return 0;
	}

	if (strcmp(ParamName, "X_COMCAST-COM_WAN_IPv6") == 0)
	{
	   CosaDmlDiGetRouterIPv6Address(NULL, pValue,pulSize);
	   return 0;
	}

	if (strcmp(ParamName, "X_COMCAST-COM_MTA_IP") == 0)
	{
	   CosaDmlDiGetMTAIPAddress(NULL, pValue,pulSize);
	   return 0;
	}

	if (strcmp(ParamName, "X_COMCAST-COM_MTA_IPV6") == 0)
	{
	   CosaDmlDiGetMTAIPV6Address(NULL, pValue,pulSize);
	   return 0;
	}
#endif

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DeviceInfo_SetParamBoolValue_Custom
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL                        bValue
            );

    description:

        This function is called to set BOOL parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL                        bValue
                The updated BOOL value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL
DeviceInfo_SetParamBoolValue_Custom
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{

    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

#ifdef CONFIG_INTERNET2P0

    if (strcmp(ParamName, "X_RDKCENTRAL-COM_ConfigureWiFi") == 0)
    {
	if ( bValue == TRUE )
	{
		CcspTraceWarning(("CaptivePortal:Wi-Fi SSID and Passphrase are not configured,setting ConfigureWiFi to true ...\n"));
		CcspTraceWarning(("RDKB_GW_MODE:Setting RDKB GW to CaptivePortal  ...\n"));
		printf("Wi-Fi SSID and Passphrase are not configured,setting ConfigureWiFi to true ...\n");
			pMyObject->bWiFiConfigued = bValue;

			printf("%s calling redirect_url.sh script to start redirection\n",__FUNCTION__);
			system("source /etc/redirect_url.sh &");

		return TRUE;
	 }
	
         else if  ( bValue == FALSE )
	 {
		CcspTraceWarning(("CaptivePortal:Wi-Fi SSID and Passphrase are configured,setting ConfigureWiFi to false ...\n"));
		CcspTraceWarning(("RDKB_GW_MODE:Setting RDKB GW to Online  ...\n"));
		printf("Wi-Fi SSID and Passphrase are configured,setting ConfigureWiFi to false ...\n");

			pMyObject->bWiFiConfigued = bValue;
			printf("%s calling revert_redirect.sh script to remove the redirection changes\n",__FUNCTION__);
			system("source /etc/revert_redirect.sh &");
	    return TRUE;
	 }	
	return FALSE;
    } 

    if (strcmp(ParamName, "X_RDKCENTRAL-COM_CaptivePortalEnable") == 0)
    {
	if( pMyObject->bCaptivePortalEnable == bValue )
	{
		return TRUE;	
	}
        if (CosaDmlSetCaptivePortalEnable(bValue) != ANSC_STATUS_SUCCESS)
            return FALSE;
     	pMyObject->bCaptivePortalEnable = bValue;
        return TRUE;
    }

    if (strcmp(ParamName, "X_RDKCENTRAL-COM_CloudUICapable") == 0)
    {
       // We should not allow SET of Capable flag.

	  if( bValue == TRUE) {

             if (_set_db_value(SYSCFG_FILE, "cloud_capable_flag", "1") != 0) {
                     AnscTraceWarning(("syscfg_set failed\n"));
             } else {
		    pMyObject->bCloudCapable = bValue;
             }

         } else {

             if (_set_db_value(SYSCFG_FILE, "cloud_capable_flag", "0") != 0) {
                     AnscTraceWarning(("syscfg_set failed\n"));
             }  else {

		  pMyObject->bCloudCapable = bValue;
             }
         }

	return TRUE;


    }
    if (strcmp(ParamName, "X_RDKCENTRAL-COM_CloudUIEnable") == 0)
    {
        

	if ( pMyObject->bCloudCapable == TRUE )
	{

          if( bValue == TRUE) {

             if (_set_db_value(SYSCFG_FILE, "cloud_enable_flag", "1") != 0) {
                     AnscTraceWarning(("syscfg_set failed\n"));
             } else {

			pMyObject->bCloudEnable = bValue;
			CcspTraceWarning(("CaptivePortal:Enabling CloudUIEnable to start redirection to Cloud URL ...\n"));
             }

         } else {

             if (_set_db_value(SYSCFG_FILE, "cloud_enable_flag", "0") != 0) {
                     AnscTraceWarning(("syscfg_set failed\n"));
             }  else {

			pMyObject->bCloudEnable = bValue;
			CcspTraceWarning(("CaptivePortal:Disabling CloudUIEnable to stop redirection to Cloud URL ...\n"));
             }
         }
	}
	else
	{
		printf("First enable cloud capable to modify this parameter\n");
		return FALSE;
	}
	return TRUE;
    }
#endif

#ifdef CONFIG_CISCO_HOTSPOT
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "X_COMCAST_COM_xfinitywifiEnable") == 0)
    {
        if (CosaDmlDiSetXfinityWiFiEnable(bValue) != ANSC_STATUS_SUCCESS)
            return FALSE;
        pMyObject->bxfinitywifiEnable = bValue;
		printf("%s : bxfinitywifiEnable value is : %d\n",__FUNCTION__,pMyObject->bxfinitywifiEnable);
        return TRUE;
    }
#endif

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

