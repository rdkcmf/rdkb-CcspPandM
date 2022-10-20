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

    module: cosa_ip_apis.h

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


#ifndef  _COSA_IP_APIS_H
#define  _COSA_IP_APIS_H

#include "cosa_apis.h"
#include "poam_irepfo_interface.h"
#include "sys_definitions.h"
#include "plugin_main_apis.h"

#define  COSA_DML_IPV6_ADDR_LENGTH                    128
#if defined (INTEL_PUMA7)
#define  COSA_DML_IP_IF_DEFAULT_MTU                   1500
#endif

/***********************************
    Actual definition declaration
************************************/

/*
    IP Part
*/
#define     COSA_DML_IP_LINK_TYPE_TOTAL 11
typedef  enum
_COSA_DML_IP_LINK_TYPE
{
    COSA_DML_IP_LINK_TYPE_Eth       = 1,
    COSA_DML_IP_LINK_TYPE_EthVlan,
    COSA_DML_IP_LINK_TYPE_Usb,
    COSA_DML_IP_LINK_TYPE_Hpna,
    COSA_DML_IP_LINK_TYPE_Moca,
    COSA_DML_IP_LINK_TYPE_HomePlug,
    COSA_DML_IP_LINK_TYPE_Upa,
    COSA_DML_IP_LINK_TYPE_WiFiSsid,
    COSA_DML_IP_LINK_TYPE_Bridge,
    COSA_DML_IP_LINK_TYPE_PPP,
    COSA_DML_IP_LINK_TYPE_DOCSIS,
    COSA_DML_IP_LINK_TYPE_LAST
}
COSA_DML_IP_LINK_TYPE, *PCOSA_DML_IP_LINK_TYPE;


typedef  enum
_COSA_DML_IP_IF_TYPE
{
    COSA_DML_IP_IF_TYPE_Normal          = 1,
    COSA_DML_IP_IF_TYPE_Loopback,
    COSA_DML_IP_IF_TYPE_Tunnel,
    COSA_DML_IP_IF_TYPE_Tunneled
}
COSA_DML_IP_IF_TYPE, *PCOSA_DML_IP_IF_TYPE;


typedef  enum
_COSA_DML_IP_ADDR_TYPE
{
    COSA_DML_IP_ADDR_TYPE_DHCP          = 1,
    COSA_DML_IP_ADDR_TYPE_AutoIP,
    COSA_DML_IP_ADDR_TYPE_IPCP,
    COSA_DML_IP_ADDR_TYPE_Static
}
COSA_DML_IP_ADDR_TYPE, *PCOSA_DML_IP_ADDR_TYPE;


typedef  enum
_COSA_DML_IP_PORT_STATUS
{
    COSA_DML_IP_PORT_STATUS_Listen      = 1,
    COSA_DML_IP_PORT_STATUS_Established
}
COSA_DML_IP_PORT_STATUS, *PCOSA_DML_IP_PORT_STATUS;

typedef enum
{
    COSA_DML_IP_ADDR_STATUS_Disabled = 1,
    COSA_DML_IP_ADDR_STATUS_Enabled,
    COSA_DML_IP_ADDR_STATUS_Error_Misconfigured,
    COSA_DML_IP_ADDR_STATUS_Error
}
COSA_DML_IP_ADDR_STATUS_TYPE;

typedef enum
{
    COSA_DML_IPV4V6_STATUS_Disabled = 1,
    COSA_DML_IPV4V6_STATUS_Enabled,
    COSA_DML_IPV4V6_STATUS_Error
}
COSA_DML_IPV4V6_STATUS_TYPE;

typedef enum
{
    COSA_DML_IP6_ADDRSTATUS_Preferred = 1,
    COSA_DML_IP6_ADDRSTATUS_Deprecated,
    COSA_DML_IP6_ADDRSTATUS_Invalid, 
    COSA_DML_IP6_ADDRSTATUS_Inaccessible, 
    COSA_DML_IP6_ADDRSTATUS_Unknown, 
    COSA_DML_IP6_ADDRSTATUS_Tentative, 
    COSA_DML_IP6_ADDRSTATUS_Duplicate, 
    COSA_DML_IP6_ADDRSTATUS_Optimistic 
}
COSA_DML_IP6_ADDRSTATUS_TYPE;

typedef enum
{
    COSA_DML_IP6_ORIGIN_AutoConfigured = 1,
    COSA_DML_IP6_ORIGIN_DHCPv6,
    COSA_DML_IP6_ORIGIN_WellKnown,
    COSA_DML_IP6_ORIGIN_Static
}
COSA_DML_IP6_ORIGIN_TYPE;

typedef enum
{
    COSA_DML_PREFIXENTRY_STATUS_Disabled = 1,
    COSA_DML_PREFIXENTRY_STATUS_Enabled,
    COSA_DML_PREFIXENTRY_STATUS_Error
}
COSA_DML_PREFIXENTRY_STATUS_TYPE;

