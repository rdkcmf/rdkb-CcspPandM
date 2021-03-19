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
#include <syscfg/syscfg.h>

#define WHITE	0
#define SOLID	0
#define BLINK	1



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
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

#ifdef CONFIG_INTERNET2P0
    if (AnscEqualString(ParamName, "X_RDKCENTRAL-COM_ConfigureWiFi", TRUE))
    {
       *pBool = pMyObject->bWiFiConfigued;
	return TRUE;
    }
    if (AnscEqualString(ParamName, "X_RDKCENTRAL-COM_WiFiNeedsPersonalization",TRUE))
    {
	    char buf[5];
        syscfg_get( NULL, "redirection_flag", buf, sizeof(buf));
    	if( buf != NULL )
    	{
    		if (strcmp(buf,"true") == 0)
    		        *pBool = TRUE;
    		    else
    		        *pBool = FALSE;
    	}
	    return TRUE;
    }
	    if (AnscEqualString(ParamName, "X_RDKCENTRAL-COM_CaptivePortalEnable", TRUE))
    {
 //      *pBool = pMyObject->bCaptivePortalEnable;
        if (CosaDmlGetCaptivePortalEnable(&pMyObject->bCaptivePortalEnable) != ANSC_STATUS_SUCCESS)
            return FALSE;
       *pBool = pMyObject->bCaptivePortalEnable;
	return TRUE;
    }
    if (AnscEqualString(ParamName, "X_RDKCENTRAL-COM_CloudUICapable", TRUE))
    {

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
//        if (CosaDmlDiGetXfinityWiFiEnable(pBool) != ANSC_STATUS_SUCCESS)
//            return FALSE;
			// printf("%s : bxfinitywifiEnable value is : %d\n",__FUNCTION__,pMyObject->bxfinitywifiEnable);
			*pBool = pMyObject->bxfinitywifiEnable;
        return TRUE;
    }
