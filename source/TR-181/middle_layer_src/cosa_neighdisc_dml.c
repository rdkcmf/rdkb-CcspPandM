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

    module: cosa_apis_neighbordiscovery.c

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

        10/11/2011    initial revision.

**************************************************************************/

#include "ansc_platform.h"
#include "cosa_neighdisc_apis.h"
#include "cosa_neighdisc_internal.h"
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
/***********************************************************************

 APIs for Object:

    NeighborDiscovery.

    *  NeighborDiscovery_GetParamBoolValue
    *  NeighborDiscovery_GetParamIntValue
    *  NeighborDiscovery_GetParamUlongValue
    *  NeighborDiscovery_GetParamStringValue
    *  NeighborDiscovery_SetParamBoolValue
    *  NeighborDiscovery_SetParamIntValue
    *  NeighborDiscovery_SetParamUlongValue
    *  NeighborDiscovery_SetParamStringValue
    *  NeighborDiscovery_Validate
    *  NeighborDiscovery_Commit
    *  NeighborDiscovery_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        NeighborDiscovery_GetParamBoolValue
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
NeighborDiscovery_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_NEIGHDISC pEntry = (PCOSA_DATAMODEL_NEIGHDISC)g_pCosaBEManager->hNeighdisc;
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        CosaDmlNeighdiscGetEnabled(&pEntry->bEnabled);
        
        *pBool = pEntry->bEnabled;

        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        NeighborDiscovery_GetParamIntValue
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
NeighborDiscovery_GetParamIntValue
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
        NeighborDiscovery_GetParamUlongValue
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
NeighborDiscovery_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(puLong);
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        NeighborDiscovery_GetParamStringValue
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
NeighborDiscovery_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pValue);
    UNREFERENCED_PARAMETER(pUlSize);
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        NeighborDiscovery_SetParamBoolValue
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
NeighborDiscovery_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_NEIGHDISC pEntry = (PCOSA_DATAMODEL_NEIGHDISC)g_pCosaBEManager->hNeighdisc;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* save update to backup */

        pEntry->bEnabled = bValue; 

        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        NeighborDiscovery_SetParamIntValue
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
NeighborDiscovery_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(iValue);
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        NeighborDiscovery_SetParamUlongValue
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
NeighborDiscovery_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(uValue);
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        NeighborDiscovery_SetParamStringValue
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
NeighborDiscovery_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pString);
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        NeighborDiscovery_Validate
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
NeighborDiscovery_Validate
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
        NeighborDiscovery_Commit
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
NeighborDiscovery_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_NEIGHDISC pEntry = (PCOSA_DATAMODEL_NEIGHDISC)g_pCosaBEManager->hNeighdisc;

    CosaDmlNeighdiscSetEnabled(pEntry->bEnabled);

    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        NeighborDiscovery_Rollback
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
NeighborDiscovery_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_NEIGHDISC pEntry = (PCOSA_DATAMODEL_NEIGHDISC)g_pCosaBEManager->hNeighdisc;

    CosaDmlNeighdiscGetEnabled(&pEntry->bEnabled);

    return 0;
}

