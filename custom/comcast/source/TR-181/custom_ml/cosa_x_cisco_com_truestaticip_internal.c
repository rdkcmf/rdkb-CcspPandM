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

    module: cosa_x_cisco_com_truestaticip_internal.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaTSIPCreate
        *  CosaTSIPInitialize
        *  CosaTSIPRemove
        *  
    -------------------------------------------------------------------

    environment:

        platform independent

    -------------------------------------------------------------------

    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**************************************************************************/

#include "dml_tr181_custom_cfg.h"

#ifdef   CONFIG_CISCO_TRUE_STATIC_IP

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "cosa_x_cisco_com_truestaticip_internal.h"
#include "cosa_x_cisco_com_truestaticip_apis.h"
#include "cosa_x_cisco_com_filetransfer_apis.h"
#include "cosa_x_cisco_com_filetransfer_internal.h"
#include "ansc_ato_interface.h"
#include "ansc_ato_external_api.h"
#include "ansc_platform.h"
#include "safec_lib_common.h"
#include <syscfg/syscfg.h>

extern void* g_pDslhDmlAgent;

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        CosaTSIPCreate
            (
            );

    description:

        This function constructs cosa nat object and return handle.

    argument:  

    return:     newly created nat object.

**********************************************************************/

ANSC_HANDLE
CosaTSIPCreate
    (
        VOID
    )
{
    PCOSA_DATAMODEL_TSIP    pMyObject    = (PCOSA_DATAMODEL_TSIP)NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PCOSA_DATAMODEL_TSIP)AnscAllocateMemory(sizeof(COSA_DATAMODEL_TSIP));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->Oid               = COSA_DATAMODEL_TSIP_OID;
    pMyObject->Create            = CosaTSIPCreate;
    pMyObject->Remove            = CosaTSIPRemove;
    pMyObject->Initialize        = CosaTSIPInitialize;

    pMyObject->Initialize   ((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaTSIPInitialize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate  cosa nat object and return handle.

    argument: ANSC_HANDLE                 hThisObject
            This handle is actually the pointer of this object
            itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaTSIPInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus           = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_TSIP            pMyObject              = (PCOSA_DATAMODEL_TSIP)hThisObject;
    PCOSA_DATAMODEL_FILETRANSFER    pFtObject              = (PCOSA_DATAMODEL_FILETRANSFER)g_pCosaBEManager->hFileTransfer;
    PCOSA_DML_FILETRANSFER_CFG      pCfg                   = (PCOSA_DML_FILETRANSFER_CFG  )&pFtObject->Cfg;
    PANSC_ATOM_TABLE_OBJECT         pAtomNamespace         = NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoCOSA        = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PPOAM_IREP_FOLDER_OBJECT        hIrepFolderTSIPRule    = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PSLAP_VARIABLE                  pSlapVariable          = (PSLAP_VARIABLE          )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext           = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_TSIP_RULE_ENTRY       pEntry2                = (PCOSA_DML_TSIP_RULE_ENTRY)NULL;
    ULONG                           ulEntryCount           = 0;
    ULONG                           ulIndex                = 0;
    errno_t                         rc                     = -1;

    /* Initiation all functions */

    CosaDmlTSIPInit(NULL, NULL);

    pMyObject->hFtCfg = (ANSC_HANDLE)pCfg;
    pMyObject->ulSubnetNextInsNum = 1;

    pMyObject->hIrepFolderCOSA = g_GetRegistryRootFolder(g_pDslhDmlAgent);

    /* Load Mapping file */
    pAtomNamespace =
        (PANSC_ATOM_TABLE_OBJECT)AnscCreateAtomTable
            (
                (ANSC_HANDLE)NULL,
                (ANSC_HANDLE)NULL,
                NULL
            );

    if ( pAtomNamespace != NULL )
    {
        pMyObject->hAtomNamespace = pAtomNamespace;
    }

    CosaDmlTSIPLoadMappingFile((ANSC_HANDLE)pMyObject);

#ifdef _COSA_FOR_BCI_
    /* Initialize Device.X_CISCO_COM_TrueStaticIP. */
    CosaDmlTSIPGetCfg(NULL, &pMyObject->TSIPCfg);
#endif

    /* Initiation Device.X_CISCO_COM_TrueStaticIP.Subnet.{i} */
    AnscSListInitializeHeader(&pMyObject->SubnetList);
    CosaDmlAdditionalSubnetLoadPsm((ANSC_HANDLE)pMyObject);


    pPoamIrepFoCOSA = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderCOSA;
    if ( !pPoamIrepFoCOSA )
    {
        return ANSC_STATUS_FAILURE;
    }
#if 0

    PPOAM_IREP_FOLDER_OBJECT        hIrepFolderTSIPSubnet  = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    hIrepFolderTSIPSubnet =
        (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoCOSA->GetFolder
            (
                (ANSC_HANDLE)pPoamIrepFoCOSA,
                COSA_IREP_FOLDER_NAME_TSIP_SUBNET
            );

    if ( !hIrepFolderTSIPSubnet )
    {
        hIrepFolderTSIPSubnet =
            pPoamIrepFoCOSA->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoCOSA,
                    COSA_IREP_FOLDER_NAME_TSIP_SUBNET,
                    0
                );
    }

    if ( !hIrepFolderTSIPSubnet )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pMyObject->hIrepFolderTSIPSubnet = (ANSC_HANDLE)hIrepFolderTSIPSubnet;
    }

    /* Retrieve Next Instance Number */
    
    if ( TRUE )
    {
        pSlapVariable =
            (PSLAP_VARIABLE)hIrepFolderTSIPSubnet->GetRecord
                (
                    (ANSC_HANDLE)hIrepFolderTSIPSubnet,
                    COSA_DML_RR_NAME_TSIP_SUBNETNextInsNunmber,
                    NULL
                );

        if ( pSlapVariable )
        {
            pMyObject->ulSubnetNextInsNum = pSlapVariable->Variant.varUint32;

            SlapFreeVariable(pSlapVariable);
        }
    }
#endif
    /* Initiation Device.X_CISCO_COM_TrueStaticIP.PortManagement.Rule.{i}. */

    AnscSListInitializeHeader(&pMyObject->RuleList);

    pMyObject->ulRuleNextInsNum = 1;

    pPoamIrepFoCOSA = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderCOSA;
    if ( !pPoamIrepFoCOSA )
    {
        return ANSC_STATUS_FAILURE;
    }

    hIrepFolderTSIPRule =
        (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoCOSA->GetFolder
            (
                (ANSC_HANDLE)pPoamIrepFoCOSA,
                COSA_IREP_FOLDER_NAME_TSIP_RULE
            );

    if ( !hIrepFolderTSIPRule )
    {
        hIrepFolderTSIPRule =
            pPoamIrepFoCOSA->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoCOSA,
                    COSA_IREP_FOLDER_NAME_TSIP_RULE,
                    0
                );
    }

    if ( !hIrepFolderTSIPRule )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pMyObject->hIrepFolderTSIPRule = (ANSC_HANDLE)hIrepFolderTSIPRule;
    }

    /* Retrieve Next Instance Number */
    
    if ( TRUE )
    {
        pSlapVariable =
            (PSLAP_VARIABLE)hIrepFolderTSIPRule->GetRecord
                (
                    (ANSC_HANDLE)hIrepFolderTSIPRule,
                    COSA_DML_RR_NAME_TSIP_RULENextInsNunmber,
                    NULL
                );

        if ( pSlapVariable )
        {
            pMyObject->ulRuleNextInsNum = pSlapVariable->Variant.varUint32;

            SlapFreeVariable(pSlapVariable);
        }
    }

    /* Initialize middle layer */

    /* CosaDmlTSIPGetCfg(NULL, &pMyObject->TSIPCfg); */

    CosaDmlTSIPPortManagementGetCfg(NULL, &pMyObject->PortManagementCfg);

