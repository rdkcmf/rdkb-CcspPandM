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

    module: cosa_dhcpv4_apis.c

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
        03/15/2011    Added for backend Dhcp4 clients.
        04/08/2011    Added Dhcp4 clients Sent Options and Req Options.
 

**************************************************************************/
#include "cosa_apis.h"
#include "cosa_dhcpv4_apis.h"
#include "cosa_dhcpv4_internal.h"
#include "plugin_main_apis.h"


COSA_DML_DHCPC_FULL  g_dhcpv4_client[] =
    {
        {
            {
                1,
                "Client1",
                TRUE,
                "Layer3Interface1",
                TRUE,
                "POOL1"
            },
            {
                COSA_DML_DHCP_STATUS_Enabled,
                COSA_DML_DHCPC_STATUS_Bound,
                {
                    "\x40\x40\x40\x01"
                },
                {
                    "\xff\xff\xff\x00"
                },
                1,
                {
                    "\xff\xff\xff\xf4"
                },
                1,
                {
                    "\xff\xff\xff\xf3"
                },
                12722,
                {
                    "\x40\x40\x40\x09"
                }
            }
        }
    };


COSA_DML_DHCP_OPT    g_dhcpv4_client_sent[] = 
    {
        {
            1,
            "SentOption1",
            TRUE,
            2,
            "23243abf3"
        },
        {
            2,
            "SentOption2",
            TRUE,
            3,
            "23243abf3"
        }
    };

COSA_DML_DHCPC_REQ_OPT g_dhcpv4_client_req[] =
    {
        {
            1,
            "ReqOption1",
            TRUE,
            1,
            23,
            "623552xaf327"
        },
        {
            2,
            "ReqOption2",
            TRUE,
            1,
            232,
            "223552xaf327"
        }
    };

BOOL  g_dhcpv4_server = TRUE;

COSA_DML_DHCPS_POOL_FULL  g_dhcpv4_server_pool[]=
{
    {
        {
            1,
            "Pool1",
            TRUE,
            1,
            "Layer3Interface",
            "23feaa",                       /*VendorClassID*/
            TRUE,
            COSA_DML_DHCPS_VID_MODE_Prefix, /*VendorClassIDMode*/
            "a23443",
            TRUE,                           /*ClientIDExclude*/
            "a23443",
            TRUE,                           /*UserClassIDExclude*/
            "\x23\x23\x23\x23\x23\x23",
            "\xff\xff\xff\xff\xff\xff",
            TRUE,
            TRUE,
            {
                "\xc0\xa8\x00\x03"
            },
            {
                "\xc0\xa8\x00\xfc"
            },        
            {
                {
                    "\xc0\xa8\x00\x01"
                },
                {
                    "\xc0\xa8\x00\x02"
                } 
            },
            {
                "\xff\xff\xff\x00"
            },  
            {
                {
                    "\xc0\xa8\x00\x01"
                },
                {
                    "\xc0\xa8\x00\x02"
                } 
            },
            "cisco.com",
            {
                {
                    "\xc0\xa8\x00\x01"
                },
                {
                    "\xc0\xa8\x00\x02"
                } 
            },
            232323
        },
        {
            COSA_DML_DHCP_STATUS_Enabled
        }
    }
};

COSA_DML_DHCPS_SADDR g_dhcpv4_server_pool_stadicaddress[] =
    {
        {
            1,
            "StaticAddress1",
            TRUE,
            "\x23\x23\x23\x23\x23\x23",
            {
                "\xc0\xa8\x00\x10"
            }
        },
        {
            2,
            "StaticAddress2",
            TRUE,
            "\x23\x23\x23\x23\x23\x23",
            {
                "\xc0\xa8\x00\x20"
            }
        }
    };


COSA_DML_DHCPSV4_CLIENT g_dhcpv4_server_client[] = 
{
    "client1",
    "01:00:5e:00:00:16",
    TRUE
};

COSA_DML_DHCPSV4_CLIENT_IPADDRESS g_dhcpv4_server_client_ipaddress[] = 
{
   {
        0x01010101,
        "2011-11-23T23:59:59Z",
    }
};


COSA_DML_DHCPSV4_CLIENT_OPTION g_dhcpv4_server_client_option[] = 
{
  
    {
        0x6,
        "Value1"
    }
};

