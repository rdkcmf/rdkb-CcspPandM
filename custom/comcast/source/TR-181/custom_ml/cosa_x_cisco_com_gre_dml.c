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

/*
 *  Where is the file header section? Taking shortcut?...
 */

#include "dml_tr181_custom_cfg.h"

#ifdef CONFIG_CISCO_HOTSPOT
#include "cosa_x_cisco_com_gre_dml.h"
#include "cosa_x_cisco_com_gre_internal.h"

ULONG
CGreIf_GetEntryCount
    (
        ANSC_HANDLE hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    return CosaDml_CGreIfGetNumberOfEntries();
}

ANSC_HANDLE
CGreIf_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    COSA_DATAMODEL_CGRE              *pMyObject   = (COSA_DATAMODEL_CGRE *)g_pCosaBEManager->hCGRE;
    UNREFERENCED_PARAMETER(hInsContext);

    if (nIndex >= MAX_CGRE_IFS || nIndex >= (ULONG)pMyObject->IfCnt)
        return NULL;

    *pInsNumber = pMyObject->GreIf[nIndex].InstanceNumber;
    return (ANSC_HANDLE)&pMyObject->GreIf[nIndex];
}

ANSC_HANDLE
CGreIf_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pInsNumber);
    return ANSC_STATUS_SUCCESS;
}

ULONG
CGreIf_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(hInstance);
    return ANSC_STATUS_SUCCESS;
}

BOOL
CGreIf_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    COSA_DML_CGRE_IF                *pCGreIf      = (COSA_DML_CGRE_IF *)hInsContext;

    if (AnscEqualString(ParamName, "Enable", TRUE))
    {
        *pBool = pCGreIf->Enable;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "ChecksumEnabled", TRUE))
    {
        *pBool = pCGreIf->ChecksumEnabled;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "SequenceNumberEnabled", TRUE))
    {
        *pBool = pCGreIf->SequenceNumberEnabled;
        return TRUE;
    }

    return FALSE;
}

BOOL
CGreIf_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    )
{
    COSA_DML_CGRE_IF                *pCGreIf      = (COSA_DML_CGRE_IF *)hInsContext;

    if (AnscEqualString(ParamName, "Status", TRUE))
    {
        if (CosaDml_CGreIfGetStatus(pCGreIf, (COSA_DML_CGRE_STATUS *)pUlong) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "LastChange", TRUE))
    {
        if (CosaDml_CGreIfGetLastChange(pCGreIf, pUlong) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "Mode", TRUE))
    {
        *pUlong = pCGreIf->Mode;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "KeyMode", TRUE))
    {
        *pUlong = pCGreIf->KeyMode;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "TOSMode", TRUE))
    {
        *pUlong = pCGreIf->TOSMode;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "TOS", TRUE))
    {
        *pUlong = pCGreIf->TOS;
        return TRUE;
    }

    return FALSE;
}

ULONG
CGreIf_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )

{
    COSA_DML_CGRE_IF                *pCGreIf      = (COSA_DML_CGRE_IF *)hInsContext;

    if (AnscEqualString(ParamName, "Alias", TRUE))
    {
        if (*pUlSize <= AnscSizeOfString(pCGreIf->Alias))
        {
            *pUlSize = AnscSizeOfString(pCGreIf->Alias) + 1;
            return 1;
        }

        AnscCopyString(pValue, pCGreIf->Alias);
        return 0;
    }
    if (AnscEqualString(ParamName, "Name", TRUE))
    {
        if (*pUlSize <= AnscSizeOfString(pCGreIf->Name))
        {
            *pUlSize = AnscSizeOfString(pCGreIf->Name) + 1;
            return 1;
        }

        AnscCopyString(pValue, pCGreIf->Name);
        return 0;
    }
    if (AnscEqualString(ParamName, "LowerLayers", TRUE))
    {
        if (*pUlSize <= AnscSizeOfString(pCGreIf->LowerLayers))
        {
            *pUlSize = AnscSizeOfString(pCGreIf->LowerLayers) + 1;
            return 1;
        }

        AnscCopyString(pValue, pCGreIf->LowerLayers);
        return 0;
    }
    if (AnscEqualString(ParamName, "RemoteEndpoint", TRUE))
    {
        if (*pUlSize <= AnscSizeOfString(pCGreIf->RemoteEndpoint))
        {
            *pUlSize = AnscSizeOfString(pCGreIf->RemoteEndpoint) + 1;
            return 1;
        }

        AnscCopyString(pValue, pCGreIf->RemoteEndpoint);
        return 0;
    }
    if (AnscEqualString(ParamName, "Key", TRUE))
    {
        if (*pUlSize <= AnscSizeOfString(pCGreIf->Key))
        {
            *pUlSize = AnscSizeOfString(pCGreIf->Key) + 1;
            return 1;
        }

        AnscCopyString(pValue, pCGreIf->Key);
        return 0;
    }

    return -1;
}

