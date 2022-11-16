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

    module: cosa_x_cisco_com_ddns_dml.c

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

#include "ansc_platform.h"
#include "cosa_x_cisco_com_ddns_dml.h"
#include "cosa_x_cisco_com_ddns_internal.h"
#include "dml_tr181_custom_cfg.h"
#include "ansc_string_util.h"
#include "safec_lib_common.h"

extern void * g_pDslhDmlAgent;

struct detail
{
    char word[ 256 ];
};

BOOL Service_IsDomainStringHaveRepeatedWord ( char* pStringDomain );

int Service_CheckRepeatString( struct detail stDetailArray[], const char unit[], int count, int* pIsHaveRepeatedWord );


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

    X_CISCO_COM_DDNS.

    *  X_CISCO_COM_DDNS_GetParamBoolValue
    *  X_CISCO_COM_DDNS_GetParamIntValue
    *  X_CISCO_COM_DDNS_GetParamUlongValue
    *  X_CISCO_COM_DDNS_GetParamStringValue
    *  X_CISCO_COM_DDNS_SetParamBoolValue
    *  X_CISCO_COM_DDNS_SetParamIntValue
    *  X_CISCO_COM_DDNS_SetParamUlongValue
    *  X_CISCO_COM_DDNS_SetParamStringValue
    *  X_CISCO_COM_DDNS_Validate
    *  X_CISCO_COM_DDNS_Commit
    *  X_CISCO_COM_DDNS_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_DDNS_GetParamBoolValue
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
X_CISCO_COM_DDNS_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* PCOSA_DATAMODEL_DDNS            pCosaDMDdns     = (PCOSA_DATAMODEL_DDNS)g_pCosaBEManager->hDdns;
    PCOSA_DML_DDNS_CFG              pDdnsCfg        = (PCOSA_DML_DDNS_CFG  )&pCosaDMDdns->DdnsConfig; */
    UNREFERENCED_PARAMETER(hInsContext);

#if !defined(DDNS_BROADBANDFORUM)

    COSA_DML_DDNS_CFG               DdnsCfg;

     _ansc_memset(&DdnsCfg, 0, sizeof(COSA_DML_DDNS_CFG));

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        CosaDmlDdnsGetConfig(NULL, &DdnsCfg);
        /* *pBool = pDdnsCfg->bEnabled; */
        *pBool = DdnsCfg.bEnabled; 

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
#endif
#if defined(DDNS_BROADBANDFORUM)
    return TRUE;
#endif
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_DDNS_GetParamIntValue
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
X_CISCO_COM_DDNS_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    /* check the parameter name and return the corresponding value */
#if !defined(DDNS_BROADBANDFORUM)

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pInt);

    return FALSE;
#endif
#if defined(DDNS_BROADBANDFORUM)
   return TRUE;
#endif

}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_DDNS_GetParamUlongValue
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
X_CISCO_COM_DDNS_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    /* check the parameter name and return the corresponding value */
#if !defined(DDNS_BROADBANDFORUM)

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(puLong);
    
    return FALSE;
#endif
#if defined(DDNS_BROADBANDFORUM)
   return TRUE;
#endif
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        X_CISCO_COM_DDNS_GetParamStringValue
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
X_CISCO_COM_DDNS_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    /* check the parameter name and return the corresponding value */
#if !defined(DDNS_BROADBANDFORUM)

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pValue);
    UNREFERENCED_PARAMETER(pUlSize);

    return -1;
#endif
#if defined(DDNS_BROADBANDFORUM)
    return 0;
#endif
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_DDNS_SetParamBoolValue
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
X_CISCO_COM_DDNS_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);

#if !defined(DDNS_BROADBANDFORUM)

    PCOSA_DATAMODEL_DDNS            pCosaDMDdns = (PCOSA_DATAMODEL_DDNS)g_pCosaBEManager->hDdns;
    PCOSA_DML_DDNS_CFG              pDdnsCfg    = (PCOSA_DML_DDNS_CFG  )&pCosaDMDdns->DdnsConfig;

    if ( !pDdnsCfg )
    {
        CcspTraceWarning(("X_CISCO_COM_DDNS_SetParamBoolValue -- pDdnsCfg is NULL\n"));

        return FALSE;
    }

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        pDdnsCfg->bEnabled = bValue;

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
#endif
#if defined(DDNS_BROADBANDFORUM)
    return TRUE;