#if 0

    PCOSA_DML_TSIP_SUBNET_ENTRY     pEntry                 = (PCOSA_DML_TSIP_SUBNET_ENTRY)NULL;
    ulEntryCount = CosaDmlTSIPSubnetGetNumberOfEntries(NULL);

    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        pEntry = (PCOSA_DML_TSIP_SUBNET_ENTRY)AnscAllocateMemory(sizeof(COSA_DML_TSIP_SUBNET_ENTRY));

        if ( !pEntry )
        {
            return ANSC_STATUS_RESOURCES;
        }
        
        CosaDmlTSIPSubnetGetEntry(NULL, ulIndex, pEntry);

        if ( TRUE )
        {
            pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

            if ( !pCosaContext )
            {
                AnscFreeMemory(pEntry);
                
                return ANSC_STATUS_RESOURCES;
            }

            if ( pEntry->InstanceNumber != 0 )
            {
                pCosaContext->InstanceNumber = pEntry->InstanceNumber;

                if ( pMyObject->ulSubnetNextInsNum <= pEntry->InstanceNumber )
                {
                    pMyObject->ulSubnetNextInsNum = pEntry->InstanceNumber + 1;

                    if ( pMyObject->ulSubnetNextInsNum == 0 )
                    {
                        pMyObject->ulSubnetNextInsNum = 1;
                    }
                }
            }
            else
            {
                pEntry->InstanceNumber = pCosaContext->InstanceNumber = pMyObject->ulSubnetNextInsNum;

                pMyObject->ulSubnetNextInsNum++;

                if ( pMyObject->ulSubnetNextInsNum == 0 )
                {
                    pMyObject->ulSubnetNextInsNum = 1;
                }

                /* Generate Alias */
                rc = sprintf_s(pEntry->Alias, sizeof(pEntry->Alias), "Subnet%d", pCosaContext->InstanceNumber);
                if(rc < EOK)
                {
                    ERR_CHK(rc);
                }

                CosaDmlTSIPSubnetSetValues
                (
                    NULL,
                    ulIndex,
                    pCosaContext->InstanceNumber,
                    pEntry->Alias
                );
            }

            pCosaContext->hContext      = (ANSC_HANDLE)pEntry;
            pCosaContext->hParentTable  = NULL;
            pCosaContext->bNew          = FALSE;
            pCosaContext->hPoamIrepUpperFo = NULL;
            pCosaContext->hPoamIrepFo      = NULL;

            CosaSListPushEntryByInsNum(&pMyObject->SubnetList, pCosaContext);
        }
    }
