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

    Copyright (c) 2011 - 2013, Cisco Systems, Inc.

                    CISCO CONFIDENTIAL
      Unauthorized distribution or copying is prohibited
                    All rights reserved

    No part of this computer software may be reprinted, reproduced or utilized
    in any form or by any electronic, mechanical, or other means, now known or
    hereafter invented, including photocopying and recording, or using any
    information storage and retrieval system, without permission in writing
    from Cisco Systems, Inc.

    -------------------------------------------------------------------

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

#ifdef CONFIG_INTERNET2.0
    if (AnscEqualString(ParamName, "X_RDKCENTRAL-COM_ConfigureWiFi", TRUE))
    {
       *pBool = pMyObject->bWiFiConfigued;
	return TRUE;
    }
    if (AnscEqualString(ParamName, "X_RDKCENTRAL-COM_CloudUICapable", TRUE))
    {
/*       *pBool = pMyObject->bCloudCapable;
	return TRUE; */

	char buf[5];
        syscfg_get( NULL, "cloud_capable_flag", buf, sizeof(buf));
    	if( buf != NULL )
    		{
    		    if (strcmp(buf,"1") == 0)
    		        pMyObject->bCloudCapable = TRUE;
    		    else
    		        pMyObject->bCloudCapable = FALSE;
    		}
	*pBool = pMyObject->bCloudCapable;
	return TRUE;

    }
    if (AnscEqualString(ParamName, "X_RDKCENTRAL-COM_CloudUIEnable", TRUE))
    {
       *pBool = pMyObject->bCloudEnable;
	char buf[5];
        syscfg_get( NULL, "cloud_enable_flag", buf, sizeof(buf));
    	if( buf != NULL )
    		{
    		    if (strcmp(buf,"1") == 0)
    		        pMyObject->bCloudEnable = TRUE;
    		    else
    		        pMyObject->bCloudEnable = FALSE;
    		}
	*pBool = pMyObject->bCloudEnable;
	return TRUE;
    }
#endif
 
