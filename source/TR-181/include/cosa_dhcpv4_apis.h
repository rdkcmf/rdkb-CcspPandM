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

    module: dml_api_dhcp.h

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

        Ding Hua

    ---------------------------------------------------------------

    revision:

        12/15/2010    initial revision.

**********************************************************************/


#ifndef  _DML_API_DHCP_
#define  _DML_API_DHCP_

/**********************************************************************
                STRUCTURE AND CONSTANT DEFINITIONS
**********************************************************************/

#define  COSA_DML_DHCP_MAX_ENTRIES                  4
#define  COSA_DML_DHCP_MAX_RESERVED_ADDRESSES       8
#define  COSA_DML_DHCP_MAX_OPT_ENTRIES              8

#define  COSA_DML_DHCP_LEASES_FILE                  "/tmp/dnsmasq.leases"
#define  COSA_DML_DHCP_OPTIONS_FILE                 "/tmp/dnsmasq.options"

#define  COSA_DML_DHCPV4_CLIENT_IFNAME              CFG_TR181_DHCPv4_CLIENT_IfName

#define _DEBUG_DHCPV4
#ifdef _DEBUG_DHCPV4
    #define ULOGF ulogf
#else   
    #define ULOGF
#endif

typedef  enum
_COSA_DML_DHCP_STATUS
{
    COSA_DML_DHCP_STATUS_Disabled               = 1,
    COSA_DML_DHCP_STATUS_Enabled,
    COSA_DML_DHCP_STATUS_Error_Misconfigured,
    COSA_DML_DHCP_STATUS_Error
}
COSA_DML_DHCP_STATUS, *PCOSA_DML_DHCP_STATUS;


typedef  enum
_COSA_DML_DHCPC_STATUS
{
    COSA_DML_DHCPC_STATUS_Init                  = 1,
    COSA_DML_DHCPC_STATUS_Selecting,
    COSA_DML_DHCPC_STATUS_Requesting,
    COSA_DML_DHCPC_STATUS_Rebinding,
    COSA_DML_DHCPC_STATUS_Bound,
    COSA_DML_DHCPC_STATUS_Renewing
}
COSA_DML_DHCPC_STATUS, *PCOSA_DML_DHCPC_STATUS;


typedef  enum
_COSA_DML_DHCPS_VID_MODE
{
    COSA_DML_DHCPS_VID_MODE_Exact               = 1,
    COSA_DML_DHCPS_VID_MODE_Prefix,
    COSA_DML_DHCPS_VID_MODE_Suffix,
    COSA_DML_DHCPS_VID_MODE_Substring
}
COSA_DML_DHCPS_VID_MODE, *PCOSA_DML_DHCPS_VID_MODE;


typedef  struct
_COSA_DML_DHCP_OPT
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    UCHAR                           Tag;
    UCHAR                           Value[255];
}
COSA_DML_DHCP_OPT,  *PCOSA_DML_DHCP_OPT;


/*
 *  DHCP Client
 */
typedef  struct
_COSA_DML_DHCPC_CFG
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_ALIAS_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    char                            Interface[COSA_DML_ALIAS_NAME_LENGTH]; /* IP interface name */
    BOOLEAN                         PassthroughEnable;
    char                            PassthroughDHCPPool[64];            /* DHCP server pool alias */
    char                            X_CISCO_COM_BootFileName[256];
}
COSA_DML_DHCPC_CFG,  *PCOSA_DML_DHCPC_CFG;


typedef  struct
_COSA_DML_DHCPC_INFO
{
    COSA_DML_DHCP_STATUS            Status;
    COSA_DML_DHCPC_STATUS           DHCPStatus;
    ANSC_IPV4_ADDRESS               IPAddress;
    ANSC_IPV4_ADDRESS               SubnetMask;
    ULONG                           NumIPRouters;
    ANSC_IPV4_ADDRESS               IPRouters[COSA_DML_DHCP_MAX_ENTRIES];
    ULONG                           NumDnsServers;
    ANSC_IPV4_ADDRESS               DNSServers[COSA_DML_DHCP_MAX_ENTRIES];
    int                             LeaseTimeRemaining;
    ANSC_IPV4_ADDRESS               DHCPServer;
}
COSA_DML_DHCPC_INFO,  *PCOSA_DML_DHCPC_INFO;


