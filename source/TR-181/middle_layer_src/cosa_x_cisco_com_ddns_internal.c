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

    module: cosa_x_cisco_com_ddns_internal.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the internal apis for objects to support Data Model Library.

        *   CosaDdnsCreate
        *   CosaDdnsInitialize
        *   CosaDdnsRemove
        *   CosaDdnsRegGetDdnsInfo
        *   CosaDdnsRegAddDdnsInfo
        *   CosaDdnsRegDelDdnsInfo

    -------------------------------------------------------------------


    author:

        Tom Chang

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**************************************************************************/
#if !defined(DDNS_BROADBANDFORUM)
#include "cosa_x_cisco_com_ddns_internal.h"
#include "cosa_x_cisco_com_security_apis.h"
#include "safec_lib_common.h"

extern void * g_pDslhDmlAgent;

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        CosaDdnsCreate
            (
                VOID
            );

    description:

        This function constructs cosa Ddns object and return handle.

    argument:  

    return:     newly created Ddns object.

**********************************************************************/

ANSC_HANDLE
CosaDdnsCreate
    (
        VOID
    )
{
    PCOSA_DATAMODEL_DDNS            pMyObject    = (PCOSA_DATAMODEL_DDNS)NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PCOSA_DATAMODEL_DDNS)AnscAllocateMemory(sizeof(COSA_DATAMODEL_DDNS));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->Oid               = COSA_DATAMODEL_DDNS_OID;
    pMyObject->Create            = CosaDdnsCreate;
    pMyObject->Remove            = CosaDdnsRemove;
    pMyObject->Initialize        = CosaDdnsInitialize;

    pMyObject->Initialize   ((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    prototype:

        ANSC_STATUS
        CosaDdnsInitialize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate  cosa Ddns object and return handle.

    argument:	ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaDdnsInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DDNS            pMyObject       = (PCOSA_DATAMODEL_DDNS     )hThisObject;
    PSLIST_HEADER                   pListHead       = (PSLIST_HEADER            )&pMyObject->ContextHead;
    PCOSA_DML_DDNS_CFG              pDdnsCfg        = (PCOSA_DML_DDNS_CFG       )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_DDNS_SERVICE          pDdnsService    = (PCOSA_DML_DDNS_SERVICE   )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoCOSA = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoDdns = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PSLAP_VARIABLE                  pSlapVariable   = (PSLAP_VARIABLE           )NULL;
    ULONG                           ulServiceCount  = 0;
    ULONG                           ulIndex         = 0;
    errno_t                         rc              = -1;
    
    returnStatus = CosaDmlDdnsInit(NULL, NULL);

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        return returnStatus;
    }

    /* Initiation all functions */
    pMyObject->ulNextInstance      = 1;

    AnscSListInitializeHeader(&pMyObject->ContextHead);

    pDdnsCfg = AnscAllocateMemory(sizeof(COSA_DML_DDNS_CFG));

    if ( !pDdnsCfg )
    {
        CcspTraceWarning(("X_CISCO_COM_DDNS Resource allocation error\n"));

        return ANSC_STATUS_RESOURCES;
    }

    returnStatus = CosaDmlDdnsGetConfig(NULL, pDdnsCfg);

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        goto EXIT1;
    }

    pMyObject->DdnsConfig.bEnabled = pDdnsCfg->bEnabled;

    pMyObject->hIrepFolderCOSA = g_GetRegistryRootFolder(g_pDslhDmlAgent);

    pPoamIrepFoCOSA = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderCOSA;

    if ( !pPoamIrepFoCOSA )
    {
        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT1;
    }

    pPoamIrepFoDdns = 
        (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoCOSA->GetFolder
            (
                (ANSC_HANDLE)pPoamIrepFoCOSA,
                COSA_IREP_FOLDER_NAME_DDNS
            );

    if ( !pPoamIrepFoDdns )
    {
        pPoamIrepFoDdns =
            pPoamIrepFoCOSA->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoCOSA,
                    COSA_IREP_FOLDER_NAME_DDNS,
                    0
                );
    }

    if ( !pPoamIrepFoDdns )
    {
        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT1;
    }
    else
    {
        pMyObject->hIrepFolderDdns = (ANSC_HANDLE)pPoamIrepFoDdns;
    }

    /* Retrieve Next Instance Number */

    if ( TRUE )
    {
        pSlapVariable =
            (PSLAP_VARIABLE)pPoamIrepFoDdns->GetRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoDdns,
                    COSA_DML_RR_NAME_DdnsServiceNextInsNumber,
                    NULL
                );

        if ( pSlapVariable )
        {
            pMyObject->ulNextInstance = pSlapVariable->Variant.varUint32;

            SlapFreeVariable(pSlapVariable);
        }
    }

    /* Initialize middle layer */

    ulServiceCount = CosaDmlDdnsGetNumberOfServices(NULL);

    for ( ulIndex = 0; ulIndex < ulServiceCount; ulIndex++ )
    {
        pDdnsService = (PCOSA_DML_DDNS_SERVICE)AnscAllocateMemory(sizeof(COSA_DML_DDNS_SERVICE));

        if ( !pDdnsService )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto EXIT1;
        }

        returnStatus = CosaDmlDdnsGetService(NULL, ulIndex, pDdnsService);

        if ( returnStatus == ANSC_STATUS_SUCCESS )
        {
            pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

            if ( !pCosaContext )
            {
                returnStatus = ANSC_STATUS_RESOURCES;

                goto EXIT2;
            }

            if ( pDdnsService->InstanceNumber != 0 )
            {
                pCosaContext->InstanceNumber = pDdnsService->InstanceNumber;

                if ( pMyObject->ulNextInstance <= pDdnsService->InstanceNumber )
                {
                    pMyObject->ulNextInstance = pDdnsService->InstanceNumber + 1;

                    if ( pMyObject->ulNextInstance == 0 )
                    {
                        pMyObject->ulNextInstance = 1;
                    }
                }  
            }
            else
            {
                pDdnsService->InstanceNumber = pCosaContext->InstanceNumber = pMyObject->ulNextInstance;

                pMyObject->ulNextInstance++;

                if ( pMyObject->ulNextInstance == 0 )
                {
                    pMyObject->ulNextInstance = 1;
                }

                /* Generate Alias */
                rc = sprintf_s(pDdnsService->Alias, sizeof(pDdnsService->Alias),"DdnsService%lu", pCosaContext->InstanceNumber);
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                  AnscFreeMemory(pDdnsService);
                  AnscFreeMemory(pCosaContext);
                  continue;
                }

                returnStatus = 
                    CosaDmlDdnsServiceSetValues
                        (
                            NULL, 
                            ulIndex, 
                            pCosaContext->InstanceNumber, 
                            pDdnsService->Alias
                        );

                if ( returnStatus != ANSC_STATUS_SUCCESS )
                {
                    CcspTraceWarning(("!!! CosaDmlDdnsServiceSetValues index: %lu-- fail !!!\n", ulIndex));
                    
                    AnscFreeMemory(pDdnsService);
                    AnscFreeMemory(pCosaContext);

                    continue;
                }
            }

            pCosaContext->hContext     = (ANSC_HANDLE)pDdnsService;
            pCosaContext->hParentTable = NULL;
            pCosaContext->bNew         = FALSE;

            CosaSListPushEntryByInsNum(pListHead, pCosaContext);
        }
    }

    /* Load the newly added but not yet committed entries, if exist */

    CosaDdnsRegGetDdnsInfo((ANSC_HANDLE)pMyObject);

    goto EXIT1;

