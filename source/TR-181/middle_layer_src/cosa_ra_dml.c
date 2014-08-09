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

    module: cosa_ra_dml.c

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

        10/09/2011    initial revision.

**************************************************************************/

#include "ansc_platform.h"
#include "plugin_main_apis.h"
#include "cosa_ra_apis.h"
#include "cosa_ra_dml.h"
#include "cosa_ra_internal.h"

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

    RouterAdvertisement.

    *  RouterAdvertisement_GetParamBoolValue
    *  RouterAdvertisement_GetParamIntValue
    *  RouterAdvertisement_GetParamUlongValue
    *  RouterAdvertisement_GetParamStringValue
    *  RouterAdvertisement_SetParamBoolValue
    *  RouterAdvertisement_SetParamIntValue
    *  RouterAdvertisement_SetParamUlongValue
    *  RouterAdvertisement_SetParamStringValue
    *  RouterAdvertisement_Validate
    *  RouterAdvertisement_Commit
    *  RouterAdvertisement_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        RouterAdvertisement_GetParamBoolValue
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
RouterAdvertisement_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_DATAMODEL_RA pEntry = (PCOSA_DATAMODEL_RA)g_pCosaBEManager->hRA;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE) )
    {
        /* collect value */
        CosaDmlRAGetEnabled(&pEntry->bEnabled);
        
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
        RouterAdvertisement_GetParamIntValue
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
RouterAdvertisement_GetParamIntValue
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
        RouterAdvertisement_GetParamUlongValue
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
RouterAdvertisement_GetParamUlongValue
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
        RouterAdvertisement_GetParamStringValue
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
RouterAdvertisement_GetParamStringValue
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
        RouterAdvertisement_SetParamBoolValue
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
RouterAdvertisement_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_DATAMODEL_RA pEntry = (PCOSA_DATAMODEL_RA)g_pCosaBEManager->hRA;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE) )
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
        RouterAdvertisement_SetParamIntValue
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
RouterAdvertisement_SetParamIntValue
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
        RouterAdvertisement_SetParamUlongValue
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
RouterAdvertisement_SetParamUlongValue
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
        RouterAdvertisement_SetParamStringValue
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
RouterAdvertisement_SetParamStringValue
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
        RouterAdvertisement_Validate
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
RouterAdvertisement_Validate
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
        RouterAdvertisement_Commit
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
RouterAdvertisement_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_RA pEntry = (PCOSA_DATAMODEL_RA)g_pCosaBEManager->hRA;

    CosaDmlRASetEnabled(pEntry->bEnabled);

    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        RouterAdvertisement_Rollback
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
RouterAdvertisement_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_RA pEntry = (PCOSA_DATAMODEL_RA)g_pCosaBEManager->hRA;

    CosaDmlRAGetEnabled(&pEntry->bEnabled);
    
    return 0;
}



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

    InterfaceSetting.{i}.

    *  InterfaceSetting1_GetEntryCount
    *  InterfaceSetting1_GetEntry
    *  InterfaceSetting1_AddEntry
    *  InterfaceSetting1_DelEntry
    *  InterfaceSetting1_GetParamBoolValue
    *  InterfaceSetting1_GetParamIntValue
    *  InterfaceSetting1_GetParamUlongValue
    *  InterfaceSetting1_GetParamStringValue
    *  InterfaceSetting1_SetParamBoolValue
    *  InterfaceSetting1_SetParamIntValue
    *  InterfaceSetting1_SetParamUlongValue
    *  InterfaceSetting1_SetParamStringValue
    *  InterfaceSetting1_Validate
    *  InterfaceSetting1_Commit
    *  InterfaceSetting1_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        InterfaceSetting1_GetEntryCount
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
InterfaceSetting1_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_RA              pMyObject = (PCOSA_DATAMODEL_RA)g_pCosaBEManager->hRA;

    return AnscSListQueryDepth(&pMyObject->InterfaceList);
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        InterfaceSetting1_GetEntry
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
InterfaceSetting1_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_RA              pMyObject     = (PCOSA_DATAMODEL_RA)g_pCosaBEManager->hRA; 
    PSLIST_HEADER                   pRAIFHead     = (PSLIST_HEADER)&pMyObject->InterfaceList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pLink         = (PSINGLE_LINK_ENTRY       )NULL;

    pLink = AnscSListGetEntryByIndex(pRAIFHead, nIndex);

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
        InterfaceSetting1_AddEntry
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
InterfaceSetting1_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_RA              pMyObject     = (PCOSA_DATAMODEL_RA)g_pCosaBEManager->hRA; 
    PSLIST_HEADER                   pRAIFHead     = (PSLIST_HEADER)&pMyObject->InterfaceList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_RA_IF_FULL2           pRAInterface  = (PCOSA_DML_RA_IF_FULL2)NULL;

