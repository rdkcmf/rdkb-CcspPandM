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

    module: cosa_bridging_apis.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        Li Shi

    -------------------------------------------------------------------

    revision:

        03/16/2011    initial revision.

**************************************************************************/

#include "cosa_bridging_apis.h"
#include "safec_lib_common.h"


#if defined _COSA_DRG_TPG_ || _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_
#include "cosa_bridging_apis_ext.h"
#include "cosa_ethernet_apis_ext.h"
#include "utctx/utctx_api.h"
#include "cosa_drg_common.h"
#include "secure_wrapper.h"

#ifndef NELEMS
#define NELEMS(a)   (sizeof(a) / sizeof((a)[0]))
#endif

static BRIDGING_INFO g_Bridgings;

/*
static BRIDGE_CONTROL g_lanSwCtl = {
    lSwBrGetStatus,
    lSwBrSetEnabled,
    lSwBrAddPort,
    lSwBrRemovePort,
    lSwBrConfirmStructureUpdate,
    lSwBrAddVlan,
    lSwBrRemoveVlan,
};
*/

static int gInit = 0;

/*
static int loadID(char* ifName, ULONG* ulInstanceNumber, char* pAlias);
static int saveID(char* Namespace, char* ifName, ULONG ulInstanceNumber,char* pAlias);
*/
#endif


#ifdef _COSA_DRG_TPG_
#include "cosa_moca_apis.h"
#include "ulog/ulog.h"

#include "libswcfg.h"

static BRIDGE_PORT g_fixedLanPorts[6];
static BRIDGE g_lanSwitchBridge;



static BRIDGE_PORT_CONTROL g_lanSwPCtl = {
    lanSwBrPCtlSetEnabled,
    lanSwBrPCtlGetEnabled,
    lanSwBrPCtlSetMgtPort,
    lanSwBrPCtlGetStatus,
    lanSwBrPCtlGetAFT,
    lanSwBrPCtlSetAFT,
    lanSwBrPCtlGetLastChanged,
    lanSwBrPCtlSetLowerLink,
    lanSwBrPCtlGetState,
    lanSwBrPCtlGetStats,
    lanSwBrPCtlAddToVlan,
    lanSwBrPCtlRemoveFromVlan,
};

static SwitchPortID g_swid[] = {
    {0,0},
    {0,1},
    {0,2},
    {0,3},
    {0,5},
    {0,8}
};

static void pollRemotePorts();

#endif

#if defined _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_
#include <net/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if_bridge.h>
#include <stdio.h>
#include "sysevent/sysevent.h"
#include "ulog/ulog.h"

/*
static LINUX_IF_ID g_linIFIDs = {
    LAN_ETHERNET_IFNAME
};
*/

static BRIDGE_PORT_CONTROL g_lanEthBrPCtl = {
//Control functions for ethernet lan's linux bridge port
    lanBrPCtlSetEnabled,
    lanBrPCtlGetEnabled,
    lanBrPCtlSetMgtPort,
    lanBrPCtlGetStatus,
    lanBrPCtlGetAFT,
    lanBrPCtlSetAFT,
    lanBrPCtlGetLastChanged,
    lanBrPCtlSetLowerLink,
    lanBrPCtlGetState,
    lanBrPCtlGetStats,
    lanBrPCtlAddToVlan,
    lanBrPCtlRemoveFromVlan,
};

static BRIDGE_PORT_CONTROL g_SWBrPCtl = {
    lnxBrPCtlSetEnabled,
    lnxBrPCtlGetEnabled,
    lnxBrPCtlSetMgtPort,
    lnxBrPCtlGetStatus,
    lnxBrPCtlGetAFT,
    lnxBrPCtlSetAFT,
    lnxBrPCtlGetLastChanged,
    lnxBrPCtlSetLowerLink,
    lnxBrPCtlGetState,
    lanBrPCtlGetStats,
    lnxBrPCtlAddToVlan,
    lnxBrPCtlRemoveFromVlan,
};

static BRIDGE_CONTROL g_SWBrCtl = {
    SWBrGetStatus,
    SWBrSetEnabled,
    SWBrAddPort,
    SWBrRemovePort,
    SWBrConfirmStructureUpdate,
    SWBrAddVlan,
    SWBrRemoveVlan,
};

//$HL 4/10/2013
/*
DeviceName
----------
Local Network interface: athx, ethport-x, moca-x
ethport-1 -> sw0
ethport-2-> sw1
ethport-3-> sw2
ethport-4 -> sw3

WAN Network interface: eRouter0 (routing traffic), lbr0 (bridge traffic)

L2 configuration data
----------------------
dmsb.l2net.x.BridgeName= brx
dmsb.l2net.x.vlanid = 100
dmsb.atom.l2net.x.BridgeName= brx
dmsb.atom.l2net.x.vlanid = 100
dmsb.l2net.x.PriorityTagging = false (not support now)
dmsb.l2net.xupstream.vid = 150 or same as dmsb.l2net.x.vid (not support now)
dmsb.l2net.x.Enable = true
dmsb.atom.l2net.x.members.wifi= wifiSsid.x(-t), wifiSsid.x(-t)s
dmsb.l2net.x.members.eth= ethport-x(-t)
dmsb.l2net.x.members.moca= moca-x(-t)
dmsb.l2net.x.members.link = l2sd0
dmsb.l2net.37.Port.1.pvid = 103

Port level
dmsb.l2net.1.Port.1.priTag = FALSE
dmsb.l2net.1.Port.1.enable = FALSE
dmsb.l2net.1.Port.1.alias = Port1
dmsb.l2net.1.Port.1.name = br1
dmsb.l2net.1.Port.1.linkName = 
dmsb.l2net.1.Port.1.linkType = NONE
dmsb.l2net.1.Port.1.mgt = TRUE
dmsb.l2net.1.Port.1.standard = 0
dmsb.l2net.1.Port.1.mode = 2
dmsb.l2net.1.Port.1.instanceNum = 1
dmsb.l2net.1.Port.1.link2eRouter = FALSE

dmsb.l2net.1.Port.2.priTag = FALSE
dmsb.l2net.1.Port.2.enable = FALSE
dmsb.l2net.1.Port.2.alias = Port1
dmsb.l2net.1.Port.2.name = 
dmsb.l2net.1.Port.2.linkName = ath1
dmsb.l2net.1.Port.2.linkType = WiFi
dmsb.l2net.1.Port.2.mgt = FALSE
dmsb.l2net.1.Port.2.standard = 0
dmsb.l2net.1.Port.2.mode = 2
dmsb.l2net.1.Port.2.instanceNum = 1
dmsb.l2net.1.Port.2.link2eRouter = FALSE

*/
#include <string.h>
#include "ccsp_psm_helper.h"
#include "dmsb_tr181_psm_definitions.h"

#define DMSB_VLAN_LOWBOUND               164
#define DMSB_VLAN_HIGHBOUND              400

#define DMSB_BRID_LOWBOUND              64

extern ANSC_HANDLE bus_handle;
extern char g_Subsystem[32];

typedef  enum
_PSM_BRIDGE_TML
{
/*-----------More detail on low level -----------*/
    _PSM_BRIDGE_TML_NAME = 0,
    _PSM_BRIDGE_TML_VLANID,
    _PSM_BRIDGE_TML_PRITAG,
    _PSM_BRIDGE_TML_ENABLE,
//    _PSM_BRIDGE_TML_ATOM_NAME,
//    _PSM_BRIDGE_TML_ATOM_VLANID,
//    _PSM_BRIDGE_TML_ATOM_PRITAG,
//    _PSM_BRIDGE_TML_ATOM_BENABLE,
    _PSM_BRIDGE_TML_SW_MEMBERS,
    _PSM_BRIDGE_TML_ETH_MEMBERS,
    _PSM_BRIDGE_TML_MOCA_MEMBERS,
    //$HL 07/15/2013
    _PSM_BRIDGE_TML_GRE_MEMBERS,
    _PSM_BRIDGE_TML_WIFI_MEMBERS,
    _PSM_BRIDGE_TML_LINK_MEMBERS,
/*-----------DM managment info only -----------*/
    _PSM_BRIDGE_TML_ALIAS,
    _PSM_BRIDGE_TML_STANDARD,
    _PSM_BRIDGE_TML_INSTNUM,    
    _PSM_BRIDGE_TML_TYPE,
    _PSM_BRIDGE_TML_ALLOWDELETE
} PSM_BRIDGE_TML;

typedef enum
_PSM_BVLAN_TML
{
    _PSM_BVLAN_TML_ALIAS =0,
    _PSM_BVLAN_TML_INSTNUM    
} PSM_BVLAN_TML;

typedef  enum
_PSM_BPORT_TML
{
/*-----------More detail on low level -----------*/
//    _PSM_BPORT_TML_ATOM_PVID = 0,
//    _PSM_BPORT_TML_ATOM_PRITAG,
//    _PSM_BPORT_TML_ATOM_ENABLE,
    _PSM_BPORT_TML_PVID = 0 ,
    _PSM_BPORT_TML_PRITAG,
/*-----------DM managment info only -----------*/    
    _PSM_BPORT_TML_ENABLE,
    _PSM_BPORT_TML_ALIAS,
    _PSM_BPORT_TML_NAME,
    _PSM_BPORT_TML_LINKNAME,
    _PSM_BPORT_TML_LINKTYPE,
    _PSM_BPORT_TML_MGT,
    _PSM_BPORT_TML_MODE,
    _PSM_BPORT_TML_INSTNUM,
    _PSM_BPORT_TML_UPSTREAM,
    _PSM_BPORT_TML_ALLOWDELETE
} PSM_BPORT_TML;

typedef struct _COSA_DML_BRIDGE_PSM_INFO {
        ULONG numOfEntries;
        ULONG firstInstanceNum;
        ULONG lastInstanceNum;
        ULONG vlanLowBound;
        ULONG vlanHighBound;
        int   nxtVid;
        int   nxtBrId;
        char  bridgeNamePrefix[256];
} COSA_DML_BRIDGE_PSM_INFO, PCOSA_DML_BRIDGE_PSM_INFO;

static COSA_DML_BRIDGE_PSM_INFO g_brInfo;

static const char *g_brTemple[] = {
/*-----------More detail on low level -----------*/
    DMSB_L2_CONCAT("Name"),
    DMSB_L2_CONCAT("Vid"),
    DMSB_L2_CONCAT("PriorityTag"),
    DMSB_L2_CONCAT("Enable"),
//    DMSB_ATOM_L2_CONCAT("name"),
//    DMSB_ATOM_L2_CONCAT("vlanid"),
//    DMSB_ATOM_L2_CONCAT("priTag"),
//    DMSB_ATOM_L2_CONCAT("enable"),
    DMSB_L2_CONCAT("Members.SW"),
    DMSB_L2_CONCAT("Members.Eth"),
    DMSB_L2_CONCAT("Members.Moca"),
    //$HL 7/15/2013
    DMSB_L2_CONCAT("Members.Gre"),
    DMSB_L2_CONCAT("Members.WiFi"),
    DMSB_L2_CONCAT("Members.Link"),
/*-----------DM managment info only -----------*/
    DMSB_L2_CONCAT("Alias"),
    DMSB_L2_CONCAT("Standard"),
    DMSB_L2_CONCAT("InstanceNum"),
    DMSB_L2_CONCAT("Type"),
    DMSB_L2_CONCAT("AllowDelete"),
    NULL
};

static const char *g_bPortTemple[] = {
/*-----------More detail on low level -----------*/
//    DMSB_L2_DEVICE_CONCAT("pvid"),
//    DMSB_L2_DEVICE_CONCAT("priTag"),
/*-----------DM managment info only -----------*/
    DMSB_L2_BPORT_CONCAT("Pvid"),
    DMSB_L2_BPORT_CONCAT("PriorityTag"),
    DMSB_L2_BPORT_CONCAT("Enable"),
    DMSB_L2_BPORT_CONCAT("Alias"),
    DMSB_L2_BPORT_CONCAT("Name"),
    DMSB_L2_BPORT_CONCAT("LinkName"),
    DMSB_L2_BPORT_CONCAT("LinkType"),
    DMSB_L2_BPORT_CONCAT("Management"),
    DMSB_L2_BPORT_CONCAT("Mode"),
    DMSB_L2_BPORT_CONCAT("InstanceNum"),
    DMSB_L2_BPORT_CONCAT("Upstream"),
    DMSB_L2_BPORT_CONCAT("AllowDelete"),
    NULL
};

static const char *g_bVlanTemple[] = {
    DMSB_L2_BVLAN_CONCAT("Alias"),
    DMSB_L2_BVLAN_CONCAT("InstanceNum"),
    NULL
};
#define _PSM_GET_BR(_DmsbInx) { \
        _ansc_memset(param_name, 0, sizeof(param_name)); \
        errno_t rc = -1; \
        rc = sprintf_s(param_name, sizeof(param_name), g_brTemple[_DmsbInx], instancenum); \
        if(rc < EOK) { \
           ERR_CHK(rc); \
        } \
        retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, param_name, NULL, &param_value); \
        if (retPsmGet != CCSP_SUCCESS) { \
            goto _Psm_GetBr_Err;\
        } \
        else { \
            /*AnscTraceFlow(("%s: retPsmGet == CCSP_SUCCESS reading %s = %s\n", __FUNCTION__,param_name,param_value)); */\
        } \
    }

#define _PSM_SET_BR(_DmsbInx) { \
        errno_t rc = -1; \
        rc = sprintf_s(param_name, sizeof(param_name), g_brTemple[_DmsbInx], instancenum); \
        if(rc < EOK) { \
           ERR_CHK(rc); \
        } \
        retPsmGet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, param_name, ccsp_string, param_value); \
        if (retPsmGet != CCSP_SUCCESS) { \
            goto _Psm_SetBr_Err;\
        } \
        /*AnscTraceFlow(("%s: retPsmSet == CCSP_SUCCESS setting %s = %s \n", __FUNCTION__,param_name,param_value)); */\
        _ansc_memset(param_name, 0, sizeof(param_name)); \
        _ansc_memset(param_value, 0, sizeof(param_value)); \
    }

#define _PSM_GET_BPORT(_DmsbInx) { \
        _ansc_memset(param_name, 0, sizeof(param_name)); \
        errno_t rc = -1; \
        if (deviceStr) { \
            rc = sprintf_s(param_name, sizeof(param_name), g_bPortTemple[_DmsbInx], l2InstNum, deviceStr); \
            if(rc < EOK) { \
               ERR_CHK(rc); \
            } \
        } \
        else { \
            rc = sprintf_s (param_name, sizeof(param_name), g_bPortTemple[_DmsbInx], l2InstNum, bportInstNum); \
            if(rc < EOK) { \
               ERR_CHK(rc); \
            } \
        } \
        retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, param_name, NULL, &param_value); \
        if (retPsmGet != CCSP_SUCCESS) { \
            goto _Psm_GetBPort_Err; \
        } \
        /*AnscTraceFlow(("%s: retPsmGet == CCSP_SUCCESS reading %s=%s\n", __FUNCTION__,param_name,param_value)); */\
    }    

#define _PSM_SET_BPORT(_DmsbInx) { \
        errno_t rc = -1; \
        if (deviceStr) { \
            rc = sprintf_s(param_name, sizeof(param_name), g_bPortTemple[_DmsbInx], l2InstNum,deviceStr); \
            if(rc < EOK) { \
                ERR_CHK(rc); \
            } \
        } \
        else { \
            rc = sprintf_s(param_name, sizeof(param_name), g_bPortTemple[_DmsbInx], l2InstNum,bportInstNum); \
            if(rc < EOK) { \
                ERR_CHK(rc); \
            } \
        } \
        retPsmGet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, param_name, ccsp_string, param_value); \
        if (retPsmGet != CCSP_SUCCESS) { \
            goto _Psm_SetBPort_Err; \
        } \
        _ansc_memset(param_name, 0, sizeof(param_name)); \
        _ansc_memset(param_value, 0, sizeof(param_value)); \
    }    

#define _PSM_DEL_BPORT(_DmsbInx) { \
            errno_t rc = -1; \
            if (deviceStr) { \
                rc = sprintf_s(param_name, sizeof(param_name), g_bPortTemple[_DmsbInx], l2InstNum, deviceStr); \
                if(rc < EOK) { \
                    ERR_CHK(rc); \
                } \
            } \
            else \
            { \
                rc = sprintf_s(param_name, sizeof(param_name), g_bPortTemple[_DmsbInx], l2InstNum,  bportInstNum); \
                if(rc < EOK) { \
                    ERR_CHK(rc); \
                } \
            } \
            AnscTraceFlow(("%s delete brigde inst=%d Port inst =%d param_name=%s\n", __FUNCTION__,\
                l2InstNum, bportInstNum,param_name));\
            PSM_Del_Record(bus_handle, g_Subsystem, param_name); \
            _ansc_memset(param_name, 0, sizeof(param_name)); \
        }

#define _PSM_GET_BVLAN(_DmsbInx) { \
        errno_t rc = -1; \
        _ansc_memset(param_name, 0, sizeof(param_name)); \
        rc = sprintf_s(param_name, sizeof(param_name), g_bVlanTemple[_DmsbInx], l2InstNum, bvlanInstNum); \
        if(rc < EOK) { \
            ERR_CHK(rc); \
        } \
        retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, param_name, NULL, &param_value); \
        if (retPsmGet != CCSP_SUCCESS) { \
            goto _Psm_GetBVlan_Err; \
        } \
        /*AnscTraceFlow(("%s: retPsmGet == CCSP_SUCCESS reading %s=%s\n", __FUNCTION__,param_name,param_value)); */\
    }    

#define _PSM_SET_BVLAN(_DmsbInx) { \
        errno_t rc = -1; \
        rc = sprintf_s(param_name, sizeof(param_name), g_bVlanTemple[_DmsbInx], l2InstNum,bvlanInstNum); \
        if(rc < EOK) { \
            ERR_CHK(rc); \
        } \
        retPsmGet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, param_name, ccsp_string, param_value); \
        if (retPsmGet != CCSP_SUCCESS) { \
            goto _Psm_SetBVlan_Err; \
        } \
        /*AnscTraceFlow(("%s: retPsmSet == CCSP_SUCCESS setting %s = %s\n", __FUNCTION__,param_name,param_value)); */\
        _ansc_memset(param_name, 0, sizeof(param_name)); \
        _ansc_memset(param_value, 0, sizeof(param_value)); \
    }    

