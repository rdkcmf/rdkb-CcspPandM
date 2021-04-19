/************************************************************************************
  If not stated otherwise in this file or this component's Licenses.txt file the
  following copyright and licenses apply:

  Copyright 2018 RDK Management

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
**************************************************************************/

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
#include "cosa_x_comcast_com_gre_dml.h"
#include "cosa_x_comcast_com_gre_internal.h"
#include "safec_lib_common.h"

ULONG
GreIf_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    return CosaDml_GreIfGetNumberOfEntries();
}

ANSC_HANDLE
GreIf_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    COSA_DATAMODEL_GRE              *pMyObject   = (COSA_DATAMODEL_GRE *)g_pCosaBEManager->hGRE;

    if (nIndex >= MAX_GRE_IF)
        return NULL;

    *pInsNumber = pMyObject->GreIf[nIndex].InstanceNumber;
    return (ANSC_HANDLE)&pMyObject->GreIf[nIndex];
}

BOOL
GreIf_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    COSA_DML_GRE_IF                 *pGreIf      = (COSA_DML_GRE_IF *)hInsContext;

    if (strcmp(ParamName, "Enable") == 0)
    {
        *pBool = pGreIf->Enable;
        return TRUE;
    }
    if (strcmp(ParamName, "UseSequenceNumber") == 0)
    {
        *pBool = pGreIf->UseSequenceNumber;
        return TRUE;
    }
    if (strcmp(ParamName, "UseChecksum") == 0)
    {
        *pBool = pGreIf->UseChecksum;
        return TRUE;
    }
    if (strcmp(ParamName, "DHCPCircuitIDSSID") == 0)
    {
        *pBool = pGreIf->DHCPCircuitIDSSID;
        if(pGreIf->DHCPCircuitIDSSID) {
		// update  circuit ids 
			hotspot_update_circuit_ids(1,1);
	    }
        return TRUE;
    }
    if (strcmp(ParamName, "DHCPRemoteID") == 0)
    {
        *pBool = pGreIf->DHCPRemoteID;
        return TRUE;
    }

    return FALSE;
}

BOOL
GreIf_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    )
{
    COSA_DML_GRE_IF                 *pGreIf      = (COSA_DML_GRE_IF *)hInsContext;
    ULONG                           ins = pGreIf->InstanceNumber;

    if (strcmp(ParamName, "Status") == 0)
    {
        if (CosaDml_GreIfGetStatus(ins, (COSA_DML_GRE_STATUS *)pUlong) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }
    if (strcmp(ParamName, "LastChange") == 0)
    {
        if (CosaDml_GreIfGetLastchange(ins, pUlong) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }
    if (strcmp(ParamName, "KeyIdentifierGenerationPolicy") == 0)
    {
        *pUlong = pGreIf->KeyIdentifierGenerationPolicy;
        return TRUE;
    }
    if (strcmp(ParamName, "KeepAlivePolicy") == 0)
    {
        *pUlong = pGreIf->KeepAlivePolicy;
        return TRUE;
    }
    if (strcmp(ParamName, "KeepAliveInterval") == 0)
    {
        *pUlong = pGreIf->KeepAliveInterval;
        return TRUE;
    }
    if (strcmp(ParamName, "KeepAliveThreshold") == 0)
    {
        *pUlong = pGreIf->KeepAliveThreshold;
        return TRUE;
    }
    if (strcmp(ParamName, "KeepAliveCount") == 0)
    {
        *pUlong = pGreIf->KeepAliveCount;
        return TRUE;
    }
    if (strcmp(ParamName, "KeepAliveFailInterval") == 0)
    {
        *pUlong = pGreIf->KeepAliveFailInterval;
        return TRUE;
    }
    if (strcmp(ParamName, "ReconnectPrimary") == 0)
    {
        *pUlong = pGreIf->ReconnectPrimary;
        return TRUE;
    }

    return FALSE;
}

ULONG
GreIf_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    COSA_DML_GRE_IF                 *pGreIf      = (COSA_DML_GRE_IF *)hInsContext;
    ULONG                           ins = pGreIf->InstanceNumber;

    if (strcmp(ParamName, "LocalInterfaces") == 0)
    {
        snprintf(pValue, *pUlSize, "%s", pGreIf->LocalInterfaces);
        return 0;
    }
    if (strcmp(ParamName, "RemoteEndpoints") == 0)
    {
        snprintf(pValue, *pUlSize, "%s", pGreIf->RemoteEndpoints);
        return 0;
    }
    if (strcmp(ParamName, "ConnectedRemoteEndpoint") == 0)
    {	
		CosaDml_GreIfGetConnectedRemoteEndpoint(ins,pGreIf);
	   	snprintf(pValue, *pUlSize, "%s", pGreIf->ConnectedRemoteEndpoint);
        return 0;
    }
    if (strcmp(ParamName, "KeyIdentifier") == 0)
    {
        snprintf(pValue, *pUlSize, "%s", pGreIf->KeyIdentifier);
        return 0;
    }
    if (strcmp(ParamName, "AssociatedBridges") == 0)
    {
        snprintf(pValue, *pUlSize, "%s", pGreIf->AssociatedBridges);
        return 0;
    }
    if (strcmp(ParamName, "AssociatedBridgesWiFiPort") == 0)
    {
        snprintf(pValue, *pUlSize, "%s", pGreIf->AssociatedBridgesWiFiPort);
        return 0;
    }
    if (strcmp(ParamName, "GRENetworkInterface") == 0)
    {
        snprintf(pValue, *pUlSize, "%s", pGreIf->GRENetworkInterface);
        return 0;
    }

    return -1;
}

BOOL
GreIf_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    COSA_DML_GRE_IF                 *pGreIf      = (COSA_DML_GRE_IF *)hInsContext;

    if (strcmp(ParamName, "DSCPMarkPolicy") == 0)
    {
        *pInt = pGreIf->DSCPMarkPolicy;
        return TRUE;
    }
    if (strcmp(ParamName, "VLANID") == 0)
    {
        *pInt = pGreIf->VLANID;
        return TRUE;
    }

    return FALSE;
}

