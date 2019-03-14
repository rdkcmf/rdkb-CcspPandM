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

    module: cosa_dns_internal.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaDNSCreate
        *  CosaDNSInitialize
        *  CosaDNSRemove
        *  CosaDNSRegGetInfo
        *  CosaDNSRegAddInfo
        *  CosaDNSRegDelInfo
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
#include "cosa_dns_internal.h"
#include "plugin_main_apis.h"

extern void * g_pDslhDmlAgent;

/*
    this gloable var are test values
*/

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        CosaDNSCreate
            (
            );

    description:

        This function constructs cosa nat object and return handle.

    argument:  

    return:     newly created nat object.

**********************************************************************/

ANSC_HANDLE
CosaDNSCreate
    (
        VOID
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DNS             pMyObject    = (PCOSA_DATAMODEL_DNS)NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PCOSA_DATAMODEL_DNS)AnscAllocateMemory(sizeof(COSA_DATAMODEL_DNS));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->Oid               = COSA_DATAMODEL_DNS_OID;
    pMyObject->Create            = CosaDNSCreate;
    pMyObject->Remove            = CosaDNSRemove;
    pMyObject->Initialize        = CosaDNSInitialize;

    pMyObject->Initialize   ((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDNSInitialize
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
CosaDNSInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DNS             pMyObject       = (PCOSA_DATAMODEL_DNS)hThisObject;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoCOSA = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoServer   = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoServerHA = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoNextIns  = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PSLAP_VARIABLE                  pSlapVariable   = (PSLAP_VARIABLE          )NULL;
    PCOSA_DML_DNS_CLIENT_SERVER     pDnsServer      = (PCOSA_DML_DNS_CLIENT_SERVER)NULL;
    PCOSA_DML_DNS_RELAY_ENTRY       pForward        = (PCOSA_DML_DNS_RELAY_ENTRY)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2   = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    char                            FolderName[32]  = {0};
    ULONG                           ulEntryCount    = 0;
    ULONG                           ulIndex         = 0;
    ULONG                           ulEntryCount2   = 0;
    ULONG                           ulIndex2        = 0;

    CosaDmlDnsInit(NULL, NULL);

    /* Initiation all functions */
    pMyObject->ulNextServerInsNum    = 1;
    pMyObject->ulNextForwardInsNum   = 1;
    pMyObject->PreVisitSrvTime       = 0;
    pMyObject->PreVisitForwardTime   = 0;
    
    AnscSListInitializeHeader(&pMyObject->ServerList)
    AnscSListInitializeHeader(&pMyObject->ForwardList)

    pMyObject->hIrepFolderCOSA = g_GetRegistryRootFolder(g_pDslhDmlAgent);

    pPoamIrepFoCOSA = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderCOSA;
    
    if ( !pPoamIrepFoCOSA )
    {
        returnStatus = ANSC_STATUS_FAILURE;
        
        goto  EXIT;
    }

    pPoamIrepFoServer = (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoCOSA->GetFolder
                        (
                            (ANSC_HANDLE)pPoamIrepFoCOSA,
                            COSA_IREP_FOLDER_NAME_SERVER
                        );

    if ( !pPoamIrepFoServer )
    {
        pPoamIrepFoServer = pPoamIrepFoCOSA->AddFolder
                            (
                                (ANSC_HANDLE)pPoamIrepFoCOSA,
                                COSA_IREP_FOLDER_NAME_SERVER,
                                0
                            );
    }

    if ( !pPoamIrepFoServer )
    {
        returnStatus = ANSC_STATUS_FAILURE;
        
        goto  EXIT;
    }
    else
    {        
        pMyObject->hIrepFolderServer = (ANSC_HANDLE)pPoamIrepFoServer;
    }

    pPoamIrepFoServerHA = (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoServer->GetFolder
                        (
                            (ANSC_HANDLE)pPoamIrepFoCOSA,
                            COSA_IREP_FOLDER_NAME_SERVER_HA
                        );

    if ( !pPoamIrepFoServerHA )
    {
        pPoamIrepFoServerHA = pPoamIrepFoServer->AddFolder
                            (
                                (ANSC_HANDLE)pPoamIrepFoServer,
                                COSA_IREP_FOLDER_NAME_SERVER_HA,
                                0
                            );
    }

    if ( !pPoamIrepFoServerHA )
    {
        returnStatus = ANSC_STATUS_FAILURE;
        
        goto  EXIT;
    }
    else
    {
        pMyObject->hIrepFolderServerHA = (ANSC_HANDLE)pPoamIrepFoServerHA;
    }

    /* Retrieve the next Instance Number for DNS server */
    
    if ( TRUE )
    {
        _ansc_sprintf
        (
            FolderName, 
            "%s%d", 
            COSA_DML_RR_NAME_SERVER_NextInsNum,
            0
        );
        
        pPoamIrepFoNextIns = (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoServer->GetFolder
                                (
                                    (ANSC_HANDLE)pPoamIrepFoServer,
                                    FolderName
                                );

        AnscZeroMemory(FolderName, 32);

        if ( pPoamIrepFoNextIns )
        {
            pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoNextIns->GetRecord
                                (
                                    (ANSC_HANDLE)pPoamIrepFoNextIns,
                                    COSA_DML_RR_NAME_SERVER_NextInsNum,
                                    NULL
                                );

            if ( pSlapVariable )
            {
                pMyObject->ulNextServerInsNum = pSlapVariable->Variant.varUint32;
                
                SlapFreeVariable(pSlapVariable);
            }

            pPoamIrepFoNextIns->Remove((ANSC_HANDLE)pPoamIrepFoNextIns);
            pPoamIrepFoNextIns = NULL;
        }
    }  

    /* Initialize middle layer for Device.DNS.Client.Server.{i}.  */

    /* local variable, tmp memory, need to free it in this routine */
    pDnsServer = CosaDmlDnsClientGetServers(NULL, (PULONG)&ulEntryCount);
    
    if ( !pDnsServer || (ulEntryCount ==0))
    {
        if(pDnsServer) /*RDKB-6738, CID-33327, free unused resources before exit*/
        {
            AnscFreeMemory(pDnsServer);
            pDnsServer = NULL;
        }
        returnStatus = ANSC_STATUS_RESOURCES;
        goto  EXIT;
    }
    else
    {
        for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
        {
            pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

            if ( !pCosaContext )
            {
                AnscFreeMemory(pDnsServer); /*RDKB-6738, CID-32697*/
                returnStatus = ANSC_STATUS_RESOURCES;
                goto  EXIT;
            }

            if ( 0 != pDnsServer[ulIndex].InstanceNumber )
            {
                pCosaContext->InstanceNumber = pDnsServer[ulIndex].InstanceNumber;
                
                if ( pMyObject->ulNextServerInsNum <=  pDnsServer[ulIndex].InstanceNumber )
                {
                    pMyObject->ulNextServerInsNum =  pDnsServer[ulIndex].InstanceNumber + 1;

                    if ( 0 == pMyObject->ulNextServerInsNum )
                    {
                        pMyObject->ulNextServerInsNum = 1;
                    }
                }
            }
            else
            {
                pCosaContext->InstanceNumber = pMyObject->ulNextServerInsNum;  
                
                pDnsServer[ulIndex].InstanceNumber = pCosaContext->InstanceNumber;
                
                pMyObject->ulNextServerInsNum++;
                
                if ( 0 == pMyObject->ulNextServerInsNum )
                {
                    pMyObject->ulNextServerInsNum = 1;
                }

                /* Generate Alias */
                _ansc_sprintf(pDnsServer[ulIndex].Alias, "Server%d", pMyObject->ulNextServerInsNum);

                CosaDmlDnsClientSetServerValues
                (
                    NULL, 
                    ulIndex, 
                    pDnsServer[ulIndex].InstanceNumber,
                    pDnsServer[ulIndex].Alias
                );
            }

            pCosaContext->hContext      =  (ANSC_HANDLE)AnscAllocateMemory(
		    sizeof(pDnsServer[ulIndex]));
            memcpy(pCosaContext->hContext, (ANSC_HANDLE)&pDnsServer[ulIndex],
			sizeof(pDnsServer[ulIndex]));
            pCosaContext->hParentTable  = NULL;            
            pCosaContext->bNew          = FALSE;

            CosaSListPushEntryByInsNum(&pMyObject->ServerList, pCosaContext);
        }

        /* Update refresh timestamp */
        pMyObject->PreVisitSrvTime = AnscGetTickInSeconds();        
    }

    AnscFreeMemory(pDnsServer);

    /* Retrieve the next Instance Number for  Device.DNS.Relay.Forwarding.{i}. */
    if ( TRUE )
    {
        _ansc_sprintf
        (
            FolderName, 
            "%s%d", 
            COSA_DML_RR_NAME_FORWARD_NextInsNum,
            0
        );
        
        pPoamIrepFoNextIns = 
            (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoServer->GetFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoServer,
                    FolderName
                );

        if ( pPoamIrepFoNextIns )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoNextIns->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoNextIns,
                        COSA_DML_RR_NAME_FORWARD_NextInsNum,
                        NULL
                    );

            if ( pSlapVariable )
            {
                pMyObject->ulNextForwardInsNum = pSlapVariable->Variant.varUint32;
                
                SlapFreeVariable(pSlapVariable);
            }

            pPoamIrepFoNextIns->Remove((ANSC_HANDLE)pPoamIrepFoNextIns);
            pPoamIrepFoNextIns = NULL;
        } 
    }
    
     /* Initialize middle layer for Device.DNS.Relay.Forwarding.{i}. */
    pForward = CosaDmlDnsRelayGetServers(NULL, (PULONG)&ulEntryCount2);

    if ( !pForward )
    {
        returnStatus = ANSC_STATUS_RESOURCES;
        
        goto  EXIT;
    }
    else
    {
        for ( ulIndex2 = 0; ulIndex2 < ulEntryCount2; ulIndex2++ )
        {
            pCosaContext2 = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

            if ( !pCosaContext2 )
            {
                AnscFreeMemory(&pForward[ulIndex2]);

                returnStatus = ANSC_STATUS_RESOURCES;
                
                goto  EXIT;  
            }

            if ( 0 != pForward[ulIndex2].InstanceNumber )
            {
                pCosaContext2->InstanceNumber = pForward[ulIndex2].InstanceNumber;

                if (  pMyObject->ulNextForwardInsNum <= pForward[ulIndex2].InstanceNumber )
                {
                     pMyObject->ulNextForwardInsNum = pForward[ulIndex2].InstanceNumber + 1;

                    if (  pMyObject->ulNextForwardInsNum == 0 )
                    {
                         pMyObject->ulNextForwardInsNum = 1;
                    }
                }  
            }
            else
            {
                pCosaContext2->InstanceNumber = pForward[ulIndex2].InstanceNumber =  pMyObject->ulNextForwardInsNum;

                 pMyObject->ulNextForwardInsNum++;

                if (  pMyObject->ulNextForwardInsNum == 0 )
                {
                     pMyObject->ulNextForwardInsNum = 1;
                }

                /* Generate Alias */
                _ansc_sprintf(pForward[ulIndex2].Alias, "Forwarding%d", pCosaContext2->InstanceNumber);

                /* TODO: Set InstanceNumber Alias back */
                CosaDmlDnsRelaySetServerValues
                (
                    NULL, 
                    ulIndex2, 
                    pForward[ulIndex2].InstanceNumber,
                    pForward[ulIndex2].Alias
                );
            }                

            pCosaContext2->hContext     = (ANSC_HANDLE)&pForward[ulIndex2];
            pCosaContext2->hParentTable = NULL;
            pCosaContext2->bNew         = FALSE;

            CosaSListPushEntryByInsNum(&pMyObject->ForwardList, pCosaContext2);
        }
        
        /* Update refresh timestamp */
        pMyObject->PreVisitForwardTime = AnscGetTickInSeconds();        
    }
       

    CosaDNSRegGetInfo((ANSC_HANDLE)pMyObject);

    EXIT:
        
    return returnStatus;
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDNSRemove
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
CosaDNSRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DNS             pMyObject       = (PCOSA_DATAMODEL_DNS)hThisObject;
    PSLIST_HEADER                   pSrvHead       = (PSLIST_HEADER)&pMyObject->ServerList;
    PSLIST_HEADER                   pForwardHead    = (PSLIST_HEADER)&pMyObject->ForwardList;
    PCOSA_DML_DNS_CLIENT_SERVER     pDnsServer      = (PCOSA_DML_DNS_CLIENT_SERVER)NULL;
    PCOSA_DML_DNS_RELAY_ENTRY       pForward        = (PCOSA_DML_DNS_RELAY_ENTRY)NULL;
    PSINGLE_LINK_ENTRY              pLink           = (PSINGLE_LINK_ENTRY )NULL;
    PSINGLE_LINK_ENTRY              pLink2          = (PSINGLE_LINK_ENTRY )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2   = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoTemp = (PPOAM_IREP_FOLDER_OBJECT)NULL;

    /* DNS server List */
    pLink = AnscSListGetFirstEntry(pSrvHead);

    while ( pLink )
    {
        pCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        pLink        = AnscSListGetNextEntry(pLink);

        pDnsServer = (PCOSA_DML_DNS_CLIENT_SERVER)pCosaContext->hContext;

        if ( pDnsServer )
        {
            AnscSListPopEntryByLink(pSrvHead, &pCosaContext->Linkage);

            AnscFreeMemory(pDnsServer);
            AnscFreeMemory(pCosaContext);
        }
    }

    /* Relay forwarding list*/
    pLink2 = AnscSListGetFirstEntry(pForwardHead);

    while ( pLink2 )
    {
        pCosaContext2 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink2);
        pLink2         = AnscSListGetNextEntry(pLink2);

        pForward = (PCOSA_DML_DNS_RELAY_ENTRY)pCosaContext2->hContext;

        if ( pForward )
        {
            AnscSListPopEntryByLink(pForwardHead, &pCosaContext2->Linkage);

            AnscFreeMemory(pForward);
            AnscFreeMemory(pCosaContext2);
        }
    }
 
    /* Remove self */
    pPoamIrepFoTemp = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderServerHA;
    if ( pPoamIrepFoTemp )
    {
        pPoamIrepFoTemp->Remove(pPoamIrepFoTemp);
    }

        
    pPoamIrepFoTemp = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderServer;
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
        CosaDNSRegGetInfo
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
CosaDNSRegGetInfo

    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DNS             pMyObject      = (PCOSA_DATAMODEL_DNS)hThisObject;
    PCOSA_DML_DNS_CLIENT_SERVER     pDnsServer     = (PCOSA_DML_DNS_CLIENT_SERVER)NULL;
    PCOSA_DML_DNS_RELAY_ENTRY       pForward       = (PCOSA_DML_DNS_RELAY_ENTRY)NULL;
    PSINGLE_LINK_ENTRY              pLink          = (PSINGLE_LINK_ENTRY )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext   = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2  = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoServerHA = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderServerHA;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoServer   = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PSLAP_VARIABLE                  pSlapVariable       = (PSLAP_VARIABLE          )NULL;
    ULONG                           ulEntryCount            = 0;
    ULONG                           ulIndex                 = 0;
    ULONG                           ulInstanceNumber        = 0;
    ULONG                           ulUpperInstanceNumber   = 0;
    char*                           pFolderName             = NULL;
    char*                           pAlias                  = NULL;

    if ( !pPoamIrepFoServerHA )
    {
        return ANSC_STATUS_FAILURE;
    }

    /* Load the newly added but not yet commited entries */

    ulEntryCount = pPoamIrepFoServerHA->GetFolderCount((ANSC_HANDLE)pPoamIrepFoServerHA);

    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        pFolderName = pPoamIrepFoServerHA->EnumFolder
                        (
                            (ANSC_HANDLE)pPoamIrepFoServerHA,
                            ulIndex
                        );

        if ( !pFolderName )
        {
            continue;
        }

        pPoamIrepFoServer = (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoServerHA->GetFolder
                            (
                                (ANSC_HANDLE)pPoamIrepFoServerHA, 
                                pFolderName
                            );

        if ( !pPoamIrepFoServer )
        {
            continue;
        }

        if ( _ansc_strstr(pFolderName, "DNSServer") )
        {
            if ( TRUE )
            {
                pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoServer->GetRecord
                                    (
                                        (ANSC_HANDLE)pPoamIrepFoServer,
                                        COSA_DML_RR_NAME_SERVER_InsNum,
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
                pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoServer->GetRecord
                                (
                                    (ANSC_HANDLE)pPoamIrepFoServer,
                                     COSA_DML_RR_NAME_SERVER_Alias,
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

            pDnsServer = (PCOSA_DML_DNS_CLIENT_SERVER)AnscAllocateMemory(sizeof(COSA_DML_DNS_CLIENT_SERVER));

            if ( !pDnsServer )
            {
                AnscFreeMemory(pCosaContext);
                
                returnStatus = ANSC_STATUS_RESOURCES;
                
                goto EXIT;
            }
           
            pDnsServer->InstanceNumber = ulInstanceNumber;
            AnscCopyString(pDnsServer->Alias, pAlias);

            pCosaContext->bNew             = TRUE;
            pCosaContext->hParentTable     = NULL;
            pCosaContext->InstanceNumber   = ulInstanceNumber;
            pCosaContext->hContext         = (ANSC_HANDLE)pDnsServer;
            pCosaContext->hPoamIrepUpperFo = (ANSC_HANDLE)pPoamIrepFoServerHA;
            pCosaContext->hPoamIrepFo      = (ANSC_HANDLE)pPoamIrepFoServer;
            
            CosaSListPushEntryByInsNum(&pMyObject->ServerList, pCosaContext);
        }
        else
        {
            if ( TRUE )
            {
                pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoServer->GetRecord
                                    (
                                        (ANSC_HANDLE)pPoamIrepFoServer,
                                        COSA_DML_RR_NAME_SERVER_InsNum,
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
                pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoServer->GetRecord
                                    (
                                        (ANSC_HANDLE)pPoamIrepFoServer,
                                        COSA_DML_RR_NAME_SERVER_Alias,
                                        NULL
                                    );
            
                if ( pSlapVariable )
                {
                    pAlias = AnscCloneString(pSlapVariable->Variant.varString);            

                    SlapFreeVariable(pSlapVariable);
                }
            }
            
            if ( _ansc_strstr(pFolderName, "Forwarding") )
            {
                pForward = (PCOSA_DML_DNS_RELAY_ENTRY)AnscAllocateMemory(sizeof(COSA_DML_DNS_RELAY_ENTRY));

                if ( !pForward )
                {
                    returnStatus = ANSC_STATUS_RESOURCES;

                    goto EXIT;
                }

                pCosaContext2 = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

                if ( !pCosaContext2 )
                {
                    AnscFreeMemory(pForward);            

                    returnStatus = ANSC_STATUS_RESOURCES;

                    goto EXIT;
                }

                pForward->InstanceNumber = ulInstanceNumber;
                AnscCopyString(pForward->Alias, pAlias);

                pCosaContext2->bNew             = TRUE;
                pCosaContext2->hParentTable     = NULL;
                pCosaContext2->InstanceNumber   = ulInstanceNumber;
                pCosaContext2->hContext         = (ANSC_HANDLE)pForward;
                pCosaContext2->hPoamIrepUpperFo = (ANSC_HANDLE)pPoamIrepFoServerHA;
                pCosaContext2->hPoamIrepFo      = (ANSC_HANDLE)pPoamIrepFoServer;

                CosaSListPushEntryByInsNum(&pMyObject->ForwardList, pCosaContext2);
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
        CosaDNSRegAddInfo
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
CosaDNSRegAddInfo
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
    PCOSA_DATAMODEL_DNS             pMyObject         = (PCOSA_DATAMODEL_DNS )hThisObject;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hCosaContext;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoServer = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderServer;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoUpper  = (PPOAM_IREP_FOLDER_OBJECT )pCosaContext->hPoamIrepUpperFo;  
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFo       = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PSLAP_VARIABLE                  pSlapVariable     = (PSLAP_VARIABLE           )NULL;
    char                            FolderName[32]    = {0};

    if ( !pPoamIrepFoServer || !pPoamIrepFoUpper )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pPoamIrepFoServer->EnableFileSync((ANSC_HANDLE)pPoamIrepFoServer, FALSE);
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
        pPoamIrepFo = (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoServer->GetFolder
                        (
                            (ANSC_HANDLE)pPoamIrepFoServer,
                            FolderName
                        );

        if ( !pPoamIrepFo )
        {
            pPoamIrepFo = pPoamIrepFoServer->AddFolder
                            (
                                (ANSC_HANDLE)pPoamIrepFoServer,
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
        _ansc_sprintf(FolderName, "%s%d%d", pPreffix, ulUpperInsNum, pCosaContext->InstanceNumber);

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
                                COSA_DML_RR_NAME_SERVER_Upper_InsNum,
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
                                COSA_DML_RR_NAME_SERVER_InsNum,
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
                                COSA_DML_RR_NAME_SERVER_Alias,
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

    pPoamIrepFoServer->EnableFileSync((ANSC_HANDLE)pPoamIrepFoServer, TRUE);

    return returnStatus;
}



/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        CosaDNSRegDelInfo
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
CosaDNSRegDelInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DNS             pMyObject         = (PCOSA_DATAMODEL_DNS )hThisObject;
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




