

#include "ansc_platform.h"
#include "cosa_dynamicdns_dml.h"
#include "cosa_dynamicdns_internal.h"
#include "cosa_dynamicdns_apis.h"
          /* MACROS */
#define  MAX_CLIENT_COUNT 1
#define  MAX_HOST_COUNT 1
#define  HOST_DISABLED                    5
#define  CLIENT_DISABLED                  6
#define  CLIENT_LAST_NO_ERROR             1
/***********************************************************************
 APIs for Object:

    DyanmicDNS.

    *  DynamicDNS_GetParamBoolValue
    *  DynamicDNS_GetParamStringValue
    *  DynamicDNS_SetParamBoolValue
***********************************************************************/

BOOL
DynamicDNS_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
#if defined(DDNS_BROADBANDFORUM)
    if (AnscEqualString(ParamName, "X_RDK-COM_Enable", TRUE))
    {
        *pBool = CosaDmlDynamicDns_GetEnable();
        return TRUE;
    }
    return FALSE;
#endif
#if !defined(DDNS_BROADBANDFORUM)
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pBool); 
    return TRUE;
#endif
}

ULONG
DynamicDNS_GetParamStringValue
    (
        ANSC_HANDLE                     hInsContext,
        char*                           ParamName,
        char*                           pValue,
        ULONG*                          puLong
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(puLong);
#if defined(DDNS_BROADBANDFORUM)
    char supportedServices[1024] = {0};
    if (AnscEqualString(ParamName, "SupportedServices", TRUE))
    {
        if (!CosaDmlDynamicDns_GetsupportedServices(supportedServices))
        {
            AnscCopyString(pValue, supportedServices);
            return 0;
        }
    }
    return -1;
#endif

#if !defined(DDNS_BROADBANDFORUM)
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pValue); 
    return  0;
#endif
}

BOOL
DynamicDNS_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
#if defined(DDNS_BROADBANDFORUM)
    if (AnscEqualString(ParamName, "X_RDK-COM_Enable", TRUE))
    {
        if (!CosaDmlDynamicDns_SetEnable(bValue))
        {
            return TRUE;
        }
    }
    return FALSE;
#endif
#if !defined(DDNS_BROADBANDFORUM)
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(bValue);
    return TRUE;
#endif
}

BOOL
DynamicDNS_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pReturnParamName);
    UNREFERENCED_PARAMETER(puLength);
    return TRUE;
}

ULONG
DynamicDNS_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    return ANSC_STATUS_SUCCESS;
}

ULONG
DynamicDNS_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    return ANSC_STATUS_SUCCESS;
}

/***********************************************************************
 APIs for Object:

    DyanmicDNS.Client.{i}

    *  DDNSClient_GetEntryCount
    *  DDNSClient_GetEntry
    *  DDNSClient_AddEntry
    *  DDNSClient_DelEntry
    *  DDNSClient_GetParamBoolValue
    *  DDNSClient_GetParamUlongValue
    *  DDNSClient_GetParamStringValue
    *  DDNSClient_SetParamBoolValue
    *  DDNSClient_SetParamStringValue
    *  Client_Validate
    *  Client_Commit
    *  Client_Rollback
***********************************************************************/

ULONG
DDNSClient_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
#if defined(DDNS_BROADBANDFORUM)
    COSA_DATAMODEL_TR181_DDNS *pDynamicDns = (COSA_DATAMODEL_TR181_DDNS *)g_pCosaBEManager->hDynamicDns;

    return AnscSListQueryDepth(&pDynamicDns->DDNSClientList);
#endif
#if !defined(DDNS_BROADBANDFORUM) 
   return 0;
#endif

}

ANSC_HANDLE
DDNSClient_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
#if defined(DDNS_BROADBANDFORUM)
    COSA_DATAMODEL_TR181_DDNS *pDynamicDns = (COSA_DATAMODEL_TR181_DDNS *)g_pCosaBEManager->hDynamicDns;
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry = NULL;

    pSLinkEntry = AnscQueueGetEntryByIndex((ANSC_HANDLE)&pDynamicDns->DDNSClientList, nIndex);
    if (pSLinkEntry)
    {
        pLinkObj = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        *pInsNumber = pLinkObj->InstanceNumber;
    }

    return pSLinkEntry;
#endif

#if !defined(DDNS_BROADBANDFORUM)
    UNREFERENCED_PARAMETER(nIndex);
    UNREFERENCED_PARAMETER(pInsNumber); 
    return 0;
#endif
}