#endif
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_DDNS_SetParamIntValue
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
X_CISCO_COM_DDNS_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
#if !defined(DDNS_BROADBANDFORUM)

    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(iValue);

    return FALSE;
#endif
#if defined(DDNS_BROADBANDFORUM)
   return TRUE;
#endif
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_DDNS_SetParamUlongValue
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
X_CISCO_COM_DDNS_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
#if !defined(DDNS_BROADBANDFORUM)

    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(uValue);

    return FALSE;
#endif
#if defined(DDNS_BROADBANDFORUM)
    return TRUE;
#endif
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_DDNS_SetParamStringValue
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
X_CISCO_COM_DDNS_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
#if !defined(DDNS_BROADBANDFORUM)

    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pString);

    return FALSE;
#endif
#if defined(DDNS_BROADBANDFORUM)
   return TRUE;
#endif
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_DDNS_Validate
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
X_CISCO_COM_DDNS_Validate
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
        X_CISCO_COM_DDNS_Commit
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
X_CISCO_COM_DDNS_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);

#if !defined(DDNS_BROADBANDFORUM)

    PCOSA_DATAMODEL_DDNS            pCosaDMDdns = (PCOSA_DATAMODEL_DDNS)g_pCosaBEManager->hDdns;
    PCOSA_DML_DDNS_CFG              pDdnsCfg    = (PCOSA_DML_DDNS_CFG  )&pCosaDMDdns->DdnsConfig;

    if ( !pDdnsCfg )
    {
        CcspTraceWarning(("X_CISCO_COM_DDNS_SetParamBoolValue -- pDdnsCfg is NULL\n"));

        return ANSC_STATUS_FAILURE;
    }

    return CosaDmlDdnsSetConfig(NULL, pDdnsCfg);
#endif
#if defined(DDNS_BROADBANDFORUM)
   return 0;
#endif
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        X_CISCO_COM_DDNS_Rollback
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
X_CISCO_COM_DDNS_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    return 0;
}

/***********************************************************************

 APIs for Object:

    X_CISCO_COM_DDNS.Service.{i}.

    *  Service_GetEntryCount
    *  Service_GetEntry
    *  Service_AddEntry
    *  Service_DelEntry
    *  Service_GetParamBoolValue
    *  Service_GetParamIntValue
    *  Service_GetParamUlongValue
    *  Service_GetParamStringValue
    *  Service_SetParamBoolValue
    *  Service_SetParamIntValue
    *  Service_SetParamUlongValue
    *  Service_SetParamStringValue
    *  Service_Validate
    *  Service_Commit
    *  Service_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Service_GetEntryCount
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
Service_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);

#if !defined(DDNS_BROADBANDFORUM)
    PCOSA_DATAMODEL_DDNS            pCosaDMDdns  = (PCOSA_DATAMODEL_DDNS)g_pCosaBEManager->hDdns;
    
    return AnscSListQueryDepth(&pCosaDMDdns->ContextHead);
#endif
#if defined(DDNS_BROADBANDFORUM)
    return 0;
#endif

}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Service_GetEntry
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
Service_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);

#if !defined(DDNS_BROADBANDFORUM)
    PCOSA_DATAMODEL_DDNS            pCosaDMDdns  = (PCOSA_DATAMODEL_DDNS     )g_pCosaBEManager->hDdns;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = (PSINGLE_LINK_ENTRY       )NULL;

    pSLinkEntry = AnscSListGetEntryByIndex(&pCosaDMDdns->ContextHead, nIndex);

    if ( pSLinkEntry )
    {
        pCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);

        *pInsNumber = pCosaContext->InstanceNumber;
    }

    return pCosaContext; /* return the handle */
#endif
#if defined(DDNS_BROADBANDFORUM)
   return 0;
#endif
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Service_AddEntry
            (
                ANSC_HANDLE                 hInsContext,
                ULONG*                      pInsNumber
            );

    description:

        This function is called to add a new entry.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ULONG*                      pInsNumber
                The output instance number;

    return:     The handle of new added entry.

**********************************************************************/
ANSC_HANDLE
Service_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )

