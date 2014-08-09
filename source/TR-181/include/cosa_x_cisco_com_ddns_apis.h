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

    module: cosa_x_cisco_com_ddns_apis.h

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

        Tom Chang

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**************************************************************************/

/*
 * This file defines data types and methods as the middle layer between COSA DM library
 * and backend implementation.
 */

#ifndef  _COSA_X_CISCO_COM_DDNS_APIS_H
#define  _COSA_X_CISCO_COM_DDNS_APIS_H

#include "ansc_platform.h"
#include "../middle_layer_src/cosa_apis.h"
#include "../middle_layer_src/plugin_main_apis.h"
#include "poam_irepfo_interface.h"
#include "sys_definitions.h"

#define COSA_DML_SERVICE_NAME_LENGTH    512 
#define DDNS_NAMESPACE "CosaDDNS"

typedef  enum
_COSA_DML_DDNS_STATE
{
    COSA_DML_DDNS_STATE_Idle            = 1,
    COSA_DML_DDNS_STATE_InProgress,
    COSA_DML_DDNS_STATE_Succeeded,
    COSA_DML_DDNS_STATE_ErrorAuthentication,
    COSA_DML_DDNS_STATE_InvalidDomainName,
    COSA_DML_DDNS_STATE_Abuse,
    COSA_DML_DDNS_STATE_NoChange
}
COSA_DML_DDNS_STATE, *PCOSA_DML_DDNS_STATE;

typedef  struct
_COSA_DML_DDNS_CFG
{
    BOOLEAN                         bEnabled;
}
COSA_DML_DDNS_CFG,  *PCOSA_DML_DDNS_CFG;

/*
 *  DDNS Service
 */
typedef  struct
_COSA_DML_DDNS_SERVICE
{
    BOOLEAN                         bEnabled;
    ULONG                           InstanceNumber;
    char                            Alias[64];
    char                            ServiceName[32];
    char                            Username[64];
    char                            Password[64];
    char                            Domain[64];
    char                            AssociatedConnection[64];   /* IP interface name */
    COSA_DML_DDNS_STATE             ConnectionState;
    char                            Mail_exch[128];
    BOOLEAN                         Backup_mx;
    BOOLEAN                         Wildcard;
}
COSA_DML_DDNS_SERVICE,  *PCOSA_DML_DDNS_SERVICE;

/* 
 *  The actual function declaration 
 */

ANSC_STATUS
CosaDmlDdnsInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

ANSC_STATUS
CosaDmlDdnsSetConfig
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DDNS_CFG          pCfg
    );

ANSC_STATUS
CosaDmlDdnsGetConfig
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DDNS_CFG          pCfg
    );

ULONG
CosaDmlDdnsGetNumberOfServices
    (
        ANSC_HANDLE                 hContext
    );

ANSC_STATUS
CosaDmlDdnsGetService
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_DDNS_SERVICE      pService
    );

ANSC_STATUS
CosaDmlDdnsServiceSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

ANSC_STATUS
CosaDmlDdnsGetServiceByInstNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_DDNS_SERVICE      pService
    );

ANSC_STATUS
CosaDmlDdnsAddService
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DDNS_SERVICE      pService
    );

ANSC_STATUS
CosaDmlDdnsDelService
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

ANSC_STATUS
CosaDmlDdnsSetService
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DDNS_SERVICE      pService
    );

ANSC_STATUS
CosaDmlDdnsGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_DDNS_SERVICE      pInfo
);

#endif