ANSC_HANDLE
DDNSClient_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
#if defined(DDNS_BROADBANDFORUM)
    COSA_DATAMODEL_TR181_DDNS *pDynamicDns     = (COSA_DATAMODEL_TR181_DDNS *)g_pCosaBEManager->hDynamicDns;
    PCOSA_CONTEXT_LINK_OBJECT  pLinkObj  = NULL;
    COSA_DML_DDNS_CLIENT  *pClientEntry  = NULL;

    if(MAX_CLIENT_COUNT == AnscSListQueryDepth(&pDynamicDns->DDNSClientList)) {
        return NULL;
    }
    pLinkObj = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
    if (!pLinkObj)
    {
        return NULL;
    }

    pClientEntry = (COSA_DML_DDNS_CLIENT *)AnscAllocateMemory(sizeof(COSA_DML_DDNS_CLIENT));
    if (!pClientEntry)
    {
        AnscFreeMemory(pLinkObj);
        return NULL;
    }

    pLinkObj->InstanceNumber     = pDynamicDns->DDNSClientNextInsNum;
    pClientEntry->InstanceNumber = pDynamicDns->DDNSClientNextInsNum;
    pDynamicDns->DDNSClientNextInsNum++;

    if (pDynamicDns->DDNSClientNextInsNum == 0)
    {
        pDynamicDns->DDNSClientNextInsNum = 1;
    }
    _ansc_sprintf(pClientEntry->Alias, "cpe-ddns-client-%d", pLinkObj->InstanceNumber);
    pLinkObj->hContext      = (ANSC_HANDLE)pClientEntry;
    pLinkObj->hParentTable  = NULL;
    pLinkObj->bNew          = TRUE;

    CosaSListPushEntryByInsNum((PSLIST_HEADER)&pDynamicDns->DDNSClientList, pLinkObj);
    CosaDynamicDns_ClientAddInfo((ANSC_HANDLE)pDynamicDns, (ANSC_HANDLE)pLinkObj);

    *pInsNumber = pLinkObj->InstanceNumber;
    return pLinkObj;
#endif

#if !defined(DDNS_BROADBANDFORUM)
    UNREFERENCED_PARAMETER(pInsNumber); 
    return 0;
#endif
}

ULONG
DDNSClient_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
#if defined(DDNS_BROADBANDFORUM)
    COSA_DATAMODEL_TR181_DDNS        *pDynamicDns  = (COSA_DATAMODEL_TR181_DDNS *)g_pCosaBEManager->hDynamicDns;
    PCOSA_CONTEXT_LINK_OBJECT  pLinkObj      = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    COSA_DML_DDNS_CLIENT       *pClientEntry = (COSA_DML_DDNS_CLIENT *)pLinkObj->hContext;
    ANSC_STATUS                returnStatus  = ANSC_STATUS_SUCCESS;

    CosaDmlDynamicDns_Client_DelEntry(pLinkObj->InstanceNumber);
    AnscSListPopEntryByLink((PSLIST_HEADER)&pDynamicDns->DDNSClientList, &pLinkObj->Linkage);

    if (pLinkObj->bNew)
    {
        pLinkObj->bNew = FALSE;
        CosaDynamicDns_ClientDelInfo((ANSC_HANDLE)pDynamicDns, (ANSC_HANDLE)pLinkObj);
    }
/*    else
    {
        CosaDmlDynamicDns_Client_DelEntry(pLinkObj->InstanceNumber);
    }
    if (returnStatus == ANSC_STATUS_SUCCESS)
    {
        AnscSListPopEntryByLink((PSLIST_HEADER)&pDynamicDns->DDNSClientList, &pLinkObj->Linkage);
        AnscFreeMemory(pClientEntry);
        AnscFreeMemory(pLinkObj);
    }
*/
    AnscFreeMemory(pClientEntry);
    AnscFreeMemory(pLinkObj);

    return returnStatus;
#endif
#if !defined(DDNS_BROADBANDFORUM)
    UNREFERENCED_PARAMETER(hInstance); 
    return 0;
#endif
}

BOOL
DDNSClient_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
#if defined(DDNS_BROADBANDFORUM)
    PCOSA_CONTEXT_LINK_OBJECT    pLinkObj      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_CLIENT         *pClientEntry = (COSA_DML_DDNS_CLIENT *)pLinkObj->hContext;

    CosaDmlDynamicDns_Client_GetConf(pClientEntry->InstanceNumber,pClientEntry);
    if (AnscEqualString(ParamName, "Enable", TRUE))
    {
        *pBool = pClientEntry->Enable;
        return TRUE;
    }
    return FALSE;
#endif
#if !defined(DDNS_BROADBANDFORUM)
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pBool); 
    return TRUE;
#endif
}
BOOL
DDNSClient_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
#if defined(DDNS_BROADBANDFORUM)
    PCOSA_CONTEXT_LINK_OBJECT    pLinkObj      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_CLIENT         *pClientEntry = (COSA_DML_DDNS_CLIENT *)pLinkObj->hContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Status", TRUE))
    {
        /* collect value */
        if(pClientEntry->Enable)
            *puLong = CosaDmlDynamicDns_GetClientStatus();
        else
            *puLong = CLIENT_DISABLED;
        return TRUE;
    }
    if( AnscEqualString(ParamName, "LastError", TRUE))
    {
        /* collect value */
        if(pClientEntry->Enable)
            *puLong = CosaDmlDynamicDns_GetClientLastError();
        else
            *puLong = CLIENT_LAST_NO_ERROR;
        return TRUE;
    }

    return FALSE;
#endif
#if !defined(DDNS_BROADBANDFORUM)
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(puLong); 
    return TRUE;
#endif
}

ULONG
DDNSClient_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(pUlSize);
#if defined(DDNS_BROADBANDFORUM)
    PCOSA_CONTEXT_LINK_OBJECT    pLinkObj      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_CLIENT         *pClientEntry = (COSA_DML_DDNS_CLIENT *)pLinkObj->hContext;

    if (AnscEqualString(ParamName, "Alias", TRUE))
    {
        AnscCopyString(pValue, pClientEntry->Alias);
        return 0;
    }
    if (AnscEqualString(ParamName, "Username", TRUE))
    {
        AnscCopyString(pValue, pClientEntry->Username);
        return 0;
    }
    if (AnscEqualString(ParamName, "Password", TRUE))
    {
        AnscCopyString(pValue, "");
        return 0;
    }
    if (AnscEqualString(ParamName, "Server", TRUE))
    {
        AnscCopyString(pValue, pClientEntry->Server);
        return 0;
    }
    if (AnscEqualString(ParamName, "Interface", TRUE))
    {
        AnscCopyString(pValue, "Device.IP.Interface.1");
        return 0;
    }
    return -1;
