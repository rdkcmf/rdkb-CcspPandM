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

    module: cosa_deviceinfo_dml.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    copyright:

        Cisco Systems, Inc.
        All Rights Reserved.

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

    -------------------------------------------------------------------

    environment:

        platform independent

    -------------------------------------------------------------------

    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        01/14/2011    initial revision.

**************************************************************************/

#include "cosa_deviceinfo_dml.h"
#include "dml_tr181_custom_cfg.h"

#if defined (_ARRIS_XB6_PRODUCT_REQ_) //ARRISXB6-7328, ARRISXB6-7332
#include "cm_hal.h"
#include "cm_hal_oem.h"
#endif

extern ANSC_HANDLE bus_handle;
extern char g_Subsystem[32];
/***********************************************************************
 IMPORTANT NOTE:

 According to TR69 spec:
 On successful receipt of a SetParameterValues RPC, the CPE MUST apply 
 the changes to all of the specified Parameters atomically. That is, either 
 all of the value changes are applied together, or none of the changes are 
 applied at all. In the latter case, the CPE MUST return a fault response 
 indicating the reason for the failure to apply the changes. 
 
 The CPE MUST NOT apply any of the specified changes without applying all 
 of them.

 In order to set parameter values correctly, the back-end is required to
 hold the updated values until "Validate" and "Commit" are called. Only after
 all the "Validate" passed in different objects, the "Commit" will be called.
 Otherwise, "Rollback" will be called instead.

 The sequence in COSA Data Model will be:

 SetParamBoolValue/SetParamIntValue/SetParamUlongValue/SetParamStringValue
 -- Backup the updated values;

 if( Validate_XXX())
 {
     Commit_XXX();    -- Commit the update all together in the same object
 }
 else
 {
     Rollback_XXX();  -- Remove the update at backup;
 }
 
***********************************************************************/

/***********************************************************************

 APIs for Object:

    DeviceInfo.

    *  DeviceInfo_GetParamBoolValue
    *  DeviceInfo_GetParamIntValue
    *  DeviceInfo_GetParamUlongValue
    *  DeviceInfo_GetParamStringValue
    *  DeviceInfo_SetParamBoolValue
    *  DeviceInfo_SetParamIntValue
    *  DeviceInfo_SetParamUlongValue
    *  DeviceInfo_SetParamStringValue
    *  DeviceInfo_Validate
    *  DeviceInfo_Commit
    *  DeviceInfo_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DeviceInfo_GetParamBoolValue
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
DeviceInfo_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    BOOL                            bReturnValue;
    
    if( AnscEqualString(ParamName, "ClearResetCount", TRUE))
    {
        *pBool = FALSE ; 
        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_AkerEnable", TRUE))
    {
        /* collect value */
        char buf[8];
        if( syscfg_get( NULL, "X_RDKCENTRAL-COM_AkerEnable", buf, sizeof(buf))==0)
        {
            if (strcmp(buf, "true") == 0)
            {
                *pBool = TRUE;
            }
            else
            {
                *pBool = FALSE;
            }
        }
        else
        {
            CcspTraceWarning(("%s syscfg_get failed  for AkerEnable\n",__FUNCTION__));
            *pBool = FALSE;
        }

        return TRUE;
    }

  bReturnValue =
        DeviceInfo_GetParamBoolValue_Custom
            (
                hInsContext,
                ParamName,
                pBool
            );

    if ( bReturnValue )
    {
        return bReturnValue;
    }
    else
    {
        /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
        return FALSE;
    }
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DeviceInfo_GetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int*                        pInt
            );

    description:

        This function is called to retrieve integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The buffer of returned integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
DeviceInfo_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
    {
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_FirmwareDownloadAndFactoryReset", TRUE))
    {
        *pInt = 0;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DeviceInfo_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

    description:

        This function is called to retrieve ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
DeviceInfo_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "UpTime", TRUE))
    {
        /* collect value */
        *puLong = CosaDmlDiGetUpTime(NULL);
        return TRUE;
    }
	/* Required for xPC sync */
	if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_ConfigureDocsicPollTime", TRUE))
    {
        /* collect value */
	   FILE *fp;
	   char buff[30];
	   int retValue;
	   memset(buff,0,sizeof(buff));
   	   fp = fopen("/nvram/docsispolltime.txt", "r");
	   if(!fp)
	   {
		 CcspTraceError(("%s falied to open /nvram/docsispolltime.txt file \n",__FUNCTION__));
		 return FALSE;
	   }
           retValue = fscanf(fp, "%s", buff);      

           if( (retValue != -1) && (buff != NULL ) )
	   {
          	 *puLong = atoi(buff);
           }

	   fclose(fp);	
           return TRUE;
    }
    
    if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_BootTime", TRUE))
    {
        /* collect value */
       *puLong = CosaDmlDiGetBootTime(NULL);
        return TRUE;
    }
    
    if( AnscEqualString(ParamName, "FactoryResetCount", TRUE))
    {
        /* collect value */
        CosaDmlDiGetFactoryResetCount(NULL,puLong);
        return TRUE;
    }
	
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        DeviceInfo_GetParamStringValue
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
DeviceInfo_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    ULONG                           ReturnValue;
    
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Manufacturer", TRUE))
    {
        /* collect value */
        CosaDmlDiGetManufacturer(NULL,pValue,pulSize);
        return 0;
    }

    if( AnscEqualString(ParamName, "ManufacturerOUI", TRUE))
    {
        /* collect value */
        CosaDmlDiGetManufacturerOUI(NULL,pValue,pulSize);
        return 0;
    }

    if( AnscEqualString(ParamName, "ModelName", TRUE))
    {
        /* collect value */
        CosaDmlDiGetModelName(NULL,pValue,pulSize);
        return 0;
    }

    if( AnscEqualString(ParamName, "Description", TRUE))
    {
        /* collect value */
        CosaDmlDiGetDescription(NULL,pValue, pulSize);
        return 0;
    }

    if( AnscEqualString(ParamName, "ProductClass", TRUE))
    {
        /* collect value */
        CosaDmlDiGetProductClass(NULL, pValue,pulSize);
        return 0;
    }

    if( AnscEqualString(ParamName, "SerialNumber", TRUE))
    {
        /* collect value */
        CosaDmlDiGetSerialNumber(NULL,pValue,pulSize);
        return 0;
    }

    if( AnscEqualString(ParamName, "HardwareVersion", TRUE))
    {
        /* collect value */
       CosaDmlDiGetHardwareVersion(NULL,pValue,pulSize);
        return 0;
    }

    if( AnscEqualString(ParamName, "SoftwareVersion", TRUE))
    {
        /* collect value */
        CosaDmlDiGetFirmwareName(NULL, pValue,pulSize);
        return 0;
    }

    if( AnscEqualString(ParamName, "AdditionalHardwareVersion", TRUE))
    {
        /* collect value */
         CosaDmlDiGetAdditionalHardwareVersion(NULL,pValue,pulSize);
        return 0;
    }

    if( AnscEqualString(ParamName, "AdditionalSoftwareVersion", TRUE))
    {
        /* collect value */
        CosaDmlDiGetAdditionalSoftwareVersion(NULL,pValue,pulSize);
        return 0;
    }

    if( AnscEqualString(ParamName, "ProvisioningCode", TRUE))
    {
        /* collect value */
        CosaDmlDiGetProvisioningCode(NULL,pValue,pulSize);
       // AnscCopyString(pValue,  pMyObject->ProvisioningCode);
        return 0;
    }

    if( AnscEqualString(ParamName, "FirstUseDate", TRUE))
    {

	CosaDmlDiGetFirstUseDate(NULL, pValue,pulSize);
        return 0;
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_BootloaderVersion", TRUE))
    {

	    CosaDmlDiGetBootloaderVersion(NULL, pValue,pulSize);
        return 0;
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_FirmwareName", TRUE))
    {

	    CosaDmlDiGetFirmwareName(NULL, pValue,pulSize);
        return 0;
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_FirmwareBuildTime", TRUE))
    {

	    CosaDmlDiGetFirmwareBuildTime(NULL, pValue,pulSize);
        return 0;
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_BaseMacAddress", TRUE))
    {

	    CosaDmlDiGetBaseMacAddress(NULL, pValue,pulSize);
        return 0;
    }

    if( AnscEqualString(ParamName, "Hardware", TRUE))
    {
        CosaDmlDiGetHardware(NULL, pValue,pulSize);
        return 0;
    }

    if( AnscEqualString(ParamName, "Hardware_MemUsed", TRUE))
    {
        CosaDmlDiGetHardware_MemUsed(NULL, pValue,pulSize);
        return 0;
    }

    if( AnscEqualString(ParamName, "Hardware_MemFree", TRUE))
    {
        CosaDmlDiGetHardware_MemFree(NULL, pValue,pulSize);
        return 0;
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_AdvancedServices", TRUE))
    {

    	CosaDmlDiGetAdvancedServices(NULL, pValue,pulSize);
        return 0;
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_ProcessorSpeed", TRUE))
    {

    	CosaDmlDiGetProcessorSpeed(NULL, pValue,pulSize);
        return 0;
    }

    /* Required for WebPA timestamp */
    if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_SystemTime", TRUE))
    {
        struct timespec sysTime;

        if( clock_gettime(CLOCK_REALTIME, &sysTime) != -1 )
        {
              static long prevtime_sec = 0;
              static long prevtime_nsec = 0;
              static int count = 0;
              char sbuf[32] = {0};

              if( (sysTime.tv_sec == prevtime_sec) && (sysTime.tv_nsec == prevtime_nsec) )
              {
                    count++;
              } else {
                    count = 0;
              }
              prevtime_sec = sysTime.tv_sec;
              prevtime_nsec = sysTime.tv_nsec;
              sysTime.tv_nsec += count;

              if( sysTime.tv_nsec > 999999999L)
              {
                   sysTime.tv_sec = sysTime.tv_sec + 1;
                   sysTime.tv_nsec = sysTime.tv_nsec - 1000000000L;
              }                   

              sprintf(sbuf, "%ld.%09ld", sysTime.tv_sec, sysTime.tv_nsec);

              AnscCopyString(pValue, sbuf);
              *pulSize = strlen(sbuf)+1;
        }
        return 0;
    }
        
     if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_LastRebootReason", TRUE))
    {
        char buf[64] = {'\0'};
        syscfg_get( NULL, "X_RDKCENTRAL-COM_LastRebootReason", buf, sizeof(buf));
    	if( buf != NULL )
    		{
    		    AnscCopyString(pValue,  buf);
    		    
    		}
		else
		{
			AnscTraceWarning(("Error in syscfg_get for RebootReason\n"));
		}
		return 0;
        
    }
	
	/* Changes for EMS begins here */
	
	if( AnscEqualString(ParamName, "X_COMCAST-COM_EMS_MobileNumber", TRUE))
    {
		AnscCopyString(pValue,  pMyObject->EMS_MobileNo);
        return 0;
    }
	
	if( AnscEqualString(ParamName, "X_COMCAST-COM_EMS_ServerURL", TRUE))
    {
       //pMyObject->EMS_ServerURL;
		char buf[60];
        memset(buf, 0 ,sizeof(buf));
        syscfg_get( NULL, "ems_server_url", buf, sizeof(buf));
    	if( buf != NULL )
    		{
    		    AnscCopyString(pValue,  buf);
    		    
    		}
		else
			{
			
			}
		return 0;
    }

	/* Changes for EMS end here */
	/*Changes for RDKB-6560*/
   	if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_CMTS_MAC", TRUE))
    		{
#if defined(_ENABLE_EPON_SUPPORT_)
	// Do nothing
#else
		    CosaDmlDiGetCMTSMac(NULL, pValue,pulSize);
#endif
        	    return 0;
    		}
	/*Changes for RDKB-6560 end*/
	/*Changes for RDKB-5878*/
	if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_CloudPersonalizationURL", TRUE))
	{
	   syscfg_get(NULL, "CloudPersonalizationURL", pMyObject->CloudPersonalizationURL, sizeof(pMyObject->CloudPersonalizationURL));
	   AnscCopyString(pValue, pMyObject->CloudPersonalizationURL);
	   return 0;
	}
	/*Changes for RDKB-5878 end*/

    ReturnValue =
        DeviceInfo_GetParamStringValue_Custom
            (
                hInsContext,
                ParamName,
                pValue,
                pulSize
            );

    if ( ReturnValue == 0 )
    {
        return ReturnValue;
    }
    else
    {
        /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
        return -1;
    }
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DeviceInfo_SetParamBoolValue
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
DeviceInfo_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    BOOL                            bReturnValue;
    
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "ClearResetCount", TRUE))
    {
        /* collect value */
        CosaDmlDiClearResetCount(NULL,bValue);
        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_AkerEnable", TRUE))
    {
        /* collect value */
        char buf[8];
        memset(buf, 0, sizeof(buf));
        snprintf(buf, sizeof(buf), "%s", bValue ? "true" : "false");

        if (syscfg_set(NULL, "X_RDKCENTRAL-COM_AkerEnable", buf) != 0)
        {
            AnscTraceWarning(("syscfg_set failed for AkerEnable\n"));
        }
        else
        {
            if (syscfg_commit() != 0)
            {
                AnscTraceWarning(("syscfg_commit failed for AkerEnable\n"));
            }
	    else
	    {
		/* Restart Firewall */
		system("sysevent set firewall-restart");
	    }	
        }

        return TRUE;
    }

    bReturnValue =
        DeviceInfo_SetParamBoolValue_Custom
            (
                hInsContext,
                ParamName,
                bValue
            );

    if ( bReturnValue )
    {
        return bReturnValue;
    }
    else
    {
        /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
        return FALSE;
    }
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DeviceInfo_SetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int                         iValue
            );

    description:

        This function is called to set integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int                         iValue
                The updated integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
DeviceInfo_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_FirmwareDownloadAndFactoryReset", TRUE))
    {
        if( iValue == 1)
        {
            if(ANSC_STATUS_SUCCESS != CosaDmlDiSetFirmwareDownloadAndFactoryReset())
            {
                return FALSE;
            }
        }
	return TRUE;
    }

	/* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DeviceInfo_SetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG                       uValue
            );

    description:

        This function is called to set ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue
                The updated ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
DeviceInfo_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_ConfigureDocsicPollTime", TRUE))
    {
        /* collect value */
           FILE *fp;
           char buff[30];
	   snprintf(buff,sizeof(buff),"%d",uValue);

           fp = fopen("/nvram/docsispolltime.txt", "w+");
           if(!fp)
           {
                 CcspTraceError(("%s falied to open /nvram/docsispolltime.txt file \n",__FUNCTION__));
                 return FALSE;
           }
	   fprintf(fp, "%s\n", buff);
 	   fclose(fp);
	   return TRUE;
    } 
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DeviceInfo_SetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pString
            );

    description:

        This function is called to set string parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pString
                The updated string value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
DeviceInfo_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    ANSC_STATUS ret=ANSC_STATUS_FAILURE;
    
    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "ProvisioningCode", TRUE))
    {
        /* save update to backup */
        AnscCopyString(pMyObject->ProvisioningCode, pString);
        return TRUE;
    }