{
    UNREFERENCED_PARAMETER(hInsContext);

#if !defined(DDNS_BROADBANDFORUM)

    PCOSA_DATAMODEL_DDNS            pCosaDMDdns  = (PCOSA_DATAMODEL_DDNS     )g_pCosaBEManager->hDdns;
    PSLIST_HEADER                   pListHead    = (PSLIST_HEADER            )&pCosaDMDdns->ContextHead;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_DDNS_SERVICE          pDdnsService = (PCOSA_DML_DDNS_SERVICE   )NULL;
    errno_t                         rc           = -1;

    pDdnsService = (PCOSA_DML_DDNS_SERVICE)AnscAllocateMemory(sizeof(COSA_DML_DDNS_SERVICE));

    if ( !pDdnsService )
    {
        return NULL;
    }

    rc = sprintf_s(pDdnsService->Alias, sizeof(pDdnsService->Alias),"DdnsService%lu", pCosaDMDdns->ulNextInstance);
    if(rc < EOK)
    {
      ERR_CHK(rc);
      AnscFreeMemory(pDdnsService);
      return NULL;
    }

    /* Update middle layer cache */
    if ( TRUE )
    {
        pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

        if ( !pCosaContext )
        {
            AnscFreeMemory(pDdnsService);

            return NULL;
        }

        pCosaContext->InstanceNumber = pDdnsService->InstanceNumber = pCosaDMDdns->ulNextInstance;

        pCosaDMDdns->ulNextInstance++;

        if ( pCosaDMDdns->ulNextInstance == 0 )
        {
            pCosaDMDdns->ulNextInstance = 1;
        }

        pCosaContext->hContext     = (ANSC_HANDLE)pDdnsService;
        pCosaContext->hParentTable = NULL;
        pCosaContext->bNew         = TRUE;

        CosaSListPushEntryByInsNum(pListHead, pCosaContext);

        CosaDdnsRegAddDdnsInfo((ANSC_HANDLE)pCosaDMDdns, (ANSC_HANDLE)pCosaContext);
    }

    *pInsNumber = pCosaContext->InstanceNumber;

    return pCosaContext; /* return the handle */
#endif
#if defined(DDNS_BROADBANDFORUM)
   return 0;
#endif
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Service_DelEntry
            (
                ANSC_HANDLE                 hInsContext,
                ANSC_HANDLE                 hInstance
            );

    description:

        This function is called to delete an exist entry.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ANSC_HANDLE                 hInstance
                The exist entry handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
Service_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    UNREFERENCED_PARAMETER(hInsContext);

#if !defined(DDNS_BROADBANDFORUM)

    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_DML_DDNS_SERVICE          pDdnsService  = (PCOSA_DML_DDNS_SERVICE   )pCosaContext->hContext;
    PCOSA_DATAMODEL_DDNS            pCosaDMDdns   = (PCOSA_DATAMODEL_DDNS     )g_pCosaBEManager->hDdns;
    PSLIST_HEADER                   pListHead     = (PSLIST_HEADER            )&pCosaDMDdns->ContextHead;

    if ( pCosaContext->bNew )
    {
        returnStatus = CosaDdnsRegDelDdnsInfo((ANSC_HANDLE)pCosaDMDdns, (ANSC_HANDLE)pCosaContext);
    }
    else
    {
        returnStatus = CosaDmlDdnsDelService(NULL, pDdnsService->InstanceNumber);

        if ( returnStatus == ANSC_STATUS_SUCCESS )
        {
            AnscSListPopEntryByLink(pListHead, &pCosaContext->Linkage);

            AnscFreeMemory(pDdnsService);
            AnscFreeMemory(pCosaContext);
        }
    }

    return returnStatus;
#endif
#if defined(DDNS_BROADBANDFORUM)
   return 0;
#endif
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Service_GetParamBoolValue
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
Service_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
#if !defined(DDNS_BROADBANDFORUM)

    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DDNS_SERVICE          pDdnsService = (PCOSA_DML_DDNS_SERVICE   )pCosaContext->hContext;
    COSA_DML_DDNS_SERVICE           DdnsService;

    _ansc_memset(&DdnsService, 0, sizeof(COSA_DML_DDNS_SERVICE));

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        CosaDmlDdnsGetInfo(NULL, pDdnsService->InstanceNumber, &DdnsService);
        /* *pBool = pDdnsService->bEnabled; */
        *pBool = DdnsService.bEnabled;

        return TRUE;
    }

    if (strcmp(ParamName, "Wildcard") == 0)
    {
        *pBool = pDdnsService->Wildcard;

        return TRUE;
    }

    if (strcmp(ParamName, "MXHostAsSecondary") == 0)
    {
        *pBool = pDdnsService->Backup_mx;

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
#endif
#if defined(DDNS_BROADBANDFORUM)
   return TRUE;
#endif

}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Service_GetParamIntValue
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
Service_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
#if !defined(DDNS_BROADBANDFORUM)

    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pInt);

    return FALSE;
