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

    module: cosa_apis_x_cisco_com_mld_dml.c

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

        08/12/2011    initial revision.

**************************************************************************/

#include "ansc_platform.h"
#include "cosa_x_cisco_com_diagnostics_dml.h"
#include "safec_lib_common.h"

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

ULONG
Entry_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DIAGNOSTICS     pDiag    = (PCOSA_DATAMODEL_DIAGNOSTICS)g_pCosaBEManager->hDiag;

    return pDiag->ulDiagEntryNumber;
}

ANSC_HANDLE
Entry_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DIAGNOSTICS     pDiag    = (PCOSA_DATAMODEL_DIAGNOSTICS)g_pCosaBEManager->hDiag;

    *pInsNumber                     = nIndex + 1;

    if ( pDiag->pDiagEntry )
    {
        return &(pDiag->pDiagEntry[nIndex]);
    }
    
    return NULL;
}


static ULONG last_tick;
#define REFRESH_INTERVAL 30
#define TIME_NO_NEGATIVE(x) ((long)(x) < 0 ? 0 : (x))

BOOL
Entry_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if ( !last_tick ) 
    {
        last_tick = AnscGetTickInSeconds();

        return TRUE;
    }
    
    if ( last_tick >= TIME_NO_NEGATIVE(AnscGetTickInSeconds() - REFRESH_INTERVAL) )
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
        Group_Synchronize
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
Entry_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    static int first = 1;
    PCOSA_DATAMODEL_DIAGNOSTICS     pMyObject           = (PCOSA_DATAMODEL_DIAGNOSTICS)g_pCosaBEManager->hDiag;
    ULONG                           index               = 0;
    
    if (first) 
    {
        first =0;
        return 0;
    }
    
    if ( pMyObject->pDiagEntry )
    {
        for( index = 0; index < pMyObject->ulDiagEntryNumber; index++ )
        {
            if ( pMyObject->pDiagEntry[index].pMessage )
            {
                /* We must use free() here. */
                free(pMyObject->pDiagEntry[index].pMessage);
            }
        }

        AnscFreeMemory(pMyObject->pDiagEntry);
        pMyObject->pDiagEntry        = NULL;
        pMyObject->ulDiagEntryNumber = 0;
    }

    CosaDmlDiagnosticsGetEntry
        (
            (ANSC_HANDLE)NULL, 
            &pMyObject->ulDiagEntryNumber,
            &pMyObject->pDiagEntry
        );
    
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Group_GetParamBoolValue
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
Entry_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pBool);
    /* check the parameter name and return the corresponding value */
    
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Group_GetParamIntValue
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
Entry_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pInt);
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Group_GetParamUlongValue
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
Entry_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DML_DIAGNOSTICS_ENTRY            pDiagGroupEntry = (PCOSA_DML_DIAGNOSTICS_ENTRY)hInsContext;
    
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Level") == 0)
    {
        /* collect value */
        *puLong = pDiagGroupEntry->Level; 

        return TRUE;
    }
    /* check the parameter name and return the corresponding value */

    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Group_GetParamStringValue
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
                Usually size of 4095 will be used.
                If it's not big enough, put required size here and return 1;

    return:     0 if succeeded;
                1 if short of buffer size; (*pUlSize = required size)
                -1 if not supported.

**********************************************************************/
ULONG
Entry_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_DML_DIAGNOSTICS_ENTRY     pDiagEntry = (PCOSA_DML_DIAGNOSTICS_ENTRY)hInsContext;
    errno_t                         rc         = -1;

    /* check the parameter name and return the corresponding value */    
    if (strcmp(ParamName, "Time") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pDiagEntry->Time);
        if ( rc != EOK) {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
   
    if (strcmp(ParamName, "Tag") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pDiagEntry->Tag);
        if ( rc != EOK) {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }

    if (strcmp(ParamName, "Message") == 0)
    {
        /* collect value */
        if ( (_ansc_strlen(pDiagEntry->pMessage)+1) < *pUlSize )
        {
            rc = strcpy_s(pValue, *pUlSize, pDiagEntry->pMessage);
            if ( rc != EOK) {
                ERR_CHK(rc);
                return -1;
            }
        }
        else
        {
            *pUlSize = _ansc_strlen(pDiagEntry->pMessage) + 1;
            return 1;
        }
        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}


ULONG
Eventlog_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DIAGNOSTICS     pDiag    = (PCOSA_DATAMODEL_DIAGNOSTICS)g_pCosaBEManager->hDiag;

    return pDiag->ulDiagEventlogNumber;
}

