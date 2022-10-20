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
#include "ccsp_trace.h"
#include "ansc_string_util.h"
#include "safec_lib_common.h"
#include <syscfg/syscfg.h>

BOOL
ParentalControl_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{

    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "RollbackUTC_Local") == 0)
    {
        *pBool = FALSE;
        return TRUE;
    }

    return FALSE;
}

BOOL
ParentalControl_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
  
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "RollbackUTC_Local") == 0)
    {
		#ifdef UTC_ENABLE

        if(TRUE == bValue)
		{
			CcspTraceWarning(("Rollback to LocalTime requested\n"));
			CosaDmlBlkURL_RollbackUTCtoLocal();

			CosaDmlMSServ_RollbackUTCtoLocal();

			CosaDmlMDDev_RollbackUTCtoLocal();
			system("rm /nvram/UTC_ENABLE");
			CcspTraceWarning(("Parental Control rules are converted in Local time zone\n"));
		}
		#endif
        return TRUE;
    }

    return FALSE;

}


BOOL
MngSites_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pReturnParamName);
    UNREFERENCED_PARAMETER(puLength);
    return TRUE;
}

ULONG
MngSites_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pReturnParamName);
    UNREFERENCED_PARAMETER(puLength);
    return TRUE;
}

ULONG
MngServs_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pReturnParamName);
    UNREFERENCED_PARAMETER(puLength);
    return TRUE;
}