#endif

#if !defined(DDNS_BROADBANDFORUM)
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pValue);  
    return 0;
#endif
}

BOOL
DDNSClient_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
#if defined(DDNS_BROADBANDFORUM)
    PCOSA_CONTEXT_LINK_OBJECT    pLinkObj      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_CLIENT         *pClientEntry = (COSA_DML_DDNS_CLIENT *)pLinkObj->hContext;

    if (AnscEqualString(ParamName, "Enable", TRUE))
    {
        pClientEntry->Enable = bValue;
        return TRUE;
    }
    return FALSE;
#endif
#if !defined(DDNS_BROADBANDFORUM)
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(bValue);
    return TRUE;
#endif
}

BOOL
DDNSClient_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    )
{
#if defined(DDNS_BROADBANDFORUM)
    PCOSA_CONTEXT_LINK_OBJECT    pLinkObj      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_CLIENT         *pClientEntry = (COSA_DML_DDNS_CLIENT *)pLinkObj->hContext;

    if (AnscEqualString(ParamName, "Interface", TRUE))
    {
        _ansc_snprintf(pClientEntry->Interface, sizeof(pClientEntry->Interface), "%s", strValue);
        return TRUE;
    }
    if (AnscEqualString(ParamName, "Alias", TRUE))
    {
        _ansc_snprintf(pClientEntry->Alias, sizeof(pClientEntry->Alias), "%s", strValue);
        return TRUE;
    }
    if (AnscEqualString(ParamName, "Username", TRUE))
    {
        _ansc_snprintf(pClientEntry->Username, sizeof(pClientEntry->Username), "%s", strValue);
        return TRUE;
    }
    if (AnscEqualString(ParamName, "Password", TRUE))
    {
        _ansc_snprintf(pClientEntry->Password, sizeof(pClientEntry->Password), "%s", strValue);
        return TRUE;
    }
    if (AnscEqualString(ParamName, "Server", TRUE))
    {
        _ansc_snprintf(pClientEntry->Server, sizeof(pClientEntry->Server), "%s", strValue);
        return TRUE;
    }
    return FALSE;
#endif

#if !defined(DDNS_BROADBANDFORUM)
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(strValue); 
    return TRUE;
#endif
}

BOOL
DDNSClient_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
#if defined(DDNS_BROADBANDFORUM)
    PCOSA_CONTEXT_LINK_OBJECT    pLinkObj      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_CLIENT         *pClientEntry = (COSA_DML_DDNS_CLIENT *)pLinkObj->hContext;
    PCOSA_DATAMODEL_TR181_DDNS   pDynamicDns   = (PCOSA_DATAMODEL_TR181_DDNS)g_pCosaBEManager->hDynamicDns;
    int validate;

    if (!(1 == sscanf(pClientEntry->Server, "Device.DynamicDNS.Server.%d", &validate) &&
        (validate <= AnscSListQueryDepth(&pDynamicDns->DDNSServerList))))
    {
        _ansc_strcpy(pReturnParamName, "Server");
        return FALSE;
    }
    if(pClientEntry->Enable && !CosaDmlDynamicDns_GetEnable())
    {
        _ansc_strcpy(pReturnParamName, "Enable");
        return FALSE;
    }

    return TRUE;
#endif

#if !defined(DDNS_BROADBANDFORUM)
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pReturnParamName);
    UNREFERENCED_PARAMETER(puLength); 
    return TRUE;
#endif
}

ULONG
DDNSClient_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
#if defined(DDNS_BROADBANDFORUM)
    PCOSA_CONTEXT_LINK_OBJECT    pLinkObj      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_CLIENT         *pClientEntry = (COSA_DML_DDNS_CLIENT *)pLinkObj->hContext;
    PCOSA_DATAMODEL_TR181_DDNS   pDynamicDns   = (PCOSA_DATAMODEL_TR181_DDNS)g_pCosaBEManager->hDynamicDns;

    if (pLinkObj->bNew)
    {
        pLinkObj->bNew = FALSE;
        if (CosaDmlDynamicDns_Client_AddEntry(pClientEntry) != ANSC_STATUS_SUCCESS)
        {
            return -1;
        }
        CosaDynamicDns_ClientDelInfo((ANSC_HANDLE)pDynamicDns, (ANSC_HANDLE)pLinkObj);
    }
    else
    {
        if (CosaDmlDynamicDns_Client_SetConf(pClientEntry->InstanceNumber, pClientEntry) != ANSC_STATUS_SUCCESS)
        {
            CosaDmlDynamicDns_Client_GetConf(pClientEntry->InstanceNumber, pClientEntry);
            return -1;
        }
    }

    return 0;
#endif
#if !defined(DDNS_BROADBANDFORUM)
    UNREFERENCED_PARAMETER(hInsContext); 
    return 0;
#endif
}

ULONG
DDNSClient_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
#if defined(DDNS_BROADBANDFORUM)
    PCOSA_CONTEXT_LINK_OBJECT    pLinkObj      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_CLIENT         *pClientEntry = (COSA_DML_DDNS_CLIENT *)pLinkObj->hContext;

    if (CosaDmlDynamicDns_Client_GetConf(pClientEntry->InstanceNumber, pClientEntry) == ANSC_STATUS_SUCCESS)
    {
        return 0;
    }
    return -1;
