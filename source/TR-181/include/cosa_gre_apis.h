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

#ifndef _COSA_GRE_APIS_H
#define _COSA_GRE_APIS_H

#include "ansc_platform.h"
#include "plugin_main_apis.h"
#include "hotspotfd.h"
#include "dhcpsnooper.h"
/**********************************************************************
                STRUCTURE AND CONSTANT DEFINITIONS
**********************************************************************/

#define COSA_DML_NAME_LEN 64
#define COSA_DML_CONNECTEDENDPT_LEN 256
#define COSA_DML_REMOTEENDPTLEN 1024
#define COSA_DML_LOWERLAYERLEN 1024

typedef enum
_COSA_DML_GRE_TUNNEL_STATUS
{
    COSA_DML_GRE_TUNNEL_DISABLED = 1,
    COSA_DML_GRE_TUNNEL_ENABLED,
    COSA_DML_GRE_TUNNEL_ERROR
}
COSA_DML_GRE_TUNNEL_STATUS;

typedef enum
_COSA_DML_KEY_ID_GEN_POLICY
{
    COSA_DML_KEY_ID_GEN_POLICY_DISABLED = 1,
    COSA_DML_KEY_ID_GEN_POLICY_PROVISIONED,
    COSA_DML_KEY_ID_GEN_POLICY_AUTO
}
COSA_DML_KEY_ID_GEN_POLICY;

typedef enum
_COSA_DML_KEEPALIVE_POLICY
{
    COSA_DML_KEEPALIVE_POLICY_NONE = 1,
    COSA_DML_KEEPALIVE_POLICY_ICMP
}
COSA_DML_KEEPALIVE_POLICY;

typedef enum
_COSA_DML_DELIVERY_HEADER_PROTOCOL
{
    COSA_DML_IPV4 = 1,
    COSA_DML_IPV6
}
COSA_DML_DELIVERY_HEADER_PROTOCOL;


typedef enum
_COSA_DML_GRE_TUNNEL_IF_STATUS
{
    COSA_DML_TUNNEL_IF_UP = 1,
    COSA_DML_TUNNEL_IF_DOWN,
    COSA_DML_TUNNEL_IF_UNKNOWN,
    COSA_DML_TUNNEL_IF_DORMANT,
    COSA_DML_TUNNEL_IF_NOTPRESENT,
    COSA_DML_TUNNEL_IF_LOWERLAYERDOWN,
    COSA_DML_TUNNEL_IF_ERROR
}
COSA_DML_GRE_TUNNEL_IF_STATUS;

typedef struct
_COSA_DML_GRE_TUNNEL_STATS
{
    ULONG                       KeepAliveSent;
    ULONG                       KeepAliveReceived;
    ULONG                       BytesSent;
    ULONG                       BytesReceived;
    ULONG                       PacketsSent;
    ULONG                       PacketsReceived;
    ULONG                       ErrorsSent;
    ULONG                       ErrorsReceived;
    ULONG                       MulticastPacketsSent;
    ULONG                       MulticastPacketsReceived;
    ULONG                       DiscardPacketsSent;
    ULONG                       DiscardPacketsReceived;
}
COSA_DML_GRE_TUNNEL_STATS, *PCOSA_DML_GRE_TUNNEL_STATS;

typedef struct
_COSA_DML_GRE_TUNNEL_IF_STATS
{
    ULONG                       BytesSent;
    ULONG                       BytesReceived;
    ULONG                       PacketsSent;
    ULONG                       PacketsReceived;
    ULONG                       ErrorsSent;
    ULONG                       ErrorsReceived;
    ULONG                       DiscardChecksumReceived;
    ULONG                       DiscardSequenceNumberReceived;
    ULONG                       MulticastPacketsSent;
    ULONG                       MulticastPacketsReceived;
    ULONG                       DiscardPacketsSent;
    ULONG                       DiscardPacketsReceived; 
}
COSA_DML_GRE_TUNNEL_IF_STATS, *PCOSA_DML_GRE_TUNNEL_IF_STATS;

typedef struct
_COSA_DML_GRE_TUNNEL_CFG
{
    ULONG                              TunnelInstanceNumber;
    BOOL                               Enable;
    char                               Alias[COSA_DML_NAME_LEN];
    char                               RemoteEndpoints[COSA_DML_REMOTEENDPTLEN];
    COSA_DML_KEEPALIVE_POLICY          KeepAlivePolicy;
    ULONG                              KeepAliveTimeout;
    ULONG                              KeepAliveThreshold;
    COSA_DML_DELIVERY_HEADER_PROTOCOL  DeliveryHeaderProtocol;
    ULONG                              DefaultDSCPMark;
    char                               ConnectedRemoteEndpoint[COSA_DML_CONNECTEDENDPT_LEN];
    ULONG                              InterfaceNumberOfEntries;
    COSA_DML_GRE_TUNNEL_STATS          TunnelStats;
    ULONG                              KeepAliveCount;
    ULONG                              KeepAliveInterval;
    ULONG                              KeepAliveFailureInterval;
    ULONG                              KeepAliveRecoverInterval;
    ULONG                              MSSClampingValue;
    BOOL                               SnoopCircuitEnable;
    BOOL                               SnoopRemoteEnable;
    ULONG                              MaxClients;
}
COSA_DML_GRE_TUNNEL_CFG,*PCOSA_DML_GRE_TUNNEL_CFG;