#ifdef CONFIG_INTERNET2P0
    if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_CloudUIWebURL", TRUE))
    {
    char wrapped_inputparam[256]={0};
	ret=isValidInput(pString,wrapped_inputparam, AnscSizeOfString(pString), sizeof( wrapped_inputparam ));
	if(ANSC_STATUS_SUCCESS != ret)
	    return FALSE;
	
	if (syscfg_set(NULL, "redirection_url", wrapped_inputparam) != 0) {
             AnscTraceWarning(("syscfg_set failed\n"));
          } else {
	       if (syscfg_commit() != 0) {
                    AnscTraceWarning(("syscfg_commit failed\n"));
                    }
		char url[150];	
		snprintf(url,sizeof(url),"/etc/whitelist.sh %s",wrapped_inputparam);
		system(url);
		AnscCopyString(pMyObject->WebURL, wrapped_inputparam);
		CcspTraceWarning(("CaptivePortal:Cloud URL is changed, new URL is %s ...\n",pMyObject->WebURL));
             }

	return TRUE;

    }
#endif
    /*Changes for 5878 start*/
    if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_CloudPersonalizationURL", TRUE))
    {
		if (syscfg_set(NULL, "CloudPersonalizationURL", pString) != 0)
		{
	        AnscTraceWarning(("syscfg_set failed\n"));
	    } 
		else
		{
		    if (syscfg_commit() != 0)
			{
	        	AnscTraceWarning(("syscfg_commit failed\n"));
	    	}
			AnscCopyString(pMyObject->CloudPersonalizationURL, pString);
			CcspTraceWarning(("CloudPersonalizationURL URL is changed, new URL is %s ...\n",pMyObject->CloudPersonalizationURL));
	    }

		return TRUE;

	}
	/*Changes for 5878 end*/
   if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_UI_ACCESS", TRUE))
   {

         if (AnscEqualString(pString, "ui_access", TRUE))
         {
		 CcspTraceInfo(("Local UI Access : RDKB_LOCAL_UI_ACCESS\n"));
         }
         else if (AnscEqualString(pString, "ui_success", TRUE))
         {
		CcspTraceInfo(("Local UI Access : RDKB_LOCAL_UI_SUCCESS\n"));
                CcspTraceInfo(("WebUi admin login success\n"));
         }
         else if(AnscEqualString(pString, "ui_failed", TRUE))
         {
         	CcspTraceInfo(("Local UI Access : RDKB_LOCAL_UI_FAILED\n"));
                CcspTraceInfo(("WebUi admin login failed\n"));
         }
	 else if (AnscEqualString(pString, "reboot_device", TRUE))
         {
                CcspTraceInfo(("RDKB_REBOOT : RebootDevice triggered from GUI\n"));
		 
                #if defined (_ARRIS_XB6_PRODUCT_REQ_) //ARRISXB6-7328, ARRISXB6-7332
                ARRIS_RESET_REASON("RDKB_REBOOT : RebootDevice triggered from GUI\n");
                #endif

		char buffer[8] = {0};
		syscfg_get( NULL, "restore_reboot", buffer, sizeof(buffer));

		if (strcmp(buffer, "true") != 0)
		{
			char buf[8];
			snprintf(buf,sizeof(buf),"%d",1);

			if (syscfg_set(NULL, "X_RDKCENTRAL-COM_LastRebootReason", "gui-reboot") != 0)
			{
				AnscTraceWarning(("RDKB_REBOOT : RebootDevice syscfg_set failed GUI\n"));
			}
			else
			{
				if (syscfg_commit() != 0)
				{
					AnscTraceWarning(("RDKB_REBOOT : RebootDevice syscfg_commit failed GUI\n"));
				}
			}
	        
	        
			if (syscfg_set(NULL, "X_RDKCENTRAL-COM_LastRebootCounter", buf) != 0)
			{
				AnscTraceWarning(("syscfg_set failed\n"));
			}
			else
			{
				if (syscfg_commit() != 0)
				{
					AnscTraceWarning(("syscfg_commit failed\n"));
				}
			}
	        }
		else
		{
			CcspTraceInfo(("RDKB_REBOOT : RebootDevice to restore configuration\n"));
		}
	 }
         else if(AnscEqualString(pString, "factory_reset", TRUE))
         {
               CcspTraceInfo(("RDKB_REBOOT : Reboot Device triggered through Factory reset from GUI\n"));
			 
               #if defined (_ARRIS_XB6_PRODUCT_REQ_) //ARRISXB6-7328, ARRISXB6-7332
               ARRIS_RESET_REASON("RDKB_REBOOT : Reboot Device triggered through Factory reset from GUI\n");
               #endif
         }
	 else if(AnscEqualString(pString, "captiveportal_failure", TRUE)) {

	 	CcspTraceInfo(("Local UI Access : Out of Captive Poratl, Captive Portal is disabled\n"));
	 }
         else
         {
	        CcspTraceInfo(("Local UI Access : Unsupported value\n"));
         }
         return TRUE;
   }

   /* Changes for EMS */
   if( AnscEqualString(ParamName, "X_COMCAST-COM_EMS_ServerURL", TRUE))
    {
    char wrapped_inputparam[256]={0};
	ret=isValidInput(pString,wrapped_inputparam, AnscSizeOfString(pString), sizeof( wrapped_inputparam ));
	if(ANSC_STATUS_SUCCESS != ret)
	    return FALSE;
	
	if (syscfg_set(NULL, "ems_server_url", wrapped_inputparam) != 0) {
             AnscTraceWarning(("syscfg_set failed\n"));
          } else {
	       if (syscfg_commit() != 0) {
                    AnscTraceWarning(("syscfg_commit failed\n"));
                    }
		char ems_url[150];	
		snprintf(ems_url,sizeof(ems_url),"/etc/whitelist.sh %s",wrapped_inputparam);
		system(ems_url);
		AnscCopyString(pMyObject->EMS_ServerURL, wrapped_inputparam);
             }

	return TRUE;

    }
    
     if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_LastRebootReason", TRUE))
        {
            int val = 1;
            char buf[8];
		    snprintf(buf,sizeof(buf),"%d",val);     
        
                if (syscfg_set(NULL, "X_RDKCENTRAL-COM_LastRebootReason", pString) != 0) 
	            {
			        AnscTraceWarning(("syscfg_set failed for Reason and counter \n"));
			    }
		   	    else 
		        {
		             if (syscfg_commit() != 0) 
		            {
				        AnscTraceWarning(("syscfg_commit failed for Reason and counter \n"));
				
			        }
		
		        }
		        if (syscfg_set(NULL, "X_RDKCENTRAL-COM_LastRebootCounter", buf) != 0) 
	            {
                      AnscTraceWarning(("syscfg_set failed\n"));
                }
                else 
		        {
		          if (syscfg_commit() != 0)
	                  {
                            AnscTraceWarning(("syscfg_commit failed\n"));
                       }
	             }
	    return TRUE;
				
        }
    
	if( AnscEqualString(ParamName, "X_COMCAST-COM_EMS_MobileNumber", TRUE))
    {
        /* save update to backup */
        AnscCopyString(pMyObject->EMS_MobileNo, pString);
        return TRUE;
		
    }
	
/* Changes end here */
	
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DeviceInfo_Validate
            (
                ANSC_HANDLE                 hInsContext,
                char*                       pReturnParamName,
                ULONG*                      puLength
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       pReturnParamName,
                The buffer (128 bytes) of parameter name if there's a validation. 

                ULONG*                      puLength
                The output length of the param name. 

    return:     TRUE if there's no validation.

**********************************************************************/
BOOL
DeviceInfo_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        DeviceInfo_Commit
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
DeviceInfo_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    
    CosaDmlDiSetProvisioningCode(NULL, pMyObject->ProvisioningCode);
    
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        DeviceInfo_Rollback
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to roll back the update whenever there's a 
        validation found.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
DeviceInfo_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    ULONG pulSize = 0;

    CosaDmlDiGetProvisioningCode(NULL,pMyObject->ProvisioningCode, &pulSize);
    
    return 0;
}

/***********************************************************************

 APIs for Object:

    DeviceInfo.X_RDKCENTRAL-COM_WIFI_TELEMETRY.{i}.

    *  WiFi_Telemetry_SetParamIntValue
    *  WiFi_Telemetry_SetParamStringValue
    *  WiFi_Telemetry_GetParamIntValue
    *  WiFi_Telemetry_GetParamStringValue
***********************************************************************/

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        WiFi_Telemetry_SetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int                         iValue
            );

    description:

        This function is called to set integer parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int                         iValue
                The updated integer value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL
WiFi_Telemetry_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "LogInterval", TRUE))
    {
        char str[10];
        int retPsmGet = CCSP_SUCCESS;

        /* Updating the LogInterval  in PSM database  */
        sprintf(str,"%d",iValue);
        retPsmGet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, "dmsb.device.deviceinfo.X_RDKCENTRAL-COM_WHIX.LogInterval", ccsp_string, str);
        if (retPsmGet != CCSP_SUCCESS) {
        CcspTraceError(("Set failed for LogInterval Support \n"));
        return FALSE;
        }
        CcspTraceInfo(("Successfully set  LogInterval in PSM \n"));
        /* save update to backup */
        pMyObject->WiFi_Telemetry.LogInterval = iValue;
        return TRUE;
    }

    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        WiFi_Telemetry_SetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pString
            );

    description:

        This function is called to set string parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pString
                The updated string value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL
WiFi_Telemetry_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "NormalizedRssiList", TRUE))
    {
        int retPsmGet = CCSP_SUCCESS;

        /* Updating the NormalizedRssiList in PSM database  */
        retPsmGet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, "dmsb.device.deviceinfo.X_RDKCENTRAL-COM_WHIX.NormalizedRssiList", ccsp_string, pString);
        if (retPsmGet != CCSP_SUCCESS) {
        CcspTraceError(("Set failed for NormalizedRssiList Support \n"));
        return FALSE;
        }
        CcspTraceInfo(("Successfully set  NormalizedRssiList in PSM \n"));
        /* save update to backup */
        AnscCopyString(pMyObject->WiFi_Telemetry.NormalizedRssiList, pString);
        return TRUE;
    }

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "CliStatList", TRUE) )
    {
        int retPsmGet = CCSP_SUCCESS;

        /* Updating the CliStatList in PSM database  */
        retPsmGet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, "dmsb.device.deviceinfo.X_RDKCENTRAL-COM_WHIX.CliStatList", ccsp_string, pString);
        if (retPsmGet != CCSP_SUCCESS) {
        CcspTraceError(("Set failed for CliStatList Support \n"));
        return FALSE;
        }
        CcspTraceInfo(("Successfully set  CliStatList in PSM \n"));
        /* save update to backup */
        AnscCopyString( pMyObject->WiFi_Telemetry.CliStatList, pString );
        return TRUE;
    }

        /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "TxRxRateList", TRUE) )
    {
        int retPsmGet = CCSP_SUCCESS;

        /* Updating the TxRxRateList  in PSM database  */
        retPsmGet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, "dmsb.device.deviceinfo.X_RDKCENTRAL-COM_WHIX.TxRxRateList", ccsp_string, pString);
        if (retPsmGet != CCSP_SUCCESS) {
        CcspTraceError(("Set failed for TxRxRateList Support \n"));
        return FALSE;
        }
        CcspTraceInfo(("Successfully set  TxRxRateList in PSM \n"));
        /* save update to backup */
        AnscCopyString( pMyObject->WiFi_Telemetry.TxRxRateList, pString );
        return TRUE;
    }

    if( AnscEqualString(ParamName, "SNRList", TRUE))
    {
        int retPsmSet = CCSP_SUCCESS;
    
        retPsmSet = PSM_Set_Record_Value2( bus_handle, g_Subsystem, "dmsb.device.deviceinfo.X_RDKCENTRAL-COM_WIFI_TELEMETRY.SNRList", ccsp_string, pString );

        if (retPsmSet != CCSP_SUCCESS )
        {
            CcspTraceInfo(("Failed to set SNRList in PSM"));
            return FALSE;
        }
        
        CcspTraceInfo(("Successfully set SNRList in PSM \n"));
    
        AnscCopyString( pMyObject->WiFi_Telemetry.SNRList, pString );
        return TRUE;
    }

    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        WiFi_Telemetry_GetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int*                        pInt
            );

    description:

        This function is called to retrieve integer parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The buffer of returned integer value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL
