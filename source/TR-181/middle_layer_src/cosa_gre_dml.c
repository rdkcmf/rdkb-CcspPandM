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
/****************************************************************************
  Copyright 2016-2018 Intel Corporation

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
******************************************************************************/

#ifdef INTEL_GRE_HOTSPOT

#include "dml_tr181_custom_cfg.h"
#include "cosa_gre_dml.h"
#include "cosa_gre_internal.h"

#define MAX_CMD_LENGTH 256
#define STR_VAL_LENGTH 64
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        GRE_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

    description:

        This function is called to retrieve ULONG parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
GRE_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        GRETunnel_GetEntryCount
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG
GRETunnel_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_GRE         pMyObject   = (PCOSA_DATAMODEL_GRE )g_pCosaBEManager->hIGRE;

    return AnscSListQueryDepth(&pMyObject->GRETunnelList);
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        GRETunnel_GetEntry
            (
                ANSC_HANDLE                 hInsContext,
                ULONG                       nIndex,
                ULONG*                      pInsNumber
            );

    description:

        This function is called to retrieve the entry specified by the index.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ULONG                       nIndex,
                The index of this entry;

                ULONG*                      pInsNumber
                The output instance number;

    return:     The handle to identify the entry

**********************************************************************/
ANSC_HANDLE
GRETunnel_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_GRE             pMyObject               = (PCOSA_DATAMODEL_GRE )g_pCosaBEManager->hIGRE;
    PSLIST_HEADER                   pListHead               = (PSLIST_HEADER            )&pMyObject->GRETunnelList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry             = (PSINGLE_LINK_ENTRY       )NULL;
    PCOSA_DML_GRE_TUNNEL_FULL       pEntry                  = (PCOSA_DML_GRE_TUNNEL_FULL  )NULL;

    pSLinkEntry = AnscSListGetEntryByIndex(&pMyObject->GRETunnelList, nIndex);

    if ( pSLinkEntry )
    {
        pCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);

        *pInsNumber = pCosaContext->InstanceNumber;
    }
    return pCosaContext;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        GRETunnel_AddEntry
            (
                ANSC_HANDLE                 hInsContext,
                ULONG*                      pInsNumber
            );

    description:

        This function is called to add a new entry.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ULONG*                      pInsNumber
                The output instance number;

    return:     The handle of new added entry.

**********************************************************************/
ANSC_HANDLE
GRETunnel_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_GRE             pMyObject               = (PCOSA_DATAMODEL_GRE)g_pCosaBEManager->hIGRE;
    PSLIST_HEADER                   pListHead               = (PSLIST_HEADER            )&pMyObject->GRETunnelList;
    PCOSA_DML_GRE_TUNNEL_FULL_ALL   pEntry                  = (PCOSA_DML_GRE_TUNNEL_FULL_ALL  )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)NULL;

    pEntry = (PCOSA_DML_GRE_TUNNEL_FULL_ALL)AnscAllocateMemory(sizeof(COSA_DML_GRE_TUNNEL_FULL_ALL));
    if (!pEntry)
    {
        return NULL;
    }

    _ansc_sprintf(pEntry->Cfg.Alias, "cpe-GRETunnel%d", pMyObject->ulNextGRETunnelInstance);

    pEntry->Cfg.DeliveryHeaderProtocol = COSA_DML_IPV4;
    pEntry->Cfg.KeepAlivePolicy = COSA_DML_KEEPALIVE_POLICY_NONE;
    pEntry->Cfg.KeepAliveTimeout = 10;
    pEntry->Cfg.KeepAliveThreshold = 3;
    pEntry->Cfg.DefaultDSCPMark = 0;

    AnscSListInitializeHeader(&pEntry->InterfaceList);
    pEntry->ulNextIfInsNum = 1;

    /* Update the cache */
    pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
    if (!pCosaContext)
    {
        AnscFreeMemory(pEntry);

        return NULL;
    }

    pCosaContext->InstanceNumber = pEntry->Cfg.TunnelInstanceNumber = pMyObject->ulNextGRETunnelInstance;
    pMyObject->ulNextGRETunnelInstance++;

    if ( pMyObject->ulNextGRETunnelInstance == 0 )
    {
        pMyObject->ulNextGRETunnelInstance = 1;
    }

    pCosaContext->hContext        = (ANSC_HANDLE)pEntry;
    pCosaContext->hParentTable    = NULL;
    pCosaContext->bNew            = TRUE;

    CosaSListPushEntryByInsNum(pListHead, pCosaContext);
    *pInsNumber = pCosaContext->InstanceNumber;

    return pCosaContext;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        GRETunnel_DelEntry
            (
                ANSC_HANDLE                 hInsContext,
                ANSC_HANDLE                 hInstance
            );

    description:

        This function is called to delete an exist entry.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ANSC_HANDLE                 hInstance
                The exist entry handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
GRETunnel_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    PCOSA_DATAMODEL_GRE             pMyObject     = (PCOSA_DATAMODEL_GRE)g_pCosaBEManager->hIGRE;
    PSLIST_HEADER                   pListHead     = (PSLIST_HEADER)&pMyObject->GRETunnelList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_DML_GRE_TUNNEL_FULL_ALL   pDmlGRETunnel = (PCOSA_DML_GRE_TUNNEL_FULL_ALL  )pCosaContext->hContext;

    CosaDmlGRETunnelDelEntry(pMyObject->hSbContext, pDmlGRETunnel->Cfg.TunnelInstanceNumber);

    AnscSListPopEntryByLink(pListHead, &pCosaContext->Linkage);

    AnscFreeMemory(pDmlGRETunnel);
    AnscFreeMemory(pCosaContext);

    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        GRETunnel_GetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL*                       pBool
            );

    description:

        This function is called to retrieve Boolean parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL*                       pBool
                The buffer of returned boolean value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