COSA_DML_DHCPSV4_OPTION g_dhcpv4_server_pool_option[] =
{
    {
        1,
        "Option1",
        6,
        "Value2",
        TRUE
    }
};


ANSC_STATUS
CosaDmlDhcpInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    return ANSC_STATUS_SUCCESS;
}

/*
    Description:
        The API retrieves the number of DHCP clients in the system.
 */
ULONG
CosaDmlDhcpcGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    )
{
    PCOSA_DATAMODEL_DHCPV4          pDhcpv4           = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;
    
    return 1;
}

/*
    Description:
        The API retrieves the complete info of the DHCP clients designated by index. The usual process is the caller gets the total number of entries, then iterate through those by calling this API.
    Arguments:
        ulIndex        Indicates the index number of the entry.
        pEntry        To receive the complete info of the entry.
*/
ANSC_STATUS
CosaDmlDhcpcGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_DHCPC_FULL        pEntry
    )
{  
    
   if ( ulIndex+1 > sizeof(g_dhcpv4_client)/sizeof(COSA_DML_DHCPC_FULL) )
        return ANSC_STATUS_FAILURE;    
    
    AnscCopyMemory(pEntry, &g_dhcpv4_client[ulIndex], sizeof(COSA_DML_DHCPC_FULL));
        
        return ANSC_STATUS_SUCCESS;
    }

ANSC_STATUS
CosaDmlDhcpcSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    if ( ulIndex+1 > sizeof(g_dhcpv4_client)/sizeof(COSA_DML_DHCPC_FULL) )
        return ANSC_STATUS_SUCCESS;

    g_dhcpv4_client[ulIndex].Cfg.InstanceNumber  = ulInstanceNumber;
    AnscCopyString(g_dhcpv4_client[ulIndex].Cfg.Alias, pAlias);
    
    return ANSC_STATUS_SUCCESS;
}

/*
    Description:
        The API adds DHCP client. 
    Arguments:
        pEntry        Caller fills in pEntry->Cfg, except Alias field. Upon return, callee fills pEntry->Cfg.Alias field and as many as possible fields in pEntry->Info.
*/
ANSC_STATUS
CosaDmlDhcpcAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPC_FULL        pEntry
    )
{
    return ANSC_STATUS_SUCCESS;
    }


/*
    Description:
        The API removes the designated DHCP client entry. 
    Arguments:
        pAlias        The entry is identified through Alias.
*/
ANSC_STATUS
CosaDmlDhcpcDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
            return ANSC_STATUS_SUCCESS;
        }

/*
Description:
    The API re-configures the designated DHCP client entry. 
Arguments:
    pAlias        The entry is identified through Alias.
    pEntry        The new configuration is passed through this argument, even Alias field can be changed.
*/
ANSC_STATUS
CosaDmlDhcpcSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPC_CFG         pCfg
    )
{
    ULONG                           index  = 0;

    for( index = 0 ; index < sizeof (g_dhcpv4_client)/sizeof(COSA_DML_DHCPC_FULL); index++)
    {
        if ( pCfg->InstanceNumber == g_dhcpv4_client[index].Cfg.InstanceNumber )
        {
            AnscCopyMemory(&g_dhcpv4_client[index].Cfg, pCfg, sizeof(COSA_DML_DHCPC_CFG));
            return ANSC_STATUS_SUCCESS;
        }
    }   

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpcGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPC_CFG         pCfg
    )
{
    ULONG                           index  = 0;
  
    for( index = 0 ; index < sizeof (g_dhcpv4_client)/sizeof(COSA_DML_DHCPC_FULL); index++)
    {
        if ( pCfg->InstanceNumber == g_dhcpv4_client[index].Cfg.InstanceNumber )
        {
            AnscCopyMemory(pCfg,  &g_dhcpv4_client[index].Cfg, sizeof(COSA_DML_DHCPC_CFG));
            return ANSC_STATUS_SUCCESS;
        }
    }
      
    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDmlDhcpcGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_DHCPC_INFO        pInfo
    )
{
    ULONG                           index  = 0;
   
    for( index = 0 ; index < sizeof (g_dhcpv4_client)/sizeof(COSA_DML_DHCPC_FULL); index++)
    {
        if ( ulInstanceNumber == g_dhcpv4_client[index].Cfg.InstanceNumber )
        {
            AnscCopyMemory(pInfo,  &g_dhcpv4_client[index].Info, sizeof(COSA_DML_DHCPC_INFO));
            return ANSC_STATUS_SUCCESS;
        }
    }   
    
    return ANSC_STATUS_FAILURE;
}