typedef  struct
_COSA_DML_GRE_TUNNEL_INFO
{
    COSA_DML_GRE_TUNNEL_STATUS         Status;
}COSA_DML_GRE_TUNNEL_INFO, *PCOSA_DML_GRE_TUNNEL_INFO;

typedef  struct
_COSA_DML_GRE_TUNNEL_FULL
{
    COSA_DML_GRE_TUNNEL_CFG                Cfg;
    COSA_DML_GRE_TUNNEL_INFO               Info;
}
COSA_DML_GRE_TUNNEL_FULL, *PCOSA_DML_GRE_TUNNEL_FULL;

typedef struct
_COSA_DML_GRE_TUNNEL_IF_CFG
{
    ULONG                               IfInstanceNumber;
    BOOL                                Enable;
    char                                Alias[COSA_DML_NAME_LEN];
    char                                LowerLayers[COSA_DML_LOWERLAYERLEN];
    ULONG                               ProtocolIdOverride;
    BOOL                                UseChecksum;
    COSA_DML_KEY_ID_GEN_POLICY          KeyIdentifierGenerationPolicy;
    ULONG                               KeyIdentifier;
    BOOL                                UseSequenceNumber;

    COSA_DML_GRE_TUNNEL_IF_STATS        IfStats;
}
COSA_DML_GRE_TUNNEL_IF_CFG;

typedef  struct
_COSA_DML_GRE_TUNNEL_IF_INFO
{
    char                            Name[COSA_DML_NAME_LEN];      /* Read-only */
    COSA_DML_GRE_TUNNEL_IF_STATUS   Status;
    ULONG                           LastChange;
}
COSA_DML_GRE_TUNNEL_IF_INFO, *PCOSA_DML_GRE_TUNNEL_IF_INFO;

typedef  struct
_COSA_DML_GRE_TUNNEL_IF_FULL
{
    COSA_DML_GRE_TUNNEL_IF_CFG           Cfg;
    COSA_DML_GRE_TUNNEL_IF_INFO          Info;
}
COSA_DML_GRE_TUNNEL_IF_FULL, *PCOSA_DML_GRE_TUNNEL_IF_FULL;

typedef  struct
_DMSB_TR181_GRE_TUNNEL_IF
{
    SINGLE_LINK_ENTRY                    Linkage;
    COSA_DML_GRE_TUNNEL_IF_CFG           Cfg;
    COSA_DML_GRE_TUNNEL_IF_INFO          Info;

    COSA_DML_GRE_TUNNEL_IF_STATS         IfStats;
}
DMSB_TR181_GRE_TUNNEL_IF, *PDMSB_TR181_GRE_TUNNEL_IF;


typedef  struct
_COSA_DML_GRE_TUNNEL_FULL_ALL
{
    COSA_DML_GRE_TUNNEL_CFG         Cfg;
    COSA_DML_GRE_TUNNEL_INFO        Info;

    SLIST_HEADER                    InterfaceList;
    ULONG                           ulNextIfInsNum;
}
COSA_DML_GRE_TUNNEL_FULL_ALL, *PCOSA_DML_GRE_TUNNEL_FULL_ALL;

typedef struct
_DMSB_TR181_GRE_TUNNEL
{
     SINGLE_LINK_ENTRY              Linkage;
     COSA_DML_GRE_TUNNEL_CFG        Cfg;
     COSA_DML_GRE_TUNNEL_INFO       Info;

     SLIST_HEADER                   InterfaceTR181List;

     COSA_DML_GRE_TUNNEL_STATS      TunnelStats;
}
DMSB_TR181_GRE_TUNNEL, *PDMSB_TR181_GRE_TUNNEL;

#define  ACCESS_DMSB_TR181_GRE_TUNNEL(p)          ACCESS_CONTAINER(p, DMSB_TR181_GRE_TUNNEL, Linkage)

typedef struct
_DMSB_TR181_GRE_CONTEXT
{
    /* GRE Tunnel*/
    BOOLEAN                         GRETunnelInit;
    SLIST_HEADER                    GRETunnelTR181List;
}
DMSB_TR181_GRE_CONTEXT, *PDMSB_TR181_GRE_CONTEXT;

#define  DMSB_TR181_GRE_CONTEXT_Init(p)                            \
            {                                                      \
                p->GRETunnelInit      = FALSE;                     \
                AnscSListInitializeHeader(&p->GRETunnelTR181List); \
            }

