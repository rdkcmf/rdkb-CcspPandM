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

    module: cosa_x_cisco_com_security_apis.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        Tom Chang

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**************************************************************************/

/*
 * This file defines data types and methods as the middle layer between COSA DM library
 * and backend implementation.
 */

#ifndef  _COSA_X_CISCO_SECURITY_APIS_H
#define  _COSA_X_CISCO_SECURITY_APIS_H

#include "ansc_platform.h"
#include "../middle_layer_src/cosa_apis.h"
#include "../middle_layer_src/plugin_main_apis.h"
#include "cosa_firewall_apis.h"
#include "poam_irepfo_interface.h"
#include "sys_definitions.h"

#define  COSA_DML_IA_LH_MAX_MAC                     10
#define  COSA_DML_IA_LH_MAX_IP                      6
#define  COSA_DML_IA_LH_MAX_IP_RANGE                4
#define  COSA_DML_IA_URL_LENGTH                     256
#define  COSA_DML_IA_KEYWORD_LENGTH                 128


struct
_COSA_DML_SECURITY_CFG
{
    BOOLEAN                         ApplyFirewallSettings;
    BOOLEAN                         CleanLog;
    BOOLEAN                         TriggerEmailLog;
    BOOLEAN                         EmailEnable;
    CHAR                            FilterWebTraffic[64];
    CHAR                            TrafficDetect[64];
    CHAR                            FilterLanTraffic[64];
    CHAR                            AllowPassthrough[64];
    CHAR                            EmailSendTo[64];
    CHAR                            EmailServer[64];
    CHAR                            EmailUserName[64];
    CHAR                            EmailPassword[64];
    CHAR                            EmailFromAddress[64];
    BOOLEAN                         EmailParentalControlBreach;
    BOOLEAN                         EmailAlertsOrWarnings;
    BOOLEAN                         EmailFirewallBreach;
    BOOLEAN                         EmailSendLogs;
}_struct_pack_;

typedef struct _COSA_DML_SECURITY_CFG COSA_DML_SECURITY_CFG,  *PCOSA_DML_SECURITY_CFG;

struct
_COSA_DML_FIREWALL_CFG2
{
    COSA_DML_FIREWALL_LEVEL         FirewallLevel;
    COSA_DML_FIREWALL_LEVEL         FirewallLevelV6;
    BOOLEAN                         WebBlockProxy;
    BOOLEAN                         WebBlockJava;
    BOOLEAN                         WebBlockActiveX;
    BOOLEAN                         WebBlockCookies;
    BOOLEAN                         FilterAnonymousInternetRequests;
    BOOLEAN                         FilterAnonymousInternetRequestsV6;
    BOOLEAN                         FilterMulticast;
    BOOLEAN                         FilterMulticastV6;
    BOOLEAN                         FilterNATRedirection;
    BOOLEAN                         IPSecPassthrough;
    BOOLEAN                         PPTPPassthrough;
    BOOLEAN                         L2TPPassthrough;
    BOOLEAN                         FilterHTTP;
    BOOLEAN                         FilterHTTPV6;
    BOOLEAN                         FilterHTTPs;
    BOOLEAN                         FilterHTTPsV6;
    BOOLEAN                         FilterP2P;
    BOOLEAN                         FilterP2PV6;
    BOOLEAN                         FilterIdent;
    BOOLEAN                         FilterIdentV6;
    BOOLEAN                         TrueStaticIpEnable;
    BOOLEAN                         TrueStaticIpEnableV6;
    BOOLEAN                         SmartPktDectionEnable;
    BOOLEAN                         SmartPktDectionEnableV6;
    BOOLEAN                         WanPingEnable;
    BOOLEAN                         WanPingEnableV6;
}_struct_pack_;

typedef struct _COSA_DML_FIREWALL_CFG2 COSA_DML_FIREWALL_CFG2,  *PCOSA_DML_FIREWALL_CFG2;


typedef  enum
_COSA_DML_IA_APP_PROTO
{
    COSA_DML_IA_APP_PROTO_Tcp       = 1,
    COSA_DML_IA_APP_PROTO_Udp,
    COSA_DML_IA_APP_PROTO_Both
}
COSA_DML_IA_APP_PROTO, *PCOSA_DML_IA_APP_PROTO;

typedef  struct
_COSA_DML_IA_POLICY_SCH
{
    BOOLEAN                         AllowAccess;
    BOOLEAN                         IncludeSunday;
    BOOLEAN                         IncludeMonday;
    BOOLEAN                         IncludeTuesday;
    BOOLEAN                         IncludeWednesday;
    BOOLEAN                         IncludeThursday;
    BOOLEAN                         IncludeFriday;
    BOOLEAN                         IncludeSaturday;
    BOOLEAN                         AllDay;
    char                            StartTime[32];
    char                            EndTime[32];
}
COSA_DML_IA_POLICY_SCH,  *PCOSA_DML_IA_POLICY_SCH;


typedef  struct
_COSA_DML_IA_LH_MAC
{
    UCHAR                           Mac[6];
}
COSA_DML_IA_LH_MAC,  *PCOSA_DML_IA_LH_MAC;

