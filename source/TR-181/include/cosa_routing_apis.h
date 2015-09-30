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

    module: cosa_routing_apis.h

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


#ifndef  _COSA_ROUTING_APIS_H
#define  _COSA_ROUTING_APIS_H

#include "../middle_layer_src/cosa_apis.h"
#include "poam_irepfo_interface.h"
#include "sys_definitions.h"
#include "../middle_layer_src/plugin_main_apis.h"
#include "cosa_ip_apis.h"
#include "dml_tr181_custom_cfg.h"

/***********************************
    Actual definition declaration
************************************/

#define MAXINSTANCE 128

typedef  enum
_COSA_DML_ROUTING_STATUS
{
    COSA_DML_ROUTING_STATUS_Enabled     = 1,
    COSA_DML_ROUTING_STATUS_Disabled,
    COSA_DML_ROUTING_STATUS_Error
}
COSA_DML_ROUTING_STATUS, *PCOSA_DML_ROUTING_STATUS;

typedef  enum
_COSA_DML_ROUTING_V4_ORIGIN
{
    COSA_DML_ROUTING_IPV4_ORIGIN_DHCPv4     = 1,
    COSA_DML_ROUTING_IPV4_ORIGIN_OSPF,
    COSA_DML_ROUTING_IPV4_ORIGIN_IPCP,
    COSA_DML_ROUTING_IPV4_ORIGIN_RIP,
    COSA_DML_ROUTING_IPV4_ORIGIN_Static
}
COSA_DML_ROUTING_V4_ORIGIN, *PCOSA_DML_ROUTING_V4_ORIGIN;

typedef  enum
_COSA_DML_ROUTING_V6_ORIGIN
{
    COSA_DML_ROUTING_IPV6_ORIGIN_DHCPv6     = 1,
    COSA_DML_ROUTING_IPV6_ORIGIN_OSPF,
    COSA_DML_ROUTING_IPV6_ORIGIN_RA,
    COSA_DML_ROUTING_IPV6_ORIGIN_RIPng,
    COSA_DML_ROUTING_IPV6_ORIGIN_Static
}
COSA_DML_ROUTING_V6_ORIGIN, *PCOSA_DML_ROUTING_V6_ORIGIN;

typedef  enum
_COSA_DML_RIP_MODE
{
    COSA_DML_RIP_MODE_Send              = 1,
    COSA_DML_RIP_MODE_Receive,
    COSA_DML_RIP_MODE_Both
}
COSA_DML_RIP_MODE, *PCOSA_DML_RIP_MODE;

/*
 *  Structure definitions for Routing Router
 */
typedef  struct
_COSA_DML_ROUTER_CFG
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    BOOLEAN                         bEnabled;
}
COSA_DML_ROUTER_CFG,  *PCOSA_DML_ROUTER_CFG;


typedef  struct
_COSA_DML_ROUTER_INFO
{
    COSA_DML_ROUTING_STATUS         Status;
}
COSA_DML_ROUTER_INFO,  *PCOSA_DML_ROUTER_INFO;


typedef  struct
_COSA_DML_ROUTER_FULL
{
    COSA_DML_ROUTER_CFG             Cfg;
    COSA_DML_ROUTER_INFO            Info;
}
COSA_DML_ROUTER_FULL, *PCOSA_DML_ROUTER_FULL;

typedef  struct
_COSA_DML_ROUTING_V4_ENTRY
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    BOOLEAN                         Enable;
    COSA_DML_ROUTING_STATUS         Status;             /* Read-only */
    BOOLEAN                         StaticRoute;        /* Read-only */
    ANSC_IPV4_ADDRESS               DestIPAddress;
    ANSC_IPV4_ADDRESS               DestSubnetMask;
    int                             ForwardingPolicy;   /* Not supported for now */
    ANSC_IPV4_ADDRESS               GatewayIPAddress;
    char                            Interface[64];      /* IP interface name */
    int                             ForwardingMetric;
    COSA_DML_ROUTING_V4_ORIGIN      Origin;                                
    BOOLEAN                         X_CISCO_COM_RIPAdvertise;
}
COSA_DML_ROUTING_V4_ENTRY,  *PCOSA_DML_ROUTING_V4_ENTRY;