#endif
    ulEntryCount = CosaDmlTSIPRuleGetNumberOfEntries(NULL);

    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        pEntry2 = (PCOSA_DML_TSIP_RULE_ENTRY)AnscAllocateMemory(sizeof(COSA_DML_TSIP_RULE_ENTRY));

        if ( !pEntry2 )
        {
            return ANSC_STATUS_RESOURCES;
        }
        
        CosaDmlTSIPRuleGetEntry(NULL, ulIndex, pEntry2);

        if ( TRUE )
        {
            pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

            if ( !pCosaContext )
            {
                AnscFreeMemory(pEntry2);
                
                return ANSC_STATUS_RESOURCES;
            }

            if ( pEntry2->InstanceNumber != 0 )
            {
                pCosaContext->InstanceNumber = pEntry2->InstanceNumber;

                if ( pMyObject->ulRuleNextInsNum <= pEntry2->InstanceNumber )
                {
                    pMyObject->ulRuleNextInsNum = pEntry2->InstanceNumber + 1;

                    if ( pMyObject->ulRuleNextInsNum == 0 )
                    {
                        pMyObject->ulRuleNextInsNum = 1;
                    }
                }
            }
            else
            {
                pEntry2->InstanceNumber = pCosaContext->InstanceNumber = pMyObject->ulRuleNextInsNum;

                pMyObject->ulRuleNextInsNum++;

                if ( pMyObject->ulRuleNextInsNum == 0 )
                {
                    pMyObject->ulRuleNextInsNum = 1;
                }

                /* Generate Alias */
                rc = sprintf_s(pEntry2->Alias, sizeof(pEntry2->Alias), "Rule%lu", pCosaContext->InstanceNumber);
                if(rc < EOK)
                {
                    ERR_CHK(rc);
                }

                CosaDmlTSIPRuleSetValues
                (
                    NULL,
                    ulIndex,
                    pCosaContext->InstanceNumber,
                    pEntry2->Alias
                );
            }

            pCosaContext->hContext      = (ANSC_HANDLE)pEntry2;
            pCosaContext->hParentTable  = NULL;
            pCosaContext->bNew          = FALSE;
            pCosaContext->hPoamIrepUpperFo = NULL;
            pCosaContext->hPoamIrepFo      = NULL;

            CosaSListPushEntryByInsNum(&pMyObject->RuleList, pCosaContext);
        }
    }

    /* Load the newly added but not yet commited entries, if exist */
