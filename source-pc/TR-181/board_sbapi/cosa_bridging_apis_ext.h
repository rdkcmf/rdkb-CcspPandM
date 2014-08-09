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
} BRIDGE_PORT_CONTROL, *PBRIDGE_PORT_CONTROL;

//Struture for a TR-181 Bridge object
typedef struct bridge {
    SINGLE_LINK_ENTRY Linkage;
    SLIST_HEADER portList;
    char alias[COSA_DML_IF_NAME_LENGTH];
    ULONG instanceNumber;
    void * hwid;
    COSA_DML_BRG_STD standard;
    struct _bridge_ctl* control;
    //To be extended with vlan entries
} BRIDGE, *PBRIDGE;

typedef struct _bridge_ctl {
    ANSC_STATUS (*getStatus)(struct bridge* bridge, PCOSA_DML_BRG_STATUS status);
    ANSC_STATUS (*setEnabled)(struct bridge* bridge, BOOLEAN enable);
    ANSC_STATUS (*addPort)(struct bridge* bridge, PCOSA_DML_BRG_PORT_CFG newPortCfg);
    ANSC_STATUS (*removePort)(struct bridge* bridge, ULONG portInstanceNumber);
    ANSC_STATUS (*confirmStructureUpdate)(struct bridge* bridge, PCOSA_DML_BRG_PORT_CFG cfg);
} BRIDGE_CONTROL, *PBRIDGE_CONTROL;

typedef struct bridging {
    SLIST_HEADER bridgeList;
    SLIST_HEADER filterList;
} BRIDGING_INFO;

PBRIDGE getBridge(ULONG instanceNumber);
PBRIDGE_PORT getBPort(PBRIDGE bridge, ULONG instanceNumber);

//Lan switch bridge functions
ANSC_STATUS lSwBrGetStatus(struct bridge* bridge, PCOSA_DML_BRG_STATUS status);
ANSC_STATUS lSwBrSetEnabled(struct bridge* bridge, BOOLEAN enable);
ANSC_STATUS lSwBrAddPort(struct bridge* bridge, PCOSA_DML_BRG_PORT_CFG newPortCfg);
ANSC_STATUS lSwBrRemovePort(struct bridge* bridge, ULONG portInstanceNumber);
ANSC_STATUS lSwBrConfirmStructureUpdate(struct bridge* bridge, PCOSA_DML_BRG_PORT_CFG cfg);

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

#endif //_COSA_DRG_TPG_

#ifdef _COSA_DRG_CNS_
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

//Lan software bridge functions
ANSC_STATUS SWBrGetStatus(struct bridge* bridge, PCOSA_DML_BRG_STATUS status);
ANSC_STATUS SWBrSetEnabled(struct bridge* bridge, BOOLEAN enable);
ANSC_STATUS SWBrAddPort(struct bridge* bridge, PCOSA_DML_BRG_PORT_CFG newPortCfg);
ANSC_STATUS SWBrRemovePort(struct bridge* bridge, ULONG portInstanceNumber);
ANSC_STATUS SWBrConfirmStructureUpdate(struct bridge* bridge, PCOSA_DML_BRG_PORT_CFG cfg);
#endif

#endif //_COSA_BRIDGE_API_EXT_
