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

    module: cosa_ppp_apis.h

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


#ifndef  _COSA_PPP_APIS_H
#define  _COSA_PPP_APIS_H

#include "cosa_apis.h"

#define MAXINSTANCE                 128

#define COSA_DML_PPP_SUPPORTED_NCP_ATCP   0x01
#define COSA_DML_PPP_SUPPORTED_NCP_IPCP   0x02
#define COSA_DML_PPP_SUPPORTED_NCP_IPXCP  0x04
#define COSA_DML_PPP_SUPPORTED_NCP_NBFCP  0x08
#define COSA_DML_PPP_SUPPORTED_NCP_IPv6CP 0x10

typedef  enum
_COSA_DML_PPP_LINK_TYPE
{
    COSA_DML_PPP_LINK_TYPE_Eth       = 1,
    COSA_DML_PPP_LINK_TYPE_EthVlan,
    COSA_DML_PPP_LINK_TYPE_Usb,
    COSA_DML_PPP_LINK_TYPE_Hpna,
    COSA_DML_PPP_LINK_TYPE_Moca,
    COSA_DML_PPP_LINK_TYPE_HomePlug,
    COSA_DML_PPP_LINK_TYPE_Upa,
    COSA_DML_PPP_LINK_TYPE_WiFiSsid,
    COSA_DML_PPP_LINK_TYPE_Bridge,
    COSA_DML_PPP_LINK_TYPE_PPP
}
COSA_DML_PPP_LINK_TYPE, *PCOSA_DML_PPP_LINK_TYPE;


typedef  enum
_COSA_DML_PPP_CONN_STATUS
{
    COSA_DML_PPP_CONN_STATUS_Unconfigured           = 1,
    COSA_DML_PPP_CONN_STATUS_Connecting,
    COSA_DML_PPP_CONN_STATUS_Authenticating,
    COSA_DML_PPP_CONN_STATUS_Connected,
    COSA_DML_PPP_CONN_STATUS_PendingDisconnect,
    COSA_DML_PPP_CONN_STATUS_Disconnecting,
    COSA_DML_PPP_CONN_STATUS_Disconnected
}
COSA_DML_PPP_CONN_STATUS, *PCOSA_DML_PPP_CONN_STATUS;


typedef  enum
_COSA_DML_PPP_CONN_ERROR
{
    COSA_DML_PPP_CONN_ERROR_NONE                    = 1,
    COSA_DML_PPP_CONN_ERROR_ISP_TIME_OUT,
    COSA_DML_PPP_CONN_ERROR_COMMAND_ABORTED,
    COSA_DML_PPP_CONN_ERROR_NOT_ENABLED_FOR_INTERNET,
    COSA_DML_PPP_CONN_ERROR_BAD_PHONE_NUMBER,
    COSA_DML_PPP_CONN_ERROR_USER_DISCONNECT,
    COSA_DML_PPP_CONN_ERROR_ISP_DISCONNECT,
    COSA_DML_PPP_CONN_ERROR_IDLE_DISCONNECT,
    COSA_DML_PPP_CONN_ERROR_FORCED_DISCONNECT,
    COSA_DML_PPP_CONN_ERROR_SERVER_OUT_OF_RESOURCES,
    COSA_DML_PPP_CONN_ERROR_RESTRICTED_LOGON_HOURS,
    COSA_DML_PPP_CONN_ERROR_ACCOUNT_DISABLED,
    COSA_DML_PPP_CONN_ERROR_ACCOUNT_EXPIRED,
    COSA_DML_PPP_CONN_ERROR_PASSWORD_EXPIRED,
    COSA_DML_PPP_CONN_ERROR_AUTHENTICATION_FAILURE,
    COSA_DML_PPP_CONN_ERROR_NO_DIALTONE,
    COSA_DML_PPP_CONN_ERROR_NO_CARRIER,
    COSA_DML_PPP_CONN_ERROR_NO_ANSWER,
    COSA_DML_PPP_CONN_ERROR_LINE_BUSY,
    COSA_DML_PPP_CONN_ERROR_UNSUPPORTED_BITSPERSECOND,
    COSA_DML_PPP_CONN_ERROR_TOO_MANY_LINE_ERRORS,
    COSA_DML_PPP_CONN_ERROR_IP_CONFIGURATION,
    COSA_DML_PPP_CONN_ERROR_UNKNOWN
}
COSA_DML_PPP_CONN_ERROR, *PCOSA_DML_PPP_CONN_ERROR;


typedef  enum
_COSA_DML_PPP_ENCRYPTION
{
    COSA_DML_PPP_ENCRYPTION_None                    = 1,
    COSA_DML_PPP_ENCRYPTION_MPPE
}
COSA_DML_PPP_ENCRYPTION, *PCOSA_DML_PPP_ENCRYPTION;


typedef  enum
_COSA_DML_PPP_COMPRESSION
{
    COSA_DML_PPP_COMPRESSION_None                   = 1,
    COSA_DML_PPP_COMPRESSION_VanJacobson            ,
    COSA_DML_PPP_COMPRESSION_Lzs
}
COSA_DML_PPP_COMPRESSION, *PCOSA_DML_PPP_COMPRESSION;


typedef  enum
_COSA_DML_PPP_AUTH
{
    COSA_DML_PPP_AUTH_PAP                           = 1,
    COSA_DML_PPP_AUTH_CHAP,
    COSA_DML_PPP_AUTH_MS_CHAP
}
COSA_DML_PPP_AUTH, *PCOSA_DML_PPP_AUTH;