#if 0
    CosaTSIPSubnetRegGetInfo((ANSC_HANDLE)pMyObject);
#endif
    CosaTSIPRuleRegGetInfo((ANSC_HANDLE)pMyObject);


    return returnStatus;
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaRoutingRemove
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate  cosa nat object and return handle.

    argument:   ANSC_HANDLE                 hThisObject
            This handle is actually the pointer of this object
            itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaTSIPRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus            = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_TSIP            pMyObject               = (PCOSA_DATAMODEL_TSIP)hThisObject;
    PSINGLE_LINK_ENTRY              pSLinkEntry             = NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = NULL;

    /* Remove necessary resounce */
    if (AnscSListQueryDepth(&pMyObject->SubnetList) != 0)
    {
        pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->SubnetList);

        while ( pSLinkEntry )
        {
            pCosaContext            = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
            pSLinkEntry             = AnscSListGetNextEntry(pSLinkEntry);

            AnscSListPopEntryByLink(&pMyObject->SubnetList, &pCosaContext->Linkage);           

            AnscFreeMemory(pCosaContext->hContext);
            AnscFreeMemory(pCosaContext);
        }
    }

    if (AnscSListQueryDepth(&pMyObject->RuleList) != 0)
    {
        pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->RuleList);

        while ( pSLinkEntry )
        {
            pCosaContext            = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
            pSLinkEntry             = AnscSListGetNextEntry(pSLinkEntry);

            AnscSListPopEntryByLink(&pMyObject->RuleList, &pCosaContext->Linkage);           

            AnscFreeMemory(pCosaContext->hContext);
            AnscFreeMemory(pCosaContext);
        }
    }
    /* Remove self */
    AnscFreeMemory((ANSC_HANDLE)pMyObject);

    return returnStatus;
}