#define _PSM_DEL_BVLAN(_DmsbInx) { \
            errno_t rc = -1; \
            rc = sprintf_s(param_name, sizeof(param_name), g_bVlanTemple[_DmsbInx], l2InstNum,  bvlanInstNum); \
            if(rc < EOK) { \
                ERR_CHK(rc); \
            } \
            AnscTraceFlow(("%s delete brigde inst=%d Vlan inst =%d param_name=%s\n", __FUNCTION__,\
                l2InstNum, bvlanInstNum,param_name));\
            PSM_Del_Record(bus_handle, g_Subsystem, param_name); \
            _ansc_memset(param_name, 0, sizeof(param_name)); \
        }

#define _COSA_FORMAT_MEMBERS_STRING(_Bridge, _BPort, _DmsbInx,_Value )  { \
            errno_t rc = -1; \
            if (_BPort->mode==COSA_DML_BPORT_PASSTHRU) \
            { \
                rc = sprintf_s(_Value, 256, "%s-t", _BPort->linkName); \
                if(rc < EOK) { \
                    ERR_CHK(rc); \
                } \
            } \
            else \
            { \
                rc = sprintf_s(_Value, 256, "%s", _BPort->linkName); \
                if(rc < EOK) { \
                    ERR_CHK(rc); \
                } \
            }  \
            if (_ansc_strstr(_BPort->linkName,"sw")) \
            { \
                rc = sprintf_s(_DmsbInx, 256, g_brTemple[_PSM_BRIDGE_TML_SW_MEMBERS], _Bridge->l2InstanceNumber, \
                    _Value); \
                if(rc < EOK) { \
                    ERR_CHK(rc); \
                } \
            } \
            else if (_BPort->linkType == COSA_DML_BRG_LINK_TYPE_WiFiSsid) \
            { \
                rc = sprintf_s(_DmsbInx, 256, g_brTemple[_PSM_BRIDGE_TML_WIFI_MEMBERS], _Bridge->l2InstanceNumber, \
                    _Value); \
                if(rc < EOK) { \
                    ERR_CHK(rc); \
                } \
            } \
            else if ((_BPort->linkType == COSA_DML_BRG_LINK_TYPE_Eth) || \
                (_BPort->linkType == COSA_DML_BRG_LINK_TYPE_EthVlan)) \
            { \
                rc = sprintf_s(_DmsbInx, 256, g_brTemple[_PSM_BRIDGE_TML_ETH_MEMBERS], _Bridge->l2InstanceNumber, \
                    _Value); \
                if(rc < EOK) { \
                    ERR_CHK(rc); \
                } \
            } \
            else if (_BPort->linkType == COSA_DML_BRG_LINK_TYPE_Moca)\
            { \
                rc = sprintf_s(_DmsbInx, 256, g_brTemple[_PSM_BRIDGE_TML_MOCA_MEMBERS], _Bridge->l2InstanceNumber, \
                    _Value); \
                if(rc < EOK) { \
                    ERR_CHK(rc); \
                } \
            } \
            else if (_BPort->linkType == COSA_DML_BRG_LINK_TYPE_Gre)\
            { \
                rc = sprintf_s(_DmsbInx, 256, g_brTemple[_PSM_BRIDGE_TML_GRE_MEMBERS], _Bridge->l2InstanceNumber, \
                    _Value); \
                if(rc < EOK) { \
                    ERR_CHK(rc); \
                } \
            } \
        } 

#define _PSM_REMOVE_MEMBERS(_Bridge, _BPort, _DmsbInx, _Value )  { \
            if (AnscSizeOfString(_BPort->linkName)) { \
                _COSA_FORMAT_MEMBERS_STRING(_Bridge,_BPort, _DmsbInx, _Value ); \
                AnscTraceFlow(("<HL>%s port remove member Inx=%s, Val=%s\n", __FUNCTION__, _DmsbInx, _Value)); \
                _COSA_RemoveBPortMember(_DmsbInx, _Value); \
            } \
        }

#define _PSM_ADD_MEMBERS(_Bridge, _BPort, _DmsbInx, _Value )  { \
            if (AnscSizeOfString(_BPort->linkName)) { \
                _COSA_FORMAT_MEMBERS_STRING(_Bridge,_BPort, _DmsbInx, _Value ); \
                AnscTraceFlow(("<HL>%s port add member Inx=%s, Val=%s\n", __FUNCTION__, _DmsbInx, _Value)); \
                _COSA_AddBPortMember(_DmsbInx, _Value); \
            } \
        }

static ANSC_STATUS _COSA_DelBr(ULONG instanceNumber);
static ANSC_STATUS _COSA_DelBPort(ULONG ulInstanceNumber,PBRIDGE pBridge);
static ANSC_STATUS _COSA_DelBVlan(ULONG ulInstanceNumber,PBRIDGE pBridge);
static char * _COSA_GetInterfaceTypeStr(COSA_DML_BRG_LINK_TYPE linktype);

static ANSC_STATUS _COSA_AddToken(char *token, char *pStr);
static ANSC_STATUS _COSA_DelToken(char *token, char *pStr);
static ANSC_STATUS _COSA_GetNextVlanId(PBRIDGE pBridge);
static ANSC_STATUS _COSA_GetNewBrName(PBRIDGE pBridge);
static ANSC_STATUS _COSA_NewL2InstNumber(PBRIDGE pBridge);
static ANSC_STATUS _Psm_AddLinkMembers(ULONG instanceNum);
static ANSC_STATUS _Psm_DelLinkMembers(ULONG instanceNum);
static ANSC_STATUS _COSA_UpdateBrVLANID(PBRIDGE pBridge,int vlanid);

static ANSC_STATUS _Psm_GetDefaultSetting(void);

static ANSC_STATUS _Psm_GetBPortMemberList(char *path, char *list);
static ANSC_STATUS _Psm_SetBPortMemberList(char *path, char *list);
static ANSC_STATUS _COSA_AddBPortMember(char *path, char *name);
static ANSC_STATUS _COSA_RemoveBPortMember(char *path, char *name);

static ANSC_STATUS _Psm_GetBr(ULONG instanPBRIDGE, PBRIDGE pBridge);
static ANSC_STATUS _Psm_SetBr(ULONG instanPBRIDGE,PBRIDGE pBridge);
static ANSC_STATUS _Psm_DelBr(PBRIDGE pBridge);
static ANSC_STATUS _Psm_GetBPort(ULONG l2InstNum, ULONG bportInstNum,PBRIDGE_PORT pPort);
static ANSC_STATUS _Psm_SetBPort(ULONG l2InstNum, ULONG bportInstNum,PBRIDGE_PORT pPort);
static ANSC_STATUS _Psm_Cleanup(int instanceNumber);
static ANSC_STATUS _Psm_GetBVlan(ULONG l2InstNum, ULONG bVlanInstNum, PBRIDGE_VLAN pBVlan);
static ANSC_STATUS _Psm_SetBVlan(ULONG l2InstNum, ULONG bvlanInstNum, PBRIDGE_VLAN pBVlan);
#endif

/**********************************************************************

    caller:     COSA DML

    prototype:

        ULONG
        CosaDmlBrgInit
            (
                ANSC_HANDLE                 hContext,
            );

    Description:

        Backend implementation to get the number of Ddns servies.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

    Return:       The number of Ddns servies.

**********************************************************************/

#include <sys/time.h>
static void print_time(const char *msg)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    fprintf(stderr, "[DM-Bridge] %20s: %6d.%06d\n", msg ? msg : "", (int)tv.tv_sec, (int)tv.tv_usec);
}

void CosaDmlPrintHSVlanPsmValue(char *fun,int linenum)
{
	char *param_value= NULL;

	if ( CCSP_SUCCESS == PSM_Get_Record_Value2(bus_handle, g_Subsystem,"dmsb.l2net.4.Members.WiFi", NULL, &param_value) )
	{
		AnscTraceFlow(("HSVLAN_CORRUPT:[%s,%d]PSM_VALUE=%s\n", fun, linenum, ( param_value )	? param_value : "NULL" ));

		if( param_value )
		{
			((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(param_value);
			param_value = NULL;
		}
	}
}

ANSC_STATUS
CosaDmlBrgInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
)
{
    UNREFERENCED_PARAMETER(hDml);
    UNREFERENCED_PARAMETER(phContext);
    print_time("CosaDmlBrgInit -- in");
#if defined _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_
    int retPsmGet1 = CCSP_SUCCESS;
    int retPsmGet2 = CCSP_SUCCESS;
    //$HL 7/8/2013
    PBRIDGE lanbr = NULL;
    PBRIDGE_PORT curPort = NULL;
    PBRIDGE_VLAN curVlan = NULL;
    unsigned int brCnt = 0, portCnt=0, vlanCnt=0;
    unsigned  int *brList = NULL, *portList = NULL, *vlanList= NULL;
    unsigned int i = 0, j=0;
    char param_name[256] = {0};
    errno_t rc = -1;

    //$HL 4/10/2013
    AnscTraceFlow(("<HL> Start Bridge/Port Initiation %s\n",__FUNCTION__));
    _ansc_memset(&g_brInfo, 0, sizeof(g_brInfo));        
    g_Bridgings.bridgeList.Depth = 0;

    CosaDmlPrintHSVlanPsmValue((char*) __FUNCTION__ , __LINE__ );
    //Set up ulog
    ulog_init();

    _Psm_GetDefaultSetting();

    retPsmGet1 = PsmGetNextLevelInstances(bus_handle, g_Subsystem, DMSB_L2_PREFIX, &brCnt, &brList);
 
    if ( retPsmGet1 == CCSP_SUCCESS && brList != NULL ) 
    {
        AnscTraceFlow(("<HL> %s Bridge Init: count=%d\n",__FUNCTION__,brCnt));
        for(i = 0; i < brCnt; i++) 
        {
            fprintf(stdout, "%d\n", brList[i]);
            if (brList[i] == 0)
            {
                //$HL 4/26/2013

                continue;
            }
            lanbr = (PBRIDGE)AnscAllocateMemory(sizeof (BRIDGE));
            _ansc_memset(lanbr, 0, sizeof(BRIDGE));
            _ansc_memset(param_name, 0, sizeof(param_name));
            rc = sprintf_s(param_name, sizeof(param_name), DMSB_L2_BPORT_PREFIX, brList[i]);
            if(rc < EOK)
            {
                ERR_CHK(rc);
            }
            //AnscTraceFlow(("%s: PSM L2 InstanceNumber %d, Search Path=%s\n", __FUNCTION__,brList[i],param_name));
            if (_Psm_GetBr(brList[i],lanbr)!=ANSC_STATUS_SUCCESS)
            {
                AnscTraceFlow(("<HL>%s Cannot get bridge dm data from PSM instancenum=%d\n",
                    __FUNCTION__,brList[i]));
                _Psm_Cleanup(brList[i]);
                AnscFreeMemory(lanbr);
                continue;
            }
            g_brInfo.nxtVid = lanbr->vlanid >= g_brInfo.nxtVid && (ULONG)lanbr->vlanid < g_brInfo.vlanHighBound ? lanbr->vlanid +1 : g_brInfo.nxtVid;
            lanbr->bUpstream = TRUE;
            if (i==0) 
            {
                AnscSListPushEntry(&g_Bridgings.bridgeList, &lanbr->Linkage);
            }
            else
            { 
                AnscSListPushEntryAtBack(&g_Bridgings.bridgeList, &lanbr->Linkage);
            }
            _ansc_memset(param_name, 0, sizeof(param_name));
            rc = sprintf_s(param_name, sizeof(param_name), DMSB_L2_BVLAN_PREFIX, brList[i]);
            if(rc < EOK)
            {
                ERR_CHK(rc);
            }
            //AnscTraceFlow(("%s: PSM L2 InstanceNumber %d, Search Path=%s\n", __FUNCTION__,brList[i],param_name));
            //rebuild vlan list for the bridge
            retPsmGet2 = PsmGetNextLevelInstances(bus_handle, g_Subsystem, param_name, &vlanCnt, &vlanList);
            if ( retPsmGet2 == CCSP_SUCCESS && vlanList != NULL ) 
            {
                AnscTraceFlow(("<HL> %s VLAN Init: count=%d\n",__FUNCTION__,vlanCnt));
                for( j=0; j < vlanCnt; j++) 
                {
                    curVlan = (PBRIDGE_VLAN)AnscAllocateMemory(sizeof (BRIDGE_VLAN));
                    _ansc_memset(curVlan, 0, sizeof(BRIDGE_VLAN));
                    if(_Psm_GetBVlan(brList[i],vlanList[j],curVlan)==ANSC_STATUS_SUCCESS)
                    {
                        if (j == 0) 
                        {
                            AnscSListPushEntry(&lanbr->vlanList, &curVlan->Linkage);
                        }
                        else
                        {
                            AnscSListPushEntryAtBack(&lanbr->vlanList, &curVlan->Linkage);
                        }
                        lanbr->numOfVlans++;
                    }
                    else
                    {
                        AnscFreeMemory(curVlan);
                        continue;
                    }
                }
                ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(vlanList);
            }
            else
            {
                AnscTraceFlow(("<HL> %s Cannot find any VLAN entry:%s\n",__FUNCTION__, param_name));
            }
            //rebuild port list for the bridge
            _ansc_memset(param_name, 0, sizeof(param_name));
            rc = sprintf_s(param_name, sizeof(param_name), DMSB_L2_BPORT_PREFIX, brList[i]);
            if(rc < EOK)
            {
                ERR_CHK(rc);
            }
            //AnscTraceFlow(("%s: PSM L2 InstanceNumber %d, Search Path=%s\n", __FUNCTION__,brList[i],param_name));
            retPsmGet2 = 0;
            retPsmGet2 = PsmGetNextLevelInstances(bus_handle, g_Subsystem, param_name, &portCnt, &portList);
            if ( retPsmGet2 == CCSP_SUCCESS && portList != NULL ) 
            {
                AnscTraceFlow(("<HL> %s Port Init: count=%d\n",__FUNCTION__,portCnt));
                for( j=0; j < portCnt; j++) 
                {
                    curPort = (PBRIDGE_PORT)AnscAllocateMemory(sizeof (BRIDGE_PORT));
                    _ansc_memset(curPort, 0, sizeof(BRIDGE_PORT));
                    if(_Psm_GetBPort(brList[i],portList[j],curPort)==ANSC_STATUS_SUCCESS)
                    {
                        if (j == 0) 
                        {
                            AnscSListPushEntry(&lanbr->portList, &curPort->Linkage);
                        }
                        else
                        {
                            AnscSListPushEntryAtBack(&lanbr->portList, &curPort->Linkage);
                        }
                        if (curPort->bUpstream==FALSE)
                            lanbr->bUpstream = FALSE;
                        lanbr->numOfPorts++;
                    }
                    else
                    {
                        AnscFreeMemory(curPort);
                        continue;
                    }
                }
                ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(portList);
            }
            else
            {
                AnscTraceFlow(("<HL> %s Cannot find any port entry:%s\n",__FUNCTION__, param_name));
            }
        }
        ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(brList);
    } 
    else 
    {
        AnscTraceFlow(("<HL> %s Cannot find any bridge entry: %s\n",__FUNCTION__,DMSB_L2_PREFIX));
        fprintf(stdout, "\n");
    }
    gInit = 1;
    CosaDmlPrintHSVlanPsmValue( (char*)__FUNCTION__ , __LINE__ );
    print_time("CosaDmlBrgInit -- OUT");
    return ANSC_STATUS_SUCCESS;
#endif
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ULONG
        CosaDmlBrgGetNumberOfEntries
            (
                ANSC_HANDLE                 hContext,
            );

    Description:

        Backend implementation to get the number of Ddns servies.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

    Return:       The number of Ddns servies.

**********************************************************************/

ULONG
CosaDmlBrgGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    )
{
#if defined _COSA_DRG_TPG_ || _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_
    int numOfEntries = 0;
    CcspTraceInfo(("------CosaDmlBrgGetNumberOfEntries...\n"));
    AnscTraceFlow(("<HL> %s \n",__FUNCTION__));
    if (!gInit) {
        CosaDmlBrgInit(hContext, NULL);
    }
    numOfEntries = AnscSListQueryDepth(&g_Bridgings.bridgeList);
    AnscTraceFlow(("<HL> %s numOfEntries = %d\n",__FUNCTION__,numOfEntries));
    return(numOfEntries);
#endif
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrgGetEntry
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulIndex,
                PCOSA_DML_BRG_FULL          pEntry
            );

    Description:

        Backend implementation to get a ddns service entry.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulIndex
                  Index of the service entry to be retrieved.

                  PCOSA_DML_BRG_FULL          pEntry
                  Pointer of the service to be retrieved.

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_BRG_FULL          pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
#if defined _COSA_DRG_TPG_ || _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_
    //CcspTraceInfo(("------CosaDmlBrgGetEntry, ulIndex:%d...\n", ulIndex));
    PBRIDGE pBridge = (PBRIDGE)AnscSListGetEntryByIndex(&g_Bridgings.bridgeList,ulIndex);
    if (!pBridge)
    {
        AnscTraceFlow(("<HL> %s index=%d not found \n",__FUNCTION__,ulIndex));
        return ANSC_STATUS_CANT_FIND;
    }
    //$HL /16/2013
    //loadID(pBridge->hwid, &pBridge->instanceNumber, pBridge->alias);
    //CcspTraceInfo(("------In CosaDmlBrgGetEntry, name:%s, instanceNumber:%d,alias:%s\n", pBridge->hwid, pBridge->instanceNumber, pBridge->alias));
    pEntry->Cfg.bEnabled = pBridge->bEnabled;
    AnscCopyString(pEntry->Cfg.Alias, pBridge->alias);
#if defined (MULTILAN_FEATURE)
    AnscCopyString(pEntry->Cfg.name, pBridge->name);
#endif
    pEntry->Cfg.InstanceNumber = pBridge->instanceNumber;
    pEntry->Cfg.Std = pBridge->standard;
    pEntry->Cfg.bAllowDelete = pBridge->bAllowDelete;
    pBridge->control->getStatus(pBridge, &pEntry->Info.Status);
    //AnscTraceFlow(("<HL> %s index=%d, SB instanceNumber=%d, DM instanceNumber=%d\n",__FUNCTION__,ulIndex,pBridge->instanceNumber,pEntry->Cfg.InstanceNumber));
    return ANSC_STATUS_SUCCESS;
#endif
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrgSetValues
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulIndex,
                ULONG                       ulInstanceNumber,
                char*                       pAlias
            );

    Description:

        Backend implementation to set back instance number and alias.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulIndex
                  Index of the service entry to be retrieved.

                  PCOSA_DML_DDNS_CFG          pCfg
                  Pointer of the service to be retrieved.

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
#if defined (MULTILAN_FEATURE)
        char*                       pAlias,
        char*                       pName
#else
        char*                       pAlias
