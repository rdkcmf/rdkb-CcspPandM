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

/**************************************************************************

    module: cosa_lanmanagement_dml.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

    -------------------------------------------------------------------

    environment:

        platform independent

    -------------------------------------------------------------------

    author:


    -------------------------------------------------------------------

    revision:

        05/09/2019    initial revision.

**************************************************************************/

/*************************************************************************
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

    X_RDK-Central_COM_DeviceControl.

    *  LanManagement_GetParamBoolValue
    *  LanManagement_GetParamIntValue
    *  LanManagement_GetParamUlongValue
    *  LanManagement_GetParamStringValue
    *  LanManagement_SetParamBoolValue
    *  LanManagement_SetParamIntValue
    *  LanManagement_SetParamUlongValue
    *  LanManagement_SetParamStringValue
    *  LanManagement_Validate
    *  LanManagement_Commit
    *  LanManagement_Rollback

***********************************************************************/
#if defined(CUSTOM_ULA)
#include "cosa_lanmanagement_dml.h"
#include "cosa_drg_common.h"
#include "safec_lib_common.h"

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        LanManagement_GetParamBoolValue
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
LanManagement_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_LANMANAGEMENT  pMyObject = (PCOSA_DATAMODEL_LANMANAGEMENT)g_pCosaBEManager->hLanMngm;

    if (strcmp(ParamName, "LanIpv6UlaEnable") == 0) {
        *pBool = pMyObject->LanMngmCfg.LanIpv6UlaEnable;
        return TRUE;
    }

    if (strcmp(ParamName, "LanIpv6Enable") == 0) {
        *pBool = pMyObject->LanMngmCfg.LanIpv6Enable;
        return TRUE;
    }

    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        LanManagement_GetParamStringValue
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
LanManagement_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_LANMANAGEMENT  pMyObject = (PCOSA_DATAMODEL_LANMANAGEMENT)g_pCosaBEManager->hLanMngm;
    errno_t                        rc        = -1;

    if (strcmp(ParamName, "LanIpv6Ula") == 0) {
        if ( AnscSizeOfString(pMyObject->LanMngmCfg.LanIpv6Ula) < *pulSize) {
            rc = strcpy_s(pValue, *pulSize,  pMyObject->LanMngmCfg.LanIpv6Ula);
            if (rc != EOK) {
                ERR_CHK(rc);
                return -1;
            }
            return 0;
        }
        else {
            *pulSize = AnscSizeOfString(pMyObject->LanMngmCfg.LanIpv6Ula)+1;
            return 1;
        }
    }

    if (strcmp(ParamName, "LanIpv6UlaPrefix") == 0) {
        if ( AnscSizeOfString(pMyObject->LanMngmCfg.LanIpv6UlaPrefix) < *pulSize) {
            rc = strcpy_s(pValue, *pulSize, pMyObject->LanMngmCfg.LanIpv6UlaPrefix);
            if (rc != EOK) {
                ERR_CHK(rc);
                return -1;
            }
            return 0;
        }
        else {
            *pulSize = AnscSizeOfString(pMyObject->LanMngmCfg.LanIpv6UlaPrefix)+1;
            return 1;
        }
    }

    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        LanManagement_SetParamBoolValue
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
LanManagement_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_LANMANAGEMENT  pMyObject = (PCOSA_DATAMODEL_LANMANAGEMENT)g_pCosaBEManager->hLanMngm;
    
    if (strcmp(ParamName, "LanIpv6UlaEnable") == 0) {
        pMyObject->LanMngmCfg.LanIpv6UlaEnable = bValue;
        return TRUE;
    }

    if (strcmp(ParamName, "LanIpv6Enable") == 0) {
        pMyObject->LanMngmCfg.LanIpv6Enable = bValue;
        return TRUE;
    }

    return FALSE;
}

BOOL
LanManagement_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_LANMANAGEMENT  pMyObject = (PCOSA_DATAMODEL_LANMANAGEMENT)g_pCosaBEManager->hLanMngm;
    errno_t                        rc        = -1;

    if (strcmp(ParamName, "LanIpv6Ula") == 0) {
        rc = STRCPY_S_NOCLOBBER(pMyObject->LanMngmCfg.LanIpv6Ula, sizeof(pMyObject->LanMngmCfg.LanIpv6Ula), pString);
        if (rc != EOK) {
           ERR_CHK(rc);
           return FALSE;
        }
        return TRUE;
    }

    if (strcmp(ParamName, "LanIpv6UlaPrefix") == 0) {
        rc = STRCPY_S_NOCLOBBER(pMyObject->LanMngmCfg.LanIpv6UlaPrefix, sizeof(pMyObject->LanMngmCfg.LanIpv6UlaPrefix), pString);
        if (rc != EOK) {
            ERR_CHK(rc);
            return FALSE;
        }
        return TRUE;
    }

    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        LanManagement_Validate
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
LanManagement_Validate
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
        LanManagement_Commit
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
LanManagement_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
#ifdef _HUB4_PRODUCT_REQ_
    PCOSA_DATAMODEL_LANMANAGEMENT  pMyObject = (PCOSA_DATAMODEL_LANMANAGEMENT)g_pCosaBEManager->hLanMngm;
    CosaDmlLanManagementSetCfg(NULL, &pMyObject->LanMngmCfg);
#endif
    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        LanManagement_Rollback
            (
                ANSC_HANDLE                 hInsContext,
                char*                       pReturnParamName,
                ULONG*                      puLength
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE if there's no validation.

**********************************************************************/
ULONG
LanManagement_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
#ifdef _HUB4_PRODUCT_REQ_
    PCOSA_DATAMODEL_LANMANAGEMENT  pMyObject = (PCOSA_DATAMODEL_LANMANAGEMENT)g_pCosaBEManager->hLanMngm;
    CosaDmlLanManagementGetCfg(NULL, &pMyObject->LanMngmCfg);
#endif
    return TRUE;
}
#endif