ANSC_STATUS
CosaTSIPSubnetRegGetInfo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PCOSA_DATAMODEL_TSIP            pMyObject               = (PCOSA_DATAMODEL_TSIP     )hThisObject;
    PSLIST_HEADER                   pListHead               = (PSLIST_HEADER            )&pMyObject->SubnetList;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoTSIPSubnet   = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderTSIPSubnet;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoTSIPSubnetSp = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PCOSA_DML_TSIP_SUBNET_ENTRY     pEntry                  = (PCOSA_DML_TSIP_SUBNET_ENTRY)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSLAP_VARIABLE                  pSlapVariable           = (PSLAP_VARIABLE           )NULL;
    ULONG                           ulEntryCount            = 0;
    ULONG                           ulIndex                 = 0;
    ULONG                           ulInstanceNumber        = 0;
    char*                           pFolderName             = NULL;
    char*                           pAlias                  = NULL;
    errno_t                         rc                      = -1;

    if ( !pPoamIrepFoTSIPSubnet )
    {
        return ANSC_STATUS_FAILURE;
    }

    /* Load the newly added but not yet commited entries */
    
    ulEntryCount = pPoamIrepFoTSIPSubnet->GetFolderCount((ANSC_HANDLE)pPoamIrepFoTSIPSubnet);

    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        pFolderName =
            pPoamIrepFoTSIPSubnet->EnumFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoTSIPSubnet,
                    ulIndex
                );

        if ( !pFolderName )
        {
            continue;
        }

        pPoamIrepFoTSIPSubnetSp = pPoamIrepFoTSIPSubnet->GetFolder((ANSC_HANDLE)pPoamIrepFoTSIPSubnet, pFolderName);

        AnscFreeMemory(pFolderName);

        if ( !pPoamIrepFoTSIPSubnetSp )
        {
            continue;
        }

        if ( TRUE )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoTSIPSubnetSp->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoTSIPSubnetSp,
                        COSA_DML_RR_NAME_TSIP_SUBNETInsNum,
                        NULL
                    );

            if ( pSlapVariable )
            {
                ulInstanceNumber = pSlapVariable->Variant.varUint32;

                SlapFreeVariable(pSlapVariable);
            }
        }

        if ( TRUE )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoTSIPSubnetSp->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoTSIPSubnetSp,
                        COSA_DML_RR_NAME_TSIP_SUBNETAlias,
                        NULL
                    );

            if ( pSlapVariable )
            {
                pAlias = AnscCloneString(pSlapVariable->Variant.varString);

                SlapFreeVariable(pSlapVariable);
            }
        }

        pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

        if ( !pCosaContext )
        {
            AnscFreeMemory(pAlias);

            return ANSC_STATUS_RESOURCES;
        }

        pEntry = (PCOSA_DML_TSIP_SUBNET_ENTRY)AnscAllocateMemory(sizeof(COSA_DML_TSIP_SUBNET_ENTRY));

        if ( !pEntry )
        {
            AnscFreeMemory(pAlias);
            AnscFreeMemory(pCosaContext);

            return ANSC_STATUS_RESOURCES;
        }

        rc = strcpy_s(pEntry->Alias, sizeof(pEntry->Alias), pAlias);
        ERR_CHK(rc);

        pEntry->InstanceNumber = ulInstanceNumber;

        pCosaContext->InstanceNumber        = ulInstanceNumber;
        pCosaContext->bNew                  = TRUE;
        pCosaContext->hContext              = (ANSC_HANDLE)pEntry;
        pCosaContext->hParentTable          = NULL;
        pCosaContext->hPoamIrepUpperFo      = (ANSC_HANDLE)pPoamIrepFoTSIPSubnet;
        pCosaContext->hPoamIrepFo           = (ANSC_HANDLE)pPoamIrepFoTSIPSubnetSp;

        CosaSListPushEntryByInsNum(pListHead, pCosaContext);

        if ( pAlias )
        {
            AnscFreeMemory(pAlias);

            pAlias = NULL;
        }
    }
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaTSIPSubnetRegAddInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    )
{
    ANSC_STATUS                     returnStatus            = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_TSIP            pMyObject               = (PCOSA_DATAMODEL_TSIP     )hThisObject;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoTSIPSubnet   = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderTSIPSubnet;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoTSIPSubnetSp = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)hCosaContext;
    PCOSA_DML_TSIP_SUBNET_ENTRY     pEntry                  = (PCOSA_DML_TSIP_SUBNET_ENTRY)pCosaContext->hContext;
    PSLAP_VARIABLE                  pSlapVariable           = (PSLAP_VARIABLE           )NULL;

    if ( !pPoamIrepFoTSIPSubnet )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pPoamIrepFoTSIPSubnet->EnableFileSync((ANSC_HANDLE)pPoamIrepFoTSIPSubnet, FALSE);
    }

    if ( TRUE )
    {
        SlapAllocVariable(pSlapVariable);

        if ( !pSlapVariable )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT1;
        }
    }

    if ( TRUE )
    {
        returnStatus = 
            pPoamIrepFoTSIPSubnet->DelRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoTSIPSubnet,
                    COSA_DML_RR_NAME_TSIP_SUBNETNextInsNunmber
                );

        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
        pSlapVariable->Variant.varUint32 = pMyObject->ulSubnetNextInsNum;

        returnStatus =
            pPoamIrepFoTSIPSubnet->AddRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoTSIPSubnet,
                    COSA_DML_RR_NAME_TSIP_SUBNETNextInsNunmber,
                    SYS_REP_RECORD_TYPE_UINT,
                    SYS_RECORD_CONTENT_DEFAULT,
                    pSlapVariable,
                    0
                );

        SlapCleanVariable(pSlapVariable);
        SlapInitVariable (pSlapVariable);
    }

    if ( TRUE )
    {
        pPoamIrepFoTSIPSubnetSp =
            pPoamIrepFoTSIPSubnet->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoTSIPSubnet,
                    pEntry->Alias,
                    0
                );

        if ( !pPoamIrepFoTSIPSubnetSp )
        {
            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT1;
        }

        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
            pSlapVariable->Variant.varUint32 = pEntry->InstanceNumber;

            returnStatus =
                pPoamIrepFoTSIPSubnetSp->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoTSIPSubnetSp,
                        COSA_DML_RR_NAME_TSIP_SUBNETInsNum,
                        SYS_REP_RECORD_TYPE_UINT,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );

            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }

        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;
            pSlapVariable->Variant.varString = AnscCloneString(pEntry->Alias);

            returnStatus =
                pPoamIrepFoTSIPSubnetSp->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoTSIPSubnetSp,
                        COSA_DML_RR_NAME_TSIP_SUBNETAlias,
                        SYS_REP_RECORD_TYPE_ASTR,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );

            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }

        pCosaContext->hPoamIrepUpperFo = (ANSC_HANDLE)pPoamIrepFoTSIPSubnet;
        pCosaContext->hPoamIrepFo      = (ANSC_HANDLE)pPoamIrepFoTSIPSubnetSp;
    }

