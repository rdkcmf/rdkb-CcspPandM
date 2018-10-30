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

    module: cosa_ra_apis.h

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


#ifndef  _COSA_RA_APIS_H
#define  _COSA_RA_APIS_H

#include "cosa_apis.h"
#include "poam_irepfo_interface.h"
#include "sys_definitions.h"
#include "plugin_main_apis.h"

typedef  enum
_COSA_DML_RA_IF_STATUS
{
    COSA_DML_RA_IF_STATUS_Disabled = 1,
    COSA_DML_RA_IF_STATUS_Enabled,
    COSA_DML_RA_IF_STATUS_ErrorMisconfigured,
    COSA_DML_RA_IF_STATUS_Error
}
COSA_DML_RA_IF_STATUS, *PCOSA_DML_RA_IF_STATUS;

typedef  enum
_COSA_DML_RA_PREFER_ROUTER_TYPE
{
    COSA_DML_RA_PREFER_ROUTER_High = 1,
    COSA_DML_RA_PREFER_ROUTER_Medium,
    COSA_DML_RA_PREFER_ROUTER_Low
}
COSA_DML_RA_PREFER_ROUTER_TYPE, *PCOSA_DML_RA_PREFER_ROUTER_TYPE;


/*
 *  Structure definitions for RA InterfaceSetting
 */
typedef  struct
_COSA_DML_RA_IF_CFG
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    char                            Interface[COSA_DML_IF_NAME_LENGTH];
    char                            ManualPrefixes[COSA_DML_IF_NAME_LENGTH];

    ULONG                           MaxRtrAdvInterval;
    ULONG                           MinRtrAdvInterval;
    ULONG                           AdvDefaultLifetime;
    BOOLEAN                         bAdvManagedFlag;
    BOOLEAN                         bAdvOtherConfigFlag;
    BOOLEAN                         bAdvMobileAgentFlag;
    COSA_DML_RA_PREFER_ROUTER_TYPE  AdvPreferredRouterFlag;
    BOOLEAN                         bAdvNDProxyFlag;
    ULONG                           AdvLinkMTU;
    ULONG                           AdvReachableTime;
    ULONG                           AdvRetransTimer;
    ULONG                           AdvCurHopLimit;
}
COSA_DML_RA_IF_CFG,  *PCOSA_DML_RA_IF_CFG;


typedef  struct
_COSA_DML_RA_IF_INFO
{
    COSA_DML_RA_IF_STATUS           Status;
    char                            Prefixes[COSA_DML_IF_NAME_LENGTH];
}
COSA_DML_RA_IF_INFO,  *PCOSA_DML_RA_IF_INFO;


typedef  struct
_COSA_DML_RA_IF_FULL
{
    COSA_DML_RA_IF_CFG             Cfg;
    COSA_DML_RA_IF_INFO            Info;
}
COSA_DML_RA_IF_FULL, *PCOSA_DML_RA_IF_FULL;

typedef  struct
_COSA_DML_RA_IF_FULL2
{
    COSA_DML_RA_IF_CFG            Cfg;
    COSA_DML_RA_IF_INFO           Info;

    SLIST_HEADER                  OptionList;
    ULONG                         ulNextOptionInsNum;
}
COSA_DML_RA_IF_FULL2, *PCOSA_DML_RA_IF_FULL2;

#define RA_OPTION_VALUE_SIZE   1024
typedef struct
_COSA_DML_RA_OPTION
{
    SINGLE_LINK_ENTRY               Link;
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_IF_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    ULONG                           Tag;
    char                            Value[RA_OPTION_VALUE_SIZE];
}
COSA_DML_RA_OPTION, * PCOSA_DML_RA_OPTION;

#define  ACCESS_RA_OPTION_LINK_OBJECT(p)              \
         ACCESS_CONTAINER(p, COSA_DML_RA_OPTION, Link)

ANSC_STATUS
CosaDmlRAInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

ULONG 
CosaDmlRAGetEnabled
    (
        BOOLEAN * pEnabled
    );

ULONG 
CosaDmlRASetEnabled
    (
        BOOLEAN  bEnabled
    );

/*
 *  Ra InterfaceSetting
 */
ULONG
CosaDmlRaIfGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    );

ANSC_STATUS
CosaDmlRaIfGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_RA_IF_FULL       pEntry
    );

ANSC_STATUS
CosaDmlRaIfSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );

ANSC_STATUS
CosaDmlRaIfAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RA_IF_FULL       pEntry
    );

ANSC_STATUS
CosaDmlRaIfDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

ANSC_STATUS
CosaDmlRaIfSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RA_IF_CFG        pCfg        /* Identified by InstanceNumber */
    );

ANSC_STATUS
CosaDmlRaIfGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RA_IF_CFG        pCfg        /* Identified by InstanceNumber */
    );

ANSC_STATUS
CosaDmlRaIfGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_RA_IF_INFO       pInfo
    );

ANSC_STATUS
CosaDmlRaIfReset
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

ANSC_STATUS
CosaDmlRaIfAddOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulRaIfInstanceNumber,
        PCOSA_DML_RA_OPTION         pEntry
    );

ANSC_STATUS
CosaDmlRaIfDelOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulRaIfInstanceNumber,
        PCOSA_DML_RA_OPTION         pEntry
    );

ANSC_STATUS
CosaDmlRaIfSetOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulRaIfInstanceNumber,
        PCOSA_DML_RA_OPTION         pEntry
    );

ANSC_STATUS
CosaDmlRaIfGetOption2
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulRaIfInstanceNumber,
        PCOSA_DML_RA_OPTION         pEntry         
    );
ANSC_STATUS
CosaDmlRaIfSetOptionValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulRaIfInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    );
ANSC_STATUS
CosaDmlRaIfGetOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulRaIfInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_RA_OPTION         pEntry
    );

ULONG
CosaDmlRaIfGetNumberOfOptions
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulRaIfInstanceNumber
    );


ANSC_STATUS
CosaDmlRaIfBERemove
    (
        void
    );


#endif
