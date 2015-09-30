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

    module: cosa_userinterface_apis.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    copyright:

        Cisco Systems, Inc.
        All Rights Reserved.

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaDmlRaInit
        *  CosaDmlRaSetCfg
        *  CosaDmlRaGetCfg
        *  CosaDmlRaGetSupportedProtocols
    -------------------------------------------------------------------

    environment:

        platform independent

    -------------------------------------------------------------------

    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**************************************************************************/

#include "cosa_userinterface_apis.h"

static COSA_DML_RA_CFG  g_RaCfg = {
    .bEnabled           = TRUE,
    .bFromAnyIp         = FALSE,
    .SupportedProtocols = "HTTP,HTTPS,SSH,Telnet",
    .StartIp.Dot        = {10, 74, 120, 100},
    .EndIp.Dot          = {10, 74, 120, 200},
    .HttpEnable         = TRUE,
    .HttpPort           = 80,
    .HttpsEnable        = TRUE,
    .HttpsPort          = 443,
    .SSHEnable          = FALSE,
    .SSHPort            = 22,
    .TelnetEnable       = FALSE,
    .TelnetPort         = 23,
    .UpgradePermission  = FALSE,
};

ANSC_STATUS
CosaDmlRaInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlRaSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RA_CFG            pCfg
    )
{
    if (pCfg)
        AnscCopyMemory(&g_RaCfg, pCfg, sizeof(COSA_DML_RA_CFG));
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlRaGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RA_CFG            pCfg
    )
{
    if (pCfg)
        AnscCopyMemory(pCfg, &g_RaCfg, sizeof(COSA_DML_RA_CFG));
    
    return ANSC_STATUS_SUCCESS;
}


ULONG
CosaDmlUIIPRangeGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    )
{
    fprintf(stderr, "%s is not implemented!\n", __FUNCTION__);

    return 0;
}

ANSC_STATUS
CosaDmlUIIPRangeGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_UI_IPRANGE_ENTRY  pEntry
    )
{
    fprintf(stderr, "%s is not implemented!\n", __FUNCTION__);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlUIIPRangeSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    fprintf(stderr, "%s is not implemented!\n", __FUNCTION__);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlUIIPRangeAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_UI_IPRANGE_ENTRY  pEntry
    )
{
    fprintf(stderr, "%s is not implemented!\n", __FUNCTION__);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlUIIPRangeDelEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_UI_IPRANGE_ENTRY  pEntry
    )
{
    fprintf(stderr, "%s is not implemented!\n", __FUNCTION__);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlUIIPRangeSetEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_UI_IPRANGE_ENTRY  pEntry
    )
{
    fprintf(stderr, "%s is not implemented!\n", __FUNCTION__);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlUIIPRangeGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_UI_IPRANGE_ENTRY  pEntry
    )
{
    fprintf(stderr, "%s is not implemented!\n", __FUNCTION__);

    return ANSC_STATUS_SUCCESS;
}