#endif
#if !defined(DDNS_BROADBANDFORUM)
    UNREFERENCED_PARAMETER(hInsContext); 
    return 0;
#endif
}

/***********************************************************************

 APIs for Object:

    Device.DynamicDNS.Client.{i}.Hostname.{i}

    *  DDNSHostname_GetEntryCount
    *  DDNSHostname_GetEntry
    *  DDNSHostname_AddEntry
    *  DDNSHostname_DelEntry
    *  DDNSHostname_GetParamBoolValue
    *  DDNSHostname_GetParamUlongValue
    *  DDNSHostname_GetParamStringValue
    *  DDNSHostname_SetParamBoolValue
    *  DDNSHostname_SetParamStringValue
    *  DDNSHostname_Validate
    *  DDNSHostname_Commit
    *  DDNSHostname_Rollback

***********************************************************************/

ULONG
DDNSHostname_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
#if defined(DDNS_BROADBANDFORUM)
    COSA_DATAMODEL_TR181_DDNS *pDynamicDns = (COSA_DATAMODEL_TR181_DDNS *)g_pCosaBEManager->hDynamicDns;

    return AnscSListQueryDepth(&pDynamicDns->DDNSHostList); //Hard-coded to 1,as maximum instance will be always 1
#endif
#if !defined(DDNS_BROADBANDFORUM) 
  return 0;
#endif
}

ANSC_HANDLE
DDNSHostname_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
#if defined(DDNS_BROADBANDFORUM)
    COSA_DATAMODEL_TR181_DDNS *pDynamicDns = (COSA_DATAMODEL_TR181_DDNS *)g_pCosaBEManager->hDynamicDns;
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry = NULL;

    pSLinkEntry = AnscQueueGetEntryByIndex((ANSC_HANDLE)&pDynamicDns->DDNSHostList, nIndex);
    if (pSLinkEntry)
    {
        pLinkObj = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        *pInsNumber = pLinkObj->InstanceNumber;
    }

    return pSLinkEntry;
#endif

#if !defined(DDNS_BROADBANDFORUM) 
    UNREFERENCED_PARAMETER(nIndex);
    UNREFERENCED_PARAMETER(pInsNumber);
    return 0;
#endif
}

ANSC_HANDLE
DDNSHostname_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
#if defined(DDNS_BROADBANDFORUM)
    COSA_DATAMODEL_TR181_DDNS *pDynamicDns     = (COSA_DATAMODEL_TR181_DDNS *)g_pCosaBEManager->hDynamicDns;
    PCOSA_CONTEXT_LINK_OBJECT  pLinkObj  = NULL;
    COSA_DML_DDNS_HOST  *pHostEntry      = NULL;

    if(MAX_HOST_COUNT == AnscSListQueryDepth(&pDynamicDns->DDNSHostList)) {
        return NULL;
    }

    pLinkObj = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
    if (!pLinkObj)
    {
        return NULL;
    }

    pHostEntry = (COSA_DML_DDNS_HOST *)AnscAllocateMemory(sizeof(COSA_DML_DDNS_HOST));
    if (!pHostEntry)
    {
        AnscFreeMemory(pLinkObj);
        return NULL;
    }

    pLinkObj->InstanceNumber     = pDynamicDns->DDNSHostNextInsNum;
    pHostEntry->InstanceNumber   = pDynamicDns->DDNSHostNextInsNum;
    pDynamicDns->DDNSHostNextInsNum++;

    if (pDynamicDns->DDNSHostNextInsNum == 0)
    {
        pDynamicDns->DDNSHostNextInsNum = 1;
    }
    _ansc_sprintf(pHostEntry->Alias, "cpe-ddns-host-%d", pLinkObj->InstanceNumber);
    pHostEntry->Status      = 5;
    pLinkObj->hContext      = (ANSC_HANDLE)pHostEntry;
    pLinkObj->hParentTable  = NULL;
    pLinkObj->bNew          = TRUE;

    CosaSListPushEntryByInsNum((PSLIST_HEADER)&pDynamicDns->DDNSHostList, pLinkObj);
    CosaDynamicDns_HostAddInfo((ANSC_HANDLE)pDynamicDns, (ANSC_HANDLE)pLinkObj);

    *pInsNumber = pLinkObj->InstanceNumber;
    return  pLinkObj;
#endif

#if !defined(DDNS_BROADBANDFORUM)
    UNREFERENCED_PARAMETER(pInsNumber); 
    return 0;
#endif
}

ULONG
DDNSHostname_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
#if defined(DDNS_BROADBANDFORUM)
    COSA_DATAMODEL_TR181_DDNS        *pDynamicDns  = (COSA_DATAMODEL_TR181_DDNS *)g_pCosaBEManager->hDynamicDns;
    PCOSA_CONTEXT_LINK_OBJECT  pLinkObj      = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    ANSC_STATUS                returnStatus  = ANSC_STATUS_SUCCESS;

    CosaDmlDynamicDns_Host_DelEntry(pLinkObj->InstanceNumber);
    AnscSListPopEntryByLink((PSLIST_HEADER)&pDynamicDns->DDNSHostList, &pLinkObj->Linkage);
    if (pLinkObj->bNew)
    {
        pLinkObj->bNew = FALSE;
        CosaDynamicDns_HostDelInfo((ANSC_HANDLE)pDynamicDns, (ANSC_HANDLE)pLinkObj);
    }
