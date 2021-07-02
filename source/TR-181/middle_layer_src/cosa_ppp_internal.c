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

    module: cosa_ppp_internal.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaPPPCreate
        *  CosaPPPInitialize
        *  CosaPPPRemove
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
#ifndef FEATURE_RDKB_XDSL_PPP_MANAGER

#include "cosa_ppp_internal.h"
#include "safec_lib_common.h"

extern void * g_pDslhDmlAgent;

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        CosaPPPCreate
            (
            );

    description:

        This function constructs cosa nat object and return handle.

    argument:  

    return:     newly created nat object.

**********************************************************************/

ANSC_HANDLE
CosaPPPCreate
    (
        VOID
    )
{
    PCOSA_DATAMODEL_PPP             pMyObject    = (PCOSA_DATAMODEL_PPP)NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PCOSA_DATAMODEL_PPP)AnscAllocateMemory(sizeof(COSA_DATAMODEL_PPP));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->Oid               = COSA_DATAMODEL_PPP_OID;
    pMyObject->Create            = CosaPPPCreate;
    pMyObject->Remove            = CosaPPPRemove;
    pMyObject->Initialize        = CosaPPPInitialize;

    pMyObject->Initialize   ((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaPPPInitialize
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
CosaPPPInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_PPP             pMyObject           = (PCOSA_DATAMODEL_PPP)hThisObject;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoCOSA     = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoPPPIf    = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PSLAP_VARIABLE                  pSlapVariable       = (PSLAP_VARIABLE          )NULL;
    PCOSA_DML_PPP_IF_FULL           pEntry              = (PCOSA_DML_PPP_IF_FULL   )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext        = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    ULONG                           ulEntryCount        = 0;
    ULONG                           ulIndex             = 0;
    errno_t                         rc                  = -1;

    /* Initiation all functions */
    
    CosaDmlPppInit(NULL, NULL);
    
    /* Initiation Device.PPP.Interface */

    AnscSListInitializeHeader(&pMyObject->IfList);

    pMyObject->ulIfNextInstance = 1;

    pMyObject->hIrepFolderCOSA = g_GetRegistryRootFolder(g_pDslhDmlAgent);

    pPoamIrepFoCOSA = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderCOSA;
    if ( !pPoamIrepFoCOSA )
    {
        return ANSC_STATUS_FAILURE;
    }

    pPoamIrepFoPPPIf =
        (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoCOSA->GetFolder
            (
                (ANSC_HANDLE)pPoamIrepFoCOSA,
                COSA_IREP_FOLDER_NAME_PPPIF
            );

    if ( !pPoamIrepFoPPPIf )
    {
        pPoamIrepFoPPPIf =
            pPoamIrepFoCOSA->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoCOSA,
                    COSA_IREP_FOLDER_NAME_PPPIF,
                    0
                );
    }

    if ( !pPoamIrepFoPPPIf )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pMyObject->hIrepFolderPPPIf = (ANSC_HANDLE)pPoamIrepFoPPPIf;
    }

    /* Retrieve Next Instance Number */
    
    if ( TRUE )
    {
        pSlapVariable =
            (PSLAP_VARIABLE)pPoamIrepFoPPPIf->GetRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoPPPIf,
                    COSA_DML_RR_NAME_PPPIFNextInsNunmber,
                    NULL
                );

        if ( pSlapVariable )
        {
            pMyObject->ulIfNextInstance = pSlapVariable->Variant.varUint32;

            SlapFreeVariable(pSlapVariable);
        }
    }    

    /* Initialize middle layer */

    ulEntryCount = CosaDmlPppIfGetNumberOfEntries(NULL);

    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        pEntry = (PCOSA_DML_PPP_IF_FULL)AnscAllocateMemory(sizeof(COSA_DML_PPP_IF_FULL));

        if ( !pEntry )
        {
            return ANSC_STATUS_RESOURCES;
        }
        
        CosaDmlPppIfGetEntry(NULL, ulIndex, pEntry);

        if ( TRUE )
        {
            pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

            if ( !pCosaContext )
            {
                AnscFreeMemory(pEntry);
                
                return ANSC_STATUS_RESOURCES;
            }

            if ( pEntry->Cfg.InstanceNumber != 0 )
            {
                pCosaContext->InstanceNumber = pEntry->Cfg.InstanceNumber;

                if ( pMyObject->ulIfNextInstance <= pEntry->Cfg.InstanceNumber )
                {
                    pMyObject->ulIfNextInstance = pEntry->Cfg.InstanceNumber + 1;

                    if ( pMyObject->ulIfNextInstance == 0 )
                    {
                        pMyObject->ulIfNextInstance = 1;
                    }
                }
            }
            else
            {
                pEntry->Cfg.InstanceNumber = pCosaContext->InstanceNumber = pMyObject->ulIfNextInstance;

                pMyObject->ulIfNextInstance++;

                if ( pMyObject->ulIfNextInstance == 0 )
                {
                    pMyObject->ulIfNextInstance = 1;
                }

                /* Generate Alias */
                rc = sprintf_s(pEntry->Cfg.Alias, sizeof(pEntry->Cfg.Alias),"Interface%lu", pCosaContext->InstanceNumber);
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                }

                CosaDmlPppIfSetValues
                (
                    NULL,
                    ulIndex,
                    pCosaContext->InstanceNumber,
                    pEntry->Cfg.Alias
                );
            }

            pCosaContext->hContext      = (ANSC_HANDLE)pEntry;
            pCosaContext->hParentTable  = NULL;
            pCosaContext->bNew          = FALSE;

            CosaSListPushEntryByInsNum(&pMyObject->IfList, pCosaContext);
        }
    }

    /* Load the newly added but not yet commited entries, if exist */

    CosaPPPIfRegGetInfo((ANSC_HANDLE)pMyObject);
    
    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaPPPRemove
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
CosaPPPRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_PPP             pMyObject    = (PCOSA_DATAMODEL_PPP)hThisObject;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = NULL;

    /* Remove necessary resounce */

    CosaPPPApiRemove((ANSC_HANDLE)pMyObject);
    
    if (AnscSListQueryDepth(&pMyObject->IfList) != 0)
    {
        pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->IfList);

        while ( pSLinkEntry )
        {
            pCosaContext            = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
            pSLinkEntry             = AnscSListGetNextEntry(pSLinkEntry);

            AnscSListPopEntryByLink(&pMyObject->IfList, &pCosaContext->Linkage);           

            AnscFreeMemory(pCosaContext->hContext);
            AnscFreeMemory(pCosaContext);
        }
    }

    /* Remove self */
    AnscFreeMemory((ANSC_HANDLE)pMyObject);

    return returnStatus;
}

