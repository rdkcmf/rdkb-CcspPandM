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


#ifndef _COSA_ETH_INT_EXT_
#define _COSA_ETH_INT_EXT_

#define COSA_ETH_INT_SYSCFG_NAMESPACE "CosaEthInt"
#define COSA_ETH_INT_ID_SYSCFG_NAMESPACE COSA_ETH_INT_SYSCFG_NAMESPACE"IDs"

#define COSA_ETH_LINK_SYSCFG_NAMESPACE "CosaEthLink"
#define COSA_ETH_LINK_ID_SYSCFG_NAMESPACE COSA_ETH_LINK_SYSCFG_NAMESPACE"IDs"

//$HL 4/24/2013
#define SWITCH_PORT_0_NAME "sw_1"
#define SWITCH_PORT_1_NAME "sw_2"
#define SWITCH_PORT_2_NAME "sw_3"
#define SWITCH_PORT_3_NAME "sw_4"

#if defined (_CBR_PRODUCT_REQ_)
    #define SWITCH_PORT_4_NAME "sw_5"
    #define SWITCH_PORT_5_NAME "sw_6"
    #define SWITCH_PORT_6_NAME "sw_7"
    #define SWITCH_PORT_7_NAME "sw_8"
    #define SWITCH_PORT_MOCA_NAME "sw_9"
#else
    #define SWITCH_PORT_MOCA_NAME "sw_5"
#endif

#define SWITCH_PORT_EWAN_NAME "ewan"
#define SWITCH_PORT_CM_NAME "cmwan"

#define LAN_ETHERNET_IFNAME "lan0"
#define BRLAN_ETHERNET_IFNAME "brlan0"

#ifdef _COSA_INTEL_USG_ARM_
#define WAN_ETHERNET_IFNAME "erouter0"
#else
#define WAN_ETHERNET_IFNAME "wan0"
#endif

#define DMSB_ETH_IF_NAME_DFT_WanBridging            "lbr0"
#define DMSB_ETH_IF_NAME_DFT_WanRouting             WAN_ETHERNET_IFNAME

#include "cosa_ethernet_apis.h"

typedef struct _CosaEthInt
{
    PCOSA_DML_ETH_PORT_SINFO sInfo;
    char Alias[COSA_DML_IF_NAME_LENGTH];
    ULONG instanceNumber;
    ULONG LastChange;
    struct _EthCtlFn* control;
    void* hwid;
    COSA_DML_ETH_STATS LastStats;
} CosaEthInterfaceInfo, *PCosaEthInterfaceInfo;

typedef struct _CosaEthLink
{
    PCOSA_DML_ETH_LINK_FULL pEthLink;
    COSA_DML_ETH_STATS      LastStats;
} CosaEthLinkInfo, *PCosaEthLinkInfo;

typedef struct _EthCtlFn {
    int (*getCfg)(PCosaEthInterfaceInfo eth, PCOSA_DML_ETH_PORT_CFG pCfg);
    int (*setCfg)(PCosaEthInterfaceInfo eth, PCOSA_DML_ETH_PORT_CFG pCfg);
    int (*getStats)(PCosaEthInterfaceInfo eth, PCOSA_DML_ETH_STATS pStats);
    int (*getDInfo)(PCosaEthInterfaceInfo eth, PCOSA_DML_ETH_PORT_DINFO pDinfo);
} EthIntControlFuncs, *PEthIntControlFuncs;

/// Map key to interface object
PCosaEthInterfaceInfo getIF(const ULONG instanceNumber);
PCosaEthLinkInfo getLink(const ULONG ulInstanceNumber);
PCOSA_DML_ETH_VLAN_TERMINATION_FULL getVlanTermination(const ULONG ulInstanceNumber);

#ifdef _COSA_DRG_TPG_
typedef struct _swPortID {
    int unit;
    int port;
} SwitchPortID, *PSwitchPortID;

int getSwitchCfg(PCosaEthInterfaceInfo eth, PCOSA_DML_ETH_PORT_CFG pcfg);
int setSwitchCfg(PCosaEthInterfaceInfo eth, PCOSA_DML_ETH_PORT_CFG pcfg);
int ethGetSwitchStats(PCosaEthInterfaceInfo eth, PCOSA_DML_ETH_STATS pStats);
int getSwitchStats(PSwitchPortID swID, PCOSA_DML_IF_STATS pStats);
int getSwitchDInfo(PCosaEthInterfaceInfo eth, PCOSA_DML_ETH_PORT_DINFO pDinfo);
#endif

#ifdef _COSA_INTEL_USG_ARM_
static int getIfCfg(PCosaEthInterfaceInfo eth, PCOSA_DML_ETH_PORT_CFG pcfg);
static int setIfCfg(PCosaEthInterfaceInfo eth, PCOSA_DML_ETH_PORT_CFG pcfg);
static int getIfStats(PCosaEthInterfaceInfo eth, PCOSA_DML_ETH_STATS pStats);
static int getIfDInfo(PCosaEthInterfaceInfo eth, PCOSA_DML_ETH_PORT_DINFO pDinfo);
#endif

static int getIfStats2(const PUCHAR pName, PCOSA_DML_ETH_STATS pStats);

#endif
