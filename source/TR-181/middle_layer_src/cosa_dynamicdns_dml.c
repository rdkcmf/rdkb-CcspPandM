/*
* If not stated otherwise in this file or this component's LICENSE file the
* following copyright and licenses apply:
*
* Copyright 2021 RDK Management
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

#if defined(DDNS_BROADBANDFORUM)
#include "ansc_platform.h"
#include "cosa_dynamicdns_dml.h"
#include "cosa_dynamicdns_internal.h"
#include "cosa_dynamicdns_apis.h"
#include "safec_lib_common.h"

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
    if (strcmp(ParamName, "X_RDK-COM_Enable") == 0)
    {
        *pBool = CosaDmlDynamicDns_GetEnable();
        return TRUE;
    }
    return FALSE;
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
    char supportedServices[1024] = {0};
    errno_t rc = -1;
    if (strcmp(ParamName, "SupportedServices") == 0)
    {
        if (!CosaDmlDynamicDns_GetsupportedServices(supportedServices))
        {
            rc =strcpy_s(pValue, *puLong, supportedServices);
            if(rc != EOK)
            {
               ERR_CHK(rc);
               return -1;
            }
            return 0;
        }
    }
    return -1;
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
    if (strcmp(ParamName, "X_RDK-COM_Enable") == 0)
    {
        if (!CosaDmlDynamicDns_SetEnable(bValue))
        {
            return TRUE;
        }
    }
    return FALSE;
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
    COSA_DATAMODEL_TR181_DDNS *pDynamicDns = (COSA_DATAMODEL_TR181_DDNS *)g_pCosaBEManager->hDynamicDns;

    return AnscSListQueryDepth(&pDynamicDns->DDNSClientList);

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

}

ANSC_HANDLE
DDNSClient_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    COSA_DATAMODEL_TR181_DDNS *pDynamicDns     = (COSA_DATAMODEL_TR181_DDNS *)g_pCosaBEManager->hDynamicDns;
    PCOSA_CONTEXT_LINK_OBJECT  pLinkObj  = NULL;
    COSA_DML_DDNS_CLIENT  *pClientEntry  = NULL;
    errno_t               rc             = -1;

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
    rc = sprintf_s(pClientEntry->Alias, sizeof(pClientEntry->Alias), "cpe-ddns-client-%d", pLinkObj->InstanceNumber);
    if(rc < EOK)
    {
      ERR_CHK(rc);
      AnscFreeMemory(pLinkObj);
      AnscFreeMemory(pClientEntry);
      return NULL;
    }
    pLinkObj->hContext      = (ANSC_HANDLE)pClientEntry;
    pLinkObj->hParentTable  = NULL;
    pLinkObj->bNew          = TRUE;

    CosaSListPushEntryByInsNum((PSLIST_HEADER)&pDynamicDns->DDNSClientList, pLinkObj);
    CosaDynamicDns_ClientAddInfo((ANSC_HANDLE)pDynamicDns, (ANSC_HANDLE)pLinkObj);

    *pInsNumber = pLinkObj->InstanceNumber;
    return pLinkObj;
}

ULONG
DDNSClient_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
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
}

BOOL
DDNSClient_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{

    PCOSA_CONTEXT_LINK_OBJECT    pLinkObj      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_CLIENT         *pClientEntry = (COSA_DML_DDNS_CLIENT *)pLinkObj->hContext;

    CosaDmlDynamicDns_Client_GetConf(pClientEntry->InstanceNumber,pClientEntry);
    if (strcmp(ParamName, "Enable") == 0)
    {
        *pBool = pClientEntry->Enable;
        return TRUE;
    }
    return FALSE;
}
BOOL
DDNSClient_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT    pLinkObj      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_CLIENT         *pClientEntry = (COSA_DML_DDNS_CLIENT *)pLinkObj->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Status") == 0)
    {
        /* collect value */
        if(pClientEntry->Enable)
            *puLong = CosaDmlDynamicDns_GetClientStatus();
        else
            *puLong = CLIENT_DISABLED;
        return TRUE;
    }
    if (strcmp(ParamName, "LastError") == 0)
    {
        /* collect value */
        if(pClientEntry->Enable)
            *puLong = CosaDmlDynamicDns_GetClientLastError();
        else
            *puLong = CLIENT_LAST_NO_ERROR;
        return TRUE;
    }

    return FALSE;
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
    PCOSA_CONTEXT_LINK_OBJECT    pLinkObj      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_CLIENT         *pClientEntry = (COSA_DML_DDNS_CLIENT *)pLinkObj->hContext;
    errno_t                      rc            = -1;
    if (strcmp(ParamName, "Alias") == 0)
    {
        rc =strcpy_s(pValue, *pUlSize, pClientEntry->Alias);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    if (strcmp(ParamName, "Username") == 0)
    {
        rc =strcpy_s(pValue, *pUlSize,  pClientEntry->Username);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    if (strcmp(ParamName, "Password") == 0)
    {
        pValue[0] = '\0';
        return 0;
    }
    if (strcmp(ParamName, "Server") == 0)
    {
        rc =strcpy_s(pValue, *pUlSize, pClientEntry->Server);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    if (strcmp(ParamName, "Interface") == 0)
    {
        rc =strcpy_s(pValue, *pUlSize, "Device.IP.Interface.1");
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    return -1;
}

BOOL
DDNSClient_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT    pLinkObj      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_CLIENT         *pClientEntry = (COSA_DML_DDNS_CLIENT *)pLinkObj->hContext;

    if (strcmp(ParamName, "Enable") == 0)
    {
        pClientEntry->Enable = bValue;
        return TRUE;
    }
    return FALSE;

}

BOOL
DDNSClient_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT    pLinkObj      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_CLIENT         *pClientEntry = (COSA_DML_DDNS_CLIENT *)pLinkObj->hContext;

    if (strcmp(ParamName, "Interface") == 0)
    {
        _ansc_snprintf(pClientEntry->Interface, sizeof(pClientEntry->Interface), "%s", strValue);
        return TRUE;
    }
    if (strcmp(ParamName, "Alias") == 0)
    {
        _ansc_snprintf(pClientEntry->Alias, sizeof(pClientEntry->Alias), "%s", strValue);
        return TRUE;
    }
    if (strcmp(ParamName, "Username") == 0)
    {
        _ansc_snprintf(pClientEntry->Username, sizeof(pClientEntry->Username), "%s", strValue);
        return TRUE;
    }
    if (strcmp(ParamName, "Password") == 0)
    {
        _ansc_snprintf(pClientEntry->Password, sizeof(pClientEntry->Password), "%s", strValue);
        return TRUE;
    }
    if (strcmp(ParamName, "Server") == 0)
    {
        _ansc_snprintf(pClientEntry->Server, sizeof(pClientEntry->Server), "%s", strValue);
        return TRUE;
    }
    return FALSE;
}

BOOL
DDNSClient_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_CONTEXT_LINK_OBJECT    pLinkObj      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_CLIENT         *pClientEntry = (COSA_DML_DDNS_CLIENT *)pLinkObj->hContext;
    PCOSA_DATAMODEL_TR181_DDNS   pDynamicDns   = (PCOSA_DATAMODEL_TR181_DDNS)g_pCosaBEManager->hDynamicDns;
    int validate;
    errno_t                      rc            = -1;

    if (!(1 == sscanf(pClientEntry->Server, "Device.DynamicDNS.Server.%d", &validate) &&
        (validate <= AnscSListQueryDepth(&pDynamicDns->DDNSServerList))))
    {
        rc = STRCPY_S_NOCLOBBER(pReturnParamName, *puLength,"Server");
        if(rc != EOK)
        {
          ERR_CHK(rc);
          return FALSE;
        }
    }
    if(pClientEntry->Enable && !CosaDmlDynamicDns_GetEnable())
    {
        rc = STRCPY_S_NOCLOBBER(pReturnParamName, *puLength,"Enable");
        if(rc != EOK)
        {
          ERR_CHK(rc);
          return FALSE;
        }
    }

    return TRUE;
}

