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
#include "cimplog.h"
#if defined (_XB6_PRODUCT_REQ_)
#include "bt_hal.h"
#endif

#if defined (_ARRIS_XB6_PRODUCT_REQ_) //ARRISXB6-7328, ARRISXB6-7332
#include "cm_hal.h"
#include "cm_hal_oem.h"
#endif

#if defined(_PLATFORM_RASPBERRYPI_)
#include <unistd.h>
#include <sys/types.h>
#endif

#include "ccsp_base_api.h"
#include "messagebus_interface_helper.h"

extern ULONG g_currentBsUpdate;
extern ANSC_HANDLE bus_handle;
extern char g_Subsystem[32];
extern void* g_pDslhDmlAgent;


#define IS_UPDATE_ALLOWED_IN_DM(paramName, requestorStr) ({                                                                                                  \
    if ( g_currentBsUpdate == DSLH_CWMP_BS_UPDATE_firmware ||                                                                                     \
         (g_currentBsUpdate == DSLH_CWMP_BS_UPDATE_rfcUpdate && !AnscEqualString(requestorStr, BS_SOURCE_RFC_STR, TRUE)))                         \
    {                                                                                                                                             \
       CcspTraceWarning(("Do NOT allow override of param: %s bsUpdate = %d, requestor = %s\n", paramName, g_currentBsUpdate, requestorStr));      \
       return FALSE;                                                                                                                              \
    }                                                                                                                                             \
})

// If the requestor is RFC but the param was previously set by webpa, do not override it.
#define IS_UPDATE_ALLOWED_IN_JSON(paramName, requestorStr, UpdateSource) ({                                                                                \
   if (AnscEqualString(requestorStr, BS_SOURCE_RFC_STR, TRUE) && AnscEqualString(UpdateSource, BS_SOURCE_WEBPA_STR, TRUE))                         \
   {                                                                                                                                               \
      CcspTraceWarning(("Do NOT allow override of param: %s requestor = %d updateSource = %s\n", paramName, g_currentWriteEntity, UpdateSource));  \
      return FALSE;                                                                                                                                \
   }                                                                                                                                               \
})

#if defined(_PLATFORM_RASPBERRYPI_)
int sock;
int id = 0;
#endif
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

    if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_OnBoarding_DeleteLogs", TRUE))
    {
        *pBool = FALSE ;
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
     #if !defined(_COSA_BCM_MIPS_) && !defined(_ENABLE_DSL_SUPPORT_)
        /* collect value */
	   FILE *fp;
	   char buff[30];
	   int retValue;
	   memset(buff,0,sizeof(buff));
           *puLong = 0;
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
	 #else
	   *puLong = 0;
	 #endif
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

	if( AnscEqualString(ParamName, "RouterName", TRUE))
    {
        char buf[16] = {'\0'};
        syscfg_get( NULL, "router_name", buf, sizeof(buf));
    	if( buf != NULL )
    		{
    		    AnscCopyString(pValue,  buf);
    		}
		else
		{
			AnscTraceWarning(("Error in syscfg_get for RouterName\n"));
		}
		return 0;
    }

	/* Changes for EMS end here */
	/*Changes for RDKB-6560*/
   	if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_CMTS_MAC", TRUE))
    		{
                    #if defined(_COSA_BCM_MIPS_) || defined(_ENABLE_DSL_SUPPORT_)
                    return -1;
                    #else
		    CosaDmlDiGetCMTSMac(NULL, pValue,pulSize);
        	    return 0;
                    #endif
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

    if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_OnBoarding_State", TRUE))
    {
        if(access("/nvram/.device_onboarded", F_OK) != -1)
        {
            AnscCopyString(pValue, "OnBoarded");
        }
        else
        {
            AnscCopyString(pValue, "NONE");
        }
        return 0;
    }

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
#if defined(_PLATFORM_RASPBERRYPI_)
    id =getuid();    
#endif
    
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
#if defined(_PLATFORM_RASPBERRYPI_)
               if(id!=0)
               {
                       char *lxcevt = "sysevent set firewall-restart";
                       send(sock , lxcevt , strlen(lxcevt) , 0 );
               }
#endif
	    }	
        }

        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_OnBoarding_DeleteLogs", TRUE))
    {
        if(bValue)
        {
            char cmd[128];
            memset(cmd, 0, sizeof(cmd));
            AnscCopyString(cmd, "sh /rdklogger/onboardLogUpload.sh delete &");
            system(cmd);
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

/**
 *  RFC Features TelemetryEndpoint
*/

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        TelemetryEndpoint_GetParamBoolValue
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
TelemetryEndpoint_GetParamBoolValue

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
        syscfg_get( NULL, "TelemetryEndpointEnabled", buf, sizeof(buf));

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
        TelemetryEndpoint_SetParamBoolValue
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
TelemetryEndpoint_SetParamBoolValue

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

        if (syscfg_set(NULL, "TelemetryEndpointEnabled", buf) != 0)
        {
            CcspTraceError(("syscfg_set TelemetryEndpointEnabled failed\n"));
        }
        else
        {
            if (syscfg_commit() != 0)
            {
                CcspTraceError(("syscfg_commit TelemetryEndpointEnabled failed\n"));
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
        TelemetryEndpoint_GetParamStringValue
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
    TelemetryEndpoint_GetParamStringValue
(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 char*                       pValue,
 ULONG*                      pUlSize
 )
{

    /* Required for xPC sync */
    if( AnscEqualString(ParamName, "URL", TRUE))
    {
        /* collect value */
        char buf[64];
        memset(buf, 0 ,sizeof(buf));
        syscfg_get( NULL, "TelemetryEndpointURL", buf, sizeof(buf));

        if( buf != NULL )
        {
            AnscCopyString(pValue, buf);
            *pUlSize = AnscSizeOfString( pValue );
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
        TelemetryEndpoint_SetParamBoolValue
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
    TelemetryEndpoint_SetParamStringValue
(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 char*                       pString
 )
{

    /* Required for xPC sync */
    if( AnscEqualString(ParamName, "URL", TRUE))
    {
        if (syscfg_set(NULL, "TelemetryEndpointURL", pString) != 0)
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

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AccountInfo_GetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pValue,
                ULONG*                      pulSize
            );

    description:

        This function is called to retrieve Boolean parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue
                The buffer of returned string value;

                ULONG*                      pulSize
                The buffer of returned string size;

    return:     TRUE if succeeded.

**********************************************************************/
#define ACCOUNT_ID_SIZE 32

ULONG
AccountInfo_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "AccountID", TRUE))
    {
        /* collect value */
           char buff[ACCOUNT_ID_SIZE]={'\0'};

           syscfg_get( NULL, "AccountID", buff, sizeof(buff));
           if( buff != NULL )
           {
                AnscCopyString(pValue,  buff);
                *pulSize = AnscSizeOfString( pValue );
                return 0;
           }
           return -1;
    }

    return -1;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        AccountInfo_SetParamStringValue
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
AccountInfo_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    BOOL bReturnValue = FALSE;
    
    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "AccountID", TRUE))
    {
        /* collect value */
           char buff[ACCOUNT_ID_SIZE]={'\0'};
           int idlen = strlen(pString)-1;
           int i;

           if ( idlen > ACCOUNT_ID_SIZE )
           {
                CcspTraceError(("%s Account ID size is too large %d characters..!! \n",__FUNCTION__, idlen));
                bReturnValue = FALSE;
           }
           else
           {
                snprintf(buff,sizeof(buff),"%s",pString);

                for(i=0; i<=idlen; ++i)
                {
                   if(isalnum(buff[i]) == 0 && buff[i] != '_' && buff[i] != '-')
                   {
                       CcspTraceError(("[%s] Account ID is not alphanumeric contains special characters...!!\n",__FUNCTION__));
                       bReturnValue = FALSE;
                       break;
                   }
                   else
                   {
                       bReturnValue = TRUE;
                   }
               }
               if(bReturnValue == TRUE)
               {
                    CcspTraceInfo(("[%s] Account ID is alphanumeric...!!", __FUNCTION__));
                    if (syscfg_set(NULL, "AccountID", pString) != 0)
                    {
                        CcspTraceError(("syscfg_set failed for AccountID \n", __FUNCTION__));
                        bReturnValue = FALSE;
                    }
                    else
                    {
                        if (syscfg_commit() != 0)
                        {
                             CcspTraceError(("syscfg_commit failed for AccountID \n", __FUNCTION__));
                             bReturnValue = FALSE;
                        }
                        bReturnValue = TRUE;
                    }         
                }
           }
    }
    else
    {
           CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
           bReturnValue = FALSE;
    }

    return bReturnValue;
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
                OnboardLog("RDKB_REBOOT : RebootDevice triggered from GUI\n");
		 
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
               OnboardLog("RDKB_REBOOT : Reboot Device triggered through Factory reset from GUI\n");
			 
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
		OnboardLog("Device reboot due to reason %s\n", pString);
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

    if( AnscEqualString(ParamName, "RouterName", TRUE))
    {
      if (syscfg_set(NULL, "router_name", pString) != 0)
      {
	AnscTraceWarning(("syscfg_set failed for RouterName\n"));
      } 
      else
      {
	if (syscfg_commit() != 0)
	{
	  AnscTraceWarning(("syscfg_commit failed for RouterName\n"));
	}
	CcspTraceWarning(("RouterName is changed, new RouterName: %s ...\n", pString));
      }
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

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        ManageableNotification_GetParamBoolValue
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
ManageableNotification_GetParamBoolValue

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
        syscfg_get( NULL, "ManageableNotificationEnabled", buf, sizeof(buf));

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
        ManageableNotification_SetParamBoolValue
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
ManageableNotification_SetParamBoolValue

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
            syscfg_set(NULL, "ManageableNotificationEnabled", "true");
        }
        else
        {
            syscfg_set(NULL, "ManageableNotificationEnabled", "false");
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
        TR069support_GetParamBoolValue
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
TR069support_GetParamBoolValue
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
        syscfg_get( NULL, "EnableTR69Binary", buf, sizeof(buf));

        if( buf != NULL )
        {
            if (strcmp(buf, "false") == 0)
                *pBool = FALSE;
            else
                *pBool = TRUE;
        }
        return TRUE;
    }

    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        TR069support_SetParamBoolValue
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
TR069support_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* collect value */
        if( bValue == FALSE)
        {
            AnscTraceWarning(("Disabling Tr069 from RFC \n"));
            system("/usr/ccsp/pam/launch_tr69.sh disable &");
        }
        else
        {
            AnscTraceWarning(("Enabling Tr069 from RFC \n"));
            system("/usr/ccsp/pam/launch_tr69.sh enable &");
        }
		
        return TRUE;
    }

    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        newNTP_GetParamBoolValue
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
newNTP_GetParamBoolValue
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
        syscfg_get( NULL, "new_ntp_enabled", buf, sizeof(buf));

        if( buf != NULL )
        {
            if (strcmp(buf, "false") == 0)
                *pBool = FALSE;
            else
                *pBool = TRUE;
        }
        return TRUE;
    }

    return FALSE;
}
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        newNTP_SetParamBoolValue
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
newNTP_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        if( ANSC_STATUS_SUCCESS != CosaDmlSetnewNTPEnable(bValue))
	    return FALSE;
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


/* HTTPS config download can be enabled/disabled for bci routers */
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        HTTPSConfigDownload_GetParamBoolValue
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
HTTPSConfigDownload_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* check the parameter name and return the corresponding value */

    if( AnscEqualString(ParamName, "Enabled", TRUE))
    {
        char *strValue = NULL;
        char str[2];
        int retPsmGet = CCSP_SUCCESS;

        retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, "dmsb.device.deviceinfo.X_RDKCENTRAL-COM_RFC.Feature.HTTPSConfigDownload.Enabled", NULL, &strValue);
        if (retPsmGet == CCSP_SUCCESS)
        {
            *pBool = _ansc_atoi(strValue);
            ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(strValue);
        }
        else
        {
            *pBool = FALSE;
        }

        return TRUE;
    }

    return FALSE;
}


/* HTTPS config download can be enabled/disabled for bci routers */
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        HTTPSConfigDownload_SetParamBoolValue
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
HTTPSConfigDownload_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    if( AnscEqualString(ParamName, "Enabled", TRUE))
    {
        char *strValue = NULL;
        char str[2];
        int retPsmGet = CCSP_SUCCESS;
        BOOL getVal = 0;

        retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, "dmsb.device.deviceinfo.X_RDKCENTRAL-COM_RFC.Feature.HTTPSConfigDownload.Enabled", NULL, &strValue);
        if (retPsmGet == CCSP_SUCCESS)
        {
            getVal = _ansc_atoi(strValue);
            ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(strValue);
        }

        if(getVal != bValue)
        {
            sprintf(str,"%d",bValue);
            retPsmGet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, "dmsb.device.deviceinfo.X_RDKCENTRAL-COM_RFC.Feature.HTTPSConfigDownload.Enabled", ccsp_string, str);
            if (retPsmGet != CCSP_SUCCESS)
            {
                CcspTraceError(("Set failed for HTTPSConfigDownloadEnabled \n"));
                return ANSC_STATUS_FAILURE;
            }
        }
        else
        {
            CcspTraceInfo(("HTTPSConfigDownloadEnabled is already %d \n",getVal));
        }

        return TRUE;
    }

    return FALSE;
}


/**********************************************************************

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


/* PresenceDetect can be enabled/disabled to detect connected clients presence */
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        PresenceDetect_GetParamBoolValue
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
PresenceDetect_GetParamBoolValue

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
        syscfg_get( NULL, "PresenceDetectEnabled", buf, sizeof(buf));

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

/* PresenceDetect can be enabled/disabled to detect connected clients presence */
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        PresenceDetect_SetParamBoolValue
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
PresenceDetect_SetParamBoolValue

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
            syscfg_set(NULL, "PresenceDetectEnabled", "true");
        }
        else
        {
            syscfg_set(NULL, "PresenceDetectEnabled", "false");
        }
        if (syscfg_commit() != 0)
        {
             AnscTraceWarning(("syscfg_commit failed for Presence feature param update\n"));
             return FALSE;
        }
        CosaDmlPresenceEnable(bValue);
        return TRUE;
    }

    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG
        OAUTH_GetParamStringValue
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
static unsigned char OAUTHAuthMode[8] = "potd";