WiFi_Telemetry_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{

    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "LogInterval", TRUE))
    {
        *pInt =  pMyObject->WiFi_Telemetry.LogInterval;
        return TRUE;
    }

    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        WiFi_Telemetry_GetParamStringValue
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
WiFi_Telemetry_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

    ULONG                           ReturnValue;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "NormalizedRssiList", TRUE))
    {
        /* collect value */
        AnscCopyString( pValue, pMyObject->WiFi_Telemetry.NormalizedRssiList);
        *pulSize = AnscSizeOfString( pValue );
        return 0;
    }

    if( AnscEqualString(ParamName, "CliStatList", TRUE))
    {
        /* collect value */
        AnscCopyString( pValue, pMyObject->WiFi_Telemetry.CliStatList);
        *pulSize = AnscSizeOfString( pValue );
        return 0;
    }

    if( AnscEqualString(ParamName, "TxRxRateList", TRUE))
    {
        /* collect value */
        AnscCopyString( pValue, pMyObject->WiFi_Telemetry.TxRxRateList);
        *pulSize = AnscSizeOfString( pValue );
        return 0;
    }

    if( AnscEqualString(ParamName, "SNRList", TRUE))
    {
        /* collect value */
        AnscCopyString( pValue, pMyObject->WiFi_Telemetry.SNRList);
        *pulSize = AnscSizeOfString( pValue );
        return 0;
    }


    return -1;
}

/***********************************************************************

 APIs for Object:

    DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.UniqueTelemetryId.

    *  UniqueTelemetryId_GetParamBoolValue
    *  UniqueTelemetryId_GetParamStringValue
    *  UniqueTelemetryId_GetParamIntValue
    *  UniqueTelemetryId_SetParamBoolValue
    *  UniqueTelemetryId_SetParamStringValue
    *  UniqueTelemetryId_SetParamIntValue
***********************************************************************/

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        UniqueTelemetryId_GetParamBoolValue
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
UniqueTelemetryId_GetParamBoolValue
	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		BOOL*						pBool
	)
{
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;


    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* collect value */
        *pBool = pMyObject->UniqueTelemetryId.Enable;
        return TRUE;
    }

    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        UniqueTelemetryId_GetParamStringValue
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
UniqueTelemetryId_GetParamStringValue
	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		char*						pValue,
		ULONG*						pUlSize
	)
{
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;


    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "TagString", TRUE))
    {
		if ( AnscSizeOfString( pMyObject->UniqueTelemetryId.TagString ) < *pUlSize )
   		{
       		AnscCopyString( pValue, pMyObject->UniqueTelemetryId.TagString );		
        	return 0;
   		}
   		else
   		{
       		*pUlSize = AnscSizeOfString( pMyObject->UniqueTelemetryId.TagString ) + 1;
       		return 1;
   		}

        AnscCopyString( pValue, pMyObject->UniqueTelemetryId.TagString );
        return 0;
    }

	return -1;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        UniqueTelemetryId_GetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int*                        pInt
            );

    description:

        This function is called to retrieve integer parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The buffer of returned integer value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL
UniqueTelemetryId_GetParamIntValue
	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		int*						pInt
	)
{
	PCOSA_DATAMODEL_DEVICEINFO		pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
	
	/* check the parameter name and return the corresponding value */
	if( AnscEqualString(ParamName, "TimingInterval", TRUE))
	{
		*pInt =  pMyObject->UniqueTelemetryId.TimingInterval;
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        UniqueTelemetryId_SetParamBoolValue
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
UniqueTelemetryId_SetParamBoolValue
	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		BOOL						bValue
	)
{
    PCOSA_DATAMODEL_DEVICEINFO		pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    char buf[8] = {0};

    if (AnscEqualString(ParamName, "Enable", TRUE))
    {
        if(bValue)
            strcpy(buf,"true");
        else
            strcpy(buf,"false");

        if (syscfg_set(NULL, "unique_telemetry_enable", buf) != 0) 
        {
            AnscTraceWarning(("syscfg_set failed\n"));
        } 
        else
        {
            if (syscfg_commit() != 0) 
            {
                AnscTraceWarning(("syscfg_commit failed\n"));
            }
            else
            {
                pMyObject->UniqueTelemetryId.Enable = bValue;
            }
        }

	UniqueTelemetryCronJob(pMyObject->UniqueTelemetryId.Enable, pMyObject->UniqueTelemetryId.TimingInterval, pMyObject->UniqueTelemetryId.TagString);

        return TRUE;
    }

    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        UniqueTelemetryId_SetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pString
            );

    description:

        This function is called to set string parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pString
                The updated string value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL
UniqueTelemetryId_SetParamStringValue
	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		char*						strValue
	)
{
    PCOSA_DATAMODEL_DEVICEINFO		pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

    if (AnscEqualString(ParamName, "TagString", TRUE))
    {

        if (syscfg_set(NULL, "unique_telemetry_tag", strValue) != 0)
        {
            AnscTraceWarning(("syscfg_set failed\n"));
        }
        else
        {
            if (syscfg_commit() != 0)
            {
                AnscTraceWarning(("syscfg_commit failed\n"));
            }
            else
            {
                AnscCopyString(pMyObject->UniqueTelemetryId.TagString, strValue);
            }
        }
        return TRUE;
    }

    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        UniqueTelemetryId_SetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int                         iValue
            );

    description:

        This function is called to set integer parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int                         iValue
                The updated integer value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL
UniqueTelemetryId_SetParamIntValue
	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		int 						value
	)
{
    PCOSA_DATAMODEL_DEVICEINFO		pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    char buf[16]={0};

    if (AnscEqualString(ParamName, "TimingInterval", TRUE))
    {
        sprintf(buf, "%d", value);

        if (syscfg_set(NULL, "unique_telemetry_interval", buf) != 0) 
        {
            AnscTraceWarning(("syscfg_set failed\n"));
        } 
        else
        {
            if (syscfg_commit() != 0)
            {
                AnscTraceWarning(("syscfg_commit failed\n"));
            }
            else
            {
                pMyObject->UniqueTelemetryId.TimingInterval = value;
            }
        }

	UniqueTelemetryCronJob(pMyObject->UniqueTelemetryId.Enable, pMyObject->UniqueTelemetryId.TimingInterval, pMyObject->UniqueTelemetryId.TagString);

        return TRUE;
    }

    return FALSE;
}



/***********************************************************************

 APIs for Object:

    DeviceInfo.VendorConfigFile.{i}.

    *  VendorConfigFile_GetEntryCount
    *  VendorConfigFile_GetEntry
    *  VendorConfigFile_GetParamBoolValue
    *  VendorConfigFile_GetParamIntValue
    *  VendorConfigFile_GetParamUlongValue
    *  VendorConfigFile_GetParamStringValue
    *  VendorConfigFile_SetParamBoolValue
    *  VendorConfigFile_SetParamIntValue
    *  VendorConfigFile_SetParamUlongValue
    *  VendorConfigFile_SetParamStringValue
    *  VendorConfigFile_Validate
    *  VendorConfigFile_Commit
    *  VendorConfigFile_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        VendorConfigFile_GetEntryCount
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG
VendorConfigFile_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        VendorConfigFile_GetEntry
            (
                ANSC_HANDLE                 hInsContext,
                ULONG                       nIndex,
                ULONG*                      pInsNumber
            );

    description:

        This function is called to retrieve the entry specified by the index.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ULONG                       nIndex,
                The index of this entry;

                ULONG*                      pInsNumber
                The output instance number;

    return:     The handle to identify the entry

**********************************************************************/
ANSC_HANDLE
VendorConfigFile_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    *pInsNumber  = nIndex + 1; 
    return NULL; /* return the handle */
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        VendorConfigFile_GetParamBoolValue
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
VendorConfigFile_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        VendorConfigFile_GetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int*                        pInt
            );

    description:

        This function is called to retrieve integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The buffer of returned integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
VendorConfigFile_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        VendorConfigFile_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

    description:

        This function is called to retrieve ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
VendorConfigFile_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        VendorConfigFile_GetParamStringValue
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
VendorConfigFile_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE))
    {
        /* collect value */
        return 0;
    }

    if( AnscEqualString(ParamName, "Name", TRUE))
    {
        /* collect value */
        return 0;
    }

    if( AnscEqualString(ParamName, "Version", TRUE))
    {
        /* collect value */
        return 0;
    }

    if( AnscEqualString(ParamName, "Date", TRUE))
    {
        /* collect value */
        return 0;
    }

    if( AnscEqualString(ParamName, "Description", TRUE))
    {
        /* collect value */
        return 0;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        VendorConfigFile_SetParamBoolValue
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
VendorConfigFile_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        VendorConfigFile_SetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int                         iValue
            );

    description:

        This function is called to set integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int                         iValue
                The updated integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
VendorConfigFile_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        VendorConfigFile_SetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG                       uValue
            );

    description:

        This function is called to set ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue
                The updated ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
VendorConfigFile_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        VendorConfigFile_SetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pString
            );

    description:

        This function is called to set string parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pString
                The updated string value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
VendorConfigFile_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE))
    {
        /* save update to backup */
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        VendorConfigFile_Validate
            (
                ANSC_HANDLE                 hInsContext,
                char*                       pReturnParamName,
                ULONG*                      puLength
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       pReturnParamName,
                The buffer (128 bytes) of parameter name if there's a validation. 

                ULONG*                      puLength
                The output length of the param name. 

    return:     TRUE if there's no validation.

**********************************************************************/
BOOL
VendorConfigFile_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        VendorConfigFile_Commit
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
VendorConfigFile_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        VendorConfigFile_Rollback
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to roll back the update whenever there's a 
        validation found.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
VendorConfigFile_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    return 0;
}

/***********************************************************************

 APIs for Object:

    DeviceInfo.SupportedDataModel.{i}.

    *  SupportedDataModel_GetEntryCount
    *  SupportedDataModel_GetEntry
    *  SupportedDataModel_GetParamBoolValue
    *  SupportedDataModel_GetParamIntValue
    *  SupportedDataModel_GetParamUlongValue
    *  SupportedDataModel_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        SupportedDataModel_GetEntryCount
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG
SupportedDataModel_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        SupportedDataModel_GetEntry
            (
                ANSC_HANDLE                 hInsContext,
                ULONG                       nIndex,
                ULONG*                      pInsNumber
            );

    description:

        This function is called to retrieve the entry specified by the index.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ULONG                       nIndex,
                The index of this entry;

                ULONG*                      pInsNumber
                The output instance number;

    return:     The handle to identify the entry

**********************************************************************/
ANSC_HANDLE
SupportedDataModel_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    *pInsNumber  = nIndex + 1; 
    return NULL; /* return the handle */
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        SupportedDataModel_GetParamBoolValue
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
SupportedDataModel_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        SupportedDataModel_GetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int*                        pInt
            );

    description:

        This function is called to retrieve integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The buffer of returned integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
SupportedDataModel_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        SupportedDataModel_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

    description:

        This function is called to retrieve ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
SupportedDataModel_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        SupportedDataModel_GetParamStringValue
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
SupportedDataModel_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "URL", TRUE))
    {
        /* collect value */
        return 0;
    }

    if( AnscEqualString(ParamName, "URN", TRUE))
    {
        /* collect value */
        return 0;
    }

    if( AnscEqualString(ParamName, "Features", TRUE))
    {
        /* collect value */
        return 0;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/***********************************************************************

 APIs for Object:

    DeviceInfo.MemoryStatus.

    *  MemoryStatus_GetParamBoolValue
    *  MemoryStatus_GetParamIntValue
    *  MemoryStatus_GetParamUlongValue
    *  MemoryStatus_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        MemoryStatus_GetParamBoolValue
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
MemoryStatus_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        MemoryStatus_GetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int*                        pInt
            );

    description:

        This function is called to retrieve integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The buffer of returned integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
MemoryStatus_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        MemoryStatus_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

    description:

        This function is called to retrieve ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
MemoryStatus_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Total", TRUE))
    {
         /* collect value */
        *puLong = COSADmlGetMemoryStatus(ParamName);
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Free", TRUE))
    {
        /* collect value */
        *puLong = COSADmlGetMemoryStatus(ParamName);
        return TRUE;
    }


    if( AnscEqualString(ParamName, "Used", TRUE))
    {
        /* collect value */
        *puLong = COSADmlGetMemoryStatus(ParamName);
        return TRUE;
    }


    if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_FreeMemThreshold", TRUE))
    {
        /* collect value */
        *puLong = COSADmlGetMemoryStatus(ParamName);
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        MemoryStatus_GetParamStringValue
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
MemoryStatus_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}



/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        MemoryStatus_SetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG                       uValue
            );

    description:

        This function is called to set ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue
                The updated ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
MemoryStatus_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_FreeMemThreshold", TRUE))
    {
        /* collect value */
	   COSADmlSetMemoryStatus(ParamName,uValue);
	   return TRUE;
    } 
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}
/***********************************************************************

 APIs for Object:

    DeviceInfo.X_RDKCENTRAL-COM.

    *  X_RDKCENTRAL-COM_GetParamBoolValue
    *  X_RDKCENTRAL-COM_GetParamIntValue
    *  X_RDKCENTRAL-COM_GetParamUlongValue
    *  X_RDKCENTRAL-COM_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_RDKCENTRAL-COM_GetParamBoolValue
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
X_RDKCENTRAL_COM_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "DhcpServDetectEnable", TRUE))
    {
        *pBool = pMyObject->bDhcpServDetectEnable;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "MultipleGW", TRUE))
    {
        *pBool = pMyObject->bMultipleGW;
        return TRUE;
    }
	
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_RDKCENTRAL-COM_GetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int*                        pInt
            );

    description:

        This function is called to retrieve integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The buffer of returned integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
X_RDKCENTRAL_COM_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    /* check the parameter name and return the corresponding value */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_RDKCENTRAL-COM_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

    description:

        This function is called to retrieve ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
X_RDKCENTRAL_COM_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        X_RDKCENTRAL-COM_GetParamStringValue
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
X_RDKCENTRAL_COM_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    /* check the parameter name and return the corresponding value */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_RDKCENTRAL_COM_SetParamBoolValue
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
X_RDKCENTRAL_COM_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "DhcpServDetectEnable", TRUE))
    {
		/* Same value should not be process at anytime */
		if( bValue == pMyObject->bDhcpServDetectEnable )
		{
			return TRUE;
		}

		CosaDmlDiSetAndProcessDhcpServDetectionFlag( pMyObject, &bValue, &pMyObject->bDhcpServDetectEnable );

        return TRUE;
    }

    if( AnscEqualString(ParamName, "MultipleGW", TRUE))
    {
		pMyObject->bMultipleGW = bValue;

        return TRUE;
    }

    return FALSE;
}

