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

    module: cosa_ipv6rd_internal.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

    -------------------------------------------------------------------

    environment:

        platform independent

    -------------------------------------------------------------------

    author:

        leichen2@cisco.com

    -------------------------------------------------------------------

    revision:

        08/05/2011    initial revision.

**************************************************************************/

#include "cosa_ipv6rd_internal.h"
#include "safec_lib_common.h"

extern void * g_pDslhDmlAgent;

ANSC_HANDLE
CosaIPv6rdCreate(
        VOID
        )
{
    PCOSA_DATAMODEL_IPV6RD  pMyObject = (PCOSA_DATAMODEL_IPV6RD)NULL;

    pMyObject = (PCOSA_DATAMODEL_IPV6RD)AnscAllocateMemory(sizeof(COSA_DATAMODEL_IPV6RD));
    if (!pMyObject)
    {
        return (ANSC_HANDLE)NULL;
    }

    pMyObject->Oid          = COSA_DATAMODEL_IPV6RD_OID;
    pMyObject->Create       = CosaIPv6rdCreate;
    pMyObject->Remove       = CosaIPv6rdRemove;
    pMyObject->Initialize   = CosaIPv6rdInitialize;

    pMyObject->Initialize((ANSC_HANDLE)pMyObject);

    return (ANSC_HANDLE)pMyObject;
}

