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

// for PSM access - RDKB-EMU
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
            /* CID: 64133 Array compared against 0*/
           if( (retValue != -1) )
	   {
          	 *puLong = atoi(buff);
           }

	   fclose(fp);	
           return TRUE;
    }
    
    if( AnscEqualString(ParamName, "BootTime", TRUE))
    {
        /* collect value */
       	*puLong = CosaDmlDiGetBootTime(NULL);
        return TRUE;
    }
    
    if( AnscEqualString(ParamName, "FactoryResetCount", TRUE))
    {
        /* collect value */
#if 0//LNT_EMU
        CosaDmlDiGetFactoryResetCount(NULL,puLong);
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
        CosaDmlDiGetSoftwareVersion(NULL,pValue,pulSize);
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
#if 0//LNT_EMU
        CosaDmlDiGetHardware_MemUsed(NULL, pValue,pulSize);
#endif
        return 0;
    }

    if( AnscEqualString(ParamName, "Hardware_MemFree", TRUE))
    {
#if 0//LNT_EMU
        CosaDmlDiGetHardware_MemFree(NULL, pValue,pulSize);
#endif
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

    if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_LastRebootReason", TRUE))
    {
	char *param_value = NULL;
        PSM_Get_Record_Value2(bus_handle,g_Subsystem,"dmsb.DeviceInfo.X_RDKCENTRAL-COM_LastRebootReason", NULL, &param_value);
        if( param_value != NULL)
                AnscCopyString(pValue,  param_value);
        else
               AnscCopyString(pValue,"unknown");
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
#if 0//LNT_EMU
        syscfg_get( NULL, "ems_server_url", buf, sizeof(buf));
    	if( buf != NULL )
    		{
    		    AnscCopyString(pValue,  buf);
    		    
    		}
		else
			{
			
			}
		return 0;
#endif
    }

	/* Changes for EMS end here */
	
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
#if 0//LNT_EMU
        CosaDmlDiClearResetCount(NULL,bValue);
#endif
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
#if 0//LNT_EMU
	if (syscfg_set(NULL, "redirection_url", pString) != 0) {
             AnscTraceWarning(("syscfg_set failed\n"));
          } else {
	       if (syscfg_commit() != 0) {
                    AnscTraceWarning(("syscfg_commit failed\n"));
                    }
		char url[150];	
		snprintf(url,sizeof(url),"/etc/whitelist.sh %s",pString);
		system(url);
		AnscCopyString(pMyObject->WebURL, pString);
		CcspTraceWarning(("CaptivePortal:Cloud URL is changed, new URL is %s ...\n",pMyObject->WebURL));
             }
#endif
	return TRUE;

    }
