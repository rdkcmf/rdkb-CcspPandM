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

#define FEATURE_IPV6 1
#define XDNS_RESOLV_CONF "/etc/resolv.conf"
#define XDNS_DNSMASQ_SERVERS_CONF "/nvram/dnsmasq_servers.conf"
#define XDNS_DNSMASQ_SERVERS_BAK "/nvram/dnsmasq_servers.bak"
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

    // XDNS - get XDNS Enable/Disable flag
    if (AnscEqualString(ParamName, "X_RDKCENTRAL-COM_EnableXDNS", TRUE))
    {
        char buf[5] = {0};
        syscfg_get( NULL, "X_RDKCENTRAL-COM_XDNS", buf, sizeof(buf));
        if( buf != NULL )
        {
                if (strcmp(buf,"1") == 0)
                {
                        *pBool = TRUE;
                        return TRUE;
                }
        }

        *pBool = FALSE;

        return TRUE;
    }

 
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
            		CcspTraceInfo(("RDK_LOG_INFO, Ethernet WAN is enabled\n"));
			EthWANEnable = 1;
		}
			
#ifdef CONFIG_VENDOR_CUSTOMER_COMCAST
	if( AnscEqualString(ParamName, "X_COMCAST-COM_CM_MAC", TRUE))
	{
	        if( EthWANEnable )
        	{
		   CosaDmlDiGetRouterMacAddress(NULL, pValue,pulSize);        		
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

    if (AnscEqualString(ParamName, "X_RDKCENTRAL-COM_ConfigureWiFi", TRUE))
    {
	if ( bValue == TRUE )
	{

		CcspTraceWarning(("CaptivePortal:Wi-Fi SSID and Passphrase are not configured,setting ConfigureWiFi to true ...\n"));
		CcspTraceWarning(("RDKB_GW_MODE:Setting RDKB GW to CaptivePortal  ...\n"));
		printf("Wi-Fi SSID and Passphrase are not configured,setting ConfigureWiFi to true ...\n");
		pMyObject->bWiFiConfigued = bValue;

#if defined(INTEL_PUMA7) || defined(_XB6_PRODUCT_REQ_)
		char buf[5];
		syscfg_get( NULL, "CaptivePortal_Enable" , buf, sizeof(buf));
		if( buf != NULL )
		{
		    if (strcmp(buf,"true") == 0)
		    {
			if ( ANSC_STATUS_SUCCESS == CosaDmlSetLED(WHITE, BLINK, 1) )
				CcspTraceInfo(("Front LED Transition: WHITE LED will blink, Reason: CaptivePortal_MODE\n"));
		    }
		    else
		    {
			if ( ANSC_STATUS_SUCCESS == CosaDmlSetLED(WHITE, SOLID, 0) )
				CcspTraceInfo(("Front LED Transition: WHITE LED will be SOLID, Reason: ConfigureWiFi is TRUE, but CaptivePortal is disabled\n"));
		    }
		}
#endif

		printf("%s calling redirect_url.sh script to start redirection\n",__FUNCTION__);
		system("source /etc/redirect_url.sh &");
		return TRUE;
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

    // XDNS -  set XDNS Enable/Disable flag
    if (AnscEqualString(ParamName, "X_RDKCENTRAL-COM_EnableXDNS", TRUE))
    {   
    	char bval[2] = {0};
    	if( bValue == TRUE)
    	{
    		if(!SetXdnsConfig())
    			return FALSE;

    		bval[0] = '1';
    	}
    	else
    	{
			if(!UnsetXdnsConfig())
				return FALSE;

			bval[0] = '0';
		}


    	if (syscfg_set(NULL, "X_RDKCENTRAL-COM_XDNS", bval) != 0)
    	{
    		AnscTraceWarning(("[XDNS] syscfg_set X_RDKCENTRAL-COM_XDNS failed!\n"));
    	}
    	else
    	{
    		if (syscfg_commit() != 0)
    		{
    			AnscTraceWarning(("[XDNS] syscfg_commit X_RDKCENTRAL-COM_XDNS failed!\n"));
    		}
    		else
    		{
    			//Restart firewall to apply XDNS setting
    			commonSyseventSet("firewall-restart", "");
    		}
    	}

    	return TRUE;
    }   

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

// XDNS - Copy dnsoverride entries from dnsmasq_servers.conf to resolv.conf
//        Validate and cleanup dnsmasq_servers.conf entries
//        return 1 - if success in copy to resolv.conf
//        return 0 - for any error.
int SetXdnsConfig()
{
	char confEntry[256] = {0};
	char tempEntry[256] = {0};

	FILE *fp1 = NULL, *fp2 = NULL, *fp3 = NULL;

	fp1 = fopen(XDNS_RESOLV_CONF, "r"); // r mode - file must exist
	if(fp1 == NULL)
	{
		fprintf(stderr,"## XDNS : SetXdnsConfig() - fopen(XDNS_RESOLV_CONF, 'r') Error !!\n");
		return 0; // If resolv.conf does not exist return return fail.
	}

	fp2 = fopen(XDNS_DNSMASQ_SERVERS_CONF ,"r");
	if(fp2 == NULL)
	{
		fprintf(stderr,"## XDNS : SetXdnsConfig() - fopen(XDNS_DNSMASQ_SERVERS_CONF, 'r') Error !!\n");
		fclose(fp1); fp1 = NULL;
		return 0; //if dnsmasq_servers doesnt exist, return fail.
	}

	unlink(XDNS_DNSMASQ_SERVERS_BAK);

	fp3 = fopen(XDNS_DNSMASQ_SERVERS_BAK ,"a");
	if(fp3 == NULL)
	{
		fprintf(stderr,"## XDNS : SetXdnsConfig() - fopen(XDNS_DNSMASQ_SERVERS_BAK, 'a') Error !!\n");
		fclose(fp2); fp2 = NULL;
		fclose(fp1); fp1 = NULL;
		return 0;
	}

    //Get all entries (other than dnsoverride) from resolv.conf file//
    while(fgets(confEntry, sizeof(confEntry), fp1) != NULL)
    {
   	    if ( strstr(confEntry, "dnsoverride"))
		{
			continue;
		}

#if defined(_COSA_FOR_BCI_)

            if ( strstr(confEntry, "XDNS_Multi_Profile"))
                {
                        continue;
                }
#endif

    	// write resolv.conf entries to temp file
		printf("############ SetXdnsConfig() copy entry from resolv to temp: [%s]\n", confEntry);
        fprintf(fp3, "%s", confEntry);
    }

	// Get dnsoverride entries from dnsmasq_servers file. Validate IPs.
	// Look for default entry.
	int founddefault = 0;
	while(fgets(confEntry, sizeof(confEntry), fp2) != NULL)
	{
		//validate dnsoverride tokens
		// cleanup invalid entries in nvram leftover from ipv4 only stack and previous formats.

		strcpy(tempEntry, confEntry);
		int gotdefault = 0;

		char *token = strtok(tempEntry, " \t\n\r");

#if defined(_COSA_FOR_BCI_)
                if(!strcmp(token, "XDNS_Multi_Profile"))
                {
                        fprintf(fp3, "%s", confEntry);
                        continue;
                }
#endif

		if(token && strcmp(token, "dnsoverride") == 0)
		{
			char *macaddr = NULL, *srvaddr4 = NULL;
#ifdef FEATURE_IPV6
			char *srvaddr6 = NULL;
#endif
			if(!(macaddr = strtok(NULL, " \t\n\r")))
			{
				printf("############ SetXdnsConfig() mac check failed!\n");
				continue;
			}
			else
			{
				// check if default
				if(strcmp(macaddr, "00:00:00:00:00:00") == 0)
					gotdefault = 1;
			}

			if(!(srvaddr4 = strtok(NULL, " \t\n\r")))
			{
				printf("############ SetXdnsConfig() addr4 failed!\n");
				continue;
			}

			struct sockaddr_in sa;
			if (inet_pton(AF_INET, srvaddr4, &(sa.sin_addr)) != 1)
			{
				printf("############ SetXdnsConfig() addr4 check failed!: %s\n", srvaddr4);
				continue;
			}

#ifdef FEATURE_IPV6
			if(!(srvaddr6 = strtok(NULL, " \t\n\r")))
			{
				printf("############ SetXdnsConfig() addr6 failed!\n");
				continue;
			}

			struct sockaddr_in6 sa6;
			if (inet_pton(AF_INET6, srvaddr6, &(sa6.sin6_addr)) != 1)
			{
				printf("############ SetXdnsConfig() addr6 check failed!: %s\n", srvaddr6);
				continue;
			}
#endif

			if(gotdefault)
			{
				founddefault++;
				if(founddefault==1)
                                {
					CcspTraceWarning(("Enabling primary XDNS: %s\n", confEntry));
                                }
				else if(founddefault==2)
                                {
					CcspTraceWarning(("Enabling secondary XDNS: %s\n", confEntry));
                                }
                          	else
                                {
                                	CcspTraceWarning(("Logging Invalid XDNS parameter: %s\n", confEntry));
                                }

			}

			//copy validated entry to temp
			printf("############ SetXdnsConfig() copy entry from dnsmasq_servers to temp: [%s]\n", confEntry);
			fprintf(fp3, "%s", confEntry);
		}
	}

	fclose(fp3); fp3 = NULL;
	fclose(fp2); fp2 = NULL;
	fclose(fp1); fp1 = NULL;

	// check if we found exactly primary and secondary default dnsoverride entry,
	if(founddefault >= 1)
	{
		fp1 = fopen(XDNS_RESOLV_CONF, "w");
		if(fp1 == NULL)
		{
			fprintf(stderr,"## XDNS : SetXdnsConfig() - fopen(XDNS_RESOLV_CONF, 'w') Error !!\n");
			return 0;
		}

	}
	else // default corrupted, missing or more than 1 default
	{
		printf("############ SetXdnsConfig() Error: dnsmasq_servers has invalid default entry! cleanup.\n");
	}

	fp2 = fopen(XDNS_DNSMASQ_SERVERS_CONF,"w");
	if(fp2 == NULL)
	{
		fprintf(stderr,"## XDNS : SetXdnsConfig() - fopen(XDNS_DNSMASQ_SERVERS_CONF, 'w') Error !!\n");
		if(fp1) fclose(fp1); fp1 = NULL;
		return 0;
	}

	fp3 = fopen(XDNS_DNSMASQ_SERVERS_BAK ,"r");  //file must exist
	if(fp3 == NULL)
	{
		fprintf(stderr,"## XDNS : SetXdnsConfig() - fopen(XDNS_DNSMASQ_SERVERS_BAK, 'r') Error !!\n");
		fclose(fp2); fp2 = NULL;
		if(fp1) fclose(fp1); fp1 = NULL;
		return 0;
	}


	//copy back the cleaned up entries to nvram from temp
	// copy back to resolv.conf if default entry is not corrupt
	int gotdefault = 0;
	while(fgets(confEntry, sizeof(confEntry), fp3) != NULL)
	{
		//copy back entries to resolv.conf if default is found. else keep the old resolv.
		if(fp1)
		{
			printf("############ SetXdnsConfig() copy to resolv: [%s]\n", confEntry);
			fprintf(fp1, "%s", confEntry);
		}

		//copy only dnsoverride entries (pruned) into nvram
		if (strstr(confEntry, "dnsoverride"))
		{
			
			printf("############ SetXdnsConfig() copy to dnsmasq_servers: [%s]\n", confEntry);
			fprintf(fp2, "%s", confEntry);
		}

#if defined(_COSA_FOR_BCI_)
                if (strstr(confEntry, "XDNS_Multi_Profile"))
                {

                        printf("############ SetXdnsConfig() copy to dnsmasq_servers: [%s]\n", confEntry);
                        fprintf(fp2, "%s", confEntry);
                }
#endif

	}

	if(fp3) fclose(fp3); fp3 = NULL;
	if(fp2) fclose(fp2); fp2 = NULL;
	if(fp1) fclose(fp1); fp1 = NULL;

	if(founddefault >= 1)
		return 1; //success
	else
		return 0; //error
}


// XDNS - UnetXdnsConfig: Delete all dnsoverride entries from resolv.conf
int UnsetXdnsConfig()
{
	char confEntry[256] = {0};

	FILE *fp1 = NULL, *fp3 = NULL;

	fp1 = fopen(XDNS_RESOLV_CONF, "r");
	if(fp1 == NULL) // if we cannot open resolv.conf, return success.
	{
		return 1;
	}

	// 1. copy all non-dnsoverride entries from resolv.conf to temp file
	// 2. clear resolv.conf file by opening in 'w' mode
	// 3. copy all contents from temp file to resolv.conf

	unlink(XDNS_DNSMASQ_SERVERS_BAK);

	fp3 = fopen(XDNS_DNSMASQ_SERVERS_BAK ,"a");
	if(fp3 == NULL)
	{
		fprintf(stderr,"## XDNS : UnsetXdnsConfig() - fopen(XDNS_DNSMASQ_SERVERS_BAK, 'a') Error !!\n");
		fclose(fp1);
		return 0;
	}

	while(fgets(confEntry, sizeof(confEntry), fp1) != NULL)
	{
		if ( strstr(confEntry, "dnsoverride"))
		{
			continue; //skip
		}

#if defined(_COSA_FOR_BCI_)

            if ( strstr(confEntry, "XDNS_Multi_Profile"))
                {
                        continue;
                }
#endif

		printf("############ UnsetXdnsConfig() saving from resolv.conf to bak [%s]\n", confEntry);
		fprintf(fp3, "%s", confEntry);
	}

	// now all entries (non-dnsoverride) from resolv.conf is saved to bak
	// copy back
	fclose(fp3); fp3 = NULL;
	fclose(fp1); fp1 = NULL;
	
	fp1 = fopen(XDNS_RESOLV_CONF, "w");
	if(fp1 == NULL)
	{
		fprintf(stderr,"## XDNS : UnsetXdnsConfig() - fopen(XDNS_RESOLV_CONF, 'w') Error !!\n");
		return 0;
	}

	fp3 = fopen(XDNS_DNSMASQ_SERVERS_BAK ,"r");  //file must exist
	if(fp3 == NULL)
	{
		fprintf(stderr,"## XDNS : UnsetXdnsConfig() - fopen(XDNS_DNSMASQ_SERVERS_BAK, 'r') Error !!\n");
		fclose(fp1); fp1 = NULL;
		return 0;
	}

	while(fgets(confEntry, sizeof(confEntry), fp3) != NULL)
	{
		printf("############ UnsetXdnsConfig() reading from bak and writing to resolv.conf[%s]\n", confEntry);
		fprintf(fp1, "%s", confEntry);
	}
	
  	CcspTraceWarning(("############ Disabled XDNS#######\n"));
	fclose(fp3); fp3 = NULL;
	fclose(fp1); fp1 = NULL;
	return 1;
}



