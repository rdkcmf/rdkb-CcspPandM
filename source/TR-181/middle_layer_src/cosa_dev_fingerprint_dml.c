/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2016 RDK Management
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
   
#include "cosa_dev_fingerprint_dml.h"
#include "cosa_dev_fingerprint_internal.h"
#include "ansc_platform.h"
#include "syslog.h"
#include "ccsp_trace.h"


extern ANSC_STATUS CosaAdvSecInit(ANSC_HANDLE hThisObject);
extern ANSC_STATUS CosaAdvSecDeInit(ANSC_HANDLE hThisObject);
extern ANSC_STATUS CosaGetSysCfgString(char* setting, char *value, PULONG pulSize);
extern ANSC_STATUS CosaSetSysCfgString(char* setting, char *pValue);

static char *g_DeviceFingerPrintEndpointURL = "Advsecurity_EndpointURL";

int urlStartsWith(const char *haystack, const char *needle)
{
   if(strncmp(haystack, needle, strlen(needle)) == 0) 
       return 0;
   return 1;
}

ANSC_STATUS isValidUrl( char *inputparam )
{
    ANSC_STATUS returnStatus = ANSC_STATUS_SUCCESS;

    if(urlStartsWith(inputparam, "https://"))
    {
        returnStatus = ANSC_STATUS_FAILURE;
    }
    if(strstr(inputparam,";")) // check for possible command injection 
    {
        returnStatus = ANSC_STATUS_FAILURE;
    }
    else if(strstr(inputparam,"&"))
    {
        returnStatus = ANSC_STATUS_FAILURE;
    }
    else if(strstr(inputparam,"|"))
    {
        returnStatus = ANSC_STATUS_FAILURE;
    }
    else if(strstr(inputparam,"'"))
        returnStatus = ANSC_STATUS_FAILURE;

    return returnStatus;
}

/***********************************************************************

 APIs for Object:

	X_RDKCENTRAL-COM_DeviceFingerPrint.

    *  DeviceFingerPrint_GetParamBoolValue
    *  DeviceFingerPrint_SetParamBoolValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DeviceFingerPrint_GetParamBoolValue
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
DeviceFingerPrint_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* check the parameter name and return the corresponding value */
    PCOSA_DATAMODEL_FPAGENT       pMyObject     = (PCOSA_DATAMODEL_FPAGENT)g_pCosaBEManager->hDeviceFingerprint;
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        *pBool = pMyObject->bEnable;
        return TRUE;
    }

    CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        DeviceFingerPrint_SetParamBoolValue
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
DeviceFingerPrint_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{

    /* check the parameter name and return the corresponding value */
    PCOSA_DATAMODEL_FPAGENT       pMyObject     = (PCOSA_DATAMODEL_FPAGENT)g_pCosaBEManager->hDeviceFingerprint;
    ANSC_STATUS  returnStatus = ANSC_STATUS_SUCCESS;
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        if(bValue == pMyObject->bEnable)
                return TRUE;
        if( bValue )
                returnStatus = CosaAdvSecInit(pMyObject);
        else
                returnStatus = CosaAdvSecDeInit(pMyObject);

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            CcspTraceInfo(("%s EXIT Error\n", __FUNCTION__));
            return  returnStatus;
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
        DeviceFingerPrint_SetParamStringValue
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
DeviceFingerPrint_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    ANSC_STATUS returnStatus = ANSC_STATUS_SUCCESS;
    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "EndpointURL", TRUE))
    {
        if(ANSC_STATUS_SUCCESS == isValidUrl(pString))
        {
            CosaSetSysCfgString(g_DeviceFingerPrintEndpointURL, pString);
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
        DeviceFingerPrint_GetParamStringValue
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
DeviceFingerPrint_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "EndpointURL", TRUE))
    {
        /* collect value */
        CosaGetSysCfgString(g_DeviceFingerPrintEndpointURL, pValue, pUlSize);
        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