ULONG
DDNSClient_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
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
}

ULONG
DDNSClient_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT    pLinkObj      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_CLIENT         *pClientEntry = (COSA_DML_DDNS_CLIENT *)pLinkObj->hContext;

    if (CosaDmlDynamicDns_Client_GetConf(pClientEntry->InstanceNumber, pClientEntry) == ANSC_STATUS_SUCCESS)
    {
        return 0;
    }
    return -1;
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
    COSA_DATAMODEL_TR181_DDNS *pDynamicDns = (COSA_DATAMODEL_TR181_DDNS *)g_pCosaBEManager->hDynamicDns;

    return AnscSListQueryDepth(&pDynamicDns->DDNSHostList); //Hard-coded to 1,as maximum instance will be always 1
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
}

ANSC_HANDLE
DDNSHostname_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    COSA_DATAMODEL_TR181_DDNS *pDynamicDns     = (COSA_DATAMODEL_TR181_DDNS *)g_pCosaBEManager->hDynamicDns;
    PCOSA_CONTEXT_LINK_OBJECT  pLinkObj  = NULL;
    COSA_DML_DDNS_HOST  *pHostEntry      = NULL;
    errno_t             rc               = -1;

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
    rc = sprintf_s(pHostEntry->Alias, sizeof(pHostEntry->Alias),"cpe-ddns-host-%d", pLinkObj->InstanceNumber);
    if(rc < EOK)
    {
      ERR_CHK(rc);
      AnscFreeMemory(pLinkObj);
      AnscFreeMemory(pHostEntry);
      return NULL;
    }
    pHostEntry->Status      = 5;
    pLinkObj->hContext      = (ANSC_HANDLE)pHostEntry;
    pLinkObj->hParentTable  = NULL;
    pLinkObj->bNew          = TRUE;

    CosaSListPushEntryByInsNum((PSLIST_HEADER)&pDynamicDns->DDNSHostList, pLinkObj);
    CosaDynamicDns_HostAddInfo((ANSC_HANDLE)pDynamicDns, (ANSC_HANDLE)pLinkObj);

    *pInsNumber = pLinkObj->InstanceNumber;
    return  pLinkObj;
}

