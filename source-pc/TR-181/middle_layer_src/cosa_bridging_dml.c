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

    module: cosa_bridging_dml.c

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

        03/14/2011    initial revision.

**************************************************************************/

#include "ansc_platform.h"
#include "cosa_bridging_dml.h"
#include "cosa_bridging_internal.h" // "cosa_bridging_apis.h" is included in here
#include "messagebus_interface_helper.h"
#include "cosa_ethernet_apis_ext.h"
#include "dml_tr181_custom_cfg.h"

#if defined _COSA_DRG_TPG_ || _COSA_DRG_CNS_
#include "utctx/utctx_api.h"
#endif
extern ANSC_HANDLE bus_handle;
extern char g_Subsystem[32];
extern void* g_pDslhDmlAgent;

//$HL 4/30/2013
#include "ccsp_psm_helper.h"

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

    Bridging.

    *  Bridging_GetParamBoolValue
    *  Bridging_GetParamIntValue
    *  Bridging_GetParamUlongValue
    *  Bridging_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Bridging_GetParamBoolValue
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
Bridging_GetParamBoolValue
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
        Bridging_GetParamIntValue
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
Bridging_GetParamIntValue
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
        Bridging_GetParamUlongValue
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
Bridging_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "MaxBridgeEntries") == 0)
    {
        /* collect value */
        /* return fixed value*/
        *puLong = 8;
        return TRUE;
    }

    if (strcmp(ParamName, "MaxDBridgeEntries") == 0)
    {
        /* collect value */
        /* return fixed value*/
        *puLong = 8;
        return TRUE;
    }

    if (strcmp(ParamName, "MaxQBridgeEntries") == 0)
    {
        /* collect value */
        *puLong = 0;
        return TRUE;
    }

    if (strcmp(ParamName, "MaxVLANEntries") == 0)
    {
        /* collect value */
        *puLong = 8;//LNT_EMU
        return TRUE;
    }

    if (strcmp(ParamName, "MaxFilterEntries") == 0)
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

        ULONG
        Bridging_GetParamStringValue
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
Bridging_GetParamStringValue
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

    Bridging.Bridge.{i}.

    *  Bridge_GetEntryCount
    *  Bridge_GetEntry
    *  Bridge_AddEntry
    *  Bridge_DelEntry
    *  Bridge_GetParamBoolValue
    *  Bridge_GetParamIntValue
    *  Bridge_GetParamUlongValue
    *  Bridge_GetParamStringValue
    *  Bridge_SetParamBoolValue
    *  Bridge_SetParamIntValue
    *  Bridge_SetParamUlongValue
    *  Bridge_SetParamStringValue
    *  Bridge_Validate
    *  Bridge_Commit
    *  Bridge_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Bridge_GetEntryCount
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
Bridge_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_BRIDGING         pCosaDMBridging   = (PCOSA_DATAMODEL_BRIDGING )g_pCosaBEManager->hBridging;

    return AnscSListQueryDepth(&pCosaDMBridging->BridgeList);
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Bridge_GetEntry
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
Bridge_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_BRIDGING        pCosaDMBridging   = (PCOSA_DATAMODEL_BRIDGING )g_pCosaBEManager->hBridging;
    PSLIST_HEADER                   pBridgeHead       = (PSLIST_HEADER            )&pCosaDMBridging->BridgeList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY       )NULL;

    pSLinkEntry = AnscSListGetEntryByIndex(pBridgeHead, nIndex);

    if ( pSLinkEntry )
    {
        pCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);

        *pInsNumber = pCosaContext->InstanceNumber;
    }

    return (ANSC_HANDLE)pCosaContext; /* return the handle */
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Bridge_AddEntry
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
Bridge_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_BRIDGING        pCosaDMBridging   = (PCOSA_DATAMODEL_BRIDGING )g_pCosaBEManager->hBridging;
    PSLIST_HEADER                   pBridgeHead       = (PSLIST_HEADER            )&pCosaDMBridging->BridgeList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext1      = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge        = (PCOSA_DML_BRG_FULL_ALL   )NULL;
    PCOSA_DML_BRG_VLAN_FULL         pVLAN           = (PCOSA_DML_BRG_VLAN_FULL  )NULL;
    PSLIST_HEADER                   pListHead       = (PSLIST_HEADER            )NULL;

    pDmlBridge = (PCOSA_DML_BRG_FULL_ALL)AnscAllocateMemory(sizeof(COSA_DML_BRG_FULL_ALL));

    if ( !pDmlBridge )
    {
        return NULL;
    }

    _ansc_sprintf(pDmlBridge->Cfg.Alias, "Bridge%d", pCosaDMBridging->ulNextBridgeInstance);
    pDmlBridge->Cfg.Std = COSA_DML_BRG_STD_8021Q_2005;
    pDmlBridge->Cfg.bAllowDelete = TRUE;
    //$HL 12/2/2013
    //pDmlBridge->Cfg.bEnabled = FALSE;
    pDmlBridge->Cfg.bEnabled = TRUE;//LNT_EMU
    AnscSListInitializeHeader(&pDmlBridge->PortList);
    AnscSListInitializeHeader(&pDmlBridge->VLANList);
    AnscSListInitializeHeader(&pDmlBridge->VLANPortList);

    pDmlBridge->ulNextPortInsNum = 1;
    pDmlBridge->ulNextVLANInsNum = 1;
    pDmlBridge->ulNextVLANPortInsNum = 1;

    if ( TRUE )
    {
        pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

        if ( !pCosaContext )
        {
            AnscFreeMemory(pDmlBridge);
            
            return NULL;
        }
        _ansc_memset(pCosaContext, 0, sizeof(COSA_CONTEXT_LINK_OBJECT));

        pCosaContext->InstanceNumber = pDmlBridge->Cfg.InstanceNumber = pCosaDMBridging->ulNextBridgeInstance;

        pCosaDMBridging->ulNextBridgeInstance++;

        if ( pCosaDMBridging->ulNextBridgeInstance == 0 )
        {
            pCosaDMBridging->ulNextBridgeInstance = 1;
        }

        pCosaContext->hContext         = (ANSC_HANDLE)pDmlBridge;
        pCosaContext->hParentTable     = NULL;
        pCosaContext->hPoamIrepUpperFo = pCosaDMBridging->hIrepFolderBRGHA;
        pCosaContext->bNew             = TRUE;

        CosaSListPushEntryByInsNum(pBridgeHead, pCosaContext);

        CosaBridgingRegAddInfo
            (
                (ANSC_HANDLE)pCosaDMBridging,
                COSA_DML_RR_NAME_Bridge_NextInsNunmber,
                pCosaDMBridging->ulNextBridgeInstance,
                COSA_DML_RR_NAME_Bridge_Prefix,
                0,
                pDmlBridge->Cfg.Alias,
                (ANSC_HANDLE)pCosaContext
            );
    }
    CosaDmlBrgAddEntry(NULL, (PCOSA_DML_BRG_CFG)&pDmlBridge->Cfg);

    pListHead = (PSLIST_HEADER)&pDmlBridge->VLANList;

    pVLAN = (PCOSA_DML_BRG_VLAN_FULL)AnscAllocateMemory(sizeof(COSA_DML_BRG_VLAN_FULL));

    if ( !pVLAN )
    {
        return NULL;
    }

    _ansc_memset(pVLAN, 0, sizeof(COSA_DML_BRG_VLAN_FULL));

    _ansc_sprintf(pVLAN->Cfg.Alias, "cpe-VLAN%d", pDmlBridge->ulNextVLANInsNum);
    
    /* Update the middle layer cache */
    if ( TRUE )
    {
        pCosaContext1 = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

        if ( !pCosaContext1 )
        {
            AnscFreeMemory(pVLAN);
            AnscFreeMemory(pDmlBridge);
            AnscFreeMemory(pCosaContext);            
            return NULL;
        }

        _ansc_memset(pCosaContext1, 0, sizeof(COSA_CONTEXT_LINK_OBJECT));
     
        pCosaContext1->InstanceNumber = pVLAN->Cfg.InstanceNumber = pDmlBridge->ulNextVLANInsNum;

        pDmlBridge->ulNextVLANInsNum++;

        if ( pDmlBridge->ulNextVLANInsNum == 0 )
        {
            pDmlBridge->ulNextVLANInsNum = 1;
        }

        pCosaContext1->hContext         = (ANSC_HANDLE)pVLAN;
        pCosaContext1->hParentTable     = (ANSC_HANDLE)pDmlBridge;
        pCosaContext1->hPoamIrepUpperFo = pCosaDMBridging->hIrepFolderBRGHA;
        pCosaContext1->bNew             = FALSE;
        
        CosaSListPushEntryByInsNum(pListHead, (ANSC_HANDLE)pCosaContext1);

        //$HL 7/2/2013
        //pVLAN->Cfg.bEnabled = TRUE;
        //$HL 12/3/2012
        pVLAN->Cfg.bEnabled = FALSE;
        //AnscCopyString(pVLAN->Info.Name,CosaDmlBrgGetName(pDmlBridge->Cfg.InstanceNumber));
        AnscCopyString(pVLAN->Info.Name, pVLAN->Cfg.Alias);//LNT_EMU
        pVLAN->Cfg.VLANID = CosaDmlBrgGetVLANID(pDmlBridge->Cfg.InstanceNumber);
        CosaDmlBrgVlanAddEntry(NULL, pDmlBridge->Cfg.InstanceNumber,pVLAN);
    }
    
    *pInsNumber = pCosaContext->InstanceNumber;

    //$HL 5/1/2013
    pCosaContext->bNew  = FALSE;
    CosaBridgingRegDelInfo(NULL, pCosaContext);
    return (ANSC_HANDLE)pCosaContext; /* return the handle */
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Bridge_DelEntry
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
Bridge_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge        = (PCOSA_DML_BRG_FULL_ALL   )pCosaContext->hContext;
    PCOSA_DATAMODEL_BRIDGING        pCosaDMBridging   = (PCOSA_DATAMODEL_BRIDGING )g_pCosaBEManager->hBridging;
    PSLIST_HEADER                   pBridgeHead       = (PSLIST_HEADER            )&pCosaDMBridging->BridgeList;
    //$HL 5/17/2013
    if (!pDmlBridge->Cfg.bAllowDelete)
    {
        return -1;
    }

    if ( pCosaContext->bNew )
    {
        pCosaContext->bNew = FALSE;
        CosaBridgingRegDelInfo(NULL, pCosaContext);
    }
    else
    {
        CosaDmlBrgDelEntry(NULL, pDmlBridge->Cfg.InstanceNumber);
    }

    AnscSListPopEntryByLink(pBridgeHead, &pCosaContext->Linkage);
    
    AnscFreeMemory(pDmlBridge);
    AnscFreeMemory(pCosaContext);
    return 0; /* succeeded */
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Bridge_GetParamBoolValue
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
Bridge_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge        = (PCOSA_DML_BRG_FULL_ALL   )pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        *pBool = pDmlBridge->Cfg.bEnabled;

        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Bridge_GetParamIntValue
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
Bridge_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge        = (PCOSA_DML_BRG_FULL_ALL   )pCosaContext->hContext;
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Bridge_GetParamUlongValue
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
Bridge_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge        = (PCOSA_DML_BRG_FULL_ALL   )pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Status") == 0)
    {
        /* collect value */

        CosaDmlBrgGetInfo(NULL, pDmlBridge->Cfg.InstanceNumber, &pDmlBridge->Info);

        *puLong = pDmlBridge->Info.Status;

        return TRUE;
    }

    if (strcmp(ParamName, "Standard") == 0)
    {
        /* collect value */
        //*puLong = pDmlBridge->Cfg.Std;
        *puLong = COSA_DML_BRG_STD_8021Q_2005;//LNT_EMU

        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Bridge_GetParamStringValue
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
Bridge_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge        = (PCOSA_DML_BRG_FULL_ALL   )pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* collect value */
        AnscCopyString(pValue, pDmlBridge->Cfg.Alias);

        return 0;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Bridge_SetParamBoolValue
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
Bridge_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge        = (PCOSA_DML_BRG_FULL_ALL   )pCosaContext->hContext;
    //$Hl 12/3/2013
    PSLIST_HEADER                   pListHead     = (PSLIST_HEADER            )&pDmlBridge->VLANList;
    PSINGLE_LINK_ENTRY              pSLinkEntry   = (PSINGLE_LINK_ENTRY       )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext1 = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_BRG_VLAN_FULL         pVLAN         =  (PCOSA_DML_BRG_VLAN_FULL  ) NULL;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* save update to backup */
        pDmlBridge->Cfg.bEnabled = bValue;
        pSLinkEntry = AnscSListGetFirstEntry(pListHead);

        if ( pSLinkEntry )
        {
            pCosaContext1 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);

            pVLAN = pCosaContext1->hContext;
            pVLAN->Cfg.bEnabled = bValue;
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
        Bridge_SetParamIntValue
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
Bridge_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge        = (PCOSA_DML_BRG_FULL_ALL   )pCosaContext->hContext;

    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Bridge_SetParamUlongValue
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
Bridge_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge        = (PCOSA_DML_BRG_FULL_ALL   )pCosaContext->hContext;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Standard") == 0)
    {
        /* save update to backup */
        pDmlBridge->Cfg.Std = uValue;

        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Bridge_SetParamStringValue
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
Bridge_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge        = (PCOSA_DML_BRG_FULL_ALL   )pCosaContext->hContext;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* save update to backup */
        AnscCopyString(pDmlBridge->Cfg.Alias, pString);

        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Bridge_Validate
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
Bridge_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_DATAMODEL_BRIDGING        pCosaDMBridging   = (PCOSA_DATAMODEL_BRIDGING )g_pCosaBEManager->hBridging;
    PSLIST_HEADER                   pBridgeHead       = (PSLIST_HEADER            )&pCosaDMBridging->BridgeList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge        = (PCOSA_DML_BRG_FULL_ALL   )pCosaContext->hContext;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2     = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge2       = (PCOSA_DML_BRG_FULL_ALL   )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY       )NULL;

    pSLinkEntry = AnscSListGetFirstEntry(pBridgeHead);

    while ( pSLinkEntry )
    {
        pCosaContext2 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        pSLinkEntry   = AnscSListGetNextEntry(pSLinkEntry);

        pDmlBridge2 = (PCOSA_DML_BRG_FULL_ALL)pCosaContext2->hContext;

        if ( pDmlBridge2 && ((ULONG)pDmlBridge2 != (ULONG)pDmlBridge) && AnscEqualString(pDmlBridge2->Cfg.Alias, pDmlBridge->Cfg.Alias, TRUE))
        {
            AnscCopyString(pReturnParamName, "Alias");

            *puLength = AnscSizeOfString("Alias");

            return FALSE;
        }
    }

    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Bridge_Commit
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
Bridge_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_BRIDGING        pCosaDMBridging   = (PCOSA_DATAMODEL_BRIDGING )g_pCosaBEManager->hBridging;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge        = (PCOSA_DML_BRG_FULL_ALL   )pCosaContext->hContext;

    if ( pCosaContext->bNew )
    {
        pCosaContext->bNew = FALSE;

        CosaDmlBrgAddEntry(NULL, (PCOSA_DML_BRG_CFG)&pDmlBridge->Cfg);

        CosaBridgingRegDelInfo(NULL, pCosaContext);
    }
    else
    {
        CosaDmlBrgSetCfg(NULL, (PCOSA_DML_BRG_CFG)&pDmlBridge->Cfg);
    }

    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Bridge_Rollback
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
Bridge_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_BRIDGING        pCosaDMBridging   = (PCOSA_DATAMODEL_BRIDGING )g_pCosaBEManager->hBridging;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge        = (PCOSA_DML_BRG_FULL_ALL   )pCosaContext->hContext;

    return 0;
}

