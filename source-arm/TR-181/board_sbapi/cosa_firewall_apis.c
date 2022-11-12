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

    module: cosa_firewall_apis.c

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

#include "cosa_firewall_apis.h"
#include <syscfg/syscfg.h>
#include "safec_lib_common.h"

#define V4_BLOCKFRAGIPPKT   "v4_BlockFragIPPkts"
#define V4_PORTSCANPROTECT  "v4_PortScanProtect"
#define V4_IPFLOODDETECT    "v4_IPFloodDetect"

#define V6_BLOCKFRAGIPPKT   "v6_BlockFragIPPkts"
#define V6_PORTSCANPROTECT  "v6_PortScanProtect"
#define V6_IPFLOODDETECT    "v6_IPFloodDetect"

#ifdef _COSA_SIM_

COSA_DML_FIREWALL_CFG g_FirewallConfig = 
{
    COSA_DML_FIREWALL_LEVEL_Low, 
    "1.0",
    "2011-01-01T00:00:00Z"
};


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlFirewallInit
            (
                ANSC_HANDLE                 hDml,
                PANSC_HANDLE                phContext
            )

    Description:

        Backend implementation to get Firewall configuration.

    Arguments:    ANSC_HANDLE                 hDml
                  Reserved.

                  PANSC_HANDLE                phContext
                  Pointer of the configuration to be retrieved.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlFirewallInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlFirewallGetConfig
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_FIREWALL_CFG      pCfg
            );

    Description:

        Backend implementation to get Firewall configuration.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PCOSA_DML_FIREWALL_CFG      pCfg
                  Pointer of the configuration to be retrieved.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlFirewallGetConfig
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_FIREWALL_CFG      pCfg
    )
{
    pCfg->FirewallLevel = g_FirewallConfig.FirewallLevel;
    errno_t rc = -1;

    rc = strcpy_s(pCfg->Version,sizeof(pCfg->Version),g_FirewallConfig.Version);
    ERR_CHK(rc);
    rc = strcpy_s(pCfg->LastChange,sizeof(pCfg->LastChange), g_FirewallConfig.LastChange);
    ERR_CHK(rc);

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlFirewallSetConfig
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_FIREWALL_CFG      pCfg
            );

    Description:

        Backend implementation to set Firewall configuration.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PCOSA_DML_FIREWALL_CFG      pCfg
                  Pointer of the configuration to be set.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlFirewallSetConfig
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_FIREWALL_CFG      pCfg
    )
{
    g_FirewallConfig.FirewallLevel = pCfg->FirewallLevel;

    return ANSC_STATUS_SUCCESS;
}

#elif (_COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_)

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlFirewallInit
            (
                ANSC_HANDLE                 hDml,
                PANSC_HANDLE                phContext
            )

    Description:

        Backend implementation to get Firewall configuration.

    Arguments:    ANSC_HANDLE                 hDml
                  Reserved.

                  PANSC_HANDLE                phContext
                  Pointer of the configuration to be retrieved.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlFirewallInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    UNREFERENCED_PARAMETER(hDml);
    UNREFERENCED_PARAMETER(phContext);
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlFirewallGetConfig
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_FIREWALL_CFG      pCfg
            );

    Description:

        Backend implementation to get Firewall configuration.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PCOSA_DML_FIREWALL_CFG      pCfg
                  Pointer of the configuration to be retrieved.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlFirewallGetConfig
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_FIREWALL_CFG      pCfg
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pCfg);
    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlFirewallSetConfig
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_FIREWALL_CFG      pCfg
            );

    Description:

        Backend implementation to set Firewall configuration.

    Arguments:    ANSC_HANDLE                 hContext
                  Reserved.

                  PCOSA_DML_FIREWALL_CFG      pCfg
                  Pointer of the configuration to be set.

    Return:       The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlFirewallSetConfig
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_FIREWALL_CFG      pCfg
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pCfg);
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ULONG
        CosaDmlGatewayV4GetBlockFragIPPkts
            (
                BOOL                        *pValue
            );

    Description:

        Backend implementation to get V4_BLOCKFRAGIPPKT value from syscfg

    Arguments:    BOOL                     *pValue

    Return:       The status of the operation.

