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

    module: cosa_routing_internal.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaRoutingCreate
        *  CosaRoutingInitialize
        *  CosaRoutingRemove
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
#include "cosa_routing_apis.h"
#include "cosa_routing_internal.h"
#include "plugin_main_apis.h"
#include "safec_lib_common.h"

extern void * g_pDslhDmlAgent;

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        CosaRoutingCreate
            (
            );

    description:

        This function constructs cosa nat object and return handle.

    argument:  

    return:     newly created nat object.

**********************************************************************/

ANSC_HANDLE
CosaRoutingCreate
    (
        VOID
    )
{
    PCOSA_DATAMODEL_ROUTING         pMyObject    = (PCOSA_DATAMODEL_ROUTING)NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PCOSA_DATAMODEL_ROUTING)AnscAllocateMemory(sizeof(COSA_DATAMODEL_ROUTING));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->Oid               = COSA_DATAMODEL_ROUTING_OID;
    pMyObject->Create            = CosaRoutingCreate;
    pMyObject->Remove            = CosaRoutingRemove;
    pMyObject->Initialize        = CosaRoutingInitialize;

    pMyObject->Initialize   ((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaRoutingInitialize
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
CosaRoutingInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_ROUTING         pMyObject           = (PCOSA_DATAMODEL_ROUTING)hThisObject;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoCOSA     = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoRouter   = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoRouterHA = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoNextIns  = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PSLAP_VARIABLE                  pSlapVariable       = (PSLAP_VARIABLE           )NULL;
    PCOSA_DML_ROUTER_FULL2          pRouter             = (PCOSA_DML_ROUTER_FULL2   )NULL;
    PCOSA_DML_ROUTING_V4_ENTRY      pRouterForward      = (PCOSA_DML_ROUTING_V4_ENTRY)NULL;
    PCOSA_DML_ROUTING_V6_ENTRY      pRouterIPv6Forward  = (PCOSA_DML_ROUTING_V6_ENTRY)NULL;
    PCOSA_DML_RIP_IF_CFG            pRipIF              = (PCOSA_DML_RIP_IF_CFG)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext        = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2       = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    char                            FolderName[32]      = {0};
    ULONG                           ulEntryCount        = 1;
    ULONG                           ulIndex             = 0;
    ULONG                           ulSubEntryCount     = 0;
    ULONG                           ulSubIndex          = 0;

    PCOSA_DML_STATICROUTE_CFG pStaticRoute  = NULL;
    PCOSA_DML_STATICROUTE_CFG pStaticRoute2 = NULL;
    ULONG                     RouteCount   = 0;
    errno_t                   rc           = -1;

    AnscSListInitializeHeader(&pMyObject->StaticRoute);
    pMyObject->NextInstanceNumber   =   1;
    
    /* Get static route from end */
    pStaticRoute = CosaDmlStaticRouteGetEntries( NULL, &RouteCount);
    if ( !pStaticRoute )
    {
        AnscTraceError(("CosaRoutingInitialize -- failed to retrieve static routes!"));
        returnStatus = ANSC_STATUS_RESOURCES;
        goto  EXIT;
    }

    /* We put this into a link because this table is writable. */    
    for ( ulIndex = 0; ulIndex < RouteCount; ulIndex++ )
    {
        pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

        if ( !pCosaContext )
        {
            returnStatus = ANSC_STATUS_RESOURCES;
            goto  EXIT;  
        }

        pStaticRoute2 = (PCOSA_DML_STATICROUTE_CFG)AnscAllocateMemory(sizeof(COSA_DML_STATICROUTE_CFG));

        if ( !pStaticRoute2 )
        {
            AnscFreeMemory(pCosaContext);
            returnStatus = ANSC_STATUS_RESOURCES;
            goto  EXIT;  
        }

        *pStaticRoute2 = pStaticRoute[ulIndex];

        pCosaContext->hContext       = (ANSC_HANDLE)pStaticRoute2;
        pCosaContext->hParentTable   = NULL;
        pCosaContext->bNew           = FALSE;
        pCosaContext->InstanceNumber = pMyObject->NextInstanceNumber++;

        CosaSListPushEntryByInsNum(&pMyObject->StaticRoute, pCosaContext);

    }
    ulIndex      = 0;

    pCosaContext  = NULL;

    CosaDmlRoutingInit(pMyObject, NULL);

    /* Initiation all functions */
    pMyObject->ulNextRouterInsNum = 1;
    
    pMyObject->ulNextRipIFInsNum = 1;
    
    AnscSListInitializeHeader(&pMyObject->RouterList)
    AnscSListInitializeHeader(&pMyObject->RipIFList)
    
    pMyObject->RouteInfo.IfNum = 0;
    pMyObject->RouteInfo.pInfo = NULL;

    pMyObject->PreviousTime    = 0;

    pMyObject->hIrepFolderCOSA = g_GetRegistryRootFolder(g_pDslhDmlAgent);

    pPoamIrepFoCOSA = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderCOSA;
    
    if ( !pPoamIrepFoCOSA )
    {
        returnStatus = ANSC_STATUS_FAILURE;
        
        goto  EXIT;
    }

    pPoamIrepFoRouter = (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoCOSA->GetFolder
                        (
                            (ANSC_HANDLE)pPoamIrepFoCOSA,
                            COSA_IREP_FOLDER_NAME_ROUTER
                        );

    if ( !pPoamIrepFoRouter )
    {
        pPoamIrepFoRouter = pPoamIrepFoCOSA->AddFolder
                            (
                                (ANSC_HANDLE)pPoamIrepFoCOSA,
                                COSA_IREP_FOLDER_NAME_ROUTER,
                                0
                            );
    }

    if ( !pPoamIrepFoRouter )
    {
        returnStatus = ANSC_STATUS_FAILURE;
        
        goto  EXIT;
    }
    else
    {        
        pMyObject->hIrepFolderRouter = (ANSC_HANDLE)pPoamIrepFoRouter;
    }

    pPoamIrepFoRouterHA = (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoRouter->GetFolder
                        (
                            (ANSC_HANDLE)pPoamIrepFoCOSA,
                            COSA_IREP_FOLDER_NAME_ROUTER_HA
                        );

    if ( !pPoamIrepFoRouterHA )
    {
        pPoamIrepFoRouterHA = pPoamIrepFoRouter->AddFolder
                            (
                                (ANSC_HANDLE)pPoamIrepFoRouter,
                                COSA_IREP_FOLDER_NAME_ROUTER_HA,
                                0
                            );
    }

    if ( !pPoamIrepFoRouterHA )
    {
        returnStatus = ANSC_STATUS_FAILURE;
        
        goto  EXIT;
    }
    else
    {
        pMyObject->hIrepFolderRouterHA = (ANSC_HANDLE)pPoamIrepFoRouterHA;
    }

    /* Retrieve the next Instance Number for routing router */
    if ( TRUE )
    {
        rc = sprintf_s
        (
            FolderName,
            sizeof(FolderName),
            "%s%d", 
            COSA_DML_RR_NAME_ROUTER_NextInsNum,
            0
        );
        if(rc < EOK)
        {
          ERR_CHK(rc);
        }
        
        pPoamIrepFoNextIns = (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoRouter->GetFolder
                                (
                                    (ANSC_HANDLE)pPoamIrepFoRouter,
                                    FolderName
                                );

        AnscZeroMemory(FolderName, 32);

        if ( pPoamIrepFoNextIns )
        {
            pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoNextIns->GetRecord
                                (
                                    (ANSC_HANDLE)pPoamIrepFoNextIns,
                                    COSA_DML_RR_NAME_ROUTER_NextInsNum,
                                    NULL
                                );

            if ( pSlapVariable )
            {
                pMyObject->ulNextRouterInsNum = pSlapVariable->Variant.varUint32;
                
                SlapFreeVariable(pSlapVariable);
            }

            pPoamIrepFoNextIns->Remove((ANSC_HANDLE)pPoamIrepFoNextIns);
            pPoamIrepFoNextIns = NULL;
        }
    }

    /* Initialize middle layer for Device.Routing.Router.{i}.  Assume one router */
    if ( TRUE )
    {
        pRouter = (PCOSA_DML_ROUTER_FULL2)AnscAllocateMemory(sizeof(COSA_DML_ROUTER_FULL2));

        if ( !pRouter )
        {
            returnStatus = ANSC_STATUS_RESOURCES;
            
            goto  EXIT;
        }

        AnscSListInitializeHeader(&pRouter->ForwardList);
        
        pRouter->ulNextForwardInsNum = 1;
        pRouter->ulNextIPv6ForwardInsNum = 1;

        CosaDmlRoutingRouterGetCfg(NULL, (PCOSA_DML_ROUTER_CFG)&pRouter->Cfg);
        CosaDmlRoutingRouterGetInfo(NULL, (PCOSA_DML_ROUTER_INFO)&pRouter->Info);
    }

    if ( TRUE )
    {
        pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

        if ( !pCosaContext )
        {
            AnscFreeMemory(pRouter);               
            
            returnStatus = ANSC_STATUS_RESOURCES;
            
            goto  EXIT;
        }

        if ( 0 != pRouter->Cfg.InstanceNumber )
        {
            pCosaContext->InstanceNumber = pRouter->Cfg.InstanceNumber;
            
            if ( pMyObject->ulNextRouterInsNum <=  pRouter->Cfg.InstanceNumber )
            {
                pMyObject->ulNextRouterInsNum =  pRouter->Cfg.InstanceNumber + 1;

                if ( 0 == pMyObject->ulNextRouterInsNum )
                {
                    pMyObject->ulNextRouterInsNum = 1;
                }
            }
        }
        else
        {
            pCosaContext->InstanceNumber = pMyObject->ulNextRouterInsNum;  
            
            pRouter->Cfg.InstanceNumber = pCosaContext->InstanceNumber ;
            
            pMyObject->ulNextRouterInsNum++;
            
            if ( 0 == pMyObject->ulNextRouterInsNum )
            {
                pMyObject->ulNextRouterInsNum = 1;
            }

            /* Generate Alias */
            rc = sprintf_s(pRouter->Cfg.Alias, sizeof(pRouter->Cfg.Alias),"Router%lu", pMyObject->ulNextRouterInsNum);
            if(rc < EOK)
            {
              ERR_CHK(rc);
            }

            CosaDmlRoutingRouterSetCfg(NULL, (PCOSA_DML_ROUTER_CFG)&pRouter->Cfg);
        }

        pCosaContext->hContext      = (ANSC_HANDLE)pRouter;            
        pCosaContext->hParentTable  = NULL;            
        pCosaContext->bNew          = FALSE;

        CosaSListPushEntryByInsNum(&pMyObject->RouterList, pCosaContext);
    }

    /* Retrieve the next Instance Number for routing router IPv4Forwarding  */
    if ( TRUE )
    {
        rc = sprintf_s
        (
            FolderName,
            sizeof(FolderName),
            "%s%lu", 
            COSA_DML_RR_NAME_ROUTER_Forward_NextInsNum, 
            pRouter->Cfg.InstanceNumber
        );
        if(rc < EOK)
        {
          ERR_CHK(rc);
        }
        
        pPoamIrepFoNextIns = 
            (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoRouter->GetFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoRouter,
                    FolderName
                );

        if ( pPoamIrepFoNextIns )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoNextIns->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoNextIns,
                        COSA_DML_RR_NAME_ROUTER_Forward_NextInsNum,
                        NULL
                    );

            if ( pSlapVariable )
            {
                pRouter->ulNextForwardInsNum = pSlapVariable->Variant.varUint32;
                
                SlapFreeVariable(pSlapVariable);
            }

            pPoamIrepFoNextIns->Remove((ANSC_HANDLE)pPoamIrepFoNextIns);
            pPoamIrepFoNextIns = NULL;
        } 
    }

    /* Initialize middle layer for Device.Routing.Router.{i}.IPv4Forwarding.{i}. */
    ulSubEntryCount = CosaDmlRoutingGetNumberOfV4Entries(NULL);

    for ( ulSubIndex = 0; ulSubIndex < ulSubEntryCount; ulSubIndex++ )
    {
        pRouterForward = (PCOSA_DML_ROUTING_V4_ENTRY)AnscAllocateMemory(sizeof(COSA_DML_ROUTING_V4_ENTRY));
    
        if ( !pRouterForward )
        {
            returnStatus = ANSC_STATUS_RESOURCES;
            
            goto  EXIT;
        }
    
        CosaDmlRoutingGetV4Entry(NULL, ulSubIndex, pRouterForward);
    
        if ( TRUE )
        {
            pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

            if ( !pSubCosaContext )
            {
                AnscFreeMemory(pRouterForward);

                returnStatus = ANSC_STATUS_RESOURCES;
                
                goto  EXIT;  
            }

            if ( 0 != pRouterForward->InstanceNumber )
            {
                pSubCosaContext->InstanceNumber = pRouterForward->InstanceNumber;

                if ( pRouter->ulNextForwardInsNum <= pRouterForward->InstanceNumber )
                {
                    pRouter->ulNextForwardInsNum = pRouterForward->InstanceNumber + 1;

                    if ( pRouter->ulNextForwardInsNum == 0 )
                    {
                        pRouter->ulNextForwardInsNum = 1;
                    }
                }  
            }
            else
            {
                pSubCosaContext->InstanceNumber = pRouterForward->InstanceNumber = pRouter->ulNextForwardInsNum;

                pRouter->ulNextForwardInsNum++;

                if ( pRouter->ulNextForwardInsNum == 0 )
                {
                    pRouter->ulNextForwardInsNum = 1;
                }

                /* Generate Alias */
                rc = sprintf_s(pRouterForward->Alias, sizeof(pRouterForward->Alias),"IPv4Forwarding%lu", pSubCosaContext->InstanceNumber);
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                }

                /* TODO: Set InstanceNumber Alias back */
                CosaDmlRoutingSetV4EntryValues
                (
                    NULL, 
                    ulSubIndex, 
                    pRouterForward->InstanceNumber,
                    pRouterForward->Alias
                );
            }                

            pSubCosaContext->hContext     = (ANSC_HANDLE)pRouterForward;
            pSubCosaContext->hParentTable = (ANSC_HANDLE)pRouter;
            pSubCosaContext->bNew         = FALSE;

            CosaSListPushEntryByInsNum(&pRouter->ForwardList, pSubCosaContext);
        }
    }

    /* Retrieve the next Instance Number for routing router IPv6Forwarding  */
    if ( TRUE )
    {
        rc = sprintf_s
        (
            FolderName,
            sizeof(FolderName),
            "%s%lu", 
            COSA_DML_RR_NAME_ROUTER_IPv6Forward_NextInsNum, 
            pRouter->Cfg.InstanceNumber
        );
        if(rc < EOK)
        {
          ERR_CHK(rc);
        }
        
        pPoamIrepFoNextIns = 
            (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoRouter->GetFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoRouter,
                    FolderName
                );

        if ( pPoamIrepFoNextIns )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoNextIns->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoNextIns,
                        COSA_DML_RR_NAME_ROUTER_IPv6Forward_NextInsNum,
                        NULL
                    );

            if ( pSlapVariable )
            {
                pRouter->ulNextIPv6ForwardInsNum = pSlapVariable->Variant.varUint32;
                
                SlapFreeVariable(pSlapVariable);
            }

            pPoamIrepFoNextIns->Remove((ANSC_HANDLE)pPoamIrepFoNextIns);
            pPoamIrepFoNextIns = NULL;
        } 
    }

    /* Initialize middle layer for Device.Routing.Router.{i}.IPv6Forwarding.{i}. */
    ulSubEntryCount = CosaDmlRoutingGetNumberOfV6Entries(NULL);

    for ( ulSubIndex = 0; ulSubIndex < ulSubEntryCount; ulSubIndex++ )
    {
        pRouterIPv6Forward = (PCOSA_DML_ROUTING_V6_ENTRY)AnscAllocateMemory(sizeof(COSA_DML_ROUTING_V6_ENTRY));
    
        if ( !pRouterIPv6Forward )
        {
            returnStatus = ANSC_STATUS_RESOURCES;
            
            goto  EXIT;
        }
    
        CosaDmlRoutingGetV6Entry(NULL, ulSubIndex, pRouterIPv6Forward);
    
        if ( TRUE )
        {
            pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

            if ( !pSubCosaContext )
            {
                AnscFreeMemory(pRouterIPv6Forward);

                returnStatus = ANSC_STATUS_RESOURCES;
                
                goto  EXIT;  
            }

            if ( 0 != pRouterIPv6Forward->InstanceNumber )
            {
                pSubCosaContext->InstanceNumber = pRouterIPv6Forward->InstanceNumber;

                if ( pRouter->ulNextIPv6ForwardInsNum <= pRouterIPv6Forward->InstanceNumber )
                {
                    pRouter->ulNextIPv6ForwardInsNum = pRouterIPv6Forward->InstanceNumber + 1;

                    if ( pRouter->ulNextIPv6ForwardInsNum == 0 )
                    {
                        pRouter->ulNextIPv6ForwardInsNum = 1;
                    }
                }  
            }
            else
            {
                pSubCosaContext->InstanceNumber = pRouterIPv6Forward->InstanceNumber = pRouter->ulNextIPv6ForwardInsNum;

                pRouter->ulNextIPv6ForwardInsNum++;

                if ( pRouter->ulNextIPv6ForwardInsNum == 0 )
                {
                    pRouter->ulNextIPv6ForwardInsNum = 1;
                }

                /* Generate Alias */
                rc = sprintf_s(pRouterIPv6Forward->Alias, sizeof(pRouterIPv6Forward->Alias),"IPv6Forwarding%lu", pSubCosaContext->InstanceNumber);
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                }

                /* TODO: Set InstanceNumber Alias back */
                CosaDmlRoutingSetV6EntryValues
                (
                    NULL, 
                    ulSubIndex, 
                    pRouterIPv6Forward->InstanceNumber,
                    pRouterIPv6Forward->Alias
                );
            }                

            pSubCosaContext->hContext     = (ANSC_HANDLE)pRouterIPv6Forward;
            pSubCosaContext->hParentTable = (ANSC_HANDLE)pRouter;
            pSubCosaContext->bNew         = FALSE;

            CosaSListPushEntryByInsNum(&pRouter->IPv6ForwardList, pSubCosaContext);
        }
    }

   
    /* Initialize middle layer for Device.Routing.RIP. */
    CosaDmlRipGetCfg(NULL, &pMyObject->RIPCfg);

    /* Initialize middle layer for Device.Routing.RIP.InterfaceSetting.{i}. */    
    ulEntryCount = CosaDmlRipGetNumberOfIfEntries(NULL);

    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        pRipIF = (PCOSA_DML_RIP_IF_CFG)AnscAllocateMemory(sizeof(COSA_DML_RIP_IF_CFG));

        if ( !pRipIF )
        {
            returnStatus = ANSC_STATUS_RESOURCES;
            
            goto  EXIT;
        }
        
        CosaDmlRipIfGetCfg(NULL, ulIndex, pRipIF);

        if ( TRUE )
        {
            pCosaContext2 = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

            if ( !pCosaContext2 )
            {
                AnscFreeMemory(pRipIF);               
                
                returnStatus = ANSC_STATUS_RESOURCES;
                
                goto  EXIT;
            }

            if ( 0 != pRipIF->InstanceNumber )
            {
                pCosaContext2->InstanceNumber = pRipIF->InstanceNumber;
                
                if ( pMyObject->ulNextRipIFInsNum <=  pRipIF->InstanceNumber )
                {
                    pMyObject->ulNextRipIFInsNum =  pRipIF->InstanceNumber + 1;

                    if ( 0 == pMyObject->ulNextRipIFInsNum )
                    {
                        pMyObject->ulNextRipIFInsNum = 1;
                    }
                }
            }
            else
            {
                pCosaContext2->InstanceNumber = pMyObject->ulNextRipIFInsNum;  
                
                pRipIF->InstanceNumber = pCosaContext2->InstanceNumber ;
                
                pMyObject->ulNextRipIFInsNum++;
                
                if ( 0 == pMyObject->ulNextRipIFInsNum )
                {
                    pMyObject->ulNextRipIFInsNum = 1;
                }

            }

            pCosaContext2->hContext      = (ANSC_HANDLE)pRipIF;            
            pCosaContext2->hParentTable  = NULL;            
            pCosaContext2->bNew          = FALSE;

            CosaSListPushEntryByInsNum(&pMyObject->RipIFList, pCosaContext2);
        }
    }
  
    
    CosaRoutingRegGetInfo((ANSC_HANDLE)pMyObject);