/***********************************************************************

 APIs for Object:

    Bridging.Bridge.{i}.Port.{i}.

    *  Port_GetEntryCount
    *  Port_GetEntry
    *  Port_AddEntry
    *  Port_DelEntry
    *  Port_GetParamBoolValue
    *  Port_GetParamIntValue
    *  Port_GetParamUlongValue
    *  Port_GetParamStringValue
    *  Port_SetParamBoolValue
    *  Port_SetParamIntValue
    *  Port_SetParamUlongValue
    *  Port_SetParamStringValue
    *  Port_Validate
    *  Port_Commit
    *  Port_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Port_GetEntryCount
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
Port_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge   = (PCOSA_DML_BRG_FULL_ALL   )pCosaContext->hContext;
    //$HL 4/27/2013
    AnscTraceFlow(("<HL >%s bridge instance = %d get num of port entries=%d\n", 
           __FUNCTION__,pDmlBridge->Cfg.InstanceNumber, AnscSListQueryDepth(&pDmlBridge->PortList)));
    return AnscSListQueryDepth(&pDmlBridge->PortList);
}

/**********************************************************************  

    caller:     local functions

    prototype: 

        PCOSA_CONTEXT_LINK_OBJECT
        Find_SListEntry_By_InstanceNumber
            (
                PSLIST_HEADER               pList
                ULONG                       InstanceNumber
            );

    description:

        This is a utility function that returns list entry by instance number
        

    argument:   PSLIST_HEADER               pList
                entry list

                ULONG                       InstanceNumber
                Instance number of list entry

    return:     list entry

**********************************************************************/
/* CID:65939 useless_type_qualifier_on_return_type*/
static PCOSA_CONTEXT_LINK_OBJECT
Find_SListEntry_By_InstanceNumber
    (
        PSLIST_HEADER pList,
        ULONG         InstanceNumber
    )
{
    PSINGLE_LINK_ENTRY              pSLinkEntry      = (PSINGLE_LINK_ENTRY       )NULL;

    pSLinkEntry = AnscSListGetFirstEntry(pList);

    while ( pSLinkEntry )
    {
        PCOSA_CONTEXT_LINK_OBJECT pCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        pSLinkEntry   = AnscSListGetNextEntry(pSLinkEntry);

        if (pCosaContext->InstanceNumber == InstanceNumber)
        {
            return pCosaContext;
        }
    }

    return (PCOSA_CONTEXT_LINK_OBJECT)NULL;
}
                            

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Port_GetEntry
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
Port_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge    = (PCOSA_DML_BRG_FULL_ALL   )pCosaContext->hContext;
    PSLIST_HEADER                   pListHead     = (PSLIST_HEADER            )&pDmlBridge->PortList;
    PSINGLE_LINK_ENTRY              pSLinkEntry   = (PSINGLE_LINK_ENTRY       )NULL;

    pSLinkEntry = AnscSListGetEntryByIndex(pListHead, nIndex);

    if ( pSLinkEntry )
    {
        pCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);

        *pInsNumber = pCosaContext->InstanceNumber;
    }

    return (ANSC_HANDLE)pCosaContext;
}

/**********************************************************************  
To Resolve Port Deletion issue Port_IsUpdated and Port_Synchronize is 
implemented by LNT
**********************************************************************/

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        Port_IsUpdated
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/
#if defined __VLAN__
BOOL
Port_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_BRIDGING             pMyObject    = (PCOSA_DATAMODEL_BRIDGING)g_pCosaBEManager->hBridging;
    BOOL                            bIsUpdated   = TRUE;

    /*
        We can use one rough granularity interval to get whole table in case
        that the updating is too frequent.
        */
    if ( ( AnscGetTickInSeconds() - pMyObject->PreviousVisitTime ) < COSA_DML_BRIDGE_ACCESS_INTERVAL )
    {
        bIsUpdated  = FALSE;
    }
    else
    {
        pMyObject->PreviousVisitTime =  AnscGetTickInSeconds();
        bIsUpdated  = TRUE;
    }

    return bIsUpdated;
}
/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        Port_Synchronize
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
Port_Synchronize
 (
        ANSC_HANDLE                 hInsContext
    )
{
	ANSC_STATUS                           returnStatus      = ANSC_STATUS_FAILURE;
	PCOSA_DATAMODEL_BRIDGING        pCosaDMBridging = (PCOSA_DATAMODEL_BRIDGING )g_pCosaBEManager->hBridging;
	PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
	PCOSA_DML_BRG_FULL_ALL          pDmlBridge      = (PCOSA_DML_BRG_FULL_ALL   )pCosaContext->hContext;
	PSLIST_HEADER                   pBridgeHead       = (PSLIST_HEADER            )&pCosaDMBridging->BridgeList;
	PSLIST_HEADER                   pListHead       = (PSLIST_HEADER            )&pDmlBridge->PortList;
	PCOSA_DML_BRG_PORT_FULL         pPort           = (PCOSA_DML_BRG_PORT_FULL  )NULL;
	PCOSA_DML_BRG_PORT_FULL         pPort2           = (PCOSA_DML_BRG_PORT_FULL  )NULL;
	PSINGLE_LINK_ENTRY              pSLinkEntry   = (PSINGLE_LINK_ENTRY       )NULL;
	PSINGLE_LINK_ENTRY              pSLinkEntry2   = (PSINGLE_LINK_ENTRY       )NULL;
	ULONG                                 entryCount        = 0;
	ULONG                                 i                 = 0;
	ULONG                                 j                 = 0;
	ULONG                                 HashValue         = 0;
	PULONG                                pulTmp            = 0;

	if(pDmlBridge->Cfg.InstanceNumber!=1)
		printf("pDmlBridge->Cfg.InstanceNumber%d\n",pDmlBridge->Cfg.InstanceNumber);
	if(pDmlBridge->Cfg.InstanceNumber!=0){
		pPort         = CosaDmlPortGetEntry(NULL,&entryCount,pDmlBridge->Cfg.InstanceNumber-1);}
	if ( pPort && entryCount )
	{
		pulTmp    =   AnscAllocateMemory( entryCount * sizeof(ULONG) );
		if ( !pulTmp )
		{
			goto EXIT;
		}
	}
	/* Search the whole link and mark bFound of exist entry to FALSE */
	pSLinkEntry =   AnscSListGetFirstEntry(pListHead);
	while( pSLinkEntry )
	{
		pCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
		pSLinkEntry       = AnscSListGetNextEntry(pSLinkEntry);
		pCosaContext->Found  = FALSE;
	}

	/* go over all new entries, find them in current link table and mark them */
	for ( i = 0; i < entryCount; i++)
	{
		pSLinkEntry =   AnscSListGetFirstEntry(pListHead);
		while( pSLinkEntry )
		{
			pCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
			pSLinkEntry       = AnscSListGetNextEntry(pSLinkEntry);
			pPort2 = (PCOSA_DML_BRG_PORT_FULL)pCosaContext->hContext;
			if ( pPort2->Cfg.InstanceNumber == pPort[i].Cfg.InstanceNumber )
			{
				pulTmp[i]         =  1;
				pCosaContext->Found  = TRUE;

				/* If found, update the content also */
				*pPort2    = pPort[i];
				pCosaContext->bNew    = FALSE;

				break;
			}
		}
	}
	/* We need del unreferred entry if it's not delay_added entry */
	pSLinkEntry =   AnscSListGetFirstEntry(pListHead);
	while( pSLinkEntry )
	{   
		pCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
		pSLinkEntry2      = pSLinkEntry;
		pSLinkEntry       = AnscSListGetNextEntry(pSLinkEntry);

		if ( pCosaContext->Found  == FALSE && pCosaContext->bNew  == FALSE )
		{
			AnscSListPopEntryByLink(pListHead, pSLinkEntry2);
			AnscFreeMemory( pCosaContext->hContext );
			AnscFreeMemory( pCosaContext );
		}
	}
	/* We add new entry into our link table */
	for ( i = 0; i < entryCount; i++ )
	{
		if ( pulTmp[i] == 0 )
		{
			/* Add new one */
			pCosaContext = AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
			if ( !pCosaContext )
			{
				goto EXIT;
			}

			pPort2 = AnscAllocateMemory(sizeof(COSA_DML_BRG_PORT_FULL));
			if ( !pPort2 )
			{
				AnscFreeMemory(pCosaContext);
				goto EXIT;
			}
			/* copy new content which should include InstanceNumber and Alias */
			*pPort2       = pPort[i];
			pCosaContext->hContext         = (ANSC_HANDLE)pPort2;          
			pCosaContext->InstanceNumber = pPort2->Cfg.InstanceNumber;
			pCosaContext->bNew       = FALSE;
			CosaSListPushEntryByInsNum(pListHead,pCosaContext);
		}
	}
	returnStatus =  ANSC_STATUS_SUCCESS;
EXIT:
	if (pulTmp)
		AnscFreeMemory(pulTmp);

	return returnStatus;
}
#endif
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Port_AddEntry
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
Port_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
	PCOSA_DATAMODEL_BRIDGING        pCosaDMBridging = (PCOSA_DATAMODEL_BRIDGING )g_pCosaBEManager->hBridging;
	PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
	PCOSA_DML_BRG_FULL_ALL          pDmlBridge      = (PCOSA_DML_BRG_FULL_ALL   )pCosaContext->hContext;
	PSLIST_HEADER                   pListHead       = (PSLIST_HEADER            )&pDmlBridge->PortList;
	PCOSA_DML_BRG_PORT_FULL         pPort           = (PCOSA_DML_BRG_PORT_FULL  )NULL;

	pPort = (PCOSA_DML_BRG_PORT_FULL)AnscAllocateMemory(sizeof(COSA_DML_BRG_PORT_FULL));

	if ( !pPort )
	{
		return NULL;
	}

	_ansc_sprintf(pPort->Cfg.Alias, "Port%d", pDmlBridge->ulNextPortInsNum);

	if(pDmlBridge->ulNextPortInsNum<3){
		_ansc_sprintf(pPort->Cfg.LinkName, "eth%d",pDmlBridge->ulNextPortInsNum);//LNT_EMU
	}else{_ansc_sprintf(pPort->Cfg.LinkName, "wlan0");}

	/* Update the middle layer cache */
	if ( TRUE )
	{
		pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

		if ( !pCosaContext )
		{
			AnscFreeMemory(pPort);

			return NULL;
		}

		pCosaContext->InstanceNumber = pPort->Cfg.InstanceNumber = pDmlBridge->ulNextPortInsNum;

		pDmlBridge->ulNextPortInsNum++;

		if ( pDmlBridge->ulNextPortInsNum == 0 )
		{
			pDmlBridge->ulNextPortInsNum = 1;
		}

		pCosaContext->hContext         = (ANSC_HANDLE)pPort;
		pCosaContext->hParentTable     = (ANSC_HANDLE)pDmlBridge;
		pCosaContext->hPoamIrepUpperFo = pCosaDMBridging->hIrepFolderBRGHA;
		pCosaContext->bNew             = TRUE;

		CosaSListPushEntryByInsNum(pListHead, (ANSC_HANDLE)pCosaContext);

		CosaBridgingRegAddInfo
			(
			 (ANSC_HANDLE)pCosaDMBridging,
			 COSA_DML_RR_NAME_Bridge_Port_NextInsNumber,
			 pDmlBridge->ulNextPortInsNum,
			 COSA_DML_RR_NAME_Bridge_Port_Prefix,
			 pDmlBridge->Cfg.InstanceNumber,
			 pPort->Cfg.Alias,
			 (ANSC_HANDLE)pCosaContext
			);
	}

	pPort->Cfg.bAllowDelete = TRUE;
	if(pDmlBridge->Cfg.InstanceNumber!=1){
		pPort->Cfg.bEnabled=TRUE;}//LNT_EMU
	*pInsNumber = pCosaContext->InstanceNumber;

	return (ANSC_HANDLE)pCosaContext; /* return the handle */
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Port_DelEntry
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
Port_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
	PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
	PCOSA_DML_BRG_FULL_ALL          pDmlBridge      = (PCOSA_DML_BRG_FULL_ALL   )pCosaContext->hContext;
	PSLIST_HEADER                   pListHead       = (PSLIST_HEADER            )&pDmlBridge->PortList;
	PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2   = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
	PCOSA_DML_BRG_PORT_FULL         pPort           = (PCOSA_DML_BRG_PORT_FULL  )pCosaContext2->hContext;

	//$HL 5/17/2013