/***********************************************************************

 APIs for Object:

    DeviceInfo.X_RDKCENTRAL-COM.Ops.{i}.

    *  Ops_GetEntryCount
    *  Ops_GetEntry
    *  Ops_IsUpdated
    *  Ops_Synchronize
    *  Ops_GetParamBoolValue
    *  Ops_GetParamIntValue
    *  Ops_GetParamUlongValue
    *  Ops_GetParamStringValue

***********************************************************************/




/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Ops_GetParamBoolValue
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
Ops_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "UploadLogsNow", TRUE))
    {
        /* collect value */
         *pBool = FALSE;
         
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Ops_GetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int*                        pInt
            );

    description:

        This function is called to retrieve integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The buffer of returned integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
Ops_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    /* check the parameter name and return the corresponding value */
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Ops_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

    description:

        This function is called to retrieve ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
Ops_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
	
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Ops_GetParamStringValue
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
Ops_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{

    if( AnscEqualString(ParamName, "LogsUploadStatus", TRUE))
    {
        /* collect value */
	COSADmlUploadLogsStatus(NULL, pValue,pUlSize);
        return 0;
    }
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Ops_SetParamBoolValue
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
Ops_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    BOOL                            bReturnValue;

    if( AnscEqualString(ParamName, "UploadLogsNow", TRUE))
    {

	bReturnValue =
        	COSADmlUploadLogsNow
            	(
                	(ANSC_HANDLE)NULL, 
			bValue
            	);


    		if ( ! bReturnValue )
    		{
        		return TRUE;
    		}
    		else
    		{
        		/* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
        		return FALSE;
    		}
    }
    return FALSE;
}


/***********************************************************************

 APIs for Object:

    DeviceInfo.ProcessStatus.

    *  ProcessStatus_GetParamBoolValue
    *  ProcessStatus_GetParamIntValue
    *  ProcessStatus_GetParamUlongValue
    *  ProcessStatus_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        ProcessStatus_GetParamBoolValue
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
ProcessStatus_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        ProcessStatus_GetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int*                        pInt
            );

    description:

        This function is called to retrieve integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The buffer of returned integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
ProcessStatus_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        ProcessStatus_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

    description:

        This function is called to retrieve ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
ProcessStatus_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "CPUUsage", TRUE))
    {
        /* collect value */
         *puLong = COSADmlGetCpuUsage();
         
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        ProcessStatus_GetParamStringValue
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
ProcessStatus_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/***********************************************************************

 APIs for Object:

    DeviceInfo.ProcessStatus.Process.{i}.

    *  Process_GetEntryCount
    *  Process_GetEntry
    *  Process_IsUpdated
    *  Process_Synchronize
    *  Process_GetParamBoolValue
    *  Process_GetParamIntValue
    *  Process_GetParamUlongValue
    *  Process_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Process_GetEntryCount
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG
Process_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_PROCSTATUS      pProc     = (PCOSA_DATAMODEL_PROCSTATUS)g_pCosaBEManager->hProcStatus;

    return pProc->ProcessNumberOfEntries;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Process_GetEntry
            (
                ANSC_HANDLE                 hInsContext,
                ULONG                       nIndex,
                ULONG*                      pInsNumber
            );

    description:

        This function is called to retrieve the entry specified by the index.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ULONG                       nIndex,
                The index of this entry;

                ULONG*                      pInsNumber
                The output instance number;

    return:     The handle to identify the entry

**********************************************************************/
ANSC_HANDLE
Process_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_PROCSTATUS      pProc     = (PCOSA_DATAMODEL_PROCSTATUS)g_pCosaBEManager->hProcStatus;

    *pInsNumber  = nIndex + 1;             

    if (nIndex < pProc->ProcessNumberOfEntries)
    {
        return pProc->pProcTable+nIndex;
    }

    return NULL; /* return the handle */
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Process_IsUpdated
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/
static ULONG last_tick;
#define REFRESH_INTERVAL 120
#define TIME_NO_NEGATIVE(x) ((long)(x) < 0 ? 0 : (x))
BOOL
Process_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    )
{
    if (!last_tick) 
    {
        last_tick = AnscGetTickInSeconds();
        return TRUE;
    }
    
    if (last_tick >= TIME_NO_NEGATIVE(AnscGetTickInSeconds() - REFRESH_INTERVAL))
    {
        return FALSE;
    }
    else 
    {
        last_tick = AnscGetTickInSeconds();
        return TRUE;
    }
    
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Process_Synchronize
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
Process_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_PROCSTATUS      pProc     = (PCOSA_DATAMODEL_PROCSTATUS)g_pCosaBEManager->hProcStatus;

    if (pProc->pProcTable != NULL)
    {
         AnscFreeMemory(pProc->pProcTable);
         pProc->pProcTable = NULL;
         pProc->ProcessNumberOfEntries = 0;
    }

    COSADmlGetProcessInfo(pProc);
    
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Process_GetParamBoolValue
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
Process_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Process_GetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int*                        pInt
            );

    description:

        This function is called to retrieve integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The buffer of returned integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
Process_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Process_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

    description:

        This function is called to retrieve ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
Process_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_PROCESS_ENTRY        p_proc = (PCOSA_PROCESS_ENTRY)hInsContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "PID", TRUE))
    {
        /* collect value */
        *puLong = p_proc->Pid;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Size", TRUE))
    {
        /* collect value */
        *puLong = p_proc->Size;        
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Priority", TRUE))
    {
        /* collect value */
        *puLong = p_proc->Priority;                
        return TRUE;
    }

    if( AnscEqualString(ParamName, "CPUTime", TRUE))
    {
        /* collect value */
        *puLong = p_proc->CPUTime;                
        return TRUE;
    }

    if( AnscEqualString(ParamName, "State", TRUE))
    {
        /* collect value */
        *puLong = p_proc->State;                
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Process_GetParamStringValue
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
Process_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_PROCESS_ENTRY        p_proc = (PCOSA_PROCESS_ENTRY)hInsContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Command", TRUE))
    {
        /* collect value */
        if ( AnscSizeOfString(p_proc->Command) < *pUlSize)
        {
            AnscCopyString(pValue, p_proc->Command);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(p_proc->Command)+1;
            return 1;
        }
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/***********************************************************************

 APIs for Object:

    DeviceInfo.NetworkProperties.

    *  NetworkProperties_GetParamBoolValue
    *  NetworkProperties_GetParamIntValue
    *  NetworkProperties_GetParamUlongValue
    *  NetworkProperties_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        NetworkProperties_GetParamBoolValue
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
NetworkProperties_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        NetworkProperties_GetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int*                        pInt
            );

    description:

        This function is called to retrieve integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The buffer of returned integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
NetworkProperties_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        NetworkProperties_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

    description:

        This function is called to retrieve ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
NetworkProperties_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "MaxTCPWindowSize", TRUE))
    {
        /* collect value */
        *puLong = COSADmlGetMaxWindowSize();
        return TRUE;
    }

    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        NetworkProperties_GetParamStringValue
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
NetworkProperties_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    if( AnscEqualString(ParamName, "TCPImplementation", TRUE))
    {
	/* collect value */
        CosaDmlGetTCPImplementation(NULL,pValue,pulSize);

        return 0;
    }
	
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Iot_GetParamBoolValue
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
Iot_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_ENABLEIOT", TRUE))
    {
        /* collect value */
        char buf[8];
        syscfg_get( NULL, "X_RDKCENTRAL-COM_ENABLEIOT", buf, sizeof(buf));

        if( buf != NULL )
        {
            if (strcmp(buf, "true") == 0)
                *pBool = TRUE;
            else
                *pBool = FALSE;
        }
        return TRUE;
    }
    return FALSE;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Iot_SetParamBoolValue
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
Iot_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_ENABLEIOT", TRUE))
    {
        char buf[8];
        memset(buf, 0, sizeof(buf));
        snprintf(buf, sizeof(buf), "%s", bValue ? "true" : "false");

        if (syscfg_set(NULL, "X_RDKCENTRAL-COM_ENABLEIOT", buf) != 0) 
        {
            AnscTraceWarning(("syscfg_set failed\n"));
        }
        else 
        {
            if (syscfg_commit() != 0) 
            {
                AnscTraceWarning(("syscfg_commit failed\n"));
            }
            else
            {
                if(bValue){
                   AnscTraceWarning(("IOT_LOG : Raise IOT event up from DML\n"));
                   system("sysevent set iot_status up");
                }
                else{
                   AnscTraceWarning(("IOT_LOG : Raise IOT event down from DML\n"));
                   system("sysevent set iot_status down");
                }
                return TRUE;
            }
        }
    }

    return FALSE;
}
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Logging_GetParamBoolValue
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
Logging_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "xOpsDMRetrieveConfigLogNow", TRUE))
    {
        /* collect value */
		*pBool=FALSE;
		return TRUE;
        
    }
	if( AnscEqualString(ParamName, "xOpsDMUploadLogsNow", TRUE))
    {
        /* collect value */
		*pBool=FALSE;
		return TRUE;
    }

    return FALSE;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Logging_SetParamBoolValue
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
Logging_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
	BOOL bReturnValue;
    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "xOpsDMRetrieveConfigLogNow", TRUE))
    {
		if( TRUE == bValue )
		{
			AnscTraceWarning((" Run DCM service\n"));
			system("sh /lib/rdk/dcm.service &");
		}

    	return TRUE;
	}
	else if( AnscEqualString(ParamName, "xOpsDMUploadLogsNow", TRUE))
	{
		bReturnValue = COSADmlUploadLogsNow((ANSC_HANDLE)NULL, bValue);
		if( !bReturnValue )
		{
			return TRUE;
		}
	}

    return FALSE;
}

ULONG
Logging_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    if( AnscEqualString(ParamName, "xOpsDMLogsUploadStatus", TRUE))
    {
        /* collect value */
        COSADmlUploadLogsStatus(NULL, pValue,pUlSize);
        return 0;
    }
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/***********************************************************************

 APIs for Object:

    DeviceInfo.X_RDKCENTRAL-COM_RFC.Control

    *  Control_GetParamUlongValue
    *  Control_SetParamUlongValue
***********************************************************************/


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        Control_GetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                        pUlValue
            );

    description:

        This function is called to retrieve integer parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                        pUlValue
                The buffer of returned unsigned integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
Control_GetParamUlongValue
    (
	ANSC_HANDLE 		hInsContext,
	char*			ParamName,
	ULONG*			pUlValue
    )
{
	PCOSA_DATAMODEL_DEVICEINFO		pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

	/* check the parameter name and return the corresponding value */
	if( AnscEqualString(ParamName, "RetrieveNow", TRUE))
	{
		*pUlValue =  0;
		return TRUE;
	}

	return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        Control_SetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG                         ulValue
            );

    description:

        This function is called to set integer parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                         ulValue
                The updated unsigned integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
Control_SetParamUlongValue
    (
	ANSC_HANDLE 		hInsContext,
	char*			ParamName,
	ULONG 			ulValue
    )
{
    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "RetrieveNow", TRUE))
    {
	if( ulValue == 1 )
	{
		AnscTraceWarning((" Run RFC rfc.service\n"));
		system("sh /lib/rdk/rfc.service &");
	}

        return TRUE;
    }

    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Feature_GetParamBoolValue
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
Feature_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* check the parameter name and return the corresponding value */
#if defined(MOCA_HOME_ISOLATION)
    if( AnscEqualString(ParamName, "HomeNetworkIsolation", TRUE))
    {
        /* collect value */

    char *strValue = NULL;
    char str[2];
    int retPsmGet = CCSP_SUCCESS;

    retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, "dmsb.l2net.HomeNetworkIsolation", NULL, &strValue);
    if (retPsmGet == CCSP_SUCCESS) {
        *pBool = _ansc_atoi(strValue);
        ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(strValue);
    }
    else
        *pBool = FALSE;

     return TRUE;
    }

#endif
    if( AnscEqualString(ParamName, "ContainerSupport", TRUE))
    {
       /* Collect Value */
       char *strValue = NULL;
       char str[2];
       int retPsmGet = CCSP_SUCCESS;


        retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.Container", NULL, &strValue);
        if (retPsmGet == CCSP_SUCCESS) {
            *pBool = _ansc_atoi(strValue);
            ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(strValue);
        }
        else
            *pBool = FALSE;

         return TRUE;
    }

    if( AnscEqualString(ParamName, "CodebigSupport", TRUE))
    {
         char value[8];
         syscfg_get(NULL,"codebigsupport",value, sizeof(value));
         if( value != NULL )
         {
             if (strcmp(value, "true") == 0)
                 *pBool = TRUE;
             else
                 *pBool = FALSE;
         }     
         return TRUE;
    }

    return FALSE;
}

BOOL
SyndicationFlowControl_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    PCOSA_DATAMODEL_RDKB_SYNDICATIONFLOWCONTROL SyndicatonFlowControl = (PCOSA_DATAMODEL_RDKB_SYNDICATIONFLOWCONTROL)&(pMyObject->SyndicatonFlowControl);
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        *pBool = SyndicatonFlowControl->Enable;
        return TRUE;
    }
    return FALSE;
}

