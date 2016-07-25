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

    module: cosa_dhcpv4_internal.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    copyright:

        Cisco Systems, Inc.
        All Rights Reserved.

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaDhcpv4Create
        *  CosaDhcpv4Initialize
        *  CosaDhcpv4Remove
        *  CosaDhcpv4RegGetDhcpv4Info
        *  CosaDhcpv4RegGetDhcpv4Info
        *  CosaDhcpv4RegSetDhcpv4Info
        *  CosaDhcpv4ClientHasDelayAddedChild
        *  CosaDhcpv4PoolHasDelayAddedChild
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
#include "cosa_dhcpv4_apis.h"
#include "cosa_dhcpv4_internal.h"
#include "plugin_main_apis.h"
#include "poam_irepfo_interface.h"
#include "sys_definitions.h"

extern void * g_pDslhDmlAgent;

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        CosaDhcpv4Create
            (
            );

    description:

        This function constructs cosa nat object and return handle.

    argument:  

    return:     newly created nat object.

**********************************************************************/

ANSC_HANDLE
CosaDhcpv4Create
    (
        VOID
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DHCPV4          pMyObject    = (PCOSA_DATAMODEL_DHCPV4)NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PCOSA_DATAMODEL_DHCPV4)AnscAllocateMemory(sizeof(COSA_DATAMODEL_DHCPV4));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->Oid               = COSA_DATAMODEL_DHCPV4_OID;
    pMyObject->Create            = CosaDhcpv4Create;
    pMyObject->Remove            = CosaDhcpv4Remove;
    pMyObject->Initialize        = CosaDhcpv4Initialize;

    pMyObject->Initialize   ((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDhcpv4Initialize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate  cosa nat object and return handle.

    argument:    ANSC_HANDLE                 hThisObject
            This handle is actually the pointer of this object
            itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaDhcpv4Initialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DHCPV4          pMyObject         = (PCOSA_DATAMODEL_DHCPV4)hThisObject;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoCOSA   = NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoDhcpv4 = NULL;

    /* We need call the initiation function of backend firstly .
            When backend return failure, we don't return because if return, all middle layer function will be not complete*/
    CosaDmlDhcpInit( NULL, NULL );

    /* Initiation all functions */
    AnscSListInitializeHeader( &pMyObject->ClientList );
    AnscSListInitializeHeader( &pMyObject->PoolList );
    pMyObject->maxInstanceOfClient  = 0;
    pMyObject->maxInstanceOfPool    = 0;
    pMyObject->maxInstanceX_CISCO_COM_SAddr = 0;
    AnscZeroMemory(pMyObject->AliasOfClient, sizeof(pMyObject->AliasOfClient));
    AnscZeroMemory(pMyObject->AliasOfPool, sizeof(pMyObject->AliasOfPool));
    AnscZeroMemory(pMyObject->AliasOfX_CISCO_COM_SAddr, sizeof(pMyObject->AliasOfX_CISCO_COM_SAddr));

    /*We need to get Instance Info from cosa configuration*/
    pPoamIrepFoCOSA = (PPOAM_IREP_FOLDER_OBJECT)g_GetRegistryRootFolder(g_pDslhDmlAgent);

    if ( !pPoamIrepFoCOSA )
    {
        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT;
    }

    pPoamIrepFoDhcpv4 = 
        (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoCOSA->GetFolder
            (
                (ANSC_HANDLE)pPoamIrepFoCOSA,
                COSA_IREP_FOLDER_NAME_DHCPV4
            );

    if ( !pPoamIrepFoDhcpv4 )
    {
        pPoamIrepFoCOSA->EnableFileSync((ANSC_HANDLE)pPoamIrepFoCOSA, FALSE);
    
        pPoamIrepFoDhcpv4 =
            pPoamIrepFoCOSA->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoCOSA,
                    COSA_IREP_FOLDER_NAME_DHCPV4,
                    0
                );

        pPoamIrepFoCOSA->EnableFileSync((ANSC_HANDLE)pPoamIrepFoCOSA, TRUE);

    }

    if ( !pPoamIrepFoDhcpv4 )
    {
        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT;
    }
    else
    {
        pMyObject->hIrepFolderDhcpv4 = (ANSC_HANDLE)pPoamIrepFoDhcpv4;
    }

    /* We need get NextInstanceNumber from backend. By the way, the whole tree 
            was created. Moreover, we also need get delay-added entry and put them
            into our tree. */
    CosaDhcpv4RegGetDhcpv4Info((ANSC_HANDLE)pMyObject);

    /* Firstly we create the whole system from backend */
    CosaDhcpv4BackendGetDhcpv4Info((ANSC_HANDLE)pMyObject);

EXIT:
    
    return returnStatus;
}



/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDhcpv4Remove
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
CosaDhcpv4Remove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DHCPV4          pMyObject           = (PCOSA_DATAMODEL_DHCPV4)hThisObject;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoDhcpv4   = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderDhcpv4;

    PCOSA_CONTEXT_DHCPC_LINK_OBJECT pCxtDhcpcLink       = NULL;
    PCOSA_CONTEXT_POOL_LINK_OBJECT  pCxtPoolLink        = NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink            = NULL;
    PSINGLE_LINK_ENTRY              pSListEntry         = NULL;
    PSINGLE_LINK_ENTRY              pSListEntry2        = NULL;
    BOOL                            bFound              = FALSE;

    /* Remove necessary resource */
    
    pSListEntry         = AnscSListPopEntry(&pMyObject->ClientList);
    while( pSListEntry != NULL)
    {
        pCxtDhcpcLink     = ACCESS_COSA_CONTEXT_DHCPC_LINK_OBJECT(pSListEntry);
        pSListEntry       = AnscSListGetNextEntry(pSListEntry);
        
        pSListEntry2         = AnscSListPopEntry(&pCxtDhcpcLink->SendOptionList);
        while( pSListEntry2 != NULL)
        {
            pCxtLink         = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSListEntry2);
            pSListEntry2       = AnscSListGetNextEntry(pSListEntry2);

            AnscFreeMemory(pCxtLink->hContext);
            AnscFreeMemory(pCxtLink);  
        }

        pSListEntry2         = AnscSListPopEntry(&pCxtDhcpcLink->ReqOptionList);
        while( pSListEntry2 != NULL)
        {
            pCxtLink         = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSListEntry2);
            pSListEntry2       = AnscSListGetNextEntry(pSListEntry2);

            AnscFreeMemory(pCxtLink->hContext);
            AnscFreeMemory(pCxtLink); 
        }

        AnscFreeMemory(pCxtDhcpcLink->hContext);
        AnscFreeMemory(pCxtDhcpcLink);
    }

    pSListEntry         = AnscSListPopEntry(&pMyObject->PoolList);
    while( pSListEntry != NULL)
    {
        pCxtPoolLink     = ACCESS_COSA_CONTEXT_POOL_LINK_OBJECT(pSListEntry);
        pSListEntry       = AnscSListGetNextEntry(pSListEntry);
        
        pSListEntry2         = AnscSListPopEntry(&pCxtPoolLink->StaticAddressList);
        while( pSListEntry2 != NULL)
        {
            pCxtLink         = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSListEntry2);
            pSListEntry2       = AnscSListGetNextEntry(pSListEntry2);
    
            AnscFreeMemory(pCxtLink->hContext);
            AnscFreeMemory(pCxtLink); 
        }
        
        AnscFreeMemory(pCxtPoolLink->hContext);
        AnscFreeMemory(pCxtPoolLink);
    }

    if ( pPoamIrepFoDhcpv4 )
    {
        pPoamIrepFoDhcpv4->Remove( (ANSC_HANDLE)pPoamIrepFoDhcpv4);
    }
    
    /* Remove self */
    AnscFreeMemory((ANSC_HANDLE)pMyObject);
    
    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        CosaDhcpv4RegGetDhcpv4Info
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the NextInstanceNumber for every table, Create
        the link tree. For delay_added entry, we also need create them.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of dhcpv4
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CosaDhcpv4BackendGetDhcpv4Info
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;    
    PCOSA_DATAMODEL_DHCPV4          pDhcpv4           = (PCOSA_DATAMODEL_DHCPV4)hThisObject;
    PCOSA_DML_DHCPC_FULL            pDhcpc            = NULL;
    PCOSA_DML_DHCPC_REQ_OPT         pReqOption        = NULL;
    PCOSA_DML_DHCP_OPT              pSendOption       = NULL;
    PCOSA_DML_DHCPS_POOL_FULL       pPool             = NULL;
    PCOSA_DML_DHCPS_SADDR           pStaticAddr       = NULL;
    PCOSA_DML_DHCPSV4_OPTION        pOption           = NULL;
    ULONG                           clientCount       = 0;
    ULONG                           poolCount         = 0;
    ULONG                           count             = 0;
    ULONG                           ulIndex           = 0;
    ULONG                           ulIndex2          = 0;

    PCOSA_CONTEXT_DHCPC_LINK_OBJECT pClientCxtLink    = NULL;    
    PCOSA_CONTEXT_DHCPC_LINK_OBJECT pClientCxtLink2   = NULL;    
    PCOSA_CONTEXT_POOL_LINK_OBJECT  pPoolCxtLink      = NULL;    
    PCOSA_CONTEXT_POOL_LINK_OBJECT  pPoolCxtLink2     = NULL;    
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink2         = NULL;

    BOOL                            bNeedSave         = FALSE;

    /* Get DHCPv4.Client.{i} */
    clientCount = CosaDmlDhcpcGetNumberOfEntries(NULL);
    for ( ulIndex = 0; ulIndex < clientCount; ulIndex++ )
    {
        pDhcpc  = (PCOSA_DML_DHCPC_FULL)AnscAllocateMemory( sizeof(COSA_DML_DHCPC_FULL) );
        if ( !pDhcpc )
        {
            break;
        }

        DHCPV4_CLIENT_SET_DEFAULTVALUE(pDhcpc);
        returnStatus = CosaDmlDhcpcGetEntry(NULL, ulIndex, pDhcpc);
        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            AnscFreeMemory(pDhcpc);
            break;
        }

        pClientCxtLink = (PCOSA_CONTEXT_DHCPC_LINK_OBJECT)AnscAllocateMemory( sizeof(COSA_CONTEXT_DHCPC_LINK_OBJECT) );
        if ( !pClientCxtLink )
        {
            AnscFreeMemory(pDhcpc);
            break;
        }

        DHCPV4_CLIENT_INITIATION_CONTEXT(pClientCxtLink)
        pClientCxtLink->hContext       = (ANSC_HANDLE)pDhcpc;
        pClientCxtLink->bNew           = FALSE;

        if ( !pDhcpc->Cfg.InstanceNumber )
        {
            if ( !++pDhcpv4->maxInstanceOfClient )
            {
                pDhcpv4->maxInstanceOfClient = 1;
            }
            bNeedSave                        = TRUE;

            pDhcpc->Cfg.InstanceNumber     = pDhcpv4->maxInstanceOfClient;
            pClientCxtLink->InstanceNumber = pDhcpc->Cfg.InstanceNumber;
            
            _ansc_sprintf(pDhcpc->Cfg.Alias, "DHCPv4%u", pDhcpc->Cfg.InstanceNumber);

            returnStatus = CosaDmlDhcpcSetValues
                            (
                                NULL, 
                                ulIndex,
                                pDhcpc->Cfg.InstanceNumber, 
                                pDhcpc->Cfg.Alias
                            );

            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                AnscFreeMemory(pDhcpc);
                AnscFreeMemory(pClientCxtLink);
                break;
            }

            /* Put into our list */
            CosaSListPushEntryByInsNum(&pDhcpv4->ClientList, (PCOSA_CONTEXT_LINK_OBJECT)pClientCxtLink);
        }
        else
        {
            pClientCxtLink->InstanceNumber = pDhcpc->Cfg.InstanceNumber;

            /* This case never happen. Add it just for simulation code run well */
            if ( pDhcpv4->maxInstanceOfClient < pClientCxtLink->InstanceNumber )
            {
                pDhcpv4->maxInstanceOfClient = pClientCxtLink->InstanceNumber;
                bNeedSave                    = TRUE;
            }

            /* if this entry is in link tree already because it's the parent of delay_added table */
            pClientCxtLink2 = (PCOSA_CONTEXT_DHCPC_LINK_OBJECT)CosaSListGetEntryByInsNum(&pDhcpv4->ClientList, pClientCxtLink->InstanceNumber);
            if ( !pClientCxtLink2 )
            {
                CosaSListPushEntryByInsNum(&pDhcpv4->ClientList, (PCOSA_CONTEXT_LINK_OBJECT)pClientCxtLink);
            }
            else
            {
                AnscFreeMemory( pClientCxtLink2->hContext );
                pClientCxtLink2->hContext       = (ANSC_HANDLE)pDhcpc;
                if ( pClientCxtLink2->bNew )
                {
                    pClientCxtLink2->bNew       = FALSE;
                    bNeedSave                   = TRUE;
                }
                
                AnscFreeMemory(pClientCxtLink);
                pClientCxtLink                  = pClientCxtLink2;
                pClientCxtLink2                 = NULL;
            }            
        }

        /* We begin treat DHCPv4.Client.{i}.ReqOption.{i} */
        count = CosaDmlDhcpcGetNumberOfReqOption
                    (
                        NULL,
                        pDhcpc->Cfg.InstanceNumber
                    );
        
        for ( ulIndex2 = 0; ulIndex2 < count; ulIndex2++ )
        {
            pReqOption  = (PCOSA_DML_DHCPC_REQ_OPT)AnscAllocateMemory( sizeof(COSA_DML_DHCPC_REQ_OPT) );
            if ( !pReqOption )
            {
                break;
            }
        
            DHCPV4_REQOPTION_SET_DEFAULTVALUE(pReqOption);
            returnStatus = CosaDmlDhcpcGetReqOption
                            (
                                NULL, 
                                pDhcpc->Cfg.InstanceNumber, 
                                ulIndex2, 
                                pReqOption
                            );
            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                AnscFreeMemory(pReqOption);
                break;
            }
        
            pCxtLink = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory( sizeof(COSA_CONTEXT_LINK_OBJECT) );
            if ( !pCxtLink )
            {
                AnscFreeMemory(pReqOption);
                break;
            }
        
            COSA_CONTEXT_LINK_INITIATION_CONTENT(pCxtLink);
            pCxtLink->hContext       = (ANSC_HANDLE)pReqOption;
            pCxtLink->hParentTable   = (ANSC_HANDLE)pClientCxtLink;
            pCxtLink->bNew           = FALSE;
        
            if ( !pReqOption->InstanceNumber )
            {
                if ( !++pClientCxtLink->maxInstanceOfReq )
                {
                    pClientCxtLink->maxInstanceOfReq = 1;
                }
                bNeedSave                        = TRUE;

                pReqOption->InstanceNumber = pClientCxtLink->maxInstanceOfReq;
        
                _ansc_sprintf( pReqOption->Alias, "ReqOption%u", pReqOption->InstanceNumber );
        
                returnStatus = CosaDmlDhcpcSetReqOptionValues
                                (
                                    NULL,
                                    pDhcpc->Cfg.InstanceNumber,
                                    ulIndex, 
                                    pReqOption->InstanceNumber, 
                                    pReqOption->Alias
                                );
        
                if ( returnStatus != ANSC_STATUS_SUCCESS )
                {
                    AnscFreeMemory(pReqOption);
                    AnscFreeMemory(pCxtLink);
                    break;                    
                }
                pCxtLink->InstanceNumber = pReqOption->InstanceNumber; 
                
                /* Put into our list */
                CosaSListPushEntryByInsNum(&pClientCxtLink->ReqOptionList, (PCOSA_CONTEXT_LINK_OBJECT)pCxtLink);
            } 
            else
            {
                 pCxtLink->InstanceNumber = pReqOption->InstanceNumber; 
                
                /* This case never happen. Add it just for simulation code run well */
                if ( pClientCxtLink->maxInstanceOfReq < pReqOption->InstanceNumber )
                {
                    pClientCxtLink->maxInstanceOfReq = pReqOption->InstanceNumber;
                    bNeedSave                  = TRUE;
                }

                /* if this entry is in link tree already because it's  delay_added table */
                pCxtLink2 = (PCOSA_CONTEXT_LINK_OBJECT)CosaSListGetEntryByInsNum(&pClientCxtLink->ReqOptionList, pReqOption->InstanceNumber);
                if ( !pCxtLink2 )
                {
                    CosaSListPushEntryByInsNum(&pClientCxtLink->ReqOptionList, (PCOSA_CONTEXT_LINK_OBJECT)pCxtLink);
                }
                else
                {
                    AnscFreeMemory( pCxtLink2->hContext );                
                    pCxtLink2->hContext       = (ANSC_HANDLE)pSendOption;
                    if ( pCxtLink2->bNew )
                    {
                        pCxtLink2->bNew       = FALSE;
                        bNeedSave             = TRUE;
                    }
                    
                    AnscFreeMemory(pCxtLink);
                    pCxtLink                  = pCxtLink2;
                    pCxtLink2                 = NULL;
                }
                
            }
        }

        /* We begin treat DHCPv4.Client.{i}.SentOption.{i} */
        count = CosaDmlDhcpcGetNumberOfSentOption
                    (
                        NULL,
                        pDhcpc->Cfg.InstanceNumber
                    );
        
        for ( ulIndex2 = 0; ulIndex2 < count; ulIndex2++ )
        {
            pSendOption  = (PCOSA_DML_DHCP_OPT)AnscAllocateMemory( sizeof(COSA_DML_DHCP_OPT) );
            if ( !pSendOption )
            {
                break;
            }
        
            DHCPV4_SENDOPTION_SET_DEFAULTVALUE(pSendOption);
            returnStatus = CosaDmlDhcpcGetSentOption
                            (
                                NULL, 
                                pDhcpc->Cfg.InstanceNumber, 
                                ulIndex2, 
                                pSendOption
                            );
            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                AnscFreeMemory(pSendOption);
                break;
            }
        
            pCxtLink = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory( sizeof(COSA_CONTEXT_LINK_OBJECT) );
            if ( !pCxtLink )
            {
                AnscFreeMemory(pSendOption);
                break;
            }
        
            COSA_CONTEXT_LINK_INITIATION_CONTENT(pCxtLink);
            pCxtLink->hContext       = (ANSC_HANDLE)pSendOption;
            pCxtLink->hParentTable   = (ANSC_HANDLE)pClientCxtLink;
            pCxtLink->bNew           = FALSE;
        
            if ( !pSendOption->InstanceNumber )
            {
                if ( !++pClientCxtLink->maxInstanceOfSend )
                {
                    pClientCxtLink->maxInstanceOfSend = 1;
                    bNeedSave                  = TRUE;
                }
                bNeedSave                        = TRUE;
                pSendOption->InstanceNumber = pClientCxtLink->maxInstanceOfSend;

                _ansc_sprintf( pSendOption->Alias, "SentOption%u", pSendOption->InstanceNumber );
        
                returnStatus = CosaDmlDhcpcSetSentOptionValues
                                (
                                    NULL,
                                    pDhcpc->Cfg.InstanceNumber,
                                    ulIndex, 
                                    pSendOption->InstanceNumber, 
                                    pSendOption->Alias
                                );
        
                if ( returnStatus != ANSC_STATUS_SUCCESS )
                {
                    AnscFreeMemory(pSendOption);
                    AnscFreeMemory(pCxtLink);
                    break;                    
                }

                pCxtLink->InstanceNumber = pSendOption->InstanceNumber; 
            
                /* Put into our list */
                CosaSListPushEntryByInsNum(&pClientCxtLink->SendOptionList, (PCOSA_CONTEXT_LINK_OBJECT)pCxtLink);
                
            } 
            else
            {
                pCxtLink->InstanceNumber = pSendOption->InstanceNumber; 
                
                /* This case never happen. Add it just for simulation code run well */
                if ( pClientCxtLink->maxInstanceOfSend < pSendOption->InstanceNumber )
                {
                    pClientCxtLink->maxInstanceOfSend = pSendOption->InstanceNumber;
                }

                /* if this entry is in link tree already because it's  delay_added table */
                pCxtLink2 = (PCOSA_CONTEXT_LINK_OBJECT)CosaSListGetEntryByInsNum(&pClientCxtLink->SendOptionList, pSendOption->InstanceNumber);
                if ( !pCxtLink2 )
                {
                    CosaSListPushEntryByInsNum(&pClientCxtLink->SendOptionList, (PCOSA_CONTEXT_LINK_OBJECT)pCxtLink);
                }
                else
                {
                    AnscFreeMemory( pCxtLink2->hContext );                
                    pCxtLink2->hContext       = (ANSC_HANDLE)pSendOption;
                    if ( pCxtLink2->bNew )
                    {
                        pCxtLink2->bNew       = FALSE;
                        bNeedSave             = TRUE;
                    }

                    AnscFreeMemory(pCxtLink);
                    pCxtLink                  = pCxtLink2;
                    pCxtLink2                 = NULL;
                }            

            }
        }
    }

    /*****************************************

                Get DHCPv4.Server.Pool.{i} 


        ******************************************/
    poolCount = CosaDmlDhcpsGetNumberOfPools(NULL);
    for ( ulIndex = 0; ulIndex < poolCount; ulIndex++ )
    {
        pPool  = (PCOSA_DML_DHCPS_POOL_FULL)AnscAllocateMemory( sizeof(COSA_DML_DHCPS_POOL_FULL) );
        if ( !pPool )
        {
            break;
        }

        DHCPV4_POOL_SET_DEFAULTVALUE(pPool);
        returnStatus = CosaDmlDhcpsGetPool(NULL, ulIndex, pPool);
        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            AnscFreeMemory(pPool);/*RDKB-6735, CID-33032, free unused resource before exit*/
            pPool = NULL; 
            break;
        }

        pPoolCxtLink = (PCOSA_CONTEXT_POOL_LINK_OBJECT)AnscAllocateMemory( sizeof(COSA_CONTEXT_POOL_LINK_OBJECT) );
        if ( !pPoolCxtLink )
        {
            AnscFreeMemory(pPool);
            pPool = NULL; 
            break;
        }

        DHCPV4_POOL_INITIATION_CONTEXT(pPoolCxtLink)
        pPoolCxtLink->hContext       = (ANSC_HANDLE)pPool;
        pPoolCxtLink->bNew           = FALSE;

        if ( !pPool->Cfg.InstanceNumber )
        {
            if ( !++pDhcpv4->maxInstanceOfPool )
            {
                pDhcpv4->maxInstanceOfPool = 1;
            }
            bNeedSave                        = TRUE;

            pPool->Cfg.InstanceNumber    = pDhcpv4->maxInstanceOfPool;
            pPoolCxtLink->InstanceNumber = pPool->Cfg.InstanceNumber; 
             
            _ansc_sprintf(pPool->Cfg.Alias, "DHCPv4%u", pPool->Cfg.InstanceNumber);

            returnStatus = CosaDmlDhcpsSetPoolValues
                            (
                                NULL, 
                                ulIndex,
                                pPool->Cfg.InstanceNumber, 
                                pPool->Cfg.Alias
                            );

            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                AnscFreeMemory(pPool);
                AnscFreeMemory(pPoolCxtLink);
                break;
            }

            /* Put into our list */
            CosaSListPushEntryByInsNum(&pDhcpv4->PoolList, (PCOSA_CONTEXT_LINK_OBJECT)pPoolCxtLink);
        }
        else
        {
            pPoolCxtLink->InstanceNumber = pPool->Cfg.InstanceNumber;
            
            /* This case never happen. Add it just for simulation code run well */
            if ( pDhcpv4->maxInstanceOfPool < pPool->Cfg.InstanceNumber )
            {
                pDhcpv4->maxInstanceOfPool = pPool->Cfg.InstanceNumber;
                bNeedSave                  = TRUE;
            }

            /* if this entry is in link tree already because it's the parent of delay_added table */
            pPoolCxtLink2 = (PCOSA_CONTEXT_POOL_LINK_OBJECT)CosaSListGetEntryByInsNum(&pDhcpv4->PoolList, pPoolCxtLink->InstanceNumber);
            if ( !pPoolCxtLink2 )
            {
                CosaSListPushEntryByInsNum(&pDhcpv4->PoolList, (PCOSA_CONTEXT_LINK_OBJECT)pPoolCxtLink);
            }
            else
            {
                AnscFreeMemory( pPoolCxtLink2->hContext );                
                pPoolCxtLink2->hContext       = (ANSC_HANDLE)pPool;
                if ( pPoolCxtLink2->bNew )
                {
                    pPoolCxtLink2->bNew       = FALSE;
                    bNeedSave                 = TRUE;
                }

                AnscFreeMemory(pPoolCxtLink);
                pPoolCxtLink                  = pPoolCxtLink2;
                pPoolCxtLink2                 = NULL;
                
            }            

        }

        /* We begin treat DHCPv4.Server.Pool.{i}.StaticAddress.{i} */
        count = CosaDmlDhcpsGetNumberOfSaddr
                    (
                        NULL,
                        pPool->Cfg.InstanceNumber
                    );
        
        for ( ulIndex2 = 0; ulIndex2 < count; ulIndex2++ )
        {
            pStaticAddr  = (PCOSA_DML_DHCPS_SADDR)AnscAllocateMemory( sizeof(COSA_DML_DHCPS_SADDR) );
            if ( !pStaticAddr )
            {
                break;
            }
        
            DHCPV4_STATICADDRESS_SET_DEFAULTVALUE(pStaticAddr);
            returnStatus = CosaDmlDhcpsGetSaddr
                            (
                                NULL, 
                                pPool->Cfg.InstanceNumber, 
                                ulIndex2, 
                                pStaticAddr
                            );
            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                AnscFreeMemory(pStaticAddr);
                break;
            }
        
            pCxtLink = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory( sizeof(COSA_CONTEXT_LINK_OBJECT) );
            if ( !pCxtLink )
            {
                AnscFreeMemory(pStaticAddr);
                break;
            }
        
            COSA_CONTEXT_LINK_INITIATION_CONTENT(pCxtLink);
            pCxtLink->hContext       = (ANSC_HANDLE)pStaticAddr;
            pCxtLink->hParentTable   = (ANSC_HANDLE)pPoolCxtLink;
            pCxtLink->bNew           = FALSE;
        
            if ( !pStaticAddr->InstanceNumber )
            {
                if ( !++pPoolCxtLink->maxInstanceOfStaticAddress )
                {
                    pPoolCxtLink->maxInstanceOfStaticAddress = 1;
                }
                bNeedSave                        = TRUE;
                pStaticAddr->InstanceNumber = pPoolCxtLink->maxInstanceOfStaticAddress;
        
                _ansc_sprintf( pStaticAddr->Alias, "StaticAddress%u", pStaticAddr->InstanceNumber );
        
                returnStatus = CosaDmlDhcpsSetSaddrValues
                                (
                                    NULL,
                                    pPool->Cfg.InstanceNumber,
                                    ulIndex2, 
                                    pStaticAddr->InstanceNumber, 
                                    pStaticAddr->Alias
                                );
        
                if ( returnStatus != ANSC_STATUS_SUCCESS )
                {
                    AnscFreeMemory(pStaticAddr);
                    AnscFreeMemory(pCxtLink);
                    break;                    
                }
                
                pCxtLink->InstanceNumber = pStaticAddr->InstanceNumber; 

                /* Put into our list */
                CosaSListPushEntryByInsNum(&pPoolCxtLink->StaticAddressList, (PCOSA_CONTEXT_LINK_OBJECT)pCxtLink);
            } 
            else
            {
                pCxtLink->InstanceNumber = pStaticAddr->InstanceNumber; 
                
                /* This case never happen. Add it just for simulation code run well */
                if ( pPoolCxtLink->maxInstanceOfStaticAddress < pStaticAddr->InstanceNumber )
                {
                    pPoolCxtLink->maxInstanceOfStaticAddress = pStaticAddr->InstanceNumber;
                    bNeedSave                                = TRUE;
                }

                /* if this entry is in link tree already because it's  delay_added table */
                pCxtLink2 = (PCOSA_CONTEXT_LINK_OBJECT)CosaSListGetEntryByInsNum(&pPoolCxtLink->StaticAddressList, pStaticAddr->InstanceNumber);
                if ( !pCxtLink2 )
                {
                    CosaSListPushEntryByInsNum(&pPoolCxtLink->StaticAddressList, (PCOSA_CONTEXT_LINK_OBJECT)pCxtLink);
                }
                else
                {
                    AnscFreeMemory( pCxtLink2->hContext );                
                    pCxtLink2->hContext       = (ANSC_HANDLE)pStaticAddr;
                    if ( pCxtLink2->bNew )
                    {
                        pCxtLink2->bNew       = FALSE;
                        bNeedSave             = TRUE;
                    }

                    AnscFreeMemory(pCxtLink);
                    pCxtLink                  = pCxtLink2;
                    pCxtLink2                 = NULL;
                }            

            }
        }

        /* We begin treat DHCPv4.Server.Pool.{i}.option.{i} */
        count = CosaDmlDhcpsGetNumberOfOption
                    (
                        NULL,
                        pPool->Cfg.InstanceNumber
                    );
        
        for ( ulIndex2 = 0; ulIndex2 < count; ulIndex2++ )
        {
            pOption = (PCOSA_DML_DHCPSV4_OPTION)AnscAllocateMemory( sizeof(COSA_DML_DHCPSV4_OPTION) );
            if ( !pOption)
            {
                break;
            }
        
            DHCPV4_OPTION_SET_DEFAULTVALUE(pOption);
            returnStatus = CosaDmlDhcpsGetOption
                            (
                                NULL, 
                                pPool->Cfg.InstanceNumber, 
                                ulIndex2, 
                                pOption
                            );
            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                AnscFreeMemory(pOption);
                break;
            }
        
            pCxtLink = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory( sizeof(COSA_CONTEXT_LINK_OBJECT) );
            if ( !pCxtLink )
            {
                AnscFreeMemory(pOption);
                break;
            }
        
            COSA_CONTEXT_LINK_INITIATION_CONTENT(pCxtLink);
            pCxtLink->hContext       = (ANSC_HANDLE)pOption;
            pCxtLink->hParentTable   = (ANSC_HANDLE)pPoolCxtLink;
            pCxtLink->bNew           = FALSE;
        
            if ( !pOption->InstanceNumber )
            {
                if ( !++pPoolCxtLink->maxInstanceOfOption )
                {
                    pPoolCxtLink->maxInstanceOfOption = 1;
                }
                bNeedSave                        = TRUE;
                pOption->InstanceNumber = pPoolCxtLink->maxInstanceOfOption;
        
                _ansc_sprintf( pOption->Alias, "Option%u", pOption->InstanceNumber );
        
                returnStatus = CosaDmlDhcpsSetOptionValues
                                (
                                    NULL,
                                    pPool->Cfg.InstanceNumber,
                                    ulIndex2, 
                                    pOption->InstanceNumber, 
                                    pOption->Alias
                                );
        
                if ( returnStatus != ANSC_STATUS_SUCCESS )
                {
                    AnscFreeMemory(pOption);
                    AnscFreeMemory(pCxtLink);
                    break;                    
                }
                
                pCxtLink->InstanceNumber = pOption->InstanceNumber; 
        
                /* Put into our list */
                CosaSListPushEntryByInsNum(&pPoolCxtLink->OptionList, (PCOSA_CONTEXT_LINK_OBJECT)pCxtLink);
            } 
            else
            {
                pCxtLink->InstanceNumber = pOption->InstanceNumber; 
                
                /* This case never happen. Add it just for simulation code run well */
                if ( pPoolCxtLink->maxInstanceOfOption < pOption->InstanceNumber )
                {
                    pPoolCxtLink->maxInstanceOfOption = pOption->InstanceNumber;
                    bNeedSave                                = TRUE;
                }
        
                /* if this entry is in link tree already because it's  delay_added table */
                pCxtLink2 = (PCOSA_CONTEXT_LINK_OBJECT)CosaSListGetEntryByInsNum(&pPoolCxtLink->OptionList, pOption->InstanceNumber);
                if ( !pCxtLink2 )
                {
                    CosaSListPushEntryByInsNum(&pPoolCxtLink->OptionList, (PCOSA_CONTEXT_LINK_OBJECT)pCxtLink);
                }
                else
                {
                    AnscFreeMemory( pCxtLink2->hContext );                
                    pCxtLink2->hContext       = (ANSC_HANDLE)pOption;
                    if ( pCxtLink2->bNew )
                    {
                        pCxtLink2->bNew       = FALSE;
                        bNeedSave             = TRUE;
                    }
        
                    AnscFreeMemory(pCxtLink);
                    pCxtLink                  = pCxtLink2;
                    pCxtLink2                 = NULL;
                }            
        
            }
        }


    }

    /* Max InstanceNumber is changed. Save now.*/
    if (bNeedSave)
    {
        CosaDhcpv4RegSetDhcpv4Info(pDhcpv4);
    }


    return returnStatus;    
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        CosaDhcpv4RegGetDhcpv4Info
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve backend inform and put them into our trees.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of dhcpv4
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CosaDhcpv4RegGetDhcpv4Info
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DHCPV4          pMyObject         = (PCOSA_DATAMODEL_DHCPV4   )hThisObject;
    
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoDhcpv4 = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderDhcpv4;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoClient = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoReqOpt = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoSndOpt = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoX_COM_CISCO_SAddr = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoPool   = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoStaticAddr       = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoOption           = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoEnumClient       = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoEnumReqOpt       = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoEnumSndOpt       = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoEnumX_COM_CISCO_SAddr = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoEnumPool         = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoEnumStaticAddr   = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoEnumOption       = (PPOAM_IREP_FOLDER_OBJECT )NULL;

    PCOSA_CONTEXT_DHCPC_LINK_OBJECT pCosaDhcpcContext = NULL;
    PCOSA_CONTEXT_POOL_LINK_OBJECT  pCosaPoolContext = NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaX_COM_CISCO_SAddrContext = NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaReqOptionContext      = NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaSendOptionContext     = NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaStaticAddrContext     = NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaOptionContext         = NULL;
    PSLAP_VARIABLE                  pSlapVariable     = NULL;
    ULONG                           ulEntryCount      = 0;
    ULONG                           ulIndex           = 0;
    ULONG                           ulEntryCount2     = 0;
    ULONG                           ulIndex2          = 0;
    ULONG                           uInstanceNumber   = 0;
    BOOL                            bNew              = FALSE;
    char*                           pAliasClient      = NULL;
    char*                           pAliasReqOption   = NULL;
    char*                           pAliasSendOption  = NULL;
    char*                           pAliasX_COM_CISCO_SAddr = NULL;
    char*                           pAliasPool        = NULL;
    char*                           pAliasStaticAddr  = NULL;
    char*                           pAliasOption      = NULL;
    char*                           pFolderName       = NULL;
    
    PCOSA_DML_DHCPC_FULL            pDhcpv4Client     = NULL;
    PCOSA_DML_DHCPC_REQ_OPT         pDhcpv4ReqOpt     = NULL;
    PCOSA_DML_DHCP_OPT              pDhcpv4SndOpt     = NULL;
    PCOSA_DML_DHCPS_X_CISCO_COM_SADDR pDhcpv4X_COM_CISCO_SAddr = NULL;
    PCOSA_DML_DHCPS_POOL_FULL       pDhcpv4Pool       = NULL;
    PCOSA_DML_DHCPS_SADDR           pDhcpv4StaticAddr = NULL;
    PCOSA_DML_DHCPSV4_OPTION        pDhcpv4Option     = NULL;

    if ( !pPoamIrepFoDhcpv4 )
    {
        return ANSC_STATUS_FAILURE;
    }

    /* This is saved structure for dhcpv4
        *****************************************
              <Dhcpv4>
                  <client>
                      <NextInstanceNumber> xxx </>
                      <1>
                           <alias>xxx</>
                           <bNew>false</>
                           <ReqOption>
                               <NextInstanceNumber> xxx </>
                               <1>
                                   <alias>xxx</>
                                   <bNew>true</>
                               </1>
                           </ReqOption>
                          <SendOption>
                              <NextInstanceNumber> xxx </>
                              <1>
                                  <alias>xxx</>
                                  <bNew>true</>
                              </1>
                          </SendOption>
                     </1>
               </client>
              <pool>
                    <NextInstanceNumber> xxx </>
                    <1>
                        <alias>xxx</>
                        <bNew>true</>
                        <staticAddress>
                            <NextInstanceNumber> xxx </>
                            <1>
                                <alias>xxx</>
                                <bNew>true</>
                            </1>
                        </staticAddress>
                       <option>
                           <NextInstanceNumber> xxx </>
                           <1>
                               <alias>xxx</>
                               <bNew>true</>
                           </1>
                       </option>
                    </1>
              </pool>
            </Dhcpv4>
      ****************************************************
      */

    /* Get Folder Client */ 
    pPoamIrepFoClient  = 
        (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoDhcpv4->GetFolder
            (
                (ANSC_HANDLE)pPoamIrepFoDhcpv4,
                COSA_IREP_FOLDER_NAME_DHCPV4_CLIENT
            );

    if ( !pPoamIrepFoClient )
    {
        returnStatus      = ANSC_STATUS_FAILURE;
        goto EXIT1;
    }

    /* Get Maximum number */
    if ( TRUE )
    {
        pSlapVariable =
            (PSLAP_VARIABLE)pPoamIrepFoClient->GetRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoClient,
                    COSA_DML_RR_NAME_Dhcpv4NextInsNunmber,
                    NULL
                );

        if ( pSlapVariable )
        {
            pMyObject->maxInstanceOfClient = pSlapVariable->Variant.varUint32;

            SlapFreeVariable(pSlapVariable);
        }
    }

    /* enumerate client.{i} */
    ulEntryCount = pPoamIrepFoClient->GetFolderCount((ANSC_HANDLE)pPoamIrepFoClient);
    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        /* Get i in client.{i} */
        pFolderName =
            pPoamIrepFoClient->EnumFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoClient,
                    ulIndex
                );

        if ( !pFolderName )
        {
            continue;
        }

        uInstanceNumber = _ansc_atol(pFolderName);

        if ( uInstanceNumber == 0 )
        {
            AnscFreeMemory(pFolderName);
            continue;
        }

        pPoamIrepFoEnumClient = pPoamIrepFoClient->GetFolder((ANSC_HANDLE)pPoamIrepFoClient, pFolderName);

        AnscFreeMemory(pFolderName);

        if ( !pPoamIrepFoEnumClient )
        {
            continue;
        }

        /* Get client.{i}.Alias value*/
        if ( TRUE )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoEnumClient->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEnumClient,
                        COSA_DML_RR_NAME_Dhcpv4Alias,
                        NULL
                    );

            if ( pSlapVariable )
            {
                pAliasClient = AnscCloneString(pSlapVariable->Variant.varString);

                SlapFreeVariable(pSlapVariable);
            }
        }

        /* Get client.{i}.bNew value*/
        if ( TRUE )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoEnumClient->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEnumClient,
                        COSA_DML_RR_NAME_Dhcpv4bNew,
                        NULL
                    );

            if ( pSlapVariable )
            {
                bNew = pSlapVariable->Variant.varBool;

                SlapFreeVariable(pSlapVariable);
            }
            else
            {
                bNew = TRUE;
            }
        }

        /* Create one entry and keep this delay_added entry */
        pDhcpv4Client = (PCOSA_DML_DHCPC_FULL)AnscAllocateMemory(sizeof(COSA_DML_DHCPC_FULL));
        if ( !pDhcpv4Client )
        {
            returnStatus = ANSC_STATUS_FAILURE;            
            goto EXIT2;
        }

        /* set some default value firstly */
        DHCPV4_CLIENT_SET_DEFAULTVALUE(pDhcpv4Client);

        /* save alias and instanceNumber */
        pDhcpv4Client->Cfg.InstanceNumber = uInstanceNumber;
        AnscCopyString( pDhcpv4Client->Cfg.Alias, pAliasClient );

        /* Create one link point */
        pCosaDhcpcContext = (PCOSA_CONTEXT_DHCPC_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_DHCPC_LINK_OBJECT));
        if ( !pCosaDhcpcContext )
        {
            returnStatus = ANSC_STATUS_FAILURE;
            goto EXIT3;
        }

        DHCPV4_CLIENT_INITIATION_CONTEXT(pCosaDhcpcContext)

        pCosaDhcpcContext->InstanceNumber = uInstanceNumber;
        pCosaDhcpcContext->hContext       = (ANSC_HANDLE)pDhcpv4Client;
        pDhcpv4Client                     = 0;
        pCosaDhcpcContext->bNew           = bNew; /* set to true */

        CosaSListPushEntryByInsNum(&pMyObject->ClientList, (PCOSA_CONTEXT_LINK_OBJECT)pCosaDhcpcContext);

        /*************************************
               * Begin treat client.{i}.reqOption.                *
               *************************************/
        pPoamIrepFoReqOpt = 
            (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoEnumClient->GetFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoEnumClient,
                    COSA_IREP_FOLDER_NAME_DHCPV4_REQOPTION
                );
        
        if ( !pPoamIrepFoReqOpt )
        {
            goto SentOption;
        }
        
        /* Get Maximum number */
        if ( TRUE )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoReqOpt->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoReqOpt,
                        COSA_DML_RR_NAME_Dhcpv4NextInsNunmber,
                        NULL
                    );
        
            if ( pSlapVariable )
            {
                pCosaDhcpcContext->maxInstanceOfReq = pSlapVariable->Variant.varUint32;
        
                SlapFreeVariable(pSlapVariable);
            }
        }

        /* enumerate client.{i}.reqOption.{i} */
        ulEntryCount2 = pPoamIrepFoReqOpt->GetFolderCount((ANSC_HANDLE)pPoamIrepFoReqOpt);

        for ( ulIndex2 = 0; ulIndex2 < ulEntryCount2; ulIndex2++ )
        {
            /* Get i in client.{i}.reqOption.{i} */
            pFolderName =
                pPoamIrepFoReqOpt->EnumFolder
                    (
                        (ANSC_HANDLE)pPoamIrepFoReqOpt,
                        ulIndex2
                    );
            
            if ( !pFolderName )
            {
                continue;
            }
            
            uInstanceNumber = _ansc_atol(pFolderName);
            
            if ( uInstanceNumber == 0 )
            {
                AnscFreeMemory(pFolderName); /* tom*/
                continue;
            }

            pPoamIrepFoEnumReqOpt = pPoamIrepFoReqOpt->GetFolder((ANSC_HANDLE)pPoamIrepFoReqOpt, pFolderName);

            AnscFreeMemory(pFolderName);
            
            if ( !pPoamIrepFoEnumReqOpt )
            {
                continue;
            }
            
            /* Get client.{i}.reqOption.{i}.Alias value*/
            if ( TRUE )
            {
                pSlapVariable =
                    (PSLAP_VARIABLE)pPoamIrepFoEnumReqOpt->GetRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoEnumReqOpt,
                            COSA_DML_RR_NAME_Dhcpv4Alias,
                            NULL
                        );
            
                if ( pSlapVariable )
                {
                    pAliasReqOption= AnscCloneString(pSlapVariable->Variant.varString);
            
                    SlapFreeVariable(pSlapVariable);
                }
            }
            
            /* Get client.{i}.reqOption.{i}.bNew value*/
            if ( TRUE )
            {
                pSlapVariable =
                    (PSLAP_VARIABLE)pPoamIrepFoEnumReqOpt->GetRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoEnumReqOpt,
                            COSA_DML_RR_NAME_Dhcpv4bNew,
                            NULL
                        );
            
                if ( pSlapVariable )
                {
                    bNew = pSlapVariable->Variant.varBool;
            
                    SlapFreeVariable(pSlapVariable);
                }
                else
                {
                    bNew = TRUE;
                }
            }
            

            /* Create one link and ask backend to get content */
            pDhcpv4ReqOpt = (PCOSA_DML_DHCPC_REQ_OPT)AnscAllocateMemory(sizeof(COSA_DML_DHCPC_REQ_OPT));
            if ( !pDhcpv4ReqOpt )
            {
                returnStatus = ANSC_STATUS_FAILURE;                
                goto EXIT3;
            }

            /* set some default value firstly */
            DHCPV4_REQOPTION_SET_DEFAULTVALUE(pDhcpv4ReqOpt);

            /* save alias and instanceNumber */
            pDhcpv4ReqOpt->InstanceNumber = uInstanceNumber;
            AnscCopyString( pDhcpv4ReqOpt->Alias, pAliasReqOption );

            /* Create one link */
            pCosaReqOptionContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
            if ( !pCosaReqOptionContext )
            {
                returnStatus = ANSC_STATUS_FAILURE;
                AnscFreeMemory( pDhcpv4ReqOpt ); /*RDKB-6735, CID-33487, free unused resource before exit*/
                pDhcpv4ReqOpt = NULL;
                goto EXIT2;
            }

            COSA_CONTEXT_LINK_INITIATION_CONTENT(pCosaReqOptionContext);

            pCosaReqOptionContext->InstanceNumber = uInstanceNumber;
            pCosaReqOptionContext->hContext       = (ANSC_HANDLE)pDhcpv4ReqOpt;
            pDhcpv4ReqOpt                         = NULL;
            pCosaReqOptionContext->hParentTable   = (ANSC_HANDLE)pCosaDhcpcContext;
            pCosaReqOptionContext->bNew           = bNew;
            
            CosaSListPushEntryByInsNum(&pCosaDhcpcContext->ReqOptionList, (PCOSA_CONTEXT_LINK_OBJECT)pCosaReqOptionContext);

            /* release some memory */
            if (pAliasReqOption)
            {
                AnscFreeMemory(pAliasReqOption);
                pAliasReqOption = NULL;
            }
            
            pPoamIrepFoEnumReqOpt->Remove((ANSC_HANDLE)pPoamIrepFoEnumReqOpt);
            pPoamIrepFoEnumReqOpt = NULL;
        }

        pPoamIrepFoReqOpt->Remove((ANSC_HANDLE)pPoamIrepFoReqOpt);
        pPoamIrepFoReqOpt = NULL;

