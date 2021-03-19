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

    module: cosa_neighdisc_apis.h

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


#ifndef  _COSA_NEIGHDISC_APIS_H
#define  _COSA_NEIGHDISC_APIS_H

#include "cosa_apis.h"
#include "poam_irepfo_interface.h"
#include "sys_definitions.h"
#include "plugin_main_apis.h"
#include "dml_tr181_custom_cfg.h"

typedef  enum
_COSA_DML_NEIGHDISC_IF_STATUS
{
    COSA_DML_NEIGHDISC_IF_STATUS_Disabled = 1,
    COSA_DML_NEIGHDISC_IF_STATUS_Enabled,
    COSA_DML_NEIGHDISC_IF_STATUS_ErrorMisconfigured,
    COSA_DML_NEIGHDISC_IF_STATUS_Error
}
COSA_DML_NEIGHDISC_IF_STATUS, *PCOSA_DML_NEIGHDISC_IF_STATUS;

#define COSA_DML_NEIGHDISC_IFNAME                   CFG_TR181_NeighborDiscovery_IfName

/*
 *  Structure definitions for NEIGHDISC InterfaceSetting
 */
typedef  struct
_COSA_DML_NEIGHDISC_IF_CFG
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    char                            Interface[COSA_DML_IF_NAME_LENGTH];
    ULONG                           RetransTimer;
    ULONG                           RtrSolicitationInterval;
    ULONG                           MaxRtrSolicitations;
    BOOLEAN                         bNUDEnable;
    BOOLEAN                         bRSEnable;
}
COSA_DML_NEIGHDISC_IF_CFG,  *PCOSA_DML_NEIGHDISC_IF_CFG;


typedef enum 
_NEIGHBOR_STATUS{
    NEIGHBOR_STATUS_INCOMPLETE = 1,
    NEIGHBOR_STATUS_REACHABLE,
    NEIGHBOR_STATUS_STALE,
    NEIGHBOR_STATUS_DELAY,
    NEIGHBOR_STATUS_PROBE
}
NEIGHBOR_STATUS;

typedef  struct
_COSA_DML_NEIGHTABLE_INFO
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    char                            Address[64];
    char                            MACAddress[32];
    char                            Interface[16];
    NEIGHBOR_STATUS                 Status;
}
COSA_DML_NEIGHTABLE_INFO, *PCOSA_DML_NEIGHTABLE_INFO;

typedef  struct
_COSA_DML_NEIGHDISC_IF_INFO
{
    COSA_DML_NEIGHDISC_IF_STATUS           Status;
}
COSA_DML_NEIGHDISC_IF_INFO,  *PCOSA_DML_NEIGHDISC_IF_INFO;


typedef  struct
_COSA_DML_NEIGHDISC_IF_FULL
{
    COSA_DML_NEIGHDISC_IF_CFG             Cfg;
    COSA_DML_NEIGHDISC_IF_INFO            Info;
}
COSA_DML_NEIGHDISC_IF_FULL, *PCOSA_DML_NEIGHDISC_IF_FULL;

ANSC_STATUS
CosaDmlNeighdiscInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

ULONG 
CosaDmlNeighdiscGetEnabled
    (
        BOOLEAN * pEnabled
    );

ULONG 
CosaDmlNeighdiscSetEnabled
    (
        BOOLEAN  bEnabled
    );

/*
 *  Neighdisc InterfaceSetting
 */
ULONG
CosaDmlNeighdiscIfGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    );

ANSC_STATUS
CosaDmlNeighdiscIfGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_NEIGHDISC_IF_FULL       pEntry
    );

ANSC_STATUS
CosaDmlNeighdiscIfSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

ANSC_STATUS
CosaDmlNeighdiscIfAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NEIGHDISC_IF_FULL       pEntry
    );

ANSC_STATUS
CosaDmlNeighdiscIfDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

ANSC_STATUS
CosaDmlNeighdiscIfSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NEIGHDISC_IF_CFG        pCfg        /* Identified by InstanceNumber */
    );

ANSC_STATUS
CosaDmlNeighdiscIfGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NEIGHDISC_IF_CFG        pCfg        /* Identified by InstanceNumber */
    );

ANSC_STATUS
CosaDmlNeighdiscIfGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_NEIGHDISC_IF_INFO       pInfo
    );

ANSC_STATUS
CosaDmlNeighdiscIfReset
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );


ANSC_STATUS
CosaDmlNeighdiscIfBERemove
    (
        void
    );

void 
CosaDmlNeighborTableGetEntry
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulInstanceNumber,
        PCOSA_DML_NEIGHTABLE_INFO*  ppNbTbl
    );

#endif
