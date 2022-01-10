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

#include "cosa_apis.h"
#include "cosa_x_comcast_com_parentalcontrol_internal.h"
#include "plugin_main_apis.h"
#include <syscfg/syscfg.h>
#include "safec_lib_common.h"

extern void * g_pDslhDmlAgent;
    BOOL                            bMigration         = FALSE;
ANSC_HANDLE
CosaParentalControlCreate
    (
        VOID
    )
{
    PCOSA_DATAMODEL_PARENTALCONTROL  pMyObject    = (PCOSA_DATAMODEL_PARENTALCONTROL)NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PCOSA_DATAMODEL_PARENTALCONTROL)AnscAllocateMemory(sizeof(COSA_DATAMODEL_PARENTALCONTROL));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->Oid               = COSA_DATAMODEL_PARENTALCONTROL_OID;
    pMyObject->Create            = CosaParentalControlCreate;
    pMyObject->Remove            = CosaParentalControlRemove;
    pMyObject->Initialize        = CosaParentalControlInitialize;

    pMyObject->Initialize   ((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}

ANSC_STATUS
CosaParentalControlInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PCOSA_DATAMODEL_PARENTALCONTROL pParCtrl       = (PCOSA_DATAMODEL_PARENTALCONTROL)hThisObject;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoCOSA = NULL;
    PSLAP_VARIABLE                  pSlapVariable   = NULL;
    /* ManagedSites.BlockedURL */
    ULONG                           ulBlkUrlCnt;
    ULONG                           ulBlkUrlIdx;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoBlkUrl   = NULL;
    COSA_DML_BLOCKEDURL             *pBlkUrl;
    PCOSA_CONTEXT_LINK_OBJECT       pBlkUrlLinkObj      = NULL;
    /* ManagedSites.TrustedUser */
    ULONG                           ulTrustedUserCnt;
    ULONG                           ulTrustedUserIdx;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoTrustedUser   = NULL;
    COSA_DML_TRUSTEDUSER             *pTrustedUser;
    PCOSA_CONTEXT_LINK_OBJECT       pTrustedUserLinkObj      = NULL;
    /* ManagedServices.Service.{i}. */
    ULONG                           ulMSServCnt;
    ULONG                           ulMSServIdx;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoMSServ   = NULL;
    COSA_DML_MS_SERV             *pMSServ;
    PCOSA_CONTEXT_LINK_OBJECT       pMSServLinkObj      = NULL;
    /* ManagedServices.TrustedUser.{i}. */
    ULONG                           ulMSTrustedUserCnt;
    ULONG                           ulMSTrustedUserIdx;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoMSTrustedUser   = NULL;
    COSA_DML_MS_TRUSTEDUSER             *pMSTrustedUser;
    PCOSA_CONTEXT_LINK_OBJECT       pMSTrustedUserLinkObj      = NULL;
    /* ManagedDevices.Device.{i}. */
    ULONG                           ulMDDevCnt;
    ULONG                           ulMDDevIdx;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoMDDev   = NULL;
    COSA_DML_MD_DEV             *pMDDev;
    PCOSA_CONTEXT_LINK_OBJECT       pMDDevLinkObj      = NULL;
    errno_t                         rc                 = -1;
	FILE *fp = NULL;
	#ifdef UTC_ENABLE
    if ( (fp=fopen("/nvram/UTC_ENABLE", "r")) == NULL )
    {
        bMigration = TRUE;
    }else
	{
		fclose(fp);
	}
	#endif
    /* Initiation all functions */
    CosaDmlParentalControlInit(NULL, NULL);

    /* ManagedSites/ManagedServices/ManagedDevices */
    CosaDmlMngSites_GetConf(&pParCtrl->ManagedSites);
    CosaDmlMngServs_GetConf(&pParCtrl->ManagedServs);
    CosaDmlMngDevs_GetConf(&pParCtrl->ManagedDevs);
    CosaDmlMDRed_GetConf(&pParCtrl->MDRedirect);

    /* ManagedSites.BlockedURL.{i}. */
    ulBlkUrlCnt = CosaDmlBlkURL_GetNumberOfEntries();
	AnscTraceWarning(("%s -- ulBlkUrlCnt = %lu ...\n", __FUNCTION__,ulBlkUrlCnt));
	#ifdef UTC_ENABLE
	
	if(bMigration == TRUE)
	{
		AnscTraceWarning(("%s -- callCosaDmlBlkURL_Migration ...\n", __FUNCTION__));
		
		if (ANSC_STATUS_SUCCESS == CosaDmlBlkURL_Migration())
		{
			AnscTraceWarning(("%s -- Success CosaDmlBlkURL_Migration ...\n", __FUNCTION__));
		    ulBlkUrlCnt = CosaDmlBlkURL_GetNumberOfEntries();
			AnscTraceWarning(("%s -- ulBlkUrlCnt = %lu ...\n", __FUNCTION__,ulBlkUrlCnt));
		}
		else
		{
			AnscTraceWarning(("%s -- Failed CosaDmlBlkURL_Migration ...\n", __FUNCTION__));
		}
		
	}	
	#endif

    AnscSListInitializeHeader(&pParCtrl->BlkUrlList);
    pParCtrl->ulBlkUrlNextInsNum = 1;
    pParCtrl->hIrepFolderCOSA = g_GetRegistryRootFolder(g_pDslhDmlAgent);

    pPoamIrepFoCOSA = (PPOAM_IREP_FOLDER_OBJECT)pParCtrl->hIrepFolderCOSA;
    if (!pPoamIrepFoCOSA)
    {
        return ANSC_STATUS_FAILURE;
    }
    pPoamIrepFoBlkUrl = (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoCOSA->GetFolder(
            (ANSC_HANDLE)pPoamIrepFoCOSA,
            COSA_IREP_FOLDER_NAME_PARENTALCONTROL);
    if (!pPoamIrepFoBlkUrl)
    {
        pPoamIrepFoBlkUrl = 
            pPoamIrepFoCOSA->AddFolder(
                    (ANSC_HANDLE)pPoamIrepFoCOSA,
                    COSA_IREP_FOLDER_NAME_PARENTALCONTROL,
                    0);
    }

    if (!pPoamIrepFoBlkUrl)
        return ANSC_STATUS_FAILURE;
    else
        pParCtrl->hIrepFolderBlkUrl = pPoamIrepFoBlkUrl;

    pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoBlkUrl->GetRecord(
            (ANSC_HANDLE)pPoamIrepFoBlkUrl,
            COSA_DML_RR_NAME_PARENTALCONTROL_NextInsNumber,
            NULL);
    if (pSlapVariable)
    {
        pParCtrl->ulBlkUrlNextInsNum = pSlapVariable->Variant.varUint32;
        SlapFreeVariable(pSlapVariable);
    }

    for (ulBlkUrlIdx = 0; ulBlkUrlIdx < ulBlkUrlCnt; ulBlkUrlIdx++)
    {
        pBlkUrl = AnscAllocateMemory(sizeof(COSA_DML_BLOCKEDURL));
        if (!pBlkUrl)
            return ANSC_STATUS_FAILURE;

        if (CosaDmlBlkURL_GetEntryByIndex(ulBlkUrlIdx, pBlkUrl) != ANSC_STATUS_SUCCESS)
        {
            CcspTraceError(("%s: CosaDmlBlkURL_GetEntryByIndex error\n", __FUNCTION__));
            AnscFreeMemory(pBlkUrl);
            return ANSC_STATUS_FAILURE;
        }

        pBlkUrlLinkObj = AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
        if (!pBlkUrlLinkObj)
        {
            AnscFreeMemory(pBlkUrl);
            return ANSC_STATUS_FAILURE;
        }

        if (pBlkUrl->InstanceNumber != 0)
        {
            if (pParCtrl->ulBlkUrlNextInsNum <= pBlkUrl->InstanceNumber)
            {
                pParCtrl->ulBlkUrlNextInsNum = pBlkUrl->InstanceNumber + 1;
                if (pParCtrl->ulBlkUrlNextInsNum == 0)
                {
                    pParCtrl->ulBlkUrlNextInsNum = 1;
                }
            }
        }
        else
        {
            pBlkUrl->InstanceNumber = pParCtrl->ulBlkUrlNextInsNum;

            pParCtrl->ulBlkUrlNextInsNum++;
            if (pParCtrl->ulBlkUrlNextInsNum == 0)
            {
                pParCtrl->ulBlkUrlNextInsNum = 1;
            }

            rc = sprintf_s(pBlkUrl->Alias, sizeof(pBlkUrl->Alias),"cpe-BlockedURL-%d", (int)pBlkUrl->InstanceNumber);
            if(rc < EOK)
            {
              ERR_CHK(rc);
              AnscFreeMemory(pBlkUrl);
              AnscFreeMemory(pBlkUrlLinkObj);
              return ANSC_STATUS_FAILURE;
            }
            CosaDmlBlkURL_SetValues(ulBlkUrlIdx, pBlkUrl->InstanceNumber, pBlkUrl->Alias);
        }

        pBlkUrlLinkObj->InstanceNumber  = pBlkUrl->InstanceNumber;
        pBlkUrlLinkObj->hContext        = pBlkUrl;
        pBlkUrlLinkObj->hParentTable    = NULL;
        pBlkUrlLinkObj->bNew            = FALSE;

        CosaSListPushEntryByInsNum(&pParCtrl->BlkUrlList, pBlkUrlLinkObj);
    }

    CosaPcReg_BlkUrlGetInfo((ANSC_HANDLE)pParCtrl);

    /* ManagedSites.TrustedUser.{i}. */
    ulTrustedUserCnt = CosaDmlTrustedUser_GetNumberOfEntries();

    AnscSListInitializeHeader(&pParCtrl->TrustedUserList);
    pParCtrl->ulTrustedUserNextInsNum = 1;
    pParCtrl->hIrepFolderCOSA = g_GetRegistryRootFolder(g_pDslhDmlAgent);

    pPoamIrepFoCOSA = (PPOAM_IREP_FOLDER_OBJECT)pParCtrl->hIrepFolderCOSA;
    if (!pPoamIrepFoCOSA)
    {
        return ANSC_STATUS_FAILURE;
    }
    pPoamIrepFoTrustedUser = (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoCOSA->GetFolder(
            (ANSC_HANDLE)pPoamIrepFoCOSA,
            COSA_IREP_FOLDER_NAME_PARENTALCONTROL);
    if (!pPoamIrepFoTrustedUser)
    {
        pPoamIrepFoTrustedUser = 
            pPoamIrepFoCOSA->AddFolder(
                    (ANSC_HANDLE)pPoamIrepFoCOSA,
                    COSA_IREP_FOLDER_NAME_PARENTALCONTROL,
                    0);
    }

    if (!pPoamIrepFoTrustedUser)
        return ANSC_STATUS_FAILURE;
    else
        pParCtrl->hIrepFolderTrustedUser = pPoamIrepFoTrustedUser;

    pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoTrustedUser->GetRecord(
            (ANSC_HANDLE)pPoamIrepFoTrustedUser,
            COSA_DML_RR_NAME_PARENTALCONTROL_NextInsNumber,
            NULL);
    if (pSlapVariable)
    {
        pParCtrl->ulTrustedUserNextInsNum = pSlapVariable->Variant.varUint32;
        SlapFreeVariable(pSlapVariable);
    }

    for (ulTrustedUserIdx = 0; ulTrustedUserIdx < ulTrustedUserCnt; ulTrustedUserIdx++)
    {
        pTrustedUser = AnscAllocateMemory(sizeof(COSA_DML_TRUSTEDUSER));
        if (!pTrustedUser)
            return ANSC_STATUS_FAILURE;

        if (CosaDmlTrustedUser_GetEntryByIndex(ulTrustedUserIdx, pTrustedUser) != ANSC_STATUS_SUCCESS)
        {
            CcspTraceError(("%s: CosaDmlTrustedUser_GetEntryByIndex error\n", __FUNCTION__));
            AnscFreeMemory(pTrustedUser);
            return ANSC_STATUS_FAILURE;
        }

        pTrustedUserLinkObj = AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
        if (!pTrustedUserLinkObj)
        {
            AnscFreeMemory(pTrustedUser);
            return ANSC_STATUS_FAILURE;
        }

        if (pTrustedUser->InstanceNumber != 0)
        {
            if (pParCtrl->ulTrustedUserNextInsNum <= pTrustedUser->InstanceNumber)
            {
                pParCtrl->ulTrustedUserNextInsNum = pTrustedUser->InstanceNumber + 1;
                if (pParCtrl->ulTrustedUserNextInsNum == 0)
                {
                    pParCtrl->ulTrustedUserNextInsNum = 1;
                }
            }
        }
        else
        {
            pTrustedUser->InstanceNumber = pParCtrl->ulTrustedUserNextInsNum;

            pParCtrl->ulTrustedUserNextInsNum++;
            if (pParCtrl->ulTrustedUserNextInsNum == 0)
            {
                pParCtrl->ulTrustedUserNextInsNum = 1;
            }

            rc = sprintf_s(pTrustedUser->Alias, sizeof(pTrustedUser->Alias),"cpe-TrustedUser-%d", (int)pTrustedUser->InstanceNumber);
            if(rc < EOK)
            {
              ERR_CHK(rc);
              AnscFreeMemory(pTrustedUser);
              AnscFreeMemory(pTrustedUserLinkObj);
              return ANSC_STATUS_FAILURE;
            }

            CosaDmlTrustedUser_SetValues(ulTrustedUserIdx, pTrustedUser->InstanceNumber, pTrustedUser->Alias);
        }

        pTrustedUserLinkObj->InstanceNumber  = pTrustedUser->InstanceNumber;
        pTrustedUserLinkObj->hContext        = pTrustedUser;
        pTrustedUserLinkObj->hParentTable    = NULL;
        pTrustedUserLinkObj->bNew            = FALSE;

        CosaSListPushEntryByInsNum(&pParCtrl->TrustedUserList, pTrustedUserLinkObj);
    }

    CosaPcReg_TrustedUserGetInfo((ANSC_HANDLE)pParCtrl);

    /* ManagedServices.Service.{i}. */
    ulMSServCnt = CosaDmlMSServ_GetNumberOfEntries();
	#ifdef UTC_ENABLE
	if(bMigration == TRUE)
	{
		AnscTraceWarning(("%s -- Call CosaDmlMSServ_Migration ...\n", __FUNCTION__));		
		if (ANSC_STATUS_SUCCESS == CosaDmlMSServ_Migration())
		{
			AnscTraceWarning(("%s -- Success CosaDmlMSServ_Migration ...\n", __FUNCTION__));
			ulMSServCnt = CosaDmlMSServ_GetNumberOfEntries();
		}
		else
		{
			AnscTraceWarning(("%s -- Failed CosaDmlMSServ_Migration ...\n", __FUNCTION__));
		}
		
	}
	#endif
    AnscSListInitializeHeader(&pParCtrl->MSServList);
    pParCtrl->ulMSServNextInsNum = 1;
    pParCtrl->hIrepFolderCOSA = g_GetRegistryRootFolder(g_pDslhDmlAgent);

    pPoamIrepFoCOSA = (PPOAM_IREP_FOLDER_OBJECT)pParCtrl->hIrepFolderCOSA;
    if (!pPoamIrepFoCOSA)
    {
        return ANSC_STATUS_FAILURE;
    }
    pPoamIrepFoMSServ = (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoCOSA->GetFolder(
            (ANSC_HANDLE)pPoamIrepFoCOSA,
            COSA_IREP_FOLDER_NAME_PARENTALCONTROL);
    if (!pPoamIrepFoMSServ)
    {
        pPoamIrepFoMSServ = 
            pPoamIrepFoCOSA->AddFolder(
                    (ANSC_HANDLE)pPoamIrepFoCOSA,
                    COSA_IREP_FOLDER_NAME_PARENTALCONTROL,
                    0);
    }

    if (!pPoamIrepFoMSServ)
        return ANSC_STATUS_FAILURE;
    else
        pParCtrl->hIrepFolderMSServ = pPoamIrepFoMSServ;

    pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoMSServ->GetRecord(
            (ANSC_HANDLE)pPoamIrepFoMSServ,
            COSA_DML_RR_NAME_PARENTALCONTROL_NextInsNumber,
            NULL);
    if (pSlapVariable)
    {
        pParCtrl->ulMSServNextInsNum = pSlapVariable->Variant.varUint32;
        SlapFreeVariable(pSlapVariable);
    }

    for (ulMSServIdx = 0; ulMSServIdx < ulMSServCnt; ulMSServIdx++)
    {
        pMSServ = AnscAllocateMemory(sizeof(COSA_DML_MS_SERV));
        if (!pMSServ)
            return ANSC_STATUS_FAILURE;

        if (CosaDmlMSServ_GetEntryByIndex(ulMSServIdx, pMSServ) != ANSC_STATUS_SUCCESS)
        {
            CcspTraceError(("%s: CosaDmlMSServ_GetEntryByIndex error\n", __FUNCTION__));
            AnscFreeMemory(pMSServ);
            return ANSC_STATUS_FAILURE;
        }

        pMSServLinkObj = AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
        if (!pMSServLinkObj)
        {
            AnscFreeMemory(pMSServ);
            return ANSC_STATUS_FAILURE;
        }

        if (pMSServ->InstanceNumber != 0)
        {
            if (pParCtrl->ulMSServNextInsNum <= pMSServ->InstanceNumber)
            {
                pParCtrl->ulMSServNextInsNum = pMSServ->InstanceNumber + 1;
                if (pParCtrl->ulMSServNextInsNum == 0)
                {
                    pParCtrl->ulMSServNextInsNum = 1;
                }
            }
        }
        else
        {
            pMSServ->InstanceNumber = pParCtrl->ulMSServNextInsNum;

            pParCtrl->ulMSServNextInsNum++;
            if (pParCtrl->ulMSServNextInsNum == 0)
            {
                pParCtrl->ulMSServNextInsNum = 1;
            }

            rc = sprintf_s(pMSServ->Alias, sizeof(pMSServ->Alias),"cpe-MSService-%d", (int)pMSServ->InstanceNumber);
            if(rc < EOK)
            {
              ERR_CHK(rc);
              AnscFreeMemory(pMSServ);
              AnscFreeMemory(pMSServLinkObj);
              return ANSC_STATUS_FAILURE;
            }
            CosaDmlMSServ_SetValues(ulMSServIdx, pMSServ->InstanceNumber, pMSServ->Alias);
        }

        pMSServLinkObj->InstanceNumber  = pMSServ->InstanceNumber;
        pMSServLinkObj->hContext        = pMSServ;
        pMSServLinkObj->hParentTable    = NULL;
        pMSServLinkObj->bNew            = FALSE;

        CosaSListPushEntryByInsNum(&pParCtrl->MSServList, pMSServLinkObj);
    }

    CosaPcReg_MSServGetInfo((ANSC_HANDLE)pParCtrl);

    /* ManagedServices.TrustedUser.{i}. */
    ulMSTrustedUserCnt = CosaDmlMSTrustedUser_GetNumberOfEntries();

    AnscSListInitializeHeader(&pParCtrl->MSTrustedUserList);
    pParCtrl->ulMSTrustedUserNextInsNum = 1;
    pParCtrl->hIrepFolderCOSA = g_GetRegistryRootFolder(g_pDslhDmlAgent);

    pPoamIrepFoCOSA = (PPOAM_IREP_FOLDER_OBJECT)pParCtrl->hIrepFolderCOSA;
    if (!pPoamIrepFoCOSA)
    {
        return ANSC_STATUS_FAILURE;
    }
    pPoamIrepFoMSTrustedUser = (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoCOSA->GetFolder(
            (ANSC_HANDLE)pPoamIrepFoCOSA,
            COSA_IREP_FOLDER_NAME_PARENTALCONTROL);
    if (!pPoamIrepFoMSTrustedUser)
    {
        pPoamIrepFoMSTrustedUser = 
            pPoamIrepFoCOSA->AddFolder(
                    (ANSC_HANDLE)pPoamIrepFoCOSA,
                    COSA_IREP_FOLDER_NAME_PARENTALCONTROL,
                    0);
    }

    if (!pPoamIrepFoMSTrustedUser)
        return ANSC_STATUS_FAILURE;
    else
        pParCtrl->hIrepFolderMSTrustedUser = pPoamIrepFoMSTrustedUser;

    pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoMSTrustedUser->GetRecord(
            (ANSC_HANDLE)pPoamIrepFoMSTrustedUser,
            COSA_DML_RR_NAME_PARENTALCONTROL_NextInsNumber,
            NULL);
    if (pSlapVariable)
    {
        pParCtrl->ulMSTrustedUserNextInsNum = pSlapVariable->Variant.varUint32;
        SlapFreeVariable(pSlapVariable);
    }

    for (ulMSTrustedUserIdx = 0; ulMSTrustedUserIdx < ulMSTrustedUserCnt; ulMSTrustedUserIdx++)
    {
        pMSTrustedUser = AnscAllocateMemory(sizeof(COSA_DML_MS_TRUSTEDUSER));
        if (!pMSTrustedUser)
            return ANSC_STATUS_FAILURE;

        if (CosaDmlMSTrustedUser_GetEntryByIndex(ulMSTrustedUserIdx, pMSTrustedUser) != ANSC_STATUS_SUCCESS)
        {
            CcspTraceError(("%s: CosaDmlMSTrustedUser_GetEntryByIndex error\n", __FUNCTION__));
            AnscFreeMemory(pMSTrustedUser);
            return ANSC_STATUS_FAILURE;
        }

        pMSTrustedUserLinkObj = AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
        if (!pMSTrustedUserLinkObj)
        {
            AnscFreeMemory(pMSTrustedUser);
            return ANSC_STATUS_FAILURE;
        }

        if (pMSTrustedUser->InstanceNumber != 0)
        {
            if (pParCtrl->ulMSTrustedUserNextInsNum <= pMSTrustedUser->InstanceNumber)
            {
                pParCtrl->ulMSTrustedUserNextInsNum = pMSTrustedUser->InstanceNumber + 1;
                if (pParCtrl->ulMSTrustedUserNextInsNum == 0)
                {
                    pParCtrl->ulMSTrustedUserNextInsNum = 1;
                }
            }
        }
        else
        {
            pMSTrustedUser->InstanceNumber = pParCtrl->ulMSTrustedUserNextInsNum;

            pParCtrl->ulMSTrustedUserNextInsNum++;
            if (pParCtrl->ulMSTrustedUserNextInsNum == 0)
            {
                pParCtrl->ulMSTrustedUserNextInsNum = 1;
            }

            rc = sprintf_s(pMSTrustedUser->Alias, sizeof(pMSTrustedUser->Alias),"cpe-MSTrustedUser-%d", (int)pMSTrustedUser->InstanceNumber);
            if(rc < EOK)
            {
              ERR_CHK(rc);
              AnscFreeMemory(pMSTrustedUser);
              AnscFreeMemory(pMSTrustedUserLinkObj);
              return ANSC_STATUS_FAILURE;
            }
            CosaDmlMSTrustedUser_SetValues(ulMSTrustedUserIdx, pMSTrustedUser->InstanceNumber, pMSTrustedUser->Alias);
        }

        pMSTrustedUserLinkObj->InstanceNumber  = pMSTrustedUser->InstanceNumber;
        pMSTrustedUserLinkObj->hContext        = pMSTrustedUser;
        pMSTrustedUserLinkObj->hParentTable    = NULL;
        pMSTrustedUserLinkObj->bNew            = FALSE;

        CosaSListPushEntryByInsNum(&pParCtrl->MSTrustedUserList, pMSTrustedUserLinkObj);
    }

    CosaPcReg_MSTrustedUserGetInfo((ANSC_HANDLE)pParCtrl);

    /* ManagedDevices.Device.{i}. */
    ulMDDevCnt = CosaDmlMDDev_GetNumberOfEntries();
	#ifdef UTC_ENABLE
	if(bMigration == TRUE)
	{
		AnscTraceWarning(("%s -- Call CosaDmlMDDev_Migration ...\n", __FUNCTION__));
		printf("%s -- call CosaDmlMDDev_Migration ...\n", __FUNCTION__);
		
		if (ANSC_STATUS_SUCCESS == CosaDmlMDDev_Migration())
		{
			AnscTraceWarning(("%s -- Success CosaDmlMDDev_Migration ...\n", __FUNCTION__));
			ulMDDevCnt = CosaDmlMDDev_GetNumberOfEntries();
		}
		else
		{
			AnscTraceWarning(("%s -- Failed CosaDmlMDDev_Migration ...\n", __FUNCTION__));
		}
		
	}
	#endif
    AnscSListInitializeHeader(&pParCtrl->MDDevList);
    pParCtrl->ulMDDevNextInsNum = 1;
    pParCtrl->hIrepFolderCOSA = g_GetRegistryRootFolder(g_pDslhDmlAgent);

    pPoamIrepFoCOSA = (PPOAM_IREP_FOLDER_OBJECT)pParCtrl->hIrepFolderCOSA;
    if (!pPoamIrepFoCOSA)
    {
        return ANSC_STATUS_FAILURE;
    }
    pPoamIrepFoMDDev = (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoCOSA->GetFolder(
            (ANSC_HANDLE)pPoamIrepFoCOSA,
            COSA_IREP_FOLDER_NAME_PARENTALCONTROL);
    if (!pPoamIrepFoMDDev)
    {
        pPoamIrepFoMDDev = 
            pPoamIrepFoCOSA->AddFolder(
                    (ANSC_HANDLE)pPoamIrepFoCOSA,
                    COSA_IREP_FOLDER_NAME_PARENTALCONTROL,
                    0);
    }

    if (!pPoamIrepFoMDDev)
        return ANSC_STATUS_FAILURE;
    else
        pParCtrl->hIrepFolderMDDev = pPoamIrepFoMDDev;

    pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoMDDev->GetRecord(
            (ANSC_HANDLE)pPoamIrepFoMDDev,
            COSA_DML_RR_NAME_PARENTALCONTROL_NextInsNumber,
            NULL);
    if (pSlapVariable)
    {
        pParCtrl->ulMDDevNextInsNum = pSlapVariable->Variant.varUint32;
        SlapFreeVariable(pSlapVariable);
    }

    for (ulMDDevIdx = 0; ulMDDevIdx < ulMDDevCnt; ulMDDevIdx++)
    {
        pMDDev = AnscAllocateMemory(sizeof(COSA_DML_MD_DEV));
        if (!pMDDev)
            return ANSC_STATUS_FAILURE;

        if (CosaDmlMDDev_GetEntryByIndex(ulMDDevIdx, pMDDev) != ANSC_STATUS_SUCCESS)
        {
            CcspTraceError(("%s: CosaDmlMDDev_GetEntryByIndex error\n", __FUNCTION__));
            AnscFreeMemory(pMDDev);
            return ANSC_STATUS_FAILURE;
        }

        pMDDevLinkObj = AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
        if (!pMDDevLinkObj)
        {
            AnscFreeMemory(pMDDev);
            return ANSC_STATUS_FAILURE;
        }

        if (pMDDev->InstanceNumber != 0)
        {
            if (pParCtrl->ulMDDevNextInsNum <= pMDDev->InstanceNumber)
            {
                pParCtrl->ulMDDevNextInsNum = pMDDev->InstanceNumber + 1;
                if (pParCtrl->ulMDDevNextInsNum == 0)
                {
                    pParCtrl->ulMDDevNextInsNum = 1;
                }
            }
        }
        else
        {
            pMDDev->InstanceNumber = pParCtrl->ulMDDevNextInsNum;

            pParCtrl->ulMDDevNextInsNum++;
            if (pParCtrl->ulMDDevNextInsNum == 0)
            {
                pParCtrl->ulMDDevNextInsNum = 1;
            }

            rc = sprintf_s(pMDDev->Alias, sizeof(pMDDev->Alias),"cpe-MDDevice-%d", (int)pMDDev->InstanceNumber);
            if(rc < EOK)
            {
              ERR_CHK(rc);
              AnscFreeMemory(pMDDev);
              AnscFreeMemory(pMDDevLinkObj);
              return ANSC_STATUS_FAILURE;
            }
            CosaDmlMDDev_SetValues(ulMDDevIdx, pMDDev->InstanceNumber, pMDDev->Alias);
        }

        pMDDevLinkObj->InstanceNumber  = pMDDev->InstanceNumber;
        pMDDevLinkObj->hContext        = pMDDev;
        pMDDevLinkObj->hParentTable    = NULL;
        pMDDevLinkObj->bNew            = FALSE;

        CosaSListPushEntryByInsNum(&pParCtrl->MDDevList, pMDDevLinkObj);
    }

    CosaPcReg_MDDevGetInfo((ANSC_HANDLE)pParCtrl);
	#ifdef UTC_ENABLE
    bMigration = FALSE;
    system("touch /nvram/UTC_ENABLE");
	#endif

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaParentalControlRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_PARENTALCONTROL   pParCtrl    = (PCOSA_DATAMODEL_PARENTALCONTROL)hThisObject;

    /* Remove necessary resounce */


    /* Remove self */
    AnscFreeMemory((ANSC_HANDLE)pParCtrl);

    return returnStatus;
}

ANSC_STATUS
CosaPcReg_BlkUrlGetInfo(
        ANSC_HANDLE                 hThisObject
    )
{
    PCOSA_DATAMODEL_PARENTALCONTROL   pMyObject               = (PCOSA_DATAMODEL_PARENTALCONTROL   )hThisObject;
    PSLIST_HEADER                   pListHead               = (PSLIST_HEADER            )&pMyObject->BlkUrlList;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoBlkUrl           = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderBlkUrl;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoBlkUrlSp         = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    COSA_DML_BLOCKEDURL        *pEntry                  = (COSA_DML_BLOCKEDURL    *)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSLAP_VARIABLE                  pSlapVariable           = (PSLAP_VARIABLE           )NULL;
    ULONG                           ulEntryCount            = 0;
    ULONG                           ulIndex                 = 0;
    ULONG                           ulInstanceNumber        = 0;
    char*                           pFolderName             = NULL;
    char*                           pAlias                  = NULL;
    errno_t                         rc                      = -1;

    if ( !pPoamIrepFoBlkUrl )
    {
        return ANSC_STATUS_FAILURE;
    }

    /* Load the newly added but not yet commited entries */
    
    ulEntryCount = pPoamIrepFoBlkUrl->GetFolderCount((ANSC_HANDLE)pPoamIrepFoBlkUrl);

    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        pFolderName =
            pPoamIrepFoBlkUrl->EnumFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoBlkUrl,
                    ulIndex
                );

        if ( !pFolderName )
        {
            continue;
        }

        pPoamIrepFoBlkUrlSp = pPoamIrepFoBlkUrl->GetFolder((ANSC_HANDLE)pPoamIrepFoBlkUrl, pFolderName);

        AnscFreeMemory(pFolderName);

        if ( !pPoamIrepFoBlkUrlSp )
        {
            continue;
        }

        if ( TRUE )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoBlkUrlSp->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoBlkUrlSp,
                        COSA_DML_RR_NAME_PARENTALCONTROL_InsNum,
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
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoBlkUrlSp->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoBlkUrlSp,
                        COSA_DML_RR_NAME_PARENTALCONTROL_Alias,
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
            AnscFreeMemory(pAlias);

            return ANSC_STATUS_RESOURCES;
        }

        pEntry = (COSA_DML_BLOCKEDURL*)AnscAllocateMemory(sizeof(COSA_DML_BLOCKEDURL));

        if ( !pEntry )
        {
            AnscFreeMemory(pAlias);
            AnscFreeMemory(pCosaContext);

            return ANSC_STATUS_RESOURCES;
        }

        rc = STRCPY_S_NOCLOBBER(pEntry->Alias, sizeof(pEntry->Alias), pAlias ? pAlias : "");
        ERR_CHK(rc);

        pEntry->InstanceNumber = ulInstanceNumber;

        pCosaContext->InstanceNumber        = ulInstanceNumber;
        pCosaContext->bNew                  = TRUE;
        pCosaContext->hContext              = (ANSC_HANDLE)pEntry;
        pCosaContext->hParentTable          = NULL;
        pCosaContext->hPoamIrepUpperFo      = (ANSC_HANDLE)pPoamIrepFoBlkUrl;
        pCosaContext->hPoamIrepFo           = (ANSC_HANDLE)pPoamIrepFoBlkUrlSp;

        CosaSListPushEntryByInsNum(pListHead, pCosaContext);

        if ( pAlias )
        {
            AnscFreeMemory(pAlias);

            pAlias = NULL;
        }
    }
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaPcReg_BlkUrlAddInfo(
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    )
{
    ANSC_STATUS                     returnStatus            = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_PARENTALCONTROL   pMyObject               = (PCOSA_DATAMODEL_PARENTALCONTROL   )hThisObject;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoBlkUrl           = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderBlkUrl;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoBlkUrlSp         = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)hCosaContext;
    COSA_DML_BLOCKEDURL        *pEntry                  = (COSA_DML_BLOCKEDURL*      )pCosaContext->hContext;
    PSLAP_VARIABLE                  pSlapVariable           = (PSLAP_VARIABLE           )NULL;

    if ( !pPoamIrepFoBlkUrl )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pPoamIrepFoBlkUrl->EnableFileSync((ANSC_HANDLE)pPoamIrepFoBlkUrl, FALSE);
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

    if ( TRUE )
    {
        returnStatus = 
            pPoamIrepFoBlkUrl->DelRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoBlkUrl,
                    COSA_DML_RR_NAME_PARENTALCONTROL_NextInsNumber
                );

        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
        pSlapVariable->Variant.varUint32 = pMyObject->ulBlkUrlNextInsNum;

        returnStatus =
            pPoamIrepFoBlkUrl->AddRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoBlkUrl,
                    COSA_DML_RR_NAME_PARENTALCONTROL_NextInsNumber,
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
        pPoamIrepFoBlkUrlSp =
            pPoamIrepFoBlkUrl->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoBlkUrl,
                    pEntry->Alias,
                    0
                );

        if ( !pPoamIrepFoBlkUrlSp )
        {
            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT1;
        }

        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
            pSlapVariable->Variant.varUint32 = pEntry->InstanceNumber;

            returnStatus =
                pPoamIrepFoBlkUrlSp->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoBlkUrlSp,
                        COSA_DML_RR_NAME_PARENTALCONTROL_InsNum,
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
            pSlapVariable->Variant.varString = AnscCloneString(pEntry->Alias);

            returnStatus =
                pPoamIrepFoBlkUrlSp->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoBlkUrlSp,
                        COSA_DML_RR_NAME_PARENTALCONTROL_Alias,
                        SYS_REP_RECORD_TYPE_ASTR,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );

            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }

        pCosaContext->hPoamIrepUpperFo = (ANSC_HANDLE)pPoamIrepFoBlkUrl;
        pCosaContext->hPoamIrepFo      = (ANSC_HANDLE)pPoamIrepFoBlkUrlSp;
    }

