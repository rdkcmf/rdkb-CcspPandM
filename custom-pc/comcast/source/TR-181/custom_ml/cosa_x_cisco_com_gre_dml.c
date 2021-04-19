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

    if (nIndex >= MAX_CGRE_IFS || nIndex >= pMyObject->IfCnt)
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
    return ANSC_STATUS_SUCCESS;
}

ULONG
CGreIf_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
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

    if (strcmp(ParamName, "Enable") == 0)
    {
	CosaDml_CGreIfGetCfg(pCGreIf->InstanceNumber,pCGreIf);//RDKB-EMULATOR
        *pBool = pCGreIf->Enable;
        return TRUE;
    }
    if (strcmp(ParamName, "ChecksumEnabled") == 0)
    {
        *pBool = pCGreIf->ChecksumEnabled;
        return TRUE;
    }
    if (strcmp(ParamName, "SequenceNumberEnabled") == 0)
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

    if (strcmp(ParamName, "Status") == 0)
    {
        if (CosaDml_CGreIfGetStatus(pCGreIf, (COSA_DML_CGRE_STATUS *)pUlong) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }
    if (strcmp(ParamName, "LastChange") == 0)
    {
        if (CosaDml_CGreIfGetLastChange(pCGreIf, pUlong) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }
    if (strcmp(ParamName, "Mode") == 0)
    {
        *pUlong = pCGreIf->Mode;
        return TRUE;
    }
    if (strcmp(ParamName, "KeyMode") == 0)
    {
        *pUlong = pCGreIf->KeyMode;
        return TRUE;
    }
    if (strcmp(ParamName, "TOSMode") == 0)
    {
        *pUlong = pCGreIf->TOSMode;
        return TRUE;
    }
    if (strcmp(ParamName, "TOS") == 0)
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

    if (strcmp(ParamName, "Alias") == 0)
    {
        if (*pUlSize <= AnscSizeOfString(pCGreIf->Alias))
        {
            *pUlSize = AnscSizeOfString(pCGreIf->Alias) + 1;
            return 1;
        }

        AnscCopyString(pValue, pCGreIf->Alias);
        return 0;
    }
    if (strcmp(ParamName, "Name") == 0)
    {
        if (*pUlSize <= AnscSizeOfString(pCGreIf->Name))
        {
            *pUlSize = AnscSizeOfString(pCGreIf->Name) + 1;
            return 1;
        }

        AnscCopyString(pValue, pCGreIf->Name);
        return 0;
    }
    if (strcmp(ParamName, "LowerLayers") == 0)
    {
        if (*pUlSize <= AnscSizeOfString(pCGreIf->LowerLayers))
        {
            *pUlSize = AnscSizeOfString(pCGreIf->LowerLayers) + 1;
            return 1;
        }

        AnscCopyString(pValue, pCGreIf->LowerLayers);
        return 0;
    }
    if (strcmp(ParamName, "RemoteEndpoint") == 0)
    {
        if (*pUlSize <= AnscSizeOfString(pCGreIf->RemoteEndpoint))
        {
            *pUlSize = AnscSizeOfString(pCGreIf->RemoteEndpoint) + 1;
            return 1;
        }

        AnscCopyString(pValue, pCGreIf->RemoteEndpoint);
        return 0;
    }
    if (strcmp(ParamName, "Key") == 0)
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

    if (strcmp(ParamName, "Enable") == 0)
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
    if (strcmp(ParamName, "ChecksumEnabled") == 0)
    {
        if (bValue == pCGreIf->ChecksumEnabled)
            return TRUE;

        pCGreIf->ChecksumEnabled = bValue;
        pCGreIf->ChangeFlag |= COSA_DML_CGRE_CF_CSUMEN;
        return TRUE;
    }
    if (strcmp(ParamName, "SequenceNumberEnabled") == 0)
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

    if (strcmp(ParamName, "Mode") == 0)
    {
        if (uValue == pCGreIf->Mode)
            return TRUE;

        pCGreIf->Mode = uValue;
        pCGreIf->ChangeFlag |= COSA_DML_CGRE_CF_MODE;
        return TRUE;
    }
    if (strcmp(ParamName, "KeyMode") == 0)
    {
        if (uValue == pCGreIf->KeyMode)
            return TRUE;

        pCGreIf->KeyMode = uValue;
        pCGreIf->ChangeFlag |= COSA_DML_CGRE_CF_KEYMOD;
        return TRUE;
    }
    if (strcmp(ParamName, "TOSMode") == 0)
    {
        if (uValue == pCGreIf->TOSMode)
            return TRUE;

        pCGreIf->TOSMode = uValue;
        pCGreIf->ChangeFlag |= COSA_DML_CGRE_CF_TOSMOD;
        return TRUE;
    }
    if (strcmp(ParamName, "TOS") == 0)
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

    if (strcmp(ParamName, "Alias") == 0)
    {
        if (AnscEqualString(strValue, pCGreIf->Alias, TRUE))
            return TRUE;

        snprintf(pCGreIf->Alias, sizeof(pCGreIf->Alias), "%s", strValue);
        pCGreIf->ChangeFlag |= COSA_DML_CGRE_CF_ALIAS;
        return TRUE;
    }
    if (strcmp(ParamName, "Name") == 0)
    {
        if (AnscEqualString(strValue, pCGreIf->Name, TRUE))
            return TRUE;

        snprintf(pCGreIf->Name, sizeof(pCGreIf->Name), "%s", strValue);
        pCGreIf->ChangeFlag |= COSA_DML_CGRE_CF_NAME;
        return TRUE;
    }
    if (strcmp(ParamName, "LowerLayers") == 0)
    {
        if (AnscEqualString(strValue, pCGreIf->LowerLayers, TRUE))
            return TRUE;

        snprintf(pCGreIf->LowerLayers, sizeof(pCGreIf->LowerLayers), "%s", strValue);
        pCGreIf->ChangeFlag |= COSA_DML_CGRE_CF_LOWLAY;
        return TRUE;
    }
    if (strcmp(ParamName, "RemoteEndpoint") == 0)
    {
        if (AnscEqualString(strValue, pCGreIf->RemoteEndpoint, TRUE))
            return TRUE;

        snprintf(pCGreIf->RemoteEndpoint, sizeof(pCGreIf->RemoteEndpoint), "%s", strValue);
        pCGreIf->ChangeFlag |= COSA_DML_CGRE_CF_EP;
        return TRUE;
    }
    if (strcmp(ParamName, "Key") == 0)
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

    if (strcmp(ParamName, "BytesSent") == 0)
    {
        *pUlong = pStats->BytesSent;
        return TRUE;
    }
    if (strcmp(ParamName, "BytesReceived") == 0)
    {
        *pUlong = pStats->BytesReceived;
        return TRUE;
    }
    if (strcmp(ParamName, "PacketsSent") == 0)
    {
        *pUlong = pStats->PacketsSent;
        return TRUE;
    }
    if (strcmp(ParamName, "PacketsReceived") == 0)
    {
        *pUlong = pStats->PacketsReceived;
        return TRUE;
    }
    if (strcmp(ParamName, "ErrorsSent") == 0)
    {
        *pUlong = pStats->ErrorsSent;
        return TRUE;
    }
    if (strcmp(ParamName, "ErrorsReceived") == 0)
    {
        *pUlong = pStats->ErrorsReceived;
        return TRUE;
    }
    if (strcmp(ParamName, "UnicastPacketsSent") == 0)
    {
        *pUlong = pStats->UnicastPacketsSent;
        return TRUE;
    }
    if (strcmp(ParamName, "UnicastPacketsReceived") == 0)
    {
        *pUlong = pStats->UnicastPacketsReceived;
        return TRUE;
    }
    if (strcmp(ParamName, "DiscardPacketsSent") == 0)
    {
        *pUlong = pStats->DiscardPacketsSent;
        return TRUE;
    }
    if (strcmp(ParamName, "DiscardPacketsReceived") == 0)
    {
        *pUlong = pStats->DiscardPacketsReceived;
        return TRUE;
    }
    if (strcmp(ParamName, "MulticastPacketsSent") == 0)
    {
        *pUlong = pStats->MulticastPacketsSent;
        return TRUE;
    }
    if (strcmp(ParamName, "MulticastPacketsReceived") == 0)
    {
        *pUlong = pStats->MulticastPacketsReceived;
        return TRUE;
    }
    if (strcmp(ParamName, "BroadcastPacketsSent") == 0)
    {
        *pUlong = pStats->BroadcastPacketsSent;
        return TRUE;
    }
    if (strcmp(ParamName, "BroadcastPacketsReceived") == 0)
    {
        *pUlong = pStats->BroadcastPacketsReceived;
        return TRUE;
    }
    if (strcmp(ParamName, "UnknownProtoPacketsReceived") == 0)
    {
        *pUlong = pStats->UnknownProtoPacketsReceived;
        return TRUE;
    }
 
    return FALSE;
}


#endif
