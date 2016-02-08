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

#ifndef _COSA_BRIDGE_API_EXT_
#define _COSA_BRIDGE_API_EXT_

//#include "cosa_networking_common.h"

#define BRIDGE_NAMESPACE "CosaBridge"
#define BRIDGE_PORT_NAMESPACE "CosaBridgePort"
#define BRIDGE_PORT_ID_NAMESPACE BRIDGE_PORT_NAMESPACE"IDs"
#define BRIDGE_ID_NAMESPACE BRIDGE_NAMESPACE"IDs"

typedef struct _cosa_bport {
    SINGLE_LINK_ENTRY Linkage;
    char alias[COSA_DML_IF_NAME_LENGTH];
    ULONG instanceNumber;
    char name[COSA_DML_IF_NAME_LENGTH];
    COSA_DML_BRG_STD standard;
    struct _cosa_bport_control* control;
    char linkName[COSA_DML_IF_NAME_LENGTH];
    void* hwid; //ID for the control functions to map to platform
    COSA_DML_BRG_LINK_TYPE linkType;
    BOOLEAN bMgt;
    //$HL 4/15/2013
    BOOLEAN bEnabled;
    BOOLEAN bPriTag;
    BOOLEAN bUpstream;
    BOOLEAN bAllowDelete;
    COSA_DML_BPORT_MODE mode;
    int   pvid;
} BRIDGE_PORT, *PBRIDGE_PORT;

typedef struct _cosa_bport_control {
    ANSC_STATUS (*setEnabled)(PBRIDGE_PORT port, BOOLEAN enable);
    ANSC_STATUS (*getEnabled)(PBRIDGE_PORT port, BOOLEAN* enabled);
    ANSC_STATUS (*setManagementPort)(PBRIDGE_PORT port, BOOLEAN isMgt);
    ANSC_STATUS (*getStatus)(PBRIDGE_PORT port, PCOSA_DML_IF_STATUS status);
    ANSC_STATUS (*getAFT)(PBRIDGE_PORT port, PCOSA_DML_BRG_PORT_AFT aft);
    ANSC_STATUS (*setAFT)(PBRIDGE_PORT port, COSA_DML_BRG_PORT_AFT aft);
    uint32_t (*getLastChanged)(PBRIDGE_PORT port);
    ANSC_STATUS (*setLowerLink)(PBRIDGE_PORT port, char* name, COSA_DML_BRG_LINK_TYPE type);
    ANSC_STATUS (*getState)(PBRIDGE_PORT port, PCOSA_DML_BRG_PORT_STATE state);
    ANSC_STATUS (*getStats)(PBRIDGE_PORT port, PCOSA_DML_IF_STATS stats);
    ANSC_STATUS (*addToVlan)(PBRIDGE_PORT port, ULONG vlanId, BOOLEAN untagged);
    ANSC_STATUS (*removeFromVlan)(PBRIDGE_PORT port, ULONG vlanId);
} BRIDGE_PORT_CONTROL, *PBRIDGE_PORT_CONTROL;

//$HL 7/8/2013
typedef struct _cosa_bvlan {
    SINGLE_LINK_ENTRY Linkage;
    char alias[COSA_DML_IF_NAME_LENGTH];
    ULONG instanceNumber;
} BRIDGE_VLAN, *PBRIDGE_VLAN;

//Struture for a TR-181 Bridge object
typedef struct bridge {
    SINGLE_LINK_ENTRY Linkage;
    SLIST_HEADER portList;
    //$HL 7/8/2013
    SLIST_HEADER vlanList;
    char alias[COSA_DML_IF_NAME_LENGTH];
    ULONG instanceNumber;
    void * hwid;
    //$HL 4/15/2013
    int   numOfPorts;
    int   numOfVlans;
    int   vlanid;
    char  type[COSA_DML_IF_NAME_LENGTH];
    ULONG   l2InstanceNumber;
    BOOLEAN bPriTag;
    BOOLEAN bUpstream;
    BOOLEAN bAllowDelete;      
    //BOOLEAN atom_bPriTag;  
    BOOLEAN bEnabled;
    //BOOLEAN atom_bEnabled;
    COSA_DML_BRG_STATUS status;
    char    name[COSA_DML_IF_NAME_LENGTH];
    //char    atom_Name[COSA_DML_IF_NAME_LENGTH];   
    //$HL
    COSA_DML_BRG_STD standard;
    struct _bridge_ctl* control;
    //To be extended with vlan entries
} BRIDGE, *PBRIDGE;

