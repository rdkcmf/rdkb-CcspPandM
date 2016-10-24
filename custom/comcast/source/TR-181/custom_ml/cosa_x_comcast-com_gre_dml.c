/**********************************************************************
   Copyright [2015] [Comcast Corp.]
 
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

    module: cosa_x_comcast-com_gre_dml.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    copyright:

        Comcast Corp.
        All Rights Reserved.

    -------------------------------------------------------------------

    description:

        This file defines the functions for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        zhicheng_qiu@cable.comcast.com

    -------------------------------------------------------------------

    revision:

        05/18/2015    initial revision.

**************************************************************************/

#include "dml_tr181_custom_cfg.h"

#ifdef CONFIG_CISCO_HOTSPOT
#include "cosa_x_comcast-com_gre_dml.h"
#include "cosa_x_comcast-com_gre_internal.h"

ULONG GreTunnel_GetEntryCount (  ANSC_HANDLE hInsContext  ) {
    return CosaDml_GreTunnelGetNumberOfEntries();
}

ULONG GreTunnelIf_GetEntryCount (  ANSC_HANDLE hInsContext  ) {
	//COSA_DATAMODEL_GRET              *pMyObject = (COSA_DATAMODEL_GRETU *)g_pCosaBEManager->hGRE;
	//TUDO: get Tunnel index somewhere
	return CosaDml_GreTunnelIfGetNumberOfEntries(1);
}

ANSC_HANDLE GreTunnel_GetEntry( ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber ) {
	COSA_DATAMODEL_GRE2           *pMyObject   = (COSA_DATAMODEL_GRE2 *)g_pCosaBEManager->hTGRE;
	
    if (nIndex >= MAX_GRE_TU)
        return NULL;

    *pInsNumber = pMyObject->GreTu[nIndex].InstanceNumber;
    return (ANSC_HANDLE)&pMyObject->GreTu[nIndex];
}

ANSC_HANDLE GreTunnelIf_GetEntry( ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber ) {
	COSA_DATAMODEL_GRE2             *pGre2   = (COSA_DATAMODEL_GRE2 *)g_pCosaBEManager->hTGRE;
	COSA_DML_GRE_TUNNEL_IF			*pTuIf=NULL;
    if (nIndex >= MAX_GRE_TUIF)
        return NULL;

	//TODO: donot hardcode tunnel index
    *pInsNumber = pGre2->GreTu[0].GreTunnelIf[nIndex].InstanceNumber;
    return (ANSC_HANDLE)&pGre2->GreTu[0].GreTunnelIf[nIndex];
}

BOOL GreTunnel_GetParamBoolValue ( ANSC_HANDLE hInsContext, char*  ParamName, BOOL*  pBool) {
	COSA_DML_GRE_TUNNEL                 *pGreTu      = (COSA_DML_GRE_TUNNEL *)hInsContext;
    //ULONG                           ins = pGreTu->InstanceNumber;

    if (AnscEqualString(ParamName, "Enable", TRUE))
    {
        *pBool = pGreTu->Enable;
        return TRUE;
    }
	if (AnscEqualString(ParamName, "HotSpotReset", TRUE))
	{
		//always return false 
		*pBool = pGreTu->HotSpotReset;
		return TRUE;
	}
    if (AnscEqualString(ParamName, "UseSequenceNumber", TRUE))
    {
        *pBool = pGreTu->UseSequenceNumber;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "UseChecksum", TRUE))
    {
        *pBool = pGreTu->UseChecksum;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "EnableCircuitID", TRUE))
    {
        *pBool = pGreTu->EnableCircuitID;
        if(pGreTu->EnableCircuitID) {
		// update  circuit ids 
		//TODO: hotspot_update_circuit_ids ?
			hotspot_update_circuit_ids(1,1);
	    }
        return TRUE;
    }
    if (AnscEqualString(ParamName, "EnableRemoteID", TRUE))
    {
        *pBool = pGreTu->EnableRemoteID;
        return TRUE;
    }

    return FALSE;
}

