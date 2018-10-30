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

    module: cosa_DHCPv6_internal.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaDhcpv6Create
        *  CosaDhcpv6Initialize
        *  CosaDhcpv6Remove
        *  CosaDhcpv6RegGetDhcpv6Info
        *  CosaDhcpv6RegGetDhcpv6Info
        *  CosaDhcpv6RegSetDhcpv6Info
        *  CosaDhcpv6ClientHasDelayAddedChild
        *  CosaDhcpv6PoolHasDelayAddedChild
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
#include "cosa_dhcpv6_apis.h"
#include "cosa_dhcpv6_internal.h"
#include "plugin_main_apis.h"
#include "poam_irepfo_interface.h"
#include "sys_definitions.h"

extern void * g_pDslhDmlAgent;

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        CosaDhcpv6Create
            (
            );

    description:

        This function constructs cosa nat object and return handle.

    argument:  

    return:     newly created nat object.

**********************************************************************/

ANSC_HANDLE
CosaDhcpv6Create
    (
        VOID
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DHCPV6          pMyObject    = (PCOSA_DATAMODEL_DHCPV6)NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PCOSA_DATAMODEL_DHCPV6)AnscAllocateMemory(sizeof(COSA_DATAMODEL_DHCPV6));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->Oid               = COSA_DATAMODEL_DHCPV6_OID;
    pMyObject->Create            = CosaDhcpv6Create;
    pMyObject->Remove            = CosaDhcpv6Remove;
    pMyObject->Initialize        = CosaDhcpv6Initialize;

    pMyObject->Initialize   ((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDhcpv6Initialize
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
CosaDhcpv6Initialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DHCPV6          pMyObject         = (PCOSA_DATAMODEL_DHCPV6)hThisObject;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoCOSA   = NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoDhcpv6 = NULL;

    /* We need call the initiation function of backend firstly .
            When backend return failure, we don't return because if return, all middle layer function will be not complete*/
    CosaDmlDhcpv6Init( NULL, NULL );

    /* Initiation all functions */
    AnscSListInitializeHeader( &pMyObject->ClientList );
    AnscSListInitializeHeader( &pMyObject->PoolList );
    pMyObject->maxInstanceOfClient  = 0;
    pMyObject->maxInstanceOfPool    = 0;
    AnscZeroMemory(pMyObject->AliasOfClient, sizeof(pMyObject->AliasOfClient));
    AnscZeroMemory(pMyObject->AliasOfPool, sizeof(pMyObject->AliasOfPool));

    /*We need to get Instance Info from cosa configuration*/
    pPoamIrepFoCOSA = (PPOAM_IREP_FOLDER_OBJECT)g_GetRegistryRootFolder(g_pDslhDmlAgent);

    if ( !pPoamIrepFoCOSA )
    {
        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT;
    }

    pPoamIrepFoDhcpv6 = 
        (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoCOSA->GetFolder
            (
                (ANSC_HANDLE)pPoamIrepFoCOSA,
                COSA_IREP_FOLDER_NAME_DHCPV6
            );

    if ( !pPoamIrepFoDhcpv6 )
    {
        pPoamIrepFoCOSA->EnableFileSync((ANSC_HANDLE)pPoamIrepFoCOSA, FALSE);
    
        pPoamIrepFoDhcpv6 =
            pPoamIrepFoCOSA->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoCOSA,
                    COSA_IREP_FOLDER_NAME_DHCPV6,
                    0
                );

        pPoamIrepFoCOSA->EnableFileSync((ANSC_HANDLE)pPoamIrepFoCOSA, TRUE);

    }

    if ( !pPoamIrepFoDhcpv6 )
    {
        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT;
    }
    else
    {
        pMyObject->hIrepFolderDhcpv6 = (ANSC_HANDLE)pPoamIrepFoDhcpv6;
    }

    /* We need get NextInstanceNumber from backend. By the way, the whole tree 
            was created. Moreover, we also need get delay-added entry and put them
            into our tree. */
    CosaDhcpv6RegGetDhcpv6Info((ANSC_HANDLE)pMyObject);

    /* Firstly we create the whole system from backend */
    CosaDhcpv6BackendGetDhcpv6Info((ANSC_HANDLE)pMyObject);

EXIT:
    
    return returnStatus;
}



/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDhcpv6Remove
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
CosaDhcpv6Remove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DHCPV6          pMyObject           = (PCOSA_DATAMODEL_DHCPV6)hThisObject;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoDhcpv6   = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderDhcpv6;

    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtDhcpcLink       = NULL;
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtPoolLink        = NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink            = NULL;
    PSINGLE_LINK_ENTRY              pSListEntry         = NULL;
    PSINGLE_LINK_ENTRY              pSListEntry2        = NULL;
    BOOL                            bFound              = FALSE;
    ULONG                           Index               = 0;
    
    /* Remove necessary resource */
    
    pSListEntry         = AnscSListPopEntry(&pMyObject->ClientList);
    while( pSListEntry != NULL)
    {
        pCxtDhcpcLink     = ACCESS_COSA_CONTEXT_DHCPCV6_LINK_OBJECT(pSListEntry);
        pSListEntry       = AnscSListGetNextEntry(pSListEntry);

        pSListEntry2         = AnscSListPopEntry(&pCxtDhcpcLink->SentOptionList);
        while( pSListEntry2 != NULL)
        {
            pCxtLink         = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSListEntry2);
            pSListEntry2       = AnscSListGetNextEntry(pSListEntry2);

            AnscFreeMemory(pCxtLink->hContext);
            AnscFreeMemory(pCxtLink);  
        }

        AnscFreeMemory( pCxtDhcpcLink->pServerEntry );
        AnscFreeMemory( pCxtDhcpcLink->pRecvEntry );

        AnscFreeMemory(pCxtDhcpcLink->hContext);
        AnscFreeMemory(pCxtDhcpcLink);
    }

    pSListEntry         = AnscSListPopEntry(&pMyObject->PoolList);
    while( pSListEntry != NULL)
    {
        pCxtPoolLink      = ACCESS_COSA_CONTEXT_POOLV6_LINK_OBJECT(pSListEntry);
        pSListEntry       = AnscSListGetNextEntry(pSListEntry);
        
        pSListEntry2         = AnscSListPopEntry(&pCxtPoolLink->OptionList);
        while( pSListEntry2 != NULL)
        {
            pCxtLink         = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSListEntry2);
            pSListEntry2     = AnscSListGetNextEntry(pSListEntry2);
    
            AnscFreeMemory(pCxtLink->hContext);
            AnscFreeMemory(pCxtLink); 
        }
        
        for ( Index = 0 ; Index < pCxtPoolLink->NumberOfClient; Index++ )
        {
            AnscFreeMemory( pCxtPoolLink->pClientContentList[Index].pIPv6Address );
            AnscFreeMemory( pCxtPoolLink->pClientContentList[Index].pIPv6Prefix  );
            AnscFreeMemory( pCxtPoolLink->pClientContentList[Index].pOption      );
        }
        
        AnscFreeMemory( pCxtPoolLink->pClientContentList );
        AnscFreeMemory( pCxtPoolLink->pClientList );        

        AnscFreeMemory(pCxtPoolLink->hContext);
        AnscFreeMemory(pCxtPoolLink);
    }

    if ( pPoamIrepFoDhcpv6 )
    {
        pPoamIrepFoDhcpv6->Remove( (ANSC_HANDLE)pPoamIrepFoDhcpv6);
    }

    CosaDmlDhcpv6Remove((ANSC_HANDLE)pMyObject);
    
    /* Remove self */
    AnscFreeMemory((ANSC_HANDLE)pMyObject);
    
    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        CosaDhcpv6RegGetDhcpv6Info
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the NextInstanceNumber for every table, Create
        the link tree. For delay_added entry, we also need create them.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of DHCPv6
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CosaDhcpv6BackendGetDhcpv6Info
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;    
    PCOSA_DATAMODEL_DHCPV6          pDhcpv6           = (PCOSA_DATAMODEL_DHCPV6)hThisObject;
    
    PCOSA_DML_DHCPCV6_FULL          pDhcpc            = NULL;    
    PCOSA_DML_DHCPCV6_SVR           pDhcpcServer      = NULL;
    PCOSA_DML_DHCPCV6_RECV          pDhcpcRecv        = NULL;    
    PCOSA_DML_DHCPCV6_SENT          pSentOption       = NULL;
    
    PCOSA_DML_DHCPSV6_POOL_FULL     pPool             = NULL;
    PCOSA_DML_DHCPSV6_POOL_OPTION   pPoolOption       = NULL;
    PCOSA_DML_DHCPSV6_CLIENT        pPoolClient       = NULL;
    PCOSA_DML_DHCPSV6_CLIENT_IPV6ADDRESS   pPoolIPv6Address = NULL;
    PCOSA_DML_DHCPSV6_CLIENT_IPV6PREFIX    pPoolIPv6Prefix  =NULL;
    PCOSA_DML_DHCPSV6_CLIENT_OPTION        pPoolIPv6Option  =NULL;

    ULONG                           clientCount       = 0;
    ULONG                           poolCount         = 0;
    ULONG                           count             = 0;
    ULONG                           count1            = 0;
    ULONG                           ulIndex           = 0;
    ULONG                           ulIndex2          = 0;

    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pClientCxtLink    = NULL;    
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pClientCxtLink2   = NULL;    
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pPoolCxtLink      = NULL;    
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pPoolCxtLink2     = NULL;    
    PCOSA_CONTEXT_LINK_OBJECT         pCxtLink          = NULL;
    PCOSA_CONTEXT_LINK_OBJECT         pCxtLink2         = NULL;

    BOOL                            bNeedSave         = FALSE;

    /* Get DHCPv6.Client.{i} */
    clientCount = CosaDmlDhcpv6cGetNumberOfEntries(NULL);
    for ( ulIndex = 0; ulIndex < clientCount; ulIndex++ )
    {
        pDhcpc  = (PCOSA_DML_DHCPCV6_FULL)AnscAllocateMemory( sizeof(COSA_DML_DHCPCV6_FULL) );
        if ( !pDhcpc )
        {
            break;
        }

        DHCPV6_CLIENT_SET_DEFAULTVALUE(pDhcpc);
        returnStatus = CosaDmlDhcpv6cGetEntry(NULL, ulIndex, pDhcpc);
        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            AnscFreeMemory(pDhcpc);
            break;
        }

        pClientCxtLink = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)AnscAllocateMemory( sizeof(COSA_CONTEXT_DHCPCV6_LINK_OBJECT) );
        if ( !pClientCxtLink )
        {
            AnscFreeMemory(pDhcpc);
            break;
        }

        DHCPV6_CLIENT_INITIATION_CONTEXT(pClientCxtLink)
        pClientCxtLink->hContext       = (ANSC_HANDLE)pDhcpc;
        pClientCxtLink->bNew           = FALSE;
        pClientCxtLink->hParentTable   = (ANSC_HANDLE)pDhcpv6;

        if ( !pDhcpc->Cfg.InstanceNumber )
        {
            if ( !++pDhcpv6->maxInstanceOfClient )
            {
                pDhcpv6->maxInstanceOfClient = 1;
            }
            bNeedSave                        = TRUE;

            pDhcpc->Cfg.InstanceNumber     = pDhcpv6->maxInstanceOfClient;
            pClientCxtLink->InstanceNumber = pDhcpc->Cfg.InstanceNumber;
            
            _ansc_sprintf(pDhcpc->Cfg.Alias, "DHCPv6%d", pDhcpc->Cfg.InstanceNumber);

            returnStatus = CosaDmlDhcpv6cSetValues
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
            CosaSListPushEntryByInsNum(&pDhcpv6->ClientList, (PCOSA_CONTEXT_LINK_OBJECT)pClientCxtLink);
        }
        else
        {
            pClientCxtLink->InstanceNumber = pDhcpc->Cfg.InstanceNumber;

            /* This case never happen. Add it just for simulation code run well */
            if ( pDhcpv6->maxInstanceOfClient < pClientCxtLink->InstanceNumber )
            {
                pDhcpv6->maxInstanceOfClient = pClientCxtLink->InstanceNumber;
                bNeedSave                    = TRUE;
            }

            /* if this entry is in link tree already because it's the parent of delay_added table */
            pClientCxtLink2 = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)CosaSListGetEntryByInsNum(&pDhcpv6->ClientList, pClientCxtLink->InstanceNumber);
            if ( !pClientCxtLink2 )
            {
                CosaSListPushEntryByInsNum(&pDhcpv6->ClientList, (PCOSA_CONTEXT_LINK_OBJECT)pClientCxtLink);
            }
            else
            {
                /* When this case happens, somethings happens to be error. We harmonize it here.*/
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

        /* We begin treat DHCPv6.Client.{i}.Server.{i} 
                    This is one dynamic table. We get all once */
        returnStatus = CosaDmlDhcpv6cGetServerCfg
                        (
                            NULL,
                            pDhcpc->Cfg.InstanceNumber,
                            &pDhcpcServer,
                            &count
                        );
        if ( returnStatus == ANSC_STATUS_SUCCESS )
        {
            pClientCxtLink->pServerEntry    = pDhcpcServer;
            pClientCxtLink->NumberOfServer  = count;
        }
        else
        {
            CcspTraceWarning(("CosaDhcpv6BackendGetDhcpv6Info -- CosaDmlDhcpv6cGetServerCfg() return error:%d.\n", returnStatus));
        }
 
        /* We begin treat DHCPv6.Client.{i}.SentOption.{i} */
        count = CosaDmlDhcpv6cGetNumberOfSentOption
                    (
                        NULL,
                        pDhcpc->Cfg.InstanceNumber
                    );
        
        for ( ulIndex2 = 0; ulIndex2 < count; ulIndex2++ )
        {
            pSentOption  = (PCOSA_DML_DHCPCV6_SENT)AnscAllocateMemory( sizeof(COSA_DML_DHCPCV6_SENT) );
            if ( !pSentOption )
            {
                break;
            }
        
            DHCPV6_SENTOPTION_SET_DEFAULTVALUE(pSentOption);
            returnStatus = CosaDmlDhcpv6cGetSentOption
                            (
                                NULL, 
                                pDhcpc->Cfg.InstanceNumber, 
                                ulIndex2, 
                                pSentOption
                            );
            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                AnscFreeMemory(pSentOption);
                break;
            }
        
            pCxtLink = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory( sizeof(COSA_CONTEXT_LINK_OBJECT) );
            if ( !pCxtLink )
            {
                AnscFreeMemory(pSentOption);
                break;
            }
        
            COSA_CONTEXT_LINK_INITIATION_CONTENT(pCxtLink);
            pCxtLink->hContext       = (ANSC_HANDLE)pSentOption;
            pCxtLink->hParentTable   = (ANSC_HANDLE)pClientCxtLink;
            pCxtLink->bNew           = FALSE;
        
            if ( !pSentOption->InstanceNumber )
            {
                if ( !++pClientCxtLink->maxInstanceOfSent )
                {
                    pClientCxtLink->maxInstanceOfSent = 1;
                    bNeedSave                  = TRUE;
                }
                bNeedSave                        = TRUE;
                pSentOption->InstanceNumber = pClientCxtLink->maxInstanceOfSent;

                _ansc_sprintf( pSentOption->Alias, "SentOption%d", pSentOption->InstanceNumber );
        
                returnStatus = CosaDmlDhcpv6cSetSentOptionValues
                                (
                                    NULL,
                                    pDhcpc->Cfg.InstanceNumber,
                                    ulIndex, 
                                    pSentOption->InstanceNumber, 
                                    pSentOption->Alias
                                );
        
                if ( returnStatus != ANSC_STATUS_SUCCESS )
                {
                    AnscFreeMemory(pSentOption);
                    AnscFreeMemory(pCxtLink);
                    break;                    
                }

                pCxtLink->InstanceNumber = pSentOption->InstanceNumber; 
            
                /* Put into our list */
                CosaSListPushEntryByInsNum(&pClientCxtLink->SentOptionList, (PCOSA_CONTEXT_LINK_OBJECT)pCxtLink);
                
            } 
            else
            {
                pCxtLink->InstanceNumber = pSentOption->InstanceNumber; 
                
                /* This case never happen. Add it just for simulation code run well */
                if ( pClientCxtLink->maxInstanceOfSent < pSentOption->InstanceNumber )
                {
                    pClientCxtLink->maxInstanceOfSent = pSentOption->InstanceNumber;
                }

                /* if this entry is in link tree already because it's  delay_added table */
                pCxtLink2 = (PCOSA_CONTEXT_LINK_OBJECT)CosaSListGetEntryByInsNum(&pClientCxtLink->SentOptionList, pSentOption->InstanceNumber);
                if ( !pCxtLink2 )
                {
                    CosaSListPushEntryByInsNum(&pClientCxtLink->SentOptionList, (PCOSA_CONTEXT_LINK_OBJECT)pCxtLink);
                }
                else
                {
                    AnscFreeMemory( pCxtLink2->hContext );                
                    pCxtLink2->hContext       = (ANSC_HANDLE)pSentOption;
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
        // CosaDmlStartDHCP6Client();
        /* We begin treat DHCPv6.Client.{i}.ReceivedOption.{i} 
                    This is one dynamic table. We get all once */
        returnStatus = CosaDmlDhcpv6cGetReceivedOptionCfg
                        (
                            NULL,
                            pDhcpc->Cfg.InstanceNumber,
                            &pDhcpcRecv,
                            &count
                        );
        if ( returnStatus == ANSC_STATUS_SUCCESS )
        {
            pClientCxtLink->pRecvEntry    = pDhcpcRecv;
            pClientCxtLink->NumberOfRecv  = count;
        }
        else
        {
            CcspTraceWarning(("CosaDhcpv6BackendGetDhcpv6Info -- CosaDmlDhcpv6cGetReceivedOptionCfg() return error:%d.\n", returnStatus));
        }
        
    }

    /*****************************************

                Get DHCPv6.Server.Pool.{i} 

        ******************************************/
    poolCount = CosaDmlDhcpv6sGetNumberOfPools(NULL);
    for ( ulIndex = 0; ulIndex < poolCount; ulIndex++ )
    {
        pPool  = (PCOSA_DML_DHCPSV6_POOL_FULL)AnscAllocateMemory( sizeof(COSA_DML_DHCPSV6_POOL_FULL) );
        if ( !pPool )
        {
            break;
        }

        DHCPV6_POOL_SET_DEFAULTVALUE(pPool);
        returnStatus = CosaDmlDhcpv6sGetPool(NULL, ulIndex, pPool);
        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            AnscFreeMemory(pPool); /*RDKB-6737, CID-33316, free unused resource before exit */
            break;
        }

        pPoolCxtLink = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)AnscAllocateMemory( sizeof(COSA_CONTEXT_POOLV6_LINK_OBJECT) );
        if ( !pPoolCxtLink )
        {
            AnscFreeMemory(pPool);
            break;
        }

        DHCPV6_POOL_INITIATION_CONTEXT(pPoolCxtLink)
        pPoolCxtLink->hContext       = (ANSC_HANDLE)pPool;
        pPoolCxtLink->bNew           = FALSE;

        if ( !pPool->Cfg.InstanceNumber )
        {
            if ( !++pDhcpv6->maxInstanceOfPool )
            {
                pDhcpv6->maxInstanceOfPool = 1;
            }
            bNeedSave                        = TRUE;

            pPool->Cfg.InstanceNumber    = pDhcpv6->maxInstanceOfPool;
            pPoolCxtLink->InstanceNumber = pPool->Cfg.InstanceNumber; 
             
            _ansc_sprintf(pPool->Cfg.Alias, "DHCPv6%d", pPool->Cfg.InstanceNumber);

            returnStatus = CosaDmlDhcpv6sSetPoolValues
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
            CosaSListPushEntryByInsNum(&pDhcpv6->PoolList, (PCOSA_CONTEXT_LINK_OBJECT)pPoolCxtLink);
        }
        else
        {
            pPoolCxtLink->InstanceNumber = pPool->Cfg.InstanceNumber;
            
            /* This case never happen. Add it just for simulation code run well */
            if ( pDhcpv6->maxInstanceOfPool < pPool->Cfg.InstanceNumber )
            {
                pDhcpv6->maxInstanceOfPool = pPool->Cfg.InstanceNumber;
                bNeedSave                  = TRUE;
            }

            /* if this entry is in link tree already because it's the parent of delay_added table */
            pPoolCxtLink2 = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)CosaSListGetEntryByInsNum(&pDhcpv6->PoolList, pPoolCxtLink->InstanceNumber);
            if ( !pPoolCxtLink2 )
            {
                CosaSListPushEntryByInsNum(&pDhcpv6->PoolList, (PCOSA_CONTEXT_LINK_OBJECT)pPoolCxtLink);
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

        /* We begin treat DHCPv6.Server.Pool.{i}.Option.{i} */
        count = CosaDmlDhcpv6sGetNumberOfOption
                    (
                        NULL,
                        pPool->Cfg.InstanceNumber
                    );
        
        for ( ulIndex2 = 0; ulIndex2 < count; ulIndex2++ )
        {
            pPoolOption  = (PCOSA_DML_DHCPSV6_POOL_OPTION)AnscAllocateMemory( sizeof(COSA_DML_DHCPSV6_POOL_OPTION) );
            if ( !pPoolOption )
            {
                break;
            }

            DHCPV6_POOLOPTION_SET_DEFAULTVALUE(pPoolOption);
            returnStatus = CosaDmlDhcpv6sGetOption
                            (
                                NULL, 
                                pPool->Cfg.InstanceNumber, 
                                ulIndex2,
                                pPoolOption
                            );
            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                AnscFreeMemory(pPoolOption);
                break;
            }
        
            pCxtLink = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory( sizeof(COSA_CONTEXT_LINK_OBJECT) );
            if ( !pCxtLink )
            {
                AnscFreeMemory(pPoolOption);
                break;
            }
        
            COSA_CONTEXT_LINK_INITIATION_CONTENT(pCxtLink);
            pCxtLink->hContext       = (ANSC_HANDLE)pPoolOption;
            pCxtLink->hParentTable   = (ANSC_HANDLE)pPoolCxtLink;
            pCxtLink->bNew           = FALSE;
        
            if ( !pPoolOption->InstanceNumber )
            {
                if ( !++pPoolCxtLink->maxInstanceOfOption )
                {
                    pPoolCxtLink->maxInstanceOfOption = 1;
                }
                bNeedSave                        = TRUE;
                pPoolOption->InstanceNumber = pPoolCxtLink->maxInstanceOfOption;
        
                _ansc_sprintf( pPoolOption->Alias, "Option%d", pPoolOption->InstanceNumber );
        
                returnStatus = CosaDmlDhcpv6sSetOptionValues
                                (
                                    NULL,
                                    pPool->Cfg.InstanceNumber,
                                    ulIndex2, 
                                    pPoolOption->InstanceNumber, 
                                    pPoolOption->Alias
                                );
        
                if ( returnStatus != ANSC_STATUS_SUCCESS )
                {
                    AnscFreeMemory(pPoolOption);
                    AnscFreeMemory(pCxtLink);
                    break;                    
                }
                
                pCxtLink->InstanceNumber = pPoolOption->InstanceNumber; 

                /* Put into our list */
                CosaSListPushEntryByInsNum(&pPoolCxtLink->OptionList, (PCOSA_CONTEXT_LINK_OBJECT)pCxtLink);
            } 
            else
            {
                pCxtLink->InstanceNumber = pPoolOption->InstanceNumber; 
                
                /* This case shouldn't happen. Add it just for simulation code run well */
                if ( pPoolCxtLink->maxInstanceOfOption < pPoolOption->InstanceNumber )
                {
                    pPoolCxtLink->maxInstanceOfOption        = pPoolOption->InstanceNumber;
                    bNeedSave                                = TRUE;
                }

                /* if this entry is in link tree already because it's  delay_added table */
                pCxtLink2 = (PCOSA_CONTEXT_LINK_OBJECT)CosaSListGetEntryByInsNum(&pPoolCxtLink->OptionList, pPoolOption->InstanceNumber);
                if ( !pCxtLink2 )
                {
                    CosaSListPushEntryByInsNum(&pPoolCxtLink->OptionList, (PCOSA_CONTEXT_LINK_OBJECT)pCxtLink);
                }
                else
                {
                    AnscFreeMemory( pCxtLink2->hContext );                
                    pCxtLink2->hContext       = (ANSC_HANDLE)pPoolOption;
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

        /* We begin treat DHCPv6.Server.{i}.Pool.{i}.Client.
                    This is one dynamic table. We get all once */
        returnStatus = CosaDmlDhcpv6sGetClient
                        (
                            NULL,
                            pPool->Cfg.InstanceNumber,
                            &pPoolClient,
                            &count1
                        );
        if ( returnStatus == ANSC_STATUS_SUCCESS )
        {
            pPoolCxtLink->pClientList       = pPoolClient;
            pPoolCxtLink->NumberOfClient    = count1;
        }
        else
        {
            CcspTraceWarning(("CosaDhcpv6BackendGetDhcpv6Info -- CosaDmlDhcpv6cGetServerCfg() return error:%d.\n", returnStatus));
        }

        /* The following three tables are static table. We can get them one by one
                   DHCPv6.Server.{i}.Pool.{i}.Client.{i}.IPv6Address.
                   DHCPv6.Server.{i}.Pool.{i}.Client.{i}.IPv6Prefix.
                   DHCPv6.Server.{i}.Pool.{i}.Client.{i}.Option.                    
                */       
        pPoolCxtLink->pClientContentList = (PCOSA_DML_DHCPSV6_CLIENTCONTENT)AnscAllocateMemory( count1 * sizeof(COSA_DML_DHCPSV6_CLIENTCONTENT) );
        if ( pPoolCxtLink->pClientContentList == NULL )
        {
            continue;
        }

        for ( ulIndex2 = 0; ulIndex2 < count1; ulIndex2++ )
        {
            /* DHCPv6.Server.{i}.Pool.{i}.Client.{i}.IPv6Address. */
            returnStatus = CosaDmlDhcpv6sGetIPv6Address
                            (
                                NULL,
                                pPool->Cfg.InstanceNumber,
                                ulIndex2,
                                &pPoolIPv6Address,
                                &count
                            );
            
            if ( returnStatus == ANSC_STATUS_SUCCESS )
            {
                pPoolCxtLink->pClientContentList[ulIndex2].pIPv6Address         = pPoolIPv6Address;
                pPoolCxtLink->pClientContentList[ulIndex2].NumberofIPv6Address  = count;
            }
            else
            {
                CcspTraceWarning(("CosaDhcpv6BackendGetDhcpv6Info -- CosaDmlDhcpv6sGetIPv6Address() return error:%d.\n", returnStatus));
            }

            /* DHCPv6.Server.{i}.Pool.{i}.Client.{i}.IPv6Prefix. */
            returnStatus = CosaDmlDhcpv6sGetIPv6Prefix
                            (
                                NULL,
                                pPool->Cfg.InstanceNumber,
                                ulIndex2,
                                &pPoolIPv6Prefix,
                                &count
                            );
            
            if ( returnStatus == ANSC_STATUS_SUCCESS )
            {
                pPoolCxtLink->pClientContentList[ulIndex2].pIPv6Prefix          = pPoolIPv6Prefix;
                pPoolCxtLink->pClientContentList[ulIndex2].NumberofIPv6Prefix   = count;
            }
            else
            {
                CcspTraceWarning(("CosaDhcpv6BackendGetDhcpv6Info -- CosaDmlDhcpv6sGetIPv6Prefix() return error:%d.\n", returnStatus));
            }

            /* DHCPv6.Server.{i}.Pool.{i}.Client.{i}.Option. */
            returnStatus = CosaDmlDhcpv6sGetIPv6Option
                            (
                                NULL,
                                pPool->Cfg.InstanceNumber,
                                ulIndex2,
                                &pPoolIPv6Option,
                                &count
                            );
            
            if ( returnStatus == ANSC_STATUS_SUCCESS )
            {
                pPoolCxtLink->pClientContentList[ulIndex2].pOption          = pPoolIPv6Option;
                pPoolCxtLink->pClientContentList[ulIndex2].NumberofOption   = count;
            }
            else
            {
                CcspTraceWarning(("CosaDhcpv6BackendGetDhcpv6Info -- CosaDmlDhcpv6sGetIPv6Option() return error:%d.\n", returnStatus));
            }

        }
       
    }

    /* Max InstanceNumber is changed. Save now.*/
    if (bNeedSave)
    {
        CosaDhcpv6RegSetDhcpv6Info(pDhcpv6);
    }


    return returnStatus;    
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        CosaDhcpv6RegGetDhcpv6Info
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve backend inform and put them into our trees.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of DHCPv6
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CosaDhcpv6RegGetDhcpv6Info
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DHCPV6          pMyObject         = (PCOSA_DATAMODEL_DHCPV6   )hThisObject;
    
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoDhcpv6 = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderDhcpv6;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoClient = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoSntOpt = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoPool   = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoPoolOption       = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoEnumClient       = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoEnumSntOpt    = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoEnumPool         = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoEnumPoolOption   = (PPOAM_IREP_FOLDER_OBJECT )NULL;

    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCosaDhcpcContext = NULL;
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCosaPoolContext = NULL;
    PCOSA_CONTEXT_LINK_OBJECT         pCosaSentOptionContext     = NULL;
    PCOSA_CONTEXT_LINK_OBJECT         pCosaPoolOptionContext     = NULL;
    
    PSLAP_VARIABLE                  pSlapVariable     = NULL;
    ULONG                           ulEntryCount      = 0;
    ULONG                           ulIndex           = 0;
    ULONG                           ulEntryCount2     = 0;
    ULONG                           ulIndex2          = 0;
    ULONG                           uInstanceNumber   = 0;
    BOOL                            bNew              = FALSE;
    char*                           pAliasClient      = NULL;
    char*                           pAliasSentOption  = NULL;
    char*                           pAliasPool        = NULL;
    char*                           pAliasPoolOption  = NULL;
    char*                           pFolderName       = NULL;
    
    PCOSA_DML_DHCPCV6_FULL          pDhcpv6Client     = NULL;
    PCOSA_DML_DHCPCV6_SENT          pDhcpv6SntOpt     = NULL;
    PCOSA_DML_DHCPSV6_POOL_FULL     pDhcpv6Pool       = NULL;
    PCOSA_DML_DHCPSV6_POOL_OPTION      pDhcpv6PoolOption = NULL;

    if ( !pPoamIrepFoDhcpv6 )
    {
        return ANSC_STATUS_FAILURE;
    }

    /* This is saved structure for DHCPv6
        *****************************************
              <Dhcpv6>
                  <client>
                      <NextInstanceNumber> xxx </>
                      <1>
                           <alias>xxx</>
                           <bNew>false</>
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
                        <Option>
                            <NextInstanceNumber> xxx </>
                            <1>
                                <alias>xxx</>
                                <bNew>true</>
                            </1>
                        </Option>
                    </1>
              </pool>
            </Dhcpv6>
      ****************************************************
      */

    /* Get Folder Client */ 
    pPoamIrepFoClient  = 
        (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoDhcpv6->GetFolder
            (
                (ANSC_HANDLE)pPoamIrepFoDhcpv6,
                COSA_IREP_FOLDER_NAME_DHCPV6_CLIENT
            );

    if ( !pPoamIrepFoClient )
    {
        returnStatus      = ANSC_STATUS_FAILURE;
        goto EXIT1;
    }

    /* Get Client.NextInstanceNumber */
    if ( TRUE )
    {
        pSlapVariable =
            (PSLAP_VARIABLE)pPoamIrepFoClient->GetRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoClient,
                    COSA_DML_RR_NAME_Dhcpv6NextInsNunmber,
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

        /*get folder client.{i} */
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
                        COSA_DML_RR_NAME_Dhcpv6Alias,
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
                        COSA_DML_RR_NAME_Dhcpv6bNew,
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
        /* Firstly create dhcpc content struct */
        pDhcpv6Client = (PCOSA_DML_DHCPCV6_FULL)AnscAllocateMemory(sizeof(COSA_DML_DHCPCV6_FULL));
        if ( !pDhcpv6Client )
        {
            returnStatus = ANSC_STATUS_FAILURE;            
            goto EXIT2;
        }

        /* set some default value firstly */
        DHCPV6_CLIENT_SET_DEFAULTVALUE(pDhcpv6Client);

        /* save alias and instanceNumber */
        pDhcpv6Client->Cfg.InstanceNumber = uInstanceNumber;
        AnscCopyString( pDhcpv6Client->Cfg.Alias, pAliasClient );
        if (pAliasClient)
        {
            AnscFreeMemory(pAliasClient);
            pAliasClient = NULL;
        }
        
        /* Create one link point */
        pCosaDhcpcContext = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_DHCPCV6_LINK_OBJECT));
        if ( !pCosaDhcpcContext )
        {
            returnStatus = ANSC_STATUS_FAILURE;
            goto EXIT3;
        }

        DHCPV6_CLIENT_INITIATION_CONTEXT(pCosaDhcpcContext)

        pCosaDhcpcContext->InstanceNumber = uInstanceNumber;
        pCosaDhcpcContext->hContext       = (ANSC_HANDLE)pDhcpv6Client;
        pDhcpv6Client                     = NULL; /* reset to NULL */
        pCosaDhcpcContext->bNew           = bNew; /* set to true */

        CosaSListPushEntryByInsNum(&pMyObject->ClientList, (PCOSA_CONTEXT_LINK_OBJECT)pCosaDhcpcContext);

        /* 
                   Begin treat client.{i}.sentOption. 
                */
        pPoamIrepFoSntOpt = 
            (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoEnumClient->GetFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoEnumClient,
                    COSA_IREP_FOLDER_NAME_DHCPV6_SENTOPTION
                );
        
        if ( !pPoamIrepFoSntOpt )
        {
            goto ClientEnd;
        }
        
        /* Get Maximum number */
        if ( TRUE )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoSntOpt->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoSntOpt,
                        COSA_DML_RR_NAME_Dhcpv6NextInsNunmber,
                        NULL
                    );
        
            if ( pSlapVariable )
            {
                pCosaDhcpcContext->maxInstanceOfSent = pSlapVariable->Variant.varUint32;
        
                SlapFreeVariable(pSlapVariable);
            }
        }
        
        /* enumerate client.{i}.sentOption.{i} */
        ulEntryCount2 = pPoamIrepFoSntOpt->GetFolderCount((ANSC_HANDLE)pPoamIrepFoSntOpt);

        for ( ulIndex2 = 0; ulIndex2 < ulEntryCount2; ulIndex2++ )
        {
            /* Get i in client.{i}.sentOption.{i} */
            pFolderName =
                pPoamIrepFoSntOpt->EnumFolder
                    (
                        (ANSC_HANDLE)pPoamIrepFoSntOpt,
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
        
            pPoamIrepFoEnumSntOpt = pPoamIrepFoSntOpt->GetFolder((ANSC_HANDLE)pPoamIrepFoSntOpt, pFolderName);
        
            AnscFreeMemory(pFolderName);
            
            if ( !pPoamIrepFoEnumSntOpt )
            {
                continue;
            }
            
            /* Get client.{i}.sentOption.{i}.Alias value*/
            if ( TRUE )
            {
                pSlapVariable =
                    (PSLAP_VARIABLE)pPoamIrepFoEnumSntOpt->GetRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoEnumSntOpt,
                            COSA_DML_RR_NAME_Dhcpv6Alias,
                            NULL
                        );
            
                if ( pSlapVariable )
                {
                    pAliasSentOption = AnscCloneString(pSlapVariable->Variant.varString);
            
                    SlapFreeVariable(pSlapVariable);
                }
            }
            
            /* Get client.{i}.sentOption.{i}.bNew value*/
            if ( TRUE )
            {
                pSlapVariable =
                    (PSLAP_VARIABLE)pPoamIrepFoEnumSntOpt->GetRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoEnumSntOpt,
                            COSA_DML_RR_NAME_Dhcpv6bNew,
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
            pDhcpv6SntOpt = (PCOSA_DML_DHCPCV6_SENT)AnscAllocateMemory(sizeof(COSA_DML_DHCPCV6_SENT));
            if ( !pDhcpv6SntOpt )
            {
                returnStatus = ANSC_STATUS_FAILURE;
                AnscFreeMemory(pDhcpv6SntOpt); /*RDKB-6737, CID-32983, free unused resource before exit*/
                pDhcpv6SntOpt = NULL;
                goto EXIT3;
            }

            /* set some default value firstly */
            DHCPV6_SENTOPTION_SET_DEFAULTVALUE(pDhcpv6SntOpt);

            /* save alias and instanceNumber */
            pDhcpv6SntOpt->InstanceNumber = uInstanceNumber;
            AnscCopyString( pDhcpv6SntOpt->Alias, pAliasSentOption );
            if (pAliasSentOption)
            {
                AnscFreeMemory(pAliasSentOption);
                pAliasSentOption = NULL;
            }

            pCosaSentOptionContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
            if ( !pCosaSentOptionContext )
            {
                returnStatus = ANSC_STATUS_FAILURE;            
                goto EXIT2;
            }

            COSA_CONTEXT_LINK_INITIATION_CONTENT(pCosaSentOptionContext);

            pCosaSentOptionContext->InstanceNumber = uInstanceNumber;
            pCosaSentOptionContext->hContext       = (ANSC_HANDLE)pDhcpv6SntOpt;
            pDhcpv6SntOpt = NULL;
            pCosaSentOptionContext->hParentTable   = pCosaDhcpcContext;
            pCosaSentOptionContext->bNew           = bNew;

            CosaSListPushEntryByInsNum(&pCosaDhcpcContext->SentOptionList, (PCOSA_CONTEXT_LINK_OBJECT)pCosaSentOptionContext);
        
            /* release some memory */  
            pPoamIrepFoEnumSntOpt->Remove((ANSC_HANDLE)pPoamIrepFoEnumSntOpt);
            pPoamIrepFoEnumSntOpt = NULL;            
        }

        pPoamIrepFoSntOpt->Remove((ANSC_HANDLE)pPoamIrepFoSntOpt);
        pPoamIrepFoSntOpt = NULL;

ClientEnd:
        /* release some memory */
        pPoamIrepFoEnumClient->Remove((ANSC_HANDLE)pPoamIrepFoEnumClient);
        pPoamIrepFoEnumClient = NULL;
        
    }

    pPoamIrepFoClient->Remove((ANSC_HANDLE)pPoamIrepFoClient);
    pPoamIrepFoClient = NULL;

    /*
            Begin process pool.{i}, and pool.{i}.option.{j}
        */
        
    /* Get Folder Client */ 
    pPoamIrepFoPool  = 
        (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoDhcpv6->GetFolder
            (
                (ANSC_HANDLE)pPoamIrepFoDhcpv6,
                COSA_IREP_FOLDER_NAME_DHCPV6_POOL
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
                    COSA_DML_RR_NAME_Dhcpv6NextInsNunmber,
                    NULL
                );
    
        if ( pSlapVariable )
        {
            pMyObject->maxInstanceOfPool = pSlapVariable->Variant.varUint32;
    
            SlapFreeVariable(pSlapVariable);
        }
    }
    
    /* enumerate server.pool.{i} */
    /*ulEntryCount = pPoamIrepFoPool->GetFolderCount((ANSC_HANDLE)pPoamIrepFoPool);*/
	/*We don't use PSM to save informaiont for DHCPv6*/
	ulEntryCount = 0;
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
                        COSA_DML_RR_NAME_Dhcpv6Alias,
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
                        COSA_DML_RR_NAME_Dhcpv6bNew,
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
        pDhcpv6Pool = (PCOSA_DML_DHCPSV6_POOL_FULL)AnscAllocateMemory(sizeof(COSA_DML_DHCPSV6_POOL_FULL));
        if ( !pDhcpv6Pool )
        {
            returnStatus = ANSC_STATUS_FAILURE;            
            goto EXIT2;
        }
    
        /* set some default value firstly */
        DHCPV6_POOL_SET_DEFAULTVALUE(pDhcpv6Pool);
        
        /* save alias and instanceNumber */
        pDhcpv6Pool->Cfg.InstanceNumber = uInstanceNumber;
        AnscCopyString( pDhcpv6Pool->Cfg.Alias, pAliasPool );
        if (pAliasPool)
        {
            AnscFreeMemory(pAliasPool);
            pAliasPool = NULL;
        }

        pCosaPoolContext = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_POOLV6_LINK_OBJECT));
        if ( !pCosaPoolContext )
        {
            returnStatus = ANSC_STATUS_FAILURE;
            goto EXIT3;
        }
    
        DHCPV6_POOL_INITIATION_CONTEXT(pCosaPoolContext)
    
        pCosaPoolContext->InstanceNumber = uInstanceNumber;
        pCosaPoolContext->hContext       = (ANSC_HANDLE)pDhcpv6Pool;
        pDhcpv6Pool                      = NULL;
        pCosaPoolContext->hParentTable   = NULL;
        pCosaPoolContext->bNew           = bNew;
    
        CosaSListPushEntryByInsNum(&pMyObject->PoolList, (PCOSA_CONTEXT_LINK_OBJECT)pCosaPoolContext);

        /*************************************
               * Begin treat pool.{i}.option.            *
               *************************************/
        pPoamIrepFoPoolOption = 
            (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoEnumPool->GetFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoEnumPool,
                    COSA_IREP_FOLDER_NAME_DHCPV6_OPTION
                );
        
        if ( !pPoamIrepFoPoolOption )
        {
            goto PoolEnd;
        }
        
        /* Get Maximum number */
        if ( TRUE )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoPoolOption->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoPoolOption,
                        COSA_DML_RR_NAME_Dhcpv6NextInsNunmber,
                        NULL
                    );
        
            if ( pSlapVariable )
            {
                pCosaPoolContext->maxInstanceOfOption = pSlapVariable->Variant.varUint32;
        
                SlapFreeVariable(pSlapVariable);
            }
        }

        /* enumerate pool.{i}.option.{i} */
        ulEntryCount2 = pPoamIrepFoPoolOption->GetFolderCount((ANSC_HANDLE)pPoamIrepFoPoolOption);

        for ( ulIndex2 = 0; ulIndex2 < ulEntryCount2; ulIndex2++ )
        {
            /* Get i in client.{i}.option.{i} */
            pFolderName =
                pPoamIrepFoPoolOption->EnumFolder
                    (
                        (ANSC_HANDLE)pPoamIrepFoPoolOption,
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
    
            pPoamIrepFoEnumPoolOption = pPoamIrepFoPoolOption->GetFolder((ANSC_HANDLE)pPoamIrepFoPoolOption, pFolderName);
    
            AnscFreeMemory(pFolderName);
            
            if ( !pPoamIrepFoEnumPoolOption )
            {
                continue;
            }
            
            /* Get pool.{i}.option.{i}.Alias value*/
            if ( TRUE )
            {
                pSlapVariable =
                    (PSLAP_VARIABLE)pPoamIrepFoEnumPoolOption->GetRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoEnumPoolOption,
                            COSA_DML_RR_NAME_Dhcpv6Alias,
                            NULL
                        );
            
                if ( pSlapVariable )
                {
                    pAliasPoolOption = AnscCloneString(pSlapVariable->Variant.varString);
            
                    SlapFreeVariable(pSlapVariable);
                }
            }

            /* Get pool.{i}.bNew value*/
            if ( TRUE )
            {
                pSlapVariable =
                    (PSLAP_VARIABLE)pPoamIrepFoEnumPoolOption->GetRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoEnumPoolOption,
                            COSA_DML_RR_NAME_Dhcpv6bNew,
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
            pDhcpv6PoolOption    = (PCOSA_DML_DHCPSV6_POOL_OPTION)AnscAllocateMemory(sizeof(COSA_DML_DHCPSV6_POOL_OPTION ));
            if ( !pDhcpv6PoolOption )
            {
                returnStatus = ANSC_STATUS_FAILURE;                
                goto EXIT3;
            }
    
            /* set some default value firstly */
            DHCPV6_POOLOPTION_SET_DEFAULTVALUE(pDhcpv6PoolOption);
            
            /* save alias and instanceNumber */
            pDhcpv6PoolOption->InstanceNumber = uInstanceNumber;
            AnscCopyString( pDhcpv6PoolOption->Alias, pAliasPoolOption );
            if (pAliasPoolOption)
            {
                AnscFreeMemory(pAliasPoolOption);
                pAliasPoolOption = NULL;
            }

            pCosaPoolOptionContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
            if ( !pCosaPoolOptionContext )
            {
                returnStatus = ANSC_STATUS_FAILURE;            
                goto EXIT2;
            }

            COSA_CONTEXT_LINK_INITIATION_CONTENT(pCosaPoolOptionContext)

            pCosaPoolOptionContext->InstanceNumber = uInstanceNumber;
            pCosaPoolOptionContext->hContext       = (ANSC_HANDLE)pDhcpv6PoolOption   ;
            pDhcpv6PoolOption    = NULL;
            pCosaPoolOptionContext->hParentTable   = pCosaPoolContext;
            pCosaPoolOptionContext->bNew           = bNew;

            CosaSListPushEntryByInsNum(&pCosaPoolContext->OptionList, (PCOSA_CONTEXT_LINK_OBJECT)pCosaPoolOptionContext);

            /* release some memory */
            
            pPoamIrepFoEnumPoolOption->Remove((ANSC_HANDLE)pPoamIrepFoEnumPoolOption);
            pPoamIrepFoEnumPoolOption = NULL;
        }
    
        pPoamIrepFoPoolOption->Remove((ANSC_HANDLE)pPoamIrepFoPoolOption);
        pPoamIrepFoPoolOption = NULL;

PoolEnd:

        /* release some memory */
        
        pPoamIrepFoEnumPool->Remove((ANSC_HANDLE)pPoamIrepFoEnumPool);
        pPoamIrepFoEnumPool = NULL;
    }
    
    pPoamIrepFoPool->Remove((ANSC_HANDLE)pPoamIrepFoPool);
    pPoamIrepFoPool = NULL;

    
