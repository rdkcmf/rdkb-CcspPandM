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

#include "cosa_x_comcast_com_parentalcontrol_dml.h"
#include "dml_tr181_custom_cfg.h"
//RDKB-EMULATOR
#include "dmsb_tr181_psm_definitions.h"
extern ANSC_HANDLE bus_handle;
extern char g_Subsystem[32];

BOOL
MngSites_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    COSA_DML_MANAGEDSITES           *pMngSites = &pParCtrl->ManagedSites;

    if (strcmp(ParamName, "Enable") == 0)
    {
        *pBool = pMngSites->Enable;
        return TRUE;
    }

    return FALSE;
}

BOOL
MngSites_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    COSA_DML_MANAGEDSITES           *pMngSites = &pParCtrl->ManagedSites;
    BOOL                            pBridgeMode     = FALSE;

    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&pBridgeMode)) && (pBridgeMode == TRUE))
        return FALSE;

    if (strcmp(ParamName, "Enable") == 0)
    {
        pMngSites->Enable = bValue;
        return TRUE;
    }

    return FALSE;

}

BOOL
MngSites_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    return TRUE;
}

ULONG
MngSites_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    COSA_DML_MANAGEDSITES           *pMngSites = &pParCtrl->ManagedSites;

    if (CosaDmlMngSites_SetConf(pMngSites) != ANSC_STATUS_SUCCESS)
    {
        CosaDmlMngSites_GetConf(pMngSites);
        return -1;
    }

    return 0;
}

ULONG
MngSites_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    COSA_DML_MANAGEDSITES           *pMngSites = &pParCtrl->ManagedSites;

    if (CosaDmlMngSites_GetConf(pMngSites) != ANSC_STATUS_SUCCESS)
        return -1;

    return 0;
}

BOOL
MngServs_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    COSA_DML_MANAGED_SERVS           *pMngServs = &pParCtrl->ManagedServs;

    if (strcmp(ParamName, "Enable") == 0)
    {
        *pBool = pMngServs->Enable;
        return TRUE;
    }

    return FALSE;
}

BOOL
MngServs_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    COSA_DML_MANAGED_SERVS           *pMngServs = &pParCtrl->ManagedServs;
    BOOL                            pBridgeMode     = FALSE;

    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&pBridgeMode)) && (pBridgeMode == TRUE))
        return FALSE;

    if (strcmp(ParamName, "Enable") == 0)
    {
        pMngServs->Enable = bValue;
        return TRUE;
    }

    return FALSE;

}

BOOL
MngServs_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    return TRUE;
}

ULONG
MngServs_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    COSA_DML_MANAGED_SERVS           *pMngServs = &pParCtrl->ManagedServs;

    if (CosaDmlMngServs_SetConf(pMngServs) != ANSC_STATUS_SUCCESS)
    {
        CosaDmlMngServs_GetConf(pMngServs);
        return -1;
    }

    return 0;
}

ULONG
MngServs_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    COSA_DML_MANAGED_SERVS           *pMngServs = &pParCtrl->ManagedServs;

    if (CosaDmlMngServs_GetConf(pMngServs) != ANSC_STATUS_SUCCESS)
        return -1;

    return 0;
}

BOOL
MngDevs_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    COSA_DML_MANAGED_DEVS           *pMngDevs = &pParCtrl->ManagedDevs;

    if (strcmp(ParamName, "Enable") == 0)
    {
        *pBool = pMngDevs->Enable;
        return TRUE;
    }
    if (strcmp(ParamName, "AllowAll") == 0)
    {
        *pBool = pMngDevs->AllowAll;
        return TRUE;
    }

    return FALSE;
}

BOOL
MngDevs_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    COSA_DML_MANAGED_DEVS           *pMngDevs = &pParCtrl->ManagedDevs;
    BOOL                            pBridgeMode     = FALSE;

    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&pBridgeMode)) && (pBridgeMode == TRUE))
        return FALSE;

    if (strcmp(ParamName, "Enable") == 0)
    {
        pMngDevs->Enable = bValue;
        return TRUE;
    }
    if (strcmp(ParamName, "AllowAll") == 0)
    {
        pMngDevs->AllowAll = bValue;
        return TRUE;
    }

    return FALSE;

}

BOOL
MngDevs_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    return TRUE;
}

ULONG
MngDevs_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    COSA_DML_MANAGED_DEVS           *pMngDevs = &pParCtrl->ManagedDevs;

    if (CosaDmlMngDevs_SetConf(pMngDevs) != ANSC_STATUS_SUCCESS)
    {
        CosaDmlMngDevs_GetConf(pMngDevs);
        return -1;
    }

    return 0;
}

ULONG
MngDevs_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    COSA_DML_MANAGED_DEVS           *pMngDevs = &pParCtrl->ManagedDevs;

    if (CosaDmlMngDevs_GetConf(pMngDevs) != ANSC_STATUS_SUCCESS)
        return -1;

    return 0;
}

ULONG
PcBlkURL_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    ULONG                           ulCount;

    AnscTraceWarning(("%s -- pParCtrl = 0x%lX\n", __FUNCTION__, pParCtrl));

    ulCount = AnscSListQueryDepth(&pParCtrl->BlkUrlList);

    AnscTraceWarning(("%s -- ulCount = %d...\n", __FUNCTION__, ulCount));

    return ulCount;
}

ANSC_HANDLE
PcBlkURL_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry = NULL;

    AnscTraceWarning(("%s -- nIndex = %d...\n", __FUNCTION__, nIndex));

    pSLinkEntry = AnscQueueGetEntryByIndex((ANSC_HANDLE)&pParCtrl->BlkUrlList, nIndex);
    
    if (pSLinkEntry)
    {
        pLinkObj = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        *pInsNumber = pLinkObj->InstanceNumber;
    }
    
    return pLinkObj;
}