#if 0 //LNT_EMU
	if (!pPort->Cfg.bAllowDelete)
	{
		printf("Cfg.bAllowDelete IF\n");
		return -1;
	}
#endif
	if(pDmlBridge->Cfg.InstanceNumber==1)//LNT_EMU
	{
		return -1;
	}
	if ( pCosaContext->bNew )
	{
		pCosaContext->bNew = FALSE;
		CosaBridgingRegDelInfo(NULL, pCosaContext2);
	}
	else 
	{    
		CosaDmlBrgPortDelEntry(NULL, pCosaContext->InstanceNumber, pPort->Cfg.InstanceNumber);
	}    
	/* Update the middle layer cache */

	AnscSListPopEntryByLink(pListHead, &pCosaContext2->Linkage);

	AnscFreeMemory(pPort);
	AnscFreeMemory(pCosaContext2);

	pDmlBridge->ulNextPortInsNum--;//LNT_EMU

	return ANSC_STATUS_SUCCESS;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Port_GetParamBoolValue
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
Port_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_PORT_FULL         pPort            = (PCOSA_DML_BRG_PORT_FULL  )pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        *pBool = pPort->Cfg.bEnabled;

        return TRUE;
    }

    if (strcmp(ParamName, "ManagementPort") == 0)
    {
        /* collect value */
        *pBool = pPort->Cfg.bManagementPort;

        return TRUE;
    }

    if (strcmp(ParamName, "IngressFiltering") == 0)
    {
        /* collect value */
        *pBool = pPort->Cfg.bIngressFiltering;

        return TRUE;
    }

    if (strcmp(ParamName, "PriorityTagging") == 0)
    {
        /* collect value */
        *pBool = pPort->Cfg.bPriorityTagging;

        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Port_GetParamIntValue
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
Port_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_PORT_FULL         pPort            = (PCOSA_DML_BRG_PORT_FULL  )pCosaContext->hContext;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge       = (PCOSA_DML_BRG_FULL_ALL   )pCosaContext->hParentTable;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "PVID") == 0)
    {
        //$HL 7/3/2013
        /* collect value */
        *pInt = pPort->Cfg.PVID;
       // *pInt = CosaDmlBrgGetVLANID(pDmlBridge->Cfg.InstanceNumber);
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Port_GetParamUlongValue
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
Port_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge       = (PCOSA_DML_BRG_FULL_ALL   )pCosaContext->hParentTable; 
    PCOSA_DML_BRG_PORT_FULL         pPort            = (PCOSA_DML_BRG_PORT_FULL  )pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Status") == 0)
    {
        /* collect value */
        CosaDmlBrgPortGetInfo(NULL, pDmlBridge->Cfg.InstanceNumber, pPort->Cfg.InstanceNumber, &pPort->Info);//RDKB-EMU

        *puLong = pPort->Info.Status;

        return TRUE;
    }

    if (strcmp(ParamName, "LastChange") == 0)
    {
        /* collect value */
	// CosaDmlBrgPortGetInfo(NULL, pDmlBridge->Cfg.InstanceNumber, pPort->Cfg.InstanceNumber, &pPort->Info);//LNT_EMU

        *puLong = AnscGetTimeIntervalInSeconds(pPort->Info.LastChange, AnscGetTickInSeconds());

        return TRUE;
    }

    if (strcmp(ParamName, "DefaultUserPriority") == 0)
    {
        /* collect value */
        *puLong = pPort->Cfg.DftUserPriority;

        return TRUE;
    }

    if (strcmp(ParamName, "PriorityRegeneration") == 0)
    {
        /* Not implementation here */
        *puLong = 0;

        return TRUE;
    }

    if (strcmp(ParamName, "PortState") == 0)
    {
        /* collect value */
        //CosaDmlBrgPortGetInfo(NULL, pDmlBridge->Cfg.InstanceNumber, pPort->Cfg.InstanceNumber, &pPort->Info);//LNT_EMU

        *puLong = pPort->Info.PortState;

        return TRUE;
    }

    if (strcmp(ParamName, "AcceptableFrameTypes") == 0)
    {
        /* collect value */
        *puLong = pPort->Cfg.AcceptableFrameTypes;

        return TRUE;
    }

    if (strcmp(ParamName, "X_CISCO_COM_Mode") == 0)
    {
        /* collect value */
        // no need to collect from lower level, because Middle Layer should have the latest
        //        CosaDmlBrgPortGetCfg(NULL, pDmlBridge->Cfg.InstanceNumber, &(pPort->Cfg)); 
        *puLong = pPort->Cfg.mode;
        //        AnscTraceFlow(("%s: Bridge.%d.Port.%d.%s = %d\n", __FUNCTION__, 
        //               pDmlBridge->Cfg.InstanceNumber, pPort->Cfg.InstanceNumber, ParamName, pPort->Cfg.mode));
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Port_GetParamStringValue
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
Port_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_PORT_FULL         pPort            = (PCOSA_DML_BRG_PORT_FULL  )pCosaContext->hContext;
    PUCHAR                          pLowerLayer      = NULL;
    
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* collect value */
        AnscCopyString(pValue, pPort->Cfg.Alias);

        return 0;
    }


    if (strcmp(ParamName, "Name") == 0)
    {
        /* collect value */
        //AnscCopyString(pValue, pPort->Cfg.LinkName);//LNT_EMU
        AnscCopyString(pValue, pPort->Info.Name);//RDKB_EMU

        return 0;
    }

    if (strcmp(ParamName, "LowerLayers") == 0)
    //if( AnscEqualString(ParamName, "LinkName", TRUE) )
    {
        /* collect value */
#ifdef _COSA_SIM_

#if 1 //RDKB-EMU
	    if(pPort->Cfg.InstanceNumber == 2)
	    {
		    if(AnscEqualString( pPort->Info.Name, "eth1", TRUE))
		    {
			    pLowerLayer = CosaUtilGetLowerLayers("Device.Ethernet.Interface.", "eth1");
			    if ( pLowerLayer != NULL )
			    {
				    AnscCopyString(pValue, pLowerLayer);
				    AnscFreeMemory(pLowerLayer);
			    }
		}
			    if(AnscEqualString( pPort->Info.Name, "gretap0", TRUE))
			    {
				    pLowerLayer = CosaUtilGetLowerLayers("Device.X_CISCO_COM_GRE.Interface.", "gretap0");
				    if ( pLowerLayer != NULL )
				    {
					    AnscCopyString(pValue, pLowerLayer);
					    AnscFreeMemory(pLowerLayer);
				    }

			    }
	    }
	    if(pPort->Cfg.InstanceNumber == 3)
	    {
		    if(AnscEqualString( pPort->Info.Name, "wlan0", TRUE))
		    {
			    pLowerLayer = CosaUtilGetLowerLayers("Device.WiFi.SSID.", "wlan0");
			    if ( pLowerLayer != NULL )
			    {
				    AnscCopyString(pValue, pLowerLayer);
				    AnscFreeMemory(pLowerLayer);
			    }
			}
			    if(AnscEqualString( pPort->Info.Name, "wlan0_0", TRUE))
			    {
				    pLowerLayer = CosaUtilGetLowerLayers("Device.WiFi.SSID.", "wlan0_0");
				    if ( pLowerLayer != NULL )
				    {
					    AnscCopyString(pValue, pLowerLayer);
					    AnscFreeMemory(pLowerLayer);
				    }

			    }
	    }
#endif
	if(AnscEqualString( pPort->Cfg.LinkName, "wlan0", TRUE))//LNT_EMU
	{
		AnscCopyString(pValue, "Device.WiFi.SSID.1");
	}
	else if ( AnscEqualString( pPort->Cfg.LinkName, "eth1", TRUE)) {
        	    AnscCopyString(pValue, "Device.Ethernet.Interface.1");
        }
	else if ( AnscEqualString( pPort->Cfg.LinkName, "eth2", TRUE)) {
        	    AnscCopyString(pValue, "Device.Ethernet.Interface.2");
        }
	else if ( AnscEqualString( pPort->Cfg.LinkName, "eth3", TRUE)) {
        	    AnscCopyString(pValue, "Device.Ethernet.Interface.3");
        }
        else {
            CcspTraceInfo(("----------Unknown interface...\n")); 
        }
#elif defined  _COSA_DRG_CNS_
        CcspTraceInfo(("----------LinkName:%s\n", pPort->Info.Name)); 
        if( AnscEqualString( pPort->Info.Name, "brlan0", TRUE) ) {
            pLowerLayer = CosaUtilGetLowerLayers("Device.Bridging.Bridge.", "lan0");
            if ( pLowerLayer != NULL )
            {
                AnscCopyString(pValue, pLowerLayer);
                AnscFreeMemory(pLowerLayer);
            }
            pLowerLayer = CosaUtilGetLowerLayers("Device.Bridging.Bridge.", "eth0");
            if ( pLowerLayer != NULL )
            {
                AnscCatString(pValue, ",");
                AnscCatString(pValue, pLowerLayer);
                AnscFreeMemory(pLowerLayer);
            }
            pLowerLayer = CosaUtilGetLowerLayers("Device.Bridging.Bridge.", "eth1");
            if ( pLowerLayer != NULL )
            {
                AnscCatString(pValue, ",");
                AnscCatString(pValue, pLowerLayer);
                AnscFreeMemory(pLowerLayer);
            }
        }
        else if ( AnscEqualString( pPort->Info.Name, "lan0", TRUE) ) {
            pLowerLayer = CosaUtilGetLowerLayers("Device.Ethernet.Interface.", pPort->Info.Name);
            if ( pLowerLayer != NULL )
            {
                AnscCopyString(pValue, pLowerLayer);
                AnscFreeMemory(pLowerLayer);
            }
        }
        else if ( AnscEqualString( pPort->Info.Name, "eth0", TRUE)) {
            AnscCopyString(pValue, "Device.WiFi.SSID.1");
        }
        else if ( AnscEqualString( pPort->Info.Name, "eth1", TRUE)) {
            AnscCopyString(pValue, "Device.WiFi.SSID.2");
        }
        else {
            CcspTraceInfo(("----------Unknown interface...\n")); 
        }
#elif defined _COSA_DRG_TPG_
        if( AnscEqualString( pPort->Info.Name, "lan0", TRUE) ) {
            pLowerLayer = CosaUtilGetLowerLayers("Device.Bridging.Bridge.", "br0"); 
            if ( pLowerLayer != NULL )
            {
                AnscCopyString(pValue, pLowerLayer);
                AnscFreeMemory(pLowerLayer);
            }
            pLowerLayer = CosaUtilGetLowerLayers("Device.Bridging.Bridge.", "br1");
            if ( pLowerLayer != NULL )
            {
                AnscCatString(pValue, ",");
                AnscCatString(pValue, pLowerLayer);
                AnscFreeMemory(pLowerLayer);
            }
            pLowerLayer = CosaUtilGetLowerLayers("Device.Bridging.Bridge.", "br2");
            if ( pLowerLayer != NULL )
            {
                AnscCatString(pValue, ",");
                AnscCatString(pValue, pLowerLayer);
                AnscFreeMemory(pLowerLayer);
            }
            pLowerLayer = CosaUtilGetLowerLayers("Device.Bridging.Bridge.", "br3");
            if ( pLowerLayer != NULL )
            {
                AnscCatString(pValue, ",");
                AnscCatString(pValue, pLowerLayer);
                AnscFreeMemory(pLowerLayer);
            }
            pLowerLayer = CosaUtilGetLowerLayers("Device.Bridging.Bridge.", "br4");
            if ( pLowerLayer != NULL )
            {
                AnscCatString(pValue, ",");
                AnscCatString(pValue, pLowerLayer);
                AnscFreeMemory(pLowerLayer);
            }
        }
        else if ( AnscEqualString( pPort->Info.Name, "br0", TRUE) ) {
            pLowerLayer = CosaUtilGetLowerLayers("Device.Ethernet.Interface.", "sw0");
            if ( pLowerLayer != NULL )
            {
                AnscCopyString(pValue, pLowerLayer);
                AnscFreeMemory(pLowerLayer);
            }
        }
        else if ( AnscEqualString( pPort->Info.Name, "br1", TRUE) ) {
            pLowerLayer = CosaUtilGetLowerLayers("Device.Ethernet.Interface.", "sw1");
            if ( pLowerLayer != NULL )
            {
                AnscCopyString(pValue, pLowerLayer);
                AnscFreeMemory(pLowerLayer);
            }
        }
        else if ( AnscEqualString( pPort->Info.Name, "br2", TRUE) ) {
            pLowerLayer = CosaUtilGetLowerLayers("Device.Ethernet.Interface.", "sw2");
            if ( pLowerLayer != NULL )
            {
                AnscCopyString(pValue, pLowerLayer);
                AnscFreeMemory(pLowerLayer);
            }
        }
        else if ( AnscEqualString( pPort->Info.Name, "br3", TRUE) ) {
            pLowerLayer = CosaUtilGetLowerLayers("Device.Ethernet.Interface.", "sw3");
            if ( pLowerLayer != NULL )
            {
                AnscCopyString(pValue, pLowerLayer);
                AnscFreeMemory(pLowerLayer);
            }
        }
        else if ( AnscEqualString( pPort->Info.Name, "br4", TRUE) ) {
            AnscCopyString(pValue, "Device.MoCA.Interface.1");
        }

#else
        //$HL 4/16/2013
        //ToDO
        //LinkName has the whole path (wrong)
        switch (pPort->Cfg.LinkType) {
            case COSA_DML_BRG_LINK_TYPE_EthVlan:
            case COSA_DML_BRG_LINK_TYPE_Eth:
                pLowerLayer = CosaUtilGetLowerLayers("Device.Ethernet.Interface.", pPort->Cfg.LinkName);
                AnscCopyString(pValue, pLowerLayer);
                AnscFreeMemory(pLowerLayer);
                break;
            case COSA_DML_BRG_LINK_TYPE_Moca:
                pLowerLayer = CosaUtilGetLowerLayers("Device.MoCA.Interface.", pPort->Cfg.LinkName);
                AnscTraceFlow(("<HL>%s moca lowerlayer=%s\n",__FUNCTION__,pLowerLayer ));
                AnscCopyString(pValue, pLowerLayer);
                AnscFreeMemory(pLowerLayer);
                break;
            case COSA_DML_BRG_LINK_TYPE_WiFiSsid:
                pLowerLayer = CosaUtilGetLowerLayers("Device.WiFi.SSID.", pPort->Cfg.LinkName);
                AnscTraceFlow(("<HL>%s wifi lowerlayer=%s\n",__FUNCTION__,pLowerLayer ));
                AnscCopyString(pValue, pLowerLayer);
                AnscFreeMemory(pLowerLayer);
                break;
            case COSA_DML_BRG_LINK_TYPE_Bridge:
                pLowerLayer = CosaUtilGetLowerLayers("Device.Bridging.Bridge.", pPort->Cfg.LinkName);
                AnscCopyString(pValue, pLowerLayer);
                AnscFreeMemory(pLowerLayer);
                break;
            case COSA_DML_BRG_LINK_TYPE_Gre: //$HL 07/15/2013
                pLowerLayer = CosaUtilGetLowerLayers("Device.X_CISCO_COM_GRE.Interface.", pPort->Cfg.LinkName);
                AnscTraceFlow(("<HL>%s Gre lowerlayer=%s\n",__FUNCTION__,pLowerLayer ));
                AnscCopyString(pValue, pLowerLayer);
                AnscFreeMemory(pLowerLayer);
                break;
            case COSA_DML_BRG_LINK_TYPE_Usb:
            case COSA_DML_BRG_LINK_TYPE_Hpna:
            case COSA_DML_BRG_LINK_TYPE_HomePlug:
            case COSA_DML_BRG_LINK_TYPE_Upa:
            case COSA_DML_BRG_LINK_TYPE_NONE:
            default:
                pValue[0] ='\0';
                break;
        }
        //AnscCopyString(pValue, pPort->Cfg.LinkName);
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
        Port_SetParamBoolValue
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
extern ANSC_HANDLE bus_handle;
extern char        g_Subsystem[32];

static int
PsmGet1(const char *param, char *value, int size)
{
    char *val;
    CCSP_MESSAGE_BUS_INFO *businfo;

    if (PSM_Get_Record_Value2(g_MessageBusHandle, g_GetSubsystemPrefix(g_pDslhDmlAgent),
                (char *)param, NULL, &val) != CCSP_SUCCESS)
        return -1;

    snprintf(value, size, "%s", val);

    businfo = g_MessageBusHandle;
    businfo->freefunc(val);
    return 0;
}

BOOL
Port_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_PORT_FULL         pPort            = (PCOSA_DML_BRG_PORT_FULL  )pCosaContext->hContext;
    static int			    pvid	     = 0;

    if (!pvid)
    {
        char val[12];
        
        if (PsmGet1("dmsb.l2net.2.Vid", val, sizeof(val)) != 0)
            pvid = 0;
        
        pvid = atoi(val);
    }

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* save update to backup */
        pPort->Cfg.bEnabled = bValue;
	
        printf("link name is %s %d\n", pPort->Cfg.LinkName, pPort->Cfg.PVID);		

    #if CFG_TR181_BRIDGE_ETH4_SYSCFG
        /* we are setting ethernet port 4 to VLAN*/
        if( !strcmp(pPort->Info.Name, SWITCH_PORT_3_NAME) &&  pPort->Cfg.PVID == pvid)
        {
            syscfg_set(NULL, SYSCFG_HOMESECURITY_ETH4_FLAG, bValue == TRUE ? "1" : "0");
            syscfg_commit();
        }
    #endif
    
        return TRUE;
    }

    if (strcmp(ParamName, "ManagementPort") == 0)
    {
        /* save update to backup */
        pPort->Cfg.bManagementPort = bValue;

        return TRUE;
    }

    if (strcmp(ParamName, "IngressFiltering") == 0)
    {
        /* save update to backup */
        pPort->Cfg.bIngressFiltering = bValue;

        return TRUE;
    }

    if (strcmp(ParamName, "PriorityTagging") == 0)
    {
        /* save update to backup */
        pPort->Cfg.bPriorityTagging = bValue;

        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Port_SetParamIntValue
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
Port_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_PORT_FULL         pPort            = (PCOSA_DML_BRG_PORT_FULL  )pCosaContext->hContext;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "PVID") == 0)
    {
        /* save update to backup */
        pPort->Cfg.PVID = iValue;

        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Port_SetParamUlongValue
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
Port_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_PORT_FULL         pPort            = (PCOSA_DML_BRG_PORT_FULL  )pCosaContext->hContext;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "DefaultUserPriority") == 0)
    {
        /* save update to backup */
        pPort->Cfg.DftUserPriority = (UCHAR)uValue;

        return TRUE;
    }

    if (strcmp(ParamName, "PriorityRegeneration") == 0)
    {
        /* Not supported here */
        AnscCopyString(pPort->Cfg.PriorityRegeneration, "");

        return TRUE;
    }

    if (strcmp(ParamName, "AcceptableFrameTypes") == 0)
    {
        /* save update to backup */
        pPort->Cfg.AcceptableFrameTypes = uValue;

        return TRUE;
    }

    if (strcmp(ParamName, "X_CISCO_COM_Mode") == 0)
    {
        /* save update to backup */
        // LinkName is just the sw_X without "-t", even for PassThrough
        // Value of "-t" is assigned in _COSA_FORMAT_MEMBERS_STRING
        /*
        if(uValue == COSA_DML_BPORT_PASSTHRU) { 
            if(pPort->Cfg.LinkName && !AnscStrStr(pPort->Cfg.LinkName, "-t")) { // tagging -> passthrough
                AnscCatString(pPort->Cfg.LinkName, "-t");
                AnscTraceFlow(("<%s>: Tagging to PassThru: LinkName=%s\n", __func__, pPort->Cfg.LinkName));
            }
        }
        else if(uValue == COSA_DML_BPORT_TAGGING) { 
            if(pPort->Cfg.LinkName && AnscStrStr(pPort->Cfg.LinkName, "-t")) { // passthrough -> tagging
                *(AnscStrStr(pPort->Cfg.LinkName, "-t")) = '\0';
                AnscTraceFlow(("<%s>: PassThru to Tagging: LinkName=%s\n", __func__, pPort->Cfg.LinkName));
            }
        }
        */
        pPort->Cfg.mode = uValue;
        //        AnscTraceFlow(("%s: Bridge.%d.Port.%d.%s = %d\n", __FUNCTION__, 
        //               pDmlBridge->Cfg.InstanceNumber, pPort->Cfg.InstanceNumber, ParamName, pPort->Cfg.mode));

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Port_SetParamStringValue
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
Port_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_PORT_FULL         pPort            = (PCOSA_DML_BRG_PORT_FULL  )pCosaContext->hContext;
    ULONG                           ulEntryNameLen   = 256;
    UCHAR                           ucEntryParamName[256] = {0};
    UCHAR                           ucEntryNameValue[256] = {0};
    parameterValStruct_t            varStruct;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* save update to backup */
        AnscCopyString(pPort->Cfg.Alias, pString);

        return TRUE;
    }


    if (strcmp(ParamName, "LinkName") == 0)//LNT_EMU
    //if( AnscEqualString(ParamName, "LowerLayers", TRUE) )
    {
        /* save update to backup */
#ifdef _COSA_SIM_

#if 0 //LNT_EMU
        _ansc_sprintf(ucEntryParamName, "%s%s", pString, "Name");
        if ( ( 0 == CosaGetParamValueString(ucEntryParamName, ucEntryNameValue, &ulEntryNameLen ) ) &&
             ( AnscSizeOfString(ucEntryNameValue) != 0 ) )
        {
            AnscCopyString(pPort->Cfg.LinkName, ucEntryNameValue);
            
            return TRUE;
        }
#endif
	AnscCopyString(pPort->Cfg.LinkName, pString);//LNT_EMU
	return TRUE;
#else
        //$HL 4/16/2013
        //fixed the linkname and linktype issue
        _ansc_sprintf(ucEntryParamName, "%s%s", pString, ".Name");
        if ( !pPort->Cfg.bManagementPort)
        {
            varStruct.parameterName = ucEntryParamName;
            varStruct.parameterValue = ucEntryNameValue;
            ulEntryNameLen = sizeof(ucEntryNameValue);
fprintf(stderr, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 1\n"); 	
			if (_ansc_strstr(pString,"WiFi"))
            {
            
                int ret = COSAGetParamValueByPathName(g_MessageBusHandle,&varStruct,&ulEntryNameLen);
                
                AnscTraceFlow(("<HL>%s >>>>>> ret is %d \n",__func__,ret));
            
                 if ( ret == 0 && AnscSizeOfString(ucEntryNameValue) != 0 ) {
                 
                    AnscTraceFlow(("<HL>%s 11 STRING LENGTH NOT ZERO \n",__func__));
                    AnscCopyString(pPort->Cfg.LinkName, ucEntryNameValue);
                    pPort->Cfg.LinkType = COSA_DML_BRG_LINK_TYPE_WiFiSsid;
                    _ansc_memset(ucEntryParamName, 0, sizeof(ucEntryParamName));
                    _ansc_memset(ucEntryNameValue, 0, sizeof(ucEntryNameValue));
                    _ansc_sprintf(ucEntryParamName, "%s%s", pString, ".LowerLayers");
                  }  
            }
            else if (( 0 == CosaGetParamValueString(ucEntryParamName, ucEntryNameValue, &ulEntryNameLen ) ) &&
                ( AnscSizeOfString(ucEntryNameValue) != 0 ) )
            {
                AnscCopyString(pPort->Cfg.LinkName, ucEntryNameValue);
                if (_ansc_strstr(pString,"Ethernet"))
                {
                    pPort->Cfg.LinkType = COSA_DML_BRG_LINK_TYPE_Eth;
                }
                else if (_ansc_strstr(pString,"MoCA"))
                {
                    pPort->Cfg.LinkType = COSA_DML_BRG_LINK_TYPE_Moca;
                }   
                else if (_ansc_strstr(pString,"Bridge"))
                {
                    pPort->Cfg.LinkType = COSA_DML_BRG_LINK_TYPE_Bridge;   
                }
                //$HL 07/15/2013
                else if (_ansc_strstr(pString,"GRE"))
                {
                    pPort->Cfg.LinkType = COSA_DML_BRG_LINK_TYPE_Gre;   
                }
            }
            else 
            {
                CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
                pPort->Cfg.LinkType = COSA_DML_BRG_LINK_TYPE_NONE;
                pPort->Cfg.LinkName[0]='\0';
            }
            _ansc_memset(ucEntryParamName, 0, sizeof(ucEntryParamName));
            _ansc_memset(ucEntryNameValue, 0, sizeof(ucEntryNameValue));
            _ansc_sprintf(ucEntryParamName, "%s%s", pString, ".Upstream");
        }            
        else
        {
            pPort->Cfg.LinkType =COSA_DML_BRG_LINK_TYPE_NONE;
            pPort->Cfg.LinkName[0]='\0';
            pPort->Cfg.bUpstream = FALSE;
        }

        AnscCopyString(pPort->Info.Name,pPort->Cfg.LinkName);

        /* Do not spontaneously change the port mode -- from 1.6.2 commit c9992ba4 */
        //pPort->Cfg.mode = COSA_DML_BPORT_TAGGING;
        AnscTraceFlow(("<HL>%s search linkname and linktype pString=%s ucEntryParamName=%s \nucEntryNameValue=%s linktype=%d linkname=%s\n",
            __FUNCTION__, pString, ucEntryParamName,ucEntryNameValue,pPort->Cfg.LinkType,pPort->Cfg.LinkName));


        if ( !pPort->Cfg.bManagementPort )
        {
            varStruct.parameterName = ucEntryParamName;
            varStruct.parameterValue = ucEntryNameValue;
            ulEntryNameLen = sizeof(ucEntryNameValue);
            pPort->Cfg.bUpstream = FALSE;
fprintf(stderr, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 2\n"); 
			if (_ansc_strstr(pString,"WiFi") &&
                0 == (COSAGetParamValueByPathName(g_MessageBusHandle,&varStruct,&ulEntryNameLen)) && ( AnscSizeOfString(ucEntryNameValue) != 0 ))
            {
            
                AnscTraceFlow(("<HL>%s 222 STRING LENGTH NOT ZERO \n",__func__));
                AnscTraceFlow(("<HL>%s %s=%s\n", __func__,pString,ucEntryNameValue));
                _ansc_memset(ucEntryParamName, 0, sizeof(ucEntryParamName));
                _ansc_sprintf(ucEntryParamName, "%s%s", ucEntryNameValue,".Upstream");
                _ansc_memset(ucEntryNameValue, 0, sizeof(ucEntryNameValue));

                if( 0 == (COSAGetParamValueByPathName(g_MessageBusHandle,&varStruct,&ulEntryNameLen)) &&
                    ( AnscSizeOfString(ucEntryNameValue) != 0 ) )
                {
                    AnscTraceFlow(("<HL>%s %s=%s\n", __func__, pString, ucEntryNameValue));
                    if (_ansc_strstr(ucEntryNameValue,"true"))
                    {
                        pPort->Cfg.bUpstream = true;
                    }
                }
            }
            if (_ansc_strstr(pString,"WiFi") &&
                !(COSAGetParamValueByPathName(g_MessageBusHandle,&varStruct,&ulEntryNameLen)) && ( AnscSizeOfString(ucEntryNameValue) != 0 ))
            {
                AnscTraceFlow(("<HL>%s %s=%s\n", __FUNCTION__,pString,ucEntryNameValue));
                _ansc_memset(ucEntryParamName, 0, sizeof(ucEntryParamName));
                _ansc_sprintf(ucEntryParamName, "%s%s", ucEntryNameValue,".Upstream");
                _ansc_memset(ucEntryNameValue, 0, sizeof(ucEntryNameValue));

                if( !(COSAGetParamValueByPathName(g_MessageBusHandle,&varStruct,&ulEntryNameLen)) &&
                    ( AnscSizeOfString(ucEntryNameValue) != 0 ) )
                {
                    AnscTraceFlow(("<HL>%s %s=%s\n", __FUNCTION__, pString, ucEntryNameValue));
                    if (_ansc_strstr(ucEntryNameValue,"true"))
                    {
                        pPort->Cfg.bUpstream = true;
                    }
                }
            }
            else if (( 0 == CosaGetParamValueString(ucEntryParamName, ucEntryNameValue, &ulEntryNameLen ) ) &&
                ( AnscSizeOfString(ucEntryNameValue) != 0 ) )
            {
                AnscTraceFlow(("<HL>%s %s=%s\n", __FUNCTION__,pString,ucEntryNameValue));
                if (_ansc_strstr(ucEntryNameValue,"true"))
                {
                    pPort->Cfg.bUpstream = true;
                }
            }
        }     
        AnscTraceFlow(("<HL>%s search UpStream pString=%s \nucEntryParamName=%s \nucEntryNameValue=%s \nbUpstream=%d\n",
            __FUNCTION__, pString, ucEntryParamName,ucEntryNameValue,pPort->Cfg.bUpstream));       
        return TRUE;
#endif
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Port_Validate
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
Port_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge       = (PCOSA_DML_BRG_FULL_ALL   )pCosaContext->hParentTable; 
    PCOSA_DML_BRG_PORT_FULL         pPort            = (PCOSA_DML_BRG_PORT_FULL  )pCosaContext->hContext;
    PCOSA_DML_BRG_PORT_FULL         pPort2           = (PCOSA_DML_BRG_PORT_FULL  )NULL;
    PSLIST_HEADER                   pListHead        = (PSLIST_HEADER            )&pDmlBridge->PortList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2    = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry      = (PSINGLE_LINK_ENTRY       )NULL;

    pSLinkEntry = AnscSListGetFirstEntry(pListHead);

    while ( pSLinkEntry )
    {
        pCosaContext2 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        pSLinkEntry   = AnscSListGetNextEntry(pSLinkEntry);

        pPort2 = (PCOSA_DML_BRG_PORT_FULL)pCosaContext2->hContext;

        if ( pPort2 && ((ULONG)pPort2 != (ULONG)pPort) && AnscEqualString(pPort->Cfg.Alias, pPort2->Cfg.Alias, TRUE))
        {
            AnscCopyString(pReturnParamName, "Alias");

            *puLength = AnscSizeOfString("Alias");

            return FALSE;
        }
    }

    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Port_Commit
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
Port_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_BRIDGING        pCosaDMBridging  = (PCOSA_DATAMODEL_BRIDGING )g_pCosaBEManager->hBridging;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_PORT_FULL         pPort            = (PCOSA_DML_BRG_PORT_FULL  )pCosaContext->hContext;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge       = (PCOSA_DML_BRG_FULL_ALL   )pCosaContext->hParentTable;

    if ( pCosaContext->bNew )
    {
        pCosaContext->bNew = FALSE;
        CosaDmlBrgPortAddEntry(NULL, pDmlBridge->Cfg.InstanceNumber, &pPort->Cfg);

        CosaBridgingRegDelInfo((ANSC_HANDLE)pCosaDMBridging, (ANSC_HANDLE)pCosaContext);
        
    }
    else
    {
        CosaDmlBrgPortSetCfg(NULL, pDmlBridge->Cfg.InstanceNumber, &pPort->Cfg);
    }
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Port_Rollback
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
Port_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    return 0;
}

/***********************************************************************

 APIs for Object:

    Bridging.Bridge.{i}.Port.{i}.Stats.

    *  PortStats_GetParamBoolValue
    *  PortStats_GetParamIntValue
    *  PortStats_GetParamUlongValue
    *  PortStats_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        PortStats_GetParamBoolValue
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
PortStats_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        PortStats_GetParamIntValue
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
PortStats_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        PortStats_GetParamUlongValue
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
PortStats_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_PORT_FULL         pPort            = (PCOSA_DML_BRG_PORT_FULL  )pCosaContext->hContext;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge       = (PCOSA_DML_BRG_FULL_ALL   )pCosaContext->hParentTable;
    COSA_DML_IF_STATS               Stats;

    CosaDmlBrgPortGetStats(NULL, pDmlBridge->Cfg.InstanceNumber, pPort->Cfg.InstanceNumber, &Stats);//RDKB-EMU

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "ErrorsSent") == 0)
    {
        /* collect value */
        *puLong = Stats.ErrorsSent;

        return TRUE;
    }

    if (strcmp(ParamName, "ErrorsReceived") == 0)
    {
        /* collect value */
        *puLong = Stats.ErrorsReceived;

        return TRUE;
    }

    if (strcmp(ParamName, "DiscardPacketsSent") == 0)
    {
        /* collect value */
        *puLong = Stats.DiscardPacketsSent;

        return TRUE;
    }

    if (strcmp(ParamName, "DiscardPacketsReceived") == 0)
    {
        /* collect value */
        *puLong = Stats.DiscardPacketsReceived;

        return TRUE;
    }

    if (strcmp(ParamName, "UnknownProtoPacketsReceived") == 0)
    {
        /* collect value */
        *puLong = Stats.UnknownProtoPacketsReceived;

        return TRUE;
    }

    if (strcmp(ParamName, "BytesSent") == 0)
    {
        /* collect value */
        *puLong = Stats.BytesSent;

        return TRUE;
    }

    if (strcmp(ParamName, "BytesReceived") == 0)
    {
        /* collect value */
        *puLong = Stats.BytesReceived;

        return TRUE;
    }

    if (strcmp(ParamName, "PacketsSent") == 0)
    {
        /* collect value */
        *puLong = Stats.PacketsSent;

        return TRUE;
    }

    if (strcmp(ParamName, "PacketsReceived") == 0)
    {
        /* collect value */
        *puLong = Stats.PacketsReceived;

        return TRUE;
    }

    if (strcmp(ParamName, "UnicastPacketsSent") == 0)
    {
        /* collect value */
        *puLong = Stats.UnicastPacketsSent;

        return TRUE;
    }

    if (strcmp(ParamName, "UnicastPacketsReceived") == 0)
    {
        /* collect value */
        *puLong = Stats.UnicastPacketsReceived;

        return TRUE;
    }

    if (strcmp(ParamName, "MulticastPacketsSent") == 0)
    {
        /* collect value */
        *puLong = Stats.MulticastPacketsSent;

        return TRUE;
    }

    if (strcmp(ParamName, "MulticastPacketsReceived") == 0)
    {
        /* collect value */
        *puLong = Stats.MulticastPacketsReceived;

        return TRUE;
    }

    if (strcmp(ParamName, "BroadcastPacketsSent") == 0)
    {
        /* collect value */
        *puLong = Stats.BroadcastPacketsSent;

        return TRUE;
    }

    if (strcmp(ParamName, "BroadcastPacketsReceived") == 0)
    {
        /* collect value */
        *puLong = Stats.BroadcastPacketsReceived;

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        PortStats_GetParamStringValue
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
PortStats_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/***********************************************************************

 APIs for Object:

    Bridging.Bridge.{i}.VLAN.{i}.

    *  VLAN_GetEntryCount
    *  VLAN_GetEntry
    *  VLAN_AddEntry
    *  VLAN_DelEntry
    *  VLAN_GetParamBoolValue
    *  VLAN_GetParamIntValue
    *  VLAN_GetParamUlongValue
    *  VLAN_GetParamStringValue
    *  VLAN_SetParamBoolValue
    *  VLAN_SetParamIntValue
    *  VLAN_SetParamUlongValue
    *  VLAN_SetParamStringValue
    *  VLAN_Validate
    *  VLAN_Commit
    *  VLAN_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        VLAN_GetEntryCount
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
VLAN_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge   = (PCOSA_DML_BRG_FULL_ALL   )pCosaContext->hContext;

    return AnscSListQueryDepth(&pDmlBridge->VLANList);
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        VLAN_GetEntry
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
VLAN_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge    = (PCOSA_DML_BRG_FULL_ALL   )pCosaContext->hContext;
    PSLIST_HEADER                   pListHead     = (PSLIST_HEADER            )&pDmlBridge->VLANList;
    PSINGLE_LINK_ENTRY              pSLinkEntry   = (PSINGLE_LINK_ENTRY       )NULL;

    pSLinkEntry = AnscSListGetEntryByIndex(pListHead, nIndex);

    if ( pSLinkEntry )
    {
        pCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);

        *pInsNumber = pCosaContext->InstanceNumber;
    }

    return (ANSC_HANDLE)pCosaContext;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        VLAN_AddEntry
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
VLAN_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_BRIDGING        pCosaDMBridging = (PCOSA_DATAMODEL_BRIDGING )g_pCosaBEManager->hBridging;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge      = (PCOSA_DML_BRG_FULL_ALL   )pCosaContext->hContext;
    PSLIST_HEADER                   pListHead       = (PSLIST_HEADER            )&pDmlBridge->VLANList;
    PCOSA_DML_BRG_VLAN_FULL         pVLAN           = (PCOSA_DML_BRG_VLAN_FULL  )NULL;

	// not support
	return NULL;

    pVLAN = (PCOSA_DML_BRG_VLAN_FULL)AnscAllocateMemory(sizeof(COSA_DML_BRG_VLAN_FULL));

    if ( !pVLAN )
    {
        return NULL;
    }

    _ansc_sprintf(pVLAN->Cfg.Alias, "cpe-VLAN%d", pDmlBridge->ulNextVLANInsNum);
    _ansc_sprintf(pVLAN->Info.Name, "cpe-VLAN%d", pDmlBridge->ulNextVLANInsNum);

    /* Update the middle layer cache */
    if ( TRUE )
    {
        pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

        if ( !pCosaContext )
        {
            AnscFreeMemory(pVLAN);
            
            return NULL;
        }

        pCosaContext->InstanceNumber = pVLAN->Cfg.InstanceNumber = pDmlBridge->ulNextVLANInsNum;

        pDmlBridge->ulNextVLANInsNum++;

        if ( pDmlBridge->ulNextVLANInsNum == 0 )
        {
            pDmlBridge->ulNextVLANInsNum = 1;
        }

        pCosaContext->hContext         = (ANSC_HANDLE)pVLAN;
        pCosaContext->hParentTable     = (ANSC_HANDLE)pDmlBridge;
        pCosaContext->hPoamIrepUpperFo = pCosaDMBridging->hIrepFolderBRGHA;
        pCosaContext->bNew             = TRUE;

        CosaSListPushEntryByInsNum(pListHead, (ANSC_HANDLE)pCosaContext);

        CosaBridgingRegAddInfo
            (
                (ANSC_HANDLE)pCosaDMBridging,
                COSA_DML_RR_NAME_Bridge_VLAN_NextInsNumber,
                pDmlBridge->ulNextVLANInsNum,
                COSA_DML_RR_NAME_Bridge_VLAN_Prefix,
                pDmlBridge->Cfg.InstanceNumber,
                pVLAN->Cfg.Alias,
                (ANSC_HANDLE)pCosaContext
            );

        CosaDmlBrgVlanAddEntry(NULL, pDmlBridge->Cfg.InstanceNumber,pVLAN);
    }

    *pInsNumber = pCosaContext->InstanceNumber;

    return (ANSC_HANDLE)pCosaContext; /* return the handle */
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        VLAN_DelEntry
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
VLAN_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge      = (PCOSA_DML_BRG_FULL_ALL   )pCosaContext->hContext;
    PSLIST_HEADER                   pListHead       = (PSLIST_HEADER            )&pDmlBridge->VLANList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2   = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_DML_BRG_VLAN_FULL         pVLAN           = (PCOSA_DML_BRG_VLAN_FULL  )pCosaContext2->hContext;

    PSLIST_HEADER                   pVlanPortList   = (PSLIST_HEADER            )&pDmlBridge->VLANPortList;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = (PSINGLE_LINK_ENTRY       )AnscSListGetFirstEntry(pVlanPortList);
    
    //$HL 7/8/2013
 /*   while ( pSLinkEntry )
    {
        PCOSA_CONTEXT_LINK_OBJECT pCosaContext3 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        pSLinkEntry   = AnscSListGetNextEntry(pSLinkEntry);

        PCOSA_DML_BRG_VLANPORT_FULL pVLANPort = (PCOSA_DML_BRG_VLANPORT_FULL)pCosaContext3->hContext;

        if ( pVLANPort && pVLANPort->Cfg.VLANInsNum == pVLAN->Cfg.InstanceNumber)
        {
            // If VLAN is deleted, deleted its references in VLANPort
            pVLANPort->Cfg.VLANInsNum = 0;
        }
    }
*/

    if (pVLAN->Cfg.bEnabled && pVLAN->Cfg.VLANID > 0)
    {
	printf("Inside IF\n");
       //CosaDmlBrgVlanDelEntry(NULL, pCosaContext->InstanceNumber, pVLAN->Cfg.InstanceNumber);
       CosaDmlBrgVlanDelEntry(NULL, pDmlBridge->Cfg.InstanceNumber, pVLAN->Cfg.VLANID);//LNT_EMU
    }

    /* Update the middle layer cache */

    AnscSListPopEntryByLink(pListHead, &pCosaContext2->Linkage);

    AnscFreeMemory(pVLAN);
    AnscFreeMemory(pCosaContext2);

    return 0; /* succeeded */
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        VLAN_GetParamBoolValue
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
VLAN_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_VLAN_FULL         pVLAN            = (PCOSA_DML_BRG_VLAN_FULL  )pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        *pBool = pVLAN->Cfg.bEnabled;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        VLAN_GetParamIntValue
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
VLAN_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_VLAN_FULL         pVLAN            = (PCOSA_DML_BRG_VLAN_FULL  )pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "VLANID") == 0)
    {
        /* collect value */
        *pInt = pVLAN->Cfg.VLANID;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        VLAN_GetParamUlongValue
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
VLAN_GetParamUlongValue
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
        VLAN_GetParamStringValue
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
VLAN_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_VLAN_FULL         pVLAN            = (PCOSA_DML_BRG_VLAN_FULL  )pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* collect value */
        AnscCopyString(pValue, pVLAN->Cfg.Alias);

        return 0;
    }

    if (strcmp(ParamName, "Name") == 0)
    {
        /* collect value */
        //AnscCopyString(pValue, pVLAN->Info.Name);
        AnscCopyString(pValue, pVLAN->Cfg.Alias);//LNT_EMU
        return 0;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        VLAN_SetParamBoolValue
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
VLAN_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_VLAN_FULL         pVLAN            = (PCOSA_DML_BRG_VLAN_FULL  )pCosaContext->hContext;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* save update to backup */
        //$HL 07/2/2013
        pVLAN->Cfg.bEnabled = bValue;//LNT_EMU

        return TRUE;//LNT_EMU
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        VLAN_SetParamIntValue
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
VLAN_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_VLAN_FULL         pVLAN            = (PCOSA_DML_BRG_VLAN_FULL  )pCosaContext->hContext;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "VLANID") == 0)
    {
        /* save update to backup */
        pVLAN->Cfg.VLANID = iValue;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        VLAN_SetParamUlongValue
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
VLAN_SetParamUlongValue
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
        VLAN_SetParamStringValue
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
VLAN_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_VLAN_FULL         pVLAN            = (PCOSA_DML_BRG_VLAN_FULL  )pCosaContext->hContext;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* save update to backup */
        AnscCopyString(pVLAN->Cfg.Alias, pString);
        return TRUE;
    }

    if (strcmp(ParamName, "Name") == 0)
    {
        /* save update to backup */
        return FALSE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        VLAN_Validate
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
VLAN_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge       = (PCOSA_DML_BRG_FULL_ALL   )pCosaContext->hParentTable; 
    PCOSA_DML_BRG_VLAN_FULL         pVLAN            = (PCOSA_DML_BRG_VLAN_FULL  )pCosaContext->hContext;
    PCOSA_DML_BRG_VLAN_FULL         pVLAN2           = (PCOSA_DML_BRG_VLAN_FULL  )NULL;
    PSLIST_HEADER                   pListHead        = (PSLIST_HEADER            )&pDmlBridge->PortList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2    = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry      = (PSINGLE_LINK_ENTRY       )NULL;

    // check against invalid VLANID
    if (pVLAN->Cfg.bEnabled && pVLAN->Cfg.VLANID <= 0)
    {
        AnscCopyString(pReturnParamName, "VLANID");

        *puLength = AnscSizeOfString("VLANID");

        return FALSE;
    }

    pSLinkEntry = AnscSListGetFirstEntry(pListHead);

    while ( pSLinkEntry )
    {
        pCosaContext2 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        pSLinkEntry   = AnscSListGetNextEntry(pSLinkEntry);

        pVLAN2 = (PCOSA_DML_BRG_VLAN_FULL)pCosaContext2->hContext;

        // check against duplicate of Alias
        if ( pVLAN2 && ((ULONG)pVLAN2 != (ULONG)pVLAN) && AnscEqualString(pVLAN->Cfg.Alias, pVLAN2->Cfg.Alias, TRUE))
        {
            AnscCopyString(pReturnParamName, "Alias");

            *puLength = AnscSizeOfString("Alias");

            return FALSE;
        }

        // check against duplicate of VLANID
        if ( pVLAN2 && ((ULONG)pVLAN2 != (ULONG)pVLAN) &&
             pVLAN->Cfg.VLANID > 0 && pVLAN2->Cfg.VLANID > 0 && pVLAN->Cfg.VLANID == pVLAN2->Cfg.VLANID)
        {
            AnscCopyString(pReturnParamName, "VLANID");

            *puLength = AnscSizeOfString("VLANID");

            return FALSE;
        }
    }

    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        VLAN_Commit
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
VLAN_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_BRIDGING        pCosaDMBridging  = (PCOSA_DATAMODEL_BRIDGING )g_pCosaBEManager->hBridging;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_VLAN_FULL         pVLAN            = (PCOSA_DML_BRG_VLAN_FULL  )pCosaContext->hContext;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge       = (PCOSA_DML_BRG_FULL_ALL   )pCosaContext->hParentTable;

    //$HL 07/2/2013
/*    if (pVLAN->Cfg1.bEnabled && pVLAN->Cfg1.VLANID > 0)
    {
        if (!pVLAN->Cfg.bEnabled || pVLAN->Cfg.VLANID != pVLAN->Cfg1.VLANID)
        {
            // delete old vlan if just disabled or vlan ID changed
            CosaDmlBrgVlanDelEntry(NULL, pDmlBridge->Cfg.InstanceNumber, pVLAN->Cfg1.VLANID);
        }
    }

    if (pVLAN->Cfg.bEnabled && pVLAN->Cfg.VLANID > 0)
    {
        if (!pVLAN->Cfg1.bEnabled || pVLAN->Cfg.VLANID != pVLAN->Cfg1.VLANID)
        {
            PSLIST_HEADER           pListHead        = (PSLIST_HEADER            )&pDmlBridge->VLANPortList;
            PSINGLE_LINK_ENTRY      pSLinkEntry      = (PSINGLE_LINK_ENTRY       )NULL;
            PCOSA_CONTEXT_LINK_OBJECT pCosaContext2  = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
            PCOSA_DML_BRG_VLANPORT_FULL pVLANPort    = (PCOSA_DML_BRG_VLANPORT_FULL)NULL;
	    printf("Add before vlan commit\n");
            // add new vlan if just enabled or vlanID changed
            CosaDmlBrgVlanAddEntry(NULL, pDmlBridge->Cfg.InstanceNumber, pVLAN->Cfg.VLANID);

	    printf("Add After vlan commit\n");
            pSLinkEntry = AnscSListGetFirstEntry(pListHead);

            while ( pSLinkEntry )
            {
                pCosaContext2 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
                pSLinkEntry   = AnscSListGetNextEntry(pSLinkEntry);

                pVLANPort = (PCOSA_DML_BRG_VLANPORT_FULL)pCosaContext2->hContext;

                if (pVLANPort && pVLANPort->Cfg.bEnabled
                              && pVLANPort->Cfg.VLANInsNum == pVLAN->Cfg.InstanceNumber
                              && pVLANPort->Cfg.PortInsNum)
                {
                    // activate all associated vlan port entry
                //    CosaDmlBrgVlanPortAddEntry(NULL, pDmlBridge->Cfg.InstanceNumber, pVLAN->Cfg.VLANID,
                  //                             pVLANPort->Cfg.PortInsNum, pVLANPort->Cfg.bUntagged);
                }
            }
        }
    }
*/
    if ( pCosaContext->bNew )
    {
        pCosaContext->bNew = FALSE;

        CosaBridgingRegDelInfo((ANSC_HANDLE)pCosaDMBridging, (ANSC_HANDLE)pCosaContext);
    }
    else
    {
        CosaDmlBrgVlanSetCfg(NULL, pDmlBridge->Cfg.InstanceNumber, &pVLAN->Cfg);
    }
    // save configuration to backup
    //pVLAN->Cfg1.bEnabled = pVLAN->Cfg.bEnabled;
    //pVLAN->Cfg1.VLANID   = pVLAN->Cfg.VLANID;
    //AnscCopyString(pVLAN->Cfg1.Alias, pVLAN->Cfg.Alias);

    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        VLAN_Rollback
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
VLAN_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_VLAN_FULL         pVLAN            = (PCOSA_DML_BRG_VLAN_FULL  )pCosaContext->hContext;

    // restore configuration from backup
    pVLAN->Cfg.bEnabled = pVLAN->Cfg1.bEnabled;
    pVLAN->Cfg.VLANID   = pVLAN->Cfg1.VLANID;
    AnscCopyString(pVLAN->Cfg.Alias, pVLAN->Cfg1.Alias);

    return 0;
}

