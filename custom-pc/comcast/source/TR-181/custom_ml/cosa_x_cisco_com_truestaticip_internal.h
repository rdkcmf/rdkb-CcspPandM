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

    module: cosa_x_cisco_com_truestaticip_internal.h

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


#ifndef  _COSA_TRUESTATICIP_INTERNAL_H
#define  _COSA_TRUESTATICIP_INTERNAL_H

#include "cosa_apis.h"
#include "cosa_x_cisco_com_truestaticip_apis.h"

#define  COSA_IREP_FOLDER_NAME_TSIP_SUBNET              "TrueStaticIPSubnet"
#define  COSA_DML_RR_NAME_TSIP_SUBNETAlias              "Alias"
#define  COSA_DML_RR_NAME_TSIP_SUBNETInsNum             "InstanceNumber"
#define  COSA_DML_RR_NAME_TSIP_SUBNETNextInsNunmber     "NextInstanceNumber"
#define  COSA_DML_RR_NAME_TSIP_SUBNETNewlyAdded         "NewlyAdded"

#define  COSA_IREP_FOLDER_NAME_TSIP_RULE             "TrueStaticIPRule"
#define  COSA_DML_RR_NAME_TSIP_RULEAlias             "Alias"
#define  COSA_DML_RR_NAME_TSIP_RULEInsNum            "InstanceNumber"
#define  COSA_DML_RR_NAME_TSIP_RULENextInsNunmber    "NextInstanceNumber"
#define  COSA_DML_RR_NAME_TSIP_RULENewlyAdded        "NewlyAdded"

/***********************************
    Actual definition declaration
************************************/

#define  COSA_DATAMODEL_TSIP_CLASS_CONTENT                                  \
    /* duplication of the base object class content */                      \
    COSA_BASE_CONTENT                                                       \
    /* start of the object class content */                                 \
    COSA_DML_TSIP_CFG               TSIPCfg;                                \
    SLIST_HEADER                    SubnetList;                             \
    ULONG                           ulSubnetNextInsNum;                     \
    ANSC_HANDLE                     hFtCfg;                                 \
    ANSC_HANDLE                     hAtomNamespace;                         \
    COSA_DML_TSIP_PORTMANAGEMENT_CFG PortManagementCfg;                     \
    SLIST_HEADER                    RuleList;                               \
    ULONG                           ulRuleNextInsNum;                       \
    ANSC_HANDLE                     hIrepFolderCOSA;                        \
    ANSC_HANDLE                     hIrepFolderTSIPSubnet;                  \
    ANSC_HANDLE                     hIrepFolderTSIPRule;                    \


typedef  struct
_COSA_DATAMODEL_TSIP
{
    COSA_DATAMODEL_TSIP_CLASS_CONTENT
}
COSA_DATAMODEL_TSIP,  *PCOSA_DATAMODEL_TSIP;

/**********************************
    Standard function declaration 
***********************************/
ANSC_HANDLE
CosaTSIPCreate
    (
        VOID
    );

ANSC_STATUS
CosaTSIPInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaTSIPRemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaTSIPSubnetRegGetInfo
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaTSIPSubnetRegAddInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );

ANSC_STATUS
CosaTSIPSubnetRegDelInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );

ANSC_STATUS
CosaTSIPRuleRegGetInfo
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaTSIPRuleRegAddInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );

ANSC_STATUS
CosaTSIPRuleRegDelInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );

#endif