typedef  struct
_COSA_DML_ROUTING_V6_ENTRY
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    BOOLEAN                         Enable;
    COSA_DML_ROUTING_STATUS         Status;                 /* Read-only */
    
    char                            DestIPPrefix[64];
    int                             ForwardingPolicy;       /* Not supported for now */
    char                            NextHop[COSA_DML_IPV6_ADDR_LENGTH];
    char                            Interface[COSA_DML_IF_NAME_LENGTH]; /* IP interface name */
    COSA_DML_ROUTING_V6_ORIGIN      Origin;                 /* Read-only */
    int                             ForwardingMetric;
    char                            ExpirationTime[64];         /* Read-only */
}
COSA_DML_ROUTING_V6_ENTRY,  *PCOSA_DML_ROUTING_V6_ENTRY;

typedef  struct
_COSA_DML_RIP_CFG
{
    BOOLEAN                         Enable;
    COSA_DML_RIP_MODE               Mode;
    ULONG                           X_CISCO_COM_UpdateInterval;
    ULONG                           X_CISCO_COM_DefaultMetric;
}
COSA_DML_RIP_CFG, *PCOSA_DML_RIP_CFG;

typedef  struct
_COSA_DML_RIP_IF_CFG
{
    /*readonly*/
    ULONG                           InstanceNumber;
    COSA_DML_STATUS                 Status;

    /*writable */
    CHAR                            Alias[COSA_DML_IF_NAME_LENGTH];

    CHAR                            Interface[COSA_DML_IF_NAME_LENGTH]; /* IP interface name */
    BOOLEAN                         Enable;
    
    BOOLEAN                         AcceptRA;
    BOOLEAN                         SendRA;
    USHORT                          X_CISCO_COM_SendVersion;
    USHORT                          X_CISCO_COM_ReceiveVersion;
    
    ULONG                           X_CISCO_COM_Neighbor;
    
    ULONG                           X_CISCO_COM_AuthenticationType;
    ULONG                           X_CISCO_COM_Md5KeyID;    
    CHAR                            X_CISCO_COM_Md5KeyValue[128];
    CHAR                            X_CISCO_COM_SimplePassword[128];
}
COSA_DML_RIP_IF_CFG, *PCOSA_DML_RIP_IF_CFG;

#define COSA_DML_ROUTEINFO_IFNAME                   CFG_TR181_RouteInfo_IfName

#if CFG_TR181_ps_no_arg

#define COSA_DML_CMD_PS           "ps |grep %s|grep -v grep "

#else

#define COSA_DML_CMD_PS           "ps -A|grep %s|grep -v grep "

#endif


typedef  enum
_COSA_DML_ROUTEINFO_PRF_TYPE
{
    COSA_DML_ROUTEINFO_PRF_High     = 1,
    COSA_DML_ROUTEINFO_PRF_Medium,     
    COSA_DML_ROUTEINFO_PRF_Low     
}
COSA_DML_ROUTEINFO_PRF_TYPE, *PCOSA_DML_ROUTEINFO_PRF_TYPE;

typedef  enum
_COSA_DML_ROUTEINFO_STATUS_TYPE
{
    COSA_DML_ROUTEINFO_STATUS_ForwardingEntryCreated = 1,
    COSA_DML_ROUTEINFO_STATUS_NoForwardingEntry ,
    COSA_DML_ROUTEINFO_STATUS_Error ,
}
COSA_DML_ROUTEINFO_STATUS_TYPE, *PCOSA_DML_ROUTEINFO_STATUS_TYPE;

