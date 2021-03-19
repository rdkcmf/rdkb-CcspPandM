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

    module: cosa_x_cisco_com_truestaticip_apis.h

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


#ifndef  _COSA_TRUESTATICIP_APIS_H
#define  _COSA_TRUESTATICIP_APIS_H

#include "../middle_layer_src/cosa_apis.h"
#include "poam_irepfo_interface.h"
#include "sys_definitions.h"
#include "../middle_layer_src/plugin_main_apis.h"

#define  TRUESTATICIP_MAPPING_FILE        "/nvram/mapping.txt"

/***********************************
    Actual definition declaration
************************************/

#define MAXINSTANCE 128
#define KEY_LENGTH_64BIT 16

typedef  enum
_COSA_DML_TSIP_RULE_TYPE
{
    COSA_DML_TSIP_RULE_White     = 1,
    COSA_DML_TSIP_RULE_Black
}
COSA_DML_TSIP_RULE, *PCOSA_DML_TSIP_RULE;

typedef  enum
_COSA_DML_TSIP_RULE_PROTOCOL
{
    COSA_DML_TSIP_RULE_PROTOCOL_TCP     = 1,
    COSA_DML_TSIP_RULE_PROTOCOL_UDP,
    COSA_DML_TSIP_RULE_PROTOCOL_BOTH
}
COSA_DML_TSIP_RULE_PROTOCOL, *PCOSA_DML_TSIP_RULE_PROTOCOL;

/*
 *  Structure definitions
 */
typedef  struct
_COSA_DML_TSIP_CFG
{
    BOOLEAN                         Enabled;
    char                            IPAddress[16];
    char                            SubnetMask[16];
    char                            GatewayIPAddress[16];
    char                            ConfigEncryptKey[17];
    BOOLEAN                         ConfigApply;
    BOOLEAN                         bIPInfoChanged;
}
COSA_DML_TSIP_CFG,  *PCOSA_DML_TSIP_CFG;

typedef  struct
_COSA_DML_TSIP_SUBNET_ENTRY
{
    SINGLE_LINK_ENTRY               Linkage;
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];
 
    BOOLEAN                         Enabled;
    char                            IPAddress[16];
    char                            SubnetMask[16];
}
COSA_DML_TSIP_SUBNET_ENTRY,  *PCOSA_DML_TSIP_SUBNET_ENTRY;

#define  ACCESS_DMSB_TR181_TSIP_ASN(p)                 ACCESS_CONTAINER(p, COSA_DML_TSIP_SUBNET_ENTRY, Linkage)

typedef  struct
_COSA_DML_TSIP_PORTMANAGEMENT_CFG
{
    BOOLEAN                         Enabled;
    ULONG                           RuleType;
}
COSA_DML_TSIP_PORTMANAGEMENT_CFG,  *PCOSA_DML_TSIP_PORTMANAGEMENT_CFG;

typedef  struct
_COSA_DML_TSIP_RULE_ENTRY
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    BOOLEAN                         Enabled;
    char                            Name[32];
    ULONG                           Protocol;
    char                            IPRangeMin[16];
    char                            IPRangeMax[16];
    ULONG                           PortRangeMin;
    ULONG                           PortRangeMax;
}
COSA_DML_TSIP_RULE_ENTRY,  *PCOSA_DML_TSIP_RULE_ENTRY;

typedef struct
_NAMESPACE_MAPPING
{
    char FullPath[256];
    enum dataType_e Type;
}
NAMESPACE_MAPPING, *PNAMESPACE_MAPPING;

/*************************************
    The actual function declaration 
**************************************/
ANSC_STATUS
CosaDmlTSIPInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
);

ANSC_STATUS
CosaDmlTSIPLoadMappingFile
    (
        ANSC_HANDLE                 hContext
    );

/* Device.X_CISCO_COM_TrueStaticIP. */

ANSC_STATUS
CosaDmlTSIPSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TSIP_CFG          pCfg
    );

ANSC_STATUS
CosaDmlTSIPGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TSIP_CFG          pCfg
    );

/* Device.X_CISCO_COM_TrueStaticIP.Subnet.{i} */

ULONG
CosaDmlTSIPSubnetGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    );

ANSC_STATUS
CosaDmlTSIPSubnetGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_TSIP_SUBNET_ENTRY pEntry
    );

ANSC_STATUS
CosaDmlTSIPSubnetSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

ANSC_STATUS
CosaDmlTSIPSubnetAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TSIP_SUBNET_ENTRY pEntry
    );

ANSC_STATUS
CosaDmlTSIPSubnetDelEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TSIP_SUBNET_ENTRY pEntry
    );

ANSC_STATUS
CosaDmlTSIPSubnetSetEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TSIP_SUBNET_ENTRY pEntry
    );

ANSC_STATUS
CosaDmlTSIPSubnetGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TSIP_SUBNET_ENTRY pEntry
    );

/* Device.X_CISCO_COM_TrueStaticIP.PortManagement. */

ANSC_STATUS
CosaDmlTSIPPortManagementSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TSIP_PORTMANAGEMENT_CFG pCfg
    );

ANSC_STATUS
CosaDmlTSIPPortManagementGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TSIP_PORTMANAGEMENT_CFG pCfg
    );

/* Device.X_CISCO_COM_TrueStaticIP.PortManagement.Rule.{i}. */

ULONG
CosaDmlTSIPRuleGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    );

ANSC_STATUS
CosaDmlTSIPRuleGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_TSIP_RULE_ENTRY   pEntry
    );

ANSC_STATUS
CosaDmlTSIPRuleSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

ANSC_STATUS
CosaDmlTSIPRuleAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TSIP_RULE_ENTRY   pEntry
    );

ANSC_STATUS
CosaDmlTSIPRuleDelEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TSIP_RULE_ENTRY   pEntry
    );

ANSC_STATUS
CosaDmlTSIPRuleSetEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TSIP_RULE_ENTRY   pEntry
    );

ANSC_STATUS
CosaDmlTSIPRuleGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_TSIP_RULE_ENTRY   pEntry
    );

ANSC_STATUS
CosaDmlTSIPApplyConfigFile
    (
        ANSC_HANDLE                 hContext
    );

ANSC_STATUS
CosaDmlAdditionalSubnetLoadPsm
    (
        ANSC_HANDLE                 hContext
    );

#endif