SentOption:
        /* 
                   Begin treat client.{i}.sentOption. 
                */
        pPoamIrepFoSndOpt = 
            (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoEnumClient->GetFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoEnumClient,
                    COSA_IREP_FOLDER_NAME_DHCPV4_SENTOPTION
                );
        
        if ( !pPoamIrepFoSndOpt )
        {
            goto ClientEnd;
        }
        
        /* Get Maximum number */
        if ( TRUE )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoSndOpt->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoSndOpt,
                        COSA_DML_RR_NAME_Dhcpv4NextInsNunmber,
                        NULL
                    );
        
            if ( pSlapVariable )
            {
                pCosaDhcpcContext->maxInstanceOfSend = pSlapVariable->Variant.varUint32;
        
                SlapFreeVariable(pSlapVariable);
            }
        }
        
        /* enumerate client.{i}.sentOption.{i} */
        ulEntryCount2 = pPoamIrepFoSndOpt->GetFolderCount((ANSC_HANDLE)pPoamIrepFoSndOpt);

        for ( ulIndex2 = 0; ulIndex2 < ulEntryCount2; ulIndex2++ )
        {
            /* Get i in client.{i}.sentOption.{i} */
            pFolderName =
                pPoamIrepFoSndOpt->EnumFolder
                    (
                        (ANSC_HANDLE)pPoamIrepFoSndOpt,
                        ulIndex2
                    );
            
            if ( !pFolderName )
            {
                continue;
            }
            
            uInstanceNumber = _ansc_atol(pFolderName);
            
            if ( uInstanceNumber == 0 )
            {
                AnscFreeMemory(pFolderName); /* tom*/
                continue;
            }
        
            pPoamIrepFoEnumSndOpt = pPoamIrepFoSndOpt->GetFolder((ANSC_HANDLE)pPoamIrepFoSndOpt, pFolderName);
        
            AnscFreeMemory(pFolderName);
            
            if ( !pPoamIrepFoEnumSndOpt )
            {
                continue;
            }
            
            /* Get client.{i}.sentOption.{i}.Alias value*/
            if ( TRUE )
            {
                pSlapVariable =
                    (PSLAP_VARIABLE)pPoamIrepFoEnumSndOpt->GetRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoEnumSndOpt,
                            COSA_DML_RR_NAME_Dhcpv4Alias,
                            NULL
                        );
            
                if ( pSlapVariable )
                {
                    pAliasSendOption = AnscCloneString(pSlapVariable->Variant.varString);
            
                    SlapFreeVariable(pSlapVariable);
                }
            }
            
            /* Get client.{i}.sentOption.{i}.bNew value*/
            if ( TRUE )
            {
                pSlapVariable =
                    (PSLAP_VARIABLE)pPoamIrepFoEnumSndOpt->GetRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoEnumSndOpt,
                            COSA_DML_RR_NAME_Dhcpv4bNew,
                            NULL
                        );
            
                if ( pSlapVariable )
                {
                    bNew = pSlapVariable->Variant.varBool;
            
                    SlapFreeVariable(pSlapVariable);
                }
                else
                {
                    bNew = TRUE;
                }
            }

            /* Create one link and ask backend to get content */
            pDhcpv4SndOpt = (PCOSA_DML_DHCP_OPT)AnscAllocateMemory(sizeof(COSA_DML_DHCP_OPT));
            if ( !pDhcpv4SndOpt )
            {
                returnStatus = ANSC_STATUS_FAILURE;                
                goto EXIT3;
            }

            /* set some default value firstly */
            DHCPV4_SENDOPTION_SET_DEFAULTVALUE(pDhcpv4SndOpt);

            /* save alias and instanceNumber */
            pDhcpv4SndOpt->InstanceNumber = uInstanceNumber;
            AnscCopyString( pDhcpv4SndOpt->Alias, pAliasSendOption );

            pCosaSendOptionContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
            if ( !pCosaSendOptionContext )
            {
                AnscFreeMemory(pDhcpv4SndOpt); /*RDKB-6735, CID-33261, free unused resource before exit*/
                pDhcpv4SndOpt = NULL;
                returnStatus = ANSC_STATUS_FAILURE;            
                goto EXIT2;
            }

            COSA_CONTEXT_LINK_INITIATION_CONTENT(pCosaSendOptionContext);

            pCosaSendOptionContext->InstanceNumber = uInstanceNumber;
            pCosaSendOptionContext->hContext       = (ANSC_HANDLE)pDhcpv4SndOpt;
            pDhcpv4SndOpt = NULL;
            pCosaSendOptionContext->hParentTable   = pCosaDhcpcContext;
            pCosaSendOptionContext->bNew           = bNew;

            CosaSListPushEntryByInsNum(&pCosaDhcpcContext->SendOptionList, (PCOSA_CONTEXT_LINK_OBJECT)pCosaSendOptionContext);
        
            /* release some memory */
            
            if (pAliasSendOption)
            {
                AnscFreeMemory(pAliasSendOption);
                pAliasSendOption = NULL;
            }
            
            pPoamIrepFoEnumSndOpt->Remove((ANSC_HANDLE)pPoamIrepFoEnumSndOpt);
            pPoamIrepFoEnumSndOpt = NULL;
        }

        pPoamIrepFoSndOpt->Remove((ANSC_HANDLE)pPoamIrepFoSndOpt);
        pPoamIrepFoSndOpt = NULL;