typedef  struct
_COSA_DML_ROUTEINFO_CFG
{
    BOOLEAN                         bEnabled;
}
COSA_DML_ROUTEINFO_CFG, *PCOSA_DML_ROUTEINFO_CFG;

typedef  struct
_COSA_DML_ROUTEINFO_IF_INFO
{
    char                            Interface[256]; /* IP interface name */
    char                            SourceRouter[64];
    char                            Prefix[64];
    char                            RouteLifetime[32];
    COSA_DML_ROUTEINFO_PRF_TYPE     PreferredRouteFlag;
    COSA_DML_ROUTEINFO_STATUS_TYPE            Status;
}
COSA_DML_ROUTEINFO_IF_INFO,  *PCOSA_DML_ROUTEINFO_IF_INFO;

typedef  struct
_COSA_DML_ROUTEINFO_FULL
{
    COSA_DML_ROUTEINFO_CFG                Cfg;
    ULONG                                 IfNum;
    COSA_DML_ROUTEINFO_IF_INFO            * pInfo;
}
COSA_DML_ROUTEINFO_FULL, *PCOSA_DML_ROUTEINFO_FULL;

/* Added by middlelayer implement */

typedef  struct
_COSA_DML_ROUTER_FULL2
{
    COSA_DML_ROUTER_CFG             Cfg;
    COSA_DML_ROUTER_INFO            Info;
    
    SLIST_HEADER                    ForwardList;
    ULONG                           ulNextForwardInsNum;

    SLIST_HEADER                    IPv6ForwardList;
    ULONG                           ulNextIPv6ForwardInsNum;
}
COSA_DML_ROUTER_FULL2, *PCOSA_DML_ROUTER_FULL2;

#define MAX_ROUTING_LIST_NUM 10
typedef  struct
_COSA_PRI_ROUTER_FULL
{
    COSA_DML_ROUTER_CFG             Cfg;
    COSA_DML_ROUTER_INFO            Info;
    
    USHORT                          ulNumOfForward;
    COSA_DML_ROUTING_V4_ENTRY       V4ForwardList[MAX_ROUTING_LIST_NUM];

    USHORT                          ulNumOfIPv6Forward;
    COSA_DML_ROUTING_V6_ENTRY       V6ForwardList[MAX_ROUTING_LIST_NUM];

}
COSA_PRI_ROUTER_FULL, *PCOSA_PRI_ROUTER_FULL;


/*The following is for RIPV1/V2*/

#if ( defined(_COSA_SIM_))
#define COSA_RIPD_TMP_CONF      "./ripd_tmp.conf"
#define COSA_RIPD_CUR_CONF      "./ripd.conf"
#define COSA_ZEBRA_TMP_CONF     "./zebra_tmp.conf"
#define COSA_ZEBRA_CUR_CONF     "./zebra.conf"
#else
#define COSA_RIPD_TMP_CONF      "/etc/ripd_tmp.conf"
#define COSA_RIPD_CUR_CONF      "/etc/ripd.conf"
#define COSA_ZEBRA_TMP_CONF     "/etc/zebra_tmp.conf"
#define COSA_ZEBRA_CUR_CONF     "/etc/zebra.conf"
#endif

#define COSA_ZEBRA_BIN          "zebra"
#define COSA_RIPD_BIN           "ripd"

#define COSA_RIPD_IF1_NAME      "erouter0"
#define COSA_RIPD_IF2_NAME      "wan0"


enum COSA_RIP_AUTHEN
{
    COSA_RIP_AUTHEN_NONE = 1,
    COSA_RIP_AUTHEN_TEXT,
    COSA_RIP_AUTHEN_MD5,
    COSA_RIP_AUTHEN_md5
};

enum COSA_RIP_VERSION
{
    COSA_RIP_VERSION_1 = 1,
    COSA_RIP_VERSION_2,
    COSA_RIP_VERSION_1_2
};