/*    else
    {
        returnStatus = CosaDmlDynamicDns_Host_DelEntry(pLinkObj->InstanceNumber);
    }

    if (returnStatus == ANSC_STATUS_SUCCESS)
    {
        AnscSListPopEntryByLink((PSLIST_HEADER)&pDynamicDns->DDNSHostList, &pLinkObj->Linkage);
        AnscFreeMemory(pLinkObj->hContext);
        AnscFreeMemory(pLinkObj);
    } */

    AnscFreeMemory(pLinkObj->hContext);
    AnscFreeMemory(pLinkObj);
    return returnStatus;
#endif

#if !defined(DDNS_BROADBANDFORUM)
    UNREFERENCED_PARAMETER(hInstance);   
    return 0;
#endif
}

BOOL
DDNSHostname_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
#if defined(DDNS_BROADBANDFORUM)
    PCOSA_CONTEXT_LINK_OBJECT    pLinkObj      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_HOST           *pHostEntry   = (COSA_DML_DDNS_HOST *)pLinkObj->hContext;
    CosaDmlDynamicDns_Host_GetConf(pHostEntry->InstanceNumber,pHostEntry);

    if (AnscEqualString(ParamName, "Enable", TRUE))
    {
        *pBool = pHostEntry->Enable;
        return TRUE;
    }
    return FALSE;
#endif

#if !defined(DDNS_BROADBANDFORUM)
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pBool); 
    return TRUE;
#endif
}

BOOL
DDNSHostname_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
#if defined(DDNS_BROADBANDFORUM)
    PCOSA_CONTEXT_LINK_OBJECT    pLinkObj      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_HOST           *pHostEntry   = (COSA_DML_DDNS_HOST *)pLinkObj->hContext;
    CosaDmlDynamicDns_Host_GetConf(pHostEntry->InstanceNumber,pHostEntry);

    if( AnscEqualString(ParamName, "Status", TRUE))
    {
        /* collect value */
        if(pHostEntry->Enable)
            *puLong = pHostEntry->Status;
        else
            *puLong = HOST_DISABLED;
        return TRUE;
    }

    return FALSE;
#endif

#if !defined(DDNS_BROADBANDFORUM)
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(puLong); 
    return TRUE;
#endif
}

ULONG
DDNSHostname_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(pUlSize);
#if defined(DDNS_BROADBANDFORUM)
    PCOSA_CONTEXT_LINK_OBJECT    pLinkObj        = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_HOST           *pHostEntry     = (COSA_DML_DDNS_HOST *)pLinkObj->hContext;
    CosaDmlDynamicDns_Host_GetConf(pHostEntry->InstanceNumber,pHostEntry);

    if (AnscEqualString(ParamName, "Name", TRUE))
    {
        AnscCopyString(pValue, pHostEntry->Name);
        return 0;
    }
    if (AnscEqualString(ParamName, "LastUpdate", TRUE))
    {
        //Need to check the date time format
        CosaDmlDynamicDns_GetHostLastUpdate(pHostEntry->LastUpdate);
        AnscCopyString(pValue, pHostEntry->LastUpdate);
        return 0;
    }

    return -1;
#endif

#if !defined(DDNS_BROADBANDFORUM)
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pValue); 
    return 0;
#endif
}

BOOL
DDNSHostname_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
#if defined(DDNS_BROADBANDFORUM)
    PCOSA_CONTEXT_LINK_OBJECT    pLinkObj      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_HOST           *pHostEntry   = (COSA_DML_DDNS_HOST *)pLinkObj->hContext;

    if (AnscEqualString(ParamName, "Enable", TRUE))
    {
        pHostEntry->Enable = bValue;
        return TRUE;
    }
    return FALSE;
#endif

#if !defined(DDNS_BROADBANDFORUM)
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(bValue); 
   return TRUE;
#endif
}

BOOL
DDNSHostname_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    )
{
#if defined(DDNS_BROADBANDFORUM)
    PCOSA_CONTEXT_LINK_OBJECT    pLinkObj      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_HOST           *pHostEntry   = (COSA_DML_DDNS_HOST *)pLinkObj->hContext;

    if (AnscEqualString(ParamName, "Name", TRUE))
    {
        _ansc_snprintf(pHostEntry->Name, sizeof(pHostEntry->Name), "%s", strValue);
        return TRUE;
    }

    return FALSE;
#endif
#if !defined(DDNS_BROADBANDFORUM)
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(strValue); 
   return TRUE;
#endif
}


BOOL
DDNSHostname_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
#if defined(DDNS_BROADBANDFORUM)
    PCOSA_CONTEXT_LINK_OBJECT    pLinkObj      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_HOST           *pHostEntry   = (COSA_DML_DDNS_HOST *)pLinkObj->hContext;

    if(pHostEntry->Enable && !CosaDmlDynamicDns_GetEnable())
    {
        _ansc_strcpy(pReturnParamName, "Enable");
        return FALSE;
    }

    return TRUE;
#endif

#if !defined(DDNS_BROADBANDFORUM)
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pReturnParamName);
    UNREFERENCED_PARAMETER(puLength); 
    return TRUE;
#endif
}