EXIT1:
    
    if ( pSlapVariable )
    {
        SlapFreeVariable(pSlapVariable);
    }

    pPoamIrepFoBlkUrl->EnableFileSync((ANSC_HANDLE)pPoamIrepFoBlkUrl, TRUE);

    return returnStatus;
}

ANSC_STATUS
CosaPcReg_BlkUrlDelInfo(
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

        AnscFreeMemory(pPoamIrepFo);
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaPcReg_TrustedUserGetInfo(
        ANSC_HANDLE                 hThisObject
    )
{
    PCOSA_DATAMODEL_PARENTALCONTROL   pMyObject               = (PCOSA_DATAMODEL_PARENTALCONTROL   )hThisObject;
    PSLIST_HEADER                   pListHead               = (PSLIST_HEADER            )&pMyObject->TrustedUserList;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoTrustedUser           = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderTrustedUser;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoTrustedUserSp         = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    COSA_DML_TRUSTEDUSER        *pEntry                  = (COSA_DML_TRUSTEDUSER    *)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSLAP_VARIABLE                  pSlapVariable           = (PSLAP_VARIABLE           )NULL;
    ULONG                           ulEntryCount            = 0;
    ULONG                           ulIndex                 = 0;
    ULONG                           ulInstanceNumber        = 0;
    char*                           pFolderName             = NULL;
    char*                           pAlias                  = NULL;
    errno_t                         rc                      = -1;

    if ( !pPoamIrepFoTrustedUser )
    {
        return ANSC_STATUS_FAILURE;
    }

    /* Load the newly added but not yet commited entries */
    
    ulEntryCount = pPoamIrepFoTrustedUser->GetFolderCount((ANSC_HANDLE)pPoamIrepFoTrustedUser);

    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        pFolderName =
            pPoamIrepFoTrustedUser->EnumFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoTrustedUser,
                    ulIndex
                );

        if ( !pFolderName )
        {
            continue;
        }

        pPoamIrepFoTrustedUserSp = pPoamIrepFoTrustedUser->GetFolder((ANSC_HANDLE)pPoamIrepFoTrustedUser, pFolderName);

        AnscFreeMemory(pFolderName);

        if ( !pPoamIrepFoTrustedUserSp )
        {
            continue;
        }

        if ( TRUE )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoTrustedUserSp->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoTrustedUserSp,
                        COSA_DML_RR_NAME_PARENTALCONTROL_InsNum,
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
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoTrustedUserSp->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoTrustedUserSp,
                        COSA_DML_RR_NAME_PARENTALCONTROL_Alias,
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
            AnscFreeMemory(pAlias);

            return ANSC_STATUS_RESOURCES;
        }

        pEntry = (COSA_DML_TRUSTEDUSER*)AnscAllocateMemory(sizeof(COSA_DML_TRUSTEDUSER));

        if ( !pEntry )
        {
            AnscFreeMemory(pAlias);
            AnscFreeMemory(pCosaContext);

            return ANSC_STATUS_RESOURCES;
        }

        rc = STRCPY_S_NOCLOBBER(pEntry->Alias, sizeof(pEntry->Alias), pAlias ? pAlias : "");
        ERR_CHK(rc);

        pEntry->InstanceNumber = ulInstanceNumber;

        pCosaContext->InstanceNumber        = ulInstanceNumber;
        pCosaContext->bNew                  = TRUE;
        pCosaContext->hContext              = (ANSC_HANDLE)pEntry;
        pCosaContext->hParentTable          = NULL;
        pCosaContext->hPoamIrepUpperFo      = (ANSC_HANDLE)pPoamIrepFoTrustedUser;
        pCosaContext->hPoamIrepFo           = (ANSC_HANDLE)pPoamIrepFoTrustedUserSp;

        CosaSListPushEntryByInsNum(pListHead, pCosaContext);

        if ( pAlias )
        {
            AnscFreeMemory(pAlias);

            pAlias = NULL;
        }
    }
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaPcReg_TrustedUserAddInfo(
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    )
{
    ANSC_STATUS                     returnStatus            = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_PARENTALCONTROL   pMyObject               = (PCOSA_DATAMODEL_PARENTALCONTROL   )hThisObject;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoTrustedUser           = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderTrustedUser;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoTrustedUserSp         = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)hCosaContext;
    COSA_DML_TRUSTEDUSER        *pEntry                  = (COSA_DML_TRUSTEDUSER*      )pCosaContext->hContext;
    PSLAP_VARIABLE                  pSlapVariable           = (PSLAP_VARIABLE           )NULL;

    if ( !pPoamIrepFoTrustedUser )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pPoamIrepFoTrustedUser->EnableFileSync((ANSC_HANDLE)pPoamIrepFoTrustedUser, FALSE);
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

    if ( TRUE )
    {
        returnStatus = 
            pPoamIrepFoTrustedUser->DelRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoTrustedUser,
                    COSA_DML_RR_NAME_PARENTALCONTROL_NextInsNumber
                );

        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
        pSlapVariable->Variant.varUint32 = pMyObject->ulTrustedUserNextInsNum;

        returnStatus =
            pPoamIrepFoTrustedUser->AddRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoTrustedUser,
                    COSA_DML_RR_NAME_PARENTALCONTROL_NextInsNumber,
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
        pPoamIrepFoTrustedUserSp =
            pPoamIrepFoTrustedUser->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoTrustedUser,
                    pEntry->Alias,
                    0
                );

        if ( !pPoamIrepFoTrustedUserSp )
        {
            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT1;
        }

        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
            pSlapVariable->Variant.varUint32 = pEntry->InstanceNumber;

            returnStatus =
                pPoamIrepFoTrustedUserSp->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoTrustedUserSp,
                        COSA_DML_RR_NAME_PARENTALCONTROL_InsNum,
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
            pSlapVariable->Variant.varString = AnscCloneString(pEntry->Alias);

            returnStatus =
                pPoamIrepFoTrustedUserSp->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoTrustedUserSp,
                        COSA_DML_RR_NAME_PARENTALCONTROL_Alias,
                        SYS_REP_RECORD_TYPE_ASTR,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );

            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }

        pCosaContext->hPoamIrepUpperFo = (ANSC_HANDLE)pPoamIrepFoTrustedUser;
        pCosaContext->hPoamIrepFo      = (ANSC_HANDLE)pPoamIrepFoTrustedUserSp;
    }