static unsigned char OAUTHServerUrl[256] = "";

static unsigned char OAUTHTokenEndpoint[256] = "";

static unsigned char OAUTHClientId[16] = "";

ULONG
OAUTH_GetParamStringValue
    (
        ANSC_HANDLE        hInsContext,
        char*              ParamName,
        char*              pValue,
        ULONG*             pUlSize
    )
{
    LONG retval = 0;
    int outlen;
    char *outptr = NULL;

    hInsContext = hInsContext;    // prevent compiler unused variable warning
    if( AnscEqualString(ParamName, "AuthMode", TRUE) )
    {
        outptr = OAUTHAuthMode;
    }
    else if( AnscEqualString(ParamName, "ServerUrl", TRUE) == TRUE
           && AnscEqualString(OAUTHAuthMode, "potd", TRUE) != TRUE )
    {
        outptr = OAUTHServerUrl;
    }
    else if( AnscEqualString(ParamName, "TokenEndpoint", TRUE) == TRUE
           && AnscEqualString(OAUTHAuthMode, "potd", TRUE) != TRUE )
    {
        outptr = OAUTHTokenEndpoint;
    }
    else if( AnscEqualString(ParamName, "ClientId", TRUE) == TRUE
           && AnscEqualString(OAUTHAuthMode, "potd", TRUE) != TRUE )
    {
        outptr = OAUTHClientId;
    }
    else
    {
        retval = -1;
    }

    if( retval == 0 && outptr != NULL )
    {
        outlen = strlen( outptr );
        if( pValue && outlen < *pUlSize )
        {
            *pValue = 0;    // default to zero length output
            AnscCopyString( pValue, outptr );
        }
        else
        {
            *pUlSize = outlen + 1;
            retval = 1;
        }
    }
    return (ULONG)retval;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        OAUTH_SetParamStringValue
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
OAUTH_SetParamStringValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    int maxlen;
    char *inptr = NULL;
    BOOL bRet = FALSE;

    if( AnscEqualString(ParamName, "AuthMode", TRUE) )
    {
        maxlen = sizeof(OAUTHAuthMode);
        inptr = OAUTHAuthMode;
    }
    else if( AnscEqualString(ParamName, "ServerUrl", TRUE) )
    {
        maxlen = sizeof(OAUTHServerUrl);
        inptr = OAUTHServerUrl;
    }
    else if( AnscEqualString(ParamName, "TokenEndpoint", TRUE) )
    {
        maxlen = sizeof(OAUTHTokenEndpoint);
        inptr = OAUTHTokenEndpoint;
    }
    else if( AnscEqualString(ParamName, "ClientId", TRUE) )
    {
        maxlen = sizeof(OAUTHClientId);
        inptr = OAUTHClientId;
    }
    else
    {
        CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    }

    if( inptr != NULL && pString != NULL && AnscSizeOfString( pString ) < maxlen )
    {
        AnscCopyString( inptr, pString );
        bRet = TRUE;
    }

    return bRet;
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
#if defined(_PLATFORM_RASPBERRYPI_)
       id=getuid();
#endif
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
#if defined(_PLATFORM_RASPBERRYPI_)
               if(id!=0)
               {
                 char *lxcevt = "sysevent set iot_status up";
                 send(sock , lxcevt , strlen(lxcevt) , 0 );
               }
#endif
             }
                else{
                   AnscTraceWarning(("IOT_LOG : Raise IOT event down from DML\n"));
                   system("sysevent set iot_status down");
#if defined(_PLATFORM_RASPBERRYPI_)
                if(id!=0)
                 {
                   char *lxcevt = "sysevent set iot_status down";
                   send(sock , lxcevt , strlen(lxcevt) , 0 );
                 }
#endif
                }
                return TRUE;
            }
        }
    }

    return FALSE;
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
        Control_GetParamUlongValue
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

        ULONG
        Control_GetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pValue,
                ULONG*                      pulSize
            );

    description:

        This function is called to retrieve Boolean parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue
                The buffer of returned string value;

                ULONG*                      pulSize
                The buffer of returned string size;

    return:     TRUE if succeeded.

**********************************************************************/
#define XCONF_SELECTOR_SIZE 10
#define XCONF_URL_SIZE 60

ULONG
Control_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    /* check the "XconfSelector" parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "XconfSelector", TRUE))
    {
        /* collect value */
           char buff[XCONF_SELECTOR_SIZE]={'\0'};

           syscfg_get( NULL, "XconfSelector", buff, sizeof(buff));
           if( buff != NULL )
           {
                AnscCopyString(pValue,  buff);
                *pulSize = AnscSizeOfString( pValue );
                return 0;
           }
           return -1;
    }

    /* check the "XconfUrl" parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "XconfUrl", TRUE))
    {
        /* collect value */
           char buff[XCONF_URL_SIZE]={'\0'};

           syscfg_get( NULL, "XconfUrl", buff, sizeof(buff));
           if( buff != NULL )
           {
                AnscCopyString(pValue,  buff);
                *pulSize = AnscSizeOfString( pValue );
                return 0;
           }
           return -1;
    }

    return -1;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        Control_SetParamStringValue
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
Control_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    BOOL bReturnValue = FALSE;
 
    /* check the "XconfSelector" parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "XconfSelector", TRUE))
    {
        /* collect value */
           char buff[XCONF_SELECTOR_SIZE]={'\0'};
           int idlen = strlen(pString)-1;
           int i;

           if ( idlen > XCONF_SELECTOR_SIZE )
           {
                CcspTraceError(("[%s] Invalid XconfSelector length ..!! \n",__FUNCTION__));
                bReturnValue = FALSE;
           }
           else
           {
                if (syscfg_set(NULL, "XconfSelector", pString) != 0)
                {
                    CcspTraceError(("[%s] syscfg_set failed for XconfSelector \n",__FUNCTION__));
                    bReturnValue = FALSE;
                }
                else
                {
                    if (syscfg_commit() != 0)
                    {
                         CcspTraceError(("[%s] syscfg_commit failed for XconfSelector \n",__FUNCTION__));
                         bReturnValue = FALSE;
                    }
                    bReturnValue = TRUE;
                    CcspTraceInfo(("[%s] XconfSelector value set as %s success..!!\n",__FUNCTION__,pString));
                } 
           }
    }
    else
    {
           CcspTraceWarning(("[%s] Unsupported parameter '%s'\n",__FUNCTION__,ParamName));
           bReturnValue = FALSE;
    }


    /* check the "XconfUrl" parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "XconfUrl", TRUE))
    {
        /* collect value */
           char buff[XCONF_URL_SIZE]={'\0'};
           int idlen = strlen(pString)-1;
           int i;

           if ( idlen > XCONF_URL_SIZE )
           {
                CcspTraceError(("[%s] Invalid XconfUrl length ..!! \n",__FUNCTION__));
                bReturnValue = FALSE;
           }
           else
           {
               if (syscfg_set(NULL, "XconfUrl", pString) != 0)
                {
                    CcspTraceError(("[%s] syscfg_set failed for XconfUrl \n",__FUNCTION__));
                    bReturnValue = FALSE;
                }
                else
                {
                    if (syscfg_commit() != 0)
                    {
                         CcspTraceError(("[%s] syscfg_commit failed for XconfUrl \n",__FUNCTION__));
                         bReturnValue = FALSE;
                    }
                    bReturnValue = TRUE;
                    CcspTraceInfo(("[%s] XconfUrl value set as %s success..!!\n",__FUNCTION__,pString));
                }
           }
    }
    else
    {
           CcspTraceWarning(("[%s] Unsupported parameter '%s'\n",__FUNCTION__,ParamName));
           bReturnValue = FALSE;
    }

    return bReturnValue;
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

    if( AnscEqualString(ParamName, "HomeNetworkIsolation", TRUE))
    {
#if defined(MOCA_HOME_ISOLATION)
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
#else        
        *pBool = FALSE;
#endif
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

    if( AnscEqualString(ParamName, "ContainerSupport", TRUE))
    {
       /* Collect Value */
       char *strValue = NULL;
       char str[2];
       int retPsmGet = CCSP_SUCCESS;


        retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.Container", NULL, &strValue);
        if (retPsmGet == CCSP_SUCCESS) 
        {
            *pBool = _ansc_atoi(strValue);
            ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(strValue);
        }
        else
            *pBool = FALSE;

         return TRUE;
    }

#if defined(_COSA_FOR_BCI_)
    if( AnscEqualString(ParamName, "OneToOneNAT", TRUE))
    {
         char value[8];
         syscfg_get(NULL,"one_to_one_nat",value, sizeof(value));
         if( value != NULL )
         {
             if (strcmp(value, "true") == 0)
                 *pBool = TRUE;
             else
                 *pBool = FALSE;
         }
         return TRUE;
    }

    if (AnscEqualString(ParamName, "EnableMultiProfileXDNS", TRUE))
    {
        char buf[5] = {0};
        syscfg_get( NULL, "MultiProfileXDNS", buf, sizeof(buf));
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
#endif

   if(AnscEqualString(ParamName, "BLERadio", TRUE))
    {
#if defined(_XB6_PRODUCT_REQ_)   
        BLE_Status_e status;
        if(!ble_GetStatus(&status))
        {
           if(status == BLE_ENABLE)
           {
             *pBool = TRUE;
           }else
            *pBool = FALSE;

            return TRUE;
        }
#else
            *pBool = FALSE;
            return TRUE;
#endif
     }

    if( AnscEqualString(ParamName, "Xupnp", TRUE))
    {
	 char value[8];
         syscfg_get(NULL, "start_upnp_service", value, sizeof(value));
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
        *pBool = SyndicatonFlowControl->Enable.ActiveValue;
        return TRUE;
    }
    return FALSE;
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
       if ( AnscSizeOfString(SyndicatonFlowControl->InitialForwardedMark.ActiveValue) < *pUlSize)
        {
            AnscCopyString( pValue, SyndicatonFlowControl->InitialForwardedMark.ActiveValue);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(SyndicatonFlowControl->InitialForwardedMark.ActiveValue)+1;
            return 1;
        }
    }
    if( AnscEqualString(ParamName, "InitialOutputMark", TRUE))
    {
        if ( AnscSizeOfString(SyndicatonFlowControl->InitialOutputMark.ActiveValue) < *pUlSize)
        {
            AnscCopyString( pValue, SyndicatonFlowControl->InitialOutputMark.ActiveValue);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(SyndicatonFlowControl->InitialOutputMark.ActiveValue)+1;
            return 1;
        }
    }
    return -1;
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
        RDKFirmwareUpgrader_GetParamBoolValue
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
RDKFirmwareUpgrader_GetParamBoolValue
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


        retPsmGet = PSM_Get_Record_Value2(bus_handle, g_Subsystem, "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.RDKFirmwareUpgrader.Enable", NULL, &strValue);
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
                    system("sysevent set multinet-restart 1");
                    system("sh /usr/ccsp/moca/MoCA_isolation.sh &");
                    
                }
                else
                {

                    CcspTraceInfo(("reverting changes for HomeNetworkIsolation \n"));
                    system("sysevent set multinet-down 9");
                    system("rm /tmp/MoCABridge_up");
                    system("sysevent set multinet-restart 1");
                    system("killall MRD; killall smcroute;igmpproxy -c /tmp/igmpproxy.conf");
                }
	}
	else
		CcspTraceInfo(("HomeNetworkIsolation is already %d \n",getVal));
	
    	return TRUE;
    }
#endif

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
       if (retPsmGet != CCSP_SUCCESS) 
       {
          CcspTraceError(("Set failed for ContainerSupport \n"));
          return ANSC_STATUS_FAILURE;
       }
       CcspTraceInfo(("Successfully set ContainerSupport \n"));
       return TRUE;
    }

#if defined(_COSA_FOR_BCI_)
    if( AnscEqualString(ParamName, "OneToOneNAT", TRUE))
    {
        BOOL bNatEnable = FALSE;
        bNatEnable  = g_GetParamValueBool(g_pDslhDmlAgent, "Device.NAT.X_Comcast_com_EnableNATMapping");
        if ( bValue != bNatEnable )
        {
            g_SetParamValueBool("Device.NAT.X_Comcast_com_EnableNATMapping", bValue);
        }

        if ( bValue == TRUE)
        {
            syscfg_set(NULL, "one_to_one_nat", "true");
        }
        else
        {
            syscfg_set(NULL, "one_to_one_nat", "false");
        }
        syscfg_commit();
        return TRUE;
    }

    if( AnscEqualString(ParamName, "EnableMultiProfileXDNS", TRUE))
    {
        char buf[5] = {0};
        syscfg_get( NULL, "X_RDKCENTRAL-COM_XDNS", buf, sizeof(buf));
        if( buf != NULL && !strcmp(buf,"1") )
        {
                if(!setMultiProfileXdnsConfig(bValue))
                        return FALSE;

                char bval[2] = {0};
                if( bValue == TRUE)
                {
                        bval[0] = '1';
                }
                else
                {
                        bval[0] = '0';
                }

                if (syscfg_set(NULL, "MultiProfileXDNS", bval) != 0)
                {
                        AnscTraceWarning(("[XDNS] syscfg_set MultiProfileXDNS failed!\n"));
                }
                else
                {
                        if (syscfg_commit() != 0)
                        {
                                AnscTraceWarning(("[XDNS] syscfg_commit MultiProfileXDNS failed!\n"));
                        }
                }

        }
        else
        {
                CcspTraceError(("XDNS Feature is not Enabled. so,EnableMultiProfileXDNS set operation to %d failed \n",bValue));
                return FALSE;
        }

        return TRUE;

    }
