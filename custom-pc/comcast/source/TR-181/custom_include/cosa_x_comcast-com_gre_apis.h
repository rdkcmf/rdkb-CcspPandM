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
   Copyright [2014] [Comcast Corp.]
 
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

    module: cosa_x_comcast-com_gre_api.h

        For GRE API Development

    -------------------------------------------------------------------

    copyright:

        Comcast Corp.
        All Rights Reserved.

    -------------------------------------------------------------------

    description:

        This file defines the apis to support Data Model Library.

    -------------------------------------------------------------------

    author:

        zhicheng_qiu@cable.comcast.com

    -------------------------------------------------------------------

    revision:

        05/18/2015    initial revision.

**************************************************************************/

#ifdef CONFIG_CISCO_HOTSPOT
#ifndef _COSA_X_COMCAST__COM_GRE_APIS_H
#define _COSA_X_COMCAST__COM_GRE_APIS_H

#define MAX_GRE_TU              1
#define MAX_GRE_TUIF              2

typedef enum
_COSA_DML_GRE_STATUS 
{
    COSA_DML_GRE_STATUS_UP      = 1,
    COSA_DML_GRE_STATUS_DOWN    = 2,
    COSA_DML_GRE_STATUS_ERROR   = 3,
}
COSA_DML_GRE_STATUS;

typedef enum
_COSA_DML_KEY_ID_GEN_POLICY
{
    COSA_DML_KEY_ID_GEN_POLICY_DISABLED     = 1,
    COSA_DML_KEY_ID_GEN_POLICY_PROVISIONED  = 2,
    COSA_DML_KEY_ID_GEN_POLICY_AUTO         = 3,
}
COSA_DML_KEY_ID_GEN_POLICY;

typedef enum
_COSA_DML_KEEPALIVE_POLICY
{
    COSA_DML_KEEPALIVE_POLICY_NONE          = 1,
    COSA_DML_KEEPALIVE_POLICY_ICMP          = 2,
}
COSA_DML_KEEPALIVE_POLICY;

typedef struct 
_COSA_DML_GRE_TUNNEL_STATS
{
    ULONG                       KeepAliveSent;
    ULONG                       KeepAliveReceived;
    ULONG                       DiscardChecksumReceived;
    ULONG                       DiscardSequenceNumberReceived;
}
COSA_DML_GRE_TUNNEL_STATS;

typedef enum
_COSA_DML_GRE_TUNNEL_CHANGE_FLAG
{
    GRETU_CF_ENABLE         = 0x01<<0,
    GRETU_CF_PRIEP          = 0x01<<1,
	GRETU_CF_SECEP          = 0x01<<2,
    GRETU_CF_KEYGENPOL      = 0x01<<3,
    GRETU_CF_KEYID          = 0x01<<4,
    GRETU_CF_USESEQ         = 0x01<<5,
    GRETU_CF_USECSUM        = 0x01<<6,
    GRETU_CF_DSCP           = 0x01<<7,
    GRETU_CF_KEEPPOL        = 0x01<<9,
    GRETU_CF_KEEPITVL       = 0x01<<10,
    GRETU_CF_KEEPTHRE       = 0x01<<11,
    GRETU_CF_KEEPCNT        = 0x01<<12,
    GRETU_CF_KEEPFAILITVL   = 0x01<<13,
    GRETU_CF_RECONNPRIM     = 0x01<<14,
    GRETU_CF_DHCPCIRID      = 0x01<<15,
    GRETU_CF_DHCPRMID       = 0x01<<16,
    GRETU_CF_GRETU          = 0x01<<19,
}
COSA_DML_GRE_TUNNEL_CHANGE_FLAG;

typedef enum
_COSA_DML_GRE_TUNNEL_IF_CHANGE_FLAG
{
    GRETUIF_CF_ENABLE         = 0x01<<0,
    GRETUIF_CF_LOCALIF        = 0x01<<1,
    GRETUIF_CF_VLANID         = 0x01<<2,
    GRETUIF_CF_ASSOBR         = 0x01<<3,
    GRETUIF_CF_ASSOBRWFP      = 0x01<<4,
}
COSA_DML_GRE_TUNNEL_IF_CHANGE_FLAG;

typedef struct 
_COSA_DML_GRE_TUNNEL_IF
{
    ULONG                       InstanceNumber;
    COSA_DML_GRE_TUNNEL_IF_CHANGE_FLAG ChangeFlag;

    BOOL                        Enable;
    COSA_DML_GRE_STATUS         Status;
    ULONG                       LastChange;
    char                        LocalInterfaces[1029];
    INT                         VLANID;

    char                        AssociatedBridges[256];
    char                        AssociatedBridgesWiFiPort[256];
} 
COSA_DML_GRE_TUNNEL_IF;

