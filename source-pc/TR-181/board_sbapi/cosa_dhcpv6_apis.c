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

    module: cosa_dhcpv6_apis.c

        For COSA Data Model Library Development

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
#include "cosa_dhcpv6_apis.h"
#include "cosa_dhcpv6_internal.h"
#include "plugin_main_apis.h"

/*
 *  This is copied from another board folder -- it should be in a header file!!!
 */
enum {
    DHCPV6_SERVER_TYPE_STATEFUL  =1,
    DHCPV6_SERVER_TYPE_STATELESS
};


COSA_DML_DHCPCV6_FULL  g_dhcpv6_client[] =
    {
        {
            {
                1,
                "Client1",
                1,
                5,
                "Layer3Interface1",
                "RequestedOption1,RequestedOption2",
                TRUE,
                TRUE,
                TRUE,
                TRUE,
                FALSE
            },
            {
                COSA_DML_DHCP_STATUS_Enabled,
                "SupportedOptions1,SupportedOptions2",
                "duid1,duid2"
            }
        }
    };


COSA_DML_DHCPCV6_SENT    g_dhcpv6_client_sent[] = 
    {
        {
            1,
            "SentOption1",
            TRUE,
            11,
            "Value1"
        },
        {
            2,
            "SentOption2",
            TRUE,
            12,
            "Value2"
        }
    };

COSA_DML_DHCPCV6_SVR g_dhcpv6_client_server[] =
    {
        {
            "2002::1",
            "Interface1",
            "2002:02:03",
        },
        {
            "2002::2",
            "Interface2",
            "2002:02:04",
        }
    };

COSA_DML_DHCPCV6_RECV g_dhcpv6_client_recv[] =
    {
        {
            23,
            "Server1",
            "Value1"
        },
        {
            24,
            "Server2",
            "Value2"
        }
    };

BOOL  g_dhcpv6_server = TRUE;

COSA_DML_DHCPSV6_POOL_FULL  g_dhcpv6_server_pool[]=
{
    {
        {
            1,
            "Pool1",
            3,
            "Layer3Interface",
            "vendorid",                       /*VendorClassID*/
            "usrerid",
            "2002:3:4",
            "ffff:ffff:ffff:0",
            "IANAManualPrefixes",  /*IANAManualPrefixes*/
            "IAPDManualPrefixes",  /*IAPDManualPrefixes*/
            "2002:3:4:100",
            "2002:3:4:150",
            "2002:3:4:100",
            1999,
            1999,
            "duid1",                        /*DUID*/
            TRUE,
            TRUE,                           /*DUIDExclude*/
            TRUE,
            TRUE,                           /*UserClassIDExclude*/
            TRUE,
            TRUE,  /*IANA enable*/
            TRUE
        },
        {
            COSA_DML_DHCP_STATUS_Enabled,
            "IANAPrefixes",  /*IANAPrefixes*/
            "IAPDPrefixes"   /*IAPDPrefixes*/
        }
    }
};

COSA_DML_DHCPSV6_CLIENT g_dhcpv6_server_client[] = 
{
    "client1",
    "2002::01",
    TRUE
};

COSA_DML_DHCPSV6_CLIENT_IPV6ADDRESS g_dhcpv6_server_client_ipv6address[] = 
{
   {
        "2002::01",
        "1000",
        "1200"
    }
};

COSA_DML_DHCPSV6_CLIENT_IPV6PREFIX g_dhcpv6_server_client_ipv6prefix[] = 
{
    {
        "2002::03",
        "1100",
        "1300"
    }
};

COSA_DML_DHCPSV6_CLIENT_OPTION g_dhcpv6_server_client_option[] = 
{
  
    {
        23,
        "Value1"
    }
};

COSA_DML_DHCPSV6_POOL_OPTION g_dhcpv6_server_pool_option[] =
{
    {
        1,
        "Option1",
        10023,
        "PassthroughClient",
        "Value1",
        TRUE
    }
};