ANSC_HANDLE
PcBlkURL_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = NULL;
    COSA_DML_BLOCKEDURL             *pBlkUrl    = NULL;

    AnscTraceWarning(("%s...\n", __FUNCTION__));

    pLinkObj = AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
    if (!pLinkObj)
        return NULL;
    
    pBlkUrl = AnscAllocateMemory(sizeof(COSA_DML_BLOCKEDURL));
    if (!pBlkUrl)
    {
        AnscFreeMemory(pLinkObj);
        return NULL;
    }

    pLinkObj->InstanceNumber = pParCtrl->ulBlkUrlNextInsNum;
    pBlkUrl->InstanceNumber = pParCtrl->ulBlkUrlNextInsNum;
    pParCtrl->ulBlkUrlNextInsNum++;
    if (pParCtrl->ulBlkUrlNextInsNum == 0)
        pParCtrl->ulBlkUrlNextInsNum = 1;

    _ansc_sprintf(pBlkUrl->Alias, "cpe-BlockedURL-%d", (int)pLinkObj->InstanceNumber);
    pLinkObj->hContext      = (ANSC_HANDLE)pBlkUrl;
    pLinkObj->hParentTable  = NULL;
    pLinkObj->bNew          = TRUE;
#if 1
//RDKB-EMULATOR
     char param_value[256] = {0};
     char param_name[256]  = {0};
     sprintf(param_value,"%ld",pBlkUrl->InstanceNumber);
     sprintf(param_name,PSM_PC_MS_INSTANCENUMBER);
     PSM_Set_Record_Value2(bus_handle,g_Subsystem, param_name, ccsp_string, param_value);
#endif

    CosaSListPushEntryByInsNum((PSLIST_HEADER)&pParCtrl->BlkUrlList, pLinkObj);
    CosaPcReg_BlkUrlAddInfo((ANSC_HANDLE)pParCtrl, (ANSC_HANDLE)pLinkObj);

    *pInsNumber = pLinkObj->InstanceNumber;

    return pLinkObj;
}

ULONG
PcBlkURL_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    COSA_DML_BLOCKEDURL             *pBlkUrl    = (COSA_DML_BLOCKEDURL*)pLinkObj->hContext;

    AnscTraceWarning(("%s...\n", __FUNCTION__));

    if (CosaDmlBlkURL_DelEntry(pLinkObj->InstanceNumber) != ANSC_STATUS_SUCCESS)
        return -1;

    AnscSListPopEntryByLink((PSLIST_HEADER)&pParCtrl->BlkUrlList, &pLinkObj->Linkage);
    if (pLinkObj->bNew)
        CosaPcReg_BlkUrlDelInfo((ANSC_HANDLE)pParCtrl, (ANSC_HANDLE)pLinkObj);

    AnscFreeMemory(pBlkUrl);
    AnscFreeMemory(pLinkObj);

    return 0;
}

BOOL
PcBlkURL_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_BLOCKEDURL             *pBlkUrl    = (COSA_DML_BLOCKEDURL*)pLinkObj->hContext;

    AnscTraceWarning(("%s -- param name = %s...\n", __FUNCTION__, ParamName));

    if (strcmp(ParamName, "AlwaysBlock") == 0)
    {
        *pBool = pBlkUrl->AlwaysBlock;
        return TRUE;
    }

    return FALSE;
}

BOOL
PcBlkURL_GetParamUlongValue(
        ANSC_HANDLE hInsContext, 
        char *ParamName, 
        ULONG *pUlong)
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_BLOCKEDURL             *pBlkUrl    = (COSA_DML_BLOCKEDURL*)pLinkObj->hContext;

    AnscTraceWarning(("%s -- param name = %s...\n", __FUNCTION__, ParamName));

    if (strcmp(ParamName, "BlockMethod") == 0)
    {
        *pUlong = pBlkUrl->BlockMethod;
        return TRUE;
    }

    return FALSE;
}

ULONG
PcBlkURL_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_BLOCKEDURL             *pBlkUrl    = (COSA_DML_BLOCKEDURL*)pLinkObj->hContext;

    AnscTraceWarning(("%s -- param name = %s...\n", __FUNCTION__, ParamName));

    if (strcmp(ParamName, "Site") == 0)
    {
        AnscCopyString(pValue, pBlkUrl->Site);
        return 0;
    }
    if (strcmp(ParamName, "StartTime") == 0)
    {
        AnscCopyString(pValue, pBlkUrl->StartTime);
        return 0;
    }
    if (strcmp(ParamName, "EndTime") == 0)
    {
        AnscCopyString(pValue, pBlkUrl->EndTime);
        return 0;
    }
    if (strcmp(ParamName, "BlockDays") == 0)
    {
        AnscCopyString(pValue, pBlkUrl->BlockDays);
        return 0;
    }
#ifdef CONFIG_CISCO_FEATURE_CISCOCONNECT
    if (strcmp(ParamName, "MAC") == 0)
    {
        AnscCopyString(pValue, pBlkUrl->MAC);
        return 0;
    }
    if (strcmp(ParamName, "DeviceName") == 0)
    {
        AnscCopyString(pValue, pBlkUrl->DeviceName);
        return 0;
    }
#endif
    return -1;
}

BOOL
PcBlkURL_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_BLOCKEDURL             *pBlkUrl    = (COSA_DML_BLOCKEDURL*)pLinkObj->hContext;
    BOOL                            pBridgeMode     = FALSE;

    AnscTraceWarning(("%s -- param name = %s...\n", __FUNCTION__, ParamName));

    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&pBridgeMode)) && (pBridgeMode == TRUE))
        return FALSE;

    if (strcmp(ParamName, "AlwaysBlock") == 0)
    {
        pBlkUrl->AlwaysBlock = bValue;
        return TRUE;
    }

    return FALSE;
}

BOOL
PcBlkURL_SetParamUlongValue(
        ANSC_HANDLE hInsContext,
        char        *ParamName,
        ULONG       ulValue)
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_BLOCKEDURL             *pBlkUrl    = (COSA_DML_BLOCKEDURL*)pLinkObj->hContext;
    BOOL                            pBridgeMode     = FALSE;

    AnscTraceWarning(("%s -- param name = %s...\n", __FUNCTION__, ParamName));

    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&pBridgeMode)) && (pBridgeMode == TRUE))
        return FALSE;

    if (strcmp(ParamName, "BlockMethod") == 0)
    {
        pBlkUrl->BlockMethod = ulValue;
        return TRUE;
    }

    return FALSE;
}