GRETunnel_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_GRE_TUNNEL_FULL_ALL   pDmlGRETunnel     = (PCOSA_DML_GRE_TUNNEL_FULL_ALL)pCosaContext->hContext;

    if (strcmp(ParamName, "Enable") == 0)
    {
        *pBool = pDmlGRETunnel->Cfg.Enable;
        return TRUE;
    }
    if (strcmp(ParamName, "X_INTEL_Hotspot_SnoopCircuitEnable") == 0)
    {
        *pBool = pDmlGRETunnel->Cfg.SnoopCircuitEnable;
        GRE_hotspot_update_circuit_ids(pDmlGRETunnel->Cfg.TunnelInstanceNumber, 1);
        return TRUE;
    }
    if (strcmp(ParamName, "X_INTEL_Hotspot_SnoopRemoteEnable") == 0)
    {
        *pBool = pDmlGRETunnel->Cfg.SnoopRemoteEnable;
        return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        GRETunnel_GetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pValue,
                ULONG*                      pUlSize
            );

    description:

        This function is called to retrieve string parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue,
                The string value buffer;

                ULONG*                      pUlSize
                The buffer of length of string value;
                Usually size of 1023 will be used.
                If it's not big enough, put required size here and return 1;

    return:     0 if succeeded;
                1 if short of buffer size; (*pUlSize = required size)
                -1 if not supported.

**********************************************************************/
ULONG
GRETunnel_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )

{
    PCOSA_DATAMODEL_GRE             pMyObject         = (PCOSA_DATAMODEL_GRE)g_pCosaBEManager->hIGRE;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_GRE_TUNNEL_FULL_ALL   pDmlGRETunnel     = (PCOSA_DML_GRE_TUNNEL_FULL_ALL)pCosaContext->hContext;

    if (strcmp(ParamName, "Alias") == 0)
    {
        AnscCopyString(pValue, pDmlGRETunnel->Cfg.Alias);
        return 0;
    }
    if (strcmp(ParamName, "RemoteEndpoints") == 0)
    {
        AnscCopyString(pValue, pDmlGRETunnel->Cfg.RemoteEndpoints);
        return 0;
    }
    if (strcmp(ParamName, "ConnectedRemoteEndpoint") == 0)
    {
        CosaDmlGRETunnelGetCfg(pMyObject->hSbContext, &pDmlGRETunnel->Cfg);
        AnscCopyString(pValue, pDmlGRETunnel->Cfg.ConnectedRemoteEndpoint);
        return 0;
    }
    return -1;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        GRETunnel_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

    description:

        This function is called to retrieve ULONG parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
GRETunnel_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DATAMODEL_GRE             pMyObject         = (PCOSA_DATAMODEL_GRE)g_pCosaBEManager->hIGRE;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_GRE_TUNNEL_FULL_ALL   pDmlGRETunnel     = (PCOSA_DML_GRE_TUNNEL_FULL_ALL)pCosaContext->hContext;

    if (strcmp(ParamName, "Status") == 0)
    {
        /* collect value */
        CosaDmlGRETunnelGetInfo(pMyObject->hSbContext, pDmlGRETunnel->Cfg.TunnelInstanceNumber, &pDmlGRETunnel->Info);
        *puLong = pDmlGRETunnel->Info.Status;
        return TRUE;
    }
    if (strcmp(ParamName, "KeepAlivePolicy") == 0)
    {
        *puLong = pDmlGRETunnel->Cfg.KeepAlivePolicy;
        return TRUE;
    }
    if (strcmp(ParamName, "DeliveryHeaderProtocol") == 0)
    {
        *puLong = pDmlGRETunnel->Cfg.DeliveryHeaderProtocol;
        return TRUE;
    }
    if (strcmp(ParamName, "KeepAliveTimeout") == 0)
    {
        *puLong = pDmlGRETunnel->Cfg.KeepAliveTimeout;
        return TRUE;
    }
    if (strcmp(ParamName, "KeepAliveThreshold") == 0)
    {
        *puLong = pDmlGRETunnel->Cfg.KeepAliveThreshold;
        return TRUE;
    }
    if (strcmp(ParamName, "DefaultDSCPMark") == 0)
    {
        *puLong = pDmlGRETunnel->Cfg.DefaultDSCPMark;
        return TRUE;
    }
    if (strcmp(ParamName, "InterfaceNumberOfEntries") == 0)
    {
        *puLong = pDmlGRETunnel->Cfg.InterfaceNumberOfEntries;
        return TRUE;
    }
    if (strcmp(ParamName, "X_CABLELABS_COM_KeepAliveCount") == 0)
    {
        *puLong = pDmlGRETunnel->Cfg.KeepAliveCount;
        return TRUE;
    }
    if (strcmp(ParamName, "X_CABLELABS_COM_KeepAliveInterval") == 0)
    {
        *puLong = pDmlGRETunnel->Cfg.KeepAliveInterval;
        return TRUE;
    }
    if (strcmp(ParamName, "X_CABLELABS_COM_KeepAliveFailureInterval") == 0)
    {
        *puLong = pDmlGRETunnel->Cfg.KeepAliveFailureInterval;
        return TRUE;
    }
    if (strcmp(ParamName, "X_CABLELABS_COM_KeepAliveRecoverInterval") == 0)
    {
        *puLong = pDmlGRETunnel->Cfg.KeepAliveRecoverInterval;
        return TRUE;
    }
    if (strcmp(ParamName, "X_CABLELABS_COM_MSSClampingValue") == 0)
    {
        *puLong = pDmlGRETunnel->Cfg.MSSClampingValue;
        return TRUE;
    }
    if (strcmp(ParamName, "X_INTEL_Hotspot_MaxClients") == 0)
    {
        *puLong = pDmlGRETunnel->Cfg.MaxClients;
        return TRUE;
    }

    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        GRETunnel_GetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int*                        pInt
            );

    description:

        This function is called to retrieve integer parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The buffer of returned integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