ANSC_STATUS
CosaPPPIfRegGetInfo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PCOSA_DATAMODEL_PPP             pMyObject               = (PCOSA_DATAMODEL_PPP      )hThisObject;
    PSLIST_HEADER                   pListHead               = (PSLIST_HEADER            )&pMyObject->IfList;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoPPPIf        = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderPPPIf;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoPPPIfSp      = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PCOSA_DML_PPP_IF_FULL           pEntry                  = (PCOSA_DML_PPP_IF_FULL    )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSLAP_VARIABLE                  pSlapVariable           = (PSLAP_VARIABLE           )NULL;
    ULONG                           ulEntryCount            = 0;
    ULONG                           ulIndex                 = 0;
    ULONG                           ulInstanceNumber        = 0;
    char*                           pFolderName             = NULL;
    char*                           pAlias                  = NULL;
    errno_t                         rc                      = -1;

    if ( !pPoamIrepFoPPPIf )
    {
        return ANSC_STATUS_FAILURE;
    }

    /* Load the newly added but not yet commited entries */
    
    ulEntryCount = pPoamIrepFoPPPIf->GetFolderCount((ANSC_HANDLE)pPoamIrepFoPPPIf);

    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        pFolderName =
            pPoamIrepFoPPPIf->EnumFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoPPPIf,
                    ulIndex
                );

        if ( !pFolderName )
        {
            continue;
        }

        pPoamIrepFoPPPIfSp = pPoamIrepFoPPPIf->GetFolder((ANSC_HANDLE)pPoamIrepFoPPPIf, pFolderName);

        AnscFreeMemory(pFolderName);

        if ( !pPoamIrepFoPPPIfSp )
        {
            continue;
        }

        if ( TRUE )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoPPPIfSp->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoPPPIfSp,
                        COSA_DML_RR_NAME_PPPIFInsNum,
                        NULL
                    );

            if ( pSlapVariable )
            {
                ulInstanceNumber = pSlapVariable->Variant.varUint32;

                SlapFreeVariable(pSlapVariable);
            }
        }

            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoPPPIfSp->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoPPPIfSp,
                        COSA_DML_RR_NAME_PPPIFAlias,
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

        pEntry = (PCOSA_DML_PPP_IF_FULL)AnscAllocateMemory(sizeof(COSA_DML_PPP_IF_FULL));

        if ( !pEntry )
        {
            AnscFreeMemory(pAlias);
            AnscFreeMemory(pCosaContext);

            return ANSC_STATUS_RESOURCES;
        }

        rc = strcpy_s(pEntry->Cfg.Alias, sizeof(pEntry->Cfg.Alias), pAlias ? pAlias : "");
        ERR_CHK(rc);

        pEntry->Cfg.InstanceNumber = ulInstanceNumber;

        pCosaContext->InstanceNumber        = ulInstanceNumber;
        pCosaContext->bNew                  = TRUE;
        pCosaContext->hContext              = (ANSC_HANDLE)pEntry;
        pCosaContext->hParentTable          = NULL;
        pCosaContext->hPoamIrepUpperFo      = (ANSC_HANDLE)pPoamIrepFoPPPIf;
        pCosaContext->hPoamIrepFo           = (ANSC_HANDLE)pPoamIrepFoPPPIfSp;

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
CosaPPPIfRegAddInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    )
{
    ANSC_STATUS                     returnStatus            = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_PPP             pMyObject               = (PCOSA_DATAMODEL_PPP      )hThisObject;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoPPPIf        = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderPPPIf;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoPPPIfSp      = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)hCosaContext;
    PCOSA_DML_PPP_IF_FULL           pEntry                  = (PCOSA_DML_PPP_IF_FULL    )pCosaContext->hContext;
    PSLAP_VARIABLE                  pSlapVariable           = (PSLAP_VARIABLE           )NULL;

    if ( !pPoamIrepFoPPPIf )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pPoamIrepFoPPPIf->EnableFileSync((ANSC_HANDLE)pPoamIrepFoPPPIf, FALSE);
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
            pPoamIrepFoPPPIf->DelRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoPPPIf,
                    COSA_DML_RR_NAME_PPPIFNextInsNunmber
                );

        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
        pSlapVariable->Variant.varUint32 = pMyObject->ulIfNextInstance;

        returnStatus =
            pPoamIrepFoPPPIf->AddRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoPPPIf,
                    COSA_DML_RR_NAME_PPPIFNextInsNunmber,
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
        pPoamIrepFoPPPIfSp =
            pPoamIrepFoPPPIf->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoPPPIf,
                    pEntry->Cfg.Alias,
                    0
                );

        if ( !pPoamIrepFoPPPIfSp )
        {
            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT1;
        }

        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
            pSlapVariable->Variant.varUint32 = pEntry->Cfg.InstanceNumber;

            returnStatus =
                pPoamIrepFoPPPIfSp->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoPPPIfSp,
                        COSA_DML_RR_NAME_PPPIFInsNum,
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
            pSlapVariable->Variant.varString = AnscCloneString(pEntry->Cfg.Alias);

            returnStatus =
                pPoamIrepFoPPPIfSp->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoPPPIfSp,
                        COSA_DML_RR_NAME_PPPIFAlias,
                        SYS_REP_RECORD_TYPE_ASTR,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );

            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }

        pCosaContext->hPoamIrepUpperFo = (ANSC_HANDLE)pPoamIrepFoPPPIf;
        pCosaContext->hPoamIrepFo      = (ANSC_HANDLE)pPoamIrepFoPPPIfSp;
    }

EXIT1:
    
    if ( pSlapVariable )
    {
        SlapFreeVariable(pSlapVariable);
    }

    pPoamIrepFoPPPIf->EnableFileSync((ANSC_HANDLE)pPoamIrepFoPPPIf, TRUE);

    return returnStatus;
}

ANSC_STATUS
CosaPPPIfRegDelInfo
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