BOOL
GreIf_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    COSA_DML_GRE_IF                 *pGreIf      = (COSA_DML_GRE_IF *)hInsContext;

    if (strcmp(ParamName, "Enable") == 0)
    {
        pGreIf->Enable = bValue;
        pGreIf->ChangeFlag |= GREIF_CF_ENABLE;
        return TRUE;
    }
    if (strcmp(ParamName, "UseSequenceNumber") == 0)
    {
        pGreIf->UseSequenceNumber = bValue;
        pGreIf->ChangeFlag |= GREIF_CF_USESEQ;
        return TRUE;
    }
    if (strcmp(ParamName, "UseChecksum") == 0)
    {
        pGreIf->UseChecksum = bValue;
        pGreIf->ChangeFlag |= GREIF_CF_USECSUM;
        return TRUE;
    }
    if (strcmp(ParamName, "DHCPCircuitIDSSID") == 0)
    {
        pGreIf->DHCPCircuitIDSSID = bValue;
        pGreIf->ChangeFlag |= GREIF_CF_DHCPCIRID;
        return TRUE;
    }
    if (strcmp(ParamName, "DHCPRemoteID") == 0)
    {
        pGreIf->DHCPRemoteID = bValue;
        pGreIf->ChangeFlag |= GREIF_CF_DHCPRMID;
        return TRUE;
    }

    return FALSE;
}

BOOL
GreIf_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    COSA_DML_GRE_IF                 *pGreIf      = (COSA_DML_GRE_IF *)hInsContext;

    if (strcmp(ParamName, "KeyIdentifierGenerationPolicy") == 0)
    {
        pGreIf->KeyIdentifierGenerationPolicy = uValue;
        pGreIf->ChangeFlag |= GREIF_CF_KEYGENPOL;
        return TRUE;
    }
    if (strcmp(ParamName, "KeepAlivePolicy") == 0)
    {
        pGreIf->KeepAlivePolicy = uValue;
        pGreIf->ChangeFlag |= GREIF_CF_KEEPPOL;
        return TRUE;
    }
    if (strcmp(ParamName, "KeepAliveInterval") == 0)
    {
        pGreIf->KeepAliveInterval = uValue;
        pGreIf->ChangeFlag |= GREIF_CF_KEEPITVL;
        return TRUE;
    }
    if (strcmp(ParamName, "KeepAliveThreshold") == 0)
    {
        pGreIf->KeepAliveThreshold = uValue;
        pGreIf->ChangeFlag |= GREIF_CF_KEEPTHRE;
        return TRUE;
    }
    if (strcmp(ParamName, "KeepAliveCount") == 0)
    {
        pGreIf->KeepAliveCount = uValue;
        pGreIf->ChangeFlag |= GREIF_CF_KEEPCNT;
        return TRUE;
    }
    if (strcmp(ParamName, "KeepAliveFailInterval") == 0)
    {
        pGreIf->KeepAliveFailInterval = uValue;
        pGreIf->ChangeFlag |= GREIF_CF_KEEPFAILITVL;
        return TRUE;
    }
    if (strcmp(ParamName, "ReconnectPrimary") == 0)
    {
        pGreIf->ReconnectPrimary = uValue;
        pGreIf->ChangeFlag |= GREIF_CF_RECONNPRIM;
        return TRUE;
    }

    return FALSE;
}