BOOL GreTunnelIf_GetParamBoolValue ( ANSC_HANDLE hInsContext, char*  ParamName, BOOL*  pBool) {
	COSA_DML_GRE_TUNNEL_IF                 *pGreTuIf      = (COSA_DML_GRE_TUNNEL_IF *)hInsContext;
    //ULONG                           ins = pGreTuIf->InstanceNumber;

    if (AnscEqualString(ParamName, "Enable", TRUE))
    {
        *pBool = pGreTuIf->Enable;
        return TRUE;
    }
	
	return FALSE;
}

BOOL GreTunnel_GetParamUlongValue ( ANSC_HANDLE hInsContext, char* ParamName, ULONG* pUlong) {
    COSA_DML_GRE_TUNNEL                 *pGreTu      = (COSA_DML_GRE_TUNNEL *)hInsContext;
    ULONG                           ins = pGreTu->InstanceNumber;

    if (AnscEqualString(ParamName, "Status", TRUE))
    {
        if (CosaDml_GreTunnelGetStatus(ins, (COSA_DML_GRE_STATUS *)pUlong) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "LastChange", TRUE))
    {
        if (CosaDml_GreTunnelGetLastchange(ins, pUlong) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "KeyIdentifierGenerationPolicy", TRUE))
    {
        *pUlong = pGreTu->KeyIdentifierGenerationPolicy;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "KeepAlivePolicy", TRUE))
    {
        *pUlong = pGreTu->KeepAlivePolicy;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "RemoteEndpointHealthCheckPingInterval", TRUE))
    {
        *pUlong = pGreTu->RemoteEndpointHealthCheckPingInterval;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "RemoteEndpointHealthCheckPingFailThreshold", TRUE))
    {
        *pUlong = pGreTu->RemoteEndpointHealthCheckPingFailThreshold;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "RemoteEndpointHealthCheckPingCount", TRUE))
    {
        *pUlong = pGreTu->RemoteEndpointHealthCheckPingCount;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "RemoteEndpointHealthCheckPingIntervalInFailure", TRUE))
    {
        *pUlong = pGreTu->RemoteEndpointHealthCheckPingIntervalInFailure;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "ReconnectToPrimaryRemoteEndpoint", TRUE))
    {
        *pUlong = pGreTu->ReconnectToPrimaryRemoteEndpoint;
        return TRUE;
    }

    return FALSE;
}

