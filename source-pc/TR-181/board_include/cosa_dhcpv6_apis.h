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

    module:	dml_api_dhcp.h

        For Data Model Library Implementation (DML),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc.
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structure and function
        prototypes for DML DHCP API.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Ding Hua

    ---------------------------------------------------------------

    revision:

        12/15/2010    initial revision.

**********************************************************************/


#ifndef  _DML_API_DHCPV6_
#define  _DML_API_DHCPV6_

#include "../middle_layer_src/cosa_apis.h"
#include "cosa_dhcpv4_apis.h"
#include "dml_tr181_custom_cfg.h"

/**********************************************************************
                STRUCTURE AND CONSTANT DEFINITIONS
**********************************************************************/

#define  COSA_DML_DHCP_MAX_ENTRIES                  4
#define  COSA_DML_DHCP_MAX_RESERVED_ADDRESSES       8
#define  COSA_DML_DHCP_MAX_OPT_ENTRIES              8

#define _DEBUG_DHCPV6
#ifdef _DEBUG_DHCPV6
    #define ULOGF ulogf
#else   
    #define ULOGF
#endif

#define COSA_DML_DHCPV6_CLIENT_IFNAME                 CFG_TR181_DHCPv6_CLIENT_IfName
#define COSA_DML_DHCPV6_SERVER_IFNAME                 CFG_TR181_DHCPv6_SERVER_IfName

#define COSA_DML_DHCPV6C_PREF_SYSEVENT_NAME           "tr_"COSA_DML_DHCPV6_CLIENT_IFNAME"_dhcpv6_client_v6pref"
#define COSA_DML_DHCPV6C_PREF_IAID_SYSEVENT_NAME      "tr_"COSA_DML_DHCPV6_CLIENT_IFNAME"_dhcpv6_client_pref_iaid"
#define COSA_DML_DHCPV6C_PREF_T1_SYSEVENT_NAME        "tr_"COSA_DML_DHCPV6_CLIENT_IFNAME"_dhcpv6_client_pref_t1"
#define COSA_DML_DHCPV6C_PREF_T2_SYSEVENT_NAME        "tr_"COSA_DML_DHCPV6_CLIENT_IFNAME"_dhcpv6_client_pref_t2"
#define COSA_DML_DHCPV6C_PREF_PRETM_SYSEVENT_NAME     "tr_"COSA_DML_DHCPV6_CLIENT_IFNAME"_dhcpv6_client_pref_pretm"
#define COSA_DML_DHCPV6C_PREF_VLDTM_SYSEVENT_NAME     "tr_"COSA_DML_DHCPV6_CLIENT_IFNAME"_dhcpv6_client_pref_vldtm"

#define COSA_DML_DHCPV6C_ADDR_SYSEVENT_NAME           "tr_"COSA_DML_DHCPV6_CLIENT_IFNAME"_dhcpv6_client_v6addr"
#define COSA_DML_DHCPV6C_ADDR_IAID_SYSEVENT_NAME      "tr_"COSA_DML_DHCPV6_CLIENT_IFNAME"_dhcpv6_client_addr_iaid"
#define COSA_DML_DHCPV6C_ADDR_T1_SYSEVENT_NAME        "tr_"COSA_DML_DHCPV6_CLIENT_IFNAME"_dhcpv6_client_addr_t1"
#define COSA_DML_DHCPV6C_ADDR_T2_SYSEVENT_NAME        "tr_"COSA_DML_DHCPV6_CLIENT_IFNAME"_dhcpv6_client_addr_t2"
#define COSA_DML_DHCPV6C_ADDR_PRETM_SYSEVENT_NAME     "tr_"COSA_DML_DHCPV6_CLIENT_IFNAME"_dhcpv6_client_addr_pretm"
#define COSA_DML_DHCPV6C_ADDR_VLDTM_SYSEVENT_NAME     "tr_"COSA_DML_DHCPV6_CLIENT_IFNAME"_dhcpv6_client_addr_vldtm"

#define COSA_DML_DHCPV6S_ADDR_SYSEVENT_NAME      "tr_"COSA_DML_DHCPV6_SERVER_IFNAME"_dhcpv6_server_v6addr"
/*
 *  DHCP Client
 */
typedef  struct
_COSA_DML_DHCPCV6_SVR
{
    UCHAR                           SourceAddress[40];
    UCHAR                           DUID[131]; /* IP interface name */
    UCHAR                           InformationRefreshTime[32];
}
COSA_DML_DHCPCV6_SVR,  *PCOSA_DML_DHCPCV6_SVR;

