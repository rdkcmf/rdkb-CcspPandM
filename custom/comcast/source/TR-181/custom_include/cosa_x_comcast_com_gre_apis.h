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

#ifdef CONFIG_CISCO_HOTSPOT
#ifndef _COSA_X_COMCAST_COM_GRE_APIS_H
#define _COSA_X_COMCAST_COM_GRE_APIS_H

#define MAX_GRE_IF              1

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
_COSA_DML_GRE_IF_STATS
{
    ULONG                       KeepAliveSent;
    ULONG                       KeepAliveReceived;
    ULONG                       DiscardChecksumReceived;
    ULONG                       DiscardSequenceNumberReceived;
}
COSA_DML_GRE_IF_STATS;

typedef enum
_COSA_DML_GRE_IF_CHANGE_FLAG
{
    GREIF_CF_ENABLE         = 0x01<<0,
    GREIF_CF_LOCALIF        = 0x01<<1,
    GREIF_CF_RMEP           = 0x01<<2,
    GREIF_CF_KEYGENPOL      = 0x01<<3,
    GREIF_CF_KEYID          = 0x01<<4,
    GREIF_CF_USESEQ         = 0x01<<5,
    GREIF_CF_USECSUM        = 0x01<<6,
    GREIF_CF_DSCP           = 0x01<<7,
    GREIF_CF_VLANID         = 0x01<<8,
    GREIF_CF_KEEPPOL        = 0x01<<9,
    GREIF_CF_KEEPITVL       = 0x01<<10,
    GREIF_CF_KEEPTHRE       = 0x01<<11,
    GREIF_CF_KEEPCNT        = 0x01<<12,
    GREIF_CF_KEEPFAILITVL   = 0x01<<13,
    GREIF_CF_RECONNPRIM     = 0x01<<14,
    GREIF_CF_DHCPCIRID      = 0x01<<15,
    GREIF_CF_DHCPRMID       = 0x01<<16,
    GREIF_CF_ASSOBR         = 0x01<<17,
    GREIF_CF_ASSOBRWFP      = 0x01<<18,
    GREIF_CF_GREIF          = 0x01<<19,
}
COSA_DML_GRE_IF_CHANGE_FLAG;

typedef struct 
_COSA_DML_GRE_IF
{
    ULONG                       InstanceNumber;
    COSA_DML_GRE_IF_CHANGE_FLAG ChangeFlag;

    BOOL                        Enable;
    COSA_DML_GRE_STATUS         Status;
    ULONG                       LastChange;
    char                        LocalInterfaces[256]; /*we can restrict to 256 bytes, because internally coping LocalInterfaces data to LowerLayers buffer which is 256 bytes buffer. So buffer mismatch happening*/
    char                        RemoteEndpoints[1029];
    char                        ConnectedRemoteEndpoint[257];
    COSA_DML_KEY_ID_GEN_POLICY  KeyIdentifierGenerationPolicy;
    char                        KeyIdentifier[65];
    BOOL                        UseSequenceNumber;
    BOOL                        UseChecksum;
    INT                         DSCPMarkPolicy;
    INT                         VLANID;
    COSA_DML_KEEPALIVE_POLICY   KeepAlivePolicy;
    ULONG                       KeepAliveInterval;
    ULONG                       KeepAliveThreshold;

    ULONG                       KeepAliveCount;
    ULONG                       KeepAliveFailInterval;
    ULONG                       ReconnectPrimary;
    BOOL                        DHCPCircuitIDSSID;
    BOOL                        DHCPRemoteID;

    char                        AssociatedBridges[256];
    char                        AssociatedBridgesWiFiPort[256];
    char                        GRENetworkInterface[256];

    COSA_DML_GRE_IF_STATS       IfStats;
} 
COSA_DML_GRE_IF;

ANSC_STATUS 
CosaDml_GreInit(void);

ANSC_STATUS 
CosaDml_GreFinalize(void);

ANSC_STATUS
CosaDml_GreIfGetConnectedRemoteEndpoint(ULONG idx, COSA_DML_GRE_IF *greIf);

ULONG 
CosaDml_GreIfGetNumberOfEntries(void);

ANSC_STATUS
CosaDml_GreIfGetEntryByIndex(ULONG idx, COSA_DML_GRE_IF *greIf);

ANSC_STATUS
CosaDml_GreIfSetIns(ULONG idx, ULONG ins);

ANSC_STATUS
CosaDml_GreIfGetEnable(ULONG ins, BOOL *enable);

ANSC_STATUS
CosaDml_GreIfSetEnable(ULONG ins, BOOL enable);

ANSC_STATUS
CosaDml_GreIfGetStatus(ULONG ins, COSA_DML_GRE_STATUS *st);

ANSC_STATUS
CosaDml_GreIfGetLastchange(ULONG ins, ULONG *time);

ANSC_STATUS
CosaDml_GreIfGetLocalInterfaces(ULONG ins, char *ssids, ULONG size);