typedef  struct
_COSA_DML_IA_LH_IP
{
    ANSC_IPV4_ADDRESS               Ip;
}
COSA_DML_IA_LH_IP,  *PCOSA_DML_IA_LH_IP;

typedef  struct
_COSA_DML_IA_LH_IPR
{
    ANSC_IPV4_ADDRESS               StartIp;
    ANSC_IPV4_ADDRESS               EndIp;
}
COSA_DML_IA_LH_IPR,  *PCOSA_DML_IA_LH_IPR;


typedef  struct
_COSA_DML_IA_LH
{
    BOOLEAN                         bUseLanHosts;
    USHORT                          MacCount;
    USHORT                          IpCount;
    USHORT                          IprCount;
    COSA_DML_IA_LH_MAC              MacList[COSA_DML_IA_LH_MAX_MAC];
    COSA_DML_IA_LH_IP               IpList[COSA_DML_IA_LH_MAX_IP];
    COSA_DML_IA_LH_IPR              IprList[COSA_DML_IA_LH_MAX_IP_RANGE];
}
COSA_DML_IA_LH, *PCOSA_DML_IA_LH;

typedef  struct
_COSA_DML_IA_POLICY
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH]; /* This is also the policy name */

    BOOLEAN                         bEnabled;
    BOOLEAN                         EnableLanHostMACAddresses;
    BOOLEAN                         AllowLanHostMACAddresses;
    COSA_DML_IA_LH                  LanHost;
}
COSA_DML_IA_POLICY,  *PCOSA_DML_IA_POLICY;

typedef  struct
_COSA_DML_IA_POLICY2
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH]; /* This is also the policy name */

    BOOLEAN                         bEnabled;
    BOOLEAN                         EnableLanHostMACAddresses;
    BOOLEAN                         AllowLanHostMACAddresses;
    COSA_DML_IA_LH                  LanHost;

    /* Tom Added */
    COSA_DML_IA_POLICY_SCH          Schedule;
    SLIST_HEADER                    URLList;
    SLIST_HEADER                    KeywordList;
    SLIST_HEADER                    AppList;
    ULONG                           ulNextURLInsNum;
    ULONG                           ulNextKeyInsNum;
    ULONG                           ulNextAppInsNum;
}
COSA_DML_IA_POLICY2,  *PCOSA_DML_IA_POLICY2;

typedef  struct
_COSA_DML_IA_POLICY_URL
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    char                            Url[COSA_DML_IA_URL_LENGTH];
}
COSA_DML_IA_POLICY_URL,  *PCOSA_DML_IA_POLICY_URL;

typedef  struct
_COSA_DML_IA_POLICY_KEYWORD
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    char                            Keyword[COSA_DML_IA_KEYWORD_LENGTH];
}
COSA_DML_IA_POLICY_KEYWORD,  *PCOSA_DML_IA_POLICY_KEYWORD;

typedef  struct
_COSA_DML_IA_POLICY_APP
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH]; /* This is also the app name */

    BOOLEAN                         IsWellKnown;
    COSA_DML_IA_APP_PROTO           Protocol;
    USHORT                          StartPortNumber;
    USHORT                          EndPortNumber;
}
COSA_DML_IA_POLICY_APP,  *PCOSA_DML_IA_POLICY_APP;


typedef  struct
_COSA_SECURITY_IA_POLICY2
{
    ULONG                           ulInstanceNumber;
    BOOLEAN                         bEnabled;
    BOOLEAN                         EnableLanHostMACAddresses;
    BOOLEAN                         AllowLanHostMACAddresses;
    char                            Name[64];
    BOOLEAN                         bUseLanHosts;
    COSA_DML_IA_POLICY_SCH          Schedule;
    USHORT                          ulNumOfMac;
    COSA_DML_IA_LH_MAC              MacList[COSA_DML_IA_LH_MAX_MAC];
    USHORT                          ulNumOfIp;
    COSA_DML_IA_LH_IP               IPList[6];
    USHORT                          ulNumOfIpr;
    COSA_DML_IA_LH_IPR              IPRList[4];
    USHORT                          ulNumOfUrl;
    COSA_DML_IA_POLICY_URL          URLList[4];
    USHORT                          ulNumOfKeyword;
    COSA_DML_IA_POLICY_KEYWORD      KeywordList[4];
    USHORT                          ulNumOfApp;
    COSA_DML_IA_POLICY_APP          AppList[10];
}
COSA_SECURITY_IA_POLICY2,  *PCOSA_SECURITY_IA_POLICY2;


typedef  struct
_COSA_DML_IA_LOG_ENTRY
{
    /* ANSC_UNIVERSAL_TIME             OccuranceTime; */
    char                            OccuranceTime[32];
    char                            Action[256];
    ANSC_IPV4_ADDRESS               TargetIP;
    char                            User[64];
    ANSC_IPV4_ADDRESS               SourceIP;
    ULONG                           Count;
    char                            Description[64];
}
COSA_DML_IA_LOG_ENTRY,  *PCOSA_DML_IA_LOG_ENTRY;