ULONG
SyndicationFlowControl_GetParamStringValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    PCOSA_DATAMODEL_RDKB_SYNDICATIONFLOWCONTROL SyndicatonFlowControl = (PCOSA_DATAMODEL_RDKB_SYNDICATIONFLOWCONTROL)&(pMyObject->SyndicatonFlowControl);
    if( AnscEqualString(ParamName, "InitialForwardedMark", TRUE))
    {
       if ( AnscSizeOfString(SyndicatonFlowControl->InitialForwardedMark) < *pUlSize)
        {
            AnscCopyString( pValue, SyndicatonFlowControl->InitialForwardedMark);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(SyndicatonFlowControl->InitialForwardedMark)+1;
            return 1;
        }
    }
    if( AnscEqualString(ParamName, "InitialOutputMark", TRUE))
    {
        if ( AnscSizeOfString(SyndicatonFlowControl->InitialOutputMark) < *pUlSize)
        {
            AnscCopyString( pValue, SyndicatonFlowControl->InitialOutputMark);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(SyndicatonFlowControl->InitialOutputMark)+1;
            return 1;
        }
    }
    return -1;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        EncryptCloudUpload_GetParamBoolValue
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
EncryptCloudUpload_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        char value[8];
        syscfg_get(NULL,"encryptcloudupload",value, sizeof(value));
        if( value != NULL )
        {
             if (strcmp(value, "true") == 0)
                 *pBool = TRUE;
             else
                 *pBool = FALSE;
        }     
        return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        MEMSWAP_GetParamBoolValue
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
MEMSWAP_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
       /* Collect Value */
       char *strValue = NULL;
       char str[2];
       int retPsmGet = CCSP_SUCCESS;


        retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.MEMSWAP.Enable", NULL, &strValue);
        if (retPsmGet == CCSP_SUCCESS) {
            *pBool = _ansc_atoi(strValue);
            ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(strValue);
        }
        else
            *pBool = FALSE;

         return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        DNSSTRICTORDER_GetParamBoolValue
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
DNSSTRICTORDER_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        char value[8];
        syscfg_get(NULL,"DNSStrictOrder",value, sizeof(value));
        if( value != NULL )
        {
             if (strcmp(value, "true") == 0)
                 *pBool = TRUE;
             else
                 *pBool = FALSE;
        } 
        return TRUE;
    }
    return FALSE;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Feature_SetParamBoolValue
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
Feature_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
#if defined(MOCA_HOME_ISOLATION)
    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "HomeNetworkIsolation", TRUE))
    {
    char *strValue = NULL;
    char str[2];
    int retPsmGet = CCSP_SUCCESS;
    BOOL getVal = 0;

    retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, "dmsb.l2net.HomeNetworkIsolation", NULL, &strValue);
    if (retPsmGet == CCSP_SUCCESS) {
        getVal = _ansc_atoi(strValue);
        ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(strValue);
    }

    if(getVal != bValue)
	{
             sprintf(str,"%d",bValue);
             retPsmGet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, "dmsb.l2net.HomeNetworkIsolation", ccsp_string, str);
             if (retPsmGet != CCSP_SUCCESS) {
             CcspTraceError(("Set failed for HomeNetworkIsolation \n"));
             return ANSC_STATUS_FAILURE;
             }
                if(bValue)
                {
                    CcspTraceInfo(("Apply changes for HomeNetworkIsolation \n"));
#if defined(_COSA_BCM_MIPS_)
                    system("sh /usr/ccsp/lan_handler.sh home_lan_isolation_enable &");
                    sleep(2);
#else
                    system("sysevent set multinet-restart 1");
#endif
                    system("sh /usr/ccsp/moca/MoCA_isolation.sh &");
                    
                }
                else
                {

                    CcspTraceInfo(("reverting changes for HomeNetworkIsolation \n"));
#if defined(_COSA_BCM_MIPS_)
                    system("sh /usr/ccsp/lan_handler.sh home_lan_isolation_disable &");
                    system("rm /tmp/MoCABridge_up");
#else
                    system("sysevent set multinet-down 9");
                    system("rm /tmp/MoCABridge_up");
                    system("sysevent set multinet-restart 1");
#endif

                    system("killall MRD; killall smcroute;igmpproxy -c /tmp/igmpproxy.conf");
                }
	}
	else
		CcspTraceInfo(("HomeNetworkIsolation is already %d \n",getVal));
	
    	return TRUE;
    }
#endif
    if( AnscEqualString(ParamName, "ContainerSupport", TRUE))
    {
       char str[2];
       int retPsmGet = CCSP_SUCCESS;
       
       if ( bValue == TRUE)
       {
          syscfg_set(NULL, "containersupport", "true");
       }
       else
       {
          syscfg_set(NULL, "containersupport", "false");
       }
       syscfg_commit();
       
       sprintf(str,"%d",bValue);
       retPsmGet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.Container", ccsp_string, str);
       if (retPsmGet != CCSP_SUCCESS) {
       CcspTraceError(("Set failed for ContainerSupport \n"));
       return ANSC_STATUS_FAILURE;
       }
       CcspTraceInfo(("Successfully set ContainerSupport \n"));
       return TRUE;
    }

    if( AnscEqualString(ParamName, "CodebigSupport", TRUE))
    {
        if ( bValue == TRUE)
        {
            syscfg_set(NULL, "codebigsupport", "true");
        }
        else
        {
            syscfg_set(NULL, "codebigsupport", "false");
        }
        syscfg_commit();
        return TRUE;
    }

    return FALSE;
}

BOOL
SyndicationFlowControl_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    PCOSA_DATAMODEL_RDKB_SYNDICATIONFLOWCONTROL SyndicatonFlowControl = (PCOSA_DATAMODEL_RDKB_SYNDICATIONFLOWCONTROL)&(pMyObject->SyndicatonFlowControl);
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        if(CosaDmlDiSet_SyndicationFlowControl_Enable(bValue) == 0)
        {
            SyndicatonFlowControl->Enable =bValue;
            return TRUE;
        }
    }
    return FALSE;
}

BOOL
SyndicationFlowControl_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    CcspTraceWarning(("\nSyndicationFlowControl_SetParamStringValue\n"));
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    PCOSA_DATAMODEL_RDKB_SYNDICATIONFLOWCONTROL SyndicatonFlowControl = (PCOSA_DATAMODEL_RDKB_SYNDICATIONFLOWCONTROL)&(pMyObject->SyndicatonFlowControl);
    if( AnscEqualString(ParamName, "InitialForwardedMark", TRUE))
    {
        if(CosaDmlDiSet_SyndicationFlowControl_InitialForwardedMark(SyndicatonFlowControl->InitialForwardedMark)==0)
        {
            AnscCopyString(SyndicatonFlowControl->InitialForwardedMark, pString);
            return TRUE;
        }
    }
    if( AnscEqualString(ParamName, "InitialOutputMark", TRUE))
    {
        if(CosaDmlDiSet_SyndicationFlowControl_InitialOutputMark(SyndicatonFlowControl->InitialOutputMark)==0)
        {
            AnscCopyString(SyndicatonFlowControl->InitialOutputMark, pString);
            return TRUE;
        }
    }
    return FALSE;
}
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        EncryptCloudUpload_SetParamBoolValue
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
EncryptCloudUpload_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        if ( bValue == TRUE)
        {
            syscfg_set(NULL, "encryptcloudupload", "true");
        }
        else
        {
            syscfg_set(NULL, "encryptcloudupload", "false");
        }
        syscfg_commit();
        return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        MEMSWAP_SetParamBoolValue
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
MEMSWAP_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
       char str[2];
       int retPsmGet = CCSP_SUCCESS;

       sprintf(str,"%d",bValue);
       retPsmGet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.MEMSWAP.Enable", ccsp_string, str);
       if (retPsmGet != CCSP_SUCCESS) {
           CcspTraceError(("Set failed for ContainerSupport \n"));
           return ANSC_STATUS_FAILURE;
       }
       CcspTraceInfo(("Successfully set ContainerSupport \n"));
       return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        DNSSTRICTORDER_SetParamBoolValue
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
DNSSTRICTORDER_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        if ( bValue == TRUE)
        {
            syscfg_set(NULL, "DNSStrictOrder", "true");
        }
        else
        {
            syscfg_set(NULL, "DNSStrictOrder", "false");
        }
        syscfg_commit();
        return TRUE;
    }
    return FALSE;
}

/* CodeBigFirst config download can be enabled/disabled               */
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        CodeBigFirst_GetParamBoolValue
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
CodeBigFirst_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    char buf[8];

    /* check the parameter name and return the corresponding value */

    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* collect value */
        syscfg_get( NULL, "CodeBigFirstEnabled", buf, sizeof(buf));

        if( buf != NULL )
        {
            if (strcmp(buf, "true") == 0)
                *pBool = TRUE;
            else
                *pBool = FALSE;
        }
        return TRUE;
    }

    return FALSE;
}

/* CodeBigFirst  download can be enabled/disabled                     */
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        CodeBig_First_SetParamBoolValue
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
CodeBig_First_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* collect value */
        if( bValue == TRUE)
        {
            syscfg_set(NULL, "CodeBigFirstEnabled", "true");
        }
        else
        {
            syscfg_set(NULL, "CodeBigFirstEnabled", "false");
        }
        syscfg_commit();
        return TRUE;
    }

    return FALSE;
}

/* TFTP config download can be enabled/disabled for bci routers */
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        TFTPConfigDownload_GetParamBoolValue
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
TFTPConfigDownload_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    char buf[8];

    /* check the parameter name and return the corresponding value */

    if( AnscEqualString(ParamName, "Enabled", TRUE))
    {
        /* collect value */
        syscfg_get( NULL, "TFTPConfigDownloadEnabled", buf, sizeof(buf));

        if( buf != NULL )
        {
            if (strcmp(buf, "true") == 0)
                *pBool = TRUE;
            else
                *pBool = FALSE;
        }
        return TRUE;
    }

    return FALSE;
}


/* TFTP config download can be enabled/disabled for bci routers */
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        TFTPConfigDownload_SetParamBoolValue
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
TFTPConfigDownload_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    if( AnscEqualString(ParamName, "Enabled", TRUE))
    {
        /* collect value */
        if( bValue == TRUE)
        {
            syscfg_set(NULL, "TFTPConfigDownloadEnabled", "true");
            syscfg_commit();
        }
        else
        {
            syscfg_set(NULL, "TFTPConfigDownloadEnabled", "false");
            syscfg_commit();
        }
        return TRUE;
    }

    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BLE_GetParamBoolValue
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
BLE_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    char buf[8];
    if( AnscEqualString(ParamName, "Discovery", TRUE))
    {
        /* collect value */
        syscfg_get( NULL, "BLEDiscovery", buf, sizeof(buf));

        if( buf != NULL )
        {
            if (strcmp(buf, "true") == 0)
                *pBool = TRUE;
            else
                *pBool = FALSE;
        }
        return TRUE;
    }

    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BLE_SetParamBoolValue
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
BLE_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    if( AnscEqualString(ParamName, "Discovery", TRUE))
    {
        /* collect value */
        if( bValue == TRUE)
        {
            syscfg_set(NULL, "BLEDiscovery", "true");
            syscfg_commit();
        }
        else
        {
            syscfg_set(NULL, "BLEDiscovery", "false");
            syscfg_commit();
        }
        return TRUE;
    }

    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        Tile_GetParamStringValue
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

                char*                       pValue
                The string value buffer;


    return:     0 if succeeded;
                -1 if not supported.

**********************************************************************/
ULONG
Tile_GetParamStringValue
(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 char*                       pValue,
 ULONG*                      pUlSize
 )
{
    if( AnscEqualString(ParamName, "ReportingURL", TRUE))
    {
        /* collect value */
        char buf[256]="";
        memset(buf,0,sizeof(buf));
        syscfg_get( NULL, "TileReportingURL", buf, sizeof(buf));

        if( buf != NULL )
        {
            AnscCopyString(pValue, buf);
            *pUlSize = AnscSizeOfString(pValue); 
            return 0;
        }
    }
    CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return -1;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        Tile_SetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pString
            );

    description:

        This function is called to set BOOL parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pString
                The updated string value to be set;

    return:     TRUE if succeeded.

	**********************************************************************/