BOOL
PcBlkURL_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_BLOCKEDURL             *pBlkUrl    = (COSA_DML_BLOCKEDURL*)pLinkObj->hContext;
    BOOL                            pBridgeMode     = FALSE;

    AnscTraceWarning(("%s -- param name = %s...\n", __FUNCTION__, ParamName));

    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&pBridgeMode)) && (pBridgeMode == TRUE))
        return FALSE;

    if (strcmp(ParamName, "Site") == 0)
    {
        _ansc_snprintf(pBlkUrl->Site, sizeof(pBlkUrl->Site), "%s", strValue);
        return TRUE;
    }
    if (strcmp(ParamName, "StartTime") == 0)
    {
        _ansc_snprintf(pBlkUrl->StartTime, sizeof(pBlkUrl->StartTime), "%s", strValue);
        pBlkUrl->StartTimeFlg = TRUE;
        return TRUE;
    }
    if (strcmp(ParamName, "EndTime") == 0)
    {
        _ansc_snprintf(pBlkUrl->EndTime, sizeof(pBlkUrl->EndTime), "%s", strValue);
        pBlkUrl->EndTimeFlg = TRUE;
        return TRUE;
    }
    if (strcmp(ParamName, "BlockDays") == 0)
    {
        _ansc_snprintf(pBlkUrl->BlockDays, sizeof(pBlkUrl->BlockDays), "%s", strValue);
        return TRUE;
    }
#ifdef CONFIG_CISCO_FEATURE_CISCOCONNECT
    if (strcmp(ParamName, "MAC") == 0)
    {
        int len = strlen(strValue);
        for(;len > 0; len--) {
            strValue[len-1] = tolower(strValue[len-1]);
        }
        _ansc_snprintf(pBlkUrl->MAC, sizeof(pBlkUrl->MAC), "%s", strValue);
        return TRUE;
    }
    if (strcmp(ParamName, "DeviceName") == 0)
    {
        _ansc_snprintf(pBlkUrl->DeviceName, sizeof(pBlkUrl->DeviceName), "%s", strValue);
        return TRUE;
    }
#endif
    return FALSE;
}

BOOL
PcBlkURL_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_BLOCKEDURL             *pBlkUrl    = (COSA_DML_BLOCKEDURL*)pLinkObj->hContext;
#if defined(CONFIG_CISCO_CCSP_PRODUCT_ARES) || defined(CONFIG_CISCO_CCSP_PRODUCT_XB3)  
    if(!CosaDmlMngSites_Chktime(pBlkUrl))
        return FALSE;
#endif
    return TRUE;
}

ULONG
PcBlkURL_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_BLOCKEDURL             *pBlkUrl    = (COSA_DML_BLOCKEDURL*)pLinkObj->hContext;
    PCOSA_DATAMODEL_PARENTALCONTROL pParCtrl    = (PCOSA_DATAMODEL_PARENTALCONTROL)g_pCosaBEManager->hParentalControl;

    AnscTraceWarning(("%s...\n", __FUNCTION__));

    if (pLinkObj->bNew)
    {
        if (CosaDmlBlkURL_AddEntry(pBlkUrl) != ANSC_STATUS_SUCCESS)
            return -1;
        CosaPcReg_BlkUrlDelInfo((ANSC_HANDLE)pParCtrl, (ANSC_HANDLE)pLinkObj);
        pLinkObj->bNew = FALSE;
    }
    else
    {
        if (CosaDmlBlkURL_SetConf(pBlkUrl->InstanceNumber, pBlkUrl) != ANSC_STATUS_SUCCESS)
        {
            CosaDmlBlkURL_GetConf(pBlkUrl->InstanceNumber, pBlkUrl);
            return -1;
        }
    }

    return 0;
}

ULONG
PcBlkURL_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_BLOCKEDURL             *pBlkUrl    = (COSA_DML_BLOCKEDURL*)pLinkObj->hContext;

    if (CosaDmlBlkURL_GetConf(pBlkUrl->InstanceNumber, pBlkUrl) != ANSC_STATUS_SUCCESS)
        return -1;

    return 0;
}

ULONG
PcTrustedUser_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;

    AnscTraceWarning(("%s...\n", __FUNCTION__));

    return AnscSListQueryDepth(&pParCtrl->TrustedUserList);
}

ANSC_HANDLE
PcTrustedUser_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry = NULL;

    pSLinkEntry = AnscQueueGetEntryByIndex((ANSC_HANDLE)&pParCtrl->TrustedUserList, nIndex);
    
    if (pSLinkEntry)
    {
        pLinkObj = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        *pInsNumber = pLinkObj->InstanceNumber;
    }
    
    return pLinkObj;
}

ANSC_HANDLE
PcTrustedUser_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = NULL;
    COSA_DML_TRUSTEDUSER             *pTrustedUser    = NULL;

    pLinkObj = AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
    if (!pLinkObj)
        return NULL;
    
    pTrustedUser = AnscAllocateMemory(sizeof(COSA_DML_TRUSTEDUSER));
    if (!pTrustedUser)
    {
        AnscFreeMemory(pLinkObj);
        return NULL;
    }

    pLinkObj->InstanceNumber = pParCtrl->ulTrustedUserNextInsNum;
    pTrustedUser->InstanceNumber = pParCtrl->ulTrustedUserNextInsNum;
    pParCtrl->ulTrustedUserNextInsNum++;
    if (pParCtrl->ulTrustedUserNextInsNum == 0)
        pParCtrl->ulTrustedUserNextInsNum = 1;

    _ansc_sprintf(pTrustedUser->Alias, "cpe-TrustedUser-%d", (int)pLinkObj->InstanceNumber);
    pLinkObj->hContext      = (ANSC_HANDLE)pTrustedUser;
    pLinkObj->hParentTable  = NULL;
    pLinkObj->bNew          = TRUE;
#if 1
//RDKB-EMULATOR
     char param_value[256] = {0};
     char param_name[256]  = {0};
     sprintf(param_value,"%ld",pTrustedUser->InstanceNumber);
     sprintf(param_name,PSM_PC_MS_TRUSTED_INSTANCENUMBER);
     PSM_Set_Record_Value2(bus_handle,g_Subsystem, param_name, ccsp_string, param_value);
#endif

    CosaSListPushEntryByInsNum((PSLIST_HEADER)&pParCtrl->TrustedUserList, pLinkObj);
    CosaPcReg_TrustedUserAddInfo((ANSC_HANDLE)pParCtrl, (ANSC_HANDLE)pLinkObj);

    *pInsNumber = pLinkObj->InstanceNumber;

    return pLinkObj;
}

