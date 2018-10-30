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

    File: cosa_ip_apis_multilan.h

    For Data Model SBAPI Implementation,
    Common Component Software Platform (CCSP)

    ---------------------------------------------------------------

    description:

        This file defines the APIs servicing TR-181
        data model IP Interface SBAPI integration.

    ---------------------------------------------------------------

    environment:

        Platform Independent, but with multi-LAN support

    ---------------------------------------------------------------

    author:

        Ding Hua

    ---------------------------------------------------------------

    revision:

        02/28/2013  initial revision.

**********************************************************************/

#ifndef _COSA_IP_APIS_MULTILAN_
#define _COSA_IP_APIS_MULTILAN_

#include "cosa_ip_apis.h"
#include "dmsb_tr181_psm_definitions.h"
#include "dml_tr181_custom_cfg.h"

/**************************************************************************
                        DATA STRUCTURE DEFINITIONS
**************************************************************************/

typedef struct
_DMSB_TR181_IP_IF
{
     SINGLE_LINK_ENTRY              Linkage;
     COSA_DML_IP_IF_CFG             Cfg;
     COSA_DML_IP_IF_INFO            Info;

     COSA_DML_IF_STATS              LastStats;
}
DMSB_TR181_IP_IF, *PDMSB_TR181_IP_IF;

#define  ACCESS_DMSB_TR181_IP_IF(p)                 ACCESS_CONTAINER(p, DMSB_TR181_IP_IF, Linkage)

typedef struct
_DMSB_TR181_IP_CONTEXT
{
    /* IP IF */
    BOOLEAN                         IpIfInit;
    SLIST_HEADER                    IpIfList;
}
DMSB_TR181_IP_CONTEXT, *PDMSB_TR181_IP_CONTEXT;

#define  DMSB_TR181_IP_CONTEXT_Init(p)                          \
            {                                                   \
                p->IpIfInit      = FALSE;                       \
                AnscSListInitializeHeader(&p->IpIfList);        \
            }

/**************************************************************************
                        ROUTINE PROTOTYPES
**************************************************************************/

ANSC_STATUS
CosaDmlIpMlanInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

/*
 *  IP Interface
 */
ULONG
CosaDmlIpIfMlanGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    );
    
ANSC_STATUS
CosaDmlIpIfMlanGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_IP_IF_FULL        pEntry
    );

ANSC_STATUS
CosaDmlIpIfMlanSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

ANSC_STATUS
CosaDmlIpIfMlanAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_IP_IF_FULL        pEntry
    );

ANSC_STATUS
CosaDmlIpIfMlanDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

ANSC_STATUS
CosaDmlIpIfMlanSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_IP_IF_CFG         pCfg
    );

ANSC_STATUS
CosaDmlIpIfMlanGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_IP_IF_CFG         pCfg
    );

ANSC_STATUS
CosaDmlIpIfMlanGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_IP_IF_INFO        pInfo
    );

ANSC_STATUS
CosaDmlIpIfMlanReset
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

/*
 *  IP Interface IPv4Address
 */
ULONG
CosaDmlIpIfMlanGetNumberOfV4Addrs
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber
    );

ANSC_STATUS
CosaDmlIpIfMlanGetIPv4Addr
    (
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V4ADDR         pEntry
    );


ANSC_STATUS
CosaDmlIpIfMlanGetSubnetMask
    (
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V4ADDR         pEntry
    );


ANSC_STATUS
CosaDmlIpIfMlanGetV4Addr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_IP_V4ADDR         pEntry
    );

ANSC_STATUS
CosaDmlIpIfMlanSetV4AddrValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );
ANSC_STATUS
CosaDmlIpIfMlanAddV4Addr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V4ADDR         pEntry
    );

ANSC_STATUS
CosaDmlIpIfMlanDelV4Addr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V4ADDR         pEntry
    );

ANSC_STATUS
CosaDmlIpIfMlanSetV4Addr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V4ADDR         pEntry
    );

ANSC_STATUS
CosaDmlIpIfMlanGetV4Addr2
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V4ADDR         pEntry          /* Identified by InstanceNumber */
    );

/*
 *  IP Interface IPv6Address
 */
ANSC_STATUS
CosaDmlIpIfMlanAddV6Addr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6ADDR         pEntry
    );

ANSC_STATUS
CosaDmlIpIfMlanDelV6Addr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6ADDR         pEntry
    );

ANSC_STATUS
CosaDmlIpIfMlanSetV6Addr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6ADDR         pEntry
    );

ANSC_STATUS
CosaDmlIpIfMlanGetV6Addr2
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6ADDR         pEntry          /* Identified by InstanceNumber */
    );

/*
 *  IP Interface IPv6Prefix
 */
ULONG
CosaDmlIpIfMlanGetNumberOfV6Prefixs
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber
    );

ANSC_STATUS
CosaDmlIpIfMlanGetV6Prefix
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_IP_V6PREFIX       pEntry
    );

ANSC_STATUS
CosaDmlIpIfMlanSetV6PrefixValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );
ANSC_STATUS
CosaDmlIpIfMlanAddV6Prefix
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6PREFIX       pEntry
    );

ANSC_STATUS
CosaDmlIpIfMlanDelV6Prefix
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6PREFIX       pEntry
    );

ANSC_STATUS
CosaDmlIpIfMlanSetV6Prefix
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6PREFIX       pEntry
    );

ANSC_STATUS
CosaDmlIpIfMlanGetV6Prefix2
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6PREFIX       pEntry          /* Identified by InstanceNumber */
    );

/*
 *  IP Interface statistics
 */
ANSC_STATUS
CosaDmlIpIfMlanGetStats
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_STATS          pStats
    );


#endif
