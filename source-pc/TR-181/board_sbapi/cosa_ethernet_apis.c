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

    module: cosa_ethernet_apis.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaDmlEthInit
        *  CosaDmlEthPortGetNumberOfEntries
        *  CosaDmlEthPortGetEntry
        *  CosaDmlEthPortSetCfg
        *  CosaDmlEthPortGetCfg
        *  CosaDmlEthPortGetDinfo
        *  CosaDmlEthPortGetStats
        *  CosaDmlEthLinkGetNumberOfEntries
        *  CosaDmlEthLinkGetEntry
        *  CosaDmlEthLinkAddEntry
        *  CosaDmlEthLinkDelEntry
        *  CosaDmlEthLinkSetCfg
        *  CosaDmlEthLinkGetCfg
        *  CosaDmlEthLinkGetDinfo
        *  CosaDmlEthLinkGetStats
        *  CosaDmlEthVlanTerminationGetNumberOfEntries
        *  CosaDmlEthVlanTerminationGetEntry
        *  CosaDmlEthVlanTerminationAddEntry
        *  CosaDmlEthVlanTerminationDelEntry
        *  CosaDmlEthVlanTerminationSetCfg
        *  CosaDmlEthVlanTerminationGetCfg
        *  CosaDmlEthVlanTerminationGetDinfo
        *  CosaDmlEthVlanTerminationGetStats
    -------------------------------------------------------------------

    environment:

        Linux x86

    -------------------------------------------------------------------

    author:

        Shiyan Wang
        Ding Hua

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**************************************************************************/

#include "cosa_ethernet_apis.h"
#include "cosa_ethernet_apis_multilan.h"

#include "plugin_main_apis.h"
#include "ccsp_psm_helper.h"
#include "dmsb_tr181_psm_definitions.h"

#include "linux/if.h"
#include "linux/sockios.h"
#include <sys/ioctl.h>


/**************************************************************************
                        GLOBAL VARIABLES
**************************************************************************/

//#define ETH_INT_COUNT                               2
//RDKB-EMULATOR
#define ETH_INT_COUNT                               3

struct ethernet_ports_details
{
        char Name[64];
        UCHAR MacAddress[6];
};

COSA_DML_ETH_PORT_FULL              g_EthPortFull1 =
{
    {1,         "EthPort1",     TRUE,            1000,      COSA_DML_ETH_DUPLEX_Auto},
    {"eth0",    TRUE,           {0}                                                 },
    {COSA_DML_IF_STATUS_Up,     0,  2, {0x38, 0xC8, 0x5C, 0xFF, 0xD9, 0x01, 0x38, 0xC8, 0x5C, 0xFF, 0xD9, 0x02}}
};

COSA_DML_ETH_PORT_FULL              g_EthPortFull2 =
{
    {2,         "EthPort2",   TRUE,            1000,      COSA_DML_ETH_DUPLEX_Auto},
    {"eth1",    FALSE,          {0}                                                 },
    {COSA_DML_IF_STATUS_Up,     0,  2, {0x38, 0xC8, 0x5C, 0xFF, 0xD9, 0x03, 0x38, 0xC8, 0x5C, 0xFF, 0xD9, 0x04}}
};

ANSC_STATUS
CosaDmlEthInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    return  CosaDmlEthMlanInit(hDml, phContext);
}

ULONG
CosaDmlEthPortGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    )
{  
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    return ETH_INT_COUNT;
}