typedef  enum
_COSA_DML_PPP_CONN_TRIGGER
{
    COSA_DML_PPP_CONN_TRIGGER_OnDemand              = 1,
    COSA_DML_PPP_CONN_TRIGGER_AlwaysOn,
    COSA_DML_PPP_CONN_TRIGGER_Manual
}
COSA_DML_PPP_CONN_TRIGGER, *PCOSA_DML_PPP_CONN_TRIGGER;



/*
 *  Structure definitions for PPP Interface
 */
typedef  struct
_COSA_DML_PPP_IF_CFG
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    BOOLEAN                         ipcpEnabled;
    BOOLEAN                         ipv6cpEnabled;
    COSA_DML_PPP_LINK_TYPE          LinkType;       /* LinkType/LinkName constitutes LowerLayers */
    char                            LinkName[COSA_DML_IF_NAME_LENGTH];
    ULONG                           AutoDisconnectTime;
    ULONG                           IdleDisconnectTime;
    ULONG                           WarnDisconnectDelay;
    char                            Username[65];
    char                            Password[65];
    USHORT                          MaxMRUSize;
    COSA_DML_PPP_CONN_TRIGGER       ConnectionTrigger;
    /*
     *  PPPoE
     */
    char                            ACName[257];
    char                            ServiceName[257];
    /*
     *  IPCP
     */
    BOOLEAN                         PassthroughEnable;
    char                            PassthroughDHCPPool[COSA_DML_IF_NAME_LENGTH];   /* Alias of the DHCP pool */
}
COSA_DML_PPP_IF_CFG,  *PCOSA_DML_PPP_IF_CFG;


typedef  struct
_COSA_DML_PPP_IF_INFO
{
    COSA_DML_IF_STATUS              Status;
    char                            Name[64];       /* netdev name in Linux */
    ULONG                           LastChange;
    COSA_DML_PPP_CONN_STATUS        ConnectionStatus;
    COSA_DML_PPP_CONN_ERROR         LastConnectionError;
    COSA_DML_PPP_ENCRYPTION         EncryptionProtocol;
    COSA_DML_PPP_COMPRESSION        CompressionProtocol;
    COSA_DML_PPP_AUTH               AuthenticationProtocol;
    USHORT                          CurrentMRUSize;
    ULONG                           LCPEcho;
    ULONG                           LCPEchoRetry;
    /*
     *  PPPoE
     */
    ULONG                           SessionID;
    /*
     *  IPCP
     */
    ANSC_IPV4_ADDRESS               LocalIPAddress;
    ANSC_IPV4_ADDRESS               RemoteIPAddress;
    ANSC_IPV4_ADDRESS               DNSServers[2];
  /*
   * IPv6CP
   */
    char LocalInterfaceIdentifier[64];
    char RemoteInterfaceIdentifier[64];
}
COSA_DML_PPP_IF_INFO,  *PCOSA_DML_PPP_IF_INFO;


typedef  struct
_COSA_DML_PPP_IF_FULL
{
    COSA_DML_PPP_IF_CFG             Cfg;
    COSA_DML_PPP_IF_INFO            Info;
}
COSA_DML_PPP_IF_FULL, *PCOSA_DML_PPP_IF_FULL;

ULONG
CosaDmlPpp_auto_disconnect_time
(
       ULONG var
);

ULONG
CosaDmlPpp_idle_disconnect_time
(
       ULONG var
);

BOOL
CosaDmlPPPEnable
(
       BOOL pppenable
);

BOOL
CosaDmlIPv6CPEnable
(
       BOOL   ipvc6penable
);

BOOL
CosaDmlIPCPEnable
(
       BOOL ipcpenable
);

ANSC_STATUS
CosaDmlPppInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

 ANSC_STATUS
CosaDmlPPPGetWanProtocolConfiguration
    (
        ANSC_HANDLE                 hContext,
        char*                       pString
    );

ANSC_STATUS
CosaDmlPPPSetWanProtocolConfiguration
    (
        ANSC_HANDLE                 hContext,
        char*                       pString
    );

ANSC_STATUS
CosaDmlPPPGetSupportedNCPs
    (
        ANSC_HANDLE                 hContext,
        PULONG                      puLong
    );

/*
 *  PPP Interface
 */
ULONG
CosaDmlPppIfGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    );

ANSC_STATUS
CosaDmlPppIfGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_PPP_IF_FULL       pEntry
    );

ANSC_STATUS
CosaDmlPppIfSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

ANSC_STATUS
CosaDmlPppIfAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_PPP_IF_FULL       pEntry
    );

ANSC_STATUS
CosaDmlPppIfDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

ANSC_STATUS
CosaDmlPppIfSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_PPP_IF_CFG        pCfg        /* Identified by InstanceNumber */
    );

ANSC_STATUS
CosaDmlPppIfGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_PPP_IF_CFG        pCfg        /* Identified by InstanceNumber */
    );

ANSC_STATUS
CosaDmlPppIfGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_PPP_IF_INFO       pInfo
    );

ANSC_STATUS
CosaDmlPppIfReset
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

/*
 *  PPP Interface statistics
 */
ANSC_STATUS
CosaDmlPppIfGetStats
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPppIfInstanceNumber,
        PCOSA_DML_IF_STATS          pStats
);

#ifndef _COSA_SIM_
ULONG get_ppp_ip_addr(void);
#endif

ANSC_STATUS
CosaPPPApiRemove(ANSC_HANDLE  hContext);

#endif