typedef enum
{
    COSA_DML_IP6PREFIX_STATUS_Preferred = 1,
    COSA_DML_IP6PREFIX_STATUS_Deprecated,
    COSA_DML_IP6PREFIX_STATUS_Invalid,
    COSA_DML_IP6PREFIX_STATUS_Inaccessible,
    COSA_DML_IP6PREFIX_STATUS_Unknown
}
COSA_DML_IP6PREFIX_STATUS_TYPE;

typedef enum
{
    COSA_DML_IP6PREFIX_ORIGIN_AutoConfigured = 1,
    COSA_DML_IP6PREFIX_ORIGIN_PrefixDelegation,
    COSA_DML_IP6PREFIX_ORIGIN_RouterAdvertisement,
    COSA_DML_IP6PREFIX_ORIGIN_WellKnown,
    COSA_DML_IP6PREFIX_ORIGIN_Static,
    COSA_DML_IP6PREFIX_ORIGIN_Child
}
COSA_DML_IP6PREFIX_ORIGIN_TYPE;

typedef enum
{
    COSA_DML_IP6PREFIX_STATICTYPE_Static = 1,
    COSA_DML_IP6PREFIX_STATICTYPE_Inapplicable,
    COSA_DML_IP6PREFIX_STATICTYPE_PrefixDelegation,
    COSA_DML_IP6PREFIX_STATICTYPE_Child
}
COSA_DML_IP6PREFIX_STATICTYPE_TYPE;

/*
 *  Structure definitions for IP Interface
 */
typedef  struct
_COSA_DML_IP_IF_CFG
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    COSA_DML_LINK_TYPE              LinkType;           /* LinkType and LinkName/LinkInstNum constitutes LowerLayers */
    char                            LinkName[COSA_DML_IF_NAME_LENGTH];  /* This field is filled anyway */
    ULONG                           LinkInstNum;        /* Option 2, multi-LAN SB PSM design */
    ULONG                           MaxMTUSize;
    COSA_DML_IP_IF_TYPE             IfType;
    BOOLEAN                         Loopback;
    BOOLEAN                         AutoIPEnable;
    char                            RouterName[COSA_DML_IF_NAME_LENGTH];
    char                            LowerLayers[COSA_DML_LOWERLAYER_NAME_LENGTH];/* keep lowerlayer here*/
    /* Extensions */
    ULONG                           WanTTL;
    ULONG                           ArpCacheTimeout;
    BOOLEAN                         UpnpIgdEnabled;
#if defined (MULTILAN_FEATURE)
    BOOLEAN                         bIPv6Enabled;
#endif
}
COSA_DML_IP_IF_CFG,  *PCOSA_DML_IP_IF_CFG;


typedef  struct
_COSA_DML_IP_IF_INFO
{
    COSA_DML_IF_STATUS              Status;
    char                            Name[64];       /* netdev name in Linux */
    ULONG                           LastChange;
    int                             iana_t1;
    int                             iana_t2;
    ULONG                           iana_iaid;
    int                             iapd_t1;
    int                             iapd_t2;
    ULONG                           iapd_iaid;

}
COSA_DML_IP_IF_INFO,  *PCOSA_DML_IP_IF_INFO;


typedef  struct
_COSA_DML_IP_IF_FULL
{
    COSA_DML_IP_IF_CFG            Cfg;
    COSA_DML_IP_IF_INFO           Info;
}
COSA_DML_IP_IF_FULL, *PCOSA_DML_IP_IF_FULL;

typedef  struct
_COSA_DML_IP_V4ADDR
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    COSA_DML_IP_ADDR_STATUS_TYPE    Status;
    ANSC_IPV4_ADDRESS               IPAddress;
    ANSC_IPV4_ADDRESS               SubnetMask;
    COSA_DML_IP_ADDR_TYPE           AddressingType;
       
}
COSA_DML_IP_V4ADDR,  *PCOSA_DML_IP_V4ADDR;


typedef  struct
_COSA_DML_IP_STATS
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

    /* Warning: pointers to COSA_DML_IF_STATS (in cosa_dml_api_common.h) are
       cast to pointers to COSA_DML_IP_STATS (this struct) (e.g. in
       CosaDmlIpIfGetStats()) so the two structure must be kept in sync
    */
}
COSA_DML_IP_STATS, *PCOSA_DML_IP_STATS;


typedef  struct
_COSA_DML_IP_ACTIVE_PORT
{
    char                            LocalIPAddress[COSA_DML_IPV6_ADDR_LENGTH];
    char                            RemoteIPAddress[COSA_DML_IPV6_ADDR_LENGTH];
    USHORT                          LocalPort;
    USHORT                          RemotePort;
    COSA_DML_IP_PORT_STATUS         Status;
}
COSA_DML_IP_ACTIVE_PORT,  *PCOSA_DML_IP_ACTIVE_PORT;

