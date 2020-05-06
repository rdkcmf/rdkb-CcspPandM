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

    module: cosa_firewall_apis.h

        For COSA Data Model Library Development

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

#ifndef  _COSA_FIREWALL_APIS_H
#define  _COSA_FIREWALL_APIS_H

#include "ansc_platform.h"
#include "cosa_apis.h"
#include "plugin_main_apis.h"


typedef  enum
_COSA_DML_FIREWALL_LEVEL
{
    COSA_DML_FIREWALL_LEVEL_High    = 1,
    COSA_DML_FIREWALL_LEVEL_Medium,         /* Not applicable for phase 4 */
    COSA_DML_FIREWALL_LEVEL_Low,
    COSA_DML_FIREWALL_LEVEL_Custom,      
    COSA_DML_FIREWALL_LEVEL_None
}
COSA_DML_FIREWALL_LEVEL, *PCOSA_DML_FIREWALL_LEVEL;


typedef  struct
_COSA_DML_FIREWALL_CFG
{
    COSA_DML_FIREWALL_LEVEL         FirewallLevel;
    char                            Version[8];
    char                            LastChange[32];
}
COSA_DML_FIREWALL_CFG,  *PCOSA_DML_FIREWALL_CFG;


/* 
 *  The actual function declaration 
 */

ANSC_STATUS
CosaDmlFirewallInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

ANSC_STATUS
CosaDmlFirewallSetConfig
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_FIREWALL_CFG      pCfg
    );

ANSC_STATUS
CosaDmlFirewallGetConfig
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_FIREWALL_CFG      pCfg
    );

ULONG
CosaDmlGatewayV4GetBlockFragIPPkts
    (
        BOOL *pBool
    );

ULONG
CosaDmlGatewayV4GetPortScanProtect
    (
        BOOL *pBool
    );

ULONG
CosaDmlGatewayV4GetIPFloodDetect
    (
        BOOL *pBool
    );

ULONG
CosaDmlGatewayV4SetBlockFragIPPkts
    (
        BOOL bValue
    );

ULONG
CosaDmlGatewayV4SetPortScanProtect
    (
        BOOL bValue
    );

ULONG
CosaDmlGatewayV4SetIPFloodDetect
    (
        BOOL bValue
    );

ULONG
CosaDmlGatewayV6GetBlockFragIPPkts
    (
        BOOL *pBool
    );

ULONG
CosaDmlGatewayV6GetPortScanProtect
    (
        BOOL *pBool
    );

ULONG
CosaDmlGatewayV6GetIPFloodDetect
    (
        BOOL *pBool
    );

ULONG
CosaDmlGatewayV6SetBlockFragIPPkts
    (
        BOOL bValue
    );

ULONG
CosaDmlGatewayV6SetPortScanProtect
    (
        BOOL bValue
    );

ULONG
CosaDmlGatewayV6SetIPFloodDetect
    (
        BOOL bValue
    );
#endif