EXIT1:
    
    if ( pSlapVariable )
    {
        SlapFreeVariable(pSlapVariable);
    }

    pPoamIrepFoTrustedUser->EnableFileSync((ANSC_HANDLE)pPoamIrepFoTrustedUser, TRUE);

    return returnStatus;
}

ANSC_STATUS
CosaPcReg_TrustedUserDelInfo(
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

        AnscFreeMemory(pPoamIrepFo);
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaPcReg_MSServGetInfo(
        ANSC_HANDLE                 hThisObject
    )
{
    PCOSA_DATAMODEL_PARENTALCONTROL   pMyObject               = (PCOSA_DATAMODEL_PARENTALCONTROL   )hThisObject;
    PSLIST_HEADER                   pListHead               = (PSLIST_HEADER            )&pMyObject->MSServList;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoMSServ           = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderMSServ;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoMSServSp         = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    COSA_DML_MS_SERV        *pEntry                  = (COSA_DML_MS_SERV    *)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSLAP_VARIABLE                  pSlapVariable           = (PSLAP_VARIABLE           )NULL;
    ULONG                           ulEntryCount            = 0;
    ULONG                           ulIndex                 = 0;
    ULONG                           ulInstanceNumber        = 0;
    char*                           pFolderName             = NULL;
    char*                           pAlias                  = NULL;
    errno_t                         rc                      = -1;

    if ( !pPoamIrepFoMSServ )
    {
        return ANSC_STATUS_FAILURE;
    }

    /* Load the newly added but not yet commited entries */
    
    ulEntryCount = pPoamIrepFoMSServ->GetFolderCount((ANSC_HANDLE)pPoamIrepFoMSServ);

    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        pFolderName =
            pPoamIrepFoMSServ->EnumFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoMSServ,
                    ulIndex
                );

        if ( !pFolderName )
        {
            continue;
        }

        pPoamIrepFoMSServSp = pPoamIrepFoMSServ->GetFolder((ANSC_HANDLE)pPoamIrepFoMSServ, pFolderName);

        AnscFreeMemory(pFolderName);

        if ( !pPoamIrepFoMSServSp )
        {
            continue;
        }

        if ( TRUE )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoMSServSp->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoMSServSp,
                        COSA_DML_RR_NAME_MSSERV_InsNum,
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
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoMSServSp->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoMSServSp,
                        COSA_DML_RR_NAME_MSSERV_Alias,
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
            AnscFreeMemory(pAlias);

            return ANSC_STATUS_RESOURCES;
        }

        pEntry = (COSA_DML_MS_SERV*)AnscAllocateMemory(sizeof(COSA_DML_MS_SERV));

        if ( !pEntry )
        {
            AnscFreeMemory(pAlias);
            AnscFreeMemory(pCosaContext);

            return ANSC_STATUS_RESOURCES;
        }

        rc = STRCPY_S_NOCLOBBER(pEntry->Alias, sizeof(pEntry->Alias), pAlias ? pAlias : "");
        ERR_CHK(rc);

        pEntry->InstanceNumber = ulInstanceNumber;

        pCosaContext->InstanceNumber        = ulInstanceNumber;
        pCosaContext->bNew                  = TRUE;
        pCosaContext->hContext              = (ANSC_HANDLE)pEntry;
        pCosaContext->hParentTable          = NULL;
        pCosaContext->hPoamIrepUpperFo      = (ANSC_HANDLE)pPoamIrepFoMSServ;
        pCosaContext->hPoamIrepFo           = (ANSC_HANDLE)pPoamIrepFoMSServSp;

        CosaSListPushEntryByInsNum(pListHead, pCosaContext);

        if ( pAlias )
        {
            AnscFreeMemory(pAlias);

            pAlias = NULL;
        }
    }
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaPcReg_MSServAddInfo(
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    )
{
    ANSC_STATUS                     returnStatus            = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_PARENTALCONTROL   pMyObject               = (PCOSA_DATAMODEL_PARENTALCONTROL   )hThisObject;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoMSServ           = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderMSServ;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoMSServSp         = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)hCosaContext;
    COSA_DML_MS_SERV        *pEntry                  = (COSA_DML_MS_SERV*      )pCosaContext->hContext;
    PSLAP_VARIABLE                  pSlapVariable           = (PSLAP_VARIABLE           )NULL;

    if ( !pPoamIrepFoMSServ )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pPoamIrepFoMSServ->EnableFileSync((ANSC_HANDLE)pPoamIrepFoMSServ, FALSE);
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

    if ( TRUE )
    {
        returnStatus = 
            pPoamIrepFoMSServ->DelRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoMSServ,
                    COSA_DML_RR_NAME_MSSERV_NextInsNumber
                );

        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
        pSlapVariable->Variant.varUint32 = pMyObject->ulMSServNextInsNum;

        returnStatus =
            pPoamIrepFoMSServ->AddRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoMSServ,
                    COSA_DML_RR_NAME_MSSERV_NextInsNumber,
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
        pPoamIrepFoMSServSp =
            pPoamIrepFoMSServ->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoMSServ,
                    pEntry->Alias,
                    0
                );

        if ( !pPoamIrepFoMSServSp )
        {
            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT1;
        }

        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
            pSlapVariable->Variant.varUint32 = pEntry->InstanceNumber;

            returnStatus =
                pPoamIrepFoMSServSp->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoMSServSp,
                        COSA_DML_RR_NAME_MSSERV_InsNum,
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
            pSlapVariable->Variant.varString = AnscCloneString(pEntry->Alias);

            returnStatus =
                pPoamIrepFoMSServSp->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoMSServSp,
                        COSA_DML_RR_NAME_MSSERV_Alias,
                        SYS_REP_RECORD_TYPE_ASTR,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );

            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }

        pCosaContext->hPoamIrepUpperFo = (ANSC_HANDLE)pPoamIrepFoMSServ;
        pCosaContext->hPoamIrepFo      = (ANSC_HANDLE)pPoamIrepFoMSServSp;
    }