/*
    Description:
        The API initiates a DHCP client renewal. 
    Arguments:
        pAlias        The entry is identified through Alias.
*/
ANSC_STATUS
CosaDmlDhcpcRenew
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{  
    CcspTraceInfo((" CosaDmlDhcpcRenew -- ulInstanceNumber:%d.\n", ulInstanceNumber)); 

    return ANSC_STATUS_SUCCESS;
}

/*
 *  DHCP Client Send/Req Option
 *
 *  The options are managed on top of a DHCP client,
 *  which is identified through pClientAlias
 */
ULONG
CosaDmlDhcpcGetNumberOfSentOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber
    )
{
    return sizeof(g_dhcpv4_client_sent)/sizeof(COSA_DML_DHCP_OPT);
}

ANSC_STATUS
CosaDmlDhcpcGetSentOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_DHCP_OPT          pEntry
    )
{
    PCOSA_DML_DHCP_OPT              pDhcpSendOption      = pEntry;

    if ( ulIndex+1 > sizeof(g_dhcpv4_client_sent)/sizeof(COSA_DML_DHCP_OPT) )
        return ANSC_STATUS_FAILURE;

    AnscCopyMemory( pEntry, &g_dhcpv4_client_sent[ulIndex], sizeof(COSA_DML_DHCP_OPT));

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlDhcpcGetSentOptionbyInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCP_OPT          pEntry
    )
{   
    ULONG                           index  = 0;

    for( index=0;  index<sizeof(g_dhcpv4_client_sent)/sizeof(COSA_DML_DHCP_OPT); index++)
    {
        if ( pEntry->InstanceNumber == g_dhcpv4_client_sent[index].InstanceNumber )
        {
            AnscCopyMemory( pEntry, &g_dhcpv4_client_sent[index], sizeof(COSA_DML_DHCP_OPT));
             return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_FAILURE;
}


ANSC_STATUS
CosaDmlDhcpcSetSentOptionValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    if ( ulIndex+1 > sizeof(g_dhcpv4_client_sent)/sizeof(COSA_DML_DHCP_OPT) )
        return ANSC_STATUS_SUCCESS;

    g_dhcpv4_client_sent[ulIndex].InstanceNumber  = ulInstanceNumber;
    AnscCopyString( g_dhcpv4_client_sent[ulIndex].Alias, pAlias);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpcAddSentOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCP_OPT          pEntry
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpcDelSentOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        ULONG                       ulInstanceNumber
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpcSetSentOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCP_OPT          pEntry
    )
{
    ULONG                           index  = 0;

    for( index=0;  index<sizeof(g_dhcpv4_client_sent)/sizeof(COSA_DML_DHCP_OPT); index++)
    {
        if ( pEntry->InstanceNumber == g_dhcpv4_client_sent[index].InstanceNumber )
        {
            AnscCopyMemory( &g_dhcpv4_client_sent[index], pEntry, sizeof(COSA_DML_DHCP_OPT));
            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_SUCCESS;
}

/*
 *  DHCP Client Send/Req Option
 *
 *  The options are managed on top of a DHCP client,
 *  which is identified through pClientAlias
 */
ULONG
CosaDmlDhcpcGetNumberOfReqOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber
    )
{
        return sizeof(g_dhcpv4_client_req)/sizeof(COSA_DML_DHCPC_REQ_OPT);
}

ANSC_STATUS
CosaDmlDhcpcGetReqOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_DHCPC_REQ_OPT     pEntry
    )
{   
    if ( ulIndex+1 > sizeof(g_dhcpv4_client_req)/sizeof(COSA_DML_DHCPC_REQ_OPT) )
        return ANSC_STATUS_FAILURE;

    AnscCopyMemory(pEntry, &g_dhcpv4_client_req[ulIndex], sizeof(COSA_DML_DHCPC_REQ_OPT));
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpcGetReqOptionbyInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCPC_REQ_OPT     pEntry
    )
{   
    ULONG                           index  = 0;

    for( index=0;  index<sizeof(g_dhcpv4_client_req)/sizeof(COSA_DML_DHCPC_REQ_OPT); index++)
    {
        if ( pEntry->InstanceNumber == g_dhcpv4_client_req[index].InstanceNumber )
            {
                AnscCopyMemory( pEntry, &g_dhcpv4_client_req[index], sizeof(COSA_DML_DHCPC_REQ_OPT));
                return ANSC_STATUS_SUCCESS;
            }
    }

    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDmlDhcpcSetReqOptionValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{   
    if ( ulIndex+1 > sizeof(g_dhcpv4_client_req)/sizeof(COSA_DML_DHCPC_REQ_OPT) )
        return ANSC_STATUS_SUCCESS;

    g_dhcpv4_client_req[ulIndex].InstanceNumber  = ulInstanceNumber;
    AnscCopyString(g_dhcpv4_client_req[ulIndex].Alias, pAlias);
    
    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlDhcpcAddReqOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCPC_REQ_OPT     pEntry
    )
{

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpcDelReqOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        ULONG                       ulInstanceNumber
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpcSetReqOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCPC_REQ_OPT     pEntry
    )
{
    ULONG                           index  = 0;

    for( index=0;  index<sizeof(g_dhcpv4_client_req)/sizeof(COSA_DML_DHCPC_REQ_OPT); index++)
    {
        if ( pEntry->InstanceNumber == g_dhcpv4_client_req[index].InstanceNumber )
            {
                AnscCopyMemory( &g_dhcpv4_client_req[index], pEntry, sizeof(COSA_DML_DHCPC_REQ_OPT));
                return ANSC_STATUS_SUCCESS;
            }
    }

    return ANSC_STATUS_SUCCESS;
}

/*
 *  DHCP Server
 */
ANSC_STATUS
CosaDmlDhcpsEnable
    (
        ANSC_HANDLE                 hContext,
        BOOL                        bEnable
    )
{
    g_dhcpv4_server = bEnable;
    
    return ANSC_STATUS_SUCCESS;
}

/*
    Description:
        The API retrieves the current state of DHCP server: Enabled or Disabled. 
*/
BOOLEAN
CosaDmlDhcpsGetState
    (
        ANSC_HANDLE                 hContext
    )
{
    return g_dhcpv4_server;
}


/*
 *  DHCP Server X_COM_CISCO_StaticAddress
 *
 *  The static addresses are managed on top of a DHCP server pool,
 *  which is identified through pPoolAlias
 */
ULONG
CosaDmlDhcpsGetNumberOfX_COM_CISCO_Saddr
    (
        ANSC_HANDLE                 hContext
    )
{
    return sizeof(g_dhcpv4_server_pool_stadicaddress)/sizeof(COSA_DML_DHCPS_SADDR);
}

ANSC_STATUS
CosaDmlDhcpsGetX_COM_CISCO_Saddr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_DHCPS_SADDR       pEntry
    )
{        
    if ( ulIndex+1 > sizeof(g_dhcpv4_server_pool_stadicaddress)/sizeof(COSA_DML_DHCPS_SADDR) )
        return ANSC_STATUS_FAILURE;

    AnscCopyMemory(pEntry, &g_dhcpv4_server_pool_stadicaddress[ulIndex], sizeof(COSA_DML_DHCPS_SADDR));
    
    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlDhcpsGetX_COM_CISCO_SaddrbyInsNum
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPS_SADDR       pEntry
    )
{
    ULONG                           index = 0;

    for(index = 0; index < sizeof(g_dhcpv4_server_pool_stadicaddress[index])/sizeof(COSA_DML_DHCPS_SADDR); index++)
    {
        if ( pEntry->InstanceNumber == g_dhcpv4_server_pool_stadicaddress[index].InstanceNumber )
        {
            AnscCopyMemory(pEntry, &g_dhcpv4_server_pool_stadicaddress[index], sizeof(COSA_DML_DHCPS_SADDR));
            return ANSC_STATUS_SUCCESS;
        }
    }
    
    return ANSC_STATUS_FAILURE;
}


ANSC_STATUS
CosaDmlDhcpsSetX_COM_CISCO_SaddrValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    if ( ulIndex+1 > sizeof(g_dhcpv4_server_pool_stadicaddress)/sizeof(COSA_DML_DHCPS_SADDR) )
        return ANSC_STATUS_SUCCESS;

    g_dhcpv4_server_pool_stadicaddress[ulIndex].InstanceNumber  = ulInstanceNumber;
    AnscCopyString( g_dhcpv4_server_pool_stadicaddress[ulIndex].Alias, pAlias );

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpsAddX_COM_CISCO_Saddr
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPS_SADDR       pEntry
    )
{

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpsDelX_COM_CISCO_Saddr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpsSetX_COM_CISCO_Saddr
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPS_SADDR       pEntry
    )
{
    ULONG                           index = 0;

    for(index = 0; index < sizeof(g_dhcpv4_server_pool_stadicaddress[index])/sizeof(COSA_DML_DHCPS_SADDR); index++)
    {
        if ( pEntry->InstanceNumber == g_dhcpv4_server_pool_stadicaddress[index].InstanceNumber )
        {
            AnscCopyMemory(&g_dhcpv4_server_pool_stadicaddress[index], pEntry, sizeof(COSA_DML_DHCPS_SADDR));
            return ANSC_STATUS_SUCCESS;
        }
    }
    
    return ANSC_STATUS_SUCCESS;
}