ANSC_HANDLE
Eventlog_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DIAGNOSTICS     pDiag    = (PCOSA_DATAMODEL_DIAGNOSTICS)g_pCosaBEManager->hDiag;

    *pInsNumber                     = nIndex + 1;

    if ( pDiag->pDiagEventlog )
    {
        return &(pDiag->pDiagEventlog[nIndex]);
    }
    
    return NULL;
}

static ULONG eventlog_last_tick = 0;

BOOL
Eventlog_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    if ( !eventlog_last_tick ) 
    {
        eventlog_last_tick = AnscGetTickInSeconds();

        return TRUE;
    }
    
    if ( eventlog_last_tick >= TIME_NO_NEGATIVE(AnscGetTickInSeconds() - REFRESH_INTERVAL) )
    {
        return FALSE;
    }
    else 
    {
        eventlog_last_tick = AnscGetTickInSeconds();

        return TRUE;
    }
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Group_Synchronize
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
Eventlog_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DIAGNOSTICS     pMyObject           = (PCOSA_DATAMODEL_DIAGNOSTICS)g_pCosaBEManager->hDiag;
    ULONG                           index               = 0;
    static int first = 1;

    UNREFERENCED_PARAMETER(hInsContext);

    if (first) 
    {
        first =0;
        return 0;
    }
   
    if ( pMyObject->pDiagEventlog )
    {
        for( index = 0; index < pMyObject->ulDiagEventlogNumber; index++ )
        {
            if ( pMyObject->pDiagEventlog[index].pMessage )
            {
                /* We must use free() here. */
                free(pMyObject->pDiagEventlog[index].pMessage);
            }
        }

        AnscFreeMemory(pMyObject->pDiagEventlog);
        pMyObject->pDiagEventlog        = NULL;
        pMyObject->ulDiagEventlogNumber = 0;
    }

    CosaDmlDiagnosticsGetEventlog
        (
            (ANSC_HANDLE)NULL, 
            &pMyObject->ulDiagEventlogNumber,
            &pMyObject->pDiagEventlog
        );
    
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Group_GetParamBoolValue
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
Eventlog_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pBool);
    /* check the parameter name and return the corresponding value */
    
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Group_GetParamIntValue
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
Eventlog_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pInt);
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Group_GetParamUlongValue
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
Eventlog_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DML_DIAGNOSTICS_ENTRY  pDiagGroupEventlog = (PCOSA_DML_DIAGNOSTICS_ENTRY)hInsContext;
    
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Level") == 0)
    {
        /* collect value */
        *puLong = pDiagGroupEventlog->Level; 

        return TRUE;
    }
    /* check the parameter name and return the corresponding value */

    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Group_GetParamStringValue
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
                Usually size of 4095 will be used.
                If it's not big enough, put required size here and return 1;

    return:     0 if succeeded;
                1 if short of buffer size; (*pUlSize = required size)
                -1 if not supported.

**********************************************************************/
ULONG
Eventlog_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_DML_DIAGNOSTICS_ENTRY pDiagEventlog = (PCOSA_DML_DIAGNOSTICS_ENTRY)hInsContext;
    errno_t                     rc            = -1;

    /* check the parameter name and return the corresponding value */    
    if (strcmp(ParamName, "Time") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pDiagEventlog->Time);
        if ( rc != EOK) {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
   
    if (strcmp(ParamName, "Tag") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pDiagEventlog->Tag);
        if ( rc != EOK) {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }

    if (strcmp(ParamName, "Message") == 0)
    {
        /* collect value */
        if ( (_ansc_strlen(pDiagEventlog->pMessage)+1) < *pUlSize )
        {
            rc = strcpy_s(pValue, *pUlSize, pDiagEventlog->pMessage);
            if ( rc != EOK) {
                ERR_CHK(rc);
                return -1;
            }
        }
        else
        {
            *pUlSize = _ansc_strlen(pDiagEventlog->pMessage) + 1;
            return 1;
        }
        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

ULONG
X_CISCO_COM_Diagnostics_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    ULONG logSize = *pUlSize;
    if (strcmp(ParamName, "DumpAllSyslog") == 0)
    {
        if((ANSC_STATUS_FAILURE == CosaDmlDiagnosticsGetAllSyslog(pValue, &logSize)) != ANSC_STATUS_SUCCESS)
        {
            if(logSize > *pUlSize){
                *pUlSize = logSize;
                return 1;
            }
            else
                return -1;
        }
        return 0;
    }
 
    if (strcmp(ParamName, "DumpAllEventlog") == 0)
    {
        if((ANSC_STATUS_FAILURE == CosaDmlDiagnosticsGetAllEventlog(pValue, &logSize)) != ANSC_STATUS_SUCCESS)
        {
            if(logSize > *pUlSize){
                *pUlSize = logSize;
                return 1;
            }
            else
                return -1;
        }
        return 0;
    }
   /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}