typedef  struct
_COSA_DML_DHCPCV6_CFG
{
    ULONG                           InstanceNumber;
    UCHAR                           Alias[COSA_DML_ALIAS_NAME_LENGTH];
    LONG                            SuggestedT1;
    LONG                            SuggestedT2;
    UCHAR                           Interface[COSA_DML_ALIAS_NAME_LENGTH]; /* IP interface name */
    UCHAR                           RequestedOptions[512];
    BOOLEAN                         bEnabled;
    BOOLEAN                         RequestAddresses;
    BOOLEAN                         RequestPrefixes;
    BOOLEAN                         RapidCommit;
    BOOLEAN                         Renew;
}
COSA_DML_DHCPCV6_CFG,  *PCOSA_DML_DHCPCV6_CFG;

typedef struct
_COSA_DML_DHCPCV6_INFO
{
    COSA_DML_DHCP_STATUS            Status;
    UCHAR                           SupportedOptions[512];
    UCHAR                           DUID[131];
}
COSA_DML_DHCPCV6_INFO,  *PCOSA_DML_DHCPCV6_INFO;


typedef  struct
_COSA_DML_DHCPCV6_FULL
{
    COSA_DML_DHCPCV6_CFG              Cfg;
    COSA_DML_DHCPCV6_INFO             Info;
}
COSA_DML_DHCPCV6_FULL, *PCOSA_DML_DHCPCV6_FULL;