EXIT3:
    if(pDhcpv6Client)
        AnscFreeMemory(pDhcpv6Client);

    if(pDhcpv6Pool )
        AnscFreeMemory(pDhcpv6Pool);

    if(pDhcpv6PoolOption   )
        AnscFreeMemory(pDhcpv6PoolOption   );
        
EXIT2:
    
    if(pAliasSentOption)
        AnscFreeMemory(pAliasSentOption);
    
    if(pAliasClient)
        AnscFreeMemory(pAliasClient);
    
    if(pAliasPool)
        AnscFreeMemory(pAliasPool);
    
    if(pAliasPoolOption)
        AnscFreeMemory(pAliasPoolOption);

    if(pDhcpv6SntOpt )
        AnscFreeMemory(pDhcpv6SntOpt);
    
EXIT1:

    if ( pPoamIrepFoClient )
        pPoamIrepFoClient->Remove((ANSC_HANDLE)pPoamIrepFoClient);

    if ( pPoamIrepFoEnumClient )
        pPoamIrepFoEnumClient->Remove((ANSC_HANDLE)pPoamIrepFoEnumClient);

    if ( pPoamIrepFoSntOpt)
        pPoamIrepFoSntOpt->Remove((ANSC_HANDLE)pPoamIrepFoSntOpt);

    if ( pPoamIrepFoEnumSntOpt)
        pPoamIrepFoEnumSntOpt->Remove((ANSC_HANDLE)pPoamIrepFoEnumSntOpt);
    
    if ( pPoamIrepFoPool)
        pPoamIrepFoPool->Remove((ANSC_HANDLE)pPoamIrepFoPool);

    if ( pPoamIrepFoEnumPool)
        pPoamIrepFoEnumPool->Remove((ANSC_HANDLE)pPoamIrepFoEnumPool);
    
    if ( pPoamIrepFoPoolOption)
        pPoamIrepFoPoolOption->Remove((ANSC_HANDLE)pPoamIrepFoPoolOption);

    if ( pPoamIrepFoEnumPoolOption)
        pPoamIrepFoEnumPoolOption->Remove((ANSC_HANDLE)pPoamIrepFoEnumPoolOption);

    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        CosaDhcpv6RegSetDhcpv6Info
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to save current NextInstanceNumber and Delay_added
        entry into sysregistry.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of DHCPv6
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CosaDhcpv6RegSetDhcpv6Info
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DHCPV6          pMyObject         = (PCOSA_DATAMODEL_DHCPV6   )hThisObject;
    
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoDhcpv6 = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderDhcpv6;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoClient = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoSntOpt = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoPool   = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoPoolOption      = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoEnumClient      = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoEnumSntOpt      = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoEnumPool        = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoEnumPoolOption  = (PPOAM_IREP_FOLDER_OBJECT )NULL;

    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY       )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry2      = (PSINGLE_LINK_ENTRY       )NULL;
    
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCosaDhcpcContext = NULL;
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCosaPoolContext  = NULL;
    PCOSA_CONTEXT_LINK_OBJECT         pCosaSentOptionContext   = NULL;
    PCOSA_CONTEXT_LINK_OBJECT         pCosaPoolOptionContext   = NULL;
    
    PSLAP_VARIABLE                  pSlapVariable     = NULL;
    ULONG                           ulEntryCount      = 0;
    ULONG                           ulIndex           = 0;
    ULONG                           ulEntryCount2     = 0;
    ULONG                           ulIndex2          = 0;
    ULONG                           uInstanceNumber   = 0;
    char*                           pAliasClient      = NULL;
    char*                           pAliasSentOption  = NULL;
    char*                           pAliasPool        = NULL;
    char*                           pAliasPoolOption  = NULL;
    char*                           pFolderName       = NULL;
    char                            FolderName[16]    = {0};
    
    PCOSA_DML_DHCPCV6_FULL          pDhcpv6Client     = NULL;
    PCOSA_DML_DHCPCV6_SENT          pDhcpv6SntOpt     = NULL;
    PCOSA_DML_DHCPSV6_POOL_FULL     pDhcpv6Pool       = NULL;
    PCOSA_DML_DHCPSV6_POOL_OPTION      pDhcpv6PoolOption = NULL;

    if ( !pPoamIrepFoDhcpv6 )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pPoamIrepFoDhcpv6->EnableFileSync((ANSC_HANDLE)pPoamIrepFoDhcpv6, FALSE);
    }

    if ( TRUE )
    {
        pPoamIrepFoDhcpv6->Clear((ANSC_HANDLE)pPoamIrepFoDhcpv6);

        SlapAllocVariable(pSlapVariable);

        if ( !pSlapVariable )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT1;
        }
    }

    /* This is saved structure for DHCPv6
          *****************************************
                <Dhcpv6>
                    <client>
                        <NextInstanceNumber> xxx </>
                        <1>
                             <alias>xxx</>
                             <bNew>false</>
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
                          <Option>
                              <NextInstanceNumber> xxx </>
                              <1>
                                  <alias>xxx</>
                                  <bNew>true</>
                              </1>
                          </Option>
                      </1>
                </pool>
              </Dhcpv6>
        ****************************************************
      */

    /* Add DHCPv6.client.*/
    pPoamIrepFoClient =
        pPoamIrepFoDhcpv6->AddFolder
            (
                (ANSC_HANDLE)pPoamIrepFoDhcpv6,
                COSA_IREP_FOLDER_NAME_DHCPV6_CLIENT,
                0
            );

    if ( !pPoamIrepFoClient )
    {
        goto EXIT1;
    }

    /* add client.{i}.NextInstanceNumber  */
    if ( TRUE )
    {
        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
        pSlapVariable->Variant.varUint32 = pMyObject->maxInstanceOfClient;
    
        returnStatus =
            pPoamIrepFoClient->AddRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoClient,
                    COSA_DML_RR_NAME_Dhcpv6NextInsNunmber,
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
        /* create DHCPv6.client.{i} */
        
        pCosaDhcpcContext = ACCESS_COSA_CONTEXT_DHCPCV6_LINK_OBJECT(pSLinkEntry);
        pSLinkEntry       = AnscSListGetNextEntry(pSLinkEntry);

        pDhcpv6Client = (PCOSA_DML_DHCPCV6_FULL)pCosaDhcpcContext->hContext;

        /* When this entry has been added to backend, has not any child and maxInstanceNumber is 0
                  We need not save this entry  */
        if ( !pCosaDhcpcContext->bNew && 
             !AnscSListQueryDepth(&pCosaDhcpcContext->SentOptionList ) &&
             ( pCosaDhcpcContext->maxInstanceOfSent == 0 )
           )
        {
            continue;
        }

        _ansc_sprintf(FolderName, "%d", pCosaDhcpcContext->InstanceNumber);

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

        /* add  DHCPv6.client.{i}.alias */
        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;
            pSlapVariable->Variant.varString = AnscCloneString(pDhcpv6Client->Cfg.Alias);

            returnStatus =
                pPoamIrepFoEnumClient->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEnumClient,
                        COSA_DML_RR_NAME_Dhcpv6Alias,
                        SYS_REP_RECORD_TYPE_ASTR,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );

            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }

        /* add  DHCPv6.client.{i}.bNew */
        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_bool;
            pSlapVariable->Variant.varBool   = pCosaDhcpcContext->bNew;

            returnStatus =
                pPoamIrepFoEnumClient->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEnumClient,
                        COSA_DML_RR_NAME_Dhcpv6bNew,
                        SYS_REP_RECORD_TYPE_BOOL,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );

            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }

        /*
                    begin add sentOption
                */
        if ( !AnscSListQueryDepth(&pCosaDhcpcContext->SentOptionList) )
        {
            goto ClientEnd;
        }

        /* Add DHCPv6.client.{i}.sentOption */
        pPoamIrepFoSntOpt =
            pPoamIrepFoEnumClient->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoEnumClient,
                    COSA_IREP_FOLDER_NAME_DHCPV6_SENTOPTION,
                    0
                );
        
        if ( !pPoamIrepFoSntOpt )
        {
            goto EXIT1;
        }
        
        /* add client.{i}.sendOption.maxInstanceNumber  */
        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
            pSlapVariable->Variant.varUint32 = pCosaDhcpcContext->maxInstanceOfSent;
        
            returnStatus =
                pPoamIrepFoSntOpt->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoSntOpt,
                        COSA_DML_RR_NAME_Dhcpv6NextInsNunmber,
                        SYS_REP_RECORD_TYPE_UINT,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );
        
            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }
        
        pSLinkEntry2 = AnscSListGetFirstEntry(&pCosaDhcpcContext->SentOptionList);
        
        while ( pSLinkEntry2 )
        {
            /* create DHCPv6.client.{i}.sentOption.{i} */
        
            pCosaSentOptionContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry2);
            pSLinkEntry2          = AnscSListGetNextEntry(pSLinkEntry2);
        
            pDhcpv6SntOpt= (PCOSA_DML_DHCPCV6_SENT)pCosaSentOptionContext->hContext;

            if ( !pCosaSentOptionContext->bNew )
            {
                continue;
            }
        
            _ansc_sprintf(FolderName, "%d", pCosaSentOptionContext->InstanceNumber);
        
            pPoamIrepFoEnumSntOpt =
                pPoamIrepFoSntOpt->AddFolder
                    (
                        (ANSC_HANDLE)pPoamIrepFoSntOpt,
                        FolderName,
                        0
                    );
        
            if ( !pPoamIrepFoEnumSntOpt )
            {
                continue;
            }

            /* create DHCPv6.client.{i}.sendOption.{i}.alias */
            if ( TRUE )
            {
                pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;
                pSlapVariable->Variant.varString = AnscCloneString(pDhcpv6SntOpt->Alias);
        
                returnStatus =
                    pPoamIrepFoEnumSntOpt->AddRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoEnumSntOpt,
                            COSA_DML_RR_NAME_Dhcpv6Alias,
                            SYS_REP_RECORD_TYPE_ASTR,
                            SYS_RECORD_CONTENT_DEFAULT,
                            pSlapVariable,
                            0
                        );
                
                SlapCleanVariable(pSlapVariable);
                SlapInitVariable (pSlapVariable);
            }

            /* create DHCPv6.client.{i}.sendOption.{i}.bNew */
            if ( TRUE )
            {
                pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_bool;
                pSlapVariable->Variant.varBool   = pCosaSentOptionContext->bNew;
        
                returnStatus =
                    pPoamIrepFoEnumSntOpt->AddRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoEnumSntOpt,
                            COSA_DML_RR_NAME_Dhcpv6bNew,
                            SYS_REP_RECORD_TYPE_BOOL,
                            SYS_RECORD_CONTENT_DEFAULT,
                            pSlapVariable,
                            0
                        );
                
                SlapCleanVariable(pSlapVariable);
                SlapInitVariable (pSlapVariable);
            }

            pPoamIrepFoEnumSntOpt->Remove((ANSC_HANDLE)pPoamIrepFoEnumSntOpt);
            pPoamIrepFoEnumSntOpt = NULL;
        }
        
        pPoamIrepFoSntOpt->Remove((ANSC_HANDLE)pPoamIrepFoSntOpt);
        pPoamIrepFoSntOpt = NULL;
        
