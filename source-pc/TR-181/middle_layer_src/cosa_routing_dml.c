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

    module: cosa_routing_dml.c

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
#include "cosa_routing_dml.h"
#include "cosa_routing_apis.h"
#include "plugin_main_apis.h"
#include "cosa_routing_internal.h"

#define REFRESH_INTERVAL 120
#define TIME_NO_NEGATIVE(x) ((long)(x) < 0 ? 0 : (x))
static ULONG last_tick;


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

    Routing.

    *  Routing_GetParamBoolValue
    *  Routing_GetParamIntValue
    *  Routing_GetParamUlongValue
    *  Routing_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Routing_GetParamBoolValue
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
Routing_GetParamBoolValue
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
        Routing_GetParamIntValue
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
Routing_GetParamIntValue
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
        Routing_GetParamUlongValue
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
Routing_GetParamUlongValue
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
        Routing_GetParamStringValue
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
Routing_GetParamStringValue
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

    Routing.Router.{i}.

    *  Router_GetEntryCount
    *  Router_GetEntry
    *  Router_AddEntry
    *  Router_DelEntry
    *  Router_GetParamBoolValue
    *  Router_GetParamIntValue
    *  Router_GetParamUlongValue
    *  Router_GetParamStringValue
    *  Router_SetParamBoolValue
    *  Router_SetParamIntValue
    *  Router_SetParamUlongValue
    *  Router_SetParamStringValue
    *  Router_Validate
    *  Router_Commit
    *  Router_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Router_GetEntryCount
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
Router_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    /*
     *     assuming there is only one router in the system
    */
    PCOSA_DATAMODEL_ROUTING         pMyObject = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting; 
    
    return AnscSListQueryDepth(&pMyObject->RouterList);
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Router_GetEntry
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
Router_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_ROUTING         pMyObject    = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting; 
    PSLIST_HEADER                   pRouterHead  = (PSLIST_HEADER)&pMyObject->RouterList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pLink        = (PSINGLE_LINK_ENTRY       )NULL;

    pLink = AnscSListGetEntryByIndex(pRouterHead, nIndex);

    if ( pLink )
    {
        pCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        *pInsNumber = pCosaContext->InstanceNumber;
    }
    
    return pCosaContext;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Router_AddEntry
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
Router_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_ROUTING         pMyObject    = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting; 
    PSLIST_HEADER                   pRouterHead  = (PSLIST_HEADER)&pMyObject->RouterList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_ROUTER_FULL2          pRouter      = (PCOSA_DML_ROUTER_FULL2)NULL;

    pRouter = (PCOSA_DML_ROUTER_FULL2)AnscAllocateMemory(sizeof(COSA_DML_ROUTER_FULL2));

    if ( !pRouter )
    {
        return NULL;
    }

    _ansc_sprintf(pRouter->Cfg.Alias, "Router%lu", pMyObject->ulNextRouterInsNum);

    AnscSListInitializeHeader(&pRouter->ForwardList);
    
    pRouter->ulNextForwardInsNum = 1;
    
    if ( TRUE )
    {
        pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

        if ( !pCosaContext )
        {
            AnscFreeMemory(pRouter);
            
            return NULL;
        }

        pCosaContext->InstanceNumber = pRouter->Cfg.InstanceNumber = pMyObject->ulNextRouterInsNum;

        pMyObject->ulNextRouterInsNum++;

        if ( 0 == pMyObject->ulNextRouterInsNum )
        {
            pMyObject->ulNextRouterInsNum = 1;
        }

        pCosaContext->hContext         = (ANSC_HANDLE)pRouter;
        pCosaContext->hParentTable     = NULL;
        pCosaContext->hPoamIrepUpperFo = pMyObject->hIrepFolderRouterHA;
        pCosaContext->bNew             = TRUE;

        CosaSListPushEntryByInsNum(pRouterHead, pCosaContext);

        CosaRoutingRegAddInfo
        (
            (ANSC_HANDLE)pMyObject,
            COSA_DML_RR_NAME_ROUTER_NextInsNum,
            pMyObject->ulNextRouterInsNum,
            COSA_DML_RR_NAME_ROUTER_Prefix,
            0,
            pRouter->Cfg.Alias,
            (ANSC_HANDLE)pCosaContext
        );
    }

    *pInsNumber = pCosaContext->InstanceNumber;

    return pCosaContext;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Router_DelEntry
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
Router_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_ROUTING         pMyObject    = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting; 
    PSLIST_HEADER                   pRouterHead  = (PSLIST_HEADER)&pMyObject->RouterList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_DML_ROUTER_FULL2          pRouter      = (PCOSA_DML_ROUTER_FULL2)pCosaContext->hContext;
    
    /*Not delete entry from backend for no corresponding API */

    AnscSListPopEntryByLink(pRouterHead, &pCosaContext->Linkage);

    if ( pCosaContext->bNew )
    {
        CosaRoutingRegDelInfo(NULL, (ANSC_HANDLE)pCosaContext);
    }
    
    AnscFreeMemory(pRouter);
    AnscFreeMemory(pCosaContext);

    return 0;
    }


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Router_GetParamBoolValue
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
Router_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ROUTER_FULL2          pRouter      = (PCOSA_DML_ROUTER_FULL2)pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        *pBool = pRouter->Cfg.bEnabled;
        /* collect value */
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Router_GetParamIntValue
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
Router_GetParamIntValue
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
        Router_GetParamUlongValue
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
Router_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ROUTER_FULL2          pRouter      = (PCOSA_DML_ROUTER_FULL2)pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Status") == 0)
    {
        /* collect value */
        *puLong = pRouter->Info.Status;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Router_GetParamStringValue
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
Router_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ROUTER_FULL2          pRouter      = (PCOSA_DML_ROUTER_FULL2)pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* collect value */
        AnscCopyString(pValue, pRouter->Cfg.Alias);
        return 0;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Router_SetParamBoolValue
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
Router_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_DATAMODEL_ROUTING         pMyObject    = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ROUTER_FULL2          pRouter      = (PCOSA_DML_ROUTER_FULL2)pCosaContext->hContext;

    BRIDGE_MODE_JUDGEMENT_IFTRUE_RETURNFALSE

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* save update to backup */
        pRouter->Cfg.bEnabled = bValue;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Router_SetParamIntValue
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
Router_SetParamIntValue
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
        Router_SetParamUlongValue
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
Router_SetParamUlongValue
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
        Router_SetParamStringValue
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
Router_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ROUTER_FULL2          pRouter      = (PCOSA_DML_ROUTER_FULL2)pCosaContext->hContext;

    BRIDGE_MODE_JUDGEMENT_IFTRUE_RETURNFALSE

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* save update to backup */
         AnscCopyString(pRouter->Cfg.Alias, pString);
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Router_Validate
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
Router_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_DATAMODEL_ROUTING         pMyObject     = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2 = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_ROUTER_FULL2          pRouter       = (PCOSA_DML_ROUTER_FULL2)pCosaContext->hContext;
    PCOSA_DML_ROUTER_FULL2          pRouter2      = (PCOSA_DML_ROUTER_FULL2)NULL;
    PSLIST_HEADER                   pRouterHead   = (PSLIST_HEADER)&pMyObject->RouterList;
    PSINGLE_LINK_ENTRY              pLink         = (PSINGLE_LINK_ENTRY)NULL;

    pLink = AnscSListGetFirstEntry(pRouterHead);

    while ( pLink )
    {
        pCosaContext2 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        pLink = AnscSListGetNextEntry(pLink);

        pRouter2 = (PCOSA_DML_ROUTER_FULL2)pCosaContext2->hContext;

        if ( pRouter2 && 
            ((ULONG)pRouter2 != (ULONG)pRouter) &&
             AnscEqualString(pRouter2->Cfg.Alias, pRouter->Cfg.Alias, TRUE))
        {
            AnscCopyString(pReturnParamName, "Alias");
            *puLength = AnscSizeOfString("Alias");

            CcspTraceWarning(("Router_Validate() failed.\n"));
            return FALSE;
        }
    }

    return TRUE;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Router_Commit
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
Router_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_ROUTING         pMyObject    = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ROUTER_FULL2          pRouter      = (PCOSA_DML_ROUTER_FULL2)pCosaContext->hContext;
    PSLIST_HEADER                   pListHead         = (PSLIST_HEADER)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContextEntry = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY       )NULL;
    ULONG                           ulIndex           = 0;
    COSA_DML_ROUTER_CFG             cfg;

    if ( pCosaContext->bNew )
    {
        pCosaContext->bNew = FALSE;

        /*Not add entry to backend for no corresponding API */

        CosaRoutingRegDelInfo(NULL, (ANSC_HANDLE)pCosaContext);
    }
    else
    {
        CosaDmlRoutingRouterGetCfg(NULL, (PCOSA_DML_ROUTER_CFG)&cfg);
        if (cfg.bEnabled != pRouter->Cfg.bEnabled) {
            /* Enable/Disable all static route*/
            pListHead = (PSLIST_HEADER)&pRouter->ForwardList;
            pSLinkEntry = AnscSListGetFirstEntry(pListHead);

            for ( ulIndex = 0; ulIndex < pListHead->Depth; ulIndex++ )
            {
                pCosaContextEntry = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
                pSLinkEntry       = AnscSListGetNextEntry(pSLinkEntry);

                /*set IPv4Forwarding parameter*/
                IPv4Forwarding_SetParamBoolValue(pCosaContextEntry,"Enable", pRouter->Cfg.bEnabled);
                IPv4Forwarding_Commit(pCosaContextEntry);
            }
        }

        CosaDmlRoutingRouterSetCfg(NULL, (PCOSA_DML_ROUTER_CFG)&pRouter->Cfg);

    }

    return 0;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Router_Rollback
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
Router_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{   
    PCOSA_DATAMODEL_ROUTING         pMyObject    = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ROUTER_FULL2          pRouter      = (PCOSA_DML_ROUTER_FULL2)pCosaContext->hContext;

    CcspTraceWarning(("Router_Rollback()----begin to rollback......\n"));

    CosaDmlRoutingRouterGetCfg(NULL, (PCOSA_DML_ROUTER_CFG)&pRouter->Cfg);

    return 0; 
}