#endif
    )
{
    UNREFERENCED_PARAMETER(hContext);
#if defined _COSA_DRG_TPG_ || _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_
    AnscTraceFlow(("<HL> %s Index=%d instancenum=%d\n",__FUNCTION__,ulIndex,ulInstanceNumber));

    CcspTraceInfo(("------CosaDmlBrgSetValues...\n"));
    PBRIDGE pBridge = (PBRIDGE) AnscSListGetEntryByIndex(&g_Bridgings.bridgeList,ulIndex);
    if (!pBridge)
    {
        AnscTraceFlow(("<HL> %s index=%d not found \n",__FUNCTION__,ulIndex));
        return ANSC_STATUS_CANT_FIND;
    }
    AnscCopyString( pBridge->alias, pAlias);
#if defined (MULTILAN_FEATURE)
    AnscCopyString( pBridge->name, pName);
#endif
    pBridge->instanceNumber = ulInstanceNumber;
    CcspTraceInfo(("------CosaDmlBrgSetValues, Alias:%s, instanceNum:%d...\n", pBridge->alias, pBridge->instanceNumber));
    //saveID(BRIDGE_ID_NAMESPACE, pBridge->hwid, ulInstanceNumber, pAlias);
    //$HL 4/15/2013
    _Psm_SetBr(pBridge->l2InstanceNumber, pBridge);
    return ANSC_STATUS_SUCCESS;
#endif
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrgAddEntry
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_BRG_CFG           pEntry
            );

    Description:

        Backend implementation to add a bridge service entry.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PCOSA_DML_BRG_CFG           pEntry
                  Pointer of the service to be added.

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_BRG_CFG           pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
#if defined _COSA_DRG_TPG_ || _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_
    //TPG can support 1 additional bridge, but we will leave
    //it static for now.

    //$HL 4/15/2013
    PBRIDGE pBridge = NULL;
    pBridge = (PBRIDGE)AnscAllocateMemory(sizeof (BRIDGE));

    AnscTraceFlow(("<HL> %s \n",__FUNCTION__));
    if (!pBridge)
    {
        AnscTraceFlow(("<HL> %s cannot allocate bridge resource\n",__FUNCTION__));
        return ANSC_STATUS_FAILURE;        
    }
    _ansc_memset(pBridge, 0, sizeof(BRIDGE));
    if (_COSA_GetNextVlanId(pBridge)==ANSC_STATUS_FAILURE)
    {
        AnscTraceFlow(("<HL> %s cannot allocate vlanId\n",__FUNCTION__));
        AnscFreeMemory(pBridge);
        return ANSC_STATUS_FAILURE;
    }
    
    pBridge->instanceNumber = pEntry->InstanceNumber;

    if (_COSA_NewL2InstNumber(pBridge)==ANSC_STATUS_FAILURE)
    {
        AnscTraceFlow(("<HL> %s cannot assign psm instanceNumber\n",__FUNCTION__));
        AnscFreeMemory(pBridge);
        return ANSC_STATUS_FAILURE;
    }

    if (_COSA_GetNewBrName(pBridge)==ANSC_STATUS_FAILURE)
    {
        AnscTraceFlow(("<HL> %s cannot assign bridge device name\n",__FUNCTION__));
        AnscFreeMemory(pBridge);
        return ANSC_STATUS_FAILURE;
    }
    
    pBridge->bEnabled = pEntry->bEnabled;
    pBridge->standard = pEntry->Std;
    AnscCopyString(pBridge->alias, pEntry->Alias);
#if defined (MULTILAN_FEATURE)
    AnscCopyString(pBridge->name, pEntry->name);
#endif
    pBridge->status = COSA_DML_BRG_STATUS_Disabled;

    //$HL 4/15/2013
    pBridge->numOfPorts = 0;
    pBridge->bUpstream = TRUE;
    pBridge->bAllowDelete = pEntry->bAllowDelete ;
    AnscCopyString(pBridge->type, DMSB_L2_TYPE_BRIDGE);

    if (g_Bridgings.bridgeList.Depth == 0) {
        AnscSListPushEntry(&g_Bridgings.bridgeList, &pBridge->Linkage);
    }
    else {
        AnscSListPushEntryAtBack(&g_Bridgings.bridgeList, &pBridge->Linkage);
    }

    _Psm_SetBr(pBridge->l2InstanceNumber, pBridge);

    pBridge->hwid = pBridge->name;
    pBridge->control = &g_SWBrCtl;

    v_secure_system("sysevent set multinet-syncNets");

    return ANSC_STATUS_SUCCESS;
#endif
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrg
        Entry
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulInstanceNumber
            );

    Description:

        Backend implementation to delete a ddns service entry.

    Arguments:    ANSC_HANDLE                hContext
                  Reserved.

                  ULONG                      ulInstanceNumber
                  Pointer of the service to be deleted.

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
    UNREFERENCED_PARAMETER(hContext);
#if defined _COSA_DRG_TPG_ || _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_
    AnscTraceFlow(("<HL> %s \n",__FUNCTION__));

    PBRIDGE pBridge = getBridge(ulInstanceNumber);
    if (!pBridge)
    {
        AnscTraceFlow(("<HL> %s bridge instancenum=%d not found \n",__FUNCTION__,ulInstanceNumber));
        return ANSC_STATUS_CANT_FIND;
    }
    if (!pBridge->bAllowDelete)
    {
        AnscTraceFlow(("<HL> %s bridge instancenum=%d not allow to delete \n",__FUNCTION__,ulInstanceNumber));
        return ANSC_STATUS_NOT_SUPPORTED;
    }
    _COSA_DelBr(pBridge->instanceNumber);
    v_secure_system("sysevent set multinet-syncNets");
    return ANSC_STATUS_SUCCESS;
#endif
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrgSetCfg
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_BRG_CFG           pCfg
            );

    Description:

        Backend implementation to delete a ddns service entry.

    Arguments:    ANSC_HANDLE                hContext
                  Reserved.

                  PCOSA_DML_BRG_CFG          pCfg
                  Pointer of the new service entry.

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_BRG_CFG           pCfg        /* Identified by InstanceNumber */
    )
{
    UNREFERENCED_PARAMETER(hContext);
#if defined _COSA_DRG_TPG_ || _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_
    int bEvent = 0;
    AnscTraceFlow(("<HL> %s \n",__FUNCTION__));

    CcspTraceInfo(("------CosaDmlBrgSetCfg...\n"));
    PBRIDGE pBridge = getBridge(pCfg->InstanceNumber);
    if (pBridge == NULL) {
        return ANSC_STATUS_CANT_FIND;
    }
    
    pBridge->standard = pCfg->Std;  //TODO: what impact should this have on the bridge? Maybe need another bridge function? Automatically set AFT to all? Set standard on all ports?
    AnscCopyString(pBridge->alias, pCfg->Alias);
#if defined (MULTILAN_FEATURE)
    AnscCopyString(pBridge->name, pCfg->name);
#endif
    //saveID(BRIDGE_ID_NAMESPACE, pBridge->hwid, pBridge->instanceNumber, pBridge->alias);
    CcspTraceInfo(("------CosaDmlBrgSetCfg...,name:%s,instancenum:%d,alias:%s\n",pBridge->hwid,pBridge->instanceNumber,pBridge->alias));
    if (pBridge->bEnabled != pCfg->bEnabled)
    {
        pBridge->bEnabled = pCfg->bEnabled;
        pBridge->control->setEnabled(pBridge, pCfg->bEnabled);
        bEvent = 1;
    }
    _Psm_SetBr(pBridge->l2InstanceNumber,pBridge);
    if (bEvent)
    {
        v_secure_system("sysevent set multinet-syncMembers %lu", pBridge->l2InstanceNumber);
    }
    return ANSC_STATUS_SUCCESS;
#endif
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrgGetCfg
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_BRG_CFG           pCfg
            );

    Description:

        Backend implementation to get Ddns configuration.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PCOSA_DML_BRG_CFG           pCfg
                  Pointer of the configuration to be retrieved.

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_BRG_CFG           pCfg        /* Identified by InstanceNumber */
    )
{
    UNREFERENCED_PARAMETER(hContext);
#if defined _COSA_DRG_TPG_ || _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_
    CcspTraceInfo(("------CosaDmlBrgGetCfg...\n"));
    PBRIDGE pBridge = getBridge(pCfg->InstanceNumber);
    AnscTraceFlow(("<HL> %s \n",__FUNCTION__));

    if (pBridge == NULL) {
        AnscTraceFlow(("<HL> %s bridge not found instanceNumber=%d\n",__FUNCTION__,
            pCfg->InstanceNumber));
        return ANSC_STATUS_CANT_FIND;
    }

    AnscCopyString(pCfg->Alias, pBridge->alias);
#if defined (MULTILAN_FEATURE)
    AnscCopyString(pCfg->name, pBridge->name);
#endif
    pCfg->InstanceNumber = pBridge->instanceNumber;
    pCfg->Std = pBridge->standard;
    pCfg->bEnabled = pBridge->bEnabled;
    pCfg->bAllowDelete = pBridge->bAllowDelete;
    return ANSC_STATUS_SUCCESS;
#endif
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrgGetInfo
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_BRG_INFO          pInfo
            );

    Description:

        Backend implementation to get Ddns configuration.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PCOSA_DML_BRG_INFO          pInfo
                  Pointer of the configuration to be retrieved.

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_BRG_INFO          pInfo
)
{
    UNREFERENCED_PARAMETER(hContext);
#if defined _COSA_DRG_TPG_ || _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_
    CcspTraceInfo(("------CosaDmlBrgGetInfo...\n"));
    AnscTraceFlow(("<HL> %s\n",__FUNCTION__));
    PBRIDGE pBridge = getBridge(ulInstanceNumber);
    if (pBridge == NULL) {
        return ANSC_STATUS_CANT_FIND;
    }

    pBridge->control->getStatus(pBridge, &pInfo->Status);

    return ANSC_STATUS_SUCCESS;
#endif
}

//$HL 7/8/2013
int CosaDmlBrgGetVLANID
    (
        ULONG                       ulInstanceNumber
    )
{
#if defined _COSA_DRG_TPG_ || _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_
    CcspTraceInfo(("------CosaDmlBrgGetInfo...\n"));
    AnscTraceFlow(("<HL> %s\n",__FUNCTION__));
    PBRIDGE pBridge = getBridge(ulInstanceNumber);
    if (pBridge == NULL) {
        return ANSC_STATUS_CANT_FIND;
    }
    return pBridge->vlanid;
#endif
}

char * CosaDmlBrgGetName
    (
        ULONG                       ulInstanceNumber
    )
{
#if defined _COSA_DRG_TPG_ || _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_
    CcspTraceInfo(("------CosaDmlBrgGetInfo...\n"));
    AnscTraceFlow(("<HL> %s\n",__FUNCTION__));
    PBRIDGE pBridge = getBridge(ulInstanceNumber);
    if (pBridge == NULL) {
        return '\0';
    }
    return pBridge->name;
#endif
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ULONG
        CosaDmlBrgPortGetNumberOfEntries
            (
                ANSC_HANDLE                 hContext,
            );

    Description:

        Backend implementation to get the number of Ddns servies.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

    Return:       The number of Ddns servies.

**********************************************************************/

ULONG
CosaDmlBrgPortGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber
    )
{
    UNREFERENCED_PARAMETER(hContext);
#if defined _COSA_DRG_TPG_ || _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_
//    CcspTraceInfo(("------CosaDmlBrgPortGetNumberOfEntries...\n"));
//    AnscTraceFlow(("<HL> %s\n",__FUNCTION__));
    PBRIDGE pBridge = getBridge(ulBrgInstanceNumber);
    if (pBridge == NULL) {
        AnscTraceFlow(("<HL> %s ulBrgInstanceNumber=%d Not found\n",__FUNCTION__,
            ulBrgInstanceNumber));
        return 0;
    }
    //return AnscSListQueryDepth(&pBridge->portList);
    AnscTraceFlow(("<HL> %s ulBrgInstanceNumber=%d numOfPorts=%d\n",__FUNCTION__,
            ulBrgInstanceNumber,pBridge->numOfPorts));
    return (pBridge->numOfPorts);
#endif
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrgPortGetEntry
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulIndex,
                PCOSA_DML_BRG_PORT_FULL     pEntry
            );

    Description:

        Backend implementation to get a ddns service entry.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulIndex
                  Index of the service entry to be retrieved.

                  PCOSA_DML_DDNS_CFG          pCfg
                  Pointer of the service to be retrieved.

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgPortGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_BRG_PORT_FULL     pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
#if defined _COSA_DRG_TPG_ || _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_
//    CcspTraceInfo(("------CosaDmlBrgPortGetEntry...\n"));
    PBRIDGE pBridge = getBridge(ulBrgInstanceNumber);
//    AnscTraceFlow(("<HL> %s ulIndex=%d\n",__FUNCTION__,ulIndex));
    if (pBridge == NULL) {
        AnscTraceFlow(("<HL> %s Bridge not found ulBrgInstanceNumber=%d, ulIndex=%d\n",__FUNCTION__,
            ulBrgInstanceNumber,ulIndex));
        return ANSC_STATUS_CANT_FIND;
    }
    PBRIDGE_PORT pBPort = (PBRIDGE_PORT) AnscSListGetEntryByIndex(&pBridge->portList, ulIndex);
    if (pBPort == NULL) {
        AnscTraceFlow(("<HL> %s port not found ulBrgInstanceNumber=%d, ulIndex=%d\n",__FUNCTION__,
            ulBrgInstanceNumber,ulIndex));
        return ANSC_STATUS_CANT_FIND;
    }
    
    //$HL 4/13/2013
    //loadID(pBPort->name, &pBPort->instanceNumber, pBPort->alias);
//    CcspTraceInfo(("------CosaDmlBrgPortGetEntry,name:%s,instancenum:%d,alias\n", pBPort->name, pBPort->instanceNumber, pBPort->alias));
    pBPort->control->getAFT(pBPort, &pEntry->Cfg.AcceptableFrameTypes);
    pEntry->Cfg.bEnabled = pBPort->bEnabled; 
    pEntry->Cfg.bIngressFiltering = FALSE;          //not supported for now
    pEntry->Cfg.bManagementPort = pBPort->bMgt;
    pEntry->Cfg.bPriorityTagging = pBPort->bPriTag;           //not supported for now
    pEntry->Cfg.DftUserPriority = 0;                //not supported for now
    pEntry->Cfg.InstanceNumber = pBPort->instanceNumber;
    pEntry->Cfg.LinkType = pBPort->linkType;
    pEntry->Cfg.PVID = pBPort->pvid;
    pEntry->Cfg.mode = pBPort->mode;
    //pEntry->Cfg.Std = pBPort->standard;
    pEntry->Cfg.bUpstream = pBPort->bUpstream;
    pEntry->Cfg.bAllowDelete = pBPort->bAllowDelete;
    pEntry->Info.LastChange = pBPort->control->getLastChanged(pBPort);        // Backend does not support for now
    pBPort->control->getState(pBPort, &pEntry->Info.PortState);
    pBPort->control->getStatus(pBPort,&pEntry->Info.Status);

    if (pEntry->Cfg.PriorityRegeneration)
        pEntry->Cfg.PriorityRegeneration[0] = '\0'; //not supported for now
    AnscCopyString(pEntry->Cfg.LinkName, pBPort->linkName);
    AnscCopyString(pEntry->Cfg.Alias, pBPort->alias);
    AnscCopyString(pEntry->Info.Name, pBPort->name);
    return ANSC_STATUS_SUCCESS;
#endif
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrgPortSetValues
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulIndex,
                ULONG                       ulInstanceNumber,
                char*                       pAlias
            );

    Description:

        Backend implementation to set back instance number and alias.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulIndex
                  Index of the service entry to be retrieved.

                  PCOSA_DML_DDNS_CFG          pCfg
                  Pointer of the service to be retrieved.

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgPortSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    UNREFERENCED_PARAMETER(hContext);
#if defined _COSA_DRG_TPG_ || _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_
    PBRIDGE pBridge = getBridge(ulBrgInstanceNumber);
    CcspTraceInfo(("------CosaDmlBrgPortSetValues...\n"));
    AnscTraceFlow(("<HL> %s\n",__FUNCTION__));
    if (pBridge == NULL) {
        AnscTraceFlow(("<$HL> %s cannot find bridge inst =%d, \n",__FUNCTION__,
            ulBrgInstanceNumber));
        return ANSC_STATUS_CANT_FIND;
    }
    PBRIDGE_PORT pBPort = (PBRIDGE_PORT) AnscSListGetEntryByIndex(&pBridge->portList, ulIndex);
    if (pBPort == NULL) {
        AnscTraceFlow(("<$HL> %s cannot find bridge inst =%d, port index=%d\n",__FUNCTION__,
            ulBrgInstanceNumber,ulIndex));
        return ANSC_STATUS_CANT_FIND;
    }
    AnscCopyString(pBPort->alias, pAlias);
    pBPort->instanceNumber = ulInstanceNumber;
    CcspTraceInfo(("------CosaDmlBrgPortSetValues, We are trying to save instancenum %d:%s...\n", pBPort->instanceNumber, pBPort->alias));
    //$HL 4//15/2013
    //saveID(BRIDGE_PORT_ID_NAMESPACE, pBPort->name, pBPort->instanceNumber,pBPort->alias);
    _Psm_SetBPort(pBridge->l2InstanceNumber, ulInstanceNumber, pBPort); 
    return ANSC_STATUS_SUCCESS;
#endif
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrgPortAddEntry
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_BRG_PORT_CFG      pEntry
            );

    Description:

        Backend implementation to add a ddns service entry.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PCOSA_DML_BRG_PORT_CFG      pEntry
                  Pointer of the service to be added.

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgPortAddEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        PCOSA_DML_BRG_PORT_CFG      pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
#if defined _COSA_DRG_TPG_ || _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_
    CcspTraceInfo(("------CosaDmlBrgPortAddEntry...\n"));
    AnscTraceFlow(("<HL> %s bridge inst=%d\n",__FUNCTION__,ulBrgInstanceNumber));
    PBRIDGE pBridge = getBridge(ulBrgInstanceNumber);
    if (pBridge == NULL) {
        AnscTraceFlow(("<HL> cannot find Bridge inst = %d",ulBrgInstanceNumber));
        return ANSC_STATUS_CANT_FIND;
    }

    return pBridge->control->addPort(pBridge, pEntry);