EXIT2:

    AnscFreeMemory(pDdnsService);

EXIT1:

    AnscFreeMemory(pDdnsCfg);

    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDdnsRemove
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate  cosa Ddns object and return handle.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaDdnsRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DDNS            pMyObject        = (PCOSA_DATAMODEL_DDNS     )hThisObject;
    PSLIST_HEADER                   pListHead        = (PSLIST_HEADER            )&pMyObject->ContextHead;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoDdns  = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderDdns;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFo      = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_DDNS_SERVICE          pDdnsEntry       = (PCOSA_DML_DDNS_SERVICE   )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry      = (PSINGLE_LINK_ENTRY       )NULL;

	/* Remove necessary resounce */
    pSLinkEntry = AnscSListGetFirstEntry(pListHead);

    while ( pSLinkEntry )
    {
        pCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        pSLinkEntry  = AnscSListGetNextEntry(pSLinkEntry);

        pDdnsEntry = (PCOSA_DML_DDNS_SERVICE)pCosaContext->hContext;

        if ( pDdnsEntry )
        {
            AnscSListPopEntryByLink(pListHead, &pCosaContext->Linkage);

            AnscFreeMemory(pDdnsEntry);

            pPoamIrepFo = pCosaContext->hPoamIrepFo;

            if ( pPoamIrepFo )
            {
                pPoamIrepFo->Remove((ANSC_HANDLE)pPoamIrepFo);
            }

            AnscFreeMemory(pCosaContext);
        }
    }

    if ( pPoamIrepFoDdns )
    {
        pPoamIrepFoDdns->Remove((ANSC_HANDLE)pPoamIrepFoDdns);
    }

    /* Remove self */
    AnscFreeMemory((ANSC_HANDLE)pMyObject);

	return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        CosaDdnsRegGetDdnsInfo
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to get dns service info from sys registry.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hDdnsInfo
                Specifies the Dslm policy parameters to be filled.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CosaDdnsRegGetDdnsInfo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PCOSA_DATAMODEL_DDNS            pMyObject         = (PCOSA_DATAMODEL_DDNS     )hThisObject;
    PSLIST_HEADER                   pListHead         = (PSLIST_HEADER            )&pMyObject->ContextHead;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoDdns   = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderDdns;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoDdnsSp = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_DDNS_SERVICE          pDdnsService      = (PCOSA_DML_DDNS_SERVICE   )NULL;
    PSLAP_VARIABLE                  pSlapVariable     = (PSLAP_VARIABLE           )NULL;
    ULONG                           ulEntryCount      = 0;
    ULONG                           ulIndex           = 0;
    ULONG                           ulInstanceNumber  = 0;
    char*                           pFolderName       = NULL;
    char*                           pAlias            = NULL;
    errno_t                         rc                = -1;

    if ( !pPoamIrepFoDdns )
    {
        return ANSC_STATUS_FAILURE;
    }

    /* Load the newly added but not yet commited entries */

    ulEntryCount = pPoamIrepFoDdns->GetFolderCount((ANSC_HANDLE)pPoamIrepFoDdns);

    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        pFolderName =
            pPoamIrepFoDdns->EnumFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoDdns,
                    ulIndex
                );

        if ( !pFolderName )
        {
            continue;
        }

        pPoamIrepFoDdnsSp = pPoamIrepFoDdns->GetFolder((ANSC_HANDLE)pPoamIrepFoDdns, pFolderName);

        AnscFreeMemory(pFolderName);

        if ( !pPoamIrepFoDdnsSp )
        {
            continue;
        }

        if ( TRUE )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoDdnsSp->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoDdnsSp,
                        COSA_DML_RR_NAME_DdnsServiceInsNum,
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
                (PSLAP_VARIABLE)pPoamIrepFoDdnsSp->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoDdnsSp,
                        COSA_DML_RR_NAME_DdnsServiceAlias,
                        NULL
                    );

            if ( pSlapVariable )
            {
                pAlias = AnscCloneString(pSlapVariable->Variant.varString);

                SlapFreeVariable(pSlapVariable);
            }

            if ( pAlias == NULL )
            {
                continue;
            }
        }

        pPoamIrepFoDdnsSp->Remove((ANSC_HANDLE)pPoamIrepFoDdnsSp);

        pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

        if ( !pCosaContext )
        {
            if(pAlias)
            {
                AnscFreeMemory(pAlias);
            }
            return ANSC_STATUS_RESOURCES;
        }

        pDdnsService = (PCOSA_DML_DDNS_SERVICE)AnscAllocateMemory(sizeof(COSA_DML_DDNS_SERVICE));

        if ( !pDdnsService )
        {
            AnscFreeMemory(pCosaContext);
            if(pAlias)
            {
                AnscFreeMemory(pAlias);
            }

            return ANSC_STATUS_RESOURCES;
        }

        pDdnsService->InstanceNumber = ulInstanceNumber;
        if(pAlias) /*RDKB-6744, CID-33330, use only after null check*/
        {
            rc = strcpy_s(pDdnsService->Alias, sizeof(pDdnsService->Alias), pAlias);
            ERR_CHK(rc);
            AnscFreeMemory(pAlias);
            pAlias = NULL;
        }
        else
        {
            pDdnsService->Alias[0] = '\0';
        }

        pCosaContext->InstanceNumber   = ulInstanceNumber;
        pCosaContext->bNew             = TRUE;
        pCosaContext->hContext         = (ANSC_HANDLE)pDdnsService;
        pCosaContext->hParentTable     = NULL;
        pCosaContext->hPoamIrepUpperFo = (ANSC_HANDLE)pPoamIrepFoDdns;
        pCosaContext->hPoamIrepFo      = (ANSC_HANDLE)pPoamIrepFoDdnsSp;

        CosaSListPushEntryByInsNum(pListHead, pCosaContext);    
    }

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        CosaDdnsRegAddDdnsInfo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCosaContext
            );

    description:

        This function is called to add dns service entry to sys registry.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCosaContext
                Specifies the cosa context object.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CosaDdnsRegAddDdnsInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DDNS            pMyObject         = (PCOSA_DATAMODEL_DDNS     )hThisObject;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoDdns   = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderDdns;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoDdnsSp = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hCosaContext;
    PCOSA_DML_DDNS_SERVICE          pDdnsService      = (PCOSA_DML_DDNS_SERVICE   )pCosaContext->hContext;
    PSLAP_VARIABLE                  pSlapVariable     = (PSLAP_VARIABLE           )NULL;

    if ( !pPoamIrepFoDdns )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pPoamIrepFoDdns->EnableFileSync((ANSC_HANDLE)pPoamIrepFoDdns, FALSE);
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
            pPoamIrepFoDdns->DelRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoDdns,
                    COSA_DML_RR_NAME_DdnsServiceNextInsNumber
                );

        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
        pSlapVariable->Variant.varUint32 = pMyObject->ulNextInstance;

        returnStatus =
            pPoamIrepFoDdns->AddRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoDdns,
                    COSA_DML_RR_NAME_DdnsServiceNextInsNumber,
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
        pPoamIrepFoDdnsSp =
            pPoamIrepFoDdns->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoDdns,
                    pDdnsService->Alias,
                    0
                );

        if ( !pPoamIrepFoDdnsSp )
        {
            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT1;
        }

        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
            pSlapVariable->Variant.varUint32 = pDdnsService->InstanceNumber;

            returnStatus =
                pPoamIrepFoDdnsSp->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoDdnsSp,
                        COSA_DML_RR_NAME_DdnsServiceInsNum,
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
            pSlapVariable->Variant.varString = AnscCloneString(pDdnsService->Alias);

            returnStatus =
                pPoamIrepFoDdnsSp->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoDdnsSp,
                        COSA_DML_RR_NAME_DdnsServiceAlias,
                        SYS_REP_RECORD_TYPE_ASTR,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );

            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }

        pCosaContext->hPoamIrepUpperFo = (ANSC_HANDLE)pPoamIrepFoDdns;
        pCosaContext->hPoamIrepFo      = (ANSC_HANDLE)pPoamIrepFoDdnsSp;
    }