GRETunnel_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        GRETunnel_SetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL                        bValue
            );

    description:

        This function is called to set BOOL parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL                        bValue
                The updated BOOL value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
GRETunnel_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_GRE_TUNNEL_FULL_ALL   pDmlGRETunnel     = (PCOSA_DML_GRE_TUNNEL_FULL_ALL)pCosaContext->hContext;
    char                            str_value[STR_VAL_LENGTH]   = {0};

    if (strcmp(ParamName, "Enable") == 0)
    {
        pDmlGRETunnel->Cfg.Enable = bValue;
        return TRUE;
    }
    if (strcmp(ParamName, "X_INTEL_Hotspot_SnoopCircuitEnable") == 0)
    {
        pDmlGRETunnel->Cfg.SnoopCircuitEnable = bValue;
        snprintf(str_value, sizeof(str_value), "%d", bValue);

        if(sysevent_set(sysevent_fd, sysevent_token, kSnooper_circuit_enable, str_value, 0))
        {
            AnscTraceError(("sysevent set %s failed\n", str_value));
            return FALSE;
        }
        else
        {
            AnscTraceWarning(("sysevent set %s to %s\n", kSnooper_circuit_enable, str_value));
        }

        return TRUE;
    }
    if (strcmp(ParamName, "X_INTEL_Hotspot_SnoopRemoteEnable") == 0)
    {
        pDmlGRETunnel->Cfg.SnoopRemoteEnable = bValue;
        snprintf(str_value, sizeof(str_value), "%d", bValue);

        if(sysevent_set(sysevent_fd, sysevent_token, kSnooper_remote_enable, str_value, 0))
        {
            AnscTraceError(("sysevent set %s failed\n", str_value));
            return FALSE;
        }
        else
        {
            AnscTraceWarning(("sysevent set %s to %s\n", kSnooper_remote_enable, str_value));
        }

        return TRUE;
    }

    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        GRETunnel_SetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG                       uValue
            );

    description:

        This function is called to set ULONG parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue
                The updated ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