#endif

    if( AnscEqualString(ParamName, "BLERadio", TRUE))
    {
#if defined (_XB6_PRODUCT_REQ_)
       BLE_Status_e status;
       if(bValue == TRUE)
       {
          status = BLE_ENABLE;
          CcspTraceInfo(("***BLE_ENABLED***\n"));
       }
       else
       {
          status = BLE_DISABLE;
          CcspTraceInfo(("***BLE_DISABLED***\n"));
       }
       if( !ble_Enable(status))
       {
          if (syscfg_set(NULL, "BLEEnabledOnBoot", status==BLE_ENABLE?"true":"false") != 0) 
          {
             AnscTraceWarning(("syscfg_set BLEEnabledOnBoot failed\n"));
          }
          else
          {
             if (syscfg_commit() != 0)
             {
                AnscTraceWarning(("syscfg_commit BLEEnabledOnBoot failed\n"));
             }
          }
          return TRUE;
       }
#else
       return FALSE;
#endif
    }
    if( AnscEqualString(ParamName, "Xupnp", TRUE))
    {
       if ( bValue == TRUE)
       {
           if (syscfg_set(NULL, "start_upnp_service", "true") != 0)
           {
               AnscTraceWarning(("syscfg_set start_upnp_service:true failed\n"));
           }
       }
       else
       {
           if (syscfg_set(NULL, "start_upnp_service", "false") != 0)
           {
               AnscTraceWarning(("syscfg_set start_upnp_service:false failed\n"));
           }
       }
       if (syscfg_commit() != 0)
       {
           AnscTraceWarning(("syscfg_commit start_upnp_service failed\n"));
       }
       return TRUE;
    }

    return FALSE;
}

#define BS_SOURCE_WEBPA_STR "webpa"
#define BS_SOURCE_RFC_STR "rfc"

char * getRequestorString()
{
   switch(g_currentWriteEntity)
   {
      case 0x0A: //CCSP_COMPONENT_ID_WebPA from webpa_internal.h(parodus2ccsp)
      case 0x0B: //CCSP_COMPONENT_ID_XPC
         return BS_SOURCE_WEBPA_STR;

      case 0x08: //DSLH_MPA_ACCESS_CONTROL_CLI
      case 0x10: //DSLH_MPA_ACCESS_CONTROL_CLIENTTOOL
         return BS_SOURCE_RFC_STR;

      default:
         return "unknown";
   }
}

char * getTime()
{
    time_t timer;
    static char buffer[50];
    struct tm* tm_info;
    time(&timer);
    tm_info = localtime(&timer);
    strftime(buffer, 50, "%Y-%m-%d %H:%M:%S ", tm_info);
    return buffer;
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

    char * requestorStr = getRequestorString();
    char * currentTime = getTime();

    IS_UPDATE_ALLOWED_IN_DM(ParamName, requestorStr);

    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, SyndicatonFlowControl->Enable.UpdateSource);

        if(CosaDmlDiSet_SyndicationFlowControl_Enable(bValue) == 0)
        {
            SyndicatonFlowControl->Enable.ActiveValue =bValue;

            memset( SyndicatonFlowControl->Enable.UpdateSource, 0, sizeof( SyndicatonFlowControl->Enable.UpdateSource ));
            AnscCopyString( SyndicatonFlowControl->Enable.UpdateSource, requestorStr );

            char *value = ( bValue ==TRUE ) ?  "true" : "false";
            char PartnerID[PARTNER_ID_LEN] = {0};
            if((CCSP_SUCCESS == getPartnerId(PartnerID) ) && (PartnerID[ 0 ] != '\0') )
                UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.SyndicationFlowControl.Enable",PartnerID, value, requestorStr, currentTime);

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

    char * requestorStr = getRequestorString();
    char * currentTime = getTime();

    IS_UPDATE_ALLOWED_IN_DM(ParamName, requestorStr);

    char PartnerID[PARTNER_ID_LEN] = {0};
    getPartnerId(PartnerID);

    if( AnscEqualString(ParamName, "InitialForwardedMark", TRUE))
    {
        IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, SyndicatonFlowControl->InitialForwardedMark.UpdateSource);

        if(CosaDmlDiSet_SyndicationFlowControl_InitialForwardedMark(SyndicatonFlowControl->InitialForwardedMark)==0)
        {
            AnscCopyString(SyndicatonFlowControl->InitialForwardedMark.ActiveValue, pString);

            memset( SyndicatonFlowControl->InitialForwardedMark.UpdateSource, 0, sizeof( SyndicatonFlowControl->InitialForwardedMark.UpdateSource ));
            AnscCopyString( SyndicatonFlowControl->InitialForwardedMark.UpdateSource, requestorStr );

            if (PartnerID[ 0 ] != '\0')
                UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.SyndicationFlowControl.InitialForwardedMark",PartnerID, pString, requestorStr, currentTime);

            return TRUE;
        }
    }
    if( AnscEqualString(ParamName, "InitialOutputMark", TRUE))
    {
        IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, SyndicatonFlowControl->InitialOutputMark.UpdateSource);

        if(CosaDmlDiSet_SyndicationFlowControl_InitialOutputMark(SyndicatonFlowControl->InitialOutputMark)==0)
        {
            AnscCopyString(SyndicatonFlowControl->InitialOutputMark.ActiveValue, pString);

            memset( SyndicatonFlowControl->InitialOutputMark.UpdateSource, 0, sizeof( SyndicatonFlowControl->InitialOutputMark.UpdateSource ));
            AnscCopyString( SyndicatonFlowControl->InitialOutputMark.UpdateSource, requestorStr );

            if (PartnerID[ 0 ] != '\0')
                UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.SyndicationFlowControl.InitialOutputMark",PartnerID, pString, requestorStr, currentTime);

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
           CcspTraceError(("Set failed for MEMSWAP support \n"));
           return ANSC_STATUS_FAILURE;
       }
       CcspTraceInfo(("Successfully set MEMSWAP support \n"));
       return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        RDKFirmwareUpgrader_SetParamBoolValue
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
RDKFirmwareUpgrader_SetParamBoolValue
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
       retPsmGet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.RDKFirmwareUpgrader.Enable", ccsp_string, str);
       if (retPsmGet != CCSP_SUCCESS) {
           CcspTraceError(("Set failed for RDKFirmwareUpgrader support \n"));
           return ANSC_STATUS_FAILURE;
       }
       CcspTraceInfo(("Successfully set RDKFirmwareUpgrader support \n"));
       return TRUE;
    }
    return FALSE;
}

/* CodeBigFirst config download can be enabled/disabled               */
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
                BOOL*                       bValue
            );

    description:

        This function is called to set Boolean parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL*                       bValue
                The buffer with updated value;

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
#if defined(_PLATFORM_RASPBERRYPI_)
      if(id!=0)
       {
		   char *lxcevt = "sysevent set firewall-restart";
                   send(sock ,  lxcevt, strlen(lxcevt) , 0 );
       }
#endif
            return TRUE;
        }
    return FALSE;
}
/**********************************************************************  


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SNMP_GetParamBoolValue
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
SNMP_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    if( AnscEqualString(ParamName, "V3Support", TRUE))
    {
        char value[8];
        syscfg_get(NULL,"V3Support",value, sizeof(value));
        if( value != NULL )
        {
             if (strcmp(value, "true") == 0)
                 *pBool = TRUE;
             else
                 *pBool = FALSE;
        }
        return TRUE;
    }
    if( AnscEqualString(ParamName, "V2Support", TRUE))
    {
        char value[8];
        syscfg_get(NULL,"V2Support",value, sizeof(value));
        if( value != NULL )
        {
             if (strcmp(value, "true") == 0)
                 *pBool = TRUE;
             else
                 *pBool = FALSE;
        }
        return TRUE;
    }
    if( AnscEqualString(ParamName, "RestartMaintenanceEnable", TRUE))
    {
        char value[8] = { 0 };
        syscfg_get(NULL,"SNMP_RestartMaintenanceEnable",value, sizeof(value));
        if( value[ 0] != '\0' )
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
        SNMP_SetParamBoolValue
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
SNMP_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    if( AnscEqualString(ParamName, "V3Support", TRUE))
    {
        if ( bValue == TRUE)
        {
            syscfg_set(NULL, "V3Support", "true");
        }
        else
        {
            syscfg_set(NULL, "V3Support", "false");
        }
        syscfg_commit();
        return TRUE;
    }
    if( AnscEqualString(ParamName, "V2Support", TRUE))
    {
        if ( bValue == TRUE)
        {
            syscfg_set(NULL, "V2Support", "true");
        }
        else
        {
            syscfg_set(NULL, "V2Support", "false");
        }
        syscfg_commit();
        return TRUE;
    }
    if( AnscEqualString(ParamName, "RestartMaintenanceEnable", TRUE))
    {
        if ( bValue == TRUE)
        {
            syscfg_set(NULL, "SNMP_RestartMaintenanceEnable", "true");
        }
        else
        {
            syscfg_set(NULL, "SNMP_RestartMaintenanceEnable", "false");
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
        SysCfg_GetParamBoolValue
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
SysCfg_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    if( AnscEqualString(ParamName, "UpdateNvram", TRUE))
    {
        char value[8];
        syscfg_get(NULL,"UpdateNvram",value, sizeof(value));
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
        SysCfg_SetParamBoolValue
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
SysCfg_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    if( AnscEqualString(ParamName, "UpdateNvram", TRUE))
    {
        if ( bValue == TRUE)
        {
            syscfg_set(NULL, "UpdateNvram", "true");
        }
        else
        {
            syscfg_set(NULL, "UpdateNvram", "false");
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
        IPv6onXHS_GetParamBoolValue
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
IPv6onXHS_GetParamBoolValue

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
	        char *Inf_name = NULL;
    	    int retPsmGet = CCSP_SUCCESS;

            retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, "dmsb.l2net.2.Port.1.Name", NULL, &Inf_name);
            if (retPsmGet == CCSP_SUCCESS) {
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
		    ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(Inf_name);
		    return TRUE;
	        }
            }
            else
            *pBool = FALSE;
        }

    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        IPv6onXHS_SetParamBoolValue
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
IPv6onXHS_SetParamBoolValue

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
	    char *Inf_name = NULL;
	    BOOL bFound = FALSE;
    	    int retPsmGet = CCSP_SUCCESS;

            retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, "dmsb.l2net.2.Port.1.Name", NULL, &Inf_name);
            if (retPsmGet == CCSP_SUCCESS)
				{		
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
					    ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(Inf_name);
						return TRUE;
					}
				   ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(Inf_name);
            }
            return TRUE;
        }
    return FALSE;
}
/**********************************************************************  

    caller:     owner of this object

    prototype: 

        BOOL
        EvoStream_DirectConnect_GetParamBoolValue
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
EvoStream_DirectConnect_GetParamBoolValue

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
            syscfg_get( NULL, "EvoStreamDirectConnect", buf, sizeof(buf));

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
        EvoStream_DirectConnect_SetParamBoolValue
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
EvoStream_DirectConnect_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
        if( AnscEqualString(ParamName, "Enable", TRUE))
        {
            /* collect value */
            if(syscfg_set(NULL, "EvoStreamDirectConnect", (bValue == TRUE ? "true": "false")) != 0)
		{
			CcspTraceError(("EvoStreamDirectConnect :%d Failed to SET\n", bValue ));
		}
		else
		{
            	if(syscfg_commit() != 0)
			{
				CcspTraceError(("EvoStreamDirectConnect :%d Failed to Commit\n", bValue ));
			}
			else
			{
	    			CcspTraceInfo(("EvoStreamDirectConnect :%d Success\n", bValue ));
	    			system("sysevent set firewall-restart");
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
    int status = 0;
    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "xconfCheckNow", TRUE))
    {
        AnscTraceWarning(("Triggering firmware download check from TR181\n"));
        if( TRUE == bValue )
        {
            // static collection as we don't want upgrade path to be changed without a reboot
            static BOOL rdkfwupgraderEnabledCollected = false;
            static BOOL RDKFWUpgraderEnabled = false;

            if (!rdkfwupgraderEnabledCollected) {
                if (RDKFirmwareUpgrader_GetParamBoolValue(hInsContext,
                                                          "Enable",
                                                          &RDKFWUpgraderEnabled) ) {
                    rdkfwupgraderEnabledCollected = true;
                }
            }

	    // NOTE:: this might have an addional issue, rfc enabled but rdkfwupgrader was disabled at 
            // build time. Do we need to take care of that like checking for presense /usr/bin/rdkfwupgrader
	    // at runtime 
            if (RDKFWUpgraderEnabled) {
                // FIXME:: call RDKFWUpgrader's dbus api. As of now we are calling using
                // dbus-send but we can bring in codegen generated proxy and call it using that.
                // and get rid of these system calls. But this is supposed to stay for short term only
                // not wasting time on this.
                AnscTraceWarning(("Triggering firmware download check using RDKFirmwareUpgrader TR181\n"));
                system("/lib/rdk/rdkfwupgrader_check_now.sh &");
            } else {
                // NOTE:: Firmwaresched.sh used to check for reboot pending before killing, this one doesn't
                // leaving a note behind if it comes out to be a problem
#if defined(INTEL_PUMA7) || defined(_COSA_BCM_ARM_)
#ifdef _CBR_PRODUCT_REQ_
            if(0 == system("pidof cbr_firmwareDwnld.sh"))  {
                           system ("kill -9 `pidof cbr_firmwareDwnld.sh `");
                       }
                           status = system("/etc/cbr_firmwareDwnld.sh &");
#elif defined(_HUB4_PRODUCT_REQ_)
                        if(0 == system("pidof Hub4_firmwareDwnld.sh"))  {
                           system ("kill -9 `pidof Hub4_firmwareDwnld.sh `");
                       }
                           status = system("/etc/Hub4_firmwareDwnld.sh &");
#else
                if(0 == system("pidof xb6_firmwareDwnld.sh"))  {
                    system ("kill -9 `pidof xb6_firmwareDwnld.sh `");
                }
                status = system("/etc/xb6_firmwareDwnld.sh &");
#endif
#elif defined(_COSA_BCM_MIPS_)
                if(0 == system("pidof xf3_firmwareDwnld.sh"))  {
                    system ("kill -9 `pidof xf3_firmwareDwnld.sh `");
                }
                status = system("/etc/xf3_firmwareDwnld.sh &");
#else
                if(0 == system("pidof xb3_firmwareDwnld.sh"))  {
                    system ("kill -9 `pidof xb3_firmwareDwnld.sh `");
                }
                status = system("/etc/xb3_firmwareDwnld.sh &");
#endif

                if (0 == status)
                {
                    AnscTraceWarning(("xconf process started successfully\n"));
                }
                else
                {
                    AnscTraceWarning(("xconf process did not start successfully\n"));
                }
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

    return:     0 if succeeded;
                1 if short of buffer size; (*pUlSize = required size)
                -1 if not supported.

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

/***********************************************************************

 APIs for Object:

    Device.DeviceInfo.X_RDKCENTRAL-COM_EthernetWAN.

    *  EthernetWAN_GetParamStringValue

***********************************************************************/

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        EthernetWAN_GetParamStringValue
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
EthernetWAN_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "CurrentOperationalMode", TRUE))
    {
        /* collect value */
        char buf[32] = { 0 };
		
        if( ( 0 == syscfg_get( NULL, "eth_wan_enabled", buf, sizeof( buf ) ) ) && \
			( '\0' != buf[ 0 ] )
		   )
        {
			if( 0 == strcmp( buf, "true" ) )
			{
				AnscCopyString( pValue, "Ethernet");
			}
			else
			{
				AnscCopyString( pValue, "DOCSIS");
			}
        }
        else
        {
            CcspTraceError(("%s syscfg_get failed for eth_wan_enabled. so giving default as DOCSIS\n",__FUNCTION__));
			AnscCopyString( pValue, "DOCSIS");
        }
		
        return 0;
    }

    return -1;
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


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Logging_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
	        ULONG*                      puLong
            );

    description:

        This function is called to retrieve ulong parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

     		   ULONG*                      puLong
                The buffer of returned ulong value;

    return:     TRUE if succeeded.