typedef struct _bridge_ctl {
    ANSC_STATUS (*getStatus)(struct bridge* bridge, PCOSA_DML_BRG_STATUS status);
    ANSC_STATUS (*setEnabled)(struct bridge* bridge, BOOLEAN enable);
    ANSC_STATUS (*addPort)(struct bridge* bridge, PCOSA_DML_BRG_PORT_CFG newPortCfg);
    ANSC_STATUS (*removePort)(struct bridge* bridge, ULONG portInstanceNumber);
//$HL 4/16/2013    
    ANSC_STATUS (*confirmStructureUpdate)(struct bridge* bridge, PBRIDGE_PORT pBPort,PCOSA_DML_BRG_PORT_CFG cfg);
    ANSC_STATUS (*addVlan)(struct bridge* bridge, PCOSA_DML_BRG_VLAN_CFG pEntry);
    ANSC_STATUS (*removeVlan)(struct bridge* bridge, ULONG vlanId);
} BRIDGE_CONTROL, *PBRIDGE_CONTROL;

typedef struct bridging {
    SLIST_HEADER bridgeList;
    SLIST_HEADER filterList;
} BRIDGING_INFO;

PBRIDGE getBridge(ULONG instanceNumber);
PBRIDGE_PORT getBPort(PBRIDGE bridge, ULONG instanceNumber);
PBRIDGE_VLAN getBVlan(PBRIDGE pBridge, ULONG ulInstanceNumber);

//Lan switch bridge functions
ANSC_STATUS lSwBrGetStatus(struct bridge* bridge, PCOSA_DML_BRG_STATUS status);
ANSC_STATUS lSwBrSetEnabled(struct bridge* bridge, BOOLEAN enable);
ANSC_STATUS lSwBrAddPort(struct bridge* bridge, PCOSA_DML_BRG_PORT_CFG newPortCfg);
ANSC_STATUS lSwBrRemovePort(struct bridge* bridge, ULONG portInstanceNumber);
//$HL 4/19/2013
ANSC_STATUS lSwBrConfirmStructureUpdate(struct bridge* bridge, PBRIDGE_PORT pBPort,PCOSA_DML_BRG_PORT_CFG cfg);
ANSC_STATUS lSwBrAddVlan(struct bridge *bridge, PCOSA_DML_BRG_VLAN_CFG pEntry);
ANSC_STATUS lSwBrRemoveVlan(struct bridge *bridge, ULONG vlanId);

#ifdef _COSA_DRG_TPG_
//hwid typedefs and functions



//Lan switch bridge port functions, assume hwid from ethernet module.
ANSC_STATUS lanSwBrPCtlSetEnabled(PBRIDGE_PORT port, BOOLEAN enable);
ANSC_STATUS lanSwBrPCtlGetEnabled(PBRIDGE_PORT port, BOOLEAN* enabled);
ANSC_STATUS lanSwBrPCtlSetMgtPort(PBRIDGE_PORT port, BOOLEAN isMgt);
ANSC_STATUS lanSwBrPCtlGetStatus(PBRIDGE_PORT port, PCOSA_DML_IF_STATUS status);
ANSC_STATUS lanSwBrPCtlGetAFT(PBRIDGE_PORT port, PCOSA_DML_BRG_PORT_AFT aft);
ANSC_STATUS lanSwBrPCtlSetAFT(PBRIDGE_PORT port, COSA_DML_BRG_PORT_AFT aft);
uint32_t lanSwBrPCtlGetLastChanged(PBRIDGE_PORT port);
ANSC_STATUS lanSwBrPCtlSetLowerLink(PBRIDGE_PORT port, char* name, COSA_DML_BRG_LINK_TYPE type);
ANSC_STATUS lanSwBrPCtlGetState(PBRIDGE_PORT port, PCOSA_DML_BRG_PORT_STATE state);
ANSC_STATUS lanSwBrPCtlGetStats(PBRIDGE_PORT port, PCOSA_DML_IF_STATS stats);
ANSC_STATUS lanSwBrPCtlAddToVlan(PBRIDGE_PORT port, ULONG vlanId, BOOLEAN untagged);
ANSC_STATUS lanSwBrPCtlRemoveFromVlan(PBRIDGE_PORT port, ULONG vlanId);

#endif //_COSA_DRG_TPG_

#ifdef _COSA_INTEL_USG_ARM_
//Linux interface functions and hwid structure
typedef struct _linuxInterfaceID {
    char* ifname;
} LINUX_IF_ID, *PLINUX_IF_ID;