EXIT1:
    
    if ( pSlapVariable )
    {
        SlapFreeVariable(pSlapVariable);
    }

    pPoamIrepFoMSServ->EnableFileSync((ANSC_HANDLE)pPoamIrepFoMSServ, TRUE);

    return returnStatus;
}

ANSC_STATUS
CosaPcReg_MSServDelInfo(
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

        AnscFreeMemory(pPoamIrepFo);
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaPcReg_MSTrustedUserGetInfo(
        ANSC_HANDLE                 hThisObject
    )
{
    PCOSA_DATAMODEL_PARENTALCONTROL   pMyObject               = (PCOSA_DATAMODEL_PARENTALCONTROL   )hThisObject;
    PSLIST_HEADER                   pListHead               = (PSLIST_HEADER            )&pMyObject->MSTrustedUserList;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoMSTrustedUser           = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderMSTrustedUser;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoMSTrustedUserSp         = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    COSA_DML_MS_TRUSTEDUSER        *pEntry                  = (COSA_DML_MS_TRUSTEDUSER    *)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSLAP_VARIABLE                  pSlapVariable           = (PSLAP_VARIABLE           )NULL;
    ULONG                           ulEntryCount            = 0;
    ULONG                           ulIndex                 = 0;
    ULONG                           ulInstanceNumber        = 0;
    char*                           pFolderName             = NULL;
    char*                           pAlias                  = NULL;
    errno_t                         rc                      = -1;

    if ( !pPoamIrepFoMSTrustedUser )
    {
        return ANSC_STATUS_FAILURE;
    }

    /* Load the newly added but not yet commited entries */
    
    ulEntryCount = pPoamIrepFoMSTrustedUser->GetFolderCount((ANSC_HANDLE)pPoamIrepFoMSTrustedUser);

    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        pFolderName =
            pPoamIrepFoMSTrustedUser->EnumFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoMSTrustedUser,
                    ulIndex
                );

        if ( !pFolderName )
        {
            continue;
        }

        pPoamIrepFoMSTrustedUserSp = pPoamIrepFoMSTrustedUser->GetFolder((ANSC_HANDLE)pPoamIrepFoMSTrustedUser, pFolderName);

        AnscFreeMemory(pFolderName);

        if ( !pPoamIrepFoMSTrustedUserSp )
        {
            continue;
        }

        if ( TRUE )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoMSTrustedUserSp->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoMSTrustedUserSp,
                        COSA_DML_RR_NAME_MSTRUSTEDUSER_InsNum,
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
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoMSTrustedUserSp->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoMSTrustedUserSp,
                        COSA_DML_RR_NAME_MSTRUSTEDUSER_Alias,
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
            AnscFreeMemory(pAlias);

            return ANSC_STATUS_RESOURCES;
        }

        pEntry = (COSA_DML_MS_TRUSTEDUSER*)AnscAllocateMemory(sizeof(COSA_DML_MS_TRUSTEDUSER));

        if ( !pEntry )
        {
            AnscFreeMemory(pAlias);
            AnscFreeMemory(pCosaContext);

            return ANSC_STATUS_RESOURCES;
        }

        rc = STRCPY_S_NOCLOBBER(pEntry->Alias, sizeof(pEntry->Alias), pAlias ? pAlias : "");
        ERR_CHK(rc);

        pEntry->InstanceNumber = ulInstanceNumber;

        pCosaContext->InstanceNumber        = ulInstanceNumber;
        pCosaContext->bNew                  = TRUE;
        pCosaContext->hContext              = (ANSC_HANDLE)pEntry;
        pCosaContext->hParentTable          = NULL;
        pCosaContext->hPoamIrepUpperFo      = (ANSC_HANDLE)pPoamIrepFoMSTrustedUser;
        pCosaContext->hPoamIrepFo           = (ANSC_HANDLE)pPoamIrepFoMSTrustedUserSp;

        CosaSListPushEntryByInsNum(pListHead, pCosaContext);

        if ( pAlias )
        {
            AnscFreeMemory(pAlias);

            pAlias = NULL;
        }
    }
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaPcReg_MSTrustedUserAddInfo(
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    )
{
    ANSC_STATUS                     returnStatus            = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_PARENTALCONTROL   pMyObject               = (PCOSA_DATAMODEL_PARENTALCONTROL   )hThisObject;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoMSTrustedUser           = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderMSTrustedUser;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoMSTrustedUserSp         = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)hCosaContext;
    COSA_DML_MS_TRUSTEDUSER        *pEntry                  = (COSA_DML_MS_TRUSTEDUSER*      )pCosaContext->hContext;
    PSLAP_VARIABLE                  pSlapVariable           = (PSLAP_VARIABLE           )NULL;

    if ( !pPoamIrepFoMSTrustedUser )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pPoamIrepFoMSTrustedUser->EnableFileSync((ANSC_HANDLE)pPoamIrepFoMSTrustedUser, FALSE);
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

    if ( TRUE )
    {
        returnStatus = 
            pPoamIrepFoMSTrustedUser->DelRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoMSTrustedUser,
                    COSA_DML_RR_NAME_MSTRUSTEDUSER_NextInsNumber
                );

        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
        pSlapVariable->Variant.varUint32 = pMyObject->ulMSTrustedUserNextInsNum;

        returnStatus =
            pPoamIrepFoMSTrustedUser->AddRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoMSTrustedUser,
                    COSA_DML_RR_NAME_MSTRUSTEDUSER_NextInsNumber,
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
        pPoamIrepFoMSTrustedUserSp =
            pPoamIrepFoMSTrustedUser->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoMSTrustedUser,
                    pEntry->Alias,
                    0
                );

        if ( !pPoamIrepFoMSTrustedUserSp )
        {
            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT1;
        }

        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
            pSlapVariable->Variant.varUint32 = pEntry->InstanceNumber;

            returnStatus =
                pPoamIrepFoMSTrustedUserSp->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoMSTrustedUserSp,
                        COSA_DML_RR_NAME_MSTRUSTEDUSER_InsNum,
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
            pSlapVariable->Variant.varString = AnscCloneString(pEntry->Alias);

            returnStatus =
                pPoamIrepFoMSTrustedUserSp->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoMSTrustedUserSp,
                        COSA_DML_RR_NAME_MSTRUSTEDUSER_Alias,
                        SYS_REP_RECORD_TYPE_ASTR,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );

            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }

        pCosaContext->hPoamIrepUpperFo = (ANSC_HANDLE)pPoamIrepFoMSTrustedUser;
        pCosaContext->hPoamIrepFo      = (ANSC_HANDLE)pPoamIrepFoMSTrustedUserSp;
    }

