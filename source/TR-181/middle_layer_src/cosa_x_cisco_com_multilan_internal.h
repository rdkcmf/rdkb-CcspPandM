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

    module: cosa_x_cisco_com_multilan_internal.h

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


#ifndef  _COSA_X_CISCO_COM_MULTILAN_INTERNAL_H
#define  _COSA_X_CISCO_COM_MULTILAN_INTERNAL_H

#include "cosa_apis.h"

/**************************************************************************
                        CONSTANT DEFINITIONS
**************************************************************************/

#define  DM_TR181_NAME_IP_IF                        "Device.IP.Interface."
#define  DM_TR181_NAME_BRIDGE                       "Device.Bridging.Bridge."
#define  DM_TR181_NAME_DHCPv4ServerPool             "Device.DHCPv4.Server.Pool."

#define  DM_TR181_NAME_WiFi_Radio                   "Device.WiFi.Radio."
#define  DM_TR181_NAME_WiFi_SSID                    "Device.WiFi.SSID."
#define  DM_TR181_NAME_WiFi_AP                      "Device.WiFi.AccessPoint."

/**************************************************************************
                        STRUCURE DEFINITIONS
**************************************************************************/
/* 
    Object definition
*/
#define  COSA_DATAMODEL_MULTILAN_CLASS_CONTENT                                      \
    /* duplication of the base object class content */                              \
    COSA_BASE_CONTENT                                                               \
    /* start of X_CISCO_COM_MultiLAN object class content */                        \
    char*                           pPrimaryLanIpIf;                                \
    char*                           pHomeSecurityIpIf;                              \
    char*                           pPrimaryLanBridge;                              \
    char*                           pPrimaryLanBridgeHsPorts;                       \
    char*                           pPrimaryLanDhcpv4ServerPool;                    \
    char*                           pHomeSecurityBridge;                            \
    char*                           pHomeSecurityBridgePorts;                       \
    char*                           pHomeSecurityDHCPv4ServerPool;                  \
    char*                           pHomeSecurityWiFiRadio;                         \
    char*                           pHomeSecurityWiFiSsid;                          \
    char*                           pHomeSecurityWiFiAp;                            \

typedef  struct
_COSA_DATAMODEL_MULTILAN
{
    COSA_DATAMODEL_MULTILAN_CLASS_CONTENT
}
COSA_DATAMODEL_MULTILAN,  *PCOSA_DATAMODEL_MULTILAN;

#define  COSA_DATAMODEL_MULTILAN_Init(p)                                            \
            {                                                                       \
                p->pPrimaryLanIpIf                  = NULL;                         \
                p->pHomeSecurityIpIf                = NULL;                         \
                p->pPrimaryLanBridge                = NULL;                         \
                p->pPrimaryLanBridgeHsPorts         = NULL;                         \
                p->pPrimaryLanDhcpv4ServerPool      = NULL;                         \
                p->pHomeSecurityBridge              = NULL;                         \
                p->pHomeSecurityBridgePorts         = NULL;                         \
                p->pHomeSecurityDHCPv4ServerPool    = NULL;                         \
                p->pHomeSecurityWiFiRadio           = NULL;                         \
                p->pHomeSecurityWiFiSsid            = NULL;                         \
                p->pHomeSecurityWiFiAp              = NULL;                         \
            }

#define  COSA_DATAMODEL_MULTILAN_Clean(p)                                                                   \
            {                                                                                               \
                if ( p->pPrimaryLanIpIf ) AnscFreeMemory(p->pPrimaryLanIpIf);                               \
                if ( p->pHomeSecurityIpIf ) AnscFreeMemory(p->pHomeSecurityIpIf);                           \
                if ( p->pPrimaryLanBridge ) AnscFreeMemory(p->pPrimaryLanBridge);                           \
                if ( p->pPrimaryLanBridgeHsPorts ) AnscFreeMemory(p->pPrimaryLanBridgeHsPorts);             \
                if ( p->pPrimaryLanDhcpv4ServerPool ) AnscFreeMemory(p->pPrimaryLanDhcpv4ServerPool);       \
                if ( p->pHomeSecurityBridge ) AnscFreeMemory(p->pHomeSecurityBridge);                       \
                if ( p->pHomeSecurityBridgePorts ) AnscFreeMemory(p->pHomeSecurityBridgePorts);             \
                if ( p->pHomeSecurityDHCPv4ServerPool ) AnscFreeMemory(p->pHomeSecurityDHCPv4ServerPool);   \
                if ( p->pHomeSecurityWiFiRadio ) AnscFreeMemory(p->pHomeSecurityWiFiRadio);                 \
                if ( p->pHomeSecurityWiFiSsid ) AnscFreeMemory(p->pHomeSecurityWiFiSsid);                   \
                if ( p->pHomeSecurityWiFiAp ) AnscFreeMemory(p->pHomeSecurityWiFiAp);                       \
                                                                                                            \
                AnscFreeMemory(p);                                                                          \
            }

#endif