/**********************************************************************
                HOTSPOT SYSEVENT VARIABLES
**********************************************************************/
extern int sysevent_fd;
extern token_t sysevent_token;

/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/

ULONG CosaDmlGRETunnelGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    );

ULONG CosaDmlGRETunnelIfGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext,
        ULONG                       TunnelInstanceNumber
    );

PDMSB_TR181_GRE_TUNNEL
CosaDmlGRETunnelFindByInstNum
    (
        PDMSB_TR181_GRE_CONTEXT                 pGREContext,
        ULONG                                   ulInstNum
    );

ANSC_STATUS
CosaDmlGRETunnelLoadConf
    (
        PDMSB_TR181_GRE_CONTEXT                 pGREContext
    );

ANSC_STATUS
CosaDmlGRETunnelSaveConf
    (
        PDMSB_TR181_GRE_CONTEXT                 pGREContext,
        COSA_DML_GRE_TUNNEL_CFG                 *cfg
    );

ANSC_STATUS
CosaDmlGRETunnelIfSaveConf
    (
        ULONG                         TunnelInstanceNum,
        PCOSA_DML_GRE_TUNNEL_IF_FULL  pGRETunnelIf
    );

ANSC_STATUS
CosaDmlGRETunnelGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_GRE_TUNNEL_CFG    pCfg
    );

ANSC_STATUS
CosaDmlGRETunnelSetCfg
    (
        ANSC_HANDLE              hContext,
        COSA_DML_GRE_TUNNEL_CFG *greTunnel
    );

ANSC_STATUS
CosaDmlGRETunnelAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_GRE_TUNNEL_FULL   pEntry
    );

ANSC_STATUS
CosaDmlGRETunnelGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_GRE_TUNNEL_FULL   pEntry
    );

ANSC_STATUS
CosaDmlGRETunnelIfGetEntry
    (
        ANSC_HANDLE                      hContext,
        ULONG                            ulGRETunnelInstanceNumber,
        ULONG                            ulIndex,
        PCOSA_DML_GRE_TUNNEL_IF_FULL     pEntry
    );

ANSC_STATUS
CosaDmlGRETunnelIfAddEntry
    (
        ANSC_HANDLE                      hContext,
        ULONG                            ulGRETunnelInstanceNumber,
        PCOSA_DML_GRE_TUNNEL_IF_FULL     pEntry
    );

ANSC_STATUS
CosaDmlGRETunnelIfSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulTunnelInstanceNumber,
        ULONG                       ulIfInstanceNumber,
        char*                       pAlias
    );

ANSC_STATUS
CosaDmlGRETunnelSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        char*                       pAlias
    );

ANSC_STATUS
CosaDmlGRETunnelIfSetCfg
    (
        ANSC_HANDLE                     hContext,
        ULONG                           ulGRETunnelInstanceNumber,
        PCOSA_DML_GRE_TUNNEL_IF_FULL    pEntry
    );

ANSC_STATUS
CosaDmlGRETunnelIfGetCfg
    (
        ANSC_HANDLE                      hContext,
        ULONG                            ulGRETunnelInstanceNumber,
        PCOSA_DML_GRE_TUNNEL_IF_FULL     pEntry
    );

ANSC_STATUS
CosaDmlGRETunnelGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulGRETunnelInstanceNumber,
        PCOSA_DML_GRE_TUNNEL_INFO   pEntry
    );

ANSC_STATUS
CosaDmlGRETunnelIfGetInfo
    (
        ANSC_HANDLE                    hContext,
        ULONG                          ulGRETunnelInstanceNumber,
        ULONG                          ulIfInstanceNumber,
        PCOSA_DML_GRE_TUNNEL_IF_INFO   pEntry
    );

ANSC_STATUS
CosaDmlGRETunnelDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

ANSC_STATUS
CosaDmlGRETunnelIfDelEntry
    (
        ANSC_HANDLE                  hContext,
        ULONG                        ulGRETunnelInstanceNumber,
        PCOSA_DML_GRE_TUNNEL_IF_FULL pInterface
    );

ANSC_STATUS
CosaDmlGRETunnelDelPsm
    (
        PCOSA_DML_GRE_TUNNEL_CFG     pCfg
    );

ANSC_STATUS
CosaDmlGRETunnelIfDelPsm
    (
        ULONG                        TunnelInstanceNumber,
        ULONG                        IfInstanceNumber
    );

ANSC_STATUS
CosaDmlGRETunnelGetStats
    (
        ANSC_HANDLE                          hContext,
        ULONG                                ulIndex,
        PCOSA_DML_GRE_TUNNEL_STATS           pStats
    );

ANSC_STATUS
CosaDmlGRETunnelIfGetStats
    (
        ANSC_HANDLE                          hContext,
        UCHAR*                               intfName,
        PCOSA_DML_GRE_TUNNEL_IF_STATS        pStats
    );

int GRE_hotspot_update_circuit_ids
    (
        int tunnelInst,
        int queuestart
    );

#endif

#endif