ANSC_STATUS
CosaDml_GreIfSetLocalInterfaces(ULONG ins, const char *ssids);

ANSC_STATUS
CosaDml_GreIfGetEndpoints(ULONG ins, char *eps, ULONG size);

ANSC_STATUS
CosaDml_GreIfSetEndpoints(ULONG ins, const char *eps);

ANSC_STATUS
CosaDml_GreIfGetConnEndpoint(ULONG ins, char *ep, ULONG size);

ANSC_STATUS
CosaDml_GreIfGetKeyGenPolicy(ULONG ins, COSA_DML_KEY_ID_GEN_POLICY *policy);

ANSC_STATUS
CosaDml_GreIfSetKeyGenPolicy(ULONG ins, COSA_DML_KEY_ID_GEN_POLICY policy);

ANSC_STATUS
CosaDml_GreIfGetKeyId(ULONG ins, char *keyId, ULONG size);

ANSC_STATUS
CosaDml_GreIfSetKeyId(ULONG ins, const char *keyId);

ANSC_STATUS
CosaDml_GreIfGetUseSeqNum(ULONG ins, BOOL *enable);

ANSC_STATUS
CosaDml_GreIfSetUseSeqNum(ULONG ins, BOOL enable);

ANSC_STATUS
CosaDml_GreIfGetUseChecksum(ULONG ins, BOOL *enable);

ANSC_STATUS
CosaDml_GreIfSetUseChecksum(ULONG ins, BOOL enable);

ANSC_STATUS
CosaDml_GreIfGetDSCPMarkPolicy(ULONG ins, INT *dscp);

ANSC_STATUS
CosaDml_GreIfSetDSCPMarkPolicy(ULONG ins, INT dscp);

ANSC_STATUS
CosaDml_GreIfGetVlanId(ULONG ins, INT *vlanId);

ANSC_STATUS
CosaDml_GreIfSetVlanId(ULONG ins, INT vlanId);

ANSC_STATUS
CosaDml_GreIfGetKeepAlivePolicy(ULONG ins, COSA_DML_KEEPALIVE_POLICY *policy);

ANSC_STATUS
CosaDml_GreIfSetKeepAlivePolicy(ULONG ins, COSA_DML_KEEPALIVE_POLICY policy);

ANSC_STATUS
CosaDml_GreIfGetKeepAliveInterval(ULONG ins, ULONG *val);

ANSC_STATUS
CosaDml_GreIfSetKeepAliveInterval(ULONG ins, ULONG val);

ANSC_STATUS
CosaDml_GreIfGetKeepAliveThreshold(ULONG ins, ULONG *val);

ANSC_STATUS
CosaDml_GreIfSetKeepAliveThreshold(ULONG ins, ULONG val);

ANSC_STATUS
CosaDml_GreIfGetKeepAliveCount(ULONG ins, ULONG *val);

ANSC_STATUS
CosaDml_GreIfSetKeepAliveCount(ULONG ins, ULONG val);

ANSC_STATUS
CosaDml_GreIfGetKeepAliveFailInterval(ULONG ins, ULONG *val);

ANSC_STATUS
CosaDml_GreIfSetKeepAliveFailInterval(ULONG ins, ULONG val);

ANSC_STATUS
CosaDml_GreIfGetReconnPrimary(ULONG ins, ULONG *time);

ANSC_STATUS
CosaDml_GreIfSetReconnPrimary(ULONG ins, ULONG time);

ANSC_STATUS
CosaDml_GreIfGetDhcpCircuitSsid(ULONG ins, BOOL *enable);

ANSC_STATUS
CosaDml_GreIfSetDhcpCircuitSsid(ULONG ins, BOOL enable);

ANSC_STATUS
CosaDml_GreIfGetDhcpRemoteId(ULONG ins, BOOL *enable);

ANSC_STATUS
CosaDml_GreIfSetDhcpRemoteId(ULONG ins, BOOL enable);

ANSC_STATUS
CosaDml_GreIfGetAssociatedBridges(ULONG ins, char *brs, ULONG size);

ANSC_STATUS
CosaDml_GreIfSetAssociatedBridges(ULONG ins, const char *brs);

ANSC_STATUS
CosaDml_GreIfGetAssociatedBridgesWiFiPort(ULONG ins, char *brswfp, ULONG size);

ANSC_STATUS
CosaDml_GreIfSetAssociatedBridgesWiFiPort(ULONG ins, const char *brswfp);

ANSC_STATUS
CosaDml_GreIfGetGREInterface(ULONG ins, char *greif, ULONG size);

ANSC_STATUS
CosaDml_GreIfSetGREInterface(ULONG ins, const char *greif);
 
ANSC_STATUS
CosaDml_GreIfGetStats(ULONG ins, COSA_DML_GRE_IF_STATS *stats);

int hotspot_update_circuit_ids(int greinst, int queuestart);

#endif
#endif