ULONG
DDNSHostname_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
#if defined(DDNS_BROADBANDFORUM)
    PCOSA_CONTEXT_LINK_OBJECT    pLinkObj      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_HOST           *pHostEntry   = (COSA_DML_DDNS_HOST *)pLinkObj->hContext;
    PCOSA_DATAMODEL_TR181_DDNS     pDynamicDns   = (PCOSA_DATAMODEL_TR181_DDNS)g_pCosaBEManager->hDynamicDns;

    if (pLinkObj->bNew)
    {
        if (CosaDmlDynamicDns_Host_AddEntry(pHostEntry) != ANSC_STATUS_SUCCESS)
        {
            return -1;
        }
        CosaDynamicDns_HostDelInfo((ANSC_HANDLE)pDynamicDns, (ANSC_HANDLE)pLinkObj);
        pLinkObj->bNew = FALSE;
    }
    else
    {
        if (CosaDmlDynamicDns_Host_SetConf(pHostEntry->InstanceNumber, pHostEntry) != ANSC_STATUS_SUCCESS)
        {
            CosaDmlDynamicDns_Host_GetConf(pHostEntry->InstanceNumber, pHostEntry);
            return -1;
        }
    }

    return 0;
#endif
#if !defined(DDNS_BROADBANDFORUM)
    UNREFERENCED_PARAMETER(hInsContext);
    return 0;
#endif
}

ULONG
DDNSHostname_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
#if defined(DDNS_BROADBANDFORUM)
    PCOSA_CONTEXT_LINK_OBJECT    pLinkObj      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_HOST           *pHostEntry   = (COSA_DML_DDNS_HOST *)pLinkObj->hContext;

    if (CosaDmlDynamicDns_Host_GetConf(pHostEntry->InstanceNumber, pHostEntry) == ANSC_STATUS_SUCCESS)
    {
        return 0;
    }
    return -1;
#endif
#if !defined(DDNS_BROADBANDFORUM) 
    UNREFERENCED_PARAMETER(hInsContext);
    return 0;
#endif
}

/***********************************************************************
 APIs for Object:

    DyanmicDNS.Server.{i}

    *  DDNSServer_GetEntryCount
    *  DDNSServer_GetEntry
    *  DDNSServer_AddEntry
    *  DDNSServer_DelEntry
    *  DDNSServer_GetParamBoolValue
    *  DDNSServer_GetParamUlongValue
    *  DDNSServer_GetParamStringValue
    *  DDNSServer_SetParamBoolValue
    *  DDNSServer_SetParamStringValue
    *  DDNSServer_Validate
    *  DDNSServer_Commit
    *  DDNSServer_Rollback
***********************************************************************/

ULONG
DDNSServer_GetEntryCount
    (
       ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
#if defined(DDNS_BROADBANDFORUM)
    PCOSA_DATAMODEL_TR181_DDNS             pMyObject         = (PCOSA_DATAMODEL_TR181_DDNS)g_pCosaBEManager->hDynamicDns;

    return AnscSListQueryDepth(&pMyObject->DDNSServerList);
#endif
#if !defined(DDNS_BROADBANDFORUM) 
    return 0;
#endif
}

ANSC_HANDLE
DDNSServer_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
#if defined(DDNS_BROADBANDFORUM)
    PCOSA_DATAMODEL_TR181_DDNS         pMyObject         = (PCOSA_DATAMODEL_TR181_DDNS)g_pCosaBEManager->hDynamicDns;
    PCOSA_CONTEXT_LINK_OBJECT        pLinkObj          = NULL;
    PSINGLE_LINK_ENTRY               pSLinkEntry       = NULL;

    pSLinkEntry = AnscQueueGetEntryByIndex((ANSC_HANDLE)&pMyObject->DDNSServerList, nIndex);
    if (pSLinkEntry)
    {
        pLinkObj = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        *pInsNumber = pLinkObj->InstanceNumber;
    }

    return pLinkObj;
#endif
#if !defined(DDNS_BROADBANDFORUM)
    UNREFERENCED_PARAMETER(nIndex);
    UNREFERENCED_PARAMETER(pInsNumber);
    return 0;
#endif
}

ANSC_HANDLE
DDNSServer_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
#if defined(DDNS_BROADBANDFORUM)
    COSA_DATAMODEL_TR181_DDNS        *pMyObject         = (COSA_DATAMODEL_TR181_DDNS *)g_pCosaBEManager->hDynamicDns;
    PCOSA_CONTEXT_LINK_OBJECT      pLinkObj           = NULL;
    COSA_DML_DDNS_SERVER           *pDDNSServer       = NULL;

    pLinkObj = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
    if (!pLinkObj)
    {
        return NULL;
    }

    pDDNSServer = (COSA_DML_DDNS_SERVER*)AnscAllocateMemory(sizeof(COSA_DML_DDNS_SERVER));

    if (!pDDNSServer)
    {
        AnscFreeMemory(pLinkObj);
        return NULL;
    }
     /* now we have this link content */
    pLinkObj->InstanceNumber = pMyObject->DDNSServerNextInsNum;
    pDDNSServer->InstanceNumber = pMyObject->DDNSServerNextInsNum;
    pMyObject->DDNSServerNextInsNum++;

    if (pMyObject->DDNSServerNextInsNum == 0)
    {
        pMyObject->DDNSServerNextInsNum = 1;
    }

    _ansc_sprintf(pDDNSServer->Alias, "cpe-ddns-server-%d", (int)pLinkObj->InstanceNumber);
    pLinkObj->hContext      = (ANSC_HANDLE)pDDNSServer;
    pLinkObj->hParentTable  = NULL;
    pLinkObj->bNew          = TRUE;

    CosaSListPushEntryByInsNum((PSLIST_HEADER)&pMyObject->DDNSServerList, pLinkObj);
    CosaDynamicDns_ServerAddInfo((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pLinkObj);

    *pInsNumber = pLinkObj->InstanceNumber;
    return pLinkObj;
#endif
#if !defined(DDNS_BROADBANDFORUM)
    UNREFERENCED_PARAMETER(pInsNumber);
    return 0;
#endif
}