/***********************************************************************

 APIs for Object:

    LANDevice.{i}.LANHostConfigManagement.X_CISCO_COM_StaticIPv4Forwarding.{i}.

    *  X_CISCO_COM_StaticIPv4Forwarding_GetEntryCount
    *  X_CISCO_COM_StaticIPv4Forwarding_GetEntry
    *  X_CISCO_COM_StaticIPv4Forwarding_AddEntry
    *  X_CISCO_COM_StaticIPv4Forwarding_DelEntry
    *  X_CISCO_COM_StaticIPv4Forwarding_GetParamBoolValue
    *  X_CISCO_COM_StaticIPv4Forwarding_GetParamIntValue
    *  X_CISCO_COM_StaticIPv4Forwarding_GetParamUlongValue
    *  X_CISCO_COM_StaticIPv4Forwarding_GetParamStringValue
    *  X_CISCO_COM_StaticIPv4Forwarding_SetParamBoolValue
    *  X_CISCO_COM_StaticIPv4Forwarding_SetParamIntValue
    *  X_CISCO_COM_StaticIPv4Forwarding_SetParamUlongValue
    *  X_CISCO_COM_StaticIPv4Forwarding_SetParamStringValue
    *  X_CISCO_COM_StaticIPv4Forwarding_Validate
    *  X_CISCO_COM_StaticIPv4Forwarding_Commit
    *  X_CISCO_COM_StaticIPv4Forwarding_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        X_CISCO_COM_StaticIPv4Forwarding_GetEntryCount
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
X_CISCO_COM_StaticIPv4Forwarding_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_ROUTING         pMyObject = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting; 
    PSLIST_HEADER                   pListHead       = (PSLIST_HEADER       )&pMyObject->StaticRoute;

    return AnscSListQueryDepth( pListHead );
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        X_CISCO_COM_StaticIPv4Forwarding_GetEntry
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
X_CISCO_COM_StaticIPv4Forwarding_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_ROUTING         pMyObject       = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting; 
    PSLIST_HEADER                   pListHead       = (PSLIST_HEADER       )&pMyObject->StaticRoute;
    PSINGLE_LINK_ENTRY              pSListEntry     = NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink             = NULL;

    pSListEntry = AnscSListGetEntryByIndex(pListHead, nIndex);

    if ( pSListEntry )
    {
        pCxtLink          = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSListEntry);
        *pInsNumber       = pCxtLink->InstanceNumber;
    }

    return pSListEntry;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        X_CISCO_COM_StaticIPv4Forwarding_AddEntry
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
X_CISCO_COM_StaticIPv4Forwarding_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_ROUTING         pMyObject       = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting; 
    PSLIST_HEADER                   pListHead       = (PSLIST_HEADER       )&pMyObject->StaticRoute;
    PSINGLE_LINK_ENTRY              pSListEntry     = NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink        = NULL;
    PCOSA_DML_STATICROUTE_CFG       pEntry          = NULL;
    
    pEntry  = (PCOSA_DML_STATICROUTE_CFG)AnscAllocateMemory( sizeof(COSA_DML_STATICROUTE_CFG) );
    if ( !pEntry )
    {
        goto EXIT2;
    }
    
    pCxtLink = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory( sizeof(COSA_CONTEXT_LINK_OBJECT) );
    if ( !pCxtLink )
    {
        goto EXIT1;
    }

    if ( !pMyObject->NextInstanceNumber )
    {
        pMyObject->NextInstanceNumber = 1;
    }

    pCxtLink->InstanceNumber       = pMyObject->NextInstanceNumber++; 
    *pInsNumber                    = pCxtLink->InstanceNumber;

    /* We give a permenant name here*/
    _ansc_sprintf(pEntry->Name, "StaticRoute_%x_%lu", (ULONG)pEntry, pCxtLink->InstanceNumber );

    pCxtLink->hContext       = (ANSC_HANDLE)pEntry;
    pCxtLink->hParentTable   = (ANSC_HANDLE)pMyObject;
    pCxtLink->bNew           = TRUE;

    /* Put into our list */
    CosaSListPushEntryByInsNum(pListHead, (PCOSA_CONTEXT_LINK_OBJECT)pCxtLink);

    return (ANSC_HANDLE)pCxtLink;
           
EXIT1:
            
    AnscFreeMemory(pEntry);
        
EXIT2:   
            
        return NULL;

}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        X_CISCO_COM_StaticIPv4Forwarding_DelEntry
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
X_CISCO_COM_StaticIPv4Forwarding_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_ROUTING         pMyObject       = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting; 
    PSLIST_HEADER                   pListHead       = (PSLIST_HEADER                )&pMyObject->StaticRoute;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink        = (PCOSA_CONTEXT_LINK_OBJECT    )hInstance;
    PCOSA_DML_STATICROUTE_CFG       pEntry          = (PCOSA_DML_STATICROUTE_CFG    )pCxtLink->hContext;

    if ( !pCxtLink->bNew )
    {
        returnStatus = CosaDmlStaticRouteDelEntry(NULL,pEntry);
        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            return returnStatus;
        }
    }

    if ( AnscSListPopEntryByLink(pListHead, &pCxtLink->Linkage) )
    {
        AnscFreeMemory(pCxtLink->hContext);
        AnscFreeMemory(pCxtLink);
    }

    return returnStatus;    

}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_StaticIPv4Forwarding_GetParamBoolValue
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
X_CISCO_COM_StaticIPv4Forwarding_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_STATICROUTE_CFG       pEntry          = pCosaContext->hContext;
    
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "RIPAdvertise") == 0)
    {
        /* collect value */
        *pBool = pEntry->RIPAdvertise;
        return TRUE;
    }


    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_StaticIPv4Forwarding_GetParamIntValue
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
X_CISCO_COM_StaticIPv4Forwarding_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    /* check the parameter name and return the corresponding value */

    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_StaticIPv4Forwarding_GetParamUlongValue
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
X_CISCO_COM_StaticIPv4Forwarding_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_STATICROUTE_CFG       pEntry          = pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "DestIPAddress") == 0)
    {
        /* collect value */
        *puLong = pEntry->DestIPAddress;
        return TRUE;
    }
    
    if (strcmp(ParamName, "DestSubnetMask") == 0)
    {
        /* collect value */
        *puLong = pEntry->DestSubnetMask;
        return TRUE;
    }

    if (strcmp(ParamName, "GatewayIPAddress") == 0)
    {
        /* collect value */
        *puLong = pEntry->GatewayIPAddress;
        return TRUE;
    }

    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        X_CISCO_COM_StaticIPv4Forwarding_GetParamStringValue
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
X_CISCO_COM_StaticIPv4Forwarding_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_STATICROUTE_CFG       pEntry          = pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */

    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_StaticIPv4Forwarding_SetParamBoolValue
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
X_CISCO_COM_StaticIPv4Forwarding_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_STATICROUTE_CFG       pEntry          = pCosaContext->hContext;

    BRIDGE_MODE_JUDGEMENT_IFTRUE_RETURNFALSE

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "RIPAdvertise") == 0)
    {
        /* save update to backup */
        pEntry->RIPAdvertise = bValue;
        return TRUE;
    }


    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_StaticIPv4Forwarding_SetParamIntValue
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
X_CISCO_COM_StaticIPv4Forwarding_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    /* check the parameter name and set the corresponding value */

    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_StaticIPv4Forwarding_SetParamUlongValue
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
X_CISCO_COM_StaticIPv4Forwarding_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_STATICROUTE_CFG       pEntry          = pCosaContext->hContext;

    BRIDGE_MODE_JUDGEMENT_IFTRUE_RETURNFALSE

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "DestIPAddress") == 0)
    {
        /* save update to backup */
        pEntry->DestIPAddress = uValue;
        return TRUE;
    }
    
    if (strcmp(ParamName, "DestSubnetMask") == 0)
    {
        /* save update to backup */
        pEntry->DestSubnetMask = uValue;
        return TRUE;
    }
    
    if (strcmp(ParamName, "GatewayIPAddress") == 0)
    {
        /* save update to backup */
        pEntry->GatewayIPAddress = uValue;
        return TRUE;
    }


    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_StaticIPv4Forwarding_SetParamStringValue
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
X_CISCO_COM_StaticIPv4Forwarding_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_STATICROUTE_CFG       pEntry          = pCosaContext->hContext;

    /* check the parameter name and set the corresponding value */

    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_StaticIPv4Forwarding_Validate
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
X_CISCO_COM_StaticIPv4Forwarding_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    ANSC_STATUS                     reStatus          = ANSC_STATUS_SUCCESS;        
    PCOSA_DATAMODEL_ROUTING         pMyObject         = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting; 
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_STATICROUTE_CFG       pEntry            = (PCOSA_DML_STATICROUTE_CFG)pCxtLink->hContext;

    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        X_CISCO_COM_StaticIPv4Forwarding_Commit
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
X_CISCO_COM_StaticIPv4Forwarding_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                     reStatus          = ANSC_STATUS_SUCCESS;        
    PCOSA_DATAMODEL_ROUTING         pMyObject         = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting; 
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_STATICROUTE_CFG       pEntry            = (PCOSA_DML_STATICROUTE_CFG)pCxtLink->hContext;

    if ( pCxtLink->bNew )
    {
        reStatus = CosaDmlStaticRouteAddEntry(NULL, pEntry);
        pCxtLink->bNew = FALSE;
    }
    else
    {
        reStatus = CosaDmlStaticRouteSetEntry(NULL, pEntry);
    }
    
    if ( reStatus == ANSC_STATUS_SUCCESS )
    {
        return 0;
    }
    else
    {
        return -1;
    }    
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        X_CISCO_COM_StaticIPv4Forwarding_Rollback
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
X_CISCO_COM_StaticIPv4Forwarding_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                     reStatus          = ANSC_STATUS_SUCCESS;        
    PCOSA_DATAMODEL_ROUTING         pMyObject         = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting; 
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_STATICROUTE_CFG       pEntry            = (PCOSA_DML_STATICROUTE_CFG)pCxtLink->hContext;

    reStatus = CosaDmlStaticRouteGetEntryByName(NULL, &pEntry->Name);
    
    if ( reStatus == ANSC_STATUS_SUCCESS )
    {
        return 0;
    }
    else
    {
        return -1;
    }
}