ULONG
PcTrustedUser_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    COSA_DML_TRUSTEDUSER             *pTrustedUser    = (COSA_DML_TRUSTEDUSER*)pLinkObj->hContext;

    if (CosaDmlTrustedUser_DelEntry(pLinkObj->InstanceNumber) != ANSC_STATUS_SUCCESS)
        return -1;

    AnscSListPopEntryByLink((PSLIST_HEADER)&pParCtrl->TrustedUserList, &pLinkObj->Linkage);
    if (pLinkObj->bNew)
        CosaPcReg_TrustedUserDelInfo((ANSC_HANDLE)pParCtrl, (ANSC_HANDLE)pLinkObj);

    AnscFreeMemory(pTrustedUser);
    AnscFreeMemory(pLinkObj);

    return 0;
}

BOOL
PcTrustedUser_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_TRUSTEDUSER             *pTrustedUser    = (COSA_DML_TRUSTEDUSER*)pLinkObj->hContext;

    if (strcmp(ParamName, "Trusted") == 0)
    {
        *pBool = pTrustedUser->Trusted;
        return TRUE;
    }

    return FALSE;
}

ULONG
PcTrustedUser_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_TRUSTEDUSER             *pTrustedUser    = (COSA_DML_TRUSTEDUSER*)pLinkObj->hContext;

    if (strcmp(ParamName, "HostDescription") == 0)
    {
        AnscCopyString(pValue, pTrustedUser->HostDescription);
        return 0;
    }
    if (strcmp(ParamName, "IPAddress") == 0)
    {
        AnscCopyString(pValue, pTrustedUser->IPAddress);
        return 0;
    }

    return -1;
}

BOOL
PcTrustedUser_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_TRUSTEDUSER             *pTrustedUser    = (COSA_DML_TRUSTEDUSER*)pLinkObj->hContext;
    BOOL                            pBridgeMode     = FALSE;

    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&pBridgeMode)) && (pBridgeMode == TRUE))
        return FALSE;

    if (strcmp(ParamName, "Trusted") == 0)
    {
        pTrustedUser->Trusted = bValue;
        return TRUE;
    }

    return FALSE;
}

BOOL
PcTrustedUser_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_TRUSTEDUSER             *pTrustedUser    = (COSA_DML_TRUSTEDUSER*)pLinkObj->hContext;
    BOOL                            pBridgeMode     = FALSE;

    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&pBridgeMode)) && (pBridgeMode == TRUE))
        return FALSE;

    if (strcmp(ParamName, "HostDescription") == 0)
    {
        _ansc_snprintf(pTrustedUser->HostDescription, sizeof(pTrustedUser->HostDescription), "%s", strValue);
        return TRUE;
    }
    if (strcmp(ParamName, "IPAddress") == 0)
    {
        _ansc_snprintf(pTrustedUser->IPAddress, sizeof(pTrustedUser->IPAddress), "%s", strValue);
        return TRUE;
    }

    return FALSE;
}

BOOL
PcTrustedUser_GetParamUlongValue(
        ANSC_HANDLE hInsContext, 
        char *ParamName, 
        ULONG *pUlong)
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_TRUSTEDUSER             *pTrustedUser    = (COSA_DML_TRUSTEDUSER*)pLinkObj->hContext;

    if (strcmp(ParamName, "IPAddressType") == 0)
    {
        *pUlong = pTrustedUser->IPAddressType;
        return TRUE;
    }

    return FALSE;
}

BOOL
PcTrustedUser_SetParamUlongValue(
        ANSC_HANDLE hInsContext,
        char        *ParamName,
        ULONG       ulValue
        )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_TRUSTEDUSER             *pTrustedUser    = (COSA_DML_TRUSTEDUSER*)pLinkObj->hContext;
    BOOL                            pBridgeMode     = FALSE;

    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&pBridgeMode)) && (pBridgeMode == TRUE))
        return FALSE;

    if (strcmp(ParamName, "IPAddressType") == 0)
    {
        pTrustedUser->IPAddressType = ulValue;
        return TRUE;
    }

    return FALSE;
}

BOOL
PcTrustedUser_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    return TRUE;
}

ULONG
PcTrustedUser_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_TRUSTEDUSER             *pTrustedUser    = (COSA_DML_TRUSTEDUSER*)pLinkObj->hContext;
    PCOSA_DATAMODEL_PARENTALCONTROL pParCtrl    = (PCOSA_DATAMODEL_PARENTALCONTROL)g_pCosaBEManager->hParentalControl;

    if (pLinkObj->bNew)
    {
        if (CosaDmlTrustedUser_AddEntry(pTrustedUser) != ANSC_STATUS_SUCCESS)
            return -1;
        CosaPcReg_TrustedUserDelInfo((ANSC_HANDLE)pParCtrl, (ANSC_HANDLE)pLinkObj);
        pLinkObj->bNew = FALSE;
    }
    else
    {
        if (CosaDmlTrustedUser_SetConf(pTrustedUser->InstanceNumber, pTrustedUser) != ANSC_STATUS_SUCCESS)
        {
            CosaDmlTrustedUser_GetConf(pTrustedUser->InstanceNumber, pTrustedUser);
            return -1;
        }
    }

    return 0;
}

ULONG
PcTrustedUser_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_TRUSTEDUSER             *pTrustedUser    = (COSA_DML_TRUSTEDUSER*)pLinkObj->hContext;

    if (CosaDmlTrustedUser_GetConf(pTrustedUser->InstanceNumber, pTrustedUser) != ANSC_STATUS_SUCCESS)
        return -1;

    return 0;
}

ULONG
MSServ_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;

    AnscTraceWarning(("%s...\n", __FUNCTION__));

    return AnscSListQueryDepth(&pParCtrl->MSServList);
}

ANSC_HANDLE
MSServ_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry = NULL;

    pSLinkEntry = AnscQueueGetEntryByIndex((ANSC_HANDLE)&pParCtrl->MSServList, nIndex);
    
    if (pSLinkEntry)
    {
        pLinkObj = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        *pInsNumber = pLinkObj->InstanceNumber;
    }
    
    return pLinkObj;
}