/***********************************************************************

 APIs for Object:

    NeighborDiscovery.InterfaceSetting.{i}.

    *  InterfaceSetting2_GetEntryCount
    *  InterfaceSetting2_GetEntry
    *  InterfaceSetting2_AddEntry
    *  InterfaceSetting2_DelEntry
    *  InterfaceSetting2_GetParamBoolValue
    *  InterfaceSetting2_GetParamIntValue
    *  InterfaceSetting2_GetParamUlongValue
    *  InterfaceSetting2_GetParamStringValue
    *  InterfaceSetting2_SetParamBoolValue
    *  InterfaceSetting2_SetParamIntValue
    *  InterfaceSetting2_SetParamUlongValue
    *  InterfaceSetting2_SetParamStringValue
    *  InterfaceSetting2_Validate
    *  InterfaceSetting2_Commit
    *  InterfaceSetting2_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        InterfaceSetting2_GetEntryCount
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
InterfaceSetting2_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_NEIGHDISC              pMyObject = (PCOSA_DATAMODEL_NEIGHDISC)g_pCosaBEManager->hNeighdisc;

    return AnscSListQueryDepth(&pMyObject->InterfaceList);
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        InterfaceSetting2_GetEntry
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
InterfaceSetting2_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_NEIGHDISC       pMyObject     = (PCOSA_DATAMODEL_NEIGHDISC)g_pCosaBEManager->hNeighdisc; 
    PSLIST_HEADER                   pNeighdiscIFHead     = (PSLIST_HEADER)&pMyObject->InterfaceList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pLink         = (PSINGLE_LINK_ENTRY       )NULL;

    pLink = AnscSListGetEntryByIndex(pNeighdiscIFHead, nIndex);

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
        InterfaceSetting2_AddEntry
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
InterfaceSetting2_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_NEIGHDISC       pMyObject            = (PCOSA_DATAMODEL_NEIGHDISC)g_pCosaBEManager->hNeighdisc; 
    PSLIST_HEADER                   pNeighdiscIFHead     = (PSLIST_HEADER)&pMyObject->InterfaceList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext         = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_NEIGHDISC_IF_FULL     pNeighdiscInterface  = (PCOSA_DML_NEIGHDISC_IF_FULL)NULL;
    errno_t                         rc                   = -1;


    pNeighdiscInterface = (PCOSA_DML_NEIGHDISC_IF_FULL)AnscAllocateMemory(sizeof(COSA_DML_NEIGHDISC_IF_FULL));

    if ( !pNeighdiscInterface )
    {
        return NULL;
    }

    rc = sprintf_s(pNeighdiscInterface->Cfg.Alias, sizeof(pNeighdiscInterface->Cfg.Alias),"Interface%d", (int)pMyObject->ulNextInterfaceInsNum);
    if(rc < EOK)
    {
      ERR_CHK(rc);
      AnscFreeMemory(pNeighdiscInterface);
      return NULL;
    }

    if ( TRUE )
    {
        pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

        if ( !pCosaContext )
        {
            AnscFreeMemory(pNeighdiscInterface);
            
            return NULL;
        }

        pCosaContext->InstanceNumber = pNeighdiscInterface->Cfg.InstanceNumber = pMyObject->ulNextInterfaceInsNum;

        pMyObject->ulNextInterfaceInsNum++;

        if ( 0 == pMyObject->ulNextInterfaceInsNum )
        {
            pMyObject->ulNextInterfaceInsNum = 1;
        }

        pCosaContext->hContext         = (ANSC_HANDLE)pNeighdiscInterface;
        pCosaContext->hParentTable     = NULL;
        pCosaContext->hPoamIrepUpperFo = pMyObject->hIrepFolderNeighdiscInterfaceHA;
        pCosaContext->bNew             = TRUE;

        CosaSListPushEntryByInsNum(pNeighdiscIFHead, pCosaContext);

        CosaNeighdiscRegAddInfo
        (
            (ANSC_HANDLE)pMyObject,
            COSA_DML_RR_NAME_NeighdiscIF_NextInsNum,
            pMyObject->ulNextInterfaceInsNum,
            COSA_DML_RR_NAME_NeighdiscIF_Prefix,
            0,
            pNeighdiscInterface->Cfg.Alias,
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
        InterfaceSetting2_DelEntry
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
InterfaceSetting2_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_NEIGHDISC       pMyObject     = (PCOSA_DATAMODEL_NEIGHDISC)g_pCosaBEManager->hNeighdisc; 
    PSLIST_HEADER                   pNeighdiscIFHead     = (PSLIST_HEADER)&pMyObject->InterfaceList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_DML_NEIGHDISC_IF_FULL     pNeighdiscInterface  = (PCOSA_DML_NEIGHDISC_IF_FULL)pCosaContext->hContext;


    if (!pCosaContext->bNew)
        CosaDmlNeighdiscIfDelEntry(NULL, pNeighdiscInterface->Cfg.InstanceNumber);

    AnscSListPopEntryByLink(pNeighdiscIFHead, &pCosaContext->Linkage);

    if ( pCosaContext->bNew )
    {
        CosaNeighdiscRegDelInfo(NULL, (ANSC_HANDLE)pCosaContext);
    }
    
    AnscFreeMemory(pNeighdiscInterface);
    AnscFreeMemory(pCosaContext);

    return returnStatus;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        InterfaceSetting2_GetParamBoolValue
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
InterfaceSetting2_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_NEIGHDISC_IF_FULL     pNeighdiscInterface = (PCOSA_DML_NEIGHDISC_IF_FULL)pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        *pBool = pNeighdiscInterface->Cfg.bEnabled;
        return TRUE;
    }

    if (strcmp(ParamName, "NUDEnable") == 0)
    {
        /* collect value */
        *pBool = pNeighdiscInterface->Cfg.bNUDEnable;
        return TRUE;
    }

    if (strcmp(ParamName, "RSEnable") == 0)
    {
        /* collect value */
        *pBool = pNeighdiscInterface->Cfg.bRSEnable;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        InterfaceSetting2_GetParamIntValue
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
InterfaceSetting2_GetParamIntValue
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
        InterfaceSetting2_GetParamUlongValue
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
InterfaceSetting2_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_NEIGHDISC_IF_FULL     pNeighdiscInterface = (PCOSA_DML_NEIGHDISC_IF_FULL)pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Status") == 0)
    {
        /* collect value */
        CosaDmlNeighdiscIfGetInfo
            (
                pNeighdiscInterface, 
                pNeighdiscInterface->Cfg.InstanceNumber, 
                (PCOSA_DML_NEIGHDISC_IF_INFO)&pNeighdiscInterface->Info
            );

        *puLong = pNeighdiscInterface->Info.Status;

        return TRUE;
    }

    if (strcmp(ParamName, "RetransTimer") == 0)
    {
        /* collect value */
        *puLong = pNeighdiscInterface->Cfg.RetransTimer;
        return TRUE;
    }

    if (strcmp(ParamName, "RtrSolicitationInterval") == 0)
    {
        /* collect value */
        *puLong = pNeighdiscInterface->Cfg.RtrSolicitationInterval;
        return TRUE;
    }

    if (strcmp(ParamName, "MaxRtrSolicitations") == 0)
    {
        /* collect value */
        *puLong = pNeighdiscInterface->Cfg.MaxRtrSolicitations;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        InterfaceSetting2_GetParamStringValue
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
InterfaceSetting2_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(pUlSize);
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_NEIGHDISC_IF_FULL     pNeighdiscInterface = (PCOSA_DML_NEIGHDISC_IF_FULL)pCosaContext->hContext;
    errno_t                         rc           = -1;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pNeighdiscInterface->Cfg.Alias);
        if ( rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }

    if (strcmp(ParamName, "Interface") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pNeighdiscInterface->Cfg.Interface);
        if ( rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        InterfaceSetting2_SetParamBoolValue
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
InterfaceSetting2_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_NEIGHDISC_IF_FULL     pNeighdiscInterface = (PCOSA_DML_NEIGHDISC_IF_FULL)pCosaContext->hContext;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* save update to backup */

        pNeighdiscInterface ->Cfg.bEnabled = bValue;
        return TRUE;
    }

    if (strcmp(ParamName, "NUDEnable") == 0)
    {
        /* save update to backup */
        pNeighdiscInterface ->Cfg.bNUDEnable = bValue;
        return TRUE;
    }

    if (strcmp(ParamName, "RSEnable") == 0)
    {
        /* save update to backup */
        pNeighdiscInterface ->Cfg.bRSEnable = bValue;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        InterfaceSetting2_SetParamIntValue
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
InterfaceSetting2_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(iValue);

    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        InterfaceSetting2_SetParamUlongValue
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
InterfaceSetting2_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_NEIGHDISC_IF_FULL     pNeighdiscInterface = (PCOSA_DML_NEIGHDISC_IF_FULL)pCosaContext->hContext;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "RetransTimer") == 0)
    {
        /* save update to backup */
        pNeighdiscInterface->Cfg.RetransTimer = uValue;
        return TRUE;
    }

    if (strcmp(ParamName, "RtrSolicitationInterval") == 0)
    {
        /* save update to backup */
        pNeighdiscInterface->Cfg.RtrSolicitationInterval = uValue;
        return TRUE;
    }

    if (strcmp(ParamName, "MaxRtrSolicitations") == 0)
    {
        /* save update to backup */
        pNeighdiscInterface->Cfg.MaxRtrSolicitations = uValue;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        InterfaceSetting2_SetParamStringValue
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
InterfaceSetting2_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_NEIGHDISC_IF_FULL     pNeighdiscInterface = (PCOSA_DML_NEIGHDISC_IF_FULL)pCosaContext->hContext;
    int                             len = 0;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* save update to backup */
        len = (_ansc_strlen(pString) > sizeof(pNeighdiscInterface->Cfg.Alias)-1 ? sizeof(pNeighdiscInterface->Cfg.Alias)-1 : _ansc_strlen(pString));
        
        AnscZeroMemory(pNeighdiscInterface->Cfg.Alias, sizeof(pNeighdiscInterface->Cfg.Alias));
        _ansc_strncpy(pNeighdiscInterface->Cfg.Alias, pString, len);

        return TRUE;
    }

    if (strcmp(ParamName, "Interface") == 0)
    {
        /* save update to backup */

        len = (_ansc_strlen(pString) > sizeof(pNeighdiscInterface->Cfg.Interface)-1 ? sizeof(pNeighdiscInterface->Cfg.Interface)-1 : _ansc_strlen(pString));
        
        AnscZeroMemory(pNeighdiscInterface->Cfg.Interface, sizeof(pNeighdiscInterface->Cfg.Interface));
        _ansc_strncpy(pNeighdiscInterface->Cfg.Interface, pString, len);

        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        InterfaceSetting2_Validate
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
InterfaceSetting2_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_DATAMODEL_NEIGHDISC              pMyObject     = (PCOSA_DATAMODEL_NEIGHDISC)g_pCosaBEManager->hNeighdisc; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2 = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_NEIGHDISC_IF_FULL           pNeighdiscInterface  = (PCOSA_DML_NEIGHDISC_IF_FULL)pCosaContext->hContext;
    PCOSA_DML_NEIGHDISC_IF_FULL           pNeighdiscInterface2 = (PCOSA_DML_NEIGHDISC_IF_FULL)NULL;
    PSLIST_HEADER                   pNeighdiscIFHead     = (PSLIST_HEADER)&pMyObject->InterfaceList;
    PSINGLE_LINK_ENTRY              pLink         = (PSINGLE_LINK_ENTRY)NULL;
    errno_t                         rc            = -1;

    pLink = AnscSListGetFirstEntry(pNeighdiscIFHead);

    while ( pLink )
    {
        pCosaContext2 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        pLink   = AnscSListGetNextEntry(pLink);

        pNeighdiscInterface2 = (PCOSA_DML_NEIGHDISC_IF_FULL)pCosaContext2->hContext;

        if ( pNeighdiscInterface2 && 
            ((ULONG)pNeighdiscInterface2 != (ULONG)pNeighdiscInterface) &&
             (strcmp(pNeighdiscInterface2->Cfg.Alias, pNeighdiscInterface->Cfg.Alias) == 0))
        {
            rc = strcpy_s(pReturnParamName, *puLength, "Alias");
            if(rc != EOK)
            {
                ERR_CHK(rc);
                return FALSE;
            }
            *puLength = AnscSizeOfString("Alias")+1;

            CcspTraceWarning(("InterfaceSetting1_Validate() failed.\n"));
            return FALSE;
        }
    }

    if (!pNeighdiscInterface->Cfg.RetransTimer)
    {
        rc = strcpy_s(pReturnParamName, *puLength, "RetransTimer");
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
        *puLength = AnscSizeOfString("RetransTimer")+1;

        return FALSE;
    }

    if (!pNeighdiscInterface->Cfg.RtrSolicitationInterval)
    {
        rc = strcpy_s(pReturnParamName, *puLength, "RtrSolicitationInterval");
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
        *puLength = AnscSizeOfString("RtrSolicitationInterval")+1;

        return FALSE;
    }

    if (!pNeighdiscInterface->Cfg.MaxRtrSolicitations)
    {
        rc = strcpy_s(pReturnParamName, *puLength, "MaxRtrSolicitations");
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
        *puLength = AnscSizeOfString("MaxRtrSolicitations")+1;

        return FALSE;
    }

    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        InterfaceSetting2_Commit
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
InterfaceSetting2_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_NEIGHDISC_IF_FULL     pNeighdiscInterface = (PCOSA_DML_NEIGHDISC_IF_FULL)pCosaContext->hContext;

    if ( pCosaContext->bNew )
    {
        pCosaContext->bNew = FALSE;

        CosaDmlNeighdiscIfAddEntry(NULL, (PCOSA_DML_NEIGHDISC_IF_FULL)pNeighdiscInterface);

        CosaNeighdiscRegDelInfo(NULL, (ANSC_HANDLE)pCosaContext);
    }
    else
    {
        CosaDmlNeighdiscIfSetCfg(pNeighdiscInterface, (PCOSA_DML_NEIGHDISC_IF_CFG)&pNeighdiscInterface->Cfg);
    }

    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        InterfaceSetting2_Rollback
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
InterfaceSetting2_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_NEIGHDISC_IF_FULL     pNeighdiscInterface = (PCOSA_DML_NEIGHDISC_IF_FULL)pCosaContext->hContext;

    CosaDmlNeighdiscIfGetCfg (pNeighdiscInterface, (PCOSA_DML_NEIGHDISC_IF_CFG)&pNeighdiscInterface->Cfg);
    CosaDmlNeighdiscIfGetInfo(pNeighdiscInterface, pNeighdiscInterface->Cfg.InstanceNumber, (PCOSA_DML_NEIGHDISC_IF_INFO)&pNeighdiscInterface->Info);

    return 0;
}