/***********************************************************************

 APIs for Object:

    Routing.Router.{i}.IPv4Forwarding.{i}.

    *  IPv4Forwarding_GetEntryCount
    *  IPv4Forwarding_GetEntry
    *  IPv4Forwarding_AddEntry
    *  IPv4Forwarding_DelEntry
    *  IPv4Forwarding_GetParamBoolValue
    *  IPv4Forwarding_GetParamIntValue
    *  IPv4Forwarding_GetParamUlongValue
    *  IPv4Forwarding_GetParamStringValue
    *  IPv4Forwarding_SetParamBoolValue
    *  IPv4Forwarding_SetParamIntValue
    *  IPv4Forwarding_SetParamUlongValue
    *  IPv4Forwarding_SetParamStringValue
    *  IPv4Forwarding_Validate
    *  IPv4Forwarding_Commit
    *  IPv4Forwarding_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv4Forwarding_GetEntryCount
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
IPv4Forwarding_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ROUTER_FULL2          pRouter      = (PCOSA_DML_ROUTER_FULL2)pCosaContext->hContext;

    return AnscSListQueryDepth(&pRouter->ForwardList);
}



/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        IPv4Forwarding_GetEntry
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
IPv4Forwarding_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ROUTER_FULL2          pRouter         = (PCOSA_DML_ROUTER_FULL2)pCosaContext->hContext;
    PSINGLE_LINK_ENTRY              pLink           = (PSINGLE_LINK_ENTRY)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)NULL;

    pLink = AnscSListGetEntryByIndex((PSLIST_HEADER)&pRouter->ForwardList, nIndex);

    if ( pLink )
    {
        pSubCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        *pInsNumber = pSubCosaContext->InstanceNumber;
    }
    
    return pSubCosaContext;

}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        IPv4Forwarding_AddEntry
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
IPv4Forwarding_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_ROUTING         pMyObject       = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ROUTER_FULL2          pRouter         = (PCOSA_DML_ROUTER_FULL2)pCosaContext->hContext;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_ROUTING_V4_ENTRY      pRouterForward  = (PCOSA_DML_ROUTING_V4_ENTRY)NULL;


    pRouterForward = (PCOSA_DML_ROUTING_V4_ENTRY)AnscAllocateMemory(sizeof(COSA_DML_ROUTING_V4_ENTRY));

    if ( !pRouterForward )
    {
        return NULL;
    }

    _ansc_sprintf(pRouterForward->Alias, "IPv4Forwarding%d", pRouter->ulNextForwardInsNum);

    /* Update the middle layer data */
    if ( TRUE )
    {
        pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

        if ( !pSubCosaContext )
        {
            AnscFreeMemory(pRouterForward);
            
            return NULL;
        }

        pSubCosaContext->InstanceNumber = pRouterForward->InstanceNumber = pRouter->ulNextForwardInsNum;
        

        pRouter->ulNextForwardInsNum++;

        if ( 0 == pRouter->ulNextForwardInsNum )
        {
            pRouter->ulNextForwardInsNum = 1;
        }

        pSubCosaContext->hContext         = (ANSC_HANDLE)pRouterForward;
        pSubCosaContext->hParentTable     = pRouter;
        pSubCosaContext->hPoamIrepUpperFo = pMyObject->hIrepFolderRouterHA;
        pSubCosaContext->bNew             = TRUE;

        CosaSListPushEntryByInsNum((PSLIST_HEADER)&pRouter->ForwardList, pSubCosaContext);

        CosaRoutingRegAddInfo
        (
            (ANSC_HANDLE)pMyObject,
            COSA_DML_RR_NAME_ROUTER_Forward_NextInsNum,
            pRouter->ulNextForwardInsNum,
            COSA_DML_RR_NAME_ROUTER_Forward_Prefix,
            pRouter->Cfg.InstanceNumber,
            pRouterForward->Alias,
            (ANSC_HANDLE)pSubCosaContext
        );
    }

    *pInsNumber = pSubCosaContext->InstanceNumber;

    return pSubCosaContext;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv4Forwarding_DelEntry
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
IPv4Forwarding_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_ROUTING         pMyObject       = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ROUTER_FULL2          pRouter         = (PCOSA_DML_ROUTER_FULL2)pCosaContext->hContext;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_DML_ROUTING_V4_ENTRY      pRouterForward  = (PCOSA_DML_ROUTING_V4_ENTRY)pSubCosaContext->hContext;
    
    CosaDmlRoutingDelV4Entry(NULL, pRouterForward);

    AnscSListPopEntryByLink((PSLIST_HEADER)&pRouter->ForwardList, &pSubCosaContext->Linkage);

    if ( pSubCosaContext->bNew )
    {
        CosaRoutingRegDelInfo(NULL, (ANSC_HANDLE)pSubCosaContext);
    }

    AnscFreeMemory(pRouterForward);
    AnscFreeMemory(pSubCosaContext);

    return 0;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv4Forwarding_GetParamBoolValue
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
IPv4Forwarding_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext   = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ROUTING_V4_ENTRY      pRouterForward = (PCOSA_DML_ROUTING_V4_ENTRY)pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        *pBool = pRouterForward->Enable;
        return TRUE;
    }

    if (strcmp(ParamName, "StaticRoute") == 0)
    {
        /* collect value */
        *pBool = pRouterForward->StaticRoute;
        return TRUE;
    }

    if (strcmp(ParamName, "X_CISCO_COM_RIPAdvertise") == 0)
    {
        /* collect value */
        *pBool = pRouterForward->X_CISCO_COM_RIPAdvertise;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv4Forwarding_GetParamIntValue
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
IPv4Forwarding_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext   = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ROUTING_V4_ENTRY      pRouterForward = (PCOSA_DML_ROUTING_V4_ENTRY)pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "ForwardingPolicy") == 0)
    {
        /* collect value */
        *pInt = pRouterForward->ForwardingPolicy;
        return TRUE;
    }

    if (strcmp(ParamName, "ForwardingMetric") == 0)
    {
        /* collect value */
        *pInt = pRouterForward->ForwardingMetric;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv4Forwarding_GetParamUlongValue
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
IPv4Forwarding_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext   = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ROUTING_V4_ENTRY      pRouterForward = (PCOSA_DML_ROUTING_V4_ENTRY)pCosaContext->hContext;


    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Status") == 0)
    {
        /* collect value */
        *puLong = pRouterForward->Status;
        return TRUE;
    }

    if (strcmp(ParamName, "DestIPAddress") == 0)
    {
        /* collect value */
        *puLong = pRouterForward->DestIPAddress.Value;
        return TRUE;
    }

    if (strcmp(ParamName, "DestSubnetMask") == 0)
    {
        /* collect value */
        *puLong = pRouterForward->DestSubnetMask.Value;
        return TRUE;
    }

    if (strcmp(ParamName, "GatewayIPAddress") == 0)
    {
        /* collect value */
        *puLong = pRouterForward->GatewayIPAddress.Value;
        return TRUE;
    }

    if (strcmp(ParamName, "Origin") == 0)
    {
        /* collect value */
        CosaDmlRoutingGetV4Entry2(NULL, pRouterForward);
        *puLong = pRouterForward->Origin;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv4Forwarding_GetParamStringValue
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
IPv4Forwarding_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext   = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ROUTING_V4_ENTRY      pRouterForward = (PCOSA_DML_ROUTING_V4_ENTRY)pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* collect value */
        AnscCopyString(pValue, pRouterForward->Alias);
        return 0;
    }

    if (strcmp(ParamName, "Interface") == 0)
    {
        char                            * pString      = NULL;

        /* collect value */
        pString = CosaUtilGetFullPathNameByKeyword
            (
                "Device.IP.Interface.",
                "Name",
                pRouterForward->Interface
            );
        
        if ( pString )
        {
            if ( AnscSizeOfString(pString) < *pUlSize)
            {
                AnscCopyString(pValue, pString);

                AnscFreeMemory(pString);

                return 0;
            }
            else
            {
                *pUlSize = AnscSizeOfString(pString)+1;

                AnscFreeMemory(pString);
                
                return 1;
            }
        }
        else
        {
            return 0;
        }
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv4Forwarding_SetParamBoolValue
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
IPv4Forwarding_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext   = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ROUTING_V4_ENTRY      pRouterForward = (PCOSA_DML_ROUTING_V4_ENTRY)pCosaContext->hContext;

    BRIDGE_MODE_JUDGEMENT_IFTRUE_RETURNFALSE

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* save update to backup */
        pRouterForward->Enable = bValue;
        return TRUE;
    }

    if (strcmp(ParamName, "X_CISCO_COM_RIPAdvertise") == 0)
    {
        /* save update to backup */
        pRouterForward->X_CISCO_COM_RIPAdvertise = bValue;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv4Forwarding_SetParamIntValue
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
IPv4Forwarding_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext   = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ROUTING_V4_ENTRY      pRouterForward = (PCOSA_DML_ROUTING_V4_ENTRY)pCosaContext->hContext;

    BRIDGE_MODE_JUDGEMENT_IFTRUE_RETURNFALSE

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "ForwardingPolicy") == 0)
    {
        /* save update to backup */
        pRouterForward->ForwardingPolicy = iValue;
        return TRUE;
    }

    if (strcmp(ParamName, "ForwardingMetric") == 0)
    {
        /* save update to backup */
        pRouterForward->ForwardingMetric = iValue;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv4Forwarding_SetParamUlongValue
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
IPv4Forwarding_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext   = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ROUTING_V4_ENTRY      pRouterForward = (PCOSA_DML_ROUTING_V4_ENTRY)pCosaContext->hContext;

    BRIDGE_MODE_JUDGEMENT_IFTRUE_RETURNFALSE

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "DestIPAddress") == 0)
    {
        /* save update to backup */
        pRouterForward->DestIPAddress.Value = uValue;
        return TRUE;
    }

    if (strcmp(ParamName, "DestSubnetMask") == 0)
    {
        /* save update to backup */
        pRouterForward->DestSubnetMask.Value = uValue;
        return TRUE;
    }

    if (strcmp(ParamName, "GatewayIPAddress") == 0)
    {
        /* save update to backup */
        pRouterForward->GatewayIPAddress.Value = uValue;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv4Forwarding_SetParamStringValue
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
IPv4Forwarding_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext   = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ROUTING_V4_ENTRY      pRouterForward = (PCOSA_DML_ROUTING_V4_ENTRY)pCosaContext->hContext;

    BRIDGE_MODE_JUDGEMENT_IFTRUE_RETURNFALSE

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* save update to backup */
        AnscCopyString(pRouterForward->Alias, pString);
        return TRUE;
    }

    if (strcmp(ParamName, "Interface") == 0)
    {
        /* save update to backup */
        AnscCopyString(pRouterForward->Interface, pString);
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv4Forwarding_Validate
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
IPv4Forwarding_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2 = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_ROUTER_FULL2          pRouter       = (PCOSA_DML_ROUTER_FULL2)pCosaContext->hParentTable;
    PCOSA_DML_ROUTING_V4_ENTRY      pRouterForward  = (PCOSA_DML_ROUTING_V4_ENTRY)pCosaContext->hContext;
    PCOSA_DML_ROUTING_V4_ENTRY      pRouterForward2 = (PCOSA_DML_ROUTING_V4_ENTRY)NULL;
    PSINGLE_LINK_ENTRY              pLink           = (PSINGLE_LINK_ENTRY)NULL;

    pLink = AnscSListGetFirstEntry(&pRouter->ForwardList);

    while ( pLink )
    {
        pCosaContext2 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        pLink   = AnscSListGetNextEntry(pLink);

        pRouterForward2 = (PCOSA_DML_ROUTING_V4_ENTRY)pCosaContext2->hContext;

        if ( pRouterForward2 &&
            ((ULONG)pRouterForward2 != (ULONG)pRouterForward) &&
             AnscEqualString(pRouterForward2->Alias, pRouterForward->Alias, TRUE))
        {
            AnscCopyString(pReturnParamName, "Alias");
            *puLength = AnscSizeOfString("Alias");

            CcspTraceWarning(("IPv4Forwarding_Validate() failed.\n"));            
            return FALSE;
        }
    }

    return TRUE;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv4Forwarding_Commit
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
IPv4Forwarding_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_ROUTING         pMyObject      = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext   = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ROUTING_V4_ENTRY      pRouterForward = (PCOSA_DML_ROUTING_V4_ENTRY)pCosaContext->hContext;

    if ( pCosaContext->bNew )
    {
        pCosaContext->bNew = FALSE;

        CosaDmlRoutingAddV4Entry(NULL, pRouterForward);

        CosaRoutingRegDelInfo(NULL, (ANSC_HANDLE)pCosaContext);
    }
    else
    {
        CosaDmlRoutingSetV4Entry(NULL, pRouterForward);
    }
    
    return 0;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv4Forwarding_Rollback
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
IPv4Forwarding_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext   = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ROUTING_V4_ENTRY      pRouterForward = (PCOSA_DML_ROUTING_V4_ENTRY)pCosaContext->hContext;

    CcspTraceWarning(("IPv4Forwarding_Rollback()----begin to rollback......\n"));

    CosaDmlRoutingGetV4Entry2(NULL, pRouterForward);

    return 0;
}