**********************************************************************/


BOOL
Logging_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "DmesgLogSyncInterval", TRUE))
    {
        /* collect value */
	 char buf[8]={0};
	 syscfg_get(NULL, "dmesglogsync_interval", buf, sizeof(buf));
         *puLong = atoi(buf);
        return TRUE;
    }

	
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

BOOL
Logging_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    ) ;
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
Logging_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
	PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
	
    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "DmesgLogSyncInterval", TRUE))
    {
        /* collect value */
		char buf[8]={0};
		snprintf(buf,sizeof(buf),"%lu",uValue);
			if (syscfg_set(NULL, "dmesglogsync_interval", buf) != 0) 
			{
				CcspTraceWarning(("syscfg_set failed to set DmesgLogSyncInterval \n"));
			       return FALSE;
			}
			else 
			{
				if (syscfg_commit() != 0) 
				{
					CcspTraceWarning(("syscfg_commit failed to set DmesgLogSyncInterval \n"));
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
        SwitchToDibbler_GetParamBoolValue
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
SwitchToDibbler_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
	/* This Get API is only for XB3,AXB6 devices */
#if defined(_COSA_INTEL_XB3_ARM_) || defined(INTEL_PUMA7)
    PCOSA_DATAMODEL_DEVICEINFO              pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    BOOL                            bReturnValue;

    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* collect value */
        char buf[8];
        if( syscfg_get( NULL, "dibbler_client_enable", buf, sizeof(buf))==0)
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
            CcspTraceWarning(("%s syscfg_get failed  for dibbler_client_enable\n",__FUNCTION__));
            *pBool = FALSE;
        }

        return TRUE;
    }
#endif
    return FALSE;
}

void dhcpSwitchThread(void* vptr_value)
{
        char command[64];
        char *str = (char *) vptr_value;
        pthread_detach(pthread_self());
        snprintf(command,sizeof(command),"sh /etc/dhcpswitch.sh %s &",str);
        system(command);
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SwitchToDibbler_SetParamBoolValue
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
SwitchToDibbler_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
     /* This set API is only for XB3,AXB6 devices */
#if defined(_COSA_INTEL_XB3_ARM_) || defined(INTEL_PUMA7)
    PCOSA_DATAMODEL_DEVICEINFO              pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        char buf[8];
        char event[8];
        pthread_t tid;
        char* operation = NULL;
        int val;

        /* collect previous flag value */
       if( syscfg_get( NULL, "dibbler_client_enable", buf, sizeof(buf)) == 0)
        {
                val = strcmp(buf,"true")?0:1;
                if (val != bValue)
                {
                        if (bValue)
                        {
                                commonSyseventGet("dhcpclient_v4", event, sizeof(event));
                                if (atoi(event) == 1)
                                {
                                        commonSyseventSet("dhcpclient_v6", "1");
                                        AnscTraceWarning(("dhcpclient_v4 event is enabled. So enabling dhcpclient_v6 event \n"));
                                }
                                else
                                {
                                        operation = "schedule_v6_cron";
                                        AnscTraceWarning(("dhcpclient_v4 event is not enabled.scheduling cron \n"));
                                        pthread_create(&tid, NULL, &dhcpSwitchThread, (void *)operation);
                                }
                        }
                        else
                        {
                                operation = "clear_v6_cron";
                                AnscTraceWarning(("dhcp client switching back to default \n"));
                                pthread_create(&tid, NULL, &dhcpSwitchThread, (void *)operation);
                        }
                }
                else if(!bValue)
                {
                        char v6event[8];
                        commonSyseventGet("dhcpclient_v6", v6event, sizeof(v6event));
                        if (atoi(v6event) == 1)
                        {
                                char v4event[8];
                                commonSyseventGet("dhcpclient_v4", v4event, sizeof(v4event));
                                if (atoi(v4event) ==1)
                                {
                                        commonSyseventSet("dhcpclient_v6", "0");
                                        AnscTraceWarning(("dhcpclient_v6 is disabled\n"));
                                }
                                else
                                {
                                        commonSyseventSet("dhcpclient_v6", "0");
                                        system("sed -i '/dhcpswitch.sh/d' /var/spool/cron/crontabs/root &");
                                        AnscTraceWarning(("dhcpclient_v6 is disabled and scheduled cron removed\n"));
                                }

                        }
                        else
                        {
                                AnscTraceWarning(("No set operation done since dibbler_client_enable flag already set to %d\n", bValue));
                        }
                }
                else
                {
                        AnscTraceWarning(("No set operation done since dibbler_client_enable flag already set to %d\n", bValue));
                }
        }
        else
        {
                AnscTraceWarning(("syscfg_get failed for diibler_client_enable\n"));
                return FALSE;
        }


        return TRUE;
    }
#endif

   return FALSE;

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
        CosaDmlDiGetSyndicationTR69CertLocation( hInsContext, pMyObject->TR69CertLocation.ActiveValue );
	AnscCopyString( pValue, pMyObject->TR69CertLocation.ActiveValue );
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
	 if ( AnscSizeOfString(pMyObject->UiBrand.PauseScreenFileLocation.ActiveValue) < *pulSize)
	 {
		 AnscCopyString( pValue, pMyObject->UiBrand.PauseScreenFileLocation.ActiveValue);		
		 return 0;
	 }
	 else
	 {
	 	 *pulSize = AnscSizeOfString(pMyObject->UiBrand.PauseScreenFileLocation.ActiveValue)+1;
		 return 1;
	 }
     }
    if( AnscEqualString(ParamName, "CMVoiceImageSelect", TRUE))
    {
#if defined(_COSA_BCM_ARM_) && !defined(_CBR_PRODUCT_REQ_)
	char buf[64] = { 0 };
	if(0 == syscfg_get(NULL, "CMVoiceImg", buf, sizeof(buf)))
	{
		if (AnscSizeOfString(buf) < *pulSize)
		{
			AnscCopyString( pValue, buf);
			return 0;
		}
		else
		{
			*pulSize = AnscSizeOfString(buf)+1;
			return 1;
		}
	}
	else
	{
		CcspTraceError(("syscfg_get for CMVoiceImg failed\n"));
		return 1;
	}
#else
	return 0;
#endif
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
    char * requestorStr = getRequestorString();
    char * currentTime = getTime();

    IS_UPDATE_ALLOWED_IN_DM(ParamName, requestorStr);

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "TR69CertLocation", TRUE) )
    {
                IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pMyObject->TR69CertLocation.UpdateSource);

		retValue = CosaDmlDiSetSyndicationTR69CertLocation( hInsContext, pString );
		if( ANSC_STATUS_SUCCESS == retValue )
		{
			memset( pMyObject->TR69CertLocation.ActiveValue, 0, sizeof( pMyObject->TR69CertLocation.ActiveValue ));
			AnscCopyString( pMyObject->TR69CertLocation.ActiveValue, pString );

                        memset( pMyObject->TR69CertLocation.UpdateSource, 0, sizeof( pMyObject->TR69CertLocation.UpdateSource ));
                        AnscCopyString( pMyObject->TR69CertLocation.UpdateSource, pString );

                        getPartnerId(PartnerID);
                        if ( PartnerID[ 0 ] != '\0')
                            UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.TR69CertLocation",PartnerID, pString, requestorStr, currentTime);
		}
		
		return TRUE;
    }
    if( AnscEqualString(ParamName, "PartnerId", TRUE) )
    {
		if ( (strcmp(pMyObject->PartnerID,pString) != 0 ) )
		{
			retValue = setTempPartnerId( pString );
			if( ANSC_STATUS_SUCCESS == retValue )
			{
			   ULONG    size = 0;
				//Get the Factory PartnerID
			    memset(PartnerID, 0, sizeof(PartnerID));
			    getFactoryPartnerId(PartnerID, &size);
			
			    CcspTraceInfo(("[SET-PARTNERID] Factory_Partner_ID:%s\n", ( PartnerID[ 0 ] != '\0' ) ? PartnerID : "NULL" ));
			    CcspTraceInfo(("[SET-PARTNERID] Current_PartnerID:%s\n", pMyObject->PartnerID ));
			    CcspTraceInfo(("[SET-PARTNERID] Overriding_PartnerID:%s\n", pString ));
								
				return TRUE;
			}
		}
    }
    if((CCSP_SUCCESS == getPartnerId(PartnerID) ) && ( PartnerID[ 0 ] != '\0'))
    {
   	 /* check the parameter name and set the corresponding value */
	 if( AnscEqualString(ParamName, "PauseScreenFileLocation", TRUE) )
	{
                IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pMyObject->UiBrand.PauseScreenFileLocation.UpdateSource);

		if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.PauseScreenFileLocation",PartnerID, pString, requestorStr, currentTime))
		{
			memset( pMyObject->UiBrand.PauseScreenFileLocation.ActiveValue, 0, sizeof( pMyObject->UiBrand.PauseScreenFileLocation.ActiveValue ));
			AnscCopyString( pMyObject->UiBrand.PauseScreenFileLocation.ActiveValue, pString );

                        memset( pMyObject->UiBrand.PauseScreenFileLocation.UpdateSource, 0, sizeof( pMyObject->UiBrand.PauseScreenFileLocation.UpdateSource ));
                        AnscCopyString( pMyObject->UiBrand.PauseScreenFileLocation.UpdateSource, requestorStr );

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
        SwitchToUDHCPC_GetParamBoolValue
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
SwitchToUDHCPC_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
	/* This Get API is only for XB3,AXB6 devices */
    PCOSA_DATAMODEL_DEVICEINFO              pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    BOOL                            bReturnValue;
#if defined(_COSA_INTEL_XB3_ARM_) || defined(INTEL_PUMA7)

    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* collect value */
        char buf[8];
        if( syscfg_get( NULL, "UDHCPEnable", buf, sizeof(buf))==0)
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
            CcspTraceWarning(("%s syscfg_get failed  for UDHCPEnable\n",__FUNCTION__));
            *pBool = FALSE;
        }

        return TRUE;
    }
#else
    return FALSE;
#endif

}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        SwitchToUDHCPC_SetParamBoolValue
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
SwitchToUDHCPC_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
     /* This set API is only for XB3,AXB6 devices */

    PCOSA_DATAMODEL_DEVICEINFO              pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
#if defined(_COSA_INTEL_XB3_ARM_) || defined(INTEL_PUMA7)
if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        char buf[8];
        char event[8];
        int val;

        /* collect previous flag value */
       if( syscfg_get( NULL, "UDHCPEnable", buf, sizeof(buf)) == 0)
        {
        	val = strcmp(buf,"true")?0:1;
                if (val != bValue)
                {
                        if (bValue)
                        {
                                commonSyseventGet("dhcpclient_v6", event, sizeof(event));
                                if (atoi(event) == 1)
                                {
                                        commonSyseventSet("dhcpclient_v4", "1");
                                        AnscTraceWarning(("dhcpclient_v6 event is enabled.so,enabling dhcpclient_v4 event \n"));
                                }
                                else
                                {
                                        system("sh /etc/dhcpswitch.sh schedule_v4_cron &");
                                        AnscTraceWarning(("dhcpclient_v6 event is not enabled.scheduling cron \n"));
                                }
                        }
                        else
                        {
                                system("sh /etc/dhcpswitch.sh clear_v4_cron &");
                                AnscTraceWarning(("dhcp client switching back to default \n"));
                        }
                }
                else if(!bValue)
                {
                        char v4event[8];
                        commonSyseventGet("dhcpclient_v4", v4event, sizeof(v4event));
                        if (atoi(v4event) == 1)
                        {
                                char v6event[8];
                                commonSyseventGet("dhcpclient_v6", v6event, sizeof(v6event));
                                if (atoi(v6event) ==1)
                                {
                                        commonSyseventSet("dhcpclient_v4", "0");
                                        AnscTraceWarning(("dhcpclient_v4 is disabled and no wan restart\n"));
                                }
                                else
                                {
                                        commonSyseventSet("dhcpclient_v4", "0");
                                        system("sh /etc/dhcpswitch.sh removecron &");
                                        AnscTraceWarning(("dhcpclient_v4 is disabled and scheduled cron removed\n"));
                                }


                        }
                        else
                        {
                                AnscTraceWarning(("No set operation done since UDHCPEnable flag already set to %d\n", bValue));
                        }
                }
                else
                {
                        AnscTraceWarning(("No set operation done since UDHCPEnable flag already set to %d\n", bValue));
                }
        }
        else
        {
                AnscTraceWarning(("syscfg_get failed for UDHCPEnable\n"));
                return FALSE;
        }


        return TRUE;
    }
#else

   return FALSE;

#endif
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

    return FALSE;
}

/***********************************************************************
APIs for Object:
        Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.WANsideSSH.
          *  WANsideSSH_GetParamBoolValue
          *  WANsideSSH_SetParamBoolValue

***********************************************************************/

BOOL
WANsideSSH_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
	/* check the parameter name and return the corresponding value */
        PCOSA_DATAMODEL_DEVICEINFO              pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;


        if( AnscEqualString(ParamName, "Enable", TRUE))
        {
		*pBool = pMyObject->bWANsideSSHEnable.ActiveValue;
                return TRUE;
        }


	return FALSE;
}

