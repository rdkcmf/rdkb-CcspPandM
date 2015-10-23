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

    module: cosa_moca_apis.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    copyright:

        Cisco Systems, Inc.
        All Rights Reserved.

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**************************************************************************/


#ifndef  _COSA_MOCA_API_H
#define  _COSA_MOCA_API_H

#ifndef SA_CUSTOM

#include "../middle_layer_src/cosa_apis.h"
#include "../middle_layer_src/plugin_main_apis.h"

#else

#define _struct_pack_   __attribute__((packed))
#define  COSA_DML_ALIAS_NAME_LENGTH     64
#define ANSC_STATUS                     ULONG
#define ANSC_HANDLE                     PVOID
#define PANSC_HANDLE                    PVOID *

typedef  enum
COSA_DML_IF_STATUS
{
    IF_STATUS_Up               = 1,
    IF_STATUS_Down,
    IF_STATUS_Unknown,
    IF_STATUS_Dormant,
    IF_STATUS_NotPresent,
    IF_STATUS_LowerLayerDown,
    IF_STATUS_Error
}
COSA_DML_IF_STATUS, *PCOSA_DML_IF_STATUS;

typedef  struct
_SINGLE_LINK_ENTRY
{
    struct  _SINGLE_LINK_ENTRY*     Next;
}
SINGLE_LINK_ENTRY,  *PSINGLE_LINK_ENTRY;

typedef  struct
_SLIST_HEADER
{
    SINGLE_LINK_ENTRY               Next;
    USHORT                          Depth;
    USHORT                          Sequence;
}
SLIST_HEADER,  *PSLIST_HEADER;
#endif

/**********************************************************************
                STRUCTURE AND CONSTANT DEFINITIONS
**********************************************************************/
#define COSA_DML_MOCA_AssocDeviceSafeguard          8
#define MOCA_INTEFACE_NUMBER        1

typedef 
enum _MOCA_PROVISIONING_SERVERADDRESS_TYPE
{
    MOCA_PROVISIONING_SERVADDR_TYPE_IPV4=1,
    MOCA_PROVISIONING_SERVADDR_TYPE_IPV6
} MOCA_PROVISIONING_SERVERADDRESS_TYPE;

struct _COSA_DML_MOCA_CFG
{
    UCHAR                                 X_CISCO_COM_ProvisioningFilename[128];
    UCHAR                                 X_CISCO_COM_ProvisioningServerAddress[64];
    MOCA_PROVISIONING_SERVERADDRESS_TYPE  X_CISCO_COM_ProvisioningServerAddressType;
}_struct_pack_;

typedef struct _COSA_DML_MOCA_CFG COSA_DML_MOCA_CFG,  *PCOSA_DML_MOCA_CFG;



struct _COSA_DML_MOCA_IF_CFG
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_ALIAS_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    BOOLEAN                         bPreferredNC;
    BOOLEAN                         PrivacyEnabledSetting;
    UCHAR                           FreqCurrentMaskSetting[128];
    char                            KeyPassphrase[18];
    INT                             TxPowerLimit;
    ULONG                           PowerCntlPhyTarget;
    ULONG                           BeaconPowerLimit;
    
    /*
     * Extensions
     */
    ULONG                           MaxIngressBWThreshold;
    ULONG                           MaxEgressBWThreshold;

    /* usgv2 new extensions  */
    BOOLEAN                         X_CISCO_COM_Reset;
    BOOLEAN                         X_CISCO_COM_MixedMode;
    BOOLEAN                         X_CISCO_COM_ChannelScanning;
    BOOLEAN                         X_CISCO_COM_AutoPowerControlEnable;
    BOOLEAN                         X_CISCO_COM_EnableTabooBit;
    UCHAR                           NodeTabooMask[128];
    ULONG                           AutoPowerControlPhyRate;
    UCHAR                           X_CISCO_COM_ChannelScanMask[128];
}_struct_pack_;

typedef struct _COSA_DML_MOCA_IF_CFG COSA_DML_MOCA_IF_CFG,  *PCOSA_DML_MOCA_IF_CFG;