ANSC_STATUS
CosaIPv6rdInitialize(
        ANSC_HANDLE hThisObject
        )
{
    PCOSA_DATAMODEL_IPV6RD      pMyObject = (PCOSA_DATAMODEL_IPV6RD)hThisObject;
    PPOAM_IREP_FOLDER_OBJECT    pPoamIrepFoCOSA = NULL;
    PPOAM_IREP_FOLDER_OBJECT    pPoamIrepFoIPv6rdIF = NULL;
    PSLAP_VARIABLE              pSlapVariable = NULL;
    ULONG                       ulEntryCount = 0;
    ULONG                       ulIndex = 0;
    PCOSA_DML_IPV6RD_IF         pEntry = NULL;
    PCOSA_CONTEXT_LINK_OBJECT   pLinkObject = NULL;
    ULONG                       InsNumArray[MAX_6RDIF_INS];
    errno_t                     rc = -1;

    if (CosaDml_IPv6rdInit(NULL, NULL) != ANSC_STATUS_SUCCESS)
    {
        CcspTraceError(("CosaIPv6rdInitialize: CosaDml_IPv6rdInit error\n"));
        return ANSC_STATUS_FAILURE;
    }

    /*
     * Device.IPv6rd.Enable
     */
    if (CosaDml_IPv6rdGetEnable(NULL, NULL, &pMyObject->Enable) != ANSC_STATUS_SUCCESS)
    {
        CcspTraceError(("CosaIPv6rdInitialize: CosaDml_IPv6rdInit error\n"));
        return ANSC_STATUS_FAILURE;
    }

    /* Device.IPv6rd.InterfaceSettingNumberOfEntries */
    ulEntryCount = MAX_6RDIF_INS;
    if (CosaDml_IPv6rdGetNumberOfEntries(NULL, &ulEntryCount, InsNumArray) != ANSC_STATUS_SUCCESS)
    {
        CcspTraceError(("CosaIPv6rdInitialize: CosaDml_IPv6rdGetNumberOfEntries error\n"));
        return ANSC_STATUS_FAILURE;
    }
    pMyObject->NumOfInterface = ulEntryCount;

    /*
     * Device.IPv6rd.InterfaceSetting.
     */
    AnscSListInitializeHeader(&pMyObject->IfList);
    pMyObject->ulIfNextInstance = 1;
    pMyObject->hIrepFolderCOSA = g_GetRegistryRootFolder(g_pDslhDmlAgent);

    /* folders */
    pPoamIrepFoCOSA = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderCOSA;
    if (!pPoamIrepFoCOSA)
    {
        return ANSC_STATUS_FAILURE;
    }

    pPoamIrepFoIPv6rdIF = 
        (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoCOSA->GetFolder(
                (ANSC_HANDLE)pPoamIrepFoCOSA,
                COSA_IREP_FOLDER_NAME_IPV6RDIF);
    if (!pPoamIrepFoIPv6rdIF)
    {
        pPoamIrepFoIPv6rdIF = 
            pPoamIrepFoCOSA->AddFolder(
                    (ANSC_HANDLE)pPoamIrepFoCOSA,
                    COSA_IREP_FOLDER_NAME_IPV6RDIF,
                    0);
    }
    if (!pPoamIrepFoIPv6rdIF)
    {
        return ANSC_STATUS_FAILURE;
    }
    else 
    {
        pMyObject->hIrepFolderIPv6rdIF = pPoamIrepFoIPv6rdIF;
    }

    /* get next instance number */
    pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoIPv6rdIF->GetRecord(
            (ANSC_HANDLE)pPoamIrepFoIPv6rdIF,
            COSA_DML_RR_NAME_IPV6RDIFNextInsNumber,
            NULL);
    if (pSlapVariable)
    {
        pMyObject->ulIfNextInstance = pSlapVariable->Variant.varUint32;
        SlapFreeVariable(pSlapVariable);
    }

    /* interfaces */
    for (ulIndex = 0; ulIndex < ulEntryCount; ulIndex++)
    {
        pEntry = AnscAllocateMemory(sizeof(COSA_DML_IPV6RD_IF));
        if (!pEntry)
        {
            return ANSC_STATUS_FAILURE;
        }

        /* get entry info from backend */
        if (CosaDml_IPv6rdGetEntry(NULL, InsNumArray[ulIndex], pEntry) != ANSC_STATUS_SUCCESS)
        {
            AnscFreeMemory(pEntry);/*RDKB-6741, CID-33367, free resources before exit*/
            CcspTraceError(("CosaIPv6rdInitialize: CosaDml_IPv6rdGetEntry error\n"));
            return ANSC_STATUS_FAILURE;
        }

        //if (pMyObject->ulIfNextInstance <= pEntry->InstanceNumber)
        //    pMyObject->ulIfNextInstance = pEntry->InstanceNumber + 1;

        pLinkObject = AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
        if (!pLinkObject)
        {
            AnscFreeMemory(pEntry);
            return ANSC_STATUS_FAILURE;
        }

        if (pEntry->InstanceNumber != 0)
        {
            if (pMyObject->ulIfNextInstance <= pEntry->InstanceNumber)
            {
                pMyObject->ulIfNextInstance = pEntry->InstanceNumber + 1;
                if (pMyObject->ulIfNextInstance == 0)
                {
                    pMyObject->ulIfNextInstance = 1;
                }
            }
        }
        else
        {
            CcspTraceWarning(("%s: Should not go here!!\n", __func__));
            pEntry->InstanceNumber = pMyObject->ulIfNextInstance;

            pMyObject->ulIfNextInstance++;
            if (pMyObject->ulIfNextInstance == 0)
            {
                pMyObject->ulIfNextInstance = 1;
            }

            rc = sprintf_s(pEntry->Alias, sizeof(pEntry->Alias),"tun6rd%lu", pEntry->InstanceNumber);
            if(rc < EOK)
            {
              ERR_CHK(rc);
              AnscFreeMemory(pLinkObject);
              AnscFreeMemory(pEntry);
              return ANSC_STATUS_FAILURE;
            }
            CosaDml_IPv6rdSetEntry(NULL, pEntry->InstanceNumber, pEntry);
        }

        pLinkObject->InstanceNumber = pEntry->InstanceNumber;
        pLinkObject->hContext = (ANSC_HANDLE)pEntry;
        pLinkObject->hParentTable = NULL;
        pLinkObject->bNew = FALSE;

        CosaSListPushEntryByInsNum(&pMyObject->IfList, pLinkObject);
    }

    CosaIPv6rdIfRegGetInfo((ANSC_HANDLE)pMyObject);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaIPv6rdRemove(
        ANSC_HANDLE hThisObject
        )
{
    PCOSA_DATAMODEL_IPV6RD      pMyObject = (PCOSA_DATAMODEL_IPV6RD)hThisObject;
    PSINGLE_LINK_ENTRY          pSLinkEntry = NULL;
    PCOSA_CONTEXT_LINK_OBJECT   pLinkObject = NULL;

    if (AnscSListQueryDepth(&pMyObject->IfList) != 0)
    {
        pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->IfList);
        while (!pSLinkEntry)
        {
            pLinkObject = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
            /* TODO: deref pSLinkEntry, while condition should be != NULL*/
            pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);
            AnscSListPopEntryByLink(&pMyObject->IfList, &pLinkObject->Linkage);

            AnscFreeMemory(pLinkObject->hContext);
            AnscFreeMemory(pLinkObject);
        }
    }

    AnscFreeMemory((ANSC_HANDLE)pMyObject);

    if (CosaDml_IPv6rdFinalize(NULL, NULL) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaIPv6rdIfRegGetInfo(
        ANSC_HANDLE                 hThisObject
    )
{
    PCOSA_DATAMODEL_IPV6RD          pMyObject               = (PCOSA_DATAMODEL_IPV6RD   )hThisObject;
    PSLIST_HEADER                   pListHead               = (PSLIST_HEADER            )&pMyObject->IfList;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoIPv6rdIF     = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderIPv6rdIF;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoIPv6rdIFSp   = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PCOSA_DML_IPV6RD_IF             pEntry                  = (PCOSA_DML_IPV6RD_IF    )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSLAP_VARIABLE                  pSlapVariable           = (PSLAP_VARIABLE           )NULL;
    ULONG                           ulEntryCount            = 0;
    ULONG                           ulIndex                 = 0;
    ULONG                           ulInstanceNumber        = 0;
    char*                           pFolderName             = NULL;
    char*                           pAlias                  = NULL;
    errno_t                         rc                      = -1;

    if ( !pPoamIrepFoIPv6rdIF )
    {
        return ANSC_STATUS_FAILURE;
    }

    /* Load the newly added but not yet commited entries */
    
    ulEntryCount = pPoamIrepFoIPv6rdIF->GetFolderCount((ANSC_HANDLE)pPoamIrepFoIPv6rdIF);

    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        pFolderName =
            pPoamIrepFoIPv6rdIF->EnumFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoIPv6rdIF,
                    ulIndex
                );

        if ( !pFolderName )
        {
            continue;
        }

        pPoamIrepFoIPv6rdIFSp = pPoamIrepFoIPv6rdIF->GetFolder((ANSC_HANDLE)pPoamIrepFoIPv6rdIF, pFolderName);

        AnscFreeMemory(pFolderName);

        if ( !pPoamIrepFoIPv6rdIFSp )
        {
            continue;
        }

            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoIPv6rdIFSp->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoIPv6rdIFSp,
                        COSA_DML_RR_NAME_IPV6RDIFInsNum,
                        NULL
                    );

            if ( pSlapVariable )
            {
                ulInstanceNumber = pSlapVariable->Variant.varUint32;

                SlapFreeVariable(pSlapVariable);
            }

            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoIPv6rdIFSp->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoIPv6rdIFSp,
                        COSA_DML_RR_NAME_IPV6RDIFAlias,
                        NULL
                    );

            if ( pSlapVariable )
            {
                pAlias = AnscCloneString(pSlapVariable->Variant.varString);

                SlapFreeVariable(pSlapVariable);
            }

        pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

        if ( !pCosaContext )
        {
            AnscFreeMemory(pAlias);

            return ANSC_STATUS_RESOURCES;
        }

        pEntry = (PCOSA_DML_IPV6RD_IF)AnscAllocateMemory(sizeof(COSA_DML_IPV6RD_IF));

        if ( !pEntry )
        {
            AnscFreeMemory(pAlias);
            AnscFreeMemory(pCosaContext);

            return ANSC_STATUS_RESOURCES;
        }

        rc = strcpy_s(pEntry->Alias, sizeof(pEntry->Alias), pAlias ? pAlias : "");
        ERR_CHK(rc);

        pEntry->InstanceNumber = ulInstanceNumber;

        pCosaContext->InstanceNumber        = ulInstanceNumber;
        pCosaContext->bNew                  = TRUE;
        pCosaContext->hContext              = (ANSC_HANDLE)pEntry;
        pCosaContext->hParentTable          = NULL;
        pCosaContext->hPoamIrepUpperFo      = (ANSC_HANDLE)pPoamIrepFoIPv6rdIF;
        pCosaContext->hPoamIrepFo           = (ANSC_HANDLE)pPoamIrepFoIPv6rdIFSp;

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
CosaIPv6rdIfRegAddInfo(
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    )
{
    ANSC_STATUS                     returnStatus            = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_IPV6RD          pMyObject               = (PCOSA_DATAMODEL_IPV6RD   )hThisObject;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoIPv6rdIf     = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderIPv6rdIF;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoIPv6rdIfSp    = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)hCosaContext;
    PCOSA_DML_IPV6RD_IF             pEntry                  = (PCOSA_DML_IPV6RD_IF      )pCosaContext->hContext;
    PSLAP_VARIABLE                  pSlapVariable           = (PSLAP_VARIABLE           )NULL;

    if ( !pPoamIrepFoIPv6rdIf )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pPoamIrepFoIPv6rdIf->EnableFileSync((ANSC_HANDLE)pPoamIrepFoIPv6rdIf, FALSE);
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
            pPoamIrepFoIPv6rdIf->DelRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoIPv6rdIf,
                    COSA_DML_RR_NAME_IPV6RDIFNextInsNumber
                );

        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
        pSlapVariable->Variant.varUint32 = pMyObject->ulIfNextInstance;

        returnStatus =
            pPoamIrepFoIPv6rdIf->AddRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoIPv6rdIf,
                    COSA_DML_RR_NAME_IPV6RDIFNextInsNumber,
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
        pPoamIrepFoIPv6rdIfSp =
            pPoamIrepFoIPv6rdIf->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoIPv6rdIf,
                    pEntry->Alias,
                    0
                );
        /*CID: 61342 Logically dead code - NULL check done starting of the func*/

        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
            pSlapVariable->Variant.varUint32 = pEntry->InstanceNumber;

            returnStatus =
                pPoamIrepFoIPv6rdIfSp->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoIPv6rdIfSp,
                        COSA_DML_RR_NAME_IPV6RDIFInsNum,
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
                pPoamIrepFoIPv6rdIfSp->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoIPv6rdIfSp,
                        COSA_DML_RR_NAME_IPV6RDIFAlias,
                        SYS_REP_RECORD_TYPE_ASTR,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );

            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }

        pCosaContext->hPoamIrepUpperFo = (ANSC_HANDLE)pPoamIrepFoIPv6rdIf;
        pCosaContext->hPoamIrepFo      = (ANSC_HANDLE)pPoamIrepFoIPv6rdIfSp;
    }

EXIT1:
    
    if ( pSlapVariable )
    {
        SlapFreeVariable(pSlapVariable);
    }

    pPoamIrepFoIPv6rdIf->EnableFileSync((ANSC_HANDLE)pPoamIrepFoIPv6rdIf, TRUE);

    return returnStatus;
}

ANSC_STATUS
CosaIPv6rdIfRegDelInfo(
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hCosaContext;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepUpperFo  = (PPOAM_IREP_FOLDER_OBJECT )pCosaContext->hPoamIrepUpperFo;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFo       = (PPOAM_IREP_FOLDER_OBJECT )pCosaContext->hPoamIrepFo;

    UNREFERENCED_PARAMETER(hThisObject);

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