EXIT1:
    
    if ( pSlapVariable )
    {
        SlapFreeVariable(pSlapVariable);
    }

    pPoamIrepFoMSTrustedUser->EnableFileSync((ANSC_HANDLE)pPoamIrepFoMSTrustedUser, TRUE);

    return returnStatus;
}

ANSC_STATUS
CosaPcReg_MSTrustedUserDelInfo(
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

        AnscFreeMemory(pPoamIrepFo);
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaPcReg_MDDevGetInfo(
        ANSC_HANDLE                 hThisObject
    )
{
    PCOSA_DATAMODEL_PARENTALCONTROL   pMyObject               = (PCOSA_DATAMODEL_PARENTALCONTROL   )hThisObject;
    PSLIST_HEADER                   pListHead               = (PSLIST_HEADER            )&pMyObject->MDDevList;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoMDDev           = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderMDDev;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoMDDevSp         = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    COSA_DML_MD_DEV        *pEntry                  = (COSA_DML_MD_DEV    *)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSLAP_VARIABLE                  pSlapVariable           = (PSLAP_VARIABLE           )NULL;
    ULONG                           ulEntryCount            = 0;
    ULONG                           ulIndex                 = 0;
    ULONG                           ulInstanceNumber        = 0;
    char*                           pFolderName             = NULL;
    char*                           pAlias                  = NULL;
    errno_t                         rc                      = -1;

    if ( !pPoamIrepFoMDDev )
    {
        return ANSC_STATUS_FAILURE;
    }

    /* Load the newly added but not yet commited entries */
    
    ulEntryCount = pPoamIrepFoMDDev->GetFolderCount((ANSC_HANDLE)pPoamIrepFoMDDev);

    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        pFolderName =
            pPoamIrepFoMDDev->EnumFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoMDDev,
                    ulIndex
                );

        if ( !pFolderName )
        {
            continue;
        }

        pPoamIrepFoMDDevSp = pPoamIrepFoMDDev->GetFolder((ANSC_HANDLE)pPoamIrepFoMDDev, pFolderName);

        AnscFreeMemory(pFolderName);

        if ( !pPoamIrepFoMDDevSp )
        {
            continue;
        }

        if ( TRUE )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoMDDevSp->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoMDDevSp,
                        COSA_DML_RR_NAME_MDDEV_InsNum,
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
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoMDDevSp->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoMDDevSp,
                        COSA_DML_RR_NAME_MDDEV_Alias,
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
            AnscFreeMemory(pAlias);

            return ANSC_STATUS_RESOURCES;
        }

        pEntry = (COSA_DML_MD_DEV*)AnscAllocateMemory(sizeof(COSA_DML_MD_DEV));

        if ( !pEntry )
        {
            AnscFreeMemory(pAlias);
            AnscFreeMemory(pCosaContext);

            return ANSC_STATUS_RESOURCES;
        }

        rc = STRCPY_S_NOCLOBBER(pEntry->Alias, sizeof(pEntry->Alias), pAlias ? pAlias : "");
        ERR_CHK(rc);

        pEntry->InstanceNumber = ulInstanceNumber;

        pCosaContext->InstanceNumber        = ulInstanceNumber;
        pCosaContext->bNew                  = TRUE;
        pCosaContext->hContext              = (ANSC_HANDLE)pEntry;
        pCosaContext->hParentTable          = NULL;
        pCosaContext->hPoamIrepUpperFo      = (ANSC_HANDLE)pPoamIrepFoMDDev;
        pCosaContext->hPoamIrepFo           = (ANSC_HANDLE)pPoamIrepFoMDDevSp;

        CosaSListPushEntryByInsNum(pListHead, pCosaContext);

        if ( pAlias )
        {
            AnscFreeMemory(pAlias);

            pAlias = NULL;
        }
    }
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaPcReg_MDDevAddInfo(
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    )
{
    ANSC_STATUS                     returnStatus            = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_PARENTALCONTROL   pMyObject               = (PCOSA_DATAMODEL_PARENTALCONTROL   )hThisObject;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoMDDev           = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderMDDev;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoMDDevSp         = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)hCosaContext;
    COSA_DML_MD_DEV        *pEntry                  = (COSA_DML_MD_DEV*      )pCosaContext->hContext;
    PSLAP_VARIABLE                  pSlapVariable           = (PSLAP_VARIABLE           )NULL;

    if ( !pPoamIrepFoMDDev )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pPoamIrepFoMDDev->EnableFileSync((ANSC_HANDLE)pPoamIrepFoMDDev, FALSE);
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

    if ( TRUE )
    {
        returnStatus = 
            pPoamIrepFoMDDev->DelRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoMDDev,
                    COSA_DML_RR_NAME_MDDEV_NextInsNumber
                );

        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
        pSlapVariable->Variant.varUint32 = pMyObject->ulMDDevNextInsNum;

        returnStatus =
            pPoamIrepFoMDDev->AddRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoMDDev,
                    COSA_DML_RR_NAME_MDDEV_NextInsNumber,
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
        pPoamIrepFoMDDevSp =
            pPoamIrepFoMDDev->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoMDDev,
                    pEntry->Alias,
                    0
                );

        if ( !pPoamIrepFoMDDevSp )
        {
            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT1;
        }

        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
            pSlapVariable->Variant.varUint32 = pEntry->InstanceNumber;

            returnStatus =
                pPoamIrepFoMDDevSp->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoMDDevSp,
                        COSA_DML_RR_NAME_MDDEV_InsNum,
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
            pSlapVariable->Variant.varString = AnscCloneString(pEntry->Alias);

            returnStatus =
                pPoamIrepFoMDDevSp->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoMDDevSp,
                        COSA_DML_RR_NAME_MDDEV_Alias,
                        SYS_REP_RECORD_TYPE_ASTR,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );

            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }

        pCosaContext->hPoamIrepUpperFo = (ANSC_HANDLE)pPoamIrepFoMDDev;
        pCosaContext->hPoamIrepFo      = (ANSC_HANDLE)pPoamIrepFoMDDevSp;
    }

EXIT1:
    
    if ( pSlapVariable )
    {
        SlapFreeVariable(pSlapVariable);
    }

    pPoamIrepFoMDDev->EnableFileSync((ANSC_HANDLE)pPoamIrepFoMDDev, TRUE);

    return returnStatus;
}

ANSC_STATUS
CosaPcReg_MDDevDelInfo(
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

        AnscFreeMemory(pPoamIrepFo);
    }

    return ANSC_STATUS_SUCCESS;
}

