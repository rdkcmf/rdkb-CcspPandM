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

    module: cosa_ip_apis.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    copyright:

        Cisco Systems, Inc.
        All Rights Reserved.

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

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

#include "cosa_apis.h"
#include "cosa_ip_apis.h"
#include "cosa_ip_apis_multilan.h"

/*
 *  simulation data -- IPv6 address/prefix of a single interface
 */
static  COSA_DML_IP_V6ADDR          gIpv6Addr   = {1, "ipv6a-1",    TRUE, 1, 1, "", 0, 0, 1, "", "", "", FALSE};
static  COSA_DML_IP_V6PREFIX        gIpv6Prefix = {1, "ipv6preA-1", TRUE, 1, 1, "", 0, 0, 1, 1,  "", "", TRUE, TRUE, "", ""};

//RDKB-EMU
extern ANSC_HANDLE bus_handle;
extern char g_Subsystem[32];


/*
 *  IP Interface
 */
ANSC_STATUS
CosaDmlIpInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    return  CosaDmlIpMlanInit(hDml, phContext);
}


ULONG
CosaDmlIpIfGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    )
{
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    return CosaDmlIpIfMlanGetNumberOfEntries(hContext);
}


PCOSA_DML_IP_IF_INFO
CosaDmlIpIfGetEntry2
    (
        ANSC_HANDLE                 hContext,
        ULONG                       InstanceNumber
    )
{
    return  NULL;
}

ANSC_STATUS
CosaDmlIpIfGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_IP_IF_FULL        pEntry
    )
{
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    return  CosaDmlIpIfMlanGetEntry(hContext, ulIndex, pEntry);
}


ANSC_STATUS
CosaDmlIpIfSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    return  CosaDmlIpIfMlanSetValues(hContext, ulIndex, ulInstanceNumber, pAlias);
}


ANSC_STATUS
CosaDmlIpIfAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_IP_IF_FULL        pEntry
    )
{
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    return  CosaDmlIpIfMlanAddEntry(hContext, pEntry);
}

ANSC_STATUS
CosaDmlIpIfDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    return  CosaDmlIpIfMlanDelEntry(hContext, ulInstanceNumber);
}

ANSC_STATUS
CosaDmlIpIfSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_IP_IF_CFG         pCfg
    )
{
    AnscTraceFlow
        ((
            "%s -- if instance number %d, if name %s\n",
            __FUNCTION__,
            pCfg->InstanceNumber,
            pCfg->LinkName
        ));

    return  CosaDmlIpIfMlanSetCfg(hContext, pCfg);
}


ANSC_STATUS
CosaDmlIpIfGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_IP_IF_CFG         pCfg
    )
{
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    return  CosaDmlIpIfMlanGetCfg(hContext, pCfg);
}


ANSC_STATUS
CosaDmlIpIfGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_IP_IF_INFO        pInfo
    )
{
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    return  CosaDmlIpIfMlanGetInfo(hContext, ulInstanceNumber, pInfo);
}


ANSC_STATUS
CosaDmlIpIfReset
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    return  CosaDmlIpIfMlanReset(hContext, ulInstanceNumber);
}


/*
 *  IP Interface IPv4Address
 */
ULONG
CosaDmlIpIfGetNumberOfV4Addrs
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber
    )
{
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    return  CosaDmlIpIfMlanGetNumberOfV4Addrs(hContext, ulIpIfInstanceNumber);
}


ANSC_STATUS
CosaDmlIpIfGetV4Addr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_IP_V4ADDR         pEntry
    )
{
	AnscTraceFlow(("%s...\n", __FUNCTION__));
	char status[100] ={0},path[256] = {0};//RDKB-EMU
	FILE *fp;
	int count = 0;
	fp = popen("ifconfig eth0  | grep inet | tr -s ' ' | cut -d ' ' -f3 | cut -d ':' -f2","r");
	if(fp == NULL)
	{
		printf("Failed to run command in Function %s\n",__FUNCTION__);
		return 0;
	}
	if(fgets(path, sizeof(path)-1, fp) != NULL)
	{
		for(count=0;path[count]!='\n';count++)
			status[count]=path[count];
		status[count]='\0';
	}
	PSM_Set_Record_Value2(bus_handle,g_Subsystem,"dmsb.l3net.1.V4Addr", ccsp_string, status);
	return  CosaDmlIpIfMlanGetV4Addr(hContext, ulIpIfInstanceNumber, ulIndex, pEntry);
}


ANSC_STATUS
CosaDmlIpIfSetV4AddrValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    return  CosaDmlIpIfMlanSetV4AddrValues(hContext, ulIpIfInstanceNumber, ulIndex, ulInstanceNumber, pAlias);
}


ANSC_STATUS
CosaDmlIpIfAddV4Addr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V4ADDR         pEntry
    )
{
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    return  CosaDmlIpIfMlanAddV4Addr(hContext, ulIpIfInstanceNumber, pEntry);
}


ANSC_STATUS
CosaDmlIpIfDelV4Addr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V4ADDR         pEntry
    )
{
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    return  CosaDmlIpIfMlanDelV4Addr(hContext, ulIpIfInstanceNumber, pEntry);
}

ANSC_STATUS
CosaDmlIpIfSetV4Addr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V4ADDR         pEntry
    )
{
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    return  CosaDmlIpIfMlanSetV4Addr(hContext, ulIpIfInstanceNumber, pEntry);
}


ANSC_STATUS
CosaDmlIpIfGetV4Addr2
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V4ADDR         pEntry         
    )
{
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    return  CosaDmlIpIfMlanGetV4Addr2(hContext, ulIpIfInstanceNumber, pEntry);
}