typedef  struct
_COSA_DML_DHCPCV6_SENT
{
    ULONG                           InstanceNumber;
    UCHAR                           Alias[COSA_DML_ALIAS_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    ULONG                           Tag;
    UCHAR                           Value[255];
}
COSA_DML_DHCPCV6_SENT,  *PCOSA_DML_DHCPCV6_SENT;

struct
_COSA_DML_DHCPCV6_RECV
{
    SINGLE_LINK_ENTRY               Link;
    ULONG                           Tag;
    UCHAR                           Server[255];
    UCHAR                           Value[1024];
};
typedef struct _COSA_DML_DHCPCV6_RECV COSA_DML_DHCPCV6_RECV,  *PCOSA_DML_DHCPCV6_RECV;

#define  ACCESS_DHCPV6_RECV_LINK_OBJECT(p)              \
         ACCESS_CONTAINER(p, COSA_DML_DHCPCV6_RECV, Link)

/*
 *  DHCP Server
 */
struct
_COSA_DML_DHCPSV6_CFG
{
    BOOLEAN                         bEnabled;
};
typedef struct _COSA_DML_DHCPSV6_CFG COSA_DML_DHCPSV6_CFG,  *PCOSA_DML_DHCPSV6_CFG;


struct
_COSA_DML_DHCPSV6_POOL_CFG
{
    ULONG                           InstanceNumber;
    UCHAR                           Alias[COSA_DML_ALIAS_NAME_LENGTH];

    ULONG                           Order;
    char                            Interface[COSA_DML_ALIAS_NAME_LENGTH];         /* IP interface name */
    char                            VendorClassID[256];
    UCHAR                           UserClassID[256];
    UCHAR                           SourceAddress[40];
    UCHAR                           SourceAddressMask[40];
    UCHAR                           IANAManualPrefixes[320];
    UCHAR                           IAPDManualPrefixes[320];
    UCHAR                           PrefixRangeBegin[64];
    UCHAR                           PrefixRangeEnd[64];
    UCHAR                           StartAddress[64];
    signed long                     LeaseTime;
    ULONG                           IAPDAddLength;
    ULONG                           IANAAmount;
    UCHAR                           DUID[131];
    BOOLEAN                         IAPDEnable;
    BOOLEAN                         SourceAddressExclude;
    BOOLEAN                         IANAEnable;
    BOOLEAN                         UserClassIDExclude;
    BOOLEAN                         VendorClassIDExclude;
    BOOLEAN                         DUIDExclude;
    BOOLEAN                         bEnabled;
    BOOLEAN                         UnicastEnable;
    BOOLEAN                         RapidEnable;
    BOOLEAN                         EUI64Enable;    
};
typedef struct _COSA_DML_DHCPSV6_POOL_CFG COSA_DML_DHCPSV6_POOL_CFG,  *PCOSA_DML_DHCPSV6_POOL_CFG;


struct
_COSA_DML_DHCPSV6_POOL_INFO
{
    COSA_DML_DHCP_STATUS            Status;
    UCHAR                           IANAPrefixes[320];
    UCHAR                           IAPDPrefixes[320];
};
typedef  struct _COSA_DML_DHCPSV6_POOL_INFO COSA_DML_DHCPSV6_POOL_INFO,  *PCOSA_DML_DHCPSV6_POOL_INFO;


struct
_COSA_DML_DHCPSV6_POOL_FULL
{
    COSA_DML_DHCPSV6_POOL_CFG    Cfg;
    COSA_DML_DHCPSV6_POOL_INFO   Info;
};
typedef  struct _COSA_DML_DHCPSV6_POOL_FULL COSA_DML_DHCPSV6_POOL_FULL, *PCOSA_DML_DHCPSV6_POOL_FULL;


struct
_COSA_DML_DHCPSV6_CLIENT
{
    char                            Alias[COSA_DML_ALIAS_NAME_LENGTH];

    UCHAR                           SourceAddress[40];
    BOOLEAN                         Active;
};
typedef  struct _COSA_DML_DHCPSV6_CLIENT COSA_DML_DHCPSV6_CLIENT,  *PCOSA_DML_DHCPSV6_CLIENT;

struct
_COSA_DML_DHCPSV6_CLIENT_IPV6ADDRESS
{
    UCHAR                           IPAddress[40];
    UCHAR                           PreferredLifetime[32];
    UCHAR                           ValidLifetime[32];
};
typedef  struct _COSA_DML_DHCPSV6_CLIENT_IPV6ADDRESS COSA_DML_DHCPSV6_CLIENT_IPV6ADDRESS,  *PCOSA_DML_DHCPSV6_CLIENT_IPV6ADDRESS;

struct
_COSA_DML_DHCPSV6_CLIENT_IPV6PREFIX
{
    UCHAR                           Prefix[40];
    UCHAR                           PreferredLifetime[32];
    UCHAR                           ValidLifetime[32];
};
typedef  struct _COSA_DML_DHCPSV6_CLIENT_IPV6PREFIX COSA_DML_DHCPSV6_CLIENT_IPV6PREFIX,  *PCOSA_DML_DHCPSV6_CLIENT_IPV6PREFIX;

struct
_COSA_DML_DHCPSV6_CLIENT_OPTION
{
    ULONG                           Tag;
    UCHAR                           Value[255];
};
typedef  struct _COSA_DML_DHCPSV6_CLIENT_OPTION COSA_DML_DHCPSV6_CLIENT_OPTION,  *PCOSA_DML_DHCPSV6_CLIENT_OPTION;

struct
_COSA_DML_DHCPSV6_CLIENTCONTENT
{
    PCOSA_DML_DHCPSV6_CLIENT_IPV6ADDRESS     pIPv6Address;
    PCOSA_DML_DHCPSV6_CLIENT_IPV6PREFIX      pIPv6Prefix;
    PCOSA_DML_DHCPSV6_CLIENT_OPTION          pOption;
    ULONG                                    NumberofIPv6Address;
    ULONG                                    NumberofIPv6Prefix;
    ULONG                                    NumberofOption;
};
typedef  struct _COSA_DML_DHCPSV6_CLIENTCONTENT COSA_DML_DHCPSV6_CLIENTCONTENT,  *PCOSA_DML_DHCPSV6_CLIENTCONTENT;

struct
_COSA_DML_DHCPSV6_POOL_OPTION
{
    ULONG                           InstanceNumber;
    UCHAR                           Alias[COSA_DML_ALIAS_NAME_LENGTH];

    ULONG                           Tag;
    UCHAR                           PassthroughClient[COSA_DML_ALIAS_NAME_LENGTH];
    UCHAR                           Value[255];
    BOOLEAN                         bEnabled;
};
typedef struct _COSA_DML_DHCPSV6_POOL_OPTION COSA_DML_DHCPSV6_POOL_OPTION,  *PCOSA_DML_DHCPSV6_POOL_OPTION;

struct DHCP_TAG
{
    int tag;
    char * cmdstring;
};

BOOL tagPermitted(int tag);
int _datetime_to_secs(char * p_dt);

/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/

ANSC_STATUS
CosaDmlDhcpv6Init
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );


ULONG
CosaDmlDhcpv6cGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    );

ANSC_STATUS
CosaDmlDhcpv6cGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_DHCPCV6_FULL      pEntry
    );

ANSC_STATUS
CosaDmlDhcpv6cSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

ANSC_STATUS
CosaDmlDhcpv6cAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPCV6_FULL      pEntry
    );

ANSC_STATUS
CosaDmlDhcpv6cDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

ANSC_STATUS
CosaDmlDhcpv6cSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPCV6_CFG       pCfg
    );

ANSC_STATUS
CosaDmlDhcpv6cGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPCV6_CFG       pCfg
    );

ANSC_STATUS
CosaDmlDhcpv6cGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_DHCPCV6_INFO      pInfo
    );

ANSC_STATUS
CosaDmlDhcpv6cGetServerCfg
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCPCV6_SVR      *ppCfg,
        PULONG                      pSize
    );

ANSC_STATUS
CosaDmlDhcpv6cRenew
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