typedef struct _COSA_DML_RIPD_CONF
{
    /*Global Version */
    BOOL   Enable;
    ULONG  Version;
    ULONG  UpdateTime;
    ULONG  TimoutTime;
    ULONG  CollectionTime;
    ULONG  ConnectedMetric;
    ULONG  DefaultMetric;

    /*Interface Parameters*/
    BOOL   If1Enable;
    BOOL   If1SendEnable;
    BOOL   If1ReceiveEnable;
    CHAR   If1Alias[16];
    CHAR   If1Name[16];
    CHAR   If1Md5KeyValue[32];
    CHAR   If1SimplePassword[32];
    CHAR   If1KeyChainName[16];
    ULONG  If1AuthenticateType;
    ULONG  If1KeyID;
    ULONG  If1SendVersion;
    ULONG  If1ReceiveVersion;
    ULONG  If1Neighbor;
    
}COSA_DML_RIPD_CONF,*PCOSA_DML_RIPD_CONF;


#define SETS_INTO_UTOPIA( uniqueName, table1Name, table1Index, table2Name, table2Index, parameter, value ) \
{  \
    CHAR  Namespace[128] = {0}; \
    _ansc_snprintf(&Namespace[0], sizeof(Namespace)-1, "%s%s%lu%s%lu", uniqueName, table1Name, (ULONG)table1Index, table2Name, (ULONG)table2Index ); \
    Utopia_RawSet(&utctx,&Namespace[0],parameter,value);  \
} \

#define SETI_INTO_UTOPIA( uniqueName, table1Name, table1Index, table2Name, table2Index, parameter, value ) \
{  \
    CHAR  Namespace[128]  = {0}; \
    CHAR  Value[12]  = {0}; \
    _ansc_snprintf(&Namespace[0], sizeof(Namespace)-1, "%s%s%lu%s%lu", uniqueName, table1Name, (ULONG)table1Index, table2Name, (ULONG)table2Index ); \
    _ansc_snprintf(&Value[0], sizeof(Value)-1, "%lu", (ULONG)value ); \
    Utopia_RawSet(&utctx,&Namespace[0],parameter,Value);  \
} \

#define UNSET_FROM_UTOPIA( uniqueName, table1Name, table1Index, table2Name, table2Index, parameter ) \
{  \
    CHAR  Namespace[128]  = {0}; \
    _ansc_snprintf(&Namespace[0], sizeof(Namespace)-1, "%s%s%lu%s%lu", uniqueName, table1Name, (ULONG)table1Index, table2Name, (ULONG)table2Index ); \
    syscfg_unset(&Namespace[0],parameter);  \
} \

#define GETS_FROM_UTOPIA( uniqueName, table1Name, table1Index, table2Name, table2Index, parameter, out ) \
{  \
    CHAR  Namespace[128] = {0}; \
    _ansc_snprintf(&Namespace[0], sizeof(Namespace)-1, "%s%s%lu%s%lu", uniqueName, table1Name, (ULONG)table1Index, table2Name, (ULONG)table2Index ); \
    Utopia_RawGet(&utctx,&Namespace[0], parameter, out,sizeof(out));  \
} \

#define GETI_FROM_UTOPIA( uniqueName, table1Name, table1Index, table2Name, table2Index, parameter, out ) \
{  \
    CHAR  Namespace[128] = {0}; \
    CHAR  Value[12]  = {0}; \
    _ansc_snprintf(&Namespace[0], sizeof(Namespace)-1, "%s%s%lu%s%lu", uniqueName, table1Name, (ULONG)table1Index, table2Name, (ULONG)table2Index ); \
    Utopia_RawGet(&utctx,&Namespace[0], parameter,Value,sizeof(Value));  \
    if ( Value[0] ) out = strtoul(Value, (char **)NULL, 10); \
}  \