/*
 *  Static portion of MoCA interface info
 */
struct _COSA_DML_MOCA_IF_SINFO
{
    char                            Name[64];           /* Uniquely identifying the entry */
    BOOLEAN                         bUpstream;
    UCHAR                           MacAddress[18];
    char                            FirmwareVersion[64];
    ULONG                           MaxBitRate;
    char                            HighestVersion[64];
    UCHAR                           FreqCapabilityMask[8];
    UCHAR                           NetworkTabooMask[128];
    ULONG                           TxBcastPowerReduction;
    BOOLEAN                         QAM256Capable;
    ULONG                           PacketAggregationCapability;

    /* usgv2 new extensions */
    BOOLEAN                         X_CISCO_COM_CycleMaster;
}_struct_pack_;

typedef struct _COSA_DML_MOCA_IF_SINFO COSA_DML_MOCA_IF_SINFO,  *PCOSA_DML_MOCA_IF_SINFO;

/*
 *  Dynamic portion of MoCA interface info
 */
struct _COSA_DML_MOCA_IF_DINFO
{
    COSA_DML_IF_STATUS              Status;
    ULONG                           LastChange;
    ULONG                           MaxIngressBW;
    ULONG                           MaxEgressBW;
    char                            CurrentVersion[64];
    ULONG                           NetworkCoordinator;
    ULONG                           NodeID;
    BOOLEAN                         MaxNodes;
    ULONG                           BackupNC;
    BOOLEAN                         PrivacyEnabled;
    UCHAR                           FreqCurrentMask[8];
    ULONG                           CurrentOperFreq;
    ULONG                           LastOperFreq;
    ULONG                           TxBcastRate;
    /*
     * Extensions
     */
    BOOLEAN                         MaxIngressBWThresholdReached;
    BOOLEAN                         MaxEgressBWThresholdReached;
    ULONG                           X_CISCO_COM_NumberOfConnectedClients;

    char                            X_CISCO_NetworkCoordinatorMACAddress[18];
    ULONG                           X_CISCO_COM_BestNetworkCoordinatorID;
}_struct_pack_;

typedef struct _COSA_DML_MOCA_IF_DINFO COSA_DML_MOCA_IF_DINFO,  *PCOSA_DML_MOCA_IF_DINFO;

struct _COSA_DML_MOCA_IF_FULL
{
    COSA_DML_MOCA_IF_CFG            Cfg;
    COSA_DML_MOCA_IF_SINFO          StaticInfo;
    COSA_DML_MOCA_IF_DINFO          DynamicInfo;
}_struct_pack_;

typedef struct _COSA_DML_MOCA_IF_FULL  COSA_DML_MOCA_IF_FULL, *PCOSA_DML_MOCA_IF_FULL;

struct _COSA_DML_MOCA_STATS
{
    ULONG                           BytesSent;
    ULONG                           BytesReceived;
    ULONG                           PacketsSent;
    ULONG                           PacketsReceived;
    ULONG                           ErrorsSent;
    ULONG                           ErrorsReceived;
    ULONG                           UnicastPacketsSent;
    ULONG                           UnicastPacketsReceived;
    ULONG                           DiscardPacketsSent;
    ULONG                           DiscardPacketsReceived;
    ULONG                           MulticastPacketsSent;
    ULONG                           MulticastPacketsReceived;
    ULONG                           BroadcastPacketsSent;
    ULONG                           BroadcastPacketsReceived;
    ULONG                           UnknownProtoPacketsReceived;

    /* usgv2 new extensions */
    ULONG                           X_CISCO_COM_ExtAggrAverageTx;
    ULONG                           X_CISCO_COM_ExtAggrAverageRx;
}_struct_pack_;

typedef struct _COSA_DML_MOCA_STATS COSA_DML_MOCA_STATS, *PCOSA_DML_MOCA_STATS;

struct _COSA_DML_MOCA_QOS
{
    BOOLEAN                         Enabled;
}_struct_pack_;