#ifdef CONFIG_CISCO_HOTSPOT
    /* check the parameter name and return the corresponding value */
    if (AnscEqualString(ParamName, "X_COMCAST-COM_xfinitywifiCapableCPE", TRUE))
    {
        if (CosaDmlDiGetXfinityWiFiCapable(pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "X_COMCAST_COM_xfinitywifiEnable", TRUE))
    {
        if (CosaDmlDiGetXfinityWiFiEnable(pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }
#endif

    if (AnscEqualString(ParamName, "X_COMCAST-COM_rdkbPlatformCapable", TRUE))
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

#ifdef CONFIG_INTERNET2.0   
    if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_CloudUIWebURL", TRUE))
    { 
	syscfg_get(NULL, "redirection_url", pMyObject->WebURL, sizeof(pMyObject->WebURL));
	AnscCopyString(pValue, pMyObject->WebURL);
        return 0;
    } 
#endif

#ifdef CONFIG_VENDOR_CUSTOMER_COMCAST
	if( AnscEqualString(ParamName, "X_COMCAST-COM_CM_MAC", TRUE))
	{
	   CosaDmlDiGetCMMacAddress(NULL, pValue,pulSize);
	   return 0;
	}

	if( AnscEqualString(ParamName, "X_COMCAST-COM_WAN_MAC", TRUE))
	{
	   CosaDmlDiGetRouterMacAddress(NULL, pValue,pulSize);
	   return 0;
	}

	if( AnscEqualString(ParamName, "X_COMCAST-COM_AP_MAC", TRUE))
	{
	   CosaDmlDiGetRouterMacAddress(NULL, pValue,pulSize);
	   return 0;
	}

	if( AnscEqualString(ParamName, "X_COMCAST-COM_MTA_MAC", TRUE))
	{
	   CosaDmlDiGetMTAMacAddress(NULL, pValue,pulSize);
	   return 0;
	}

	if( AnscEqualString(ParamName, "X_COMCAST-COM_CM_IP", TRUE))
	{
	   CosaDmlDiGetCMIPAddress(NULL, pValue,pulSize);
	   return 0;
	}

	if( AnscEqualString(ParamName, "X_COMCAST-COM_WAN_IP", TRUE))
	{
	   CosaDmlDiGetRouterIPAddress(NULL, pValue,pulSize);
	   return 0;
	}

	if( AnscEqualString(ParamName, "X_COMCAST-COM_MTA_IP", TRUE))
	{
	   CosaDmlDiGetMTAIPAddress(NULL, pValue,pulSize);
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

#ifdef CONFIG_INTERNET2.0

    if (AnscEqualString(ParamName, "X_RDKCENTRAL-COM_ConfigureWiFi", TRUE))
    {
	if ( bValue == TRUE )
	{
	   if ( pMyObject->bWiFiConfigued == TRUE )
		{
			printf("Already set to TRUE, do nothing\n");
		}
	   else
		{
		CcspTraceWarning(("CaptivePortal:Wi-Fi SSID and Passphrase are not configured,setting ConfigureWiFi to true ...\n"));
		CcspTraceWarning(("RDKB_GW_MODE:Setting RDKB GW to CaptivePortal  ...\n"));
		printf("Wi-Fi SSID and Passphrase are not configured,setting ConfigureWiFi to true ...\n");
			pMyObject->bWiFiConfigued = bValue;

			printf("%s calling redirect_url.sh script to start redirection\n",__FUNCTION__);
			system("source /etc/redirect_url.sh &");

		}
		return TRUE;
	 }
	
         else if  ( bValue == FALSE )
	 {
                if ( pMyObject->bWiFiConfigued == FALSE )
		{
			printf("Already set to FALSE, do nothing\n");
		}
		else
		{
		CcspTraceWarning(("CaptivePortal:Wi-Fi SSID and Passphrase are configured,setting ConfigureWiFi to false ...\n"));
		CcspTraceWarning(("RDKB_GW_MODE:Setting RDKB GW to Online  ...\n"));
		printf("Wi-Fi SSID and Passphrase are configured,setting ConfigureWiFi to false ...\n");

			pMyObject->bWiFiConfigued = bValue;
			printf("%s calling revert_redirect.sh script to remove the redirection changes\n",__FUNCTION__);
			system("source /etc/revert_redirect.sh &");
		}
	    return TRUE;
	 }	
	return FALSE;
    } 

    if (AnscEqualString(ParamName, "X_RDKCENTRAL-COM_CloudUICapable", TRUE))
    {
       // We should not allow SET of Capable flag.
#if 0 
	  if( bValue == TRUE) {

             if (syscfg_set(NULL, "cloud_capable_flag", "1") != 0) {
                     AnscTraceWarning(("syscfg_set failed\n"));
             } else {

                    if (syscfg_commit() != 0) {
                            AnscTraceWarning(("syscfg_commit failed\n"));
                    }
		    pMyObject->bCloudCapable = bValue;
             }

         } else {

             if (syscfg_set(NULL, "cloud_capable_flag", "0") != 0) {
                     AnscTraceWarning(("syscfg_set failed\n"));
             }  else {

                 if (syscfg_commit() != 0) {
                     AnscTraceWarning(("syscfg_commit failed\n"));
                 }
		  pMyObject->bCloudCapable = bValue;
             }
         }
#endif
	return TRUE;


    }
    if (AnscEqualString(ParamName, "X_RDKCENTRAL-COM_CloudUIEnable", TRUE))
    {
        

	if ( pMyObject->bCloudCapable == TRUE )
	{

          if( bValue == TRUE) {

             if (syscfg_set(NULL, "cloud_enable_flag", "1") != 0) {
                     AnscTraceWarning(("syscfg_set failed\n"));
             } else {

                    if (syscfg_commit() != 0) {
                            AnscTraceWarning(("syscfg_commit failed\n"));
                    }
			pMyObject->bCloudEnable = bValue;
			CcspTraceWarning(("CaptivePortal:Enabling CloudUIEnable to start redirection to Cloud URL ...\n"));
             }

         } else {

             if (syscfg_set(NULL, "cloud_enable_flag", "0") != 0) {
                     AnscTraceWarning(("syscfg_set failed\n"));
             }  else {

                 if (syscfg_commit() != 0) {
                     AnscTraceWarning(("syscfg_commit failed\n"));
                 }
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
    if (AnscEqualString(ParamName, "X_COMCAST_COM_xfinitywifiEnable", TRUE))
    {
        if (CosaDmlDiSetXfinityWiFiEnable(bValue) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }
#endif

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

