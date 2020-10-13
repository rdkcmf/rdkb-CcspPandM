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


/**********************************************************************

    module:	dml_api_bridging.h

        For Data Model Library Implementation (DML),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structure and function
        prototypes for DML DHCP API.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Li Shi

    ---------------------------------------------------------------

    revision:

        03/15/2011    initial revision.

**********************************************************************/


#ifndef  _DML_API_BRIDGING_
#define  _DML_API_BRIDGING_

#include "ansc_platform.h"
#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "poam_irepfo_interface.h"
#include "sys_definitions.h"

/**********************************************************************
                STRUCTURE AND CONSTANT DEFINITIONS
**********************************************************************/

#define SYSCFG_HOMESECURITY_ETH4_FLAG			"HomeSecurityEthernet4Flag"

typedef  enum
_COSA_DML_BRG_STATUS
{
    COSA_DML_BRG_STATUS_Disabled        = 1,
    COSA_DML_BRG_STATUS_Enabled,
    COSA_DML_BRG_STATUS_Error
}
COSA_DML_BRG_STATUS, *PCOSA_DML_BRG_STATUS;

typedef  enum
_COSA_DML_BRG_STD
{
    COSA_DML_BRG_STD_8021D_2004        = 1,
    COSA_DML_BRG_STD_8021Q_2005
}
COSA_DML_BRG_STD, *PCOSA_DML_BRG_STD;

//$HL 4/26/2013
typedef  enum
_COSA_DML_BPORT_MODE
{
    COSA_DML_BPORT_UNKNOWN  = 0,
    COSA_DML_BPORT_PASSTHRU  = 1,
    COSA_DML_BPORT_TAGGING
} 
COSA_DML_BPORT_MODE, *PCOSA_DML_BPORT_MODE;

//$HL 07/15/2013
typedef  enum
_COSA_DML_BRG_LINK_TYPE
{
    COSA_DML_BRG_LINK_TYPE_NONE, //$HL 4/23/2013
    COSA_DML_BRG_LINK_TYPE_Eth = 1,
    COSA_DML_BRG_LINK_TYPE_EthVlan,
    COSA_DML_BRG_LINK_TYPE_Usb,
    COSA_DML_BRG_LINK_TYPE_Hpna,
    COSA_DML_BRG_LINK_TYPE_Moca,
    COSA_DML_BRG_LINK_TYPE_HomePlug,
    COSA_DML_BRG_LINK_TYPE_Upa,
    COSA_DML_BRG_LINK_TYPE_WiFiSsid,
    COSA_DML_BRG_LINK_TYPE_Bridge,
    COSA_DML_BRG_LINK_TYPE_Gre
}
COSA_DML_BRG_LINK_TYPE, *PCOSA_DML_BRG_LINK_TYPE;

typedef  enum
_COSA_DML_BRG_PORT_STATE
{
    COSA_DML_BRG_PORT_STATE_Disabled    = 1,
    COSA_DML_BRG_PORT_STATE_Blocking,
    COSA_DML_BRG_PORT_STATE_Listening,
    COSA_DML_BRG_PORT_STATE_Learning,
    COSA_DML_BRG_PORT_STATE_Forwarding,
    COSA_DML_BRG_PORT_STATE_Broken
}
COSA_DML_BRG_PORT_STATE, *PCOSA_DML_BRG_PORT_STATE;

typedef  enum
_COSA_DML_BRG_PORT_AFT
{
    COSA_DML_BRG_PORT_AFT_AdmitAll      = 1,
    COSA_DML_BRG_PORT_AFT_AdmitOnlyVLANTagged,
    COSA_DML_BRG_PORT_AFT_AdmitOnlyPrioUntagged
}
COSA_DML_BRG_PORT_AFT, *PCOSA_DML_BRG_PORT_AFT;


typedef  struct
_COSA_DML_BRG_CFG
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];
#if defined (MULTILAN_FEATURE)
    char                            name[COSA_DML_IF_NAME_LENGTH];
#endif
    BOOLEAN                         bEnabled;
    BOOLEAN                         bAllowDelete;
    COSA_DML_BRG_STD                Std;
}
COSA_DML_BRG_CFG, *PCOSA_DML_BRG_CFG;