ANSC_HANDLE
MSServ_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = NULL;
    COSA_DML_MS_SERV             *pMSServ    = NULL;

    pLinkObj = AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
    if (!pLinkObj)
        return NULL;
    
    pMSServ = AnscAllocateMemory(sizeof(COSA_DML_MS_SERV));
    if (!pMSServ)
    {
        AnscFreeMemory(pLinkObj);
        return NULL;
    }

    pLinkObj->InstanceNumber = pParCtrl->ulMSServNextInsNum;
    pMSServ->InstanceNumber = pParCtrl->ulMSServNextInsNum;
    pParCtrl->ulMSServNextInsNum++;
    if (pParCtrl->ulMSServNextInsNum == 0)
        pParCtrl->ulMSServNextInsNum = 1;

    _ansc_sprintf(pMSServ->Alias, "cpe-MSService-%d", (int)pLinkObj->InstanceNumber);
    pLinkObj->hContext      = (ANSC_HANDLE)pMSServ;
    pLinkObj->hParentTable  = NULL;
    pLinkObj->bNew          = TRUE;
#if 1
//RDKB-EMULATOR
     char param_value[256] = {0};
     char param_name[256]  = {0};
     sprintf(param_value,"%ld",pMSServ->InstanceNumber);
     sprintf(param_name,PSM_PC_MSE_INSTANCENUMBER);
     PSM_Set_Record_Value2(bus_handle,g_Subsystem, param_name, ccsp_string, param_value);
#endif

    CosaSListPushEntryByInsNum((PSLIST_HEADER)&pParCtrl->MSServList, pLinkObj);
    CosaPcReg_MSServAddInfo((ANSC_HANDLE)pParCtrl, (ANSC_HANDLE)pLinkObj);

    *pInsNumber = pLinkObj->InstanceNumber;

    return pLinkObj;
}

ULONG
MSServ_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    COSA_DML_MS_SERV             *pMSServ    = (COSA_DML_MS_SERV*)pLinkObj->hContext;

    if (CosaDmlMSServ_DelEntry(pLinkObj->InstanceNumber) != ANSC_STATUS_SUCCESS)
        return -1;

    AnscSListPopEntryByLink((PSLIST_HEADER)&pParCtrl->MSServList, &pLinkObj->Linkage);
    if (pLinkObj->bNew)
        CosaPcReg_MSServDelInfo((ANSC_HANDLE)pParCtrl, (ANSC_HANDLE)pLinkObj);

    AnscFreeMemory(pMSServ);
    AnscFreeMemory(pLinkObj);

    return 0;
}

BOOL
MSServ_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_MS_SERV             *pMSServ    = (COSA_DML_MS_SERV*)pLinkObj->hContext;

    if (strcmp(ParamName, "AlwaysBlock") == 0)
    {
        *pBool = pMSServ->AlwaysBlock;
        return TRUE;
    }

    return FALSE;
}

ULONG
MSServ_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_MS_SERV             *pMSServ    = (COSA_DML_MS_SERV*)pLinkObj->hContext;

    if (strcmp(ParamName, "Description") == 0)
    {
        AnscCopyString(pValue, pMSServ->Description);
        return 0;
    }
    if (strcmp(ParamName, "StartTime") == 0)
    {
        AnscCopyString(pValue, pMSServ->StartTime);
        return 0;
    }
    if (strcmp(ParamName, "EndTime") == 0)
    {
        AnscCopyString(pValue, pMSServ->EndTime);
        return 0;
    }
    if (strcmp(ParamName, "BlockDays") == 0)
    {
        AnscCopyString(pValue, pMSServ->BlockDays);
        return 0;
    }

    return -1;
}

BOOL
MSServ_GetParamUlongValue(
        ANSC_HANDLE hInsContext, 
        char *ParamName, 
        ULONG *pUlong)
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_MS_SERV             *pMSServ    = (COSA_DML_MS_SERV*)pLinkObj->hContext;

    if (strcmp(ParamName, "Protocol") == 0)
    {
        *pUlong = pMSServ->Protocol;
        return TRUE;
    }
    if (strcmp(ParamName, "StartPort") == 0)
    {
        *pUlong = pMSServ->StartPort;
        return TRUE;
    }
    if (strcmp(ParamName, "EndPort") == 0)
    {
        *pUlong = pMSServ->EndPort;
        return TRUE;
    }

    return FALSE;
}


BOOL
MSServ_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_MS_SERV             *pMSServ    = (COSA_DML_MS_SERV*)pLinkObj->hContext;
    BOOL                            pBridgeMode     = FALSE;

    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&pBridgeMode)) && (pBridgeMode == TRUE))
        return FALSE;

    if (strcmp(ParamName, "AlwaysBlock") == 0)
    {
        pMSServ->AlwaysBlock = bValue;
        return TRUE;
    }

    return FALSE;
}

BOOL
MSServ_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_MS_SERV             *pMSServ    = (COSA_DML_MS_SERV*)pLinkObj->hContext;
    BOOL                            pBridgeMode     = FALSE;

    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&pBridgeMode)) && (pBridgeMode == TRUE))
        return FALSE;

    if (strcmp(ParamName, "Description") == 0)
    {
        _ansc_snprintf(pMSServ->Description, sizeof(pMSServ->Description), "%s", strValue);
        return TRUE;
    }
    if (strcmp(ParamName, "StartTime") == 0)
    {
        _ansc_snprintf(pMSServ->StartTime, sizeof(pMSServ->StartTime), "%s", strValue);
        return TRUE;
    }
    if (strcmp(ParamName, "EndTime") == 0)
    {
        _ansc_snprintf(pMSServ->EndTime, sizeof(pMSServ->EndTime), "%s", strValue);
        return TRUE;
    }
    if (strcmp(ParamName, "BlockDays") == 0)
    {
        _ansc_snprintf(pMSServ->BlockDays, sizeof(pMSServ->BlockDays), "%s", strValue);
        return TRUE;
    }

    return FALSE;
}

BOOL
MSServ_SetParamUlongValue(
        ANSC_HANDLE hInsContext,
        char        *ParamName,
        ULONG       ulValue
        )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_MS_SERV             *pMSServ    = (COSA_DML_MS_SERV*)pLinkObj->hContext;
    BOOL                            pBridgeMode     = FALSE;

    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&pBridgeMode)) && (pBridgeMode == TRUE))
        return FALSE;

    if (strcmp(ParamName, "Protocol") == 0)
    {
        pMSServ->Protocol = ulValue;
        return TRUE;
    }
    if (strcmp(ParamName, "StartPort") == 0)
    {
        pMSServ->StartPort = ulValue;
        return TRUE;
    }
    if (strcmp(ParamName, "EndPort") == 0)
    {
        pMSServ->EndPort = ulValue;
        return TRUE;
    }

    return FALSE;
}