/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/

ANSC_STATUS
CosaDmlIaInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

ANSC_STATUS
CosaDmlSecurityGetConfig
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_SECURITY_CFG      pCfg
    );

ANSC_STATUS
CosaDmlSecuritySetConfig
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_SECURITY_CFG      pCfg
    );

ANSC_STATUS
CosaDmlFirewallGetConfig2
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_FIREWALL_CFG2     pCfg
    );

ANSC_STATUS
CosaDmlFirewallSetConfig2
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_FIREWALL_CFG2     pCfg
    );


ULONG
CosaDmlIaGetNumberOfPolicies
    (
        ANSC_HANDLE                 hContext
    );


ANSC_STATUS
CosaDmlIaGetPolicy
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_IA_POLICY         pEntry
    );

ANSC_STATUS
CosaDmlIaSetPolicyValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

ANSC_STATUS
CosaDmlIaGetPolicyByInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInsNum,
        PCOSA_DML_IA_POLICY         pEntry
    );

ANSC_STATUS
CosaDmlIaAddPolicy
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_IA_POLICY         pEntry
    );

ANSC_STATUS
CosaDmlIaDelPolicy
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

ANSC_STATUS
CosaDmlIaSetPolicy
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_IA_POLICY         pEntry
    );

ANSC_STATUS
CosaDmlIaGetPolicySchedule
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_IA_POLICY_SCH     pEntry
    );

ANSC_STATUS
CosaDmlIaSetPolicySchedule
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_IA_POLICY_SCH     pEntry
    );

ULONG
CosaDmlIaPolicyGetNumberOfUrls
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber
    );

ANSC_STATUS
CosaDmlIaPolicyGetUrl
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_IA_POLICY_URL     pUrl
    );

ANSC_STATUS
CosaDmlIaPolicyGetUrlByInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        ULONG                       ulURLInstanceNumber,
        PCOSA_DML_IA_POLICY_URL     pUrl
    );

ANSC_STATUS
CosaDmlIaPolicySetUrlValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

ANSC_STATUS
CosaDmlIaPolicyAddUrl
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        PCOSA_DML_IA_POLICY_URL     pUrl
    );

ANSC_STATUS
CosaDmlIaPolicyDelUrl
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        PCOSA_DML_IA_POLICY_URL     pUrl        /* Identified by InstanceNumber */
    );

ANSC_STATUS
CosaDmlIaPolicySetUrl
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        PCOSA_DML_IA_POLICY_URL     pUrl        /* Identified by InstanceNumber */
    );

ULONG
CosaDmlIaPolicyGetNumberOfKeywords
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber
    );

ANSC_STATUS
CosaDmlIaPolicyGetKeyword
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_IA_POLICY_KEYWORD pKeyword
    );

ANSC_STATUS
CosaDmlIaPolicyGetKeywordByInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        ULONG                       ulKeywordInstanceNumber,
        PCOSA_DML_IA_POLICY_KEYWORD pKeyword
    );

ANSC_STATUS
CosaDmlIaPolicySetKeywordValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

ANSC_STATUS
CosaDmlIaPolicyAddKeyword
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        PCOSA_DML_IA_POLICY_KEYWORD pKeyword
    );

ANSC_STATUS
CosaDmlIaPolicyDelKeyword
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        PCOSA_DML_IA_POLICY_KEYWORD pKeyword    /* Identified by InstanceNumber */
    );

ANSC_STATUS
CosaDmlIaPolicySetKeyword
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        PCOSA_DML_IA_POLICY_KEYWORD pKeyword    /* Identified by InstanceNumber */
    );

ULONG
CosaDmlIaPolicyGetNumberOfApps
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber
    );

ANSC_STATUS
CosaDmlIaPolicyGetApp
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_IA_POLICY_APP     pApp
    );

ANSC_STATUS
CosaDmlIaPolicyGetAppByInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        ULONG                       ulAppInstanceNumber,
        PCOSA_DML_IA_POLICY_APP     pApp
    );

ANSC_STATUS
CosaDmlIaPolicySetAppValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

ANSC_STATUS
CosaDmlIaPolicyAddApp
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        PCOSA_DML_IA_POLICY_APP     pApp
    );

ANSC_STATUS
CosaDmlIaPolicyDelBlockedApp
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        PCOSA_DML_IA_POLICY_APP     pApp        /* Identified by InstanceNumber */
    );

ANSC_STATUS
CosaDmlIaPolicySetBlockedApp
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPolicyInstanceNumber,
        PCOSA_DML_IA_POLICY_APP     pApp        /* Identified by InstanceNumber */
    );

PCOSA_DML_IA_LOG_ENTRY
CosaDmlIaGetLogEntries
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount
    );

BOOL
CosaDmlIaLogIsUpdated
    (
        ANSC_HANDLE                 hContext
    );

BOOL isWellKnownService (const char *name);

void CosaDmlIaRemove();
#endif