ULONG
MngDevs_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    ULONG                           ulCount;

    AnscTraceWarning(("%s -- pParCtrl = 0x%p\n", __FUNCTION__, pParCtrl));

    ulCount = AnscSListQueryDepth(&pParCtrl->BlkUrlList);

    AnscTraceWarning(("%s -- ulCount = %lu...\n", __FUNCTION__, ulCount));

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
    UNREFERENCED_PARAMETER(hInsContext);
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry = NULL;

    AnscTraceWarning(("%s -- nIndex = %lu...\n", __FUNCTION__, nIndex));

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
    UNREFERENCED_PARAMETER(hInsContext);
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = NULL;
    COSA_DML_BLOCKEDURL             *pBlkUrl    = NULL;
    errno_t                         rc          = -1;

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

    rc = sprintf_s(pBlkUrl->Alias, sizeof(pBlkUrl->Alias),"cpe-BlockedURL-%d", (int)pLinkObj->InstanceNumber);
    if(rc < EOK)
    {
      ERR_CHK(rc);
      AnscFreeMemory(pLinkObj);
      AnscFreeMemory(pBlkUrl);
      return NULL;
    }
    pLinkObj->hContext      = (ANSC_HANDLE)pBlkUrl;
    pLinkObj->hParentTable  = NULL;
    pLinkObj->bNew          = TRUE;

    CosaSListPushEntryByInsNum((PSLIST_HEADER)&pParCtrl->BlkUrlList, pLinkObj);
    CosaPcReg_BlkUrlAddInfo((ANSC_HANDLE)pParCtrl, (ANSC_HANDLE)pLinkObj);
    AnscTraceWarning(("%s-%d RDKB_PCONTROL[URL]:%lu\n", __FUNCTION__, __LINE__, *pInsNumber));

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
    UNREFERENCED_PARAMETER(hInsContext);
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    COSA_DML_BLOCKEDURL             *pBlkUrl    = (COSA_DML_BLOCKEDURL*)pLinkObj->hContext;

    AnscTraceWarning(("%s...\n", __FUNCTION__));

    AnscTraceWarning(("%s-%d RDKB_PCONTROL[URL]:%lu\n", __FUNCTION__, __LINE__, pLinkObj->InstanceNumber));
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
    errno_t                         rc          =  -1;

    UNREFERENCED_PARAMETER(pUlSize);
    AnscTraceWarning(("%s -- param name = %s...\n", __FUNCTION__, ParamName));

    if (strcmp(ParamName, "Site") == 0)
    {
        rc = strcpy_s(pValue, *pUlSize, pBlkUrl->Site);
        if ( rc != EOK) {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    if (strcmp(ParamName, "StartTime") == 0)
    {
        rc = strcpy_s(pValue, *pUlSize, pBlkUrl->StartTime);
        if ( rc != EOK) {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    if (strcmp(ParamName, "EndTime") == 0)
    {
        rc = strcpy_s(pValue, *pUlSize, pBlkUrl->EndTime);
        if ( rc != EOK) {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    if (strcmp(ParamName, "BlockDays") == 0)
    {
        rc = strcpy_s(pValue, *pUlSize, pBlkUrl->BlockDays);
        if ( rc != EOK) {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
#ifdef CONFIG_CISCO_FEATURE_CISCOCONNECT
    if (strcmp(ParamName, "MAC") == 0)
    {
        rc = strcpy_s(pValue, *pUlSize, pBlkUrl->MAC);
        if ( rc != EOK) {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    if (strcmp(ParamName, "DeviceName") == 0)
    {
        rc = strcpy_s(pValue, *pUlSize, pBlkUrl->DeviceName);
        if ( rc != EOK) {
            ERR_CHK(rc);
            return -1;
        }
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

BOOL ValidateTime(int hh , int mm)
{
    if((hh<0) || (hh>23))
        return FALSE;
    if((mm < 0) || (mm >59))
        return FALSE;
    return TRUE;
}

BOOL is_valid_day(char *str)
{
    char *day[7]={"Mon","Tue","Wed","Thu","Fri","Sat","Sun"};
    int i;
    for(i=0;i<7;++i) 
    {
        if (!strcmp(str, day[i]))
            return TRUE;
    }
        return FALSE;
}

BOOL is_url(char *buff)
{
    int i=0;
    char *token=NULL;
    char *delim=".";
    char *str=NULL;
    int len=_ansc_strlen(buff);
    int count=0;
    const char spl[] = "-._~:/?#[]@!$&'()*+,;%=";
    while(buff[i] != '\0')
    {
        //Allowing only integers, alphabets(lower and upper) and certain special characters
        if(strchr(spl,buff[i]) || ((buff[i]>='A') && (buff[i]<='Z')) || ((buff[i]>='a') && (buff[i]<='z')) || ((buff[i]>='0') && (buff[i]<='9')))
            i++;
        else
            return FALSE;
    }    
    if((strncasecmp(buff,"http://",_ansc_strlen("http://"))!=0) && (strncasecmp(buff,"https://",_ansc_strlen("https://"))!=0))
        return FALSE;

    if(buff[len-1] == '.')
        return FALSE;

    str = strdup(buff);
    if (! str) {
       return FALSE;
    }

    token=strtok(str,delim);
    while(token!=NULL)
    {
        count++;
        token=strtok(NULL,delim);
    }
    if(count<2)
    {
        AnscFreeMemory(str);
        return FALSE;
    }
    AnscFreeMemory(str);
return TRUE;
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
    
    #define BUFF_SIZE 2048
    int i=0,HH=0,MM=0,count=0;
    char dump;
    char *delimiter=",";
    char *token;
    char *blockdays=NULL;
    int len=0;
    char *arr[7];
    
    /* check if strValue doesn't hold null or whitespaces */
    if(AnscValidStringCheck(strValue) != TRUE)
        return FALSE;
        
    AnscTraceWarning(("%s -- param name = %s...\n", __FUNCTION__, ParamName));

    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&pBridgeMode)) && (pBridgeMode == TRUE))
        return FALSE;

    if (strcmp(ParamName, "Site") == 0)
    {
        len=_ansc_strlen(strValue);
        if(len > BUFF_SIZE)
            return FALSE;
        if(is_url(strValue) || (pBlkUrl->BlockMethod == BLOCK_METHOD_KEYWORD))
        {
            _ansc_snprintf(pBlkUrl->Site, sizeof(pBlkUrl->Site), "%s", strValue);
            return TRUE;
        }
	else
	{
	     AnscTraceWarning(("%s -- invalid url = %s\n", __FUNCTION__, strValue));
	}
    }
    if (strcmp(ParamName, "StartTime") == 0)
    {
        if(_ansc_sscanf(strValue, "%d:%d %c", &HH,&MM,&dump)==2)
        {
            if(ValidateTime(HH,MM))
            {
                _ansc_snprintf(pBlkUrl->StartTime, sizeof(pBlkUrl->StartTime), "%s", strValue);
                pBlkUrl->StartTimeFlg = TRUE;
                return TRUE;
            }
        }
    }
    if (strcmp(ParamName, "EndTime") == 0)
    {
        if(_ansc_sscanf(strValue, "%d:%d %c", &HH,&MM,&dump)==2)
        {
            if(((MM==59) && (HH<=23)) || (ValidateTime(HH,MM)))
            {
                _ansc_snprintf(pBlkUrl->EndTime, sizeof(pBlkUrl->EndTime), "%s", strValue);
                pBlkUrl->EndTimeFlg = TRUE;
                return TRUE;
            }
        }
    }
    if (strcmp(ParamName, "BlockDays") == 0)
    {
        blockdays = strdup(strValue);
        if (! blockdays) {
           return FALSE;
        }
        token = strtok(blockdays, delimiter);
        if((!token) || (strValue[len-1]==','))
        {
            AnscFreeMemory(blockdays);
            return FALSE;
        }
        while(token)
        {
            if(!is_valid_day(token))
            {
                AnscFreeMemory(blockdays);
                return FALSE;
            }
            for(i=0;i<count;i++)
            {
                if(strncmp(arr[i], token, _ansc_strlen(token)) == 0)
                {
                    for(i=0; i<count; i++) {
                        AnscFreeMemory(arr[i]);
                    }
                    AnscFreeMemory(blockdays);
                    return FALSE;
                }
            }
            arr[count] = (char *) malloc(sizeof(char)*(_ansc_strlen(token)+1));
            strncpy(arr[count], token, (_ansc_strlen(token)+1));
            count++;
            token = strtok(NULL, delimiter);
        }        
        AnscFreeMemory(blockdays);
        for(i=0; i<count; i++) {
            AnscFreeMemory(arr[i]);
        }
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pReturnParamName);
    UNREFERENCED_PARAMETER(puLength);
#if defined(CONFIG_CISCO_CCSP_PRODUCT_ARES) || defined(CONFIG_CISCO_CCSP_PRODUCT_XB3)
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_BLOCKEDURL             *pBlkUrl    = (COSA_DML_BLOCKEDURL*)pLinkObj->hContext;
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = NULL;
    COSA_DML_TRUSTEDUSER             *pTrustedUser    = NULL;
    errno_t                         rc          = -1;

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

    rc = sprintf_s(pTrustedUser->Alias, sizeof(pTrustedUser->Alias),"cpe-TrustedUser-%d", (int)pLinkObj->InstanceNumber);
    if(rc < EOK)
    {
      ERR_CHK(rc);
      AnscFreeMemory(pTrustedUser);
      AnscFreeMemory(pLinkObj);
      return NULL;
    }
    pLinkObj->hContext      = (ANSC_HANDLE)pTrustedUser;
    pLinkObj->hParentTable  = NULL;
    pLinkObj->bNew          = TRUE;

    CosaSListPushEntryByInsNum((PSLIST_HEADER)&pParCtrl->TrustedUserList, pLinkObj);
    CosaPcReg_TrustedUserAddInfo((ANSC_HANDLE)pParCtrl, (ANSC_HANDLE)pLinkObj);
    AnscTraceWarning(("%s-%d RDKB_PCONTROL[TUSER]:%lu\n", __FUNCTION__, __LINE__, *pInsNumber));
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
    UNREFERENCED_PARAMETER(hInsContext);
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    COSA_DML_TRUSTEDUSER             *pTrustedUser    = (COSA_DML_TRUSTEDUSER*)pLinkObj->hContext;

    AnscTraceWarning(("%s-%d RDKB_PCONTROL[TUSER]:%lu\n", __FUNCTION__, __LINE__, pLinkObj->InstanceNumber));
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
    UNREFERENCED_PARAMETER(pUlSize);
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_TRUSTEDUSER             *pTrustedUser    = (COSA_DML_TRUSTEDUSER*)pLinkObj->hContext;
    errno_t                         rc                = -1;

    if (strcmp(ParamName, "HostDescription") == 0)
    {
        rc = strcpy_s(pValue, *pUlSize, pTrustedUser->HostDescription);
        if ( rc != EOK) {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    if (strcmp(ParamName, "IPAddress") == 0)
    {
        rc = strcpy_s(pValue, *pUlSize, pTrustedUser->IPAddress);
        if ( rc != EOK) {
            ERR_CHK(rc);
            return -1;
        }
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

    /* check if strValue doesn't hold null or  whitespaces */
    if(AnscValidStringCheck(strValue) != TRUE)
        return FALSE;
        
    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&pBridgeMode)) && (pBridgeMode == TRUE))
        return FALSE;

    if (strcmp(ParamName, "HostDescription") == 0)
    {
        _ansc_snprintf(pTrustedUser->HostDescription, sizeof(pTrustedUser->HostDescription), "%s", strValue);
        return TRUE;
    }
    if (strcmp(ParamName, "IPAddress") == 0)
    {
        if(pTrustedUser->IPAddressType == IPADDR_IPV4)
        {
            if(is_IpAddress((PUCHAR)strValue))
            {
                _ansc_snprintf(pTrustedUser->IPAddress, sizeof(pTrustedUser->IPAddress), "%s", strValue);
                return TRUE;
            }
        }
        if(pTrustedUser->IPAddressType == IPADDR_IPV6)
        {
            if(is_Ipv6_address((PUCHAR)strValue))
            {
                _ansc_snprintf(pTrustedUser->IPAddress, sizeof(pTrustedUser->IPAddress), "%s", strValue);
                return TRUE;
            }
        }
    }
    return FALSE;
}

BOOL
PcTrustedUser_GetParamUlongValue(
        ANSC_HANDLE hInsContext, 
        char *ParamName, 
        ULONG *pUlong)
{
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pReturnParamName);
    UNREFERENCED_PARAMETER(puLength);
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = NULL;
    COSA_DML_MS_SERV             *pMSServ    = NULL;
    errno_t                      rc          = -1;

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

    rc = sprintf_s(pMSServ->Alias, sizeof(pMSServ->Alias),"cpe-MSService-%d", (int)pLinkObj->InstanceNumber);
    if(rc < EOK)
    {
      ERR_CHK(rc);
      AnscFreeMemory(pMSServ);
      AnscFreeMemory(pLinkObj);
      return NULL;
    }
    pLinkObj->hContext      = (ANSC_HANDLE)pMSServ;
    pLinkObj->hParentTable  = NULL;
    pLinkObj->bNew          = TRUE;

    CosaSListPushEntryByInsNum((PSLIST_HEADER)&pParCtrl->MSServList, pLinkObj);
    CosaPcReg_MSServAddInfo((ANSC_HANDLE)pParCtrl, (ANSC_HANDLE)pLinkObj);
    AnscTraceWarning(("%s-%d RDKB_PCONTROL[MSSERV]:%lu\n", __FUNCTION__, __LINE__, *pInsNumber));

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
    UNREFERENCED_PARAMETER(hInsContext);
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    COSA_DML_MS_SERV             *pMSServ    = (COSA_DML_MS_SERV*)pLinkObj->hContext;

    AnscTraceWarning(("%s-%d RDKB_PCONTROL[MSSERV]:%lu\n", __FUNCTION__, __LINE__, pLinkObj->InstanceNumber));
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
    UNREFERENCED_PARAMETER(pUlSize);
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_MS_SERV             *pMSServ    = (COSA_DML_MS_SERV*)pLinkObj->hContext;
    errno_t                         rc          = -1;

    if (strcmp(ParamName, "Description") == 0)
    {
        rc = strcpy_s(pValue, *pUlSize, pMSServ->Description);
        if ( rc != EOK) {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    if (strcmp(ParamName, "StartTime") == 0)
    {
        rc = strcpy_s(pValue, *pUlSize, pMSServ->StartTime);
        if ( rc != EOK) {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    if (strcmp(ParamName, "EndTime") == 0)
    {
        rc = strcpy_s(pValue, *pUlSize, pMSServ->EndTime);
        if ( rc != EOK) {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    if (strcmp(ParamName, "BlockDays") == 0)
    {
        rc = strcpy_s(pValue, *pUlSize, pMSServ->BlockDays);
        if ( rc != EOK) {
            ERR_CHK(rc);
            return -1;
        }
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

    int HH=0,MM=0,i=0,count=0;
    char dump;
    char *delimiter=",";
    char *token;
    char *blockdays=NULL;
    int len=0;
    char *arr[7];
    BOOL is_space=FALSE;
    
    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&pBridgeMode)) && (pBridgeMode == TRUE))
        return FALSE;

    if (strcmp(ParamName, "Description") == 0)
    {
        /* check if description doesn't hold certain spl charaters */
        if((strValue == NULL) || (*strValue=='\0'))
        {
            return FALSE;
        }
        while(strValue[i] != '\0')
        {
            if(strValue[i] != ' ')		//check if non-space character is present
                is_space=TRUE;
            if ((strValue[i] == '<') || (strValue[i] == '>') || (strValue[i] == '&') || (strValue[i] == '\'') || (strValue[i] == '\"') || (strValue[i] == '|'))
            {
                return FALSE;
            }
            i++;
        }
        if(is_space==FALSE)
            return FALSE;		//returning FALSE as only space is present in Desc.
        _ansc_snprintf(pMSServ->Description, sizeof(pMSServ->Description), "%s", strValue);
        return TRUE;
    }

    /* check if strValue doesn't hold null or whitespaces */
    if(AnscValidStringCheck(strValue) != TRUE)
        return FALSE;

    if (strcmp(ParamName, "StartTime") == 0)
    {
        if(_ansc_sscanf(strValue, "%d:%d %c", &HH,&MM,&dump)==2)
        {
            if(ValidateTime(HH,MM))
            {
                _ansc_snprintf(pMSServ->StartTime, sizeof(pMSServ->StartTime), "%s", strValue);
                return TRUE;
            }
        }
    }
    if (strcmp(ParamName, "EndTime") == 0)
    {
        if(_ansc_sscanf(strValue, "%d:%d %c", &HH,&MM,&dump)==2)
        {
            if(((MM==59) && (HH<=23)) || (ValidateTime(HH,MM)))
            {
                _ansc_snprintf(pMSServ->EndTime, sizeof(pMSServ->EndTime), "%s", strValue);
                return TRUE;
            }
        }
    }
    if (strcmp(ParamName, "BlockDays") == 0)
    {
        blockdays = strdup(strValue);
        if (! blockdays) {
            return FALSE;
        }
        token = strtok(blockdays, delimiter);
        if((!token) || (strValue[len-1]==','))
        {
            AnscFreeMemory(blockdays);
            return FALSE;
        }
        while(token)
        {
            if(!is_valid_day(token))
            {
                AnscFreeMemory(blockdays);
                return FALSE;
            }
            for(i=0;i<count;i++)
            {
                if(strncmp(arr[i], token, _ansc_strlen(token)) == 0)
                {
                    for(i=0; i<count; i++) {
                        AnscFreeMemory(arr[i]);
                    }
                    AnscFreeMemory(blockdays);
                    return FALSE;
                }
            }
            arr[count] = (char *) malloc(sizeof(char)*(_ansc_strlen(token)+1));
            strncpy(arr[count], token, (_ansc_strlen(token)+1));
            count++;
            token = strtok(NULL, delimiter);
        }
        AnscFreeMemory(blockdays);
        for(i=0; i<count; i++) {
            AnscFreeMemory(arr[i]);
        }
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

    #define MAX_PORT 65535
    #define MIN_PORT 1
    
    if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&pBridgeMode)) && (pBridgeMode == TRUE))
        return FALSE;

    if (strcmp(ParamName, "Protocol") == 0)
    {
        pMSServ->Protocol = ulValue;
        return TRUE;
    }
    if (strcmp(ParamName, "StartPort") == 0)
    {
        if((ulValue >= MIN_PORT) && (ulValue <= MAX_PORT))
        {
            pMSServ->StartPort = ulValue;
            return TRUE;
        }
    }
    if (strcmp(ParamName, "EndPort") == 0)
    {
        if((ulValue >= pMSServ->StartPort) && (ulValue <= MAX_PORT))
        {
            pMSServ->EndPort = ulValue;
            return TRUE;
        }
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pReturnParamName);
    UNREFERENCED_PARAMETER(puLength);
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = NULL;
    COSA_DML_MS_TRUSTEDUSER             *pMSTrustedUser    = NULL;
    errno_t                              rc                = -1;

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

    rc = sprintf_s(pMSTrustedUser->Alias, sizeof(pMSTrustedUser->Alias),"cpe-MSTrustedUser-%d", (int)pLinkObj->InstanceNumber);
    if(rc < EOK)
    {
      ERR_CHK(rc);
      AnscFreeMemory(pMSTrustedUser);
      AnscFreeMemory(pLinkObj);
      return NULL;
    }

    pLinkObj->hContext      = (ANSC_HANDLE)pMSTrustedUser;
    pLinkObj->hParentTable  = NULL;
    pLinkObj->bNew          = TRUE;

    CosaSListPushEntryByInsNum((PSLIST_HEADER)&pParCtrl->MSTrustedUserList, pLinkObj);
    CosaPcReg_MSTrustedUserAddInfo((ANSC_HANDLE)pParCtrl, (ANSC_HANDLE)pLinkObj);
    AnscTraceWarning(("%s-%d RDKB_PCONTROL[MSTUSER]:%lu\n", __FUNCTION__, __LINE__, *pInsNumber));

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
    UNREFERENCED_PARAMETER(hInsContext);
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    COSA_DML_MS_TRUSTEDUSER             *pMSTrustedUser    = (COSA_DML_MS_TRUSTEDUSER*)pLinkObj->hContext;

    AnscTraceWarning(("%s-%d RDKB_PCONTROL[MSTUSER]:%lu\n", __FUNCTION__, __LINE__, pLinkObj->InstanceNumber));
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
    UNREFERENCED_PARAMETER(pUlSize);
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_MS_TRUSTEDUSER             *pMSTrustedUser    = (COSA_DML_MS_TRUSTEDUSER*)pLinkObj->hContext;
    errno_t                         rc          = -1;

    if (strcmp(ParamName, "HostDescription") == 0)
    {
        rc = strcpy_s(pValue, *pUlSize, pMSTrustedUser->HostDescription);
        if ( rc != EOK) {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    if (strcmp(ParamName, "IPAddress") == 0)
    {
        rc = strcpy_s(pValue, *pUlSize, pMSTrustedUser->IPAddress);
        if ( rc != EOK) {
            ERR_CHK(rc);
            return -1;
        }
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
    
    /* check if strValue doesn't hold null or whitespaces */
    if(AnscValidStringCheck(strValue) != TRUE)
        return FALSE;

    if (strcmp(ParamName, "HostDescription") == 0)
    {
        _ansc_snprintf(pMSTrustedUser->HostDescription, sizeof(pMSTrustedUser->HostDescription), "%s", strValue);
        return TRUE;
    }
    if (strcmp(ParamName, "IPAddress") == 0)
    {
        if(pMSTrustedUser->IPAddressType == IPADDR_IPV4)
        {
            if(is_IpAddress((PUCHAR)strValue))
            {
                _ansc_snprintf(pMSTrustedUser->IPAddress, sizeof(pMSTrustedUser->IPAddress), "%s", strValue);
                return TRUE;
            }
        }
        if(pMSTrustedUser->IPAddressType == IPADDR_IPV6)
        {
            if(is_Ipv6_address((PUCHAR)strValue))
            {
                _ansc_snprintf(pMSTrustedUser->IPAddress, sizeof(pMSTrustedUser->IPAddress), "%s", strValue);
                return TRUE;
            }
        }
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pReturnParamName);
    UNREFERENCED_PARAMETER(puLength);
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = NULL;
    COSA_DML_MD_DEV             *pMDDev    = NULL;
    errno_t                      rc         = -1;

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
    if (pParCtrl->ulMDDevNextInsNum <= 0)
        {
            pParCtrl->ulMDDevNextInsNum = 1;
            pLinkObj->InstanceNumber = pParCtrl->ulMDDevNextInsNum;
            pMDDev->InstanceNumber = pParCtrl->ulMDDevNextInsNum;
        }

    rc = sprintf_s(pMDDev->Alias, sizeof(pMDDev->Alias),"cpe-MDDevice-%d", (int)pLinkObj->InstanceNumber);
    if(rc < EOK)
    {
      ERR_CHK(rc);
      AnscFreeMemory(pMDDev);
      AnscFreeMemory(pLinkObj);
      return NULL;
    }
    pLinkObj->hContext      = (ANSC_HANDLE)pMDDev;
    pLinkObj->hParentTable  = NULL;
    pLinkObj->bNew          = TRUE;

    CosaSListPushEntryByInsNum((PSLIST_HEADER)&pParCtrl->MDDevList, pLinkObj);
    CosaPcReg_MDDevAddInfo((ANSC_HANDLE)pParCtrl, (ANSC_HANDLE)pLinkObj);
    AnscTraceWarning(("%s-%d RDKB_PCONTROL[MDDEV]:%lu\n", __FUNCTION__, __LINE__, *pInsNumber));

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
    UNREFERENCED_PARAMETER(hInsContext);
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    COSA_DML_MD_DEV             *pMDDev    = (COSA_DML_MD_DEV*)pLinkObj->hContext;

    AnscTraceWarning(("%s-%d RDKB_PCONTROL[MDDEV]:%lu\n", __FUNCTION__, __LINE__, pLinkObj->InstanceNumber));
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
    UNREFERENCED_PARAMETER(pUlSize);
    PCOSA_CONTEXT_LINK_OBJECT       pLinkObj    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    COSA_DML_MD_DEV             *pMDDev    = (COSA_DML_MD_DEV*)pLinkObj->hContext;
    errno_t                         rc          = -1;

    if (strcmp(ParamName, "Description") == 0)
    {
        rc = strcpy_s(pValue, *pUlSize, pMDDev->Description);
        if ( rc != EOK) {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    if (strcmp(ParamName, "MACAddress") == 0)
    {
        rc = strcpy_s(pValue, *pUlSize, pMDDev->MACAddress);
        if ( rc != EOK) {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    if (strcmp(ParamName, "StartTime") == 0)
    {
        rc = strcpy_s(pValue, *pUlSize, pMDDev->StartTime);
        if ( rc != EOK) {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    if (strcmp(ParamName, "EndTime") == 0)
    {
        rc = strcpy_s(pValue, *pUlSize, pMDDev->EndTime);
        if ( rc != EOK) {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    if (strcmp(ParamName, "BlockDays") == 0)
    {
        rc = strcpy_s(pValue, *pUlSize, pMDDev->BlockDays);
        if ( rc != EOK) {
            ERR_CHK(rc);
            return -1;
        }
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
    UNREFERENCED_PARAMETER(pUlong);
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

    UINT MacAddress[6] = {0};
    int HH=0,MM=0,i=0,count=0;
    char dump;
    char *delimiter=",";
    char *token;
    char *blockdays=NULL;
    int len=0;
    char *arr[7];
    BOOL is_space=TRUE;

    if (strcmp(ParamName, "Description") == 0)
    {
        /* check if description doesn't hold certain spl charaters */
        if((strValue == NULL) || (*strValue=='\0'))
        {
            return FALSE;
        }
        while(strValue[i] != '\0')
        {
            if(strValue[i] != ' ')		//check if non-space character is present
                is_space=FALSE;
            if ((strValue[i] == '<') || (strValue[i] == '>') || (strValue[i] == '&') || (strValue[i] == '\'') || (strValue[i] == '\"') || (strValue[i] == '|'))
            {
                return FALSE;
            }
            i++;
        }
        if(is_space==TRUE)
            return FALSE;		//returning FALSE as only space is present in Desc.
        _ansc_snprintf(pMDDev->Description, sizeof(pMDDev->Description), "%s", strValue);
        return TRUE;
    }

     /* check if strValue doesn't hold null or whitespaces */
    if(AnscValidStringCheck(strValue) != TRUE)
        return FALSE;

    if (strcmp(ParamName, "MACAddress") == 0)
    {
        if((_ansc_strlen(strValue)) != 17)
            return FALSE;
        if(_ansc_sscanf(strValue, "%02X:%02X:%02X:%02X:%02X:%02X%c"
                , &MacAddress[0]
                , &MacAddress[1]
                , &MacAddress[2]
                , &MacAddress[3]
                , &MacAddress[4]
                , &MacAddress[5]
                , &dump)==6)
        {
            _ansc_snprintf(pMDDev->MACAddress, sizeof(pMDDev->MACAddress), "%s", strValue);
            return TRUE;
        }
    }
    if (strcmp(ParamName, "StartTime") == 0)
    {
        if(_ansc_sscanf(strValue, "%d:%d %c", &HH,&MM,&dump)==2)
        {  
            if(ValidateTime(HH,MM))
            {
                _ansc_snprintf(pMDDev->StartTime, sizeof(pMDDev->StartTime), "%s", strValue);
                return TRUE;
            }
        }
    }
    if (strcmp(ParamName, "EndTime") == 0)
    {
        if(_ansc_sscanf(strValue, "%d:%d %c", &HH,&MM,&dump)==2)
        {
            if(((MM==59) && (HH<=23)) || (ValidateTime(HH,MM)))
            {
                _ansc_snprintf(pMDDev->EndTime, sizeof(pMDDev->EndTime), "%s", strValue);
                return TRUE;
            }
        }
    }
    if (strcmp(ParamName, "BlockDays") == 0)
    {
        blockdays = strdup(strValue);
        if (! blockdays) {
            return FALSE;
        }
        token = strtok(blockdays, delimiter);
        if((!token) || (strValue[len-1]==','))
        {
            AnscFreeMemory(blockdays);
            return FALSE;
        }
        while(token)
        {
            if(!is_valid_day(token))
            {
                AnscFreeMemory(blockdays);
                return FALSE;
            }
            for(i=0;i<count;i++)
            {
                if(strncmp(arr[i], token, _ansc_strlen(token)) == 0)
                {
                    for(i=0; i<count; i++) {
                        AnscFreeMemory(arr[i]);
                    }
                    AnscFreeMemory(blockdays);
                    return FALSE;
                }
            }
            arr[count] = (char *) malloc(sizeof(char)*(_ansc_strlen(token)+1));
            strncpy(arr[count], token, (_ansc_strlen(token)+1));
            count++;
            token = strtok(NULL, delimiter);
        }        
        AnscFreeMemory(blockdays);
        for(i=0; i<count; i++) {
            AnscFreeMemory(arr[i]);
        }
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pReturnParamName);
    UNREFERENCED_PARAMETER(puLength);
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

ULONG
MDRed_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pUlSize);
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    COSA_DML_MD_RED                 *pMDRed = &pParCtrl->MDRedirect;
    char IPv4[17] = "0"; 
    errno_t                         rc        = -1;

    if (strcmp(ParamName, "HTTP_Server_IP") == 0)
    { 
        syscfg_get(NULL, "HTTP_Server_IP", IPv4, sizeof(IPv4));
        rc = strcpy_s(pValue, *pUlSize, IPv4);
        if ( rc != EOK) {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    if (strcmp(ParamName, "HTTPS_Server_IP") == 0)
    {
        syscfg_get(NULL, "HTTPS_Server_IP", IPv4, sizeof(IPv4));
        rc = strcpy_s(pValue, *pUlSize, IPv4);
        if ( rc != EOK) {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    if (strcmp(ParamName, "Default_Server_IP") == 0)
    {
        syscfg_get(NULL, "Default_Server_IP", IPv4, sizeof(IPv4));
        rc = strcpy_s(pValue, *pUlSize, IPv4);
        if ( rc != EOK) {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    if (strcmp(ParamName, "HTTP_Server_IPv6") == 0)
    {
        rc = strcpy_s(pValue, *pUlSize, pMDRed->HTTP_Server_IPv6);
        if ( rc != EOK) {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    if (strcmp(ParamName, "HTTPS_Server_IPv6") == 0)
    {
        rc = strcpy_s(pValue, *pUlSize, pMDRed->HTTP_Server_IPv6);
        if ( rc != EOK) {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    if (strcmp(ParamName, "Default_Server_IPv6") == 0)
    {
        rc = strcpy_s(pValue, *pUlSize, pMDRed->Default_Server_IPv6);
        if ( rc != EOK) {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }

    return -1;
}

BOOL
MDRed_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    COSA_DML_MD_RED                 *pMDRed = &pParCtrl->MDRedirect;
    ANSC_STATUS ret=ANSC_STATUS_FAILURE;
    errno_t     rc =  -1;
    int ind = -1;
    char wrapstring[256] = {0};

    rc = strcmp_s("HTTP_Server_IP", strlen("HTTP_Server_IP"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
	ret=isValidIP(4, strValue, wrapstring, sizeof(wrapstring));
        if(ANSC_STATUS_SUCCESS != ret)
	    return FALSE;

        rc = STRCPY_S_NOCLOBBER(pMDRed->HTTP_Server_IP, sizeof(pMDRed->HTTP_Server_IP), wrapstring);
        if(rc != EOK)
        {
          ERR_CHK(rc);
          return FALSE;
        }
		CcspTraceWarning(("HTTP_Server_IP is set to %s\n", pMDRed->HTTP_Server_IP));
        return TRUE;
    }
    rc = strcmp_s("HTTPS_Server_IP", strlen("HTTPS_Server_IP"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        ret=isValidIP(4, strValue, wrapstring, sizeof(wrapstring));
        if(ANSC_STATUS_SUCCESS != ret)
            return FALSE;

        rc = STRCPY_S_NOCLOBBER(pMDRed->HTTPS_Server_IP,sizeof(pMDRed->HTTPS_Server_IP), wrapstring);
        if(rc != EOK)
        {
          ERR_CHK(rc);
          return FALSE;
        }
		CcspTraceWarning(("HTTPS_Server_IP is set to %s\n", pMDRed->HTTPS_Server_IP));
        return TRUE;
    }
    rc = strcmp_s("Default_Server_IP", strlen("Default_Server_IP"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        ret=isValidIP(4, strValue, wrapstring, sizeof(wrapstring));
        if(ANSC_STATUS_SUCCESS != ret)
            return FALSE;

        rc = STRCPY_S_NOCLOBBER(pMDRed->Default_Server_IP,sizeof(pMDRed->Default_Server_IP), wrapstring);
        if(rc != EOK)
        {
          ERR_CHK(rc);
          return FALSE;
        }
		CcspTraceWarning(("Default_Server_IP is set to %s\n", pMDRed->Default_Server_IP));
        return TRUE;
    }
    rc = strcmp_s("HTTP_Server_IPv6", strlen("HTTP_Server_IPv6"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        ret=isValidIP(6, strValue, wrapstring, sizeof(wrapstring));
        if(ret!=1)
            return FALSE;

        rc = STRCPY_S_NOCLOBBER(pMDRed->HTTP_Server_IPv6,sizeof(pMDRed->HTTP_Server_IPv6), wrapstring);
        if(rc != EOK)
        {
          ERR_CHK(rc);
          return FALSE;
        }
		CcspTraceWarning(("HTTP_Server_IPv6 is set to %s\n", pMDRed->HTTP_Server_IPv6));
        return TRUE;
    }
    rc = strcmp_s("HTTPS_Server_IPv6", strlen("HTTPS_Server_IPv6"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        ret=isValidIP(6, strValue, wrapstring, sizeof(wrapstring));
        if(ANSC_STATUS_SUCCESS != ret)
            return FALSE;

        rc = STRCPY_S_NOCLOBBER(pMDRed->HTTPS_Server_IPv6,sizeof(pMDRed->HTTPS_Server_IPv6), wrapstring);
        if(rc != EOK)
        {
          ERR_CHK(rc);
          return FALSE;
        }
		CcspTraceWarning(("HTTPS_Server_IPv6 is set to %s\n", pMDRed->HTTPS_Server_IPv6));
        return TRUE;
    }
    rc = strcmp_s("Default_Server_IPv6", strlen("Default_Server_IPv6"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        ret=isValidIP(6, strValue, wrapstring, sizeof(wrapstring));
        if(ANSC_STATUS_SUCCESS != ret)
            return FALSE;

        rc = STRCPY_S_NOCLOBBER(pMDRed->Default_Server_IPv6,sizeof(pMDRed->Default_Server_IPv6), wrapstring);
        if(rc != EOK)
        {
          ERR_CHK(rc);
          return FALSE;
        }
		CcspTraceWarning(("Default_Server_IPv6 is set to %s\n", pMDRed->Default_Server_IPv6));
        return TRUE;
    }

    return FALSE;
}

BOOL
MDRed_Validate
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
MDRed_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    COSA_DML_MD_RED                 *pMDRed = &pParCtrl->MDRedirect;

    if (CosaDmlMDRed_SetConf(pMDRed) != ANSC_STATUS_SUCCESS)
    {
        CosaDmlMDRed_GetConf(pMDRed);
        return -1;
    }

    return 0;
}

ULONG
MDRed_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    COSA_DATAMODEL_PARENTALCONTROL  *pParCtrl = (COSA_DATAMODEL_PARENTALCONTROL*)g_pCosaBEManager->hParentalControl;
    COSA_DML_MD_RED                 *pMDRed = &pParCtrl->MDRedirect;

    if (CosaDmlMDRed_GetConf(pMDRed) != ANSC_STATUS_SUCCESS)
        return -1;

    return 0;
}
