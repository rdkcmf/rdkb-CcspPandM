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

    module: cosa_userinterface_apis.h

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


#ifndef  _COSA_USERINTERFACE_API_H
#define  _COSA_USERINTERFACE_API_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"

#define HTTP_INTERNAL_PORT            80
#define HTTPS_INTERNAL_PORT           443
#define SSH_INTERNAL_PORT             22

/**********************************************************************
                STRUCTURE AND CONSTANT DEFINITIONS
**********************************************************************/

typedef  enum
_COSA_DML_RA_PROTOCOL
{
    COSA_DML_RA_PROTOCOL_Http           = 1,
    COSA_DML_RA_PROTOCOL_Https,
    COSA_DML_RA_PROTOCOL_Ssh,
    COSA_DML_RA_PROTOCOL_Telnet,
}
COSA_DML_RA_PROTOCOL, *PCOSA_DML_RA_PROTOCOL;


struct
_COSA_DML_RA_CFG
{
    /*
     *  Extensions -- X_CISCO_COM_RemoteAccess
     */    
    BOOLEAN                         bEnabled;
    char                            SupportedProtocols[64];
    BOOLEAN                         bFromAnyIp;
    ANSC_IPV4_ADDRESS               StartIp;
    ANSC_IPV4_ADDRESS               EndIp;
    BOOLEAN                         HttpEnable;
    ULONG                           HttpPort;
    BOOLEAN                         HttpsEnable;
    ULONG                           HttpsPort;
    BOOLEAN                         SSHEnable;
    ULONG                           SSHPort;
    BOOLEAN                         TelnetEnable;
    ULONG                           TelnetPort;
    BOOLEAN                         UpgradePermission;
    char                            StartIpV6[64];
    char                            EndIpV6[64];
}_struct_pack_;

typedef struct _COSA_DML_RA_CFG  COSA_DML_RA_CFG,  *PCOSA_DML_RA_CFG;

typedef struct
_COSA_DML_USERINTERFACE_CFG
{
    BOOLEAN                         bPasswordLockoutEnable;
    BOOLEAN                         bHTTPSecurityHeaderEnable;
    ULONG                           PasswordLockoutAttempts;
    ULONG                           PasswordLockoutTime;
}
COSA_DML_USERINTERFACE_CFG,  *PCOSA_DML_USERINTERFACE_CFG;

typedef  struct
_COSA_DML_UI_IPRANGE_ENTRY
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    char                            StartIP[16];
    char                            EndIP[16];
    char                            Desp[64];
}
COSA_DML_UI_IPRANGE_ENTRY,  *PCOSA_DML_UI_IPRANGE_ENTRY;

/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/

ANSC_STATUS
CosaDmlRaInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

ANSC_STATUS
CosaDmlRaSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RA_CFG            pCfg
    );

ANSC_STATUS
CosaDmlRaGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RA_CFG            pCfg
    );

ANSC_STATUS
CosaDmlUserInterfaceSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_USERINTERFACE_CFG            pCfg
    );

ANSC_STATUS
CosaDmlUserInterfaceGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_USERINTERFACE_CFG            pCfg
    );


ANSC_STATUS
CosaDmlRaGetSupportedProtocols
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pCount,
        PCOSA_DML_RA_PROTOCOL       pEntries
    );

ULONG
CosaDmlUIIPRangeGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    );

ANSC_STATUS
CosaDmlUIIPRangeGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_UI_IPRANGE_ENTRY  pEntry
    );

ANSC_STATUS
CosaDmlUIIPRangeSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

ANSC_STATUS
CosaDmlUIIPRangeAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_UI_IPRANGE_ENTRY  pEntry
    );

ANSC_STATUS
CosaDmlUIIPRangeDelEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_UI_IPRANGE_ENTRY  pEntry
    );

ANSC_STATUS
CosaDmlUIIPRangeSetEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_UI_IPRANGE_ENTRY  pEntry
    );

ANSC_STATUS
CosaDmlUIIPRangeGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_UI_IPRANGE_ENTRY  pEntry
    );
#endif