ClientEnd:

        /* release some memory */
        if (pAliasClient)
        {
            AnscFreeMemory(pAliasClient);
            pAliasClient = NULL;
        }
        
        pPoamIrepFoEnumClient->Remove((ANSC_HANDLE)pPoamIrepFoEnumClient);
        pPoamIrepFoEnumClient = NULL;
    }

    pPoamIrepFoClient->Remove((ANSC_HANDLE)pPoamIrepFoClient);
    pPoamIrepFoClient = NULL;


    /*
      Begin process Server.X_COM_CISCO_StaticAddress.{j}
    */
        
    /* Get Folder */ 
    pPoamIrepFoX_COM_CISCO_SAddr  = 
        (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoDhcpv4->GetFolder
            (
                (ANSC_HANDLE)pPoamIrepFoDhcpv4,
                COSA_IREP_FOLDER_NAME_DHCPV4_X_CISCO_COM_SADDR
            );
    
    if ( !pPoamIrepFoX_COM_CISCO_SAddr )
    {
        returnStatus      = ANSC_STATUS_FAILURE;
        goto EXIT1;
    }
    
    /* Get Maximum number */
    if ( TRUE )
    {
        pSlapVariable =
            (PSLAP_VARIABLE)pPoamIrepFoX_COM_CISCO_SAddr->GetRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoX_COM_CISCO_SAddr,
                    COSA_DML_RR_NAME_Dhcpv4NextInsNunmber,
                    NULL
                );
    
        if ( pSlapVariable )
        {
            pMyObject->maxInstanceX_CISCO_COM_SAddr = pSlapVariable->Variant.varUint32;
    
            SlapFreeVariable(pSlapVariable);
        }
    }


    /* enumerate server.X_COM_CISCO_StaticAddress.{i} */
    ulEntryCount = pPoamIrepFoX_COM_CISCO_SAddr->GetFolderCount((ANSC_HANDLE)pPoamIrepFoX_COM_CISCO_SAddr);
    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        /* Get i in server.X_COM_CISCO_StaticAddress.{i} */
        pFolderName =
            pPoamIrepFoX_COM_CISCO_SAddr->EnumFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoX_COM_CISCO_SAddr,
                    ulIndex
                );
    
        if ( !pFolderName )
        {
            continue;
        }
    
        uInstanceNumber = _ansc_atol(pFolderName);
    
        if ( uInstanceNumber == 0 )
        {
            AnscFreeMemory(pFolderName); /* tom*/
            continue;
        }
    
        pPoamIrepFoEnumX_COM_CISCO_SAddr = pPoamIrepFoX_COM_CISCO_SAddr->GetFolder((ANSC_HANDLE)pPoamIrepFoX_COM_CISCO_SAddr, pFolderName);
    
        AnscFreeMemory(pFolderName);
    
        if ( !pPoamIrepFoEnumX_COM_CISCO_SAddr )
        {
            continue;
        }
    
        /* Get server.X_COM_CISCO_StaticAddress.{i}.Alias value*/
        if ( TRUE )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoEnumX_COM_CISCO_SAddr->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEnumX_COM_CISCO_SAddr,
                        COSA_DML_RR_NAME_Dhcpv4Alias,
                        NULL
                    );
    
            if ( pSlapVariable )
            {
                pAliasX_COM_CISCO_SAddr = AnscCloneString(pSlapVariable->Variant.varString);
    
                SlapFreeVariable(pSlapVariable);
            }
        }

        /* Get server.X_COM_CISCO_StaticAddress.{i}.bNew value*/
        if ( TRUE )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoEnumX_COM_CISCO_SAddr->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEnumX_COM_CISCO_SAddr,
                        COSA_DML_RR_NAME_Dhcpv4bNew,
                        NULL
                    );

            if ( pSlapVariable )
            {
                bNew = pSlapVariable->Variant.varBool;
        
                SlapFreeVariable(pSlapVariable);
            }
            else
            {
                bNew = TRUE;
            }
        }
        
        /* Create one link and ask backend to get content */
        pDhcpv4X_COM_CISCO_SAddr = (PCOSA_DML_DHCPS_X_CISCO_COM_SADDR)AnscAllocateMemory(sizeof(COSA_DML_DHCPS_X_CISCO_COM_SADDR));
        if ( !pDhcpv4X_COM_CISCO_SAddr )
        {
            returnStatus = ANSC_STATUS_FAILURE;            
            goto EXIT2;
        }
    
        /* set some default value firstly */
        DHCPV4_X_COM_CISCO_SADDR_SET_DEFAULTVALUE(pDhcpv4X_COM_CISCO_SAddr);
        
        /* save alias and instanceNumber */
        pDhcpv4X_COM_CISCO_SAddr->InstanceNumber = uInstanceNumber;
        AnscCopyString( pDhcpv4X_COM_CISCO_SAddr->Alias, pAliasX_COM_CISCO_SAddr );

        pCosaX_COM_CISCO_SAddrContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
        if ( !pCosaX_COM_CISCO_SAddrContext )
        {
            returnStatus = ANSC_STATUS_FAILURE;
            AnscFreeMemory( pDhcpv4X_COM_CISCO_SAddr ); /*RDKB-6735, CID-33543, free unused resource before exit*/
            pDhcpv4X_COM_CISCO_SAddr = NULL;

            if( pAliasX_COM_CISCO_SAddr )
            {
                AnscFreeMemory( pAliasX_COM_CISCO_SAddr ); /*RDKB-6735, CID-33425, free unused resource before exit*/
                pAliasX_COM_CISCO_SAddr = NULL;
            }

            goto EXIT3;
        }
    
        pCosaX_COM_CISCO_SAddrContext->InstanceNumber = uInstanceNumber;
        pCosaX_COM_CISCO_SAddrContext->hContext       = (ANSC_HANDLE)pDhcpv4X_COM_CISCO_SAddr;
        pCosaX_COM_CISCO_SAddrContext->hParentTable   = NULL;
        pCosaX_COM_CISCO_SAddrContext->bNew           = bNew;
    
        CosaSListPushEntryByInsNum(&pMyObject->X_CISCO_COM_StaticAddressList, (PCOSA_CONTEXT_LINK_OBJECT)pCosaX_COM_CISCO_SAddrContext);

        /* release some memory */
        if (pAliasX_COM_CISCO_SAddr)
        {
            AnscFreeMemory(pAliasX_COM_CISCO_SAddr);
            pAliasX_COM_CISCO_SAddr = NULL;
        }

        pPoamIrepFoEnumX_COM_CISCO_SAddr->Remove((ANSC_HANDLE)pPoamIrepFoEnumX_COM_CISCO_SAddr);
        pPoamIrepFoEnumX_COM_CISCO_SAddr = NULL;
    }

    pPoamIrepFoX_COM_CISCO_SAddr->Remove((ANSC_HANDLE)pPoamIrepFoX_COM_CISCO_SAddr);
    pPoamIrepFoX_COM_CISCO_SAddr = NULL;

    /*
      Begin process pool.{i}, and pool.{i}.StaticAddress.{j}
    */
        
    /* Get Folder Client */ 
    pPoamIrepFoPool  = 
        (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoDhcpv4->GetFolder
            (
                (ANSC_HANDLE)pPoamIrepFoDhcpv4,
                COSA_IREP_FOLDER_NAME_DHCPV4_POOL
            );
    
    if ( !pPoamIrepFoPool )
    {
        returnStatus      = ANSC_STATUS_FAILURE;
        goto EXIT1;
    }
    
    /* Get Maximum number */
    if ( TRUE )
    {
        pSlapVariable =
            (PSLAP_VARIABLE)pPoamIrepFoPool->GetRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoPool,
                    COSA_DML_RR_NAME_Dhcpv4NextInsNunmber,
                    NULL
                );
    
        if ( pSlapVariable )
        {
            pMyObject->maxInstanceOfPool = pSlapVariable->Variant.varUint32;
    
            SlapFreeVariable(pSlapVariable);
        }
    }
    
    /* enumerate server.pool.{i} */
    ulEntryCount = pPoamIrepFoPool->GetFolderCount((ANSC_HANDLE)pPoamIrepFoPool);
    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        /* Get i in pool.{i} */
        pFolderName =
            pPoamIrepFoPool->EnumFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoPool,
                    ulIndex
                );
    
        if ( !pFolderName )
        {
            continue;
        }
    
        uInstanceNumber = _ansc_atol(pFolderName);
    
        if ( uInstanceNumber == 0 )
        {
            AnscFreeMemory(pFolderName); /* tom*/
            continue;
        }
    
        pPoamIrepFoEnumPool = pPoamIrepFoPool->GetFolder((ANSC_HANDLE)pPoamIrepFoPool, pFolderName);
    
        AnscFreeMemory(pFolderName);
    
        if ( !pPoamIrepFoEnumPool )
        {
            continue;
        }
    
        /* Get pool.{i}.Alias value*/
        if ( TRUE )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoEnumPool->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEnumPool,
                        COSA_DML_RR_NAME_Dhcpv4Alias,
                        NULL
                    );
    
            if ( pSlapVariable )
            {
                pAliasPool = AnscCloneString(pSlapVariable->Variant.varString);
    
                SlapFreeVariable(pSlapVariable);
            }
        }

        /* Get pool.{i}.bNew value*/
        if ( TRUE )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoEnumPool->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEnumPool,
                        COSA_DML_RR_NAME_Dhcpv4bNew,
                        NULL
                    );

            if ( pSlapVariable )
            {
                bNew = pSlapVariable->Variant.varBool;
        
                SlapFreeVariable(pSlapVariable);
            }
            else
            {
                bNew = TRUE;
            }
        }
        
        /* Create one link and ask backend to get content */
        pDhcpv4Pool = (PCOSA_DML_DHCPS_POOL_FULL)AnscAllocateMemory(sizeof(COSA_DML_DHCPS_POOL_FULL));
        if ( !pDhcpv4Pool )
        {
            returnStatus = ANSC_STATUS_FAILURE;            
            goto EXIT2;
        }
    
        /* set some default value firstly */
        DHCPV4_POOL_SET_DEFAULTVALUE(pDhcpv4Pool);
        
        /* save alias and instanceNumber */
        pDhcpv4Pool->Cfg.InstanceNumber = uInstanceNumber;
        AnscCopyString( pDhcpv4Pool->Cfg.Alias, pAliasPool );

        pCosaPoolContext = (PCOSA_CONTEXT_POOL_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_POOL_LINK_OBJECT));
        if ( !pCosaPoolContext )
        {
            returnStatus = ANSC_STATUS_FAILURE;
            goto EXIT3;
        }
    
        DHCPV4_POOL_INITIATION_CONTEXT(pCosaPoolContext)
    
        pCosaPoolContext->InstanceNumber = uInstanceNumber;
        pCosaPoolContext->hContext       = (ANSC_HANDLE)pDhcpv4Pool;
        pDhcpv4Pool  = 0;
        pCosaPoolContext->hParentTable   = NULL;
        pCosaPoolContext->bNew           = bNew;
    
        CosaSListPushEntryByInsNum(&pMyObject->PoolList, (PCOSA_CONTEXT_LINK_OBJECT)pCosaPoolContext);

        /*************************************
               * Begin treat pool.{i}.StaticAddress.            *
               *************************************/
        pPoamIrepFoStaticAddr = 
            (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoEnumPool->GetFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoEnumPool,
                    COSA_IREP_FOLDER_NAME_DHCPV4_STATICADDRESS
                );
        
        if ( !pPoamIrepFoStaticAddr )
        {
            goto PoolEnd;
        }
        
        /* Get Maximum number */
        if ( TRUE )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoStaticAddr->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoStaticAddr,
                        COSA_DML_RR_NAME_Dhcpv4NextInsNunmber,
                        NULL
                    );
        
            if ( pSlapVariable )
            {
                pCosaPoolContext->maxInstanceOfStaticAddress = pSlapVariable->Variant.varUint32;
        
                SlapFreeVariable(pSlapVariable);
            }
        }
        
        /* enumerate pool.{i}.staticAddress.{i} */
        ulEntryCount2 = pPoamIrepFoStaticAddr->GetFolderCount((ANSC_HANDLE)pPoamIrepFoStaticAddr);

        for ( ulIndex2 = 0; ulIndex2 < ulEntryCount2; ulIndex2++ )
        {
            /* Get i in client.{i}.reqOption.{i} */
            pFolderName =
                pPoamIrepFoStaticAddr->EnumFolder
                    (
                        (ANSC_HANDLE)pPoamIrepFoStaticAddr,
                        ulIndex2
                    );
            
            if ( !pFolderName )
            {
                continue;
            }
            
            uInstanceNumber = _ansc_atol(pFolderName);
            
            if ( uInstanceNumber == 0 )
            {
                AnscFreeMemory(pFolderName); /* tom*/
                continue;
            }
    
            pPoamIrepFoEnumStaticAddr = pPoamIrepFoStaticAddr->GetFolder((ANSC_HANDLE)pPoamIrepFoStaticAddr, pFolderName);
    
            AnscFreeMemory(pFolderName);
            
            if ( !pPoamIrepFoEnumStaticAddr )
            {
                continue;
            }
            
            /* Get pool.{i}.staticAddress.{i}.Alias value*/
            if ( TRUE )
            {
                pSlapVariable =
                    (PSLAP_VARIABLE)pPoamIrepFoEnumStaticAddr->GetRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoEnumStaticAddr,
                            COSA_DML_RR_NAME_Dhcpv4Alias,
                            NULL
                        );
            
                if ( pSlapVariable )
                {
                    pAliasStaticAddr = AnscCloneString(pSlapVariable->Variant.varString);
            
                    SlapFreeVariable(pSlapVariable);
                }
            }

            /* Get pool.{i}.bNew value*/
            if ( TRUE )
            {
                pSlapVariable =
                    (PSLAP_VARIABLE)pPoamIrepFoEnumStaticAddr->GetRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoEnumStaticAddr,
                            COSA_DML_RR_NAME_Dhcpv4bNew,
                            NULL
                        );
            
                if ( pSlapVariable )
                {
                    bNew = pSlapVariable->Variant.varBool;
            
                    SlapFreeVariable(pSlapVariable);
                }
                else
                {
                    bNew = TRUE;
                }
            }
            
            /* Create one link and ask backend to get content */
            pDhcpv4StaticAddr = (PCOSA_DML_DHCPS_SADDR)AnscAllocateMemory(sizeof(COSA_DML_DHCPS_SADDR ));
            if ( !pDhcpv4StaticAddr )
            {
                returnStatus = ANSC_STATUS_FAILURE;                
                goto EXIT3;
            }
    
            /* set some default value firstly */
            DHCPV4_STATICADDRESS_SET_DEFAULTVALUE(pDhcpv4StaticAddr);
            
            /* save alias and instanceNumber */
            pDhcpv4StaticAddr->InstanceNumber = uInstanceNumber;
            AnscCopyString( pDhcpv4StaticAddr->Alias, pAliasStaticAddr );
    
            pCosaStaticAddrContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
            if ( !pCosaStaticAddrContext )
            {
                AnscFreeMemory(pDhcpv4StaticAddr);/*RDKB-6735, CID-33116, free unused resource before exit*/
                pDhcpv4StaticAddr = NULL;
                returnStatus = ANSC_STATUS_FAILURE;            
                goto EXIT2;
            }

            COSA_CONTEXT_LINK_INITIATION_CONTENT(pCosaStaticAddrContext)

            pCosaStaticAddrContext->InstanceNumber = uInstanceNumber;
            pCosaStaticAddrContext->hContext       = (ANSC_HANDLE)pDhcpv4StaticAddr;
            pDhcpv4StaticAddr = NULL;
            pCosaStaticAddrContext->hParentTable   = pCosaPoolContext;
            pCosaStaticAddrContext->bNew           = bNew;

            CosaSListPushEntryByInsNum(&pCosaPoolContext->StaticAddressList, (PCOSA_CONTEXT_LINK_OBJECT)pCosaStaticAddrContext);

            /* release some memory */
            if (pAliasStaticAddr)
            {
                AnscFreeMemory(pAliasStaticAddr);
                pAliasStaticAddr = NULL;
            }
            
            pPoamIrepFoEnumStaticAddr->Remove((ANSC_HANDLE)pPoamIrepFoEnumStaticAddr);
            pPoamIrepFoEnumStaticAddr = NULL;
        }
    
        pPoamIrepFoStaticAddr->Remove((ANSC_HANDLE)pPoamIrepFoStaticAddr);
        pPoamIrepFoStaticAddr = NULL;

        /*************************************
               * Begin treat pool.{i}.Option.            *
               *************************************/
        pPoamIrepFoOption = 
            (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoEnumPool->GetFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoEnumPool,
                    COSA_IREP_FOLDER_NAME_DHCPV4_OPTION
                );
        
        if ( !pPoamIrepFoOption )
        {
            goto PoolEnd;
        }
        
        /* Get Maximum number */
        if ( TRUE )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoOption->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoOption,
                        COSA_DML_RR_NAME_Dhcpv4NextInsNunmber,
                        NULL
                    );
        
            if ( pSlapVariable )
            {
                pCosaPoolContext->maxInstanceOfOption = pSlapVariable->Variant.varUint32;
        
                SlapFreeVariable(pSlapVariable);
            }
        }
        
        /* enumerate pool.{i}.Option.{i} */
        ulEntryCount2 = pPoamIrepFoOption->GetFolderCount((ANSC_HANDLE)pPoamIrepFoOption);

        for ( ulIndex2 = 0; ulIndex2 < ulEntryCount2; ulIndex2++ )
        {
            /* Get i in pool.{i}.Option.{i} */
            pFolderName =
                pPoamIrepFoOption->EnumFolder
                    (
                        (ANSC_HANDLE)pPoamIrepFoOption,
                        ulIndex2
                    );
            
            if ( !pFolderName )
            {
                continue;
            }
            
            uInstanceNumber = _ansc_atol(pFolderName);
            
            if ( uInstanceNumber == 0 )
            {
                AnscFreeMemory(pFolderName);
                continue;
            }

            pPoamIrepFoEnumOption = pPoamIrepFoOption->GetFolder((ANSC_HANDLE)pPoamIrepFoOption, pFolderName);

            AnscFreeMemory(pFolderName);
            
            if ( !pPoamIrepFoEnumOption )
            {
                continue;
            }
            
            /* Get pool.{i}.Option.{i}.Alias value*/
            if ( TRUE )
            {
                pSlapVariable =
                    (PSLAP_VARIABLE)pPoamIrepFoEnumOption->GetRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoEnumOption,
                            COSA_DML_RR_NAME_Dhcpv4Alias,
                            NULL
                        );
            
                if ( pSlapVariable )
                {
                    pAliasOption = AnscCloneString(pSlapVariable->Variant.varString);
            
                    SlapFreeVariable(pSlapVariable);
                }
            }

            /* Get pool.{i}.bNew value*/
            if ( TRUE )
            {
                pSlapVariable =
                    (PSLAP_VARIABLE)pPoamIrepFoEnumOption->GetRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoEnumOption,
                            COSA_DML_RR_NAME_Dhcpv4bNew,
                            NULL
                        );
            
                if ( pSlapVariable )
                {
                    bNew = pSlapVariable->Variant.varBool;
            
                    SlapFreeVariable(pSlapVariable);
                }
                else
                {
                    bNew = TRUE;
                }
            }
            
            /* Create one link and ask backend to get content */
            pDhcpv4Option = (PCOSA_DML_DHCPSV4_OPTION)AnscAllocateMemory(sizeof(COSA_DML_DHCPSV4_OPTION ));
            if ( !pDhcpv4Option )
            {
                returnStatus = ANSC_STATUS_FAILURE;                
                goto EXIT3;
            }

            /* set some default value firstly */
            DHCPV4_OPTION_SET_DEFAULTVALUE(pDhcpv4Option);
            
            /* save alias and instanceNumber */
            pDhcpv4Option->InstanceNumber = uInstanceNumber;
            AnscCopyString( pDhcpv4Option->Alias, pAliasOption );

            pCosaOptionContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
            if ( !pCosaOptionContext )
            {
                AnscFreeMemory(pDhcpv4Option); /*RDKB-6735, CID-33374, free unused resource before exit*/
                pDhcpv4Option = NULL;
                returnStatus = ANSC_STATUS_FAILURE;            
                goto EXIT2;
            }

            COSA_CONTEXT_LINK_INITIATION_CONTENT(pCosaOptionContext)

            pCosaOptionContext->InstanceNumber = uInstanceNumber;
            pCosaOptionContext->hContext       = (ANSC_HANDLE)pDhcpv4Option;
            pDhcpv4Option = NULL;
            pCosaOptionContext->hParentTable   = pCosaPoolContext;
            pCosaOptionContext->bNew           = bNew;

            CosaSListPushEntryByInsNum(&pCosaPoolContext->OptionList, (PCOSA_CONTEXT_LINK_OBJECT)pCosaOptionContext);

            /* release some memory */
            if (pAliasOption)
            {
                AnscFreeMemory(pAliasOption);
                pAliasOption = NULL;
            }
            
            pPoamIrepFoEnumOption->Remove((ANSC_HANDLE)pPoamIrepFoEnumOption);
            pPoamIrepFoEnumOption = NULL;
        }

        pPoamIrepFoOption->Remove((ANSC_HANDLE)pPoamIrepFoOption);
        pPoamIrepFoOption = NULL;
            