GRETunnel_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext                = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_GRE_TUNNEL_FULL_ALL   pDmlGRETunnel               = (PCOSA_DML_GRE_TUNNEL_FULL_ALL)pCosaContext->hContext;
    char                            param_name[MAX_CMD_LENGTH]  = {0};
    char                            str_value[STR_VAL_LENGTH]   = {0};

    snprintf(str_value, sizeof(str_value), "%d", uValue);

    if (strcmp(ParamName, "KeepAlivePolicy") == 0)
    {
        pDmlGRETunnel->Cfg.KeepAlivePolicy = uValue;
        snprintf(str_value, sizeof(str_value), "%d", uValue);

        if(sysevent_set(sysevent_fd, sysevent_token,khotspotfd_keep_alive_policy, str_value, 0))
        {
            AnscTraceError(("sysevent set %s failed\n", str_value));
            return FALSE;
        }
        else
        {
            AnscTraceWarning(("sysevent set %s to %s\n", khotspotfd_keep_alive_policy, str_value));
        }
        return TRUE;
    }
    if (strcmp(ParamName, "DeliveryHeaderProtocol") == 0)
    {
        pDmlGRETunnel->Cfg.DeliveryHeaderProtocol = uValue;
        return TRUE;
    }
    if (strcmp(ParamName, "KeepAliveTimeout") == 0)
    {
        pDmlGRETunnel->Cfg.KeepAliveTimeout = uValue;
        return TRUE;
    }
    if (strcmp(ParamName, "KeepAliveThreshold") == 0)
    {
        pDmlGRETunnel->Cfg.KeepAliveThreshold = uValue;
        snprintf(str_value, sizeof(str_value), "%d", uValue);

        if(sysevent_set(sysevent_fd, sysevent_token,khotspotfd_keep_alive_threshold, str_value, 0))
        {
            AnscTraceError(("sysevent set %s failed\n", str_value));
            return FALSE;
        }
        else
        {
            AnscTraceWarning(("sysevent set %s to %s\n", khotspotfd_keep_alive_threshold, str_value));
        }
        return TRUE;
    }
    if (strcmp(ParamName, "DefaultDSCPMark") == 0)
    {
        pDmlGRETunnel->Cfg.DefaultDSCPMark = uValue;
        return TRUE;
    }
    if (strcmp(ParamName, "X_CABLELABS_COM_KeepAliveCount") == 0)
    {
        pDmlGRETunnel->Cfg.KeepAliveCount = uValue;

        if(sysevent_set(sysevent_fd, sysevent_token,khotspotfd_keep_alive_count, str_value, 0))
        {
            AnscTraceError(("sysevent set %s failed\n", str_value));
            return FALSE;
        }
        else
        {
            AnscTraceWarning(("sysevent set %s to %s\n", khotspotfd_keep_alive_count, str_value));
        }
        return TRUE;
    }
    if (strcmp(ParamName, "X_CABLELABS_COM_KeepAliveInterval") == 0)
    {
        pDmlGRETunnel->Cfg.KeepAliveInterval = uValue;

        if(sysevent_set(sysevent_fd, sysevent_token,khotspotfd_keep_alive, str_value, 0))
        {
            AnscTraceError(("sysevent set %s failed\n", str_value));
            return FALSE;
        }
        else
        {
            AnscTraceWarning(("sysevent set %s to %s\n", khotspotfd_keep_alive, str_value));
        }
        return TRUE;
    }
    if (strcmp(ParamName, "X_CABLELABS_COM_KeepAliveFailureInterval") == 0)
    {
        pDmlGRETunnel->Cfg.KeepAliveFailureInterval = uValue;

        if(sysevent_set(sysevent_fd, sysevent_token, khotspotfd_dead_interval, str_value, 0))
        {
            AnscTraceError(("sysevent set %s failed\n", str_value));
            return FALSE;
        }
        else
        {
            AnscTraceWarning(("sysevent set %s to %s\n", khotspotfd_dead_interval, str_value));
        }
        return TRUE;
    }
    if (strcmp(ParamName, "X_CABLELABS_COM_KeepAliveRecoverInterval") == 0)
    {
        pDmlGRETunnel->Cfg.KeepAliveRecoverInterval = uValue;

        if(sysevent_set(sysevent_fd, sysevent_token,khotspotfd_max_secondary, str_value, 0))
        {
            AnscTraceError(("sysevent set %s failed\n", str_value));
            return FALSE;
        }
        else
        {
            AnscTraceWarning(("sysevent set %s to %s\n", khotspotfd_max_secondary, str_value));
        }
        return TRUE;
    }
    if (strcmp(ParamName, "X_CABLELABS_COM_MSSClampingValue") == 0)
    {
        pDmlGRETunnel->Cfg.MSSClampingValue = uValue;
        _ansc_sprintf(param_name, "sysevent set mssclamping %d", uValue);
        system(param_name);
        return TRUE;
    }
    if (strcmp(ParamName, "X_INTEL_Hotspot_MaxClients") == 0)
    {
        pDmlGRETunnel->Cfg.MaxClients = uValue;

        if(sysevent_set(sysevent_fd, sysevent_token, kSnooper_max_clients, str_value, 0))
        {
            AnscTraceError(("sysevent set %s failed\n", str_value));
            return FALSE;
        }
        else
        {
            AnscTraceWarning(("sysevent set %s to %s\n", kSnooper_max_clients, str_value));
        }
        return TRUE;
    }

    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        GRETunnel_SetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pString
            );

    description:

        This function is called to set string parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pString
                The updated string value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
GRETunnel_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_GRE_TUNNEL_FULL_ALL   pDmlGRETunnel     = (PCOSA_DML_GRE_TUNNEL_FULL_ALL)pCosaContext->hContext;

    if (strcmp(ParamName, "Alias") == 0)
    {
        AnscCopyString(pDmlGRETunnel->Cfg.Alias, pString);
        return TRUE;
    }
    if (strcmp(ParamName, "RemoteEndpoints") == 0)
    {
        AnscCopyString(pDmlGRETunnel->Cfg.RemoteEndpoints, pString);
        return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        GRETunnel_SetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int                         iValue
            );

    description:

        This function is called to set integer parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int                         iValue
                The updated integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
