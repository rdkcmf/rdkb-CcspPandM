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

    module: cosa_neighdisc_internal.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaNeighdiscCCreate
        *  CosaNeighdiscInitialize
        *  CosaNeighdiscRemove
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
#include "cosa_apis.h"
#include "cosa_neighdisc_apis.h"
#include "cosa_neighdisc_internal.h"
#include "plugin_main_apis.h"
#include "slap_vho_exported_api.h"
#include "safec_lib_common.h"

extern void * g_pDslhDmlAgent;

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        CosaNeighdiscCreate
            (
            );

    description:

        This function constructs cosa nat object and return handle.

    argument:  

    return:     newly created nat object.

**********************************************************************/

ANSC_HANDLE
CosaNeighdiscCreate
    (
        VOID
    )
{
    PCOSA_DATAMODEL_NEIGHDISC       pMyObject    = (PCOSA_DATAMODEL_NEIGHDISC)NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PCOSA_DATAMODEL_NEIGHDISC)AnscAllocateMemory(sizeof(COSA_DATAMODEL_NEIGHDISC));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->Oid               = COSA_DATAMODEL_NEIGHDISC_OID;
    pMyObject->Create            = CosaNeighdiscCreate;
    pMyObject->Remove            = CosaNeighdiscRemove;
    pMyObject->Initialize        = CosaNeighdiscInitialize;

    pMyObject->Initialize   ((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaNeighdiscInitialize
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
CosaNeighdiscInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_NEIGHDISC       pMyObject       = (PCOSA_DATAMODEL_NEIGHDISC)hThisObject;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoCOSA = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoNeighdiscIF = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoNeighdiscHA = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoNextIns 
                                                    = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PSLAP_VARIABLE                  pSlapVariable   = (PSLAP_VARIABLE          )NULL;
    PCOSA_DML_NEIGHDISC_IF_FULL     pNeighdiscInterface    = (PCOSA_DML_NEIGHDISC_IF_FULL   )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    char                            FolderName[32]  = {0};
    ULONG                           ulEntryCount    = 0;
    ULONG                           ulIndex         = 0;
    errno_t                         rc              = -1;

    CosaDmlNeighdiscInit(NULL, NULL);

    /* Initiation all functions */
    pMyObject->ulInterfaceCount                = 0;
    pMyObject->ulNextInterfaceInsNum           = 1;
    
    AnscSListInitializeHeader(&pMyObject->InterfaceList);

    pMyObject->hIrepFolderCOSA = g_GetRegistryRootFolder(g_pDslhDmlAgent);

    pPoamIrepFoCOSA = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderCOSA;
    
    if ( !pPoamIrepFoCOSA )
    {
        returnStatus = ANSC_STATUS_FAILURE;
        
        goto  EXIT;
    }

    pPoamIrepFoNeighdiscIF = (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoCOSA->GetFolder
                        (
                            (ANSC_HANDLE)pPoamIrepFoCOSA,
                            COSA_IREP_FOLDER_NAME_NeighdiscIF
                        );

    if ( !pPoamIrepFoNeighdiscIF )
    {
        pPoamIrepFoNeighdiscIF = pPoamIrepFoCOSA->AddFolder
                            (
                                (ANSC_HANDLE)pPoamIrepFoCOSA,
                                COSA_IREP_FOLDER_NAME_NeighdiscIF,
                                0
                            );
    }

    if ( !pPoamIrepFoNeighdiscIF )
    {
        returnStatus = ANSC_STATUS_FAILURE;
        
        goto  EXIT;
    }
    else
    {        
        pMyObject->hIrepFolderNeighdiscInterface = (ANSC_HANDLE)pPoamIrepFoNeighdiscIF;
    }

    pPoamIrepFoNeighdiscHA = (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoNeighdiscIF->GetFolder
                        (
                            (ANSC_HANDLE)pPoamIrepFoCOSA,
                            COSA_IREP_FOLDER_NAME_NeighdiscIF_HA
                        );

    if ( !pPoamIrepFoNeighdiscHA )
    {
        pPoamIrepFoNeighdiscHA = pPoamIrepFoNeighdiscIF->AddFolder
                            (
                                (ANSC_HANDLE)pPoamIrepFoNeighdiscIF,
                                COSA_IREP_FOLDER_NAME_NeighdiscIF_HA,
                                0
                            );
    }

    if ( !pPoamIrepFoNeighdiscHA )
    {
        returnStatus = ANSC_STATUS_FAILURE;
        
        goto  EXIT;
    }
    else
    {
        pMyObject->hIrepFolderNeighdiscInterfaceHA = (ANSC_HANDLE)pPoamIrepFoNeighdiscHA;
    }

    /* Retrieve the next Instance Number for Neighdisc interface */
    
    if ( TRUE )
    {
        rc = sprintf_s
        (
            FolderName,
            sizeof(FolderName),
            "%s%d", 
            COSA_DML_RR_NAME_NeighdiscIF_NextInsNum,
            0
        );
        if(rc < EOK)
        {
          ERR_CHK(rc);
        }
        
        pPoamIrepFoNextIns = (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoNeighdiscIF->GetFolder
                                (
                                    (ANSC_HANDLE)pPoamIrepFoNeighdiscIF,
                                    FolderName
                                );

        AnscZeroMemory(FolderName, 32);

        if ( pPoamIrepFoNextIns )
        {
            pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoNextIns->GetRecord
                                (
                                    (ANSC_HANDLE)pPoamIrepFoNextIns,
                                    COSA_DML_RR_NAME_NeighdiscIF_NextInsNum,
                                    NULL
                                );

            if ( pSlapVariable )
            {
                pMyObject->ulNextInterfaceInsNum = pSlapVariable->Variant.varUint32;
                
                SlapFreeVariable(pSlapVariable);
            }

            pPoamIrepFoNextIns->Remove((ANSC_HANDLE)pPoamIrepFoNextIns);
            pPoamIrepFoNextIns = NULL;
        }
    }

    /* Initialize middle layer for Device.NeighborDiscovery.InterfaceSetting.{i}. */

    ulEntryCount = CosaDmlNeighdiscIfGetNumberOfEntries(NULL);
    
    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        pNeighdiscInterface = (PCOSA_DML_NEIGHDISC_IF_FULL)AnscAllocateMemory(sizeof(COSA_DML_NEIGHDISC_IF_FULL));

        if ( !pNeighdiscInterface )
        {
            returnStatus = ANSC_STATUS_RESOURCES;
            
            goto  EXIT;
        }

        CosaDmlNeighdiscIfGetEntry(NULL, ulIndex,(PCOSA_DML_NEIGHDISC_IF_FULL)pNeighdiscInterface);

        if ( TRUE )
        {
            pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

            if ( !pCosaContext )
            {
                AnscFreeMemory(pNeighdiscInterface);               
                
                returnStatus = ANSC_STATUS_RESOURCES;
                
                goto  EXIT;
            }

            if ( 0 != pNeighdiscInterface->Cfg.InstanceNumber )
            {
                pCosaContext->InstanceNumber = pNeighdiscInterface->Cfg.InstanceNumber;
                
                if ( pMyObject->ulNextInterfaceInsNum <=  pNeighdiscInterface->Cfg.InstanceNumber )
                {
                    pMyObject->ulNextInterfaceInsNum =  pNeighdiscInterface->Cfg.InstanceNumber + 1;

                    if ( 0 == pMyObject->ulNextInterfaceInsNum )
                    {
                        pMyObject->ulNextInterfaceInsNum = 1;
                    }
                }
            }
            else
            {
                pCosaContext->InstanceNumber = pMyObject->ulNextInterfaceInsNum;  
                
                pNeighdiscInterface->Cfg.InstanceNumber = pCosaContext->InstanceNumber ;
                
                pMyObject->ulNextInterfaceInsNum++;
                
                if ( 0 == pMyObject->ulNextInterfaceInsNum )
                {
                    pMyObject->ulNextInterfaceInsNum = 1;
                }

                /* Generate Alias */
                rc = sprintf_s(pNeighdiscInterface->Cfg.Alias, sizeof(pNeighdiscInterface->Cfg.Alias),"Interface%d", (int)pMyObject->ulNextInterfaceInsNum);
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                }

                CosaDmlNeighdiscIfSetValues
                (
                    NULL,
                    ulIndex,
                    pNeighdiscInterface->Cfg.InstanceNumber,
                    pNeighdiscInterface->Cfg.Alias
                );
            }

            pCosaContext->hContext      = (ANSC_HANDLE)pNeighdiscInterface;            
            pCosaContext->hParentTable  = NULL;            
            pCosaContext->bNew          = FALSE;

            CosaSListPushEntryByInsNum(&pMyObject->InterfaceList, pCosaContext);
                  
        }

    }

    CosaNeighdiscRegGetInfo((ANSC_HANDLE)pMyObject);

    EXIT:
        
    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaNeighdiscRemove
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
CosaNeighdiscRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_NEIGHDISC       pMyObject       = (PCOSA_DATAMODEL_NEIGHDISC)hThisObject;
    PSLIST_HEADER                   pNeighdiscIFHead       = (PSLIST_HEADER)&pMyObject->InterfaceList;
    PCOSA_DML_NEIGHDISC_IF_FULL     pNeighdiscInterface    = (PCOSA_DML_NEIGHDISC_IF_FULL)NULL;
    PSINGLE_LINK_ENTRY              pLink           = (PSINGLE_LINK_ENTRY )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoTemp = (PPOAM_IREP_FOLDER_OBJECT)NULL;

    /*free the back end memory*/
    CosaDmlNeighdiscIfBERemove();

    /* Neighdisc Interface List */
    pLink = AnscSListGetFirstEntry(pNeighdiscIFHead);

    while ( pLink )
    {
        pCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        pLink   = AnscSListGetNextEntry(pLink);

        pNeighdiscInterface = (PCOSA_DML_NEIGHDISC_IF_FULL)pCosaContext->hContext;

        if ( pNeighdiscInterface )
        {
            AnscSListPopEntryByLink(pNeighdiscIFHead, &pCosaContext->Linkage);

            AnscFreeMemory(pNeighdiscInterface);
            AnscFreeMemory(pCosaContext);
        }
    }


    /* Remove self */
    pPoamIrepFoTemp = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderNeighdiscInterfaceHA;
    if ( pPoamIrepFoTemp )
    {
        pPoamIrepFoTemp->Remove(pPoamIrepFoTemp);
    }

    
    pPoamIrepFoTemp = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderNeighdiscInterface;
    if ( pPoamIrepFoTemp )
    {
        pPoamIrepFoTemp->Remove(pPoamIrepFoTemp);
    }
 

    AnscFreeMemory((ANSC_HANDLE)pMyObject);

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        CosaNeighdiscRegGetInfo
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve Neighdisc intreface parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CosaNeighdiscRegGetInfo

    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_NEIGHDISC       pMyObject       = (PCOSA_DATAMODEL_NEIGHDISC)hThisObject;
    PSLIST_HEADER                   pNeighdiscIFHead       = (PSLIST_HEADER)&pMyObject->InterfaceList;
    PCOSA_DML_NEIGHDISC_IF_FULL     pNeighdiscInterface    = (PCOSA_DML_NEIGHDISC_IF_FULL)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoNeighdiscHA = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderNeighdiscInterfaceHA;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoNeighdiscIF = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PSLAP_VARIABLE                  pSlapVariable   = (PSLAP_VARIABLE          )NULL;
    ULONG                           ulEntryCount            = 0;
    ULONG                           ulIndex                 = 0;
    ULONG                           ulInstanceNumber        = 0;
    char*                           pFolderName             = NULL;
    char*                           pAlias                  = NULL;
    errno_t                         rc                      = -1;

    if ( !pPoamIrepFoNeighdiscHA )
    {
        return ANSC_STATUS_FAILURE;
    }

    /* Load the newly added but not yet commited entries */

    ulEntryCount = pPoamIrepFoNeighdiscHA->GetFolderCount((ANSC_HANDLE)pPoamIrepFoNeighdiscHA);

    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        pFolderName = pPoamIrepFoNeighdiscHA->EnumFolder
            (
                (ANSC_HANDLE)pPoamIrepFoNeighdiscHA,
                ulIndex
            );

        if ( !pFolderName )
        {
            continue;
        }

        pPoamIrepFoNeighdiscIF = (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoNeighdiscHA->GetFolder
            (
                (ANSC_HANDLE)pPoamIrepFoNeighdiscHA, 
                pFolderName
            );

        if ( !pPoamIrepFoNeighdiscIF )
        {
            continue;
        }

        if ( _ansc_strstr(pFolderName, COSA_DML_RR_NAME_NeighdiscIF_Prefix) )
        {

            pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoNeighdiscIF->GetRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoNeighdiscIF,
                    COSA_DML_RR_NAME_NeighdiscIF_InsNum,
                    NULL
                );

            if ( pSlapVariable )
            {
                ulInstanceNumber = pSlapVariable->Variant.varUint32;
                SlapFreeVariable(pSlapVariable);
            }

            pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoNeighdiscIF->GetRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoNeighdiscIF,
                    COSA_DML_RR_NAME_NeighdiscIF_Alias,
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
                returnStatus = ANSC_STATUS_RESOURCES;
                goto EXIT;
            }

            pNeighdiscInterface = (PCOSA_DML_NEIGHDISC_IF_FULL)AnscAllocateMemory(sizeof(COSA_DML_NEIGHDISC_IF_FULL));

            if ( !pNeighdiscInterface )
            {
                AnscFreeMemory(pCosaContext);
                
                returnStatus = ANSC_STATUS_RESOURCES;
                
                goto EXIT;
            }

            pNeighdiscInterface->Cfg.InstanceNumber = ulInstanceNumber;

            rc = strcpy_s(pNeighdiscInterface->Cfg.Alias, sizeof(pNeighdiscInterface->Cfg.Alias), pAlias ? pAlias : "");
            ERR_CHK(rc);

            pCosaContext->InstanceNumber   = ulInstanceNumber;
            pCosaContext->bNew             = TRUE;
            pCosaContext->hContext         = (ANSC_HANDLE)pNeighdiscInterface;
            pCosaContext->hParentTable     = NULL;
            pCosaContext->hPoamIrepUpperFo = (ANSC_HANDLE)pPoamIrepFoNeighdiscHA;
            pCosaContext->hPoamIrepFo      = (ANSC_HANDLE)pPoamIrepFoNeighdiscIF;
            
            CosaSListPushEntryByInsNum(pNeighdiscIFHead, pCosaContext);
        }
    
        if ( pAlias )
        {
            AnscFreeMemory(pAlias);
            pAlias = NULL;
        }

        if ( pFolderName )
        {
            AnscFreeMemory(pFolderName);
            pFolderName = NULL;
        }
    }

EXIT:

    if ( pAlias )
    {
        AnscFreeMemory(pAlias);
    }

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        CosaNeighdiscRegAddInfo
            (
                ANSC_HANDLE                 hThisObject,
                char*                            pNextInsNumName,
                ULONG                           ulNextInsNum,
                char*                            pPreffix,
                ULONG                           ulUpperInsNum,
                char*                            pAlias,
                ANSC_HANDLE                 hCosaContext
            );


    description:

        This function is called to retrieve Neighdisc intreface parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CosaNeighdiscRegAddInfo
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pNextInsNumName,
        ULONG                       ulNextInsNum,
        char*                       pPreffix,
        ULONG                       ulUpperInsNum,
        char*                       pAlias,
        ANSC_HANDLE                 hCosaContext
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_NEIGHDISC       pMyObject         = (PCOSA_DATAMODEL_NEIGHDISC )hThisObject;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hCosaContext;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoNeighdiscIF   = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderNeighdiscInterface;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoUpper  = (PPOAM_IREP_FOLDER_OBJECT )pCosaContext->hPoamIrepUpperFo;  
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFo       = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PSLAP_VARIABLE                  pSlapVariable     = (PSLAP_VARIABLE           )NULL;
    char                            FolderName[32]    = {0};
    errno_t                         rc                = -1;

    if ( !pPoamIrepFoNeighdiscIF || !pPoamIrepFoUpper )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pPoamIrepFoNeighdiscIF->EnableFileSync((ANSC_HANDLE)pPoamIrepFoNeighdiscIF, FALSE);
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

    rc = sprintf_s(FolderName, sizeof(FolderName),"%s%d", pNextInsNumName, (int)ulUpperInsNum);
    if(rc < EOK)
    {
      ERR_CHK(rc);
    }

    if ( TRUE )
    {
        pPoamIrepFo = (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoNeighdiscIF->GetFolder
                        (
                            (ANSC_HANDLE)pPoamIrepFoNeighdiscIF,
                            FolderName
                        );

        if ( !pPoamIrepFo )
        {
            pPoamIrepFo = pPoamIrepFoNeighdiscIF->AddFolder
                            (
                                (ANSC_HANDLE)pPoamIrepFoNeighdiscIF,
                                FolderName,
                                0
                            );
        }
    }

    if ( TRUE )
    {
        returnStatus = pPoamIrepFo->DelRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFo,
                            pNextInsNumName
                        );

        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
        pSlapVariable->Variant.varUint32 = ulNextInsNum;

        returnStatus = pPoamIrepFo->AddRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFo,
                            pNextInsNumName,
                            SYS_REP_RECORD_TYPE_UINT,
                            SYS_RECORD_CONTENT_DEFAULT,
                            pSlapVariable,
                            0
                        );

        SlapCleanVariable(pSlapVariable);
        SlapInitVariable (pSlapVariable);
    }

    pPoamIrepFo->Remove((ANSC_HANDLE)pPoamIrepFo);
    pPoamIrepFo = NULL;

    if ( TRUE )
    {
        rc = sprintf_s(FolderName, sizeof(FolderName),"%s%d%d", pPreffix, (int)ulUpperInsNum, (int)pCosaContext->InstanceNumber);
        if(rc < EOK)
        {
          ERR_CHK(rc);
        }

        pPoamIrepFo = pPoamIrepFoUpper->AddFolder
                        (
                            (ANSC_HANDLE)pPoamIrepFoUpper,
                            FolderName,
                            0
                        );

        if ( !pPoamIrepFo )
        {
            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT1;
        }

        if ( 0 != ulUpperInsNum )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
            pSlapVariable->Variant.varUint32 = ulUpperInsNum;

            returnStatus = pPoamIrepFo->AddRecord
                            (
                                (ANSC_HANDLE)pPoamIrepFo,
                                COSA_DML_RR_NAME_NeighdiscIF_Upper_InsNum,
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
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
            pSlapVariable->Variant.varUint32 = pCosaContext->InstanceNumber;

            returnStatus = pPoamIrepFo->AddRecord
                            (
                                (ANSC_HANDLE)pPoamIrepFo,
                                COSA_DML_RR_NAME_NeighdiscIF_InsNum,
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
            pSlapVariable->Variant.varString = AnscCloneString(pAlias);

            returnStatus = pPoamIrepFo->AddRecord
                            (
                                (ANSC_HANDLE)pPoamIrepFo,
                                COSA_DML_RR_NAME_NeighdiscIF_Alias,
                                SYS_REP_RECORD_TYPE_ASTR,
                                SYS_RECORD_CONTENT_DEFAULT,
                                pSlapVariable,
                                0
                            );

            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }

        pCosaContext->hPoamIrepFo = (ANSC_HANDLE)pPoamIrepFo;
    }

EXIT1:
    
    if ( pSlapVariable )
    {
        SlapFreeVariable(pSlapVariable);
    }

    pPoamIrepFoNeighdiscIF->EnableFileSync((ANSC_HANDLE)pPoamIrepFoNeighdiscIF, TRUE);

    return returnStatus;
}



/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        CosaNeighdiscRegDelInfo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCosaContext
            );

    description:

        This function is called to retrieve Neighdisc intreface parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                    hCosaContext

    return:     status of operation.

**********************************************************************/


ANSC_STATUS
CosaNeighdiscRegDelInfo
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

        AnscFreeMemory((ANSC_HANDLE)pPoamIrepFo);
    }

    return ANSC_STATUS_SUCCESS;
}