BOOL
Tile_SetParamStringValue
(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 char*                       pString
 )
{
    if( AnscEqualString(ParamName, "ReportingURL", TRUE))
    {
        if (syscfg_set(NULL, "TileReportingURL", pString) != 0)
        {
            CcspTraceError(("syscfg_set failed\n"));
        }
        else
        {
            if (syscfg_commit() != 0)
            {
                CcspTraceError(("syscfg_commit failed\n"));
            }
            return TRUE;
        }
    }

    CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SSIDPSWDCTRL_GetParamBoolValue
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
SSIDPSWDCTRL_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
        char buf[8];

    /* check the parameter name and return the corresponding value */

        if( AnscEqualString(ParamName, "SnmpEnable", TRUE))
                {
                        /* collect value */
                        syscfg_get( NULL, "SNMPPSWDCTRLFLAG", buf, sizeof(buf));

                        if( buf != NULL )
                        {
                                if (strcmp(buf, "true") == 0)
                                        *pBool = TRUE;
                                else
                                        *pBool = FALSE;
                        }
                        return TRUE;
                }    
        if( AnscEqualString(ParamName, "Tr069Enable", TRUE))
                {
                        /* collect value */
                        syscfg_get( NULL, "TR069PSWDCTRLFLAG", buf, sizeof(buf));

                        if( buf != NULL )
                        {
                                if (strcmp(buf, "true") == 0)
                                        *pBool = TRUE;
                                else
                                        *pBool = FALSE;
                        }
                        return TRUE;
                } 
                

    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SSIDPSWDCTRL_SetParamBoolValue
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
SSIDPSWDCTRL_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
            if( AnscEqualString(ParamName, "SnmpEnable", TRUE))
                {
                        /* collect value */
                        if( bValue == TRUE)
                        {
                                        syscfg_set(NULL, "SNMPPSWDCTRLFLAG", "true");
                                        syscfg_commit();
                        }
                        else
                        {
                                        syscfg_set(NULL, "SNMPPSWDCTRLFLAG", "false");
                                        syscfg_commit();
                        }
                        return TRUE;
                }
            if( AnscEqualString(ParamName, "Tr069Enable", TRUE))
                {
                        /* collect value */
                        if( bValue == TRUE)
                        {
                                        syscfg_set(NULL, "TR069PSWDCTRLFLAG", "true");
                                        syscfg_commit();
                        }
                        else
                        {
                                        syscfg_set(NULL, "TR069PSWDCTRLFLAG", "false");
                                        syscfg_commit();
                        }
                        return TRUE;
                }
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        PeriodicFWCheck_GetParamBoolValue
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
PeriodicFWCheck_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* check the parameter name and return the corresponding value */

	if( AnscEqualString(ParamName, "Enable", TRUE))
		{
			/* collect value */
			char buf[8];
			syscfg_get( NULL, "PeriodicFWCheck_Enable", buf, sizeof(buf));
	
			if( buf != NULL )
			{
				if (strcmp(buf, "true") == 0)
					*pBool = TRUE;
				else
					*pBool = FALSE;
			}
			return TRUE;
		}

    return FALSE;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        PeriodicFWCheck_SetParamBoolValue
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
PeriodicFWCheck_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
 	    if( AnscEqualString(ParamName, "Enable", TRUE))
		{
			/* collect value */
			if( bValue == TRUE)
			{
					syscfg_set(NULL, "PeriodicFWCheck_Enable", "true");
					syscfg_commit();
					system("/etc/firmwareSched.sh &");
			}
			else
			{
					syscfg_set(NULL, "PeriodicFWCheck_Enable", "false");
					syscfg_commit();
	
					system("sh /etc/firmwareSched.sh RemoveCronJob");
					system("killall -9 firmwareSched.sh");
			}
			return TRUE;
		}	
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        AllowOpenPorts_GetParamBoolValue
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
AllowOpenPorts_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* check the parameter name and return the corresponding value */

    if( AnscEqualString(ParamName, "Enable", TRUE))
        {
            /* collect value */
            char buf[8];
            syscfg_get( NULL, "RFCAllowOpenPorts", buf, sizeof(buf));

            if( buf != NULL )
            {
                if (strcmp(buf, "true") == 0)
                    *pBool = TRUE;
                else
                    *pBool = FALSE;
            }
            return TRUE;
        }

    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        AllowOpenPorts_SetParamBoolValue
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
AllowOpenPorts_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
        if( AnscEqualString(ParamName, "Enable", TRUE))
        {
            /* collect value */
            syscfg_set(NULL, "RFCAllowOpenPorts", (bValue == TRUE ? "true": "false"));
            syscfg_commit();

            // Log that we are allowing open ports (or not)
            CcspTraceWarning(("RFC_AllowOpenPorts set to '%s'\n", (bValue == TRUE ? "true":"false")));

            // restart firewall
            system("sysevent set firewall-restart");
            return TRUE;
        }
    return FALSE;
}
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6subPrefix_GetParamBoolValue
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
IPv6subPrefix_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* check the parameter name and return the corresponding value */

    if( AnscEqualString(ParamName, "Enable", TRUE))
        {
            /* collect value */
            char buf[8];
            syscfg_get( NULL, "IPv6subPrefix", buf, sizeof(buf));

            if( buf != NULL )
            {
                if (strcmp(buf, "true") == 0)
                    *pBool = TRUE;
                else
                    *pBool = FALSE;
            }
            return TRUE;
        }

    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        IPv6subPrefix_SetParamBoolValue
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
IPv6subPrefix_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
        if( AnscEqualString(ParamName, "Enable", TRUE))
        {
            /* collect value */
            syscfg_set(NULL, "IPv6subPrefix", (bValue == TRUE ? "true": "false"));
            syscfg_commit();
            return TRUE;
        }
    return FALSE;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6onLnF_GetParamBoolValue
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
IPv6onLnF_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* check the parameter name and return the corresponding value */

    if( AnscEqualString(ParamName, "Enable", TRUE))
        {
            /* collect value */
            char buf[128];
	    char Inf_name[32];
	
	    syscfg_get( NULL, "iot_ifname", Inf_name, sizeof(Inf_name));
	    if( Inf_name != NULL )
            {
 
            
            syscfg_get( NULL, "IPv6_Interface", buf, sizeof(buf));

		    if( buf != NULL )
		    {
		        if (strstr(buf, Inf_name))
		            *pBool = TRUE;
		        else
		            *pBool = FALSE;
		    }
		    return TRUE;
	    }
        }

    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        IPv6onLnF_SetParamBoolValue
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
IPv6onLnF_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
char *token = NULL;char *pt;
        if( AnscEqualString(ParamName, "Enable", TRUE))
        {
     	    char buf[128], OutBuff[128];
	    char Inf_name[32];
	    BOOL bFound = FALSE;
	
	    syscfg_get( NULL, "iot_ifname", Inf_name, sizeof(Inf_name));
	    if( Inf_name != NULL )
            {
 
            
            syscfg_get( NULL, "IPv6_Interface", buf, sizeof(buf));

		    if( buf != NULL )
		    {
		        if (strstr(buf, Inf_name))
		            bFound = TRUE;
		        else
		            bFound = FALSE;

			
			    if(bValue)
				{
					if(bFound == FALSE)
					{
					// interface is not present in the list, we need to add interface to enable IPv6 PD

							strcpy(OutBuff,buf);
							strcat(OutBuff,Inf_name);
							strcat(OutBuff,",");
							syscfg_set(NULL, "IPv6_Interface",OutBuff);
            						syscfg_commit();

					}
				}
				else
				{
				
					if(bFound == TRUE)
					{
					// interface is present in the list, we need to remove interface to disable IPv6 PD
						pt = buf;
						   while(token = strtok_r(pt, ",", &pt)) {
							if(strncmp(Inf_name,token,strlen(Inf_name)))
							{
								strcat(OutBuff,token);
								strcat(OutBuff,",");
							}

						   }
					
						syscfg_set(NULL, "IPv6_Interface",OutBuff);
            					syscfg_commit();
					}
				}
		    }
		    else
			{
				if(bValue)
				{
				strcat(OutBuff,Inf_name);
				strcat(OutBuff,",");
				syscfg_set(NULL, "IPv6_Interface",OutBuff);
            			syscfg_commit();
				}
			}
		    return TRUE;
	    }

            return TRUE;
        }
    return FALSE;
}
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        Xconf_GetParamBoolValue
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
Xconf_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "xconfCheckNow", TRUE))
    {
        /* collect value */
                *pBool=FALSE;
                return TRUE;

    }

    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Xconf_SetParamBoolValue
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
Xconf_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    int status;
    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "xconfCheckNow", TRUE))
    {
      		AnscTraceWarning(("Triggering firmware download check from TR181\n"));
                if( TRUE == bValue )
                {
#if defined(INTEL_PUMA7) || defined(_COSA_BCM_ARM_)
#ifdef _CBR_PRODUCT_REQ_
            if(0 == system("pidof cbr_firmwareDwnld.sh"))  {
                           system ("kill -9 `pidof cbr_firmwareDwnld.sh `");
                       }
                           status = system("/etc/cbr_firmwareDwnld.sh cleanup &");
#else
			if(0 == system("pidof xb6_firmwareDwnld.sh"))  {
                           system ("kill -9 `pidof xb6_firmwareDwnld.sh `");
                       }
                           status = system("/etc/xb6_firmwareDwnld.sh cleanup &");
#endif
#elif defined(_COSA_BCM_MIPS_)
			if(0 == system("pidof xf3_firmwareDwnld.sh"))  {
                           system ("kill -9 `pidof xf3_firmwareDwnld.sh `");
                       }
                           status = system("/etc/xf3_firmwareDwnld.sh cleanup &");
#else
			if(0 == system("pidof xb3_firmwareDwnld.sh"))  {
                           system ("kill -9 `pidof xb3_firmwareDwnld.sh `");
                       }
                           status = system("/etc/xb3_firmwareDwnld.sh cleanup &");
#endif

                           if (0 == status)
                           {
                                 AnscTraceWarning(("xconf process started successfully\n"));
                           }
                           else
                           {
                                 AnscTraceWarning(("xconf process did not start successfully\n"));
                           }

                       return TRUE;
                }
                
     }
     return FALSE;
}

/***********************************************************************

 APIs for Object:

    Device.DeviceInfo.X_RDKCENTRAL-COM_xOpsDeviceMgmt.ReverseSSH

    *  ReverseSSH_SetParamStringValue // Set args required for reverse SSH
    *  ReverseSSH_GetParamStringValue // Get args set for reverse SSH

***********************************************************************/

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        ReverseSSH_GetParamStringValue
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

    return:     TRUE if succeeded;
                FALSE if failed

**********************************************************************/
ULONG
ReverseSSH_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    char* activeStr = "ACTIVE";
    char* inActiveStr = "INACTIVE";

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "xOpsReverseSshArgs", TRUE))
    {
        getXOpsReverseSshArgs(NULL, pValue,pulSize);
        return 0;
    }

    if( AnscEqualString(ParamName, "xOpsReverseSshStatus", TRUE))
    {
        if (isRevSshActive()) {
            AnscCopyString(pValue, activeStr);
            *pulSize = AnscSizeOfString(pValue);
        } else {
            AnscCopyString(pValue, inActiveStr);
            *pulSize = AnscSizeOfString(pValue);
        }
        return 0;
    }

    if( AnscEqualString(ParamName, "xOpsReverseSshTrigger", TRUE))
    {
         AnscCopyString(pValue, "");
        return 0;
    }

    CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return -1;
}



/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        ReverseSSH_SetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pString
            );

    description:

        This function is called to set string parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pString
                The updated string value;

    return:     TRUE if succeeded,
                FALSE if failed.

**********************************************************************/
BOOL
ReverseSSH_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    BOOL bReturnValue = FALSE;
    ANSC_STATUS retValue = ANSC_STATUS_FAILURE;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "xOpsReverseSshArgs", TRUE))
    {
        bReturnValue = setXOpsReverseSshArgs(pString);
        return TRUE ;
    }

    if( AnscEqualString(ParamName, "xOpsReverseSshTrigger", TRUE)) {
        bReturnValue = setXOpsReverseSshTrigger(pString);
        return TRUE ;

    }

    CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));

    return FALSE;
}


/* Maintenance window can be customized for bci routers */


/***********************************************************************

 APIs for Object:

    DeviceInfo.X_RDKCENTRAL-COM_MaintenanceWindow.

    *  MaintenanceWindow_GetParamStringValue
    *  MaintenanceWindow_SetParamStringValue

***********************************************************************/

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        MaintenanceWindow_GetParamStringValue
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

    return:     TRUE if succeeded;
                FALSE if failed

**********************************************************************/

BOOL
MaintenanceWindow_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    BOOL bReturnValue = FALSE;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "FirmwareUpgradeStartTime", TRUE))
    {
        /* collect value */
        bReturnValue = CosaDmlDiGetFirmwareUpgradeStartTime(NULL, pValue,pulSize);
        return bReturnValue;
    }

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "FirmwareUpgradeEndTime", TRUE))
    {
        /* collect value */
        bReturnValue = CosaDmlDiGetFirmwareUpgradeEndTime (NULL, pValue,pulSize);
        return bReturnValue;
    }

    CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return bReturnValue;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        MaintenanceWindow_SetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pString
            );

    description:

        This function is called to set string parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pString
                The updated string value;

    return:     TRUE if succeeded,
                FALSE if failed.

**********************************************************************/
BOOL
MaintenanceWindow_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    BOOL bReturnValue = FALSE;
    ANSC_STATUS retValue = ANSC_STATUS_FAILURE;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "FirmwareUpgradeStartTime", TRUE))
    {
        retValue = CosaDmlDiSetFirmwareUpgradeStartTime (pString);
    }

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "FirmwareUpgradeEndTime", TRUE))
    {
        retValue = CosaDmlDiSetFirmwareUpgradeEndTime (pString);
    }

    else
    {
        CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    }

    if (retValue == ANSC_STATUS_SUCCESS)
    {
        bReturnValue = TRUE;
    }
    else
    {
        bReturnValue = FALSE;
    }

    return bReturnValue;
}


/***********************************************************************

 APIs for Object:

    DeviceInfo.X_RDKCENTRAL-COM_Syndication.

    *  MaintenanceWindow_GetParamStringValue
***********************************************************************/

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Syndication_GetParamStringValue
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
Syndication_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

    if( AnscEqualString(ParamName, "PartnerId", TRUE))
    {
        /* collect value */
        //CosaDmlDiGetSyndicationPartnerId(NULL, pValue,pulSize);
        AnscCopyString( pValue, pMyObject->PartnerID);
		*pulSize = AnscSizeOfString( pValue );	
        return 0;
    }

    if( AnscEqualString(ParamName, "TR69CertLocation", TRUE))
    {
        /* collect value */
		AnscCopyString( pValue, pMyObject->TR69CertLocation );
		*pulSize = AnscSizeOfString( pValue );		
        return 0;
    }
    if( AnscEqualString(ParamName, "LocalUIBrandingTable", TRUE))
    {
        /* collect value */
        CosaDmlDiGetSyndicationLocalUIBrandingTable(NULL, pValue,pulSize);
        return 0;
    }
    if( AnscEqualString(ParamName, "WifiUIBrandingTable", TRUE))
    {
        /* collect value */
        CosaDmlDiGetSyndicationWifiUIBrandingTable(NULL, pValue,pulSize);
        return 0;
    }
    if( AnscEqualString(ParamName, "PauseScreenFileLocation", TRUE))
    {
    	 /* collect value */
	 if ( AnscSizeOfString(pMyObject->UiBrand.PauseScreenFileLocation) < *pulSize)
	 {
		 AnscCopyString( pValue, pMyObject->UiBrand.PauseScreenFileLocation);		
		 return 0;
	 }
	 else
	 {
	 	 *pulSize = AnscSizeOfString(pMyObject->UiBrand.PauseScreenFileLocation)+1;
		 return 1;
	 }
     }

    return -1;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        Syndication_SetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pString
            );

    description:

        This function is called to set string parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pString
                The updated string value;

    return:     TRUE if succeeded,
                FALSE if failed.