/*
 *  DHCP Server Pool
 */
ULONG
CosaDmlDhcpsGetNumberOfPools
    (
        ANSC_HANDLE                 hContext
    )
{
    return sizeof(g_dhcpv4_server_pool)/sizeof(COSA_DML_DHCPS_POOL_FULL);
}

ANSC_STATUS
CosaDmlDhcpsGetPool
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_DHCPS_POOL_FULL   pEntry
    )
{
    if ( ulIndex+1 > sizeof(g_dhcpv4_server_pool)/sizeof(COSA_DML_DHCPS_POOL_FULL) )
        return ANSC_STATUS_FAILURE;

    AnscCopyMemory(pEntry, &g_dhcpv4_server_pool[ulIndex], sizeof(COSA_DML_DHCPS_POOL_FULL));
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpsSetPoolValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    if ( ulIndex+1 > sizeof(g_dhcpv4_server_pool)/sizeof(COSA_DML_DHCPS_POOL_FULL) )
        return ANSC_STATUS_SUCCESS;

    g_dhcpv4_server_pool[ulIndex].Cfg.InstanceNumber  = ulInstanceNumber;
    AnscCopyString(g_dhcpv4_server_pool[ulIndex].Cfg.Alias, pAlias);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpsAddPool
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPS_POOL_FULL   pEntry
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpsDelPool
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpsSetPoolCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPS_POOL_CFG    pCfg
    )
{
    ULONG                          index = 0;

    for(index = 0; index <sizeof(g_dhcpv4_server_pool)/sizeof(COSA_DML_DHCPS_POOL_FULL); index++)
    {
        if ( pCfg->InstanceNumber == g_dhcpv4_server_pool[index].Cfg.InstanceNumber )
        {
            AnscCopyMemory( &g_dhcpv4_server_pool[index].Cfg, pCfg, sizeof(COSA_DML_DHCPS_POOL_FULL) );
            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpsGetPoolCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPS_POOL_CFG    pCfg
    )
{
    ULONG                          index = 0;

    for(index = 0; index <sizeof(g_dhcpv4_server_pool)/sizeof(COSA_DML_DHCPS_POOL_FULL); index++)
    {
        if ( pCfg->InstanceNumber == g_dhcpv4_server_pool[index].Cfg.InstanceNumber )
        {
            AnscCopyMemory( pCfg, &g_dhcpv4_server_pool[index].Cfg, sizeof(COSA_DML_DHCPS_POOL_FULL) );
            return ANSC_STATUS_FAILURE;
        }
    }

    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDmlDhcpsGetPoolInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_DHCPS_POOL_INFO   pInfo
    )
{
    ULONG                          index = 0;

    for(index = 0; index <sizeof(g_dhcpv4_server_pool)/sizeof(COSA_DML_DHCPS_POOL_FULL); index++)
    {
        if ( ulInstanceNumber == g_dhcpv4_server_pool[index].Cfg.InstanceNumber )
        {
            AnscCopyMemory( pInfo, &g_dhcpv4_server_pool[index].Info, sizeof(COSA_DML_DHCPS_POOL_INFO) );
            return ANSC_STATUS_FAILURE;
        }
    }

    return ANSC_STATUS_FAILURE;
}

/*
 *  DHCP Server Pool Static Address
 *
 *  The static addresses are managed on top of a DHCP server pool,
 *  which is identified through pPoolAlias
 */
ULONG
CosaDmlDhcpsGetNumberOfSaddr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber
    )
{
    return sizeof(g_dhcpv4_server_pool_stadicaddress)/sizeof(COSA_DML_DHCPS_SADDR);
}

ANSC_STATUS
CosaDmlDhcpsGetSaddr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_DHCPS_SADDR       pEntry
    )
{        
    if ( ulIndex+1 > sizeof(g_dhcpv4_server_pool_stadicaddress)/sizeof(COSA_DML_DHCPS_SADDR) )
        return ANSC_STATUS_FAILURE;

    AnscCopyMemory(pEntry, &g_dhcpv4_server_pool_stadicaddress[ulIndex], sizeof(COSA_DML_DHCPS_SADDR));
    
    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlDhcpsGetSaddrbyInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCPS_SADDR       pEntry
    )
{
    ULONG                           index = 0;

    for(index = 0; index < sizeof(g_dhcpv4_server_pool_stadicaddress[index])/sizeof(COSA_DML_DHCPS_SADDR); index++)
    {
        if ( pEntry->InstanceNumber == g_dhcpv4_server_pool_stadicaddress[index].InstanceNumber )
        {
            AnscCopyMemory(pEntry, &g_dhcpv4_server_pool_stadicaddress[index], sizeof(COSA_DML_DHCPS_SADDR));
            return ANSC_STATUS_SUCCESS;
        }
    }
    
    return ANSC_STATUS_FAILURE;
}