#endif
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrgPortDelEntry
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulInstanceNumber
            );

    Description:

        Backend implementation to delete a ddns service entry.

    Arguments:    ANSC_HANDLE                hContext
                  Reserved.

                  ULONG                      ulInstanceNumber
                  Pointer of the service to be deleted.

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgPortDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        ULONG                       ulInstanceNumber
    )
{
    UNREFERENCED_PARAMETER(hContext);
#if defined _COSA_DRG_TPG_ || _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_
    CcspTraceInfo(("------CosaDmlBrgPortDelEntry...\n"));
    PBRIDGE pBridge = getBridge(ulBrgInstanceNumber);
    AnscTraceFlow(("<HL> %s\n",__FUNCTION__));
    if (pBridge == NULL) {
        return ANSC_STATUS_CANT_FIND;
    }
    return pBridge->control->removePort(pBridge, ulInstanceNumber);
#endif
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrgPortSetCfg
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_BRG_PORT_CFG      pCfg
            );

    Description:

        Backend implementation to delete a ddns service entry.

    Arguments:    ANSC_HANDLE                hContext
                  Reserved.

                  PCOSA_DML_BRG_CFG          pCfg
                  Pointer of the new service entry.

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgPortSetCfg
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        PCOSA_DML_BRG_PORT_CFG      pCfg        /* Identified by InstanceNumber */
    )
{
    UNREFERENCED_PARAMETER(hContext);
#if defined _COSA_DRG_TPG_ || _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_
//    CcspTraceInfo(("------CosaDmlBrgPortSetCfg...\n"));
//    AnscTraceFlow(("<HL> %s\n",__FUNCTION__)); 
    PBRIDGE pBridge = getBridge(ulBrgInstanceNumber);
    if (pBridge == NULL) {
        AnscTraceFlow(("<HL>%s cannot find the birdge inst=%d\n",__FUNCTION__,ulBrgInstanceNumber ));
        return ANSC_STATUS_CANT_FIND;
    }
    PBRIDGE_PORT pBPort = getBPort(pBridge, pCfg->InstanceNumber);

    if (pBPort == NULL) {
        AnscTraceFlow(("<HL> %s cannot find port inst=%d\n",__FUNCTION__,pCfg->InstanceNumber)); 
        return ANSC_STATUS_CANT_FIND;
    } 
    
    if (!AnscEqualString(pCfg->LinkName, pBPort->linkName, TRUE) || pCfg->LinkType != pBPort->linkType
        || pCfg->bEnabled != pBPort->bEnabled || pCfg->mode != pBPort->mode)
    {
        pBridge->control->confirmStructureUpdate(pBridge, pBPort, pCfg);
    }
    pBPort->instanceNumber = pCfg->InstanceNumber;
    pBPort->bMgt = pCfg->bManagementPort;
    pBPort->bPriTag = pCfg->bPriorityTagging;
    pBPort->linkType = pCfg->LinkType;
    //pBPort->pvid = pCfg->PVID;
    pBPort->pvid = pBridge->vlanid;
    pBPort->mode = pCfg->mode;
    pBPort->bUpstream = pCfg->bUpstream;
    pBPort->bEnabled = pCfg->bEnabled;
    pBPort->bAllowDelete = pCfg->bAllowDelete;
    if (pBPort->bMgt)
    {
        AnscCopyString(pBPort->name, pBridge->name);
        pBPort->control = &g_SWBrPCtl;
    }
    else
    {
        pBPort->control = &g_lanEthBrPCtl;
        AnscCopyString(pBPort->name,pCfg->LinkName);
    }

    pBPort->control->setAFT(pBPort, pCfg->AcceptableFrameTypes);
    pBPort->control->setEnabled(pBPort, pCfg->bEnabled);
    
    AnscCopyString(pBPort->alias,pCfg->Alias);
    //$HL 4/16/2013
    _Psm_SetBPort(pBridge->l2InstanceNumber,pCfg->InstanceNumber,pBPort);
    return ANSC_STATUS_SUCCESS;
#endif
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrgPortGetCfg
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_BRG_PORT_CFG      pCfg
            );

    Description:

        Backend implementation to get Ddns configuration.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PCOSA_DML_BRG_PORT_CFG      pCfg
                  Pointer of the configuration to be retrieved.

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgPortGetCfg
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        PCOSA_DML_BRG_PORT_CFG      pCfg        /* Identified by InstanceNumber */
    )
{
    UNREFERENCED_PARAMETER(hContext);
#if defined _COSA_DRG_TPG_ || _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_
    CcspTraceInfo(("------CosaDmlBrgPortGetCfg...\n"));
    AnscTraceFlow(("<HL> %s\n",__FUNCTION__));
    PBRIDGE pBridge = getBridge(ulBrgInstanceNumber);
    if (pBridge == NULL) {
        return ANSC_STATUS_CANT_FIND;
    }
    PBRIDGE_PORT pBPort = getBPort(pBridge, pCfg->InstanceNumber);
    if (pBPort == NULL) {
        return ANSC_STATUS_CANT_FIND;
    }

    pBPort->control->getAFT(pBPort, &pCfg->AcceptableFrameTypes);
    pBPort->control->getEnabled(pBPort, &pCfg->bEnabled); 
    pCfg->bIngressFiltering = FALSE;          //not supported for now
    pCfg->bManagementPort = pBPort->bMgt;
    //$HL 4/16/2013
    //pCfg->bPriorityTagging = FALSE;           //not supported for now
    pCfg->bPriorityTagging = pBPort->bPriTag;           //not supported for now
    pCfg->DftUserPriority = 0;                //not supported for now
    pCfg->InstanceNumber = pBPort->instanceNumber;
    pCfg->LinkType = pBPort->linkType;
    //$HL 04/16/2013
    //pCfg->PVID = 0;                           //not supported for now
    pCfg->PVID = pBPort->pvid;                           
    pCfg->LinkType = pBPort->linkType;
    //pCfg->Std = pBPort->standard;
    pCfg->bEnabled = pBPort->bEnabled;
    pCfg->mode = pBPort->mode;
    pCfg->bUpstream = pBPort->bUpstream;
    pCfg->PriorityRegeneration[0] = '\0'; //not supported for now
    AnscCopyString(pCfg->LinkName, pBPort->linkName);
    AnscCopyString(pCfg->Alias, pBPort->alias);
    return ANSC_STATUS_SUCCESS;
#endif
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrgPortGetInfo
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_BRG_PORT_INFO     pInfo
            );

    Description:

        Backend implementation to get Ddns configuration.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PCOSA_DML_BRG_PORT_INFO     pInfo
                  Pointer of the configuration to be retrieved.

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgPortGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_BRG_PORT_INFO     pInfo
    )
{
    UNREFERENCED_PARAMETER(hContext);
#if defined _COSA_DRG_TPG_ || _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_
    CcspTraceInfo(("------CosaDmlBrgPortGetInfo...\n"));
    AnscTraceFlow(("<HL> %s\n",__FUNCTION__));
    PBRIDGE pBridge = getBridge(ulBrgInstanceNumber);
    if (pBridge == NULL) {
        return ANSC_STATUS_CANT_FIND;
    }
    PBRIDGE_PORT pBPort = getBPort(pBridge, ulInstanceNumber);
    if (pBPort == NULL) {
        return ANSC_STATUS_CANT_FIND;
    }
    pInfo->LastChange = pBPort->control->getLastChanged(pBPort);                    // Backend does not support for now
    pBPort->control->getState(pBPort, &pInfo->PortState);
    pBPort->control->getStatus(pBPort, &pInfo->Status);
    AnscCopyString(pInfo->Name, pBPort->name);
    return ANSC_STATUS_SUCCESS;
#endif
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrgPortGetStats
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_IF_STATS          pInfo
            );

    Description:

        Backend implementation to get Ddns configuration.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PCOSA_DML_IF_STATS          pInfo
                  Pointer of the configuration to be retrieved.

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgPortGetStats
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_IF_STATS          pStats
    )
{
    UNREFERENCED_PARAMETER(hContext);
#if defined _COSA_DRG_TPG_ || _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_
    PBRIDGE pBridge = getBridge(ulBrgInstanceNumber);
    if (pBridge == NULL) {
        CcspTraceInfo(("< %s > <pBridge == NULL>\n",__FUNCTION__));
        return ANSC_STATUS_CANT_FIND;
    }
    CcspTraceInfo(("------CosaDmlBrgPortGetStats, Bridge:%s...\n", pBridge->hwid));
    PBRIDGE_PORT pBPort = getBPort(pBridge, ulInstanceNumber);
    if (pBPort == NULL) {
        return ANSC_STATUS_CANT_FIND;
    }

    CcspTraceInfo(("------CosaDmlBrgPortGetStats, Port:%s...\n", pBPort->name));
    return pBPort->control->getStats(pBPort, pStats);
#endif
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrgVlanAddEntry
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulBrgInstanceNumber,
                int                         VLANID
            );

    Description:

        Backend implementation to add a ddns service entry.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulBrgInstanceNumber
                  Instance number of bridge

                  int                         VLANID
                  VLAN ID

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgVlanAddEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        PCOSA_DML_BRG_VLAN_FULL     pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
#if defined _COSA_DRG_TPG_ || _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_
    AnscTraceFlow(("------%s...\n", __func__));
    AnscTraceFlow(("<HL> %s\n",__FUNCTION__));
    PBRIDGE pBridge = getBridge(ulBrgInstanceNumber);
    if (pBridge == NULL) {
        return ANSC_STATUS_CANT_FIND;
    }
    return pBridge->control->addVlan(pBridge, &pEntry->Cfg);
#endif
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrgVlanDelEntry
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulBrgInstanceNumber,
                int                         VLANID
            );

    Description:

        Backend implementation to delete a bridge vlan entry.

    Arguments:    ANSC_HANDLE                hContext
                  Reserved.

                  ULONG                      ulBrgInstanceNumber
                  Instance number of the bridge

                  int                        VLANID
                  VLANID

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgVlanDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        ULONG                       ulVLANInsNum
    )
{
    UNREFERENCED_PARAMETER(hContext);
#if defined _COSA_DRG_TPG_ || _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_
    AnscTraceFlow(("------%s...\n", __func__));
    PBRIDGE pBridge = getBridge(ulBrgInstanceNumber);
    if (pBridge == NULL) {
        return ANSC_STATUS_CANT_FIND;
    }
    return pBridge->control->removeVlan(pBridge, ulVLANInsNum);
#endif
}

ULONG
CosaDmlBrgVlanGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber
    )
{
    UNREFERENCED_PARAMETER(hContext);
#if defined _COSA_DRG_TPG_ || _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_
//    CcspTraceInfo(("------CosaDmlBrgPortGetNumberOfEntries...\n"));
//    AnscTraceFlow(("<HL> %s\n",__FUNCTION__));
    PBRIDGE pBridge = getBridge(ulBrgInstanceNumber);
    if (pBridge == NULL) {
        AnscTraceFlow(("<HL> %s ulBrgInstanceNumber=%d Not found\n",__FUNCTION__,
            ulBrgInstanceNumber));
        return 0;
    }
    AnscTraceFlow(("<HL> %s ulBrgInstanceNumber=%d numOfVlans=%d\n",__FUNCTION__,
            ulBrgInstanceNumber,pBridge->numOfVlans));
    return (pBridge->numOfVlans);
#endif
}

ANSC_STATUS
CosaDmlBrgVlanSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    UNREFERENCED_PARAMETER(hContext);
#if defined _COSA_DRG_TPG_ || _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_
    PBRIDGE pBridge = getBridge(ulBrgInstanceNumber);
    CcspTraceInfo(("------CosaDmlBrgPortSetValues...\n"));
    AnscTraceFlow(("<HL> %s\n",__FUNCTION__));
    if (pBridge == NULL) {
        AnscTraceFlow(("<$HL> %s cannot find bridge inst =%d, \n",__FUNCTION__,
            ulBrgInstanceNumber));
        return ANSC_STATUS_CANT_FIND;
    }
    PBRIDGE_VLAN pVLAN = (PBRIDGE_VLAN) AnscSListGetEntryByIndex(&pBridge->vlanList, ulIndex);
    if (pVLAN == NULL) {
        AnscTraceFlow(("<$HL> %s cannot find bridge inst =%d, port index=%d\n",__FUNCTION__,
            ulBrgInstanceNumber,ulIndex));
        return ANSC_STATUS_CANT_FIND;
    }
    AnscCopyString(pVLAN->alias, pAlias);
    pVLAN->instanceNumber = ulInstanceNumber;
    CcspTraceInfo(("------CosaDmlBrgPortSetValues, We are trying to save instancenum %d:%s...\n", 
        pVLAN->instanceNumber, pVLAN->alias));
    _Psm_SetBVlan(ulBrgInstanceNumber, ulInstanceNumber, pVLAN); 
    return ANSC_STATUS_SUCCESS;
#endif
}

ANSC_STATUS CosaDmlBrgVlanGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_BRG_VLAN_FULL     pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
#if defined _COSA_DRG_TPG_ || _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_
//    CcspTraceInfo(("------CosaDmlBrgVlanGetEntry...\n"));
    PBRIDGE pBridge = getBridge(ulBrgInstanceNumber);
    AnscTraceFlow(("<HL> %s ulIndex=%d\n",__FUNCTION__,ulIndex));
    if (pBridge == NULL) {
        AnscTraceFlow(("<HL> %s Bridge not found ulBrgInstanceNumber=%d, ulIndex=%d\n",__FUNCTION__,
            ulBrgInstanceNumber,ulIndex));
        return ANSC_STATUS_CANT_FIND;
    }
    PBRIDGE_VLAN pBVlan = (PBRIDGE_VLAN) AnscSListGetEntryByIndex(&pBridge->vlanList, ulIndex);
    if (pBVlan == NULL) {
        AnscTraceFlow(("<HL> %s vlan not found ulBrgInstanceNumber=%d, ulIndex=%d\n",__FUNCTION__,
            ulBrgInstanceNumber,ulIndex));
        return ANSC_STATUS_CANT_FIND;
    }
    
    //$HL 4/13/2013
    //loadID(pBPort->name, &pBPort->instanceNumber, pBPort->alias);
    CcspTraceInfo(("------CosaDmlBrgPortGetEntry,instancenum:%d,alias\n", pBVlan->instanceNumber, pBVlan->alias));
    pEntry->Cfg.InstanceNumber = pBVlan->instanceNumber;
    //$HL 12/05/2013
    //pEntry->Cfg.bEnabled = TURE; 
    // Suport only one VLAN entry per Bridge
    // TODO: need to fix after bridge needs to support multi-VLANs
    pEntry->Cfg.bEnabled = pBridge->bEnabled; 
    pEntry->Cfg.VLANID = pBridge->vlanid;
    AnscCopyString(pEntry->Cfg.Alias, pBVlan->alias);
    AnscCopyString(pEntry->Info.Name, pBridge->name);
    return ANSC_STATUS_SUCCESS;
#endif
}   

ANSC_STATUS
CosaDmlBrgVlanSetCfg
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        PCOSA_DML_BRG_VLAN_CFG      pCfg
    )
{
    UNREFERENCED_PARAMETER(hContext);
#if defined _COSA_DRG_TPG_ || _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_
    AnscTraceFlow(("------%s...\n", __func__));
    PBRIDGE pBridge = getBridge(ulBrgInstanceNumber);
    PBRIDGE_VLAN curVlan = NULL;
    if (pBridge == NULL) {
        return ANSC_STATUS_CANT_FIND;
    }
    curVlan = getBVlan(pBridge, pCfg->InstanceNumber);
    AnscTraceFlow(("<HL> %s\n",__FUNCTION__));
    if (curVlan==NULL)
    {
        AnscTraceFlow(("<$HL> %s cannot find brInst =%d, bVlan inst=%d\n",__FUNCTION__,
            pBridge->l2InstanceNumber,pCfg->InstanceNumber));
        return ANSC_STATUS_CANT_FIND;
    }
    _COSA_UpdateBrVLANID(pBridge,pCfg->VLANID);
    AnscCopyString(curVlan->alias, pCfg->Alias);
    _Psm_SetBVlan(ulBrgInstanceNumber, curVlan->instanceNumber, curVlan);
    return ANSC_STATUS_SUCCESS;
#endif

}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrgVlanPortAddEntry
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulBrgInstanceNumber,
                int                         VLANID,
                ULONG                       ulPortInstanceNumber,
                BOOLEAN                     bUntagged
            );

    Description:

        Backend implementation to add a ddns service entry.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  ULONG                       ulBrgInstanceNumber
                  Instance number of bridge

                  int                         VLANID

                  ULONG                       ulPortInstanceNumber
                  Instance number of bridge port

                  BOOLEAN                     bUntagged
                  indicated egress packets will be untagged

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgVlanPortAddEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        int                         VLANID,
        ULONG                       ulPortInstanceNumber,
        BOOLEAN                     bUntagged
    )
{
    UNREFERENCED_PARAMETER(hContext);
#if defined _COSA_SIM_ 
    return ANSC_STATUS_CANT_FIND;
#endif
#if defined _COSA_DRG_TPG_ || _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_
    AnscTraceFlow(("------%s...\n", __func__));
    PBRIDGE pBridge = getBridge(ulBrgInstanceNumber);
    if (pBridge == NULL) {
        return ANSC_STATUS_CANT_FIND;
    }
    PBRIDGE_PORT pBPort = getBPort(pBridge, ulPortInstanceNumber);
    if (pBPort == NULL) {
        return ANSC_STATUS_CANT_FIND;
    }
    return pBPort->control->addToVlan(pBPort, VLANID, bUntagged);
#endif
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlBrgVlanPortDelEntry
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulBrgInstanceNumber,
                int                         VLANID,
                ULONG                       ulPortInstanceNumber,
            );

    Description:

        Backend implementation to delete a bridge vlan entry.

    Arguments:    ANSC_HANDLE                hContext
                  Reserved.

                  ULONG                      ulBrgInstanceNumber
                  Instance number of the bridge

                  ULONG                      ulInstanceNumber
                  Pointer of the bridge vlan to be deleted.

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlBrgVlanPortDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        int                         VLANID,
        ULONG                       ulPortInstanceNumber
    )
{
    UNREFERENCED_PARAMETER(hContext);
#if defined _COSA_SIM_ 
    return ANSC_STATUS_CANT_FIND;
#endif
#if defined _COSA_DRG_TPG_ || _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_
    AnscTraceFlow(("------%s...\n", __func__));
    PBRIDGE pBridge = getBridge(ulBrgInstanceNumber);
    if (pBridge == NULL) {
        return ANSC_STATUS_CANT_FIND;
    }
    PBRIDGE_PORT pBPort = getBPort(pBridge, ulPortInstanceNumber);
    if (pBPort == NULL) {
        return ANSC_STATUS_CANT_FIND;
    }
    return pBPort->control->removeFromVlan(pBPort, VLANID);
#endif
}

//Common functions to CNS and TPG
#if defined _COSA_DRG_TPG_ || _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_

