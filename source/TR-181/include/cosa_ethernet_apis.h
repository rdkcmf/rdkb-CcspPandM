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

    module: cosa_ethernet_apis.h

        For COSA Data Model Library Development

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


#ifndef  _COSA_ETHERNET_API_H
#define  _COSA_ETHERNET_API_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"

/**********************************************************************
                STRUCTURE AND CONSTANT DEFINITIONS
**********************************************************************/

#define MAXINSTANCE                 128
#define ETH_INTERFACE_MAX_ASSOC_DEVICES   256
#define MAC_SZ  6 /* Mac address in Hex format */
#define MACADDR_SZ 18 /* Expanded MAC address in 00:02:... format */

typedef  enum
_COSA_DML_ETH_DUPLEX_MODE
{
    COSA_DML_ETH_DUPLEX_Half        = 1,
    COSA_DML_ETH_DUPLEX_Full,
    COSA_DML_ETH_DUPLEX_Auto
}
COSA_DML_ETH_DUPLEX_MODE, *PCOSA_DML_ETH_DUPLEX_MODE;


typedef  struct
_COSA_DML_ETH_STATS
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
}
COSA_DML_ETH_STATS, *PCOSA_DML_ETH_STATS;


typedef  struct
_COSA_DML_ETH_PORT_CFG
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    LONG                            MaxBitRate;
    COSA_DML_ETH_DUPLEX_MODE        DuplexMode;
}
COSA_DML_ETH_PORT_CFG,  *PCOSA_DML_ETH_PORT_CFG;


/*
 *  Static portion of Ethernet port info
 */
typedef  struct
_COSA_DML_ETH_PORT_SINFO
{
    char                            Name[COSA_DML_IF_NAME_LENGTH];
    BOOLEAN                         bUpstream;
    UCHAR                           MacAddress[6];
}
COSA_DML_ETH_PORT_SINFO,  *PCOSA_DML_ETH_PORT_SINFO;


/*
 *  Dynamic portion of Ethernet port info
 */
typedef  struct
_COSA_DML_ASSOCDEV_INFO
{
    CHAR                            MacAddress[MACADDR_SZ];
}
COSA_DML_ASSOCDEV_INFO, *PCOSA_DML_ASSOCDEV_INFO;

typedef  struct
_COSA_DML_ETH_PORT_DINFO
{
    COSA_DML_IF_STATUS              Status;
    ULONG                           CurrentBitRate;
    ULONG                           LastChange;
    ULONG                           AssocDevicesCount;
    UCHAR                           AssocDevices[(MAC_SZ*ETH_INTERFACE_MAX_ASSOC_DEVICES)];
}
COSA_DML_ETH_PORT_DINFO,  *PCOSA_DML_ETH_PORT_DINFO;


typedef  struct
_COSA_DML_ETH_PORT_FULL
{
    COSA_DML_ETH_PORT_CFG           Cfg;
    COSA_DML_ETH_PORT_SINFO         StaticInfo;
    COSA_DML_ETH_PORT_DINFO         DynamicInfo;
    COSA_DML_ASSOCDEV_INFO     	    AssocClient[ETH_INTERFACE_MAX_ASSOC_DEVICES];
}
COSA_DML_ETH_PORT_FULL, *PCOSA_DML_ETH_PORT_FULL;


typedef  struct
_COSA_DML_ETH_LINK_CFG
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    BOOLEAN                         bPriorityTagging;
    COSA_DML_LINK_TYPE              LinkType;           /* LinkType and LinkName/LinkInstNum constitutes LowerLayers */
    char                            LinkName[COSA_DML_IF_NAME_LENGTH];  /* This field is filled anyway */
    ULONG                           LinkInstNum;        /* Option 2, multi-LAN SB PSM design */
    char                            LowerLayers[COSA_DML_LOWERLAYER_NAME_LENGTH];   /* Only used in middle layer */
}
COSA_DML_ETH_LINK_CFG,  *PCOSA_DML_ETH_LINK_CFG;


/*
 *  Static portion of Ethernet Link info
 */
typedef  struct
_COSA_DML_ETH_LINK_SINFO
{
    char                            Name[COSA_DML_IF_NAME_LENGTH];
    UCHAR                           MacAddress[6];
}
COSA_DML_ETH_LINK_SINFO,  *PCOSA_DML_ETH_LINK_SINFO;


/*
 *  Dynamic portion of Ethernet Link info
 */
typedef  struct
_COSA_DML_ETH_LINK_DINFO
{
    COSA_DML_IF_STATUS              Status;
    ULONG                           LastChange;
}
COSA_DML_ETH_LINK_DINFO,  *PCOSA_DML_ETH_LINK_DINFO;


typedef  struct
_COSA_DML_ETH_LINK_FULL
{
    COSA_DML_ETH_LINK_CFG           Cfg;
    COSA_DML_ETH_LINK_SINFO         StaticInfo;
    COSA_DML_ETH_LINK_DINFO         DynamicInfo;
}
COSA_DML_ETH_LINK_FULL, *PCOSA_DML_ETH_LINK_FULL;


/*
 *  Configuration portion of Ethernet VLAN Termination info
 */
typedef  struct
_COSA_DML_ETH_VLAN_TERMINATION_CFG
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    char                            LowerLayers[COSA_DML_IF_NAME_LENGTH];
    char                            EthLinkName[COSA_DML_IF_NAME_LENGTH];   /* associated Ethernet Link */
    ULONG                           VLANID;
}
COSA_DML_ETH_VLAN_TERMINATION_CFG,  *PCOSA_DML_ETH_VLAN_TERMINATION_CFG;


/*
 *  Static portion of Ethernet VLAN Termination info
 */