BOOL
GreIf_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    )
{
    COSA_DML_GRE_IF                 *pGreIf      = (COSA_DML_GRE_IF *)hInsContext;
    errno_t     rc =  -1;
    int ind = -1;

    rc = strcmp_s("LocalInterfaces", strlen("LocalInterfaces"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        rc = STRCPY_S_NOCLOBBER(pGreIf->LocalInterfaces,sizeof(pGreIf->LocalInterfaces), strValue);
        if(rc != EOK)
        {
          ERR_CHK(rc);
          return FALSE;
        }
        pGreIf->ChangeFlag |= GREIF_CF_LOCALIF;
        return TRUE;
    }
    rc = strcmp_s("RemoteEndpoints", strlen("RemoteEndpoints"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        rc = STRCPY_S_NOCLOBBER(pGreIf->RemoteEndpoints,sizeof(pGreIf->RemoteEndpoints),strValue);
        if(rc != EOK)
        {
          ERR_CHK(rc);
          return FALSE;
        }
        pGreIf->ChangeFlag |= GREIF_CF_RMEP;
        return TRUE;
    }
    rc = strcmp_s("KeyIdentifier", strlen("KeyIdentifier"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        rc = STRCPY_S_NOCLOBBER(pGreIf->KeyIdentifier,sizeof(pGreIf->KeyIdentifier),strValue);
        if(rc != EOK)
        {
          ERR_CHK(rc);
          return FALSE;
        }
        pGreIf->ChangeFlag |= GREIF_CF_KEYID;
        return TRUE;
    }
    rc = strcmp_s("AssociatedBridges", strlen("AssociatedBridges"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        rc = STRCPY_S_NOCLOBBER(pGreIf->AssociatedBridges,sizeof(pGreIf->AssociatedBridges),strValue);
        if(rc != EOK)
        {
          ERR_CHK(rc);
          return FALSE;
        }
        pGreIf->ChangeFlag |= GREIF_CF_ASSOBR;
        return TRUE;
    }
    rc = strcmp_s("AssociatedBridgesWiFiPort", strlen("AssociatedBridgesWiFiPort"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        rc = STRCPY_S_NOCLOBBER(pGreIf->AssociatedBridgesWiFiPort,sizeof(pGreIf->AssociatedBridgesWiFiPort),strValue);
        if(rc != EOK)
        {
          ERR_CHK(rc);
          return FALSE;
        }
        pGreIf->ChangeFlag |= GREIF_CF_ASSOBRWFP;
        return TRUE;
    }
    rc = strcmp_s("GRENetworkInterface", strlen("GRENetworkInterface"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        rc = STRCPY_S_NOCLOBBER(pGreIf->GRENetworkInterface,sizeof(pGreIf->GRENetworkInterface),strValue);
        if(rc != EOK)
        {
          ERR_CHK(rc);
          return FALSE;
        }
        pGreIf->ChangeFlag |= GREIF_CF_GREIF;
        return TRUE;
    }

    return FALSE;
}

BOOL
GreIf_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    )
{
    COSA_DML_GRE_IF                 *pGreIf      = (COSA_DML_GRE_IF *)hInsContext;

    if (strcmp(ParamName, "DSCPMarkPolicy") == 0)
    {
        if(pGreIf->DSCPMarkPolicy == value)
            return TRUE;

        pGreIf->DSCPMarkPolicy = value;
        pGreIf->ChangeFlag |= GREIF_CF_DSCP;
        return TRUE;
    }
    if (strcmp(ParamName, "VLANID") == 0)
    {
        pGreIf->VLANID = value;
        pGreIf->ChangeFlag |= GREIF_CF_VLANID;
        return TRUE;
    }

    return FALSE;
}

BOOL
GreIf_Validate
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
GreIf_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    COSA_DML_GRE_IF                 *pGreIf      = (COSA_DML_GRE_IF *)hInsContext;
    ULONG                           ins = pGreIf->InstanceNumber;

    if (pGreIf->ChangeFlag == 0)
        return ANSC_STATUS_SUCCESS;

    if (pGreIf->ChangeFlag & GREIF_CF_ENABLE)
    {
        if (CosaDml_GreIfSetEnable(ins, pGreIf->Enable) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
    if (pGreIf->ChangeFlag & GREIF_CF_LOCALIF)
    {
        if (CosaDml_GreIfSetLocalInterfaces(ins, pGreIf->LocalInterfaces) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
    if (pGreIf->ChangeFlag & GREIF_CF_RMEP)
    {
        if (CosaDml_GreIfSetEndpoints(ins, pGreIf->RemoteEndpoints) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
    if (pGreIf->ChangeFlag & GREIF_CF_KEYGENPOL)
    {
        if (CosaDml_GreIfSetKeyGenPolicy(ins, pGreIf->KeyIdentifierGenerationPolicy) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
    if (pGreIf->ChangeFlag & GREIF_CF_KEYID)
    {
        if (CosaDml_GreIfSetKeyId(ins, pGreIf->KeyIdentifier) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
    if (pGreIf->ChangeFlag & GREIF_CF_USESEQ)
    {
        if (CosaDml_GreIfSetUseSeqNum(ins, pGreIf->UseSequenceNumber) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
    if (pGreIf->ChangeFlag & GREIF_CF_USECSUM)
    {
        if (CosaDml_GreIfSetUseChecksum(ins, pGreIf->UseChecksum) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
    if (pGreIf->ChangeFlag & GREIF_CF_DSCP)
    {
        if (CosaDml_GreIfSetDSCPMarkPolicy(ins, pGreIf->DSCPMarkPolicy) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
    if (pGreIf->ChangeFlag & GREIF_CF_VLANID)
    {
        if (CosaDml_GreIfSetVlanId(ins, pGreIf->VLANID) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
    if (pGreIf->ChangeFlag & GREIF_CF_KEEPPOL)
    {
        if (CosaDml_GreIfSetKeepAlivePolicy(ins, pGreIf->KeepAlivePolicy) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
    if (pGreIf->ChangeFlag & GREIF_CF_KEEPITVL)
    {
        if (CosaDml_GreIfSetKeepAliveInterval(ins, pGreIf->KeepAliveInterval) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
    if (pGreIf->ChangeFlag & GREIF_CF_KEEPTHRE)
    {
        if (CosaDml_GreIfSetKeepAliveThreshold(ins, pGreIf->KeepAliveThreshold) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
    if (pGreIf->ChangeFlag & GREIF_CF_KEEPCNT)
    {
        if (CosaDml_GreIfSetKeepAliveCount(ins, pGreIf->KeepAliveCount) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
    if (pGreIf->ChangeFlag & GREIF_CF_KEEPFAILITVL)
    {
        if (CosaDml_GreIfSetKeepAliveFailInterval(ins, pGreIf->KeepAliveFailInterval) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
    if (pGreIf->ChangeFlag & GREIF_CF_RECONNPRIM)
    {
        if (CosaDml_GreIfSetReconnPrimary(ins, pGreIf->ReconnectPrimary) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
    if (pGreIf->ChangeFlag & GREIF_CF_DHCPCIRID)
    {
        if (CosaDml_GreIfSetDhcpCircuitSsid(ins, pGreIf->DHCPCircuitIDSSID) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
    if (pGreIf->ChangeFlag & GREIF_CF_DHCPRMID)
    {
        if (CosaDml_GreIfSetDhcpRemoteId(ins, pGreIf->DHCPRemoteID) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
    if (pGreIf->ChangeFlag & GREIF_CF_ASSOBR)
    {
        if (CosaDml_GreIfSetAssociatedBridges(ins, pGreIf->AssociatedBridges) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
    if (pGreIf->ChangeFlag & GREIF_CF_ASSOBRWFP)
    {
        if (CosaDml_GreIfSetAssociatedBridgesWiFiPort(ins, pGreIf->AssociatedBridgesWiFiPort) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
    if (pGreIf->ChangeFlag & GREIF_CF_GREIF)
    {
        if (CosaDml_GreIfSetGREInterface(ins, pGreIf->GRENetworkInterface) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }

    pGreIf->ChangeFlag = 0;
    return ANSC_STATUS_SUCCESS;

rollback:
    pGreIf->ChangeFlag = 0;
    GreIf_Rollback((ANSC_HANDLE)pGreIf);
    return ANSC_STATUS_FAILURE;
}

ULONG
GreIf_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    COSA_DML_GRE_IF                 *pGreIf      = (COSA_DML_GRE_IF *)hInsContext;
    ULONG                           ins = pGreIf->InstanceNumber;

    if (CosaDml_GreIfGetEnable(ins, &pGreIf->Enable) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    if (CosaDml_GreIfGetLocalInterfaces(ins, pGreIf->LocalInterfaces, sizeof(pGreIf->LocalInterfaces)) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    if (CosaDml_GreIfGetEndpoints(ins, pGreIf->RemoteEndpoints, sizeof(pGreIf->RemoteEndpoints)) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    if (CosaDml_GreIfGetKeyGenPolicy(ins, &pGreIf->KeyIdentifierGenerationPolicy) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    if (CosaDml_GreIfGetKeyId(ins, pGreIf->KeyIdentifier, sizeof(pGreIf->KeyIdentifier)) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    if (CosaDml_GreIfGetUseSeqNum(ins, &pGreIf->UseSequenceNumber) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    if (CosaDml_GreIfGetUseChecksum(ins, &pGreIf->UseChecksum) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    if (CosaDml_GreIfGetDSCPMarkPolicy(ins, &pGreIf->DSCPMarkPolicy) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    if (CosaDml_GreIfGetVlanId(ins, &pGreIf->VLANID) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    if (CosaDml_GreIfGetKeepAlivePolicy(ins, &pGreIf->KeepAlivePolicy) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    if (CosaDml_GreIfGetKeepAliveInterval(ins, &pGreIf->KeepAliveInterval) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    if (CosaDml_GreIfGetKeepAliveThreshold(ins, &pGreIf->KeepAliveThreshold) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    if (CosaDml_GreIfGetKeepAliveCount(ins, &pGreIf->KeepAliveCount) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    if (CosaDml_GreIfGetKeepAliveFailInterval(ins, &pGreIf->KeepAliveFailInterval) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    if (CosaDml_GreIfGetReconnPrimary(ins, &pGreIf->ReconnectPrimary) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    if (CosaDml_GreIfGetDhcpCircuitSsid(ins, &pGreIf->DHCPCircuitIDSSID) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    if (CosaDml_GreIfGetDhcpRemoteId(ins, &pGreIf->DHCPRemoteID) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    if (CosaDml_GreIfGetAssociatedBridges(ins, (void*)&pGreIf->AssociatedBridges, sizeof(pGreIf->AssociatedBridges)) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    if (CosaDml_GreIfGetAssociatedBridgesWiFiPort(ins, (void*)&pGreIf->AssociatedBridgesWiFiPort, sizeof(pGreIf->AssociatedBridgesWiFiPort)) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    if (CosaDml_GreIfGetGREInterface(ins, (void*)&pGreIf->GRENetworkInterface, sizeof(pGreIf->GRENetworkInterface)) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

BOOL
GreIfStat_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    )
{
    COSA_DML_GRE_IF                 *pGreIf      = (COSA_DML_GRE_IF *)hInsContext;
    COSA_DML_GRE_IF_STATS           *pStats      = (COSA_DML_GRE_IF_STATS *)&pGreIf->IfStats;

    CosaDml_GreIfGetStats(pGreIf->InstanceNumber, pStats);

    if (strcmp(ParamName, "KeepAliveSent") == 0)
    {
        *pUlong = pStats->KeepAliveSent;
        return TRUE;
    }
    if (strcmp(ParamName, "KeepAliveReceived") == 0)
    {
        *pUlong = pStats->KeepAliveReceived;
        return TRUE;
    }
    if (strcmp(ParamName, "DiscardChecksumReceived") == 0)
    {
        *pUlong = pStats->DiscardChecksumReceived;
        return TRUE;
    }
    if (strcmp(ParamName, "DiscardSequenceNumberReceived") == 0)
    {
        *pUlong = pStats->DiscardSequenceNumberReceived;
        return TRUE;
    }

    return FALSE;
}

#endif