PoolEnd:

        /* release some memory */
        if (pAliasPool)
        {
            AnscFreeMemory(pAliasPool);
            pAliasPool = NULL;
        }
        
        pPoamIrepFoEnumPool->Remove((ANSC_HANDLE)pPoamIrepFoEnumPool);
        pPoamIrepFoEnumPool = NULL;
    }
    
    pPoamIrepFoPool->Remove((ANSC_HANDLE)pPoamIrepFoPool);
    pPoamIrepFoPool = NULL;



    
EXIT3:
    if(pDhcpv4Client)
        AnscFreeMemory(pDhcpv4Client);

    if(pDhcpv4ReqOpt )
        AnscFreeMemory(pDhcpv4ReqOpt);
    
    if(pDhcpv4SndOpt )
        AnscFreeMemory(pDhcpv4SndOpt);

    if(pDhcpv4Pool )
        AnscFreeMemory(pDhcpv4Pool);

    if(pDhcpv4StaticAddr)
        AnscFreeMemory(pDhcpv4StaticAddr);

    if(pDhcpv4Option)
        AnscFreeMemory(pDhcpv4Option);
        
EXIT2:
    
    if(pAliasReqOption)
        AnscFreeMemory(pAliasReqOption);

    if(pAliasSendOption)
        AnscFreeMemory(pAliasSendOption);
    
    if(pAliasClient)
        AnscFreeMemory(pAliasClient);
    
    if(pAliasPool)
        AnscFreeMemory(pAliasPool);
    
    if(pAliasStaticAddr)
        AnscFreeMemory(pAliasStaticAddr);

    if(pAliasOption)
        AnscFreeMemory(pAliasOption);
    