ANSC_STATUS
CosaDmlDhcpv6Init
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
CosaDmlDhcpv6cGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    )
{
    PCOSA_DATAMODEL_DHCPV6          pDhcpv6           = (PCOSA_DATAMODEL_DHCPV6)g_pCosaBEManager->hDhcpv6;
    
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
CosaDmlDhcpv6cGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_DHCPCV6_FULL      pEntry
    )
{  
    
   if ( ulIndex >= sizeof(g_dhcpv6_client)/sizeof(COSA_DML_DHCPCV6_FULL) )
        return ANSC_STATUS_FAILURE;
    
    AnscCopyMemory(pEntry, &g_dhcpv6_client[ulIndex], sizeof(COSA_DML_DHCPCV6_FULL));
        
        return ANSC_STATUS_SUCCESS;
    }

ANSC_STATUS
CosaDmlDhcpv6cSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    if ( ulIndex >= sizeof(g_dhcpv6_client)/sizeof(COSA_DML_DHCPCV6_FULL) )
        return ANSC_STATUS_SUCCESS;

    g_dhcpv6_client[ulIndex].Cfg.InstanceNumber  = ulInstanceNumber;
    AnscCopyString(g_dhcpv6_client[ulIndex].Cfg.Alias, pAlias);
    
    return ANSC_STATUS_SUCCESS;
}

/*
    Description:
        The API adds DHCP client. 
    Arguments:
        pEntry        Caller fills in pEntry->Cfg, except Alias field. Upon return, callee fills pEntry->Cfg.Alias field and as many as possible fields in pEntry->Info.
*/
ANSC_STATUS
CosaDmlDhcpv6cAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPCV6_FULL        pEntry
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
CosaDmlDhcpv6cDelEntry
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
CosaDmlDhcpv6cSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPCV6_CFG       pCfg
    )
{
    ULONG                           index  = 0;

    for( index = 0 ; index < sizeof (g_dhcpv6_client)/sizeof(COSA_DML_DHCPCV6_FULL); index++)
    {
        if ( pCfg->InstanceNumber == g_dhcpv6_client[index].Cfg.InstanceNumber )
        {
            AnscCopyMemory(&g_dhcpv6_client[index].Cfg, pCfg, sizeof(COSA_DML_DHCPCV6_CFG));
            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpv6cGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPCV6_CFG       pCfg
    )
{
    ULONG                           index  = 0;
  
    for( index = 0 ; index < sizeof (g_dhcpv6_client)/sizeof(COSA_DML_DHCPCV6_FULL); index++)
    {
        if ( pCfg->InstanceNumber == g_dhcpv6_client[index].Cfg.InstanceNumber )
        {
            AnscCopyMemory(pCfg,  &g_dhcpv6_client[index].Cfg, sizeof(COSA_DML_DHCPCV6_CFG));
            return ANSC_STATUS_SUCCESS;
        }
    }
      
    return ANSC_STATUS_FAILURE;
}

BOOL
CosaDmlDhcpv6cGetEnabled
    (
        ANSC_HANDLE                 hContext
    )
{
    BOOL bEnabled = FALSE;
    char out[256] = {0};

    /** Needs rewrite for PC build */
    /*
    FILE *fp = popen("ps |grep -i ti_dhcp6c | grep erouter0 | grep -v grep", "r");
    if ( fp != NULL){
        if ( fgets(out, sizeof(out), fp) != NULL )
            if ( _ansc_strstr(out, "erouter_dhcp6c") )
                bEnabled = TRUE;
          
        pclose(fp);
    }
    */

    return bEnabled;
}

ANSC_STATUS
CosaDmlDhcpv6cGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_DHCPCV6_INFO      pInfo
    )
{
    ULONG                           index  = 0;
   
    for( index = 0 ; index < sizeof (g_dhcpv6_client)/sizeof(COSA_DML_DHCPCV6_FULL); index++)
    {
        if ( ulInstanceNumber == g_dhcpv6_client[index].Cfg.InstanceNumber )
        {
            AnscCopyMemory(pInfo,  &g_dhcpv6_client[index].Info, sizeof(COSA_DML_DHCPCV6_INFO));
            return ANSC_STATUS_SUCCESS;
        }
    }
    
    return ANSC_STATUS_FAILURE;
}

/* this memory need to be freed by caller */
ANSC_STATUS
CosaDmlDhcpv6cGetServerCfg
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCPCV6_SVR      *ppCfg,
        PULONG                      pSize
    )
{
    *ppCfg = (PCOSA_DML_DHCPCV6_SVR)AnscAllocateMemory( sizeof(g_dhcpv6_client_server ) );
    AnscCopyMemory( *ppCfg, &g_dhcpv6_client_server[0], sizeof(g_dhcpv6_client_server) );
    *pSize = sizeof(g_dhcpv6_client_server) / sizeof(COSA_DML_DHCPCV6_SVR);
        
    return ANSC_STATUS_SUCCESS;
}

