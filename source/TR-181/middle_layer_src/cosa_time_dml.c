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

    module: cosa_time_dml.c

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

#include "cosa_time_dml.h"

#include "ccsp_base_api.h"
#include "messagebus_interface_helper.h"

extern ULONG g_currentBsUpdate;
extern char * getRequestorString();
extern char * getTime();

ANSC_STATUS
CosaDmlTimeGetTimeOffset
    (
       ANSC_HANDLE                 hContext,
       char                       *pTimeOffset
    );

ANSC_STATUS
isValidInput
    (
        char                       *inputparam,
        char                       *wrapped_inputparam,
        int                         lengthof_inputparam,
        int                         sizeof_wrapped_inputparam
    );

#define BS_SOURCE_WEBPA_STR "webpa"
#define BS_SOURCE_RFC_STR "rfc"
#define  PARTNER_ID_LEN  64

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

    Time.

    *  Time_GetParamBoolValue
    *  Time_GetParamIntValue
    *  Time_GetParamUlongValue
    *  Time_GetParamStringValue
    *  Time_SetParamBoolValue
    *  Time_SetParamIntValue
    *  Time_SetParamUlongValue
    *  Time_SetParamStringValue
    *  Time_Validate
    *  Time_Commit
    *  Time_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Time_GetParamBoolValue
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
Time_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_TIME            pMyObject = (PCOSA_DATAMODEL_TIME)g_pCosaBEManager->hTime;
    
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* collect value */
        *pBool = pMyObject->TimeCfg.bEnabled;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "DaylightSaving", TRUE))
    {
        /* collect value */
        *pBool = pMyObject->TimeCfg.bDaylightSaving;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "UTC_Enable", TRUE))
    {
        /* collect value */
        *pBool = pMyObject->TimeCfg.bUTCEnabled;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Time_GetParamIntValue
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
Time_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_TIME            pMyObject = (PCOSA_DATAMODEL_TIME)g_pCosaBEManager->hTime;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "DaylightSavingOffset", TRUE))
    {
        /* collect value */
        *pInt = pMyObject->TimeCfg.DaylightSavingOffset;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Time_GetParamUlongValue
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
Time_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_TIME            pMyObject = (PCOSA_DATAMODEL_TIME)g_pCosaBEManager->hTime;
    
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Status", TRUE))
    {
        /* collect value */
        CosaDmlTimeGetState(NULL, &pMyObject->TimeStatus, &pMyObject->CurrLocalTime);
        *puLong = pMyObject->TimeStatus;
        return TRUE;
    }

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "CityIndex", TRUE))
    {
        /* collect value */
        *puLong = pMyObject->TimeCfg.cityIndex;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Time_GetParamStringValue
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
Time_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pUlSize);
    PCOSA_DATAMODEL_TIME            pMyObject = (PCOSA_DATAMODEL_TIME)g_pCosaBEManager->hTime;

    CosaDmlTimeGetCfg(NULL, &pMyObject->TimeCfg);
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "NTPServer1", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pMyObject->TimeCfg.NTPServer1.ActiveValue);
        return 0;
    }

    if( AnscEqualString(ParamName, "NTPServer2", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pMyObject->TimeCfg.NTPServer2.ActiveValue);
        return 0;
    }

    if( AnscEqualString(ParamName, "NTPServer3", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pMyObject->TimeCfg.NTPServer3.ActiveValue);
        return 0;
    }

    if( AnscEqualString(ParamName, "NTPServer4", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pMyObject->TimeCfg.NTPServer4.ActiveValue);
        return 0;
    }

    if( AnscEqualString(ParamName, "NTPServer5", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pMyObject->TimeCfg.NTPServer5.ActiveValue);
        return 0;
    }

    if( AnscEqualString(ParamName, "CurrentLocalTime", TRUE))
    {
        CosaDmlTimeGetLocalTime(NULL,pValue);
        return 0;
    }

    if( AnscEqualString(ParamName, "LocalTimeZone", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pMyObject->TimeCfg.LocalTimeZone);
        return 0;
    }

    if( AnscEqualString(ParamName, "TimeOffset", TRUE))
    {
        /* collect value */
        CosaDmlTimeGetTimeOffset(NULL,pValue);
	return 0;
    }
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Time_SetParamBoolValue
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
Time_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_TIME            pMyObject = (PCOSA_DATAMODEL_TIME)g_pCosaBEManager->hTime;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* save update to backup */
        pMyObject->TimeCfg.bEnabled = bValue;
        if ( pMyObject->TimeCfg.bEnabled )
	{
    		CcspTraceWarning(("Service_NTP : Enabling Network Time Sync \n")); 
	}
	else
	{
   		CcspTraceWarning(("Service_NTP : Disabling Network Time Sync \n")); 
	}
	
        return TRUE;
    }

    if( AnscEqualString(ParamName, "DaylightSaving", TRUE))
    {
        /* save update to backup */
        pMyObject->TimeCfg.bDaylightSaving = bValue;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Time_SetParamIntValue
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
Time_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and set the corresponding value */
    PCOSA_DATAMODEL_TIME            pMyObject = (PCOSA_DATAMODEL_TIME)g_pCosaBEManager->hTime;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "DaylightSavingOffset", TRUE))
    {
        /* save update to backup */
        pMyObject->TimeCfg.DaylightSavingOffset = iValue;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Time_SetParamUlongValue
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
Time_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and set the corresponding value */
    PCOSA_DATAMODEL_TIME            pMyObject = (PCOSA_DATAMODEL_TIME)g_pCosaBEManager->hTime;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "CityIndex", TRUE))
    {
        /* save update to backup */
        pMyObject->TimeCfg.cityIndex = uValue;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Time_SetParamStringValue
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
Time_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_TIME            pMyObject = (PCOSA_DATAMODEL_TIME)g_pCosaBEManager->hTime;
    ANSC_STATUS ret=ANSC_STATUS_FAILURE;

    char * requestorStr = getRequestorString();
    char * currentTime = getTime();

    IS_UPDATE_ALLOWED_IN_DM(ParamName, requestorStr);
    char PartnerID[PARTNER_ID_LEN] = {0};
    getPartnerId(PartnerID);

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "NTPServer1", TRUE))
    {
        IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pMyObject->TimeCfg.NTPServer1.UpdateSource);
	char wrapped_inputparam[64]={0};
	ret=isValidInput(pString,wrapped_inputparam, AnscSizeOfString(pString), sizeof( wrapped_inputparam ));
	if(ANSC_STATUS_SUCCESS != ret)
	    return FALSE;

        /* save update to backup */
        AnscCopyString(pMyObject->TimeCfg.NTPServer1.ActiveValue, wrapped_inputparam);
    	CcspTraceWarning(("Service_NTP : Setting NTPServer as %s \n",pMyObject->TimeCfg.NTPServer1.ActiveValue)); 

        AnscCopyString(pMyObject->TimeCfg.NTPServer1.UpdateSource, requestorStr);
        if( (PartnerID[ 0 ] != '\0') )
             UpdateJsonParam("Device.Time.NTPServer1",PartnerID, wrapped_inputparam, requestorStr, currentTime);
        return TRUE;
    }

    if( AnscEqualString(ParamName, "NTPServer2", TRUE))
    {
        IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pMyObject->TimeCfg.NTPServer2.UpdateSource);
	char wrapped_inputparam[64]={0};
	ret=isValidInput(pString,wrapped_inputparam, AnscSizeOfString(pString), sizeof( wrapped_inputparam ));
	if(ANSC_STATUS_SUCCESS != ret)
	    return FALSE;

        /* save update to backup */
        AnscCopyString(pMyObject->TimeCfg.NTPServer2.ActiveValue, wrapped_inputparam);
        AnscCopyString(pMyObject->TimeCfg.NTPServer2.UpdateSource, requestorStr);
        if( (PartnerID[ 0 ] != '\0') )
             UpdateJsonParam("Device.Time.NTPServer2",PartnerID, wrapped_inputparam, requestorStr, currentTime);
        return TRUE;
    }

    if( AnscEqualString(ParamName, "NTPServer3", TRUE))
    {
        IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pMyObject->TimeCfg.NTPServer3.UpdateSource);
	char wrapped_inputparam[64]={0};
	ret=isValidInput(pString,wrapped_inputparam, AnscSizeOfString(pString), sizeof( wrapped_inputparam ));
	if(ANSC_STATUS_SUCCESS != ret)
	    return FALSE;

        /* save update to backup */
        AnscCopyString(pMyObject->TimeCfg.NTPServer3.ActiveValue, wrapped_inputparam);
        AnscCopyString(pMyObject->TimeCfg.NTPServer3.UpdateSource, requestorStr);
        if( (PartnerID[ 0 ] != '\0') )
             UpdateJsonParam("Device.Time.NTPServer3",PartnerID, wrapped_inputparam, requestorStr, currentTime);
        return TRUE;
    }

    if( AnscEqualString(ParamName, "NTPServer4", TRUE))
    {
        IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pMyObject->TimeCfg.NTPServer4.UpdateSource);
	char wrapped_inputparam[64]={0};
	ret=isValidInput(pString,wrapped_inputparam, AnscSizeOfString(pString), sizeof( wrapped_inputparam ));
	if(ANSC_STATUS_SUCCESS != ret)
	    return FALSE;

        /* save update to backup */
        AnscCopyString(pMyObject->TimeCfg.NTPServer4.ActiveValue, wrapped_inputparam);
        AnscCopyString(pMyObject->TimeCfg.NTPServer4.UpdateSource, requestorStr);
        if( (PartnerID[ 0 ] != '\0') )
             UpdateJsonParam("Device.Time.NTPServer4",PartnerID, wrapped_inputparam, requestorStr, currentTime);
        return TRUE;
    }

    if( AnscEqualString(ParamName, "NTPServer5", TRUE))
    {
        IS_UPDATE_ALLOWED_IN_JSON(ParamName, requestorStr, pMyObject->TimeCfg.NTPServer5.UpdateSource);
	char wrapped_inputparam[64]={0};
	ret=isValidInput(pString,wrapped_inputparam, AnscSizeOfString(pString), sizeof( wrapped_inputparam ));
	if(ANSC_STATUS_SUCCESS != ret)
	    return FALSE;

        /* save update to backup */
        AnscCopyString(pMyObject->TimeCfg.NTPServer5.ActiveValue, wrapped_inputparam);
        AnscCopyString(pMyObject->TimeCfg.NTPServer5.UpdateSource, requestorStr);
        if( (PartnerID[ 0 ] != '\0') )
             UpdateJsonParam("Device.Time.NTPServer5",PartnerID, wrapped_inputparam, requestorStr, currentTime);
        return TRUE;
    }

    if( AnscEqualString(ParamName, "LocalTimeZone", TRUE))
    {
        /* save update to backup */
        AnscCopyString(pMyObject->TimeCfg.LocalTimeZone, pString);
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Time_Validate
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
Time_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pReturnParamName);
    UNREFERENCED_PARAMETER(puLength);
    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Time_Commit
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
Time_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_TIME            pMyObject = (PCOSA_DATAMODEL_TIME)g_pCosaBEManager->hTime;

    CosaDmlTimeSetCfg(NULL, &pMyObject->TimeCfg);
    
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Time_Rollback
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
Time_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_TIME            pMyObject = (PCOSA_DATAMODEL_TIME)g_pCosaBEManager->hTime;

    CosaDmlTimeGetCfg(NULL, &pMyObject->TimeCfg);
    
    return 0;
}