/* Added by middlelayer implement */
typedef  struct
_COSA_DML_IP_IF_FULL2
{
    COSA_DML_IP_IF_CFG            Cfg;
    COSA_DML_IP_IF_INFO           Info;
    ULONG                         V6AddrPreviousVisitTime;
    ULONG                         V6PrefPreviousVisitTime;

    SLIST_HEADER                  IPV4List;
    SLIST_HEADER                  IPV6List;
    SLIST_HEADER                  IPV6PrefixList;
    ULONG                         ulNextIPV4InsNum;
    ULONG                         ulNextIPV6InsNum;
    ULONG                         ulNextIPV6PreInsNum;
}
COSA_DML_IP_IF_FULL2, *PCOSA_DML_IP_IF_FULL2;

typedef struct 
_COSA_DML_IP_V6ADDR
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    COSA_DML_IP_ADDR_STATUS_TYPE    Status;
    COSA_DML_IP6_ADDRSTATUS_TYPE    V6Status;
    char                            IP6Address[64];
    int                             iana_pretm;
    int                             iana_vldtm;    
    COSA_DML_IP6_ORIGIN_TYPE        Origin;
    char                            Prefix[128];
    INT                             X_Comcast_com_LeaseTime;
    char                            PreferredLifetime[64];
    char                            ValidLifetime[64];
    BOOLEAN                         bAnycast;
    BOOL                            bFound;     
}
COSA_DML_IP_V6ADDR,  *PCOSA_DML_IP_V6ADDR;

typedef struct
_COSA_DML_IP_V6PREFIX
{
    ULONG                               InstanceNumber;
    char                                Alias[COSA_DML_IF_NAME_LENGTH];

    BOOLEAN                             bEnabled;
    COSA_DML_PREFIXENTRY_STATUS_TYPE    Status;
    COSA_DML_IP6PREFIX_STATUS_TYPE      PrefixStatus;
    char                                Prefix[64];
    int                                 iapd_pretm;
    int                                 iapd_vldtm;

    COSA_DML_IP6PREFIX_ORIGIN_TYPE      Origin;
    COSA_DML_IP6PREFIX_STATICTYPE_TYPE  StaticType;
    char                                ParentPrefix[128];
    char                                ChildPrefixBits[64];
    BOOLEAN                             bOnlink;
    BOOLEAN                             bAutonomous;
    char                                PreferredLifetime[64];
    char                                ValidLifetime[64];
    BOOL                                bFound;     
}
COSA_DML_IP_V6PREFIX, *PCOSA_DML_IP_V6PREFIX;

#define MAX_IPV6_ENTRY_NUM   20
#if defined (MULTILAN_FEATURE)
#define MAX_IPV4_ENTRY_NUM   20
#endif
typedef  struct
_COSA_PRI_IP_IF_FULL
{
    COSA_DML_IP_IF_CFG            Cfg;
    COSA_DML_IP_IF_INFO           Info;

    USHORT                        ulNumOfV4Addr;
    USHORT                        ulNumOfV6Addr;
    USHORT                        ulNumOfV6Pre;
#if defined (MULTILAN_FEATURE)
    COSA_DML_IP_V4ADDR            V4AddrList[MAX_IPV4_ENTRY_NUM];
#else
    COSA_DML_IP_V4ADDR            V4AddrList[10];
#endif
    COSA_DML_IP_V6ADDR            V6AddrList[MAX_IPV6_ENTRY_NUM];
    COSA_DML_IP_V6PREFIX          V6PreList[MAX_IPV6_ENTRY_NUM];
}
COSA_PRI_IP_IF_FULL, *PCOSA_PRI_IP_IF_FULL;

#define _DML_V6ADDR_ADDR_CHANGE 1
#define _DML_V6ADDR_ENABLE_CHANGE (1<<1)
#define _DML_V6ADDR_PREFIX_CHANGE (1<<2)
#define _DML_V6ADDR_PREFERED_LFT_CHANGE (1<<4)
#define _DML_V6ADDR_VALID_LFT_CHANGE (1<<5)
#define _DML_V6ADDR_ANYCAST_CHANGE (1<<6)

/*************************************
    The actual function declaration 
**************************************/
int CosaGetUsgIfNum();

ANSC_STATUS
CosaDmlIpInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

void get_uptime(long *uptime);

/*
 *  IP Interface
 */

ULONG
CosaDmlIpIfGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    );
    

ANSC_STATUS
CosaDmlIpIfGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_IP_IF_FULL        pEntry
    );

ANSC_STATUS
CosaDmlIpIfSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

ANSC_STATUS
CosaDmlIpIfAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_IP_IF_FULL        pEntry
    );