EXIT1:
    
    if ( pPoamIrepFoClient )
        pPoamIrepFoClient->Remove((ANSC_HANDLE)pPoamIrepFoClient);

    if ( pPoamIrepFoEnumClient )
        pPoamIrepFoEnumClient->Remove((ANSC_HANDLE)pPoamIrepFoEnumClient);

    if ( pPoamIrepFoReqOpt)
        pPoamIrepFoReqOpt->Remove((ANSC_HANDLE)pPoamIrepFoReqOpt);

    if ( pPoamIrepFoEnumReqOpt )
        pPoamIrepFoEnumReqOpt->Remove((ANSC_HANDLE)pPoamIrepFoEnumReqOpt);

    if ( pPoamIrepFoSndOpt)
        pPoamIrepFoSndOpt->Remove((ANSC_HANDLE)pPoamIrepFoSndOpt);

    if ( pPoamIrepFoEnumSndOpt)
        pPoamIrepFoEnumSndOpt->Remove((ANSC_HANDLE)pPoamIrepFoEnumSndOpt);
    
    if ( pPoamIrepFoPool)
        pPoamIrepFoPool->Remove((ANSC_HANDLE)pPoamIrepFoPool);

    if ( pPoamIrepFoEnumPool)
        pPoamIrepFoEnumPool->Remove((ANSC_HANDLE)pPoamIrepFoEnumPool);
    
    if ( pPoamIrepFoStaticAddr)
        pPoamIrepFoStaticAddr->Remove((ANSC_HANDLE)pPoamIrepFoStaticAddr);

    if ( pPoamIrepFoEnumStaticAddr)
        pPoamIrepFoEnumStaticAddr->Remove((ANSC_HANDLE)pPoamIrepFoEnumStaticAddr);

    if ( pPoamIrepFoOption)
        pPoamIrepFoOption->Remove((ANSC_HANDLE)pPoamIrepFoOption);

    if ( pPoamIrepFoEnumOption)
        pPoamIrepFoEnumOption->Remove((ANSC_HANDLE)pPoamIrepFoEnumOption);


    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        CosaDhcpv4RegSetDhcpv4Info
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to save current NextInstanceNumber and Delay_added
        entry into sysregistry.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of dhcpv4
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CosaDhcpv4RegSetDhcpv4Info
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DHCPV4          pMyObject         = (PCOSA_DATAMODEL_DHCPV4   )hThisObject;
    
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoDhcpv4 = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderDhcpv4;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoClient = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoReqOpt = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoSndOpt = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoPool   = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoStaticAddress   = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoOption          = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoEnumClient      = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoEnumReqOpt      = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoEnumSndOpt      = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoEnumPool        = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoEnumStaticAddress   = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoEnumOption          = (PPOAM_IREP_FOLDER_OBJECT )NULL;

    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY       )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry2      = (PSINGLE_LINK_ENTRY       )NULL;
    PCOSA_CONTEXT_DHCPC_LINK_OBJECT pCosaDhcpcContext = NULL;
    PCOSA_CONTEXT_POOL_LINK_OBJECT  pCosaPoolContext  = NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaReqOptionContext       = NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaSendOptionContext      = NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaStaticAddressContext   = NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaOptionContext          = NULL;
    PSLAP_VARIABLE                  pSlapVariable     = NULL;
    ULONG                           ulEntryCount      = 0;
    ULONG                           ulIndex           = 0;
    ULONG                           ulEntryCount2     = 0;
    ULONG                           ulIndex2          = 0;
    ULONG                           uInstanceNumber   = 0;
    char*                           pAliasClient      = NULL;
    char*                           pAliasReqOption   = NULL;
    char*                           pAliasSendOption  = NULL;
    char*                           pAliasPool        = NULL;
    char*                           pAliasStaticAddress = NULL;
    char*                           pAliasOption        = NULL;
    char*                           pFolderName       = NULL;
    char                            FolderName[16]    = {0};
    
    PCOSA_DML_DHCPC_FULL            pDhcpv4Client     = NULL;
    PCOSA_DML_DHCPC_REQ_OPT         pDhcpv4ReqOpt     = NULL;
    PCOSA_DML_DHCP_OPT              pDhcpv4SndOpt     = NULL;
    PCOSA_DML_DHCPS_POOL_FULL       pDhcpv4Pool       = NULL;
    PCOSA_DML_DHCPS_SADDR           pDhcpv4StaticAddress  = NULL;
    PCOSA_DML_DHCPSV4_OPTION        pDhcpv4Option         = NULL;

    if ( !pPoamIrepFoDhcpv4 )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pPoamIrepFoDhcpv4->EnableFileSync((ANSC_HANDLE)pPoamIrepFoDhcpv4, FALSE);
    }

    if ( TRUE )
    {
        pPoamIrepFoDhcpv4->Clear((ANSC_HANDLE)pPoamIrepFoDhcpv4);

        SlapAllocVariable(pSlapVariable);

        if ( !pSlapVariable )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT1;
        }
    }

    /* This is saved structure for dhcpv4
        *****************************************
              <Dhcpv4>
                  <client>
                      <NextInstanceNumber> xxx </>
                      <1>
                           <alias>xxx</>
                           <bNew>false</>
                           <ReqOption>
                               <NextInstanceNumber> xxx </>
                               <1>
                                   <alias>xxx</>
                                   <bNew>false</>
                               </1>
                           </ReqOption>
                          <SendOption>
                              <NextInstanceNumber> xxx </>
                              <1>
                                  <alias>xxx</>
                                  <bNew>true</>
                              </1>
                          </SendOption>
                     </1>
               </client>
              <pool>
                    <NextInstanceNumber> xxx </>
                    <1>
                        <alias>xxx</>
                        <bNew>true</>
                        <staticAddress>
                            <NextInstanceNumber> xxx </>
                            <1>
                                <alias>xxx</>
                                <bNew>true</>
                            </1>
                        </staticAddress>
                       <option>
                           <NextInstanceNumber> xxx </>
                           <1>
                               <alias>xxx</>
                               <bNew>true</>
                           </1>
                       </option>
                    </1>
              </pool>
            </Dhcpv4>
      ****************************************************
      */

    /* Add dhcpv4.client.*/
    pPoamIrepFoClient =
        pPoamIrepFoDhcpv4->AddFolder
            (
                (ANSC_HANDLE)pPoamIrepFoDhcpv4,
                COSA_IREP_FOLDER_NAME_DHCPV4_CLIENT,
                0
            );

    if ( !pPoamIrepFoClient )
    {
        goto EXIT1;
    }

    /* add client.{i}.maxInstanceNumber  */
    if ( TRUE )
    {
        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
        pSlapVariable->Variant.varUint32 = pMyObject->maxInstanceOfClient;
    
        returnStatus =
            pPoamIrepFoClient->AddRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoClient,
                    COSA_DML_RR_NAME_Dhcpv4NextInsNunmber,
                    SYS_REP_RECORD_TYPE_UINT,
                    SYS_RECORD_CONTENT_DEFAULT,
                    pSlapVariable,
                    0
                );
    
        SlapCleanVariable(pSlapVariable);
        SlapInitVariable (pSlapVariable);
    }
    
    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->ClientList);

    while ( pSLinkEntry )
    {
        /* create dhcpv4.client.{i} */
        
        pCosaDhcpcContext = ACCESS_COSA_CONTEXT_DHCPC_LINK_OBJECT(pSLinkEntry);
        pSLinkEntry       = AnscSListGetNextEntry(pSLinkEntry);

        pDhcpv4Client = (PCOSA_DML_DHCPC_FULL)pCosaDhcpcContext->hContext;

        /* When this entry has been added to backend, has not any child and maxInstanceNumber is 0
                  We need not save this entry  */
        if ( !pCosaDhcpcContext->bNew && 
             !AnscSListQueryDepth(&pCosaDhcpcContext->ReqOptionList  ) &&
             !AnscSListQueryDepth(&pCosaDhcpcContext->SendOptionList ) &&
             pCosaDhcpcContext->maxInstanceOfSend == 0 &&
             pCosaDhcpcContext->maxInstanceOfReq  == 0 
           )
        {
            continue;
        }

        _ansc_sprintf(FolderName, "%u", pCosaDhcpcContext->InstanceNumber);

        pPoamIrepFoEnumClient =
            pPoamIrepFoClient->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoClient,
                    FolderName,
                    0
                );

        if ( !pPoamIrepFoEnumClient )
        {
            continue;
        }

        /* add  dhcpv4.client.{i}.alias */
        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;
            pSlapVariable->Variant.varString = AnscCloneString(pDhcpv4Client->Cfg.Alias);

            returnStatus =
                pPoamIrepFoEnumClient->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEnumClient,
                        COSA_DML_RR_NAME_Dhcpv4Alias,
                        SYS_REP_RECORD_TYPE_ASTR,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );

            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }

        /* add  dhcpv4.client.{i}.bNew */
        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_bool;
            pSlapVariable->Variant.varBool   = pCosaDhcpcContext->bNew;

            returnStatus =
                pPoamIrepFoEnumClient->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEnumClient,
                        COSA_DML_RR_NAME_Dhcpv4bNew,
                        SYS_REP_RECORD_TYPE_BOOL,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );

            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }

        /*
                    begin add reqOption
                */

        if ( !AnscSListQueryDepth(&pCosaDhcpcContext->ReqOptionList) )
        {
            goto SentOption;
        }
            
        /* Add dhcpv4.client.{i}.reqOption */
        pPoamIrepFoReqOpt =
            pPoamIrepFoEnumClient->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoEnumClient,
                    COSA_IREP_FOLDER_NAME_DHCPV4_REQOPTION,
                    0
                );

        if ( !pPoamIrepFoReqOpt )
        {
            goto EXIT1;
        }

        /* add client.{i}.reqOption.maxInstanceNumber  */
        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
            pSlapVariable->Variant.varUint32 = pCosaDhcpcContext->maxInstanceOfReq;
        
            returnStatus =
                pPoamIrepFoReqOpt->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoReqOpt,
                        COSA_DML_RR_NAME_Dhcpv4NextInsNunmber,
                        SYS_REP_RECORD_TYPE_UINT,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );
        
            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }

        pSLinkEntry2 = AnscSListGetFirstEntry(&pCosaDhcpcContext->ReqOptionList);

        while ( pSLinkEntry2 )
        {
            /* create dhcpv4.client.{i}.reqOption.{i} */

            pCosaReqOptionContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry2);
            pSLinkEntry2          = AnscSListGetNextEntry(pSLinkEntry2);
        
            pDhcpv4ReqOpt= (PCOSA_DML_DHCPC_REQ_OPT)pCosaReqOptionContext->hContext;

            if ( !pCosaReqOptionContext->bNew )
            {
                continue;
            }
            
            _ansc_sprintf(FolderName, "%u", pCosaReqOptionContext->InstanceNumber);
        
            pPoamIrepFoEnumReqOpt =
                pPoamIrepFoReqOpt->AddFolder
                    (
                        (ANSC_HANDLE)pPoamIrepFoReqOpt,
                        FolderName,
                        0
                    );
        
            if ( !pPoamIrepFoEnumReqOpt )
            {
                continue;
            }

            /* create dhcpv4.client.{i}.reqOption.{i}.alias */
            if ( TRUE )
            {
                pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;
                pSlapVariable->Variant.varString = AnscCloneString(pDhcpv4ReqOpt->Alias);

                returnStatus =
                    pPoamIrepFoEnumReqOpt->AddRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoEnumReqOpt,
                            COSA_DML_RR_NAME_Dhcpv4Alias,
                            SYS_REP_RECORD_TYPE_ASTR,
                            SYS_RECORD_CONTENT_DEFAULT,
                            pSlapVariable,
                            0
                        );
                
                SlapCleanVariable(pSlapVariable);
                SlapInitVariable (pSlapVariable);
            }

            /* create dhcpv4.client.{i}.reqOption.{i}.alias */
            if ( TRUE )
            {
                pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_bool;
                pSlapVariable->Variant.varBool   = pCosaReqOptionContext->bNew;

                returnStatus =
                    pPoamIrepFoEnumReqOpt->AddRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoEnumReqOpt,
                            COSA_DML_RR_NAME_Dhcpv4bNew,
                            SYS_REP_RECORD_TYPE_BOOL,
                            SYS_RECORD_CONTENT_DEFAULT,
                            pSlapVariable,
                            0
                        );
                
                SlapCleanVariable(pSlapVariable);
                SlapInitVariable (pSlapVariable);
            }

            pPoamIrepFoEnumReqOpt->Remove((ANSC_HANDLE)pPoamIrepFoEnumReqOpt);
            pPoamIrepFoEnumReqOpt = NULL;
        }

        pPoamIrepFoReqOpt->Remove((ANSC_HANDLE)pPoamIrepFoReqOpt);
        pPoamIrepFoReqOpt = NULL;

        /*
                    begin add sendOption
                */
