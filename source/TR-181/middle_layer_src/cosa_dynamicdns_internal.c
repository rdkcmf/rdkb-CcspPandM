#include "cosa_apis.h"
#include "cosa_dynamicdns_apis.h"
#include "cosa_dynamicdns_internal.h"
#include "plugin_main_apis.h"
#include "poam_irepfo_interface.h"
#include "slap_vho_exported_api.h"
#include "sys_definitions.h"

extern void * g_pDslhDmlAgent;

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        CosaDynamicDnsCreate
            (
            );

    description:

        This function constructs cosa dynamicdns object and return handle.

    argument:

    return:     newly created dynamicdns object.

**********************************************************************/
ANSC_HANDLE
CosaDynamicDnsCreate
    (
        VOID
    )
{
    PCOSA_DATAMODEL_TR181_DDNS            pMyObject    = (PCOSA_DATAMODEL_TR181_DDNS)NULL;

    pMyObject = (PCOSA_DATAMODEL_TR181_DDNS)AnscAllocateMemory(sizeof(COSA_DATAMODEL_TR181_DDNS));

    if (!pMyObject)
    {
        return  (ANSC_HANDLE)NULL;
    }

    pMyObject->Create            = CosaDynamicDnsCreate;
    pMyObject->Remove            = CosaDynamicDnsRemove;
    pMyObject->Initialize        = CosaDynamicDnsInitialize;

    pMyObject->Initialize   ((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}

ANSC_STATUS
CosaDynamicDnsInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                returnStatus                 = ANSC_STATUS_FAILURE;
    PCOSA_DATAMODEL_TR181_DDNS       pMyObject                    = (PCOSA_DATAMODEL_TR181_DDNS)hThisObject;
    PSLAP_VARIABLE             pSlapVariable                = (PSLAP_VARIABLE)NULL;
    PPOAM_IREP_FOLDER_OBJECT   pPoamIrepFoCOSA              = NULL;
    PPOAM_IREP_FOLDER_OBJECT   pPoamIrepFoDynamicDnsClient  = NULL;
    PCOSA_CONTEXT_LINK_OBJECT  pDynamicDnsClientLinkObj     = NULL;
    COSA_DML_DDNS_CLIENT       *pDynamicDnsClient           = NULL;
    ULONG                      ulDynamicDnsClientCnt        = 0;
    ULONG                      ulDynamicDnsClientIdx        = 0;

    /* Device.DynamicDNS.Client.HostName.{i}. */
    ULONG                           ulDynamicDnsHostCnt = 0;
    ULONG                           ulDynamicDnsHostIdx = 0;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoDynamicDnsHost = NULL;
    COSA_DML_DDNS_HOST              *pDynamicDnsHost;
    PCOSA_CONTEXT_LINK_OBJECT       pDynamicDnsHostLinkObj = NULL;

    /* Device.DynamicDNS.Server.{i}. */
    ULONG                           ulDynamicDnsServerCnt = 0;
    ULONG                           ulDynamicDnsServerIdx = 0;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoDynamicDnsServer = NULL;
    COSA_DML_DDNS_SERVER            *pDynamicDnsServer;
    PCOSA_CONTEXT_LINK_OBJECT       pDynamicDnsServerLinkObj = NULL;

    AnscSListInitializeHeader(&pMyObject->DDNSClientList);
    pMyObject->DDNSClientNextInsNum = 1;
    //pMyObject->hIrepFolderDDNSClient = g_GetRegistryRootFolder(g_pDslhDmlAgent);
    pPoamIrepFoCOSA = (PPOAM_IREP_FOLDER_OBJECT)g_GetRegistryRootFolder(g_pDslhDmlAgent);
    ulDynamicDnsClientCnt = CosaDmlDynamicDns_Client_GetNumberOfEntries();

    if (!pPoamIrepFoCOSA)
    {
        return returnStatus;
    }

    pPoamIrepFoDynamicDnsClient = (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoCOSA->GetFolder(
            (ANSC_HANDLE)pPoamIrepFoCOSA,
            COSA_IREP_FOLDER_NAME_DDNS_CLIENT);

    if (!pPoamIrepFoDynamicDnsClient)
    {
        pPoamIrepFoDynamicDnsClient =
            pPoamIrepFoCOSA->AddFolder(
                    (ANSC_HANDLE)pPoamIrepFoCOSA,
                     COSA_IREP_FOLDER_NAME_DDNS_CLIENT,
                    0);
    }

    if (!pPoamIrepFoDynamicDnsClient)
    {
        return returnStatus;
    }
    else
    {
        pMyObject->hIrepFolderDDNSClient = pPoamIrepFoDynamicDnsClient;
    }

    pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoDynamicDnsClient->GetRecord(
             (ANSC_HANDLE)pPoamIrepFoDynamicDnsClient,
             COSA_DML_RR_NAME_DDNS_CLIENT_NextInsNumber,
             NULL);

    if (pSlapVariable)
    {
        pMyObject->DDNSClientNextInsNum = pSlapVariable->Variant.varUint32;
        SlapFreeVariable(pSlapVariable);
    }

    for (ulDynamicDnsClientIdx = 0; ulDynamicDnsClientIdx < ulDynamicDnsClientCnt; ulDynamicDnsClientIdx++)
    {
         pDynamicDnsClient  = AnscAllocateMemory(sizeof(COSA_DML_DDNS_CLIENT));
         if (!pDynamicDnsClient)
         {
             return returnStatus;
         }

         returnStatus = CosaDmlDynamicDns_Client_GetEntryByIndex(ulDynamicDnsClientIdx, pDynamicDnsClient);
        if (returnStatus != ANSC_STATUS_SUCCESS)
         {
             CcspTraceError(("%s: CosaDmlDynamicDns_Client__GetEntryByIndex error\n", __FUNCTION__));
             AnscFreeMemory(pDynamicDnsClient);
             return returnStatus;
         }

         pDynamicDnsClientLinkObj = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
         if (!pDynamicDnsClientLinkObj)
         {
             AnscFreeMemory(pDynamicDnsClient);
             return ANSC_STATUS_FAILURE;
         }

         if (pDynamicDnsClient->InstanceNumber != 0)
         {
             if (pMyObject->DDNSClientNextInsNum <= pDynamicDnsClient->InstanceNumber)
             {
                 pMyObject->DDNSClientNextInsNum = pDynamicDnsClient->InstanceNumber + 1;
                 if (pMyObject->DDNSClientNextInsNum == 0)
                 {
                     pMyObject->DDNSClientNextInsNum = 1;
                 }
             }
         }
         else
         {
             pDynamicDnsClient->InstanceNumber = pMyObject->DDNSClientNextInsNum;
             pMyObject->DDNSClientNextInsNum++;
             if (pMyObject->DDNSClientNextInsNum == 0)
             {
                 pMyObject->DDNSClientNextInsNum = 1;
             }
             _ansc_sprintf(pDynamicDnsClient->Alias, "cpe-ddns-client-%d", (int)pDynamicDnsClient->InstanceNumber);
             CosaDmlDynamicDns_Client_SetValues(ulDynamicDnsClientIdx, pDynamicDnsClient->InstanceNumber, pDynamicDnsClient->Alias);
         }

         pDynamicDnsClientLinkObj->InstanceNumber = pDynamicDnsClient->InstanceNumber;
         pDynamicDnsClientLinkObj->hContext       = pDynamicDnsClient;
         pDynamicDnsClientLinkObj->hParentTable   = NULL;
         pDynamicDnsClientLinkObj->bNew           = FALSE;

         CosaSListPushEntryByInsNum(&pMyObject->DDNSClientList, pDynamicDnsClientLinkObj);
    }
    CosaDynamicDns_ClientGetInfo((ANSC_HANDLE)pMyObject);

    /* Device.DynamicDNS.Client.HostName.{i}. */
    pSlapVariable    = NULL;
    pPoamIrepFoCOSA  = NULL;
    AnscSListInitializeHeader(&pMyObject->DDNSHostList);
    CosaInitializeTr181DdnsHost();
    pMyObject->DDNSHostNextInsNum = 1;
    pPoamIrepFoCOSA = (PPOAM_IREP_FOLDER_OBJECT)g_GetRegistryRootFolder(g_pDslhDmlAgent);
    ulDynamicDnsHostCnt = CosaDmlDynamicDns_Host_GetNumberOfEntries();

    if (!pPoamIrepFoCOSA)
    {
        return returnStatus;
    }

    pPoamIrepFoDynamicDnsHost = (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoCOSA->GetFolder(
            (ANSC_HANDLE)pPoamIrepFoCOSA,
            COSA_IREP_FOLDER_NAME_DDNS_HOST);

    if (!pPoamIrepFoDynamicDnsHost)
    {
        pPoamIrepFoDynamicDnsHost =
            pPoamIrepFoCOSA->AddFolder(
                    (ANSC_HANDLE)pPoamIrepFoCOSA,
                     COSA_IREP_FOLDER_NAME_DDNS_HOST,
                   0);
    }

    if (!pPoamIrepFoDynamicDnsHost)
   {
        return returnStatus;
    }
    else
    {
        pMyObject->hIrepFolderDDNSHost = pPoamIrepFoDynamicDnsHost;
    }

    pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoDynamicDnsHost->GetRecord(
             (ANSC_HANDLE)pPoamIrepFoDynamicDnsHost,
             COSA_DML_RR_NAME_DDNS_HOST_NextInsNumber,
             NULL);

    if (pSlapVariable)
    {
        pMyObject->DDNSHostNextInsNum = pSlapVariable->Variant.varUint32;
        SlapFreeVariable(pSlapVariable);
    }

    for (ulDynamicDnsHostIdx = 0; ulDynamicDnsHostIdx < ulDynamicDnsHostCnt; ulDynamicDnsHostIdx++)
    {
         pDynamicDnsHost  = AnscAllocateMemory(sizeof(COSA_DML_DDNS_HOST));
        if (!pDynamicDnsHost)
         {
             return returnStatus;
         }

         returnStatus = CosaDmlDynamicDns_Host_GetEntryByIndex(ulDynamicDnsHostIdx, pDynamicDnsHost);
         if (returnStatus != ANSC_STATUS_SUCCESS)
         {
             CcspTraceError(("%s: CosaDmlDynamicDns_Host__GetEntryByIndex error\n", __FUNCTION__));
             AnscFreeMemory(pDynamicDnsHost);
             return returnStatus;
         }

         pDynamicDnsHostLinkObj = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
        if (!pDynamicDnsHostLinkObj)
         {
             AnscFreeMemory(pDynamicDnsHost);
             return ANSC_STATUS_FAILURE;
         }

         if (pDynamicDnsHost->InstanceNumber != 0)
         {
             if (pMyObject->DDNSHostNextInsNum <= pDynamicDnsHost->InstanceNumber)
             {
                 pMyObject->DDNSHostNextInsNum = pDynamicDnsHost->InstanceNumber + 1;
                 if (pMyObject->DDNSHostNextInsNum == 0)
                 {
                     pMyObject->DDNSHostNextInsNum = 1;
                 }
             }
         }
         else
         {
             pDynamicDnsHost->InstanceNumber = pMyObject->DDNSHostNextInsNum;
             pMyObject->DDNSHostNextInsNum++;
             if (pMyObject->DDNSHostNextInsNum == 0)
             {
                 pMyObject->DDNSHostNextInsNum = 1;
             }
             _ansc_sprintf(pDynamicDnsHost->Alias, "cpe-ddns-host-%d", (int)pDynamicDnsHost->InstanceNumber);
             CosaDmlDynamicDns_Host_SetValues(ulDynamicDnsHostIdx, pDynamicDnsHost->InstanceNumber, pDynamicDnsHost->Alias);
         }

         pDynamicDnsHostLinkObj->InstanceNumber = pDynamicDnsHost->InstanceNumber;
         pDynamicDnsHostLinkObj->hContext       = pDynamicDnsHost;
         pDynamicDnsHostLinkObj->hParentTable   = NULL;
         pDynamicDnsHostLinkObj->bNew           = FALSE;

         CosaSListPushEntryByInsNum(&pMyObject->DDNSHostList, pDynamicDnsHostLinkObj);
    }
    CosaDynamicDns_HostGetInfo((ANSC_HANDLE)pMyObject);

    /* Device.DynamicDNS.Server.{i}. */
    pSlapVariable    = NULL;
    pPoamIrepFoCOSA  = NULL;
    AnscSListInitializeHeader(&pMyObject->DDNSServerList);
    pMyObject->DDNSServerNextInsNum = 1;
    pPoamIrepFoCOSA = (PPOAM_IREP_FOLDER_OBJECT)g_GetRegistryRootFolder(g_pDslhDmlAgent);

    CosaInitializeTr181DdnsServiceProviderList();

    ulDynamicDnsServerCnt = CosaDmlDynamicDns_Server_GetNumberOfEntries();

    if (!pPoamIrepFoCOSA)
    {
        return returnStatus;
    }

    pPoamIrepFoDynamicDnsServer = (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoCOSA->GetFolder(
            (ANSC_HANDLE)pPoamIrepFoCOSA,
            COSA_IREP_FOLDER_NAME_DDNS_SERVER);

    if (!pPoamIrepFoDynamicDnsServer)
    {
        pPoamIrepFoDynamicDnsServer =
            pPoamIrepFoCOSA->AddFolder(
                    (ANSC_HANDLE)pPoamIrepFoCOSA,
                     COSA_IREP_FOLDER_NAME_DDNS_SERVER,
                    0);
    }

    if (!pPoamIrepFoDynamicDnsServer)
    {
        return returnStatus;
    }
    else
    {
        pMyObject->hIrepFolderDDNSServer = pPoamIrepFoDynamicDnsServer;
    }

    pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoDynamicDnsServer->GetRecord(
             (ANSC_HANDLE)pPoamIrepFoDynamicDnsServer,
             COSA_DML_RR_NAME_DDNS_SERVER_NextInsNumber,
             NULL);

    if (pSlapVariable)
    {
        pMyObject->DDNSServerNextInsNum = pSlapVariable->Variant.varUint32;
        SlapFreeVariable(pSlapVariable);
    }

    for (ulDynamicDnsServerIdx = 0; ulDynamicDnsServerIdx < ulDynamicDnsServerCnt; ulDynamicDnsServerIdx++)
    {
         pDynamicDnsServer  = AnscAllocateMemory(sizeof(COSA_DML_DDNS_SERVER));
         if (!pDynamicDnsServer)
         {
             return returnStatus;
         }

         returnStatus = CosaDmlDynamicDns_Server_GetEntryByIndex(ulDynamicDnsServerIdx, pDynamicDnsServer);

         if (returnStatus != ANSC_STATUS_SUCCESS)
         {
             CcspTraceError(("%s: CosaDmlDynamicDns_Server_GetEntryByIndex error\n", __FUNCTION__));
             AnscFreeMemory(pDynamicDnsServer);
             return returnStatus;
         }

         pDynamicDnsServerLinkObj = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
         if (!pDynamicDnsServerLinkObj)
         {
             AnscFreeMemory(pDynamicDnsServer);
             return ANSC_STATUS_FAILURE;
         }

         if (pDynamicDnsServer->InstanceNumber != 0)
         {
             if (pMyObject->DDNSServerNextInsNum <= pDynamicDnsServer->InstanceNumber)
             {
                 pMyObject->DDNSServerNextInsNum = pDynamicDnsServer->InstanceNumber + 1;
                 if (pMyObject->DDNSServerNextInsNum == 0)
                 {
                     pMyObject->DDNSServerNextInsNum = 1;
                 }
             }
         }
         else
         {
             pDynamicDnsServer->InstanceNumber = pMyObject->DDNSServerNextInsNum;
             pMyObject->DDNSServerNextInsNum++;
             if (pMyObject->DDNSServerNextInsNum == 0)
             {
                 pMyObject->DDNSServerNextInsNum = 1;
             }
             _ansc_sprintf(pDynamicDnsServer->Alias, "cpe-ddns-server-%d", (int)pDynamicDnsServer->InstanceNumber);
             CosaDmlDynamicDns_Server_SetValues(ulDynamicDnsServerIdx, pDynamicDnsServer->InstanceNumber, pDynamicDnsServer->Alias);
         }

         pDynamicDnsServerLinkObj->InstanceNumber = pDynamicDnsServer->InstanceNumber;
         pDynamicDnsServerLinkObj->hContext       = pDynamicDnsServer;
         pDynamicDnsServerLinkObj->hParentTable   = NULL;
         pDynamicDnsServerLinkObj->bNew           = FALSE;

         CosaSListPushEntryByInsNum(&pMyObject->DDNSServerList, pDynamicDnsServerLinkObj);
    }
    CosaDynamicDns_ServerGetInfo((ANSC_HANDLE)pMyObject);

    return  returnStatus;
}

ANSC_STATUS
CosaDynamicDnsRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PCOSA_DATAMODEL_TR181_DDNS        pMyObject    = (PCOSA_DATAMODEL_TR181_DDNS)hThisObject;
    PSINGLE_LINK_ENTRY              pLink        = NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pDdnsEntry   = NULL;

    if (pMyObject)
    {
        /*Device.DynamicDNS.Client.{i} */
        PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFo  = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderDDNSClient;

        /*Device.DynamicDNS.Client.Host.{i}. */
        PPOAM_IREP_FOLDER_OBJECT        pPoamIrepHostFo  = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderDDNSHost;

        /*Device.DynamicDNS.Server. */
        PPOAM_IREP_FOLDER_OBJECT        pPoamIrepServerFo  = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderDDNSServer;

        for( pLink = AnscSListPopEntry(&pMyObject->DDNSClientList); pLink; )
        {
            pDdnsEntry = (PCOSA_CONTEXT_LINK_OBJECT)ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
            pLink = AnscSListGetNextEntry(pLink);

            AnscFreeMemory(pDdnsEntry->hContext);
            AnscFreeMemory(pDdnsEntry);
        }

        pDdnsEntry = NULL;
        for( pLink = AnscSListPopEntry(&pMyObject->DDNSHostList); pLink; )
        {
            pDdnsEntry = (PCOSA_CONTEXT_LINK_OBJECT)ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
            pLink = AnscSListGetNextEntry(pLink);

            AnscFreeMemory(pDdnsEntry->hContext);
            AnscFreeMemory(pDdnsEntry);
        }

        pDdnsEntry = NULL;
        for (pLink = AnscSListPopEntry(&pMyObject->DDNSServerList); pLink; )
        {
            pDdnsEntry = (PCOSA_CONTEXT_LINK_OBJECT)ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
            pLink = AnscSListGetNextEntry(pLink);

            AnscFreeMemory(pDdnsEntry->hContext);
            AnscFreeMemory(pDdnsEntry);
        }

        if (pPoamIrepFo)
        {
            pPoamIrepFo->Remove( (ANSC_HANDLE)pPoamIrepFo);
        }
        if (pPoamIrepHostFo)
        {
            pPoamIrepHostFo->Remove( (ANSC_HANDLE)pPoamIrepHostFo);
        }
        if (pPoamIrepServerFo)
        {
            pPoamIrepServerFo->Remove( (ANSC_HANDLE)pPoamIrepServerFo);
        }

        AnscFreeMemory((ANSC_HANDLE)pMyObject);
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDynamicDns_ClientGetInfo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PCOSA_DATAMODEL_TR181_DDNS        pMyObject                  = (PCOSA_DATAMODEL_TR181_DDNS)hThisObject;
    PSLIST_HEADER               pListHead                  = (PSLIST_HEADER)&pMyObject->DDNSClientList;
    PPOAM_IREP_FOLDER_OBJECT    pPoamIrepFoDDNSClient      = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderDDNSClient;
    PPOAM_IREP_FOLDER_OBJECT    pPoamIrepFoDDNSClientSlap  = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    COSA_DML_DDNS_CLIENT        *pClientEntry              = (COSA_DML_DDNS_CLIENT *)NULL;
    PCOSA_CONTEXT_LINK_OBJECT   pCosaContext               = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSLAP_VARIABLE              pSlapVariable              = (PSLAP_VARIABLE)NULL;
    ULONG                       ulEntryCount               = 0;
    ULONG                       ulIndex                    = 0;
    ULONG                       ulInstanceNumber           = 0;
    char*                       pFolderName                = NULL;
    char*                       pAlias                     = NULL;



    if (!pPoamIrepFoDDNSClient)
    {
        return ANSC_STATUS_FAILURE;
    }
    /* Load the newly added but not yet commited entries */

    ulEntryCount = pPoamIrepFoDDNSClient->GetFolderCount((ANSC_HANDLE)pPoamIrepFoDDNSClient);

    for (ulIndex = 0; ulIndex < ulEntryCount; ulIndex++)
    {
         pFolderName = pPoamIrepFoDDNSClient->EnumFolder
                           (
                               (ANSC_HANDLE)pPoamIrepFoDDNSClient,
                               ulIndex
                           );

         if (!pFolderName)
         {
             continue;
         }

         pPoamIrepFoDDNSClientSlap = pPoamIrepFoDDNSClient->GetFolder
                           (
                               (ANSC_HANDLE)pPoamIrepFoDDNSClient,
                               pFolderName
                           );
         AnscFreeMemory(pFolderName);
         pFolderName = NULL;

         if (!pPoamIrepFoDDNSClientSlap)
         {
             continue;
         }

         /* Get Instance number for the current folder */
         pSlapVariable = pPoamIrepFoDDNSClientSlap->GetRecord
                           (
                               (ANSC_HANDLE)pPoamIrepFoDDNSClientSlap,
                               COSA_DML_RR_NAME_DDNS_CLIENT_InsNum,
                               NULL
                           );

        if (pSlapVariable)
        {
            ulInstanceNumber = pSlapVariable->Variant.varUint32;
            SlapFreeVariable(pSlapVariable);
        }
        else
        {
            continue;
        }

        /* Get Alias name for the current folder */
         pSlapVariable = pPoamIrepFoDDNSClientSlap->GetRecord
                           (
                               (ANSC_HANDLE)pPoamIrepFoDDNSClientSlap,
                               COSA_DML_RR_NAME_DDNS_CLIENT_Alias,
                               NULL
                           );

        if (pSlapVariable)
        {
            pAlias = AnscCloneString(pSlapVariable->Variant.varString);
            SlapFreeVariable(pSlapVariable);
        }

        /* Also, Handles the case where pSlapVariable = NULL eventually pAlias will also be NULL */
        if (!pAlias)
        {
            continue;
        }

        pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
        if (!pCosaContext )
        {
            AnscFreeMemory(pAlias);
            return ANSC_STATUS_RESOURCES;
        }

        pClientEntry = (COSA_DML_DDNS_CLIENT *)AnscAllocateMemory(sizeof(COSA_DML_DDNS_CLIENT));
        if (!pClientEntry)
        {
            AnscFreeMemory(pAlias);
            AnscFreeMemory(pCosaContext);
            return ANSC_STATUS_RESOURCES;
        }

        pClientEntry->InstanceNumber = ulInstanceNumber;
        AnscCopyString(pClientEntry->Alias, pAlias);

        /*Copy the current entry into COSA_CONTEXT_LINK_OBJECT */
        pCosaContext->InstanceNumber        = ulInstanceNumber;
        pCosaContext->bNew                  = TRUE;
        pCosaContext->hContext              = (ANSC_HANDLE)pClientEntry;
        pCosaContext->hParentTable          = NULL;
        pCosaContext->hPoamIrepUpperFo      = (ANSC_HANDLE)pPoamIrepFoDDNSClient;
        pCosaContext->hPoamIrepFo           = (ANSC_HANDLE)pPoamIrepFoDDNSClientSlap;

        CosaSListPushEntryByInsNum(pListHead, pCosaContext);
        AnscFreeMemory(pAlias);
        pAlias = NULL;
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDynamicDns_ClientAddInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    )
{
    PCOSA_DATAMODEL_TR181_DDNS        pMyObject                  = (PCOSA_DATAMODEL_TR181_DDNS)hThisObject;
    PPOAM_IREP_FOLDER_OBJECT    pPoamIrepFoDDNSClient      = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderDDNSClient;
    PPOAM_IREP_FOLDER_OBJECT    pPoamIrepFoDDNSClientSlap  = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PCOSA_CONTEXT_LINK_OBJECT   pCosaContext               = (PCOSA_CONTEXT_LINK_OBJECT)hCosaContext;
    COSA_DML_DDNS_CLIENT        *pClientEntry              = (COSA_DML_DDNS_CLIENT *)pCosaContext->hContext;
    PSLAP_VARIABLE              pSlapVariable              = (PSLAP_VARIABLE)NULL;

    if (!pPoamIrepFoDDNSClient)
    {
        return ANSC_STATUS_FAILURE;
    }

    pPoamIrepFoDDNSClient->EnableFileSync((ANSC_HANDLE)pPoamIrepFoDDNSClient, FALSE);

    /* Allocate SLAP variable */
    SlapAllocVariable(pSlapVariable);

    if (!pSlapVariable)
    {
        pPoamIrepFoDDNSClient->EnableFileSync((ANSC_HANDLE)pPoamIrepFoDDNSClient, TRUE);
        return ANSC_STATUS_RESOURCES;
    }

    /* Delete the Next Instance number Record and Add it with the slap variable */
    pPoamIrepFoDDNSClient->DelRecord
        (
            (ANSC_HANDLE)pPoamIrepFoDDNSClient,
            COSA_DML_RR_NAME_DDNS_CLIENT_NextInsNumber
        );

    pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
    pSlapVariable->Variant.varUint32 = pMyObject->DDNSClientNextInsNum;

    pPoamIrepFoDDNSClient->AddRecord
        (
            (ANSC_HANDLE)pPoamIrepFoDDNSClient,
            COSA_DML_RR_NAME_DDNS_CLIENT_NextInsNumber,
            SYS_REP_RECORD_TYPE_UINT,
            SYS_RECORD_CONTENT_DEFAULT,
            pSlapVariable,
            0
        );

    SlapCleanVariable(pSlapVariable);
    SlapInitVariable (pSlapVariable);

    /* Create sub folder with Alias name */
    pPoamIrepFoDDNSClientSlap =
        pPoamIrepFoDDNSClient->AddFolder
            (
                (ANSC_HANDLE)pPoamIrepFoDDNSClient,
                pClientEntry->Alias,
                0
            );

    if (!pPoamIrepFoDDNSClientSlap)
    {
        SlapFreeVariable(pSlapVariable);
        pPoamIrepFoDDNSClient->EnableFileSync((ANSC_HANDLE)pPoamIrepFoDDNSClient, TRUE);
        return ANSC_STATUS_FAILURE;
    }

    /* Add the Instance number and Alias name to the sub folder with Slap variable */
    pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
    pSlapVariable->Variant.varUint32 = pClientEntry->InstanceNumber;

    pPoamIrepFoDDNSClientSlap->AddRecord
        (
            (ANSC_HANDLE)pPoamIrepFoDDNSClient,
            COSA_DML_RR_NAME_DDNS_CLIENT_InsNum,
            SYS_REP_RECORD_TYPE_UINT,
            SYS_RECORD_CONTENT_DEFAULT,
            pSlapVariable,
            0
        );

    SlapCleanVariable(pSlapVariable);
    SlapInitVariable (pSlapVariable);

    /* Add alias name */
    pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;
    pSlapVariable->Variant.varString = AnscCloneString(pClientEntry->Alias);

    pPoamIrepFoDDNSClientSlap->AddRecord
        (
            (ANSC_HANDLE)pPoamIrepFoDDNSClient,
            COSA_DML_RR_NAME_DDNS_CLIENT_Alias,
            SYS_REP_RECORD_TYPE_UINT,
            SYS_RECORD_CONTENT_DEFAULT,
            pSlapVariable,
            0
        );

    SlapCleanVariable(pSlapVariable);
    SlapInitVariable (pSlapVariable);

    /* Update the COSA_CONTEXT_LINK_OBJECT with the Folder created */
    pCosaContext->hPoamIrepUpperFo = (ANSC_HANDLE)pPoamIrepFoDDNSClient;
    pCosaContext->hPoamIrepFo      = (ANSC_HANDLE)pPoamIrepFoDDNSClientSlap;

    /* Enable the File sync and Free the allocated memory for Slap variable */
    SlapFreeVariable(pSlapVariable);
    pPoamIrepFoDDNSClient->EnableFileSync((ANSC_HANDLE)pPoamIrepFoDDNSClient, TRUE);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDynamicDns_ClientDelInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hCosaContext;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepUpperFo  = (PPOAM_IREP_FOLDER_OBJECT )pCosaContext->hPoamIrepUpperFo;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFo       = (PPOAM_IREP_FOLDER_OBJECT )pCosaContext->hPoamIrepFo;

    if ( !pPoamIrepUpperFo || !pPoamIrepFo )
    {
        return ANSC_STATUS_FAILURE;
    }

    pPoamIrepUpperFo->EnableFileSync((ANSC_HANDLE)pPoamIrepUpperFo, FALSE);

    /*Close and the Delete the sub folder */
    pPoamIrepFo->Close((ANSC_HANDLE)pPoamIrepFo);

    pPoamIrepUpperFo->DelFolder
        (
            (ANSC_HANDLE)pPoamIrepUpperFo,
            pPoamIrepFo->GetFolderName((ANSC_HANDLE)pPoamIrepFo)
        );

    pPoamIrepUpperFo->EnableFileSync((ANSC_HANDLE)pPoamIrepUpperFo, TRUE);
    AnscFreeMemory(pPoamIrepFo);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDynamicDns_HostGetInfo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PCOSA_DATAMODEL_TR181_DDNS        pMyObject              = (PCOSA_DATAMODEL_TR181_DDNS)hThisObject;
    PSLIST_HEADER               pListHead                  = (PSLIST_HEADER)&pMyObject->DDNSHostList;
    PPOAM_IREP_FOLDER_OBJECT    pPoamIrepFoDDNSHost        = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderDDNSHost;
    PPOAM_IREP_FOLDER_OBJECT    pPoamIrepFoDDNSHostSlap    = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    COSA_DML_DDNS_HOST          *pHostEntry                = (COSA_DML_DDNS_HOST *)NULL;
    PCOSA_CONTEXT_LINK_OBJECT   pCosaContext               = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSLAP_VARIABLE              pSlapVariable              = (PSLAP_VARIABLE)NULL;
    ULONG                       ulEntryCount               = 0;
    ULONG                       ulIndex                    = 0;
    ULONG                       ulInstanceNumber           = 0;
    char*                       pFolderName                = NULL;
    char*                       pAlias                     = NULL;

    if (!pPoamIrepFoDDNSHost)
    {
        return ANSC_STATUS_FAILURE;
    }
    /* Load the newly added but not yet commited entries */

    ulEntryCount = pPoamIrepFoDDNSHost->GetFolderCount((ANSC_HANDLE)pPoamIrepFoDDNSHost);

    for (ulIndex = 0; ulIndex < ulEntryCount; ulIndex++)
    {
         pFolderName = pPoamIrepFoDDNSHost->EnumFolder
                           (
                               (ANSC_HANDLE)pPoamIrepFoDDNSHost,
                               ulIndex
                           );

         if (!pFolderName)
         {
             continue;
         }

         pPoamIrepFoDDNSHostSlap = pPoamIrepFoDDNSHost->GetFolder
                           (
                               (ANSC_HANDLE)pPoamIrepFoDDNSHost,
                               pFolderName
                           );
         AnscFreeMemory(pFolderName);
         pFolderName = NULL;

         if (!pPoamIrepFoDDNSHostSlap)
         {
             continue;
         }

         /* Get Instance number for the current folder */
         pSlapVariable = pPoamIrepFoDDNSHostSlap->GetRecord
                           (
                               (ANSC_HANDLE)pPoamIrepFoDDNSHostSlap,
                               COSA_DML_RR_NAME_DDNS_HOST_InsNum,
                               NULL
                           );

        if (pSlapVariable)
        {
            ulInstanceNumber = pSlapVariable->Variant.varUint32;
            SlapFreeVariable(pSlapVariable);
        }
        else
       {
            continue;
        }

        /* Get Alias name for the current folder */
         pSlapVariable = pPoamIrepFoDDNSHostSlap->GetRecord
                           (
                               (ANSC_HANDLE)pPoamIrepFoDDNSHostSlap,
                               COSA_DML_RR_NAME_DDNS_HOST_Alias,
                               NULL
                           );

        if (pSlapVariable)
        {
            pAlias = AnscCloneString(pSlapVariable->Variant.varString);
            SlapFreeVariable(pSlapVariable);
        }

        /* Also, Handles the case where pSlapVariable = NULL eventually pAlias will also be NULL */
        if (!pAlias)
        {
            continue;
        }

        pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
        if (!pCosaContext )
        {
            AnscFreeMemory(pAlias);
            return ANSC_STATUS_RESOURCES;
        }

        pHostEntry = (COSA_DML_DDNS_HOST *)AnscAllocateMemory(sizeof(COSA_DML_DDNS_HOST));
        if (!pHostEntry)
        {
            AnscFreeMemory(pAlias);
            AnscFreeMemory(pCosaContext);
            return ANSC_STATUS_RESOURCES;
        }

        pHostEntry->InstanceNumber = ulInstanceNumber;
        AnscCopyString(pHostEntry->Alias, pAlias);

        /*Copy the current entry into COSA_CONTEXT_LINK_OBJECT */
        pCosaContext->InstanceNumber        = ulInstanceNumber;
        pCosaContext->bNew                  = TRUE;
        pCosaContext->hContext              = (ANSC_HANDLE)pHostEntry;
        pCosaContext->hParentTable          = NULL;
        pCosaContext->hPoamIrepUpperFo      = (ANSC_HANDLE)pPoamIrepFoDDNSHost;
        pCosaContext->hPoamIrepFo           = (ANSC_HANDLE)pPoamIrepFoDDNSHostSlap;

        CosaSListPushEntryByInsNum(pListHead, pCosaContext);
        AnscFreeMemory(pAlias);
        pAlias = NULL;
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDynamicDns_HostAddInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    )
{
    PCOSA_DATAMODEL_TR181_DDNS        pMyObject                  = (PCOSA_DATAMODEL_TR181_DDNS)hThisObject;
    PPOAM_IREP_FOLDER_OBJECT    pPoamIrepFoDDNSHost        = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderDDNSHost;
    PPOAM_IREP_FOLDER_OBJECT    pPoamIrepFoDDNSHostSlap    = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PCOSA_CONTEXT_LINK_OBJECT   pCosaContext               = (PCOSA_CONTEXT_LINK_OBJECT)hCosaContext;
    COSA_DML_DDNS_HOST          *pHostEntry                = (COSA_DML_DDNS_HOST *)pCosaContext->hContext;
    PSLAP_VARIABLE              pSlapVariable              = (PSLAP_VARIABLE)NULL;

    if (!pPoamIrepFoDDNSHost)
    {
        return ANSC_STATUS_FAILURE;
    }

    pPoamIrepFoDDNSHost->EnableFileSync((ANSC_HANDLE)pPoamIrepFoDDNSHost, FALSE);

    /* Allocate SLAP variable */
    SlapAllocVariable(pSlapVariable);

    if (!pSlapVariable)
    {
        pPoamIrepFoDDNSHost->EnableFileSync((ANSC_HANDLE)pPoamIrepFoDDNSHost, TRUE);
        return ANSC_STATUS_RESOURCES;
    }

    /* Delete the Next Instance number Record and Add it with the slap variable */
    pPoamIrepFoDDNSHost->DelRecord
        (
            (ANSC_HANDLE)pPoamIrepFoDDNSHost,
            COSA_DML_RR_NAME_DDNS_HOST_NextInsNumber
        );

    pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
    pSlapVariable->Variant.varUint32 = pMyObject->DDNSHostNextInsNum;

    pPoamIrepFoDDNSHost->AddRecord
        (
            (ANSC_HANDLE)pPoamIrepFoDDNSHost,
            COSA_DML_RR_NAME_DDNS_HOST_NextInsNumber,
            SYS_REP_RECORD_TYPE_UINT,
            SYS_RECORD_CONTENT_DEFAULT,
            pSlapVariable,
           0
        );

    SlapCleanVariable(pSlapVariable);
    SlapInitVariable (pSlapVariable);

    /* Create sub folder with Alias name */
    pPoamIrepFoDDNSHostSlap =
        pPoamIrepFoDDNSHost->AddFolder
            (
                (ANSC_HANDLE)pPoamIrepFoDDNSHost,
                pHostEntry->Alias,
                0
            );

    if (!pPoamIrepFoDDNSHostSlap)
    {
        SlapFreeVariable(pSlapVariable);
        pPoamIrepFoDDNSHost->EnableFileSync((ANSC_HANDLE)pPoamIrepFoDDNSHost, TRUE);
        return ANSC_STATUS_FAILURE;
    }

    /* Add the Instance number and Alias name to the sub folder with Slap variable */
    pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
    pSlapVariable->Variant.varUint32 = pHostEntry->InstanceNumber;

    pPoamIrepFoDDNSHostSlap->AddRecord
        (
            (ANSC_HANDLE)pPoamIrepFoDDNSHost,
            COSA_DML_RR_NAME_DDNS_HOST_InsNum,
            SYS_REP_RECORD_TYPE_UINT,
            SYS_RECORD_CONTENT_DEFAULT,
            pSlapVariable,
            0
        );

    SlapCleanVariable(pSlapVariable);
    SlapInitVariable (pSlapVariable);

    /* Add alias name */
    pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;
    pSlapVariable->Variant.varString = AnscCloneString(pHostEntry->Alias);

    pPoamIrepFoDDNSHostSlap->AddRecord
        (
            (ANSC_HANDLE)pPoamIrepFoDDNSHost,
            COSA_DML_RR_NAME_DDNS_HOST_Alias,
            SYS_REP_RECORD_TYPE_UINT,
            SYS_RECORD_CONTENT_DEFAULT,
            pSlapVariable,
            0
        );

    SlapCleanVariable(pSlapVariable);
    SlapInitVariable (pSlapVariable);

    /* Update the COSA_CONTEXT_LINK_OBJECT with the Folder created */
    pCosaContext->hPoamIrepUpperFo = (ANSC_HANDLE)pPoamIrepFoDDNSHost;
    pCosaContext->hPoamIrepFo      = (ANSC_HANDLE)pPoamIrepFoDDNSHostSlap;

    /* Enable the File sync and Free the allocated memory for Slap variable */
    SlapFreeVariable(pSlapVariable);
    pPoamIrepFoDDNSHost->EnableFileSync((ANSC_HANDLE)pPoamIrepFoDDNSHost, TRUE);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDynamicDns_HostDelInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hCosaContext;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepUpperFo  = (PPOAM_IREP_FOLDER_OBJECT )pCosaContext->hPoamIrepUpperFo;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFo       = (PPOAM_IREP_FOLDER_OBJECT )pCosaContext->hPoamIrepFo;

    if ( !pPoamIrepUpperFo || !pPoamIrepFo )
    {
        return ANSC_STATUS_FAILURE;
    }

    pPoamIrepUpperFo->EnableFileSync((ANSC_HANDLE)pPoamIrepUpperFo, FALSE);

    /*Close and the Delete the sub folder */
    pPoamIrepFo->Close((ANSC_HANDLE)pPoamIrepFo);

    pPoamIrepUpperFo->DelFolder
        (
            (ANSC_HANDLE)pPoamIrepUpperFo,
            pPoamIrepFo->GetFolderName((ANSC_HANDLE)pPoamIrepFo)
        );

    pPoamIrepUpperFo->EnableFileSync((ANSC_HANDLE)pPoamIrepUpperFo, TRUE);
    AnscFreeMemory(pPoamIrepFo);

    return ANSC_STATUS_SUCCESS;
}

/*Device.DynamicDNS.Server.{i}. */
ANSC_STATUS
CosaDynamicDns_ServerGetInfo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PCOSA_DATAMODEL_TR181_DDNS    pMyObject                  = (PCOSA_DATAMODEL_TR181_DDNS)hThisObject;
    PSLIST_HEADER               pListHead                  = (PSLIST_HEADER)&pMyObject->DDNSServerList;
    PPOAM_IREP_FOLDER_OBJECT    pPoamIrepFoDDNSServer      = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderDDNSServer;
    PPOAM_IREP_FOLDER_OBJECT    pPoamIrepFoDDNSServerSlap  = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    COSA_DML_DDNS_SERVER        *pServerEntry              = (COSA_DML_DDNS_SERVER *)NULL;
    PCOSA_CONTEXT_LINK_OBJECT   pCosaContext               = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSLAP_VARIABLE              pSlapVariable              = (PSLAP_VARIABLE)NULL;
    ULONG                       ulEntryCount               = 0;
    ULONG                       ulIndex                    = 0;
    ULONG                       ulInstanceNumber           = 0;
    char*                       pFolderName                = NULL;
    char*                       pAlias                     = NULL;

    if (!pPoamIrepFoDDNSServer)
    {
        return ANSC_STATUS_FAILURE;
    }
    /* Load the newly added but not yet commited entries */

    ulEntryCount = pPoamIrepFoDDNSServer->GetFolderCount((ANSC_HANDLE)pPoamIrepFoDDNSServer);

    for (ulIndex = 0; ulIndex < ulEntryCount; ulIndex++)
    {
         pFolderName = pPoamIrepFoDDNSServer->EnumFolder
                           (
                               (ANSC_HANDLE)pPoamIrepFoDDNSServer,
                               ulIndex
                           );

         if (!pFolderName)
         {
             continue;
         }

         pPoamIrepFoDDNSServerSlap = pPoamIrepFoDDNSServer->GetFolder
                           (
                               (ANSC_HANDLE)pPoamIrepFoDDNSServer,
                               pFolderName
                           );
         AnscFreeMemory(pFolderName);
         pFolderName = NULL;

         if (!pPoamIrepFoDDNSServerSlap)
         {
             continue;
         }

         /* Get Instance number for the current folder */
         pSlapVariable = pPoamIrepFoDDNSServerSlap->GetRecord
                           (
                               (ANSC_HANDLE)pPoamIrepFoDDNSServerSlap,
                               COSA_DML_RR_NAME_DDNS_SERVER_InsNum,
                               NULL
                           );

        if (pSlapVariable)
        {
            ulInstanceNumber = pSlapVariable->Variant.varUint32;
            SlapFreeVariable(pSlapVariable);
        }
        else
        {
            continue;
        }

        /* Get Alias name for the current folder */
         pSlapVariable = pPoamIrepFoDDNSServerSlap->GetRecord
                           (
                               (ANSC_HANDLE)pPoamIrepFoDDNSServerSlap,
                               COSA_DML_RR_NAME_DDNS_SERVER_Alias,
                               NULL
                           );
        if (pSlapVariable)
        {
           pAlias = AnscCloneString(pSlapVariable->Variant.varString);
            SlapFreeVariable(pSlapVariable);
        }

        /* Also, Handles the case where pSlapVariable = NULL eventually pAlias will also be NULL */
        if (!pAlias)
        {
            continue;
        }

        pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
        if (!pCosaContext )
        {
            AnscFreeMemory(pAlias);
            return ANSC_STATUS_RESOURCES;
        }

        pServerEntry = (COSA_DML_DDNS_SERVER *)AnscAllocateMemory(sizeof(COSA_DML_DDNS_SERVER));
       if (!pServerEntry)
        {
            AnscFreeMemory(pAlias);
            AnscFreeMemory(pCosaContext);
            return ANSC_STATUS_RESOURCES;
        }

        pServerEntry->InstanceNumber = ulInstanceNumber;
        AnscCopyString(pServerEntry->Alias, pAlias);
        /*Copy the current entry into COSA_CONTEXT_LINK_OBJECT */
        pCosaContext->InstanceNumber        = ulInstanceNumber;
        pCosaContext->bNew                  = TRUE;
        pCosaContext->hContext              = (ANSC_HANDLE)pServerEntry;
       pCosaContext->hParentTable          = NULL;
        pCosaContext->hPoamIrepUpperFo      = (ANSC_HANDLE)pPoamIrepFoDDNSServer;
        pCosaContext->hPoamIrepFo           = (ANSC_HANDLE)pPoamIrepFoDDNSServerSlap;

        CosaSListPushEntryByInsNum(pListHead, pCosaContext);
        AnscFreeMemory(pAlias);
        pAlias = NULL;
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDynamicDns_ServerAddInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    )
{
    PCOSA_DATAMODEL_TR181_DDNS    pMyObject                  = (PCOSA_DATAMODEL_TR181_DDNS)hThisObject;
    PPOAM_IREP_FOLDER_OBJECT    pPoamIrepFoDDNSServer      = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderDDNSServer;
    PPOAM_IREP_FOLDER_OBJECT    pPoamIrepFoDDNSServerSlap  = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PCOSA_CONTEXT_LINK_OBJECT   pCosaContext               = (PCOSA_CONTEXT_LINK_OBJECT)hCosaContext;
    COSA_DML_DDNS_SERVER        *pServerEntry              = (COSA_DML_DDNS_SERVER *)pCosaContext->hContext;
    PSLAP_VARIABLE              pSlapVariable              = (PSLAP_VARIABLE)NULL;

    if (!pPoamIrepFoDDNSServer)
    {
        return ANSC_STATUS_FAILURE;
    }

    pPoamIrepFoDDNSServer->EnableFileSync((ANSC_HANDLE)pPoamIrepFoDDNSServer, FALSE);

    /* Allocate SLAP variable */
    SlapAllocVariable(pSlapVariable);

    if (!pSlapVariable)
    {
        pPoamIrepFoDDNSServer->EnableFileSync((ANSC_HANDLE)pPoamIrepFoDDNSServer, TRUE);
        return ANSC_STATUS_RESOURCES;
    }

    /* Delete the Next Instance number Record and Add it with the slap variable */
    pPoamIrepFoDDNSServer->DelRecord
        (
            (ANSC_HANDLE)pPoamIrepFoDDNSServer,
            COSA_DML_RR_NAME_DDNS_SERVER_NextInsNumber
        );

    pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
    pSlapVariable->Variant.varUint32 = pMyObject->DDNSServerNextInsNum;

    pPoamIrepFoDDNSServer->AddRecord
        (
            (ANSC_HANDLE)pPoamIrepFoDDNSServer,
            COSA_DML_RR_NAME_DDNS_SERVER_NextInsNumber,
            SYS_REP_RECORD_TYPE_UINT,
            SYS_RECORD_CONTENT_DEFAULT,
            pSlapVariable,
            0
        );

    SlapCleanVariable(pSlapVariable);
    SlapInitVariable (pSlapVariable);

    /* Create sub folder with Alias name */
    pPoamIrepFoDDNSServerSlap =
        pPoamIrepFoDDNSServer->AddFolder
            (
                (ANSC_HANDLE)pPoamIrepFoDDNSServer,
                pServerEntry->Alias,
                0
            );

    if (!pPoamIrepFoDDNSServerSlap)
    {
        SlapFreeVariable(pSlapVariable);
        pPoamIrepFoDDNSServer->EnableFileSync((ANSC_HANDLE)pPoamIrepFoDDNSServer, TRUE);
        return ANSC_STATUS_FAILURE;
    }

    /* Add the Instance number and Alias name to the sub folder with Slap variable */
    pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
    pSlapVariable->Variant.varUint32 = pServerEntry->InstanceNumber;

    pPoamIrepFoDDNSServerSlap->AddRecord
        (
            (ANSC_HANDLE)pPoamIrepFoDDNSServer,
            COSA_DML_RR_NAME_DDNS_SERVER_InsNum,
            SYS_REP_RECORD_TYPE_UINT,
            SYS_RECORD_CONTENT_DEFAULT,
            pSlapVariable,
            0
        );

    SlapCleanVariable(pSlapVariable);
    SlapInitVariable (pSlapVariable);

    /* Add alias name */
    pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;
    pSlapVariable->Variant.varString = AnscCloneString(pServerEntry->Alias);

    pPoamIrepFoDDNSServerSlap->AddRecord
        (
            (ANSC_HANDLE)pPoamIrepFoDDNSServer,
            COSA_DML_RR_NAME_DDNS_SERVER_Alias,
            SYS_REP_RECORD_TYPE_UINT,
            SYS_RECORD_CONTENT_DEFAULT,
            pSlapVariable,
            0
        );

    SlapCleanVariable(pSlapVariable);
    SlapInitVariable (pSlapVariable);

    /* Update the COSA_CONTEXT_LINK_OBJECT with the Folder created */
    pCosaContext->hPoamIrepUpperFo = (ANSC_HANDLE)pPoamIrepFoDDNSServer;
    pCosaContext->hPoamIrepFo      = (ANSC_HANDLE)pPoamIrepFoDDNSServerSlap;

    /* Enable the File sync and Free the allocated memory for Slap variable */
    SlapFreeVariable(pSlapVariable);
    pPoamIrepFoDDNSServer->EnableFileSync((ANSC_HANDLE)pPoamIrepFoDDNSServer, TRUE);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDynamicDns_ServerDelInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hCosaContext;
   PPOAM_IREP_FOLDER_OBJECT        pPoamIrepUpperFo  = (PPOAM_IREP_FOLDER_OBJECT )pCosaContext->hPoamIrepUpperFo;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFo       = (PPOAM_IREP_FOLDER_OBJECT )pCosaContext->hPoamIrepFo;

    if ( !pPoamIrepUpperFo || !pPoamIrepFo )
    {
        return ANSC_STATUS_FAILURE;
    }

    pPoamIrepUpperFo->EnableFileSync((ANSC_HANDLE)pPoamIrepUpperFo, FALSE);

    /*Close and the Delete the sub folder */
    pPoamIrepFo->Close((ANSC_HANDLE)pPoamIrepFo);

    pPoamIrepUpperFo->DelFolder
        (
            (ANSC_HANDLE)pPoamIrepUpperFo,
            pPoamIrepFo->GetFolderName((ANSC_HANDLE)pPoamIrepFo)
        );

    pPoamIrepUpperFo->EnableFileSync((ANSC_HANDLE)pPoamIrepUpperFo, TRUE);
    AnscFreeMemory(pPoamIrepFo);

    return ANSC_STATUS_SUCCESS;
}