BOOL
CGreIf_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pInt);
    return FALSE;
}

BOOL
CGreIf_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    COSA_DML_CGRE_IF                *pCGreIf      = (COSA_DML_CGRE_IF *)hInsContext;

    if (AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* 
         * need set the flag for "force restart".
         * 1. Enable is set to TRUE
         * 2. Enable was already TRUE
         */
        //if (bValue == pCGreIf->Enable)
        //    return TRUE;

        pCGreIf->Enable = bValue;
        pCGreIf->ChangeFlag |= COSA_DML_CGRE_CF_ENABLE;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "ChecksumEnabled", TRUE))
    {
        if (bValue == pCGreIf->ChecksumEnabled)
            return TRUE;

        pCGreIf->ChecksumEnabled = bValue;
        pCGreIf->ChangeFlag |= COSA_DML_CGRE_CF_CSUMEN;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "SequenceNumberEnabled", TRUE))
    {
        if (bValue == pCGreIf->SequenceNumberEnabled)
            return TRUE;

        pCGreIf->SequenceNumberEnabled = bValue;
        pCGreIf->ChangeFlag |= COSA_DML_CGRE_CF_SEQEN;
        return TRUE;
    }

    return FALSE;
}

BOOL
CGreIf_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    COSA_DML_CGRE_IF                *pCGreIf      = (COSA_DML_CGRE_IF *)hInsContext;

    if (AnscEqualString(ParamName, "Mode", TRUE))
    {
        if (uValue == pCGreIf->Mode)
            return TRUE;

        pCGreIf->Mode = uValue;
        pCGreIf->ChangeFlag |= COSA_DML_CGRE_CF_MODE;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "KeyMode", TRUE))
    {
        if (uValue == pCGreIf->KeyMode)
            return TRUE;

        pCGreIf->KeyMode = uValue;
        pCGreIf->ChangeFlag |= COSA_DML_CGRE_CF_KEYMOD;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "TOSMode", TRUE))
    {
        if (uValue == pCGreIf->TOSMode)
            return TRUE;

        pCGreIf->TOSMode = uValue;
        pCGreIf->ChangeFlag |= COSA_DML_CGRE_CF_TOSMOD;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "TOS", TRUE))
    {
        if (uValue == pCGreIf->TOS)
            return TRUE;

        pCGreIf->TOS = uValue;
        pCGreIf->ChangeFlag |= COSA_DML_CGRE_CF_TOS;
        return TRUE;
    }

    return FALSE;
}