SentOption:

        if ( !AnscSListQueryDepth(&pCosaDhcpcContext->SendOptionList) )
        {
            goto ClientEnd;
        }

        /* Add dhcpv4.client.{i}.sendOption */
        pPoamIrepFoSndOpt =
            pPoamIrepFoEnumClient->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoEnumClient,
                    COSA_IREP_FOLDER_NAME_DHCPV4_SENTOPTION,
                    0
                );
        
        if ( !pPoamIrepFoSndOpt )
        {
            goto EXIT1;
        }
        
        /* add client.{i}.sendOption.maxInstanceNumber  */
        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
            pSlapVariable->Variant.varUint32 = pCosaDhcpcContext->maxInstanceOfSend;
        
            returnStatus =
                pPoamIrepFoSndOpt->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoSndOpt,
                        COSA_DML_RR_NAME_Dhcpv4NextInsNunmber,
                        SYS_REP_RECORD_TYPE_UINT,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );
        
            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }
        
        pSLinkEntry2 = AnscSListGetFirstEntry(&pCosaDhcpcContext->SendOptionList);
        
        while ( pSLinkEntry2 )
        {
            /* create dhcpv4.client.{i}.sendOption.{i} */
        
            pCosaSendOptionContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry2);
            pSLinkEntry2          = AnscSListGetNextEntry(pSLinkEntry2);
        
            pDhcpv4SndOpt= (PCOSA_DML_DHCP_OPT)pCosaSendOptionContext->hContext;

            if ( !pCosaSendOptionContext->bNew )
            {
                continue;
            }
        
            _ansc_sprintf(FolderName, "%u", pCosaSendOptionContext->InstanceNumber);
        
            pPoamIrepFoEnumSndOpt =
                pPoamIrepFoSndOpt->AddFolder
                    (
                        (ANSC_HANDLE)pPoamIrepFoSndOpt,
                        FolderName,
                        0
                    );
        
            if ( !pPoamIrepFoEnumSndOpt )
            {
                continue;
            }

            /* create dhcpv4.client.{i}.sendOption.{i}.alias */
            if ( TRUE )
            {
                pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;
                pSlapVariable->Variant.varString = AnscCloneString(pDhcpv4SndOpt->Alias);
        
                returnStatus =
                    pPoamIrepFoEnumSndOpt->AddRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoEnumSndOpt,
                            COSA_DML_RR_NAME_Dhcpv4Alias,
                            SYS_REP_RECORD_TYPE_ASTR,
                            SYS_RECORD_CONTENT_DEFAULT,
                            pSlapVariable,
                            0
                        );
                
                SlapCleanVariable(pSlapVariable);
                SlapInitVariable (pSlapVariable);
            }

            /* create dhcpv4.client.{i}.sendOption.{i}.bNew */
            if ( TRUE )
            {
                pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_bool;
                pSlapVariable->Variant.varBool   = pCosaSendOptionContext->bNew;
        
                returnStatus =
                    pPoamIrepFoEnumSndOpt->AddRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoEnumSndOpt,
                            COSA_DML_RR_NAME_Dhcpv4bNew,
                            SYS_REP_RECORD_TYPE_BOOL,
                            SYS_RECORD_CONTENT_DEFAULT,
                            pSlapVariable,
                            0
                        );
                
                SlapCleanVariable(pSlapVariable);
                SlapInitVariable (pSlapVariable);
            }

            pPoamIrepFoEnumSndOpt->Remove((ANSC_HANDLE)pPoamIrepFoEnumSndOpt);
            pPoamIrepFoEnumSndOpt = NULL;
        }
        
        pPoamIrepFoSndOpt->Remove((ANSC_HANDLE)pPoamIrepFoSndOpt);
        pPoamIrepFoSndOpt = NULL;
        