ULONG
DDNSHostname_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
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
}

BOOL
DDNSHostname_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT    pLinkObj      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_HOST           *pHostEntry   = (COSA_DML_DDNS_HOST *)pLinkObj->hContext;
    CosaDmlDynamicDns_Host_GetConf(pHostEntry->InstanceNumber,pHostEntry);

    if (strcmp(ParamName, "Enable") == 0)
    {
        *pBool = pHostEntry->Enable;
        return TRUE;
    }
    return FALSE;
}

BOOL
DDNSHostname_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT    pLinkObj      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_HOST           *pHostEntry   = (COSA_DML_DDNS_HOST *)pLinkObj->hContext;
    CosaDmlDynamicDns_Host_GetConf(pHostEntry->InstanceNumber,pHostEntry);

    if (strcmp(ParamName, "Status") == 0)
    {
        /* collect value */
        if(pHostEntry->Enable)
            *puLong = pHostEntry->Status;
        else
            *puLong = HOST_DISABLED;
        return TRUE;
    }

    return FALSE;
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
    PCOSA_CONTEXT_LINK_OBJECT    pLinkObj        = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_HOST           *pHostEntry     = (COSA_DML_DDNS_HOST *)pLinkObj->hContext;
    errno_t                      rc              = -1;
    CosaDmlDynamicDns_Host_GetConf(pHostEntry->InstanceNumber,pHostEntry);

    if (strcmp(ParamName, "Name") == 0)
    {
        rc =strcpy_s(pValue, *pUlSize, pHostEntry->Name);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    if (strcmp(ParamName, "LastUpdate") == 0)
    {
        //Need to check the date time format
        CosaDmlDynamicDns_GetHostLastUpdate(pHostEntry->LastUpdate);
        rc =strcpy_s(pValue, *pUlSize, pHostEntry->LastUpdate);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }

    return -1;
}

BOOL
DDNSHostname_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT    pLinkObj      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_HOST           *pHostEntry   = (COSA_DML_DDNS_HOST *)pLinkObj->hContext;

    if (strcmp(ParamName, "Enable") == 0)
    {
        pHostEntry->Enable = bValue;
        return TRUE;
    }
    return FALSE;
}

BOOL
DDNSHostname_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT    pLinkObj      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_HOST           *pHostEntry   = (COSA_DML_DDNS_HOST *)pLinkObj->hContext;

    if (strcmp(ParamName, "Name") == 0)
    {
        _ansc_snprintf(pHostEntry->Name, sizeof(pHostEntry->Name), "%s", strValue);
        return TRUE;
    }

    return FALSE;
}


BOOL
DDNSHostname_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_CONTEXT_LINK_OBJECT    pLinkObj      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_HOST           *pHostEntry   = (COSA_DML_DDNS_HOST *)pLinkObj->hContext;
    errno_t                      rc            = -1;

    if(pHostEntry->Enable && !CosaDmlDynamicDns_GetEnable())
    {
        rc = STRCPY_S_NOCLOBBER(pReturnParamName, *puLength, "Enable");
        if(rc != EOK)
        {
          ERR_CHK(rc);
          return FALSE;
        }
    }

    return TRUE;
}

