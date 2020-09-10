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

    module: cosa_x_cisco_com_devicecontrol_internal.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaDeviceControlCreate
        *  CosaDeviceControlInitialize
        *  CosaDeviceControlRemove
    -------------------------------------------------------------------

    environment:

        platform independent

    -------------------------------------------------------------------

    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        07/15/2011    initial revision.

**************************************************************************/
#include "cosa_apis.h"
#include "cosa_x_cisco_com_devicecontrol_apis.h"
#include "cosa_x_cisco_com_devicecontrol_internal.h"
#include "plugin_main_apis.h"
#include "ccsp_psm_helper.h"

extern void * g_pDslhDmlAgent;

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        CosaDeviceControlCreate
            (
            );

    description:

        This function constructs cosa DeviceControl object and return handle.

    argument:  

    return:     newly created DeviceControl object.

**********************************************************************/

ANSC_HANDLE
CosaDeviceControlCreate
    (
        VOID
    )
{
    ANSC_STATUS                 returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DEVICECONTROL  pMyObject    = (PCOSA_DATAMODEL_DEVICECONTROL)NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PCOSA_DATAMODEL_DEVICECONTROL)AnscAllocateMemory(sizeof(COSA_DATAMODEL_DEVICECONTROL));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->Oid               = COSA_DATAMODEL_DEVICECONTROL_OID;
    pMyObject->Create            = CosaDeviceControlCreate;
    pMyObject->Remove            = CosaDeviceControlRemove;
    pMyObject->Initialize        = CosaDeviceControlInitialize;

    pMyObject->Initialize   ((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDeviceControlInitialize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate  cosa DeviceControl object and return handle.

    argument:   ANSC_HANDLE                 hThisObject
            This handle is actually the pointer of this object
            itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaDeviceControlInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DEVICECONTROL   pMyObject       = (PCOSA_DATAMODEL_DEVICECONTROL)hThisObject;
    ULONG                           ulLmCnt; 
    ULONG                           ulLmIdx;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoCOSA = NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoLm   = NULL;
    PSLAP_VARIABLE                  pSlapVariable   = NULL;
    PCOSA_DML_LAN_MANAGEMENT        pLanMngm        = NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pLmLinkObj      = NULL;

    /* Initiation all functions */
    CosaDmlDcInit(NULL, NULL);

    ulLmCnt = CosaDmlLanMngm_GetNumberOfEntries();

    AnscSListInitializeHeader(&pMyObject->LanMngmList);
    pMyObject->ulLanMngmNextInsNum = 1;
    pMyObject->hIrepFolderCOSA = g_GetRegistryRootFolder(g_pDslhDmlAgent);

    pPoamIrepFoCOSA = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderCOSA;
    if (!pPoamIrepFoCOSA)
    {
        return ANSC_STATUS_FAILURE;
    }
    pPoamIrepFoLm = (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoCOSA->GetFolder(
            (ANSC_HANDLE)pPoamIrepFoCOSA,
            COSA_IREP_FOLDER_NAME_LAN_MANAGEMENT);
    if (!pPoamIrepFoLm)
    {
        pPoamIrepFoLm = 
            pPoamIrepFoCOSA->AddFolder(
                    (ANSC_HANDLE)pPoamIrepFoCOSA,
                    COSA_IREP_FOLDER_NAME_LAN_MANAGEMENT,
                    0);
    }

    if (!pPoamIrepFoLm)
        return ANSC_STATUS_FAILURE;
    else
        pMyObject->hIrepFolderLm = pPoamIrepFoLm;

    /*workaroud for upgrade from 1.3 to 1.6 which may cause core dumped
     * delete the two parameter in PSM when initialize*/
    {
        PSM_Del_Record(g_MessageBusHandle, g_GetSubsystemPrefix(g_pDslhDmlAgent), CCSP_USER_CHANGED_MONITOR_PARAM);
        PSM_Del_Record(g_MessageBusHandle, g_GetSubsystemPrefix(g_pDslhDmlAgent), CCSP_USER_COMPONENTS_PARAM);
    }

    CosaDevCtrlReg_GetUserChangedParamsControl(hThisObject);

    pSlapVariable = (PSLAP_VARIABLE)pPoamIrepFoLm->GetRecord(
            (ANSC_HANDLE)pPoamIrepFoLm,
            COSA_DML_RR_NAME_LAN_MANAGEMENT_NextInsNumber,
            NULL);
    if (pSlapVariable)
    {
        pMyObject->ulLanMngmNextInsNum = pSlapVariable->Variant.varUint32;
        SlapFreeVariable(pSlapVariable);
    }

    for (ulLmIdx = 0; ulLmIdx < ulLmCnt; ulLmIdx++)
    {
        pLanMngm = AnscAllocateMemory(sizeof(COSA_DML_LAN_MANAGEMENT));
        if (!pLanMngm)
            return ANSC_STATUS_FAILURE;

        if (CosaDmlLanMngm_GetEntryByIndex(ulLmIdx, pLanMngm) != ANSC_STATUS_SUCCESS)
        {
            CcspTraceError(("%s: CosaDmlLanMngm_GetEntryByIndex error\n", __FUNCTION__));
            AnscFreeMemory(pLanMngm);
            return ANSC_STATUS_FAILURE;
        }

        pLmLinkObj = AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
        if (!pLmLinkObj)
        {
            AnscFreeMemory(pLanMngm);
            return ANSC_STATUS_FAILURE;
        }

        if (pLanMngm->InstanceNumber != 0)
        {
            if (pMyObject->ulLanMngmNextInsNum <= pLanMngm->InstanceNumber)
            {
                pMyObject->ulLanMngmNextInsNum = pLanMngm->InstanceNumber + 1;
                if (pMyObject->ulLanMngmNextInsNum == 0)
                {
                    pMyObject->ulLanMngmNextInsNum = 1;
                }
            }
        }
        else
        {
            pLanMngm->InstanceNumber = pMyObject->ulLanMngmNextInsNum;

            pMyObject->ulLanMngmNextInsNum++;
            if (pMyObject->ulLanMngmNextInsNum == 0)
            {
                pMyObject->ulLanMngmNextInsNum = 1;
            }

            _ansc_sprintf(pLanMngm->Alias, "cpe-LanManagement-%d", pLanMngm->InstanceNumber);
            CosaDmlLanMngm_SetValues(ulLmIdx, pLanMngm->InstanceNumber, pLanMngm->Alias);
        }

        pLmLinkObj->InstanceNumber  = pLanMngm->InstanceNumber;
        pLmLinkObj->hContext        = pLanMngm;
        pLmLinkObj->hParentTable    = NULL;
        pLmLinkObj->bNew            = FALSE;

        CosaSListPushEntryByInsNum(&pMyObject->LanMngmList, pLmLinkObj);
    }

    CosaDevCtrlReg_GetLanMngmInfo((ANSC_HANDLE)pMyObject);

    pMyObject->bResetChanged = 0;
    pMyObject->bFactoryResetChanged = 0;

    CosaDmlDcGetWanAddressMode((ANSC_HANDLE)pMyObject, &pMyObject->WanAddrMode);
    CosaDmlDcGetMsoRemoteMgmtEnable((ANSC_HANDLE)pMyObject, &pMyObject->EnableMsoRemoteMgmt);
    CosaDmlDcGetCusadminRemoteMgmtEnable((ANSC_HANDLE)pMyObject, &pMyObject->EnableCusadminRemoteMgmt);

    CosaDmlDcGetHTTPPort ((ANSC_HANDLE)pMyObject, &pMyObject->HTTPPort );
    CosaDmlDcGetHTTPSPort((ANSC_HANDLE)pMyObject, &pMyObject->HTTPSPort);
    CosaDmlDcGetReinitMacThreshold((ANSC_HANDLE)pMyObject, &pMyObject->ReinitMacThreshold);
    //CosaDmlDcGetGuestPassword(NULL, pMyObject->GuestPassword);
    //pMyObject->NoOfGuests = CosaDmlDcGetNoOfGuests();

    CosaDmlXConfGetConfig((ANSC_HANDLE)pMyObject, &pMyObject->XConf);

    CosaDmlWebPAGetConfig2((ANSC_HANDLE)pMyObject, &pMyObject->WebPAConfig);

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDeviceControlRemove
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate  cosa DeviceControl object and return handle.

    argument:   ANSC_HANDLE                 hThisObject
            This handle is actually the pointer of this object
            itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaDeviceControlRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DEVICECONTROL   pMyObject    = (PCOSA_DATAMODEL_DEVICECONTROL)hThisObject;

    /* Remove necessary resounce */


    /* Remove self */
    AnscFreeMemory((ANSC_HANDLE)pMyObject);

    return returnStatus;
}

/**
 * @brief Get User Changed Control Flags from PSM
 *
 */
ANSC_STATUS
CosaDevCtrlReg_GetUserChangedParamsControl(
        ANSC_HANDLE                 hThisObject
    )
{
    PCOSA_DATAMODEL_DEVICECONTROL   pMyObject               = (PCOSA_DATAMODEL_DEVICECONTROL   )hThisObject;
    INT            ret                                      = CCSP_SUCCESS;
    char*          strValue                                 = NULL;
    int            intValue;

    ret = PSM_Get_Record_Value2(g_MessageBusHandle, g_GetSubsystemPrefix(g_pDslhDmlAgent),
                               CCSP_USER_CHANGED_MONITOR_PARAM, NULL, &strValue);
    if (ret == CCSP_SUCCESS)
    {
        intValue = _ansc_atoi(strValue);
        ((CCSP_MESSAGE_BUS_INFO *)g_MessageBusHandle)->freefunc(strValue);        
        pMyObject->EnableMonitorUserChangedParams = (intValue == 0 ? FALSE:TRUE);
    }
    ret = PSM_Get_Record_Value2(g_MessageBusHandle, g_GetSubsystemPrefix(g_pDslhDmlAgent),
                               CCSP_USER_COMPONENTS_PARAM, NULL, &strValue);
    if (ret == CCSP_SUCCESS)
    {
        intValue = _ansc_atoi(strValue);        
        ((CCSP_MESSAGE_BUS_INFO *)g_MessageBusHandle)->freefunc(strValue);
        pMyObject->UserOpComponents = intValue;
    }

    return ANSC_STATUS_SUCCESS;
}


/**
 * @brief Set User Changed Control Flags to PSM
 */
ANSC_STATUS
CosaDevCtrlReg_SetUserChangedParamsControl(
        ANSC_HANDLE                 hThisObject
    )
{
    /* workaroud for upgrade from 1.3 to 1.6 which may cause core dumped.
     * will not set these two params to PSM
     */
#if 0
    PCOSA_DATAMODEL_DEVICECONTROL   pMyObject               = (PCOSA_DATAMODEL_DEVICECONTROL   )hThisObject;
    PSLAP_VARIABLE pSlapVariable = (PSLAP_VARIABLE)NULL;

    SlapAllocVariable(pSlapVariable);
    if (pSlapVariable)
    {
        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_bool;
        pSlapVariable->Variant.varBool   = pMyObject->EnableMonitorUserChangedParams;

        // write to PSM
        PSM_Set_Record_Value(g_MessageBusHandle, g_GetSubsystemPrefix(g_pDslhDmlAgent),
                             CCSP_USER_CHANGED_MONITOR_PARAM, SYS_REP_RECORD_TYPE_BOOL, pSlapVariable);

        SlapCleanVariable(pSlapVariable);

        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
        pSlapVariable->Variant.varUint32 = pMyObject->UserOpComponents;

        PSM_Set_Record_Value(g_MessageBusHandle, g_GetSubsystemPrefix(g_pDslhDmlAgent),
                             CCSP_USER_COMPONENTS_PARAM, SYS_REP_RECORD_TYPE_BOOL, pSlapVariable);

        SlapFreeVariable(pSlapVariable);

        return ANSC_STATUS_SUCCESS;
    }
    else
    {
        return ANSC_STATUS_RESOURCES;
    }
#endif
    return ANSC_STATUS_SUCCESS;
}

/**
 * @brief Get a list of User Changed Params
 *
 * @return 0 if succeed
 *         1 if short of buffer, store required length in pulSize
 *        -1 if error
 */
ULONG CosaDevCtrlReg_GetUserChangedParams(
        ANSC_HANDLE                 hThisObject,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    char psmName[256];
    int i, size, ret_size, offset;
    parameterInfoStruct_t **val = 0;
    char *pSubSystemPrefix = NULL;
    const char* name_prefix = "UserChanged.";
    const int prefix_len = strlen(name_prefix);

    pSubSystemPrefix = g_GetSubsystemPrefix(g_pDslhDmlAgent);
    if ( pSubSystemPrefix && pSubSystemPrefix[0] != 0 )
    {
        sprintf(psmName, "%s%s", pSubSystemPrefix, CCSP_DBUS_PSM);
    }
    else
    {
        strcpy(psmName, CCSP_DBUS_PSM);
    }

    int ret = CcspBaseIf_getParameterNames(g_MessageBusHandle, psmName, CCSP_DBUS_PATH_PSM,
                                           name_prefix, 0, &size, &val);

    if(ret != CCSP_SUCCESS )
        return -1;

    ret_size = 0;
    for (i=0;i<size;i++)
    {
        int name_len = strlen(val[i]->parameterName);
        ret_size += name_len-prefix_len+1;
    }

    if (*pulSize < ret_size)
    {
        *pulSize = ret_size;
        return 1;
    }

    offset = 0;
    for (i=0;i<size;i++)
    {
        int name_len = strlen(val[i]->parameterName);

        if (offset) pValue[offset++] = ',';
        memcpy(pValue+offset, &val[i]->parameterName[prefix_len], name_len - prefix_len);
        offset += name_len - prefix_len;
    }
    pValue[offset] = '\0';

    free_parameterInfoStruct_t(g_MessageBusHandle, size, val);

    return 0;
}

ANSC_STATUS
CosaDevCtrlReg_GetLanMngmInfo(
        ANSC_HANDLE                 hThisObject
    )
{
    PCOSA_DATAMODEL_DEVICECONTROL   pMyObject               = (PCOSA_DATAMODEL_DEVICECONTROL   )hThisObject;
    PSLIST_HEADER                   pListHead               = (PSLIST_HEADER            )&pMyObject->LanMngmList;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoLm           = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderLm;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoLmSp         = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PCOSA_DML_LAN_MANAGEMENT        pEntry                  = (PCOSA_DML_LAN_MANAGEMENT    )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSLAP_VARIABLE                  pSlapVariable           = (PSLAP_VARIABLE           )NULL;
    ULONG                           ulEntryCount            = 0;
    ULONG                           ulIndex                 = 0;
    ULONG                           ulInstanceNumber        = 0;
    char*                           pFolderName             = NULL;
    char*                           pAlias                  = NULL;

    if ( !pPoamIrepFoLm )
    {
        return ANSC_STATUS_FAILURE;
    }

    /* Load the newly added but not yet commited entries */
    
    ulEntryCount = pPoamIrepFoLm->GetFolderCount((ANSC_HANDLE)pPoamIrepFoLm);

    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        pFolderName =
            pPoamIrepFoLm->EnumFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoLm,
                    ulIndex
                );

        if ( !pFolderName )
        {
            continue;
        }

        pPoamIrepFoLmSp = pPoamIrepFoLm->GetFolder((ANSC_HANDLE)pPoamIrepFoLm, pFolderName);

        AnscFreeMemory(pFolderName);

        if ( !pPoamIrepFoLmSp )
        {
            continue;
        }

        if ( TRUE )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoLmSp->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoLmSp,
                        COSA_DML_RR_NAME_LAN_MANAGEMENT_InsNum,
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
                (PSLAP_VARIABLE)pPoamIrepFoLmSp->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoLmSp,
                        COSA_DML_RR_NAME_LAN_MANAGEMENT_Alias,
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

        pEntry = (PCOSA_DML_LAN_MANAGEMENT)AnscAllocateMemory(sizeof(COSA_DML_LAN_MANAGEMENT));

        if ( !pEntry )
        {
            AnscFreeMemory(pAlias);
            AnscFreeMemory(pCosaContext);

            return ANSC_STATUS_RESOURCES;
        }

        AnscCopyString(pEntry->Alias, pAlias);

        pEntry->InstanceNumber = ulInstanceNumber;

        pCosaContext->InstanceNumber        = ulInstanceNumber;
        pCosaContext->bNew                  = TRUE;
        pCosaContext->hContext              = (ANSC_HANDLE)pEntry;
        pCosaContext->hParentTable          = NULL;
        pCosaContext->hPoamIrepUpperFo      = (ANSC_HANDLE)pPoamIrepFoLm;
        pCosaContext->hPoamIrepFo           = (ANSC_HANDLE)pPoamIrepFoLmSp;

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
CosaDevCtrlReg_AddLanMngmInfo(
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    )
{
    ANSC_STATUS                     returnStatus            = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DEVICECONTROL   pMyObject               = (PCOSA_DATAMODEL_DEVICECONTROL   )hThisObject;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoLm           = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderLm;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoLmSp         = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)hCosaContext;
    PCOSA_DML_LAN_MANAGEMENT        pEntry                  = (PCOSA_DML_LAN_MANAGEMENT      )pCosaContext->hContext;
    PSLAP_VARIABLE                  pSlapVariable           = (PSLAP_VARIABLE           )NULL;

    if ( !pPoamIrepFoLm )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pPoamIrepFoLm->EnableFileSync((ANSC_HANDLE)pPoamIrepFoLm, FALSE);
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
            pPoamIrepFoLm->DelRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoLm,
                    COSA_DML_RR_NAME_LAN_MANAGEMENT_NextInsNumber
                );

        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
        pSlapVariable->Variant.varUint32 = pMyObject->ulLanMngmNextInsNum;

        returnStatus =
            pPoamIrepFoLm->AddRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoLm,
                    COSA_DML_RR_NAME_LAN_MANAGEMENT_NextInsNumber,
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
        pPoamIrepFoLmSp =
            pPoamIrepFoLm->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoLm,
                    pEntry->Alias,
                    0
                );

        if ( !pPoamIrepFoLmSp )
        {
            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT1;
        }

        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
            pSlapVariable->Variant.varUint32 = pEntry->InstanceNumber;

            returnStatus =
                pPoamIrepFoLmSp->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoLmSp,
                        COSA_DML_RR_NAME_LAN_MANAGEMENT_InsNum,
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
                pPoamIrepFoLmSp->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoLmSp,
                        COSA_DML_RR_NAME_LAN_MANAGEMENT_Alias,
                        SYS_REP_RECORD_TYPE_ASTR,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );

            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }

        pCosaContext->hPoamIrepUpperFo = (ANSC_HANDLE)pPoamIrepFoLm;
        pCosaContext->hPoamIrepFo      = (ANSC_HANDLE)pPoamIrepFoLmSp;
    }

EXIT1:
    
    if ( pSlapVariable )
    {
        SlapFreeVariable(pSlapVariable);
    }

    pPoamIrepFoLm->EnableFileSync((ANSC_HANDLE)pPoamIrepFoLm, TRUE);

    return returnStatus;
}

ANSC_STATUS
CosaDevCtrlReg_DelLanMngmInfo(
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DEVICECONTROL   pMyObject         = (PCOSA_DATAMODEL_DEVICECONTROL   )hThisObject;
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