#endif
   if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_UI_ACCESS", TRUE))
   {

         if (AnscEqualString(pString, "ui_access", TRUE))
         {
		 CcspTraceInfo(("Local UI Access : RDKB_LOCAL_UI_ACCESS\n"));
         }
         else if (AnscEqualString(pString, "ui_success", TRUE))
         {
		 CcspTraceInfo(("Local UI Access : RDKB_LOCAL_UI_SUCCESS\n"));
         }
         else if(AnscEqualString(pString, "ui_failed", TRUE))
         {
         	CcspTraceInfo(("Local UI Access : RDKB_LOCAL_UI_FAILED\n"));
         }
	 else if (AnscEqualString(pString, "reboot_device", TRUE))
         {
                CcspTraceInfo(("RDKB_REBOOT : RebootDevice triggered from GUI\n"));
	 }
         else if(AnscEqualString(pString, "factory_reset", TRUE))
         {
                
		CcspTraceInfo(("RDKB_REBOOT : Reboot Device triggered through Factory reset from GUI\n"));
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
#if 0//LNT_EMU
	if (syscfg_set(NULL, "ems_server_url", pString) != 0) {
             AnscTraceWarning(("syscfg_set failed\n"));
          } else {
	       if (syscfg_commit() != 0) {
                    AnscTraceWarning(("syscfg_commit failed\n"));
                    }
		char ems_url[150];	
		snprintf(ems_url,sizeof(ems_url),"/etc/whitelist.sh %s",pString);
		system(ems_url);
		AnscCopyString(pMyObject->EMS_ServerURL, pString);
             }
#endif
	return TRUE;

    }
    if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_LastRebootReason", TRUE))
    {
	    PSM_Set_Record_Value2(bus_handle,g_Subsystem,"dmsb.DeviceInfo.X_RDKCENTRAL-COM_LastRebootReason", ccsp_string,pString);
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
    /*TODO: 78739 Out-of-bounds access - pulsize updated as 255 in hal layer*/
    CosaDmlDiGetProvisioningCode(NULL,pMyObject->ProvisioningCode, &pulSize);
    
    return 0;
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
    /* check the parameter name and return the corresponding value */
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
#if 0//LNT_EMU
        /* collect value */
	COSADmlUploadLogsStatus(NULL, pValue,pUlSize);
#endif
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
#if 0//LNT_EMU
	bReturnValue =
        	COSADmlUploadLogsNow
            	(
                	(ANSC_HANDLE)NULL, 
			bValue
            	);
#endif

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
        Webpa_GetParamBoolValue
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
Webpa_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
   char pchar[128];
   if (AnscEqualString(ParamName, "Enable", TRUE))
    {
#if 0//LNT_EMU
		if(ANSC_STATUS_SUCCESS != CosaDmlDiGetWebPACfg(ParamName,pchar))
		{
			return FALSE;
		}
#endif
		if(!strncmp(pchar,"true",strlen("true")))
		{
			*pBool = TRUE;
		}
		else
		{
			*pBool = FALSE;
		}
      return TRUE;
    }
	// CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
	printf("Unsupported parameter '%s'\n", ParamName);
    return FALSE;
	
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Webpa_GetParamIntValue
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
Webpa_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
	char pchar[256]; 	
#if 0
    /* check the parameter name and return the corresponding value */
	if( AnscEqualString(ParamName, "ServerPort", TRUE))
    {
        if(ANSC_STATUS_SUCCESS != CosaDmlDiGetWebPACfg(ParamName,pchar))
		{
			return FALSE;
		}
		*pInt = _ansc_atoi(pchar);
		return TRUE;
    }
	if (AnscEqualString(ParamName, "RetryIntervalInSec", TRUE))
    {
        if(ANSC_STATUS_SUCCESS != CosaDmlDiGetWebPACfg(ParamName,pchar))
		{
			return FALSE;
		}
		*pInt = _ansc_atoi(pchar);
		return TRUE;
    }
	if (AnscEqualString(ParamName, "MaxPingWaitTimeInSec", TRUE))
    {
		if(ANSC_STATUS_SUCCESS != CosaDmlDiGetWebPACfg(ParamName,pchar))
		{
			return FALSE;
		}
		*pInt = _ansc_atoi(pchar);
		return TRUE;
    }
#endif	
    /*CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
	
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Webpa_GetParamUlongValue
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
Webpa_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    /* check the parameter name and return the corresponding value */
    /* Required for xPC sync */
	if( AnscEqualString(ParamName, "X_COMCAST-COM_CMC", TRUE))
    {
        /* collect value */
		char buf[8];
#if 0
		syscfg_get( NULL, "X_COMCAST-COM_CMC", buf, sizeof(buf));
    	if( buf != NULL )
    		{
    		    *puLong = atoi(buf);
				return TRUE;
    		}
#endif
		*puLong = 0;
        return FALSE;
    }
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Webpa_GetParamStringValue
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
Webpa_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{

	/* Required for xPC sync */
	if( AnscEqualString(ParamName, "X_COMCAST-COM_CID", TRUE))
    	{
        	/* collect value */
		char buf[64];
#if 0//LNT_EMU
		syscfg_get( NULL, "X_COMCAST-COM_CID", buf, sizeof(buf));

    		if( buf != NULL )
    		{
			AnscCopyString(pValue, buf);
			return 0;
		}
#endif
		return -1;
    	}
	if( AnscEqualString(ParamName, "X_COMCAST-COM_SyncProtocolVersion", TRUE))
    {
        /* collect value */
		char buf[5];
#if 0//LNT_EMU
		syscfg_get( NULL, "X_COMCAST-COM_SyncProtocolVersion", buf, sizeof(buf));
    	if( buf != NULL )
    		{
    		    AnscCopyString(pValue,  buf);
				return 0;
    		}
#endif
		return -1;
    }
	
	if (AnscEqualString(ParamName, "ServerURL", TRUE))
	{	
#if 0//LNT_EMU
		if(ANSC_STATUS_SUCCESS != CosaDmlDiGetWebPACfg(ParamName,pValue))
		{
			printf("pValue get FAIL\n");
			return 1;
		}
#endif
		return 0;
	}
	
    if (AnscEqualString(ParamName, "DeviceNetworkInterface", TRUE))
    {
#if 0//LNT_EMU
        if(ANSC_STATUS_SUCCESS != CosaDmlDiGetWebPACfg(ParamName,pValue))
		{
			return 1;
		}
		return 0;
#endif
    }
	
	/* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
  
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Webpa_SetParamBoolValue
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
Webpa_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
	char pchar[128];
	char cValue[128];
    if (AnscEqualString(ParamName, "Enable", TRUE))
    {
		if(bValue)
		{
			AnscCopyString(cValue,"true");
		}
		else
		{
			AnscCopyString(cValue,"false");
		}
#if 0
	   if ( CosaDmlDiGetWebPACfg(ParamName, pchar) == ANSC_STATUS_SUCCESS )
		{
			int ret = strncmp(pchar,cValue,4);
			printf("ret : %d\n",ret);
				if(ret < 0)
				{
					if(ANSC_STATUS_SUCCESS != CosaDmlDiSetWebPACfg(ParamName,cValue))
					{
						return FALSE;
					}
					printf("***checking pwd\n");
					system("pwd");					
					char path[200];
					snprintf(path, 200, "/usr/bin/");
					chdir(path);
					system("pwd");
					printf("***Starting webpa\n");
					AnscCopyString(pchar,"./webpa -subsys eRT. &");
					system(pchar);
					system("pwd");
					AnscCopyString(pchar,"-");
					printf("***%s\n",pchar);
					chdir(pchar);
					system("pwd");
					return TRUE;							
				}
				else if(ret > 0)
				{
					if(ANSC_STATUS_SUCCESS != CosaDmlDiSetWebPACfg(ParamName,cValue))
					{
						return FALSE;
					}
					//memset(pchar,0,128);
					system("pwd");
					system("ls -l");
					AnscCopyString(pchar,"killall webpa");
					system(pchar);
					printf("***%s\n",pchar);
					system("pwd");
					return TRUE;
				}
				else			
				{
					printf("Already set, do nothing\n");
					return TRUE;
				}
		}
#endif
		return FALSE;
	}
	/* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}
	
    
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Webpa_SetParamIntValue
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
Webpa_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
	/* check the parameter name and set the corresponding value */
#if 0
	if( AnscEqualString(ParamName, "ServerPort", TRUE))
    {
		char pchar[256];

		sprintf(pchar,"%d",iValue);

		if(ANSC_STATUS_SUCCESS != CosaDmlDiSetWebPACfg(ParamName,pchar))
		{
			return FALSE;
		}
		return TRUE;
	}
    if( AnscEqualString(ParamName, "RetryIntervalInSec", TRUE))
    {
        char pchar[256];

		sprintf(pchar,"%d",iValue);

		if(ANSC_STATUS_SUCCESS != CosaDmlDiSetWebPACfg(ParamName,pchar))
		{
			return FALSE;
		}
		return TRUE;
    }
	if( AnscEqualString(ParamName, "MaxPingWaitTimeInSec", TRUE))
    {
        char pchar[256];

		sprintf(pchar,"%d",iValue);	

		if(ANSC_STATUS_SUCCESS != CosaDmlDiSetWebPACfg(ParamName,pchar))
		{
			return FALSE;
		}
		return TRUE;
    }
	
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
#endif
    return FALSE;
	
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Webpa_SetParamUlongValue
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
Webpa_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
	
   /* check the parameter name and set the corresponding value */
		/* Required for xPC sync */
	if( AnscEqualString(ParamName, "X_COMCAST-COM_CMC", TRUE))
    {
        /* collect value */
		char buf[8];

		snprintf(buf,sizeof(buf),"%d",uValue);
#if 0//LNT_EMU
		if (syscfg_set(NULL, "X_COMCAST-COM_CMC", buf) != 0) 
		{
			AnscTraceWarning(("syscfg_set failed\n"));
		}
		else 
		{
			if (syscfg_commit() != 0) 
			{
				AnscTraceWarning(("syscfg_commit failed\n"));
			}

			return TRUE;
		}
#endif
    }
	
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Webpa_SetParamStringValue
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
Webpa_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{

	/* Required for xPC sync */
	if( AnscEqualString(ParamName, "X_COMCAST-COM_CID", TRUE))
    	{
#if 0//LNT_EMU
		if (syscfg_set(NULL, "X_COMCAST-COM_CID", pString) != 0) 
		{
			AnscTraceWarning(("syscfg_set failed\n"));
		}
		else 
		{
			if (syscfg_commit() != 0) 
			{
				AnscTraceWarning(("syscfg_commit failed\n"));
			}

			return TRUE;
		}
#endif
    	}
#if 0
	if( AnscEqualString(ParamName, "X_COMCAST-COM_SyncProtocolVersion", TRUE))
    {
	if (syscfg_set(NULL, "X_COMCAST-COM_SyncProtocolVersion", pString) != 0) {
             AnscTraceWarning(("syscfg_set failed\n"));
          } else 
		  {
	       if (syscfg_commit() != 0)
				{
                    AnscTraceWarning(("syscfg_commit failed\n"));
                }
	       }

	return TRUE;

    }
    if (AnscEqualString(ParamName, "ServerURL", TRUE))
    {	
		if(ANSC_STATUS_SUCCESS != CosaDmlDiSetWebPACfg(ParamName,pString))
		{
			return FALSE;
		}
		return TRUE;		
    }
    
    if (AnscEqualString(ParamName, "DeviceNetworkInterface", TRUE))
    {
        if(ANSC_STATUS_SUCCESS != CosaDmlDiSetWebPACfg(ParamName,pString))
		{
			return FALSE;
		}
        return TRUE;			
    }
	/* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
#endif
	return FALSE;
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
    if( AnscEqualString(ParamName, "xOpsDMUploadLogsNow", TRUE))
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
    BOOL                     bReturnValue;

    if( AnscEqualString(ParamName, "xOpsDMUploadLogsNow", TRUE))
    {
#if 0//LNT_EMU
	bReturnValue =
        	COSADmlUploadLogsNow
            	(
                	(ANSC_HANDLE)NULL, 
			bValue
            	);
#endif

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
#if 0//LNT_EMU
        /* collect value */
	COSADmlUploadLogsStatus(NULL, pValue,pUlSize);
#endif
        return 0;
    }
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
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

        BOOL
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

BOOL
ReverseSSH_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    BOOL bReturnValue = FALSE;
    char* activeStr = "ACTIVE";
    char* inActiveStr = "INACTIVE";

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "xOpsReverseSshArgs", TRUE))
    {
        bReturnValue = getXOpsReverseSshArgs(NULL, pValue,pulSize);
        return bReturnValue;
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
        bReturnValue = TRUE;
        return bReturnValue;
    }

    CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return bReturnValue;
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
    ANSC_STATUS                                         retValue  = ANSC_STATUS_FAILURE;
    char PartnerID[PARTNER_ID_LEN] = {0};
    ULONG size = PARTNER_ID_LEN - 1;
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
                                memset( pMyObject->PartnerID, 0, sizeof( pMyObject->PartnerID ));
                                AnscCopyString( pMyObject->PartnerID, pString );

                        return TRUE;
    }
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
    ANSC_STATUS                                         retValue  = ANSC_STATUS_FAILURE;

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
                ANSC_HANDLE                             hInsContext,
                char*                                           ParamName,
                char*                                           pValue,
                ULONG*                                          pulSize
        )
{

        PCOSA_DATAMODEL_DEVICEINFO              pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
        PCOSA_DATAMODEL_RDKB_UIBRANDING pBindObj =      & pMyObject->UiBrand;

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
                ANSC_HANDLE                             hInsContext,
                char*                                           ParamName,
                char*                                           pValue,
                ULONG*                                          pulSize
        )
{
        PCOSA_DATAMODEL_DEVICEINFO              pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
        PCOSA_DATAMODEL_RDKB_UIBRANDING pBindObj =      & pMyObject->UiBrand;

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
    PCOSA_DATAMODEL_RDKB_UIBRANDING     pBindObj =      & pMyObject->UiBrand;

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
                ANSC_HANDLE                             hInsContext,
                char*                                           ParamName,
                char*                                           pValue,
                ULONG*                                          pulSize
        )
{
        PCOSA_DATAMODEL_DEVICEINFO              pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
        PCOSA_DATAMODEL_RDKB_UIBRANDING pBindObj =      & pMyObject->UiBrand;

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
    PCOSA_DATAMODEL_RDKB_UIBRANDING     pBindObj =      & pMyObject->UiBrand;

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
                ANSC_HANDLE                             hInsContext,
                char*                                           ParamName,
                char*                                           pValue,
                ULONG*                                          pulSize
        )
{
        PCOSA_DATAMODEL_DEVICEINFO              pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
        PCOSA_DATAMODEL_RDKB_UIBRANDING pBindObj =      & pMyObject->UiBrand;

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
    PCOSA_DATAMODEL_RDKB_UIBRANDING     pBindObj =      & pMyObject->UiBrand;

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
        PCOSA_DATAMODEL_DEVICEINFO              pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
        PCOSA_DATAMODEL_RDKB_UIBRANDING pBindObj =      & pMyObject->UiBrand;

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
    PCOSA_DATAMODEL_RDKB_UIBRANDING     pBindObj =      & pMyObject->UiBrand;

   char PartnerID[PARTNER_ID_LEN] = {0};
   char *value = ( bValue ==TRUE ) ?  "true" : "false";


   //if((CCSP_SUCCESS == getPartnerId(PartnerID) ) && ( PartnerID[ 0 ] != '\0'))
   //{
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
     //   }

    return FALSE;
}

