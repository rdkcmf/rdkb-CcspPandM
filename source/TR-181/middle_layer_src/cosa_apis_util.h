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

    module:	cosa_apis_util.h

        This is base file for all parameters H files.

    ---------------------------------------------------------------

    copyright:

        CISCO, Inc.,
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This file contains all utility functions for COSA DML API development.

    ---------------------------------------------------------------

    environment:

        COSA independent

    ---------------------------------------------------------------

    author:

        Roger Hu
        leichen2@cisco.com

    ---------------------------------------------------------------

    revision:

        01/30/2011    initial revision.
        06/15/2012    add IPv4 address utils

**********************************************************************/


#ifndef  _COSA_APIS_UTIL_H
#define  _COSA_APIS_UTIL_H

#include "cosa_dml_api_common.h"

typedef struct StaticRoute 
{
    char         name[64];
    char         dest_lan_ip[16];
    char         netmask[16];
    char         gateway[16];
    int          metric;
    char         dest_intf[10];
    char         origin[16];
}StaticRoute;

#ifndef _BUILD_ANDROID
enum
{
    IPV6_ADDR_SCOPE_UNKNOWN,
    IPV6_ADDR_SCOPE_GLOBAL,
    IPV6_ADDR_SCOPE_LINKLOCAL,
    IPV6_ADDR_SCOPE_SITELOCAL,
    IPV6_ADDR_SCOPE_COMPATv4,
    IPV6_ADDR_SCOPE_LOOPBACK
};
#endif

typedef struct 
{
    int    scope;
    char   v6addr[64];
    char   v6pre[64];
}ipv6_addr_info_t;

typedef  enum
_COSA_DML_LINK_TYPE
{
    COSA_DML_LINK_TYPE_Eth       = 1,
    COSA_DML_LINK_TYPE_EthLink,
    COSA_DML_LINK_TYPE_EthVlan,
    COSA_DML_LINK_TYPE_Usb,
    COSA_DML_LINK_TYPE_Hpna,
    COSA_DML_LINK_TYPE_Moca,
    COSA_DML_LINK_TYPE_HomePlug,
    COSA_DML_LINK_TYPE_Upa,
    COSA_DML_LINK_TYPE_WiFiSsid,
    COSA_DML_LINK_TYPE_Bridge,
    COSA_DML_LINK_TYPE_PPP,
    COSA_DML_LINK_TYPE_DOCSIS,
    COSA_DML_LINK_TYPE_LAST
}
COSA_DML_LINK_TYPE, *PCOSA_DML_LINK_TYPE;

#define COSA_DML_LINK_TYPE_TOTAL                    COSA_DML_LINK_TYPE_LAST

typedef struct
LINKTYPE_MAP
{
    char*                           LinkTypePath;
    char*                           LinkTypeStr;
    COSA_DML_LINK_TYPE              LinkType;
}
LINKTYPE_MAP_T;


ULONG
CosaUtilGetIfAddr
    (
        char*       netdev
    );


PUCHAR
CosaUtilGetLowerLayers
    (
        PUCHAR                      pTableName,
        PUCHAR                      pKeyword
    );

PUCHAR
CosaUtilGetFullPathNameByKeyword
    (
        PUCHAR                      pTableName,
        PUCHAR                      pParameterName,
        PUCHAR                      pKeyword
    );

int CosaUtilGetIfStats(char * ifname, PCOSA_DML_IF_STATS  pStats);

ULONG CosaUtilIoctlXXX(char * if_name, char * method, void * input);
ULONG NetmaskToNumber(char *netmask);
ANSC_STATUS
CosaUtilGetStaticRouteTable
    (
        UINT                        *count,
        StaticRoute                 **out_route
    );

int CosaUtilGetIpv6AddrInfo (char * ifname, ipv6_addr_info_t ** pp_info, int * num);
int safe_strcpy(char * dst, char * src, int dst_size);
int  __v6addr_mismatch(char * addr1, char * addr2, int pref_len);
int  __v6addr_mismatches_v6pre(char * v6addr,char * v6pre);
int  __v6pref_mismatches(char * v6pref1,char * v6pref2);
int CosaDmlV6AddrIsEqual(char * p_addr1, char * p_addr2);
int CosaDmlV6PrefIsEqual(char * p_pref1, char * p_pref2);
int _write_sysctl_file(char * fn, int val);