typedef struct _COSA_DML_MOCA_QOS COSA_DML_MOCA_QOS,  *PCOSA_DML_MOCA_QOS;

struct _COSA_DML_MOCA_EXTCOUNTER
{
    ULONG                           Type;
    ULONG                           Map;
    ULONG                           Rsrv;
    ULONG                           Lc;
    ULONG                           Adm;
    ULONG                           Probe;
    ULONG                           Async;
} _struct_pack_;

typedef  struct _COSA_DML_MOCA_EXTCOUNTER COSA_DML_MOCA_EXTCOUNTER, *PCOSA_DML_MOCA_EXTCOUNTER;

struct _COSA_DML_MOCA_EXTAGGRCOUNTER
{
    ULONG                           Tx;
    ULONG                           Rx;
}_struct_pack_;

typedef struct _COSA_DML_MOCA_EXTAGGRCOUNTER COSA_DML_MOCA_EXTAGGRCOUNTER,  *PCOSA_DML_MOCA_EXTAGGRCOUNTER;

struct _COSA_DML_MOCA_PEER
{
    ULONG                           PeerSpeed;
}_struct_pack_;

typedef struct _COSA_DML_MOCA_PEER COSA_DML_MOCA_PEER,  *PCOSA_DML_MOCA_PEER;


//Device.MoCA.Interface.{i}.X_CISCO_COM_Mesh.MeshTxNodeTable.{i}.MeshRxNodeTable.{i}.XXX
struct _COSA_DML_MOCA_MeshRxNode
{
    ULONG                           RxNodeID;
    ULONG                           TxRate;
}_struct_pack_;

typedef struct _COSA_DML_MOCA_MeshRxNode COSA_DML_MOCA_MeshRxNode,  *PCOSA_DML_MOCA_MeshRxNode;

struct _COSA_DML_MOCA_MeshTxNode
{
    SLIST_HEADER                    MoCAMeshRxNodeTable;
    ULONG                           TxNodeID;
}_struct_pack_;

typedef struct _COSA_DML_MOCA_MeshTxNode COSA_DML_MOCA_MeshTxNode,  *PCOSA_DML_MOCA_MeshTxNode;

struct _COSA_DML_MOCA_MESH
{
    ULONG                           TxNodeID;
    ULONG                           RxNodeID;
    ULONG                           TxRate;
}_struct_pack_;

typedef struct _COSA_DML_MOCA_MESH COSA_DML_MOCA_MESH,  *PCOSA_DML_MOCA_MESH;


struct _COSA_DML_MOCA_FLOW
{
    ULONG                           FlowID;
    ULONG                           IngressNodeID;
    ULONG                           EgressNodeID;
    ULONG                           FlowTimeLeft;
    char                            DestinationMACAddress[18];
    ULONG                           PacketSize;
    ULONG                           PeakDataRate;
    ULONG                           BurstSize;
    ULONG                           FlowTag;
    ULONG                           LeaseTime;
}_struct_pack_;

typedef struct _COSA_DML_MOCA_FLOW COSA_DML_MOCA_FLOW, *PCOSA_DML_MOCA_FLOW;

struct _COSA_DML_MOCA_ASSOC_DEVICE
{
    UCHAR                           MACAddress[18];
    ULONG                           NodeID;
    BOOLEAN                         PreferredNC;
    char                            HighestVersion[64];
    ULONG                           PHYTxRate;
    ULONG                           PHYRxRate;
    ULONG                           TxPowerControlReduction;
    ULONG                           RxPowerLevel;
    ULONG                           TxBcastRate;
    ULONG                           RxBcastPowerLevel;
    ULONG                           TxPackets;
    ULONG                           RxPackets;
    ULONG                           RxErroredAndMissedPackets;
    BOOLEAN                         QAM256Capable;
    ULONG                           PacketAggregationCapability;
    ULONG                           RxSNR;
    BOOLEAN                         Active;