BOOL
CGreIf_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    )
{
    COSA_DML_CGRE_IF                *pCGreIf      = (COSA_DML_CGRE_IF *)hInsContext;

    if (AnscEqualString(ParamName, "Alias", TRUE))
    {
        if (AnscEqualString(strValue, pCGreIf->Alias, TRUE))
            return TRUE;

        snprintf(pCGreIf->Alias, sizeof(pCGreIf->Alias), "%s", strValue);
        pCGreIf->ChangeFlag |= COSA_DML_CGRE_CF_ALIAS;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "Name", TRUE))
    {
        if (AnscEqualString(strValue, pCGreIf->Name, TRUE))
            return TRUE;

        snprintf(pCGreIf->Name, sizeof(pCGreIf->Name), "%s", strValue);
        pCGreIf->ChangeFlag |= COSA_DML_CGRE_CF_NAME;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "LowerLayers", TRUE))
    {
        if (AnscEqualString(strValue, pCGreIf->LowerLayers, TRUE))
            return TRUE;

        snprintf(pCGreIf->LowerLayers, sizeof(pCGreIf->LowerLayers), "%s", strValue);
        pCGreIf->ChangeFlag |= COSA_DML_CGRE_CF_LOWLAY;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "RemoteEndpoint", TRUE))
    {
        if (AnscEqualString(strValue, pCGreIf->RemoteEndpoint, TRUE))
            return TRUE;

        snprintf(pCGreIf->RemoteEndpoint, sizeof(pCGreIf->RemoteEndpoint), "%s", strValue);
        pCGreIf->ChangeFlag |= COSA_DML_CGRE_CF_EP;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "Key", TRUE))
    {
        if (AnscEqualString(strValue, pCGreIf->Key, TRUE))
            return TRUE;

        snprintf(pCGreIf->Key, sizeof(pCGreIf->Key), "%s", strValue);
        pCGreIf->ChangeFlag |= COSA_DML_CGRE_CF_KEY;
        return TRUE;
    }


    return FALSE;
}
 
BOOL
CGreIf_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(value);
    return FALSE;
}

BOOL
CGreIf_Validate
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
CGreIf_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    COSA_DML_CGRE_IF                *pCGreIf      = (COSA_DML_CGRE_IF *)hInsContext;

    if (CosaDml_CGreIfSetCfg(pCGreIf->InstanceNumber, pCGreIf) != ANSC_STATUS_SUCCESS)
    {
        CosaDml_CGreIfGetCfg(pCGreIf->InstanceNumber, pCGreIf);
        return -1;
    }

    return 0;
}

ULONG
CGreIf_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    COSA_DML_CGRE_IF                *pCGreIf      = (COSA_DML_CGRE_IF *)hInsContext;

    if (CosaDml_CGreIfGetCfg(pCGreIf->InstanceNumber, pCGreIf) != ANSC_STATUS_SUCCESS)
        return -1;

    return 0;
}

BOOL
CGreIfStat_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    )
{
    COSA_DML_CGRE_IF                *pCGreIf      = (COSA_DML_CGRE_IF *)hInsContext;
    COSA_DML_IF_STATS               *pStats = &pCGreIf->Stats;

    if (CosaDml_CGreIfGetStats(pCGreIf->InstanceNumber, &pCGreIf->Stats) != ANSC_STATUS_SUCCESS)
        return FALSE;

    if (AnscEqualString(ParamName, "BytesSent", TRUE))
    {
        *pUlong = pStats->BytesSent;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "BytesReceived", TRUE))
    {
        *pUlong = pStats->BytesReceived;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "PacketsSent", TRUE))
    {
        *pUlong = pStats->PacketsSent;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "PacketsReceived", TRUE))
    {
        *pUlong = pStats->PacketsReceived;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "ErrorsSent", TRUE))
    {
        *pUlong = pStats->ErrorsSent;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "ErrorsReceived", TRUE))
    {
        *pUlong = pStats->ErrorsReceived;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "UnicastPacketsSent", TRUE))
    {
        *pUlong = pStats->UnicastPacketsSent;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "UnicastPacketsReceived", TRUE))
    {
        *pUlong = pStats->UnicastPacketsReceived;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "DiscardPacketsSent", TRUE))
    {
        *pUlong = pStats->DiscardPacketsSent;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "DiscardPacketsReceived", TRUE))
    {
        *pUlong = pStats->DiscardPacketsReceived;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "MulticastPacketsSent", TRUE))
    {
        *pUlong = pStats->MulticastPacketsSent;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "MulticastPacketsReceived", TRUE))
    {
        *pUlong = pStats->MulticastPacketsReceived;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "BroadcastPacketsSent", TRUE))
    {
        *pUlong = pStats->BroadcastPacketsSent;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "BroadcastPacketsReceived", TRUE))
    {
        *pUlong = pStats->BroadcastPacketsReceived;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "UnknownProtoPacketsReceived", TRUE))
    {
        *pUlong = pStats->UnknownProtoPacketsReceived;
        return TRUE;
    }
 
    return FALSE;
}


#endif