ULONG
DDNSServer_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
#if defined(DDNS_BROADBANDFORUM)
    PCOSA_DATAMODEL_TR181_DDNS   pMyObject     = (PCOSA_DATAMODEL_TR181_DDNS)g_pCosaBEManager->hDynamicDns;
    PCOSA_CONTEXT_LINK_OBJECT  pLinkObj      = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    COSA_DML_DDNS_SERVER       *pDDNSServer  = (COSA_DML_DDNS_SERVER*)pLinkObj->hContext;
    ANSC_STATUS                returnStatus  = ANSC_STATUS_SUCCESS;

    if (pLinkObj->bNew)
    {
        pLinkObj->bNew = FALSE;
        returnStatus = CosaDynamicDns_ServerDelInfo((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pLinkObj);
    }
    else
    {
        returnStatus = CosaDmlDynamicDns_Server_DelEntry(pLinkObj->InstanceNumber);
    }

    if (returnStatus == ANSC_STATUS_SUCCESS)
    {
        AnscSListPopEntryByLink((PSLIST_HEADER)&pMyObject->DDNSServerList, &pLinkObj->Linkage);
        AnscFreeMemory(pDDNSServer);
        AnscFreeMemory(pLinkObj);
    }

    return returnStatus;
#endif
#if !defined(DDNS_BROADBANDFORUM)
    UNREFERENCED_PARAMETER(hInstance);
    return 0;
#endif
}

BOOL
DDNSServer_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
#if defined(DDNS_BROADBANDFORUM)
    PCOSA_CONTEXT_LINK_OBJECT     pLinkObj     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_SERVER          *pDDNSServer = (COSA_DML_DDNS_SERVER*)pLinkObj->hContext;

    CosaDmlDynamicDns_Server_GetConf(pDDNSServer->InstanceNumber,pDDNSServer);

    if (AnscEqualString(ParamName, "Enable", TRUE))
    {
        *pBool = pDDNSServer->Enable;
        return TRUE;
    }
    return FALSE;
#endif
#if !defined(DDNS_BROADBANDFORUM)
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pBool);
    return TRUE;
#endif
}

BOOL
DDNSServer_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
#if defined(DDNS_BROADBANDFORUM)

    PCOSA_CONTEXT_LINK_OBJECT    pLinkObj      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_SERVER         *pServerEntry = (COSA_DML_DDNS_SERVER *)pLinkObj->hContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "CheckInterval", TRUE))
    {
        /* collect value */
        *puLong = pServerEntry->CheckInterval;
        return TRUE;
    }
    if( AnscEqualString(ParamName, "RetryInterval", TRUE))
    {
        /* collect value */
        *puLong = pServerEntry->RetryInterval;
        return TRUE;
    }
    if( AnscEqualString(ParamName, "MaxRetries", TRUE))
    {
        /* collect value */
        *puLong = pServerEntry->MaxRetries;
        return TRUE;
    }

    return FALSE;
#endif
#if !defined(DDNS_BROADBANDFORUM)
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(puLong);
    return TRUE;
#endif
}

ULONG
DDNSServer_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(pUlSize);
#if defined(DDNS_BROADBANDFORUM)
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_SERVER            *pDDNSServer = (COSA_DML_DDNS_SERVER*)pLinkObj->hContext;

    if (AnscEqualString(ParamName, "Name", TRUE))
    {
        AnscCopyString(pValue, pDDNSServer->Name);
        return 0;
    }
    if (AnscEqualString(ParamName, "Alias", TRUE))
    {
        AnscCopyString(pValue, pDDNSServer->Alias);
        return 0;
    }
    if (AnscEqualString(ParamName, "ServiceName", TRUE))
    {
        AnscCopyString(pValue, pDDNSServer->ServiceName);
        return 0;
    }
    if (AnscEqualString(ParamName, "ServerAddress", TRUE))
    {
        AnscCopyString(pValue, pDDNSServer->ServerAddress);
        return 0;
    }
    if (AnscEqualString(ParamName, "SupportedProtocols", TRUE))
    {
        AnscCopyString(pValue, pDDNSServer->SupportedProtocols);
        return 0;
    }
    if (AnscEqualString(ParamName, "Protocol", TRUE))
    {
        AnscCopyString(pValue, pDDNSServer->Protocol);
        return 0;
    }
    return -1;
#endif
#if !defined(DDNS_BROADBANDFORUM)
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pValue);
    return 0;
#endif
}

BOOL
DDNSServer_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        pBool
    )
{
#if defined(DDNS_BROADBANDFORUM)
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_SERVER          *pDDNSServer  = (COSA_DML_DDNS_SERVER*)pLinkObj->hContext;

    if (AnscEqualString(ParamName, "Enable", TRUE))
    {
        pDDNSServer->Enable = pBool;
        return TRUE;
    }
    return FALSE;
#endif
#if !defined(DDNS_BROADBANDFORUM)
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pBool);
    return TRUE;
#endif
}