/***********************************************************************

 APIs for Object:

    X_CISCO_COM_MLD.Group.{i}.

    *  Group_GetEntryCount
    *  Group_GetEntry
    *  Group_IsUpdated
    *  Group_Synchronize
    *  Group_GetParamBoolValue
    *  Group_GetParamIntValue
    *  Group_GetParamUlongValue
    *  Group_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        NeighborTable_GetEntryCount
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
NeighborTable_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_NEIGHDISC       pMyObject     = (PCOSA_DATAMODEL_NEIGHDISC)g_pCosaBEManager->hNeighdisc; 
    
    return pMyObject->ulNbTblNumber;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        NeighborTable_GetEntry
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
NeighborTable_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_NEIGHDISC       pMyObject     = (PCOSA_DATAMODEL_NEIGHDISC)g_pCosaBEManager->hNeighdisc; 

    if (nIndex < pMyObject->ulNbTblNumber)
    {
        *pInsNumber  = nIndex + 1;

        return &pMyObject->pNbTbl[nIndex];
    }

    return NULL;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
       NeighborTable_IsUpdated
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/

#define REFRESH_INTERVAL 60
#define TIME_NO_NEGATIVE(x) ((long)(x) < 0 ? 0 : (x))

BOOL
NeighborTable_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    static ULONG last_tick;

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
        NeighborTable_Synchronize
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
NeighborTable_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_NEIGHDISC       pMyObject     = (PCOSA_DATAMODEL_NEIGHDISC)g_pCosaBEManager->hNeighdisc; 
    
    CosaDmlNeighborTableGetEntry
        (
            (ANSC_HANDLE)NULL, 
            &pMyObject->ulNbTblNumber,
            &pMyObject->pNbTbl
        );
    
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        NeighborTable_GetParamUlongValue
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
NeighborTable_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DML_NEIGHTABLE_INFO       pNbTblEntry = (PCOSA_DML_NEIGHTABLE_INFO)hInsContext;
    
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Status") == 0)
    {
        /* collect value */
        *puLong = pNbTblEntry->Status;
        return TRUE;
    }

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
NeighborTable_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pUlSize);
    PCOSA_DML_NEIGHTABLE_INFO       pNbTblEntry = (PCOSA_DML_NEIGHTABLE_INFO)hInsContext;
    errno_t                         rc            = -1;

    /* check the parameter name and return the corresponding value */    
    if (strcmp(ParamName, "Address") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pNbTblEntry->Address);
        if ( rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
   
    if (strcmp(ParamName, "Interface") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize,  pNbTblEntry->Interface);
        if ( rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }

    if (strcmp(ParamName, "MACAddress") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize,  pNbTblEntry->MACAddress);
        if ( rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }

    return -1;
}


