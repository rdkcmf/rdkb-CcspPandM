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

    copyright:

        Cisco Systems, Inc.
        All Rights Reserved.

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
    AnscCopyString(pCfg->Version,    g_FirewallConfig.Version);
    AnscCopyString(pCfg->LastChange, g_FirewallConfig.LastChange);

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

#elif (_COSA_INTEL_USG_ARM_ || _COSA_DRG_TPG_ || _COSA_BCM_MIPS_)

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
    return ANSC_STATUS_SUCCESS;
}

#endif