BOOL
MSServ_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    return TRUE;
}

ULONG
MSServ_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_MS_SERV             *pMSServ    = (COSA_DML_MS_SERV*)pLinkObj->hContext;
    PCOSA_DATAMODEL_PARENTALCONTROL pParCtrl    = (PCOSA_DATAMODEL_PARENTALCONTROL)g_pCosaBEManager->hParentalControl;

    if (pLinkObj->bNew)
    {
        if (CosaDmlMSServ_AddEntry(pMSServ) != ANSC_STATUS_SUCCESS)
            return -1;
        CosaPcReg_MSServDelInfo((ANSC_HANDLE)pParCtrl, (ANSC_HANDLE)pLinkObj);
        pLinkObj->bNew = FALSE;
    }
    else
    {
        if (CosaDmlMSServ_SetConf(pMSServ->InstanceNumber, pMSServ) != ANSC_STATUS_SUCCESS)
        {
            CosaDmlMSServ_GetConf(pMSServ->InstanceNumber, pMSServ);
            return -1;
        }
    }

    return 0;
}

ULONG
MSServ_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_MS_SERV             *pMSServ    = (COSA_DML_MS_SERV*)pLinkObj->hContext;

    if (CosaDmlMSServ_GetConf(pMSServ->InstanceNumber, pMSServ) != ANSC_STATUS_SUCCESS)
        return -1;

    return 0;
}

ULONG
MSTrustedUser_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;

    AnscTraceWarning(("%s...\n", __FUNCTION__));

    return AnscSListQueryDepth(&pParCtrl->MSTrustedUserList);
}

ANSC_HANDLE
MSTrustedUser_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry = NULL;

    pSLinkEntry = AnscQueueGetEntryByIndex((ANSC_HANDLE)&pParCtrl->MSTrustedUserList, nIndex);
    
    if (pSLinkEntry)
    {
        pLinkObj = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        *pInsNumber = pLinkObj->InstanceNumber;
    }
    
    return pLinkObj;
}

ANSC_HANDLE
MSTrustedUser_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = NULL;
    COSA_DML_MS_TRUSTEDUSER             *pMSTrustedUser    = NULL;

    pLinkObj = AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
    if (!pLinkObj)
        return NULL;
    
    pMSTrustedUser = AnscAllocateMemory(sizeof(COSA_DML_MS_TRUSTEDUSER));
    if (!pMSTrustedUser)
    {
        AnscFreeMemory(pLinkObj);
        return NULL;
    }

    pLinkObj->InstanceNumber = pParCtrl->ulMSTrustedUserNextInsNum;
    pMSTrustedUser->InstanceNumber = pParCtrl->ulMSTrustedUserNextInsNum;
    pParCtrl->ulMSTrustedUserNextInsNum++;
    if (pParCtrl->ulMSTrustedUserNextInsNum == 0)
        pParCtrl->ulMSTrustedUserNextInsNum = 1;

    _ansc_sprintf(pMSTrustedUser->Alias, "cpe-MSTrustedUser-%d", (int)pLinkObj->InstanceNumber);
    pLinkObj->hContext      = (ANSC_HANDLE)pMSTrustedUser;
    pLinkObj->hParentTable  = NULL;
    pLinkObj->bNew          = TRUE;
#if 1
//RDKB-EMULATOR
     char param_value[256] = {0};
     char param_name[256]  = {0};
     sprintf(param_value,"%ld",pMSTrustedUser->InstanceNumber);
     sprintf(param_name,PSM_PC_MSE_TRUSTED_INSTANCENUMBER);
     PSM_Set_Record_Value2(bus_handle,g_Subsystem, param_name, ccsp_string, param_value);
#endif

    CosaSListPushEntryByInsNum((PSLIST_HEADER)&pParCtrl->MSTrustedUserList, pLinkObj);
    CosaPcReg_MSTrustedUserAddInfo((ANSC_HANDLE)pParCtrl, (ANSC_HANDLE)pLinkObj);

    *pInsNumber = pLinkObj->InstanceNumber;

    return pLinkObj;
}

ULONG
MSTrustedUser_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    COSA_DML_MS_TRUSTEDUSER             *pMSTrustedUser    = (COSA_DML_MS_TRUSTEDUSER*)pLinkObj->hContext;

    if (CosaDmlMSTrustedUser_DelEntry(pLinkObj->InstanceNumber) != ANSC_STATUS_SUCCESS)
        return -1;

    AnscSListPopEntryByLink((PSLIST_HEADER)&pParCtrl->MSTrustedUserList, &pLinkObj->Linkage);
    if (pLinkObj->bNew)
        CosaPcReg_MSTrustedUserDelInfo((ANSC_HANDLE)pParCtrl, (ANSC_HANDLE)pLinkObj);

    AnscFreeMemory(pMSTrustedUser);
    AnscFreeMemory(pLinkObj);

    return 0;
}

BOOL
MSTrustedUser_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_MS_TRUSTEDUSER             *pMSTrustedUser    = (COSA_DML_MS_TRUSTEDUSER*)pLinkObj->hContext;

    if (strcmp(ParamName, "Trusted") == 0)
    {
        *pBool = pMSTrustedUser->Trusted;
        return TRUE;
    }

    return FALSE;
}

ULONG
MSTrustedUser_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_MS_TRUSTEDUSER             *pMSTrustedUser    = (COSA_DML_MS_TRUSTEDUSER*)pLinkObj->hContext;

    if (strcmp(ParamName, "HostDescription") == 0)
    {
        AnscCopyString(pValue, pMSTrustedUser->HostDescription);
        return 0;
    }
    if (strcmp(ParamName, "IPAddress") == 0)
    {
        AnscCopyString(pValue, pMSTrustedUser->IPAddress);
        return 0;
    }

    return -1;
}