EXIT:
    if ( pStaticRoute )
        AnscFreeMemory(pStaticRoute);
            
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
CosaRoutingRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_ROUTING         pMyObject       = (PCOSA_DATAMODEL_ROUTING)hThisObject;
    PSLIST_HEADER                   pRouterHead     = (PSLIST_HEADER)&pMyObject->RouterList;
    PSLIST_HEADER                   pRipIFHead      = (PSLIST_HEADER)&pMyObject->RipIFList;
    PSLIST_HEADER                   pForwardHead    = (PSLIST_HEADER)NULL;
    PCOSA_DML_ROUTER_FULL2          pRouter         = (PCOSA_DML_ROUTER_FULL2)NULL;
    PCOSA_DML_ROUTING_V4_ENTRY      pRouterForward  = (PCOSA_DML_ROUTING_V4_ENTRY)NULL;
    PCOSA_DML_RIP_IF_CFG            pRipIF          = (PCOSA_DML_RIP_IF_CFG)NULL;
    PSINGLE_LINK_ENTRY              pLink           = (PSINGLE_LINK_ENTRY )NULL;
    PSINGLE_LINK_ENTRY              pSubLink        = (PSINGLE_LINK_ENTRY )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2   = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoTemp = (PPOAM_IREP_FOLDER_OBJECT)NULL;

    /* Router List */
    pLink = AnscSListGetFirstEntry(pRouterHead);

    while ( pLink )
    {
        pCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        pLink   = AnscSListGetNextEntry(pLink);

        pRouter = (PCOSA_DML_ROUTER_FULL2)pCosaContext->hContext;

        if ( pRouter )
        {
            AnscSListPopEntryByLink(pRouterHead, &pCosaContext->Linkage);

            /* IPv4Forwarding List */
            pForwardHead = &pRouter->ForwardList;
            
            pSubLink = AnscSListGetFirstEntry(pForwardHead);
        
            while ( pSubLink )
            {
                pSubCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSubLink);
                
                pSubLink  = AnscSListGetNextEntry(pSubLink);
            
                pRouterForward = (PCOSA_DML_ROUTING_V4_ENTRY)pSubCosaContext->hContext;
            
                AnscSListPopEntryByLink(pForwardHead, &pSubCosaContext->Linkage);
            
                if ( pRouterForward )
                {
                    AnscFreeMemory(pRouterForward);
                }
            
                AnscFreeMemory(pSubCosaContext);
            }

            AnscFreeMemory(pRouter);
            AnscFreeMemory(pCosaContext);
        }
    }

    /* RIP InterfaceSetting list*/
    pLink = AnscSListGetFirstEntry(pRipIFHead);

    while ( pLink )
    {
        pCosaContext2 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        pLink   = AnscSListGetNextEntry(pLink);

        pRipIF = (PCOSA_DML_RIP_IF_CFG)pCosaContext2->hContext;

        if ( pRipIF )
        {
            AnscSListPopEntryByLink(pRipIFHead, &pCosaContext2->Linkage);

            AnscFreeMemory(pRipIF);
            AnscFreeMemory(pCosaContext2);
        }
    }

    if ( pMyObject->RouteInfo.IfNum > 0 ) 
    {
        AnscFreeMemory(pMyObject->RouteInfo.pInfo);
        pMyObject->RouteInfo.pInfo = NULL;
    }
 
    /* Remove self */
    pPoamIrepFoTemp = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderRouterHA;
    if ( pPoamIrepFoTemp )
    {
        pPoamIrepFoTemp->Remove(pPoamIrepFoTemp);
    }

    pPoamIrepFoTemp = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderRouter;
    if ( pPoamIrepFoTemp )
    {
        pPoamIrepFoTemp->Remove(pPoamIrepFoTemp);
    }

    /*RDKB-6743, CID-32970, use after free, NULL assignment is made to remove Static Analyzer warning*/
    AnscFreeMemory((ANSC_HANDLE)pMyObject);
    pMyObject = (PCOSA_DATAMODEL_ROUTING)NULL;

    CosaDmlRoutingRemove((ANSC_HANDLE)pMyObject);


    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        CosaRoutingRegGetInfo
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
CosaRoutingRegGetInfo

    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_ROUTING         pMyObject       = (PCOSA_DATAMODEL_ROUTING)hThisObject;
    PSLIST_HEADER                   pRouterHead     = (PSLIST_HEADER)&pMyObject->RouterList;
    PCOSA_DML_ROUTER_FULL2          pRouter         = (PCOSA_DML_ROUTER_FULL2)NULL;
    PCOSA_DML_ROUTER_FULL2          pRouterEntry    = (PCOSA_DML_ROUTER_FULL2)NULL;
    PCOSA_DML_ROUTING_V4_ENTRY      pRouterForward  = (PCOSA_DML_ROUTING_V4_ENTRY)NULL;
    PCOSA_DML_ROUTING_V6_ENTRY      pRouterIPv6Forward  = (PCOSA_DML_ROUTING_V6_ENTRY)NULL;
    PCOSA_DML_RIP_IF_CFG            pRipIF          = (PCOSA_DML_RIP_IF_CFG)NULL;
    PSINGLE_LINK_ENTRY              pLink           = (PSINGLE_LINK_ENTRY )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2   = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoRouterHA = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderRouterHA;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoRouter   = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PSLAP_VARIABLE                  pSlapVariable           = (PSLAP_VARIABLE          )NULL;
    ULONG                           ulEntryCount            = 0;
    ULONG                           ulIndex                 = 0;
    ULONG                           ulInstanceNumber        = 0;
    ULONG                           ulUpperInstanceNumber   = 0;
    char*                           pFolderName             = NULL;
    char*                           pAlias                  = NULL;
    errno_t                         rc                      = -1;
    if ( !pPoamIrepFoRouterHA )
    {
        return ANSC_STATUS_FAILURE;
    }

    /* Load the newly added but not yet commited entries */

    ulEntryCount = pPoamIrepFoRouterHA->GetFolderCount((ANSC_HANDLE)pPoamIrepFoRouterHA);

    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        pFolderName = pPoamIrepFoRouterHA->EnumFolder
                        (
                            (ANSC_HANDLE)pPoamIrepFoRouterHA,
                            ulIndex
                        );

        if ( !pFolderName )
        {
            continue;
        }

        pPoamIrepFoRouter = (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoRouterHA->GetFolder
                            (
                                (ANSC_HANDLE)pPoamIrepFoRouterHA, 
                                pFolderName
                            );

        if ( !pPoamIrepFoRouter )
        {
            continue;
        }

        if ( _ansc_strstr(pFolderName, "Router") )
        {
            if ( TRUE )
            {
                pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoRouter->GetRecord
                                    (
                                        (ANSC_HANDLE)pPoamIrepFoRouter,
                                        COSA_DML_RR_NAME_ROUTER_InsNum,
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
                pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoRouter->GetRecord
                                (
                                    (ANSC_HANDLE)pPoamIrepFoRouter,
                                     COSA_DML_RR_NAME_ROUTER_Alias,
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

            pRouter = (PCOSA_DML_ROUTER_FULL2)AnscAllocateMemory(sizeof(COSA_DML_ROUTER_FULL2));

            if ( !pRouter )
            {
                AnscFreeMemory(pCosaContext);
                
                returnStatus = ANSC_STATUS_RESOURCES;
                
                goto EXIT;
            }

            AnscSListInitializeHeader(&pRouter->ForwardList);
            AnscSListInitializeHeader(&pRouter->IPv6ForwardList);
            
            pRouter->ulNextForwardInsNum = 1;
            pRouter->ulNextIPv6ForwardInsNum = 1;
            
            pRouter->Cfg.InstanceNumber = ulInstanceNumber;
            rc = strcpy_s(pRouter->Cfg.Alias,sizeof(pRouter->Cfg.Alias), pAlias ? pAlias : "");
            ERR_CHK(rc);

            pCosaContext->InstanceNumber   = ulInstanceNumber;
            pCosaContext->bNew             = TRUE;
            pCosaContext->hContext         = (ANSC_HANDLE)pRouter;
            pCosaContext->hParentTable     = NULL;
            pCosaContext->hPoamIrepUpperFo = (ANSC_HANDLE)pPoamIrepFoRouterHA;
            pCosaContext->hPoamIrepFo      = (ANSC_HANDLE)pPoamIrepFoRouter;
            
            CosaSListPushEntryByInsNum(pRouterHead, pCosaContext);
        }
        else
        {
            if ( TRUE )
            {
                pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoRouter->GetRecord
                                    (
                                        (ANSC_HANDLE)pPoamIrepFoRouter,
                                        COSA_DML_RR_NAME_ROUTER_InsNum,
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
                pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoRouter->GetRecord
                                    (
                                        (ANSC_HANDLE)pPoamIrepFoRouter,
                                        COSA_DML_RR_NAME_ROUTER_Alias,
                                        NULL
                                    );
            
                if ( pSlapVariable )
                {
                    pAlias = AnscCloneString(pSlapVariable->Variant.varString);            

                    SlapFreeVariable(pSlapVariable);
                }
            }
            
            if ( _ansc_strstr(pFolderName, "IPv4Forward") )
            {
                if ( TRUE )
                {
                    pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoRouter->GetRecord
                                        (
                                            (ANSC_HANDLE)pPoamIrepFoRouter,
                                            COSA_DML_RR_NAME_ROUTER_Upper_InsNum,
                                            NULL
                                        );

                    if ( pSlapVariable )
                    {
                        ulUpperInstanceNumber = pSlapVariable->Variant.varUint32;
                        
                        SlapFreeVariable(pSlapVariable);
                    }

                    if ( ulUpperInstanceNumber != 0 )
                    {
                        pLink = AnscSListGetFirstEntry(pRouterHead);

                        while ( pLink )
                        {
                            pCosaContext  = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
                            pLink   = AnscSListGetNextEntry(pLink);

                            pRouterEntry = (PCOSA_DML_ROUTER_FULL2)pCosaContext->hContext;

                            if ( pRouterEntry && (pRouterEntry->Cfg.InstanceNumber == ulUpperInstanceNumber) )
                            {
                                break;
                            }
                        }

                        if ( !pRouterEntry )
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

                pRouterForward = (PCOSA_DML_ROUTING_V4_ENTRY)AnscAllocateMemory(sizeof(COSA_DML_ROUTING_V4_ENTRY));

                if ( !pRouterForward )
                {
                    returnStatus = ANSC_STATUS_RESOURCES;

                    goto EXIT;
                }

                pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

                if ( !pSubCosaContext )
                {
                    AnscFreeMemory(pRouterForward);            

                    returnStatus = ANSC_STATUS_RESOURCES;

                    goto EXIT;
                }

                pRouterForward->InstanceNumber = ulInstanceNumber;
                rc = strcpy_s(pRouterForward->Alias,sizeof(pRouterForward->Alias), pAlias ? pAlias : "");
                ERR_CHK(rc);

                pSubCosaContext->InstanceNumber   = ulInstanceNumber;
                pSubCosaContext->hContext         = (ANSC_HANDLE)pRouterForward;
                pSubCosaContext->hParentTable     = (ANSC_HANDLE)pRouterEntry;
                pSubCosaContext->bNew             = TRUE;
                pSubCosaContext->hPoamIrepUpperFo = (ANSC_HANDLE)pPoamIrepFoRouterHA;
                pSubCosaContext->hPoamIrepFo      = (ANSC_HANDLE)pPoamIrepFoRouter;

                CosaSListPushEntryByInsNum(&pRouterEntry->ForwardList, pSubCosaContext);
            }
            else if ( _ansc_strstr(pFolderName, "IPv6Forward") )
            {
                if ( TRUE )
                {
                    pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoRouter->GetRecord
                                        (
                                            (ANSC_HANDLE)pPoamIrepFoRouter,
                                            COSA_DML_RR_NAME_ROUTER_Upper_InsNum,
                                            NULL
                                        );

                    if ( pSlapVariable )
                    {
                        ulUpperInstanceNumber = pSlapVariable->Variant.varUint32;
                        
                        SlapFreeVariable(pSlapVariable);
                    }

                    if ( ulUpperInstanceNumber != 0 )
                    {
                        pLink = AnscSListGetFirstEntry(pRouterHead);

                        while ( pLink )
                        {
                            pCosaContext  = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
                            pLink   = AnscSListGetNextEntry(pLink);

                            pRouterEntry = (PCOSA_DML_ROUTER_FULL2)pCosaContext->hContext;

                            if ( pRouterEntry && (pRouterEntry->Cfg.InstanceNumber == ulUpperInstanceNumber) )
                            {
                                break;
                            }
                        }

                        if ( !pRouterEntry )
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

                pRouterIPv6Forward = (PCOSA_DML_ROUTING_V6_ENTRY)AnscAllocateMemory(sizeof(COSA_DML_ROUTING_V6_ENTRY));

                if ( !pRouterIPv6Forward )
                {
                    returnStatus = ANSC_STATUS_RESOURCES;

                    goto EXIT;
                }

                pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

                if ( !pSubCosaContext )
                {
                    AnscFreeMemory(pRouterIPv6Forward);            

                    returnStatus = ANSC_STATUS_RESOURCES;

                    goto EXIT;
                }

                pRouterIPv6Forward->InstanceNumber = ulInstanceNumber;
                rc = strcpy_s(pRouterIPv6Forward->Alias,sizeof(pRouterIPv6Forward->Alias), pAlias ? pAlias : "");
                ERR_CHK(rc);

                pSubCosaContext->InstanceNumber   = ulInstanceNumber;
                pSubCosaContext->hContext         = (ANSC_HANDLE)pRouterIPv6Forward;
                pSubCosaContext->hParentTable     = (ANSC_HANDLE)pRouterEntry;
                pSubCosaContext->bNew             = TRUE;
                pSubCosaContext->hPoamIrepUpperFo = (ANSC_HANDLE)pPoamIrepFoRouterHA;
                pSubCosaContext->hPoamIrepFo      = (ANSC_HANDLE)pPoamIrepFoRouter;

                CosaSListPushEntryByInsNum(&pRouterEntry->IPv6ForwardList, pSubCosaContext);
            }
            else if ( _ansc_strstr(pFolderName, "RipInterface") )
            {
                /* CID: 53503 Wrong sizeof argument*/
                pRipIF = (PCOSA_DML_RIP_IF_CFG)AnscAllocateMemory(sizeof(COSA_DML_RIP_IF_CFG));

                if ( !pRipIF )
                {
                    returnStatus = ANSC_STATUS_RESOURCES;

                    goto EXIT;
                }

                pCosaContext2 = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

                if ( !pCosaContext2 )
                {
                    AnscFreeMemory(pRipIF);            

                    returnStatus = ANSC_STATUS_RESOURCES;

                    goto EXIT;
                }

                pRipIF->InstanceNumber = ulInstanceNumber;
                rc = strcpy_s(pRipIF->Alias,sizeof(pRipIF->Alias), pAlias ? pAlias : "");
                ERR_CHK(rc);

                pCosaContext2->InstanceNumber   = ulInstanceNumber;
                pCosaContext2->hContext         = (ANSC_HANDLE)pRipIF;
                pCosaContext2->hParentTable     = NULL;
                pCosaContext2->bNew             = TRUE;
                pCosaContext2->hPoamIrepUpperFo = (ANSC_HANDLE)pPoamIrepFoRouterHA;
                pCosaContext2->hPoamIrepFo      = (ANSC_HANDLE)pPoamIrepFoRouter;

                CosaSListPushEntryByInsNum(&pMyObject->RipIFList, pCosaContext2);
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
        CosaRoutingRegAddInfo
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

        This function is called to get routing router parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CosaRoutingRegAddInfo
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
    PCOSA_DATAMODEL_ROUTING         pMyObject         = (PCOSA_DATAMODEL_ROUTING )hThisObject;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hCosaContext;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoRouter = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderRouter;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoUpper  = (PPOAM_IREP_FOLDER_OBJECT )pCosaContext->hPoamIrepUpperFo;  
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFo       = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PSLAP_VARIABLE                  pSlapVariable     = (PSLAP_VARIABLE           )NULL;
    char                            FolderName[32]    = {0};
    errno_t                         rc                = -1;

    if ( !pPoamIrepFoRouter || !pPoamIrepFoUpper )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pPoamIrepFoRouter->EnableFileSync((ANSC_HANDLE)pPoamIrepFoRouter, FALSE);
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
        pPoamIrepFo = (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoRouter->GetFolder
                        (
                            (ANSC_HANDLE)pPoamIrepFoRouter,
                            FolderName
                        );

        if ( !pPoamIrepFo )
        {
            pPoamIrepFo = pPoamIrepFoRouter->AddFolder
                            (
                                (ANSC_HANDLE)pPoamIrepFoRouter,
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
                                COSA_DML_RR_NAME_ROUTER_Upper_InsNum,
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
                                COSA_DML_RR_NAME_ROUTER_InsNum,
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
                                COSA_DML_RR_NAME_ROUTER_Alias,
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

    pPoamIrepFoRouter->EnableFileSync((ANSC_HANDLE)pPoamIrepFoRouter, TRUE);

    return returnStatus;
}



/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        CosaRoutingRegAddInfo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCosaContext
            );

    description:

        This function is called to delete routing parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                    hCosaContext

    return:     status of operation.

**********************************************************************/


ANSC_STATUS
CosaRoutingRegDelInfo
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


/*
    This function is used to get a string like this:
    " route 192.168.2.0/24
      route 192.168.3.0/20
      route 10.1.1.128/26"
      
*/
char* CosaTimeGetRipdConfStaticPart (ANSC_HANDLE   hContext)
{
    PCOSA_DATAMODEL_ROUTING         pMyObject       = NULL; 
    PSLIST_HEADER                   pListHead       = NULL;
    PSINGLE_LINK_ENTRY              pSListEntry     = NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink        = NULL;
    PCOSA_DML_STATICROUTE_CFG       pEntry          = NULL;
    CHAR                            Buff[512]       = {0};
    CHAR                            Buff2[512]      = {0};
    ULONG                           Val1            = 0;
    ULONG                           Val2            = 0;
    ULONG                           nIndex          = 0;
    ULONG                           K1              = 0;
    ULONG                           K2              = 0;
    errno_t                         rc              = -1;
    
    if ( g_pCosaBEManager->hRouting == NULL )
    {
        pMyObject = (PCOSA_DATAMODEL_ROUTING)hContext;
        AnscTraceWarning(("CosaTimeGetRipdConfStaticPart -- g_pCosaBEManager->hRouting is NULL\n"));
    }
    else
    {
        pMyObject = (PCOSA_DATAMODEL_ROUTING)g_pCosaBEManager->hRouting;
        AnscTraceWarning(("CosaTimeGetRipdConfStaticPart -- g_pCosaBEManager->hRouting is not NULL\n"));
    }

    if ( pMyObject != NULL )
    {
        pListHead = &pMyObject->StaticRoute;
        AnscTraceWarning(("CosaTimeGetRipdConfStaticPart -- pSListEntry is not NULL.\n"));
    }


    if ( pListHead == NULL ) 
    {
        AnscTraceWarning(("CosaTimeGetRipdConfStaticPart -- hRouting is NULL.\n"));
        return NULL;
    }

    pSListEntry = AnscSListGetEntryByIndex(pListHead, nIndex++);

    while ( pSListEntry )
    {
        pCxtLink          = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSListEntry);
        pEntry            = pCxtLink->hContext;

        if ( _ansc_strlen(Buff) > 500 )
        {
            AnscTraceWarning(("CosTimeGetRipdConfStaticPart -- there is error.%s.\n", Buff));
            break;
        }
        
        if ( pEntry->RIPAdvertise )
        {
            Val1 = pEntry->DestIPAddress & pEntry->DestSubnetMask;
            Val2 = pEntry->DestSubnetMask;

            for (K1=0,K2=0; K1<32; K1++)
            {
                if ( Val2 & 0x1 )
                {
                    K2++;
                    Val2 = Val2>>1;
                }
            }
            rc = sprintf_s(Buff2, sizeof(Buff2)," route %s/%lu\n", _ansc_inet_ntoa(*((struct in_addr*)&Val1)), K2);
            if(rc < EOK)
            {
              ERR_CHK(rc);
              return NULL;
            }
            _ansc_strcat(Buff,Buff2);

        }

        pSListEntry = AnscSListGetEntryByIndex(pListHead, nIndex++);
    }

    AnscTraceWarning(("CosTimeGetRipdConfStaticPart -- exits.\n"));

    return AnscCloneString(Buff);
}