#endif
#if defined(DDNS_BROADBANDFORUM)
  return TRUE;
#endif
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Service_GetParamUlongValue
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
Service_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
#if !defined(DDNS_BROADBANDFORUM)

    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DDNS_SERVICE          pDdnsService = (PCOSA_DML_DDNS_SERVICE   )pCosaContext->hContext;
    COSA_DML_DDNS_SERVICE           DdnsService;

    _ansc_memset(&DdnsService, 0, sizeof(COSA_DML_DDNS_SERVICE));
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "ConnectionState") == 0)
    {
        /* collect value */
        CosaDmlDdnsGetInfo(NULL, pDdnsService->InstanceNumber, &DdnsService);
        /* *puLong = pDdnsService->ConnectionState; */
        *puLong = DdnsService.ConnectionState; 

        return TRUE;
    }

    if (strcmp(ParamName, "ServiceName") == 0)
    {
        /* collect value */
        if (strcasecmp(pDdnsService->ServiceName, "dyndns") == 0)
        {
            *puLong = 1;
        }
        else if (strcasecmp(pDdnsService->ServiceName, "tzo") == 0)
        {
            *puLong = 2;
        }
		else if (strcasecmp(pDdnsService->ServiceName, "changeip") == 0)
        {
            *puLong = 3;
        }
		else if (strcasecmp(pDdnsService->ServiceName, "afraid") == 0)
        {
            *puLong = 4;
        }
        else
        {
            *puLong = 5;
        }

        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
#endif
#if defined(DDNS_BROADBANDFORUM)
   return TRUE;
#endif
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Service_GetParamStringValue
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
Service_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(pUlSize);
#if !defined(DDNS_BROADBANDFORUM)

    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DDNS_SERVICE          pDdnsService = (PCOSA_DML_DDNS_SERVICE   )pCosaContext->hContext;
    PUCHAR                          pLowerLayer      = NULL;
    errno_t                         rc           = -1;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "AssociatedConnection") == 0)
    {
        /* collect value */
        pLowerLayer = CosaUtilGetLowerLayers((PUCHAR)"Device.IP.Interface.", (PUCHAR)CFG_TR181_Ddns_IfName);
        if ( pLowerLayer != NULL )
        {
            rc = strcpy_s(pValue, *pUlSize, (char*)pLowerLayer);
            AnscFreeMemory(pLowerLayer);
            if ( rc != EOK) {
                ERR_CHK(rc);
                return -1;
            }
        }
        return 0;
    }

    if (strcmp(ParamName, "Alias") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pDdnsService->Alias);
        if ( rc != EOK) {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }

    if (strcmp(ParamName, "Domain") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pDdnsService->Domain);
        if ( rc != EOK) {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }

    if (strcmp(ParamName, "Password") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pDdnsService->Password);
        if ( rc != EOK) {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }

    if (strcmp(ParamName, "Username") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pDdnsService->Username);
        if ( rc != EOK) {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }

    if (strcmp(ParamName, "MXHostName") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pDdnsService->Mail_exch);
        if ( rc != EOK) {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
#endif
#if defined(DDNS_BROADBANDFORUM)
  return 0;
#endif
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Service_SetParamBoolValue
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
Service_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
#if !defined(DDNS_BROADBANDFORUM)

    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DDNS_SERVICE          pDdnsService = (PCOSA_DML_DDNS_SERVICE   )pCosaContext->hContext;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* save update to backup */
        pDdnsService->bEnabled = bValue;

        return TRUE;
    }

    if (strcmp(ParamName, "Wildcard") == 0)
    {
        pDdnsService->Wildcard = bValue;

        return TRUE;
    }

    if (strcmp(ParamName, "MXHostAsSecondary") == 0)
    {
        pDdnsService->Backup_mx = bValue;

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
#endif
#if defined(DDNS_BROADBANDFORUM)
    return TRUE;
#endif
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Service_SetParamIntValue
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
Service_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
#if !defined(DDNS_BROADBANDFORUM)

    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(iValue);

    return FALSE;
#endif
#if defined(DDNS_BROADBANDFORUM)
   return TRUE;
#endif
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Service_SetParamUlongValue
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
Service_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
#if !defined(DDNS_BROADBANDFORUM)

    /* check the parameter name and set the corresponding value */
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DDNS_SERVICE          pDdnsService = (PCOSA_DML_DDNS_SERVICE   )pCosaContext->hContext;
    errno_t                         rc           = -1;
	
	if (strcmp(ParamName, "ServiceName") == 0)
    {
        /* collect value */
        if ( uValue == 1 )
        {
            rc = STRCPY_S_NOCLOBBER(pDdnsService->ServiceName, sizeof(pDdnsService->ServiceName), "dyndns");
            if ( rc != EOK) {
               ERR_CHK(rc);
               return FALSE;
            }
        }
        else if ( uValue == 2 )
        {
            rc = STRCPY_S_NOCLOBBER(pDdnsService->ServiceName, sizeof(pDdnsService->ServiceName), "tzo");
            if ( rc != EOK) {
                ERR_CHK(rc);
                return FALSE;
            }
        }
		else if ( uValue == 3 )
        {
            rc = STRCPY_S_NOCLOBBER(pDdnsService->ServiceName, sizeof(pDdnsService->ServiceName), "changeip");
            if ( rc != EOK) {
                ERR_CHK(rc);
                return FALSE;
            }
        }
		else if ( uValue == 4 )
        {
            rc = STRCPY_S_NOCLOBBER(pDdnsService->ServiceName, sizeof(pDdnsService->ServiceName), "afraid");
            if ( rc != EOK) {
                ERR_CHK(rc);
                return FALSE;
            }
        }
        else
        {
			//AnscCopyString(pDdnsService->ServiceName, "None");
        }

        return TRUE;
    }
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
#endif
#if defined(DDNS_BROADBANDFORUM)
   return TRUE;
#endif
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Service_SetParamStringValue
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
Service_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
#if !defined(DDNS_BROADBANDFORUM)

    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DDNS_SERVICE          pDdnsService = (PCOSA_DML_DDNS_SERVICE   )pCosaContext->hContext;
    errno_t                         rc           = -1;

    /* check if pString doesn't hold null or whitespaces */
    if(AnscValidStringCheck((PUCHAR)pString) != TRUE)
        return FALSE;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "AssociatedConnection") == 0)
    {
        if ( sizeof(pDdnsService->AssociatedConnection) > AnscSizeOfString(pString))
        {
            /* save update to backup */
            rc = STRCPY_S_NOCLOBBER(pDdnsService->AssociatedConnection, sizeof(pDdnsService->AssociatedConnection), pString);
            if ( rc != EOK) {
                ERR_CHK(rc);
                return FALSE;
            }
            return TRUE;
        }
        else
        {
            CcspTraceWarning(("'%s' value should be less than (%d) charecters\n", ParamName, sizeof(pDdnsService->AssociatedConnection) ));
            return FALSE;
        }
    }

    if (strcmp(ParamName, "Alias") == 0)
    {
        if ( sizeof(pDdnsService->Alias) > AnscSizeOfString(pString))
        {
            /* save update to backup */
            rc = STRCPY_S_NOCLOBBER(pDdnsService->Alias, sizeof(pDdnsService->Alias), pString);
            if ( rc != EOK) {
                ERR_CHK(rc);
                return FALSE;
            }
            return TRUE;
        }
        else
        {
            CcspTraceWarning(("'%s' value should be less than (%d) charecters\n", ParamName, sizeof(pDdnsService->Alias) ));
            return FALSE;
        }

    }

    if (strcmp(ParamName, "Domain") == 0)
    {
        if ( sizeof(pDdnsService->Domain) > AnscSizeOfString(pString))
        {
            /* save update to backup */
            rc = STRCPY_S_NOCLOBBER(pDdnsService->Domain, sizeof(pDdnsService->Domain), pString);
            if ( rc != EOK) {
                ERR_CHK(rc);
                return FALSE;
            }
            return TRUE;
        }
        else
        {
            CcspTraceWarning(("'%s' value should be less than (%d) charecters\n", ParamName, sizeof(pDdnsService->Domain) ));
            return FALSE;
        }
    }

    if (strcmp(ParamName, "Password") == 0)
    {
        if ( sizeof(pDdnsService->Password) > AnscSizeOfString(pString))
        {
            /* save update to backup */
            rc = STRCPY_S_NOCLOBBER(pDdnsService->Password, sizeof(pDdnsService->Password), pString);
            if ( rc != EOK) {
                ERR_CHK(rc);
                return FALSE;
            }
            return TRUE;
        }
        else
        {
            CcspTraceWarning(("'%s' value should be less than (%d) charecters\n", ParamName, sizeof(pDdnsService->Password) ));
            return FALSE;
        }
    }

    if (strcmp(ParamName, "Username") == 0)
    {
        if ( sizeof(pDdnsService->Username) > AnscSizeOfString(pString))
        {
            /* save update to backup */
            rc = STRCPY_S_NOCLOBBER(pDdnsService->Username, sizeof(pDdnsService->Username), pString);
            if ( rc != EOK) {
                ERR_CHK(rc);
                return FALSE;
            }
            return TRUE;
        }
        else
        {
            CcspTraceWarning(("'%s' value should be less than (%d) charecters\n", ParamName, sizeof(pDdnsService->Username) ));
            return FALSE;
        }
    }

    if (strcmp(ParamName, "MXHostName") == 0)
    {
        if ( sizeof(pDdnsService->Mail_exch) > AnscSizeOfString(pString))
        {
            /* save update to backup */
            rc = STRCPY_S_NOCLOBBER(pDdnsService->Mail_exch, sizeof(pDdnsService->Mail_exch), pString);
            if ( rc != EOK) {
                ERR_CHK(rc);
                return FALSE;
            }
            return TRUE;
        }
        else
        {
            CcspTraceWarning(("'%s' value should be less than (%d) charecters\n", ParamName, sizeof(pDdnsService->Mail_exch) ));
            return FALSE;
        }
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
#endif
#if defined(DDNS_BROADBANDFORUM)
    return TRUE;
#endif

}