typedef  struct
_COSA_DML_DHCPC_FULL
{
    COSA_DML_DHCPC_CFG              Cfg;
    COSA_DML_DHCPC_INFO             Info;
}
COSA_DML_DHCPC_FULL, *PCOSA_DML_DHCPC_FULL;


typedef  struct
_COSA_DML_DHCPC_REQ_OPT
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_ALIAS_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    ULONG                           Order;
    UCHAR                           Tag;
    UCHAR                           Value[255];
}
COSA_DML_DHCPC_REQ_OPT,  *PCOSA_DML_DHCPC_REQ_OPT;


/*
 *  DHCP Server
 */
struct
_COSA_DML_DHCPS_CFG
{
    BOOLEAN                         bEnabled;
};
typedef struct _COSA_DML_DHCPS_CFG COSA_DML_DHCPS_CFG,  *PCOSA_DML_DHCPS_CFG;


struct
_COSA_DML_DHCPS_POOL_CFG
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_ALIAS_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    ULONG                           Order;
    char                            Interface[COSA_DML_ALIAS_NAME_LENGTH];         /* IP interface name */
    char                            VendorClassID[256];
    BOOLEAN                         VendorClassIDExclude;
    COSA_DML_DHCPS_VID_MODE         VendorClassIDMode;
    UCHAR                           ClientID[256];
    BOOLEAN                         ClientIDExclude;
    UCHAR                           UserClassID[256];
    BOOLEAN                         UserClassIDExclude;
    UCHAR                           Chaddr[6];
    UCHAR                           ChaddrMask[6];
    BOOLEAN                         ChaddrExclude;
    BOOLEAN                         DNSServersEnabled;
    ANSC_IPV4_ADDRESS               MinAddress;
    char                            MinAddressUpdateSource[16];
    ANSC_IPV4_ADDRESS               MaxAddress;
    char                            MaxAddressUpdateSource[16];
    ANSC_IPV4_ADDRESS               ReservedAddresses[COSA_DML_DHCP_MAX_RESERVED_ADDRESSES];
    ANSC_IPV4_ADDRESS               SubnetMask;
    ANSC_IPV4_ADDRESS               DNSServers[COSA_DML_DHCP_MAX_ENTRIES];
    char                            DomainName[64];
    ANSC_IPV4_ADDRESS               IPRouters[COSA_DML_DHCP_MAX_ENTRIES];
    int                             LeaseTime;
    int                             X_CISCO_COM_TimeOffset;
    BOOLEAN                         bAllowDelete;
};
typedef struct _COSA_DML_DHCPS_POOL_CFG COSA_DML_DHCPS_POOL_CFG,  *PCOSA_DML_DHCPS_POOL_CFG;


struct
_COSA_DML_DHCPS_POOL_INFO
{
    COSA_DML_DHCP_STATUS            Status;
    ULONG                           X_CISCO_COM_Connected_Device_Number;
};
typedef  struct _COSA_DML_DHCPS_POOL_INFO COSA_DML_DHCPS_POOL_INFO,  *PCOSA_DML_DHCPS_POOL_INFO;


struct
_COSA_DML_DHCPS_POOL_FULL
{
    COSA_DML_DHCPS_POOL_CFG    Cfg;
    COSA_DML_DHCPS_POOL_INFO   Info;
};
typedef  struct _COSA_DML_DHCPS_POOL_FULL COSA_DML_DHCPS_POOL_FULL, *PCOSA_DML_DHCPS_POOL_FULL;

struct
_COSA_DML_DHCPS_POOL_FULL_LINK_OBJ
{
    SINGLE_LINK_ENTRY           Linkage;
    COSA_DML_DHCPS_POOL_FULL    SPool;
    SLIST_HEADER                StaticAddressList;
    SLIST_HEADER                OptionList;
};
typedef  struct _COSA_DML_DHCPS_POOL_FULL_LINK_OBJ COSA_DML_DHCPS_POOL_FULL_LINK_OBJ, *PCOSA_DML_DHCPS_POOL_FULL_LINK_OBJ;

struct
_COSA_DML_DHCPS_X_CISCO_COM_SADDR
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_ALIAS_NAME_LENGTH];

    UCHAR                           Chaddr[6];
    ANSC_IPV4_ADDRESS               Yiaddr;
    char                            DeviceName[COSA_DML_ALIAS_NAME_LENGTH];
};
typedef  struct _COSA_DML_DHCPS_X_CISCO_COM_SADDR COSA_DML_DHCPS_X_CISCO_COM_SADDR,  *PCOSA_DML_DHCPS_X_CISCO_COM_SADDR;