GRETunnel_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        GRETunnel_Validate
            (
                ANSC_HANDLE                 hInsContext,
                char*                       pReturnParamName,
                ULONG*                      puLength
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       pReturnParamName,
                The buffer (128 bytes) of parameter name if there's a validation.

                ULONG*                      puLength
                The output length of the param name.

    return:     TRUE if there's no validation.

**********************************************************************/
BOOL
GRETunnel_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        GRETunnel_Commit
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
GRETunnel_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_GRE             pMyObject         = (PCOSA_DATAMODEL_GRE)g_pCosaBEManager->hIGRE;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_GRE_TUNNEL_FULL       pDmlGRETunnel     = (PCOSA_DML_GRE_TUNNEL_FULL)pCosaContext->hContext;

    if ( pCosaContext->bNew )
    {
        pCosaContext->bNew = FALSE;
        CosaDmlGRETunnelAddEntry(pMyObject->hSbContext, pDmlGRETunnel);
    }
    else
    {
        CosaDmlGRETunnelSetCfg(pMyObject->hSbContext, &pDmlGRETunnel->Cfg);
    }
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        GRETunnel_Rollback
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to roll back the update whenever there's a
        validation found.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
GRETunnel_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_GRE             pMyObject         = (PCOSA_DATAMODEL_GRE)g_pCosaBEManager->hIGRE;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_GRE_TUNNEL_FULL_ALL   pDmlGRETunnel     = (PCOSA_DML_GRE_TUNNEL_FULL_ALL)pCosaContext->hContext;

    if (CosaDmlGRETunnelGetCfg(pMyObject->hSbContext, &pDmlGRETunnel->Cfg) != ANSC_STATUS_SUCCESS)
        return -1;

    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        GRETunnelStats_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

    description:

        This function is called to retrieve ULONG parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
GRETunnelStats_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    )
{
    PCOSA_DATAMODEL_GRE             pMyObject         = (PCOSA_DATAMODEL_GRE)g_pCosaBEManager->hIGRE;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_GRE_TUNNEL_FULL_ALL   pDmlGRETunnel     = (PCOSA_DML_GRE_TUNNEL_FULL_ALL)pCosaContext->hContext;
    COSA_DML_GRE_TUNNEL_STATS       stats;

    CosaDmlGRETunnelGetStats(pMyObject->hSbContext, pDmlGRETunnel->Cfg.TunnelInstanceNumber, &stats);

    if (strcmp(ParamName, "KeepAliveSent") == 0)
    {
        *pUlong = stats.KeepAliveSent;
        return TRUE;
    }
    if (strcmp(ParamName, "KeepAliveReceived") == 0)
    {
        *pUlong = stats.KeepAliveReceived;
        return TRUE;
    }
    if (strcmp(ParamName, "BytesSent") == 0)
    {
        *pUlong = stats.BytesSent;
        return TRUE;
    }
    if (strcmp(ParamName, "BytesReceived") == 0)
    {
        *pUlong = stats.BytesReceived;
        return TRUE;
    }
    if (strcmp(ParamName, "PacketsSent") == 0)
    {
        *pUlong = stats.PacketsSent;
        return TRUE;
    }
    if (strcmp(ParamName, "PacketsReceived") == 0)
    {
        *pUlong = stats.PacketsReceived;
        return TRUE;
    }
    if (strcmp(ParamName, "ErrorsSent") == 0)
    {
        *pUlong = stats.ErrorsSent;
        return TRUE;
    }
    if (strcmp(ParamName, "ErrorsReceived") == 0)
    {
        *pUlong = stats.ErrorsReceived;
        return TRUE;
    }
    if (strcmp(ParamName, "MulticastPacketsReceived") == 0)
    {
        *pUlong = stats.MulticastPacketsReceived;
        return TRUE;
    }
    if (strcmp(ParamName, "DiscardPacketsSent") == 0)
    {
        *pUlong = stats.DiscardPacketsSent;
        return TRUE;
    }
    if (strcmp(ParamName, "DiscardPacketsReceived") == 0)
    {
        *pUlong = stats.DiscardPacketsReceived;
        return TRUE;
    }
    return FALSE;
}


// GRE Tunnel Interface DML FUNCTIONS

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        GRETunnelIf_GetEntryCount
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG
GRETunnelIf_GetEntryCount
    (
        ANSC_HANDLE hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT      pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_GRE_TUNNEL_FULL_ALL  pDmlGRETunnel   = (PCOSA_DML_GRE_TUNNEL_FULL_ALL)pCosaContext->hContext;
    return AnscSListQueryDepth(&pDmlGRETunnel->InterfaceList);
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        GRETunnelIf_GetEntry
            (
                ANSC_HANDLE                 hInsContext,
                ULONG                       nIndex,
                ULONG*                      pInsNumber
            );

    description:

        This function is called to retrieve the entry specified by the index.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ULONG                       nIndex,
                The index of this entry;

                ULONG*                      pInsNumber
                The output instance number;

    return:     The handle to identify the entry

**********************************************************************/
ANSC_HANDLE
GRETunnelIf_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_GRE_TUNNEL_FULL_ALL   pDmlGRETunnel    = (PCOSA_DML_GRE_TUNNEL_FULL_ALL)pCosaContext->hContext;
    PSLIST_HEADER                   pListHead        = (PSLIST_HEADER            )&pDmlGRETunnel->InterfaceList;
    PSINGLE_LINK_ENTRY              pSLinkEntry      = (PSINGLE_LINK_ENTRY       )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)NULL;

    pSLinkEntry = AnscSListGetEntryByIndex(pListHead, nIndex);

    if ( pSLinkEntry )
    {
        pSubCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);

        *pInsNumber = pSubCosaContext->InstanceNumber;
    }
    return (ANSC_HANDLE)pSubCosaContext;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        GRETunnelIf_AddEntry
            (
                ANSC_HANDLE                 hInsContext,
                ULONG*                      pInsNumber
            );

    description:

        This function is called to add a new entry.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ULONG*                      pInsNumber
                The output instance number;

    return:     The handle of new added entry.