/***********************************************************************

 APIs for Object:

    Routing.Router.{i}.IPv6Forwarding.{i}.

    *  IPv6Forwarding_GetEntryCount
    *  IPv6Forwarding_GetEntry
    *  IPv6Forwarding_AddEntry
    *  IPv6Forwarding_DelEntry
    *  IPv6Forwarding_GetParamBoolValue
    *  IPv6Forwarding_GetParamIntValue
    *  IPv6Forwarding_GetParamUlongValue
    *  IPv6Forwarding_GetParamStringValue
    *  IPv6Forwarding_SetParamBoolValue
    *  IPv6Forwarding_SetParamIntValue
    *  IPv6Forwarding_SetParamUlongValue
    *  IPv6Forwarding_SetParamStringValue
    *  IPv6Forwarding_Validate
    *  IPv6Forwarding_Commit
    *  IPv6Forwarding_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv6Forwarding_GetEntryCount
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
IPv6Forwarding_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ROUTER_FULL2          pRouter      = (PCOSA_DML_ROUTER_FULL2)pCosaContext->hContext;

    return AnscSListQueryDepth(&pRouter->IPv6ForwardList);
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        IPv6Forwarding_GetEntry
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
IPv6Forwarding_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT        )hInsContext;
    PCOSA_DML_ROUTER_FULL2          pRouter         = (PCOSA_DML_ROUTER_FULL2           )pCosaContext->hContext;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = (PSINGLE_LINK_ENTRY               )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT        )NULL;

    pSLinkEntry = AnscSListGetEntryByIndex((PSLIST_HEADER)&pRouter->IPv6ForwardList, nIndex);

    if ( pSLinkEntry )
    {
        pSubCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        
        *pInsNumber = pSubCosaContext->InstanceNumber;
    }
    
    return pSubCosaContext;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        IPv6Forwarding_AddEntry
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
IPv6Forwarding_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_ROUTING         pMyObject       = (PCOSA_DATAMODEL_ROUTING       )g_pCosaBEManager->hRouting; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT     )hInsContext;
    PCOSA_DML_ROUTER_FULL2          pRouter         = (PCOSA_DML_ROUTER_FULL2        )pCosaContext->hContext;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT     )NULL;
    PCOSA_DML_ROUTING_V6_ENTRY      pEntry          = (PCOSA_DML_ROUTING_V6_ENTRY    )NULL;

    pEntry = (PCOSA_DML_ROUTING_V6_ENTRY)AnscAllocateMemory(sizeof(COSA_DML_ROUTING_V6_ENTRY));

    if ( !pEntry )
    {
        return NULL;
    }

    _ansc_sprintf(pEntry->Alias, "IPv6Forwarding%d", pRouter->ulNextIPv6ForwardInsNum);

    /* Update the middle layer data */
    if ( TRUE )
    {
        pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

        if ( !pSubCosaContext )
        {
            AnscFreeMemory(pEntry);
            
            return NULL;
        }

        pSubCosaContext->InstanceNumber = pEntry->InstanceNumber = pRouter->ulNextIPv6ForwardInsNum;
        
        pRouter->ulNextIPv6ForwardInsNum++;

        if ( 0 == pRouter->ulNextIPv6ForwardInsNum )
        {
            pRouter->ulNextIPv6ForwardInsNum = 1;
        }

        pSubCosaContext->hContext         = (ANSC_HANDLE)pEntry;
        pSubCosaContext->hParentTable     = pRouter;
        pSubCosaContext->hPoamIrepUpperFo = pMyObject->hIrepFolderRouterHA;
        pSubCosaContext->bNew             = TRUE;

        CosaSListPushEntryByInsNum((PSLIST_HEADER)&pRouter->IPv6ForwardList, pSubCosaContext);

        CosaRoutingRegAddInfo
        (
            (ANSC_HANDLE)pMyObject,
            COSA_DML_RR_NAME_ROUTER_IPv6Forward_NextInsNum,
            pRouter->ulNextIPv6ForwardInsNum,
            COSA_DML_RR_NAME_ROUTER_IPv6Forward_Prefix,
            pRouter->Cfg.InstanceNumber,
            pEntry->Alias,
            (ANSC_HANDLE)pSubCosaContext
        );
    }

    *pInsNumber = pSubCosaContext->InstanceNumber;

    return pSubCosaContext;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv6Forwarding_DelEntry
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
IPv6Forwarding_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_ROUTING         pMyObject       = (PCOSA_DATAMODEL_ROUTING       )g_pCosaBEManager->hRouting; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT     )hInsContext;
    PCOSA_DML_ROUTER_FULL2          pRouter         = (PCOSA_DML_ROUTER_FULL2        )pCosaContext->hContext;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT     )hInstance;
    PCOSA_DML_ROUTING_V6_ENTRY      pEntry          = (PCOSA_DML_ROUTING_V6_ENTRY    )pSubCosaContext->hContext;
    
    CosaDmlRoutingDelV6Entry(NULL, pEntry);

    AnscSListPopEntryByLink((PSLIST_HEADER)&pRouter->IPv6ForwardList, &pSubCosaContext->Linkage);

    if ( pSubCosaContext->bNew )
    {
        CosaRoutingRegDelInfo(NULL, (ANSC_HANDLE)pSubCosaContext);
    }

    AnscFreeMemory(pEntry);
    AnscFreeMemory(pSubCosaContext);

    return 0;
}