/*
    Description:
        The API initiates a DHCP client renewal. 
    Arguments:
        pAlias        The entry is identified through Alias.
*/
ANSC_STATUS
CosaDmlDhcpv6cRenew
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{  
    CcspTraceInfo((" CosaDmlDhcpv6cRenew -- ulInstanceNumber:%d.\n", ulInstanceNumber)); 

    return ANSC_STATUS_SUCCESS;
}

/*
 *  DHCP Client Send/Req Option
 *
 *  The options are managed on top of a DHCP client,
 *  which is identified through pClientAlias
 */
ULONG
CosaDmlDhcpv6cGetNumberOfSentOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber
    )
{
    return sizeof(g_dhcpv6_client_sent)/sizeof(COSA_DML_DHCPCV6_SENT);
}

ANSC_STATUS
CosaDmlDhcpv6cGetSentOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_DHCPCV6_SENT      pEntry
    )
{
    PCOSA_DML_DHCPCV6_SENT          pDhcpSendOption      = pEntry;

    if ( ulIndex+1 > sizeof(g_dhcpv6_client_sent)/sizeof(COSA_DML_DHCPCV6_SENT) )
        return ANSC_STATUS_FAILURE;

    AnscCopyMemory( pEntry, &g_dhcpv6_client_sent[ulIndex], sizeof(COSA_DML_DHCPCV6_SENT));

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlDhcpv6cGetSentOptionbyInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCPCV6_SENT      pEntry
    )
{   
    ULONG                           index  = 0;

    for( index=0;  index<sizeof(g_dhcpv6_client_sent)/sizeof(COSA_DML_DHCPCV6_SENT); index++)
    {
        if ( pEntry->InstanceNumber == g_dhcpv6_client_sent[index].InstanceNumber )
        {
            AnscCopyMemory( pEntry, &g_dhcpv6_client_sent[index], sizeof(COSA_DML_DHCPCV6_SENT));
             return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_FAILURE;
}


ANSC_STATUS
CosaDmlDhcpv6cSetSentOptionValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    if ( ulIndex+1 > sizeof(g_dhcpv6_client_sent)/sizeof(COSA_DML_DHCPCV6_SENT) )
        return ANSC_STATUS_SUCCESS;

    g_dhcpv6_client_sent[ulIndex].InstanceNumber  = ulInstanceNumber;
    AnscCopyString( g_dhcpv6_client_sent[ulIndex].Alias, pAlias);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpv6cAddSentOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCPCV6_SENT      pEntry
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpv6cDelSentOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        ULONG                       ulInstanceNumber
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpv6cSetSentOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCPCV6_SENT      pEntry
    )
{
    ULONG                           index  = 0;

    for( index=0;  index<sizeof(g_dhcpv6_client_sent)/sizeof(COSA_DML_DHCPCV6_SENT); index++)
    {
        if ( pEntry->InstanceNumber == g_dhcpv6_client_sent[index].InstanceNumber )
        {
            AnscCopyMemory( &g_dhcpv6_client_sent[index], pEntry, sizeof(COSA_DML_DHCPCV6_SENT));
            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_SUCCESS;
}

/* This is to get all 
 */
ANSC_STATUS
CosaDmlDhcpv6cGetReceivedOptionCfg
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulClientInstanceNumber,
        PCOSA_DML_DHCPCV6_RECV     *ppEntry,
        PULONG                      pSize
    )
{   
    *ppEntry = (PCOSA_DML_DHCPCV6_RECV)AnscAllocateMemory( sizeof(g_dhcpv6_client_recv) );
    AnscCopyMemory( *ppEntry, &g_dhcpv6_client_recv[0], sizeof(g_dhcpv6_client_recv) );
    *pSize = sizeof(g_dhcpv6_client_recv) / sizeof(COSA_DML_DHCPCV6_RECV);
        
    return ANSC_STATUS_SUCCESS;
}

/*
 *  DHCP Server
 */
ANSC_STATUS
CosaDmlDhcpv6sEnable
    (
        ANSC_HANDLE                 hContext,
        BOOL                        bEnable
    )
{
    g_dhcpv6_server = bEnable;
    
    return ANSC_STATUS_SUCCESS;
}

/*
    Description:
        The API retrieves the current state of DHCP server: Enabled or Disabled. 
*/
BOOLEAN
CosaDmlDhcpv6sGetState
    (
        ANSC_HANDLE                 hContext
    )
{
    return g_dhcpv6_server;
}

/*
    Description:
        The API retrieves the current type of DHCP server: STATELESS or STATEFUL. 
*/
ANSC_STATUS
CosaDmlDhcpv6sSetType
    (
        ANSC_HANDLE                 hContext,
        ULONG                       type        
    )
{
    return ANSC_STATUS_SUCCESS;
}


ULONG
CosaDmlDhcpv6sGetType
    (
        ANSC_HANDLE                 hContext
    )
{
    return DHCPV6_SERVER_TYPE_STATELESS;
}


/*
 *  DHCP Server Pool
 */
ULONG
CosaDmlDhcpv6sGetNumberOfPools
    (
        ANSC_HANDLE                 hContext
    )
{
    return sizeof(g_dhcpv6_server_pool)/sizeof(COSA_DML_DHCPSV6_POOL_FULL);
}

ANSC_STATUS
CosaDmlDhcpv6sGetPool
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_DHCPSV6_POOL_FULL pEntry
    )
{
    if ( ulIndex+1 > sizeof(g_dhcpv6_server_pool)/sizeof(COSA_DML_DHCPSV6_POOL_FULL) )
        return ANSC_STATUS_FAILURE;

    AnscCopyMemory(pEntry, &g_dhcpv6_server_pool[ulIndex], sizeof(COSA_DML_DHCPSV6_POOL_FULL));
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpv6sSetPoolValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    if ( ulIndex+1 > sizeof(g_dhcpv6_server_pool)/sizeof(COSA_DML_DHCPSV6_POOL_FULL) )
        return ANSC_STATUS_FAILURE;

    g_dhcpv6_server_pool[ulIndex].Cfg.InstanceNumber  = ulInstanceNumber;
    AnscCopyString(g_dhcpv6_server_pool[ulIndex].Cfg.Alias, pAlias);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpv6sAddPool
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPSV6_POOL_FULL   pEntry
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpv6sDelPool
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpv6sSetPoolCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPSV6_POOL_CFG  pCfg
    )
{
    ULONG                          index = 0;

    for(index = 0; index <sizeof(g_dhcpv6_server_pool)/sizeof(COSA_DML_DHCPSV6_POOL_FULL); index++)
    {
        if ( pCfg->InstanceNumber == g_dhcpv6_server_pool[index].Cfg.InstanceNumber )
        {
            AnscCopyMemory( &g_dhcpv6_server_pool[index].Cfg, pCfg, sizeof(COSA_DML_DHCPSV6_POOL_FULL) );
            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpv6sGetPoolCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DHCPSV6_POOL_CFG    pCfg
    )
{
    ULONG                          index = 0;

    for(index = 0; index <sizeof(g_dhcpv6_server_pool)/sizeof(COSA_DML_DHCPSV6_POOL_FULL); index++)
    {
        if ( pCfg->InstanceNumber == g_dhcpv6_server_pool[index].Cfg.InstanceNumber )
        {
            AnscCopyMemory( pCfg, &g_dhcpv6_server_pool[index].Cfg, sizeof(COSA_DML_DHCPSV6_POOL_FULL) );
            return ANSC_STATUS_FAILURE;
        }
    }

    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDmlDhcpv6sGetPoolInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_DHCPSV6_POOL_INFO pInfo
    )
{
    ULONG                          index = 0;

    for(index = 0; index <sizeof(g_dhcpv6_server_pool)/sizeof(COSA_DML_DHCPSV6_POOL_FULL); index++)
    {
        if ( ulInstanceNumber == g_dhcpv6_server_pool[index].Cfg.InstanceNumber )
        {
            AnscCopyMemory( pInfo, &g_dhcpv6_server_pool[index].Info, sizeof(COSA_DML_DHCPSV6_POOL_INFO) );
            return ANSC_STATUS_FAILURE;
        }
    }

    return ANSC_STATUS_FAILURE;
}


ANSC_STATUS
CosaDmlDhcpv6sPing
    (
        PCOSA_DML_DHCPSV6_CLIENT    pDhcpsClient
    )
{
    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlDhcpv6sGetStartAddress
    (
        ANSC_HANDLE                 hContext,
        char*                       addr,
        int                         len
    )
{
    /* DH  TBD  fill in simulation data */
    return ANSC_STATUS_SUCCESS;
}



/*
 *   This is for 
 *     Pool.{i}.Client.{i}.
 *
 */
ANSC_STATUS
CosaDmlDhcpv6sGetClient
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        PCOSA_DML_DHCPSV6_CLIENT   *ppEntry,
        PULONG                      pSize
    )
{        
    *ppEntry = (PCOSA_DML_DHCPSV6_CLIENT)AnscAllocateMemory( sizeof(g_dhcpv6_server_client) );
    AnscCopyMemory( *ppEntry, &g_dhcpv6_server_client[0], sizeof(g_dhcpv6_server_client) );
    *pSize = sizeof(g_dhcpv6_server_client) / sizeof(COSA_DML_DHCPSV6_CLIENT);
        
    return ANSC_STATUS_SUCCESS;
}

/*
  *   This is for 
        Pool.{i}.Client.{i}.IPv6Address.{i}.
  *
  */
ANSC_STATUS
CosaDmlDhcpv6sGetIPv6Address
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulClientIndex,
        PCOSA_DML_DHCPSV6_CLIENT_IPV6ADDRESS   *ppEntry,
        PULONG                      pSize
    )
{        
    *ppEntry = (PCOSA_DML_DHCPSV6_CLIENT_IPV6ADDRESS)AnscAllocateMemory( sizeof(g_dhcpv6_server_client_ipv6address) );
    AnscCopyMemory( *ppEntry, &g_dhcpv6_server_client_ipv6address[0], sizeof(g_dhcpv6_server_client_ipv6address) );
    *pSize = sizeof(g_dhcpv6_server_client_ipv6address) / sizeof(COSA_DML_DHCPSV6_CLIENT_IPV6ADDRESS);
        
    return ANSC_STATUS_SUCCESS;
}

/*
  *   This is for 
        Pool.{i}.Client.{i}.IPv6Prefix.{i}.
  *
  */
ANSC_STATUS
CosaDmlDhcpv6sGetIPv6Prefix
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulClientIndex,
        PCOSA_DML_DHCPSV6_CLIENT_IPV6PREFIX   *ppEntry,
        PULONG                      pSize
    )
{        
    *ppEntry = (PCOSA_DML_DHCPSV6_CLIENT_IPV6PREFIX)AnscAllocateMemory( sizeof(g_dhcpv6_server_client_ipv6prefix) );
    AnscCopyMemory( *ppEntry, &g_dhcpv6_server_client_ipv6prefix[0], sizeof(g_dhcpv6_server_client_ipv6prefix) );
    *pSize = sizeof(g_dhcpv6_server_client_ipv6prefix) / sizeof(COSA_DML_DHCPSV6_CLIENT_IPV6PREFIX);
        
    return ANSC_STATUS_SUCCESS;
}

/*
  *   This is for 
        Pool.{i}.Client.{i}.IPv6Option.{i}.
  *
  */
ANSC_STATUS
CosaDmlDhcpv6sGetIPv6Option
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulClientIndex,
        PCOSA_DML_DHCPSV6_CLIENT_OPTION   *ppEntry,
        PULONG                      pSize
    )
{        
    *ppEntry = (PCOSA_DML_DHCPSV6_CLIENT_OPTION)AnscAllocateMemory( sizeof(g_dhcpv6_server_client_option) );
    AnscCopyMemory( *ppEntry, &g_dhcpv6_server_client_option[0], sizeof(g_dhcpv6_server_client_option) );
    *pSize = sizeof(g_dhcpv6_server_client_option) / sizeof(COSA_DML_DHCPSV6_CLIENT_OPTION);

    return ANSC_STATUS_SUCCESS;
}