ANSC_STATUS
CosaDmlDhcpsSetSaddrValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    if ( ulIndex+1 > sizeof(g_dhcpv4_server_pool_stadicaddress)/sizeof(COSA_DML_DHCPS_SADDR) )
        return ANSC_STATUS_SUCCESS;

    g_dhcpv4_server_pool_stadicaddress[ulIndex].InstanceNumber  = ulInstanceNumber;
    AnscCopyString( g_dhcpv4_server_pool_stadicaddress[ulIndex].Alias, pAlias );

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpsAddSaddr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        PCOSA_DML_DHCPS_SADDR       pEntry
    )
{

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpsDelSaddr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulInstanceNumber
    )
{

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpsSetSaddr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        PCOSA_DML_DHCPS_SADDR       pEntry
    )
{
    ULONG                           index = 0;

    for(index = 0; index < sizeof(g_dhcpv4_server_pool_stadicaddress[index])/sizeof(COSA_DML_DHCPS_SADDR); index++)
    {
        if ( pEntry->InstanceNumber == g_dhcpv4_server_pool_stadicaddress[index].InstanceNumber )
        {
            AnscCopyMemory(&g_dhcpv4_server_pool_stadicaddress[index], pEntry, sizeof(COSA_DML_DHCPS_SADDR));
            return ANSC_STATUS_SUCCESS;
        }
    }
    
    return ANSC_STATUS_SUCCESS;
}