typedef  struct
_COSA_DML_BRG_INFO
{
    COSA_DML_BRG_STATUS             Status;
}
COSA_DML_BRG_INFO, *PCOSA_DML_BRG_INFO;


typedef  struct
_COSA_DML_BRG_FULL
{
    COSA_DML_BRG_CFG                Cfg;
    COSA_DML_BRG_INFO               Info;
}
COSA_DML_BRG_FULL, *PCOSA_DML_BRG_FULL;

typedef  struct
_COSA_DML_BRG_PORT_CFG
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    COSA_DML_BRG_STD                Std;
    COSA_DML_BRG_LINK_TYPE          LinkType;
    char                            LinkName[COSA_DML_IF_NAME_LENGTH];  /* LinkName, including bridge name */
    BOOLEAN                         bManagementPort;
    UCHAR                           DftUserPriority;                    /* not supported for now */
    UCHAR                           PriorityRegeneration[8];            /* not supported for now */
    USHORT                          PVID;                               /* not supported for now, default 0 */
    COSA_DML_BRG_PORT_AFT           AcceptableFrameTypes;
    BOOLEAN                         bIngressFiltering;                  /* not supported for now */
    BOOLEAN                         bPriorityTagging;                   /* not supported for now */
    COSA_DML_BPORT_MODE             mode;                               //$HL 4/16/2013 need DM to support
    BOOLEAN                         bUpstream;
    BOOLEAN                         bAllowDelete;
}
COSA_DML_BRG_PORT_CFG, *PCOSA_DML_BRG_PORT_CFG;


typedef  struct
_COSA_DML_BRG_PORT_INFO
{
    char                            Name[COSA_DML_IF_NAME_LENGTH];      /* Read-only */
    COSA_DML_IF_STATUS              Status;
    ULONG                           LastChange;
    COSA_DML_BRG_PORT_STATE         PortState;
}
COSA_DML_BRG_PORT_INFO, *PCOSA_DML_BRG_PORT_INFO;


typedef  struct
_COSA_DML_BRG_PORT_FULL
{
    COSA_DML_BRG_PORT_CFG           Cfg;
    COSA_DML_BRG_PORT_INFO          Info;
}
COSA_DML_BRG_PORT_FULL, *PCOSA_DML_BRG_PORT_FULL;

typedef  struct 
_COSA_PRI_BRG_FULL
{
    COSA_DML_BRG_CFG                Cfg;
    COSA_DML_BRG_INFO               Info;
    USHORT                          ulNumOfPort;
    COSA_DML_BRG_PORT_FULL          PortList[10];
}
COSA_PRI_BRG_FULL, *PCOSA_PRI_BRG_FULL;

typedef  struct
_COSA_DML_BRG_FULL_ALL
{
    COSA_DML_BRG_CFG                Cfg;
    COSA_DML_BRG_INFO               Info;

    SLIST_HEADER                    PortList;
    ULONG                           ulNextPortInsNum;
    SLIST_HEADER                    VLANList;
    ULONG                           ulNextVLANInsNum;
    SLIST_HEADER                    VLANPortList;
    ULONG                           ulNextVLANPortInsNum;
}
COSA_DML_BRG_FULL_ALL, *PCOSA_DML_BRG_FULL_ALL;


typedef  struct
_COSA_DML_BRG_VLAN_CFG
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    int                             VLANID;                             /* 1-4094 */
}
COSA_DML_BRG_VLAN_CFG, *PCOSA_DML_BRG_VLAN_CFG;

typedef  struct
_COSA_DML_BRG_VLAN_INFO
{
    char                            Name[COSA_DML_IF_NAME_LENGTH];      /* Read-only */
}
COSA_DML_BRG_VLAN_INFO, *PCOSA_DML_BRG_VLAN_INFO;

typedef  struct
_COSA_DML_BRG_VLAN_FULL
{
    COSA_DML_BRG_VLAN_CFG           Cfg;
    COSA_DML_BRG_VLAN_INFO          Info;
    COSA_DML_BRG_VLAN_CFG           Cfg1;
}
COSA_DML_BRG_VLAN_FULL, *PCOSA_DML_BRG_VLAN_FULL;