/*
 *  IP Interface IPv6Address
 */
PCOSA_DML_IP_V6ADDR
CosaDmlIPGetIPv6Addresses
    (
        PCOSA_DML_IP_IF_FULL2       p_ipif,
        PULONG                      p_num
    )
{
    PCOSA_DML_IP_V6ADDR             pV6AddrArray    = NULL;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    pV6AddrArray = AnscAllocateMemory(sizeof(COSA_DML_IP_V6ADDR));

    if ( pV6AddrArray )
    {
        AnscCopyMemory(pV6AddrArray, &gIpv6Addr, sizeof(COSA_DML_IP_V6ADDR));
        *p_num = 1;
    }
    else
    {
        *p_num = 0;
    }

    return  pV6AddrArray;
}


ANSC_STATUS
CosaDmlIpIfAddV6Addr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6ADDR         pEntry
    )
{
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    /* Add/Del V4/V6 address is not supported */
    return  ANSC_STATUS_UNAPPLICABLE;    
}


ANSC_STATUS
CosaDmlIpIfDelV6Addr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6ADDR         pEntry
    )
{
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    /* Add/Del V4/V6 address is not supported */
    return  ANSC_STATUS_UNAPPLICABLE;    
}

ANSC_STATUS
CosaDmlIpIfSetV6Addr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6ADDR         pEntry
    )
{
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    AnscCopyMemory(&gIpv6Addr, pEntry->IP6Address, sizeof(COSA_DML_IP_V6ADDR));

    return  ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlIpIfGetV6Addr2
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6ADDR         pEntry         
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    AnscCopyMemory(pEntry, &gIpv6Addr, sizeof(COSA_DML_IP_V6ADDR));

    return  returnStatus;
}


/*
 *  IP Interface IPv6Prefix
 */
PCOSA_DML_IP_V6PREFIX
CosaDmlIPGetIPv6Prefixes
    (
        PCOSA_DML_IP_IF_FULL2       p_ipif,
        PULONG                      p_num
    )
{
    PCOSA_DML_IP_V6PREFIX           pV6Prefix       = NULL;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    pV6Prefix = AnscAllocateMemory(sizeof(COSA_DML_IP_V6PREFIX));

    if ( pV6Prefix )
    {
        AnscCopyMemory(pV6Prefix, &gIpv6Prefix, sizeof(COSA_DML_IP_V6PREFIX));
        *p_num = 1;
    }
    else
    {
        *p_num = 0;
    }

    return  pV6Prefix;
}

ANSC_STATUS
CosaDmlIpIfAddV6Prefix
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6PREFIX       pEntry
    )
{
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    /* Add/Del V4/V6 address is not supported */
    return  ANSC_STATUS_UNAPPLICABLE;    
}

ANSC_STATUS
CosaDmlIpIfDelV6Prefix
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6PREFIX       pEntry
    )
{
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    /* Add/Del V4/V6 address is not supported */
    return  ANSC_STATUS_UNAPPLICABLE;    
}


ANSC_STATUS
CosaDmlIpIfSetV6Prefix
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6PREFIX       pEntry
    )
{
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    AnscCopyMemory(pEntry, &gIpv6Prefix, sizeof(COSA_DML_IP_V6PREFIX));
}


ANSC_STATUS
CosaDmlIpIfGetV6Prefix2 
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6PREFIX       pEntry         
    )
{
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    AnscCopyMemory(&gIpv6Prefix, pEntry, sizeof(COSA_DML_IP_V6PREFIX));
}

int 
CosaDmlGetPrefixPathName
    (
        char * ifname, 
        int inst1,
        PCOSA_DML_IP_V6ADDR p_v6addr,
        char * p_val
    )
{
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    return  -1;    
}

ULONG
CosaDmlIPv6addrGetV6Status
    (
        PCOSA_DML_IP_V6ADDR p_dml_v6addr, 
        PCOSA_DML_IP_IF_FULL2 p_ipif
    )
{
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    return  COSA_DML_IP6_ADDRSTATUS_Invalid;
}

int CosaDmlDateTimeCompare(char *p_dt1, char *p_dt2)
{
    return 0;
}

int CosaDmlIpv6AddrMatchesPrefix(char * pref_path, char * v6addr, int ipif_inst_num)
{
    return 0;
}


/*
 *  IP Interface statistics
 */
ANSC_STATUS
CosaDmlIpIfGetStats
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_STATS          pStats
    )
{
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    return  CosaDmlIpIfMlanGetStats(hContext, ulIpIfInstanceNumber, pStats);
}

/*
 *  IP Active Ports
 */
PCOSA_DML_IP_ACTIVE_PORT
CosaDmlIpGetActivePorts
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount
    )
{
    PCOSA_DML_IP_ACTIVE_PORT        pIPActivePort = NULL;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    pIPActivePort = (PCOSA_DML_IP_ACTIVE_PORT)AnscAllocateMemory( sizeof(COSA_DML_IP_ACTIVE_PORT) );

    if ( !pIPActivePort )
    {
        *pulCount = 0;
        return NULL;
    }
    else
    {
        *pulCount = 1;
    }
    
    AnscCopyString(pIPActivePort->LocalIPAddress, "127.0.0.1");
    AnscCopyString(pIPActivePort->RemoteIPAddress, "202.114.96.1");
                                
    pIPActivePort->LocalPort     = 8000;
    pIPActivePort->RemotePort    = 80;
    pIPActivePort->Status        = (COSA_DML_IP_PORT_STATUS)COSA_DML_IP_PORT_STATUS_Established;
    
    
    return pIPActivePort;
}