BOOL
MSTrustedUser_GetParamUlongValue(
        ANSC_HANDLE hInsContext, 
        char *ParamName, 
        ULONG *pUlong)
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_MS_TRUSTEDUSER             *pMSTrustedUser    = (COSA_DML_MS_TRUSTEDUSER*)pLinkObj->hContext;

    if (strcmp(ParamName, "IPAddressType") == 0)
    {
        *pUlong = pMSTrustedUser->IPAddressType;
        return TRUE;
    }

    return FALSE;
}

BOOL
MSTrustedUser_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_MS_TRUSTEDUSER             *pMSTrustedUser    = (COSA_DML_MS_TRUSTEDUSER*)pLinkObj->hContext;
    BOOL                            pBridgeMode     = FALSE;

    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&pBridgeMode)) && (pBridgeMode == TRUE))
        return FALSE;

    if (strcmp(ParamName, "Trusted") == 0)
    {
        pMSTrustedUser->Trusted = bValue;
        return TRUE;
    }

    return FALSE;
}

BOOL
MSTrustedUser_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_MS_TRUSTEDUSER             *pMSTrustedUser    = (COSA_DML_MS_TRUSTEDUSER*)pLinkObj->hContext;
    BOOL                            pBridgeMode     = FALSE;

    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&pBridgeMode)) && (pBridgeMode == TRUE))
        return FALSE;

    if (strcmp(ParamName, "HostDescription") == 0)
    {
        _ansc_snprintf(pMSTrustedUser->HostDescription, sizeof(pMSTrustedUser->HostDescription), "%s", strValue);
        return TRUE;
    }
    if (strcmp(ParamName, "IPAddress") == 0)
    {
        _ansc_snprintf(pMSTrustedUser->IPAddress, sizeof(pMSTrustedUser->IPAddress), "%s", strValue);
        return TRUE;
    }

    return FALSE;
}

BOOL
MSTrustedUser_SetParamUlongValue(
        ANSC_HANDLE hInsContext,
        char        *ParamName,
        ULONG       ulValue
        )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_MS_TRUSTEDUSER             *pMSTrustedUser    = (COSA_DML_MS_TRUSTEDUSER*)pLinkObj->hContext;
    BOOL                            pBridgeMode     = FALSE;

    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&pBridgeMode)) && (pBridgeMode == TRUE))
        return FALSE;

    if (strcmp(ParamName, "IPAddressType") == 0)
    {
        pMSTrustedUser->IPAddressType = ulValue;
        return TRUE;
    }

    return FALSE;
}

BOOL
MSTrustedUser_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    return TRUE;
}

ULONG
MSTrustedUser_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_MS_TRUSTEDUSER             *pMSTrustedUser    = (COSA_DML_MS_TRUSTEDUSER*)pLinkObj->hContext;
    PCOSA_DATAMODEL_PARENTALCONTROL pParCtrl    = (PCOSA_DATAMODEL_PARENTALCONTROL)g_pCosaBEManager->hParentalControl;

    if (pLinkObj->bNew)
    {
        if (CosaDmlMSTrustedUser_AddEntry(pMSTrustedUser) != ANSC_STATUS_SUCCESS)
            return -1;
        CosaPcReg_MSTrustedUserDelInfo((ANSC_HANDLE)pParCtrl, (ANSC_HANDLE)pLinkObj);
        pLinkObj->bNew = FALSE;
    }
    else
    {
        if (CosaDmlMSTrustedUser_SetConf(pMSTrustedUser->InstanceNumber, pMSTrustedUser) != ANSC_STATUS_SUCCESS)
        {
            CosaDmlMSTrustedUser_GetConf(pMSTrustedUser->InstanceNumber, pMSTrustedUser);
            return -1;
        }
    }

    return 0;
}

ULONG
MSTrustedUser_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_MS_TRUSTEDUSER             *pMSTrustedUser    = (COSA_DML_MS_TRUSTEDUSER*)pLinkObj->hContext;

    if (CosaDmlMSTrustedUser_GetConf(pMSTrustedUser->InstanceNumber, pMSTrustedUser) != ANSC_STATUS_SUCCESS)
        return -1;

    return 0;
}

ULONG
MDDev_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;

    AnscTraceWarning(("%s...\n", __FUNCTION__));

    return AnscSListQueryDepth(&pParCtrl->MDDevList);
}

ANSC_HANDLE
MDDev_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry = NULL;

    pSLinkEntry = AnscQueueGetEntryByIndex((ANSC_HANDLE)&pParCtrl->MDDevList, nIndex);
    
    if (pSLinkEntry)
    {
        pLinkObj = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        *pInsNumber = pLinkObj->InstanceNumber;
    }
    
    return pLinkObj;
}

ANSC_HANDLE
MDDev_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = NULL;
    COSA_DML_MD_DEV             *pMDDev    = NULL;

    pLinkObj = AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
    if (!pLinkObj)
        return NULL;
    
    pMDDev = AnscAllocateMemory(sizeof(COSA_DML_MD_DEV));
    if (!pMDDev)
    {
        AnscFreeMemory(pLinkObj);
        return NULL;
    }

    pLinkObj->InstanceNumber = pParCtrl->ulMDDevNextInsNum;
    pMDDev->InstanceNumber = pParCtrl->ulMDDevNextInsNum;
    pParCtrl->ulMDDevNextInsNum++;
    if (pParCtrl->ulMDDevNextInsNum == 0)
        pParCtrl->ulMDDevNextInsNum = 1;

    _ansc_sprintf(pMDDev->Alias, "cpe-MDDevice-%d", (int)pLinkObj->InstanceNumber);
    pLinkObj->hContext      = (ANSC_HANDLE)pMDDev;
    pLinkObj->hParentTable  = NULL;
    pLinkObj->bNew          = TRUE;
#if 1
//RDKB-EMULATOR
     char param_value[256] = {0};
     char param_name[256]  = {0};
     sprintf(param_value,"%ld",pMDDev->InstanceNumber);
     sprintf(param_name,PSM_PC_MD_INSTANCENUMBER);
     PSM_Set_Record_Value2(bus_handle,g_Subsystem, param_name, ccsp_string, param_value);
#endif

    CosaSListPushEntryByInsNum((PSLIST_HEADER)&pParCtrl->MDDevList, pLinkObj);
    CosaPcReg_MDDevAddInfo((ANSC_HANDLE)pParCtrl, (ANSC_HANDLE)pLinkObj);

    *pInsNumber = pLinkObj->InstanceNumber;

    return pLinkObj;
}