// Need to define link object for dynamic table
struct
_COSA_DML_DHCPS_SADDR
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_ALIAS_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    UCHAR                           Chaddr[6];
    ANSC_IPV4_ADDRESS               Yiaddr;
    char                            DeviceName[COSA_DML_ALIAS_NAME_LENGTH];
    char                            Comment[256];
    BOOLEAN                         ActiveFlag;
};
typedef  struct _COSA_DML_DHCPS_SADDR COSA_DML_DHCPS_SADDR,  *PCOSA_DML_DHCPS_SADDR;

struct
_COSA_DML_DHCPS_SADDR_LINK_OBJ
{
    SINGLE_LINK_ENTRY           Linkage;
    COSA_DML_DHCPS_SADDR        SPoolSADDR;
};
typedef  struct _COSA_DML_DHCPS_SADDR_LINK_OBJ COSA_DML_DHCPS_SADDR_LINK_OBJ,  *PCOSA_DML_DHCPS_SADDR_LINK_OBJ;

// Need to define link object for dynamic table 
// Value is changed to 512 bytes to hold 256 bytes of hexdecimal string
struct
_COSA_DML_DHCPSV4_OPTION
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_ALIAS_NAME_LENGTH];

    ULONG                           Tag;
    UCHAR                           Value[512];
    BOOLEAN                         bEnabled;
};
typedef  struct _COSA_DML_DHCPSV4_OPTION COSA_DML_DHCPSV4_OPTION,  *PCOSA_DML_DHCPSV4_OPTION;

struct
_COSA_DML_DHCPSV4_OPTION_LINK_OBJ
{
    SINGLE_LINK_ENTRY           Linkage;
    COSA_DML_DHCPSV4_OPTION     SPoolOption;
};
typedef  struct _COSA_DML_DHCPSV4_OPTION_LINK_OBJ COSA_DML_DHCPSV4_OPTION_LINK_OBJ,  *PCOSA_DML_DHCPSV4_OPTION_LINK_OBJ;

struct
_COSA_DML_DHCPSV4_CLIENT
{
    char                            Alias[COSA_DML_ALIAS_NAME_LENGTH];

    UCHAR                           Chaddr[18];
    BOOLEAN                         Active;
    UCHAR                           X_CISCO_COM_HostName[64];
    UCHAR                           ClassId[64];
    UCHAR                           X_CISCO_COM_Interface[256];
    UCHAR                           X_CISCO_COM_Comment[256];
};
typedef  struct _COSA_DML_DHCPSV4_CLIENT COSA_DML_DHCPSV4_CLIENT,  *PCOSA_DML_DHCPSV4_CLIENT;

struct
_COSA_DML_DHCPSV4_CLIENT_IPADDRESS
{
    ULONG                           IPAddress;
    UCHAR                           LeaseTimeRemaining[32];
    UCHAR                           X_CISCO_COM_LeaseTimeCreation[32];
    UCHAR                           X_CISCO_COM_LeaseTimeDuration[32];
};
typedef  struct _COSA_DML_DHCPSV4_CLIENT_IPADDRESS COSA_DML_DHCPSV4_CLIENT_IPADDRESS,  *PCOSA_DML_DHCPSV4_CLIENT_IPADDRESS;

struct
_COSA_DML_DHCPSV4_CLIENT_OPTION
{
    ULONG                           Tag;
    UCHAR                           Value[255];
};
typedef  struct _COSA_DML_DHCPSV4_CLIENT_OPTION COSA_DML_DHCPSV4_CLIENT_OPTION,  *PCOSA_DML_DHCPSV4_CLIENT_OPTION;

struct
_COSA_DML_DHCPSV4_CLIENTCONTENT
{
    PCOSA_DML_DHCPSV4_CLIENT_IPADDRESS       pIPAddress;
    PCOSA_DML_DHCPSV4_CLIENT_OPTION          pOption;
    ULONG                                    NumberofIPAddress;
    ULONG                                    NumberofOption;
};
typedef  struct _COSA_DML_DHCPSV4_CLIENTCONTENT COSA_DML_DHCPSV4_CLIENTCONTENT,  *PCOSA_DML_DHCPSV4_CLIENTCONTENT;