/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Forwarding_IsUpdated
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/
BOOL
IPv6Forwarding_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    )
{
    static ULONG    last_tick = 0;

    if (!last_tick) 
    {
        last_tick = AnscGetTickInSeconds();
        return TRUE;
    }
    
    if (last_tick >= TIME_NO_NEGATIVE(AnscGetTickInSeconds() - 10))
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
        IPv6Forwarding_Synchronize
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
IPv6Forwarding_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_ROUTING         pRoute        = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT     )hInsContext;
    PCOSA_DML_ROUTER_FULL2          pRouter         = (PCOSA_DML_ROUTER_FULL2        )pCosaContext->hContext;

    ULONG                          *pulTmp        = NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink = NULL;
    PSINGLE_LINK_ENTRY              pSListEntry   = NULL;
    PSINGLE_LINK_ENTRY              pSListEntry2  = NULL;
    PCOSA_DML_ROUTING_V6_ENTRY      pEntry2       = {0};
    PCOSA_DML_ROUTING_V6_ENTRY      pEntry        = NULL;
    ULONG                           entryCount    = 0;
    ULONG                           i             = 0;
    ULONG                           HashValue     = 0;
    BOOL                            bFound        = FALSE;
    
 
    entryCount                      = CosaDmlRoutingGetNumberOfV6Entries(NULL);
    
    pulTmp    =   AnscAllocateMemory( entryCount * sizeof(ULONG) );
    if ( !pulTmp )
    {
        goto EXIT3;
    }

    AnscZeroMemory( pulTmp, entryCount * sizeof(ULONG) );

    pEntry2 = AnscAllocateMemory( entryCount * sizeof(COSA_DML_ROUTING_V6_ENTRY) );
    if ( !pEntry2 )
    {
        goto EXIT2;
    }

    AnscZeroMemory( pEntry2, entryCount * sizeof(COSA_DML_ROUTING_V6_ENTRY) );

    for ( i = 0; i < entryCount; i++)
        if ( CosaDmlRoutingGetV6Entry(NULL, i, &pEntry2[i]) != ANSC_STATUS_SUCCESS ){
            entryCount = i+ 1;
            break;
        }else

    
    /* Search the whole link and mark bFound of exist entry to FALSE */
    pSListEntry =   AnscSListGetFirstEntry(&pRouter->IPv6ForwardList);
    while( pSListEntry )
    {
        pCxtLink          = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSListEntry);        
        pSListEntry2      = pSListEntry;
        pSListEntry       = AnscSListGetNextEntry(pSListEntry);

        pEntry = (PCOSA_DML_ROUTING_V6_ENTRY)pCxtLink->hContext;
        
        /* go over all new entries, find them in current link table and mark them */
        bFound = FALSE;
        for ( i = 0; i < entryCount; i++)
        {
            if ( !pulTmp[i] && ( _ansc_strcmp(pEntry2[i].DestIPPrefix, pEntry->DestIPPrefix) == 0 ) && 
                 ( _ansc_strcmp(pEntry2[i].Interface, pEntry->Interface) == 0 ) ) {
                pulTmp[i] = 1;
                bFound = TRUE;
                break;
            }
        }

        if ( bFound ){
            continue;
        }
        
        /* We  need delete this one no matter whether it's not static*/
        //if ( pEntry->Origin != COSA_DML_ROUTING_IPV6_ORIGIN_Static ){
            AnscSListPopEntryByLink(&pRouter->IPv6ForwardList, pSListEntry2);

            AnscFreeMemory( pCxtLink->hContext );
            AnscFreeMemory( pCxtLink );
        //}

    }

    /* We add new entry into our link table */
    for ( i = 0; i < entryCount; i++ )
    {
        if ( pulTmp[i] == 0 )
        {
            /* Add new one */
            pCxtLink             = AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
            if ( !pCxtLink )
            {
                goto EXIT2;
            }

            pEntry        = AnscAllocateMemory(sizeof(COSA_DML_ROUTING_V6_ENTRY));
            if ( !pEntry )
            {
                goto EXIT1;
            }
            

            /* copy new content which should include InstanceNumber and Alias */
            *pEntry       = pEntry2[i];
            pEntry->InstanceNumber = pRouter->ulNextIPv6ForwardInsNum++;
            
            pCxtLink->hContext   = (ANSC_HANDLE)pEntry;
            pCxtLink->InstanceNumber = pEntry->InstanceNumber;
			pCxtLink->hParentTable = (ANSC_HANDLE)pRouter; //Fix The Crash
            pCxtLink->bNew       = FALSE;

			/* Generate Alias */
			_ansc_sprintf(pEntry->Alias, "IPv6Forwarding%d", pCxtLink->InstanceNumber);
			
			CosaDmlRoutingSetV6EntryValues(NULL,i,pEntry->InstanceNumber,pEntry->Alias);
            CosaSListPushEntryByInsNum(&pRouter->IPv6ForwardList, (PCOSA_CONTEXT_LINK_OBJECT)pCxtLink);
        }
    }

    returnStatus =  ANSC_STATUS_SUCCESS;

    goto EXIT2;

    EXIT1:
        AnscFreeMemory(pCxtLink);

    EXIT2:
        AnscFreeMemory(pulTmp);
	AnscFreeMemory(pEntry2);

    EXIT3:
        
    return returnStatus;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Forwarding_GetParamBoolValue
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
IPv6Forwarding_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext   = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ROUTING_V6_ENTRY      pRouterForward = (PCOSA_DML_ROUTING_V6_ENTRY)pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        *pBool = pRouterForward->Enable;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Forwarding_GetParamIntValue
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
IPv6Forwarding_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext   = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ROUTING_V6_ENTRY      pRouterForward = (PCOSA_DML_ROUTING_V6_ENTRY)pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "ForwardingPolicy") == 0)
    {
        /* collect value */
        *pInt = pRouterForward->ForwardingPolicy;
        return TRUE;
    }

    if (strcmp(ParamName, "ForwardingMetric") == 0)
    {
        /* collect value */
        *pInt = pRouterForward->ForwardingMetric;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Forwarding_GetParamUlongValue
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
IPv6Forwarding_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext   = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ROUTING_V6_ENTRY      pRouterForward = (PCOSA_DML_ROUTING_V6_ENTRY)pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Status") == 0)
    {
        /* collect value */
        /*CosaDmlRoutingGetV6Entry2(NULL, pRouterForward);*/
        *puLong = pRouterForward->Status;
        return TRUE;
    }

    if (strcmp(ParamName, "Origin") == 0)
    {
        /* collect value */
        /*CosaDmlRoutingGetV6Entry2(NULL, pRouterForward);*/
        *puLong = pRouterForward->Origin;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv6Forwarding_GetParamStringValue
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
IPv6Forwarding_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext   = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ROUTING_V6_ENTRY      pRouterForward = (PCOSA_DML_ROUTING_V6_ENTRY)pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* collect value */
        AnscCopyString(pValue, pRouterForward->Alias);
        return 0;
    }

    if (strcmp(ParamName, "DestIPPrefix") == 0)
    {
        /* collect value */
        AnscCopyString(pValue, pRouterForward->DestIPPrefix);
        return 0;
    }

    if (strcmp(ParamName, "NextHop") == 0)
    {
        /* collect value */
        AnscCopyString(pValue, pRouterForward->NextHop);
        return 0;
    }

    if (strcmp(ParamName, "Interface") == 0)
    {
        char                            * pString      = NULL;

        /* collect value */
        pString = CosaUtilGetFullPathNameByKeyword
            (
                "Device.IP.Interface.",
                "Name",
                pRouterForward->Interface
            );
        
        if ( pString )
        {
            if ( AnscSizeOfString(pString) < *pUlSize)
            {
                AnscCopyString(pValue, pString);

                AnscFreeMemory(pString);

                return 0;
            }
            else
            {
                *pUlSize = AnscSizeOfString(pString)+1;

                AnscFreeMemory(pString);
                
                return 1;
            }
        }
        else
        {
            return 0;
        }
    }

    if (strcmp(ParamName, "ExpirationTime") == 0)
    {
        /* collect value */
        AnscCopyString(pValue, pRouterForward->ExpirationTime);
        return 0;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Forwarding_SetParamBoolValue
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
IPv6Forwarding_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext   = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ROUTING_V6_ENTRY      pRouterForward = (PCOSA_DML_ROUTING_V6_ENTRY)pCosaContext->hContext;

    BRIDGE_MODE_JUDGEMENT_IFTRUE_RETURNFALSE

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* save update to backup */
        pRouterForward->Enable = bValue;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Forwarding_SetParamIntValue
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
IPv6Forwarding_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext   = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ROUTING_V6_ENTRY      pRouterForward = (PCOSA_DML_ROUTING_V6_ENTRY)pCosaContext->hContext;

    BRIDGE_MODE_JUDGEMENT_IFTRUE_RETURNFALSE

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "ForwardingPolicy") == 0)
    {
        /* save update to backup */
        pRouterForward->ForwardingPolicy = iValue;
        return TRUE;
    }

    if (strcmp(ParamName, "ForwardingMetric") == 0)
    {
        /* save update to backup */
        pRouterForward->ForwardingMetric = iValue;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Forwarding_SetParamUlongValue
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
IPv6Forwarding_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext   = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;

    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Forwarding_SetParamStringValue
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
IPv6Forwarding_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext   = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ROUTING_V6_ENTRY      pRouterForward = (PCOSA_DML_ROUTING_V6_ENTRY)pCosaContext->hContext;
    ANSC_STATUS ret=ANSC_STATUS_FAILURE;

    BRIDGE_MODE_JUDGEMENT_IFTRUE_RETURNFALSE

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* save update to backup */
        AnscCopyString(pRouterForward->Alias, pString);
        return TRUE;
    }

    if (strcmp(ParamName, "DestIPPrefix") == 0)
    {
        /* save update to backup */
        AnscCopyString(pRouterForward->DestIPPrefix, pString);
        return TRUE;
    }

    if (strcmp(ParamName, "NextHop") == 0)
    {
        /* save update to backup */
        AnscCopyString(pRouterForward->NextHop, pString);
        return TRUE;
    }    

    if (strcmp(ParamName, "Interface") == 0)
    {
	char wrapped_inputparam[64]={0};
	ret=isValidInput(pString,wrapped_inputparam, AnscSizeOfString(pString), sizeof( wrapped_inputparam ));
        if(ANSC_STATUS_SUCCESS != ret)
            return FALSE;
        /* save update to backup */
        AnscCopyString(pRouterForward->Interface, wrapped_inputparam);
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Forwarding_Validate
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
IPv6Forwarding_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT        )hInsContext;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2   = (PCOSA_CONTEXT_LINK_OBJECT        )NULL;
    PCOSA_DML_ROUTER_FULL2          pRouter         = (PCOSA_DML_ROUTER_FULL2           )pCosaContext->hParentTable;
    PCOSA_DML_ROUTING_V6_ENTRY      pRouterForward  = (PCOSA_DML_ROUTING_V6_ENTRY       )pCosaContext->hContext;
    PCOSA_DML_ROUTING_V6_ENTRY      pRouterForward2 = (PCOSA_DML_ROUTING_V6_ENTRY       )NULL;
    PSINGLE_LINK_ENTRY              pLink           = (PSINGLE_LINK_ENTRY               )NULL;

    pLink = AnscSListGetFirstEntry(&pRouter->IPv6ForwardList);

    while ( pLink )
    {
        pCosaContext2 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        pLink   = AnscSListGetNextEntry(pLink);

        pRouterForward2 = (PCOSA_DML_ROUTING_V6_ENTRY)pCosaContext2->hContext;

        if ( pRouterForward2 &&
            ((ULONG)pRouterForward2 != (ULONG)pRouterForward) &&
             AnscEqualString(pRouterForward2->Alias, pRouterForward->Alias, TRUE))
        {
            AnscCopyString(pReturnParamName, "Alias");
            *puLength = AnscSizeOfString("Alias");

            CcspTraceWarning(("IPv6Forwarding_Validate() failed.\n"));            
            return FALSE;
        }
    }

    return TRUE;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv6Forwarding_Commit
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
IPv6Forwarding_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_ROUTING         pMyObject      = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext   = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ROUTING_V6_ENTRY      pRouterForward = (PCOSA_DML_ROUTING_V6_ENTRY)pCosaContext->hContext;

    if ( pCosaContext->bNew )
    {
        pCosaContext->bNew = FALSE;

        CosaDmlRoutingAddV6Entry(NULL, pRouterForward);

        CosaRoutingRegDelInfo(NULL, (ANSC_HANDLE)pCosaContext);
    }
    else
    {
        CosaDmlRoutingSetV6Entry(NULL, pRouterForward);
    }
    
    return 0;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv6Forwarding_Rollback
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
IPv6Forwarding_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext   = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_ROUTING_V6_ENTRY      pRouterForward = (PCOSA_DML_ROUTING_V6_ENTRY)pCosaContext->hContext;

    CosaDmlRoutingGetV6Entry2(NULL, pRouterForward);

    return 0;
}