BOOL GreTunnelIf_GetParamUlongValue ( ANSC_HANDLE hInsContext, char* ParamName, ULONG* pUlong) {
	COSA_DML_GRE_TUNNEL_IF                 *pGreTuIf      = (COSA_DML_GRE_TUNNEL_IF *)hInsContext;
    ULONG                           ins = pGreTuIf->InstanceNumber;

    if (AnscEqualString(ParamName, "Status", TRUE))
    {
		//TODO: do not hardcode tunnel id
        if (CosaDml_GreTunnelIfGetStatus(1, ins, (COSA_DML_GRE_STATUS *)pUlong) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "LastChange", TRUE))
    {
        if (CosaDml_GreTunnelIfGetLastchange(1, ins, pUlong) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }

    if (AnscEqualString(ParamName, "VLANID", TRUE))
    {
        *pUlong = pGreTuIf->VLANID;
        return TRUE;
    } 
	
	return FALSE;
}

ULONG GreTunnel_GetParamStringValue ( ANSC_HANDLE  hInsContext, char* ParamName, char* pValue, ULONG* pUlSize ) {
	COSA_DML_GRE_TUNNEL                 *pGreTu      = (COSA_DML_GRE_TUNNEL *)hInsContext;
    ULONG                           ins = pGreTu->InstanceNumber;

    if (AnscEqualString(ParamName, "PrimaryRemoteEndpoint", TRUE))
    {
        snprintf(pValue, *pUlSize, "%s", pGreTu->PrimaryRemoteEndpoint);
        return 0;
    }
	if (AnscEqualString(ParamName, "SecondaryRemoteEndpoint", TRUE))
    {
        snprintf(pValue, *pUlSize, "%s", pGreTu->SecondaryRemoteEndpoint);
        return 0;
    }
    if (AnscEqualString(ParamName, "ConnectedRemoteEndpoint", TRUE))
    {	
		CosaDml_GreTunnelGetConnectedRemoteEndpoint(ins,pGreTu);
	   	snprintf(pValue, *pUlSize, "%s", pGreTu->ConnectedRemoteEndpoint);
        return 0;
    }
    if (AnscEqualString(ParamName, "KeyIdentifier", TRUE))
    {
        snprintf(pValue, *pUlSize, "%s", pGreTu->KeyIdentifier);
        return 0;
    }
    if (AnscEqualString(ParamName, "GRENetworkTunnel", TRUE))
    {
        snprintf(pValue, *pUlSize, "%s", pGreTu->GRENetworkTunnel);
        return 0;
    }

    return -1;
}

ULONG GreTunnelIf_GetParamStringValue ( ANSC_HANDLE  hInsContext, char* ParamName, char* pValue, ULONG* pUlSize ) {
	COSA_DML_GRE_TUNNEL_IF                 *pGreTuIf      = (COSA_DML_GRE_TUNNEL_IF *)hInsContext;
    //ULONG                           ins = pGreTuIf->InstanceNumber;

    if (AnscEqualString(ParamName, "LocalInterfaces", TRUE))
    {
        snprintf(pValue, *pUlSize, "%s", pGreTuIf->LocalInterfaces);
        return 0;
    }
	if (AnscEqualString(ParamName, "AssociatedBridges", TRUE))
    {
        snprintf(pValue, *pUlSize, "%s", pGreTuIf->AssociatedBridges);
        return 0;
    }
    if (AnscEqualString(ParamName, "AssociatedBridgesWiFiPort", TRUE))
    {
        snprintf(pValue, *pUlSize, "%s", pGreTuIf->AssociatedBridgesWiFiPort);
        return 0;
    }

	return -1;
}

BOOL GreTunnel_GetParamIntValue ( ANSC_HANDLE hInsContext, char* ParamName, int*  pInt ) {
    COSA_DML_GRE_TUNNEL                 *pGreTu      = (COSA_DML_GRE_TUNNEL *)hInsContext;
    //ULONG                           ins = pGreTu->InstanceNumber;

    if (AnscEqualString(ParamName, "DSCPMarkPolicy", TRUE))
    {
        *pInt = pGreTu->DSCPMarkPolicy;
        return TRUE;
    }   

    return FALSE;
}


BOOL GreTunnelIf_GetParamIntValue ( ANSC_HANDLE hInsContext, char* ParamName, int*  pInt ) {
	COSA_DML_GRE_TUNNEL_IF                 *pGreTuIf      = (COSA_DML_GRE_TUNNEL_IF *)hInsContext;
    //ULONG                           ins = pGreTuIf->InstanceNumber;

    if (AnscEqualString(ParamName, "VLANID", TRUE))
    {
        *pInt = pGreTuIf->VLANID;
        return TRUE;
    } 

    return FALSE;
}

BOOL GreTunnel_SetParamBoolValue ( ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue) {
	COSA_DML_GRE_TUNNEL                 *pGreTu      = (COSA_DML_GRE_TUNNEL *)hInsContext;

    if (AnscEqualString(ParamName, "Enable", TRUE))
    {
        pGreTu->Enable = bValue;
        pGreTu->ChangeFlag |= GRETU_CF_ENABLE;
        return TRUE;
    }
	if (AnscEqualString(ParamName, "HotSpotReset", TRUE))
	{
		if(bValue)
		{
			CosaDml_GreTunnelHotspotReset(pGreTu);
			pGreTu->HotSpotReset = FALSE;
			pGreTu->ChangeFlag = (GRETU_CF_DSCP|GRETU_CF_PRIEP|GRETU_CF_SECEP|GRETU_CF_KEEPCNT|GRETU_CF_KEEPITVL|GRETU_CF_KEEPTHRE|GRETU_CF_KEEPFAILITVL|GRETU_CF_RECONNPRIM|GRETU_CF_DHCPCIRID|GRETU_CF_DHCPRMID);
		}
		return TRUE;
	}
    if (AnscEqualString(ParamName, "UseSequenceNumber", TRUE))
    {
        pGreTu->UseSequenceNumber = bValue;
        pGreTu->ChangeFlag |= GRETU_CF_USESEQ;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "UseChecksum", TRUE))
    {
        pGreTu->UseChecksum = bValue;
        pGreTu->ChangeFlag |= GRETU_CF_USECSUM;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "EnableCircuitID", TRUE))
    {
        pGreTu->EnableCircuitID = bValue;
        pGreTu->ChangeFlag |= GRETU_CF_DHCPCIRID;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "EnableRemoteID", TRUE))
    {
        pGreTu->EnableRemoteID = bValue;
        pGreTu->ChangeFlag |= GRETU_CF_DHCPRMID;
        return TRUE;
    }

    return FALSE;
}