typedef struct 
_COSA_DML_GRE_TUNNEL
{
    ULONG                       InstanceNumber;
    COSA_DML_GRE_TUNNEL_CHANGE_FLAG ChangeFlag;

    BOOL                        Enable;
    COSA_DML_GRE_STATUS         Status;
    ULONG                       LastChange;
	
    char                        PrimaryRemoteEndpoint[64];
    char                        SecondaryRemoteEndpoint[64];
	char                        ConnectedRemoteEndpoint[64];
	
    COSA_DML_KEY_ID_GEN_POLICY  KeyIdentifierGenerationPolicy;
    char                        KeyIdentifier[65];
    BOOL                        UseSequenceNumber;
    BOOL                        UseChecksum;
    
	INT                         DSCPMarkPolicy;
	
    COSA_DML_KEEPALIVE_POLICY   KeepAlivePolicy;
	ULONG                       RemoteEndpointHealthCheckPingInterval;
	ULONG                       RemoteEndpointHealthCheckPingFailThreshold;
	ULONG                       RemoteEndpointHealthCheckPingCount; 
 	ULONG                       RemoteEndpointHealthCheckPingIntervalInFailure;
 	ULONG                       ReconnectToPrimaryRemoteEndpoint;	
 	BOOL                        EnableCircuitID;
 	BOOL                        EnableRemoteID;
	char                        GRENetworkTunnel[256];	//GRENetworkInterface[256];

	COSA_DML_GRE_TUNNEL_STATS       TuStats;
	
	ULONG                       InterfaceNumberOfEntries;
	COSA_DML_GRE_TUNNEL_IF 		GreTunnelIf[MAX_GRE_TUIF];
} 
COSA_DML_GRE_TUNNEL;

ANSC_STATUS 
CosaDml_GreTunnelFinalize(void);	//
ANSC_STATUS 
CosaDml_GreTunnelIfFinalize(void); //

ULONG 
CosaDml_GreTunnelGetNumberOfEntries(void);	//

ULONG 
CosaDml_GreTunnelIfGetNumberOfEntries(ULONG idx);	//

ANSC_STATUS
CosaDml_GreTunnelGetConnectedRemoteEndpoint(ULONG idx, COSA_DML_GRE_TUNNEL *greTu); //

ANSC_STATUS
CosaDml_GreTunnelGetEntryByIndex(ULONG idx, COSA_DML_GRE_TUNNEL *greIf);	//
ANSC_STATUS
CosaDml_GreTunnelIfGetEntryByIndex(ULONG tuIdx, ULONG idx, COSA_DML_GRE_TUNNEL_IF *greIf);	//

ANSC_STATUS
CosaDml_GreTunnelSetIns(ULONG idx, ULONG ins);	//
ANSC_STATUS
CosaDml_GreTunnelIfSetIns(ULONG tuIdx,  ULONG idx, ULONG ins);	//

ANSC_STATUS
CosaDml_GreTunnelGetEnable(ULONG ins, BOOL *enable);	//
ANSC_STATUS
CosaDml_GreTunnelIfGetEnable(ULONG tuIdx, ULONG ins, BOOL *enable);	//

ANSC_STATUS
CosaDml_GreTunnelSetEnable(ULONG ins, BOOL enable);	//
ANSC_STATUS
CosaDml_GreTunnelIfSetEnable(ULONG tuIns, ULONG ins, BOOL enable);	//

ANSC_STATUS
CosaDml_GreTunnelGetStatus(ULONG tuIns, COSA_DML_GRE_STATUS *st);  //
ANSC_STATUS
CosaDml_GreTunnelIfGetStatus(ULONG tuIns, ULONG ins, COSA_DML_GRE_STATUS *st);  //

ANSC_STATUS
CosaDml_GreTunnelGetLastchange(ULONG tuIns, ULONG *time); //
ANSC_STATUS
CosaDml_GreTunnelIfGetLastchange(ULONG tuIns, ULONG ins, ULONG *time); //

ANSC_STATUS
CosaDml_GreTunnelIfGetLocalInterfaces(ULONG tuIns, ULONG ins, char *ssids, ULONG size); //

ANSC_STATUS
CosaDml_GreTunnelIfSetLocalInterfaces(ULONG tuIns, ULONG ins, const char *ssids); //

ANSC_STATUS
CosaDml_GreTunnelGetPrimaryEndpoint(ULONG tuIns, char *eps, ULONG size);	//
ANSC_STATUS
CosaDml_GreTunnelGetSecondaryEndpoint(ULONG tuIns, char *eps, ULONG size);	//

ANSC_STATUS
CosaDml_GreTunnelSetPrimaryEndpoints(ULONG tuIns, const char *eps);	//
ANSC_STATUS
CosaDml_GreTunnelSetSecondaryEndpoints(ULONG tuIns, const char *eps);	//

ANSC_STATUS
CosaDml_GreTunnelGetConnEndpoint(ULONG tuIns, char *ep, ULONG size); //

ANSC_STATUS
CosaDml_GreTunnelGetKeyGenPolicy(ULONG ins, COSA_DML_KEY_ID_GEN_POLICY *policy); //

