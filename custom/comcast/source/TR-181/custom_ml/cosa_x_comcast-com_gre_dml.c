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

/**************************************************************************

    module: cosa_x_comcast-com_gre_dml.c

        For COSA Data Model Library Development

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
#include "safec_lib_common.h"

int hotspot_update_circuit_ids(int greinst, int queuestart);

ULONG GreTunnel_GetEntryCount (  ANSC_HANDLE hInsContext  ) {
    UNREFERENCED_PARAMETER(hInsContext);
    return CosaDml_GreTunnelGetNumberOfEntries();
}

ULONG GreTunnelIf_GetEntryCount (  ANSC_HANDLE hInsContext  ) {
    UNREFERENCED_PARAMETER(hInsContext);
	//COSA_DATAMODEL_GRET              *pMyObject = (COSA_DATAMODEL_GRETU *)g_pCosaBEManager->hGRE;
	//TUDO: get Tunnel index somewhere
	return CosaDml_GreTunnelIfGetNumberOfEntries(1);
}

ANSC_HANDLE GreTunnel_GetEntry( ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber ) {
    UNREFERENCED_PARAMETER(hInsContext);
	COSA_DATAMODEL_GRE2           *pMyObject   = (COSA_DATAMODEL_GRE2 *)g_pCosaBEManager->hTGRE;
	
    if (nIndex >= MAX_GRE_TU)
        return NULL;

    *pInsNumber = pMyObject->GreTu[nIndex].InstanceNumber;
    return (ANSC_HANDLE)&pMyObject->GreTu[nIndex];
}

ANSC_HANDLE GreTunnelIf_GetEntry( ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber ) {
    UNREFERENCED_PARAMETER(hInsContext);
	COSA_DATAMODEL_GRE2             *pGre2   = (COSA_DATAMODEL_GRE2 *)g_pCosaBEManager->hTGRE;
    if (nIndex >= MAX_GRE_TUIF)
        return NULL;

	//TODO: donot hardcode tunnel index
    *pInsNumber = pGre2->GreTu[0].GreTunnelIf[nIndex].InstanceNumber;
    return (ANSC_HANDLE)&pGre2->GreTu[0].GreTunnelIf[nIndex];
}

void* hotspot_thread(void* arg)
{
    UNREFERENCED_PARAMETER(arg);
    static BOOL running=0;
    if(!running)
    {
      running=1;
      CcspTraceWarning(("%s:hotspot_update_circuit_ids starting\n", __FUNCTION__));
      hotspot_update_circuit_ids(1,1);
    }
    else
    {
        CcspTraceWarning(("%s: already hotspot_update_circuit_ids is running\n", __FUNCTION__));
        return NULL;
    }
    running=0;
    return NULL;
}

BOOL GreTunnel_GetParamBoolValue ( ANSC_HANDLE hInsContext, char*  ParamName, BOOL*  pBool) {
	COSA_DML_GRE_TUNNEL                 *pGreTu      = (COSA_DML_GRE_TUNNEL *)hInsContext;
    //ULONG                           ins = pGreTu->InstanceNumber;

    if (strcmp(ParamName, "Enable") == 0)
    {
        *pBool = pGreTu->Enable;
        return TRUE;
    }
	if (strcmp(ParamName, "HotSpotReset") == 0)
	{
		//always return false 
		*pBool = pGreTu->HotSpotReset;
		return TRUE;
	}
    if (strcmp(ParamName, "UseSequenceNumber") == 0)
    {
        *pBool = pGreTu->UseSequenceNumber;
        return TRUE;
    }
    if (strcmp(ParamName, "UseChecksum") == 0)
    {
        *pBool = pGreTu->UseChecksum;
        return TRUE;
    }
    if (strcmp(ParamName, "EnableCircuitID") == 0)
    {
        *pBool = pGreTu->EnableCircuitID;
        if(pGreTu->EnableCircuitID) {
            // update  circuit ids
            //TODO: hotspot_update_circuit_ids ?
            /* TCXB6-4418: CCSP Deadlock happening because of wifi getting started
               Late in Technicolor. So the PAM is getting restarted with this DeadLock.
               Now we want this as a separate thread and update circuit ids.
               The 4418 is hardly reproduce however for the precautionary measure we
               want to run this in thread */
            pthread_t hs_thread;
            int err = pthread_create(&hs_thread, NULL, hotspot_thread, NULL);
            if(0 != err)
            {
                CcspTraceError(("%s: Error in creating hotspot_thread \n", __FUNCTION__));
            }
            else
                pthread_detach(hs_thread);
	    }
        return TRUE;
    }
    if (strcmp(ParamName, "EnableRemoteID") == 0)
    {
        *pBool = pGreTu->EnableRemoteID;
        return TRUE;
    }

    return FALSE;
}