///Maps a bridge instance number to a backend object
PBRIDGE getBridge(ULONG instanceNumber) 
{
//    AnscTraceFlow(("<HL> %s \n",__FUNCTION__));
    PBRIDGE curBridge = (PBRIDGE) AnscSListGetFirstEntry(&g_Bridgings.bridgeList);
    while (curBridge != NULL) {
        if (curBridge->instanceNumber == instanceNumber) {
            return curBridge;
        }
        curBridge = (PBRIDGE) curBridge->Linkage.Next;
    }
    //$HL 4/26/2013
    AnscTraceFlow(("<HL> %s bridge not found, instanceNumber=%d\n",__FUNCTION__, instanceNumber));
    return NULL;
}

///Maps a bridge port instance to a backend object
PBRIDGE_PORT getBPort(PBRIDGE pBridge, ULONG ulInstanceNumber)
{
    PBRIDGE_PORT curPort = (PBRIDGE_PORT) AnscSListGetFirstEntry(&pBridge->portList);
    while (curPort != NULL) {
        if (curPort->instanceNumber == ulInstanceNumber) {

            return curPort;
        }
        curPort = (PBRIDGE_PORT) curPort->Linkage.Next;
    }
    //$HL 4/26/2013
    AnscTraceFlow(("<HL> %s bport not found, bridge instanceNumber=%d port instanceNumber=%d\n",
        __FUNCTION__, pBridge->instanceNumber,ulInstanceNumber));
    return NULL;
}

PBRIDGE_VLAN getBVlan(PBRIDGE pBridge, ULONG ulInstanceNumber)
{
    PBRIDGE_VLAN curVlan = (PBRIDGE_VLAN) AnscSListGetFirstEntry(&pBridge->vlanList);
    AnscTraceFlow(("<HL> %s \n",__FUNCTION__));
    while (curVlan != NULL) {
        if (curVlan->instanceNumber == ulInstanceNumber) {

            return curVlan;
        }
        curVlan = (PBRIDGE_VLAN) curVlan->Linkage.Next;
    }
    //$HL 4/26/2013
    AnscTraceFlow(("<HL> %s bVlan not found, bridge instanceNumber=%d port instanceNumber=%d\n",
        __FUNCTION__, pBridge->instanceNumber,ulInstanceNumber));
    return NULL;
}

//$HL 4/10/2013 
static char *_COSA_GetInterfaceTypeStr(COSA_DML_BRG_LINK_TYPE linktype)
{
    AnscTraceFlow(("<HL> %s \n",__FUNCTION__));
    switch (linktype)
    {
        case COSA_DML_BRG_LINK_TYPE_Eth:
        case COSA_DML_BRG_LINK_TYPE_EthVlan:
            return("Ethernet");
        case COSA_DML_BRG_LINK_TYPE_Usb:
            return("Usb");
        case COSA_DML_BRG_LINK_TYPE_Hpna:
            return("Hpna");
        case COSA_DML_BRG_LINK_TYPE_Moca:
            return("Moca");
        //$HL 7/15/2013
        case COSA_DML_BRG_LINK_TYPE_Gre:
            return("Gre");
        case COSA_DML_BRG_LINK_TYPE_HomePlug:
            return("HomePlug");
        case COSA_DML_BRG_LINK_TYPE_Upa:
            return("Upa");
        case COSA_DML_BRG_LINK_TYPE_WiFiSsid:
            return("WiFi");
        case COSA_DML_BRG_LINK_TYPE_Bridge:
            return("Bridge");
        default:
            return("");
    }
}



static ANSC_STATUS _COSA_GetNextVlanId(PBRIDGE pBridge)
{
    PBRIDGE curBridge = NULL;
    BOOLEAN found = TRUE;
    int vid = g_brInfo.nxtVid, i;
    AnscTraceFlow(("<HL> %s vid=%d\n",__FUNCTION__,vid));
    for (i= 0; i <= g_Bridgings.bridgeList.Depth; i++)
    {
        if (found == TRUE)
        {
            curBridge = (PBRIDGE) AnscSListGetFirstEntry(&g_Bridgings.bridgeList);
            if ((ULONG)vid >= g_brInfo.vlanHighBound)
            {
                vid = g_brInfo.vlanLowBound;
            }
            AnscTraceFlow(("<HL>%s reset vid=%d\n",__FUNCTION__,vid));
            found = FALSE;
        }
        while (curBridge != NULL) 
        {
            if (curBridge->vlanid == vid) 
            {
                found = TRUE;
                vid ++;
                break;
            }
            curBridge = (PBRIDGE) curBridge->Linkage.Next;
        }
        if (found == FALSE)
        {
            pBridge->vlanid = vid;
            g_brInfo.nxtVid = vid + 1;
            AnscTraceFlow(("<HL> %s new vid=%d\n",__FUNCTION__,vid));
            return ANSC_STATUS_SUCCESS;  
        }
    }
    AnscTraceFlow(("<HL> %s Error no more VLAN resource vid=%d\n",__FUNCTION__,vid));
    return ANSC_STATUS_FAILURE; 
}

static ANSC_STATUS _COSA_GetNewBrName(PBRIDGE pBridge)
{
    PBRIDGE curBridge = NULL;
    BOOLEAN found = TRUE;
    char param_name[256] = {0};
    int brId = g_brInfo.nxtBrId, i;
    errno_t rc = -1;
    AnscTraceFlow(("<HL> %s \n",__FUNCTION__));
    for (i=0; i <= g_Bridgings.bridgeList.Depth  ; i++)
    {
        if (found == TRUE)
        {
            curBridge = (PBRIDGE) AnscSListGetFirstEntry(&g_Bridgings.bridgeList);
            rc = sprintf_s(param_name, sizeof(param_name), g_brInfo.bridgeNamePrefix,brId);
            if(rc < EOK)
            {
                ERR_CHK(rc);
            }
            found = FALSE;
        }
        AnscTraceFlow(("<HL> %s bridge_name=%s \n",__FUNCTION__,param_name));
        while (curBridge != NULL) 
        {
            if (_ansc_strstr(curBridge->name, param_name))
            {
                found = TRUE;
                brId++;
                break;
            }
            curBridge = (PBRIDGE) curBridge->Linkage.Next;
        }
        if (found == FALSE)
        {
            g_brInfo.nxtBrId = brId + 1;
            AnscCopyString(pBridge->name,param_name);
            return ANSC_STATUS_SUCCESS;   
        }
    }
    return ANSC_STATUS_FAILURE;
}


static ANSC_STATUS _COSA_NewL2InstNumber(PBRIDGE pBridge)
{
    pBridge->l2InstanceNumber = pBridge->instanceNumber;
    return ANSC_STATUS_SUCCESS;
}

static ANSC_STATUS _COSA_UpdateBrVLANID(PBRIDGE pBridge, int vlanid)
{
    if (vlanid != pBridge->vlanid)
    {
        pBridge->vlanid = vlanid; 
        _Psm_SetBr(pBridge->l2InstanceNumber,pBridge);
        v_secure_system("sysevent set multinet-syncMembers %lu", pBridge->l2InstanceNumber);
    }
    return ANSC_STATUS_SUCCESS;
}