//Control functions for ethernet lan's linux bridge port
ANSC_STATUS lanBrPCtlSetEnabled(PBRIDGE_PORT port, BOOLEAN enable);
ANSC_STATUS lanBrPCtlGetEnabled(PBRIDGE_PORT port, BOOLEAN* enabled);
ANSC_STATUS lanBrPCtlSetMgtPort(PBRIDGE_PORT port, BOOLEAN isMgt);
ANSC_STATUS lanBrPCtlGetStatus(PBRIDGE_PORT port, PCOSA_DML_IF_STATUS status);
ANSC_STATUS lanBrPCtlGetAFT(PBRIDGE_PORT port, PCOSA_DML_BRG_PORT_AFT aft);
ANSC_STATUS lanBrPCtlSetAFT(PBRIDGE_PORT port, COSA_DML_BRG_PORT_AFT aft);
uint32_t lanBrPCtlGetLastChanged(PBRIDGE_PORT port);
ANSC_STATUS lanBrPCtlSetLowerLink(PBRIDGE_PORT port, char* name, COSA_DML_BRG_LINK_TYPE type);
ANSC_STATUS lanBrPCtlGetState(PBRIDGE_PORT port, PCOSA_DML_BRG_PORT_STATE state);
ANSC_STATUS lanBrPCtlGetStats(PBRIDGE_PORT port, PCOSA_DML_IF_STATS stats);
ANSC_STATUS lanBrPCtlAddToVlan(PBRIDGE_PORT port, ULONG vlanId, BOOLEAN untagged);
ANSC_STATUS lanBrPCtlRemoveFromVlan(PBRIDGE_PORT port, ULONG vlanId);

//Control functions for linux bridge's bridge port
ANSC_STATUS lnxBrPCtlSetEnabled(PBRIDGE_PORT port, BOOLEAN enable);
ANSC_STATUS lnxBrPCtlGetEnabled(PBRIDGE_PORT port, BOOLEAN* enabled);
ANSC_STATUS lnxBrPCtlSetMgtPort(PBRIDGE_PORT port, BOOLEAN isMgt);
ANSC_STATUS lnxBrPCtlGetStatus(PBRIDGE_PORT port, PCOSA_DML_IF_STATUS status);
ANSC_STATUS lnxBrPCtlGetAFT(PBRIDGE_PORT port, PCOSA_DML_BRG_PORT_AFT aft);
ANSC_STATUS lnxBrPCtlSetAFT(PBRIDGE_PORT port, COSA_DML_BRG_PORT_AFT aft);
uint32_t lnxBrPCtlGetLastChanged(PBRIDGE_PORT port);
ANSC_STATUS lnxBrPCtlSetLowerLink(PBRIDGE_PORT port, char* name, COSA_DML_BRG_LINK_TYPE type);
ANSC_STATUS lnxBrPCtlGetState(PBRIDGE_PORT port, PCOSA_DML_BRG_PORT_STATE state);
ANSC_STATUS lnxBrPCtlGetStats(PBRIDGE_PORT port, PCOSA_DML_IF_STATS stats);
ANSC_STATUS lnxBrPCtlAddToVlan(PBRIDGE_PORT port, ULONG vlanId, BOOLEAN untagged);
ANSC_STATUS lnxBrPCtlRemoveFromVlan(PBRIDGE_PORT port, ULONG vlanId);

//Lan software bridge functions
ANSC_STATUS SWBrGetStatus(struct bridge* bridge, PCOSA_DML_BRG_STATUS status);
ANSC_STATUS SWBrSetEnabled(struct bridge* bridge, BOOLEAN enable);
ANSC_STATUS SWBrAddPort(struct bridge* bridge, PCOSA_DML_BRG_PORT_CFG newPortCfg);
ANSC_STATUS SWBrRemovePort(struct bridge* bridge, ULONG portInstanceNumber);
//$HL 4/19/2013
ANSC_STATUS SWBrConfirmStructureUpdate(struct bridge* bridge, PBRIDGE_PORT pBPort, PCOSA_DML_BRG_PORT_CFG cfg);
ANSC_STATUS SWBrAddVlan(struct bridge *bridge, PCOSA_DML_BRG_VLAN_CFG pEntry);
ANSC_STATUS SWBrRemoveVlan(struct bridge *bridge, ULONG vlanId);
#endif

#endif //_COSA_BRIDGE_API_EXT_