EXIT1:
    
    if ( pSlapVariable )
    {
        SlapFreeVariable(pSlapVariable);
    }

    pPoamIrepFoTSIPSubnet->EnableFileSync((ANSC_HANDLE)pPoamIrepFoTSIPSubnet, TRUE);

    return returnStatus;
}

ANSC_STATUS
CosaTSIPSubnetRegDelInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hCosaContext;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepUpperFo  = (PPOAM_IREP_FOLDER_OBJECT )pCosaContext->hPoamIrepUpperFo;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFo       = (PPOAM_IREP_FOLDER_OBJECT )pCosaContext->hPoamIrepFo;

    if ( !pPoamIrepUpperFo || !pPoamIrepFo )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pPoamIrepUpperFo->EnableFileSync((ANSC_HANDLE)pPoamIrepUpperFo, FALSE);
    }

    if ( TRUE )
    {
        pPoamIrepFo->Close((ANSC_HANDLE)pPoamIrepFo);
        
        pPoamIrepUpperFo->DelFolder
            (
                (ANSC_HANDLE)pPoamIrepUpperFo, 
                pPoamIrepFo->GetFolderName((ANSC_HANDLE)pPoamIrepFo)
            );

        pPoamIrepUpperFo->EnableFileSync((ANSC_HANDLE)pPoamIrepUpperFo, TRUE);

        AnscFreeMemory(pPoamIrepFo);
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaTSIPRuleRegGetInfo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PCOSA_DATAMODEL_TSIP            pMyObject               = (PCOSA_DATAMODEL_TSIP     )hThisObject;
    PSLIST_HEADER                   pListHead               = (PSLIST_HEADER            )&pMyObject->RuleList;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoTSIPRule     = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderTSIPRule;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoTSIPRuleSp   = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PCOSA_DML_TSIP_RULE_ENTRY       pEntry                  = (PCOSA_DML_TSIP_RULE_ENTRY)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSLAP_VARIABLE                  pSlapVariable           = (PSLAP_VARIABLE           )NULL;
    ULONG                           ulEntryCount            = 0;
    ULONG                           ulIndex                 = 0;
    ULONG                           ulInstanceNumber        = 0;
    char*                           pFolderName             = NULL;
    char*                           pAlias                  = NULL;
    errno_t                         rc                      = -1;

    if ( !pPoamIrepFoTSIPRule )
    {
        return ANSC_STATUS_FAILURE;
    }

    /* Load the newly added but not yet commited entries */
    
    ulEntryCount = pPoamIrepFoTSIPRule->GetFolderCount((ANSC_HANDLE)pPoamIrepFoTSIPRule);

    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        pFolderName =
            pPoamIrepFoTSIPRule->EnumFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoTSIPRule,
                    ulIndex
                );

        if ( !pFolderName )
        {
            continue;
        }

        pPoamIrepFoTSIPRuleSp = pPoamIrepFoTSIPRule->GetFolder((ANSC_HANDLE)pPoamIrepFoTSIPRule, pFolderName);

        AnscFreeMemory(pFolderName);

        if ( !pPoamIrepFoTSIPRuleSp )
        {
            continue;
        }

        if ( TRUE )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoTSIPRuleSp->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoTSIPRuleSp,
                        COSA_DML_RR_NAME_TSIP_RULEInsNum,
                        NULL
                    );

            if ( pSlapVariable )
            {
                ulInstanceNumber = pSlapVariable->Variant.varUint32;

                SlapFreeVariable(pSlapVariable);
            }
        }

        if ( TRUE )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoTSIPRuleSp->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoTSIPRuleSp,
                        COSA_DML_RR_NAME_TSIP_RULEAlias,
                        NULL
                    );

            if ( pSlapVariable )
            {
                pAlias = AnscCloneString(pSlapVariable->Variant.varString);

                SlapFreeVariable(pSlapVariable);
            }
        }

        pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

        if ( !pCosaContext )
        {
            AnscFreeMemory(pAlias);

            return ANSC_STATUS_RESOURCES;
        }

        pEntry = (PCOSA_DML_TSIP_RULE_ENTRY)AnscAllocateMemory(sizeof(COSA_DML_TSIP_RULE_ENTRY));

        if ( !pEntry )
        {
            AnscFreeMemory(pAlias);
            AnscFreeMemory(pCosaContext);

            return ANSC_STATUS_RESOURCES;
        }

        rc = strcpy_s(pEntry->Alias, sizeof(pEntry->Alias), pAlias);
        ERR_CHK(rc);

        pEntry->InstanceNumber = ulInstanceNumber;

        pCosaContext->InstanceNumber        = ulInstanceNumber;
        pCosaContext->bNew                  = TRUE;
        pCosaContext->hContext              = (ANSC_HANDLE)pEntry;
        pCosaContext->hParentTable          = NULL;
        pCosaContext->hPoamIrepUpperFo      = (ANSC_HANDLE)pPoamIrepFoTSIPRule;
        pCosaContext->hPoamIrepFo           = (ANSC_HANDLE)pPoamIrepFoTSIPRuleSp;

        CosaSListPushEntryByInsNum(pListHead, pCosaContext);

        if ( pAlias )
        {
            AnscFreeMemory(pAlias);

            pAlias = NULL;
        }
    }
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaTSIPRuleRegAddInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    )
{
    ANSC_STATUS                     returnStatus            = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_TSIP            pMyObject               = (PCOSA_DATAMODEL_TSIP     )hThisObject;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoTSIPRule     = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderTSIPRule;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoTSIPRuleSp   = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)hCosaContext;
    PCOSA_DML_TSIP_RULE_ENTRY       pEntry                  = (PCOSA_DML_TSIP_RULE_ENTRY)pCosaContext->hContext;
    PSLAP_VARIABLE                  pSlapVariable           = (PSLAP_VARIABLE           )NULL;

    if ( !pPoamIrepFoTSIPRule )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pPoamIrepFoTSIPRule->EnableFileSync((ANSC_HANDLE)pPoamIrepFoTSIPRule, FALSE);
    }

    if ( TRUE )
    {
        SlapAllocVariable(pSlapVariable);

        if ( !pSlapVariable )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT1;
        }
    }

    if ( TRUE )
    {
        returnStatus = 
            pPoamIrepFoTSIPRule->DelRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoTSIPRule,
                    COSA_DML_RR_NAME_TSIP_RULENextInsNunmber
                );

        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
        pSlapVariable->Variant.varUint32 = pMyObject->ulRuleNextInsNum;

        returnStatus =
            pPoamIrepFoTSIPRule->AddRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoTSIPRule,
                    COSA_DML_RR_NAME_TSIP_RULENextInsNunmber,
                    SYS_REP_RECORD_TYPE_UINT,
                    SYS_RECORD_CONTENT_DEFAULT,
                    pSlapVariable,
                    0
                );

        SlapCleanVariable(pSlapVariable);
        SlapInitVariable (pSlapVariable);
    }

    if ( TRUE )
    {
        pPoamIrepFoTSIPRuleSp =
            pPoamIrepFoTSIPRule->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoTSIPRule,
                    pEntry->Alias,
                    0
                );

        if ( !pPoamIrepFoTSIPRuleSp )
        {
            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT1;
        }

        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
            pSlapVariable->Variant.varUint32 = pEntry->InstanceNumber;

            returnStatus =
                pPoamIrepFoTSIPRuleSp->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoTSIPRuleSp,
                        COSA_DML_RR_NAME_TSIP_RULEInsNum,
                        SYS_REP_RECORD_TYPE_UINT,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );

            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }

        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;
            pSlapVariable->Variant.varString = AnscCloneString(pEntry->Alias);

            returnStatus =
                pPoamIrepFoTSIPRuleSp->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoTSIPRuleSp,
                        COSA_DML_RR_NAME_TSIP_RULEAlias,
                        SYS_REP_RECORD_TYPE_ASTR,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );

            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }

        pCosaContext->hPoamIrepUpperFo = (ANSC_HANDLE)pPoamIrepFoTSIPRule;
        pCosaContext->hPoamIrepFo      = (ANSC_HANDLE)pPoamIrepFoTSIPRuleSp;
    }