#endif

    if (AnscEqualString(ParamName, "X_COMCAST-COM_rdkbPlatformCapable", TRUE))
    {
       *pBool = TRUE;
	    return TRUE;
    }

    if (AnscEqualString(ParamName, "X_RDKCENTRAL-COM_IsCloudReachable", TRUE))
    {
	CcspTraceWarning(("IsCloudReachable: %s \n",pMyObject->CloudPersonalizationURL));

	*pBool = FALSE;
	/* Get the CloudURL reachable status */
	if ( ANSC_STATUS_SUCCESS != CosaDmlGetCloudUIReachableStatus( pMyObject->CloudPersonalizationURL,
			  pBool ) )
	{
            *pBool = FALSE;
            return FALSE;
	}

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
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    char isEthEnabled[64]={'\0'};
    int EthWANEnable = 0;
#ifdef CONFIG_INTERNET2P0   
    if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_CloudUIWebURL", TRUE))
    { 
	syscfg_get(NULL, "redirection_url", pMyObject->WebURL, sizeof(pMyObject->WebURL));
	AnscCopyString(pValue, pMyObject->WebURL);
        return 0;
    } 
#endif
        	if( 0 == syscfg_get( NULL, "eth_wan_enabled", isEthEnabled, sizeof(isEthEnabled)) && (isEthEnabled[0] != '\0' && strncmp(isEthEnabled, "true", strlen("true")) == 0))
        	{
			EthWANEnable = 1;
		}
			
#ifdef CONFIG_VENDOR_CUSTOMER_COMCAST
	if( AnscEqualString(ParamName, "X_COMCAST-COM_CM_MAC", TRUE))
	{
	        if( EthWANEnable )
        	{
#if defined (_HUB4_PRODUCT_REQ_)
		   CosaDmlDiGetRouterMacAddress(NULL, pValue,pulSize);
#else
                   CosaDmlDiGetEwanCMMacAddress(NULL, pValue,pulSize);
#endif /* _HUB4_PRODUCT_REQ_ */
        	}
		else
		{
		   CosaDmlDiGetCMMacAddress(NULL, pValue,pulSize);
		}
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
#if !defined(_PLATFORM_RASPBERRYPI_) && !defined(_PLATFORM_TURRIS_)
  	   CosaDmlDiGetMTAMacAddress(NULL, pValue,pulSize);
#endif
	   return 0;
	}

	if( AnscEqualString(ParamName, "X_COMCAST-COM_CM_IP", TRUE))
	{
        	if( EthWANEnable )
        	{
			CosaDmlDiGetRouterIPv6Address(NULL, pValue,pulSize);
				
			if(strlen(pValue) == 0)
				CosaDmlDiGetRouterIPAddress(NULL, pValue,pulSize);

            		
        	}
		else
		{
	   		CosaDmlDiGetCMIPAddress(NULL, pValue,pulSize);
		}
	   return 0;
	}

	if( AnscEqualString(ParamName, "X_COMCAST-COM_WAN_IP", TRUE))
	{
	   CosaDmlDiGetRouterIPAddress(NULL, pValue,pulSize);
	   return 0;
	}

	if( AnscEqualString(ParamName, "X_COMCAST-COM_WAN_IPv6", TRUE))
	{
#if !defined(_PLATFORM_RASPBERRYPI_) && !defined(_PLATFORM_TURRIS_)
	   CosaDmlDiGetRouterIPv6Address(NULL, pValue,pulSize);
           #if defined(_COSA_FOR_BCI_)
           if (syscfg_set(NULL, "wanIPv6Address",pValue) != 0) {
               AnscTraceWarning(("syscfg_set failed\n"));
           } else {
               if (syscfg_commit() != 0) {
                   AnscTraceWarning(("syscfg_commit failed\n"));
               }
           }
           #endif
#endif
	   return 0;
	}

	if( AnscEqualString(ParamName, "X_COMCAST-COM_MTA_IP", TRUE))
	{
#if !defined(_PLATFORM_RASPBERRYPI_) && !defined(_PLATFORM_TURRIS_)
   	   CosaDmlDiGetMTAIPAddress(NULL, pValue,pulSize);
#endif
	   return 0;
	}

	if( AnscEqualString(ParamName, "X_COMCAST-COM_MTA_IPV6", TRUE))
	{
#if !defined(_PLATFORM_RASPBERRYPI_)
   	   CosaDmlDiGetMTAIPV6Address(NULL, pValue,pulSize);
#endif
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
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
#ifdef CONFIG_INTERNET2P0

    if (AnscEqualString(ParamName, "X_RDKCENTRAL-COM_ConfigureWiFi", TRUE))
    {
	if ( bValue == TRUE )
	{

		CcspTraceWarning(("CaptivePortal:Wi-Fi SSID and Passphrase are not configured,setting ConfigureWiFi to true ...\n"));
		CcspTraceWarning(("RDKB_GW_MODE:Setting RDKB GW to CaptivePortal  ...\n"));
		printf("Wi-Fi SSID and Passphrase are not configured,setting ConfigureWiFi to true ...\n");
		pMyObject->bWiFiConfigued = bValue;

		char buf[5];
		syscfg_get( NULL, "CaptivePortal_Enable" , buf, sizeof(buf));
		if( buf != NULL )
		{
		    if (strcmp(buf,"true") == 0)
		    {
#if defined(INTEL_PUMA7) || defined(_XB6_PRODUCT_REQ_)
			if ( ANSC_STATUS_SUCCESS == CosaDmlSetLED(WHITE, BLINK, 1) )
				CcspTraceInfo(("Front LED Transition: WHITE LED will blink, Reason: CaptivePortal_MODE\n"));
#endif
			    //if CaptivePortal_Enable is true, Then only we need to run redirect_url.sh
			    printf("%s calling redirect_url.sh script to start redirection\n",__FUNCTION__);
			    system("source /etc/redirect_url.sh &");
			    return TRUE;
		    }
#if defined(INTEL_PUMA7) || defined(_XB6_PRODUCT_REQ_)
		    else
		    {
			if ( ANSC_STATUS_SUCCESS == CosaDmlSetLED(WHITE, SOLID, 0) )
				CcspTraceInfo(("Front LED Transition: WHITE LED will be SOLID, Reason: ConfigureWiFi is TRUE, but CaptivePortal is disabled\n"));
		    }
#endif
		}
	 }
	
         else if  ( bValue == FALSE )
	 {
#if defined(INTEL_PUMA7) || defined(_XB6_PRODUCT_REQ_)
	    FILE *responsefd=NULL;
	    char *networkResponse = "/var/tmp/networkresponse.txt";
	    int iresCode = 0;
	    char responseCode[10];

            if((responsefd = fopen(networkResponse, "r")) != NULL)
            {
                if(fgets(responseCode, sizeof(responseCode), responsefd) != NULL)
                {
                    iresCode = atoi(responseCode);
                }

                fclose(responsefd);
                responsefd = NULL;
            }
	if(iresCode == 204)
	{	
        	if ( ANSC_STATUS_SUCCESS == CosaDmlSetLED(WHITE, SOLID, 0) )
            		CcspTraceInfo(("Front LED Transition: WHITE LED will be SOLID, Reason: Gateway_MODE\n"));
	}
#endif

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

    if (AnscEqualString(ParamName, "X_RDKCENTRAL-COM_CaptivePortalEnable", TRUE))
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
        pMyObject->bxfinitywifiEnable = bValue;
		printf("%s : bxfinitywifiEnable value is : %d\n",__FUNCTION__,pMyObject->bxfinitywifiEnable);
        return TRUE;
    }
#endif

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}