/***********************************************************************

 APIs for Object:

    Routing.RIP.

    *  RIP_GetParamBoolValue
    *  RIP_GetParamIntValue
    *  RIP_GetParamUlongValue
    *  RIP_GetParamStringValue
    *  RIP_SetParamBoolValue
    *  RIP_SetParamIntValue
    *  RIP_SetParamUlongValue
    *  RIP_SetParamStringValue
    *  RIP_Validate
    *  RIP_Commit
    *  RIP_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        RIP_GetParamBoolValue
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
RIP_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_ROUTING         pMyObject     = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting; 
    PCOSA_DML_RIP_CFG               pCfg          = &(pMyObject->RIPCfg);
   
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        *pBool = pCfg->Enable;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        RIP_GetParamIntValue
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
RIP_GetParamIntValue
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
        RIP_GetParamUlongValue
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
RIP_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_ROUTING         pMyObject     = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting; 
    PCOSA_DML_RIP_CFG               pCfg          = &(pMyObject->RIPCfg);

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "SupportedModes") == 0)
    {
        /* collect value */
        *puLong = pCfg->Mode;
        return TRUE;
    }

    if (strcmp(ParamName, "X_CISCO_COM_UpdateInterval") == 0)
    {
        /* collect value */
        *puLong  = pCfg->X_CISCO_COM_UpdateInterval;
        return TRUE;
    }

    if (strcmp(ParamName, "X_CISCO_COM_DefaultMetric") == 0)
    {
        /* collect value */
        *puLong = pCfg->X_CISCO_COM_DefaultMetric;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        RIP_GetParamStringValue
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
RIP_GetParamStringValue
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
        RIP_SetParamBoolValue
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
RIP_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_DATAMODEL_ROUTING         pMyObject     = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting; 
    PCOSA_DML_RIP_CFG               pCfg          = &(pMyObject->RIPCfg);
    
    BRIDGE_MODE_JUDGEMENT_IFTRUE_RETURNFALSE

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* save update to backup */
        pCfg->Enable = bValue;        
        
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        RIP_SetParamIntValue
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
RIP_SetParamIntValue
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
        RIP_SetParamUlongValue
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
RIP_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_DATAMODEL_ROUTING         pMyObject     = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting; 
    PCOSA_DML_RIP_CFG               pCfg          = &(pMyObject->RIPCfg);

    BRIDGE_MODE_JUDGEMENT_IFTRUE_RETURNFALSE

    /* check the parameter name and set the corresponding value */

    if (strcmp(ParamName, "X_CISCO_COM_UpdateInterval") == 0)
    {
        /* collect value */
        pCfg->X_CISCO_COM_UpdateInterval  = uValue;
        return TRUE;
    }

    if (strcmp(ParamName, "X_CISCO_COM_DefaultMetric") == 0)
    {
        /* collect value */
        pCfg->X_CISCO_COM_DefaultMetric  = uValue;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        RIP_SetParamStringValue
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
RIP_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
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
        RIP_Validate
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
RIP_Validate
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
        RIP_Commit
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
RIP_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_ROUTING         pMyObject     = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting; 
    PCOSA_DML_RIP_CFG               pCfg          = &(pMyObject->RIPCfg);

    CosaDmlRipSetCfg(NULL, pCfg);

    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        RIP_Rollback
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
RIP_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    return 0;
}

/***********************************************************************

 APIs for Object:

    Routing.RIP.InterfaceSetting.{i}.

    *  InterfaceSetting_GetEntryCount
    *  InterfaceSetting_GetEntry
    *  InterfaceSetting_AddEntry
    *  InterfaceSetting_DelEntry
    *  InterfaceSetting_GetParamBoolValue
    *  InterfaceSetting_GetParamIntValue
    *  InterfaceSetting_GetParamUlongValue
    *  InterfaceSetting_GetParamStringValue
    *  InterfaceSetting_SetParamBoolValue
    *  InterfaceSetting_SetParamIntValue
    *  InterfaceSetting_SetParamUlongValue
    *  InterfaceSetting_SetParamStringValue
    *  InterfaceSetting_Validate
    *  InterfaceSetting_Commit
    *  InterfaceSetting_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        InterfaceSetting_GetEntryCount
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
InterfaceSetting_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_ROUTING         pMyObject = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting; 

    return AnscSListQueryDepth(&pMyObject->RipIFList);
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        InterfaceSetting_GetEntry
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
InterfaceSetting_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_ROUTING         pMyObject     = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting; 
    PSLIST_HEADER                   pRipIFHead    = (PSLIST_HEADER)&pMyObject->RipIFList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pLink         = (PSINGLE_LINK_ENTRY       )NULL;

    pLink = AnscSListGetEntryByIndex(pRipIFHead, nIndex);

    if ( pLink )
    {
        pCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        *pInsNumber = pCosaContext->InstanceNumber;
    }
    
    return pCosaContext;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        InterfaceSetting_AddEntry
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
InterfaceSetting_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_ROUTING         pMyObject     = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting; 
    PSLIST_HEADER                   pRipIFHead    = (PSLIST_HEADER)&pMyObject->RipIFList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_RIP_IF_CFG            pRipIF        = (PCOSA_DML_RIP_IF_CFG)NULL;

    return NULL;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        InterfaceSetting_DelEntry
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
InterfaceSetting_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_ROUTING         pMyObject     = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting; 
    PSLIST_HEADER                   pRipIFHead    = (PSLIST_HEADER)&pMyObject->RipIFList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_DML_RIP_IF_CFG            pRipIF        = (PCOSA_DML_RIP_IF_CFG)pCosaContext->hContext;

    return -1;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        InterfaceSetting_GetParamBoolValue
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
InterfaceSetting_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext   = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_RIP_IF_CFG            pRipIF         = (PCOSA_DML_RIP_IF_CFG)pCosaContext->hContext;
    PCOSA_DATAMODEL_ROUTING         pMyObject     = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting; 
    PCOSA_DML_RIP_CFG               pCfg          = &(pMyObject->RIPCfg);

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        *pBool = pRipIF->Enable;
        return TRUE;
    }

    if (strcmp(ParamName, "AcceptRA") == 0)
    {
        /* collect value */
        if(!pCfg->Enable) 
            *pBool = FALSE;
        else
            *pBool = pRipIF->AcceptRA;

        return TRUE;
    }

    if (strcmp(ParamName, "SendRA") == 0)
    {
        /* collect value */
        if(!pCfg->Enable) 
            *pBool = FALSE;
        else
            *pBool = pRipIF->SendRA;        

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        InterfaceSetting_GetParamIntValue
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
InterfaceSetting_GetParamIntValue
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
        InterfaceSetting_GetParamUlongValue
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
InterfaceSetting_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext   = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_RIP_IF_CFG           pRipIF         = (PCOSA_DML_RIP_IF_CFG)pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Status") == 0)
    {
        /* collect value */        
        *puLong = pRipIF->Status;  
        return TRUE;
    }

    if (strcmp(ParamName, "X_CISCO_COM_SendVersion") == 0)
    {
        /* collect value */
        *puLong = (ULONG)pRipIF->X_CISCO_COM_SendVersion;
        return TRUE;
    }

    if (strcmp(ParamName, "X_CISCO_COM_ReceiveVersion") == 0)
    {
        /* collect value */
        *puLong = (ULONG)pRipIF->X_CISCO_COM_ReceiveVersion;
        return TRUE;
    }

    if (strcmp(ParamName, "X_CISCO_COM_Neighbor") == 0)
    {
        /* collect value */
        *puLong = (ULONG)pRipIF->X_CISCO_COM_Neighbor;
        return TRUE;
    }

    if (strcmp(ParamName, "X_CISCO_COM_Md5KeyID") == 0)
    {
        /* collect value */
        *puLong  = pRipIF->X_CISCO_COM_Md5KeyID;
        return TRUE;
    }

    if (strcmp(ParamName, "X_CISCO_COM_AuthenticationType") == 0)
    {
        /* collect value */
        *puLong = pRipIF->X_CISCO_COM_AuthenticationType;
        return TRUE;
    }

    if (strcmp(ParamName, "ReportIPAddressType") == 0)
    {
        /* collect value */
        *puLong = 1; /*Fixed to be Unicast. This one should be from pRipIF->X_CISCO_COM_Neighbor. do this later */
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        InterfaceSetting_GetParamStringValue
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
InterfaceSetting_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext   = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_RIP_IF_CFG           pRipIF         = (PCOSA_DML_RIP_IF_CFG)pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* collect value */
        AnscCopyString(pValue, pRipIF->Alias);
        return 0;
    }

    if (strcmp(ParamName, "Interface") == 0)
    {
        /* collect value */
        if ((_ansc_strcmp(pRipIF->Alias, COSA_RIPD_IF1_NAME ) == 0 ) ||
            (_ansc_strcmp(pRipIF->Alias, "cpe" ) == 0 ) ||
            (_ansc_strcmp(pRipIF->Alias, "Ethernet" ) == 0 ) )
            AnscCopyString(pValue, "Ethernet");
        else
            AnscCopyString(pValue, "Cable");

        return 0;
    }

    if (strcmp(ParamName, "X_CISCO_COM_Md5KeyValue") == 0)
    {
        /* collect value */
        AnscCopyString(pValue, pRipIF->X_CISCO_COM_Md5KeyValue);
        return 0;
    }

    if (strcmp(ParamName, "X_CISCO_COM_SimplePassword") == 0)
    {
        /* collect value */
        AnscCopyString(pValue, pRipIF->X_CISCO_COM_SimplePassword);
        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        InterfaceSetting_SetParamBoolValue
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
InterfaceSetting_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext   = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_RIP_IF_CFG           pRipIF         = (PCOSA_DML_RIP_IF_CFG)pCosaContext->hContext;

    BRIDGE_MODE_JUDGEMENT_IFTRUE_RETURNFALSE

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* save update to backup */
        pRipIF->Enable = bValue;
        return TRUE;
    }

    if (strcmp(ParamName, "AcceptRA") == 0)
    {
        /* save update to backup */
        pRipIF->AcceptRA = bValue;
        return TRUE;
    }

    if (strcmp(ParamName, "SendRA") == 0)
    {
        /* save update to backup */
        pRipIF->SendRA = bValue;       
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        InterfaceSetting_SetParamIntValue
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
InterfaceSetting_SetParamIntValue
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
        InterfaceSetting_SetParamUlongValue
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
InterfaceSetting_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext   = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_RIP_IF_CFG           pRipIF         = (PCOSA_DML_RIP_IF_CFG)pCosaContext->hContext;

    BRIDGE_MODE_JUDGEMENT_IFTRUE_RETURNFALSE

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "X_CISCO_COM_SendVersion") == 0)
    {
        /* save update to backup */
        pRipIF->X_CISCO_COM_SendVersion = uValue;
        return TRUE;
    }

    if (strcmp(ParamName, "X_CISCO_COM_ReceiveVersion") == 0)
    {
        /* save update to backup */
        pRipIF->X_CISCO_COM_ReceiveVersion = uValue;
        return TRUE;
    }

    if (strcmp(ParamName, "X_CISCO_COM_Neighbor") == 0)
    {
        /* save update to backup */
        pRipIF->X_CISCO_COM_Neighbor = uValue;
        return TRUE;
    }
    
    if (strcmp(ParamName, "X_CISCO_COM_AuthenticationType") == 0)
    {
        if ( uValue == COSA_RIP_AUTHEN_md5 )
            pRipIF->X_CISCO_COM_AuthenticationType = (ULONG)COSA_RIP_AUTHEN_MD5;
        else
            pRipIF->X_CISCO_COM_AuthenticationType = uValue;

        return TRUE;
    }

    if (strcmp(ParamName, "X_CISCO_COM_Md5KeyID") == 0)
    {
        pRipIF->X_CISCO_COM_Md5KeyID = uValue;
        return TRUE;
    }
    
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        InterfaceSetting_SetParamStringValue
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
InterfaceSetting_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext   = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_RIP_IF_CFG           pRipIF         = (PCOSA_DML_RIP_IF_CFG)pCosaContext->hContext;

    BRIDGE_MODE_JUDGEMENT_IFTRUE_RETURNFALSE

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* save update to backup */
        AnscCopyString(pRipIF->Alias, pString);
        return TRUE;
    }

    if (strcmp(ParamName, "X_CISCO_COM_Md5KeyValue") == 0)
    {
        /* save update to backup */
        AnscCopyString(pRipIF->X_CISCO_COM_Md5KeyValue, pString);
        return TRUE;
    }

    if (strcmp(ParamName, "X_CISCO_COM_SimplePassword") == 0)
    {
        /* save update to backup */
        AnscCopyString(pRipIF->X_CISCO_COM_SimplePassword, pString);
        return TRUE;
    }

    if (strcmp(ParamName, "Interface") == 0)
    {
        /* save update to backup */
        AnscCopyString(pRipIF->Alias, pString);
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        InterfaceSetting_Validate
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
InterfaceSetting_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_DATAMODEL_ROUTING         pMyObject     = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2 = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_RIP_IF_CFG           pRipIF        = (PCOSA_DML_RIP_IF_CFG)pCosaContext->hContext;
    PCOSA_DML_RIP_IF_CFG           pRipIF2       = (PCOSA_DML_RIP_IF_CFG)NULL;
    PSLIST_HEADER                   pRipIFHead    = (PSLIST_HEADER)&pMyObject->RipIFList;
    PSINGLE_LINK_ENTRY              pLink         = (PSINGLE_LINK_ENTRY)NULL;

    return TRUE;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        InterfaceSetting_Commit
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
InterfaceSetting_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_ROUTING         pMyObject    = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_RIP_IF_CFG            pRipIF       = (PCOSA_DML_RIP_IF_CFG)pCosaContext->hContext;

    if ( pCosaContext->bNew )
    {
        pCosaContext->bNew = FALSE;

        CosaDmlRipIfAddCfg(NULL, pRipIF);

        CosaRoutingRegDelInfo(NULL, (ANSC_HANDLE)pCosaContext);
    }
    else
    {
        CosaDmlRipIfSetCfg(NULL, pRipIF);
    }
    
    return 0;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        InterfaceSetting_Rollback
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
InterfaceSetting_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{   
    PCOSA_DATAMODEL_ROUTING         pMyObject    = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_RIP_IF_CFG           pRipIF       = (PCOSA_DML_RIP_IF_CFG)pCosaContext->hContext;

    CcspTraceWarning(("InterfaceSetting_Rollback()----begin to rollback......\n"));

    CosaDmlRipIfGetCfg(NULL, pRipIF->InstanceNumber-1, pRipIF);

    return 0; 
}

/***********************************************************************

 APIs for Object:

    Routing.RouteInformation.

    *  RouteInformation_GetParamBoolValue
    *  RouteInformation_GetParamIntValue
    *  RouteInformation_GetParamUlongValue
    *  RouteInformation_GetParamStringValue
    *  RouteInformation_SetParamBoolValue
    *  RouteInformation_SetParamIntValue
    *  RouteInformation_SetParamUlongValue
    *  RouteInformation_SetParamStringValue
    *  RouteInformation_Validate
    *  RouteInformation_Commit
    *  RouteInformation_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        RouteInformation_GetParamBoolValue
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
RouteInformation_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_DATAMODEL_ROUTING              pMyObject     = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting;
    PCOSA_DML_ROUTEINFO_CFG              pCfg          = (PCOSA_DML_ROUTEINFO_CFG)&pMyObject->RouteInfo.Cfg;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        CosaDmlRouteInfoGetEnabled(pBool);
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        RouteInformation_GetParamIntValue
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
RouteInformation_GetParamIntValue
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
        RouteInformation_GetParamUlongValue
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
RouteInformation_GetParamUlongValue
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
        RouteInformation_GetParamStringValue
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
RouteInformation_GetParamStringValue
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
        RouteInformation_SetParamBoolValue
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
RouteInformation_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_DATAMODEL_ROUTING              pMyObject     = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting;
    PCOSA_DML_ROUTEINFO_CFG              pCfg          = (PCOSA_DML_ROUTEINFO_CFG)&pMyObject->RouteInfo.Cfg;

    BRIDGE_MODE_JUDGEMENT_IFTRUE_RETURNFALSE

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* save update to backup */
        pCfg->bEnabled = bValue;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        RouteInformation_SetParamIntValue
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
RouteInformation_SetParamIntValue
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
        RouteInformation_SetParamUlongValue
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
RouteInformation_SetParamUlongValue
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
        RouteInformation_SetParamStringValue
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
RouteInformation_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
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
        RouteInformation_Validate
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
RouteInformation_Validate
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
        RouteInformation_Commit
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
RouteInformation_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_ROUTING              pMyObject     = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting;
    PCOSA_DML_ROUTEINFO_CFG              pCfg          = (PCOSA_DML_ROUTEINFO_CFG)&pMyObject->RouteInfo.Cfg;
    
    CosaDmlRouteInfoSetEnabled(pCfg->bEnabled);
    
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        RouteInformation_Rollback
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
RouteInformation_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_ROUTING              pMyObject     = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting;
    PCOSA_DML_ROUTEINFO_CFG              pCfg          = (PCOSA_DML_ROUTEINFO_CFG)&pMyObject->RouteInfo.Cfg;

    CosaDmlRouteInfoGetEnabled(&pCfg->bEnabled);
    
    return 0;
}