**********************************************************************/
ANSC_HANDLE
GRETunnelIf_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_GRE             pMyObject      = (PCOSA_DATAMODEL_GRE )g_pCosaBEManager->hIGRE;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_GRE_TUNNEL_FULL_ALL   pDmlGRETunnel   = (PCOSA_DML_GRE_TUNNEL_FULL_ALL)pCosaContext->hContext;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_GRE_TUNNEL_IF_FULL    pInterface      = (PCOSA_DML_GRE_TUNNEL_IF_FULL  )NULL;

    pInterface = (PCOSA_DML_GRE_TUNNEL_IF_FULL)AnscAllocateMemory(sizeof(COSA_DML_GRE_TUNNEL_IF_FULL));

    if ( !pInterface )
    {
        return NULL;
    }
    _ansc_sprintf(pInterface->Cfg.Alias, "cpe-GREInterface%d", pDmlGRETunnel->ulNextIfInsNum);
    _ansc_sprintf(pInterface->Info.Name, "gretap%d", pDmlGRETunnel->Cfg.TunnelInstanceNumber);
    pInterface->Cfg.ProtocolIdOverride = 0;
    pInterface->Cfg.UseChecksum = FALSE;
    pInterface->Cfg.KeyIdentifierGenerationPolicy = COSA_DML_KEY_ID_GEN_POLICY_DISABLED;
    pInterface->Cfg.KeyIdentifier = 0;
    pInterface->Cfg.UseSequenceNumber = FALSE;

    /* Update the middle layer cache */
    if ( TRUE )
    {
        pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

        if ( !pSubCosaContext )
        {
            AnscFreeMemory(pInterface);

            return NULL;
        }

        pSubCosaContext->InstanceNumber = pInterface->Cfg.IfInstanceNumber = pDmlGRETunnel->ulNextIfInsNum;
        pDmlGRETunnel->ulNextIfInsNum++;
        if ( pDmlGRETunnel->ulNextIfInsNum == 0 )
        {
            pDmlGRETunnel->ulNextIfInsNum = 1;
        }

        pSubCosaContext->hContext         = (ANSC_HANDLE)pInterface;
        pSubCosaContext->hParentTable     = (ANSC_HANDLE)pDmlGRETunnel;
        pSubCosaContext->bNew             = TRUE;

        CosaSListPushEntryByInsNum((PSLIST_HEADER)&pDmlGRETunnel->InterfaceList, (ANSC_HANDLE)pSubCosaContext);
    }
    *pInsNumber = pSubCosaContext->InstanceNumber;

    return (ANSC_HANDLE)pSubCosaContext;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        GRETunnelIf_DelEntry
            (
                ANSC_HANDLE                 hInsContext,
                ANSC_HANDLE                 hInstance
            );

    description:

        This function is called to delete an exist entry.

    argument:   ANSC_HANDLE                 hInsContext
                The instance handle;

                ANSC_HANDLE                 hInstance
                The exist entry handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
GRETunnelIf_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    PCOSA_DATAMODEL_GRE             pMyObject       = (PCOSA_DATAMODEL_GRE)g_pCosaBEManager->hIGRE;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_GRE_TUNNEL_FULL_ALL   pDmlGRETunnel   = (PCOSA_DML_GRE_TUNNEL_FULL_ALL)pCosaContext->hContext;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_DML_GRE_TUNNEL_IF_FULL    pInterface      = (PCOSA_DML_GRE_TUNNEL_IF_FULL)pSubCosaContext->hContext;

    CosaDmlGRETunnelIfDelEntry(pMyObject->hSbContext, pDmlGRETunnel->Cfg.TunnelInstanceNumber, pInterface);

    AnscSListPopEntryByLink((PSLIST_HEADER)&pDmlGRETunnel->InterfaceList, &pSubCosaContext->Linkage);

    AnscFreeMemory(pInterface);
    AnscFreeMemory(pSubCosaContext);

    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        GRETunnelIf_GetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL*                       pBool
            );

    description:

        This function is called to retrieve Boolean parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL*                       pBool
                The buffer of returned boolean value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
GRETunnelIf_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_GRE_TUNNEL_IF_FULL    pDmlGRETunnelIf   = (PCOSA_DML_GRE_TUNNEL_IF_FULL)pCosaContext->hContext;

    if (strcmp(ParamName, "Enable") == 0)
    {
        *pBool = pDmlGRETunnelIf->Cfg.Enable;
        return TRUE;
    }
    if (strcmp(ParamName, "UseChecksum") == 0)
    {
        *pBool = pDmlGRETunnelIf->Cfg.UseChecksum;
        return TRUE;
    }
    if (strcmp(ParamName, "UseSequenceNumber") == 0)
    {
        *pBool = pDmlGRETunnelIf->Cfg.UseSequenceNumber;
        return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        GRETunnelIf_GetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pValue,
                ULONG*                      pUlSize
            );

    description:

        This function is called to retrieve string parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue,
                The string value buffer;

                ULONG*                      pUlSize
                The buffer of length of string value;
                Usually size of 1023 will be used.
                If it's not big enough, put required size here and return 1;

    return:     0 if succeeded;
                1 if short of buffer size; (*pUlSize = required size)
                -1 if not supported.

**********************************************************************/
ULONG
GRETunnelIf_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )

{
    PCOSA_DATAMODEL_GRE             pMyObject         = (PCOSA_DATAMODEL_GRE)g_pCosaBEManager->hIGRE;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_GRE_TUNNEL_FULL_ALL   pDmlGRETunnel     = (PCOSA_DML_GRE_TUNNEL_FULL_ALL)pCosaContext->hParentTable;
    PCOSA_DML_GRE_TUNNEL_IF_FULL    pDmlGRETunnelIf   = (PCOSA_DML_GRE_TUNNEL_IF_FULL)pCosaContext->hContext;

    if (strcmp(ParamName, "Name") == 0)
    {
        CosaDmlGRETunnelIfGetInfo(pMyObject->hSbContext, pDmlGRETunnel->Cfg.TunnelInstanceNumber, pDmlGRETunnelIf->Cfg.IfInstanceNumber, &pDmlGRETunnelIf->Info);
        AnscCopyString(pValue, pDmlGRETunnelIf->Info.Name);
        return 0;
    }
    if (strcmp(ParamName, "Alias") == 0)
    {
        AnscCopyString(pValue, pDmlGRETunnelIf->Cfg.Alias);
        return 0;
    }
    if (strcmp(ParamName, "LowerLayers") == 0)
    {
        AnscCopyString(pValue, pDmlGRETunnelIf->Cfg.LowerLayers);
        return 0;
    }

    return -1;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        GRETunnelIf_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

    description:

        This function is called to retrieve ULONG parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
GRETunnelIf_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DATAMODEL_GRE             pMyObject         = (PCOSA_DATAMODEL_GRE)g_pCosaBEManager->hIGRE;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_GRE_TUNNEL_FULL_ALL   pDmlGRETunnel     = (PCOSA_DML_GRE_TUNNEL_FULL_ALL)pCosaContext->hParentTable;
    PCOSA_DML_GRE_TUNNEL_IF_FULL    pDmlGRETunnelIf   = (PCOSA_DML_GRE_TUNNEL_IF_FULL)pCosaContext->hContext;

    if (strcmp(ParamName, "Status") == 0)
    {
        CosaDmlGRETunnelIfGetInfo(pMyObject->hSbContext, pDmlGRETunnel->Cfg.TunnelInstanceNumber, pDmlGRETunnelIf->Cfg.IfInstanceNumber, &pDmlGRETunnelIf->Info);
        *puLong = pDmlGRETunnelIf->Info.Status;
        return TRUE;
    }
    if (strcmp(ParamName, "LastChange") == 0)
    {
        CosaDmlGRETunnelIfGetInfo(pMyObject->hSbContext, pDmlGRETunnel->Cfg.TunnelInstanceNumber, pDmlGRETunnelIf->Cfg.IfInstanceNumber, &pDmlGRETunnelIf->Info);
        *puLong = AnscGetTimeIntervalInSeconds(pDmlGRETunnelIf->Info.LastChange, AnscGetTickInSeconds());
        return TRUE;
    }
    if (strcmp(ParamName, "KeyIdentifierGenerationPolicy") == 0)
    {
        *puLong = pDmlGRETunnelIf->Cfg.KeyIdentifierGenerationPolicy;
        return TRUE;
    }
    if (strcmp(ParamName, "ProtocolIdOverride") == 0)
    {
        *puLong = pDmlGRETunnelIf->Cfg.ProtocolIdOverride;
        return TRUE;
    }
    if (strcmp(ParamName, "KeyIdentifier") == 0)
    {
        *puLong = pDmlGRETunnelIf->Cfg.KeyIdentifier;
        return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        GRETunnelIf_GetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int*                        pInt
            );

    description:

        This function is called to retrieve integer parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The buffer of returned integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
GRETunnelIf_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        GRETunnelIf_SetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL                        bValue
            );

    description:

        This function is called to set BOOL parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL                        bValue
                The updated BOOL value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
GRETunnelIf_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_GRE_TUNNEL_IF_FULL    pDmlGRETunnelIf   = (PCOSA_DML_GRE_TUNNEL_IF_FULL)pCosaContext->hContext;

    if (strcmp(ParamName, "Enable") == 0)
    {
        pDmlGRETunnelIf->Cfg.Enable = bValue;
        return TRUE;
    }
    if (strcmp(ParamName, "UseChecksum") == 0)
    {
        pDmlGRETunnelIf->Cfg.UseChecksum = bValue;
        return TRUE;
    }
    if (strcmp(ParamName, "UseSequenceNumber") == 0)
    {
        pDmlGRETunnelIf->Cfg.UseSequenceNumber = bValue;
        return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        GRETunnelIf_SetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG                       uValue
            );

    description:

        This function is called to set ULONG parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue
                The updated ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
GRETunnelIf_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_GRE_TUNNEL_IF_FULL    pDmlGRETunnelIf   = (PCOSA_DML_GRE_TUNNEL_IF_FULL)pCosaContext->hContext;

    if (strcmp(ParamName, "KeyIdentifierGenerationPolicy") == 0)
    {
        pDmlGRETunnelIf->Cfg.KeyIdentifierGenerationPolicy = uValue;
        return TRUE;
    }
    if (strcmp(ParamName, "ProtocolIdOverride") == 0)
    {
        pDmlGRETunnelIf->Cfg.ProtocolIdOverride = uValue;
        return TRUE;
    }
    if (strcmp(ParamName, "KeyIdentifier") == 0)
    {
        pDmlGRETunnelIf->Cfg.KeyIdentifier = uValue;
        return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        GRETunnelIf_SetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pString
            );

    description:

        This function is called to set string parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pString
                The updated string value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
GRETunnelIf_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_GRE_TUNNEL_IF_FULL    pDmlGRETunnelIf   = (PCOSA_DML_GRE_TUNNEL_IF_FULL)pCosaContext->hContext;

    if (strcmp(ParamName, "Alias") == 0)
    {
        AnscCopyString(pDmlGRETunnelIf->Cfg.Alias, pString);
        return TRUE;
    }
    if (strcmp(ParamName, "LowerLayers") == 0)
    {
        AnscCopyString(pDmlGRETunnelIf->Cfg.LowerLayers, pString);
        return TRUE;
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        GRETunnelIf_SetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int                         iValue
            );

    description:

        This function is called to set integer parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int                         iValue
                The updated integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