/*
 *  DHCP Server Pool Option
 *
 *  The options are managed on top of a DHCP server pool,
 *  which is identified through pPoolAlias
 */
ULONG
CosaDmlDhcpsGetNumberOfOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber
    )
{
    return sizeof(g_dhcpv4_server_pool_option) / sizeof(COSA_DML_DHCPSV4_OPTION);
}

ANSC_STATUS
CosaDmlDhcpsGetOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_DHCPSV4_OPTION    pEntry
    )
{
    if ( ulIndex+1 > sizeof(g_dhcpv4_server_pool_option)/sizeof(COSA_DML_DHCPSV4_OPTION) )
        return ANSC_STATUS_FAILURE;

    AnscCopyMemory(pEntry, &g_dhcpv4_server_pool_option[ulIndex], sizeof(COSA_DML_DHCPSV4_OPTION));
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpsGetOptionbyInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        PCOSA_DML_DHCPSV4_OPTION    pEntry
    )
{
    ULONG                           index = 0;

    for(index = 0; index < sizeof(g_dhcpv4_server_pool_option[index])/sizeof(COSA_DML_DHCPSV4_OPTION); index++)
    {
        if ( pEntry->InstanceNumber == g_dhcpv4_server_pool_option[index].InstanceNumber )
        {
            AnscCopyMemory(pEntry, &g_dhcpv4_server_pool_option[index], sizeof(COSA_DML_DHCPSV4_OPTION));
            return ANSC_STATUS_SUCCESS;
        }
    }
    
    return ANSC_STATUS_FAILURE;
}