ClientEnd:

        /*release some resource */        
        pPoamIrepFoEnumClient->Remove((ANSC_HANDLE)pPoamIrepFoEnumClient);
        pPoamIrepFoEnumClient = NULL;
    }
    
    pPoamIrepFoClient->Remove((ANSC_HANDLE)pPoamIrepFoClient);
    pPoamIrepFoClient = NULL;


    /*
            begin process pool and staticaddress
        */

    /* Add dhcpv4.server.pool.*/
    pPoamIrepFoPool =
        pPoamIrepFoDhcpv4->AddFolder
            (
                (ANSC_HANDLE)pPoamIrepFoDhcpv4,
                COSA_IREP_FOLDER_NAME_DHCPV4_POOL,
                0
            );

    if ( !pPoamIrepFoPool )
    {
        goto EXIT1;
    }

    /* add pool.{i}.maxInstanceNumber  */
    if ( TRUE )
    {
        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
        pSlapVariable->Variant.varUint32 = pMyObject->maxInstanceOfPool;
    
        returnStatus =
            pPoamIrepFoPool->AddRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoPool,
                    COSA_DML_RR_NAME_Dhcpv4NextInsNunmber,
                    SYS_REP_RECORD_TYPE_UINT,
                    SYS_RECORD_CONTENT_DEFAULT,
                    pSlapVariable,
                    0
                );
    
        SlapCleanVariable(pSlapVariable);
        SlapInitVariable (pSlapVariable);
    }
    
    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->PoolList);

    while ( pSLinkEntry )
    {
        /* create dhcpv4.pool.{i} */
        
        pCosaPoolContext = ACCESS_COSA_CONTEXT_POOL_LINK_OBJECT(pSLinkEntry);
        pSLinkEntry       = AnscSListGetNextEntry(pSLinkEntry);

        pDhcpv4Pool = (PCOSA_DML_DHCPS_POOL_FULL)pCosaPoolContext->hContext;

        /*
                    When  not bNew and have not any child, maxInstanceNum is 0, we will not save this entry.
                */
        if ( !pCosaPoolContext->bNew && 
             !AnscSListQueryDepth(&pCosaPoolContext->StaticAddressList) &&
             pCosaPoolContext->maxInstanceOfStaticAddress == 0
           )
        {
            continue;
        }

        _ansc_sprintf(FolderName, "%u", pCosaPoolContext->InstanceNumber);

        pPoamIrepFoEnumPool =
            pPoamIrepFoPool->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoPool,
                    FolderName,
                    0
                );

        if ( !pPoamIrepFoEnumPool )
        {
            continue;
        }

        /* add  dhcpv4.pool.{i}.alias */
        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;
            pSlapVariable->Variant.varString = AnscCloneString(pDhcpv4Pool->Cfg.Alias);

            returnStatus =
                pPoamIrepFoEnumPool->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEnumPool,
                        COSA_DML_RR_NAME_Dhcpv4Alias,
                        SYS_REP_RECORD_TYPE_ASTR,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );

            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }

        /* add  dhcpv4.pool.{i}.bNew */
        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_bool;
            pSlapVariable->Variant.varBool   = pCosaPoolContext->bNew;

            returnStatus =
                pPoamIrepFoEnumPool->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEnumPool,
                        COSA_DML_RR_NAME_Dhcpv4bNew,
                        SYS_REP_RECORD_TYPE_BOOL,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );

            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }

        /*
                    begin add StaticAddress
                */
        if ( !AnscSListQueryDepth(&pCosaPoolContext->StaticAddressList) )
        {
            goto PoolEnd;
        }
        
        /* Add dhcpv4.pool.{i}.StaticAddress */
        pPoamIrepFoStaticAddress =
            pPoamIrepFoEnumPool->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoEnumPool,
                    COSA_IREP_FOLDER_NAME_DHCPV4_STATICADDRESS,
                    0
                );
        
        if ( !pPoamIrepFoStaticAddress )
        {
            goto EXIT1;
        }

        /* add pool.{i}.StaticAddress.maxInstanceNumber  */
        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
            pSlapVariable->Variant.varUint32 = pCosaPoolContext->maxInstanceOfStaticAddress;
        
            returnStatus =
                pPoamIrepFoStaticAddress->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoStaticAddress,
                        COSA_DML_RR_NAME_Dhcpv4NextInsNunmber,
                        SYS_REP_RECORD_TYPE_UINT,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );
        
            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }

        pSLinkEntry2 = AnscSListGetFirstEntry(&pCosaPoolContext->StaticAddressList);

        while ( pSLinkEntry2 )
        {
            /* create dhcpv4.pool.{i}.StaticAddress.{i} */

            pCosaStaticAddressContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry2);
            pSLinkEntry2              = AnscSListGetNextEntry(pSLinkEntry2);
        
            pDhcpv4StaticAddress = (PCOSA_DML_DHCPS_SADDR)pCosaStaticAddressContext->hContext;

            if ( !pCosaStaticAddressContext->bNew )
            {
                continue;
            }
            
            _ansc_sprintf(FolderName, "%u", pCosaStaticAddressContext->InstanceNumber);
        
            pPoamIrepFoEnumStaticAddress =
                pPoamIrepFoStaticAddress->AddFolder
                    (
                        (ANSC_HANDLE)pPoamIrepFoStaticAddress,
                        FolderName,
                        0
                    );
        
            if ( !pPoamIrepFoEnumStaticAddress )
            {
                continue;
            }
            
            /* create dhcpv4.pool.{i}.StaticAddress.{i}.alias */
            if ( TRUE )
            {
                pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;
                pSlapVariable->Variant.varString = AnscCloneString(pDhcpv4StaticAddress->Alias);

                returnStatus =
                    pPoamIrepFoEnumStaticAddress->AddRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoEnumStaticAddress,
                            COSA_DML_RR_NAME_Dhcpv4Alias,
                            SYS_REP_RECORD_TYPE_ASTR,
                            SYS_RECORD_CONTENT_DEFAULT,
                            pSlapVariable,
                            0
                        );
                
                SlapCleanVariable(pSlapVariable);
                SlapInitVariable (pSlapVariable);
            }    
        
            /* create dhcpv4.pool.{i}.StaticAddress.{i}.bNew */
            if ( TRUE )
            {
                pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_bool;
                pSlapVariable->Variant.varBool = pCosaStaticAddressContext->bNew;
            
                returnStatus =
                    pPoamIrepFoEnumStaticAddress->AddRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoEnumStaticAddress,
                            COSA_DML_RR_NAME_Dhcpv4bNew,
                            SYS_REP_RECORD_TYPE_BOOL,
                            SYS_RECORD_CONTENT_DEFAULT,
                            pSlapVariable,
                            0
                        );
                
                SlapCleanVariable(pSlapVariable);
                SlapInitVariable (pSlapVariable);
            }

            pPoamIrepFoEnumStaticAddress->Remove((ANSC_HANDLE)pPoamIrepFoEnumStaticAddress);
            pPoamIrepFoEnumStaticAddress = NULL;
        }

        pPoamIrepFoStaticAddress->Remove((ANSC_HANDLE)pPoamIrepFoStaticAddress);
        pPoamIrepFoStaticAddress = NULL;

        /*
                    begin add Option
                */
        if ( !AnscSListQueryDepth(&pCosaPoolContext->OptionList) )
        {
            goto PoolEnd;
        }

        /* Add dhcpv4.pool.{i}.Option */
        pPoamIrepFoOption =
            pPoamIrepFoEnumPool->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoEnumPool,
                    COSA_IREP_FOLDER_NAME_DHCPV4_OPTION,
                    0
                );

        if ( !pPoamIrepFoOption )
        {
            goto EXIT1;
        }

        /* add pool.{i}.Option.maxInstanceNumber  */
        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
            pSlapVariable->Variant.varUint32 = pCosaPoolContext->maxInstanceOfOption;

            returnStatus =
                pPoamIrepFoOption->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoOption,
                        COSA_DML_RR_NAME_Dhcpv4NextInsNunmber,
                        SYS_REP_RECORD_TYPE_UINT,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );

            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }

        pSLinkEntry2 = AnscSListGetFirstEntry(&pCosaPoolContext->OptionList);

        while ( pSLinkEntry2 )
        {
            /* create dhcpv4.pool.{i}.Option.{i} */

            pCosaOptionContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry2);
            pSLinkEntry2              = AnscSListGetNextEntry(pSLinkEntry2);

            pDhcpv4Option = (PCOSA_DML_DHCPSV4_OPTION)pCosaOptionContext->hContext;

            if ( !pCosaOptionContext->bNew )
            {
                continue;
            }
            
            _ansc_sprintf(FolderName, "%u", pCosaOptionContext->InstanceNumber);

            pPoamIrepFoEnumOption =
                pPoamIrepFoOption->AddFolder
                    (
                        (ANSC_HANDLE)pPoamIrepFoOption,
                        FolderName,
                        0
                    );

            if ( !pPoamIrepFoEnumOption )
            {
                continue;
            }
            
            /* create dhcpv4.pool.{i}.Option.{i}.alias */
            if ( TRUE )
            {
                pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;
                pSlapVariable->Variant.varString = AnscCloneString(pDhcpv4Option->Alias);

                returnStatus =
                    pPoamIrepFoEnumOption->AddRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoEnumOption,
                            COSA_DML_RR_NAME_Dhcpv4Alias,
                            SYS_REP_RECORD_TYPE_ASTR,
                            SYS_RECORD_CONTENT_DEFAULT,
                            pSlapVariable,
                            0
                        );
                
                SlapCleanVariable(pSlapVariable);
                SlapInitVariable (pSlapVariable);
            }    

            /* create dhcpv4.pool.{i}.Option.{i}.bNew */
            if ( TRUE )
            {
                pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_bool;
                pSlapVariable->Variant.varBool = pCosaOptionContext->bNew;
            
                returnStatus =
                    pPoamIrepFoEnumOption->AddRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoEnumOption,
                            COSA_DML_RR_NAME_Dhcpv4bNew,
                            SYS_REP_RECORD_TYPE_BOOL,
                            SYS_RECORD_CONTENT_DEFAULT,
                            pSlapVariable,
                            0
                        );
                
                SlapCleanVariable(pSlapVariable);
                SlapInitVariable (pSlapVariable);
            }

            pPoamIrepFoEnumOption->Remove((ANSC_HANDLE)pPoamIrepFoEnumOption);
            pPoamIrepFoEnumOption = NULL;
        }

        pPoamIrepFoOption->Remove((ANSC_HANDLE)pPoamIrepFoOption);
        pPoamIrepFoOption = NULL;

