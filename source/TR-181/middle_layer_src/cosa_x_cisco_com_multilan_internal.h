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

    copyright:

        Cisco Systems, Inc.
        All Rights Reserved.

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
    char*                           pHomeSecurityBridge;                            \
    char*                           pHomeSecurityBridgePorts;                       \
    char*                           pHomeSecurityDHCPv4ServerPool;                  \

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
                p->pHomeSecurityBridge              = NULL;                         \
                p->pHomeSecurityBridgePorts         = NULL;                         \
                p->pHomeSecurityDHCPv4ServerPool    = NULL;                         \
            }

#define  COSA_DATAMODEL_MULTILAN_Clean(p)                                                                   \
            {                                                                                               \
                if ( p->pPrimaryLanIpIf ) AnscFreeMemory(p->pPrimaryLanIpIf);                               \
                if ( p->pHomeSecurityIpIf ) AnscFreeMemory(p->pHomeSecurityIpIf);                           \
                if ( p->pPrimaryLanBridge ) AnscFreeMemory(p->pPrimaryLanBridge);                           \
                if ( p->pPrimaryLanBridgeHsPorts ) AnscFreeMemory(p->pPrimaryLanBridgeHsPorts);             \
                if ( p->pHomeSecurityBridge ) AnscFreeMemory(p->pHomeSecurityBridge);                       \
                if ( p->pHomeSecurityBridgePorts ) AnscFreeMemory(p->pHomeSecurityBridgePorts);             \
                if ( p->pHomeSecurityDHCPv4ServerPool ) AnscFreeMemory(p->pHomeSecurityDHCPv4ServerPool);   \
                                                                                                            \
                AnscFreeMemory(p);                                                                          \
            }

#endif