**********************************************************************/
ULONG
CosaDmlGatewayV4GetBlockFragIPPkts
(
    BOOL                        *pValue
)
{
    char buf[8];

    syscfg_get( NULL, V4_BLOCKFRAGIPPKT, buf, sizeof(buf));
    *pValue = (strcmp(buf, "1") == 0);

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ULONG
        CosaDmlGatewayV4GetPortScanProtect
            (
                BOOL                        *pValue
            );

    Description:

        Backend implementation to get V4_PORTSCANPROTECT value from syscfg

    Arguments:    BOOL                     *pValue

    Return:       The status of the operation.

**********************************************************************/
ULONG
CosaDmlGatewayV4GetPortScanProtect
(
    BOOL                        *pValue
)
{
    char buf[8];

    syscfg_get( NULL, V4_PORTSCANPROTECT, buf, sizeof(buf));
    *pValue = (strcmp(buf, "1") == 0);

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ULONG
        CosaDmlGatewayV4GetIPFloodDetect
            (
                BOOL                        *pValue
            );

    Description:

        Backend implementation to get V4_IPFLOODDETECT value from syscfg

    Arguments:    BOOL                     *pValue

    Return:       The status of the operation.

**********************************************************************/
ULONG
CosaDmlGatewayV4GetIPFloodDetect
(
   BOOL                        *pValue
)
{
    char buf[8];

    syscfg_get( NULL, V4_IPFLOODDETECT, buf, sizeof(buf));
    *pValue = (strcmp(buf, "1") == 0);

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ULONG
        CosaDmlGatewayV4SetBlockFragIPPkts
            (
                BOOL                        *pValue
            );

    Description:

        Backend implementation to set V4_BLOCKFRAGIPPKT value to syscfg

    Arguments:    BOOL                     *pValue

    Return:       The status of the operation.

**********************************************************************/
ULONG
CosaDmlGatewayV4SetBlockFragIPPkts
(
    BOOL                        bValue
)
{
    syscfg_set_commit(NULL, V4_BLOCKFRAGIPPKT, bValue ? "1" : "0");

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ULONG
        CosaDmlGatewayV4SetPortScanProtect
            (
                BOOL                        *pValue
            );

    Description:

        Backend implementation to set V4_PORTSCANPROTECT value to syscfg

    Arguments:    BOOL                     *pValue

    Return:       The status of the operation.

**********************************************************************/
ULONG
CosaDmlGatewayV4SetPortScanProtect
(
    BOOL                        bValue
)
{
    syscfg_set_commit(NULL, V4_PORTSCANPROTECT, bValue ? "1" : "0");

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ULONG
        CosaDmlGatewayV4SetIPFloodDetect
            (
                BOOL                        bValue
            );

    Description:

        Backend implementation to set V4_IPFLOODDETECT value to syscfg

    Arguments:    BOOL                     bValue

    Return:       The status of the operation.

**********************************************************************/
ULONG
CosaDmlGatewayV4SetIPFloodDetect
(
    BOOL                        bValue
)
{
    syscfg_set_commit(NULL, V4_IPFLOODDETECT, bValue ? "1" : "0");

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ULONG
        CosaDmlGatewayV6GetBlockFragIPPkts
            (
                BOOL                        *pValue
            );

    Description:

        Backend implementation to get V6_BLOCKFRAGIPPKT value from syscfg

    Arguments:    BOOL                     *pValue

    Return:       The status of the operation.

**********************************************************************/
ULONG
CosaDmlGatewayV6GetBlockFragIPPkts
(
    BOOL                        *pValue
)
{
    char buf[8];

    syscfg_get( NULL, V6_BLOCKFRAGIPPKT, buf, sizeof(buf));
    *pValue = (strcmp(buf, "1") == 0);

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ULONG
        CosaDmlGatewayV6GetPortScanProtect
            (
                BOOL                        *pValue
            );

    Description:

        Backend implementation to get V6_PORTSCANPROTECT value from syscfg

    Arguments:    BOOL                     *pValue

    Return:       The status of the operation.

**********************************************************************/
ULONG
CosaDmlGatewayV6GetPortScanProtect
(
    BOOL                        *pValue
)
{
    char buf[8];

    syscfg_get( NULL, V6_PORTSCANPROTECT, buf, sizeof(buf));
    *pValue = (strcmp(buf, "1") == 0);

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ULONG
        CosaDmlGatewayV6GetIPFloodDetect
            (
                BOOL                        *pValue
            );

    Description:

        Backend implementation to get V6_IPFLOODDETECT value from syscfg

    Arguments:    BOOL                     *pValue

    Return:       The status of the operation.

**********************************************************************/
ULONG
CosaDmlGatewayV6GetIPFloodDetect
(
    BOOL                        *pValue
)
{
    char buf[8];

    syscfg_get( NULL, V6_IPFLOODDETECT, buf, sizeof(buf));
    *pValue = (strcmp(buf, "1") == 0);

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ULONG
        CosaDmlGatewayV6SetBlockFragIPPkts
            (
                BOOL                       bValue
            );

    Description:

        Backend implementation to set V6_BLOCKFRAGIPPKT value to syscfg

    Arguments:    BOOL                     bValue

    Return:       The status of the operation.

**********************************************************************/
ULONG
CosaDmlGatewayV6SetBlockFragIPPkts
(
    BOOL                        bValue
)
{
    syscfg_set_commit(NULL, V6_BLOCKFRAGIPPKT, bValue ? "1" : "0");

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ULONG
        CosaDmlGatewayV6SetPortScanProtect
            (
                BOOL                        bValue
            );

    Description:

        Backend implementation to set V6_PORTSCANPROTECT value to syscfg

    Arguments:    BOOL                     bValue

    Return:       The status of the operation.

**********************************************************************/
ULONG
CosaDmlGatewayV6SetPortScanProtect
(
    BOOL                        bValue
)
{
    syscfg_set_commit(NULL, V6_PORTSCANPROTECT, bValue ? "1" : "0");

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ULONG
        CosaDmlGatewayV6SetIPFloodDetect
            (
                BOOL                        bValue
            );

    Description:

        Backend implementation to set V6_IPFLOODDETECT value to syscfg

    Arguments:    BOOL                     bValue

    Return:       The status of the operation.

**********************************************************************/
ULONG
CosaDmlGatewayV6SetIPFloodDetect
(
    BOOL                        bValue
)
{
    syscfg_set_commit(NULL, V6_IPFLOODDETECT, bValue ? "1" : "0");

    return ANSC_STATUS_SUCCESS;
}

#endif