BOOL GreTunnelIf_SetParamBoolValue ( ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue) {
	COSA_DML_GRE_TUNNEL_IF                 *pGreTuIf      = (COSA_DML_GRE_TUNNEL_IF *)hInsContext;

    if (AnscEqualString(ParamName, "Enable", TRUE))
    {
        pGreTuIf->Enable = bValue;
        pGreTuIf->ChangeFlag |= GRETUIF_CF_ENABLE;
        return TRUE;
    }
	return FALSE;
}

BOOL GreTunnel_SetParamUlongValue ( ANSC_HANDLE  hInsContext, char* ParamName, ULONG  uValue) {
	COSA_DML_GRE_TUNNEL                 *pGreTu      = (COSA_DML_GRE_TUNNEL *)hInsContext;

    if (AnscEqualString(ParamName, "KeyIdentifierGenerationPolicy", TRUE))
    {
        pGreTu->KeyIdentifierGenerationPolicy = uValue;
        pGreTu->ChangeFlag |= GRETU_CF_KEYGENPOL;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "KeepAlivePolicy", TRUE))
    {
        pGreTu->KeepAlivePolicy = uValue;
        pGreTu->ChangeFlag |= GRETU_CF_KEEPPOL;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "RemoteEndpointHealthCheckPingInterval", TRUE))
    {
        pGreTu->RemoteEndpointHealthCheckPingInterval = uValue;
        pGreTu->ChangeFlag |= GRETU_CF_KEEPITVL;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "RemoteEndpointHealthCheckPingFailThreshold", TRUE))
    {
        pGreTu->RemoteEndpointHealthCheckPingFailThreshold = uValue;
        pGreTu->ChangeFlag |= GRETU_CF_KEEPTHRE;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "RemoteEndpointHealthCheckPingCount", TRUE))
    {
        pGreTu->RemoteEndpointHealthCheckPingCount = uValue;
        pGreTu->ChangeFlag |= GRETU_CF_KEEPCNT;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "RemoteEndpointHealthCheckPingIntervalInFailure", TRUE))
    {
        pGreTu->RemoteEndpointHealthCheckPingIntervalInFailure = uValue;
        pGreTu->ChangeFlag |= GRETU_CF_KEEPFAILITVL;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "ReconnectToPrimaryRemoteEndpoint", TRUE))
    {
        pGreTu->ReconnectToPrimaryRemoteEndpoint = uValue;
        pGreTu->ChangeFlag |= GRETU_CF_RECONNPRIM;
        return TRUE;
    }

    return FALSE;
}

BOOL GreTunnelIf_SetParamUlongValue ( ANSC_HANDLE  hInsContext, char* ParamName, ULONG  uValuepUlong) {
	COSA_DML_GRE_TUNNEL_IF                 *pGreTuIf      = (COSA_DML_GRE_TUNNEL_IF *)hInsContext;
    if (AnscEqualString(ParamName, "VLANID", TRUE))
    {
        pGreTuIf->VLANID = uValuepUlong;
        pGreTuIf->ChangeFlag |= GRETUIF_CF_VLANID;
        return TRUE;
    }
	return FALSE;
}