ANSC_STATUS
CosaDmlEthPortGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_ETH_PORT_FULL     pEntry
    )
{
    struct ethernet_ports_details interface_details;
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    if (pEntry)
        _ansc_memset(pEntry, 0, sizeof(COSA_DML_ETH_PORT_FULL));
    else
        return ANSC_STATUS_FAILURE;
    
    CcspHalGetInterfaceDetails(ulIndex+1,&interface_details);//RDKB-EMULATOR        
    strcpy(pEntry->StaticInfo.Name,interface_details.Name);
    AnscCopyMemory(pEntry->StaticInfo.MacAddress,interface_details.MacAddress,6);
/*  if (ulIndex == 0)
    {
        AnscCopyMemory(pEntry, &g_EthPortFull1, sizeof(COSA_DML_ETH_PORT_FULL));
    }
    else if (ulIndex == 1)
    {
        AnscCopyMemory(pEntry, &g_EthPortFull2, sizeof(COSA_DML_ETH_PORT_FULL));
    }
    else
    {
        return ANSC_STATUS_FAILURE;
    }*/

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlEthPortSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlEthPortSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ETH_PORT_CFG      pCfg
    )
{
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    if ( !pCfg )
        return ANSC_STATUS_FAILURE;

    CcspHalSetInterfaceEnableDetails(pCfg->InstanceNumber,pCfg->bEnabled);
    if (pCfg->InstanceNumber == 1)
    {
        AnscCopyMemory(&g_EthPortFull1.Cfg, pCfg, sizeof(COSA_DML_ETH_PORT_CFG));
    }
    else if (pCfg->InstanceNumber == 2)
    {
        AnscCopyMemory(&g_EthPortFull2.Cfg, pCfg, sizeof(COSA_DML_ETH_PORT_CFG));
    }
    else
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlEthPortGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ETH_PORT_CFG      pCfg
    )
{
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    if (!pCfg)
        return ANSC_STATUS_FAILURE;

       CcspHalGetInterfaceEnableDetails(pCfg->InstanceNumber,&pCfg->bEnabled);

#if 0//RDKB-EMULATOR
    if (pCfg->InstanceNumber == 1)
    {
        AnscCopyMemory(pCfg, &g_EthPortFull1.Cfg, sizeof(COSA_DML_ETH_PORT_CFG));
    }
    else if (pCfg->InstanceNumber == 2)
    {
        AnscCopyMemory(pCfg, &g_EthPortFull2.Cfg, sizeof(COSA_DML_ETH_PORT_CFG));
    }
    else
        return ANSC_STATUS_FAILURE;
#endif

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlEthPortGetDinfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_ETH_PORT_DINFO    pInfo
    )
{
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    if (!pInfo)
        return ANSC_STATUS_FAILURE;

    _ansc_memset(pInfo, 0, sizeof(COSA_DML_ETH_PORT_DINFO));
	
    CcspHalGetInterfaceStatusDetails(ulInstanceNumber,pInfo);//RDKB-EMULATOR
#if 0//RDKB-EMULATOR
    if (ulInstanceNumber == 1)
    {
        AnscCopyMemory(pInfo, &g_EthPortFull1.DynamicInfo, sizeof(COSA_DML_ETH_PORT_DINFO));
        pInfo->Status = COSA_DML_IF_STATUS_Up;
    }
    else if (ulInstanceNumber == 2)
    {
        AnscCopyMemory(pInfo, &g_EthPortFull2.DynamicInfo, sizeof(COSA_DML_ETH_PORT_DINFO));
        pInfo->Status = COSA_DML_IF_STATUS_Up;
    }
    else
        return ANSC_STATUS_FAILURE;
#endif
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlEthPortGetStats
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_ETH_STATS         pStats
    )
{
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    if (!pStats)
        return ANSC_STATUS_FAILURE;

    _ansc_memset(pStats, 0, sizeof(COSA_DML_ETH_STATS));
  
    //RDKB-EMU
    if(ulInstanceNumber == 1)
	    if (CosaUtilGetIfStats("eth1", pStats) != 0)
		    return ANSC_STATUS_SUCCESS;
    if(ulInstanceNumber == 2)
	    if (CosaUtilGetIfStats("eth0", pStats) != 0)
		    return ANSC_STATUS_SUCCESS;
    if(ulInstanceNumber == 3)
	    if (CosaUtilGetIfStats("eth2", pStats) != 0)
		    return ANSC_STATUS_SUCCESS;

    /*  if (ulInstanceNumber == 1)
    {
        pStats->BytesSent = 11;
    }
    else if (ulInstanceNumber == 2)
    {
        pStats->BytesSent = 22;
    }
    else
        return ANSC_STATUS_FAILURE;*/

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************
       Ethernet Link  -- moved to cosa_ethernet_apis_multilan.c
**********************************************************************/

/**********************************************************************
                            Ethernet VLANTermination
**********************************************************************/

ULONG
CosaDmlEthVlanTerminationGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    )
{
    return 0;
}

ANSC_STATUS
CosaDmlEthVlanTerminationGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_ETH_VLAN_TERMINATION_FULL pEntry
    )
{
    return ANSC_STATUS_CANT_FIND;
}

ANSC_STATUS
CosaDmlEthVlanTerminationSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    return ANSC_STATUS_CANT_FIND;
}

ANSC_STATUS
CosaDmlEthVlanTerminationAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ETH_VLAN_TERMINATION_FULL pEntry
    )
{
    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDmlEthVlanTerminationDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
    return ANSC_STATUS_CANT_FIND;
}

ANSC_STATUS
CosaDmlEthVlanTerminationValidateCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ETH_VLAN_TERMINATION_CFG pCfg,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    return FALSE;
}

ANSC_STATUS
CosaDmlEthVlanTerminationSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ETH_VLAN_TERMINATION_CFG pCfg
    )
{
    return ANSC_STATUS_CANT_FIND;
}

ANSC_STATUS
CosaDmlEthVlanTerminationGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ETH_VLAN_TERMINATION_CFG pCfg
    )
{
    return ANSC_STATUS_CANT_FIND;
}

ANSC_STATUS
CosaDmlEthVlanTerminationGetDinfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_ETH_VLAN_TERMINATION_DINFO pInfo
    )
{
    return ANSC_STATUS_CANT_FIND;
}

ANSC_STATUS
CosaDmlEthVlanTerminationGetStats
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_ETH_STATS         pStats
    )
{
    return ANSC_STATUS_CANT_FIND;
}