BOOL
WANsideSSH_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
   PCOSA_DATAMODEL_DEVICEINFO              pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;

   if( AnscEqualString(ParamName, "Enable", TRUE) )
   {
        char * requestorStr = getRequestorString();
        char * currentTime = getTime();

        IS_UPDATE_ALLOWED_IN_DM(ParamName, requestorStr);
        IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pMyObject->bWANsideSSHEnable.UpdateSource);

	if (pMyObject->bWANsideSSHEnable.ActiveValue == bValue)
	{
		CcspTraceInfo(("WANsideSSH is already %s\n", ( bValue ==TRUE ) ?  "Enabled" : "Disabled"));
		return TRUE;
	}

	if(syscfg_set(NULL, "WANsideSSH_Enable", ((bValue == TRUE ) ? "true" : "false"))==0)
	{
		syscfg_commit();
	}
	else
	{
		return FALSE;
	}

	pMyObject->bWANsideSSHEnable.ActiveValue = bValue;
	if (bValue == TRUE)
		system("sh /lib/rdk/wan_ssh.sh enable &");
	else
		system("sh /lib/rdk/wan_ssh.sh disable &");

        char *value = ( bValue ==TRUE ) ?  "true" : "false";
        char PartnerID[PARTNER_ID_LEN] = {0};
        if((CCSP_SUCCESS == getPartnerId(PartnerID) ) && (PartnerID[ 0 ] != '\0') )
            UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.WANsideSSH.Enable",PartnerID, value, requestorStr, currentTime);

	return TRUE;

   }
   return FALSE;
}
/***********************************************************************
APIs for Object:
	Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_Control.
	  *  RDKB_Control_GetParamBoolValue
	  *  RDKB_Control_SetParamBoolValue

***********************************************************************/

BOOL
RDKB_Control_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* check the parameter name and return the corresponding value */
	PCOSA_DATAMODEL_DEVICEINFO		pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
	PCOSA_DATAMODEL_RDKB_UIBRANDING	pBindObj =	& pMyObject->UiBrand;

	if( AnscEqualString(ParamName, "ActivatePartnerId", TRUE))
	{
		return TRUE;
	}

	if( AnscEqualString(ParamName, "ClearPartnerId", TRUE))
	{
		return TRUE;
	}

    return FALSE;
}

BOOL
RDKB_Control_SetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    PCOSA_DATAMODEL_RDKB_UIBRANDING	pBindObj =	& pMyObject->UiBrand;

    char *value = ( bValue ==TRUE ) ?  "true" : "false";
    pthread_t tid ; 
   ANSC_STATUS 				   retValue  = ANSC_STATUS_FAILURE;

   if( AnscEqualString(ParamName, "ActivatePartnerId", TRUE) )
    {
	if ( bValue )
	{
		retValue = activatePartnerId ( ) ;
		if( ANSC_STATUS_SUCCESS == retValue )
		{
			return TRUE;
		}
	}
    }

    if( AnscEqualString(ParamName, "ClearPartnerId", TRUE) )
    {
	if ( bValue )
	{
		CcspTraceWarning(("%s: Clearing PartnerId and device going Factory Reset  \n", __FUNCTION__));
		system( "rm -rf  /nvram/.partner_ID" );
		pthread_create ( &tid, NULL, &CosaDmlDiPartnerIDChangeHandling, NULL );
		return TRUE;
	}
    }

    return FALSE;
}

/***********************************************************************
APIs for Object:
	DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.
	   *  RDKB_UIBranding_GetParamBoolValue
	   *  RDKB_UIBranding_GetParamStringValue
	   *  RDKB_UIBranding_SetParamStringValue
	  
***********************************************************************/
BOOL
RDKB_UIBranding_GetParamBoolValue

	(
		ANSC_HANDLE 			hInsContext,
		char*				ParamName,
		BOOL*                       	pBool
	)
{

	PCOSA_DATAMODEL_DEVICEINFO		pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
	PCOSA_DATAMODEL_RDKB_UIBRANDING	pBindObj =	& pMyObject->UiBrand;

        if( AnscEqualString(ParamName, "AllowEthernetWAN", TRUE))
        {
		 /* collect value */
		*pBool = pBindObj->AllowEthernetWAN.ActiveValue;
		return TRUE;
        }
    return FALSE;
}


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
	
	if( AnscEqualString(ParamName, "DefaultLocalIPv4SubnetRange", TRUE))
        {
                if ( AnscSizeOfString(pBindObj->DefaultLocalIPv4SubnetRange.ActiveValue) < *pulSize)
                {
                        AnscCopyString( pValue, pBindObj->DefaultLocalIPv4SubnetRange.ActiveValue);
                        return 0;
                }
                else
                {
                        *pulSize = AnscSizeOfString(pBindObj->DefaultLocalIPv4SubnetRange.ActiveValue)+1;
                        return 1;
                }

        }
	if( AnscEqualString(ParamName, "DefaultLanguage", TRUE))
        {
                if ( AnscSizeOfString(pBindObj->DefaultLanguage.ActiveValue) < *pulSize)
                {
                        AnscCopyString( pValue, pBindObj->DefaultLanguage.ActiveValue);
                        return 0;
                }
                else
                {
                        *pulSize = AnscSizeOfString(pBindObj->DefaultLanguage.ActiveValue)+1;
                        return 1;
                }

        }
	if( AnscEqualString(ParamName, "DefaultAdminIP", TRUE))
        {
		if ( AnscSizeOfString(pBindObj->DefaultAdminIP.ActiveValue) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->DefaultAdminIP.ActiveValue);		
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->DefaultAdminIP.ActiveValue)+1;
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
	PCOSA_DATAMODEL_DEVICEINFO              pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
        PCOSA_DATAMODEL_RDKB_UIBRANDING pBindObj =      & pMyObject->UiBrand;
        char PartnerID[PARTNER_ID_LEN] = {0};
        char * requestorStr = getRequestorString();
        char * currentTime = getTime();

        IS_UPDATE_ALLOWED_IN_DM(ParamName, requestorStr);

   if((CCSP_SUCCESS == getPartnerId(PartnerID) ) && (PartnerID[ 0 ] != '\0') )
   {

         if( AnscEqualString(ParamName, "DefaultLanguage", TRUE) )
            {
                        IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->DefaultLanguage.UpdateSource);

                        if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.DefaultLanguage",PartnerID,pString, requestorStr, currentTime))
                        {
                                memset( pBindObj->DefaultLanguage.ActiveValue, 0, sizeof( pBindObj->DefaultLanguage.ActiveValue ));
                                AnscCopyString( pBindObj->DefaultLanguage.ActiveValue, pString );

                                memset( pBindObj->DefaultLanguage.UpdateSource, 0, sizeof( pBindObj->DefaultLanguage.UpdateSource ));
                                AnscCopyString( pBindObj->DefaultLanguage.UpdateSource, requestorStr );
                                return TRUE;
                        }
            }

    return FALSE;
   }
}

/***********************************************************************
APIs for Object:
	DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.
	  *  Footer_GetParamStringValue
	  *  Footer_SetParamStringValue
	  
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
        ULONG strSize;
	if( AnscEqualString(ParamName, "PartnerLink", TRUE))
	{
		/* collect value */
                strSize = AnscSizeOfString(pBindObj->Footer.PartnerLink.ActiveValue);
		 if ( strSize < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->Footer.PartnerLink.ActiveValue);
            		return 0;
       		}
       		else
       		{
           		*pulSize = strSize+1;
           		return 1;
       		}
	}

	if( AnscEqualString(ParamName, "UserGuideLink", TRUE))
	{
		 if ( AnscSizeOfString(pBindObj->Footer.UserGuideLink.ActiveValue) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->Footer.UserGuideLink.ActiveValue);
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->Footer.UserGuideLink.ActiveValue)+1;
           		return 1;
       		}
	}
	
	if( AnscEqualString(ParamName, "CustomerCentralLink", TRUE))
	{
		if ( AnscSizeOfString(pBindObj->Footer.CustomerCentralLink.ActiveValue) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->Footer.CustomerCentralLink.ActiveValue);
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->Footer.CustomerCentralLink.ActiveValue)+1;
           		return 1;
       		}

	}

	if( AnscEqualString(ParamName, "PartnerText", TRUE))
	{
		if ( AnscSizeOfString(pBindObj->Footer.PartnerText.ActiveValue) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->Footer.PartnerText.ActiveValue);
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->Footer.PartnerText.ActiveValue)+1;
           		return 1;
       		}

	}

	if( AnscEqualString(ParamName, "UserGuideText", TRUE))
	{
		if ( AnscSizeOfString(pBindObj->Footer.UserGuideText.ActiveValue) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->Footer.UserGuideText.ActiveValue);
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->Footer.UserGuideText.ActiveValue)+1;
           		return 1;
       		}

	}

	if( AnscEqualString(ParamName, "CustomerCentralText", TRUE))
	{
		if ( AnscSizeOfString(pBindObj->Footer.CustomerCentralText.ActiveValue) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->Footer.CustomerCentralText.ActiveValue);
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->Footer.CustomerCentralText.ActiveValue)+1;
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
    char * requestorStr = getRequestorString();
    char * currentTime = getTime();

    CcspTraceWarning(("%s: writeID=%d, bsUpdate=%d\n", __FUNCTION__, g_currentWriteEntity, g_currentBsUpdate));
    IS_UPDATE_ALLOWED_IN_DM(ParamName, requestorStr);

    if((CCSP_SUCCESS == getPartnerId(PartnerID) ) && (PartnerID[ 0 ] != '\0') )
    {
   	 /* check the parameter name and set the corresponding value */
	    if( AnscEqualString(ParamName, "PartnerLink", TRUE) )
	    {
                        IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->Footer.PartnerLink.UpdateSource);

			if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.PartnerLink",PartnerID,pString, requestorStr, currentTime))
			{
				memset( pBindObj->Footer.PartnerLink.ActiveValue, 0, sizeof( pBindObj->Footer.PartnerLink.ActiveValue ));
				AnscCopyString( pBindObj->Footer.PartnerLink.ActiveValue, pString );

				memset( pBindObj->Footer.PartnerLink.UpdateSource, 0, sizeof( pBindObj->Footer.PartnerLink.UpdateSource ));
                                AnscCopyString( pBindObj->Footer.PartnerLink.UpdateSource, requestorStr);

				return TRUE;
			}

	    }
	    if( AnscEqualString(ParamName, "UserGuideLink", TRUE) )
	    {
                        IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->Footer.UserGuideLink.UpdateSource);

			if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.UserGuideLink",PartnerID,pString, requestorStr, currentTime))
			{
				memset( pBindObj->Footer.UserGuideLink.ActiveValue, 0, sizeof( pBindObj->Footer.UserGuideLink.ActiveValue ));
				AnscCopyString(pBindObj->Footer.UserGuideLink.ActiveValue, pString );

                                memset( pBindObj->Footer.UserGuideLink.UpdateSource, 0, sizeof( pBindObj->Footer.UserGuideLink.UpdateSource ));
                                AnscCopyString( pBindObj->Footer.UserGuideLink.UpdateSource, requestorStr);

				return TRUE;
			}

	    }
	    if( AnscEqualString(ParamName, "CustomerCentralLink", TRUE) )
	    {
                        IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->Footer.CustomerCentralLink.UpdateSource);

			if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.CustomerCentralLink",PartnerID,pString, requestorStr, currentTime))
			{
				memset( pBindObj->Footer.CustomerCentralLink.ActiveValue, 0, sizeof( pBindObj->Footer.CustomerCentralLink.ActiveValue ));
				AnscCopyString( pBindObj->Footer.CustomerCentralLink.ActiveValue, pString );

                                memset( pBindObj->Footer.CustomerCentralLink.UpdateSource, 0, sizeof( pBindObj->Footer.CustomerCentralLink.UpdateSource ));
                                AnscCopyString( pBindObj->Footer.CustomerCentralLink.UpdateSource, requestorStr);

				return TRUE;
			}
			
	    }

	if( AnscEqualString(ParamName, "PartnerText", TRUE) )
	{
                IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->Footer.PartnerText.UpdateSource);

		if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.PartnerText",PartnerID,pString, requestorStr, currentTime))
		{
			memset( pBindObj->Footer.PartnerText.ActiveValue, 0, sizeof( pBindObj->Footer.PartnerText.ActiveValue ));
			AnscCopyString( pBindObj->Footer.PartnerText.ActiveValue, pString );


                        memset( pBindObj->Footer.PartnerText.UpdateSource, 0, sizeof( pBindObj->Footer.PartnerText.UpdateSource ));
                        AnscCopyString( pBindObj->Footer.PartnerText.UpdateSource, requestorStr);

			return TRUE;
		}

	}

	if( AnscEqualString(ParamName, "UserGuideText", TRUE) )
	{
                IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->Footer.UserGuideText.UpdateSource);

		if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.UserGuideText",PartnerID,pString, requestorStr, currentTime))
		{
			memset( pBindObj->Footer.UserGuideText.ActiveValue, 0, sizeof( pBindObj->Footer.UserGuideText.ActiveValue ));
			AnscCopyString( pBindObj->Footer.UserGuideText.ActiveValue, pString );

                        memset( pBindObj->Footer.UserGuideText.UpdateSource, 0, sizeof( pBindObj->Footer.UserGuideText.UpdateSource ));
                        AnscCopyString( pBindObj->Footer.UserGuideText.UpdateSource, requestorStr);

			return TRUE;
		}

	}

 	if( AnscEqualString(ParamName, "CustomerCentralText", TRUE) )
 	{
                IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->Footer.CustomerCentralText.UpdateSource);

 		if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Footer.CustomerCentralText",PartnerID,pString, requestorStr, currentTime))
 		{
 			memset( pBindObj->Footer.CustomerCentralText.ActiveValue, 0, sizeof( pBindObj->Footer.CustomerCentralText.ActiveValue ));
 			AnscCopyString( pBindObj->Footer.CustomerCentralText.ActiveValue, pString );

                        memset( pBindObj->Footer.CustomerCentralText.UpdateSource, 0, sizeof( pBindObj->Footer.CustomerCentralText.UpdateSource ));
                        AnscCopyString( pBindObj->Footer.CustomerCentralText.UpdateSource, requestorStr);

 			return TRUE;
 		}
 	}
   }
   
    return FALSE;
}