/***********************************************************************

 APIs for Object:

    Bridging.Bridge.{i}.VLANPort.{i}.

    *  VLANPort_GetEntryCount
    *  VLANPort_GetEntry
    *  VLANPort_AddEntry
    *  VLANPort_DelEntry
    *  VLANPort_GetParamBoolValue
    *  VLANPort_GetParamIntValue
    *  VLANPort_GetParamUlongValue
    *  VLANPort_GetParamStringValue
    *  VLANPort_SetParamBoolValue
    *  VLANPort_SetParamIntValue
    *  VLANPort_SetParamUlongValue
    *  VLANPort_SetParamStringValue
    *  VLANPort_Validate
    *  VLANPort_Commit
    *  VLANPort_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        VLANPort_GetEntryCount
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
VLANPort_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge   = (PCOSA_DML_BRG_FULL_ALL   )pCosaContext->hContext;

    return AnscSListQueryDepth(&pDmlBridge->VLANPortList);
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        VLANPort_GetEntry
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
VLANPort_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge    = (PCOSA_DML_BRG_FULL_ALL   )pCosaContext->hContext;
    PSLIST_HEADER                   pListHead     = (PSLIST_HEADER            )&pDmlBridge->VLANPortList;
    PSINGLE_LINK_ENTRY              pSLinkEntry   = (PSINGLE_LINK_ENTRY       )NULL;

    pSLinkEntry = AnscSListGetEntryByIndex(pListHead, nIndex);

    if ( pSLinkEntry )
    {
        pCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);

        *pInsNumber = pCosaContext->InstanceNumber;
    }

    return (ANSC_HANDLE)pCosaContext;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        VLANPort_AddEntry
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
VLANPort_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_BRIDGING        pCosaDMBridging = (PCOSA_DATAMODEL_BRIDGING )g_pCosaBEManager->hBridging;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge      = (PCOSA_DML_BRG_FULL_ALL   )pCosaContext->hContext;
    PSLIST_HEADER                   pListHead       = (PSLIST_HEADER            )&pDmlBridge->VLANPortList;
    PCOSA_DML_BRG_VLANPORT_FULL     pVLANPort       = (PCOSA_DML_BRG_VLANPORT_FULL)NULL;

    pVLANPort = (PCOSA_DML_BRG_VLANPORT_FULL)AnscAllocateMemory(sizeof(COSA_DML_BRG_VLANPORT_FULL));

    if ( !pVLANPort )
    {
        return NULL;
    }

    _ansc_sprintf(pVLANPort->Cfg.Alias, "cpe-VLANPort%d", pDmlBridge->ulNextVLANPortInsNum);

    /* Update the middle layer cache */
    if ( TRUE )
    {
        pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

        if ( !pCosaContext )
        {
            AnscFreeMemory(pVLANPort);
            
            return NULL;
        }

        pCosaContext->InstanceNumber = pVLANPort->Cfg.InstanceNumber = pDmlBridge->ulNextVLANPortInsNum;

        pDmlBridge->ulNextVLANPortInsNum++;

        if ( pDmlBridge->ulNextVLANPortInsNum == 0 )
        {
            pDmlBridge->ulNextVLANPortInsNum = 1;
        }

        pCosaContext->hContext         = (ANSC_HANDLE)pVLANPort;
        pCosaContext->hParentTable     = (ANSC_HANDLE)pDmlBridge;
        pCosaContext->hPoamIrepUpperFo = pCosaDMBridging->hIrepFolderBRGHA;
        pCosaContext->bNew             = TRUE;

        CosaSListPushEntryByInsNum(pListHead, (ANSC_HANDLE)pCosaContext);

        CosaBridgingRegAddInfo
            (
                (ANSC_HANDLE)pCosaDMBridging,
                COSA_DML_RR_NAME_Bridge_VLANPort_NextInsNumber,
                pDmlBridge->ulNextVLANPortInsNum,
                COSA_DML_RR_NAME_Bridge_VLANPort_Prefix,
                pDmlBridge->Cfg.InstanceNumber,
                pVLANPort->Cfg.Alias,
                (ANSC_HANDLE)pCosaContext
            );
    }

    *pInsNumber = pCosaContext->InstanceNumber;

    return (ANSC_HANDLE)pCosaContext; /* return the handle */
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        VLANPort_DelEntry
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
VLANPort_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge      = (PCOSA_DML_BRG_FULL_ALL   )pCosaContext->hContext;
    PSLIST_HEADER                   pListHead       = (PSLIST_HEADER            )&pDmlBridge->VLANPortList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2   = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_DML_BRG_VLANPORT_FULL     pVLANPort       = (PCOSA_DML_BRG_VLANPORT_FULL)pCosaContext2->hContext;

    // TODO CosaDmlBrgPortDelEntry(NULL, pCosaContext->InstanceNumber, pVLANPort->Cfg.InstanceNumber);
    if (pVLANPort->Cfg.bEnabled && pVLANPort->Cfg.VLANInsNum > 0 && pVLANPort->Cfg.PortInsNum > 0)
    {
        PCOSA_CONTEXT_LINK_OBJECT   pCosaContext3   = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
        PCOSA_DML_BRG_VLAN_FULL     pVLAN           = (PCOSA_DML_BRG_VLAN_FULL  )NULL;

        pCosaContext3 = Find_SListEntry_By_InstanceNumber(&pDmlBridge->VLANList, pVLANPort->Cfg.VLANInsNum);
        if (pCosaContext3) pVLAN = (PCOSA_DML_BRG_VLAN_FULL)pCosaContext3->hContext;

        if (pVLAN && pVLAN->Cfg.bEnabled && pVLAN->Cfg.VLANID > 0)
        {
            CosaDmlBrgVlanPortDelEntry(NULL, pDmlBridge->Cfg.InstanceNumber, pVLAN->Cfg.VLANID,
                                       pVLANPort->Cfg.PortInsNum);
        }
    }

    /* Update the middle layer cache */

    AnscSListPopEntryByLink(pListHead, &pCosaContext2->Linkage);

    AnscFreeMemory(pVLANPort);
    AnscFreeMemory(pCosaContext2);

    return 0; /* succeeded */
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        VLANPort_GetParamBoolValue
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
VLANPort_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_VLANPORT_FULL     pVLANPort        = (PCOSA_DML_BRG_VLANPORT_FULL)pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        *pBool = pVLANPort->Cfg.bEnabled;
        return TRUE;
    }

    if (strcmp(ParamName, "Untagged") == 0)
    {
        /* collect value */
        *pBool = pVLANPort->Cfg.bUntagged;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        VLANPort_GetParamIntValue
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
VLANPort_GetParamIntValue
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
        VLANPort_GetParamUlongValue
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
VLANPort_GetParamUlongValue
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
        VLANPort_GetParamStringValue
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
VLANPort_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge      = (PCOSA_DML_BRG_FULL_ALL   )pCosaContext->hParentTable;
    PCOSA_DML_BRG_VLANPORT_FULL     pVLANPort       = (PCOSA_DML_BRG_VLANPORT_FULL)pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* collect value */
        AnscCopyString(pValue, pVLANPort->Cfg.Alias);
        return 0;
    }

    if (strcmp(ParamName, "VLAN") == 0)
    {
        /* collect value */
        if (pVLANPort->Cfg.VLANInsNum > 0)
        {
            _ansc_sprintf(pValue, "Device.Bridging.Bridge.%u.VLAN.%u.",
                          pDmlBridge->Cfg.InstanceNumber, pVLANPort->Cfg.VLANInsNum);
        }
        else
        {
            pValue[0] = '\0';
        }
        return 0;
    }

    if (strcmp(ParamName, "Port") == 0)
    {
        /* collect value */
        if (pVLANPort->Cfg.PortInsNum > 0)
        {
            _ansc_sprintf(pValue, "Device.Bridging.Bridge.%u.Port.%u.",
                        pDmlBridge->Cfg.InstanceNumber, pVLANPort->Cfg.PortInsNum);
        }
        else
        {
            pValue[0] = '\0';
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
        VLANPort_SetParamBoolValue
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
VLANPort_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_VLANPORT_FULL     pVLANPort        = (PCOSA_DML_BRG_VLANPORT_FULL)pCosaContext->hContext;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* save update to backup */
        pVLANPort->Cfg.bEnabled = bValue;
        return TRUE;
    }

    if (strcmp(ParamName, "Untagged") == 0)
    {
        /* save update to backup */
        pVLANPort->Cfg.bUntagged = bValue;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        VLANPort_SetParamIntValue
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
VLANPort_SetParamIntValue
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
        VLANPort_SetParamUlongValue
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
VLANPort_SetParamUlongValue
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
        VLANPort_SetParamStringValue
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
VLANPort_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge      = (PCOSA_DML_BRG_FULL_ALL   )pCosaContext->hParentTable;
    PCOSA_DML_BRG_VLANPORT_FULL     pVLANPort       = (PCOSA_DML_BRG_VLANPORT_FULL)pCosaContext->hContext;

    AnscTraceFlow(("%s: %s='%s'\n", __func__, ParamName, pString));

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* save update to backup */
        AnscCopyString(pVLANPort->Cfg.Alias, pString);

        return TRUE;
    }

    if (strcmp(ParamName, "VLAN") == 0)
    {
        /* save update to backup */
        ULONG brInsNum=0, vlanInsNum=0;
        if (pString[0] == '\0')
        {
            pVLANPort->Cfg.VLANInsNum = 0;
        return TRUE;
        }
        else if (_ansc_sscanf(pString, "Device.Bridging.Bridge.%u.VLAN.%u.", &brInsNum, &vlanInsNum)==2 &&
                 brInsNum > 0 && vlanInsNum > 0)
        {
            pVLANPort->Cfg.VLANInsNum = vlanInsNum;
            AnscTraceFlow(("%s: %s='%s' success %u %u\n", __func__, ParamName, pString, brInsNum, vlanInsNum));
            return TRUE;
        }
        else // invalid format
        {
            AnscTraceFlow(("%s: %s='%s' error %u %u\n", __func__, ParamName, pString, brInsNum, vlanInsNum));
            return FALSE;
        }
    }

    if (strcmp(ParamName, "Port") == 0)
    {
        /* save update to backup */
        ULONG brInsNum=0, portInsNum=0;
        if (pString[0] == '\0')
        {
            pVLANPort->Cfg.PortInsNum = 0;
        return TRUE;
        }
        else if (_ansc_sscanf(pString, "Device.Bridging.Bridge.%u.Port.%u.", &brInsNum, &portInsNum)==2 &&
                 brInsNum > 0 && portInsNum > 0)
        {
            pVLANPort->Cfg.PortInsNum = portInsNum;
            AnscTraceFlow(("%s: %s='%s' success %u %u\n", __func__, ParamName, pString, brInsNum, portInsNum));
            return TRUE;
        }
        else // invalid format
        {
            AnscTraceFlow(("%s: %s='%s' error %u %u\n", __func__, ParamName, pString, brInsNum, portInsNum));
            return FALSE;
        }
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        VLANPort_Validate
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
VLANPort_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge       = (PCOSA_DML_BRG_FULL_ALL   )pCosaContext->hParentTable; 
    PCOSA_DML_BRG_VLANPORT_FULL     pVLANPort        = (PCOSA_DML_BRG_VLANPORT_FULL)pCosaContext->hContext;
    PCOSA_DML_BRG_VLANPORT_FULL     pVLANPort2       = (PCOSA_DML_BRG_VLANPORT_FULL)NULL;
    PSLIST_HEADER                   pListHead        = (PSLIST_HEADER            )&pDmlBridge->VLANPortList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2    = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry      = (PSINGLE_LINK_ENTRY       )NULL;
    PCOSA_DML_BRG_VLAN_FULL         pVLAN            = (PCOSA_DML_BRG_VLAN_FULL  )NULL;

    AnscTraceFlow(("%s\n", __func__));

    // check against non-existing VLAN reference
    if (pVLANPort->Cfg.VLANInsNum > 0 &&
        !Find_SListEntry_By_InstanceNumber(&pDmlBridge->VLANList, pVLANPort->Cfg.VLANInsNum))
    {
        AnscCopyString(pReturnParamName, "VLAN");

        *puLength = AnscSizeOfString("VLAN");

        return FALSE;
    }

    // check against non-existing Port reference
    if (pVLANPort->Cfg.PortInsNum > 0 &&
        !Find_SListEntry_By_InstanceNumber(&pDmlBridge->PortList, pVLANPort->Cfg.PortInsNum))
    {
        AnscCopyString(pReturnParamName, "Port");

        *puLength = AnscSizeOfString("Port");

        return FALSE;
    }

    pSLinkEntry = AnscSListGetFirstEntry(pListHead);

    while ( pSLinkEntry )
    {
        pCosaContext2 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        pSLinkEntry   = AnscSListGetNextEntry(pSLinkEntry);

        pVLANPort2 = (PCOSA_DML_BRG_VLANPORT_FULL)pCosaContext2->hContext;

        // check against duplicate alias
        if ( pVLANPort2 && ((ULONG)pVLANPort2 != (ULONG)pVLANPort) &&
             AnscEqualString(pVLANPort->Cfg.Alias, pVLANPort2->Cfg.Alias, TRUE))
        {
            AnscCopyString(pReturnParamName, "Alias");

            *puLength = AnscSizeOfString("Alias");

            return FALSE;
        }
    }

    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        VLANPort_Commit
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
VLANPort_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_BRIDGING        pCosaDMBridging  = (PCOSA_DATAMODEL_BRIDGING )g_pCosaBEManager->hBridging;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2    = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_BRG_VLANPORT_FULL     pVLANPort        = (PCOSA_DML_BRG_VLANPORT_FULL)pCosaContext->hContext;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge       = (PCOSA_DML_BRG_FULL_ALL   )pCosaContext->hParentTable;
    PCOSA_DML_BRG_VLAN_FULL         pVLAN            = (PCOSA_DML_BRG_VLAN_FULL  )NULL;

    if (pVLANPort->Cfg1.bEnabled && pVLANPort->Cfg1.VLANInsNum >0 && pVLANPort->Cfg1.PortInsNum > 0)
    {
        if (!pVLANPort->Cfg.bEnabled || pVLANPort->Cfg.VLANInsNum != pVLANPort->Cfg1.VLANInsNum
                                     || pVLANPort->Cfg.PortInsNum != pVLANPort->Cfg1.PortInsNum
                                     || pVLANPort->Cfg.bUntagged  != pVLANPort->Cfg1.bUntagged)
        {

            pCosaContext2 = Find_SListEntry_By_InstanceNumber(&pDmlBridge->VLANList, pVLANPort->Cfg1.VLANInsNum);
            pVLAN = pCosaContext2 ? (PCOSA_DML_BRG_VLAN_FULL)pCosaContext2->hContext : (PCOSA_DML_BRG_VLAN_FULL)NULL;

            // remove old entry if just disabled or configuration changed
            if (pVLAN && pVLAN->Cfg.bEnabled && pVLAN->Cfg.VLANID)
                CosaDmlBrgVlanPortDelEntry(NULL, pDmlBridge->Cfg.InstanceNumber, pVLAN->Cfg.VLANID,
                                           pVLANPort->Cfg1.PortInsNum);
        }
    }

    if (pVLANPort->Cfg.bEnabled && pVLANPort->Cfg.VLANInsNum > 0 && pVLANPort->Cfg.PortInsNum > 0)
    {
        if (!pVLANPort->Cfg1.bEnabled || pVLANPort->Cfg.VLANInsNum != pVLANPort->Cfg1.VLANInsNum
                                      || pVLANPort->Cfg.PortInsNum != pVLANPort->Cfg1.PortInsNum
                                      || pVLANPort->Cfg.bUntagged  != pVLANPort->Cfg1.bUntagged)
        {
            pCosaContext2 = Find_SListEntry_By_InstanceNumber(&pDmlBridge->VLANList, pVLANPort->Cfg.VLANInsNum);
            pVLAN = pCosaContext2 ? (PCOSA_DML_BRG_VLAN_FULL)pCosaContext2->hContext : (PCOSA_DML_BRG_VLAN_FULL)NULL;

            // add new entry if just enabled or configuration changed
            if (pVLAN && pVLAN->Cfg.bEnabled && pVLAN->Cfg.VLANID > 0)
            {
                CosaDmlBrgVlanPortAddEntry(NULL, pDmlBridge->Cfg.InstanceNumber, pVLAN->Cfg.VLANID,
                                           pVLANPort->Cfg.PortInsNum, pVLANPort->Cfg.bUntagged);
           }
        }
    }

    if ( pCosaContext->bNew )
    {
        pCosaContext->bNew = FALSE;

        CosaBridgingRegDelInfo((ANSC_HANDLE)pCosaDMBridging, (ANSC_HANDLE)pCosaContext);
    }

    // backup configuration
    pVLANPort->Cfg1.bEnabled   = pVLANPort->Cfg.bEnabled;
    pVLANPort->Cfg1.bUntagged  = pVLANPort->Cfg.bUntagged;
    pVLANPort->Cfg1.VLANInsNum = pVLANPort->Cfg.VLANInsNum;
    pVLANPort->Cfg1.PortInsNum = pVLANPort->Cfg.PortInsNum;
    AnscCopyString(pVLANPort->Cfg1.Alias, pVLANPort->Cfg.Alias);

    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        VLANPort_Rollback
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
VLANPort_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_BRG_VLANPORT_FULL     pVLANPort        = (PCOSA_DML_BRG_VLANPORT_FULL)pCosaContext->hContext;

    // restore configuration
    pVLANPort->Cfg.bEnabled   = pVLANPort->Cfg1.bEnabled;
    pVLANPort->Cfg.bUntagged  = pVLANPort->Cfg1.bUntagged;
    pVLANPort->Cfg.VLANInsNum = pVLANPort->Cfg1.VLANInsNum;
    pVLANPort->Cfg.PortInsNum = pVLANPort->Cfg1.PortInsNum;
    AnscCopyString(pVLANPort->Cfg.Alias, pVLANPort->Cfg1.Alias);

    return 0;
}

/***********************************************************************

 APIs for Object:

    Bridging.Filter.{i}.

    *  Filter_GetEntryCount
    *  Filter_GetEntry
    *  Filter_AddEntry
    *  Filter_DelEntry
    *  Filter_GetParamBoolValue
    *  Filter_GetParamIntValue
    *  Filter_GetParamUlongValue
    *  Filter_GetParamStringValue
    *  Filter_SetParamBoolValue
    *  Filter_SetParamIntValue
    *  Filter_SetParamUlongValue
    *  Filter_SetParamStringValue
    *  Filter_Validate
    *  Filter_Commit
    *  Filter_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Filter_GetEntryCount
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
Filter_GetEntryCount
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
        Filter_GetEntry
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
Filter_GetEntry
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

        ANSC_HANDLE
        Filter_AddEntry
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
Filter_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    return NULL; /* return the handle */
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Filter_DelEntry
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
Filter_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    return 0; /* succeeded */
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Filter_GetParamBoolValue
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
Filter_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        return TRUE;
    }

    if (strcmp(ParamName, "EthertypeFilterExclude") == 0)
    {
        /* collect value */
        return TRUE;
    }

    if (strcmp(ParamName, "SourceMACAddressFilterExclude") == 0)
    {
        /* collect value */
        return TRUE;
    }

    if (strcmp(ParamName, "DestMACAddressFilterExclude") == 0)
    {
        /* collect value */
        return TRUE;
    }

    if (strcmp(ParamName, "SourceMACFromVendorClassIDFilterExclude") == 0)
    {
        /* collect value */
        return TRUE;
    }

    if (strcmp(ParamName, "DestMACFromVendorClassIDFilterExclude") == 0)
    {
        /* collect value */
        return TRUE;
    }

    if (strcmp(ParamName, "SourceMACFromClientIDFilterExclude") == 0)
    {
        /* collect value */
        return TRUE;
    }

    if (strcmp(ParamName, "DestMACFromClientIDFilterExclude") == 0)
    {
        /* collect value */
        return TRUE;
    }

    if (strcmp(ParamName, "SourceMACFromUserClassIDFilterExclude") == 0)
    {
        /* collect value */
        return TRUE;
    }

    if (strcmp(ParamName, "DestMACFromUserClassIDFilterExclude") == 0)
    {
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
        Filter_GetParamIntValue
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
Filter_GetParamIntValue
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
        Filter_GetParamUlongValue
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
Filter_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Status") == 0)
    {
        /* collect value */
        return TRUE;
    }

    if (strcmp(ParamName, "Order") == 0)
    {
        /* collect value */
        return TRUE;
    }

    if (strcmp(ParamName, "VLANIDFilter") == 0)
    {
        /* collect value */
        return TRUE;
    }

    if (strcmp(ParamName, "EthertypeFilterList") == 0)
    {
        /* collect value */
        return TRUE;
    }

    if (strcmp(ParamName, "SourceMACFromVendorClassIDMode") == 0)
    {
        /* collect value */
        return TRUE;
    }

    if (strcmp(ParamName, "DestMACFromVendorClassIDMode") == 0)
    {
        /* collect value */
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Filter_GetParamStringValue
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
Filter_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* collect value */
        return 0;
    }

    if (strcmp(ParamName, "Bridge") == 0)
    {
        /* collect value */
        return 0;
    }

    if (strcmp(ParamName, "Interface") == 0)
    {
        /* collect value */
        return 0;
    }

    if (strcmp(ParamName, "SourceMACAddressFilterList") == 0)
    {
        /* collect value */
        return 0;
    }

    if (strcmp(ParamName, "DestMACAddressFilterList") == 0)
    {
        /* collect value */
        return 0;
    }

    if (strcmp(ParamName, "SourceMACFromVendorClassIDFilter") == 0)
    {
        /* collect value */
        return 0;
    }

    if (strcmp(ParamName, "DestMACFromVendorClassIDFilter") == 0)
    {
        /* collect value */
        return 0;
    }

    if (strcmp(ParamName, "SourceMACFromClientIDFilter") == 0)
    {
        /* collect value */
        return 0;
    }

    if (strcmp(ParamName, "DestMACFromClientIDFilter") == 0)
    {
        /* collect value */
        return 0;
    }

    if (strcmp(ParamName, "SourceMACFromUserClassIDFilter") == 0)
    {
        /* collect value */
        return 0;
    }

    if (strcmp(ParamName, "DestMACFromUserClassIDFilter") == 0)
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
        Filter_SetParamBoolValue
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
Filter_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* save update to backup */
        return TRUE;
    }

    if (strcmp(ParamName, "EthertypeFilterExclude") == 0)
    {
        /* save update to backup */
        return TRUE;
    }

    if (strcmp(ParamName, "SourceMACAddressFilterExclude") == 0)
    {
        /* save update to backup */
        return TRUE;
    }

    if (strcmp(ParamName, "DestMACAddressFilterExclude") == 0)
    {
        /* save update to backup */
        return TRUE;
    }

    if (strcmp(ParamName, "SourceMACFromVendorClassIDFilterExclude") == 0)
    {
        /* save update to backup */
        return TRUE;
    }

    if (strcmp(ParamName, "DestMACFromVendorClassIDFilterExclude") == 0)
    {
        /* save update to backup */
        return TRUE;
    }

    if (strcmp(ParamName, "SourceMACFromClientIDFilterExclude") == 0)
    {
        /* save update to backup */
        return TRUE;
    }

    if (strcmp(ParamName, "DestMACFromClientIDFilterExclude") == 0)
    {
        /* save update to backup */
        return TRUE;
    }

    if (strcmp(ParamName, "SourceMACFromUserClassIDFilterExclude") == 0)
    {
        /* save update to backup */
        return TRUE;
    }

    if (strcmp(ParamName, "DestMACFromUserClassIDFilterExclude") == 0)
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
        Filter_SetParamIntValue
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
Filter_SetParamIntValue
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
        Filter_SetParamUlongValue
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
Filter_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Order") == 0)
    {
        /* save update to backup */
        return TRUE;
    }

    if (strcmp(ParamName, "VLANIDFilter") == 0)
    {
        /* save update to backup */
        return TRUE;
    }

    if (strcmp(ParamName, "EthertypeFilterList") == 0)
    {
        /* save update to backup */
        return TRUE;
    }

    if (strcmp(ParamName, "SourceMACFromVendorClassIDMode") == 0)
    {
        /* save update to backup */
        return TRUE;
    }

    if (strcmp(ParamName, "DestMACFromVendorClassIDMode") == 0)
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
        Filter_SetParamStringValue
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
Filter_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* save update to backup */
        return TRUE;
    }

    if (strcmp(ParamName, "Bridge") == 0)
    {
        /* save update to backup */
        return TRUE;
    }

    if (strcmp(ParamName, "Interface") == 0)
    {
        /* save update to backup */
        return TRUE;
    }

    if (strcmp(ParamName, "SourceMACAddressFilterList") == 0)
    {
        /* save update to backup */
        return TRUE;
    }

    if (strcmp(ParamName, "DestMACAddressFilterList") == 0)
    {
        /* save update to backup */
        return TRUE;
    }

    if (strcmp(ParamName, "SourceMACFromVendorClassIDFilter") == 0)
    {
        /* save update to backup */
        return TRUE;
    }

    if (strcmp(ParamName, "DestMACFromVendorClassIDFilter") == 0)
    {
        /* save update to backup */
        return TRUE;
    }

    if (strcmp(ParamName, "SourceMACFromClientIDFilter") == 0)
    {
        /* save update to backup */
        return TRUE;
    }

    if (strcmp(ParamName, "DestMACFromClientIDFilter") == 0)
    {
        /* save update to backup */
        return TRUE;
    }

    if (strcmp(ParamName, "SourceMACFromUserClassIDFilter") == 0)
    {
        /* save update to backup */
        return TRUE;
    }

    if (strcmp(ParamName, "DestMACFromUserClassIDFilter") == 0)
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
        Filter_Validate
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
Filter_Validate
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
        Filter_Commit
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
Filter_Commit
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
        Filter_Rollback
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
Filter_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    return 0;
}

