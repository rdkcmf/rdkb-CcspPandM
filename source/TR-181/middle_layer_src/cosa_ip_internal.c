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

    module: cosa_ip_internal.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaIPCreate
        *  CosaIPInitialize
        *  CosaIPRemove
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
#include "cosa_ip_apis.h"
#include "cosa_ip_internal.h"
#include "plugin_main_apis.h"
#include "safec_lib_common.h"

extern void * g_pDslhDmlAgent;

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        CosaIPCreate
            (
            );

    description:

        This function constructs cosa nat object and return handle.

    argument:  

    return:     newly created nat object.

**********************************************************************/

ANSC_HANDLE
CosaIPCreate
    (
        VOID
    )
{
    PCOSA_DATAMODEL_IP              pMyObject    = (PCOSA_DATAMODEL_IP)NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PCOSA_DATAMODEL_IP)AnscAllocateMemory(sizeof(COSA_DATAMODEL_IP));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->Oid               = COSA_DATAMODEL_IP_OID;
    pMyObject->Create            = CosaIPCreate;
    pMyObject->Remove            = CosaIPRemove;
    pMyObject->Initialize        = CosaIPInitialize;

    pMyObject->Initialize   ((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaIPInitialize
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
CosaIPInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_IP              pMyObject       = (PCOSA_DATAMODEL_IP)hThisObject;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoCOSA = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoIPIF = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoIPHA = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoNextIns = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PSLAP_VARIABLE                  pSlapVariable   = (PSLAP_VARIABLE          )NULL;
    PCOSA_DML_IP_IF_FULL2           pIPInterface    = (PCOSA_DML_IP_IF_FULL2   )NULL;
    PCOSA_DML_IP_V4ADDR             pIPv4Addr       = (PCOSA_DML_IP_V4ADDR     )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    char                            FolderName[32]  = {0};
    ULONG                           ulEntryCount    = 0;
    ULONG                           ulIndex         = 0;
    ULONG                           ulSubEntryCount = 0;
    ULONG                           ulSubIndex      = 0;
    errno_t                         rc              = -1;

    CosaDmlIpInit(NULL, &pMyObject->hSbContext);

    /* Initiation all functions */
    pMyObject->pActivePort                     = NULL;
    pMyObject->activePortCount                 = 0;
    pMyObject->ulInterfaceCount                = 0;
    pMyObject->ulNextInterfaceInsNum           = 1;
    
    AnscSListInitializeHeader(&pMyObject->InterfaceList)

    pMyObject->hIrepFolderCOSA = g_GetRegistryRootFolder(g_pDslhDmlAgent);

    pPoamIrepFoCOSA = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderCOSA;
    
    if ( !pPoamIrepFoCOSA )
    {
        returnStatus = ANSC_STATUS_FAILURE;
        
        goto  EXIT;
    }

    pPoamIrepFoIPIF = (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoCOSA->GetFolder
                        (
                            (ANSC_HANDLE)pPoamIrepFoCOSA,
                            COSA_IREP_FOLDER_NAME_IPIF
                        );

    if ( !pPoamIrepFoIPIF )
    {
        pPoamIrepFoIPIF = pPoamIrepFoCOSA->AddFolder
                            (
                                (ANSC_HANDLE)pPoamIrepFoCOSA,
                                COSA_IREP_FOLDER_NAME_IPIF,
                                0
                            );
    }

    if ( !pPoamIrepFoIPIF )
    {
        returnStatus = ANSC_STATUS_FAILURE;
        
        goto  EXIT;
    }
    else
    {        
        pMyObject->hIrepFolderIPInterface = (ANSC_HANDLE)pPoamIrepFoIPIF;
    }

    pPoamIrepFoIPHA = (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoIPIF->GetFolder
                        (
                            (ANSC_HANDLE)pPoamIrepFoCOSA,
                            COSA_IREP_FOLDER_NAME_IPIF_HA
                        );

    if ( !pPoamIrepFoIPHA )
    {
        pPoamIrepFoIPHA = pPoamIrepFoIPIF->AddFolder
                            (
                                (ANSC_HANDLE)pPoamIrepFoIPIF,
                                COSA_IREP_FOLDER_NAME_IPIF_HA,
                                0
                            );
    }

    if ( !pPoamIrepFoIPHA )
    {
        returnStatus = ANSC_STATUS_FAILURE;
        
        goto  EXIT;
    }
    else
    {
        pMyObject->hIrepFolderIPInterfaceHA = (ANSC_HANDLE)pPoamIrepFoIPHA;
    }

    /* Retrieve the next Instance Number for IP interface */
    
    if ( TRUE )
    {
        rc = sprintf_s
        (
            FolderName,
            sizeof(FolderName),
            "%s%d", 
            COSA_DML_RR_NAME_IPIF_NextInsNum,
            0
        );
        if(rc < EOK)
        {
          ERR_CHK(rc);
        }
        
        pPoamIrepFoNextIns = (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoIPIF->GetFolder
                                (
                                    (ANSC_HANDLE)pPoamIrepFoIPIF,
                                    FolderName
                                );

        AnscZeroMemory(FolderName, 32);

        if ( pPoamIrepFoNextIns )
        {
            pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoNextIns->GetRecord
                                (
                                    (ANSC_HANDLE)pPoamIrepFoNextIns,
                                    COSA_DML_RR_NAME_IPIF_NextInsNum,
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

    /* Initialize middle layer for Device.IP.Interface.{i}.  */

    ulEntryCount = CosaDmlIpIfGetNumberOfEntries(pMyObject->hSbContext);
    
    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        pIPInterface = (PCOSA_DML_IP_IF_FULL2)AnscAllocateMemory(sizeof(COSA_DML_IP_IF_FULL2));

        if ( !pIPInterface )
        {
            returnStatus = ANSC_STATUS_RESOURCES;
            
            goto  EXIT;
        }

        AnscSListInitializeHeader(&pIPInterface->IPV4List);
        AnscSListInitializeHeader(&pIPInterface->IPV6List);
        AnscSListInitializeHeader(&pIPInterface->IPV6PrefixList);
        
        pIPInterface->ulNextIPV4InsNum = 1;
        pIPInterface->ulNextIPV6InsNum = 1;
        pIPInterface->ulNextIPV6PreInsNum = 1;
                
        CosaDmlIpIfGetEntry(pMyObject->hSbContext, ulIndex,(PCOSA_DML_IP_IF_FULL)pIPInterface);

        if ( TRUE )
        {
            pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

            if ( !pCosaContext )
            {
                AnscFreeMemory(pIPInterface);               
                
                returnStatus = ANSC_STATUS_RESOURCES;
                
                goto  EXIT;
            }

            if ( 0 != pIPInterface->Cfg.InstanceNumber )
            {
                pCosaContext->InstanceNumber = pIPInterface->Cfg.InstanceNumber;
                
                if ( pMyObject->ulNextInterfaceInsNum <=  pIPInterface->Cfg.InstanceNumber )
                {
                    pMyObject->ulNextInterfaceInsNum =  pIPInterface->Cfg.InstanceNumber + 1;

                    if ( 0 == pMyObject->ulNextInterfaceInsNum )
                    {
                        pMyObject->ulNextInterfaceInsNum = 1;
                    }
                }
            }
            else
            {
                pCosaContext->InstanceNumber = pMyObject->ulNextInterfaceInsNum;  
                
                pIPInterface->Cfg.InstanceNumber = pCosaContext->InstanceNumber ;
                
                pMyObject->ulNextInterfaceInsNum++;
                
                if ( 0 == pMyObject->ulNextInterfaceInsNum )
                {
                    pMyObject->ulNextInterfaceInsNum = 1;
                }

                /* Generate Alias */
                rc = sprintf_s(pIPInterface->Cfg.Alias, sizeof(pIPInterface->Cfg.Alias),"Interface%lu", pMyObject->ulNextInterfaceInsNum);
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                }

                CosaDmlIpIfSetValues
                (
                    pMyObject->hSbContext,
                    ulIndex,
                    pIPInterface->Cfg.InstanceNumber,
                    pIPInterface->Cfg.Alias
                );
            }

            pCosaContext->hContext      = (ANSC_HANDLE)pIPInterface;            
            pCosaContext->hParentTable  = NULL;            
            pCosaContext->bNew          = FALSE;

            CosaSListPushEntryByInsNum(&pMyObject->InterfaceList, pCosaContext);
                  
        }

        /* Initialize middle layer for Device.IP.Interface.{i}.IPv4Address.{i}. */
        if ( TRUE )
        {
            rc = sprintf_s
            (
                FolderName,
                sizeof(FolderName),
                "%s%lu", 
                COSA_DML_RR_NAME_IPIF_IPv4_NextInsNum, 
                pIPInterface->Cfg.InstanceNumber
            );
            if(rc < EOK)
            {
                ERR_CHK(rc);
            }
            
            pPoamIrepFoNextIns = 
                (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoIPIF->GetFolder
                    (
                        (ANSC_HANDLE)pPoamIrepFoIPIF,
                        FolderName
                    );

            if ( pPoamIrepFoNextIns )
            {
                pSlapVariable =
                    (PSLAP_VARIABLE)pPoamIrepFoNextIns->GetRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoNextIns,
                            COSA_DML_RR_NAME_IPIF_IPv4_NextInsNum,
                            NULL
                        );

                if ( pSlapVariable )
                {
                    pIPInterface->ulNextIPV4InsNum = pSlapVariable->Variant.varUint32;
                    
                    SlapFreeVariable(pSlapVariable);
                }

                pPoamIrepFoNextIns->Remove((ANSC_HANDLE)pPoamIrepFoNextIns);
                pPoamIrepFoNextIns = NULL;
            } 
        }
        
        ulSubEntryCount = CosaDmlIpIfGetNumberOfV4Addrs(pMyObject->hSbContext, pIPInterface->Cfg.InstanceNumber);

        for ( ulSubIndex = 0; ulSubIndex < ulSubEntryCount; ulSubIndex++ )
        {
            pIPv4Addr = (PCOSA_DML_IP_V4ADDR)AnscAllocateMemory(sizeof(COSA_DML_IP_V4ADDR));
        
            if ( !pIPv4Addr )
            {
                returnStatus = ANSC_STATUS_RESOURCES;
                
                goto  EXIT;
            }
        
            CosaDmlIpIfGetV4Addr(pMyObject->hSbContext, pIPInterface->Cfg.InstanceNumber, ulSubIndex, pIPv4Addr);
        
            if ( TRUE )
            {
                pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

                if ( !pSubCosaContext )
                {
                    AnscFreeMemory(pIPv4Addr);

                    returnStatus = ANSC_STATUS_RESOURCES;
                    
                    goto  EXIT;  
                }

                if ( 0 != pIPv4Addr->InstanceNumber )
                {
                    pSubCosaContext->InstanceNumber = pIPv4Addr->InstanceNumber;

                    if ( pIPInterface->ulNextIPV4InsNum <= pIPv4Addr->InstanceNumber )
                    {
                        pIPInterface->ulNextIPV4InsNum = pIPv4Addr->InstanceNumber + 1;

                        if ( pIPInterface->ulNextIPV4InsNum == 0 )
                        {
                            pIPInterface->ulNextIPV4InsNum = 1;
                        }
                    }  
                }
                else
                {
                    pSubCosaContext->InstanceNumber = pIPv4Addr->InstanceNumber = pIPInterface->ulNextIPV4InsNum;

                    pIPInterface->ulNextIPV4InsNum++;

                    if ( pIPInterface->ulNextIPV4InsNum == 0 )
                    {
                        pIPInterface->ulNextIPV4InsNum = 1;
                    }

                    /* Generate Alias */
                    rc = sprintf_s(pIPv4Addr->Alias, sizeof(pIPv4Addr->Alias),"IPv4Address%lu", pSubCosaContext->InstanceNumber);
                    if(rc < EOK)
                    {
                      ERR_CHK(rc);
                    }

                    /* TODO: Set InstanceNumber Alias back */
                    CosaDmlIpIfSetV4AddrValues
                    (
                        pMyObject->hSbContext, 
                        pIPInterface->Cfg.InstanceNumber, 
                        ulSubIndex, 
                        pIPv4Addr->InstanceNumber,
                        pIPv4Addr->Alias
                    );
                }                

                pSubCosaContext->hContext     = (ANSC_HANDLE)pIPv4Addr;
                pSubCosaContext->hParentTable = (ANSC_HANDLE)pIPInterface;
                pSubCosaContext->bNew         = FALSE;

                CosaSListPushEntryByInsNum(&pIPInterface->IPV4List, pSubCosaContext);

            }
        }

        /* Initialize middle layer for Device.IP.Interface.{i}.IPv6Address.{i}. */
        if ( TRUE )
        {
            rc = sprintf_s
            (
                FolderName,
                sizeof(FolderName),
                "%s%lu", 
                COSA_DML_RR_NAME_IPIF_IPv6_NextInsNum, 
                pIPInterface->Cfg.InstanceNumber
            );
            if(rc < EOK)
            {
              ERR_CHK(rc);
            }
            
            pPoamIrepFoNextIns = 
                (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoIPIF->GetFolder
                    (
                        (ANSC_HANDLE)pPoamIrepFoIPIF,
                        FolderName
                    );

            if ( pPoamIrepFoNextIns )
            {
                pSlapVariable =
                    (PSLAP_VARIABLE)pPoamIrepFoNextIns->GetRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoNextIns,
                            COSA_DML_RR_NAME_IPIF_IPv6_NextInsNum,
                            NULL
                        );

                if ( pSlapVariable )
                {
                    pIPInterface->ulNextIPV6InsNum = pSlapVariable->Variant.varUint32;
                    
                    SlapFreeVariable(pSlapVariable);
                }

                pPoamIrepFoNextIns->Remove((ANSC_HANDLE)pPoamIrepFoNextIns);
                pPoamIrepFoNextIns = NULL;
            } 
        }


        /* Initialize middle layer for Device.IP.Interface.{i}.IPv6Prefix.{i}. */
        if ( TRUE )
        {
            rc = sprintf_s
            (
                FolderName,
                sizeof(FolderName),
                "%s%lu", 
                COSA_DML_RR_NAME_IPIF_IPv6Pre_NextInsNum, 
                pIPInterface->Cfg.InstanceNumber
            );
            if(rc < EOK)
            {
              ERR_CHK(rc);
            }
            
            pPoamIrepFoNextIns = 
                (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoIPIF->GetFolder
                    (
                        (ANSC_HANDLE)pPoamIrepFoIPIF,
                        FolderName
                    );

            if ( pPoamIrepFoNextIns )
            {
                pSlapVariable =
                    (PSLAP_VARIABLE)pPoamIrepFoNextIns->GetRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoNextIns,
                            COSA_DML_RR_NAME_IPIF_IPv6Pre_NextInsNum,
                            NULL
                        );

                if ( pSlapVariable )
                {
                    pIPInterface->ulNextIPV6PreInsNum = pSlapVariable->Variant.varUint32;
                    
                    SlapFreeVariable(pSlapVariable);
                }

                pPoamIrepFoNextIns->Remove((ANSC_HANDLE)pPoamIrepFoNextIns);
                pPoamIrepFoNextIns = NULL;
            } 
        }
    }

    CosaIpRegGetInfo((ANSC_HANDLE)pMyObject);

    EXIT:
        
    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaIPRemove
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
CosaIPRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_IP              pMyObject       = (PCOSA_DATAMODEL_IP)hThisObject;
    PSLIST_HEADER                   pIPIFHead       = (PSLIST_HEADER)&pMyObject->InterfaceList;
    PSLIST_HEADER                   pIPv4Head       = (PSLIST_HEADER)NULL;
    PSLIST_HEADER                   pIPv6Head       = (PSLIST_HEADER)NULL;
    PSLIST_HEADER                   pIPv6PreHead       = (PSLIST_HEADER)NULL;
    PCOSA_DML_IP_IF_FULL2           pIPInterface    = (PCOSA_DML_IP_IF_FULL2)NULL;
    PCOSA_DML_IP_V4ADDR             pIPv4Addr       = (PCOSA_DML_IP_V4ADDR)NULL;
    PCOSA_DML_IP_V6ADDR             pIPv6Addr       = (PCOSA_DML_IP_V6ADDR)NULL;
    PCOSA_DML_IP_V6PREFIX           pIPv6Pre        = (PCOSA_DML_IP_V6PREFIX)NULL;
    PSINGLE_LINK_ENTRY              pLink           = (PSINGLE_LINK_ENTRY )NULL;
    PSINGLE_LINK_ENTRY              pSubLink        = (PSINGLE_LINK_ENTRY )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoTemp = (PPOAM_IREP_FOLDER_OBJECT)NULL;

    /* IP Interface List */
    pLink = AnscSListGetFirstEntry(pIPIFHead);

    while ( pLink )
    {
        pCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        pLink   = AnscSListGetNextEntry(pLink);

        pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;

        if ( pIPInterface )
        {
            AnscSListPopEntryByLink(pIPIFHead, &pCosaContext->Linkage);

            pIPv4Head    = &pIPInterface->IPV4List;
            pIPv6Head    = &pIPInterface->IPV6List;
            pIPv6PreHead = &pIPInterface->IPV6PrefixList;

            /* ipv4addr List */            
            pSubLink = AnscSListGetFirstEntry(pIPv4Head);
        
            while ( pSubLink )
            {
                pSubCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSubLink);
                
                pSubLink  = AnscSListGetNextEntry(pSubLink);
            
                pIPv4Addr = (PCOSA_DML_IP_V4ADDR)pSubCosaContext->hContext;
            
                AnscSListPopEntryByLink(pIPv4Head, &pSubCosaContext->Linkage);
            
                if ( pIPv4Addr )
                {
                    AnscFreeMemory(pIPv4Addr);
                }
            
                AnscFreeMemory(pSubCosaContext);
            }

            /* ipv6addr List */
            pSubLink = AnscSListGetFirstEntry(pIPv6Head);
        
            while ( pSubLink )
            {
                pSubCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSubLink);
                
                pSubLink  = AnscSListGetNextEntry(pSubLink);
            
                pIPv6Addr = (PCOSA_DML_IP_V6ADDR)pSubCosaContext->hContext;
            
                AnscSListPopEntryByLink(pIPv6Head, &pSubCosaContext->Linkage);
            
                if ( pIPv6Addr )
                {
                    AnscFreeMemory(pIPv6Addr);
                }
            
                AnscFreeMemory(pSubCosaContext);
            }

            /* ipv6Prefix List */
            pSubLink = AnscSListGetFirstEntry(pIPv6PreHead);
        
            while ( pSubLink )
            {
                pSubCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSubLink);
                
                pSubLink  = AnscSListGetNextEntry(pSubLink);
            
                pIPv6Pre = (PCOSA_DML_IP_V6PREFIX)pSubCosaContext->hContext;
            
                AnscSListPopEntryByLink(pIPv6PreHead, &pSubCosaContext->Linkage);
            
                if ( pIPv6Pre )
                {
                    AnscFreeMemory(pIPv6Pre);
                }
            
                AnscFreeMemory(pSubCosaContext);
            }

            AnscFreeMemory(pIPInterface);
            AnscFreeMemory(pCosaContext);
        }
    }


    /* Remove necessary resounce */
    if ( pMyObject->pActivePort && (pMyObject->activePortCount > 0) ) 
    {
        AnscFreeMemory(pMyObject->pActivePort);
        
        pMyObject->pActivePort = NULL;
    }
 
    /* Remove self */
    pPoamIrepFoTemp = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderIPInterfaceHA;
    if ( pPoamIrepFoTemp )
    {
        pPoamIrepFoTemp->Remove(pPoamIrepFoTemp);
    }

    
    pPoamIrepFoTemp = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderIPInterface;
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
        CosaIpRegGetInfo
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve ip intreface parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CosaIpRegGetInfo

    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_IP              pMyObject       = (PCOSA_DATAMODEL_IP)hThisObject;
    PSLIST_HEADER                   pIPIFHead       = (PSLIST_HEADER)&pMyObject->InterfaceList;
    PCOSA_DML_IP_IF_FULL2           pIPInterface    = (PCOSA_DML_IP_IF_FULL2)NULL;
    PCOSA_DML_IP_IF_FULL2           pIPEntry        = (PCOSA_DML_IP_IF_FULL2)NULL;
    PCOSA_DML_IP_V4ADDR             pIPv4Addr       = (PCOSA_DML_IP_V4ADDR)NULL;
    PCOSA_DML_IP_V6ADDR             pIPv6Addr       = (PCOSA_DML_IP_V6ADDR)NULL;
    PCOSA_DML_IP_V6PREFIX           pIPv6Pre        = (PCOSA_DML_IP_V6PREFIX)NULL;
    PSINGLE_LINK_ENTRY              pLink           = (PSINGLE_LINK_ENTRY )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoIPHA = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderIPInterfaceHA;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoIPIF = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PSLAP_VARIABLE                  pSlapVariable   = (PSLAP_VARIABLE          )NULL;
    ULONG                           ulEntryCount            = 0;
    ULONG                           ulIndex                 = 0;
    ULONG                           ulInstanceNumber        = 0;
    ULONG                           ulUpperInstanceNumber   = 0;
    char*                           pFolderName             = NULL;
    char*                           pAlias                  = NULL;
    errno_t                         rc                      = -1;

    if ( !pPoamIrepFoIPHA )
    {
        return ANSC_STATUS_FAILURE;
    }

    /* Load the newly added but not yet commited entries */

    ulEntryCount = pPoamIrepFoIPHA->GetFolderCount((ANSC_HANDLE)pPoamIrepFoIPHA);

    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        pFolderName = pPoamIrepFoIPHA->EnumFolder
                        (
                            (ANSC_HANDLE)pPoamIrepFoIPHA,
                            ulIndex
                        );

        if ( !pFolderName )
        {
            continue;
        }

        pPoamIrepFoIPIF = (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoIPHA->GetFolder
                            (
                                (ANSC_HANDLE)pPoamIrepFoIPHA, 
                                pFolderName
                            );

        if ( !pPoamIrepFoIPIF )
        {
            continue;
        }

        if ( _ansc_strstr(pFolderName, COSA_DML_RR_NAME_IPIF_Prefix) )
        {
            if ( TRUE )
            {
                pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoIPIF->GetRecord
                                    (
                                        (ANSC_HANDLE)pPoamIrepFoIPIF,
                                        COSA_DML_RR_NAME_IPIF_InsNum,
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
                pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoIPIF->GetRecord
                                (
                                    (ANSC_HANDLE)pPoamIrepFoIPIF,
                                     COSA_DML_RR_NAME_IPIF_Alias,
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

            pIPInterface = (PCOSA_DML_IP_IF_FULL2)AnscAllocateMemory(sizeof(COSA_DML_IP_IF_FULL2));

            if ( !pIPInterface )
            {
                AnscFreeMemory(pCosaContext);
                
                returnStatus = ANSC_STATUS_RESOURCES;
                
                goto EXIT;
            }

            AnscSListInitializeHeader(&pIPInterface->IPV4List);
            AnscSListInitializeHeader(&pIPInterface->IPV6List);
            AnscSListInitializeHeader(&pIPInterface->IPV6PrefixList);
            
            pIPInterface->ulNextIPV4InsNum    = 1;
            pIPInterface->ulNextIPV6InsNum    = 1;
            pIPInterface->ulNextIPV6PreInsNum = 1;
            
            pIPInterface->Cfg.InstanceNumber = ulInstanceNumber;
            rc = strcpy_s(pIPInterface->Cfg.Alias, sizeof(pIPInterface->Cfg.Alias), pAlias ? pAlias : "");
            ERR_CHK(rc);

            pCosaContext->InstanceNumber   = ulInstanceNumber;
            pCosaContext->bNew             = TRUE;
            pCosaContext->hContext         = (ANSC_HANDLE)pIPInterface;
            pCosaContext->hParentTable     = NULL;
            pCosaContext->hPoamIrepUpperFo = (ANSC_HANDLE)pPoamIrepFoIPHA;
            pCosaContext->hPoamIrepFo      = (ANSC_HANDLE)pPoamIrepFoIPIF;
            
            CosaSListPushEntryByInsNum(pIPIFHead, pCosaContext);
        }
        else
        {
            if ( TRUE )
            {
                pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoIPIF->GetRecord
                                    (
                                        (ANSC_HANDLE)pPoamIrepFoIPIF,
                                        COSA_DML_RR_NAME_IPIF_Upper_InsNum,
                                        NULL
                                    );

                if ( pSlapVariable )
                {
                    ulUpperInstanceNumber = pSlapVariable->Variant.varUint32;
                    
                    SlapFreeVariable(pSlapVariable);
                }

                if ( ulUpperInstanceNumber != 0 )
                {
                    pLink = AnscSListGetFirstEntry(pIPIFHead);

                    while ( pLink )
                    {
                        pCosaContext  = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
                        pLink   = AnscSListGetNextEntry(pLink);

                        pIPEntry = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;

                        if ( pIPEntry && (pIPEntry->Cfg.InstanceNumber == ulUpperInstanceNumber) )
                        {
                            break;
                        }
                    }

                    if ( !pIPEntry )
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
            }

            if ( TRUE )
            {
                pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoIPIF->GetRecord
                                    (
                                        (ANSC_HANDLE)pPoamIrepFoIPIF,
                                        COSA_DML_RR_NAME_IPIF_InsNum,
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
                pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoIPIF->GetRecord
                                    (
                                        (ANSC_HANDLE)pPoamIrepFoIPIF,
                                        COSA_DML_RR_NAME_IPIF_Alias,
                                        NULL
                                    );
            
                if ( pSlapVariable )
                {
                    pAlias = AnscCloneString(pSlapVariable->Variant.varString);            

                    SlapFreeVariable(pSlapVariable);
                }
            }
            
            if ( _ansc_strstr(pFolderName, COSA_DML_RR_NAME_IPIF_IPV4_Prefix) )
            {
                pIPv4Addr = (PCOSA_DML_IP_V4ADDR)AnscAllocateMemory(sizeof(COSA_DML_IP_V4ADDR));

                if ( !pIPv4Addr )
                {
                    returnStatus = ANSC_STATUS_RESOURCES;

                    goto EXIT;
                }

                pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

                if ( !pSubCosaContext )
                {
                    AnscFreeMemory(pIPv4Addr);            

                    returnStatus = ANSC_STATUS_RESOURCES;

                    goto EXIT;
                }

                pIPv4Addr->InstanceNumber = ulInstanceNumber;
                rc = strcpy_s(pIPv4Addr->Alias, sizeof(pIPv4Addr->Alias), pAlias ? pAlias : "");
                ERR_CHK(rc);

                pSubCosaContext->InstanceNumber  = ulInstanceNumber;
                pSubCosaContext->hContext        = (ANSC_HANDLE)pIPv4Addr;
                pSubCosaContext->hParentTable    = (ANSC_HANDLE)pIPEntry;
                pSubCosaContext->bNew            = TRUE;
                pSubCosaContext->hPoamIrepUpperFo = (ANSC_HANDLE)pPoamIrepFoIPHA;
                pSubCosaContext->hPoamIrepFo      = (ANSC_HANDLE)pPoamIrepFoIPIF;

                CosaSListPushEntryByInsNum(&pIPEntry->IPV4List, pSubCosaContext);
            }

            if ( _ansc_strstr(pFolderName, COSA_DML_RR_NAME_IPIF_IPV6_Prefix) )
            {
                pIPv6Addr = (PCOSA_DML_IP_V6ADDR)AnscAllocateMemory(sizeof(COSA_DML_IP_V6ADDR));

                if ( !pIPv6Addr )
                {
                    returnStatus = ANSC_STATUS_RESOURCES;

                    goto EXIT;
                }
                
                pIPv6Addr->Origin = COSA_DML_IP6_ORIGIN_Static;

                pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

                if ( !pSubCosaContext )
                {
                    AnscFreeMemory(pIPv6Addr);            

                    returnStatus = ANSC_STATUS_RESOURCES;

                    goto EXIT;
                }

                pIPv6Addr->InstanceNumber = ulInstanceNumber;
                rc = strcpy_s(pIPv6Addr->Alias, sizeof(pIPv6Addr->Alias), pAlias ? pAlias : "");
                ERR_CHK(rc);

                pSubCosaContext->InstanceNumber  = ulInstanceNumber;
                pSubCosaContext->hContext        = (ANSC_HANDLE)pIPv6Addr;
                pSubCosaContext->hParentTable    = (ANSC_HANDLE)pIPEntry;
                pSubCosaContext->bNew            = TRUE;
                pSubCosaContext->hPoamIrepUpperFo = (ANSC_HANDLE)pPoamIrepFoIPHA;
                pSubCosaContext->hPoamIrepFo      = (ANSC_HANDLE)pPoamIrepFoIPIF;

                CosaSListPushEntryByInsNum(&pIPEntry->IPV6List, pSubCosaContext);
            }

            if ( _ansc_strstr(pFolderName, COSA_DML_RR_NAME_IPIF_IPV6Pre_Prefix) )
            {
                pIPv6Pre = (PCOSA_DML_IP_V6PREFIX)AnscAllocateMemory(sizeof(COSA_DML_IP_V6PREFIX));

                if ( !pIPv6Pre )
                {
                    returnStatus = ANSC_STATUS_RESOURCES;

                    goto EXIT;
                }

                pIPv6Pre->Origin   = COSA_DML_IP6PREFIX_ORIGIN_Static;
                pIPv6Pre->bEnabled = TRUE;

                pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

                if ( !pSubCosaContext )
                {
                    AnscFreeMemory(pIPv6Pre);            

                    returnStatus = ANSC_STATUS_RESOURCES;

                    goto EXIT;
                }

                pIPv6Pre->InstanceNumber = ulInstanceNumber;
                rc = strcpy_s(pIPv6Pre->Alias, sizeof(pIPv6Pre->Alias),pAlias ? pAlias : "");
                ERR_CHK(rc);

                pSubCosaContext->InstanceNumber  = ulInstanceNumber;
                pSubCosaContext->hContext        = (ANSC_HANDLE)pIPv6Pre;
                pSubCosaContext->hParentTable    = (ANSC_HANDLE)pIPEntry;
                pSubCosaContext->bNew            = TRUE;
                pSubCosaContext->hPoamIrepUpperFo = (ANSC_HANDLE)pPoamIrepFoIPHA;
                pSubCosaContext->hPoamIrepFo      = (ANSC_HANDLE)pPoamIrepFoIPIF;

                CosaSListPushEntryByInsNum(&pIPEntry->IPV6PrefixList, pSubCosaContext);
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
        CosaIPRegAddInfo
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

        This function is called to retrieve ip intreface parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CosaIPRegAddInfo
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
    PCOSA_DATAMODEL_IP              pMyObject         = (PCOSA_DATAMODEL_IP )hThisObject;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hCosaContext;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoIPIF   = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderIPInterface;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoUpper  = (PPOAM_IREP_FOLDER_OBJECT )pCosaContext->hPoamIrepUpperFo;  
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFo       = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PSLAP_VARIABLE                  pSlapVariable     = (PSLAP_VARIABLE           )NULL;
    char                            FolderName[32]    = {0};
    errno_t                         rc                = -1;

    if ( !pPoamIrepFoIPIF || !pPoamIrepFoUpper )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pPoamIrepFoIPIF->EnableFileSync((ANSC_HANDLE)pPoamIrepFoIPIF, FALSE);
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

    rc = sprintf_s(FolderName, sizeof(FolderName),"%s%lu", pNextInsNumName, ulUpperInsNum);
    if(rc < EOK)
    {
      ERR_CHK(rc);
    }

    if ( TRUE )
    {
        pPoamIrepFo = (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoIPIF->GetFolder
                        (
                            (ANSC_HANDLE)pPoamIrepFoIPIF,
                            FolderName
                        );

        if ( !pPoamIrepFo )
        {
            pPoamIrepFo = pPoamIrepFoIPIF->AddFolder
                            (
                                (ANSC_HANDLE)pPoamIrepFoIPIF,
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
        rc = sprintf_s(FolderName, sizeof(FolderName),"%s%lu%lu", pPreffix, ulUpperInsNum, pCosaContext->InstanceNumber);
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
                                COSA_DML_RR_NAME_IPIF_Upper_InsNum,
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
                                COSA_DML_RR_NAME_IPIF_InsNum,
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
                                COSA_DML_RR_NAME_IPIF_Alias,
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

    pPoamIrepFoIPIF->EnableFileSync((ANSC_HANDLE)pPoamIrepFoIPIF, TRUE);

    return returnStatus;
}



/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        CosaIPRegDelInfo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCosaContext
            );

    description:

        This function is called to retrieve ip intreface parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                    hCosaContext

    return:     status of operation.

**********************************************************************/


ANSC_STATUS
CosaIPRegDelInfo
    (
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

        AnscFreeMemory((ANSC_HANDLE)pPoamIrepFo);
    }

    return ANSC_STATUS_SUCCESS;
}


/*this func is used by non-static ipv6Prefix like RA/dhcp,here will take RA for example
 when a new RA prefix appears and we don't have it in syscfg, we'll give it a inst_num and alias,
 but since we can only add/del static v6prefix, we won't add it into HA folder,
 for the PSM, we only need to update ulNextIPV6PreInsNum for this IP interface*/
ANSC_STATUS
CosaIPv6PrefGenInstAlias
    (
        PCOSA_DML_IP_IF_FULL2       p_ipif,
        PCOSA_DML_IP_V6PREFIX       pEntry
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_IP              pMyObject        = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoIPIF   = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderIPInterface;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFo       = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PSLAP_VARIABLE                  pSlapVariable     = (PSLAP_VARIABLE           )NULL;
    char                            FolderName[256]    = {0};
    errno_t                         rc                 = -1;

    /*
      For dynamic and writable table, we don't keep the Maximum InstanceNumber.
      If there is delay_added entry, we just jump that InstanceNumber.
    */
    do
    {
        int found = 0;

        if ( !CosaSListGetEntryByInsNum(&p_ipif->IPV6PrefixList, p_ipif->ulNextIPV6PreInsNum) )
        {
            found = 1;
            pEntry->InstanceNumber            = p_ipif->ulNextIPV6PreInsNum;
        }

        p_ipif->ulNextIPV6PreInsNum++;
       
        if (!p_ipif->ulNextIPV6PreInsNum)
            p_ipif->ulNextIPV6PreInsNum = 1;

        if (found)
            break;

    }while(1);

    rc = sprintf_s( pEntry->Alias, sizeof(pEntry->Alias),"IPv6Prefix%lu", pEntry->InstanceNumber );
    if(rc < EOK)
    {
      ERR_CHK(rc);
    }

    /* keep new MaxInstanceNumber */

    if ( !pPoamIrepFoIPIF )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pPoamIrepFoIPIF->EnableFileSync((ANSC_HANDLE)pPoamIrepFoIPIF, FALSE);
    }

        
    SlapAllocVariable(pSlapVariable);

    if ( !pSlapVariable )
    {
        returnStatus = ANSC_STATUS_RESOURCES;
        goto  EXIT1;
    }

    _ansc_snprintf(FolderName, sizeof(FolderName)-1, "%s%lu", COSA_DML_RR_NAME_IPIF_IPv6Pre_NextInsNum, p_ipif->Cfg.InstanceNumber);

    if ( TRUE )
    {
        pPoamIrepFo = (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoIPIF->GetFolder
            (
                (ANSC_HANDLE)pPoamIrepFoIPIF,
                FolderName
            );

        if ( !pPoamIrepFo )
        {
            pPoamIrepFo = pPoamIrepFoIPIF->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoIPIF,
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
                COSA_DML_RR_NAME_IPIF_IPv6Pre_NextInsNum
            );
            
        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
        pSlapVariable->Variant.varUint32 = p_ipif->ulNextIPV6PreInsNum;

        returnStatus = pPoamIrepFo->AddRecord
            (
                (ANSC_HANDLE)pPoamIrepFo,
                COSA_DML_RR_NAME_IPIF_IPv6Pre_NextInsNum,
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

    

EXIT1:
    
    if ( pSlapVariable )
    {
        SlapFreeVariable(pSlapVariable);
    }

    pPoamIrepFoIPIF->EnableFileSync((ANSC_HANDLE)pPoamIrepFoIPIF, TRUE);

    return returnStatus;
}


/*this func is used by non-static ipv6Address like RA/dhcp,here will take RA for example
 when a new RA address appears and we don't have it in syscfg, we'll give it a inst_num and alias,
 but since we can only add/del static v6prefix, we won't add it into HA folder,
 for the PSM, we only need to update ulNextIPV6AddrInsNum for this IP interface*/
ANSC_STATUS
CosaIPv6AddrGenInstAlias
    (
        PCOSA_DML_IP_IF_FULL2       p_ipif,
        PCOSA_DML_IP_V6ADDR       pEntry
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_IP              pMyObject        = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoIPIF   = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderIPInterface;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFo       = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PSLAP_VARIABLE                  pSlapVariable     = (PSLAP_VARIABLE           )NULL;
    char                            FolderName[256]    = {0};
    errno_t                         rc                 = -1;

    /*
      For dynamic and writable table, we don't keep the Maximum InstanceNumber.
      If there is delay_added entry, we just jump that InstanceNumber.
    */
    do
    {
        int found = 0;
        
        if ( !CosaSListGetEntryByInsNum(&p_ipif->IPV6List, p_ipif->ulNextIPV6InsNum) )
        {
            found = 1;
            pEntry->InstanceNumber            = p_ipif->ulNextIPV6InsNum;
        }

        p_ipif->ulNextIPV6InsNum++;
       
        if (!p_ipif->ulNextIPV6InsNum)
            p_ipif->ulNextIPV6InsNum = 1;

        if (found)
            break;

    }while(1);
    

    rc = sprintf_s( pEntry->Alias, sizeof(pEntry->Alias),"IPv6Address%lu", pEntry->InstanceNumber );
    if(rc < EOK)
    {
      ERR_CHK(rc);
    }

    /* keep new MaxInstanceNumber */

    if ( !pPoamIrepFoIPIF )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pPoamIrepFoIPIF->EnableFileSync((ANSC_HANDLE)pPoamIrepFoIPIF, FALSE);
    }

        
    SlapAllocVariable(pSlapVariable);

    if ( !pSlapVariable )
    {
        returnStatus = ANSC_STATUS_RESOURCES;
        goto  EXIT1;
    }

    _ansc_snprintf(FolderName, sizeof(FolderName)-1, "%s%lu", COSA_DML_RR_NAME_IPIF_IPv6_NextInsNum, p_ipif->Cfg.InstanceNumber);

    if ( TRUE )
    {
        pPoamIrepFo = (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoIPIF->GetFolder
            (
                (ANSC_HANDLE)pPoamIrepFoIPIF,
                FolderName
            );

        if ( !pPoamIrepFo )
        {
            pPoamIrepFo = pPoamIrepFoIPIF->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoIPIF,
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
                COSA_DML_RR_NAME_IPIF_IPv6_NextInsNum
            );

        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
        pSlapVariable->Variant.varUint32 = p_ipif->ulNextIPV6InsNum;

        returnStatus = pPoamIrepFo->AddRecord
            (
                (ANSC_HANDLE)pPoamIrepFo,
                COSA_DML_RR_NAME_IPIF_IPv6_NextInsNum,
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

EXIT1:
    
    if ( pSlapVariable )
    {
        SlapFreeVariable(pSlapVariable);
    }

    pPoamIrepFoIPIF->EnableFileSync((ANSC_HANDLE)pPoamIrepFoIPIF, TRUE);

    return returnStatus;
}