ULONG
DDNSHostname_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
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
}

ULONG
DDNSHostname_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT    pLinkObj      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_HOST           *pHostEntry   = (COSA_DML_DDNS_HOST *)pLinkObj->hContext;

    if (CosaDmlDynamicDns_Host_GetConf(pHostEntry->InstanceNumber, pHostEntry) == ANSC_STATUS_SUCCESS)
    {
        return 0;
    }
    return -1;
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
    PCOSA_DATAMODEL_TR181_DDNS             pMyObject         = (PCOSA_DATAMODEL_TR181_DDNS)g_pCosaBEManager->hDynamicDns;

    return AnscSListQueryDepth(&pMyObject->DDNSServerList);
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
}

ANSC_HANDLE
DDNSServer_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    COSA_DATAMODEL_TR181_DDNS        *pMyObject         = (COSA_DATAMODEL_TR181_DDNS *)g_pCosaBEManager->hDynamicDns;
    PCOSA_CONTEXT_LINK_OBJECT      pLinkObj           = NULL;
    COSA_DML_DDNS_SERVER           *pDDNSServer       = NULL;
    errno_t                        rc                 = -1;

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

    rc = sprintf_s(pDDNSServer->Alias, sizeof(pDDNSServer->Alias),"cpe-ddns-server-%d", (int)pLinkObj->InstanceNumber);
    if(rc < EOK)
    {
      ERR_CHK(rc);
      AnscFreeMemory(pLinkObj);
      AnscFreeMemory(pDDNSServer);
      return NULL;
    }
    pLinkObj->hContext      = (ANSC_HANDLE)pDDNSServer;
    pLinkObj->hParentTable  = NULL;
    pLinkObj->bNew          = TRUE;

    CosaSListPushEntryByInsNum((PSLIST_HEADER)&pMyObject->DDNSServerList, pLinkObj);
    CosaDynamicDns_ServerAddInfo((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pLinkObj);

    *pInsNumber = pLinkObj->InstanceNumber;
    return pLinkObj;
}

ULONG
DDNSServer_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
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
}

BOOL
DDNSServer_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT     pLinkObj     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_SERVER          *pDDNSServer = (COSA_DML_DDNS_SERVER*)pLinkObj->hContext;

    CosaDmlDynamicDns_Server_GetConf(pDDNSServer->InstanceNumber,pDDNSServer);

    if (strcmp(ParamName, "Enable") == 0)
    {
        *pBool = pDDNSServer->Enable;
        return TRUE;
    }
    return FALSE;
}

