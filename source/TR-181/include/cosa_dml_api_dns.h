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

    module:	cosa_dml_api_dns.h

        For Data Model Library Implementation (DML),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structure and function
        prototypes for DML DNS API.

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


#ifndef  _COSA_DML_API_DNS_
#define  _COSA_DML_API_DNS_

#include "cosa_dml_api_common.h"

/**********************************************************************
                STRUCTURE AND CONSTANT DEFINITIONS
**********************************************************************/

typedef  enum
_COSA_DML_DNS_STATUS
{
    COSA_DML_DNS_STATUS_Disabled        = 1,
    COSA_DML_DNS_STATUS_Enabled,
    COSA_DML_DNS_STATUS_Error
}
COSA_DML_DNS_STATUS, *PCOSA_DML_DNS_STATUS;


typedef  enum
_COSA_DML_DNS_ADDR_SRC
{
    COSA_DML_DNS_ADDR_SRC_DHCP          = 1,
    COSA_DML_DNS_ADDR_SRC_DHCPV4,
    COSA_DML_DNS_ADDR_SRC_DHCPV6,
    COSA_DML_DNS_ADDR_SRC_RouterAdvertisement,
    COSA_DML_DNS_ADDR_SRC_IPCP,
    COSA_DML_DNS_ADDR_SRC_Static
}
COSA_DML_DNS_ADDR_SRC, *PCOSA_DML_DNS_ADDR_SRC;


typedef  struct
_COSA_DML_DNS_CLIENT_SERVER
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_ALIAS_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    COSA_DML_DNS_STATUS             Status;
    char                            DNSServer[64];
    char                            Interface[COSA_DML_ALIAS_NAME_LENGTH]; /* IP interface name */
    COSA_DML_DNS_ADDR_SRC           Type;
    ULONG                           Order;
}
COSA_DML_DNS_CLIENT_SERVER,  *PCOSA_DML_DNS_CLIENT_SERVER;


typedef  struct
_COSA_DML_DNS_RELAY_ENTRY
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_ALIAS_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    COSA_DML_DNS_STATUS             Status;
    ANSC_IPV4_ADDRESS               DNSServer;
    char                            Interface[COSA_DML_ALIAS_NAME_LENGTH]; /* IP interface name */
    COSA_DML_DNS_ADDR_SRC           Type;
}
COSA_DML_DNS_RELAY_ENTRY,  *PCOSA_DML_DNS_RELAY_ENTRY;


/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/

ANSC_STATUS
CosaDmlDnsInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

/*
 *  DNS Client
 */
ANSC_STATUS
CosaDmlDnsEnableClient
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bEnabled
    );

COSA_DML_DNS_STATUS
CosaDmlIpDnsGetClientStatus
    (
        ANSC_HANDLE                 hContext
    );

/*
 *  DNS Client Server
 */
PCOSA_DML_DNS_CLIENT_SERVER
CosaDmlDnsClientGetServers
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount
    );

ANSC_STATUS
CosaDmlDnsClientSetServerValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

ANSC_STATUS
CosaDmlDnsClientAddServer
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DNS_CLIENT_SERVER pEntry
    );

ANSC_STATUS
CosaDmlDnsClientDelServer
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

ANSC_STATUS
CosaDmlDnsClientSetServer
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DNS_CLIENT_SERVER pEntry      /* Identified by InstanceNumber */
    );

ANSC_STATUS
CosaDmlDnsClientGetServer
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DNS_CLIENT_SERVER pEntry      /* Identified by InstanceNumber */
    );

/*
 *  DNS Relay
 */
ANSC_STATUS
CosaDmlDnsEnableRelay
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bEnabled
    );

COSA_DML_DNS_STATUS
CosaDmlIpDnsGetRelayStatus
    (
        ANSC_HANDLE                 hContext
    );

/*
 *  DNS Relay Forwarding (server)
 */
PCOSA_DML_DNS_RELAY_ENTRY
CosaDmlDnsRelayGetServers
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount
    );

ANSC_STATUS
CosaDmlDnsRelaySetServerValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

ANSC_STATUS
CosaDmlDnsRelayAddServer
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DNS_RELAY_ENTRY   pEntry
    );

ANSC_STATUS
CosaDmlDnsRelayDelServer
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

ANSC_STATUS
CosaDmlDnsRelaySetServer
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DNS_RELAY_ENTRY   pEntry      /* Identified by InstanceNumber */
    );

ANSC_STATUS
CosaDmlDnsRelayGetServer
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DNS_RELAY_ENTRY   pEntry      /* Identified by InstanceNumber */
    );


#endif

