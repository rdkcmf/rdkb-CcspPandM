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

    module: cosa_bridging_internal.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaBridgingCreate
        *  CosaBridgingInitialize
        *  CosaBridgingRemove
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
#include "plugin_main_apis.h"
#include "cosa_bridging_dml.h"
#include "cosa_bridging_apis.h"
#include "cosa_bridging_internal.h"

extern void * g_pDslhDmlAgent;

/*
    this gloable var are test values
*/

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        CosaBridgingCreate
            (
            );

    description:

        This function constructs cosa nat object and return handle.

    argument:  

    return:     newly created nat object.

**********************************************************************/

ANSC_HANDLE
CosaBridgingCreate
    (
        VOID
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_BRIDGING        pMyObject    = (PCOSA_DATAMODEL_BRIDGING)NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PCOSA_DATAMODEL_BRIDGING)AnscAllocateMemory(sizeof(COSA_DATAMODEL_BRIDGING));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->Oid               = COSA_DATAMODEL_BRIDGING_OID;
    pMyObject->Create            = CosaBridgingCreate;
    pMyObject->Remove            = CosaBridgingRemove;
    pMyObject->Initialize        = CosaBridgingInitialize;

    pMyObject->Initialize   ((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaBridgingInitialize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate  cosa nat object and return handle.

    argument:	ANSC_HANDLE                 hThisObject
            This handle is actually the pointer of this object
            itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaBridgingInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_BRIDGING        pMyObject           = (PCOSA_DATAMODEL_BRIDGING)hThisObject;
    PSLAP_VARIABLE                  pSlapVariable       = (PSLAP_VARIABLE             )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoCOSA     = (PPOAM_IREP_FOLDER_OBJECT   )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoBRG      = (PPOAM_IREP_FOLDER_OBJECT   )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoBRGHA    = (PPOAM_IREP_FOLDER_OBJECT   )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoNextIns  = (PPOAM_IREP_FOLDER_OBJECT   )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext        = (PCOSA_CONTEXT_LINK_OBJECT  )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2       = (PCOSA_CONTEXT_LINK_OBJECT  )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext3       = (PCOSA_CONTEXT_LINK_OBJECT  )NULL;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge          = (PCOSA_DML_BRG_FULL_ALL     )NULL;
    PCOSA_DML_BRG_PORT_FULL         pPort               = (PCOSA_DML_BRG_PORT_FULL    )NULL;
    ULONG                           ulBridgeCount       = 0;
    ULONG                           ulSubCount          = 0;
    ULONG                           ulIndex             = 0;
    ULONG                           ulSubIndex          = 0;
    char                            FolderName[32]      = {0};
    PCOSA_DML_BRG_VLAN_FULL         pVLAN           = (PCOSA_DML_BRG_VLAN_FULL  )NULL;

    /* Initiation all functions */

//$HL 4/24/2013
    pMyObject->ulNextBridgeInstance = 1;

	AnscSListInitializeHeader(&pMyObject->BridgeList);

    //$HL 4/25/2013
    pMyObject->BridgeList.Depth = 0;
    
    pMyObject->hIrepFolderCOSA = g_GetRegistryRootFolder(g_pDslhDmlAgent);

    pPoamIrepFoCOSA = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderCOSA;

    if ( !pPoamIrepFoCOSA )
    {
        returnStatus = ANSC_STATUS_FAILURE;

        goto EXIT;
    }

    pPoamIrepFoBRG =
        (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoCOSA->GetFolder
            (
			    (ANSC_HANDLE)pPoamIrepFoCOSA,
                COSA_IREP_FOLDER_NAME_BRIDGING
            );

    if ( !pPoamIrepFoBRG )
    {
        pPoamIrepFoBRG =
            pPoamIrepFoCOSA->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoCOSA,
                    COSA_IREP_FOLDER_NAME_BRIDGING,
                    0
                );
    }

    if ( !pPoamIrepFoBRG )
    {
        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT;
    }
    else
    {
        pMyObject->hIrepFolderBRG = (ANSC_HANDLE)pPoamIrepFoBRG;
    }

    pPoamIrepFoBRGHA = 
        (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoBRG->GetFolder
            (
                (ANSC_HANDLE)pPoamIrepFoCOSA,
                COSA_IREP_FOLDER_NAME_BRIDGINGHA
            );

    if ( !pPoamIrepFoBRGHA )
    {
        pPoamIrepFoBRGHA =
            pPoamIrepFoBRG->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoBRG,
                    COSA_IREP_FOLDER_NAME_BRIDGINGHA,
                    0
                );
    }

    if ( !pPoamIrepFoBRGHA )
    {
        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT;
    }
    else
    {
        pMyObject->hIrepFolderBRGHA = (ANSC_HANDLE)pPoamIrepFoBRGHA;
    }

    if ( TRUE )
    {
        _ansc_sprintf
        (
            FolderName, 
            "%s%d", 
            COSA_DML_RR_NAME_Bridge_NextInsNunmber,
            0
        );
        
        pPoamIrepFoNextIns = (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoBRG->GetFolder
                                (
                                    (ANSC_HANDLE)pPoamIrepFoBRG,
                                    FolderName
                                );

        AnscZeroMemory(FolderName, 32);

        if ( pPoamIrepFoNextIns )
        {
            pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoNextIns->GetRecord
                                (
                                    (ANSC_HANDLE)pPoamIrepFoNextIns,
                                    COSA_DML_RR_NAME_Bridge_NextInsNunmber,
                                    NULL
                                );

            if ( pSlapVariable )
            {
                pMyObject->ulNextBridgeInstance = pSlapVariable->Variant.varUint32;
                
                SlapFreeVariable(pSlapVariable);
            }

            pPoamIrepFoNextIns->Remove((ANSC_HANDLE)pPoamIrepFoNextIns);
            pPoamIrepFoNextIns = NULL;
        }
    }

    ulBridgeCount = CosaDmlBrgGetNumberOfEntries(NULL);

    for ( ulIndex = 0; ulIndex < ulBridgeCount; ulIndex++ )
    {
        pDmlBridge = (PCOSA_DML_BRG_FULL_ALL)AnscAllocateMemory(sizeof(COSA_DML_BRG_FULL_ALL));
        _ansc_memset(pDmlBridge, 0, sizeof(COSA_DML_BRG_FULL_ALL));
        if ( !pDmlBridge )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto EXIT;
        }

        AnscSListInitializeHeader(&pDmlBridge->PortList);
        AnscSListInitializeHeader(&pDmlBridge->VLANList);
        AnscSListInitializeHeader(&pDmlBridge->VLANPortList);

        pDmlBridge->ulNextPortInsNum = 1;
        pDmlBridge->ulNextVLANInsNum = 1;
        pDmlBridge->ulNextVLANPortInsNum = 1;

        //$HL 4/25/2013
        if (CosaDmlBrgGetEntry(NULL, ulIndex, (PCOSA_DML_BRG_FULL)pDmlBridge)!= ANSC_STATUS_SUCCESS)
        {
            AnscFreeMemory(pDmlBridge);
            break;    
        }
        else
        //if ( TRUE )
        {
            pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
            _ansc_memset(pCosaContext, 0, sizeof(COSA_CONTEXT_LINK_OBJECT));
            if ( !pCosaContext )
            {
                AnscFreeMemory(pDmlBridge);

                returnStatus = ANSC_STATUS_RESOURCES;

                AnscTraceFlow(("<HL>%s cannot allocate pCosaContext\n", __FUNCTION__));
                goto  EXIT;
            }
            
            if ( pDmlBridge->Cfg.InstanceNumber !=0 )
            {
                pCosaContext->InstanceNumber = pDmlBridge->Cfg.InstanceNumber;

                if ( pMyObject->ulNextBridgeInstance <= pDmlBridge->Cfg.InstanceNumber )
                {
                    pMyObject->ulNextBridgeInstance = pDmlBridge->Cfg.InstanceNumber + 1;

                    if ( pMyObject->ulNextBridgeInstance == 0 )
                    {
                        pMyObject->ulNextBridgeInstance = 1;
                    }
                }
            }
            else
            {
                pDmlBridge->Cfg.InstanceNumber = pCosaContext->InstanceNumber = pMyObject->ulNextBridgeInstance;

                pMyObject->ulNextBridgeInstance++;

                if ( pMyObject->ulNextBridgeInstance == 0)
                {
                    pMyObject->ulNextBridgeInstance = 1;
                }

                _ansc_sprintf(pDmlBridge->Cfg.Alias, "DmlBridge%d", pMyObject->ulNextBridgeInstance);

                CosaDmlBrgSetValues(NULL, ulIndex, pDmlBridge->Cfg.InstanceNumber, pDmlBridge->Cfg.Alias);
            }

            pCosaContext->hContext     = (ANSC_HANDLE)pDmlBridge;
            pCosaContext->hParentTable = NULL;
            pCosaContext->bNew         = FALSE;
            //$HL 4/29/2013
            //pCosaContext->hPoamIrepUpperFo = NULL;
            CosaSListPushEntryByInsNum(&pMyObject->BridgeList, pCosaContext);
        }

        /* Port */
        
        if ( TRUE )
        {
            _ansc_sprintf
                (
                    FolderName, 
                    "%s%d", 
                    COSA_DML_RR_NAME_Bridge_Port_NextInsNumber, 
                    pDmlBridge->Cfg.InstanceNumber
                );

            pPoamIrepFoNextIns = 
                (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoBRG->GetFolder
                    (
                        (ANSC_HANDLE)pPoamIrepFoBRG,
                        FolderName
                    );

            if ( pPoamIrepFoNextIns )
            {
                pSlapVariable =
                    (PSLAP_VARIABLE)pPoamIrepFoNextIns->GetRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoNextIns,
                            COSA_DML_RR_NAME_Bridge_Port_NextInsNumber,
                            NULL
                        );

                if ( pSlapVariable )
                {
                    
                    pDmlBridge->ulNextPortInsNum = pSlapVariable->Variant.varUint32;
                    SlapFreeVariable(pSlapVariable);
                }
                else
                {
                    AnscTraceFlow(("<HL>%s pSlapVariable == NULL\n", __FUNCTION__));
                }

                pPoamIrepFoNextIns->Remove((ANSC_HANDLE)pPoamIrepFoNextIns);
                pPoamIrepFoNextIns = NULL;
            }
            else
            {
                AnscTraceFlow(("<HL>%s pPoamIrepFoNextIns == NULL\n", __FUNCTION__));
            }            
        }

        ulSubCount = CosaDmlBrgPortGetNumberOfEntries(NULL, pDmlBridge->Cfg.InstanceNumber);

        for ( ulSubIndex = 0; ulSubIndex < ulSubCount; ulSubIndex++ )
        {
            pPort = (PCOSA_DML_BRG_PORT_FULL)AnscAllocateMemory(sizeof(COSA_DML_BRG_PORT_FULL));

            if ( !pPort )
            {
                returnStatus = ANSC_STATUS_RESOURCES;

                goto EXIT;
            }

            _ansc_memset(pPort, 0, sizeof(COSA_DML_BRG_PORT_FULL));

            if(CosaDmlBrgPortGetEntry(NULL, pDmlBridge->Cfg.InstanceNumber, ulSubIndex, pPort)
                != ANSC_STATUS_SUCCESS)
            {
                AnscFreeMemory(pPort);
                break;
            }
            else 
            {
                pCosaContext2 = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

                if ( !pCosaContext2 )
                {  
                    AnscFreeMemory(pPort);
                    returnStatus = ANSC_STATUS_RESOURCES;
                    AnscTraceFlow(("<$HL> %s bridge instance:%d cannot allocate any memory for pCosaContext2\n",__FUNCTION__,
                        pDmlBridge->Cfg.InstanceNumber));
                    AnscFreeMemory(pPort);
                    goto EXIT;
                }

                _ansc_memset(pCosaContext2, 0, sizeof(COSA_CONTEXT_LINK_OBJECT));

                if ( pPort->Cfg.InstanceNumber != 0 )
                {
                    pCosaContext2->InstanceNumber = pPort->Cfg.InstanceNumber;

                    if ( pDmlBridge->ulNextPortInsNum <= pPort->Cfg.InstanceNumber )
                    {
                        pDmlBridge->ulNextPortInsNum = pPort->Cfg.InstanceNumber + 1;

                        if ( pDmlBridge->ulNextPortInsNum == 0 )
                        {
                            pDmlBridge->ulNextPortInsNum = 1;
                        }
                    }
                }
                else
                {
                    pCosaContext2->InstanceNumber = pPort->Cfg.InstanceNumber = pDmlBridge->ulNextPortInsNum;

                    pDmlBridge->ulNextPortInsNum++;

                    if ( pDmlBridge->ulNextPortInsNum == 0 )
                    {
                        pDmlBridge->ulNextPortInsNum = 1;
                    }

                    _ansc_sprintf(pPort->Cfg.Alias, "Port%d", pCosaContext2->InstanceNumber);

                    CosaDmlBrgPortSetValues
                        (
                            NULL,
                            pDmlBridge->Cfg.InstanceNumber,
                            ulSubIndex,
                            pPort->Cfg.InstanceNumber,
                            pPort->Cfg.Alias
                        );
                }

                pCosaContext2->hContext     = (ANSC_HANDLE)pPort;
                pCosaContext2->hParentTable = (ANSC_HANDLE)pDmlBridge;
                pCosaContext2->bNew         = FALSE;

                CosaSListPushEntryByInsNum(&pDmlBridge->PortList, pCosaContext2);
            }
        }
    

        //$HL 7/3/2013
        /* VLAN */
        
        if ( TRUE )
        {
            _ansc_sprintf
                (
                    FolderName, 
                    "%s%d", 
                    COSA_DML_RR_NAME_Bridge_VLAN_NextInsNumber, 
                    pDmlBridge->Cfg.InstanceNumber
                );

            pPoamIrepFoNextIns = 
                (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoBRG->GetFolder
                    (
                        (ANSC_HANDLE)pPoamIrepFoBRG,
                        FolderName
                    );

            if ( pPoamIrepFoNextIns )
            {
                pSlapVariable =
                    (PSLAP_VARIABLE)pPoamIrepFoNextIns->GetRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoNextIns,
                            COSA_DML_RR_NAME_Bridge_VLAN_NextInsNumber,
                            NULL
                        );

                if ( pSlapVariable )
                {
                    
                    pDmlBridge->ulNextVLANInsNum = pSlapVariable->Variant.varUint32;
                    SlapFreeVariable(pSlapVariable);
                }
                else
                {
                    AnscTraceFlow(("<HL>%s pSlapVariable == NULL\n", __FUNCTION__));
                }

                pPoamIrepFoNextIns->Remove((ANSC_HANDLE)pPoamIrepFoNextIns);
                pPoamIrepFoNextIns = NULL;
            }
            else
            {
                AnscTraceFlow(("<HL>%s pPoamIrepFoNextIns == NULL\n", __FUNCTION__));
            }            
        }

        ulSubCount = CosaDmlBrgVlanGetNumberOfEntries(NULL, pDmlBridge->Cfg.InstanceNumber);

        for ( ulSubIndex = 0; ulSubIndex < ulSubCount; ulSubIndex++ )
        {
            pVLAN = (PCOSA_DML_BRG_VLAN_FULL)AnscAllocateMemory(sizeof(COSA_DML_BRG_VLAN_FULL));

            if ( !pVLAN )
            {
                returnStatus = ANSC_STATUS_RESOURCES;

                goto EXIT;
            }

            _ansc_memset(pVLAN, 0, sizeof(COSA_DML_BRG_VLAN_FULL));

            if(CosaDmlBrgVlanGetEntry(NULL, pDmlBridge->Cfg.InstanceNumber, ulSubIndex, pVLAN)
                != ANSC_STATUS_SUCCESS)
            {
                AnscFreeMemory(pVLAN);
                break;
            }
            else 
            {
                pCosaContext3 = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

                if ( !pCosaContext3 )
                {  
                    returnStatus = ANSC_STATUS_RESOURCES;
                    AnscTraceFlow(("<$HL> %s bridge instance:%d cannot allocate any memory for pCosaContext3\n",__FUNCTION__,
                        pDmlBridge->Cfg.InstanceNumber));
                    AnscFreeMemory(pVLAN);
                    goto EXIT;
                }

                _ansc_memset(pCosaContext3, 0, sizeof(COSA_CONTEXT_LINK_OBJECT));

                if ( pVLAN->Cfg.InstanceNumber != 0 )
                {
                    pCosaContext3->InstanceNumber = pVLAN->Cfg.InstanceNumber;

                    if ( pDmlBridge->ulNextVLANInsNum <= pVLAN->Cfg.InstanceNumber )
                    {
                        pDmlBridge->ulNextVLANInsNum = pVLAN->Cfg.InstanceNumber + 1;

                        if ( pDmlBridge->ulNextVLANInsNum == 0 )
                        {
                            pDmlBridge->ulNextVLANInsNum = 1;
                        }
                    }
                }
                else
                {
                    pCosaContext2->InstanceNumber = pVLAN->Cfg.InstanceNumber = pDmlBridge->ulNextPortInsNum;

                    pDmlBridge->ulNextVLANInsNum++;

                    if ( pDmlBridge->ulNextVLANInsNum == 0 )
                    {
                        pDmlBridge->ulNextVLANInsNum = 1;
                    }

                    _ansc_sprintf(pVLAN->Cfg.Alias, "VLAN%d", pCosaContext2->InstanceNumber);

                    CosaDmlBrgVlanSetValues //new sbAPI
                        (
                            NULL,
                            pDmlBridge->Cfg.InstanceNumber,
                            ulSubIndex,
                            pVLAN->Cfg.InstanceNumber,
                            pVLAN->Cfg.Alias
                        );
                }

                pCosaContext3->hContext     = (ANSC_HANDLE)pVLAN;
                pCosaContext3->hParentTable = (ANSC_HANDLE)pDmlBridge;
                pCosaContext3->bNew         = FALSE;

                CosaSListPushEntryByInsNum(&pDmlBridge->VLANList, pCosaContext3);
            }
        }
    }
    //$HL 4/29/2013
    CosaBridgingRegGetInfo((ANSC_HANDLE)pMyObject);

EXIT:

    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaBridgingRemove
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
CosaBridgingRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_BRIDGING        pMyObject         = (PCOSA_DATAMODEL_BRIDGING   )hThisObject;
    PSLIST_HEADER                   pBridgeHead       = (PSLIST_HEADER              )&pMyObject->BridgeList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2     = (PCOSA_CONTEXT_LINK_OBJECT  )NULL;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge        = (PCOSA_DML_BRG_FULL_ALL     )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY         )NULL;
    PSLIST_HEADER                   pListHead         = (PSLIST_HEADER              )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry2      = (PSINGLE_LINK_ENTRY         )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext3     = (PCOSA_CONTEXT_LINK_OBJECT  )NULL;
    PCOSA_DML_BRG_PORT_FULL         pPort             = (PCOSA_DML_BRG_PORT_FULL    )NULL;

    pSLinkEntry = AnscSListGetFirstEntry(pBridgeHead);

    while ( pSLinkEntry )
    {
        pCosaContext2 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        pSLinkEntry   = AnscSListGetNextEntry(pSLinkEntry);

        pDmlBridge = (PCOSA_DML_BRG_FULL_ALL)pCosaContext2->hContext;

        if ( pDmlBridge )
        {
            AnscSListPopEntryByLink(pBridgeHead, &pCosaContext2->Linkage);

            pListHead = &pDmlBridge->PortList;

            pSLinkEntry2 = AnscSListGetFirstEntry(pListHead);

            while ( pSLinkEntry2 )
            {
                pCosaContext3 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry2);
                pSLinkEntry2 = AnscSListGetNextEntry(pSLinkEntry2);

                pPort = (PCOSA_DML_BRG_PORT_FULL)pCosaContext3->hContext;

                AnscSListPopEntryByLink(pListHead, &pCosaContext3->Linkage);

                if ( pPort )
                {
                    AnscFreeMemory(pPort);
                }

                AnscFreeMemory(pCosaContext3);
            }

            AnscFreeMemory(pDmlBridge);
            AnscFreeMemory(pCosaContext2);
        }
    }

    AnscFreeMemory((ANSC_HANDLE)pMyObject);

    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        CosaBridgingRegGetInfo
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve Dslm policy parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hDdnsInfo
                Specifies the Dslm policy parameters to be filled.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CosaBridgingRegGetInfo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus            = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_BRIDGING        pMyObject               = (PCOSA_DATAMODEL_BRIDGING    )hThisObject;
    PSLIST_HEADER                   pBridgeHead             = (PSLIST_HEADER               )&pMyObject->BridgeList;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoBRGHA        = (PPOAM_IREP_FOLDER_OBJECT    )pMyObject->hIrepFolderBRGHA;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoBRGBrg       = (PPOAM_IREP_FOLDER_OBJECT    )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT   )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2           = (PCOSA_CONTEXT_LINK_OBJECT   )NULL;
    PCOSA_DML_BRG_FULL_ALL          pDmlBridge              = (PCOSA_DML_BRG_FULL_ALL      )NULL;
    PCOSA_DML_BRG_FULL_ALL          pDmlBrgEntry            = (PCOSA_DML_BRG_FULL_ALL      )NULL;
    PCOSA_DML_BRG_PORT_FULL         pPort                   = (PCOSA_DML_BRG_PORT_FULL     )NULL;
    PSLAP_VARIABLE                  pSlapVariable           = (PSLAP_VARIABLE              )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry             = (PSINGLE_LINK_ENTRY          )NULL;
    ULONG                           ulEntryCount            = 0;
    ULONG                           ulIndex                 = 0;
    ULONG                           ulInstanceNumber        = 0;
    ULONG                           ulUpperInstanceNumber   = 0;
    char*                           pFolderName             = NULL;
    char*                           pBridgeName             = NULL;
    char*                           pAlias                  = NULL;
    BOOLEAN                         found = FALSE;
    if ( !pPoamIrepFoBRGHA )
    {
        return ANSC_STATUS_FAILURE;
    }

    /* Load the newly added but not yet commited entries */

    ulEntryCount = pPoamIrepFoBRGHA->GetFolderCount((ANSC_HANDLE)pPoamIrepFoBRGHA);

    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        pFolderName =
            pPoamIrepFoBRGHA->EnumFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoBRGHA,
                    ulIndex
                );

        if ( !pFolderName )
        {
            continue;
        }

        pPoamIrepFoBRGBrg = pPoamIrepFoBRGHA->GetFolder((ANSC_HANDLE)pPoamIrepFoBRGHA, pFolderName);

        if ( !pPoamIrepFoBRGBrg )
        {
            continue;
        }


        if ( _ansc_strstr(pFolderName, "Bridge") )
        {
            if ( TRUE )
            {
                pSlapVariable =
                    (PSLAP_VARIABLE)pPoamIrepFoBRGBrg->GetRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoBRGBrg,
                            COSA_DML_RR_NAME_Bridge_InsNum,
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
                    (PSLAP_VARIABLE)pPoamIrepFoBRGBrg->GetRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoBRGBrg,
                            COSA_DML_RR_NAME_Bridge_Alias,
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
                returnStatus = ANSC_STATUS_RESOURCES;

                goto EXIT;
            }

            pDmlBridge = (PCOSA_DML_BRG_FULL_ALL)AnscAllocateMemory(sizeof(COSA_DML_BRG_FULL_ALL));

            if ( !pDmlBridge )
            {
                AnscFreeMemory(pCosaContext);

                returnStatus = ANSC_STATUS_RESOURCES;

                goto EXIT;
            }

            AnscSListInitializeHeader(&pDmlBridge->PortList   );
            AnscSListInitializeHeader(&pDmlBridge->VLANList   );
            AnscSListInitializeHeader(&pDmlBridge->VLANPortList);

            pDmlBridge->ulNextPortInsNum = 1;
            pDmlBridge->ulNextVLANInsNum = 1;
            pDmlBridge->ulNextVLANPortInsNum = 1;
            pDmlBridge->Cfg.bAllowDelete = TRUE;
            pDmlBridge->Cfg.InstanceNumber = ulInstanceNumber;
            AnscCopyString(pDmlBridge->Cfg.Alias, pAlias);

            pCosaContext->InstanceNumber   = ulInstanceNumber;
            pCosaContext->bNew             = TRUE;
            pCosaContext->hContext         = (ANSC_HANDLE)pDmlBridge;
            pCosaContext->hParentTable     = NULL;
            pCosaContext->hPoamIrepUpperFo = (ANSC_HANDLE)pPoamIrepFoBRGHA;
            pCosaContext->hPoamIrepFo      = (ANSC_HANDLE)pPoamIrepFoBRGBrg;
            
            CosaSListPushEntryByInsNum(pBridgeHead, pCosaContext);            
        }
        else
        {
            found = FALSE;
            if ( TRUE )
            {
                pSlapVariable =
                    (PSLAP_VARIABLE)pPoamIrepFoBRGBrg->GetRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoBRGBrg,
                            COSA_DML_RR_NAME_Bridge_Upper_InsNum,
                            NULL
                        );

                if ( pSlapVariable )
                {
                    ulUpperInstanceNumber = pSlapVariable->Variant.varUint32;
                    SlapFreeVariable(pSlapVariable);
                }

                if ( ulUpperInstanceNumber != 0 )
                {
                    pSLinkEntry = AnscSListGetFirstEntry(pBridgeHead);

                    while ( pSLinkEntry )
                    {
                        pCosaContext  = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
                        pSLinkEntry   = AnscSListGetNextEntry(pSLinkEntry);

                        pDmlBrgEntry  = (PCOSA_DML_BRG_FULL_ALL)pCosaContext->hContext;

                        if ( pDmlBrgEntry && (pDmlBrgEntry->Cfg.InstanceNumber == ulUpperInstanceNumber) )
                        {
                            found = TRUE;
                            break;
                        }
                    }
                }
                else
                {
                    return ANSC_STATUS_FAILURE;
                }
            }

            if (found==FALSE)
            {
                //$HL 4/29/2013
                pPoamIrepFoBRGHA->EnableFileSync((ANSC_HANDLE)pPoamIrepFoBRGHA, FALSE);
                pPoamIrepFoBRGBrg->Close((ANSC_HANDLE)pPoamIrepFoBRGBrg);
        
                pPoamIrepFoBRGHA->DelFolder
                    (
                        (ANSC_HANDLE)pPoamIrepFoBRGHA, 
                        pPoamIrepFoBRGBrg->GetFolderName((ANSC_HANDLE)pPoamIrepFoBRGBrg)
                    );

                pPoamIrepFoBRGHA->EnableFileSync((ANSC_HANDLE)pPoamIrepFoBRGHA, TRUE);

                AnscFreeMemory(pPoamIrepFoBRGBrg);
                continue;
            }
            if ( TRUE )
            {
                pSlapVariable =
                    (PSLAP_VARIABLE)pPoamIrepFoBRGBrg->GetRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoBRGBrg,
                            COSA_DML_RR_NAME_Bridge_InsNum,
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
                    (PSLAP_VARIABLE)pPoamIrepFoBRGBrg->GetRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoBRGBrg,
                            COSA_DML_RR_NAME_Bridge_Alias,
                            NULL
                        );

                if ( pSlapVariable )
                {
                    pAlias = AnscCloneString(pSlapVariable->Variant.varString);
                    SlapFreeVariable(pSlapVariable);
                }
            }

            if ( _ansc_strstr(pFolderName, "Port") )
            {
                pPort = (PCOSA_DML_BRG_PORT_FULL)AnscAllocateMemory(sizeof(COSA_DML_BRG_PORT_FULL));

                if ( !pPort )
                {
                    returnStatus = ANSC_STATUS_RESOURCES;

                    goto EXIT;
                }

                pCosaContext2 = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

                if ( !pCosaContext2 )
                {
                    AnscFreeMemory(pPort);
            
                    returnStatus = ANSC_STATUS_RESOURCES;

                    goto EXIT;
                }

                pPort->Cfg.InstanceNumber = ulInstanceNumber;
                AnscCopyString(pPort->Cfg.Alias, pAlias);
                pPort->Cfg.bAllowDelete = TRUE;
                pCosaContext2->InstanceNumber  = ulInstanceNumber;
                pCosaContext2->hContext        = (ANSC_HANDLE)pPort;
                pCosaContext2->hParentTable    = (ANSC_HANDLE)pDmlBrgEntry;
                pCosaContext2->bNew            = TRUE;
                pCosaContext2->hPoamIrepUpperFo = (ANSC_HANDLE)pPoamIrepFoBRGHA;
                pCosaContext2->hPoamIrepFo      = (ANSC_HANDLE)pPoamIrepFoBRGBrg;

                //$HL 5/1/2013
                pDmlBrgEntry->ulNextPortInsNum = ulInstanceNumber >=pDmlBrgEntry->ulNextPortInsNum ? 
                    ulInstanceNumber+1:pDmlBrgEntry->ulNextPortInsNum;
                CosaSListPushEntryByInsNum(&pDmlBrgEntry->PortList, pCosaContext2);
            }
        }
    }