/*
 *  DHCP Server Pool Option
 *
 *  The options are managed on top of a DHCP server pool,
 *  which is identified through pPoolAlias
 */
ULONG
CosaDmlDhcpv6sGetNumberOfOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber
    )
{
    return sizeof(g_dhcpv6_server_pool_option) / sizeof(COSA_DML_DHCPSV6_POOL_OPTION);
}

ANSC_STATUS
CosaDmlDhcpv6sGetOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_DHCPSV6_POOL_OPTION  pEntry
    )
{
    if ( ulIndex+1 > sizeof(g_dhcpv6_server_pool_option)/sizeof(COSA_DML_DHCPSV6_POOL_OPTION) )
        return ANSC_STATUS_FAILURE;

    AnscCopyMemory(pEntry, &g_dhcpv6_server_pool_option[ulIndex], sizeof(COSA_DML_DHCPSV6_POOL_OPTION));
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpv6sGetOptionbyInsNum
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        PCOSA_DML_DHCPSV6_POOL_OPTION  pEntry
    )
{
    ULONG                           index = 0;

    for(index = 0; index < sizeof(g_dhcpv6_server_pool_option[index])/sizeof(COSA_DML_DHCPSV6_POOL_OPTION); index++)
    {
        if ( pEntry->InstanceNumber == g_dhcpv6_server_pool_option[index].InstanceNumber )
        {
            AnscCopyMemory(pEntry, &g_dhcpv6_server_pool_option[index], sizeof(COSA_DML_DHCPSV6_POOL_OPTION));
            return ANSC_STATUS_SUCCESS;
        }
    }
    
    return ANSC_STATUS_FAILURE;
}