BOOL
DDNSServer_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{

    PCOSA_CONTEXT_LINK_OBJECT    pLinkObj      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_SERVER         *pServerEntry = (COSA_DML_DDNS_SERVER *)pLinkObj->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "CheckInterval") == 0)
    {
        /* collect value */
        *puLong = pServerEntry->CheckInterval;
        return TRUE;
    }
    if (strcmp(ParamName, "RetryInterval") == 0)
    {
        /* collect value */
        *puLong = pServerEntry->RetryInterval;
        return TRUE;
    }
    if (strcmp(ParamName, "MaxRetries") == 0)
    {
        /* collect value */
        *puLong = pServerEntry->MaxRetries;
        return TRUE;
    }

    return FALSE;
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
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_SERVER            *pDDNSServer = (COSA_DML_DDNS_SERVER*)pLinkObj->hContext;
    errno_t                         rc           = -1;
    if (strcmp(ParamName, "Name") == 0)
    {
        rc =strcpy_s(pValue, *pUlSize, pDDNSServer->Name);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    if (strcmp(ParamName, "Alias") == 0)
    {
        rc =strcpy_s(pValue, *pUlSize, pDDNSServer->Alias);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    if (strcmp(ParamName, "ServiceName") == 0)
    {
        rc =strcpy_s(pValue, *pUlSize, pDDNSServer->ServiceName);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    if (strcmp(ParamName, "ServerAddress") == 0)
    {
        rc =strcpy_s(pValue, *pUlSize, pDDNSServer->ServerAddress);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    if (strcmp(ParamName, "SupportedProtocols") == 0)
    {
        rc =strcpy_s(pValue, *pUlSize, pDDNSServer->SupportedProtocols);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    if (strcmp(ParamName, "Protocol") == 0)
    {
        rc =strcpy_s(pValue, *pUlSize, pDDNSServer->Protocol);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    return -1;
}

BOOL
DDNSServer_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_SERVER          *pDDNSServer  = (COSA_DML_DDNS_SERVER*)pLinkObj->hContext;

    if (strcmp(ParamName, "Enable") == 0)
    {
        pDDNSServer->Enable = pBool;
        return TRUE;
    }
    return FALSE;
}

BOOL
DDNSServer_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_SERVER           *pDDNSServer = (COSA_DML_DDNS_SERVER*)pLinkObj->hContext;

    if (strcmp(ParamName, "Name") == 0)
    {
        _ansc_snprintf(pDDNSServer->Name, sizeof(pDDNSServer->Name), "%s", strValue);
        return TRUE;
    }
    if (strcmp(ParamName, "Alias") == 0)
    {
        _ansc_snprintf(pDDNSServer->Alias, sizeof(pDDNSServer->Alias), "%s", strValue);
        return TRUE;
    }
    if (strcmp(ParamName, "ServiceName") == 0)
    {
        _ansc_snprintf(pDDNSServer->ServiceName, sizeof(pDDNSServer->ServiceName), "%s", strValue);
        return TRUE;
    }
    if (strcmp(ParamName, "ServerAddress") == 0)
    {
        _ansc_snprintf(pDDNSServer->ServerAddress, sizeof(pDDNSServer->ServerAddress), "%s", strValue);
        return TRUE;
    }
    if (strcmp(ParamName, "Protocol") == 0)
    {
        _ansc_snprintf(pDDNSServer->Protocol, sizeof(pDDNSServer->Protocol), "%s", strValue);
        return TRUE;
    }
    return FALSE;
}

BOOL
DDNSServer_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT    pLinkObj      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_SERVER         *pServerEntry = (COSA_DML_DDNS_SERVER *)pLinkObj->hContext;

    if (strcmp(ParamName, "CheckInterval") == 0)
    {
        pServerEntry->CheckInterval = uValue;
        return TRUE;
    }
    if (strcmp(ParamName, "RetryInterval") == 0)
    {
        pServerEntry->RetryInterval = uValue;
        return TRUE;
    }
    if (strcmp(ParamName, "MaxRetries") == 0)
    {
        pServerEntry->MaxRetries = uValue;
        return TRUE;
    }

    return FALSE;
}

BOOL
DDNSServer_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_SERVER           *pDDNSServer = (COSA_DML_DDNS_SERVER*)pLinkObj->hContext;
    char validateBuf[1024] = {0};
    errno_t rc             = -1;

    if(pDDNSServer->Enable && !CosaDmlDynamicDns_GetEnable())
    {
        rc = STRCPY_S_NOCLOBBER(pReturnParamName, *puLength,"Enable");
        if(rc != EOK)
        {
          ERR_CHK(rc);
          return FALSE;
        }
    }

    /* ServiceName - The value MUST be a member of the list reported by the Device.DynamicDNS.SupportedServices parameter. */
    if (!(CosaDmlDynamicDns_GetsupportedServices(validateBuf)) &&
        (!strstr(validateBuf, pDDNSServer->ServiceName)))
    {
         rc = STRCPY_S_NOCLOBBER(pReturnParamName, *puLength,"ServiceName");
         if(rc != EOK)
         {
           ERR_CHK(rc);
           return FALSE;
         }
    }

    /* Protocol - The value MUST be a member of the list reported by the SupportedProtocols parameter.  */
    if (!strstr(pDDNSServer->SupportedProtocols, pDDNSServer->Protocol))
    {
         rc = STRCPY_S_NOCLOBBER(pReturnParamName, *puLength,"Protocol");
         if(rc != EOK)
         {
           ERR_CHK(rc);
           return FALSE;
         }
    }

    return TRUE;
}

ULONG
DDNSServer_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
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
}

ULONG
DDNSServer_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_DDNS_SERVER           *pDDNSServer = (COSA_DML_DDNS_SERVER *)pLinkObj->hContext;

    if (CosaDmlDynamicDns_Server_GetConf(pDDNSServer->InstanceNumber, pDDNSServer) != ANSC_STATUS_SUCCESS)
    {
        return -1;
    }
    return 0;
}
#endif