ClientEnd:

        /*release some resource */        
        pPoamIrepFoEnumClient->Remove((ANSC_HANDLE)pPoamIrepFoEnumClient);
        pPoamIrepFoEnumClient = NULL;
    }
    
    pPoamIrepFoClient->Remove((ANSC_HANDLE)pPoamIrepFoClient);
    pPoamIrepFoClient = NULL;


    /*
            begin process pool and option
        */

    /* Add DHCPv6.pool.*/
    pPoamIrepFoPool =
        pPoamIrepFoDhcpv6->AddFolder
            (
                (ANSC_HANDLE)pPoamIrepFoDhcpv6,
                COSA_IREP_FOLDER_NAME_DHCPV6_POOL,
                0
            );

    if ( !pPoamIrepFoPool )
    {
        goto EXIT1;
    }

    /* add pool.{i}.NextInstanceNumber  */
    if ( TRUE )
    {
        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
        pSlapVariable->Variant.varUint32 = pMyObject->maxInstanceOfPool;
    
        returnStatus =
            pPoamIrepFoPool->AddRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoPool,
                    COSA_DML_RR_NAME_Dhcpv6NextInsNunmber,
                    SYS_REP_RECORD_TYPE_UINT,
                    SYS_RECORD_CONTENT_DEFAULT,
                    pSlapVariable,
                    0
                );
    
        SlapCleanVariable(pSlapVariable);
        SlapInitVariable (pSlapVariable);
    }
    
    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->PoolList);

	/*Currently we don't use PSM to save information for DHCPv6 Server*/
	/*    while ( pSLinkEntry )*/
	while (0)
    {
        /* create DHCPv6.pool.{i} */
        
        pCosaPoolContext = ACCESS_COSA_CONTEXT_POOLV6_LINK_OBJECT(pSLinkEntry);
        pSLinkEntry       = AnscSListGetNextEntry(pSLinkEntry);

        pDhcpv6Pool = (PCOSA_DML_DHCPSV6_POOL_FULL)pCosaPoolContext->hContext;

        /*
                    When  not bNew and have not any child, maxInstanceNum is 0, we will not save this entry.
                */
        if ( !pCosaPoolContext->bNew && 
             !AnscSListQueryDepth(&pCosaPoolContext->OptionList) &&
             pCosaPoolContext->maxInstanceOfOption == 0
           )
        {
            continue;
        }

        _ansc_sprintf(FolderName, "%d", pCosaPoolContext->InstanceNumber);

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

        /* add  DHCPv6.pool.{i}.alias */
        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;
            pSlapVariable->Variant.varString = AnscCloneString(pDhcpv6Pool->Cfg.Alias);

            returnStatus =
                pPoamIrepFoEnumPool->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEnumPool,
                        COSA_DML_RR_NAME_Dhcpv6Alias,
                        SYS_REP_RECORD_TYPE_ASTR,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );

            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }

        /* add  DHCPv6.pool.{i}.bNew */
        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_bool;
            pSlapVariable->Variant.varBool   = pCosaPoolContext->bNew;

            returnStatus =
                pPoamIrepFoEnumPool->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEnumPool,
                        COSA_DML_RR_NAME_Dhcpv6bNew,
                        SYS_REP_RECORD_TYPE_BOOL,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );

            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }

        /*
                    begin add pool.{i}.option
                */
        if ( !AnscSListQueryDepth(&pCosaPoolContext->OptionList) )
        {
            goto PoolEnd;
        }
        
        /* Add DHCPv6.pool.{i}.Option */
        pPoamIrepFoPoolOption    =
            pPoamIrepFoEnumPool->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoEnumPool,
                    COSA_IREP_FOLDER_NAME_DHCPV6_OPTION,
                    0
                );
        
        if ( !pPoamIrepFoPoolOption    )
        {
            goto EXIT1;
        }

        /* add pool.{i}.pool.NextInstanceNumber  */
        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
            pSlapVariable->Variant.varUint32 = pCosaPoolContext->maxInstanceOfOption;
        
            returnStatus =
                pPoamIrepFoPoolOption   ->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoPoolOption   ,
                        COSA_DML_RR_NAME_Dhcpv6NextInsNunmber,
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
            /* create DHCPv6.pool.{i}.option.{i} */

            pCosaPoolOptionContext    = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry2);
            pSLinkEntry2              = AnscSListGetNextEntry(pSLinkEntry2);
        
            pDhcpv6PoolOption         = (PCOSA_DML_DHCPSV6_POOL_OPTION)pCosaPoolOptionContext->hContext;

            if ( !pCosaPoolOptionContext->bNew )
            {
                continue;
            }
            
            _ansc_sprintf(FolderName, "%d", pCosaPoolOptionContext->InstanceNumber);
        
            pPoamIrepFoEnumPoolOption    =
                pPoamIrepFoPoolOption->AddFolder
                    (
                        (ANSC_HANDLE)pPoamIrepFoPoolOption,
                        FolderName,
                        0
                    );
        
            if ( !pPoamIrepFoEnumPoolOption    )
            {
                continue;
            }
            
            /* create DHCPv6.pool.{i}.pool.{i}.alias */
            if ( TRUE )
            {
                pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;
                pSlapVariable->Variant.varString = AnscCloneString(pDhcpv6PoolOption->Alias);

                returnStatus =
                    pPoamIrepFoEnumPoolOption->AddRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoEnumPoolOption,
                            COSA_DML_RR_NAME_Dhcpv6Alias,
                            SYS_REP_RECORD_TYPE_ASTR,
                            SYS_RECORD_CONTENT_DEFAULT,
                            pSlapVariable,
                            0
                        );
                
                SlapCleanVariable(pSlapVariable);
                SlapInitVariable (pSlapVariable);
            }    
        
            /* create DHCPv6.pool.{i}.Option.{i}.bNew */
            if ( TRUE )
            {
                pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_bool;
                pSlapVariable->Variant.varBool   = pCosaPoolOptionContext->bNew;
            
                returnStatus =
                    pPoamIrepFoEnumPoolOption->AddRecord
                        (
                            (ANSC_HANDLE)pPoamIrepFoEnumPoolOption,
                            COSA_DML_RR_NAME_Dhcpv6bNew,
                            SYS_REP_RECORD_TYPE_BOOL,
                            SYS_RECORD_CONTENT_DEFAULT,
                            pSlapVariable,
                            0
                        );
                
                SlapCleanVariable(pSlapVariable);
                SlapInitVariable (pSlapVariable);
            }

            pPoamIrepFoEnumPoolOption->Remove((ANSC_HANDLE)pPoamIrepFoEnumPoolOption   );
            pPoamIrepFoEnumPoolOption    = NULL;
        }

        pPoamIrepFoPoolOption->Remove((ANSC_HANDLE)pPoamIrepFoPoolOption   );
        pPoamIrepFoPoolOption    = NULL;

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

    if ( pPoamIrepFoSntOpt)
        pPoamIrepFoSntOpt->Remove((ANSC_HANDLE)pPoamIrepFoSntOpt);

    if ( pPoamIrepFoEnumSntOpt)
        pPoamIrepFoEnumSntOpt->Remove((ANSC_HANDLE)pPoamIrepFoEnumSntOpt);
    
    if ( pPoamIrepFoPool)
        pPoamIrepFoPool->Remove((ANSC_HANDLE)pPoamIrepFoPool);

    if ( pPoamIrepFoEnumPool)
        pPoamIrepFoEnumPool->Remove((ANSC_HANDLE)pPoamIrepFoEnumPool);
    
    if ( pPoamIrepFoPoolOption   )
        pPoamIrepFoPoolOption   ->Remove((ANSC_HANDLE)pPoamIrepFoPoolOption   );

    if ( pPoamIrepFoEnumPoolOption   )
        pPoamIrepFoEnumPoolOption   ->Remove((ANSC_HANDLE)pPoamIrepFoEnumPoolOption   );

    pPoamIrepFoDhcpv6->EnableFileSync((ANSC_HANDLE)pPoamIrepFoDhcpv6, TRUE);

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        CosaDhcpv6ClientHasDelayAddedChild
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
CosaDhcpv6ClientHasDelayAddedChild
    (
        PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT                 hContext
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCosaDhcpcContext = hContext;
    
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaCxtLink      = NULL;

    pSLinkEntry = AnscSListGetFirstEntry(&pCosaDhcpcContext->SentOptionList);
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
        CosaDhcpv6PoolHasDelayAddedChild
            (
                PCOSA_CONTEXT_POOLV6_LINK_OBJECT                       hContext
            );

    description:

        This function is called to check whether this is child is pending added. If yes,
        return TRUE. Or else return FALSE.

    argument:   PCOSA_CONTEXT_POOLV6_LINK_OBJECT                hThisObject
                This handle is actually the pointer of one context link point.

    return:     TRUE or FALSE.

**********************************************************************/
BOOL
CosaDhcpv6PoolHasDelayAddedChild
    (
        PCOSA_CONTEXT_POOLV6_LINK_OBJECT                hContext
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCosaPoolContext  = hContext;
    
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaCxtLink      = NULL;

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

/*
    This function is used to find right ClientContent according to Client Entry.
*/
ANSC_HANDLE
CosaDhcpv6GetClientContentbyClient
    (
        ANSC_HANDLE                 hClientContext
    )
{
    PCOSA_DATAMODEL_DHCPV6            pDhcpv6           = (PCOSA_DATAMODEL_DHCPV6)g_pCosaBEManager->hDhcpv6;
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtLink          = NULL;
    PSINGLE_LINK_ENTRY                pSListEntry       = NULL;
    PCOSA_DML_DHCPSV6_CLIENT          pClient           = NULL;
    ULONG                             uIndex            = 0;
    PCOSA_DML_DHCPSV6_CLIENTCONTENT   pClientContent    = NULL;

    pSListEntry           = AnscSListGetFirstEntry(&pDhcpv6->PoolList);
    while( pSListEntry != NULL)
    {
        pCxtLink          = ACCESS_COSA_CONTEXT_POOLV6_LINK_OBJECT(pSListEntry);
        pSListEntry       = AnscSListGetNextEntry(pSListEntry);
    
        pClient          = (PCOSA_DML_DHCPSV6_CLIENT)pCxtLink->pClientList;
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