ULONG
CosaDmlDhcpv6cGetNumberOfSentOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber
    );

ANSC_STATUS
CosaDmlDhcpv6cGetSentOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_DHCPCV6_SENT      pEntry
    );

ANSC_STATUS
CosaDmlDhcpv6cGetSentOptionbyInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCPCV6_SENT      pEntry
    );

ANSC_STATUS
CosaDmlDhcpv6cSetSentOptionValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

ANSC_STATUS
CosaDmlDhcpv6cAddSentOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCPCV6_SENT      pEntry
    );

ANSC_STATUS
CosaDmlDhcpv6cDelSentOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        ULONG                       ulInstanceNumber
    );

ANSC_STATUS
CosaDmlDhcpv6cSetSentOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCPCV6_SENT      pEntry
    );

ANSC_STATUS
CosaDmlDhcpv6cGetReceivedOptionCfg
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCPCV6_RECV     *pEntry,
        PULONG                      pSize
    );

ANSC_STATUS
CosaDmlDhcpv6sEnable
    (
        ANSC_HANDLE                 hContext,
        BOOL                        bEnable
    );

BOOLEAN
CosaDmlDhcpv6sGetState
    (
        ANSC_HANDLE                 hContext
    );

ULONG
CosaDmlDhcpv6sGetNumberOfPools
    (
        ANSC_HANDLE                 hContext
    );

ANSC_STATUS
CosaDmlDhcpv6sGetPool
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_DHCPSV6_POOL_FULL   pEntry
    );

ANSC_STATUS
CosaDmlDhcpv6sSetPoolValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

ANSC_STATUS
CosaDmlDhcpv6sAddPool
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPSV6_POOL_FULL   pEntry
    );

ANSC_STATUS
CosaDmlDhcpv6sDelPool
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

ANSC_STATUS
CosaDmlDhcpv6sSetPoolCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPSV6_POOL_CFG    pCfg
    );

ANSC_STATUS
CosaDmlDhcpv6sGetPoolCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPSV6_POOL_CFG    pCfg
    );

ANSC_STATUS
CosaDmlDhcpv6sGetPoolInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_DHCPSV6_POOL_INFO pInfo
    );

ANSC_STATUS
CosaDmlDhcpv6sGetClient
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        PCOSA_DML_DHCPSV6_CLIENT   *ppEntry,
        PULONG                      pSize
    );

ANSC_STATUS
CosaDmlDhcpv6sGetIPv6Address
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulClientIndex,
        PCOSA_DML_DHCPSV6_CLIENT_IPV6ADDRESS   *ppEntry,
        PULONG                      pSize
    );

ANSC_STATUS
CosaDmlDhcpv6sGetIPv6Prefix
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulClientIndex,
        PCOSA_DML_DHCPSV6_CLIENT_IPV6PREFIX   *ppEntry,
        PULONG                      pSize
    );

ANSC_STATUS
CosaDmlDhcpv6sGetIPv6Option
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulClientIndex,
        PCOSA_DML_DHCPSV6_CLIENT_OPTION   *ppEntry,
        PULONG                      pSize
    );

ULONG
CosaDmlDhcpv6sGetNumberOfOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber
    );

ANSC_STATUS
CosaDmlDhcpv6sGetOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_DHCPSV6_POOL_OPTION    pEntry
    );

ANSC_STATUS
CosaDmlDhcpv6sGetOptionbyInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        PCOSA_DML_DHCPSV6_POOL_OPTION  pEntry
    );

ANSC_STATUS
CosaDmlDhcpv6sGetOptionbyInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        PCOSA_DML_DHCPSV6_POOL_OPTION  pEntry
    );

ANSC_STATUS
CosaDmlDhcpv6sSetOptionValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

ANSC_STATUS
CosaDmlDhcpv6sAddOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        PCOSA_DML_DHCPSV6_POOL_OPTION          pEntry
    );

ANSC_STATUS
CosaDmlDhcpv6sDelOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulInstanceNumber
    );

ANSC_STATUS
CosaDmlDhcpv6sSetOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        PCOSA_DML_DHCPSV6_POOL_OPTION          pEntry
    );

/* TBC  -- the functions below should be reviewed, on necessity and name convention */
void     
CosaDmlDhcpv6Remove
    (
        ANSC_HANDLE hContext
    );

int 
CosaDmlStartDHCP6Client
    (
    void
    );

int 
CosaDmlDhcpv6sGetIAPDPrefixes
    (
        PCOSA_DML_DHCPSV6_POOL_CFG  pCfg,
        char*                       pValue,
        ULONG*                      pSize
     );

ULONG
CosaDmlDhcpv6sGetType
    (
        ANSC_HANDLE                 hContext
    );

#endif