static ANSC_STATUS _Psm_AddLinkMembers(ULONG instanceNum)
{
    int retPsmGet = CCSP_SUCCESS;
    char param_name[256]={0};
    errno_t rc = -1;
    rc = sprintf_s(param_name, sizeof(param_name), g_brTemple[_PSM_BRIDGE_TML_LINK_MEMBERS], instanceNum);
    if(rc < EOK)
    {
        ERR_CHK(rc);
        return ANSC_STATUS_FAILURE;
    }
    AnscTraceFlow(("<HL> %s param_name=%s\n",__FUNCTION__,param_name));
    retPsmGet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, param_name, ccsp_string, DMSB_HIDEN_LAN_INF);
    if (retPsmGet == CCSP_SUCCESS) {
        AnscTraceFlow(("%s: retPsmGet == CCSP_SUCCESS reading %s\n", __FUNCTION__,param_name));
    }
    else 
    {
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

static ANSC_STATUS _Psm_DelLinkMembers(ULONG instanceNum)
{
    char param_name[256]={0};
    errno_t rc = -1;
    rc = sprintf_s(param_name, sizeof(param_name), g_brTemple[_PSM_BRIDGE_TML_LINK_MEMBERS], instanceNum);
    if(rc < EOK)
    {
        ERR_CHK(rc);
        return ANSC_STATUS_FAILURE;
    }
    AnscTraceFlow(("<HL> %s param_name=%s\n",__FUNCTION__,param_name));
    PSM_Del_Record(bus_handle, g_Subsystem, param_name);
    return ANSC_STATUS_SUCCESS;
}

//removeBridge()
static ANSC_STATUS _COSA_DelBr(ULONG brgInstNum) {
    PBRIDGE curBridge = NULL;
    AnscTraceFlow(("<HL> %s \n",__FUNCTION__));
    curBridge = getBridge(brgInstNum);
    if (curBridge==NULL)
    {
        AnscTraceFlow(("<HL> %s brgInstNum not found:%d \n",__FUNCTION__,brgInstNum));
        return ANSC_STATUS_CANT_FIND;
    }
    if (curBridge->numOfPorts!=0)
    {
        AnscTraceFlow(("<HL> %s brgInstNum:%d numOfPorts=%d\n",__FUNCTION__,
            brgInstNum,curBridge->numOfPorts));
        return ANSC_STATUS_CANT_FIND;
    }

    AnscSListPopEntryByLink(&g_Bridgings.bridgeList,&curBridge->Linkage);
    _Psm_DelBr(curBridge);
    //free memory 
    AnscFreeMemory(curBridge);
    return ANSC_STATUS_SUCCESS;
}

//$HL 4/1/2013 
///remove a bridge port instance 
static ANSC_STATUS _COSA_DelBPort(ULONG bportInstNum, PBRIDGE pBridge) {
    PBRIDGE_PORT curPort = getBPort(pBridge, bportInstNum);
    char param_name[256] = {0},param_value[256]={0};
    char *deviceStr = NULL;
    ULONG l2InstNum = pBridge->l2InstanceNumber;
    BOOLEAN find = FALSE;
    AnscTraceFlow(("<HL> %s\n",__FUNCTION__));
    if (curPort==NULL)
    {
        AnscTraceFlow(("<$HL> %s cannot find brInst =%d, bport inst=%d\n",__FUNCTION__,
            pBridge->l2InstanceNumber,bportInstNum));
        return ANSC_STATUS_CANT_FIND;
    }
    if (!curPort->bAllowDelete)
    {
        AnscTraceFlow(("<$HL> %s Not allow to delete brInst =%d, bport inst=%d\n",__FUNCTION__,
            pBridge->l2InstanceNumber,bportInstNum));
        return ANSC_STATUS_NOT_SUPPORTED;
    }
    AnscSListPopEntryByLink(&pBridge->portList,&curPort->Linkage);

    AnscTraceFlow(("%s Delete CurPort instanceNumber=%d, linkName=%s\n",
         __FUNCTION__, curPort->instanceNumber, curPort->linkName));

    //if (curPort->linkType != COSA_DML_BRG_LINK_TYPE_NONE)
    {
        //deviceStr = curPort->linkName;
        _PSM_DEL_BPORT(_PSM_BPORT_TML_PVID);
        _PSM_DEL_BPORT(_PSM_BPORT_TML_PRITAG);
        _PSM_DEL_BPORT(_PSM_BPORT_TML_ENABLE);
    }

    deviceStr = NULL;
    _ansc_memset(param_value, 0, sizeof(param_value)); 

    _PSM_DEL_BPORT(_PSM_BPORT_TML_ALIAS);
    _PSM_DEL_BPORT(_PSM_BPORT_TML_NAME);
    _PSM_DEL_BPORT(_PSM_BPORT_TML_LINKNAME);
    _PSM_DEL_BPORT(_PSM_BPORT_TML_LINKTYPE);
    _PSM_DEL_BPORT(_PSM_BPORT_TML_MGT);
    _PSM_DEL_BPORT(_PSM_BPORT_TML_MODE);
    _PSM_DEL_BPORT(_PSM_BPORT_TML_INSTNUM);
    _PSM_DEL_BPORT(_PSM_BPORT_TML_UPSTREAM);
    _PSM_DEL_BPORT(_PSM_BPORT_TML_ALLOWDELETE);

    // remove member from the list
    _PSM_REMOVE_MEMBERS(pBridge, curPort, param_name, param_value);

    if (pBridge->numOfPorts)
        pBridge->numOfPorts --;

    //free memory 
    AnscFreeMemory(curPort);

    curPort = (PBRIDGE_PORT) AnscSListGetFirstEntry(&pBridge->portList);
    while (curPort != NULL) {
        if (!curPort->bMgt && !curPort->bUpstream && 
            AnscSizeOfString(curPort->linkName) && curPort->bEnabled) {
            find = TRUE;
            break;
        }
        curPort = (PBRIDGE_PORT) curPort->Linkage.Next;
    }
    if (find) 
    {
        //add hide local interface
        pBridge->bUpstream = FALSE;
        _Psm_AddLinkMembers(pBridge->l2InstanceNumber);
    }
    else 
    {
        pBridge->bUpstream = TRUE;
        //remove link member
        _Psm_DelLinkMembers(pBridge->l2InstanceNumber);
    }
    return ANSC_STATUS_SUCCESS;
}

static ANSC_STATUS _COSA_DelBVlan(ULONG bvlanInstNum, PBRIDGE pBridge) {
    PBRIDGE_VLAN curVlan = getBVlan(pBridge, bvlanInstNum);
    char param_name[256] = {0},param_value[256]={0};
    ULONG l2InstNum = pBridge->l2InstanceNumber;
    AnscTraceFlow(("<HL> %s\n",__FUNCTION__));
    if (curVlan==NULL)
    {
        AnscTraceFlow(("<$HL> %s cannot find brInst =%d, bport inst=%d\n",__FUNCTION__,
            pBridge->l2InstanceNumber,bvlanInstNum));
        return ANSC_STATUS_CANT_FIND;
    }

    AnscSListPopEntryByLink(&pBridge->vlanList,&curVlan->Linkage);

    AnscTraceFlow(("%s Delete curVlan instanceNumber=%d\n",
         __FUNCTION__, curVlan->instanceNumber));

    _ansc_memset(param_value, 0, sizeof(param_value)); 

    _PSM_DEL_BVLAN(_PSM_BVLAN_TML_ALIAS);
    _PSM_DEL_BVLAN(_PSM_BVLAN_TML_INSTNUM);

    if (pBridge->numOfVlans)
        pBridge->numOfVlans --;

    //free memory 
    AnscFreeMemory(curVlan);
    return ANSC_STATUS_SUCCESS;
}

static ANSC_STATUS _COSA_AddToken(char *token, char *pStr)
{
    char *result = NULL;
    char buf[256]={0};
    char *st = NULL;
    AnscCopyString(buf,pStr);
    if (AnscSizeOfString(buf)) 
    {
        AnscTraceFlow(("<HL> %s str=%s token=%s\n",__FUNCTION__,buf,token));

        result = strtok_r(buf,DMSB_DELIM, &st);
        while( result != NULL ) 
        {
            if (!strcasecmp(result,token)) 
            {
                AnscTraceFlow(("<HL>%s found token\n",__FUNCTION__ ));
                return ANSC_STATUS_FAILURE;
            }
            result = strtok_r(NULL,DMSB_DELIM, &st);
        }
        AnscTraceFlow(("<HL> %s Str=%s not found:%s\n",__FUNCTION__,buf,token));
        strcat(pStr,DMSB_DELIM);
    }
    strcat(pStr,token);
    AnscTraceFlow(("<HL> %s new Str=%s after add:%s\n",__FUNCTION__,pStr,token));
    return ANSC_STATUS_SUCCESS;
}

static ANSC_STATUS _COSA_DelToken(char *token, char *pStr)
{
    char *result = NULL;
    char buf[256] = {0};
    char *pBuf = &buf[0];
    int find = 0;
    char *st = NULL;
    if (pStr == NULL)
        return ANSC_STATUS_FAILURE;
    AnscTraceFlow(("<HL> %s str=%s token=%s\n",__FUNCTION__,pStr,token));

    result = strtok_r( pStr, DMSB_DELIM, &st);
    while( result != NULL ) 
    {
        if (!strcasecmp(result,token)) 
        {
            find = 1;
            AnscTraceFlow(("<HL>%s found token\n",__FUNCTION__ ));
        }
        else 
        {
            strcat(pBuf, DMSB_DELIM);
            strcat(pBuf,result);
        }
        result = strtok_r(NULL, DMSB_DELIM, &st);
    }

    if (find == 1) 
    {
        AnscTraceFlow(("<HL> %s old str=%s new Str=%s after remove:%s\n",__FUNCTION__,pStr,pBuf,token));
        _ansc_memset(pStr,0,sizeof(*pStr));
        AnscCopyString(pStr,pBuf);
        return ANSC_STATUS_SUCCESS;
    }
    AnscTraceFlow(("<HL> %s Not Found token in Buffer=%s\n",__FUNCTION__,pBuf));
    return ANSC_STATUS_FAILURE;
}

static ANSC_STATUS _Psm_GetDefaultSetting()
{
    int retPsmGet = CCSP_SUCCESS;
    char* param_value= NULL;
    ULONG data1=0, data2=0;
    AnscTraceFlow(("<HL> %s \n",__FUNCTION__));
    retPsmGet = PSM_Get_Record_Value2(bus_handle, g_Subsystem, "dmsb.bridgeVlanRange", NULL, &param_value);
    if (retPsmGet == CCSP_SUCCESS) 
    {
        AnscTraceFlow(("%s: retPsmGet == CCSP_SUCCESS reading dmsb.bridgeVlanRange\n", __FUNCTION__));

        if (_ansc_sscanf(param_value, "%lu...%lu", &data1, &data2)==2 &&
            (data2 > 0) )
        {
            g_brInfo.vlanLowBound = data1;
            g_brInfo.vlanHighBound = data2;
        }
        else 
        {
            g_brInfo.vlanLowBound = DMSB_VLAN_LOWBOUND;
            g_brInfo.vlanHighBound = DMSB_VLAN_HIGHBOUND;
        }
        AnscTraceFlow(("<HL>%s vlanLowBound=%d vlanHighBound=%d\n", __FUNCTION__,
            g_brInfo.vlanLowBound, g_brInfo.vlanHighBound));
        //TODO for br Name Index and L2 Instance Range(remapping)

        ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(param_value);
    }
    else 
    {
        g_brInfo.vlanLowBound = DMSB_VLAN_LOWBOUND;
        g_brInfo.vlanHighBound = DMSB_VLAN_HIGHBOUND;
    }
    g_brInfo.nxtVid = g_brInfo.vlanLowBound ;
    g_brInfo.nxtBrId = DMSB_BRID_LOWBOUND ; 
    retPsmGet = PSM_Get_Record_Value2(bus_handle, g_Subsystem, "dmsb.BridgeNamePrefix", NULL, &param_value);
    if (retPsmGet == CCSP_SUCCESS) 
    {
        AnscTraceFlow(("%s: retPsmGet == CCSP_SUCCESS reading dmsb.BridgeNamePrefix\n", __FUNCTION__));
        AnscCopyString(g_brInfo.bridgeNamePrefix,param_value);
        strcat(g_brInfo.bridgeNamePrefix,"%d");
        ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(param_value);
    }
    else 
    {
        AnscCopyString(g_brInfo.bridgeNamePrefix,DMSB_BRG_NAME_PREFIX);
    }
    AnscTraceFlow(("<HL>%s bridgeNamePrefix=%s\n", __FUNCTION__,
            g_brInfo.bridgeNamePrefix));
    return ANSC_STATUS_SUCCESS;
}


static ANSC_STATUS _Psm_GetBPortMemberList(char *param_name, char *list)
{
    int retPsmGet = CCSP_SUCCESS;
    char *param_value= NULL;
    AnscTraceFlow(("<HL> %s path=%s value=%s\n",__FUNCTION__,param_name,param_value));
    retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, param_name, NULL, &param_value);
    if (retPsmGet == CCSP_SUCCESS) {
        if (param_value!=NULL)
            AnscCopyString(list, param_value); 
        else
            list[0]='\0';        
        AnscTraceFlow(("%s: retPsmGet == CCSP_SUCCESS reading %s=%s\n", __FUNCTION__,param_name,list));
        ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(param_value);
    }
    else 
    {
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

static ANSC_STATUS _Psm_SetBPortMemberList(char *param_name, char *param_value)
{
    int retPsmGet = CCSP_SUCCESS;
    AnscTraceFlow(("<HL> %s path=%s value=%s\n",__FUNCTION__,param_name,param_value));
    retPsmGet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, param_name, ccsp_string, param_value);
    if (retPsmGet == CCSP_SUCCESS) {
        AnscTraceFlow(("%s: retPsmSet == CCSP_SUCCESS writing %s=%s\n", __FUNCTION__,param_name,param_value));
    }
    else 
    {
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}

static ANSC_STATUS  _COSA_AddBPortMember(char *path, char *name)
{
    char list[256] ={0};
    AnscTraceFlow(("<HL> %s path=%s name=%s\n",__FUNCTION__,path,name));
    _Psm_GetBPortMemberList(path,list);
    if (_COSA_AddToken(name,list)== ANSC_STATUS_SUCCESS)
    {
        _Psm_SetBPortMemberList(path,list);
        AnscTraceFlow(("<HL> %s new path=%s name=%s\n",__FUNCTION__,path,list));
    }
    else
        return ANSC_STATUS_FAILURE;
    return ANSC_STATUS_SUCCESS;
}

static ANSC_STATUS  _COSA_RemoveBPortMember(char *path, char *name)
{
    char list[256] ={0};
    AnscTraceFlow(("<HL> %s path=%s name=%s\n",__FUNCTION__,path,name));
    _Psm_GetBPortMemberList(path,list);
    if (_COSA_DelToken(name,list)== ANSC_STATUS_SUCCESS) 
    {
        _Psm_SetBPortMemberList(path,list);
        AnscTraceFlow(("<HL> %s new path=%s name=%s\n",__FUNCTION__,path,list));

    }
    else
        return ANSC_STATUS_FAILURE;
    return ANSC_STATUS_SUCCESS;
}

static int PsmGroupGet_s(const char *names[], int nname, 
        parameterValStruct_t ***records, int *nrec)
{
    return PsmGroupGet(bus_handle, g_Subsystem, names, nname, records, nrec);
}

static void PsmFreeRecords_s(parameterValStruct_t **records, int nrec)
{
    PsmFreeRecords(bus_handle, records, nrec);
}

static ANSC_STATUS _Psm_Cleanup(int instanceNumber)
{
    unsigned int i = 0;
    char param_name[256] = {0};
    int retPsmGet = CCSP_SUCCESS;
    unsigned int portCnt = 0,vlanCnt=0;
    unsigned int *portList = NULL,*vlanList = NULL;
    ULONG l2InstNum = instanceNumber;
    ULONG bportInstNum = 0, bvlanInstNum=0;
    errno_t rc = -1;
    char * deviceStr = NULL;
    AnscTraceFlow(("<HL> %s \n",__FUNCTION__));
    while (g_brTemple[i]!=NULL) 
    {
        rc = sprintf_s(param_name, sizeof(param_name), g_brTemple[i], instanceNumber);
        if(rc < EOK)
        {
            ERR_CHK(rc);
            i++;
            continue;
        }
        AnscTraceFlow(("%s delete brigde inst=%d param_name=%s\n", __FUNCTION__,
            instanceNumber,param_name));
        PSM_Del_Record(bus_handle, g_Subsystem, param_name);
        _ansc_memset(param_name, 0, sizeof(param_name));
        i++;
    }
    _ansc_memset(param_name, 0, sizeof(param_name));
    i = 0;
    rc = sprintf_s(param_name, sizeof(param_name), DMSB_L2_BPORT_PREFIX, instanceNumber);
    if(rc < EOK)
    {
        ERR_CHK(rc);
        return ANSC_STATUS_FAILURE;
    }
    retPsmGet = PsmGetNextLevelInstances(bus_handle, g_Subsystem, param_name, &portCnt, &portList);
    if ( retPsmGet == CCSP_SUCCESS && portList != NULL ) 
    {
        AnscTraceFlow(("<HL> %s Port Init: count=%d\n",__FUNCTION__,portCnt));
        for( i=0; i < portCnt; i++) 
        {
            bportInstNum = portList[i];
            AnscTraceFlow(("<HL> %s Port Init: count=%d bportInstNum=%d\n",
                __FUNCTION__,portCnt,bportInstNum));
            
            _PSM_DEL_BPORT(_PSM_BPORT_TML_PVID);
            _PSM_DEL_BPORT(_PSM_BPORT_TML_PRITAG);
            _PSM_DEL_BPORT(_PSM_BPORT_TML_ENABLE);

            //deviceStr = NULL;

            _PSM_DEL_BPORT(_PSM_BPORT_TML_ALIAS);
            _PSM_DEL_BPORT(_PSM_BPORT_TML_NAME);
            _PSM_DEL_BPORT(_PSM_BPORT_TML_LINKNAME);
            _PSM_DEL_BPORT(_PSM_BPORT_TML_LINKTYPE);
            _PSM_DEL_BPORT(_PSM_BPORT_TML_MGT);
            _PSM_DEL_BPORT(_PSM_BPORT_TML_MODE);
            _PSM_DEL_BPORT(_PSM_BPORT_TML_INSTNUM);
            _PSM_DEL_BPORT(_PSM_BPORT_TML_UPSTREAM);
            
        }
        ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(portList);
    }
    else 
    {
        AnscTraceFlow(("<HL> %s Cannot find any port entry:%s\n",__FUNCTION__, param_name));
    }
    _ansc_memset(param_name, 0, sizeof(param_name));
    i = 0;
    rc = sprintf_s(param_name, sizeof(param_name), DMSB_L2_BVLAN_PREFIX, instanceNumber);
    if(rc < EOK)
    {
        ERR_CHK(rc);
        return ANSC_STATUS_FAILURE;
    }
    retPsmGet = PsmGetNextLevelInstances(bus_handle, g_Subsystem, param_name, &vlanCnt, &vlanList);
    if ( retPsmGet == CCSP_SUCCESS && vlanList != NULL ) 
    {
        AnscTraceFlow(("<HL> %s Vlan Init: count=%d\n",__FUNCTION__,vlanCnt));
        for( i=0; i < vlanCnt; i++) 
        {
            bvlanInstNum = vlanList[i];
            AnscTraceFlow(("<HL> %s Vlan Init: count=%d bvlanInstNum=%d\n",
                __FUNCTION__,vlanCnt,bvlanInstNum));
            
            _PSM_DEL_BVLAN(_PSM_BVLAN_TML_ALIAS);
            _PSM_DEL_BVLAN(_PSM_BVLAN_TML_INSTNUM);
            
        }
        ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(vlanList);
    }
    else 
    {
        AnscTraceFlow(("<HL> %s Cannot find any vlan entry:%s\n",__FUNCTION__, param_name));
    }
    return ANSC_STATUS_SUCCESS;
}

static ANSC_STATUS _Psm_DelBr(PBRIDGE pBridge)
{
    int i = 0;
    char param_name[256] = {0};
    errno_t rc = -1;

    AnscTraceFlow(("<HL> %s \n",__FUNCTION__));
    while (g_brTemple[i]!=NULL) 
    {
        rc = sprintf_s(param_name, sizeof(param_name), g_brTemple[i], pBridge->l2InstanceNumber);
        if(rc < EOK)
        {
            ERR_CHK(rc);
            i++;
            continue;
        }
        AnscTraceFlow(("%s delete brigde inst=%d param_name=%s\n", __FUNCTION__,
            pBridge->l2InstanceNumber,param_name));
        PSM_Del_Record(bus_handle, g_Subsystem, param_name);
        _ansc_memset(param_name, 0, sizeof(param_name));
        i++;
    }
    return ANSC_STATUS_SUCCESS;
}

static ANSC_STATUS _Psm_GetBr(ULONG insNum, PBRIDGE pBridge)
{
    static const char *brTemps[] = {
        DMSB_L2_INST_PREFIX ".AllowDelete",
        DMSB_L2_INST_PREFIX ".Name",
        DMSB_L2_INST_PREFIX ".Vid",
        DMSB_L2_INST_PREFIX ".PriorityTag",
        DMSB_L2_INST_PREFIX ".Enable",
        DMSB_L2_INST_PREFIX ".Alias",
        DMSB_L2_INST_PREFIX ".Standard",
        DMSB_L2_INST_PREFIX ".InstanceNum",
        DMSB_L2_INST_PREFIX ".Type",
    };
    char nameArr[NELEMS(brTemps)][256];
    const char *names[NELEMS(brTemps)];
    parameterValStruct_t **records;
    int i, recordCnt, brId;

    for (i = 0; i < (int)NELEMS(names); i++)
    {
        snprintf(nameArr[i], sizeof(nameArr[i]), brTemps[i], insNum);
        names[i] = (char*)&nameArr[i];
    }

    if (PsmGroupGet_s(names, NELEMS(names), &records, &recordCnt) != CCSP_SUCCESS)
        return ANSC_STATUS_FAILURE;

    for (i = 0; i < recordCnt; i++)
    {
        //fprintf(stderr, "%s: [%d] %s %s\n", __FUNCTION__, i, records[i]->parameterName, records[i]->parameterValue);
        /* if PsmGroupGet_s() make sure @records returned are consistent 
         * with @names in sequence we can use index directly */
        if (strstr(records[i]->parameterName, ".AllowDelete"))
            pBridge->bAllowDelete = (strcasecmp(records[i]->parameterValue, "TRUE") == 0);
        else if (strstr(records[i]->parameterName, ".Alias"))
            snprintf(pBridge->alias, sizeof(pBridge->alias), "%s", records[i]->parameterValue);
        else if (strstr(records[i]->parameterName, ".Name"))
            snprintf(pBridge->name, sizeof(pBridge->name), "%s", records[i]->parameterValue);
        else if (strstr(records[i]->parameterName, ".Enable"))
            pBridge->bEnabled = (strcasecmp(records[i]->parameterValue, "TRUE") == 0);
        else if (strstr(records[i]->parameterName, ".Standard"))
        {
            if (strcmp(records[i]->parameterValue, "BRG_STD_8021Q_2005") == 0)
                pBridge->standard = COSA_DML_BRG_STD_8021Q_2005;
            else
                pBridge->standard = COSA_DML_BRG_STD_8021D_2004;
        }
        else if (strstr(records[i]->parameterName, ".Vid"))
            pBridge->vlanid = atoi(records[i]->parameterValue);
        else if (strstr(records[i]->parameterName, ".PriorityTag"))
            pBridge->bPriTag = (strcasecmp(records[i]->parameterValue, "TRUE") == 0);
        else if (strstr(records[i]->parameterName, ".InstanceNum"))
            pBridge->instanceNumber = atol(records[i]->parameterValue);
        else if (strstr(records[i]->parameterName, ".Type"))
            snprintf(pBridge->type, sizeof(pBridge->type), "%s", records[i]->parameterValue);
    }

    pBridge->l2InstanceNumber = insNum;
    pBridge->numOfPorts = 0;
    pBridge->hwid = pBridge->name;
    pBridge->control = &g_SWBrCtl;
    if (sscanf(pBridge->name, g_brInfo.bridgeNamePrefix, &brId) == 1)
    {
        if (brId >= g_brInfo.nxtBrId)
            g_brInfo.nxtBrId = brId + 1;
    }
    //fprintf(stderr, "g_brInfo.nxtBrId = %d\n", g_brInfo.nxtBrId);

    PsmFreeRecords_s(records, recordCnt);

    /*
    fprintf(stderr, "====  pBridge ====\n");
    fprintf(stderr, "alias: %s\n", pBridge->alias);
    fprintf(stderr, "instanceNumber: %lu\n", pBridge->instanceNumber);
    fprintf(stderr, "vlanid: %d\n", pBridge->vlanid);
    fprintf(stderr, "type: %s\n", pBridge->type);
    fprintf(stderr, "l2InstanceNumber: %lu\n", pBridge->l2InstanceNumber);
    fprintf(stderr, "bPriTag: %d\n", pBridge->bPriTag);
    fprintf(stderr, "bUpstream: %d\n", pBridge->bUpstream);
    fprintf(stderr, "bAllowDelete: %d\n", pBridge->bAllowDelete);
    fprintf(stderr, "bEnabled: %d\n", pBridge->bEnabled);
    fprintf(stderr, "name: %s\n", pBridge->name);
    fprintf(stderr, "standard: %d\n", pBridge->standard);
    */
    return ANSC_STATUS_SUCCESS;
}

static ANSC_STATUS _Psm_SetBr(ULONG instancenum,PBRIDGE pBridge)
{
    int retPsmGet = CCSP_SUCCESS;
    char param_value[256]={0};
    char param_name[256]= {0};
    errno_t rc = -1;

    fprintf(stderr, "!!!!!!!!!! _Psm_SetBr !!!!!!!!!!!!!!!!!\n");

    AnscTraceFlow(("<HL> %s \n",__FUNCTION__));

    rc = strcpy_s(param_value, sizeof(param_value), ((pBridge->bAllowDelete==TRUE) ? "TRUE" : "FALSE"));
    ERR_CHK(rc);
    _PSM_SET_BR(_PSM_BRIDGE_TML_ALLOWDELETE);

    rc = strcpy_s(param_value, sizeof(param_value), pBridge->alias);
    ERR_CHK(rc);
    _PSM_SET_BR(_PSM_BRIDGE_TML_ALIAS);

    rc = strcpy_s(param_value, sizeof(param_value), pBridge->name);
    ERR_CHK(rc);
    _PSM_SET_BR(_PSM_BRIDGE_TML_NAME);
    
    rc = strcpy_s(param_value, sizeof(param_value), ((pBridge->bEnabled==TRUE) ? "TRUE" : "FALSE"));
    ERR_CHK(rc);
    _PSM_SET_BR(_PSM_BRIDGE_TML_ENABLE);
    
    rc = strcpy_s(param_value, sizeof(param_value), pBridge->type);
    ERR_CHK(rc);
    _PSM_SET_BR(_PSM_BRIDGE_TML_TYPE);
 
    if (pBridge->standard == COSA_DML_BRG_STD_8021Q_2005)
    {
        rc = strcpy_s(param_value, sizeof(param_value), "BRG_STD_8021Q_2005");
        ERR_CHK(rc);
    }
    else if (pBridge->standard == COSA_DML_BRG_STD_8021D_2004)
    {
        rc = strcpy_s(param_value, sizeof(param_value), "BRG_STD_8021D_2004");
        ERR_CHK(rc);
    }
    else 
    {
        rc = strcpy_s(param_value, sizeof(param_value), "UNKNOWN");
        ERR_CHK(rc);
    }
    _PSM_SET_BR(_PSM_BRIDGE_TML_STANDARD);

    rc = sprintf_s(param_value, sizeof(param_value), "%d", pBridge->vlanid);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }
    _PSM_SET_BR(_PSM_BRIDGE_TML_VLANID);
 
    rc = strcpy_s(param_value, sizeof(param_value), ((pBridge->bPriTag==TRUE) ? "TRUE" : "FALSE"));
    ERR_CHK(rc);
    _PSM_SET_BR(_PSM_BRIDGE_TML_PRITAG);
 
    rc = sprintf_s(param_value, sizeof(param_value), "%lu",pBridge->instanceNumber);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }
    _PSM_SET_BR(_PSM_BRIDGE_TML_INSTNUM);

    if (pBridge->numOfPorts==0)
    {
        AnscCopyString(param_value," ");
        _PSM_SET_BR(_PSM_BRIDGE_TML_SW_MEMBERS);
        AnscCopyString(param_value," ");
        _PSM_SET_BR(_PSM_BRIDGE_TML_ETH_MEMBERS);
        AnscCopyString(param_value," ");
        _PSM_SET_BR(_PSM_BRIDGE_TML_MOCA_MEMBERS);
        AnscCopyString(param_value," ");
        //$HL 07/15/2013
        _PSM_SET_BR(_PSM_BRIDGE_TML_GRE_MEMBERS);
        AnscCopyString(param_value," ");
        _PSM_SET_BR(_PSM_BRIDGE_TML_WIFI_MEMBERS);
        AnscCopyString(param_value," ");
        _PSM_SET_BR(_PSM_BRIDGE_TML_LINK_MEMBERS);
    }
    return ANSC_STATUS_SUCCESS;

_Psm_SetBr_Err:
    AnscTraceFlow(("%s: cannot set %s\n",__FUNCTION__,param_name));
    return ANSC_STATUS_FAILURE;
}