EXIT1:
    
    if ( pSlapVariable )
    {
        SlapFreeVariable(pSlapVariable);
    }

    pPoamIrepFoDdns->EnableFileSync((ANSC_HANDLE)pPoamIrepFoDdns, TRUE);

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        CosaDdnsRegDelDdnsInfo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCosaContext
            );

    description:

        This function is called to delete dns entry in sys registry.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCosaContext
                Specifies the cosa context object.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CosaDdnsRegDelDdnsInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hCosaContext;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoDdns   = (PPOAM_IREP_FOLDER_OBJECT )pCosaContext->hPoamIrepUpperFo;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoDdnsSp = (PPOAM_IREP_FOLDER_OBJECT )pCosaContext->hPoamIrepFo;

    if ( !pPoamIrepFoDdns || !pPoamIrepFoDdnsSp )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pPoamIrepFoDdns->EnableFileSync((ANSC_HANDLE)pPoamIrepFoDdns, FALSE);
    }

    if ( TRUE )
    {
        pPoamIrepFoDdnsSp->Close((ANSC_HANDLE)pPoamIrepFoDdnsSp);
        
        pPoamIrepFoDdns->DelFolder
            (
                (ANSC_HANDLE)pPoamIrepFoDdns, 
                pPoamIrepFoDdnsSp->GetFolderName((ANSC_HANDLE)pPoamIrepFoDdnsSp)
            );

        AnscFreeMemory(pPoamIrepFoDdnsSp);
    }

    pPoamIrepFoDdns->EnableFileSync((ANSC_HANDLE)pPoamIrepFoDdns, TRUE);

    return ANSC_STATUS_SUCCESS;
}
#endif