/*utility functions for linktype and lowerlayer*/
char*               CosaUtilGetLinkTypePath(COSA_DML_LINK_TYPE LinkType);
char*               CosaUtilGetStrFromLinkTypePath(char* pLinkTypePath);
char*               CosaUtilGetLinkTypeStr(COSA_DML_LINK_TYPE LinkType);
COSA_DML_LINK_TYPE  CosaUtilGetLinkTypeFromStr(char* pLinkTypeStr);
COSA_DML_LINK_TYPE  CosaUtilGetLinkTypeFromPath(char*pLinkTypePath);

// utility functions for getting Bridge information
PUCHAR CosaUtilFindBridgeName(char* pBridgePath);
PUCHAR CosaUtilFindBridgePath(char* pBridgeName);


#define BRIDGE_MODE_JUDGEMENT_IFTRUE_RETURNFALSE                                \
    {                                                                           \
        BOOL bridgeMode;                                                        \
        if ((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&bridgeMode)) &&      \
            (TRUE == bridgeMode))                                               \
            return FALSE;                                                       \
    }                                                                           \

/*
 * IPV4 Address check functions
 */
/* __NOTE__ all addresses here are in the host byte order */
#define IPV4_CLASSA(a)        ((((uint32_t)(a)) & 0x80000000UL) == 0) 
#define IPV4_CLASSA_NET       0xff000000 
#define IPV4_CLASSA_NSHIFT    24 
#define IPV4_CLASSA_HOST      (0xffffffff & ~IP_CLASSA_NET) 
#define IPV4_CLASSA_MAX       128 
 
#define IPV4_CLASSB(a)        ((((uint32_t)(a)) & 0xc0000000UL) == 0x80000000UL) 
#define IPV4_CLASSB_NET       0xffff0000 
#define IPV4_CLASSB_NSHIFT    16 
#define IPV4_CLASSB_HOST      (0xffffffff & ~IP_CLASSB_NET) 
#define IPV4_CLASSB_MAX       65536 
 
#define IPV4_CLASSC(a)        ((((uint32_t)(a)) & 0xe0000000UL) == 0xc0000000UL) 
#define IPV4_CLASSC_NET       0xffffff00 
#define IPV4_CLASSC_NSHIFT    8 
#define IPV4_CLASSC_HOST      (0xffffffff & ~IP_CLASSC_NET) 
 
#define IPV4_CLASSD(a)        (((uint32_t)(a) & 0xf0000000UL) == 0xe0000000UL) 
#define IPV4_CLASSD_NET       0xf0000000
#define IPV4_CLASSD_NSHIFT    28
#define IPV4_CLASSD_HOST      0x0fffffff
#define IPV4_MULTICAST(a)     IP_CLASSD(a) 
 
#define IPV4_EXPERIMENTAL(a)  (((uint32_t)(a) & 0xf0000000UL) == 0xf0000000UL) 
#define IPV4_BADCLASS(a)      (((uint32_t)(a) & 0xf0000000UL) == 0xf0000000UL) 

/* __NOTE__ all addresses here are in the network byte order */
int IPv4Addr_IsSameNetwork(uint32_t addr1, uint32_t addr2, uint32_t mask);
int IPv4Addr_IsLoopback(uint32_t addr);
int IPv4Addr_IsMulticast(uint32_t addr);
int IPv4Addr_IsBroadcast(uint32_t addr, uint32_t net, uint32_t mask);
int IPv4Addr_IsNetworkAddr(uint32_t addr, uint32_t net, uint32_t mask);
int IPv4Addr_IsNetmaskValid(uint32_t netmask);
int IPv4Addr_IsClassA(uint32_t addr);
int IPv4Addr_IsClassB(uint32_t addr);
int IPv4Addr_IsClassC(uint32_t addr);
int IPv4Addr_IsClassD(uint32_t addr);

int vsystem(const char *fmt, ...);

void chomp(char *line);

#endif