static ANSC_STATUS _Psm_GetBPort(ULONG l2InstNum, ULONG bportInstNum, PBRIDGE_PORT pBPort)
{
    static const char *bportTemps[] = {
        DMSB_L2_BPORT_INST_PREFIX ".AllowDelete",
        DMSB_L2_BPORT_INST_PREFIX ".Alias",
        DMSB_L2_BPORT_INST_PREFIX ".Name",
        DMSB_L2_BPORT_INST_PREFIX ".LinkName",
        DMSB_L2_BPORT_INST_PREFIX ".LinkType",
        DMSB_L2_BPORT_INST_PREFIX ".Management",
        DMSB_L2_BPORT_INST_PREFIX ".Mode",
        DMSB_L2_BPORT_INST_PREFIX ".InstanceNum",
        DMSB_L2_BPORT_INST_PREFIX ".Upstream",
        DMSB_L2_BPORT_INST_PREFIX ".Enable",
        DMSB_L2_BPORT_INST_PREFIX ".Pvid",
        DMSB_L2_BPORT_INST_PREFIX ".PriorityTag",
    };
    char nameArr[NELEMS(bportTemps)][256];
    const char *names[NELEMS(bportTemps)];
    parameterValStruct_t **records;
    int i, recordCnt;

    for (i = 0; i < (int)NELEMS(names); i++)
    {
        snprintf(nameArr[i], sizeof(nameArr[i]), bportTemps[i], l2InstNum, bportInstNum);
        names[i] = (char*)&nameArr[i];
    }

    if (PsmGroupGet_s(names, NELEMS(names), &records, &recordCnt) != CCSP_SUCCESS)
        return ANSC_STATUS_FAILURE;

    for (i = 0; i < recordCnt; i++)
    {
        //fprintf(stderr, "%s: [%d] %s %s\n", __FUNCTION__, i, records[i]->parameterName, records[i]->parameterValue);
        /* if PsmGroupGet_s() make sure @records returned are consistent 
         * with @names in sequence we can use index directly */
        if (strstr(records[i]->parameterName, ".AllowDelete"))
            pBPort->bAllowDelete = (strcasecmp(records[i]->parameterValue, "TRUE") == 0);
        else if (strstr(records[i]->parameterName, ".Alias"))
            snprintf(pBPort->alias, sizeof(pBPort->alias), "%s", records[i]->parameterValue);
        else if (strstr(records[i]->parameterName, ".Name"))
            snprintf(pBPort->name, sizeof(pBPort->name), "%s", records[i]->parameterValue);
        else if (strstr(records[i]->parameterName, ".LinkName"))
            snprintf(pBPort->linkName, sizeof(pBPort->linkName), "%s", records[i]->parameterValue);
        else if (strstr(records[i]->parameterName, ".LinkType"))
        {
            if (strcmp(records[i]->parameterValue, "Ethernet") == 0)
                pBPort->linkType = COSA_DML_BRG_LINK_TYPE_Eth;
            else if (strcmp(records[i]->parameterValue, "Moca") == 0)
                pBPort->linkType = COSA_DML_BRG_LINK_TYPE_Moca;
            else if (strcmp(records[i]->parameterValue, "Gre") == 0)
                pBPort->linkType = COSA_DML_BRG_LINK_TYPE_Gre;
            else if (strcmp(records[i]->parameterValue, "WiFi") == 0)
                pBPort->linkType = COSA_DML_BRG_LINK_TYPE_WiFiSsid;
            else if (strcmp(records[i]->parameterValue, "Bridge") == 0)
                pBPort->linkType = COSA_DML_BRG_LINK_TYPE_Bridge;
            else
                pBPort->linkType = COSA_DML_BRG_LINK_TYPE_NONE;
        }
        else if (strstr(records[i]->parameterName, ".Management"))
            pBPort->bMgt = (strcasecmp(records[i]->parameterValue, "TRUE") == 0);
        else if (strstr(records[i]->parameterName, ".Mode"))
        {
            if (strcmp(records[i]->parameterValue, "PassThrough") == 0)
                pBPort->mode = COSA_DML_BPORT_PASSTHRU;
            else
                pBPort->mode = COSA_DML_BPORT_TAGGING;
        }
        else if (strstr(records[i]->parameterName, ".InstanceNum"))
            pBPort->instanceNumber = atol(records[i]->parameterValue);
        else if (strstr(records[i]->parameterName, ".Upstream"))
            pBPort->bUpstream = (strcasecmp(records[i]->parameterValue, "TRUE") == 0);
        else if (strstr(records[i]->parameterName, ".Enable"))
            pBPort->bEnabled = (strcasecmp(records[i]->parameterValue, "TRUE") == 0);
        else if (strstr(records[i]->parameterName, ".Pvid"))
            pBPort->pvid = atoi(records[i]->parameterValue);
        else if (strstr(records[i]->parameterName, ".PriorityTag"))
            pBPort->bPriTag = (strcasecmp(records[i]->parameterValue, "TRUE") == 0);
    }

    if (pBPort->bMgt)
        pBPort->control = &g_SWBrPCtl;
    else
        pBPort->control = &g_lanEthBrPCtl;
    pBPort->hwid = pBPort->linkName;

    PsmFreeRecords_s(records, recordCnt);
    /*
    fprintf(stderr, "==== pBPort ====\n");
    fprintf(stderr, "alias %s\n", pBPort->alias);
    fprintf(stderr, "instanceNumber %lu\n", pBPort->instanceNumber);
    fprintf(stderr, "name %s\n", pBPort->name);
    fprintf(stderr, "standard %d\n", pBPort->standard);
    fprintf(stderr, "linkName %s\n", pBPort->linkName);
    fprintf(stderr, "linkType %d\n", pBPort->linkType);
    fprintf(stderr, "bMgt %d\n", pBPort->bMgt);
    fprintf(stderr, "bEnabled %d\n", pBPort->bEnabled);
    fprintf(stderr, "bPriTag %d\n", pBPort->bPriTag);
    fprintf(stderr, "bUpstream %d\n", pBPort->bUpstream);
    fprintf(stderr, "bAllowDelete %d\n", pBPort->bAllowDelete);
    fprintf(stderr, "mode %d\n", pBPort->mode);
    fprintf(stderr, "pvid %d\n", pBPort->pvid);
    */
    return ANSC_STATUS_SUCCESS;
}

static ANSC_STATUS _Psm_SetBPort(ULONG l2InstNum, ULONG bportInstNum, PBRIDGE_PORT pBPort)
{
    int retPsmGet = CCSP_SUCCESS;
    char param_value[256]={0};
    char *deviceStr = NULL;
    char param_name[256]= {0};
    errno_t rc = -1;

    //AnscTraceFlow(("<HL> %s \n",__FUNCTION__));

    //deviceStr = pBPort->linkName;
    rc = strcpy_s(param_value, sizeof(param_value), ((pBPort->bAllowDelete==TRUE) ? "TRUE" : "FALSE"));
    ERR_CHK(rc);
    _PSM_SET_BPORT(_PSM_BPORT_TML_ALLOWDELETE);

    rc = sprintf_s(param_value, sizeof(param_value), "%d", pBPort->pvid);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }
    _PSM_SET_BPORT(_PSM_BPORT_TML_PVID);

    rc = strcpy_s(param_value, sizeof(param_value), ((pBPort->bPriTag==TRUE) ? "TRUE" : "FALSE"));
    ERR_CHK(rc);
    _PSM_SET_BPORT(_PSM_BPORT_TML_PRITAG);
 

    deviceStr = NULL;

    rc = strcpy_s(param_value, sizeof(param_value), ((pBPort->bEnabled==TRUE) ? "TRUE" : "FALSE"));
    ERR_CHK(rc);
    _PSM_SET_BPORT(_PSM_BPORT_TML_ENABLE);

    rc = strcpy_s(param_value, sizeof(param_value), pBPort->alias);
    ERR_CHK(rc);
    _PSM_SET_BPORT(_PSM_BPORT_TML_ALIAS);

    rc = strcpy_s(param_value, sizeof(param_value), pBPort->name);
    ERR_CHK(rc);
    _PSM_SET_BPORT(_PSM_BPORT_TML_NAME);

    rc = strcpy_s(param_value, sizeof(param_value), pBPort->linkName);
    ERR_CHK(rc);
    _PSM_SET_BPORT(_PSM_BPORT_TML_LINKNAME);
 
    rc = sprintf_s(param_value, sizeof(param_value), "%s", _COSA_GetInterfaceTypeStr(pBPort->linkType));
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }
    AnscTraceFlow(("%s linkType=%s port->linkType=%d\n",__FUNCTION__,param_value,pBPort->linkType));
    _PSM_SET_BPORT(_PSM_BPORT_TML_LINKTYPE);
  
    rc = strcpy_s(param_value, sizeof(param_value), ((pBPort->bMgt==TRUE) ? "TRUE" : "FALSE"));
    ERR_CHK(rc);
    _PSM_SET_BPORT(_PSM_BPORT_TML_MGT);

    rc = strcpy_s(param_value, sizeof(param_value), ((pBPort->mode == COSA_DML_BPORT_PASSTHRU) ? "PassThrough" : "Tagging"));
    ERR_CHK(rc);
    _PSM_SET_BPORT(_PSM_BPORT_TML_MODE);
 
    rc = sprintf_s(param_value, sizeof(param_value), "%lu", pBPort->instanceNumber);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }
    _PSM_SET_BPORT(_PSM_BPORT_TML_INSTNUM);
  
    rc = strcpy_s(param_value, sizeof(param_value), ((pBPort->bUpstream==TRUE) ? "TRUE" : "FALSE"));
    ERR_CHK(rc);
    _PSM_SET_BPORT(_PSM_BPORT_TML_UPSTREAM);

    return ANSC_STATUS_SUCCESS;

_Psm_SetBPort_Err:
    AnscTraceFlow(("%s: cannot set %s\n",__FUNCTION__,param_name));
    return ANSC_STATUS_FAILURE;
}

static ANSC_STATUS _Psm_GetBVlan(ULONG l2InstNum, ULONG bvlanInstNum, PBRIDGE_VLAN pBVlan)
{
    int retPsmGet = CCSP_SUCCESS;
    char *param_value= NULL;
    char param_name[256]= {0};

    AnscTraceFlow(("<HL> %s \n",__FUNCTION__));

    _PSM_GET_BVLAN(_PSM_BVLAN_TML_ALIAS);
    AnscCopyString(pBVlan->alias, param_value);
    ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(param_value);

    _PSM_GET_BVLAN(_PSM_BVLAN_TML_INSTNUM);
    pBVlan->instanceNumber = _ansc_atol(param_value);
    ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(param_value);

    return ANSC_STATUS_SUCCESS;    

_Psm_GetBVlan_Err:
    AnscTraceFlow(("%s: cannot find %s\n",__FUNCTION__,param_name));
    return ANSC_STATUS_FAILURE;        
}

static ANSC_STATUS _Psm_SetBVlan(ULONG l2InstNum, ULONG bvlanInstNum, PBRIDGE_VLAN pBVlan)
{
    int retPsmGet = CCSP_SUCCESS;
    char param_value[256]={0};
    char param_name[256]= {0};
    errno_t rc = -1;

    AnscTraceFlow(("<HL> %s \n",__FUNCTION__));

 
    rc = strcpy_s(param_value, sizeof(param_value), pBVlan->alias);
    ERR_CHK(rc);
    _PSM_SET_BVLAN(_PSM_BVLAN_TML_ALIAS);

 
    rc = sprintf_s(param_value, sizeof(param_value), "%lu", pBVlan->instanceNumber);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }
    _PSM_SET_BVLAN(_PSM_BVLAN_TML_INSTNUM);
  
 
    return ANSC_STATUS_SUCCESS;

_Psm_SetBVlan_Err:
    AnscTraceFlow(("%s: cannot set %s\n",__FUNCTION__,param_name));
    return ANSC_STATUS_FAILURE;
}

/*
///Saves an instance number and alias for a given namespaced object name.
static int saveID(char* Namespace, char* ifName, ULONG ulInstanceNumber,char* pAlias) {
    UtopiaContext utctx;
    char idStr[COSA_DML_IF_NAME_LENGTH+10];
    Utopia_Init(&utctx);

    sprintf(idStr,"%s,%u", pAlias,ulInstanceNumber);
    Utopia_RawSet(&utctx,BRIDGE_PORT_ID_NAMESPACE,ifName,idStr);
    
    Utopia_Free(&utctx,TRUE);

    return 0;
}

///Loads an instance number and alias for a given namespaced object name.
static int loadID(char* ifName, ULONG* ulInstanceNumber, char* pAlias) {
    UtopiaContext utctx;
    char idStr[COSA_DML_IF_NAME_LENGTH+10] = {0};;
    char* instNumString;
    int rv;
    Utopia_Init(&utctx);

    rv =Utopia_RawGet(&utctx, BRIDGE_PORT_ID_NAMESPACE, ifName, idStr, sizeof(idStr));
    if (rv == -1 || idStr[0] == '\0') {
        Utopia_Free(&utctx,0);
        return -1;
    }
    instNumString=idStr + AnscSizeOfToken(idStr, ",", sizeof(idStr))+1;
    *(instNumString-1)='\0';

    AnscCopyString(pAlias, idStr);
    *ulInstanceNumber = AnscGetStringUlong(instNumString);

    Utopia_Free(&utctx,0);
    return 0;
}
*/