/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/

ANSC_STATUS
CosaDmlDhcpInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

/*
 *  DHCP Client
 */
ULONG
CosaDmlDhcpcGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    );

ANSC_STATUS
CosaDmlDhcpcGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_DHCPC_FULL        pEntry
    );

ANSC_STATUS
CosaDmlDhcpcSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

ANSC_STATUS
CosaDmlDhcpcAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPC_FULL        pEntry
    );

ANSC_STATUS
CosaDmlDhcpcDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

ANSC_STATUS
CosaDmlDhcpcSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPC_CFG         pCfg        /* Identified by InstanceNumber */
    );

ANSC_STATUS
CosaDmlDhcpcGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPC_CFG         pCfg        /* Identified by InstanceNumber */
    );

ANSC_STATUS
CosaDmlDhcpcGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_DHCPC_INFO        pInfo
    );

ANSC_STATUS
CosaDmlDhcpcRenew
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

/*
 *  DHCP Client Send/Req Option
 *
 *  The options are managed on top of a DHCP client,
 *  which is identified through pClientAlias
 */
ULONG
CosaDmlDhcpcGetNumberOfSentOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber
    );

ANSC_STATUS
CosaDmlDhcpcGetSentOptionbyInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCP_OPT          pEntry
    );

ANSC_STATUS
CosaDmlDhcpcGetSentOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_DHCP_OPT          pEntry
    );

ANSC_STATUS
CosaDmlDhcpcSetSentOptionValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

ANSC_STATUS
CosaDmlDhcpcAddSentOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCP_OPT          pEntry
    );

ANSC_STATUS
CosaDmlDhcpcDelSentOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        ULONG                       ulInstanceNumber
    );

ANSC_STATUS
CosaDmlDhcpcSetSentOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCP_OPT          pEntry        /* Identified by InstanceNumber */
    );

ULONG
CosaDmlDhcpcGetNumberOfReqOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber
    );

ANSC_STATUS
CosaDmlDhcpcGetReqOptionbyInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCPC_REQ_OPT     pEntry
    );

ANSC_STATUS
CosaDmlDhcpcGetReqOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_DHCPC_REQ_OPT     pEntry
    );

ANSC_STATUS
CosaDmlDhcpcSetReqOptionValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

ANSC_STATUS
CosaDmlDhcpcAddReqOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCPC_REQ_OPT     pEntry
    );

ANSC_STATUS
CosaDmlDhcpcDelReqOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        ULONG                       ulInstanceNumber
    );

ANSC_STATUS
CosaDmlDhcpcSetReqOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCPC_REQ_OPT     pEntry        /* Identified by InstanceNumber */
    );

/*
 *  DHCP Server
 */
ANSC_STATUS
CosaDmlDhcpsEnable
    (
        ANSC_HANDLE                 hContext,
        BOOL                        bEnable
    );

BOOLEAN
CosaDmlDhcpsGetState
    (
        ANSC_HANDLE                 hContext
    );


/*
 *  DHCP Server X_COM_CISCO_StaticAddress
 *
 *  The static addresses are managed on top of a DHCP server pool,
 *  which is identified through pPoolAlias
 */
ULONG
CosaDmlDhcpsGetNumberOfX_COM_CISCO_Saddr
    (
        ANSC_HANDLE                 hContext
    );

ANSC_STATUS
CosaDmlDhcpsGetX_COM_CISCO_Saddr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_DHCPS_SADDR       pEntry
    );

ANSC_STATUS
CosaDmlDhcpsGetX_COM_CISCO_SaddrbyInsNum
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPS_SADDR       pEntry
    );

ANSC_STATUS
CosaDmlDhcpcGetX_COM_CISCO_SaddrbyInsNum
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPS_SADDR       pEntry
    );

ANSC_STATUS
CosaDmlDhcpsSetX_COM_CISCO_SaddrValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

ANSC_STATUS
CosaDmlDhcpsAddX_COM_CISCO_Saddr
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPS_SADDR       pEntry
    );

ANSC_STATUS
CosaDmlDhcpsDelX_COM_CISCO_Saddr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

ANSC_STATUS
CosaDmlDhcpsSetX_COM_CISCO_Saddr
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPS_SADDR       pEntry        /* Identified by InstanceNumber */
    );