**********************************************************************/
BOOL
Syndication_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    ANSC_STATUS 					retValue  = ANSC_STATUS_FAILURE;
    char PartnerID[PARTNER_ID_LEN] = {0};

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "TR69CertLocation", TRUE) )
    {
		retValue = CosaDmlDiSetSyndicationTR69CertLocation( hInsContext, pString );
		if( ANSC_STATUS_SUCCESS == retValue )
		{
			memset( pMyObject->TR69CertLocation, 0, sizeof( pMyObject->TR69CertLocation ));
			AnscCopyString( pMyObject->TR69CertLocation, pString );
		}
		
		return TRUE;
    }
    if( AnscEqualString(ParamName, "PartnerId", TRUE) )
    {
		if ( (strcmp(pMyObject->PartnerID,pString) != 0 ) )
		{
			retValue = setPartnerId( pString );
			if( ANSC_STATUS_SUCCESS == retValue )
			{
			   ULONG    size = 0;
				//Get the Factory PartnerID
			    memset(PartnerID, 0, sizeof(PartnerID));
			    getFactoryPartnerId(PartnerID, &size);
			
			    CcspTraceInfo(("[SET-PARTNERID] Factory_Partner_ID:%s\n", ( PartnerID[ 0 ] != '\0' ) ? PartnerID : "NULL" ));
			    CcspTraceInfo(("[SET-PARTNERID] Current_PartnerID:%s\n", pMyObject->PartnerID ));
			    CcspTraceInfo(("[SET-PARTNERID] Overriding_PartnerID:%s\n", pString ));
								
				memset( pMyObject->PartnerID, 0, sizeof( pMyObject->PartnerID ));
				AnscCopyString( pMyObject->PartnerID, pString );
			}
		
			return TRUE;
		}
    }
    if((CCSP_SUCCESS == getPartnerId(PartnerID) ) && ( PartnerID[ 0 ] != '\0'))
    {
   	 /* check the parameter name and set the corresponding value */
	 if( AnscEqualString(ParamName, "PauseScreenFileLocation", TRUE) )
	{
		if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.PauseScreenFileLocation",PartnerID, pString))
		{
			memset( pMyObject->UiBrand.PauseScreenFileLocation, 0, sizeof( pMyObject->UiBrand.PauseScreenFileLocation ));
			AnscCopyString( pMyObject->UiBrand.PauseScreenFileLocation, pString );
			return TRUE;
		}	
	 }
	  
      }

    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Syndication_GetParamBoolValue
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
Syndication_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "enable", TRUE))
    {
       /* Collect Value */
       *pBool = pMyObject->SyndicationEnable;
        return TRUE;
    }

    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
       Syndication_SetParamBoolValue
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
Syndication_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    ANSC_STATUS 					retValue  = ANSC_STATUS_FAILURE;

    if( AnscEqualString(ParamName, "enable", TRUE))
    {
		if( bValue != pMyObject->SyndicationEnable )
		{
			retValue = CosaDmlDiSetSyndicationEnable( hInsContext, bValue ); 
			if( ANSC_STATUS_SUCCESS == retValue )
			{
				pMyObject->SyndicationEnable = bValue;
			}
		}

       return TRUE;
    }

    return FALSE;
}

/***********************************************************************
APIs for Object:
	DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.
	  *  MaintenanceWindow_GetParamStringValue
	  
***********************************************************************/

ULONG
RDKB_UIBranding_GetParamStringValue

	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		char*						pValue,
		ULONG*						pulSize
	)
{

	PCOSA_DATAMODEL_DEVICEINFO		pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
	PCOSA_DATAMODEL_RDKB_UIBRANDING	pBindObj =	& pMyObject->UiBrand;
	
	 if( AnscEqualString(ParamName, "DefaultAdminIP", TRUE))
        {
		if ( AnscSizeOfString(pBindObj->DefaultAdminIP) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->DefaultAdminIP);		
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->DefaultAdminIP)+1;
           		return 1;
       		}

        } 
	if( AnscEqualString(ParamName, "DefaultLocalIPv4SubnetRange", TRUE))
        {
		if ( AnscSizeOfString(pBindObj->DefaultLocalIPv4SubnetRange) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->DefaultLocalIPv4SubnetRange);		
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->DefaultLocalIPv4SubnetRange)+1;
           		return 1;
       		}

        } 
	 return -1;
}

BOOL
RDKB_UIBranding_SetParamStringValue


    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    return FALSE;
}

/***********************************************************************
APIs for Object:
	DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.
	  *  MaintenanceWindow_GetParamStringValue
	  
***********************************************************************/
ULONG
Footer_GetParamStringValue
	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		char*						pValue,
		ULONG*						pulSize
	)
{
	PCOSA_DATAMODEL_DEVICEINFO		pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
	PCOSA_DATAMODEL_RDKB_UIBRANDING	pBindObj =	& pMyObject->UiBrand;

	if( AnscEqualString(ParamName, "PartnerLink", TRUE))
	{
		/* collect value */
		 if ( AnscSizeOfString(pBindObj->Footer.PartnerLink) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->Footer.PartnerLink);
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->Footer.PartnerLink)+1;
           		return 1;
       		}
		
	}

	if( AnscEqualString(ParamName, "UserGuideLink", TRUE))
	{
		 if ( AnscSizeOfString(pBindObj->Footer.UserGuideLink) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->Footer.UserGuideLink);
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->Footer.UserGuideLink)+1;
           		return 1;
       		}
	}
	
	if( AnscEqualString(ParamName, "CustomerCentralLink", TRUE))
	{
		if ( AnscSizeOfString(pBindObj->Footer.CustomerCentralLink) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->Footer.CustomerCentralLink);
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->Footer.CustomerCentralLink)+1;
           		return 1;
       		}

	}

	if( AnscEqualString(ParamName, "PartnerText", TRUE))
	{
		if ( AnscSizeOfString(pBindObj->Footer.PartnerText) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->Footer.PartnerText);
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->Footer.PartnerText)+1;
           		return 1;
       		}

	}

	if( AnscEqualString(ParamName, "UserGuideText", TRUE))
	{
		if ( AnscSizeOfString(pBindObj->Footer.UserGuideText) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->Footer.UserGuideText);
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->Footer.UserGuideText)+1;
           		return 1;
       		}

	}

	if( AnscEqualString(ParamName, "CustomerCentralText", TRUE))
	{
		if ( AnscSizeOfString(pBindObj->Footer.CustomerCentralText) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->Footer.CustomerCentralText);
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->Footer.CustomerCentralText)+1;
           		return 1;
       		}

	}
	return -1;

}


BOOL
Footer_SetParamStringValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    PCOSA_DATAMODEL_RDKB_UIBRANDING	pBindObj =	& pMyObject->UiBrand;

	char PartnerID[PARTNER_ID_LEN] = {0};
	
   if((CCSP_SUCCESS == getPartnerId(PartnerID) ) && (PartnerID[ 0 ] != '\0') )
   {
   	 /* check the parameter name and set the corresponding value */
	    if( AnscEqualString(ParamName, "PartnerLink", TRUE) )
	    {
			if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.PartnerLink",PartnerID,pString))
			{
				memset( pBindObj->Footer.PartnerLink, 0, sizeof( pBindObj->Footer.PartnerLink ));
				AnscCopyString( pBindObj->Footer.PartnerLink, pString );
				return TRUE;
			}		

	    }
	    if( AnscEqualString(ParamName, "UserGuideLink", TRUE) )
	    {

			if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.UserGuideLink",PartnerID,pString))
			{
				memset( pBindObj->Footer.UserGuideLink, 0, sizeof( pBindObj->Footer.UserGuideLink ));
				AnscCopyString(pBindObj->Footer.UserGuideLink, pString );
				return TRUE;
			}	

	    }
	    if( AnscEqualString(ParamName, "CustomerCentralLink", TRUE) )
	    {
			if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.CustomerCentralLink",PartnerID,pString))
			{
				memset( pBindObj->Footer.CustomerCentralLink, 0, sizeof( pBindObj->Footer.CustomerCentralLink ));
				AnscCopyString( pBindObj->Footer.CustomerCentralLink, pString );
				return TRUE;
			}
			
	    }

	if( AnscEqualString(ParamName, "PartnerText", TRUE) )
	{
		if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.PartnerText",PartnerID,pString))
		{
			memset( pBindObj->Footer.PartnerText, 0, sizeof( pBindObj->Footer.PartnerText ));
			AnscCopyString( pBindObj->Footer.PartnerText, pString );
			return TRUE;
		}

	}

	if( AnscEqualString(ParamName, "UserGuideText", TRUE) )
	{
		if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.UserGuideText",PartnerID,pString))
		{
			memset( pBindObj->Footer.UserGuideText, 0, sizeof( pBindObj->Footer.UserGuideText ));
			AnscCopyString( pBindObj->Footer.UserGuideText, pString );
			return TRUE;
		}

	}

 	if( AnscEqualString(ParamName, "CustomerCentralText", TRUE) )
 	{
 		if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.CustomerCentralText",PartnerID,pString))
 		{
 			memset( pBindObj->Footer.CustomerCentralText, 0, sizeof( pBindObj->Footer.CustomerCentralText ));
 			AnscCopyString( pBindObj->Footer.CustomerCentralText, pString );
 			return TRUE;
 		}
 		
 	}

   }
   
    return FALSE;
}

/***********************************************************************
APIs for Object:
	DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Connection.
	  *  MaintenanceWindow_GetParamStringValue
	  
***********************************************************************/
ULONG
Connection_GetParamStringValue

	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		char*						pValue,
		ULONG*						pulSize
	)
{
	PCOSA_DATAMODEL_DEVICEINFO		pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
	PCOSA_DATAMODEL_RDKB_UIBRANDING	pBindObj =	& pMyObject->UiBrand;

	if( AnscEqualString(ParamName, "MSOmenu", TRUE))
	{
		if ( AnscSizeOfString(pBindObj->Connection.MSOmenu) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->Connection.MSOmenu);
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->Connection.MSOmenu)+1;
           		return 1;
       		}
	}

	if( AnscEqualString(ParamName, "MSOinfo", TRUE))
	{
		if ( AnscSizeOfString(pBindObj->Connection.MSOinfo) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->Connection.MSOinfo);	
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->Connection.MSOinfo)+1;
           		return 1;
       		}
		
	}
	
	if( AnscEqualString(ParamName, "StatusTitle", TRUE))
	{
		if ( AnscSizeOfString(pBindObj->Connection.StatusTitle) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->Connection.StatusTitle);
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->Connection.StatusTitle)+1;
           		return 1;
       		}
	}
	
	if( AnscEqualString(ParamName, "StatusInfo", TRUE))
	{
		if ( AnscSizeOfString(pBindObj->Connection.StatusInfo) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->Connection.StatusInfo);		
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->Connection.StatusInfo)+1;
           		return 1;
       		}
	}
	
	return -1;

}


BOOL
Connection_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    PCOSA_DATAMODEL_RDKB_UIBRANDING	pBindObj =	& pMyObject->UiBrand;

	char PartnerID[PARTNER_ID_LEN] = {0};
	
   if((CCSP_SUCCESS == getPartnerId(PartnerID) ) && ( PartnerID[ 0 ] != '\0') )
   {
   	 /* check the parameter name and set the corresponding value */
	    if( AnscEqualString(ParamName, "MSOmenu", TRUE) )
	    {
			if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Connection.MSOmenu",PartnerID,pString))
			{
				memset( pBindObj->Connection.MSOmenu, 0, sizeof( pBindObj->Connection.MSOmenu ));
				AnscCopyString( pBindObj->Connection.MSOmenu, pString );
				return TRUE;
			}
	    }
	    if( AnscEqualString(ParamName, "MSOinfo", TRUE) )
	    {	
			if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Connection.MSOinfo",PartnerID,pString) )
			{
				memset( pBindObj->Connection.MSOinfo, 0, sizeof( pBindObj->Connection.MSOinfo ));
				AnscCopyString(pBindObj->Connection.MSOinfo, pString );
				return TRUE;
			}
	    }
	    if( AnscEqualString(ParamName, "StatusTitle", TRUE) )
	    {
			if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Connection.StatusTitle",PartnerID,pString) )
			{
				memset( pBindObj->Connection.StatusTitle, 0, sizeof( pBindObj->Connection.StatusTitle ));
				AnscCopyString( pBindObj->Connection.StatusTitle, pString );
				return TRUE;
			}
			
	    }
	    if( AnscEqualString(ParamName, "StatusInfo", TRUE) )
	    {

			if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Connection.StatusInfo",PartnerID,pString))
			{
				memset(pBindObj->Connection.StatusInfo, 0, sizeof( pBindObj->Connection.StatusInfo ));
				AnscCopyString( pBindObj->Connection.StatusInfo, pString );
				return TRUE;
			}
			
	    }
   }
    return FALSE;
}

/***********************************************************************
APIs for Object:
	DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.NetworkDiagnosticTools.
	  *  MaintenanceWindow_GetParamStringValue
	  
***********************************************************************/
ULONG
NetworkDiagnosticTools_GetParamStringValue
	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		char*						pValue,
		ULONG*						pulSize
	)
{
	PCOSA_DATAMODEL_DEVICEINFO		pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
	PCOSA_DATAMODEL_RDKB_UIBRANDING	pBindObj =	& pMyObject->UiBrand;

	if( AnscEqualString(ParamName, "ConnectivityTestURL", TRUE))
	{
		if ( AnscSizeOfString(pBindObj->NDiagTool.ConnectivityTestURL) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->NDiagTool.ConnectivityTestURL);		
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->NDiagTool.ConnectivityTestURL)+1;
           		return 1;
       		}
	}
	
	return -1;

}


BOOL
NetworkDiagnosticTools_SetParamStringValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    PCOSA_DATAMODEL_RDKB_UIBRANDING	pBindObj =	& pMyObject->UiBrand;

	char PartnerID[PARTNER_ID_LEN] = {0};
	
   if((CCSP_SUCCESS == getPartnerId(PartnerID) ) && ( PartnerID[ 0 ] != '\0'))
   {
   	 /* check the parameter name and set the corresponding value */
	    if( AnscEqualString(ParamName, "ConnectivityTestURL", TRUE) )
	    {
			if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.NetworkDiagnosticTools.ConnectivityTestURL",PartnerID, pString))
			{
				memset( pBindObj->NDiagTool.ConnectivityTestURL, 0, sizeof( pBindObj->NDiagTool.ConnectivityTestURL ));
				AnscCopyString( pBindObj->NDiagTool.ConnectivityTestURL, pString );
				return TRUE;
			}	
	    }
	  
   }
    return FALSE;
}