/* Service_IsDomainStringHaveRepeatedWord() */
BOOL Service_IsDomainStringHaveRepeatedWord ( char* pStringDomain )
{

#if !defined(DDNS_BROADBANDFORUM)

        /*CID: 135402 - Large stack use - alloc heap memory*/
        struct detail *stDetailArray;
	char   		  acSubstring[ 256 ]  = { 0 };
	UINT    	  i = 0;
	int    	      j = 0;
    int           count = 0;

        stDetailArray = (struct detail *) malloc( 64 * sizeof(struct detail));
        if(stDetailArray == NULL)
            return FALSE;

	
	for ( i = 0; i < strlen( pStringDomain ); i++ )
    {
        while ( ( i < strlen( pStringDomain ) ) && \
			    ( pStringDomain[i] != ',' ) && \
			    ( isalnum( pStringDomain[i] ) ) )
        {
            acSubstring[ j++ ] = pStringDomain[ i++ ];
        }
		
        if ( j != 0 )
        {
			int IsHaveRepeatedWord = 0;

            acSubstring[j] = '\0';
            count = Service_CheckRepeatString( stDetailArray, acSubstring, count, &IsHaveRepeatedWord );
            j = 0;

			// Check if repeated
			if( 1 == IsHaveRepeatedWord )
			{
                                free(stDetailArray);
				return TRUE;
			}
        }
    }
        free(stDetailArray);
	return FALSE;
#endif
#if defined(DDNS_BROADBANDFORUM)
       return TRUE;
#endif
}

