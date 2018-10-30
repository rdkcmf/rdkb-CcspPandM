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

/**********************************************************************

    File: cosa_x_cisco_com_multilan_apis.h

    For Data Model SBAPI Implementation,
    Common Component Software Platform (CCSP)

    ---------------------------------------------------------------

    description:

        This file defines the SBAPI functions servicing 
        data model X_CISCO_COM_MultiLAN integration.

    ---------------------------------------------------------------

    environment:

        Platform Independent, but with multi-LAN support

    ---------------------------------------------------------------

    author:

        Ding Hua

    ---------------------------------------------------------------

    revision:

        02/28/2013  initial revision.

**********************************************************************/

#ifndef _COSA_X_CISCO_COM_MULTILAN_APIS_
#define _COSA_X_CISCO_COM_MULTILAN_APIS_

/**************************************************************************
                        DATA STRUCTURE DEFINITIONS
**************************************************************************/

/**************************************************************************
                        ROUTINE PROTOTYPES
**************************************************************************/

ANSC_STATUS
CosaDmlMlanInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

ANSC_STATUS
CosaDmlMlanUnload
    (
        ANSC_HANDLE                 hContext
    );

ANSC_STATUS
CosaDmlMlanGetPrimaryLanIpIf
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pValueSize
    );

ANSC_STATUS
CosaDmlMlanGetHomeSecurityIpIf
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pValueSize
    );

ANSC_STATUS
CosaDmlMlanGetPrimaryLanBridge
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pValueSize
    );

ANSC_STATUS
CosaDmlMlanGetPrimaryLanBridgeHsPorts
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pValueSize
    );

ANSC_STATUS
CosaDmlMlanGetPrimaryLanDhcpv4ServerPool
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pValueSize
    );

ANSC_STATUS
CosaDmlMlanGetHomeSecurityBridge
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pValueSize
    );

ANSC_STATUS
CosaDmlMlanGetHomeSecurityBridgePorts
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pValueSize
    );

ANSC_STATUS
CosaDmlMlanGetHomeSecurityDhcpv4ServerPool
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pValueSize
    );

ANSC_STATUS
CosaDmlMlanGetHomeSecurityWiFiRadio
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pValueSize
    );

ANSC_STATUS
CosaDmlMlanGetHomeSecurityWiFiSsid
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pValueSize
    );

ANSC_STATUS
CosaDmlMlanGetHomeSecurityWiFiAp
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pValueSize
    );

#endif