ULONG
MDDev_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    COSA_DML_MD_DEV             *pMDDev    = (COSA_DML_MD_DEV*)pLinkObj->hContext;

    if (CosaDmlMDDev_DelEntry(pLinkObj->InstanceNumber) != ANSC_STATUS_SUCCESS)
        return -1;

    AnscSListPopEntryByLink((PSLIST_HEADER)&pParCtrl->MDDevList, &pLinkObj->Linkage);
    if (pLinkObj->bNew)
        CosaPcReg_MDDevDelInfo((ANSC_HANDLE)pParCtrl, (ANSC_HANDLE)pLinkObj);

    AnscFreeMemory(pMDDev);
    AnscFreeMemory(pLinkObj);

    return 0;
}

BOOL
MDDev_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_MD_DEV             *pMDDev    = (COSA_DML_MD_DEV*)pLinkObj->hContext;

    if (strcmp(ParamName, "AlwaysBlock") == 0)
    {
        *pBool = pMDDev->AlwaysBlock;
        return TRUE;
    }

    return FALSE;
}

ULONG
MDDev_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_MD_DEV             *pMDDev    = (COSA_DML_MD_DEV*)pLinkObj->hContext;

    if (strcmp(ParamName, "Description") == 0)
    {
        AnscCopyString(pValue, pMDDev->Description);
        return 0;
    }
    if (strcmp(ParamName, "MACAddress") == 0)
    {
        AnscCopyString(pValue, pMDDev->MACAddress);
        return 0;
    }
    if (strcmp(ParamName, "StartTime") == 0)
    {
        AnscCopyString(pValue, pMDDev->StartTime);
        return 0;
    }
    if (strcmp(ParamName, "EndTime") == 0)
    {
        AnscCopyString(pValue, pMDDev->EndTime);
        return 0;
    }
    if (strcmp(ParamName, "BlockDays") == 0)
    {
        AnscCopyString(pValue, pMDDev->BlockDays);
        return 0;
    }

    return -1;
}

BOOL
MDDev_GetParamUlongValue(
        ANSC_HANDLE hInsContext, 
        char *ParamName, 
        ULONG *pUlong)
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_MD_DEV             *pMDDev    = (COSA_DML_MD_DEV*)pLinkObj->hContext;

    if (strcmp(ParamName, "Type") == 0)
    {
        *pUlong = pMDDev->Type;
        return TRUE;
    }

    return FALSE;
}

BOOL
MDDev_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_MD_DEV             *pMDDev    = (COSA_DML_MD_DEV*)pLinkObj->hContext;
    BOOL                            pBridgeMode     = FALSE;

    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&pBridgeMode)) && (pBridgeMode == TRUE))
        return FALSE;

    if (strcmp(ParamName, "AlwaysBlock") == 0)
    {
        pMDDev->AlwaysBlock = bValue;
        return TRUE;
    }

    return FALSE;
}

BOOL
MDDev_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_MD_DEV             *pMDDev    = (COSA_DML_MD_DEV*)pLinkObj->hContext;
    BOOL                            pBridgeMode     = FALSE;

    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&pBridgeMode)) && (pBridgeMode == TRUE))
        return FALSE;

    if (strcmp(ParamName, "Description") == 0)
    {
        _ansc_snprintf(pMDDev->Description, sizeof(pMDDev->Description), "%s", strValue);
        return TRUE;
    }
    if (strcmp(ParamName, "MACAddress") == 0)
    {
        _ansc_snprintf(pMDDev->MACAddress, sizeof(pMDDev->MACAddress), "%s", strValue);
        return TRUE;
    }
    if (strcmp(ParamName, "StartTime") == 0)
    {
        _ansc_snprintf(pMDDev->StartTime, sizeof(pMDDev->StartTime), "%s", strValue);
        return TRUE;
    }
    if (strcmp(ParamName, "EndTime") == 0)
    {
        _ansc_snprintf(pMDDev->EndTime, sizeof(pMDDev->EndTime), "%s", strValue);
        return TRUE;
    }
    if (strcmp(ParamName, "BlockDays") == 0)
    {
        _ansc_snprintf(pMDDev->BlockDays, sizeof(pMDDev->BlockDays), "%s", strValue);
        return TRUE;
    }

    return FALSE;
}

BOOL
MDDev_SetParamUlongValue(
        ANSC_HANDLE hInsContext,
        char        *ParamName,
        ULONG       ulValue
        )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_MD_DEV             *pMDDev    = (COSA_DML_MD_DEV*)pLinkObj->hContext;
    BOOL                            pBridgeMode     = FALSE;

    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&pBridgeMode)) && (pBridgeMode == TRUE))
        return FALSE;

    if (strcmp(ParamName, "Type") == 0)
    {
        pMDDev->Type = ulValue;
        return TRUE;
    }

    return FALSE;
}


BOOL
MDDev_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    return TRUE;
}

ULONG
MDDev_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_MD_DEV             *pMDDev    = (COSA_DML_MD_DEV*)pLinkObj->hContext;
    PCOSA_DATAMODEL_PARENTALCONTROL pParCtrl    = (PCOSA_DATAMODEL_PARENTALCONTROL)g_pCosaBEManager->hParentalControl;

    if (pLinkObj->bNew)
    {
        if (CosaDmlMDDev_AddEntry(pMDDev) != ANSC_STATUS_SUCCESS)
            return -1;
        CosaPcReg_MDDevDelInfo((ANSC_HANDLE)pParCtrl, (ANSC_HANDLE)pLinkObj);
        pLinkObj->bNew = FALSE;
    }
    else
    {
        if (CosaDmlMDDev_SetConf(pMDDev->InstanceNumber, pMDDev) != ANSC_STATUS_SUCCESS)
        {
            CosaDmlMDDev_GetConf(pMDDev->InstanceNumber, pMDDev);
            return -1;
        }
    }

    return 0;
}

ULONG
MDDev_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_MD_DEV             *pMDDev    = (COSA_DML_MD_DEV*)pLinkObj->hContext;

    if (CosaDmlMDDev_GetConf(pMDDev->InstanceNumber, pMDDev) != ANSC_STATUS_SUCCESS)
        return -1;

    return 0;
}