#ifdef _COSA_DRG_CNS_
    /*not supported*/
    return NULL;
#endif

    pRAInterface = (PCOSA_DML_RA_IF_FULL2)AnscAllocateMemory(sizeof(COSA_DML_RA_IF_FULL2));

    if ( !pRAInterface )
    {
        return NULL;
    }

    _ansc_sprintf(pRAInterface->Cfg.Alias, "Interface%d", (int)pMyObject->ulNextInterfaceInsNum); 

    AnscSListInitializeHeader(&pRAInterface->OptionList);
    
    pRAInterface->ulNextOptionInsNum = 1;
    
    if ( TRUE )
    {
        pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

        if ( !pCosaContext )
        {
            AnscFreeMemory(pRAInterface);
            
            return NULL;
        }

        pCosaContext->InstanceNumber = pRAInterface->Cfg.InstanceNumber = pMyObject->ulNextInterfaceInsNum;

        pMyObject->ulNextInterfaceInsNum++;

        if ( 0 == pMyObject->ulNextInterfaceInsNum )
        {
            pMyObject->ulNextInterfaceInsNum = 1;
        }

        pCosaContext->hContext         = (ANSC_HANDLE)pRAInterface;
        pCosaContext->hParentTable     = NULL;
        pCosaContext->hPoamIrepUpperFo = pMyObject->hIrepFolderRAInterfaceHA;
        pCosaContext->bNew             = TRUE;

        CosaSListPushEntryByInsNum(pRAIFHead, pCosaContext);

        CosaRARegAddInfo
        (
            (ANSC_HANDLE)pMyObject,
            COSA_DML_RR_NAME_RAIF_NextInsNum,
            pMyObject->ulNextInterfaceInsNum,
            COSA_DML_RR_NAME_RAIF_Prefix,
            0,
            pRAInterface->Cfg.Alias,
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
        InterfaceSetting1_DelEntry
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
InterfaceSetting1_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_RA              pMyObject     = (PCOSA_DATAMODEL_RA)g_pCosaBEManager->hRA; 
    PSLIST_HEADER                   pRAIFHead     = (PSLIST_HEADER)&pMyObject->InterfaceList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_DML_RA_IF_FULL2           pRAInterface  = (PCOSA_DML_RA_IF_FULL2)pCosaContext->hContext;

#ifdef _COSA_DRG_CNS_
    /*not supported*/
    return ANSC_STATUS_FAILURE;
        
#endif

    if (!pCosaContext->bNew)
        CosaDmlRaIfDelEntry(NULL, pRAInterface->Cfg.InstanceNumber);

    AnscSListPopEntryByLink(pRAIFHead, &pCosaContext->Linkage);

    if ( pCosaContext->bNew )
    {
        CosaRARegDelInfo(NULL, (ANSC_HANDLE)pCosaContext);
    }
    
    AnscFreeMemory(pRAInterface);
    AnscFreeMemory(pCosaContext);

    return returnStatus;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        InterfaceSetting1_GetParamBoolValue
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
InterfaceSetting1_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_RA_IF_FULL2           pRAInterface = (PCOSA_DML_RA_IF_FULL2)pCosaContext->hContext;

    CosaDmlRaIfGetCfg (pRAInterface, (PCOSA_DML_RA_IF_CFG)&pRAInterface->Cfg);

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE) )
    {
        /* collect value */
        *pBool = pRAInterface->Cfg.bEnabled;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "AdvManagedFlag", TRUE) )
    {
        /* collect value */
        *pBool = pRAInterface->Cfg.bAdvManagedFlag;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "AdvOtherConfigFlag", TRUE) )
    {
        /* collect value */
        *pBool = pRAInterface->Cfg.bAdvOtherConfigFlag;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "AdvMobileAgentFlag", TRUE) )
    {
        /* collect value */
        *pBool = pRAInterface->Cfg.bAdvMobileAgentFlag;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "AdvNDProxyFlag", TRUE) )
    {
        /* collect value */
        *pBool = pRAInterface->Cfg.bAdvNDProxyFlag;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        InterfaceSetting1_GetParamIntValue
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
InterfaceSetting1_GetParamIntValue
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
        InterfaceSetting1_GetParamUlongValue
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
InterfaceSetting1_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_RA_IF_FULL2           pRAInterface = (PCOSA_DML_RA_IF_FULL2)pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Status", TRUE) )
    {
        /* collect value */
        CosaDmlRaIfGetInfo
            (
                pRAInterface, 
                pRAInterface->Cfg.InstanceNumber, 
                (PCOSA_DML_RA_IF_INFO)&pRAInterface->Info
            );

        *puLong = pRAInterface->Info.Status;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "MaxRtrAdvInterval", TRUE) )
    {
        CosaDmlRaIfGetCfg (pRAInterface, (PCOSA_DML_RA_IF_CFG)&pRAInterface->Cfg);

        /* collect value */
        *puLong = pRAInterface->Cfg.MaxRtrAdvInterval;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "MinRtrAdvInterval", TRUE) )
    {
        CosaDmlRaIfGetCfg (pRAInterface, (PCOSA_DML_RA_IF_CFG)&pRAInterface->Cfg);

        /* collect value */
        *puLong = pRAInterface->Cfg.MinRtrAdvInterval;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "AdvDefaultLifetime", TRUE) )
    {
        CosaDmlRaIfGetCfg (pRAInterface, (PCOSA_DML_RA_IF_CFG)&pRAInterface->Cfg);

        /* collect value */
        *puLong = pRAInterface->Cfg.AdvDefaultLifetime;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "AdvPreferredRouterFlag", TRUE) )
    {
        CosaDmlRaIfGetCfg (pRAInterface, (PCOSA_DML_RA_IF_CFG)&pRAInterface->Cfg);

        /* collect value */
        *puLong = pRAInterface->Cfg.AdvPreferredRouterFlag;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "AdvLinkMTU", TRUE) )
    {
        CosaDmlRaIfGetCfg (pRAInterface, (PCOSA_DML_RA_IF_CFG)&pRAInterface->Cfg);

        /* collect value */
        *puLong = pRAInterface->Cfg.AdvLinkMTU;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "AdvReachableTime", TRUE) )
    {
        CosaDmlRaIfGetCfg (pRAInterface, (PCOSA_DML_RA_IF_CFG)&pRAInterface->Cfg);

        /* collect value */
        *puLong = pRAInterface->Cfg.AdvReachableTime;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "AdvRetransTimer", TRUE) )
    {
        CosaDmlRaIfGetCfg (pRAInterface, (PCOSA_DML_RA_IF_CFG)&pRAInterface->Cfg);

        /* collect value */
        *puLong = pRAInterface->Cfg.AdvRetransTimer;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "AdvCurHopLimit", TRUE) )
    {
        CosaDmlRaIfGetCfg (pRAInterface, (PCOSA_DML_RA_IF_CFG)&pRAInterface->Cfg);

        /* collect value */
        *puLong = pRAInterface->Cfg.AdvCurHopLimit;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        InterfaceSetting1_GetParamStringValue
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
InterfaceSetting1_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_RA_IF_FULL2           pRAInterface = (PCOSA_DML_RA_IF_FULL2)pCosaContext->hContext;
    PUCHAR                          pString = NULL;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        CosaDmlRaIfGetCfg (pRAInterface, (PCOSA_DML_RA_IF_CFG)&pRAInterface->Cfg);
        /* collect value */
        AnscCopyString(pValue, pRAInterface->Cfg.Alias);
        return 0;
    }

    if( AnscEqualString(ParamName, "Interface", TRUE) )
    {
        /* collect value */
#ifdef _COSA_DRG_CNS_
        pString = CosaUtilGetFullPathNameByKeyword
            (
                "Device.IP.Interface.",
                "Name",
                "brlan0"
                );
        if (pString)
        {
            AnscCopyString(pValue, pString);
            AnscCopyString(pRAInterface->Cfg.Interface, pString);
            AnscFreeMemory(pString);
        }
        
        return 0;
#endif

        CosaDmlRaIfGetCfg (pRAInterface, (PCOSA_DML_RA_IF_CFG)&pRAInterface->Cfg);
        AnscCopyString(pValue, pRAInterface->Cfg.Interface);
        return 0;
    }

    if( AnscEqualString(ParamName, "ManualPrefixes", TRUE) )
    {
        CosaDmlRaIfGetCfg (pRAInterface, (PCOSA_DML_RA_IF_CFG)&pRAInterface->Cfg);
        /* collect value */
        AnscCopyString(pValue, pRAInterface->Cfg.ManualPrefixes);
        return 0;
    }

    if( AnscEqualString(ParamName, "Prefixes", TRUE) )
    {
        /* collect value */
        CosaDmlRaIfGetInfo
            (
                pRAInterface, 
                pRAInterface->Cfg.InstanceNumber, 
                (PCOSA_DML_RA_IF_INFO)&pRAInterface->Info
            );

        AnscCopyString(pValue, pRAInterface->Info.Prefixes);
        return 0;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        InterfaceSetting1_SetParamBoolValue
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
InterfaceSetting1_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_RA_IF_FULL2           pRAInterface = (PCOSA_DML_RA_IF_FULL2)pCosaContext->hContext;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE) )
    {
        /* save update to backup */
        pRAInterface ->Cfg.bEnabled = bValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "AdvManagedFlag", TRUE) )
    {
        /* save update to backup */
        pRAInterface ->Cfg.bAdvManagedFlag = bValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "AdvOtherConfigFlag", TRUE) )
    {
        /* save update to backup */
        pRAInterface ->Cfg.bAdvOtherConfigFlag = bValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "AdvMobileAgentFlag", TRUE) )
    {
        /* save update to backup */
        pRAInterface ->Cfg.bAdvMobileAgentFlag = bValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "AdvNDProxyFlag", TRUE) )
    {
        /* save update to backup */
        pRAInterface ->Cfg.bAdvNDProxyFlag = bValue;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        InterfaceSetting1_SetParamIntValue
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
InterfaceSetting1_SetParamIntValue
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
        InterfaceSetting1_SetParamUlongValue
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
InterfaceSetting1_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_RA_IF_FULL2           pRAInterface = (PCOSA_DML_RA_IF_FULL2)pCosaContext->hContext;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "MaxRtrAdvInterval", TRUE) )
    {
        /* save update to backup */
        pRAInterface->Cfg.MaxRtrAdvInterval = uValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "MinRtrAdvInterval", TRUE) )
    {
        /* save update to backup */
        pRAInterface->Cfg.MinRtrAdvInterval = uValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "AdvDefaultLifetime", TRUE) )
    {
        /* save update to backup */
        pRAInterface->Cfg.AdvDefaultLifetime = uValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "AdvPreferredRouterFlag", TRUE) )
    {
        /* save update to backup */
        pRAInterface->Cfg.AdvPreferredRouterFlag = uValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "AdvLinkMTU", TRUE) )
    {
        /* save update to backup */
        pRAInterface->Cfg.AdvLinkMTU = uValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "AdvReachableTime", TRUE) )
    {
        /* save update to backup */
        pRAInterface->Cfg.AdvReachableTime = uValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "AdvRetransTimer", TRUE) )
    {
        /* save update to backup */
        pRAInterface->Cfg.AdvRetransTimer = uValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "AdvCurHopLimit", TRUE) )
    {
        /* save update to backup */
        pRAInterface->Cfg.AdvCurHopLimit = uValue;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        InterfaceSetting1_SetParamStringValue
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
InterfaceSetting1_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_RA_IF_FULL2           pRAInterface = (PCOSA_DML_RA_IF_FULL2)pCosaContext->hContext;
    int                             len = 0;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        /* save update to backup */
        len = (_ansc_strlen(pString) > sizeof(pRAInterface->Cfg.Alias)-1 ? sizeof(pRAInterface->Cfg.Alias)-1 : _ansc_strlen(pString));
        
        AnscZeroMemory(pRAInterface->Cfg.Alias, sizeof(pRAInterface->Cfg.Alias));
        _ansc_strncpy(pRAInterface->Cfg.Alias, pString, len);

        return TRUE;
    }

    if( AnscEqualString(ParamName, "Interface", TRUE) )
    {
        /* save update to backup */
#ifdef _COSA_DRG_CNS_
        /*not supported*/
        return FALSE;
#endif

        len = (_ansc_strlen(pString) > sizeof(pRAInterface->Cfg.Interface)-1 ? sizeof(pRAInterface->Cfg.Interface)-1 : _ansc_strlen(pString));
        
        AnscZeroMemory(pRAInterface->Cfg.Interface, sizeof(pRAInterface->Cfg.Interface));
        _ansc_strncpy(pRAInterface->Cfg.Interface, pString, len);

        return TRUE;
    }

    if( AnscEqualString(ParamName, "ManualPrefixes", TRUE) )
    {
        /* save update to backup */
        len = (_ansc_strlen(pString) > sizeof(pRAInterface->Cfg.ManualPrefixes)-1 ? sizeof(pRAInterface->Cfg.ManualPrefixes)-1 : _ansc_strlen(pString));
        
        AnscZeroMemory(pRAInterface->Cfg.ManualPrefixes, sizeof(pRAInterface->Cfg.ManualPrefixes));
        _ansc_strncpy(pRAInterface->Cfg.ManualPrefixes, pString, len);

        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        InterfaceSetting1_Validate
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
InterfaceSetting1_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_DATAMODEL_RA              pMyObject     = (PCOSA_DATAMODEL_RA)g_pCosaBEManager->hRA; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2 = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_RA_IF_FULL2           pRAInterface  = (PCOSA_DML_RA_IF_FULL2)pCosaContext->hContext;
    PCOSA_DML_RA_IF_FULL2           pRAInterface2 = (PCOSA_DML_RA_IF_FULL2)NULL;
    PSLIST_HEADER                   pRAIFHead     = (PSLIST_HEADER)&pMyObject->InterfaceList;
    PSINGLE_LINK_ENTRY              pLink         = (PSINGLE_LINK_ENTRY)NULL;

    pLink = AnscSListGetFirstEntry(pRAIFHead);

    while ( pLink )
    {
        pCosaContext2 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        pLink   = AnscSListGetNextEntry(pLink);

        pRAInterface2 = (PCOSA_DML_RA_IF_FULL2)pCosaContext2->hContext;

        if ( pRAInterface2 && 
            ((ULONG)pRAInterface2 != (ULONG)pRAInterface) &&
             AnscEqualString(pRAInterface2->Cfg.Alias, pRAInterface->Cfg.Alias, TRUE))
        {
            AnscCopyString(pReturnParamName, "Alias");
            *puLength = AnscSizeOfString("Alias");

            CcspTraceWarning(("InterfaceSetting1_Validate() failed.\n"));
            return FALSE;
        }
    }

    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        InterfaceSetting1_Commit
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
InterfaceSetting1_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_RA_IF_FULL2           pRAInterface = (PCOSA_DML_RA_IF_FULL2)pCosaContext->hContext;

    if ( pCosaContext->bNew )
    {
        pCosaContext->bNew = FALSE;

        CosaDmlRaIfAddEntry(NULL, (PCOSA_DML_RA_IF_FULL)pRAInterface);

        CosaRARegDelInfo(NULL, (ANSC_HANDLE)pCosaContext);
    }
    else
    {
        CosaDmlRaIfSetCfg(pRAInterface, (PCOSA_DML_RA_IF_CFG)&pRAInterface->Cfg);
    }
    

    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        InterfaceSetting1_Rollback
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
InterfaceSetting1_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_RA_IF_FULL2           pRAInterface = (PCOSA_DML_RA_IF_FULL2)pCosaContext->hContext;

    CosaDmlRaIfGetCfg (pRAInterface, (PCOSA_DML_RA_IF_CFG)&pRAInterface->Cfg);
    CosaDmlRaIfGetInfo(pRAInterface, pRAInterface->Cfg.InstanceNumber, (PCOSA_DML_RA_IF_INFO)&pRAInterface->Info);

    return 0;
}


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

    Option.{i}.

    *  Option5_GetEntryCount
    *  Option5_GetEntry
    *  Option5_AddEntry
    *  Option5_DelEntry
    *  Option5_GetParamBoolValue
    *  Option5_GetParamIntValue
    *  Option5_GetParamUlongValue
    *  Option5_GetParamStringValue
    *  Option5_SetParamBoolValue
    *  Option5_SetParamIntValue
    *  Option5_SetParamUlongValue
    *  Option5_SetParamStringValue
    *  Option5_Validate
    *  Option5_Commit
    *  Option5_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Option5_GetEntryCount
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
Option5_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_RA_IF_FULL2           pRAInterface = (PCOSA_DML_RA_IF_FULL2)pCosaContext->hContext;

    return AnscSListQueryDepth(&pRAInterface->OptionList);

    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Option5_GetEntry
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
Option5_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_RA_IF_FULL2           pRAInterface     = (PCOSA_DML_RA_IF_FULL2)pCosaContext->hContext;
    PSINGLE_LINK_ENTRY              pLink            = (PSINGLE_LINK_ENTRY)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)NULL;

    pLink = AnscSListGetEntryByIndex((PSLIST_HEADER)&pRAInterface->OptionList, nIndex);

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
        Option5_AddEntry
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
Option5_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_RA              pMyObject        = (PCOSA_DATAMODEL_RA)g_pCosaBEManager->hRA; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_RA_IF_FULL2           pRAInterface     = (PCOSA_DML_RA_IF_FULL2)pCosaContext->hContext;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_RA_OPTION             pRAOption        = (PCOSA_DML_RA_OPTION)NULL;

    pRAOption = (PCOSA_DML_RA_OPTION)AnscAllocateMemory(sizeof(COSA_DML_RA_OPTION));

    if ( !pRAOption )
    {
        return NULL;
    }

    _ansc_sprintf(pRAOption->Alias, "RAOption%d", (int)pRAInterface->ulNextOptionInsNum);

    /* Update the middle layer data */
    if ( TRUE )
    {
        pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

        if ( !pSubCosaContext )
        {
            AnscFreeMemory(pRAOption);
            
            return NULL;
        }

        pRAOption->InstanceNumber = pSubCosaContext->InstanceNumber = pRAInterface->ulNextOptionInsNum;

        pRAInterface->ulNextOptionInsNum++;

        if ( 0 == pRAInterface->ulNextOptionInsNum )
        {
            pRAInterface->ulNextOptionInsNum = 1;
        }

        pSubCosaContext->hContext         = (ANSC_HANDLE)pRAOption;
        pSubCosaContext->hParentTable     = pRAInterface;
        pSubCosaContext->hPoamIrepUpperFo = pMyObject->hIrepFolderRAInterfaceHA;
        pSubCosaContext->bNew             = TRUE;

        CosaSListPushEntryByInsNum((PSLIST_HEADER)&pRAInterface->OptionList, pSubCosaContext);

        CosaRARegAddInfo
        (
            (ANSC_HANDLE)pMyObject,
            COSA_DML_RR_NAME_RAIF_OPTION_NextInsNum,
            pRAInterface->ulNextOptionInsNum,
            COSA_DML_RR_NAME_RAIF_OPTION_Prefix,
            pRAInterface->Cfg.InstanceNumber,
            pRAOption->Alias,
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
        Option5_DelEntry
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
Option5_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_RA_IF_FULL2           pRAInterface    = (PCOSA_DML_RA_IF_FULL2)pCosaContext->hContext;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_DML_RA_OPTION             pRAOption       = (PCOSA_DML_RA_OPTION)pSubCosaContext->hContext;

    if (!pCosaContext->bNew)
        CosaDmlRaIfDelOption(pRAInterface, pRAInterface->Cfg.InstanceNumber, pRAOption);

    AnscSListPopEntryByLink((PSLIST_HEADER)&pRAInterface->OptionList, &pSubCosaContext->Linkage);

    if ( pSubCosaContext->bNew )
    {
        CosaRARegDelInfo(NULL, (ANSC_HANDLE)pSubCosaContext);
    }

    AnscFreeMemory(pRAOption);
    AnscFreeMemory(pSubCosaContext);

    return returnStatus;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Option5_GetParamBoolValue
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
Option5_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_RA_OPTION             pOption      = (PCOSA_DML_RA_OPTION)pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE) )
    {
        /* collect value */
        *pBool = pOption->bEnabled;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Option5_GetParamIntValue
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
Option5_GetParamIntValue
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
        Option5_GetParamUlongValue
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
Option5_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_RA_OPTION             pOption      = (PCOSA_DML_RA_OPTION)pCosaContext->hContext;
    PCOSA_DML_RA_IF_FULL2           pRAInterface = (PCOSA_DML_RA_IF_FULL2)pCosaContext->hParentTable;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Tag", TRUE) )
    {
        /* collect value */
        *puLong = pOption->Tag;        
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Option5_GetParamStringValue
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
Option5_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_RA_OPTION             pOption      = (PCOSA_DML_RA_OPTION)pCosaContext->hContext;
    PCOSA_DML_RA_IF_FULL2           pRAInterface = (PCOSA_DML_RA_IF_FULL2)pCosaContext->hParentTable;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        /* collect value */
        AnscCopyString(pValue, pOption->Alias);
        return 0;
    }

    if( AnscEqualString(ParamName, "Value", TRUE) )
    {
        /* collect value */
        AnscCopyString(pValue, pOption->Value);
        return 0;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Option5_SetParamBoolValue
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
Option5_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_RA_OPTION             pOption      = (PCOSA_DML_RA_OPTION)pCosaContext->hContext;
    PCOSA_DML_RA_IF_FULL2           pRAInterface = (PCOSA_DML_RA_IF_FULL2)pCosaContext->hParentTable;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE) )
    {
        /* save update to backup */
        pOption->bEnabled = bValue;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Option5_SetParamIntValue
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
Option5_SetParamIntValue
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
        Option5_SetParamUlongValue
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
Option5_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_RA_OPTION             pOption      = (PCOSA_DML_RA_OPTION)pCosaContext->hContext;
    PCOSA_DML_RA_IF_FULL2           pRAInterface = (PCOSA_DML_RA_IF_FULL2)pCosaContext->hParentTable;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Tag", TRUE) )
    {
        /* save update to backup */
        pOption->Tag = uValue;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Option5_SetParamStringValue
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
Option5_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_RA_OPTION             pOption      = (PCOSA_DML_RA_OPTION)pCosaContext->hContext;
    PCOSA_DML_RA_IF_FULL2           pRAInterface = (PCOSA_DML_RA_IF_FULL2)pCosaContext->hParentTable;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        /* save update to backup */
        int len = 0;
        
        len = (_ansc_strlen(pString) > sizeof(pOption->Alias)-1 ? sizeof(pOption->Alias)-1 : _ansc_strlen(pString));
        
        AnscZeroMemory(pOption->Alias, sizeof(pOption->Alias));
        _ansc_strncpy(pOption->Alias, pString, len);

        return TRUE;
    }

    if( AnscEqualString(ParamName, "Value", TRUE) )
    {
        /* save update to backup */
        int len = 0;
        
        len = (_ansc_strlen(pString) > sizeof(pOption->Value)-1 ? sizeof(pOption->Value)-1 : _ansc_strlen(pString));
        
        AnscZeroMemory(pOption->Value, sizeof(pOption->Value));
        _ansc_strncpy(pOption->Value, pString, len);

        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Option5_Validate
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
Option5_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2 = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_RA_IF_FULL2           pRAInterface  = (PCOSA_DML_RA_IF_FULL2)pCosaContext->hParentTable;
    PCOSA_DML_RA_OPTION             pRAOption     = (PCOSA_DML_RA_OPTION)pCosaContext->hContext;
    PCOSA_DML_RA_OPTION             pRAOption2    = (PCOSA_DML_RA_OPTION)NULL;
    PSINGLE_LINK_ENTRY              pLink         = (PSINGLE_LINK_ENTRY)NULL;

    pLink = AnscSListGetFirstEntry(&pRAInterface->OptionList);

    while ( pLink )
    {
        pCosaContext2 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        pLink   = AnscSListGetNextEntry(pLink);

        pRAOption2 = (PCOSA_DML_RA_OPTION)pCosaContext2->hContext;

        if ( pRAOption2 &&
             ((ULONG)pRAOption2 != (ULONG)pRAOption) &&
             AnscEqualString(pRAOption2->Alias, pRAOption->Alias, TRUE))
        {
            AnscCopyString(pReturnParamName, "Alias");
            *puLength = AnscSizeOfString("Alias");

            CcspTraceWarning(("RAOption_Validate() failed for Alias.\n"));            
            return FALSE;
        }

        if ( ((ULONG)pRAOption2 != (ULONG)pRAOption) &&
            (pRAOption->bEnabled && pRAOption2->bEnabled) &&
             pRAOption->Tag == pRAOption2->Tag)
        {
            AnscCopyString(pReturnParamName, "Tag");
            *puLength = AnscSizeOfString("Tag");
    
            CcspTraceWarning(("RAOption_Validate() failed for Tag.\n"));            
            return FALSE;
        }
    }


    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Option5_Commit
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
Option5_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_RA              pMyObject    = (PCOSA_DATAMODEL_RA)g_pCosaBEManager->hRA; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_RA_OPTION             pRAOption    = (PCOSA_DML_RA_OPTION)pCosaContext->hContext;
    PCOSA_DML_RA_IF_FULL2           pRAInterface = (PCOSA_DML_RA_IF_FULL2)pCosaContext->hParentTable;

    if ( pCosaContext->bNew )
    {
        pCosaContext->bNew = FALSE;

        CosaDmlRaIfAddOption(pRAInterface , pRAInterface->Cfg.InstanceNumber, pRAOption);

        CosaRARegDelInfo(NULL, (ANSC_HANDLE)pCosaContext);
    }
    else
    {
        CosaDmlRaIfSetOption(pRAInterface, pRAInterface->Cfg.InstanceNumber, pRAOption);
    }
    

    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Option5_Rollback
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
Option5_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_RA_OPTION             pRAOption    = (PCOSA_DML_RA_OPTION)pCosaContext->hContext;
    PCOSA_DML_RA_IF_FULL2           pRAInterface = (PCOSA_DML_RA_IF_FULL2)pCosaContext->hParentTable;

    CcspTraceWarning(("RAOption_Rollback()----begin to rollback......\n"));

    CosaDmlRaIfGetOption2(pRAOption, pRAInterface->Cfg.InstanceNumber, pRAOption);


    return 0;
}