EXIT:

    AnscFreeMemory(pAlias);

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        CosaBridgingRegAddInfo
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pNextInsNumName,
                ULONG                       ulNextInsNum,
                char*                       pPreffix,
                ULONG                       ulUpperInsNum,
                char*                       pAlias,
                ANSC_HANDLE                 hCosaContext
            )

    description:

        This function is called to configure Dslm policy parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hDdnsInfo
                Specifies the Dslm policy parameters to be filled.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CosaBridgingRegAddInfo
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
    PCOSA_DATAMODEL_BRIDGING        pMyObject         = (PCOSA_DATAMODEL_BRIDGING )hThisObject;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hCosaContext;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoBRG    = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderBRG;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoUpper  = (PPOAM_IREP_FOLDER_OBJECT )pCosaContext->hPoamIrepUpperFo;  
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFo       = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PSLAP_VARIABLE                  pSlapVariable     = (PSLAP_VARIABLE           )NULL;
    char                            FolderName[32]    = {0};

    if ( !pPoamIrepFoBRG || !pPoamIrepFoUpper )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pPoamIrepFoBRG->EnableFileSync((ANSC_HANDLE)pPoamIrepFoBRG, FALSE);
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

    _ansc_sprintf(FolderName, "%s%d", pNextInsNumName, ulUpperInsNum);

    if ( TRUE )
    {
        pPoamIrepFo = 
            pPoamIrepFoBRG->GetFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoBRG,
                    FolderName
                );

        if ( !pPoamIrepFo )
        {
            pPoamIrepFo = 
                pPoamIrepFoBRG->AddFolder
                    (
                        (ANSC_HANDLE)pPoamIrepFoBRG,
                        FolderName,
                        0
                    );
        }
    }

    if ( TRUE )
    {
        returnStatus = 
            pPoamIrepFo->DelRecord
                (
                    (ANSC_HANDLE)pPoamIrepFo,
                    pNextInsNumName
                );

        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
        pSlapVariable->Variant.varUint32 = ulNextInsNum;

        returnStatus =
            pPoamIrepFo->AddRecord
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
        _ansc_sprintf(FolderName, "%s%d%d", pPreffix, ulUpperInsNum, pCosaContext->InstanceNumber);

        pPoamIrepFo =
            pPoamIrepFoUpper->AddFolder
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

        if ( ulUpperInsNum != 0 )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
            pSlapVariable->Variant.varUint32 = ulUpperInsNum;

            returnStatus =
                pPoamIrepFo->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFo,
                        COSA_DML_RR_NAME_Bridge_Upper_InsNum,
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

            returnStatus =
                pPoamIrepFo->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFo,
                        COSA_DML_RR_NAME_Bridge_InsNum,
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

            returnStatus =
                pPoamIrepFo->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFo,
                        COSA_DML_RR_NAME_Bridge_Alias,
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

    pPoamIrepFoBRG->EnableFileSync((ANSC_HANDLE)pPoamIrepFoBRG, TRUE);

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        CosaBridgingRegDelInfo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCosaContext
            );

    description:

        This function is called to configure Dslm policy parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hDdnsInfo
                Specifies the Dslm policy parameters to be filled.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CosaBridgingRegDelInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_BRIDGING        pMyObject         = (PCOSA_DATAMODEL_BRIDGING )hThisObject;
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