BOOL GreTunnel_SetParamStringValue ( ANSC_HANDLE hInsContext, char*  ParamName, char*  strValue ) {
	COSA_DML_GRE_TUNNEL                 *pGreTu      = (COSA_DML_GRE_TUNNEL *)hInsContext;
    
    if (AnscEqualString(ParamName, "PrimaryRemoteEndpoint", TRUE))
    {
        snprintf(pGreTu->PrimaryRemoteEndpoint, sizeof(pGreTu->PrimaryRemoteEndpoint), "%s", strValue);
        pGreTu->ChangeFlag |= GRETU_CF_PRIEP;
        return TRUE;
    }
	if (AnscEqualString(ParamName, "SecondaryRemoteEndpoint", TRUE))
    {
        snprintf(pGreTu->SecondaryRemoteEndpoint, sizeof(pGreTu->SecondaryRemoteEndpoint), "%s", strValue);
        pGreTu->ChangeFlag |= GRETU_CF_SECEP;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "KeyIdentifier", TRUE))
    {
        snprintf(pGreTu->KeyIdentifier, sizeof(pGreTu->KeyIdentifier), "%s", strValue);
        pGreTu->ChangeFlag |= GRETU_CF_KEYID;
        return TRUE;
    }    
    if (AnscEqualString(ParamName, "GRENetworkTunnel", TRUE))
    {
        snprintf(pGreTu->GRENetworkTunnel, sizeof(pGreTu->GRENetworkTunnel), "%s", strValue);
        pGreTu->ChangeFlag |= GRETU_CF_GRETU;
        return TRUE;
    }

    return FALSE;
}

BOOL GreTunnelIf_SetParamStringValue ( ANSC_HANDLE hInsContext, char*  ParamName, char*  strValue ) {
	COSA_DML_GRE_TUNNEL_IF                 *pGreTuIf      = (COSA_DML_GRE_TUNNEL_IF *)hInsContext;

    if (AnscEqualString(ParamName, "LocalInterfaces", TRUE))
    {
        snprintf(pGreTuIf->LocalInterfaces, sizeof(pGreTuIf->LocalInterfaces), "%s", strValue);
        pGreTuIf->ChangeFlag |= GRETUIF_CF_LOCALIF;
        return TRUE;
    }
	if (AnscEqualString(ParamName, "AssociatedBridges", TRUE))
    {
        snprintf(pGreTuIf->AssociatedBridges, sizeof(pGreTuIf->AssociatedBridges), "%s", strValue);
        pGreTuIf->ChangeFlag |= GRETUIF_CF_ASSOBR;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "AssociatedBridgesWiFiPort", TRUE))
    {
        snprintf(pGreTuIf->AssociatedBridgesWiFiPort, sizeof(pGreTuIf->AssociatedBridgesWiFiPort), "%s", strValue);
        pGreTuIf->ChangeFlag |= GRETUIF_CF_ASSOBRWFP;
        return TRUE;
    }
	return FALSE;
}

BOOL GreTunnel_SetParamIntValue ( ANSC_HANDLE hInsContext, char* ParamName, int value ) {
	COSA_DML_GRE_TUNNEL                 *pGreTu      = (COSA_DML_GRE_TUNNEL *)hInsContext;

    if (AnscEqualString(ParamName, "DSCPMarkPolicy", TRUE))
    {
        if(pGreTu->DSCPMarkPolicy == value)
            return TRUE;

        pGreTu->DSCPMarkPolicy = value;
        pGreTu->ChangeFlag |= GRETU_CF_DSCP;
        return TRUE;
    }
    
    return FALSE;
}

BOOL GreTunnelIf_SetParamIntValue ( ANSC_HANDLE hInsContext, char* ParamName, int value ) {
	COSA_DML_GRE_TUNNEL_IF                 *pGreTuIf      = (COSA_DML_GRE_TUNNEL_IF *)hInsContext;

    if (AnscEqualString(ParamName, "VLANID", TRUE))
    {
        pGreTuIf->VLANID = value;
        pGreTuIf->ChangeFlag |= GRETUIF_CF_VLANID;
        return TRUE;
    }

    return FALSE;
}

