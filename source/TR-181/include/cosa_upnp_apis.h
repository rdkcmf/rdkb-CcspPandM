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

    module: cosa_upnp_apis.h

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


#ifndef  _COSA_UPNP_API_H
#define  _COSA_UPNP_API_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"

/**********************************************************************
                STRUCTURE AND CONSTANT DEFINITIONS
**********************************************************************/


/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/

ANSC_STATUS
CosaDmlUpnpInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

ANSC_STATUS
CosaDmlUpnpDevEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bEnabled
    );

BOOLEAN
CosaDmlUpnpDevGetState
    (
        ANSC_HANDLE                 hContext
    );

ANSC_STATUS
CosaDmlUpnpDevEnableMediaServer
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bEnabled
    );

BOOLEAN
CosaDmlUpnpDevGetMediaServerState
    (
        ANSC_HANDLE                 hContext
    );

ANSC_STATUS
CosaDmlUpnpDevEnableIgd
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bEnabled
    );

BOOLEAN
CosaDmlUpnpDevGetIgdState
    (
        ANSC_HANDLE                 hContext
    );

ANSC_STATUS
CosaDmlUpnpDevGetArchVer
    (
        ANSC_HANDLE                 hContext,
        ULONG*                      pMajorVer,
        ULONG*                      pMinorVer
    );

ANSC_STATUS
CosaDmlUpnpSetDiscoveryEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bEnabled
    );

BOOLEAN
CosaDmlUpnpGetDiscoveryEnable
    (
        ANSC_HANDLE                 hContext
    );

ANSC_STATUS
CosaDmlUpnpSetDiscoveryPollingInterval
    (
        ANSC_HANDLE                 hContext,
        ULONG                       PollingInterval
    );

ULONG
CosaDmlUpnpGetDiscoveryPollingInterval
    (
        ANSC_HANDLE                 hContext
    );

ANSC_STATUS
CosaDmlUpnpDevGetAdvPeriod
    (
        ANSC_HANDLE                 hContext,
        PULONG                     *val
    );

ANSC_STATUS
CosaDmlUpnpDevGetTTL
    (
        ANSC_HANDLE                 hContext,
        PULONG                     *val
    );

ANSC_STATUS
CosaDmlUpnpDevSetTTL
    (
        ANSC_HANDLE                 hContext,
        ULONG                       val
    );

ANSC_STATUS
CosaDmlUpnpDevSetAdvPeriod
    (
        ANSC_HANDLE                 hContext,
        ULONG                       val
    );
#endif