/* Service_CheckRepeatString() */
int Service_CheckRepeatString( struct detail stDetailArray[], const char unit[], int count, int* pIsHaveRepeatedWord )
{
#if !defined(DDNS_BROADBANDFORUM)

    int i;
    errno_t rc  = -1;
 
    for (i = 0; i < count; i++)
    {
        if ( strcmp(stDetailArray[i].word, unit) == 0)
        {
			/* If control reaches here, it means match found in struct */
           *pIsHaveRepeatedWord  = 1;
		   return count;
        }
    }

    /* If control reaches here, it means no match found in struct */
    rc = strcpy_s( stDetailArray[count].word, sizeof(stDetailArray[count].word),unit );
    if(rc != EOK)
    {
      ERR_CHK(rc);
      return 0;
    }
    /* count represents the number of fields updated in array stDetailArray */
    return ( count + 1);
#endif
#if defined(DDNS_BROADBANDFORUM)
    return 0;
#endif
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Service_Validate
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
Service_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
#if !defined(DDNS_BROADBANDFORUM)

    PCOSA_DATAMODEL_DDNS            pCosaDMDdns     = (PCOSA_DATAMODEL_DDNS     )g_pCosaBEManager->hDdns;
    PSLIST_HEADER                   pListHead       = (PSLIST_HEADER            )&pCosaDMDdns->ContextHead;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DDNS_SERVICE          pDdnsService    = (PCOSA_DML_DDNS_SERVICE   )pCosaContext->hContext;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2   = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_DDNS_SERVICE          pDdnsService2   = (PCOSA_DML_DDNS_SERVICE   )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = (PSINGLE_LINK_ENTRY       )NULL;
    BOOL                            bResult         = FALSE;
    errno_t                         rc              = -1;

    pSLinkEntry = AnscSListGetFirstEntry(pListHead);

    while ( pSLinkEntry )
    {
        pCosaContext2 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        pSLinkEntry   = AnscSListGetNextEntry(pSLinkEntry);

        pDdnsService2 = (PCOSA_DML_DDNS_SERVICE)pCosaContext2->hContext;

        if ( 
                 pDdnsService2 && 
                 ((ULONG)pDdnsService2 != (ULONG)pDdnsService) && 
                 AnscEqualString(pDdnsService2->Alias, pDdnsService->Alias, TRUE) 
           )
        {

            rc = strcpy_s(pReturnParamName, *puLength, "Alias");
            ERR_CHK(rc);
            *puLength = AnscSizeOfString("Alias");
             
            return FALSE;
        }
    }

	//Check whether Domain string contains repeated word or not
	if( TRUE == Service_IsDomainStringHaveRepeatedWord( pDdnsService->Domain ) )
	{
                rc = strcpy_s(pReturnParamName, *puLength, "Domain");
                ERR_CHK(rc);
		
		*puLength = AnscSizeOfString("Domain");
		 
		return FALSE;
	}

    if ( FALSE )
    {
        if ( g_ValidateInterface )
        {
            bResult = g_ValidateInterface(g_pDslhDmlAgent, pDdnsService->AssociatedConnection, DSLH_WAN_LAYER3_CONNECTION_INTERFACE);
        }

        if ( bResult == FALSE )
        {
            rc = strcpy_s(pReturnParamName, *puLength, "AssociatedConnection");
            ERR_CHK(rc);
            *puLength = AnscSizeOfString(pReturnParamName);

            return FALSE;
        }
    }

    return TRUE;
#endif
#if defined(DDNS_BROADBANDFORUM)
   return TRUE;
#endif
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Service_Commit
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
Service_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
#if !defined(DDNS_BROADBANDFORUM)

    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DDNS            pCosaDMDdns  = (PCOSA_DATAMODEL_DDNS     )g_pCosaBEManager->hDdns;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DDNS_SERVICE          pDdnsService = (PCOSA_DML_DDNS_SERVICE   )pCosaContext->hContext;

    if ( pCosaContext->bNew )
    {
        returnStatus = CosaDmlDdnsAddService (NULL, pDdnsService);

        if ( returnStatus == ANSC_STATUS_SUCCESS )
        {
            pCosaContext->bNew = FALSE;

            CosaDdnsRegDelDdnsInfo((ANSC_HANDLE)pCosaDMDdns, (ANSC_HANDLE)pCosaContext);
        }
    }
    else
    {
        returnStatus = CosaDmlDdnsSetService(NULL, pDdnsService);
    }

    return returnStatus;
#endif
#if defined(DDNS_BROADBANDFORUM)
   return 0;
#endif
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Service_Rollback
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
Service_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
#if !defined(DDNS_BROADBANDFORUM)

    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DDNS_SERVICE          pDdnsService = (PCOSA_DML_DDNS_SERVICE   )pCosaContext->hContext;

    return CosaDmlDdnsGetServiceByInstNum
               (
                   NULL, 
                   pDdnsService->InstanceNumber, 
                   pDdnsService
               );

#endif
#if defined(DDNS_BROADBANDFORUM) 
  return 0;
#endif
}