ANSC_STATUS
CosaDmlIpIfDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

ANSC_STATUS
CosaDmlIpIfSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_IP_IF_CFG         pCfg
    );

ANSC_STATUS
CosaDmlIpIfGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_IP_IF_CFG         pCfg
    );

ANSC_STATUS
CosaDmlIpIfGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_IP_IF_INFO        pInfo
    );

ANSC_STATUS
CosaDmlIpIfReset
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

/*
 *  IP Interface IPv4Address
 */
ULONG
CosaDmlIpIfGetNumberOfV4Addrs
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber
    );

ANSC_STATUS
CosaDmlIpIfGetV4Addr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_IP_V4ADDR         pEntry
    );

ANSC_STATUS
CosaDmlIpIfSetV4AddrValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );
ANSC_STATUS
CosaDmlIpIfAddV4Addr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V4ADDR         pEntry
    );

ANSC_STATUS
CosaDmlIpIfDelV4Addr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V4ADDR         pEntry
    );

ANSC_STATUS
CosaDmlIpIfSetV4Addr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V4ADDR         pEntry
    );

ANSC_STATUS
CosaDmlIpIfGetV4Addr2
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V4ADDR         pEntry          /* Identified by InstanceNumber */
    );


/*
 *  IP Interface IPv6Address
 */

/* TBC -- horrible casual design of such a SBAPI function*/
PCOSA_DML_IP_V6ADDR
CosaDmlIPGetIPv6Addresses
    (
        PCOSA_DML_IP_IF_FULL2       p_ipif,
        PULONG                      p_num
    );

ANSC_STATUS
CosaDmlIpIfAddV6Addr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6ADDR         pEntry
    );

ANSC_STATUS
CosaDmlIpIfDelV6Addr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6ADDR         pEntry
    );

ANSC_STATUS
CosaDmlIpIfSetV6Addr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6ADDR         pEntry
    );

ANSC_STATUS
CosaDmlIpIfGetV6Addr2
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6ADDR         pEntry          /* Identified by InstanceNumber */
    );

/*
 *  IP Interface IPv6Prefix
 */

/* TBC -- Definition of this SBAPI is not consistent with general SBAPI method */
PCOSA_DML_IP_V6PREFIX
CosaDmlIPGetIPv6Prefixes
    (
        PCOSA_DML_IP_IF_FULL2       p_ipif,
        PULONG                      p_num
    );

ULONG
CosaDmlIpIfGetNumberOfV6Prefixs
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber
    );

ANSC_STATUS
CosaDmlIpIfGetV6Prefix
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_IP_V6PREFIX       pEntry
    );

ANSC_STATUS
CosaDmlIpIfSetV6PrefixValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

ANSC_STATUS
CosaDmlIpIfAddV6Prefix
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6PREFIX       pEntry
    );

ANSC_STATUS
CosaDmlIpIfDelV6Prefix
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6PREFIX       pEntry
    );

ANSC_STATUS
CosaDmlIpIfSetV6Prefix
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6PREFIX       pEntry
    );

ANSC_STATUS
CosaDmlIpIfGetV6Prefix2
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6PREFIX       pEntry          /* Identified by InstanceNumber */
    );

/*
 *  Helper functions, but in the position of SBAPI and called by middle layer code
 */
#ifndef _COSA_SIM_

/* Should NOT be a SBAPI function, but a helper function */
int 
CosaDmlGetPrefixPathName
    (
        char * ifname, 
        int inst1,
        PCOSA_DML_IP_V6ADDR p_v6addr,
        char * p_val
    );

ULONG
CosaDmlIPv6addrGetV6Status
    (
        PCOSA_DML_IP_V6ADDR p_dml_v6addr, 
        PCOSA_DML_IP_IF_FULL2 p_ipif
    );

/* Should NOT be a SBAPI function */
int
CosaDmlIpv6AddrMatchesPrefix
    (
        char * pref_path,
        char * v6addr, 
        int ipif_inst_num
    );

/* This is definitely NOT a SBAPI function */
int 
CosaDmlDateTimeCompare
    (
        char *p_dt1, 
        char *p_dt2
    );
#endif


/*
 *  IP Interface statistics
 */
ANSC_STATUS
CosaDmlIpIfGetStats
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_STATS          pStats
    );

/*
 *  IP Active Ports
 */
PCOSA_DML_IP_ACTIVE_PORT
CosaDmlIpGetActivePorts
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount
    );

PCOSA_DML_IP_IF_INFO
CosaDmlIpIfGetEntry2
    (
        ANSC_HANDLE                 hContext,
        ULONG                       InstanceNumber
    );
#if defined(FEATURE_RDKB_CONFIGURABLE_WAN_INTERFACE)
ANSC_STATUS CosaUpdateIfname(int Index, char *Ifname);
#endif    
#endif