//Lan switch bridge control functions -------------------------------
ANSC_STATUS lSwBrGetStatus(struct bridge* bridge, PCOSA_DML_BRG_STATUS status){
    UNREFERENCED_PARAMETER(bridge);
    *status = COSA_DML_BRG_STATUS_Enabled;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS lSwBrSetEnabled(struct bridge* bridge, BOOLEAN enable){
    UNREFERENCED_PARAMETER(bridge);
    UNREFERENCED_PARAMETER(enable);
    return ANSC_STATUS_NOT_SUPPORTED;
}

ANSC_STATUS lSwBrAddPort(struct bridge* bridge, PCOSA_DML_BRG_PORT_CFG newPortCfg){
    UNREFERENCED_PARAMETER(bridge);
    UNREFERENCED_PARAMETER(newPortCfg);
    return ANSC_STATUS_NOT_SUPPORTED;
}

ANSC_STATUS lSwBrRemovePort(struct bridge* bridge, ULONG portInstanceNumber){
    UNREFERENCED_PARAMETER(bridge);
    UNREFERENCED_PARAMETER(portInstanceNumber);
    return ANSC_STATUS_NOT_SUPPORTED;
}

//$HL 4/17/2013
ANSC_STATUS lSwBrConfirmStructureUpdate(struct bridge* bridge, PBRIDGE_PORT pBPort, PCOSA_DML_BRG_PORT_CFG cfg){
    UNREFERENCED_PARAMETER(bridge);
    UNREFERENCED_PARAMETER(pBPort);
    UNREFERENCED_PARAMETER(cfg);
    return ANSC_STATUS_NOT_SUPPORTED;
}

ANSC_STATUS lSwBrAddVlan(struct bridge *bridge, PCOSA_DML_BRG_VLAN_CFG pEntry)
{
    UNREFERENCED_PARAMETER(bridge);
    UNREFERENCED_PARAMETER(pEntry);
    return ANSC_STATUS_NOT_SUPPORTED;
}

ANSC_STATUS lSwBrRemoveVlan(struct bridge *bridge, ULONG vlanInstanceNumber)
{
    UNREFERENCED_PARAMETER(bridge);
    UNREFERENCED_PARAMETER(vlanInstanceNumber);
    return ANSC_STATUS_NOT_SUPPORTED;
}

#endif

#if defined _COSA_DRG_TPG_

///Populate bridge objects from remote processors
///
///This function needs to see if the remote ports can be retrieved,
///and if not, either must update the backend once they can or block
///until they can. The former will require notification of the middle layer.
///
/// NOTE: the decision has been made to not represent remote bridge ports on TPG or CNS 
/// for now.
void pollRemotePorts() {

}




//Lan switch bridge port control functions---------------------------
ANSC_STATUS lanSwBrPCtlSetEnabled(PBRIDGE_PORT port, BOOLEAN enable) {
    PSwitchPortID swID = (PSwitchPortID)port->hwid;
    uint32_t swDev;
    int rv;
    uint32_t status = 0;
    if(swID->unit==0) {
        swDev = gLanSwDev;
    } else {
        swDev = gWanSwDev;
    }

    swcfg_set_link(swDev, swID->port, enable, -1, -1, -1, -1);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS lanSwBrPCtlGetEnabled(PBRIDGE_PORT port, BOOLEAN* enabled) {
    PSwitchPortID swID = (PSwitchPortID)port->hwid;
    uint32_t swDev, en;
    int rv;
    uint32_t status = 0;
    if(swID->unit==0) {
        swDev = gLanSwDev;
    } else {
        swDev = gWanSwDev;
    }

    swcfg_get_link(swDev, swID->port, &en, NULL,NULL,NULL,NULL,NULL,NULL);

    *enabled = (UCHAR)en;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS lanSwBrPCtlSetMgtPort(PBRIDGE_PORT port, BOOLEAN isMgt){
    return ANSC_STATUS_NOT_SUPPORTED;
}

ANSC_STATUS lanSwBrPCtlGetStatus(PBRIDGE_PORT port, PCOSA_DML_IF_STATUS status){
    PSwitchPortID swID = (PSwitchPortID)port->hwid;
    uint32_t swDev, theStatus, theAdmin;
    int rv;
    
    if(swID->unit==0) {
        swDev = gLanSwDev;
    } else {
        swDev = gWanSwDev;
    }

    swcfg_get_link(swDev, swID->port, &theAdmin, &theStatus, NULL, NULL, NULL, NULL, NULL);

    if (theAdmin) {
        if (theStatus) {
            *status = COSA_DML_IF_STATUS_Up;
        } else {
            *status = COSA_DML_IF_STATUS_LowerLayerDown;
        } 
    } else {
        *status = COSA_DML_IF_STATUS_Down;
    }

    return ANSC_STATUS_SUCCESS;

}

ANSC_STATUS lanSwBrPCtlGetAFT(PBRIDGE_PORT port, PCOSA_DML_BRG_PORT_AFT aft){
    //Reevaluate once vlans are implemented
    *aft = COSA_DML_BRG_PORT_AFT_AdmitAll;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS lanSwBrPCtlSetAFT(PBRIDGE_PORT port, COSA_DML_BRG_PORT_AFT aft){
    //Reevaluate once vlans are implemented
    return ANSC_STATUS_NOT_SUPPORTED;
}

uint32_t lanSwBrPCtlGetLastChanged(PBRIDGE_PORT port){

    return 0; //TODO: Support a real value
}

//Unsupported. Network structure will not be configurable via TR-181.
ANSC_STATUS lanSwBrPCtlSetLowerLink(PBRIDGE_PORT port, char* name, COSA_DML_BRG_LINK_TYPE type){
    return ANSC_STATUS_NOT_SUPPORTED;
}

ANSC_STATUS lanSwBrPCtlGetState(PBRIDGE_PORT port, PCOSA_DML_BRG_PORT_STATE state){
    PSwitchPortID swID = (PSwitchPortID)port->hwid;
    uint32_t swDev, theStatus;
    int rv;
    uint32_t status = 0;
    if(swID->unit==0) {
        swDev = gLanSwDev;
    } else {
        swDev = gWanSwDev;
    }

    swcfg_get_link(swDev, swID->port, NULL, &theStatus, NULL, NULL, NULL, NULL, NULL);

    if (theStatus) {
        *state = COSA_DML_BRG_PORT_STATE_Forwarding;
    } else {
        *state = COSA_DML_BRG_PORT_STATE_Disabled;
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS lanSwBrPCtlGetStats(PBRIDGE_PORT port, PCOSA_DML_IF_STATS stats){
    PSwitchPortID swID = (PSwitchPortID)port->hwid;
    
    getSwitchStats(swID, stats);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS lanSwBrPCtlAddToVlan(PBRIDGE_PORT port, ULONG vlanId, BOOLEAN untagged)
{
    // TODO
    return ANSC_STATUS_NOT_SUPPORTED;
}

ANSC_STATUS lanSwBrPCtlRemoveFromVlan(PBRIDGE_PORT port, ULONG vlanId)
{
    // TODO
    return ANSC_STATUS_NOT_SUPPORTED;
}

#endif

#if defined _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_

//Control functions for ethernet lan's linux bridge TR181 bridge port-------------------------------------------------------------------------
ANSC_STATUS lanBrPCtlSetEnabled(PBRIDGE_PORT port, BOOLEAN enable) {
    struct ifreq ifr;
    int fd = socket(AF_INET, SOCK_DGRAM, 0);

#if defined(MULTILAN_FEATURE)
    if(port->mode == COSA_DML_BPORT_PASSTHRU) {
        //Do not try to directly set port state of passthru device to up/down
        return ANSC_STATUS_SUCCESS;
    }
#endif

    AnscCopyString(ifr.ifr_name, port->name);
  
    ioctl(fd, SIOCGIFFLAGS, &ifr);

    if(enable) {
        if (!(ifr.ifr_flags & IFF_UP)) {
            ifr.ifr_flags |= (IFF_UP | IFF_RUNNING);
            ioctl(fd, SIOCSIFFLAGS, &ifr);
        }
    } else {
        if ((ifr.ifr_flags & IFF_UP)) {
            ifr.ifr_flags &= ~IFF_UP;
            ioctl(fd, SIOCSIFFLAGS, &ifr);
        }
    }

    close(fd);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS lanBrPCtlGetEnabled(PBRIDGE_PORT port, BOOLEAN* enabled) {
    struct ifreq ifr;
    int fd = socket(AF_INET, SOCK_DGRAM, 0);

    CcspTraceInfo(("------lanBrPCtlGetEnabled, port:%s...\n", port->name));

    AnscCopyString(ifr.ifr_name, port->name);
  
    ioctl(fd, SIOCGIFFLAGS, &ifr);

    *enabled = (ifr.ifr_flags & IFF_UP) && (ifr.ifr_flags & IFF_RUNNING);

    close(fd);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS lanBrPCtlSetMgtPort(PBRIDGE_PORT port, BOOLEAN isMgt) {
    UNREFERENCED_PARAMETER(port);
    if(isMgt) {

    } else {

    }
    return ANSC_STATUS_NOT_SUPPORTED;
}

ANSC_STATUS lanBrPCtlGetStatus(PBRIDGE_PORT port, PCOSA_DML_IF_STATUS status) {
    BOOLEAN enabled;
//    CcspTraceInfo(("------lanBrPCtlGetStatus...\n"));
    port->control->getEnabled(port, &enabled);
    if(enabled) {
        *status = COSA_DML_IF_STATUS_Up;
    } else {
        *status = COSA_DML_IF_STATUS_Down;
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS lanBrPCtlGetAFT(PBRIDGE_PORT port, PCOSA_DML_BRG_PORT_AFT aft) {
    UNREFERENCED_PARAMETER(port);
    *aft = COSA_DML_BRG_PORT_AFT_AdmitAll;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS lanBrPCtlSetAFT(PBRIDGE_PORT port, COSA_DML_BRG_PORT_AFT aft) {
    UNREFERENCED_PARAMETER(port);
    UNREFERENCED_PARAMETER(aft);
    return ANSC_STATUS_NOT_SUPPORTED;
}

uint32_t lanBrPCtlGetLastChanged(PBRIDGE_PORT port) {
    UNREFERENCED_PARAMETER(port);
    return 0; //TODO
}

ANSC_STATUS lanBrPCtlSetLowerLink(PBRIDGE_PORT port, char* name, COSA_DML_BRG_LINK_TYPE type) {
    UNREFERENCED_PARAMETER(port);
    UNREFERENCED_PARAMETER(name);
    UNREFERENCED_PARAMETER(type);
    return ANSC_STATUS_NOT_SUPPORTED;
}

ANSC_STATUS lanBrPCtlGetState(PBRIDGE_PORT port, PCOSA_DML_BRG_PORT_STATE state) {
    int stateVal;
    FILE* fd;
    char stateFile[50];
    errno_t rc = -1;
    rc = sprintf_s(stateFile, sizeof(stateFile), "/sys/class/net/%s/brport/state", port->name);
    if(rc < EOK)
    {
        ERR_CHK(rc);
        return ANSC_STATUS_FAILURE;
    }
    fd = fopen(stateFile, "r");
    
    if (!fd) {
        return ANSC_STATUS_FAILURE;
    }

    fscanf(fd, "%i", &stateVal);

    fclose(fd);

    switch(stateVal) {
    case BR_STATE_DISABLED:
        *state = COSA_DML_BRG_PORT_STATE_Disabled;
        break;
    // case BR_STATE_MEMBERSENING:
    //     *state = COSA_DML_BRG_PORT_STATE_MEMBERSening;
    //     break;
    case BR_STATE_LEARNING:
        *state = COSA_DML_BRG_PORT_STATE_Learning;
        break;
    case BR_STATE_FORWARDING:
        *state = COSA_DML_BRG_PORT_STATE_Forwarding;
        break;
    case BR_STATE_BLOCKING:
        *state = COSA_DML_BRG_PORT_STATE_Blocking;
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS lanBrPCtlGetStats(PBRIDGE_PORT port, PCOSA_DML_IF_STATS stats) {
    OSStats osstats;
    ULONG   retVal = 0; 

    _ansc_memset(&osstats, 0, sizeof (osstats));

    retVal = getOSStats((char*)port->name, &osstats);
    CcspTraceInfo(("------lanBrPCtlGetStats, getOSStats:%d...\n", retVal));

    stats->BroadcastPacketsReceived = 0; //NOT SUPPORTED
    stats->BroadcastPacketsSent = 0; //NOT SUPPORTED
    stats->BytesReceived = osstats.rxBytes;
    stats->BytesSent = osstats.txBytes;
    stats->DiscardPacketsReceived = osstats.rxDrops;
    stats->DiscardPacketsSent = osstats.txDrops;
    stats->ErrorsReceived = osstats.rxErrors;
    stats->ErrorsSent = osstats.txErrors;
    stats->MulticastPacketsReceived = osstats.rxMulticast;
    stats->MulticastPacketsSent = osstats.txMulticast;
    stats->PacketsReceived = osstats.rxPackets;
    stats->PacketsSent = osstats.txPackets;
    stats->UnicastPacketsReceived = 0; //NOT SUPPORTED (cant calculate due to broadcast packets)
    stats->UnicastPacketsSent = 0; //NOT SUPPORTED (cant calculate due to broadcast packets)
    stats->UnknownProtoPacketsReceived = 0; //NOT SUPPORTED
    return ANSC_STATUS_NOT_SUPPORTED;
}

ANSC_STATUS lanBrPCtlAddToVlan(PBRIDGE_PORT port, ULONG vlanId, BOOLEAN untagged)
{
    UNREFERENCED_PARAMETER(port);
    UNREFERENCED_PARAMETER(vlanId);
    UNREFERENCED_PARAMETER(untagged);
    // TODO
    return ANSC_STATUS_NOT_SUPPORTED;
}

ANSC_STATUS lanBrPCtlRemoveFromVlan(PBRIDGE_PORT port, ULONG vlanId)
{
    UNREFERENCED_PARAMETER(port);
    UNREFERENCED_PARAMETER(vlanId);
    // TODO
    return ANSC_STATUS_NOT_SUPPORTED;
}


//Control functions for linux bridge's TR181 bridge port-------------------------------------------------------------------------
ANSC_STATUS lnxBrPCtlSetEnabled(PBRIDGE_PORT port, BOOLEAN enable) {
    UNREFERENCED_PARAMETER(port);
    UNREFERENCED_PARAMETER(enable);
    // BOOLEAN isEnabled;
    // lnxBrPCtlGetEnabled(port,&isEnabled);
    // if (isEnabled == enable) {
    //     return ANSC_STATUS_SUCCESS;
    // }

    // if (enable) {
    //     commonSyseventSet("lan-start", "");
    // } else {
    //     commonSyseventSet("lan-stop","");
    // }

    return ANSC_STATUS_SUCCESS;;
}

ANSC_STATUS lnxBrPCtlGetEnabled(PBRIDGE_PORT port, BOOLEAN* enabled) {
    UNREFERENCED_PARAMETER(port);
    char status[10];
    commonSyseventGet("lan-status", status, sizeof(status));
    
    *enabled = !AnscEqualString("stopped",status,TRUE) && !AnscEqualString("stopping",status,TRUE);
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS lnxBrPCtlSetMgtPort(PBRIDGE_PORT port, BOOLEAN isMgt) {
    UNREFERENCED_PARAMETER(port);
    UNREFERENCED_PARAMETER(isMgt);
    return ANSC_STATUS_NOT_SUPPORTED;
}

ANSC_STATUS lnxBrPCtlGetStatus(PBRIDGE_PORT port, PCOSA_DML_IF_STATUS status) {
    BOOLEAN enabled;
    struct ifreq ifr;
    port->control->getEnabled(port,&enabled);
    if(enabled) {
        
        int fd = socket(AF_INET, SOCK_DGRAM, 0);
    
        AnscCopyString(ifr.ifr_name, (char*)port->hwid);
      
        if (ioctl(fd, SIOCGIFFLAGS, &ifr)){
            *status = COSA_DML_IF_STATUS_Unknown;
        } else {
            enabled = (ifr.ifr_flags & IFF_UP) && (ifr.ifr_flags & IFF_RUNNING);
            if (enabled) {
                *status = COSA_DML_IF_STATUS_Up;
            } else {
                *status = COSA_DML_IF_STATUS_Down;
            }
        }
    
        close(fd);
    } else {
        *status = COSA_DML_IF_STATUS_NotPresent;
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS lnxBrPCtlGetAFT(PBRIDGE_PORT port, PCOSA_DML_BRG_PORT_AFT aft) {
    UNREFERENCED_PARAMETER(port);
    *aft = COSA_DML_BRG_PORT_AFT_AdmitAll;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS lnxBrPCtlSetAFT(PBRIDGE_PORT port, COSA_DML_BRG_PORT_AFT aft) {
    UNREFERENCED_PARAMETER(port);
    UNREFERENCED_PARAMETER(aft);
    return ANSC_STATUS_NOT_SUPPORTED;
}

uint32_t lnxBrPCtlGetLastChanged(PBRIDGE_PORT port) {
    UNREFERENCED_PARAMETER(port);
    return 0; //TODO
}

ANSC_STATUS lnxBrPCtlSetLowerLink(PBRIDGE_PORT port, char* name, COSA_DML_BRG_LINK_TYPE type) {
    UNREFERENCED_PARAMETER(port);
    UNREFERENCED_PARAMETER(name);
    UNREFERENCED_PARAMETER(type);
    return ANSC_STATUS_NOT_SUPPORTED;
}

ANSC_STATUS lnxBrPCtlGetState(PBRIDGE_PORT port, PCOSA_DML_BRG_PORT_STATE state) { //TODO: Verify implementation
    BOOLEAN enabled;
    port->control->getEnabled(port, &enabled);

    if (!enabled) {
        *state = COSA_DML_BRG_PORT_STATE_Disabled;
        return ANSC_STATUS_SUCCESS;
    }

    *state = COSA_DML_BRG_PORT_STATE_Forwarding; //Not sure this makes sense for the management port representing a linux bridge.
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS lnxBrPCtlAddToVlan(PBRIDGE_PORT port, ULONG vlanId, BOOLEAN untagged)
{
    UNREFERENCED_PARAMETER(port);
    UNREFERENCED_PARAMETER(vlanId);
    UNREFERENCED_PARAMETER(untagged);
    // TODO
    return ANSC_STATUS_NOT_SUPPORTED;
}

ANSC_STATUS lnxBrPCtlRemoveFromVlan(PBRIDGE_PORT port, ULONG vlanId)
{
    UNREFERENCED_PARAMETER(port);
    UNREFERENCED_PARAMETER(vlanId);
    // TODO
    return ANSC_STATUS_NOT_SUPPORTED;
}


//Lan software bridge functions-------------------------------------------------------------------------
ANSC_STATUS SWBrGetStatus(struct bridge* bridge, PCOSA_DML_BRG_STATUS status) {
    UNREFERENCED_PARAMETER(bridge);
    *status = COSA_DML_BRG_STATUS_Enabled;
    return ANSC_STATUS_SUCCESS; //TODO: how to implement?
}

ANSC_STATUS SWBrSetEnabled(struct bridge* bridge, BOOLEAN enable) {
    UNREFERENCED_PARAMETER(bridge);
    UNREFERENCED_PARAMETER(enable);
    return ANSC_STATUS_NOT_SUPPORTED; //TODO: what does this mean for all bridge ports?
}

ANSC_STATUS SWBrAddPort(PBRIDGE pBridge, PCOSA_DML_BRG_PORT_CFG pEntry) {
    //$HL 4/15/2013
    //return ANSC_STATUS_NOT_SUPPORTED;
    PBRIDGE_PORT pBPort;
    char param_value[256]={0}, param_name[256]={0};
    AnscTraceFlow(("<HL> %s\n",__FUNCTION__));
    pBPort = (PBRIDGE_PORT)AnscAllocateMemory(sizeof (BRIDGE_PORT));
    _ansc_memset(pBPort, 0, sizeof(BRIDGE_PORT));
    AnscCopyString(pBPort->alias, pEntry->Alias);
    AnscCopyString(pBPort->linkName, pEntry->LinkName);
    pBPort->instanceNumber = pEntry->InstanceNumber;
    pBPort->bMgt = pEntry->bManagementPort;
    pBPort->bPriTag = pEntry->bPriorityTagging;
    pBPort->linkType = pEntry->LinkType;
    //pBPort->pvid = pEntry->PVID;
    pBPort->pvid = pBridge->vlanid;
    pEntry->PVID = pBridge->vlanid;
    pBPort->mode = pEntry->mode;
    pBPort->bEnabled = pEntry->bEnabled;
    pBPort->bUpstream = pEntry->bUpstream;
    pBPort->bAllowDelete = pEntry->bAllowDelete;
    if (pBPort->bMgt)
    {
        AnscCopyString(pBPort->name, pBridge->name);
        pBPort->control = &g_SWBrPCtl;
    }
    else
    {
        pBPort->control = &g_lanEthBrPCtl;
        AnscCopyString(pBPort->name,pEntry->LinkName);
    }
    pBPort->hwid = pBPort->name;
    pBridge->numOfPorts++;
    AnscSListPushEntryAtBack(&pBridge->portList, &pBPort->Linkage);
    _Psm_SetBPort(pBridge->l2InstanceNumber, pBPort->instanceNumber, pBPort);
    if (pEntry->bEnabled && !pBPort->bMgt) 
    {
        _PSM_ADD_MEMBERS(pBridge, pBPort, param_name, param_value);
    }
    if((!pBPort->bUpstream) && AnscSizeOfString(pBPort->linkName) && pBPort->bEnabled)
    {
        pBridge->bUpstream = FALSE;
        _Psm_AddLinkMembers(pBridge->l2InstanceNumber);
    }
    if (!pBPort->bMgt)
    {
        v_secure_system("sysevent set multinet-syncMembers %lu", pBridge->l2InstanceNumber);
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS SWBrRemovePort(struct bridge* bridge, ULONG portInstanceNumber) {
    //$HL 4/16/2013
    //return ANSC_STATUS_NOT_SUPPORTED;
    _COSA_DelBPort(portInstanceNumber,bridge);
    v_secure_system("sysevent set multinet-syncMembers %lu", bridge->l2InstanceNumber);
    return ANSC_STATUS_SUCCESS;
}

//$HL 4/16/2013
ANSC_STATUS SWBrConfirmStructureUpdate(struct bridge* pBridge, PBRIDGE_PORT pBPort, PCOSA_DML_BRG_PORT_CFG pCfg) 
{
    char param_value[256]={0}, param_name[256]={0};

    // remove old name from the member list in PSM, regardless of pCfg->bEnabled or not.
    _PSM_REMOVE_MEMBERS(pBridge,pBPort,param_name,param_value);

    // update values and add port to bridge in PSM if pCfg->bEnabled
    AnscCopyString(pBPort->linkName,pCfg->LinkName);
    pBPort->mode = pCfg->mode;
    pBPort->linkType = pCfg->LinkType;
    pBPort->bUpstream = pCfg->bUpstream;
    pBPort->bEnabled = pCfg->bEnabled;
    if ((pBPort->bEnabled==TRUE) && AnscSizeOfString(pBPort->linkName))
    {
        _PSM_ADD_MEMBERS(pBridge, pBPort, param_name, param_value);

        if (!pBPort->bUpstream) {
            //add hide/fake local interface
            pBridge->bUpstream = FALSE;
            _Psm_AddLinkMembers(pBridge->l2InstanceNumber);
        }
    }

    // call sysevent
    v_secure_system("sysevent set multinet-syncMembers %lu", pBridge->l2InstanceNumber);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS SWBrAddVlan(struct bridge *pBridge, PCOSA_DML_BRG_VLAN_CFG pEntry)
{
    PBRIDGE_VLAN pBVlan;
    AnscTraceFlow(("<HL> %s\n",__FUNCTION__));
    pBVlan = (PBRIDGE_VLAN)AnscAllocateMemory(sizeof (BRIDGE_VLAN));
    _ansc_memset(pBVlan, 0, sizeof(BRIDGE_VLAN));
    AnscCopyString(pBVlan->alias, pEntry->Alias);
    pBVlan->instanceNumber = pEntry->InstanceNumber;
    _COSA_UpdateBrVLANID(pBridge,pEntry->VLANID);
    pBridge->numOfVlans++;
    AnscSListPushEntryAtBack(&pBridge->vlanList, &pBVlan->Linkage);
    _Psm_SetBVlan(pBridge->l2InstanceNumber, pBVlan->instanceNumber, pBVlan);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS SWBrRemoveVlan(struct bridge *pBridge, ULONG vlanInstanceNumber)
{
    _COSA_DelBVlan(vlanInstanceNumber,pBridge);
    return ANSC_STATUS_SUCCESS;
}

#endif
