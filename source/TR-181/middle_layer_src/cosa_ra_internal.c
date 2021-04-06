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

    module: cosa_ra_internal.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaRACCreate
        *  CosaRAInitialize
        *  CosaRARemove
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
#include "cosa_ra_apis.h"
#include "cosa_ra_internal.h"
#include "plugin_main_apis.h"
#include "slap_vho_exported_api.h"
#include "safec_lib_common.h"

extern void * g_pDslhDmlAgent;

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        CosaRACreate
            (
            );

    description:

        This function constructs cosa nat object and return handle.

    argument:  

    return:     newly created nat object.

**********************************************************************/

ANSC_HANDLE
CosaRACreate
    (
        VOID
    )
{
    PCOSA_DATAMODEL_RA              pMyObject    = (PCOSA_DATAMODEL_RA)NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PCOSA_DATAMODEL_RA)AnscAllocateMemory(sizeof(COSA_DATAMODEL_RA));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->Oid               = COSA_DATAMODEL_RA_OID;
    pMyObject->Create            = CosaRACreate;
    pMyObject->Remove            = CosaRARemove;
    pMyObject->Initialize        = CosaRAInitialize;

    pMyObject->Initialize   ((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaRAInitialize
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
CosaRAInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_RA              pMyObject       = (PCOSA_DATAMODEL_RA)hThisObject;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoCOSA = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoRAIF = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoRAHA = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoNextIns 
                                                    = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PSLAP_VARIABLE                  pSlapVariable   = (PSLAP_VARIABLE          )NULL;
    PCOSA_DML_RA_IF_FULL2           pRAInterface    = (PCOSA_DML_RA_IF_FULL2   )NULL;
    PCOSA_DML_RA_OPTION             pRAOption       = (PCOSA_DML_RA_OPTION     )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    char                            FolderName[32]  = {0};
    ULONG                           ulEntryCount    = 0;
    ULONG                           ulIndex         = 0;
    ULONG                           ulSubEntryCount = 0;
    ULONG                           ulSubIndex      = 0;
    errno_t                         rc              = -1;

    CosaDmlRAInit(NULL, NULL);

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

    pPoamIrepFoRAIF = (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoCOSA->GetFolder
                        (
                            (ANSC_HANDLE)pPoamIrepFoCOSA,
                            COSA_IREP_FOLDER_NAME_RAIF
                        );

    if ( !pPoamIrepFoRAIF )
    {
        pPoamIrepFoRAIF = pPoamIrepFoCOSA->AddFolder
                            (
                                (ANSC_HANDLE)pPoamIrepFoCOSA,
                                COSA_IREP_FOLDER_NAME_RAIF,
                                0
                            );
    }

    if ( !pPoamIrepFoRAIF )
    {
        returnStatus = ANSC_STATUS_FAILURE;
        
        goto  EXIT;
    }
    else
    {        
        pMyObject->hIrepFolderRAInterface = (ANSC_HANDLE)pPoamIrepFoRAIF;
    }

    pPoamIrepFoRAHA = (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoRAIF->GetFolder
                        (
                            (ANSC_HANDLE)pPoamIrepFoCOSA,
                            COSA_IREP_FOLDER_NAME_RAIF_HA
                        );

    if ( !pPoamIrepFoRAHA )
    {
        pPoamIrepFoRAHA = pPoamIrepFoRAIF->AddFolder
                            (
                                (ANSC_HANDLE)pPoamIrepFoRAIF,
                                COSA_IREP_FOLDER_NAME_RAIF_HA,
                                0
                            );
    }

    if ( !pPoamIrepFoRAHA )
    {
        returnStatus = ANSC_STATUS_FAILURE;
        
        goto  EXIT;
    }
    else
    {
        pMyObject->hIrepFolderRAInterfaceHA = (ANSC_HANDLE)pPoamIrepFoRAHA;
    }

    /* Retrieve the next Instance Number for RA interface */
    
    if ( TRUE )
    {
        rc = sprintf_s
        (
            FolderName,
            sizeof(FolderName),
            "%s%d", 
            COSA_DML_RR_NAME_RAIF_NextInsNum,
            0
        );
        if(rc < EOK)
        {
          ERR_CHK(rc);
          return ANSC_STATUS_FAILURE;
        }
        
        pPoamIrepFoNextIns = (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoRAIF->GetFolder
                                (
                                    (ANSC_HANDLE)pPoamIrepFoRAIF,
                                    FolderName
                                );

        AnscZeroMemory(FolderName, 32);

        if ( pPoamIrepFoNextIns )
        {
            pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoNextIns->GetRecord
                                (
                                    (ANSC_HANDLE)pPoamIrepFoNextIns,
                                    COSA_DML_RR_NAME_RAIF_NextInsNum,
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

    /* Initialize middle layer for Device.RouterAdvertisement.InterfaceSetting.{i}. */

    ulEntryCount = CosaDmlRaIfGetNumberOfEntries(NULL);
    
    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        pRAInterface = (PCOSA_DML_RA_IF_FULL2)AnscAllocateMemory(sizeof(COSA_DML_RA_IF_FULL2));

        if ( !pRAInterface )
        {
            returnStatus = ANSC_STATUS_RESOURCES;
            
            goto  EXIT;
        }

        AnscSListInitializeHeader(&pRAInterface->OptionList);
        
        pRAInterface->ulNextOptionInsNum = 1;
                
        CosaDmlRaIfGetEntry(NULL, ulIndex,(PCOSA_DML_RA_IF_FULL)pRAInterface);

        if ( TRUE )
        {
            pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

            if ( !pCosaContext )
            {
                AnscFreeMemory(pRAInterface);               
                
                returnStatus = ANSC_STATUS_RESOURCES;
                
                goto  EXIT;
            }

            if ( 0 != pRAInterface->Cfg.InstanceNumber )
            {
                pCosaContext->InstanceNumber = pRAInterface->Cfg.InstanceNumber;
                
                if ( pMyObject->ulNextInterfaceInsNum <=  pRAInterface->Cfg.InstanceNumber )
                {
                    pMyObject->ulNextInterfaceInsNum =  pRAInterface->Cfg.InstanceNumber + 1;

                    if ( 0 == pMyObject->ulNextInterfaceInsNum )
                    {
                        pMyObject->ulNextInterfaceInsNum = 1;
                    }
                }
            }
            else
            {
                pCosaContext->InstanceNumber = pMyObject->ulNextInterfaceInsNum;  
                
                pRAInterface->Cfg.InstanceNumber = pCosaContext->InstanceNumber ;
                
                pMyObject->ulNextInterfaceInsNum++;
                
                if ( 0 == pMyObject->ulNextInterfaceInsNum )
                {
                    pMyObject->ulNextInterfaceInsNum = 1;
                }

                /* Generate Alias */
                rc = sprintf_s(pRAInterface->Cfg.Alias, sizeof(pRAInterface->Cfg.Alias),"Interface%d", (int)pMyObject->ulNextInterfaceInsNum);
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                  AnscFreeMemory(pCosaContext);
                  AnscFreeMemory(pRAInterface);
                  return ANSC_STATUS_FAILURE;
                }

                CosaDmlRaIfSetValues
                (
                    NULL,
                    ulIndex,
                    pRAInterface->Cfg.InstanceNumber,
                    pRAInterface->Cfg.Alias
                );
            }

            pCosaContext->hContext      = (ANSC_HANDLE)pRAInterface;            
            pCosaContext->hParentTable  = NULL;            
            pCosaContext->bNew          = FALSE;

            CosaSListPushEntryByInsNum(&pMyObject->InterfaceList, pCosaContext);
                  
        }

        /* Initialize middle layer for Device.RouterAdvertisement.InterfaceSetting.{i}.Option.{i}.  */
        if ( TRUE )
        {
            rc = sprintf_s
            (
                FolderName,
                sizeof(FolderName),
                "%s%d", 
                COSA_DML_RR_NAME_RAIF_OPTION_NextInsNum, 
                (int)pRAInterface->Cfg.InstanceNumber
            );
            if(rc < EOK)
            {
              ERR_CHK(rc);
              AnscFreeMemory(pRAInterface);
              return ANSC_STATUS_FAILURE;
            }
            
            pPoamIrepFoNextIns = 
                (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoRAIF->GetFolder
                    (
                        (ANSC_HANDLE)pPoamIrepFoRAIF,
                        FolderName
                    );

            if ( pPoamIrepFoNextIns )
            {
                pSlapVariable =
                    (PSLAP_VARIABLE)pPoamIrepFoNextIns->GetRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoNextIns,
                            COSA_DML_RR_NAME_RAIF_OPTION_NextInsNum,
                            NULL
                        );

                if ( pSlapVariable )
                {
                    pRAInterface->ulNextOptionInsNum = pSlapVariable->Variant.varUint32;
                    
                    SlapFreeVariable(pSlapVariable);
                }

                pPoamIrepFoNextIns->Remove((ANSC_HANDLE)pPoamIrepFoNextIns);
                pPoamIrepFoNextIns = NULL;
            } 
        }
        
        ulSubEntryCount = CosaDmlRaIfGetNumberOfOptions(NULL, pRAInterface->Cfg.InstanceNumber);

        for ( ulSubIndex = 0; ulSubIndex < ulSubEntryCount; ulSubIndex++ )
        {
            pRAOption = (PCOSA_DML_RA_OPTION)AnscAllocateMemory(sizeof(COSA_DML_RA_OPTION));
        
            if ( !pRAOption )
            {
                returnStatus = ANSC_STATUS_RESOURCES;
                
                goto  EXIT;
            }
        
            CosaDmlRaIfGetOption(NULL, pRAInterface->Cfg.InstanceNumber, ulSubIndex, pRAOption);
        
            if ( TRUE )
            {
                pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

                if ( !pSubCosaContext )
                {
                    AnscFreeMemory(pRAOption);

                    returnStatus = ANSC_STATUS_RESOURCES;
                    
                    goto  EXIT;  
                }

                if ( 0 != pRAOption->InstanceNumber )
                {
                    pSubCosaContext->InstanceNumber = pRAOption->InstanceNumber;

                    if ( pRAInterface->ulNextOptionInsNum <= pRAOption->InstanceNumber )
                    {
                        pRAInterface->ulNextOptionInsNum = pRAOption->InstanceNumber + 1;

                        if ( pRAInterface->ulNextOptionInsNum == 0 )
                        {
                            pRAInterface->ulNextOptionInsNum = 1;
                        }
                    }  
                }
                else
                {
                    pSubCosaContext->InstanceNumber = pRAOption->InstanceNumber = pRAInterface->ulNextOptionInsNum;

                    pRAInterface->ulNextOptionInsNum++;

                    if ( pRAInterface->ulNextOptionInsNum == 0 )
                    {
                        pRAInterface->ulNextOptionInsNum = 1;
                    }

                    /* Generate Alias */
                    rc = sprintf_s(pRAOption->Alias, sizeof(pRAOption->Alias),"Option%d", (int)pSubCosaContext->InstanceNumber);
                    if(rc < EOK)
                    {
                      ERR_CHK(rc);
                      AnscFreeMemory(pRAOption);
                      AnscFreeMemory(pSubCosaContext);
                      return ANSC_STATUS_FAILURE;
                    }

                    /* TODO: Set InstanceNumber Alias back */
                    CosaDmlRaIfSetOptionValues
                    (
                        NULL, 
                        pRAInterface->Cfg.InstanceNumber, 
                        ulSubIndex, 
                        pRAOption->InstanceNumber,
                        pRAOption->Alias
                    );
                }                

                pSubCosaContext->hContext     = (ANSC_HANDLE)pRAOption;
                pSubCosaContext->hParentTable = (ANSC_HANDLE)pRAInterface;
                pSubCosaContext->bNew         = FALSE;

                CosaSListPushEntryByInsNum(&pRAInterface->OptionList, pSubCosaContext);

            }
        }

    }

    CosaRaRegGetInfo((ANSC_HANDLE)pMyObject);

    EXIT:
        
    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaRARemove
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
CosaRARemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_RA              pMyObject       = (PCOSA_DATAMODEL_RA)hThisObject;
    PSLIST_HEADER                   pRAIFHead       = (PSLIST_HEADER)&pMyObject->InterfaceList;
    PSLIST_HEADER                   pRAOptionHead       = (PSLIST_HEADER)NULL;
    PCOSA_DML_RA_IF_FULL2           pRAInterface    = (PCOSA_DML_RA_IF_FULL2)NULL;
    PCOSA_DML_RA_OPTION             pRAOption       = (PCOSA_DML_RA_OPTION)NULL;
    PSINGLE_LINK_ENTRY              pLink           = (PSINGLE_LINK_ENTRY )NULL;
    PSINGLE_LINK_ENTRY              pSubLink        = (PSINGLE_LINK_ENTRY )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoTemp = (PPOAM_IREP_FOLDER_OBJECT)NULL;

    /*free the back end memory*/
    CosaDmlRaIfBERemove();

    /* RA Interface List */
    pLink = AnscSListGetFirstEntry(pRAIFHead);

    while ( pLink )
    {
        pCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        pLink   = AnscSListGetNextEntry(pLink);

        pRAInterface = (PCOSA_DML_RA_IF_FULL2)pCosaContext->hContext;

        if ( pRAInterface )
        {
            AnscSListPopEntryByLink(pRAIFHead, &pCosaContext->Linkage);

            pRAOptionHead    = &pRAInterface->OptionList;

            /* Option List */            
            pSubLink = AnscSListGetFirstEntry(pRAOptionHead);
        
            while ( pSubLink )
            {
                pSubCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSubLink);
                
                pSubLink  = AnscSListGetNextEntry(pSubLink);
            
                pRAOption = (PCOSA_DML_RA_OPTION)pSubCosaContext->hContext;
            
                AnscSListPopEntryByLink(pRAOptionHead, &pSubCosaContext->Linkage);
            
                if ( pRAOption )
                {
                    AnscFreeMemory(pRAOption);
                }
            
                AnscFreeMemory(pSubCosaContext);
            }

            AnscFreeMemory(pRAInterface);
            AnscFreeMemory(pCosaContext);
        }
    }


    /* Remove self */
    pPoamIrepFoTemp = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderRAInterfaceHA;
    if ( pPoamIrepFoTemp )
    {
        pPoamIrepFoTemp->Remove(pPoamIrepFoTemp);
    }

    
    pPoamIrepFoTemp = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderRAInterface;
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
        CosaRaRegGetInfo
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve RA intreface parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CosaRaRegGetInfo

    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_RA              pMyObject       = (PCOSA_DATAMODEL_RA)hThisObject;
    PSLIST_HEADER                   pRAIFHead       = (PSLIST_HEADER)&pMyObject->InterfaceList;
    PCOSA_DML_RA_IF_FULL2           pRAInterface    = (PCOSA_DML_RA_IF_FULL2)NULL;
    PCOSA_DML_RA_IF_FULL2           pRAEntry        = (PCOSA_DML_RA_IF_FULL2)NULL;
    PCOSA_DML_RA_OPTION             pRAOption       = (PCOSA_DML_RA_OPTION)NULL;
    PSINGLE_LINK_ENTRY              pLink           = (PSINGLE_LINK_ENTRY )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoRAHA = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderRAInterfaceHA;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoRAIF = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PSLAP_VARIABLE                  pSlapVariable   = (PSLAP_VARIABLE          )NULL;
    ULONG                           ulEntryCount            = 0;
    ULONG                           ulIndex                 = 0;
    ULONG                           ulInstanceNumber        = 0;
    ULONG                           ulUpperInstanceNumber   = 0;
    char*                           pFolderName             = NULL;
    char*                           pAlias                  = NULL;

    if ( !pPoamIrepFoRAHA )
    {
        return ANSC_STATUS_FAILURE;
    }

    /* Load the newly added but not yet commited entries */

    ulEntryCount = pPoamIrepFoRAHA->GetFolderCount((ANSC_HANDLE)pPoamIrepFoRAHA);

    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        pFolderName = pPoamIrepFoRAHA->EnumFolder
            (
                (ANSC_HANDLE)pPoamIrepFoRAHA,
                ulIndex
            );

        if ( !pFolderName )
        {
            continue;
        }

        pPoamIrepFoRAIF = (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoRAHA->GetFolder
            (
                (ANSC_HANDLE)pPoamIrepFoRAHA, 
                pFolderName
            );

        if ( !pPoamIrepFoRAIF )
        {
            continue;
        }

        if ( _ansc_strstr(pFolderName, COSA_DML_RR_NAME_RAIF_Prefix) )
        {

            pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoRAIF->GetRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoRAIF,
                    COSA_DML_RR_NAME_RAIF_InsNum,
                    NULL
                );

            if ( pSlapVariable )
            {
                ulInstanceNumber = pSlapVariable->Variant.varUint32;
                SlapFreeVariable(pSlapVariable);
            }
            


            pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoRAIF->GetRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoRAIF,
                    COSA_DML_RR_NAME_RAIF_Alias,
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

            pRAInterface = (PCOSA_DML_RA_IF_FULL2)AnscAllocateMemory(sizeof(COSA_DML_RA_IF_FULL2));

            if ( !pRAInterface )
            {
                AnscFreeMemory(pCosaContext);
                
                returnStatus = ANSC_STATUS_RESOURCES;
                
                goto EXIT;
            }

            AnscSListInitializeHeader(&pRAInterface->OptionList);
            
            pRAInterface->ulNextOptionInsNum    = 1;
            
            pRAInterface->Cfg.InstanceNumber = ulInstanceNumber;
            AnscCopyString(pRAInterface->Cfg.Alias, pAlias);

            pCosaContext->InstanceNumber   = ulInstanceNumber;
            pCosaContext->bNew             = TRUE;
            pCosaContext->hContext         = (ANSC_HANDLE)pRAInterface;
            pCosaContext->hParentTable     = NULL;
            pCosaContext->hPoamIrepUpperFo = (ANSC_HANDLE)pPoamIrepFoRAHA;
            pCosaContext->hPoamIrepFo      = (ANSC_HANDLE)pPoamIrepFoRAIF;
            
            CosaSListPushEntryByInsNum(pRAIFHead, pCosaContext);
        }
        else
        {
            pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoRAIF->GetRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoRAIF,
                    COSA_DML_RR_NAME_RAIF_Upper_InsNum,
                    NULL
                );

            if ( pSlapVariable )
            {
                ulUpperInstanceNumber = pSlapVariable->Variant.varUint32;
                    
                SlapFreeVariable(pSlapVariable);
            }

            if ( ulUpperInstanceNumber != 0 )
            {
                pLink = AnscSListGetFirstEntry(pRAIFHead);

                while ( pLink )
                {
                    pCosaContext  = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
                    pLink   = AnscSListGetNextEntry(pLink);

                    pRAEntry = (PCOSA_DML_RA_IF_FULL2)pCosaContext->hContext;

                    if ( pRAEntry && (pRAEntry->Cfg.InstanceNumber == ulUpperInstanceNumber) )
                    {
                        break;
                    }
                }

                if ( !pRAEntry )
                {
                    returnStatus = ANSC_STATUS_FAILURE;

                    goto EXIT;
                }
            }
            else
            {
                returnStatus = ANSC_STATUS_FAILURE;

                goto EXIT;
            }


            pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoRAIF->GetRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoRAIF,
                    COSA_DML_RR_NAME_RAIF_InsNum,
                    NULL
                );
            
            if ( pSlapVariable )
            {
                ulInstanceNumber = pSlapVariable->Variant.varUint32;            

                SlapFreeVariable(pSlapVariable);
            }
            

            pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoRAIF->GetRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoRAIF,
                    COSA_DML_RR_NAME_RAIF_Alias,
                    NULL
                );
            
            if ( pSlapVariable )
            {
                pAlias = AnscCloneString(pSlapVariable->Variant.varString);            

                SlapFreeVariable(pSlapVariable);
            }
            
            if ( _ansc_strstr(pFolderName, COSA_DML_RR_NAME_RAIF_OPTION_Prefix) )
            {
                pRAOption = (PCOSA_DML_RA_OPTION)AnscAllocateMemory(sizeof(COSA_DML_RA_OPTION));

                if ( !pRAOption )
                {
                    returnStatus = ANSC_STATUS_RESOURCES;

                    goto EXIT;
                }

                pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

                if ( !pSubCosaContext )
                {
                    AnscFreeMemory(pRAOption);            

                    returnStatus = ANSC_STATUS_RESOURCES;

                    goto EXIT;
                }

                pRAOption->InstanceNumber = ulInstanceNumber;
                AnscCopyString(pRAOption->Alias, pAlias);

                pSubCosaContext->InstanceNumber  = ulInstanceNumber;
                pSubCosaContext->hContext        = (ANSC_HANDLE)pRAOption;
                pSubCosaContext->hParentTable    = (ANSC_HANDLE)pRAEntry;
                pSubCosaContext->bNew            = TRUE;
                pSubCosaContext->hPoamIrepUpperFo = (ANSC_HANDLE)pPoamIrepFoRAHA;
                pSubCosaContext->hPoamIrepFo      = (ANSC_HANDLE)pPoamIrepFoRAIF;

                CosaSListPushEntryByInsNum(&pRAEntry->OptionList, pSubCosaContext);
            }

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
        CosaRARegAddInfo
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

        This function is called to retrieve RA intreface parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CosaRARegAddInfo
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
    PCOSA_DATAMODEL_RA              pMyObject         = (PCOSA_DATAMODEL_RA )hThisObject;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hCosaContext;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoRAIF   = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderRAInterface;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoUpper  = (PPOAM_IREP_FOLDER_OBJECT )pCosaContext->hPoamIrepUpperFo;  
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFo       = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PSLAP_VARIABLE                  pSlapVariable     = (PSLAP_VARIABLE           )NULL;
    char                            FolderName[32]    = {0};
    errno_t                         rc                = -1;

    if ( !pPoamIrepFoRAIF || !pPoamIrepFoUpper )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pPoamIrepFoRAIF->EnableFileSync((ANSC_HANDLE)pPoamIrepFoRAIF, FALSE);
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
      returnStatus = ANSC_STATUS_FAILURE;
      goto  EXIT1;
    }

    if ( TRUE )
    {
        pPoamIrepFo = (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoRAIF->GetFolder
                        (
                            (ANSC_HANDLE)pPoamIrepFoRAIF,
                            FolderName
                        );

        if ( !pPoamIrepFo )
        {
            pPoamIrepFo = pPoamIrepFoRAIF->AddFolder
                            (
                                (ANSC_HANDLE)pPoamIrepFoRAIF,
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
          returnStatus = ANSC_STATUS_FAILURE;
          goto  EXIT1;
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
                                COSA_DML_RR_NAME_RAIF_Upper_InsNum,
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
                                COSA_DML_RR_NAME_RAIF_InsNum,
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
                                COSA_DML_RR_NAME_RAIF_Alias,
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

    pPoamIrepFoRAIF->EnableFileSync((ANSC_HANDLE)pPoamIrepFoRAIF, TRUE);

    return returnStatus;
}



/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        CosaRARegDelInfo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCosaContext
            );

    description:

        This function is called to retrieve RA intreface parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                    hCosaContext

    return:     status of operation.

**********************************************************************/


ANSC_STATUS
CosaRARegDelInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hCosaContext);
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