BOOL GreTunnel_Validate ( ANSC_HANDLE hInsContext, char* pReturnParamName, ULONG* puLength ) 
{
    //All the parameters are validated in hotspot code
	return TRUE;
}	

BOOL GreTunnelIf_Validate ( ANSC_HANDLE hInsContext, char* pReturnParamName, ULONG* puLength )
{
	return TRUE;
}	

ULONG GreTunnel_Commit ( ANSC_HANDLE hInsContext ) {
	COSA_DML_GRE_TUNNEL                 *pGreTu      = (COSA_DML_GRE_TUNNEL *)hInsContext;
    ULONG                           ins = pGreTu->InstanceNumber;

    if (pGreTu->ChangeFlag == 0)
        return ANSC_STATUS_SUCCESS;

    if (pGreTu->ChangeFlag & GRETU_CF_ENABLE)
    {
        if (CosaDml_GreTunnelSetEnable(ins, pGreTu->Enable) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
    
    if (pGreTu->ChangeFlag & GRETU_CF_PRIEP)
    {
        if (CosaDml_GreTunnelSetPrimaryEndpoints(ins, pGreTu->PrimaryRemoteEndpoint) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
	if (pGreTu->ChangeFlag & GRETU_CF_SECEP)
    {
        if (CosaDml_GreTunnelSetSecondaryEndpoints(ins, pGreTu->SecondaryRemoteEndpoint) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
    if (pGreTu->ChangeFlag & GRETU_CF_KEYGENPOL)
    {
        if (CosaDml_GreTunnelSetKeyGenPolicy(ins, pGreTu->KeyIdentifierGenerationPolicy) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
    if (pGreTu->ChangeFlag & GRETU_CF_KEYID)
    {
        if (CosaDml_GreTunnelSetKeyId(ins, pGreTu->KeyIdentifier) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
    if (pGreTu->ChangeFlag & GRETU_CF_USESEQ)
    {
        if (CosaDml_GreTunnelSetUseSeqNum(ins, pGreTu->UseSequenceNumber) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
    if (pGreTu->ChangeFlag & GRETU_CF_USECSUM)
    {
        if (CosaDml_GreTunnelSetUseChecksum(ins, pGreTu->UseChecksum) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
    if (pGreTu->ChangeFlag & GRETU_CF_DSCP)
    {
        if (CosaDml_GreTunnelSetDSCPMarkPolicy(ins, pGreTu->DSCPMarkPolicy) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
    
    if (pGreTu->ChangeFlag & GRETU_CF_KEEPPOL)
    {
        if (CosaDml_GreTunnelSetKeepAlivePolicy(ins, pGreTu->KeepAlivePolicy) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
    if (pGreTu->ChangeFlag & GRETU_CF_KEEPITVL)
    {
        if (CosaDml_GreTunnelSetKeepAliveInterval(ins, pGreTu->RemoteEndpointHealthCheckPingInterval) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
    if (pGreTu->ChangeFlag & GRETU_CF_KEEPTHRE)
    {
        if (CosaDml_GreTunnelSetKeepAliveThreshold(ins, pGreTu->RemoteEndpointHealthCheckPingFailThreshold) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
    if (pGreTu->ChangeFlag & GRETU_CF_KEEPCNT)
    {
        if (CosaDml_GreTunnelSetKeepAliveCount(ins, pGreTu->RemoteEndpointHealthCheckPingCount) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
    if (pGreTu->ChangeFlag & GRETU_CF_KEEPFAILITVL)
    {
        if (CosaDml_GreTunnelSetKeepAliveFailInterval(ins, pGreTu->RemoteEndpointHealthCheckPingIntervalInFailure) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
    if (pGreTu->ChangeFlag & GRETU_CF_RECONNPRIM)
    {
        if (CosaDml_GreTunnelSetReconnPrimary(ins, pGreTu->ReconnectToPrimaryRemoteEndpoint) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
    if (pGreTu->ChangeFlag & GRETU_CF_DHCPCIRID)
    {
        if (CosaDml_GreTunnelSetDhcpCircuitSsid(ins, pGreTu->EnableCircuitID) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
    if (pGreTu->ChangeFlag & GRETU_CF_DHCPRMID)
    {
        if (CosaDml_GreTunnelSetDhcpRemoteId(ins, pGreTu->EnableRemoteID) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
    
    if (pGreTu->ChangeFlag & GRETU_CF_GRETU)
    {
        if (CosaDml_GreTunnelGetGRETunnel(ins, pGreTu->GRENetworkTunnel, sizeof(pGreTu->GRENetworkTunnel)) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }

    pGreTu->ChangeFlag = 0;
    return ANSC_STATUS_SUCCESS;

rollback:
    pGreTu->ChangeFlag = 0;
    GreTunnel_Rollback((ANSC_HANDLE)pGreTu);
    return ANSC_STATUS_FAILURE;
}

ULONG GreTunnelIf_Commit ( ANSC_HANDLE hInsContext ) {
	COSA_DML_GRE_TUNNEL_IF                 *pGreTuIf      = (COSA_DML_GRE_TUNNEL_IF *)hInsContext;
    ULONG                           ins = pGreTuIf->InstanceNumber;

    if (pGreTuIf->ChangeFlag == 0)
        return ANSC_STATUS_SUCCESS;

    if (pGreTuIf->ChangeFlag & GRETUIF_CF_ENABLE)
    {
	//TODO: do not hardcode Tunnel index
        if (CosaDml_GreTunnelIfSetEnable(1, ins, pGreTuIf->Enable) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
	if (pGreTuIf->ChangeFlag & GRETUIF_CF_LOCALIF)
    {
        if (CosaDml_GreTunnelIfSetLocalInterfaces(1, ins, pGreTuIf->LocalInterfaces) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }	
	if (pGreTuIf->ChangeFlag & GRETUIF_CF_VLANID)
    {
        if (CosaDml_GreTunnelIfSetVlanId(1, ins, pGreTuIf->VLANID) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
	if (pGreTuIf->ChangeFlag & GRETUIF_CF_ASSOBR)
    {
        if (CosaDml_GreTunnelIfSetAssociatedBridges(1, ins, pGreTuIf->AssociatedBridges) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
    if (pGreTuIf->ChangeFlag & GRETUIF_CF_ASSOBRWFP)
    {
        if (CosaDml_GreTunnelIfSetAssociatedBridgesWiFiPort(1, ins, pGreTuIf->AssociatedBridgesWiFiPort) != ANSC_STATUS_SUCCESS)
            goto rollback;
    }
	 pGreTuIf->ChangeFlag = 0;
    return ANSC_STATUS_SUCCESS;

rollback:
    pGreTuIf->ChangeFlag = 0;
    GreTunnelIf_Rollback((ANSC_HANDLE)pGreTuIf);
    return ANSC_STATUS_FAILURE;
}

ULONG GreTunnel_Rollback( ANSC_HANDLE hInsContext) {
	COSA_DML_GRE_TUNNEL                 *pGreTu      = (COSA_DML_GRE_TUNNEL *)hInsContext;
    ULONG                           ins = pGreTu->InstanceNumber;

    if (CosaDml_GreTunnelGetEnable(ins, &pGreTu->Enable) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    if (CosaDml_GreTunnelGetPrimaryEndpoint(ins, pGreTu->PrimaryRemoteEndpoint, sizeof(pGreTu->PrimaryRemoteEndpoint)) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
	if (CosaDml_GreTunnelGetSecondaryEndpoint(ins, pGreTu->SecondaryRemoteEndpoint, sizeof(pGreTu->SecondaryRemoteEndpoint)) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;	
    if (CosaDml_GreTunnelGetKeyGenPolicy(ins, &pGreTu->KeyIdentifierGenerationPolicy) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    if (CosaDml_GreTunnelGetKeyId(ins, pGreTu->KeyIdentifier, sizeof(pGreTu->KeyIdentifier)) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    if (CosaDml_GreTunnelGetUseSeqNum(ins, &pGreTu->UseSequenceNumber) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    if (CosaDml_GreTunnelGetUseChecksum(ins, &pGreTu->UseChecksum) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    if (CosaDml_GreTunnelGetDSCPMarkPolicy(ins, &pGreTu->DSCPMarkPolicy) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;		
    if (CosaDml_GreTunnelGetKeepAlivePolicy(ins, &pGreTu->KeepAlivePolicy) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    if (CosaDml_GreTunnelGetKeepAliveInterval(ins, &pGreTu->RemoteEndpointHealthCheckPingInterval) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    if (CosaDml_GreTunnelGetKeepAliveThreshold(ins, &pGreTu->RemoteEndpointHealthCheckPingFailThreshold) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    if (CosaDml_GreTunnelGetKeepAliveCount(ins, &pGreTu->RemoteEndpointHealthCheckPingCount) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    if (CosaDml_GreTunnelGetKeepAliveFailInterval(ins, &pGreTu->RemoteEndpointHealthCheckPingIntervalInFailure) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    if (CosaDml_GreTunnelGetReconnPrimary(ins, &pGreTu->ReconnectToPrimaryRemoteEndpoint) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    if (CosaDml_GreTunnelGetDhcpCircuitSsid(ins, &pGreTu->EnableCircuitID) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    if (CosaDml_GreTunnelGetDhcpRemoteId(ins, &pGreTu->EnableRemoteID) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    if (CosaDml_GreTunnelGetGRETunnel(ins, &pGreTu->GRENetworkTunnel, sizeof(pGreTu->GRENetworkTunnel)) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ULONG GreTunnelIf_Rollback( ANSC_HANDLE hInsContext) {
	COSA_DML_GRE_TUNNEL_IF                 *pGreTuIf      = (COSA_DML_GRE_TUNNEL_IF *)hInsContext;
    ULONG                           ins = pGreTuIf->InstanceNumber;

	//TODO: do not hardcode tunnel ID
    if (CosaDml_GreTunnelIfGetEnable(1, ins, &pGreTuIf->Enable) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    if (CosaDml_GreTunnelIfGetLocalInterfaces(1, ins, pGreTuIf->LocalInterfaces, sizeof(pGreTuIf->LocalInterfaces)) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    if (CosaDml_GreTunnelIfGetVlanId(1, ins, &pGreTuIf->VLANID) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    if (CosaDml_GreTunnelIfGetAssociatedBridges(1, ins, pGreTuIf->AssociatedBridges, sizeof(pGreTuIf->AssociatedBridges)) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    if (CosaDml_GreTunnelIfGetAssociatedBridgesWiFiPort(1, ins, pGreTuIf->AssociatedBridgesWiFiPort, sizeof(pGreTuIf->AssociatedBridgesWiFiPort)) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;

	return ANSC_STATUS_SUCCESS;

}

BOOL GreTunnelStat_GetParamUlongValue ( ANSC_HANDLE hInsContext, char* ParamName, ULONG* pUlong ) {
	COSA_DML_GRE_TUNNEL                 *pGreTu      = (COSA_DML_GRE_TUNNEL *)hInsContext;
    COSA_DML_GRE_TUNNEL_STATS           *pStats      = (COSA_DML_GRE_TUNNEL_STATS *)&pGreTu->TuStats;

    CosaDml_GreTunnelGetStats(pGreTu->InstanceNumber, pStats);

    if (AnscEqualString(ParamName, "KeepAliveSent", TRUE))
    {
        *pUlong = pStats->KeepAliveSent;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "KeepAliveReceived", TRUE))
    {
        *pUlong = pStats->KeepAliveReceived;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "DiscardChecksumReceived", TRUE))
    {
        *pUlong = pStats->DiscardChecksumReceived;
        return TRUE;
    }
    if (AnscEqualString(ParamName, "DiscardSequenceNumberReceived", TRUE))
    {
        *pUlong = pStats->DiscardSequenceNumberReceived;
        return TRUE;
    }
	return FALSE;
}

#endif