    /* usgv2 new extensions */
    ULONG                           X_CISCO_COM_RxBcastRate;
    ULONG                           X_CISCO_COM_NumberOfClients;
}_struct_pack_;

typedef struct _COSA_DML_MOCA_ASSOC_DEVICE COSA_DML_MOCA_ASSOC_DEVICE,  *PCOSA_DML_MOCA_ASSOC_DEVICE;

/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/

ANSC_STATUS
CosaDmlMocaInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
);

ULONG
CosaDmlMocaGetNumberOfIfs
    (
        ANSC_HANDLE                 hContext
    );

ANSC_STATUS
CosaDmlMocaIfGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_MOCA_IF_FULL      pEntry
    );

ANSC_STATUS
CosaDmlMocaIfSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
);

ANSC_STATUS
CosaDmlMocaIfSetCfg
    (
        ANSC_HANDLE                 hContext,
        ULONG                       uIndex,
        PCOSA_DML_MOCA_IF_CFG       pCfg
    );

ANSC_STATUS
CosaDmlMocaIfGetCfg
    (
        ANSC_HANDLE                 hContext,
        ULONG                       uIndex,
        PCOSA_DML_MOCA_IF_CFG       pCfg
    );

ANSC_STATUS
CosaDmlMocaIfGetDinfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_MOCA_IF_DINFO     pInfo
    );

ANSC_STATUS
CosaDmlMocaIfGetStats
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_MOCA_STATS        pStats
    );

ANSC_STATUS
CosaDmlMocaIfGetAssocDevices
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PULONG                      pulCount,
        PCOSA_DML_MOCA_ASSOC_DEVICE *ppDeviceArray,
        PULONG                      *ppMeshRateArray    /* Not used now */
    );

ANSC_STATUS
CosaDmlMocaSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_MOCA_CFG          pCfg
    );

ANSC_STATUS
CosaDmlMocaGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_MOCA_CFG          pCfg
    );

ANSC_STATUS
CosaDmlMocaIfResetStats
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex
    );

ANSC_STATUS
CosaDmlMocaIfSetQos
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_MOCA_QOS          pConf
    );

ANSC_STATUS
CosaDmlMocaIfGetQos
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_MOCA_QOS          pConf
    );

ULONG
CosaDmlMocaIfExtCounterGetNumber
    (
        ANSC_HANDLE                      hContext,
        ULONG                            ulInterfaceIndex
    );

ANSC_STATUS
CosaDmlMocaIfExtCounterGetEntry
    (
        ANSC_HANDLE                      hContext,
        ULONG                            ulInterfaceIndex,
        ULONG                            ulIndex,
        PCOSA_DML_MOCA_EXTCOUNTER        pConf
    );

ULONG
CosaDmlMocaIfExtAggrCounterGetNumber
    (
        ANSC_HANDLE                      hContext,
        ULONG                            ulInterfaceIndex
    );

ANSC_STATUS
CosaDmlMocaIfExtAggrCounterGetEntry
    (
        ANSC_HANDLE                      hContext,
        ULONG                            ulInterfaceIndex,
        ULONG                            ulIndex,
        PCOSA_DML_MOCA_EXTAGGRCOUNTER    pConf
    );

ANSC_STATUS
CosaDmlMocaIfPeerTableGetTable
    (
        ANSC_HANDLE                      hContext,
        ULONG                            ulInterfaceIndex,
        PCOSA_DML_MOCA_PEER             *ppConf,
        PULONG                           pCount
    );

ANSC_STATUS
CosaDmlMocaIfMeshTableGetTable
    (
        ANSC_HANDLE                      hContext,
        ULONG                            ulInterfaceIndex,
        PCOSA_DML_MOCA_MESH             *ppConf,
        PULONG                           pCount
    );

ANSC_STATUS
CosaDmlMocaIfFlowTableGetTable
    (
        ANSC_HANDLE                      hContext,
        ULONG                            ulInterfaceIndex,
        PCOSA_DML_MOCA_FLOW             *ppConf,
        PULONG                           pCount
    );





#endif