/*
 *  DHCP Server Pool
 */
ULONG
CosaDmlDhcpsGetNumberOfPools
    (
        ANSC_HANDLE                 hContext
    );

ANSC_STATUS
CosaDmlDhcpsGetPool
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_DHCPS_POOL_FULL   pEntry
    );

ANSC_STATUS
CosaDmlDhcpsSetPoolValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

ANSC_STATUS
CosaDmlDhcpsAddPool
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPS_POOL_FULL   pEntry
    );

ANSC_STATUS
CosaDmlDhcpsDelPool
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

ANSC_STATUS
CosaDmlDhcpsSetPoolCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPS_POOL_CFG    pCfg        /* Identified by InstanceNumber */
    );

ANSC_STATUS
CosaDmlDhcpsGetPoolCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPS_POOL_CFG         pCfg        /* Identified by InstanceNumber */
    );

ANSC_STATUS
CosaDmlDhcpsGetPoolInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_DHCPS_POOL_INFO   pInfo
    );

/*
 *  DHCP Server Pool Static Address
 *
 *  The static addresses are managed on top of a DHCP server pool,
 *  which is identified through pPoolAlias
 */
ULONG
CosaDmlDhcpsGetNumberOfSaddr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber
    );

ANSC_STATUS
CosaDmlDhcpsGetSaddr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_DHCPS_SADDR       pEntry
    );

ANSC_STATUS
CosaDmlDhcpsGetSaddrbyInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCPS_SADDR       pEntry
    );

ANSC_STATUS
CosaDmlDhcpcGetSaddrbyInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCPS_SADDR       pEntry
    );

ANSC_STATUS
CosaDmlDhcpsSetSaddrValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

ANSC_STATUS
CosaDmlDhcpsAddSaddr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        PCOSA_DML_DHCPS_SADDR       pEntry
    );

ANSC_STATUS
CosaDmlDhcpsDelSaddr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulInstanceNumber
    );

ANSC_STATUS
CosaDmlDhcpsSetSaddr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        PCOSA_DML_DHCPS_SADDR       pEntry        /* Identified by InstanceNumber */
    );


/*
 *  DHCP Server Pool Option
 *
 *  The options are managed on top of a DHCP server pool,
 *  which is identified through pPoolAlias
 */
ULONG
CosaDmlDhcpsGetNumberOfOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber
    );

ANSC_STATUS
CosaDmlDhcpsGetOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_DHCPSV4_OPTION    pEntry
    );

ANSC_STATUS
CosaDmlDhcpsGetOptionbyInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        PCOSA_DML_DHCPSV4_OPTION    pEntry
    );

ANSC_STATUS
CosaDmlDhcpsSetOptionValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

ANSC_STATUS
CosaDmlDhcpsAddOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        PCOSA_DML_DHCPSV4_OPTION    pEntry
    );

ANSC_STATUS
CosaDmlDhcpsDelOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulInstanceNumber
    );

ANSC_STATUS
CosaDmlDhcpsSetOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        PCOSA_DML_DHCPSV4_OPTION    pEntry        /* Identified by InstanceNumber */
    );

/*
    This is for dhcpv4.server.pool.{i}.client.
*/
ANSC_STATUS
CosaDmlDhcpsGetClient
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        PCOSA_DML_DHCPSV4_CLIENT   *ppEntry,
        PCOSA_DML_DHCPSV4_CLIENTCONTENT *ppClientContent,
        PULONG                      pSize
    );

ANSC_STATUS
CosaDmlDhcpsPing
    (
        PCOSA_DML_DHCPSV4_CLIENT_IPADDRESS    pDhcpsClient
    );


ANSC_STATUS
CosaDmlDhcpsGetLeaseTimeDuration
    (
        PCOSA_DML_DHCPSV4_CLIENT_IPADDRESS    pDhcpsClient
    );

extern ANSC_STATUS UpdateJsonParamLegacy
	(
		char*                       pKey,
		char*			PartnerId,
		char*			pValue
    );

extern ANSC_STATUS UpdateJsonParam
	(
		char*           pKey,
		char*			PartnerId,
		char*			pValue,
		char*                   pSource,
		char*			pCurrentTime
    );

extern ANSC_STATUS fillCurrentPartnerId
        (
                char*                       pValue,
        PULONG                      pulSize
    );
#endif