/***********************************************************************
APIs for Object:
	DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Connection.
	  *  Connection_GetParamStringValue
	  
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
		if ( AnscSizeOfString(pBindObj->Connection.MSOmenu.ActiveValue) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->Connection.MSOmenu.ActiveValue);
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->Connection.MSOmenu.ActiveValue)+1;
           		return 1;
       		}
	}

	if( AnscEqualString(ParamName, "MSOinfo", TRUE))
	{
		if ( AnscSizeOfString(pBindObj->Connection.MSOinfo.ActiveValue) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->Connection.MSOinfo.ActiveValue);	
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->Connection.MSOinfo.ActiveValue)+1;
           		return 1;
       		}
		
	}
	
	if( AnscEqualString(ParamName, "StatusTitle", TRUE))
	{
		if ( AnscSizeOfString(pBindObj->Connection.StatusTitle.ActiveValue) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->Connection.StatusTitle.ActiveValue);
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->Connection.StatusTitle.ActiveValue)+1;
           		return 1;
       		}
	}
	
	if( AnscEqualString(ParamName, "StatusInfo", TRUE))
	{
		if ( AnscSizeOfString(pBindObj->Connection.StatusInfo.ActiveValue) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->Connection.StatusInfo.ActiveValue);		
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->Connection.StatusInfo.ActiveValue)+1;
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
    char * requestorStr = getRequestorString();
    char * currentTime = getTime();

    IS_UPDATE_ALLOWED_IN_DM(ParamName, requestorStr);

   if((CCSP_SUCCESS == getPartnerId(PartnerID) ) && ( PartnerID[ 0 ] != '\0') )
   {
   	 /* check the parameter name and set the corresponding value */
	    if( AnscEqualString(ParamName, "MSOmenu", TRUE) )
	    {
                        IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->Connection.MSOmenu.UpdateSource);

			if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Connection.MSOmenu",PartnerID,pString, requestorStr, currentTime))
			{
				memset( pBindObj->Connection.MSOmenu.ActiveValue, 0, sizeof( pBindObj->Connection.MSOmenu.ActiveValue ));
				AnscCopyString( pBindObj->Connection.MSOmenu.ActiveValue, pString );

				memset( pBindObj->Connection.MSOmenu.UpdateSource, 0, sizeof( pBindObj->Connection.MSOmenu.UpdateSource ));
                                AnscCopyString( pBindObj->Connection.MSOmenu.UpdateSource, requestorStr );

				return TRUE;
			}
	    }
	    if( AnscEqualString(ParamName, "MSOinfo", TRUE) )
	    {
                        IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->Connection.MSOinfo.UpdateSource);

			if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Connection.MSOinfo",PartnerID,pString, requestorStr, currentTime) )
			{
				memset( pBindObj->Connection.MSOinfo.ActiveValue, 0, sizeof( pBindObj->Connection.MSOinfo.ActiveValue ));
				AnscCopyString(pBindObj->Connection.MSOinfo.ActiveValue, pString );

                                memset( pBindObj->Connection.MSOinfo.UpdateSource, 0, sizeof( pBindObj->Connection.MSOinfo.UpdateSource ));
                                AnscCopyString( pBindObj->Connection.MSOinfo.UpdateSource, requestorStr );

				return TRUE;
			}
	    }
	    if( AnscEqualString(ParamName, "StatusTitle", TRUE) )
	    {
                        IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->Connection.StatusTitle.UpdateSource);

			if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Connection.StatusTitle",PartnerID,pString, requestorStr, currentTime) )
			{
				memset( pBindObj->Connection.StatusTitle.ActiveValue, 0, sizeof( pBindObj->Connection.StatusTitle.ActiveValue ));
				AnscCopyString( pBindObj->Connection.StatusTitle.ActiveValue, pString );

                                memset( pBindObj->Connection.StatusTitle.UpdateSource, 0, sizeof( pBindObj->Connection.StatusTitle.UpdateSource ));
                                AnscCopyString( pBindObj->Connection.StatusTitle.UpdateSource, requestorStr );

				return TRUE;
			}
			
	    }
	    if( AnscEqualString(ParamName, "StatusInfo", TRUE) )
	    {
                        IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->Connection.StatusInfo.UpdateSource);

			if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.Connection.StatusInfo",PartnerID,pString, requestorStr, currentTime))
			{
				memset(pBindObj->Connection.StatusInfo.ActiveValue, 0, sizeof( pBindObj->Connection.StatusInfo.ActiveValue ));
				AnscCopyString( pBindObj->Connection.StatusInfo.ActiveValue, pString );

                                memset( pBindObj->Connection.StatusInfo.UpdateSource, 0, sizeof( pBindObj->Connection.StatusInfo.UpdateSource ));
                                AnscCopyString( pBindObj->Connection.StatusInfo.UpdateSource, requestorStr );

				return TRUE;
			}
	    }
   }
    return FALSE;
}

/***********************************************************************
APIs for Object:
	DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.NetworkDiagnosticTools.
	  *  NetworkDiagnosticTools_GetParamStringValue
	  
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
		if ( AnscSizeOfString(pBindObj->NDiagTool.ConnectivityTestURL.ActiveValue) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->NDiagTool.ConnectivityTestURL.ActiveValue);		
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->NDiagTool.ConnectivityTestURL.ActiveValue)+1;
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
    char * requestorStr = getRequestorString();
    char * currentTime = getTime();

    IS_UPDATE_ALLOWED_IN_DM(ParamName, requestorStr);

   if((CCSP_SUCCESS == getPartnerId(PartnerID) ) && ( PartnerID[ 0 ] != '\0'))
   {
   	 /* check the parameter name and set the corresponding value */
	    if( AnscEqualString(ParamName, "ConnectivityTestURL", TRUE) )
	    {
                        IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->NDiagTool.ConnectivityTestURL.UpdateSource);

			if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.NetworkDiagnosticTools.ConnectivityTestURL",PartnerID, pString, requestorStr, currentTime))
			{
				memset( pBindObj->NDiagTool.ConnectivityTestURL.ActiveValue, 0, sizeof( pBindObj->NDiagTool.ConnectivityTestURL.ActiveValue ));
				AnscCopyString( pBindObj->NDiagTool.ConnectivityTestURL.ActiveValue, pString );

                                memset( pBindObj->NDiagTool.ConnectivityTestURL.UpdateSource, 0, sizeof( pBindObj->NDiagTool.ConnectivityTestURL.UpdateSource ));
                                AnscCopyString( pBindObj->NDiagTool.ConnectivityTestURL.UpdateSource, requestorStr );

				return TRUE;
			}	
	    }
	  
   }
    return FALSE;
}

/***********************************************************************
APIs for Object:
	DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.
	  *  WiFiPersonalization_GetParamStringValue
	  
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
		*pBool = pBindObj->WifiPersonal.Support.ActiveValue;
		return TRUE;
        }    

	if( AnscEqualString(ParamName, "SMSsupport", TRUE))
        {
		/* collect value */
		*pBool = pBindObj->WifiPersonal.SMSsupport.ActiveValue;
		return TRUE;

        }
	
        if( AnscEqualString(ParamName, "MyAccountAppSupport", TRUE))
	{
		/* collect value */
		*pBool = pBindObj->WifiPersonal.MyAccountAppSupport.ActiveValue;
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
   char * requestorStr = getRequestorString();
   char * currentTime = getTime();

   IS_UPDATE_ALLOWED_IN_DM(ParamName, requestorStr);

   if((CCSP_SUCCESS == getPartnerId(PartnerID) ) && ( PartnerID[ 0 ] != '\0'))
   {
	    if( AnscEqualString(ParamName, "Support", TRUE))
	    {
                        IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->WifiPersonal.Support.UpdateSource);

			if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.Support",PartnerID, value, requestorStr, currentTime ))
			{
				pBindObj->WifiPersonal.Support.ActiveValue = bValue;

                                memset( pBindObj->WifiPersonal.Support.UpdateSource, 0, sizeof( pBindObj->WifiPersonal.Support.UpdateSource ));
                                AnscCopyString( pBindObj->WifiPersonal.Support.UpdateSource, requestorStr );

				return TRUE;
			}	

	    }

	    if( AnscEqualString(ParamName, "SMSsupport", TRUE))
	    {
                        IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->WifiPersonal.SMSsupport.UpdateSource);

			if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.SMSsupport",PartnerID, value, requestorStr, currentTime))
			{
				pBindObj->WifiPersonal.SMSsupport.ActiveValue = bValue;

                                memset( pBindObj->WifiPersonal.SMSsupport.UpdateSource, 0, sizeof( pBindObj->WifiPersonal.SMSsupport.UpdateSource ));
                                AnscCopyString( pBindObj->WifiPersonal.SMSsupport.UpdateSource, requestorStr );

				return TRUE;
			}
			
	    }
		
	    if( AnscEqualString(ParamName, "MyAccountAppSupport", TRUE))
	    {
                        IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->WifiPersonal.MyAccountAppSupport.UpdateSource);

			if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.MyAccountAppSupport",PartnerID, value, requestorStr, currentTime))
			{
				pBindObj->WifiPersonal.MyAccountAppSupport.ActiveValue = bValue;

                                memset( pBindObj->WifiPersonal.MyAccountAppSupport.UpdateSource, 0, sizeof( pBindObj->WifiPersonal.MyAccountAppSupport.UpdateSource ));
                                AnscCopyString( pBindObj->WifiPersonal.MyAccountAppSupport.UpdateSource, requestorStr );

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
		if ( AnscSizeOfString(pBindObj->WifiPersonal.PartnerHelpLink.ActiveValue) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->WifiPersonal.PartnerHelpLink.ActiveValue);		
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->WifiPersonal.PartnerHelpLink.ActiveValue)+1;
           		return 1;
       		}

        } 
	if( AnscEqualString(ParamName, "MSOLogo", TRUE))
        {
		if ( AnscSizeOfString(pBindObj->WifiPersonal.MSOLogo.ActiveValue) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->WifiPersonal.MSOLogo.ActiveValue);		
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->WifiPersonal.MSOLogo.ActiveValue)+1;
           		return 1;
       		}

        } 
	if( AnscEqualString(ParamName, "Title", TRUE))
        {
		if ( AnscSizeOfString(pBindObj->WifiPersonal.Title.ActiveValue) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->WifiPersonal.Title.ActiveValue);		
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->WifiPersonal.Title.ActiveValue)+1;
           		return 1;
       		}

        } 
	if( AnscEqualString(ParamName, "WelcomeMessage", TRUE))
        {
		if ( AnscSizeOfString(pBindObj->WifiPersonal.WelcomeMessage.ActiveValue) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->WifiPersonal.WelcomeMessage.ActiveValue);		
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->WifiPersonal.WelcomeMessage.ActiveValue)+1;
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
    char * requestorStr = getRequestorString();
    char * currentTime = getTime();

    IS_UPDATE_ALLOWED_IN_DM(ParamName, requestorStr);

   if((CCSP_SUCCESS == getPartnerId(PartnerID) ) && ( PartnerID[ 0 ] != '\0') )
   {
   	 /* check the parameter name and set the corresponding value */
	    if( AnscEqualString(ParamName, "PartnerHelpLink", TRUE) )
	    {
                        IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->WifiPersonal.PartnerHelpLink.UpdateSource);

			if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.WiFiPersonalization.PartnerHelpLink",PartnerID, pString, requestorStr, currentTime))
			{
				memset( pBindObj->WifiPersonal.PartnerHelpLink.ActiveValue, 0, sizeof( pBindObj->WifiPersonal.PartnerHelpLink.ActiveValue ));
				AnscCopyString( pBindObj->WifiPersonal.PartnerHelpLink.ActiveValue, pString );

                                memset( pBindObj->WifiPersonal.PartnerHelpLink.UpdateSource, 0, sizeof( pBindObj->WifiPersonal.PartnerHelpLink.UpdateSource ));
                                AnscCopyString( pBindObj->WifiPersonal.PartnerHelpLink.UpdateSource, requestorStr );

				return TRUE;
			}			
	    }
	  
   }
    return FALSE;
}

/***********************************************************************
APIs for Object:
	DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.LocalUI.
	  *  LocalUI_GetParamStringValue
      *  LocalUI_SetParamStringValue
      *  LocalUI_GetParamBoolValue
      *  LocalUI_SetParamBoolValue
	  
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
		if ( AnscSizeOfString(pBindObj->LocalUI.MSOLogo.ActiveValue) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->LocalUI.MSOLogo.ActiveValue);		
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->LocalUI.MSOLogo.ActiveValue)+1;
           		return 1;
       		}

        } 
	if( AnscEqualString(ParamName, "DefaultLoginUsername", TRUE))
        {
		if ( AnscSizeOfString(pBindObj->LocalUI.DefaultLoginUsername.ActiveValue) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->LocalUI.DefaultLoginUsername.ActiveValue);		
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->LocalUI.DefaultLoginUsername.ActiveValue)+1;
           		return 1;
       		}

        } 
	if( AnscEqualString(ParamName, "DefaultLoginPassword", TRUE))
        {
		if ( AnscSizeOfString(pBindObj->LocalUI.DefaultLoginPassword.ActiveValue) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->LocalUI.DefaultLoginPassword.ActiveValue);		
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->LocalUI.DefaultLoginPassword.ActiveValue)+1;
           		return 1;
       		}

        } 
	if( AnscEqualString(ParamName, "MSOLogoTitle", TRUE))
        {
		if ( AnscSizeOfString(pBindObj->LocalUI.MSOLogoTitle.ActiveValue) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->LocalUI.MSOLogoTitle.ActiveValue);		
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->LocalUI.MSOLogoTitle.ActiveValue)+1;
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
		*pBool = pBindObj->LocalUI.HomeNetworkControl.ActiveValue;
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
	  *  HelpTip_GetParamStringValue
      *  HelpTip_SetParamStringValue
	  
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
		if ( AnscSizeOfString(pBindObj->HelpTip.NetworkName.ActiveValue) < *pulSize)
       		{
           		AnscCopyString( pValue, pBindObj->HelpTip.NetworkName.ActiveValue);		
            		return 0;
       		}
       		else
       		{
           		*pulSize = AnscSizeOfString(pBindObj->HelpTip.NetworkName.ActiveValue)+1;
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

/***********************************************************************
APIs for Object:
    DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.CloudUI.
      *  CloudUI_GetParamStringValue
      *  CloudUI_SetParamStringValue

***********************************************************************/