BOOL
DDNSServer_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    )
{
#if defined(DDNS_BROADBANDFORUM)
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_SERVER           *pDDNSServer = (COSA_DML_DDNS_SERVER*)pLinkObj->hContext;

    if (AnscEqualString(ParamName, "Name", TRUE))
    {
        _ansc_snprintf(pDDNSServer->Name, sizeof(pDDNSServer->Name), "%s", strValue);
        return TRUE;
    }
    if (AnscEqualString(ParamName, "Alias", TRUE))
    {
        _ansc_snprintf(pDDNSServer->Alias, sizeof(pDDNSServer->Alias), "%s", strValue);
        return TRUE;
    }
    if (AnscEqualString(ParamName, "ServiceName", TRUE))
    {
        _ansc_snprintf(pDDNSServer->ServiceName, sizeof(pDDNSServer->ServiceName), "%s", strValue);
        return TRUE;
    }
    if (AnscEqualString(ParamName, "ServerAddress", TRUE))
    {
        _ansc_snprintf(pDDNSServer->ServerAddress, sizeof(pDDNSServer->ServerAddress), "%s", strValue);
        return TRUE;
    }
    if (AnscEqualString(ParamName, "Protocol", TRUE))
    {
        _ansc_snprintf(pDDNSServer->Protocol, sizeof(pDDNSServer->Protocol), "%s", strValue);
        return TRUE;
    }
    return FALSE;
#endif
#if !defined(DDNS_BROADBANDFORUM)
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(strValue);
    return TRUE;
#endif
}

BOOL
DDNSServer_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
#if defined(DDNS_BROADBANDFORUM)
    PCOSA_CONTEXT_LINK_OBJECT    pLinkObj      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_SERVER         *pServerEntry = (COSA_DML_DDNS_SERVER *)pLinkObj->hContext;

    if( AnscEqualString(ParamName, "CheckInterval", TRUE))
    {
        pServerEntry->CheckInterval = uValue;
        return TRUE;
    }
    if( AnscEqualString(ParamName, "RetryInterval", TRUE))
    {
        pServerEntry->RetryInterval = uValue;
        return TRUE;
    }
    if( AnscEqualString(ParamName, "MaxRetries", TRUE))
    {
        pServerEntry->MaxRetries = uValue;
        return TRUE;
    }

    return FALSE;
#endif
#if !defined(DDNS_BROADBANDFORUM)
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(uValue); 
   return TRUE;
#endif
}

BOOL
DDNSServer_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
#if defined(DDNS_BROADBANDFORUM)
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_SERVER           *pDDNSServer = (COSA_DML_DDNS_SERVER*)pLinkObj->hContext;
    char validateBuf[1024] = {0};

    if(pDDNSServer->Enable && !CosaDmlDynamicDns_GetEnable())
    {
        _ansc_strcpy(pReturnParamName, "Enable");
        return FALSE;
    }

    /* ServiceName - The value MUST be a member of the list reported by the Device.DynamicDNS.SupportedServices parameter. */
    if (!(CosaDmlDynamicDns_GetsupportedServices(validateBuf)) &&
        (!strstr(validateBuf, pDDNSServer->ServiceName)))
    {
         _ansc_strcpy(pReturnParamName, "ServiceName");
         return FALSE;
    }

    /* Protocol - The value MUST be a member of the list reported by the SupportedProtocols parameter.  */
    if (!strstr(pDDNSServer->SupportedProtocols, pDDNSServer->Protocol))
    {
         _ansc_strcpy(pReturnParamName, "Protocol");
         return FALSE;
    }

    return TRUE;
#endif
#if !defined(DDNS_BROADBANDFORUM)
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pReturnParamName);
    UNREFERENCED_PARAMETER(puLength);
   return TRUE;
#endif
}

ULONG
DDNSServer_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
#if defined(DDNS_BROADBANDFORUM)
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_SERVER           *pDDNSServer = (COSA_DML_DDNS_SERVER*)pLinkObj->hContext;
    PCOSA_DATAMODEL_TR181_DDNS       pMyObject    = (PCOSA_DATAMODEL_TR181_DDNS)g_pCosaBEManager->hDynamicDns;

    if (pLinkObj->bNew)
    {
        if (CosaDmlDynamicDns_Server_AddEntry(pDDNSServer) != ANSC_STATUS_SUCCESS)
        {
            return -1;
        }

        CosaDynamicDns_ServerDelInfo((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pLinkObj);
        pLinkObj->bNew = FALSE;
    }
   else
    {
        if (CosaDmlDynamicDns_Server_SetConf(pDDNSServer->InstanceNumber, pDDNSServer) != ANSC_STATUS_SUCCESS)
        {
            CosaDmlDynamicDns_Server_GetConf(pDDNSServer->InstanceNumber, pDDNSServer);
            return -1;
        }
    }
    return 0;
#endif
#if !defined(DDNS_BROADBANDFORUM)
    UNREFERENCED_PARAMETER(hInsContext);
    return 0;
#endif
}

ULONG
DDNSServer_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
#if defined(DDNS_BROADBANDFORUM)
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_SERVER           *pDDNSServer = (COSA_DML_DDNS_SERVER *)pLinkObj->hContext;

    if (CosaDmlDynamicDns_Server_GetConf(pDDNSServer->InstanceNumber, pDDNSServer) != ANSC_STATUS_SUCCESS)
    {
        return -1;
    }
    return 0;
#endif
#if !defined(DDNS_BROADBANDFORUM)
    UNREFERENCED_PARAMETER(hInsContext); 
    return 0;
#endif
}