ANSC_STATUS
CosaDml_GreTunnelSetKeyGenPolicy(ULONG ins, COSA_DML_KEY_ID_GEN_POLICY policy); //

ANSC_STATUS
CosaDml_GreTunnelGetKeyId(ULONG ins, char *keyId, ULONG size);	//

ANSC_STATUS
CosaDml_GreTunnelSetKeyId(ULONG ins, const char *keyId);	//

ANSC_STATUS
CosaDml_GreTunnelGetUseSeqNum(ULONG ins, BOOL *enable);	//

ANSC_STATUS
CosaDml_GreTunnelSetUseSeqNum(ULONG ins, BOOL enable); //

ANSC_STATUS
CosaDml_GreTunnelGetUseChecksum(ULONG ins, BOOL *enable); //

ANSC_STATUS
CosaDml_GreTunnelSetUseChecksum(ULONG ins, BOOL enable);	//

ANSC_STATUS
CosaDml_GreTunnelGetDSCPMarkPolicy(ULONG ins, INT *dscp); //

ANSC_STATUS
CosaDml_GreTunnelSetDSCPMarkPolicy(ULONG ins, INT dscp); //

ANSC_STATUS
CosaDml_GreTunnelIfGetVlanId(ULONG tuIns, ULONG ins, INT *vlanId); //

ANSC_STATUS
CosaDml_GreTunnelIfSetVlanId(ULONG tuIns, ULONG ins, INT vlanId); //

ANSC_STATUS
CosaDml_GreTunnelGetKeepAlivePolicy(ULONG ins, COSA_DML_KEEPALIVE_POLICY *policy); //

ANSC_STATUS
CosaDml_GreTunnelSetKeepAlivePolicy(ULONG ins, COSA_DML_KEEPALIVE_POLICY policy); //

ANSC_STATUS
CosaDml_GreTunnelGetKeepAliveInterval(ULONG ins, ULONG *val);  //

ANSC_STATUS
CosaDml_GreTunnelSetKeepAliveInterval(ULONG ins, ULONG val); //

ANSC_STATUS
CosaDml_GreTunnelGetKeepAliveThreshold(ULONG ins, ULONG *val); //

ANSC_STATUS
CosaDml_GreTunnelSetKeepAliveThreshold(ULONG ins, ULONG val); //

ANSC_STATUS
CosaDml_GreTunnelGetKeepAliveCount(ULONG ins, ULONG *val);  //

ANSC_STATUS
CosaDml_GreTunnelSetKeepAliveCount(ULONG ins, ULONG val); //

ANSC_STATUS
CosaDml_GreTunnelGetKeepAliveFailInterval(ULONG ins, ULONG *val); //

ANSC_STATUS
CosaDml_GreTunnelSetKeepAliveFailInterval(ULONG ins, ULONG val); //

ANSC_STATUS
CosaDml_GreTunnelGetReconnPrimary(ULONG ins, ULONG *time); //

ANSC_STATUS
CosaDml_GreTunnelSetReconnPrimary(ULONG ins, ULONG time); //

ANSC_STATUS
CosaDml_GreTunnelGetDhcpCircuitSsid(ULONG ins, BOOL *enable); //

ANSC_STATUS
CosaDml_GreTunnelSetDhcpCircuitSsid(ULONG ins, BOOL enable); //

ANSC_STATUS
CosaDml_GreTunnelGetDhcpRemoteId(ULONG ins, BOOL *enable); //

ANSC_STATUS
CosaDml_GreTunnelSetDhcpRemoteId(ULONG ins, BOOL enable); //

ANSC_STATUS
CosaDml_GreTunnelIfGetAssociatedBridges(ULONG tuIns, ULONG ins, char *brs, ULONG size); //

ANSC_STATUS
CosaDml_GreTunnelIfSetAssociatedBridges(ULONG tuIns, ULONG ins, const char *brs); //

ANSC_STATUS
CosaDml_GreTunnelIfGetAssociatedBridgesWiFiPort(ULONG tuIns, ULONG ins, char *brswfp, ULONG size); //

ANSC_STATUS
CosaDml_GreTunnelIfSetAssociatedBridgesWiFiPort(ULONG tuIns, ULONG ins, const char *brswfp); //

ANSC_STATUS
CosaDml_GreTunnelGetGRETunnel(ULONG ins, char *greif, ULONG size); //

ANSC_STATUS
CosaDml_GreTunnelSetGRETunnel(ULONG ins, const char *greif); //
 

ANSC_STATUS
CosaDml_GreTunnelGetStats(ULONG ins, COSA_DML_GRE_TUNNEL_STATS *stats); //

int GreTunnel_hotspot_update_circuit_ids(ULONG tuIns, int queuestart);//
int GreTunnelIf_hotspot_update_circuit_id(ULONG tuIns, int ins, int queuestart); //

#endif
#endif