typedef  struct
_COSA_DML_STATICROUTE_CFG
{
    BOOLEAN   RIPAdvertise;
    ULONG     DestIPAddress;       
    ULONG     DestSubnetMask;      
    ULONG     GatewayIPAddress;    
    char      Name[64];            
}
COSA_DML_STATICROUTE_CFG,  *PCOSA_DML_STATICROUTE_CFG;


/*************************************
    The actual function declaration 
**************************************/
ANSC_STATUS
CosaDmlRoutingInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
);

/*
 *  Routing Router -- assuming there is only one router in the system
 */

ANSC_STATUS
CosaDmlRoutingRouterSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTER_CFG        pCfg
    );

ANSC_STATUS
CosaDmlRoutingRouterGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTER_CFG        pCfg
    );

ANSC_STATUS
CosaDmlRoutingRouterGetInfo
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTER_INFO       pInfo
    );

/*
 *  Routing IPv4 Forwarding
 */
ULONG
CosaDmlRoutingGetNumberOfV4Entries
    (
        ANSC_HANDLE                 hContext
    );

ANSC_STATUS
CosaDmlRoutingGetV4Entry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_ROUTING_V4_ENTRY  pEntry
    );

ANSC_STATUS
CosaDmlRoutingSetV4EntryValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

ANSC_STATUS
CosaDmlRoutingAddV4Entry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V4_ENTRY  pEntry
    );

ANSC_STATUS
CosaDmlRoutingDelV4Entry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V4_ENTRY  pEntry
    );

ANSC_STATUS
CosaDmlRoutingSetV4Entry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V4_ENTRY  pEntry
    );

ANSC_STATUS
CosaDmlRoutingGetV4Entry2
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V4_ENTRY  pEntry     
    );


/*
 *  Routing IPv6 Forwarding
 */
ULONG
CosaDmlRoutingGetNumberOfV6Entries
    (
        ANSC_HANDLE                 hContext
    );

ANSC_STATUS
CosaDmlRoutingGetV6Entry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_ROUTING_V6_ENTRY  pEntry
    );

ANSC_STATUS
CosaDmlRoutingSetV6EntryValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

ANSC_STATUS
CosaDmlRoutingAddV6Entry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V6_ENTRY  pEntry
    );

ANSC_STATUS
CosaDmlRoutingDelV6Entry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V6_ENTRY  pEntry      /* Identified by InstanceNumber */
    );

ANSC_STATUS
CosaDmlRoutingSetV6Entry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V6_ENTRY  pEntry      /* Identified by InstanceNumber */
    );

ANSC_STATUS
CosaDmlRoutingGetV6Entry2
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ROUTING_V6_ENTRY  pEntry      /* Identified by InstanceNumber */
    );

/*
 *  RIP
 */
ANSC_STATUS
CosaDmlRipGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RIP_CFG           pCfg
    );

ANSC_STATUS
CosaDmlRipSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RIP_CFG           pCfg
    );

/*
 *  RIP interface
 */
ULONG
CosaDmlRipGetNumberOfIfEntries
    (
        ANSC_HANDLE                 hContext
    );

ANSC_STATUS
CosaDmlRipIfGetCfg
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_RIP_IF_CFG        pEntry
    );

ANSC_STATUS
CosaDmlRipIfSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RIP_IF_CFG        pEntry     
    );

ANSC_STATUS
CosaDmlRipIfAddCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RIP_IF_CFG        pEntry
    );

ANSC_STATUS
CosaDmlRipIfDelCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RIP_IF_CFG        pEntry
    );


ANSC_STATUS
CosaDmlRoutingRemove
    (
        ANSC_HANDLE                 hContext
    );

PCOSA_DML_ROUTEINFO_IF_INFO
CosaDmlRoutingGetRouteInfoIf
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount
    );

ANSC_STATUS
CosaDmlRouteInfoSetEnabled
    (
        BOOLEAN value
    );

ANSC_STATUS
CosaDmlRouteInfoGetEnabled
    (
        BOOLEAN * pBool
    );

#endif
