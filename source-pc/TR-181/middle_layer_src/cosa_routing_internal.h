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

    module: cosa_routing_internal.h

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


#ifndef  _COSA_ROUTING_INTERNAL_H
#define  _COSA_ROUTING_INTERNAL_H

#include "cosa_apis.h"

#define  COSA_IREP_FOLDER_NAME_ROUTER              "Router"
#define  COSA_IREP_FOLDER_NAME_ROUTER_HA           "RouterHalfAdded"

#define  COSA_DML_RR_NAME_ROUTER_Prefix             "Router"
#define  COSA_DML_RR_NAME_ROUTER_Forward_Prefix     "IPv4Forward"
#define  COSA_DML_RR_NAME_ROUTER_IPv6Forward_Prefix "IPv6Forward"
#define  COSA_DML_RR_NAME_ROUTER_RipIF_Prefix       "RipInterface"
#define  COSA_DML_RR_NAME_ROUTER_RouteinfoIF_Prefix "RouteInfoInterface"

#define  COSA_DML_RR_NAME_ROUTER_Upper_InsNum      "UpperInsNum"
#define  COSA_DML_RR_NAME_ROUTER_InsNum            "InstanceNum"
#define  COSA_DML_RR_NAME_ROUTER_Alias             "Alias"

#define  COSA_DML_RR_NAME_ROUTER_NextInsNum             "NextInsNum"
#define  COSA_DML_RR_NAME_ROUTER_Forward_NextInsNum     "NextForwardInsNum"
#define  COSA_DML_RR_NAME_ROUTER_IPv6Forward_NextInsNum "NextIPv6ForwardInsNum"
#define  COSA_DML_RR_NAME_ROUTER_RipIF_NextInsNum       "NextRouteInfoIFInsNum"

/***********************************
    Actual definition declaration
************************************/


#define  COSA_DATAMODEL_ROUTING_CLASS_CONTENT                               \
    /* duplication of the base object class content */                      \
    COSA_BASE_CONTENT                                                       \
    /* start of NAT object class content */                                 \
    SLIST_HEADER                    RouterList;                             \
    ULONG                           ulNextRouterInsNum;                     \
    ANSC_HANDLE                     hIrepFolderCOSA;                        \
    ANSC_HANDLE                     hIrepFolderRouter;                      \
    ANSC_HANDLE                     hIrepFolderRouterHA;                    \
    COSA_DML_ROUTEINFO_FULL         RouteInfo;                              \
    ULONG                           PreviousTime;                           \
    SLIST_HEADER                    StaticRoute;                            \
    ULONG                           NextInstanceNumber;                     \
    SLIST_HEADER                    RipIFList;                              \
    COSA_DML_RIP_CFG                RIPCfg;                                 \
    ULONG                           ulNextRipIFInsNum;                      \

typedef  struct
_COSA_DATAMODEL_ROUTING                                              
{
    COSA_DATAMODEL_ROUTING_CLASS_CONTENT
}
COSA_DATAMODEL_ROUTING,  *PCOSA_DATAMODEL_ROUTING;

/**********************************
    Standard function declaration 
***********************************/
ANSC_HANDLE
CosaRoutingCreate
    (
        VOID
    );

ANSC_STATUS
CosaRoutingInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaRoutingRemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaRoutingRegGetInfo

    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaRoutingRegAddInfo
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pNextInsNumName,
        ULONG                       ulNextInsNum,
        char*                       pPreffix,
        ULONG                       ulUpperInsNum,
        char*                       pAlias,
        ANSC_HANDLE                 hCosaContext
    );

ANSC_STATUS
CosaRoutingRegDelInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );

char* 
CosaTimeGetRipdConfStaticPart 
    (
        ANSC_HANDLE   hContext
    );

#endif