typedef  struct
_COSA_DML_ETH_VLAN_TERMINATION_SINFO
{
    char                            Name[COSA_DML_IF_NAME_LENGTH];
}
COSA_DML_ETH_VLAN_TERMINATION_SINFO,  *PCOSA_DML_ETH_VLAN_TERMINATION_SINFO;


/*
 *  Dynamic portion of Ethernet VLAN Termination info
 */
typedef  struct
_COSA_DML_ETH_VLAN_TERMINATION_DINFO
{
    COSA_DML_IF_STATUS              Status;
    ULONG                           LastChange;
}
COSA_DML_ETH_VLAN_TERMINATION_DINFO,  *PCOSA_DML_ETH_VLAN_TERMINATION_DINFO;


typedef  struct
_COSA_DML_ETH_VLAN_TERMINATION_FULL
{
    COSA_DML_ETH_VLAN_TERMINATION_CFG   Cfg;
    COSA_DML_ETH_VLAN_TERMINATION_SINFO StaticInfo;
    COSA_DML_ETH_VLAN_TERMINATION_DINFO DynamicInfo;
    COSA_DML_ETH_STATS                  LastStats;
}
COSA_DML_ETH_VLAN_TERMINATION_FULL, *PCOSA_DML_ETH_VLAN_TERMINATION_FULL;


/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/

ANSC_STATUS
CosaDmlEthInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

/*
 *  Ethernet Port
 */
ULONG
CosaDmlEthPortGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    );

ANSC_STATUS
CosaDmlEthPortGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_ETH_PORT_FULL     pEntry
    );

ANSC_STATUS
CosaDmlEthPortSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

ANSC_STATUS
CosaDmlEthPortSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ETH_PORT_CFG      pCfg        /* Identified by InstanceNumber */
    );

ANSC_STATUS
CosaDmlEthPortGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ETH_PORT_CFG      pCfg        /* Identified by InstanceNumber */
    );

ANSC_STATUS
CosaDmlEthPortGetDinfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_ETH_PORT_DINFO    pInfo
    );

ANSC_STATUS
CosaDmlEthPortGetClientMac
    (
        PCOSA_DML_ETH_PORT_FULL pEthernetPortFull,
        ULONG                   ulInstanceNumber
    );

ANSC_STATUS
CosaDmlEthPortGetStats
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_ETH_STATS         pStats
    );

/*
 *  Ethernet Link
 */
ULONG
CosaDmlEthLinkGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    );

ANSC_STATUS
CosaDmlEthLinkGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_ETH_LINK_FULL     pEntry
    );

ANSC_STATUS
CosaDmlEthLinkSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

ANSC_STATUS
CosaDmlEthLinkAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ETH_LINK_FULL     pEntry
    );

ANSC_STATUS
CosaDmlEthLinkDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

ANSC_STATUS
CosaDmlEthLinkSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ETH_LINK_CFG      pCfg        /* Identified by InstanceNumber */
    );

ANSC_STATUS
CosaDmlEthLinkGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ETH_LINK_CFG      pCfg        /* Identified by InstanceNumber */
    );

ANSC_STATUS
CosaDmlEthLinkGetDinfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_ETH_LINK_DINFO    pInfo
    );

ANSC_STATUS
CosaDmlEthLinkGetStats
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_ETH_STATS         pStats
    );

ANSC_STATUS
CosaDmlEthLinkUpdateStaticLowerLayerName
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_ETH_LINK_FULL     pEntry
    );

/*
 *  Ethernet VLAN Termination
 */
ULONG
CosaDmlEthVlanTerminationGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    );

ANSC_STATUS
CosaDmlEthVlanTerminationGetEntry
    (
        ANSC_HANDLE                         hContext,
        ULONG                               ulIndex,
        PCOSA_DML_ETH_VLAN_TERMINATION_FULL pEntry
    );

ANSC_STATUS
CosaDmlEthVlanTerminationSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

ANSC_STATUS
CosaDmlEthVlanTerminationAddEntry
    (
        ANSC_HANDLE                         hContext,
        PCOSA_DML_ETH_VLAN_TERMINATION_FULL pEntry
    );

ANSC_STATUS
CosaDmlEthVlanTerminationDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

ANSC_STATUS
CosaDmlEthVlanTerminationValidateCfg
    (
        ANSC_HANDLE                        hContext,
        PCOSA_DML_ETH_VLAN_TERMINATION_CFG pCfg,
        char*                              pReturnParamName,
        ULONG*                             puLength
    );

ANSC_STATUS
CosaDmlEthVlanTerminationSetCfg
    (
        ANSC_HANDLE                        hContext,
        PCOSA_DML_ETH_VLAN_TERMINATION_CFG pCfg        /* Identified by InstanceNumber */
    );

ANSC_STATUS
CosaDmlEthVlanTerminationGetCfg
    (
        ANSC_HANDLE                        hContext,
        PCOSA_DML_ETH_VLAN_TERMINATION_CFG pCfg        /* Identified by InstanceNumber */
    );

ANSC_STATUS
CosaDmlEthVlanTerminationGetDinfo
    (
        ANSC_HANDLE                          hContext,
        ULONG                                ulInstanceNumber,
        PCOSA_DML_ETH_VLAN_TERMINATION_DINFO pInfo
    );

ANSC_STATUS
CosaDmlEthVlanTerminationGetStats
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_ETH_STATS         pStats
    );
#ifdef _HUB4_PRODUCT_REQ_
ANSC_STATUS
CosaDmlEthLinkGetWanUpDownTime
    (
        char* pcWanUpDownTime,
	int nTimeLength
    );
#endif
#endif