EXIT1:
    
    if ( pSlapVariable )
    {
        SlapFreeVariable(pSlapVariable);
    }

    pPoamIrepFoTSIPRule->EnableFileSync((ANSC_HANDLE)pPoamIrepFoTSIPRule, TRUE);

    return returnStatus;
}

ANSC_STATUS
CosaTSIPRuleRegDelInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hCosaContext;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepUpperFo  = (PPOAM_IREP_FOLDER_OBJECT )pCosaContext->hPoamIrepUpperFo;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFo       = (PPOAM_IREP_FOLDER_OBJECT )pCosaContext->hPoamIrepFo;

    if ( !pPoamIrepUpperFo || !pPoamIrepFo )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pPoamIrepUpperFo->EnableFileSync((ANSC_HANDLE)pPoamIrepUpperFo, FALSE);
    }

    if ( TRUE )
    {
        pPoamIrepFo->Close((ANSC_HANDLE)pPoamIrepFo);
        
        pPoamIrepUpperFo->DelFolder
            (
                (ANSC_HANDLE)pPoamIrepUpperFo, 
                pPoamIrepFo->GetFolderName((ANSC_HANDLE)pPoamIrepFo)
            );

        pPoamIrepUpperFo->EnableFileSync((ANSC_HANDLE)pPoamIrepUpperFo, TRUE);

        AnscFreeMemory(pPoamIrepFo);
    }

    return ANSC_STATUS_SUCCESS;
}

#endif