ULONG
CloudUI_GetParamStringValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    PCOSA_DATAMODEL_RDKB_UIBRANDING pBindObj =  & pMyObject->UiBrand;

    if( AnscEqualString(ParamName, "brandname", TRUE))
    {
        if ( AnscSizeOfString(pBindObj->CloudUI.brandname.ActiveValue) < *pulSize)
            {
                AnscCopyString( pValue, pBindObj->CloudUI.brandname.ActiveValue);
                    return 0;
            }
            else
            {
                *pulSize = AnscSizeOfString(pBindObj->CloudUI.brandname.ActiveValue)+1;
                return 1;
            }
    }

    if( AnscEqualString(ParamName, "productname", TRUE))
    {
        if ( AnscSizeOfString(pBindObj->CloudUI.productname.ActiveValue) < *pulSize)
            {
                AnscCopyString( pValue, pBindObj->CloudUI.productname.ActiveValue);
                    return 0;
            }
            else
            {
                *pulSize = AnscSizeOfString(pBindObj->CloudUI.productname.ActiveValue)+1;
                return 1;
            }
    }

    if( AnscEqualString(ParamName, "link", TRUE))
    {
        if ( AnscSizeOfString(pBindObj->CloudUI.link.ActiveValue) < *pulSize)
            {
                AnscCopyString( pValue, pBindObj->CloudUI.link.ActiveValue);
                    return 0;
            }
            else
            {
                *pulSize = AnscSizeOfString(pBindObj->CloudUI.link.ActiveValue)+1;
                return 1;
            }
    }
     return -1;
}

BOOL
CloudUI_SetParamStringValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    PCOSA_DATAMODEL_RDKB_UIBRANDING pBindObj =  & pMyObject->UiBrand;

    char PartnerID[PARTNER_ID_LEN] = {0};
    char * requestorStr = getRequestorString();
    char * currentTime = getTime();

    IS_UPDATE_ALLOWED_IN_DM(ParamName, requestorStr);

   if((CCSP_SUCCESS == getPartnerId(PartnerID) ) && ( PartnerID[ 0 ] != '\0') )
   {
     /* check the parameter name and set the corresponding value */
        if( AnscEqualString(ParamName, "brandname", TRUE) )
        {
            IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->CloudUI.brandname.UpdateSource);

            if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.CloudUI.brandname",PartnerID, pString, requestorStr, currentTime))
            {
                memset( pBindObj->CloudUI.brandname.ActiveValue, 0, sizeof( pBindObj->CloudUI.brandname.ActiveValue ));
                AnscCopyString( pBindObj->CloudUI.brandname.ActiveValue, pString );

                memset( pBindObj->CloudUI.brandname.UpdateSource, 0, sizeof( pBindObj->CloudUI.brandname.UpdateSource ));
                AnscCopyString( pBindObj->CloudUI.brandname.UpdateSource, requestorStr );

                return TRUE;
            }
        }
        if( AnscEqualString(ParamName, "productname", TRUE) )
        {
            IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->CloudUI.productname.UpdateSource);

            if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.CloudUI.productname",PartnerID, pString, requestorStr, currentTime))
            {
                memset( pBindObj->CloudUI.productname.ActiveValue, 0, sizeof( pBindObj->CloudUI.productname.ActiveValue ));
                AnscCopyString( pBindObj->CloudUI.productname.ActiveValue, pString );

                memset( pBindObj->CloudUI.productname.UpdateSource, 0, sizeof( pBindObj->CloudUI.productname.UpdateSource ));
                AnscCopyString( pBindObj->CloudUI.productname.UpdateSource, requestorStr );

                return TRUE;
            }
        }
        if( AnscEqualString(ParamName, "link", TRUE) )
        {
            IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pBindObj->CloudUI.link.UpdateSource);

            if ( ANSC_STATUS_SUCCESS == UpdateJsonParam("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.RDKB_UIBranding.CloudUI.link",PartnerID, pString, requestorStr, currentTime))
            {
                memset( pBindObj->CloudUI.link.ActiveValue, 0, sizeof( pBindObj->CloudUI.link.ActiveValue ));
                AnscCopyString( pBindObj->CloudUI.link.ActiveValue, pString );

                memset( pBindObj->CloudUI.link.UpdateSource, 0, sizeof( pBindObj->CloudUI.link.UpdateSource ));
                AnscCopyString( pBindObj->CloudUI.link.UpdateSource, requestorStr );

                return TRUE;
            }
        }
    }
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
        char buff[64] = {0};
        sprintf(buff,"%d",uValue);
	Send_Notification_Task(buff, NULL, NULL, "reboot-pending", NULL);
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
     PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "RebootDevice", TRUE) )
    {
        /* collect value */
		AnscCopyString(pValue,"");
        return 0;
    }

    if( AnscEqualString(ParamName, "FirmwareDownloadStartedNotification", TRUE))
    {
	/* collect value */
	AnscCopyString( pValue, pMyObject->FirmwareDownloadStartedNotification);
	*pulSize = AnscSizeOfString( pValue );
	return 0;
    }

    if( AnscEqualString(ParamName, "DeviceManageableNotification", TRUE))
    {
	/* collect value */
	AnscCopyString( pValue, pMyObject->DeviceManageableNotification);
	*pulSize = AnscSizeOfString( pValue );
	return 0;
    }

    CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
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
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
    /* check the parameter name and set the corresponding value */

    if( AnscEqualString(ParamName, "RebootDevice", TRUE))
    {
      
        if( TRUE == CosaDmlDi_ValidateRebootDeviceParam( pString ) )
		{
			CosaDmlDiSet_RebootDevice(pString);
        	return TRUE;
		}
    }

     if( AnscEqualString(ParamName, "FirmwareDownloadStartedNotification", TRUE))
     {
	char notifyEnable[64];
        memset(notifyEnable, 0, sizeof(notifyEnable));

        syscfg_get( NULL, "ManageableNotificationEnabled", notifyEnable, sizeof(notifyEnable));
        if((notifyEnable[0] != '\0') && (strncmp(notifyEnable, "true", strlen("true")) == 0))
        {
		/* collect value */
		char buff[64];
		char *timeValue = NULL;

		memset(buff, 0, sizeof(buff));
		snprintf(buff,sizeof(buff),"%s",pString);
		memset( pMyObject->FirmwareDownloadStartedNotification, 0, sizeof( pMyObject->FirmwareDownloadStartedNotification ));
		AnscCopyString( pMyObject->FirmwareDownloadStartedNotification, pString );
		timeValue = strdup(buff);
		set_firmware_download_start_time(timeValue);
		Send_Notification_Task(NULL, buff, NULL, "firmware-download-started", NULL);
	}
	else
	{
		CcspTraceWarning(("ManageableNotificationEnabled is false, firmware download start notification is not sent\n"));
	}
	return TRUE;
     }

     if( AnscEqualString(ParamName, "DeviceManageableNotification", TRUE))
     {
        char status[64];
        memset(status, 0, sizeof(status));
        syscfg_get( NULL, "ManageableNotificationEnabled", status, sizeof(status));
        if((status[0] != '\0') && (strncmp(status, "true", strlen("true")) == 0))
        {
            /* collect value */
            char buff[64];
            memset(buff, 0, sizeof(buff));

            snprintf(buff,sizeof(buff),"%s",pString);
	    memset( pMyObject->DeviceManageableNotification, 0, sizeof( pMyObject->DeviceManageableNotification ));
	    AnscCopyString( pMyObject->DeviceManageableNotification, pString );
            Send_Notification_Task(NULL, NULL, NULL, "fully-manageable", buff);
        }
	else
	{
		CcspTraceWarning(("ManageableNotificationEnabled is false, device manage notification is not sent\n"));
	}
        return TRUE;
     }

     CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
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

    if( AnscEqualString(ParamName, "FirmwareDownloadCompletedNotification", TRUE))
    {
	/*CcspTraceWarning(("supported parameter '%s'\n", ParamName));*/
	*pBool = pMyObject->FirmwareDownloadCompletedNotification;
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
       if( TRUE == bValue )
       {
           AnscTraceWarning(("Triggering abortReboot from TR181\n"));
           // static collection as we don't want upgrade path to be changed without a reboot
           static BOOL rdkfwupgraderEnabledCollected = false;
           static BOOL RDKFWUpgraderEnabled = false;

           if (!rdkfwupgraderEnabledCollected) {
               if (RDKFirmwareUpgrader_GetParamBoolValue(hInsContext,
                                                         "Enable",
                                                         &RDKFWUpgraderEnabled) ) {
                   rdkfwupgraderEnabledCollected = true;
               }
           }

           // NOTE:: this might have an addional issue, rfc enabled but rdkfwupgrader was disabled at
           // build time. Do we need to take care of that like checking for presense /usr/bin/rdkfwupgrader
           // at runtime
           if (RDKFWUpgraderEnabled) {
               system("/lib/rdk/rdkfwupgrader_abort_reboot.sh &");
               return TRUE; //always true, let the statemachine decide if there is a reboot operation pending or not.
           } else {
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
               } else {
                   CcspTraceWarning(("Invalid request for parameter, no FW DL reboot wait in progress '%s'\n", ParamName));
                   return FALSE;
               }
           }
       }
       return FALSE;
   } 

	if( AnscEqualString(ParamName, "FirmwareDownloadCompletedNotification", TRUE))
	{
		char notifyEnable[64];
		memset(notifyEnable, 0, sizeof(notifyEnable));

		syscfg_get( NULL, "ManageableNotificationEnabled", notifyEnable, sizeof(notifyEnable));
		if((notifyEnable[0] != '\0') && (strncmp(notifyEnable, "true", strlen("true")) == 0))
		{
			/* collect value */
			char buff[8];

			memset(buff, 0, sizeof(buff));
			snprintf(buff, sizeof(buff), "%s", bValue ? "true" : "false");
			pMyObject->FirmwareDownloadCompletedNotification = bValue;
			char *start_time = get_firmware_download_start_time();
			Send_Notification_Task(NULL, start_time, buff, "firmware-download-completed", NULL);
		}
		else
		{
			CcspTraceWarning(("ManageableNotificationEnabled is false, firmware download completed notfication is not sent\n"));
		}
		return TRUE;
	}
	/* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
	return FALSE;
}
#endif

//sukant start
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

BOOL
Tile_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    /* check the parameter name and set the corresponding value */
    CcspTraceInfo(("Tile SetparamInt value:ReportingThrottling"));
    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "ReportingInterval", TRUE))
    {
        char buf[16]={0};

        memset(buf,0,sizeof(buf));
        sprintf(buf, "%d", iValue);

        if (syscfg_set(NULL, "TileReportingInterval", buf) != 0)
        {
               CcspTraceInfo(("syscfg_set Reportin  failed\n"));
        }
        else
        {
            if (syscfg_commit() != 0)
            {
                 CcspTraceInfo(("syscfg_commit ReportingThrottling failed\n"));
            }
        }

        return TRUE;
    }

   /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

BOOL
Tile_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        iValue
    )
{
    /* check the parameter name and set the corresponding value */
    CcspTraceInfo(("Tile GetParamInt Value Called\n"));
    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "ReportingInterval", TRUE))
    {
         /* collect value */
          char buf[10];

          memset(buf,0,sizeof(buf));
          syscfg_get( NULL, "TileReportingInterval", buf, sizeof(buf));
          if( buf [0] != '\0' )
          {
                 *iValue= ( atoi(buf) );
                  return TRUE;
          }
    }

   /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

BOOL
xBlueTooth_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
          if( AnscEqualString(ParamName, "LimitBeaconDetection", TRUE))
          {
               char buf[8];
               *pBool = FALSE;
               syscfg_get( NULL, "limit_beacon_detection", buf, sizeof(buf));
               if( buf != NULL )
               {
                  if (!strncasecmp(buf, "true", 4))
                  {
                      *pBool = TRUE;
                      return TRUE;
                  }
               }
           }
        return FALSE;
}

void handleBleRestart(void *arg)
{
        CcspTraceInfo(("handleBleRestart Thread Invoked \n"));
        pthread_detach(pthread_self());
        system("systemctl restart ble");
        CcspTraceInfo(("handleBleRestart Completed \n"));
}

BOOL
xBlueTooth_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
     CcspTraceInfo(("xBlueTooth_SetParamBoolValue \n"));
     pthread_t tid;
    if( AnscEqualString(ParamName, "LimitBeaconDetection", TRUE))
    {
        if(syscfg_set(NULL, "limit_beacon_detection", bValue ? "true" : "false"))
        {
            CcspTraceError(("syscfg_set failed\n"));
        }
        else
        {
            if (syscfg_commit() != 0)
            {
                CcspTraceError(("syscfg_commit failed\n"));
            }
        }
        pthread_create(&tid, NULL, handleBleRestart, NULL); 
        return TRUE;
    }
    return FALSE;
}


BOOL
Cmd_GetParamStringValue
(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 char*                       pValue,
 ULONG*                      pUlSize
 )
{
    CcspTraceWarning((" ring get Unsupported parameter '%s'\n", ParamName));
    return FALSE;
}