/***********************************************************************

 APIs for Object:

    Routing.RouteInformation.InterfaceSetting.{i}.

    *  InterfaceSetting3_GetEntryCount
    *  InterfaceSetting3_GetEntry
    *  InterfaceSetting3_IsUpdated
    *  InterfaceSetting3_Synchronize
    *  InterfaceSetting3_GetParamBoolValue
    *  InterfaceSetting3_GetParamIntValue
    *  InterfaceSetting3_GetParamUlongValue
    *  InterfaceSetting3_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        InterfaceSetting3_GetEntryCount
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
InterfaceSetting3_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_ROUTING              pMyObject     = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting;
    
    return pMyObject->RouteInfo.IfNum;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        InterfaceSetting3_GetEntry
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
InterfaceSetting3_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_ROUTING            pMyObject     = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting;
    ULONG                              entryCount    = 0;
    PCOSA_DML_ROUTEINFO_IF_INFO        pEntry        = (PCOSA_DML_ROUTEINFO_IF_INFO)pMyObject->RouteInfo.pInfo;

    entryCount   = pMyObject->RouteInfo.IfNum;
    *pInsNumber  = nIndex + 1; 

    if( nIndex > (entryCount - 1))
    {
        return NULL;
    }
    
    return (ANSC_HANDLE)&pEntry[nIndex];
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        InterfaceSetting3_IsUpdated
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/
BOOL
InterfaceSetting3_IsUpdated
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
        InterfaceSetting3_Synchronize
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
InterfaceSetting3_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_ROUTING         pMyObject     = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting;
    PCOSA_DML_ROUTEINFO_IF_INFO     pEntries      = (PCOSA_DML_ROUTEINFO_IF_INFO)NULL;
    ULONG                           Count         = 0;
    
    pEntries                   = CosaDmlRoutingGetRouteInfoIf(NULL,&Count);
    pMyObject->RouteInfo.IfNum = Count;
    
    if( pMyObject->RouteInfo.pInfo )
    {
        AnscFreeMemory(pMyObject->RouteInfo.pInfo);
    }
    
    if (Count && pEntries)
    {

        pMyObject->RouteInfo.pInfo     = (PCOSA_DML_ROUTEINFO_IF_INFO)
            AnscAllocateMemory(sizeof(COSA_DML_ROUTEINFO_IF_INFO)*Count);
    
        if( !pMyObject->RouteInfo.pInfo )
        {
            return ANSC_STATUS_RESOURCES;
        }
    

        AnscCopyMemory(pMyObject->RouteInfo.pInfo,pEntries,sizeof(COSA_DML_ROUTEINFO_IF_INFO)*Count);
    }
    /*CID: 125174 Resource leak*/
    if (pEntries != NULL)
        AnscFreeMemory(pEntries);

    return returnStatus;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        InterfaceSetting3_GetParamBoolValue
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
InterfaceSetting3_GetParamBoolValue
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
        InterfaceSetting3_GetParamIntValue
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
InterfaceSetting3_GetParamIntValue
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
        InterfaceSetting3_GetParamUlongValue
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
InterfaceSetting3_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DML_ROUTEINFO_IF_INFO        pEntry   = (PCOSA_DML_ROUTEINFO_IF_INFO)hInsContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Status") == 0)
    {
        /* collect value */
        *puLong = pEntry->Status;
        return TRUE;
    }

    if (strcmp(ParamName, "PreferredRouteFlag") == 0)
    {
        /* collect value */
        *puLong = pEntry->PreferredRouteFlag;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        InterfaceSetting3_GetParamStringValue
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
InterfaceSetting3_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_DML_ROUTEINFO_IF_INFO     pEntry   = (PCOSA_DML_ROUTEINFO_IF_INFO)hInsContext;
    PUCHAR                          pString = NULL;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Interface") == 0)
    {
        /* collect value */
#ifdef _COSA_DRG_CNS_
        pString = CosaUtilGetFullPathNameByKeyword
            (
                "Device.IP.Interface.",
                "Name",
                COSA_DML_ROUTEINFO_IFNAME
                );
        if (pString)
        {
            AnscCopyString(pValue, pString);
            AnscCopyString(pEntry->Interface, pString);
            AnscFreeMemory(pString);
        }
        
        return 0;
#endif

        AnscCopyString(pValue, pEntry->Interface);
        return 0;
    }

    if (strcmp(ParamName, "SourceRouter") == 0)
    {
        /* collect value */
        AnscCopyString(pValue, pEntry->SourceRouter);
        return 0;
    }

    if (strcmp(ParamName, "Prefix") == 0)
    {
        /* collect value */
        AnscCopyString(pValue, pEntry->Prefix);
        return 0;
    }

    if (strcmp(ParamName, "RouteLifetime") == 0)
    {
        /* collect value */
        AnscCopyString(pValue, pEntry->RouteLifetime);
        return 0;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}