ULONG
WiFiPersonalization_GetParamStringValue

        (
                ANSC_HANDLE                             hInsContext,
                char*                                           ParamName,
                char*                                           pValue,
                ULONG*                                          pulSize
        )
{

        PCOSA_DATAMODEL_DEVICEINFO              pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
        PCOSA_DATAMODEL_RDKB_UIBRANDING pBindObj =      & pMyObject->UiBrand;

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
    PCOSA_DATAMODEL_RDKB_UIBRANDING     pBindObj =      & pMyObject->UiBrand;

        char PartnerID[PARTNER_ID_LEN] = {0};

//   if((CCSP_SUCCESS == getPartnerId(PartnerID) ) && ( PartnerID[ 0 ] != '\0') )
  // {
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

  // }
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
                ANSC_HANDLE                             hInsContext,
                char*                                           ParamName,
                char*                                           pValue,
                ULONG*                                          pulSize
        )
{

        PCOSA_DATAMODEL_DEVICEINFO              pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
        PCOSA_DATAMODEL_RDKB_UIBRANDING pBindObj =      & pMyObject->UiBrand;

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
        PCOSA_DATAMODEL_DEVICEINFO              pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDeviceInfo;
        PCOSA_DATAMODEL_RDKB_UIBRANDING pBindObj =      & pMyObject->UiBrand;

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


/**
 *  RFC Features SecureWebUI
*/

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SecureWebUI_GetParamBoolValue
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
SecureWebUI_GetParamBoolValue

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
        /*CID: 68633 Array compared against 0*/
        if(!syscfg_get( NULL, "SecureWebUI_Enable", buf, sizeof(buf)))
        {
            if (strcmp(buf, "true") == 0)
                *pBool = TRUE;
            else
                *pBool = FALSE;
           return TRUE;
        } else {
          return FALSE;
        }
    }

    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SecureWebUI_SetParamBoolValue
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
                The buffer with updated value

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
SecureWebUI_SetParamBoolValue

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
            if (syscfg_set(NULL, "SecureWebUI_Enable", "true") != 0) {
                AnscTraceWarning(("syscfg_set failed\n"));
            } else {
                if (syscfg_commit() != 0) {
                    AnscTraceWarning(("syscfg_commit failed\n"));
                }
            }
        }
        else
        {
            if (syscfg_set(NULL, "SecureWebUI_Enable", "false") != 0) {
                AnscTraceWarning(("syscfg_set failed\n"));
            } else {
                if (syscfg_commit() != 0) {
                    AnscTraceWarning(("syscfg_commit failed\n"));  
                }
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
        SecureWebUI_GetParamStringValue
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


    return:     0 if succeeded;
                -1 if not supported.

**********************************************************************/
ULONG
    SecureWebUI_GetParamStringValue
(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 char*                       pValue,
 ULONG*                      pUlSize
 )
{

    /* Required for xPC sync */
    if( AnscEqualString(ParamName, "LocalFqdn", TRUE))
    {
        /* collect value */
        char buf[64];
        /*CID: 59203 Array compared against 0*/
        /*CID: 69469 Logically dead code*/
        if(!syscfg_get( NULL, "SecureWebUI_LocalFqdn", buf, sizeof(buf)))
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
        SecureWebUI_SetParamBoolValue
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
    SecureWebUI_SetParamStringValue
(
 ANSC_HANDLE                 hInsContext,
 char*                       ParamName,
 char*                       pString
 )
{

    if( AnscEqualString(ParamName, "LocalFqdn", TRUE))
    {
        if (syscfg_set(NULL, "SecureWebUI_LocalFqdn", pString) != 0)
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