BOOL
Cmd_SetParamStringValue
(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 char*                       pString
 )
{
    char *cmd =  NULL;
    int index;
 if( AnscEqualString(ParamName, "Request", TRUE))
    {

         CcspTraceInfo(("***************************\n"));
         CcspTraceInfo(("The Json string=%s\n",pString));
         CcspTraceInfo(("***************************\n"));
         cJSON *cjson = cJSON_Parse(pString);
         if(cjson)
         {
             //first Red the code if its is MEP_TOA_OPEN_CHANNEL 
             if ( cJSON_GetObjectItem( cjson, "code") != NULL )
             {
                   cmd = cJSON_GetObjectItem(cjson, "code")->valuestring;
                   if(strcmp(cmd,"MEP_TOA_OPEN_CHANNEL") == 0)
                  {

                        if ( cJSON_GetObjectItem( cjson, "tile_uuid") != NULL )
                        {
                              CcspTraceInfo(("The Ring Tile Id :%s\n", cJSON_GetObjectItem(cjson, "tile_uuid")->valuestring));
                              if (syscfg_set(NULL, "cmdTileId", cJSON_GetObjectItem(cjson, "tile_uuid")->valuestring) != 0)
                              {
                                     CcspTraceInfo(("syscfg_set failed for RingTileId \n"));
                              }
                              else
                              {
                                 if (syscfg_commit() != 0)
                                 {
                                     CcspTraceInfo(("syscfg_commit failed for RingTileId\n"));
                                 }
                              }

                        }
                        if ( cJSON_GetObjectItem( cjson, "rand_a") != NULL )
                        {
                              CcspTraceInfo(("The Rand A :%s\n", cJSON_GetObjectItem(cjson, "rand_a")->valuestring));
                             if (syscfg_set(NULL, "Rand_a", cJSON_GetObjectItem(cjson, "rand_a")->valuestring) != 0)
                              {
                                     CcspTraceInfo(("syscfg_set failed for Rand_a \n"));
                              }
                              else
                              {
                                 if (syscfg_commit() != 0)
                                 {
                                     CcspTraceInfo(("syscfg_commit failed for Rand_a \n"));
                                 }
                              }


                        }
                        if ( cJSON_GetObjectItem( cjson, "session_token") != NULL )
                        {
                              CcspTraceInfo(("The Session Token :%s\n", cJSON_GetObjectItem(cjson, "session_token")->valuestring));
                              if (syscfg_set(NULL, "TileSession_Token", cJSON_GetObjectItem(cjson, "session_token")->valuestring) != 0)
                              {
                                     CcspTraceInfo(("syscfg_set failed for TileSession_Token \n"));
                              }
                              else
                              {
                                 if (syscfg_commit() != 0)
                                 {
                                     CcspTraceInfo(("syscfg_commit failed for TileSession_Token \n"));
                                 }
                              }
                        }
                        if ( cJSON_GetObjectItem( cjson, "cmst_traceId") != NULL )
                        {
                              CcspTraceInfo(("The Trace Id :%s\n", cJSON_GetObjectItem(cjson, "cmst_traceId")->valuestring));
                              if (syscfg_set(NULL, "cmst_traceid", cJSON_GetObjectItem(cjson, "cmst_traceId")->valuestring) != 0)
                              {
                                     CcspTraceInfo(("syscfg_set failed for cmst_traceid \n"));
                              }
                              else
                              {
                                 if (syscfg_commit() != 0)
                                 {
                                     CcspTraceInfo(("syscfg_commit failed for cmst_traceid \n"));
                                 }
                              }

                        }
                        CcspTraceInfo(("******************************** \n"));
                        CcspTraceInfo(("Open channel case\n"));
                        CcspTraceInfo(("setting cmdCode \n"));
                        if (syscfg_set(NULL, "cmdOpenChannel", cmd) != 0)
                        {
                            CcspTraceInfo(("syscfg_set failed for Ring command\n"));
                        }
                        else
                        {
                            if (syscfg_commit() != 0)
                            {
                                CcspTraceInfo(("syscfg_commit failed for Ring Command\n"));
                            }
                        }
                        cJSON_Delete(cjson);
                        CcspTraceInfo(("*****Return*****\n"));
                        return TRUE;


                  }else if(strcmp(cmd,"MEP_TOA_SEND_COMMANDS") == 0)
                  {
                      //Ring case
                        CcspTraceInfo(("***************************\n"));
                        CcspTraceInfo(("Open Ring  case\n"));
                        cJSON *disconnect = cJSON_GetObjectItem( cjson, "disconnect_on_completion");
                        if(cJSON_IsTrue(disconnect))
                        {
                               CcspTraceInfo(("Disconnect on completion set to true\n"));
                               if (syscfg_set(NULL, "TileDisconnectOnCompletion", "true") != 0)
                               {
                                     CcspTraceInfo(("syscfg_set failed for  disconnect on completion \n"));
                               }
                               else
                               {
                                   if (syscfg_commit() != 0)
                                   {
                                           CcspTraceInfo(("syscfg_commit failed for disconnect on completion\n"));
                                   }
                                }
                        }
                        else
                        {
                               CcspTraceInfo(("Disconnect on completion set to false \n"));
                               if (syscfg_set(NULL, "TileDisconnectOnCompletion", "false") != 0)
                               {
                                     CcspTraceInfo(("syscfg_set failed for  disconnect on completion \n"));
                               }
                               else
                               {
                                   if (syscfg_commit() != 0)
                                   {
                                           CcspTraceInfo(("syscfg_commit failed for disconnect on completion\n"));
                                   }
                               }
                        }

                        cJSON * array = cJSON_GetObjectItem(cjson, "commands");
                        CcspTraceInfo(("The array length = %d\n",cJSON_GetArraySize(array)));
                        for (index = 0; index < cJSON_GetArraySize(array); index++)
                        {
                            cJSON * subitem = cJSON_GetArrayItem(array, index);
                            CcspTraceInfo(("***** ALL Looks Well**** \n"));
                            if(subitem != NULL)
                            {
                                 CcspTraceInfo(("***** Sub Item all good**** \n"));
                                if( cJSON_GetObjectItem( subitem, "command") != NULL )
                                {
                                     CcspTraceInfo(("***** Sub GetItem all good**** \n"));
                                     if(index == 0)
                                     {
                                       CcspTraceInfo(("The Ring Cmd Ready:%s\n", cJSON_GetObjectItem(subitem, "command")->valuestring));
                                       if (syscfg_set(NULL, "cmdReady",cJSON_GetObjectItem(subitem, "command")->valuestring) != 0)
                                       {
                                           CcspTraceInfo(("syscfg_set failed for cmdReady \n"));
                                       }
                                       else
                                       {
                                           if (syscfg_commit() != 0)
                                           {
                                               CcspTraceInfo(("syscfg_commit failed for cmdReady\n"));
                                           }
                                        }
                                     }
                                     else
                                     {

                                        CcspTraceInfo(("The Ring Cmd Play:%s\n", cJSON_GetObjectItem(subitem, "command")->valuestring));
                                        if (syscfg_set(NULL, "cmdPlay",cJSON_GetObjectItem(subitem, "command")->valuestring) != 0)
                                        {
                                           CcspTraceInfo(("syscfg_set failed for cmdPlay \n"));
                                        }
                                        else
                                        {
                                           if (syscfg_commit() != 0)
                                           {
                                               CcspTraceInfo(("syscfg_commit failed for cmdPlay\n"));
                                           }
                                        }
                                     }
                                }
 
                                if( cJSON_GetObjectItem( subitem, "response_mask") != NULL )
                                {
                                      if(index == 0 )
                                      {
                                           CcspTraceInfo(("The Ring Cmd Ready mask:%s\n", cJSON_GetObjectItem(subitem, "response_mask")->valuestring));
                                           if (syscfg_set(NULL, "cmdReady_mask",cJSON_GetObjectItem(subitem, "response_mask")->valuestring) != 0)
                                           {
                                               CcspTraceInfo(("syscfg_set failed for cmdReady_mask \n"));
                                           }
                                           else
                                           {
                                              if (syscfg_commit() != 0)
                                              {
                                                 CcspTraceInfo(("syscfg_commit failed for cmdReady mask\n"));
                                              }
                                           }
                                      }
                                      else
                                      {
                                           CcspTraceInfo(("The Ring Cmd Play mask:%s\n", cJSON_GetObjectItem(subitem, "response_mask")->valuestring));
                                           if (syscfg_set(NULL, "cmdPlay_mask",cJSON_GetObjectItem(subitem, "response_mask")->valuestring) != 0)
                                           {
                                               CcspTraceInfo(("syscfg_set failed for cmdPlay_mask \n"));
                                           }
                                           else
                                           {
                                              if (syscfg_commit() != 0)
                                              {
                                                 CcspTraceInfo(("syscfg_commit failed for cmdPlay mask\n"));
                                              }
                                           }
                                      }
                                }
                            }//subitem != NULL

                        }//end for loop
                        CcspTraceInfo(("setting cmdCode \n"));
                        if (syscfg_set(NULL, "cmdCode", cmd) != 0)
                        {
                            CcspTraceInfo(("syscfg_set failed for Ring command\n"));
                        }
                        else
                        {
                            if (syscfg_commit() != 0)
                            {
                                CcspTraceInfo(("syscfg_commit failed for Ring Command\n"));
                            }
                        }
                        CcspTraceInfo(("***************************\n"));
                        CcspTraceInfo(("ALL WELL\n"));
                        cJSON_Delete(cjson);
                        CcspTraceInfo(("*****Return*****\n"));
                        return TRUE;

                  }

             }

         }
    }
    CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return FALSE;
}






/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL
        MessageBusSource_GetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL*                       pBool
            )



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
MessageBusSource_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{

 if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        char value[8] = {'\0'};
        if( syscfg_get(NULL, "MessageBusSource", value, sizeof(value)) == 0 )
        {
            if( value != NULL )
            {
                 if (strcmp(value, "true") == 0)
                     *pBool = TRUE;
                 else
                     *pBool = FALSE;
            }
            return TRUE;
        }
        else
        {
            CcspTraceError(("syscfg_get failed for MessageBusSource\n"));
        }
    }
  return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL
        MessageBusSource_SetParamBoolValu
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL                        bValue
            )


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
MessageBusSource_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{

  if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        char buf[8] = {'\0'};
        snprintf(buf, sizeof(buf), "%s", bValue ? "true" : "false");
        if( syscfg_set(NULL, "MessageBusSource", buf) != 0 )
        {
            CcspTraceError(("syscfg_set failed for MessageBusSource\n"));
        }
        else
        {
            if( syscfg_commit() == 0 )
            {
                return TRUE;
            }
            else
            {
                 CcspTraceError(("syscfg_commit failed for MessageBusSource\n"));
            }
        }

    }
  return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL
        UPnPRefactor_GetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL*                       pBool
            )



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
UPnPRefactor_GetParamBoolValue

    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{

 if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        char value[8] = {'\0'};
        if( syscfg_get(NULL, "Refactor", value, sizeof(value)) == 0 )
        {
            if( value != NULL )
            {
                 if (strcmp(value, "true") == 0)
                     *pBool = TRUE;
                 else
                     *pBool = FALSE;
            }
            return TRUE;
        }
        else
        {
            CcspTraceError(("syscfg_get failed for UPnP Refactor\n"));
        }
    }
  return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL
        UPnPRefactor_SetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL                        bValue
            )


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
UPnPRefactor_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{

  if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        char buf[8] = {'\0'};
        snprintf(buf, sizeof(buf), "%s", bValue ? "true" : "false");
        if( syscfg_set(NULL, "Refactor", buf) != 0 )
        {
            CcspTraceError(("syscfg_set failed for UPnP Refactor \n"));
        }
        else
        {
            if( syscfg_commit() == 0 )
            {
                return TRUE;
            }
            else
            {
                 CcspTraceError(("syscfg_commit failed for UPnP Refactor \n"));
            }
        }

    }
  return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL
        Telemetry_GetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL*                       pBool
            )



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
Telemetry_GetParamBoolValue ( ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool) {

    if( AnscEqualString(ParamName, "Enable", TRUE)) {
        char value[8] = {'\0'};
        if( syscfg_get(NULL, "T2Enable", value, sizeof(value)) == 0 ) {
            if( value != NULL ) {
                 if (strcmp(value, "true") == 0)
                     *pBool = TRUE;
                 else
                     *pBool = FALSE;
            }
            return TRUE;
        } else {
            CcspTraceError(("syscfg_get failed for MessageBusSource\n"));
        }
    }
    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL
        Telemetry_SetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL                        bValue
            )


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
Telemetry_SetParamBoolValue (ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue) {

    if (AnscEqualString(ParamName, "Enable", TRUE)) {
        char buf[8] = { '\0' };
        char versionBuf[8] = { '\0' };
        snprintf(buf, sizeof(buf), "%s", bValue ? "true" : "false");
        snprintf(versionBuf, sizeof(buf), "%s", bValue ? "2" : "1");
        if (syscfg_set(NULL, "T2Enable", buf) != 0) {
            CcspTraceError(("syscfg_set failed for Telemetry.Enable\n"));
        } else {
            if (syscfg_commit( ) == 0) {

                if (syscfg_set(NULL, "T2Version", versionBuf) != 0) {
                    CcspTraceError(("syscfg_set failed\n"));

                } else {
                    if (syscfg_commit( ) != 0) {
                        CcspTraceError(("syscfg_commit failed\n"));
                    }
                    return TRUE;
                }
                return TRUE;
            } else {
                CcspTraceError(("syscfg_commit failed for Telemetry.Enable\n"));
            }
        }

    }
    return FALSE;
}



/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        TelemetryEndpoint_GetParamStringValue
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
Telemetry_GetParamStringValue (ANSC_HANDLE hInsContext, char* ParamName, char* pValue,
                               ULONG* pUlSize) {

    /* Required for xPC sync */
    if (AnscEqualString(ParamName, "ConfigURL", TRUE)) {
        /* collect value */
        char buf[128] = {'\0'};
        syscfg_get(NULL, "T2ConfigURL", buf, sizeof(buf));

        if (buf != NULL) {
            AnscCopyString(pValue, buf);
            return 0;
        }
        return -1;
    }

    if (AnscEqualString(ParamName, "Version", TRUE)) {
        /* collect value */
        char buf[5] = {'\0'};
        syscfg_get(NULL, "T2Version", buf, sizeof(buf));

        if (buf != NULL) {
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
        Telemetry_SetParamStringValue
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
Telemetry_SetParamStringValue (ANSC_HANDLE hInsContext, char* ParamName, char* pString) {

    if (AnscEqualString(ParamName, "ConfigURL", TRUE)) {
        if (syscfg_set(NULL, "T2ConfigURL", pString) != 0) {
            CcspTraceError(("syscfg_set failed\n"));

        } else {
            if (syscfg_commit( ) != 0) {
                CcspTraceError(("syscfg_commit failed\n"));
            }
            return TRUE;
        }
    }

    if (AnscEqualString(ParamName, "Version", TRUE)) {
        if (syscfg_set(NULL, "T2Version", pString) != 0) {
            CcspTraceError(("syscfg_set failed\n"));
        } else {
            if (syscfg_commit( ) != 0) {
                CcspTraceError(("syscfg_commit failed\n"));
            }
            return TRUE;
        }
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}