typedef  struct
_COSA_DML_BRG_VLANPORT_CFG
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    ULONG                           VLANInsNum;
    ULONG                           PortInsNum;
    BOOLEAN                         bUntagged;
}
COSA_DML_BRG_VLANPORT_CFG, *PCOSA_DML_BRG_VLANPORT_CFG;

typedef  struct
_COSA_DML_BRG_VLANPORT_FULL
{
    COSA_DML_BRG_VLANPORT_CFG           Cfg;
    COSA_DML_BRG_VLANPORT_CFG           Cfg1;
}
COSA_DML_BRG_VLANPORT_FULL, *PCOSA_DML_BRG_VLANPORT_FULL;


/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/

ANSC_STATUS
CosaDmlBrgInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
);

ULONG
CosaDmlBrgGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    );

//$HL 7/8/2013
int CosaDmlBrgGetVLANID
    (
        ULONG                       ulInstanceNumber
    );

char * CosaDmlBrgGetName
    (
        ULONG                       ulInstanceNumber
    );

ANSC_STATUS
CosaDmlBrgGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_BRG_FULL          pEntry
    );

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
    );

ANSC_STATUS
CosaDmlBrgAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_BRG_CFG           pEntry
    );

ANSC_STATUS
CosaDmlBrgDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

ANSC_STATUS
CosaDmlBrgSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_BRG_CFG           pCfg        /* Identified by InstanceNumber */
    );

ANSC_STATUS
CosaDmlBrgGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_BRG_CFG           pCfg        /* Identified by InstanceNumber */
    );

ANSC_STATUS
CosaDmlBrgGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_BRG_INFO          pInfo
);

/*
 *  Bridge Ports
 */
ULONG
CosaDmlBrgPortGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber
    );

ANSC_STATUS
CosaDmlBrgPortGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_BRG_PORT_FULL     pEntry
    );

ANSC_STATUS
CosaDmlBrgPortSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

ANSC_STATUS
CosaDmlBrgPortAddEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        PCOSA_DML_BRG_PORT_CFG      pEntry
    );

ANSC_STATUS
CosaDmlBrgPortDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        ULONG                       ulInstanceNumber
    );

ANSC_STATUS
CosaDmlBrgPortSetCfg
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        PCOSA_DML_BRG_PORT_CFG      pCfg        /* Identified by InstanceNumber */
    );

ANSC_STATUS
CosaDmlBrgPortGetCfg
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        PCOSA_DML_BRG_PORT_CFG      pCfg        /* Identified by InstanceNumber */
    );

ANSC_STATUS
CosaDmlBrgPortGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_BRG_PORT_INFO     pInfo
    );

ANSC_STATUS
CosaDmlBrgPortGetStats
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_IF_STATS          pStats
    );

ANSC_STATUS
CosaDmlBrgVlanAddEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        PCOSA_DML_BRG_VLAN_FULL     pVLAN 
    );
/* CID: 135308 Unrecoverable parse warning*/
ANSC_STATUS
CosaDmlBrgVlanDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        ULONG                       ulVLANInsNum
    );

ULONG
CosaDmlBrgVlanGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber
    );

ANSC_STATUS
CosaDmlBrgVlanSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

ANSC_STATUS CosaDmlBrgVlanGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_BRG_VLAN_FULL     pEntry
    );

ANSC_STATUS
CosaDmlBrgVlanSetCfg
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        PCOSA_DML_BRG_VLAN_CFG      pCfg
    );


ANSC_STATUS
CosaDmlBrgVlanPortAddEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        int                         VLANID,
        ULONG                       ulPortInstanceNumber,
        BOOLEAN                     bUntagged
    );

ANSC_STATUS
CosaDmlBrgVlanPortDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulBrgInstanceNumber,
        int                         VLANID,
        ULONG                       ulPortInstanceNumber
    );

void CosaDmlPrintHSVlanPsmValue(char *fun,int linenum);

#endif