GRETunnelIf_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        GRETunnelIf_Validate
            (
                ANSC_HANDLE                 hInsContext,
                char*                       pReturnParamName,
                ULONG*                      puLength
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       pReturnParamName,
                The buffer (128 bytes) of parameter name if there's a validation.

                ULONG*                      puLength
                The output length of the param name.

    return:     TRUE if there's no validation.

**********************************************************************/
BOOL
GRETunnelIf_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        GRETunnelIf_Commit
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
GRETunnelIf_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_GRE             pMyObject         = (PCOSA_DATAMODEL_GRE)g_pCosaBEManager->hIGRE;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_GRE_TUNNEL_FULL_ALL   pDmlGRETunnel     = (PCOSA_DML_GRE_TUNNEL_FULL_ALL)pCosaContext->hParentTable;
    PCOSA_DML_GRE_TUNNEL_IF_FULL    pDmlGRETunnelIf   = (PCOSA_DML_GRE_TUNNEL_IF_FULL)pCosaContext->hContext;

    if ( pCosaContext->bNew )
    {
        pCosaContext->bNew = FALSE;
        CosaDmlGRETunnelIfAddEntry(pMyObject->hSbContext, pDmlGRETunnel->Cfg.TunnelInstanceNumber, pDmlGRETunnelIf);
    }
    else
    {
        CosaDmlGRETunnelIfSetCfg(pMyObject->hSbContext,pDmlGRETunnel->Cfg.TunnelInstanceNumber, pDmlGRETunnelIf);
    }
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        GRETunnelIf_Rollback
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to roll back the update whenever there's a
        validation found.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
GRETunnelIf_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_GRE             pMyObject         = (PCOSA_DATAMODEL_GRE)g_pCosaBEManager->hIGRE;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_GRE_TUNNEL_FULL_ALL   pDmlGRETunnel     = (PCOSA_DML_GRE_TUNNEL_FULL_ALL)pCosaContext->hParentTable;
    PCOSA_DML_GRE_TUNNEL_IF_FULL    pDmlGRETunnelIf   = (PCOSA_DML_GRE_TUNNEL_IF_FULL)pCosaContext->hContext;
    if (CosaDmlGRETunnelIfGetCfg(pMyObject->hSbContext, pDmlGRETunnel->Cfg.TunnelInstanceNumber, pDmlGRETunnelIf) != ANSC_STATUS_SUCCESS)
        return -1;

    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        GRETunnelIfStats_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

    description:

        This function is called to retrieve ULONG parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
GRETunnelIfStats_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    )
{
    PCOSA_DATAMODEL_GRE             pMyObject         = (PCOSA_DATAMODEL_GRE)g_pCosaBEManager->hIGRE;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_GRE_TUNNEL_FULL_ALL   pDmlGRETunnel     = (PCOSA_DML_GRE_TUNNEL_FULL_ALL)pCosaContext->hParentTable;
    PCOSA_DML_GRE_TUNNEL_IF_FULL    pDmlGRETunnelIf   = (PCOSA_DML_GRE_TUNNEL_IF_FULL)pCosaContext->hContext;
    COSA_DML_GRE_TUNNEL_IF_STATS    stats;

    CosaDmlGRETunnelIfGetInfo(pMyObject->hSbContext, pDmlGRETunnel->Cfg.TunnelInstanceNumber, pDmlGRETunnelIf->Cfg.IfInstanceNumber, &pDmlGRETunnelIf->Info);

    CosaDmlGRETunnelIfGetStats(pMyObject->hSbContext, pDmlGRETunnelIf->Info.Name, &stats);

    if (strcmp(ParamName, "BytesSent") == 0)
    {
        *pUlong = stats.BytesSent;
        return TRUE;
    }
    if (strcmp(ParamName, "BytesReceived") == 0)
    {
        *pUlong = stats.BytesReceived;
        return TRUE;
    }
    if (strcmp(ParamName, "PacketsSent") == 0)
    {
        *pUlong = stats.PacketsSent;
        return TRUE;
    }
    if (strcmp(ParamName, "PacketsReceived") == 0)
    {
        *pUlong = stats.PacketsReceived;
        return TRUE;
    }
    if (strcmp(ParamName, "ErrorsSent") == 0)
    {
        *pUlong = stats.ErrorsSent;
        return TRUE;
    }
    if (strcmp(ParamName, "ErrorsReceived") == 0)
    {
        *pUlong = stats.ErrorsReceived;
        return TRUE;
    }
    if (strcmp(ParamName, "DiscardChecksumReceived") == 0)
    {
        *pUlong = stats.DiscardChecksumReceived;
        return TRUE;
    }
    if (strcmp(ParamName, "DiscardSequenceNumberReceived") == 0)
    {
        *pUlong = stats.DiscardSequenceNumberReceived;
        return TRUE;
    }
    if (strcmp(ParamName, "MulticastPacketsReceived") == 0)
    {
        *pUlong = stats.MulticastPacketsReceived;
        return TRUE;
    }
    if (strcmp(ParamName, "DiscardPacketsSent") == 0)
    {
        *pUlong = stats.DiscardPacketsSent;
        return TRUE;
    }
    if (strcmp(ParamName, "DiscardPacketsReceived") == 0)
    {
        *pUlong = stats.DiscardPacketsReceived;
        return TRUE;
    }
    return FALSE;
}

#endif