/***********************************************************************
APIs for Object:
	DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.
	  *  MaintenanceWindow_GetParamStringValue
	  
***********************************************************************/

BOOL
WiFiPersonalization_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* check the parameter name and return the corresponding value */
	PCOSA_DATAMODEL_DEVICEINFO		pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
	PCOSA_DATAMODEL_RDKB_UIBRANDING	pBindObj =	& pMyObject->UiBrand;

        if( AnscEqualString(ParamName, "Support", TRUE))
        {
		 /* collect value */
		*pBool = pBindObj->WifiPersonal.Support;
		return TRUE;
        }    

	if( AnscEqualString(ParamName, "SMSsupport", TRUE))
        {
		/* collect value */
		*pBool = pBindObj->WifiPersonal.SMSsupport;
		return TRUE;

        }
	
        if( AnscEqualString(ParamName, "MyAccountAppSupport", TRUE))
	{
		/* collect value */
		*pBool = pBindObj->WifiPersonal.MyAccountAppSupport;
		return TRUE;
	} 

    return FALSE;
}

BOOL
WiFiPersonalization_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    PCOSA_DATAMODEL_RDKB_UIBRANDING	pBindObj =	& pMyObject->UiBrand;

   char PartnerID[PARTNER_ID_LEN] = {0};
   char *value = ( bValue ==TRUE ) ?  "true" : "false";


   if((CCSP_SUCCESS == getPartnerId(PartnerID) ) && ( PartnerID[ 0 ] != '\0'))
   {
	    if( AnscEqualString(ParamName, "Support", TRUE))
	    {

			if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.Support",PartnerID, value ))
			{
				pBindObj->WifiPersonal.Support = bValue;
				return TRUE;
			}	

	    }

	    if( AnscEqualString(ParamName, "SMSsupport", TRUE))
	    {

			if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.SMSsupport",PartnerID, value))
			{
				pBindObj->WifiPersonal.SMSsupport = bValue;
				return TRUE;
			}
			
	    }
		
	    if( AnscEqualString(ParamName, "MyAccountAppSupport", TRUE))
	    {

			if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.MyAccountAppSupport",PartnerID, value))
			{
				pBindObj->WifiPersonal.MyAccountAppSupport = bValue;
				return TRUE;
			}

	    }
   	}

    return FALSE;
}

ULONG
WiFiPersonalization_GetParamStringValue

	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		char*						pValue,
		ULONG*						pulSize
	)
{

	PCOSA_DATAMODEL_DEVICEINFO		pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
	PCOSA_DATAMODEL_RDKB_UIBRANDING	pBindObj =	& pMyObject->UiBrand;
	
	 if( AnscEqualString(ParamName, "PartnerHelpLink", TRUE))
        {
		if ( AnscSizeOfString(pBindObj->WifiPersonal.PartnerHelpLink) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->WifiPersonal.PartnerHelpLink);		
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->WifiPersonal.PartnerHelpLink)+1;
           		return 1;
       		}

        } 
	if( AnscEqualString(ParamName, "MSOLogo", TRUE))
        {
		if ( AnscSizeOfString(pBindObj->WifiPersonal.MSOLogo) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->WifiPersonal.MSOLogo);		
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->WifiPersonal.MSOLogo)+1;
           		return 1;
       		}

        } 
	if( AnscEqualString(ParamName, "Title", TRUE))
        {
		if ( AnscSizeOfString(pBindObj->WifiPersonal.Title) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->WifiPersonal.Title);		
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->WifiPersonal.Title)+1;
           		return 1;
       		}

        } 
	if( AnscEqualString(ParamName, "WelcomeMessage", TRUE))
        {
		if ( AnscSizeOfString(pBindObj->WifiPersonal.WelcomeMessage) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->WifiPersonal.WelcomeMessage);		
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->WifiPersonal.WelcomeMessage)+1;
           		return 1;
       		}

        } 
	 return -1;
}

BOOL
WiFiPersonalization_SetParamStringValue


    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    PCOSA_DATAMODEL_RDKB_UIBRANDING	pBindObj =	& pMyObject->UiBrand;

	char PartnerID[PARTNER_ID_LEN] = {0};
	
   if((CCSP_SUCCESS == getPartnerId(PartnerID) ) && ( PartnerID[ 0 ] != '\0') )
   {
   	 /* check the parameter name and set the corresponding value */
	    if( AnscEqualString(ParamName, "PartnerHelpLink", TRUE) )
	    {
			if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.PartnerHelpLink",PartnerID, pString))
			{
				memset( pBindObj->WifiPersonal.PartnerHelpLink, 0, sizeof( pBindObj->WifiPersonal.PartnerHelpLink ));
				AnscCopyString( pBindObj->WifiPersonal.PartnerHelpLink, pString );
				return TRUE;
			}			
	    }
	  
   }
    return FALSE;
}

/***********************************************************************
APIs for Object:
	DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.LocalUI.
	  *  MaintenanceWindow_GetParamStringValue
	  
***********************************************************************/

ULONG
LocalUI_GetParamStringValue

	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		char*						pValue,
		ULONG*						pulSize
	)
{

	PCOSA_DATAMODEL_DEVICEINFO		pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
	PCOSA_DATAMODEL_RDKB_UIBRANDING	pBindObj =	& pMyObject->UiBrand;
	
	 if( AnscEqualString(ParamName, "MSOLogo", TRUE))
        {
		if ( AnscSizeOfString(pBindObj->LocalUI.MSOLogo) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->LocalUI.MSOLogo);		
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->LocalUI.MSOLogo)+1;
           		return 1;
       		}

        } 
	if( AnscEqualString(ParamName, "DefaultLoginUsername", TRUE))
        {
		if ( AnscSizeOfString(pBindObj->LocalUI.DefaultLoginUsername) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->LocalUI.DefaultLoginUsername);		
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->LocalUI.DefaultLoginUsername)+1;
           		return 1;
       		}

        } 
	if( AnscEqualString(ParamName, "DefaultLoginPassword", TRUE))
        {
		if ( AnscSizeOfString(pBindObj->LocalUI.DefaultLoginPassword) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->LocalUI.DefaultLoginPassword);		
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->LocalUI.DefaultLoginPassword)+1;
           		return 1;
       		}

        } 
	if( AnscEqualString(ParamName, "MSOLogoTitle", TRUE))
        {
		if ( AnscSizeOfString(pBindObj->LocalUI.MSOLogoTitle) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->LocalUI.MSOLogoTitle);		
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->LocalUI.MSOLogoTitle)+1;
           		return 1;
       		}

        } 
	 return -1;
}

BOOL
LocalUI_SetParamStringValue


    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
	return FALSE;
}

/***********************************************************************
APIs for Object:
	DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.LocalUI.
	  *  MaintenanceWindow_GetParamBoolValue
	  
***********************************************************************/

BOOL
LocalUI_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* check the parameter name and return the corresponding value */
	PCOSA_DATAMODEL_DEVICEINFO		pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
	PCOSA_DATAMODEL_RDKB_UIBRANDING	pBindObj =	& pMyObject->UiBrand;

        if( AnscEqualString(ParamName, "HomeNetworkControl", TRUE))
        {
		 /* collect value */
		*pBool = pBindObj->LocalUI.HomeNetworkControl;
		return TRUE;
        }    
    return FALSE;
}

BOOL
LocalUI_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
	return FALSE;
}

/***********************************************************************
APIs for Object:
	DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.HelpTip.
	  *  MaintenanceWindow_GetParamBoolValue
	  
***********************************************************************/

ULONG
HelpTip_GetParamStringValue

	(
		ANSC_HANDLE 				hInsContext,
		char*						ParamName,
		char*						pValue,
		ULONG*						pulSize
	)
{

	PCOSA_DATAMODEL_DEVICEINFO		pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
	PCOSA_DATAMODEL_RDKB_UIBRANDING	pBindObj =	& pMyObject->UiBrand;
	
	 if( AnscEqualString(ParamName, "NetworkName", TRUE))
        {
		if ( AnscSizeOfString(pBindObj->HelpTip.NetworkName) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->HelpTip.NetworkName);		
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->HelpTip.NetworkName)+1;
           		return 1;
       		}

        } 

	 return -1;
}

BOOL
HelpTip_SetParamStringValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
	return FALSE;
}


#ifndef _COSA_FOR_BCI_
/***********************************************************************

 APIs for Object:

    DeviceInfo.X_RDKCENTRAL-COM_xOpsDeviceMgmt.RPC.

    *  RPC_GetParamUlongValue
    *  RPC_SetParamUlongValue
    *  RPC_GetParamStringValue
    *  RPC_SetParamStringValue

***********************************************************************/

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        RPC_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

    description:

        This function is called to retrieve ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
RPC_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
	PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
	
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "DeferFWDownloadReboot", TRUE))
    {
        /* collect value */
        *puLong = pMyObject->DeferFWDownloadReboot;
        return TRUE;
    }	
    if( AnscEqualString(ParamName, "RebootPendingNotification", TRUE))
    {
        /* collect value */
        *puLong = 0;
        return TRUE;
    }	    
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        RPC_SetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG                       uValue
            );

    description:

        This function is called to set ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue
                The updated ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
RPC_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
	PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
	
    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "DeferFWDownloadReboot", TRUE))
    {
        /* collect value */
		CosaDmlDiSet_DeferFWDownloadReboot(&(pMyObject->DeferFWDownloadReboot),uValue);
		return TRUE;
    } 
    if( AnscEqualString(ParamName, "RebootPendingNotification", TRUE))
    {
        /* collect value */
#ifdef PARODUS_ENABLE
        char buff[64] = {0};
        sprintf(buff,"%d",uValue);
	Send_Notification_Task(buff);
#endif
        return TRUE;
    }	       
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        RPC_GetParamStringValue
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
RPC_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pulSize
    )
{

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "RebootDevice", TRUE) )
    {
        /* collect value */
		AnscCopyString(pValue,"");
        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        RPC_SetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pString
            );

    description:

        This function is called to set string parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pString
                The updated string value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
RPC_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{

    /* check the parameter name and set the corresponding value */

    if( AnscEqualString(ParamName, "RebootDevice", TRUE))
    {
      
        if( TRUE == CosaDmlDi_ValidateRebootDeviceParam( pString ) )
		{
			CosaDmlDiSet_RebootDevice(pString);
        	return TRUE;
		}
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        RPC_GetParamBoolValue
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
RPC_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
        PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

    if( AnscEqualString(ParamName, "AbortReboot", TRUE))
    {
        /*CcspTraceWarning(("supported parameter '%s'\n", ParamName));*/
        *pBool = pMyObject->AbortReboot;
        return TRUE;
    }
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        RPC_SetParamBoolValue
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
RPC_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
   PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

   if( AnscEqualString(ParamName, "AbortReboot", TRUE))
   {
       FILE *file = NULL;
       FILE *Abortfile = NULL;
       if (file = fopen("/tmp/.deferringreboot", "r")){
           if (Abortfile = fopen("/tmp/AbortReboot", "r")){
               fclose(Abortfile);
               CcspTraceWarning(("Abort already done '%s'\n", ParamName));
               return TRUE;
           }
           pMyObject->AbortReboot = bValue;
           if(pMyObject->AbortReboot == TRUE)
              system("touch /tmp/AbortReboot");
           else
              CcspTraceWarning(("Parameter '%s' set to false\n", ParamName));
           fclose(file);
           return TRUE;
       } else {
           CcspTraceWarning(("Invalid request for parameter, no FW DL reboot wait in progress '%s'\n", ParamName));
           return FALSE;
       }
   } 
       /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
        return FALSE;
}
#endif

/**
 *  RFC Features IDS
*/

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        IDS_GetParamBoolValue
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
IDS_GetParamBoolValue

(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 BOOL*                       pBool
 )
{
    char buf[8];

    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* collect value */
        syscfg_get( NULL, "IDSENABLE", buf, sizeof(buf));

        if( buf != NULL )
        {
            if (strcmp(buf, "true") == 0)
                *pBool = TRUE;
            else
                *pBool = FALSE;
        }
        return TRUE;                }

    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        IDS_SetParamBoolValue
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
IDS_SetParamBoolValue

(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 BOOL                        bValue
 )
{
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {

        char buf[8];
        memset (buf, 0, sizeof(buf));

        snprintf(buf, sizeof(buf), "%s", bValue ? "true" : "false");

        if (syscfg_set(NULL, "IDSENABLE", buf) != 0)
        {
            CcspTraceError(("syscfg_set IDSENABLE failed\n"));
        }
        else
        {
            if (syscfg_commit() != 0)
            {
                CcspTraceError(("syscfg_commit IDSENABLE failed\n"));
            }
            else
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        IDS_GetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pValue
            );

    description:

        This function is called to retrieve string parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue
                The string value buffer;


    return:     TRUE if succeeded;
                FALSE if not supported.

**********************************************************************/
ULONG
    IDS_GetParamStringValue
(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 char*                       pValue,
 ULONG*                      pUlSize
 )
{

    /* Required for xPC sync */
    if( AnscEqualString(ParamName, "ScanTask", TRUE))
    {
        /* collect value */
        char buf[64];
        syscfg_get( NULL, "IDSSCANTASK", buf, sizeof(buf));

        if( buf != NULL )
        {
            AnscCopyString(pValue, buf);
            return 0;
        }
        return -1;
    }
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        IDS_SetParamBoolValue
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
    IDS_SetParamStringValue
(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 char*                       pString
 )
{

    /* Required for xPC sync */
    if( AnscEqualString(ParamName, "ScanTask", TRUE))
    {
        if (syscfg_set(NULL, "IDSSCANTASK", pString) != 0)
        {
            CcspTraceError(("syscfg_set failed\n"));

        }
        else
        {
            if (syscfg_commit() != 0)
            {
                CcspTraceError(("syscfg_commit failed\n"));

            }

            return TRUE;
        }
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}