ANSC_STATUS
CosaDmlDhcpsSetOptionValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    if ( ulIndex+1 > sizeof(g_dhcpv4_server_pool_option)/sizeof(COSA_DML_DHCPSV4_OPTION) )
        return ANSC_STATUS_FAILURE;

    g_dhcpv4_server_pool_option[ulIndex].InstanceNumber  = ulInstanceNumber;
    AnscCopyString(g_dhcpv4_server_pool_option[ulIndex].Alias, pAlias);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpsAddOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        PCOSA_DML_DHCPSV4_OPTION  pEntry
    )
{
    return ANSC_STATUS_SUCCESS;

}

ANSC_STATUS
CosaDmlDhcpsDelOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulInstanceNumber
    )
{
    return ANSC_STATUS_SUCCESS;

}

ANSC_STATUS
CosaDmlDhcpsSetOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        PCOSA_DML_DHCPSV4_OPTION    pEntry
    )
{
    ULONG                          index = 0;

    for(index = 0; index <sizeof(g_dhcpv4_server_pool_option)/sizeof(COSA_DML_DHCPSV4_OPTION); index++)
    {
        if ( pEntry->InstanceNumber == g_dhcpv4_server_pool_option[index].InstanceNumber )
        {
            AnscCopyMemory( &g_dhcpv4_server_pool_option[index], pEntry, sizeof(COSA_DML_DHCPSV4_OPTION) );
            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_SUCCESS;    
}


void CosaDmlDhcpsGetPrevClientNumber(ULONG ulPoolInstanceNumber, ULONG *pNumber)
{
    *pNumber = 0;
}

/*
 *   This is for 
 *     Pool.{i}.Client.{i}.
 *
 */
ANSC_STATUS
CosaDmlDhcpsGetClient
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        PCOSA_DML_DHCPSV4_CLIENT   *ppClient,
        PCOSA_DML_DHCPSV4_CLIENTCONTENT *ppClientContent,
        PULONG                      pSize
    )
{
    PCOSA_DML_DHCPSV4_CLIENTCONTENT pcontent = NULL;

    *ppClient = (PCOSA_DML_DHCPSV4_CLIENT)AnscAllocateMemory( sizeof(COSA_DML_DHCPSV4_CLIENT) );
    AnscCopyMemory( *ppClient, &g_dhcpv4_server_client[0], sizeof(COSA_DML_DHCPSV4_CLIENT) );

    *pSize = sizeof(g_dhcpv4_server_client) / sizeof(COSA_DML_DHCPSV4_CLIENT);

    pcontent = (PCOSA_DML_DHCPSV4_CLIENTCONTENT)AnscAllocateMemory( sizeof(COSA_DML_DHCPSV4_CLIENTCONTENT) );
    pcontent->NumberofIPAddress = 1;
    pcontent->pIPAddress = (PCOSA_DML_DHCPSV4_CLIENT_IPADDRESS)AnscAllocateMemory( sizeof(COSA_DML_DHCPSV4_CLIENT_IPADDRESS) );
    AnscCopyMemory( pcontent->pIPAddress, &g_dhcpv4_server_client_ipaddress[0], sizeof(COSA_DML_DHCPSV4_CLIENT_IPADDRESS) );
    pcontent->NumberofOption = 1;
    pcontent->pOption = (PCOSA_DML_DHCPSV4_CLIENT_OPTION)AnscAllocateMemory( sizeof(COSA_DML_DHCPSV4_CLIENT_OPTION) );
    AnscCopyMemory( pcontent->pOption, &g_dhcpv4_server_client_option[0], sizeof(COSA_DML_DHCPSV4_CLIENT_OPTION) );

    *ppClientContent = pcontent;    

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpsPing
    (
        PCOSA_DML_DHCPSV4_CLIENT_IPADDRESS    pDhcpsClient
    )
{
    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlDhcpsARPing
    (
        PCOSA_DML_DHCPSV4_CLIENT_IPADDRESS    pDhcpsClient
    )
{
    return ANSC_STATUS_SUCCESS;
}