BOOL GreTunnelIf_GetParamBoolValue ( ANSC_HANDLE hInsContext, char*  ParamName, BOOL*  pBool) {
	COSA_DML_GRE_TUNNEL_IF                 *pGreTuIf      = (COSA_DML_GRE_TUNNEL_IF *)hInsContext;
    //ULONG                           ins = pGreTuIf->InstanceNumber;

    if (strcmp(ParamName, "Enable") == 0)
    {
        *pBool = pGreTuIf->Enable;
        return TRUE;
    }
	
	return FALSE;
}

BOOL GreTunnel_GetParamUlongValue ( ANSC_HANDLE hInsContext, char* ParamName, ULONG* pUlong) {
    COSA_DML_GRE_TUNNEL                 *pGreTu      = (COSA_DML_GRE_TUNNEL *)hInsContext;
    ULONG                           ins = pGreTu->InstanceNumber;

    if (strcmp(ParamName, "Status") == 0)
    {
        if (CosaDml_GreTunnelGetStatus(ins, (COSA_DML_GRE_STATUS *)pUlong) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }
    if (strcmp(ParamName, "LastChange") == 0)
    {
        if (CosaDml_GreTunnelGetLastchange(ins, pUlong) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }
    if (strcmp(ParamName, "KeyIdentifierGenerationPolicy") == 0)
    {
        *pUlong = pGreTu->KeyIdentifierGenerationPolicy;
        return TRUE;
    }
    if (strcmp(ParamName, "KeepAlivePolicy") == 0)
    {
        *pUlong = pGreTu->KeepAlivePolicy;
        return TRUE;
    }
    if (strcmp(ParamName, "RemoteEndpointHealthCheckPingInterval") == 0)
    {
        *pUlong = pGreTu->RemoteEndpointHealthCheckPingInterval;
        return TRUE;
    }
    if (strcmp(ParamName, "RemoteEndpointHealthCheckPingFailThreshold") == 0)
    {
        *pUlong = pGreTu->RemoteEndpointHealthCheckPingFailThreshold;
        return TRUE;
    }
    if (strcmp(ParamName, "RemoteEndpointHealthCheckPingCount") == 0)
    {
        *pUlong = pGreTu->RemoteEndpointHealthCheckPingCount;
        return TRUE;
    }
    if (strcmp(ParamName, "RemoteEndpointHealthCheckPingIntervalInFailure") == 0)
    {
        *pUlong = pGreTu->RemoteEndpointHealthCheckPingIntervalInFailure;
        return TRUE;
    }
    if (strcmp(ParamName, "ReconnectToPrimaryRemoteEndpoint") == 0)
    {
        *pUlong = pGreTu->ReconnectToPrimaryRemoteEndpoint;
        return TRUE;
    }

    return FALSE;
}

BOOL GreTunnelIf_GetParamUlongValue ( ANSC_HANDLE hInsContext, char* ParamName, ULONG* pUlong) {
	COSA_DML_GRE_TUNNEL_IF                 *pGreTuIf      = (COSA_DML_GRE_TUNNEL_IF *)hInsContext;
    ULONG                           ins = pGreTuIf->InstanceNumber;

    if (strcmp(ParamName, "Status") == 0)
    {
		//TODO: do not hardcode tunnel id
        if (CosaDml_GreTunnelIfGetStatus(1, ins, (COSA_DML_GRE_STATUS *)pUlong) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }
    if (strcmp(ParamName, "LastChange") == 0)
    {
        if (CosaDml_GreTunnelIfGetLastchange(1, ins, pUlong) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }

    if (strcmp(ParamName, "VLANID") == 0)
    {
        *pUlong = pGreTuIf->VLANID;
        return TRUE;
    } 
	
	return FALSE;
}

ULONG GreTunnel_GetParamStringValue ( ANSC_HANDLE  hInsContext, char* ParamName, char* pValue, ULONG* pUlSize ) {
	COSA_DML_GRE_TUNNEL                 *pGreTu      = (COSA_DML_GRE_TUNNEL *)hInsContext;
    ULONG                           ins = pGreTu->InstanceNumber;

    if (strcmp(ParamName, "PrimaryRemoteEndpoint") == 0)
    {
        snprintf(pValue, *pUlSize, "%s", pGreTu->PrimaryRemoteEndpoint);
        return 0;
    }
	if (strcmp(ParamName, "SecondaryRemoteEndpoint") == 0)
    {
        snprintf(pValue, *pUlSize, "%s", pGreTu->SecondaryRemoteEndpoint);
        return 0;
    }
    if (strcmp(ParamName, "ConnectedRemoteEndpoint") == 0)
    {	
		CosaDml_GreTunnelGetConnectedRemoteEndpoint(ins,pGreTu);
	   	snprintf(pValue, *pUlSize, "%s", pGreTu->ConnectedRemoteEndpoint);
        return 0;
    }
    if (strcmp(ParamName, "KeyIdentifier") == 0)
    {
        snprintf(pValue, *pUlSize, "%s", pGreTu->KeyIdentifier);
        return 0;
    }
    if (strcmp(ParamName, "GRENetworkTunnel") == 0)
    {
        snprintf(pValue, *pUlSize, "%s", pGreTu->GRENetworkTunnel);
        return 0;
    }
    if (strcmp(ParamName, "TunnelStatus") == 0)
    {
        snprintf(pValue, *pUlSize, "%s", pGreTu->TunnelStatus);
        return 0;
    }

    return -1;
}

ULONG GreTunnelIf_GetParamStringValue ( ANSC_HANDLE  hInsContext, char* ParamName, char* pValue, ULONG* pUlSize ) {
	COSA_DML_GRE_TUNNEL_IF                 *pGreTuIf      = (COSA_DML_GRE_TUNNEL_IF *)hInsContext;
    //ULONG                           ins = pGreTuIf->InstanceNumber;

    if (strcmp(ParamName, "LocalInterfaces") == 0)
    {
        snprintf(pValue, *pUlSize, "%s", pGreTuIf->LocalInterfaces);
        return 0;
    }
	if (strcmp(ParamName, "AssociatedBridges") == 0)
    {
        snprintf(pValue, *pUlSize, "%s", pGreTuIf->AssociatedBridges);
        return 0;
    }
    if (strcmp(ParamName, "AssociatedBridgesWiFiPort") == 0)
    {
        snprintf(pValue, *pUlSize, "%s", pGreTuIf->AssociatedBridgesWiFiPort);
        return 0;
    }

	return -1;
}

BOOL GreTunnel_GetParamIntValue ( ANSC_HANDLE hInsContext, char* ParamName, int*  pInt ) {
    COSA_DML_GRE_TUNNEL                 *pGreTu      = (COSA_DML_GRE_TUNNEL *)hInsContext;
    //ULONG                           ins = pGreTu->InstanceNumber;

    if (strcmp(ParamName, "DSCPMarkPolicy") == 0)
    {
        *pInt = pGreTu->DSCPMarkPolicy;
        return TRUE;
    }   

    return FALSE;
}


BOOL GreTunnelIf_GetParamIntValue ( ANSC_HANDLE hInsContext, char* ParamName, int*  pInt ) {
	COSA_DML_GRE_TUNNEL_IF                 *pGreTuIf      = (COSA_DML_GRE_TUNNEL_IF *)hInsContext;
    //ULONG                           ins = pGreTuIf->InstanceNumber;

    if (strcmp(ParamName, "VLANID") == 0)
    {
        *pInt = pGreTuIf->VLANID;
        return TRUE;
    } 

    return FALSE;
}

BOOL GreTunnel_SetParamBoolValue ( ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue) {
	COSA_DML_GRE_TUNNEL                 *pGreTu      = (COSA_DML_GRE_TUNNEL *)hInsContext;

    if (strcmp(ParamName, "Enable") == 0)
    {
        pGreTu->Enable = bValue;
        pGreTu->ChangeFlag |= GRETU_CF_ENABLE;
        return TRUE;
    }
	if (strcmp(ParamName, "HotSpotReset") == 0)
	{
		if(bValue)
		{
			CosaDml_GreTunnelHotspotReset(pGreTu);
			pGreTu->HotSpotReset = FALSE;
			pGreTu->ChangeFlag = (GRETU_CF_DSCP|GRETU_CF_PRIEP|GRETU_CF_SECEP|GRETU_CF_KEEPCNT|GRETU_CF_KEEPITVL|GRETU_CF_KEEPTHRE|GRETU_CF_KEEPFAILITVL|GRETU_CF_RECONNPRIM|GRETU_CF_DHCPCIRID|GRETU_CF_DHCPRMID);
		}
		return TRUE;
	}
    if (strcmp(ParamName, "UseSequenceNumber") == 0)
    {
        pGreTu->UseSequenceNumber = bValue;
        pGreTu->ChangeFlag |= GRETU_CF_USESEQ;
        return TRUE;
    }
    if (strcmp(ParamName, "UseChecksum") == 0)
    {
        pGreTu->UseChecksum = bValue;
        pGreTu->ChangeFlag |= GRETU_CF_USECSUM;
        return TRUE;
    }
    if (strcmp(ParamName, "EnableCircuitID") == 0)
    {
        pGreTu->EnableCircuitID = bValue;
        pGreTu->ChangeFlag |= GRETU_CF_DHCPCIRID;
        return TRUE;
    }
    if (strcmp(ParamName, "EnableRemoteID") == 0)
    {
        pGreTu->EnableRemoteID = bValue;
        pGreTu->ChangeFlag |= GRETU_CF_DHCPRMID;
        return TRUE;
    }

    return FALSE;
}


BOOL GreTunnelIf_SetParamBoolValue ( ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue) {
	COSA_DML_GRE_TUNNEL_IF                 *pGreTuIf      = (COSA_DML_GRE_TUNNEL_IF *)hInsContext;

    if (strcmp(ParamName, "Enable") == 0)
    {
        pGreTuIf->Enable = bValue;
        pGreTuIf->ChangeFlag |= GRETUIF_CF_ENABLE;
        return TRUE;
    }
	return FALSE;
}

BOOL GreTunnel_SetParamUlongValue ( ANSC_HANDLE  hInsContext, char* ParamName, ULONG  uValue) {
	COSA_DML_GRE_TUNNEL                 *pGreTu      = (COSA_DML_GRE_TUNNEL *)hInsContext;

    if (strcmp(ParamName, "KeyIdentifierGenerationPolicy") == 0)
    {
        pGreTu->KeyIdentifierGenerationPolicy = uValue;
        pGreTu->ChangeFlag |= GRETU_CF_KEYGENPOL;
        return TRUE;
    }
    if (strcmp(ParamName, "KeepAlivePolicy") == 0)
    {
        pGreTu->KeepAlivePolicy = uValue;
        pGreTu->ChangeFlag |= GRETU_CF_KEEPPOL;
        return TRUE;
    }
    if (strcmp(ParamName, "RemoteEndpointHealthCheckPingInterval") == 0)
    {
        pGreTu->RemoteEndpointHealthCheckPingInterval = uValue;
        pGreTu->ChangeFlag |= GRETU_CF_KEEPITVL;
        return TRUE;
    }
    if (strcmp(ParamName, "RemoteEndpointHealthCheckPingFailThreshold") == 0)
    {
        pGreTu->RemoteEndpointHealthCheckPingFailThreshold = uValue;
        pGreTu->ChangeFlag |= GRETU_CF_KEEPTHRE;
        return TRUE;
    }
    if (strcmp(ParamName, "RemoteEndpointHealthCheckPingCount") == 0)
    {
        pGreTu->RemoteEndpointHealthCheckPingCount = uValue;
        pGreTu->ChangeFlag |= GRETU_CF_KEEPCNT;
        return TRUE;
    }
    if (strcmp(ParamName, "RemoteEndpointHealthCheckPingIntervalInFailure") == 0)
    {
        pGreTu->RemoteEndpointHealthCheckPingIntervalInFailure = uValue;
        pGreTu->ChangeFlag |= GRETU_CF_KEEPFAILITVL;
        return TRUE;
    }
    if (strcmp(ParamName, "ReconnectToPrimaryRemoteEndpoint") == 0)
    {
        pGreTu->ReconnectToPrimaryRemoteEndpoint = uValue;
        pGreTu->ChangeFlag |= GRETU_CF_RECONNPRIM;
        return TRUE;
    }

    return FALSE;
}

BOOL GreTunnelIf_SetParamUlongValue ( ANSC_HANDLE  hInsContext, char* ParamName, ULONG  uValuepUlong) {
	COSA_DML_GRE_TUNNEL_IF                 *pGreTuIf      = (COSA_DML_GRE_TUNNEL_IF *)hInsContext;
    if (strcmp(ParamName, "VLANID") == 0)
    {
        pGreTuIf->VLANID = uValuepUlong;
        pGreTuIf->ChangeFlag |= GRETUIF_CF_VLANID;
        return TRUE;
    }
	return FALSE;
}

BOOL GreTunnel_SetParamStringValue ( ANSC_HANDLE hInsContext, char*  ParamName, char*  strValue ) {
	COSA_DML_GRE_TUNNEL                 *pGreTu      = (COSA_DML_GRE_TUNNEL *)hInsContext;
    if (strcmp(ParamName, "PrimaryRemoteEndpoint") == 0)
    {
        snprintf(pGreTu->PrimaryRemoteEndpoint, sizeof(pGreTu->PrimaryRemoteEndpoint), "%s", strValue);
        pGreTu->ChangeFlag |= GRETU_CF_PRIEP;
        return TRUE;
    }
	if (strcmp(ParamName, "SecondaryRemoteEndpoint") == 0)
    {
        snprintf(pGreTu->SecondaryRemoteEndpoint, sizeof(pGreTu->SecondaryRemoteEndpoint), "%s", strValue);
        pGreTu->ChangeFlag |= GRETU_CF_SECEP;
        return TRUE;
    }
    if (strcmp(ParamName, "KeyIdentifier") == 0)
    {
        snprintf(pGreTu->KeyIdentifier, sizeof(pGreTu->KeyIdentifier), "%s", strValue);
        pGreTu->ChangeFlag |= GRETU_CF_KEYID;
        return TRUE;
    }    
    if (strcmp(ParamName, "GRENetworkTunnel") == 0)
    {
        snprintf(pGreTu->GRENetworkTunnel, sizeof(pGreTu->GRENetworkTunnel), "%s", strValue);
        pGreTu->ChangeFlag |= GRETU_CF_GRETU;
        return TRUE;
    }
    if (strcmp(ParamName, "TunnelStatus") == 0)
    {
        snprintf(pGreTu->TunnelStatus, sizeof(pGreTu->TunnelStatus), "%s", strValue);
        return TRUE;
    }
    return FALSE;
}

BOOL GreTunnelIf_SetParamStringValue ( ANSC_HANDLE hInsContext, char*  ParamName, char*  strValue ) {
	COSA_DML_GRE_TUNNEL_IF                 *pGreTuIf      = (COSA_DML_GRE_TUNNEL_IF *)hInsContext;
    errno_t     rc =  -1;
    int ind = -1;

    rc = strcmp_s("LocalInterfaces", strlen("LocalInterfaces"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
       rc = STRCPY_S_NOCLOBBER(pGreTuIf->LocalInterfaces,sizeof(pGreTuIf->LocalInterfaces), strValue);
       if(rc != EOK)
       {
          ERR_CHK(rc);
          return FALSE;
       }
       pGreTuIf->ChangeFlag |= GRETUIF_CF_LOCALIF;
       return TRUE;
    }

    rc = strcmp_s("AssociatedBridges", strlen("AssociatedBridges"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
       rc = STRCPY_S_NOCLOBBER(pGreTuIf->AssociatedBridges,sizeof(pGreTuIf->AssociatedBridges), strValue);
       if(rc != EOK)
       {
          ERR_CHK(rc);
          return FALSE;
       }
       pGreTuIf->ChangeFlag |= GRETUIF_CF_ASSOBR;
       return TRUE;
    }

    rc = strcmp_s("AssociatedBridgesWiFiPort", strlen("AssociatedBridgesWiFiPort"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
       rc = STRCPY_S_NOCLOBBER(pGreTuIf->AssociatedBridgesWiFiPort,sizeof(pGreTuIf->AssociatedBridgesWiFiPort), strValue);
       if(rc != EOK)
       {
          ERR_CHK(rc);
          return FALSE;
       }
       pGreTuIf->ChangeFlag |= GRETUIF_CF_ASSOBRWFP;
       return TRUE;
    }
	return FALSE;
}

BOOL GreTunnel_SetParamIntValue ( ANSC_HANDLE hInsContext, char* ParamName, int value ) {
	COSA_DML_GRE_TUNNEL                 *pGreTu      = (COSA_DML_GRE_TUNNEL *)hInsContext;

    if (strcmp(ParamName, "DSCPMarkPolicy") == 0)
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

    if (strcmp(ParamName, "VLANID") == 0)
    {
        pGreTuIf->VLANID = value;
        pGreTuIf->ChangeFlag |= GRETUIF_CF_VLANID;
        return TRUE;
    }

    return FALSE;
}

BOOL GreTunnel_Validate ( ANSC_HANDLE hInsContext, char* pReturnParamName, ULONG* puLength ) 
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pReturnParamName);
    UNREFERENCED_PARAMETER(puLength);
    //All the parameters are validated in hotspot code
	return TRUE;
}	

BOOL GreTunnelIf_Validate ( ANSC_HANDLE hInsContext, char* pReturnParamName, ULONG* puLength )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pReturnParamName);
    UNREFERENCED_PARAMETER(puLength);
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
    if (CosaDml_GreTunnelGetGRETunnel(ins, pGreTu->GRENetworkTunnel, sizeof(pGreTu->GRENetworkTunnel)) != ANSC_STATUS_SUCCESS)
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

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        XfinityHealthCheck_GetParamIntValue
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
XfinityHealthCheck_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{

    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Cadence") == 0)
    {
        char value[8] = {'\0'};
        int itr, days = 0;
        if( syscfg_get(NULL, "XfinityHealthCheckCadence", value, sizeof(value)) == 0 )
        {
            for(itr=0; value[itr]!='\0'; itr++)
                days = days*10 + value[itr] - 48;
            *pInt = days;
            return true;
        }
        else
        {
            CcspTraceError(("syscfg_get failed for XfinityHealthCheckEnable\n"));
        }
    }
    return FALSE;

}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        XfinityHealthCheck_SetParamIntValue
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
XfinityHealthCheck_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    )
{
    char buf1[16]={0};
    char buf2[16]={0};
    errno_t rc1 = -1;
    errno_t rc2 = -1;
    UNREFERENCED_PARAMETER(hInsContext);

    if (strcmp(ParamName, "Cadence") == 0)
    {
        if(value<1 || value>1000)
        {
            AnscTraceWarning(("Cadence must be between 1 and 1000\n"));
            return FALSE;
        }
        rc1 = sprintf_s(buf1, sizeof(buf1), "%d", value);
        if(rc1 < EOK)
        {
            ERR_CHK(rc1);
            return FALSE;
        }
        rc2 = sprintf_s(buf2, sizeof(buf2), "%d", value-1);
        if(rc2 < EOK)
        {
            ERR_CHK(rc2);
            return FALSE;
        }

        if (syscfg_set(NULL, "XfinityHealthCheckCadence", buf1) != 0 ||
            syscfg_set(NULL, "XfinityHealthCheckRemDays", buf2) != 0 )
        {
            AnscTraceWarning(("syscfg_set failed\n"));
        }
        else
        {
            if (syscfg_commit() != 0)
            {
                AnscTraceWarning(("syscfg_commit failed\n"));
                return FALSE;
            }
            else
            {
                AnscTraceWarning(("syscfg_commit success\n"));
            }
        }

        return TRUE;
    }

    return FALSE;
}

#endif