PoolEnd:

        /*release some resource */        
        pPoamIrepFoEnumPool->Remove((ANSC_HANDLE)pPoamIrepFoEnumPool);
        pPoamIrepFoEnumPool = NULL;
    }
    
    pPoamIrepFoPool->Remove((ANSC_HANDLE)pPoamIrepFoPool);
    pPoamIrepFoPool = NULL;


    

EXIT1:
    if ( pSlapVariable )
    {
        SlapFreeVariable(pSlapVariable);
        pSlapVariable = NULL;
    }

    if ( pPoamIrepFoClient )
        pPoamIrepFoClient->Remove((ANSC_HANDLE)pPoamIrepFoClient);

    if ( pPoamIrepFoEnumClient )
        pPoamIrepFoEnumClient->Remove((ANSC_HANDLE)pPoamIrepFoEnumClient);

    if ( pPoamIrepFoReqOpt)
        pPoamIrepFoReqOpt->Remove((ANSC_HANDLE)pPoamIrepFoReqOpt);

    if ( pPoamIrepFoEnumReqOpt )
        pPoamIrepFoEnumReqOpt->Remove((ANSC_HANDLE)pPoamIrepFoEnumReqOpt);

    if ( pPoamIrepFoSndOpt)
        pPoamIrepFoSndOpt->Remove((ANSC_HANDLE)pPoamIrepFoSndOpt);

    if ( pPoamIrepFoEnumSndOpt)
        pPoamIrepFoEnumSndOpt->Remove((ANSC_HANDLE)pPoamIrepFoEnumSndOpt);
    
    if ( pPoamIrepFoPool)
        pPoamIrepFoPool->Remove((ANSC_HANDLE)pPoamIrepFoPool);

    if ( pPoamIrepFoEnumPool)
        pPoamIrepFoEnumPool->Remove((ANSC_HANDLE)pPoamIrepFoEnumPool);
    
    if ( pPoamIrepFoStaticAddress)
        pPoamIrepFoStaticAddress->Remove((ANSC_HANDLE)pPoamIrepFoStaticAddress);

    if ( pPoamIrepFoEnumStaticAddress)
        pPoamIrepFoEnumStaticAddress->Remove((ANSC_HANDLE)pPoamIrepFoEnumStaticAddress);
    
    if ( pPoamIrepFoOption)
        pPoamIrepFoOption->Remove((ANSC_HANDLE)pPoamIrepFoOption);

    if ( pPoamIrepFoEnumOption)
        pPoamIrepFoEnumOption->Remove((ANSC_HANDLE)pPoamIrepFoEnumOption);

    pPoamIrepFoDhcpv4->EnableFileSync((ANSC_HANDLE)pPoamIrepFoDhcpv4, TRUE);

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        CosaDhcpv4ClientHasDelayAddedChild
            (
                PCOSA_CONTEXT_DHCPC_LINK_OBJECT                       hContext
            );

    description:

        This function is called to check whether this is child is pending added. If yes,
        return TRUE. Or else return FALSE.

    argument:   PCOSA_CONTEXT_DHCPC_LINK_OBJECT                hThisObject
                This handle is actually the pointer of one context link point.

    return:     TRUE or FALSE.

**********************************************************************/
BOOL
CosaDhcpv4ClientHasDelayAddedChild
    (
        PCOSA_CONTEXT_DHCPC_LINK_OBJECT                 hContext
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_DHCPC_LINK_OBJECT pCosaDhcpcContext = hContext;
    
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaCxtLink      = NULL;

    pSLinkEntry = AnscSListGetFirstEntry(&pCosaDhcpcContext->ReqOptionList);
    while ( pSLinkEntry )
    {
        pCosaCxtLink          = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        pSLinkEntry           = AnscSListGetNextEntry(pSLinkEntry);
    
        if ( pCosaCxtLink->bNew )
        {
            return TRUE;
        }
        
    }

    pSLinkEntry = AnscSListGetFirstEntry(&pCosaDhcpcContext->SendOptionList);
    while ( pSLinkEntry )
    {
        pCosaCxtLink          = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        pSLinkEntry           = AnscSListGetNextEntry(pSLinkEntry);
    
        if ( pCosaCxtLink->bNew )
        {
            return TRUE;
        }
        
    }

    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        CosaDhcpv4PoolHasDelayAddedChild
            (
                PCOSA_CONTEXT_POOL_LINK_OBJECT                       hContext
            );

    description:

        This function is called to check whether this is child is pending added. If yes,
        return TRUE. Or else return FALSE.

    argument:   PCOSA_CONTEXT_POOL_LINK_OBJECT                hThisObject
                This handle is actually the pointer of one context link point.

    return:     TRUE or FALSE.

**********************************************************************/
BOOL
CosaDhcpv4PoolHasDelayAddedChild
    (
        PCOSA_CONTEXT_POOL_LINK_OBJECT                hContext
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_POOL_LINK_OBJECT  pCosaPoolContext  = hContext;
    
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaCxtLink      = NULL;

    pSLinkEntry = AnscSListGetFirstEntry(&pCosaPoolContext->StaticAddressList);
    while ( pSLinkEntry )
    {
        pCosaCxtLink          = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        pSLinkEntry           = AnscSListGetNextEntry(pSLinkEntry);
    
        if ( pCosaCxtLink->bNew )
        {
            return TRUE;
        }
        
    }

    pSLinkEntry = AnscSListGetFirstEntry(&pCosaPoolContext->OptionList);
    while ( pSLinkEntry )
    {
        pCosaCxtLink          = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        pSLinkEntry           = AnscSListGetNextEntry(pSLinkEntry);
    
        if ( pCosaCxtLink->bNew )
        {
            return TRUE;
        }
        
    }
    
    return FALSE;
}



BOOL
CosaDmlSetIpaddr
    (
        PULONG pIPAddr, 
        PCHAR pString, 
        ULONG MaxNumber 
    )
{                                                       
    CHAR           *pTmpString          = pString;                                                       
    ULONG           i                   = 0;                                                       
    ULONG           j                   = 0;                                                       
    ULONG           n                   = 0;
    BOOL            bReturn             = TRUE;
    ULONG           pIPAddr2[COSA_DML_DHCP_MAX_ENTRIES]  = {0};

    if ( !pIPAddr || !pString || !MaxNumber )
        return FALSE;

    while( pTmpString[i] )                                                       
    {
        if ( pTmpString[i] == ',' )
        {
            pTmpString[i] = 0;                                                                
            pIPAddr2[n]  = _ansc_inet_addr(&pTmpString[j]);      
            if (pIPAddr2[n] == INADDR_NONE)                
            {                                              
                pTmpString[i] = ',';                                                       

                pIPAddr2[n] = 0;                  
                n = MaxNumber;                                 
                bReturn = FALSE;                               
                goto EXIT;                                         
            }               
            
            pTmpString[i] = ',';                                                       
            j = i + 1;                                                       
            n++;         
            
            if ( n >= MaxNumber )                                                       
            {                                                       
                break;                                                       
            }                                                       
        }                                                                  
        i++;                                                                   
    }  
    
    /* The last one  */                                                       
    if ( ( n < MaxNumber ) && ( (i-j) >= 7 ) )
    {                                                       
        pIPAddr2[n]  = _ansc_inet_addr(&pTmpString[j]); 
        if (pIPAddr2[n] == INADDR_NONE)                
        {                                              
            pIPAddr2[n] = 0;                  
            bReturn = FALSE;                               
            goto EXIT;
        }
    }                                                       
    else if ( (i-j) > 1 )                                   
    {                                    
        /*This case means there a illegal length ip address.*/
        bReturn = FALSE;                                    
        goto EXIT;
    }

    /*The setting may be NULL. So this also may clear backend. */
    for(n=0; n < MaxNumber; n++){
        pIPAddr[n] = pIPAddr2[n];
    }

EXIT:
    return bReturn;
}                                                 
    
BOOL 
CosaDmlGetIpaddrString
    (
        PUCHAR pString, 
        PULONG pulStrLength, 
        PULONG pIPAddr, 
        ULONG  MaxNumber
    )
{        
    UCHAR              *pTmpString      = pString;
    ULONG               n               = 0;
    BOOL                bReturn         = TRUE;
    PULONG              pIPAddr2        = pIPAddr;

    if ( !pString || !pulStrLength || !pIPAddr || !MaxNumber )
        return FALSE;
    
    while( pIPAddr2[n] && ( n < MaxNumber ) && ( (*pulStrLength- (pTmpString - pString)) > 15 ) )
    {
        AnscCopyString(pTmpString, _ansc_inet_ntoa( *((struct in_addr*)&(pIPAddr2[n]))) );
    
        pTmpString[AnscSizeOfString(pTmpString)] = ',';
    
        pTmpString = &pTmpString[AnscSizeOfString(pTmpString)];
        
        n++;
    }
    
    if ( pTmpString != pString )
    {
        pTmpString[AnscSizeOfString(pTmpString) -1] = 0;
    }
    
    if ( (*pulStrLength - (pTmpString - pString)) <= 15 )
    {
        *pulStrLength = MaxNumber * 16 + 1;
        
        return FALSE;
    }
    
    return TRUE;
}


/*
    This function is used to find right ClientContent according to Client Entry.
*/
ANSC_HANDLE
CosaDhcpv4GetClientContentbyClient
    (
        ANSC_HANDLE                 hClientContext
    )
{
    PCOSA_DATAMODEL_DHCPV4            pDhcpv4           = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;
    PCOSA_CONTEXT_POOL_LINK_OBJECT    pCxtLink          = NULL;
    PSINGLE_LINK_ENTRY                pSListEntry       = NULL;
    PCOSA_DML_DHCPSV4_CLIENT          pClient           = NULL;
    ULONG                             uIndex            = 0;
    PCOSA_DML_DHCPSV4_CLIENTCONTENT   pClientContent    = NULL;

    pSListEntry           = AnscSListGetFirstEntry(&pDhcpv4->PoolList);
    while( pSListEntry != NULL)
    {
        pCxtLink          = ACCESS_COSA_CONTEXT_POOL_LINK_OBJECT(pSListEntry);
        pSListEntry       = AnscSListGetNextEntry(pSListEntry);
    
        pClient          = (PCOSA_DML_DHCPSV4_CLIENT)pCxtLink->pClientList;
        for ( uIndex = 0; uIndex < pCxtLink->NumberOfClient; uIndex++ )
        {
            if ( (ANSC_HANDLE)&pClient[uIndex] == hClientContext )
            {
                break;
            }
        }

        if ( uIndex < pCxtLink->NumberOfClient )
        {
            pClientContent = &pCxtLink->pClientContentList[uIndex];        
            break;
        }
    }

    return (ANSC_HANDLE)pClientContent;
}