ANSC_STATUS
CosaDmlDhcpv6sSetOptionValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    if ( ulIndex+1 > sizeof(g_dhcpv6_server_pool_option)/sizeof(COSA_DML_DHCPSV6_POOL_OPTION) )
        return ANSC_STATUS_FAILURE;

    g_dhcpv6_server_pool_option[ulIndex].InstanceNumber  = ulInstanceNumber;
    AnscCopyString(g_dhcpv6_server_pool_option[ulIndex].Alias, pAlias);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDhcpv6sAddOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        PCOSA_DML_DHCPSV6_POOL_OPTION  pEntry
    )
{
    return ANSC_STATUS_SUCCESS;

}

ANSC_STATUS
CosaDmlDhcpv6sDelOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        ULONG                       ulInstanceNumber
    )
{
    return ANSC_STATUS_SUCCESS;

}

ANSC_STATUS
CosaDmlDhcpv6sSetOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPoolInstanceNumber,
        PCOSA_DML_DHCPSV6_POOL_OPTION  pEntry
    )
{
    ULONG                          index = 0;

    for(index = 0; index <sizeof(g_dhcpv6_server_pool_option)/sizeof(COSA_DML_DHCPSV6_POOL_OPTION); index++)
    {
        if ( pEntry->InstanceNumber == g_dhcpv6_server_pool_option[index].InstanceNumber )
        {
            AnscCopyMemory( &g_dhcpv6_server_pool_option[index], pEntry, sizeof(COSA_DML_DHCPSV6_POOL_OPTION) );
            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_SUCCESS;    
}

void
CosaDmlDhcpv6Remove(ANSC_HANDLE hContext)
{
    return;
}

int  
CosaDmlStartDHCP6Client()
{
   return 0;
}

BOOL tagPermitted(int tag)
{
    return TRUE;
}

