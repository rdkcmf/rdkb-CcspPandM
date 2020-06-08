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

/************************************************************************* 
    module: cosa_dns_apis.c

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

**************************************************************************/

#include "cosa_dns_internal.h"


#if (defined(_COSA_SIM_))

COSA_DML_DNS_CLIENT_SERVER  g_DnsServerFull[] =
{
    {1, "Server1", TRUE, COSA_DML_DNS_STATUS_Enabled,
        "\x40\x40\x40\x08", "eth0", COSA_DML_DNS_ADDR_SRC_DHCPV4},
    {2, "Server2", TRUE, COSA_DML_DNS_STATUS_Enabled,
        "\x40\x40\x40\x08", "eth2", COSA_DML_DNS_ADDR_SRC_DHCPV4}
};

COSA_DML_DNS_RELAY_ENTRY  g_DnsRelayFull[10] =
{
    {1, "Forwarding1", TRUE, COSA_DML_DNS_STATUS_Enabled, 
        "\x40\x40\x40\x08", "wan8", COSA_DML_DNS_ADDR_SRC_DHCP}
};

ULONG g_NumOfDnsServers = 2;//RDKB-EMU

ULONG g_NumOfDnsRelays = 1;

BOOLEAN g_ClientEnable = TRUE; 

BOOLEAN g_ReplyEnable = TRUE;

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsInit
            (
                ANSC_HANDLE                 hDml,
                PANSC_HANDLE                phContext
            )


    description:

       This function enables the DNS client.

    argument:  
            ANSC_HANDLE                  hDml,

            PANSC_HANDLE                phContext

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaDmlDnsInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{

    return ANSC_STATUS_SUCCESS;
}

/*
 *  DNS Client
 */
/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsEnableClient
            (
                ANSC_HANDLE                 hContext,
                BOOLEAN                     bEnabled
            )

    description:

       This function enables the DNS client.

    argument:   ANSC_HANDLE                 hContext,
                BOOLEAN                     bEnabled

    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsEnableClient
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bEnabled
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;

    g_ClientEnable = bEnabled;
    
    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlIpDnsGetClientStatus
            (
                ANSC_HANDLE                 hContext
            );

    description:

        This function gets the status of the DNS client.

    argument:   ANSC_HANDLE                 hContext
            

    return:     operation status.

**********************************************************************/
COSA_DML_DNS_STATUS
CosaDmlIpDnsGetClientStatus
    (
        ANSC_HANDLE                 hContext
    )
{
    COSA_DML_DNS_STATUS             DnsStatus = COSA_DML_DNS_STATUS_Disabled;

    if ( g_ClientEnable )
    {
          DnsStatus = COSA_DML_DNS_STATUS_Enabled;
    }
    
    return DnsStatus;
}
/*
 *  DNS Client Server
 */
/**********************************************************************

    caller:     self

    prototype:

        PCOSA_DML_DNS_CLIENT_SERVER
        CosaDmlDnsClientGetServers
            (
                ANSC_HANDLE                 hContext,
                PULONG                      pulCount
            );

    description:

        This routine is to retrieve the complete list of DNS servers, which is a dynamic table.

    argument:   ANSC_HANDLE                 hContext,
                PULONG                      pulCount
                To receive the actual number of entries.

    return:     The pointer to the array of DNS servers, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/
PCOSA_DML_DNS_CLIENT_SERVER
CosaDmlDnsClientGetServers
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount
    )
{
    PCOSA_DML_DNS_CLIENT_SERVER     pDnsServer = (PCOSA_DML_DNS_CLIENT_SERVER)NULL;
    ULONG                           ulSize     = 0;

    ulSize = sizeof(COSA_DML_DNS_CLIENT_SERVER) * g_NumOfDnsServers;
    
    pDnsServer = AnscAllocateMemory(ulSize);

    if ( !pDnsServer )
    {
        *pulCount = 0;
    } else
    {
        *pulCount = g_NumOfDnsServers;
        AnscCopyMemory(pDnsServer, g_DnsServerFull, ulSize);
    }
    
    return pDnsServer;
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsClientSetServerValues
            (
                ANSC_HANDLE             hContext,
                ULONG                       ulIndex,
                ULONG                       ulInstanceNumber,
                char*                        pAlias
            )


    description:

        The API sets DNS server values. 

    argument:   
            ANSC_HANDLE             hContext,

            ULONG                       ulIndex,

            ULONG                       ulInstanceNumber,

            char*                        pAlias
    
    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsClientSetServerValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;

    g_DnsServerFull[ulIndex].InstanceNumber = ulInstanceNumber;
    AnscCopyString(g_DnsServerFull[ulIndex].Alias, pAlias);
        
    return returnStatus;
}



/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsClientAddServer
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_DNS_CLIENT_SERVER pEntry
            );

    description:

        The API adds one DNS server entry into DNS client. 

    argument:   ANSC_HANDLE                 hContext,
                PCOSA_DML_DNS_CLIENT_SERVER pEntry
                Caller does not need to fill in Status or Alias fields. Field Type has to be COSA_DML_DNS_ADDR_SRC_Static. Caller does not have to fill in field Interface. Upon return, callee fills in the generated Alias.
    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsClientAddServer
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DNS_CLIENT_SERVER pEntry
    )
{
    if ( g_NumOfDnsServers >= 10 )
    {
        return ANSC_STATUS_FAILURE;
    }

    g_DnsServerFull[g_NumOfDnsServers].InstanceNumber  = pEntry->InstanceNumber;  
    g_DnsServerFull[g_NumOfDnsServers].bEnabled        = pEntry->bEnabled;        
    g_DnsServerFull[g_NumOfDnsServers].Status          = pEntry->Status;        
    g_DnsServerFull[g_NumOfDnsServers].Type            = pEntry->Type;      
    AnscCopyString(g_DnsServerFull[g_NumOfDnsServers].DNSServer, pEntry->DNSServer);             
    AnscCopyString(g_DnsServerFull[g_NumOfDnsServers].Alias, pEntry->Alias);
    AnscCopyString(g_DnsServerFull[g_NumOfDnsServers].Interface, pEntry->Interface);  

    g_NumOfDnsServers++;

    return ANSC_STATUS_SUCCESS;
}



/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsClientDelServer
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulInstanceNumber
            );

    description:

        The API delete one DNS server entry from DNS client.

    argument:   ANSC_HANDLE                 hContext,
                ULONG                       ulInstanceNumber

    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsClientDelServer
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
    ULONG                           i = 0;
    ULONG                           j = 0;

     for ( i = 0; i < g_NumOfDnsServers; i++ )
    {
        if ( ulInstanceNumber == g_DnsServerFull[i].InstanceNumber )
        {
            for ( j = i; j < g_NumOfDnsServers; j++ )
            {
                AnscCopyMemory
                (
                    &g_DnsServerFull[j], 
                    &g_DnsServerFull[j+1],
                    sizeof(COSA_DML_DNS_CLIENT_SERVER)
                );
            }

            g_NumOfDnsServers--;

            return ANSC_STATUS_SUCCESS;
        }
    }
             
    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsClientSetServer
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_DNS_CLIENT_SERVER pEntry
            );

    description:

        This function sets the server of the DNS client.

    argument:   ANSC_HANDLE                 hContext,
                PCOSA_DML_DNS_CLIENT_SERVER pEntry

    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsClientSetServer
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DNS_CLIENT_SERVER pEntry
    )
{
    ULONG                           i = 0;
    
    for ( i = 0; i < g_NumOfDnsServers; i++)
    {
        if ( g_DnsServerFull[i].InstanceNumber == pEntry->InstanceNumber )
        {
            g_DnsServerFull[i].InstanceNumber  = pEntry->InstanceNumber;  
            g_DnsServerFull[i].bEnabled        = pEntry->bEnabled;        
            g_DnsServerFull[i].Status          = pEntry->Status;        
            g_DnsServerFull[i].Type            = pEntry->Type;      
            AnscCopyString(g_DnsServerFull[i].DNSServer, pEntry->DNSServer);             
            AnscCopyString(g_DnsServerFull[i].Alias, pEntry->Alias);
            AnscCopyString(g_DnsServerFull[i].Interface, pEntry->Interface);  
 
            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_CANT_FIND;   
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsClientGetServer
            (
                ANSC_HANDLE                                 hContext,
                PCOSA_DML_DNS_CLIENT_SERVER      pEntry
             )


    description:

        This routine is to retrieve the config of DNS servers, which is a dynamic table.

    argument:  
                    ANSC_HANDLE                 hContext,

                    PCOSA_DML_DNS_CLIENT_SERVER pEntry

    return:    operation status.  

**********************************************************************/

ANSC_STATUS
CosaDmlDnsClientGetServer
    (
        ANSC_HANDLE hContext,
        PCOSA_DML_DNS_CLIENT_SERVER pEntry
     )

{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    

    return returnStatus;
}



/*
 *  DNS Relay
 */
 
/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsEnableRelay
            (
                ANSC_HANDLE                 hContext,
                BOOLEAN                     bEnabled
            )

    description:

        This function enables the DNS relay.

    argument:   ANSC_HANDLE                 hContext,
                BOOLEAN                     bEnabled

    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsEnableRelay
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bEnabled
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    
    g_ReplyEnable = bEnabled;

    return returnStatus;
}


/**********************************************************************

    caller:     self

    prototype:

        COSA_DML_DNS_STATUS
        CosaDmlIpDnsGetRelayStatus
            (
                ANSC_HANDLE                 hContext
            )

    description:

        This function initiate  cosa nat object and return handle.

    argument:   ANSC_HANDLE                 hContext
          

    return:     operation status.

**********************************************************************/
COSA_DML_DNS_STATUS
CosaDmlIpDnsGetRelayStatus
    (
        ANSC_HANDLE                 hContext
    )
{
    COSA_DML_DNS_STATUS             RelayStatus = COSA_DML_DNS_STATUS_Disabled;

    if ( g_ReplyEnable )
    {
        RelayStatus = COSA_DML_DNS_STATUS_Enabled;
    }
    
    return RelayStatus;
}

/*
 *  DNS Relay Forwarding (server)
 */
/**********************************************************************

    caller:     self

    prototype:

        PCOSA_DML_DNS_RELAY_ENTRY
        CosaDmlDnsRelayGetServers
            (
                ANSC_HANDLE                 hContext,
                PULONG                      pulCount
            );

    description:

        This function Get the DNS relay server.

    argument:   ANSC_HANDLE                 hContext,
                PULONG                      pulCount
                To receive the actual number of entries.

    return:     The pointer to the array of DNS servers, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/
PCOSA_DML_DNS_RELAY_ENTRY
CosaDmlDnsRelayGetServers
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount
    )
{
    PCOSA_DML_DNS_RELAY_ENTRY       pForward = (PCOSA_DML_DNS_RELAY_ENTRY)NULL;
    ULONG                           ulSize     = 0;

    ulSize = sizeof(COSA_DML_DNS_RELAY_ENTRY) * g_NumOfDnsRelays;

    pForward = AnscAllocateMemory(ulSize);

    if ( !pForward )
    {
        *pulCount = 0;
    } else
    {
        *pulCount = g_NumOfDnsRelays;
        AnscCopyMemory(pForward, g_DnsRelayFull, ulSize);
    }
    
    return pForward;
}



/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsRelaySetServerValues
            (
                ANSC_HANDLE                 hContext,
                ULONG                           ulIndex,
                ULONG                           ulInstanceNumber,
                char*                            pAlias
            )


    description:

        This function Get the DNS relay server.

    argument:  
                 ANSC_HANDLE                 hContext,

                 ULONG                           ulIndex,

                 ULONG                           ulInstanceNumber,

                 char*                            pAlias

    return:     The pointer to the array of DNS servers, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsRelaySetServerValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;

    g_DnsRelayFull[ulIndex].InstanceNumber = ulInstanceNumber;
    AnscCopyString(g_DnsRelayFull[ulIndex].Alias, pAlias);
        
    return returnStatus;
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsRelayAddServer
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_DNS_RELAY_ENTRY   pEntry
            );

    description:

        The API adds one DNS server entry into DNS relay. 

    argument:   ANSC_HANDLE                 hContext,
    
                PCOSA_DML_DNS_RELAY_ENTRY   pEntry
                Caller does not need to fill in Status or Alias fields. Field Type has to be COSA_DML_DNS_ADDR_SRC_Static. Caller does not have to fill in field Interface. Upon return, callee fills in the generated Alias.

    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsRelayAddServer
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DNS_RELAY_ENTRY   pEntry
    )
{
    if ( g_NumOfDnsRelays >= 10 )
    {
        return ANSC_STATUS_FAILURE;
    }

    g_DnsRelayFull[g_NumOfDnsRelays].InstanceNumber  = pEntry->InstanceNumber;  
    g_DnsRelayFull[g_NumOfDnsRelays].bEnabled        = pEntry->bEnabled;        
    g_DnsRelayFull[g_NumOfDnsRelays].Status          = pEntry->Status;        
    g_DnsRelayFull[g_NumOfDnsRelays].Type            = pEntry->Type;      
    g_DnsRelayFull[g_NumOfDnsRelays].DNSServer.Value = pEntry->DNSServer.Value;             

    AnscCopyString(g_DnsRelayFull[g_NumOfDnsRelays].Alias, pEntry->Alias);
    AnscCopyString(g_DnsRelayFull[g_NumOfDnsRelays].Interface, pEntry->Interface);  

    g_NumOfDnsRelays++;

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsRelayDelServer
        (
            ANSC_HANDLE                 hContext,
            ULONG                       ulInstanceNumber
        );

    description:

        The API delete one DNS server entry from DNS client. 

    argument:   ANSC_HANDLE                 hContext,
    
                ULONG                       ulInstanceNumber

    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsRelayDelServer
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
    ULONG                           i = 0;
    ULONG                           j = 0;

    for ( i = 0; i < g_NumOfDnsRelays; i++ )
    {
        if ( ulInstanceNumber == g_DnsRelayFull[i].InstanceNumber )
        {
            for ( j = i; j < g_NumOfDnsRelays; j++ )
            {
                AnscCopyMemory
                (
                    &g_DnsRelayFull[j], 
                    &g_DnsRelayFull[j+1],
                    sizeof(COSA_DML_DNS_RELAY_ENTRY)
                );
            }

            g_NumOfDnsRelays--;

            return ANSC_STATUS_SUCCESS;
        }
    }
             
    return ANSC_STATUS_CANT_FIND;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsRelaySetServer
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_DNS_RELAY_ENTRY   pEntry
            );

    description:

        This function set the relay server.

    argument:   ANSC_HANDLE                 hContext,
    
                PCOSA_DML_DNS_RELAY_ENTRY   pEntry

    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsRelaySetServer
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DNS_RELAY_ENTRY   pEntry
    )
{
    ULONG                           i = 0;

    for ( i = 0; i < g_NumOfDnsRelays; i++)
    {
        if ( g_DnsRelayFull[i].InstanceNumber == pEntry->InstanceNumber )
        {
            g_DnsRelayFull[i].InstanceNumber  = pEntry->InstanceNumber;  
            g_DnsRelayFull[i].bEnabled        = pEntry->bEnabled;        
            g_DnsRelayFull[i].Status          = pEntry->Status;        
            g_DnsRelayFull[i].Type            = pEntry->Type;      
            g_DnsRelayFull[i].DNSServer.Value = pEntry->DNSServer.Value;             

            AnscCopyString(g_DnsRelayFull[i].Alias, pEntry->Alias);
            AnscCopyString(g_DnsRelayFull[i].Interface, pEntry->Interface);  
 
            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_CANT_FIND;   
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsRelayGetServer
            (
                ANSC_HANDLE                           hContext,
                PCOSA_DML_DNS_RELAY_ENTRY   pEntry     
            )



    description:

        This routine is to retrieve the config of DNS servers, which is a dynamic table.

    argument:  
                    ANSC_HANDLE                          hContext,

                    PCOSA_DML_DNS_RELAY_ENTRY  pEntry

    return:    operation status.  

**********************************************************************/

ANSC_STATUS
CosaDmlDnsRelayGetServer
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DNS_RELAY_ENTRY   pEntry     
    )


{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    
    return returnStatus;
}


#elif ( defined(_COSA_DRG_CNS_) || defined(_COSA_DRG_TPG_))

#include <utctx_api.h>
#include <utapi.h>
#include <utapi_util.h>
#include <ulog.h>
#define COSA_DNS_SYSCFG_NAMESPACE NULL

BOOLEAN bReplyEnable = TRUE;
BOOLEAN bClientEnable = TRUE;

#define  COSA_DML_DNS_MAX_CLIENT_SERVER                 8
#define  COSA_DML_STATIC_DNS_MAX_CLIENT_SERVER          3
#define  COSA_DML_DNS_MAX_RELAY_SERVER                  10

ULONG    g_DnsClientServerNum                       =   0;
ULONG    g_StaticDnsClientServerNum                 =   0;
ULONG    g_CountDnsRelays                           =   0;
COSA_DML_DNS_CLIENT_SERVER   g_dns_client_server[COSA_DML_DNS_MAX_CLIENT_SERVER];
COSA_DML_DNS_RELAY_ENTRY     g_dns_relay_forwarding[COSA_DML_DNS_MAX_RELAY_SERVER];
wanInfo_t       g_wan_info;


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsInit
            (
                ANSC_HANDLE                 hDml,
                PANSC_HANDLE                phContext
            )


    description:

       This function enables the DNS client.

    argument:  
            ANSC_HANDLE                  hDml,

            PANSC_HANDLE                phContext

    return:     operation status.

**********************************************************************/


ANSC_STATUS
CosaDmlDnsInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    int i = 0;
    int j = 0;
    PCOSA_DML_DNS_CLIENT_SERVER pDnsClientServer = NULL;
    PCOSA_DML_DNS_RELAY_ENTRY   pForward         = (PCOSA_DML_DNS_RELAY_ENTRY)NULL;
    CHAR                sBuf[8];
    CHAR                tmpBuff[64]    = {0};
    CHAR                sAlias[64];
    UtopiaContext       ctx;
    ULONG               ulIndex = 0;
    CHAR                str_val[64] = {0};
    CHAR                str_key[64] = {0};
    CHAR                dns[3][32];
    CHAR                line[128];
    char                *pToken;
    FILE                *fp;
    wanConnectionInfo_t wanInfo;
    char                sDns[3][IPHOSTNAME_SZ];
    CHAR                buf[64] = {'\0'};
    CHAR                count[64] = {'\0'};
    INT                 rc = -1;
    CHAR                key_buf[64] = {'\0'};
    char                *st = NULL;

    ulogf(ULOG_SYSTEM, UL_DHCP, "%s:g_DnsClientServerNum %d\n", __FUNCTION__, g_DnsClientServerNum);
    CcspTraceInfo(("CosaDmlDnsInit\n"));
    
    if (!Utopia_Init(&ctx))
    {
        return ANSC_STATUS_FAILURE;
    }
    g_DnsClientServerNum = 0;
    g_StaticDnsClientServerNum = 0;
    memset (g_dns_client_server, 0, sizeof(COSA_DML_DNS_CLIENT_SERVER) * COSA_DML_DNS_MAX_CLIENT_SERVER );
    memset (&g_wan_info, 0, sizeof(wanInfo_t));
    
    fp = fopen("/etc/resolv.conf", "r");
    
    if (!fp) 
    {
        ulogf(ULOG_SYSTEM, UL_DHCP, "%s: failed to open udhcpc log file\n", __FUNCTION__); 
	Utopia_Free(&ctx, 0);
        return ANSC_STATUS_FAILURE;
    }
    CcspTraceInfo(("CosaDmlDnsInit open fp OK\n"));

    //strcpy(sBuf, "true");
    
    //Utopia_RawSet(&ctx, COSA_DNS_SYSCFG_NAMESPACE, "tr_dns_enable_client", sBuf ); 

    while ( fgets(line, sizeof(line), fp) )
    {         
		st = NULL;
        memset(str_key, 0, sizeof(str_key));
        memset(str_val, 0, sizeof(str_val));

        pToken = strtok_r( line, " ", &st);
        if(pToken) strncpy(str_key, pToken, sizeof(str_key)-1 );
    
        pToken = strtok_r(NULL, " ", &st);
        if(pToken) strncpy(str_val, pToken, sizeof(str_val)-1 );

        if ( str_val[ _ansc_strlen(str_val) - 1 ] == '\n' )
        {
            str_val[ _ansc_strlen(str_val) - 1 ] = '\0';
        }
        CcspTraceInfo(("CosaDmlDnsInit str_key %s, str_val %s\n",str_key, str_val ));
       
        if( !strcmp(str_key, "nameserver") && strcmp(str_val, "127.0.0.1") )
        {
            CcspTraceInfo(("CosaDmlDnsInit find dns server IP %s\n", str_val));
            
            //ulogf(ULOG_SYSTEM, UL_DHCP, "%s: DNS IP %s\n", __FUNCTION__, str_val);
            
            pDnsClientServer = &g_dns_client_server[g_DnsClientServerNum];            
            
            /* Instance Number */
            memset(tmpBuff, 0, sizeof(tmpBuff));
            syscfg_get(NULL, "tr_dns_client_server_instance", tmpBuff, sizeof(tmpBuff) );
            g_dns_client_server[ g_DnsClientServerNum ].InstanceNumber = atoi(tmpBuff);   
            //ulogf(ULOG_SYSTEM, UL_DHCP, "%s, InsrtanceNumer %d \n", __FUNCTION__, g_dns_client_server[ g_DnsClientServerNum ].InstanceNumber );
                        
            /* alias */
            memset(tmpBuff, 0, sizeof(tmpBuff));
            memset(sAlias, 0, sizeof(sAlias));
            //snprintf(tmpBuff, sizeof(tmpBuff),  "tr_dns_client_server_alias_%d", g_DnsClientServerNum+1);
            //syscfg_get(NULL, tmpBuff, sAlias, sizeof(sAlias));
#if 0//LNT_EMU
            syscfg_get(NULL, "tr_dns_client_server_alias", sAlias,  sizeof(sAlias) );
#endif
            AnscCopyString(g_dns_client_server[g_DnsClientServerNum].Alias, sAlias );
            //ulogf(ULOG_SYSTEM, UL_DHCP, "%s, Alias %s \n", __FUNCTION__, g_dns_client_server[ g_DnsClientServerNum ].Alias );
            
            /* status */
            g_dns_client_server[ g_DnsClientServerNum ].bEnabled = TRUE;
            g_dns_client_server[ g_DnsClientServerNum ].Status   = COSA_DML_DNS_STATUS_Enabled;
            
            /* DNSServer */
            AnscWriteUlong(&pDnsClientServer->DNSServer.Value, _ansc_inet_addr(str_val) );
            
            /* Interface */
            //TODO:
            if ( Utopia_GetWANConnectionInfo (&ctx, &wanInfo) == SUCCESS )
            {
               AnscCopyString(g_dns_client_server[g_DnsClientServerNum].Interface, wanInfo.ifname);
            }
            /* type */
            g_dns_client_server[ g_DnsClientServerNum ].Type = COSA_DML_DNS_ADDR_SRC_DHCP;
            if ( Utopia_GetWANSettings(&ctx, &g_wan_info) == SUCCESS )
            {
                //ulogf(ULOG_SYSTEM, UL_DHCP, "%s: Get static DNS info\n", __FUNCTION__);               
                strncpy(sDns[0], g_wan_info.wstatic.dns_ipaddr1, IPADDR_SZ);
                strncpy(sDns[1], g_wan_info.wstatic.dns_ipaddr2, IPADDR_SZ );
                strncpy(sDns[2], g_wan_info.wstatic.dns_ipaddr3, IPADDR_SZ );
                for(j = 0; j < 3; j++)
                {
                    sprintf( tmpBuff, "nameserver%d", j+1);
                    //syscfg_get(NULL, sDns[j],tmpBuff, IPHOSTNAME_SZ);
                   // ulogf(ULOG_SYSTEM, UL_DHCP, "%s, sDns%d %s\n", __FUNCTION__,j, sDns[j] );
                    if( !strcmp( sDns[j], str_val ) )
                    {
                        //ulogf(ULOG_SYSTEM, UL_DHCP, "%s, sDns %d: %s\n", __FUNCTION__, j, sDns[j] );
                        g_dns_client_server[ g_DnsClientServerNum ].Type = COSA_DML_DNS_ADDR_SRC_Static;
                        g_wan_info.wan_proto = STATIC;
                        AnscCopyString( g_wan_info.domainname, g_dns_client_server[g_DnsClientServerNum].Interface );
                        g_StaticDnsClientServerNum++;
                    }
                }
            }
            
            AnscCopyString( g_dns_client_server[ g_DnsClientServerNum ].Alias, sAlias); 

            g_DnsClientServerNum++;          
        }
    }
    fclose(fp);

    /*Create the 1st entry as aresult of DHCP */

    g_dns_relay_forwarding[g_CountDnsRelays].Type = COSA_DML_DNS_ADDR_SRC_DHCP;
    g_dns_relay_forwarding[g_CountDnsRelays].bEnabled = TRUE;
    g_dns_relay_forwarding[g_CountDnsRelays].Status   = COSA_DML_DNS_STATUS_Enabled;
    
    g_CountDnsRelays++;
    sprintf(count, "%d", g_CountDnsRelays);
    Utopia_RawSet(&ctx, COSA_DNS_SYSCFG_NAMESPACE, "tr_dns_relay_dhcp_count", count);

    /*Populate the Global structure for an DNS forwarding Static entries */

    rc = -1;
    i = g_CountDnsRelays;
    memset(buf, 0, sizeof(buf));
    Utopia_RawGet(&ctx, COSA_DNS_SYSCFG_NAMESPACE, "tr_dns_relay_count", buf, sizeof(buf) );
    j = atoi(buf);
    g_CountDnsRelays = i+j;
    for( ; i < g_CountDnsRelays; i++)
    {
        pForward = AnscAllocateMemory(sizeof(COSA_DML_DNS_RELAY_ENTRY));
        if(!pForward)
        {
            CcspTraceWarning(("Device.DNS: GetDeviceDnsRelay memory alloc fail!!! \n" ));
	    Utopia_Free(&ctx, 1);
            return NULL;
        }
        memset(buf, 0, sizeof(buf));
        sprintf(key_buf, "tr_dns_relay_forwarding_instance_%d", i+1);
        key_buf[strlen(key_buf)] = '\0';
	Utopia_RawGet(&ctx, COSA_DNS_SYSCFG_NAMESPACE, key_buf, buf, sizeof(buf) );
        pForward->InstanceNumber = atoi(buf);

        memset(key_buf, 0, sizeof(key_buf));
        sprintf(key_buf, "tr_dns_relay_forwarding_alias_%d", i+1);
        key_buf[strlen(key_buf)] = '\0';
	Utopia_RawGet(&ctx, COSA_DNS_SYSCFG_NAMESPACE, key_buf, pForward->Alias, sizeof(pForward->Alias));

        rc = Utopia_Get_DeviceDnsRelayForwarding(&ctx, i+1, (void*)pForward);
        if(rc != 0)
        {
            CcspTraceWarning(("Device.DNS: GetDeviceDnsRelay Failed !!! \n" ));
            Utopia_Free(&ctx, 1);
            return NULL;
        }
        pForward->Type = COSA_DML_DNS_ADDR_SRC_Static;
        memset(buf, 0, sizeof(buf));
        memset(key_buf, 0, sizeof(key_buf));
        sprintf(key_buf, "tr_dns_relay_forwarding_enable_%d", i+1);
        Utopia_RawGet(&ctx, COSA_DNS_SYSCFG_NAMESPACE, key_buf, buf, sizeof(buf) );
        if ( AnscEqualString(buf, "true", TRUE) )
            pForward->Status = COSA_DML_DNS_STATUS_Enabled;
        else
            pForward->Status   = COSA_DML_DNS_STATUS_Disabled;

        AnscCopyMemory(&g_dns_relay_forwarding[i], pForward, sizeof(COSA_DML_DNS_RELAY_ENTRY));
        CcspTraceWarning(("\n\n"));
        CcspTraceWarning(("Device.DNS:InstanceNumber %d \n", g_dns_relay_forwarding[i].InstanceNumber ) );
        CcspTraceWarning(("Device.DNS:Alias %s \n", g_dns_relay_forwarding[i].Alias ) );
        CcspTraceWarning(("Device.DNS:bEnabled %d \n", g_dns_relay_forwarding[i].bEnabled ) );
        CcspTraceWarning(("Device.DNS:Status %d \n", g_dns_relay_forwarding[i].Status ) );
        CcspTraceWarning(("Device.DNS:DNSServer %d \n", g_dns_relay_forwarding[i].DNSServer.Value ) );
        CcspTraceWarning(("Device.DNS:Interface %s \n", g_dns_relay_forwarding[i].Interface ) );
        CcspTraceWarning(("Device.DNS:Type %d \n", g_dns_relay_forwarding[i].Type ) );
        AnscFreeMemory(pForward);
    }
   
    //ulogf(ULOG_SYSTEM, UL_DHCP, "%s: end: g_DnsClientServerNum %d, g_StaticDnsClientServerNum %d\n", __FUNCTION__, g_DnsClientServerNum, g_StaticDnsClientServerNum);
    CcspTraceInfo(("CosaDmlDnsInit end \n"));
    Utopia_Free(&ctx, 1);
    return ANSC_STATUS_SUCCESS;
}

/*
 *  DNS Client
 */
/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsEnableClient
            (
                ANSC_HANDLE                 hContext,
                BOOLEAN                     bEnabled
            )

    description:

       This function enables the DNS client.

    argument:   ANSC_HANDLE                 hContext,
                BOOLEAN                     bEnabled

    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsEnableClient
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bEnabled
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    UtopiaContext       ctx;
    bClientEnable       = bEnabled;
    char cmd[128];
    ulogf(ULOG_SYSTEM, UL_DHCP, "%s:\n", __FUNCTION__);
    
    if (!Utopia_Init(&ctx))
    {
        return ANSC_STATUS_FAILURE;
    }
    
    if ( bEnabled )
    {
        ulogf(ULOG_SYSTEM, UL_DHCP, "%s: bEnabled %d is TRUE\n", __FUNCTION__, bEnabled);
        Utopia_RawSet(&ctx, COSA_DNS_SYSCFG_NAMESPACE, "tr_dns_enable_client", "true");
    }
    else
    {
        ulogf(ULOG_SYSTEM, UL_DHCP, "%s: bEnabled %d is False\n", __FUNCTION__, bEnabled);
        snprintf(cmd, sizeof(cmd), "syscfg unset tr_dns_enable_client");        
        system(cmd);    
        //Utopia_RawSet(&ctx, COSA_DNS_SYSCFG_NAMESPACE, "tr_dns_enable_client", "false");
    }
    Utopia_Free(&ctx, 1); 
    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlIpDnsGetClientStatus
            (
                ANSC_HANDLE                 hContext
            );

    description:

        This function gets the status of the DNS client.

    argument:   ANSC_HANDLE                 hContext
            

    return:     operation status.

**********************************************************************/
COSA_DML_DNS_STATUS
CosaDmlIpDnsGetClientStatus
    (
        ANSC_HANDLE                 hContext
    )
{
    COSA_DML_DNS_STATUS             DnsStatus = COSA_DML_DNS_STATUS_Disabled;
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    COSA_DML_DNS_STATUS             dnsStatus    = COSA_DML_DNS_STATUS_Disabled;
    CHAR                            sBuf[8];
    UtopiaContext   ctx;
    ulogf(ULOG_SYSTEM, UL_DHCP, "%s:\n", __FUNCTION__);

    if (!Utopia_Init(&ctx))
    {
        return COSA_DML_DNS_STATUS_Disabled;
    }
    
    Utopia_RawGet(&ctx, COSA_DNS_SYSCFG_NAMESPACE, "tr_dns_enable_client", sBuf, sizeof(sBuf) ); 
    
    if ( AnscEqualString(sBuf, "true", TRUE) )
    {
        //ulogf(ULOG_SYSTEM, UL_DHCP, "%s: COSA_DML_DNS_STATUS_Enabled %s\n", __FUNCTION__, sBuf);
        dnsStatus = COSA_DML_DNS_STATUS_Enabled;
	Utopia_Free(&ctx, 0);
        return      COSA_DML_DNS_STATUS_Enabled;
    }
    else
    {
       dnsStatus = COSA_DML_DNS_STATUS_Disabled;
    }
    Utopia_Free(&ctx, 0);

    return DnsStatus;
}

/*
 *  DNS Client Server
 */
/**********************************************************************

    caller:     self

    prototype:

        PCOSA_DML_DNS_CLIENT_SERVER
        CosaDmlDnsClientGetServers
            (
                ANSC_HANDLE                 hContext,
                PULONG                      pulCount
            );

    description:

        This routine is to retrieve the complete list of DNS servers, which is a dynamic table.

    argument:   ANSC_HANDLE                 hContext,
                PULONG                      pulCount
                To receive the actual number of entries.

    return:     The pointer to the array of DNS servers, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/
PCOSA_DML_DNS_CLIENT_SERVER
CosaDmlDnsClientGetServers
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;

    PCOSA_DML_DNS_CLIENT_SERVER     pDNSClientServer    = (PCOSA_DML_DNS_CLIENT_SERVER)NULL;
    ULONG                           ClientCount         = g_DnsClientServerNum;

    if (!g_DnsClientServerNum)
    {
        *pulCount = 0;
        return NULL;
    }
    
    pDNSClientServer  = AnscAllocateMemory(sizeof(COSA_DML_DNS_CLIENT_SERVER) * g_DnsClientServerNum);
   
    if( !pDNSClientServer )
    {
        *pulCount = 0;
        ulogf(ULOG_SYSTEM, UL_DHCP, "%s: Meme Error\n", __FUNCTION__);
        return NULL;
    }

    *pulCount = g_DnsClientServerNum;     
    
    AnscCopyMemory(pDNSClientServer, &g_dns_client_server, sizeof(COSA_DML_DNS_CLIENT_SERVER) * g_DnsClientServerNum );
        
    return pDNSClientServer;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsClientSetServerValues
            (
                ANSC_HANDLE             hContext,
                ULONG                       ulIndex,
                ULONG                       ulInstanceNumber,
                char*                        pAlias
            )


    description:

        The API sets DNS server values. 

    argument:   
            ANSC_HANDLE             hContext,

            ULONG                       ulIndex,

            ULONG                       ulInstanceNumber,

            char*                        pAlias
    
    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsClientSetServerValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;

    int i = 0;
    CHAR            inst_str[64];
    CHAR            alias_str[64];
    CHAR            name_str[64];
    UtopiaContext   ctx;

    //ulogf(ULOG_SYSTEM, UL_DHCP, "%s: ulIndex %d, instance %d, Alias %s\n", __FUNCTION__, ulIndex, ulInstanceNumber, pAlias);
        
    if ( ulIndex > g_DnsClientServerNum ) 
    {
        return ANSC_STATUS_FAILURE;
    }
     
    if (!Utopia_Init(&ctx))
    {
        return ANSC_STATUS_FAILURE;
    }
 
    g_dns_client_server[ulIndex].InstanceNumber = ulInstanceNumber;
    AnscCopyString(g_dns_client_server[ulIndex].Alias, pAlias);
    
    sprintf(inst_str, "%d", ulInstanceNumber);
    Utopia_RawSet(&ctx, COSA_DNS_SYSCFG_NAMESPACE, "tr_dns_client_server_instance", inst_str);
    Utopia_RawSet(&ctx, COSA_DNS_SYSCFG_NAMESPACE, "tr_dns_client_server_alias", pAlias);    
        
    Utopia_Free(&ctx, 1); 
    //ulogf(ULOG_SYSTEM, UL_DHCP, "%s: ulIndex %d, g.InstanceNumber %d, alias %s\n", 
    //              __FUNCTION__, ulIndex, g_dns_client_server[ulIndex].InstanceNumber, g_dns_client_server[ulIndex].Alias );
    return returnStatus;
}



/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsClientAddServer
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_DNS_CLIENT_SERVER pEntry
            );

    description:

        The API adds one DNS server entry into DNS client. 

    argument:   ANSC_HANDLE                 hContext,
                PCOSA_DML_DNS_CLIENT_SERVER pEntry
                Caller does not need to fill in Status or Alias fields. Field Type has to be COSA_DML_DNS_ADDR_SRC_Static. Caller does not have to fill in field Interface. Upon return, callee fills in the generated Alias.
    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaDmlDnsClientAddServer
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DNS_CLIENT_SERVER pEntry
    )
{
    ANSC_STATUS     returnStatus = ANSC_STATUS_SUCCESS;
    int i = 0;
    UtopiaContext   ctx;
    CHAR            sBuff[IPADDR_SZ];
   
    ulogf(ULOG_SYSTEM, UL_DHCP, "%s:g_StaticDnsClientServerNum %d\n", __FUNCTION__, g_StaticDnsClientServerNum);
    
    if (!Utopia_Init(&ctx))
    {
        return ANSC_STATUS_FAILURE;
    }   
    
    if ( (g_DnsClientServerNum >= COSA_DML_DNS_MAX_CLIENT_SERVER ) || (g_StaticDnsClientServerNum >= COSA_DML_STATIC_DNS_MAX_CLIENT_SERVER ) )
    {
        ulogf(ULOG_SYSTEM, UL_DHCP, "%s: Error Max Entries exceeded\n", __FUNCTION__);
	Utopia_Free(&ctx, 0);
        return ANSC_STATUS_FAILURE;
    }
    
    if ( !pEntry )
    {
        ulogf(ULOG_SYSTEM, UL_DHCP, "Error %s: pEntry is NULL\n", __FUNCTION__);
	Utopia_Free(&ctx, 0);
        return ANSC_STATUS_FAILURE;
    }
    for(i = 0; i < g_DnsClientServerNum; i++)
    {
        if ( g_dns_client_server[i].InstanceNumber == pEntry->InstanceNumber  )
        {
            ulogf(ULOG_SYSTEM, UL_DHCP, "%s: i %d, conflict pEntry->InstanceNumber %d, with g_.InstanceNumber %d\n", 
                  __FUNCTION__, i, pEntry->InstanceNumber, g_dns_client_server[i].InstanceNumber );
            pEntry->InstanceNumber += 1;
            sprintf(pEntry->Alias, "Server%d", pEntry->InstanceNumber);            
        }
    }
    g_dns_client_server[g_DnsClientServerNum].InstanceNumber  = pEntry->InstanceNumber;  
    g_dns_client_server[g_DnsClientServerNum].bEnabled        = pEntry->bEnabled = TRUE;        
    g_dns_client_server[g_DnsClientServerNum].Status          = pEntry->Status = COSA_DML_DNS_STATUS_Enabled;        
    g_dns_client_server[g_DnsClientServerNum].Type            = pEntry->Type = COSA_DML_DNS_ADDR_SRC_Static; 
    AnscCopyString(g_dns_client_server[g_DnsClientServerNum].Interface, "wan0" );
    AnscCopyString(pEntry->Interface, "wan0" );
    AnscCopyString(g_wan_info.domainname, "wan0" );
    
    g_wan_info.wan_proto = STATIC;
        
    g_dns_client_server[g_DnsClientServerNum].DNSServer.Value = pEntry->DNSServer.Value;
   
#if (defined (_COSA_DRG_TPG_))
    sprintf (sBuff, "%d.%d.%d.%d", 
                        (pEntry->DNSServer.Value >> 24) & 0xFF,
                        (pEntry->DNSServer.Value >> 16) & 0xFF,
                        (pEntry->DNSServer.Value >> 8) & 0xFF,           
                        pEntry->DNSServer.Value & 0xFF );
#endif
#if (defined (_COSA_DRG_CNS_))
          sprintf (sBuff, "%d.%d.%d.%d", 
                         pEntry->DNSServer.Value & 0xFF,
                        (pEntry->DNSServer.Value >> 8) & 0xFF,
                        (pEntry->DNSServer.Value >> 16) & 0xFF,
                        (pEntry->DNSServer.Value >> 24) & 0xFF );  
#endif
                        
    switch ( g_StaticDnsClientServerNum )
    {
        case 0: 
                strncpy( g_wan_info.wstatic.dns_ipaddr1, sBuff, IPADDR_SZ);
                break;
        case 1:
                strncpy( g_wan_info.wstatic.dns_ipaddr2, sBuff, IPADDR_SZ);
                break;
        case 2:
                strncpy( g_wan_info.wstatic.dns_ipaddr3, sBuff, IPADDR_SZ);
                break;        
        default:
            break;
    }
    AnscCopyString(g_dns_client_server[g_DnsClientServerNum].Alias, pEntry->Alias);
    AnscCopyString(g_dns_client_server[g_DnsClientServerNum].Interface, pEntry->Interface);  
    if (CosaDmlDnsClientSetServerValues (
        hContext, 
        g_DnsClientServerNum, 
        pEntry->InstanceNumber, 
        pEntry->Alias ) 
        != ANSC_STATUS_SUCCESS ) 
    {
        ulogf(ULOG_SYSTEM, UL_DHCP, "%s: call CosaDmlDnsClientSetServerValues failed\n", __FUNCTION__);
	Utopia_Free(&ctx, 0);
        return ANSC_STATUS_FAILURE;
    }    
 
    g_DnsClientServerNum++;
    g_StaticDnsClientServerNum++;
    
    if ( Utopia_SetWANSettings ( &ctx, &g_wan_info) != SUCCESS )
    {
        ulogf(ULOG_SYSTEM, UL_DHCP, "Error %s: call Utopia_SetWANSettings failed\n", __FUNCTION__);
        Utopia_Free(&ctx, 0);
        //return ANSC_STATUS_FAILURE;
    }
    else
    {
        Utopia_Free(&ctx, 1); 
    }
    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsClientDelServer
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulInstanceNumber
            );

    description:

        The API delete one DNS server entry from DNS client.

    argument:   ANSC_HANDLE                 hContext,
                ULONG                       ulInstanceNumber

    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsClientDelServer
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    ULONG i = 0;
    ULONG j = 0;
    char cmd[80];
    
    ulogf(ULOG_SYSTEM, UL_DHCP, "%s: ulInstanceNumber %d, g_DnsClientServerNum %d, g_StaticDnsClientServerNum %d\n", 
          __FUNCTION__, ulInstanceNumber, g_DnsClientServerNum, g_StaticDnsClientServerNum);
    for ( i = 0; i < g_DnsClientServerNum; i++)
    {
        if ( g_dns_client_server[i].InstanceNumber == ulInstanceNumber  )
        {
            ulogf(ULOG_SYSTEM, UL_DHCP, "%s: FIND Match i %d, ulInstanceNumber %d, g_.InstanceNumber %d\n", 
                  __FUNCTION__, i, ulInstanceNumber, g_dns_client_server[i].InstanceNumber );
            for ( j = i; j < g_DnsClientServerNum; j++ )
            {
                AnscCopyMemory(&g_dns_client_server[j], 
                               &g_dns_client_server[j+1], 
                               sizeof(COSA_DML_DNS_CLIENT_SERVER));
            }
            g_DnsClientServerNum--;
           
            if( g_dns_client_server[i].Type == COSA_DML_DNS_ADDR_SRC_Static )
            {   
                ulogf(ULOG_SYSTEM, UL_DHCP, "%s: Static g_StaticDnsClientServerNum %d\n", 
                  __FUNCTION__, g_StaticDnsClientServerNum);
                switch ( g_StaticDnsClientServerNum )
                {
                    case 1: 
                        snprintf(cmd, sizeof(cmd), "syscfg set nameserver1 \"\" ");  
                        ulogf(ULOG_SYSTEM, UL_DHCP, "%s:cmd %s\n", __FUNCTION__, cmd);
                        system(cmd);                            
                        break;
                    case 2:
                        snprintf(cmd, sizeof(cmd), "syscfg set nameserver2  \"\" ");   
                        ulogf(ULOG_SYSTEM, UL_DHCP, "%s:cmd %s\n", __FUNCTION__, cmd);
                        system(cmd);       
                        break;
                    case 3:
                        snprintf(cmd, sizeof(cmd), "syscfg set nameserver3  \"\" ");   
                        ulogf(ULOG_SYSTEM, UL_DHCP, "%s:cmd %s\n", __FUNCTION__, cmd);
                        system(cmd);       
                        break;        
                    default:
                        break;
                }
                g_StaticDnsClientServerNum--;
                snprintf(cmd, sizeof(cmd), "sysevent set wan-restart");   
                ulogf(ULOG_SYSTEM, UL_DHCP, "%s:cmd %s\n", __FUNCTION__, cmd);
                system(cmd);                 
            }
            ulogf(ULOG_SYSTEM, UL_DHCP, "%s: OK return ANSC_STATUS_SUCCESS!!\n", __FUNCTION__); 
            return ANSC_STATUS_SUCCESS;
        } 
    }   
    
    ulogf(ULOG_SYSTEM, UL_DHCP, "%s: ERROR should not be here!!\n", __FUNCTION__);
    return ANSC_STATUS_CANT_FIND;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsClientSetServer
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_DNS_CLIENT_SERVER pEntry
            );

    description:

        This function sets the server of the DNS client.

    argument:   ANSC_HANDLE                 hContext,
                PCOSA_DML_DNS_CLIENT_SERVER pEntry

    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsClientSetServer
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DNS_CLIENT_SERVER pEntry
    )
{
    ANSC_STATUS     returnStatus = ANSC_STATUS_SUCCESS;
    UtopiaContext   ctx;
    int             i = 0;
    char            cmd[80];
    CHAR            ipBuff[IPADDR_SZ];
    
    ulogf(ULOG_SYSTEM, UL_DHCP, "%s:\n", __FUNCTION__);

    if( !pEntry || pEntry->Type == COSA_DML_DNS_ADDR_SRC_DHCP )
    {
        ulogf(ULOG_SYSTEM, UL_DHCP, "%s: Error pEntry cannot be NULL DNS Server must be static\n", __FUNCTION__ );
        return ANSC_STATUS_FAILURE;
    }
    
    if (!Utopia_Init(&ctx))
    {
        return ANSC_STATUS_FAILURE;
    }
    
    for ( i = 0; i < g_DnsClientServerNum; i++)
    {
        if ( g_dns_client_server[i].InstanceNumber == pEntry->InstanceNumber )
        {
           //AnscCopyString(g_dns_client_server[i].Interface, pEntry->Interface);  //Before
            AnscCopyMemory(&g_dns_client_server[i], pEntry, sizeof(COSA_DML_DNS_CLIENT_SERVER) ); //??not before
            AnscCopyString(g_wan_info.domainname, g_dns_client_server[i].Interface ); 
#if (defined (_COSA_DRG_TPG_))           
            sprintf (ipBuff, "%d.%d.%d.%d", 
                        (g_dns_client_server[i].DNSServer.Value >> 24) & 0xFF,
                        (g_dns_client_server[i].DNSServer.Value >> 16) & 0xFF,
                        (g_dns_client_server[i].DNSServer.Value >> 8) & 0xFF,                    
                        g_dns_client_server[i].DNSServer.Value & 0xFF );
#endif

#if (defined (_COSA_DRG_CNS_))
          sprintf (ipBuff, "%d.%d.%d.%d", 
                         g_dns_client_server[i].DNSServer.Value & 0xFF,
                        (g_dns_client_server[i].DNSServer.Value >> 8) & 0xFF,
                        (g_dns_client_server[i].DNSServer.Value >> 16) & 0xFF,
                        (g_dns_client_server[i].DNSServer.Value >> 24) & 0xFF );  
#endif
           
            ulogf(ULOG_SYSTEM, UL_DHCP, "%s:g_StaticDnsClientServerNum %d, ipBuff %s\n", __FUNCTION__, g_StaticDnsClientServerNum, ipBuff);
           
            switch ( g_StaticDnsClientServerNum )
            {
                case 1: 
                    snprintf(cmd, sizeof(cmd), "syscfg set nameserver1 %s", ipBuff);        
                    system(cmd);
                    ulogf(ULOG_SYSTEM, UL_DHCP, "%s:cmd %s\n", __FUNCTION__, cmd);
                    break;
                case 2:
                    snprintf(cmd, sizeof(cmd), "syscfg set nameserver2 %s", ipBuff);        
                    system(cmd);   
                    ulogf(ULOG_SYSTEM, UL_DHCP, "%s:cmd %s\n", __FUNCTION__, cmd);
                    break;
                case 3:
                    snprintf(cmd, sizeof(cmd), "syscfg set nameserver3 %s", ipBuff);        
                    system(cmd);       
                    ulogf(ULOG_SYSTEM, UL_DHCP, "%s:cmd %s\n", __FUNCTION__, cmd);
                    break;        
                default:
                    break;
            }
            snprintf(cmd, sizeof(cmd), "sysevent set wan-restart");        
            system(cmd); 
            ulogf(ULOG_SYSTEM, UL_DHCP, "%s:cmd %s\n", __FUNCTION__, cmd);
            Utopia_Free(&ctx, 0); 
            return ANSC_STATUS_SUCCESS;
        }
    }  
    
    Utopia_Free(&ctx, 0); 
    return ANSC_STATUS_CANT_FIND; 
    
   // return returnStatus;
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsClientGetServer
            (
                ANSC_HANDLE                                 hContext,
                PCOSA_DML_DNS_CLIENT_SERVER      pEntry
             )


    description:

        This routine is to retrieve the config of DNS servers, which is a dynamic table.

    argument:  
                    ANSC_HANDLE                 hContext,

                    PCOSA_DML_DNS_CLIENT_SERVER pEntry

    return:    operation status.  

**********************************************************************/

ANSC_STATUS
CosaDmlDnsClientGetServer
    (
        ANSC_HANDLE hContext,
        PCOSA_DML_DNS_CLIENT_SERVER pEntry
     )

{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    
    int i = 0;
    ulogf(ULOG_SYSTEM, UL_DHCP, "%s:\n", __FUNCTION__);

    if ( !pEntry )
    {
        return ANSC_STATUS_FAILURE;
    }
    
    for ( i = 0; i < g_DnsClientServerNum; i++)
    {
        if ( g_dns_client_server[i].InstanceNumber == pEntry->InstanceNumber )
        {
            AnscCopyMemory(pEntry, &g_dns_client_server[i], sizeof(COSA_DML_DNS_CLIENT_SERVER) );
            
            return ANSC_STATUS_SUCCESS;
        }
    }
    return ANSC_STATUS_CANT_FIND; 
   // return returnStatus;
}

/*
 *  DNS Relay
 */
 
/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsEnableRelay
            (
                ANSC_HANDLE                 hContext,
                BOOLEAN                     bEnabled
            )

    description:

        This function enables the DNS relay.

    argument:   ANSC_HANDLE                 hContext,
                BOOLEAN                     bEnabled

    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsEnableRelay
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bEnabled
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    UtopiaContext pCtx;
    int rc = -1;
    char cmd[128];

    if (!Utopia_Init(&pCtx))
    {
        CcspTraceWarning(("Device.DNS: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }
    if ( bEnabled )
    {
        ulogf(ULOG_SYSTEM, UL_DHCP, "%s: bEnabled %d is TRUE\n", __FUNCTION__, bEnabled);
        Utopia_RawSet(&pCtx, COSA_DNS_SYSCFG_NAMESPACE, "tr_dns_enable_relay", "true");
    }
    else
    {
        ulogf(ULOG_SYSTEM, UL_DHCP, "%s: bEnabled %d is False\n", __FUNCTION__, bEnabled);
        snprintf(cmd, sizeof(cmd), "syscfg unset tr_dns_enable_relay");
        system(cmd);
    }
    Utopia_Free(&pCtx, 1);
    return returnStatus;
}


/**********************************************************************

    caller:     self

    prototype:

        COSA_DML_DNS_STATUS
        CosaDmlIpDnsGetRelayStatus
            (
                ANSC_HANDLE                 hContext
            )

    description:

        This function initiate  cosa nat object and return handle.

    argument:   ANSC_HANDLE                 hContext
          

    return:     operation status.

**********************************************************************/
COSA_DML_DNS_STATUS
CosaDmlIpDnsGetRelayStatus
    (
        ANSC_HANDLE                 hContext
    )
{
    COSA_DML_DNS_STATUS             RelayStatus = COSA_DML_DNS_STATUS_Disabled;
    UtopiaContext pCtx;
    int rc = -1;
    char sBuf[128] = {'\0'};

    if (!Utopia_Init(&pCtx))
    {
        CcspTraceWarning(("Device.DNS: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }
    Utopia_RawGet(&pCtx, COSA_DNS_SYSCFG_NAMESPACE, "tr_dns_enable_relay", sBuf, sizeof(sBuf) );

    if ( AnscEqualString(sBuf, "true", TRUE) )
    {
        ulogf(ULOG_SYSTEM, UL_DHCP, "%s: COSA_DML_DNS_STATUS_Enabled %s\n", __FUNCTION__, sBuf);
        RelayStatus = COSA_DML_DNS_STATUS_Enabled;
        Utopia_Free(&pCtx, 0);
    }
    else
    {
       RelayStatus = COSA_DML_DNS_STATUS_Disabled;
       Utopia_Free(&pCtx, 0);
    }

    return RelayStatus;
}

/*
 *  DNS Relay Forwarding (server)
 */
/**********************************************************************

    caller:     self

    prototype:

        PCOSA_DML_DNS_RELAY_ENTRY
        CosaDmlDnsRelayGetServers
            (
                ANSC_HANDLE                 hContext,
                PULONG                      pulCount
            );

    description:

        This function Get the DNS relay server.

    argument:   ANSC_HANDLE                 hContext,
                PULONG                      pulCount
                To receive the actual number of entries.

    return:     The pointer to the array of DNS servers, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/
PCOSA_DML_DNS_RELAY_ENTRY
CosaDmlDnsRelayGetServers
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount
    )
{
    PCOSA_DML_DNS_RELAY_ENTRY       pForward = (PCOSA_DML_DNS_RELAY_ENTRY)NULL;
    PCOSA_DML_DNS_RELAY_ENTRY       ret_pForward = (PCOSA_DML_DNS_RELAY_ENTRY)NULL;
    ULONG                           ulSize   = 0;
    UtopiaContext pCtx;
    INT           i, g_CountDhcp = 0;
    INT           rc = -1;
    CHAR          buf[64] = {'\0'};
    CHAR          str_val[64] = {0};
    CHAR          str_key[64] = {0};
    CHAR          line[256];
    char          *pToken;
    FILE          *fp;
    char          *st = NULL;

    if(!Utopia_Init(&pCtx))
    {
        CcspTraceWarning(("Device.DNS: Error in initializing context!!! \n" ));
	return NULL;
    }
    Utopia_RawGet(&pCtx, COSA_DNS_SYSCFG_NAMESPACE, "tr_dns_relay_count", buf, sizeof(buf) );
    *pulCount = atoi(buf);
    memset(buf, 0, sizeof(buf));
    Utopia_RawGet(&pCtx, COSA_DNS_SYSCFG_NAMESPACE, "tr_dns_relay_dhcp_count", buf, sizeof(buf) );
    i = atoi(buf);
    g_CountDhcp = i;

    if(g_CountDhcp != 0)
    {
        memset(buf, 0, sizeof(buf));
	Utopia_RawGet(&pCtx, COSA_DNS_SYSCFG_NAMESPACE, "tr_dns_relay_forwarding_dhcp_instance_1", buf, sizeof(buf) );

        if (strlen(buf) == 0){
           CcspTraceWarning((" buf len %d\n", strlen(buf)));
           g_dns_relay_forwarding[0].InstanceNumber = 0;
        }else{
           g_dns_relay_forwarding[0].InstanceNumber = atoi(buf);
	   memset(str_val, 0, sizeof(str_val));
	   Utopia_RawGet(&pCtx, COSA_DNS_SYSCFG_NAMESPACE, "tr_dns_relay_forwarding_dhcp_alias_1", str_val, sizeof(str_val) );
	   AnscCopyString(g_dns_relay_forwarding[0].Alias, str_val);
        }
        fp = fopen("/tmp/udhcp.log", "r");
        /*fp = fopen("/mnt/appdata0/udhcp.log", "r");*/	/*Used for Unit testing*/
        if(fp == NULL)
        {
            CcspTraceWarning(("Device.DNS: file read error!!!\n"));
            g_CountDhcp--;
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "%d", g_CountDhcp);
            Utopia_RawSet(&pCtx, COSA_DNS_SYSCFG_NAMESPACE, "tr_dns_relay_forwarding_dhcp_instance_1", buf);
            Utopia_Free(&pCtx, 0);
            return NULL;
        }
        while ( fgets(line, sizeof(line), fp) )
        {
		st = NULL;
        memset(str_key, 0, sizeof(str_key));
        memset(str_val, 0, sizeof(str_val));
        if(strstr(line, "interface"))
        {
            pToken = strtok_r( line, ":", &st);
            if(pToken) strncpy(str_key, pToken, sizeof(str_key)-1 );
            pToken = strtok_r(NULL, ":", &st);
            if(pToken) strncpy(str_val, pToken+1, strlen(pToken)-1);
            str_val[strlen(pToken)-1] = '\0';
            AnscCopyString(g_dns_relay_forwarding[0].Interface, str_val);
        }
        else if(strstr(line, "dns server"))
        {
            pToken = strtok_r( line, ":", &st);
            if(pToken) strncpy(str_key, pToken, sizeof(str_key)-1 );
            pToken = strtok_r(NULL, "\n", &st);
            if(pToken) strncpy(str_val, pToken+1, strlen(pToken)-1);
            str_val[strlen(pToken)-1] = '\0';
            g_dns_relay_forwarding[0].DNSServer.Value = inet_addr(str_val);
            CcspTraceWarning(("\n DNS Server entry created as a result of DHCP : %s  \n", str_val));
        }
        else
            continue;
        }
        rc = Utopia_Set_DeviceDnsRelayForwarding(&pCtx, g_CountDhcp, (void*)&g_dns_relay_forwarding[0]);
        if(rc != 0)
        {
            CcspTraceWarning(("Device.DNS: Utopia_Set_DeviceDnsRelayForwarding Failed !!!\n"));
            Utopia_Free(&pCtx, 0);
            return NULL;
        }
    }
    *pulCount += g_CountDhcp;
    g_CountDnsRelays = *pulCount;
    ulSize = (sizeof(COSA_DML_DNS_RELAY_ENTRY) * (*pulCount));
    ret_pForward = AnscAllocateMemory(ulSize);
    if(!ret_pForward)
    {
        *pulCount = 0;
        CcspTraceWarning(("Device.DNS: GetDeviceDnsRelay memory alloc fail!!! \n" ));
	Utopia_Free(&pCtx, 0);
        return NULL;
    }
    AnscCopyMemory(ret_pForward, g_dns_relay_forwarding, ulSize);
    Utopia_Free(&pCtx, 1);
    return ret_pForward;
}



/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsRelaySetServerValues
            (
                ANSC_HANDLE                 hContext,
                ULONG                           ulIndex,
                ULONG                           ulInstanceNumber,
                char*                            pAlias
            )


    description:

        This function Get the DNS relay server.

    argument:  
                 ANSC_HANDLE                 hContext,

                 ULONG                           ulIndex,

                 ULONG                           ulInstanceNumber,

                 char*                            pAlias

    return:     The pointer to the array of DNS servers, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsRelaySetServerValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    UtopiaContext pCtx;
    INT         rc = -1;
    CHAR        inst_str[64] = {'\0'};
    CHAR        buf[64] = {'\0'};

    CcspTraceWarning(("\n\n ********************set server values********************************************** \n\n"));
    if(ulIndex > g_CountDnsRelays)
        return ANSC_STATUS_FAILURE;

    if (!Utopia_Init(&pCtx))
    {
        CcspTraceWarning(("Device.DNS: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }
    g_dns_relay_forwarding[ulIndex].InstanceNumber = ulInstanceNumber;
    AnscCopyString(g_dns_relay_forwarding[ulIndex].Alias, pAlias);

    sprintf(inst_str, "%d", ulInstanceNumber);
    if(ulIndex == 0)
        sprintf(buf, "tr_dns_relay_forwarding_dhcp_instance_%d", ulIndex+1);
    else
        sprintf(buf, "tr_dns_relay_forwarding_instance_%d", ulIndex+1);
    buf[strlen(buf)] = '\0';
    Utopia_RawSet(&pCtx, COSA_DNS_SYSCFG_NAMESPACE, buf, inst_str);

    memset(buf, 0, sizeof(buf));
    if(ulIndex == 0)
        sprintf(buf, "tr_dns_relay_forwarding_dhcp_alias_%d", ulIndex+1);
    else
        sprintf(buf, "tr_dns_relay_forwarding_alias_%d", ulIndex+1);
    buf[strlen(buf)] = '\0';
    Utopia_RawSet(&pCtx, COSA_DNS_SYSCFG_NAMESPACE, buf, pAlias);

    CcspTraceWarning(("Device.DNS:InstanceNumber %d;\n", g_dns_relay_forwarding[ulIndex].InstanceNumber ));
    CcspTraceWarning(("Device.DNS:Alias %s; \n", g_dns_relay_forwarding[ulIndex].Alias ));
    Utopia_Free(&pCtx, 1);
    return returnStatus;
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsRelayAddServer
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_DNS_RELAY_ENTRY   pEntry
            );

    description:

        The API adds one DNS server entry into DNS relay. 

    argument:   ANSC_HANDLE                 hContext,
    
                PCOSA_DML_DNS_RELAY_ENTRY   pEntry
                Caller does not need to fill in Status or Alias fields. Field Type has to be COSA_DML_DNS_ADDR_SRC_Static. Caller does not have to fill in field Interface. Upon return, callee fills in the generated Alias.

    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsRelayAddServer
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DNS_RELAY_ENTRY   pEntry
    )
{
    UtopiaContext ctx;
    INT           i = 0;
    INT           g_CountStatic = 0;
    CHAR          count[64] = {'\0'};

    if ( !pEntry )
    {
        CcspTraceWarning(("Device.DNS: Invalid Input parameter !! \n" ));
        return ANSC_STATUS_FAILURE;
    }
    if( g_CountDnsRelays >= COSA_DML_DNS_MAX_RELAY_SERVER )
    {
        ulogf(ULOG_SYSTEM, UL_DHCP, "%s: Error Max Entries exceeded\n", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    for( ; i < g_CountDnsRelays; i++)
    {
        if ( g_dns_relay_forwarding[i].InstanceNumber == pEntry->InstanceNumber  )
        {
            pEntry->InstanceNumber += 1;
            sprintf(pEntry->Alias, "Forwarding%d", pEntry->InstanceNumber);
            break;
        }
    }
    g_dns_relay_forwarding[g_CountDnsRelays].InstanceNumber  = pEntry->InstanceNumber;
    g_dns_relay_forwarding[g_CountDnsRelays].bEnabled        = pEntry->bEnabled = TRUE;
    g_dns_relay_forwarding[g_CountDnsRelays].Status          = pEntry->Status = COSA_DML_DNS_STATUS_Enabled;
    g_dns_relay_forwarding[g_CountDnsRelays].Type            = pEntry->Type = COSA_DML_DNS_ADDR_SRC_Static;
    g_dns_relay_forwarding[g_CountDnsRelays].DNSServer.Value = pEntry->DNSServer.Value;
    AnscCopyString(pEntry->Interface, "");	/*If an empty string is specified, the CPE MUST use its routing policy, if necessary*/
    AnscCopyString(g_dns_relay_forwarding[g_CountDnsRelays].Interface, pEntry->Interface);
    AnscCopyString(g_dns_relay_forwarding[g_CountDnsRelays].Alias, pEntry->Alias);

    if (CosaDmlDnsRelaySetServerValues(
        hContext,
        g_CountDnsRelays,
        pEntry->InstanceNumber,
        pEntry->Alias )
        != ANSC_STATUS_SUCCESS )
    {
        CcspTraceWarning(("CosaDmlDnsClientSetServerValues failed inside AddServer\n"));
        return ANSC_STATUS_FAILURE;
    }
    g_CountDnsRelays++;

    if(CosaDmlDnsRelaySetServer(NULL, pEntry)!= ANSC_STATUS_SUCCESS)
    {
        CcspTraceWarning(("CosaDmlDnsRelaySetServer failed inside AddServer\n"));
        return ANSC_STATUS_FAILURE;
    }
    
    if (!Utopia_Init(&ctx))
    {
        CcspTraceWarning(("Device.DNS: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }
    Utopia_RawGet(&ctx, COSA_DNS_SYSCFG_NAMESPACE, "tr_dns_relay_count", count, sizeof(count));
    g_CountStatic = atoi(count);
    memset(count, 0, sizeof(count));
    sprintf(count, "%d", g_CountStatic+1);
    Utopia_RawSet(&ctx, COSA_DNS_SYSCFG_NAMESPACE, "tr_dns_relay_count", count);

    Utopia_Free(&ctx, 1);
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsRelayDelServer
        (
            ANSC_HANDLE                 hContext,
            ULONG                       ulInstanceNumber
        );

    description:

        The API delete one DNS server entry from DNS client. 

    argument:   ANSC_HANDLE                 hContext,
    
                ULONG                       ulInstanceNumber

    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsRelayDelServer
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
    INT  i,j = 0;
    INT  g_CountStatic, g_CountDhcp = 0;
    CHAR count[64] = {'\0'};
    CHAR buf[64] = {'\0'};
    CHAR cmd[64] = {'\0'};
    CHAR inst_str[64] = {'\0'};
    UtopiaContext ctx;

    CcspTraceWarning(("\n\n ******************************del obj ****************************** \n\n" ));
    if (!Utopia_Init(&ctx))
    {
        CcspTraceWarning(("Device.DNS: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }
    Utopia_RawGet(&ctx, COSA_DNS_SYSCFG_NAMESPACE, "tr_dns_relay_dhcp_count", buf, sizeof(buf));
    g_CountDhcp = atoi(buf);
    memset(buf, 0, sizeof(buf));
    Utopia_RawGet(&ctx, COSA_DNS_SYSCFG_NAMESPACE, "tr_dns_relay_count", buf, sizeof(buf));
    g_CountStatic = atoi(buf);

    for ( ; i < g_CountDnsRelays; i++ )
    {
        if( g_dns_relay_forwarding[i].InstanceNumber == ulInstanceNumber)
        {
            if( g_dns_relay_forwarding[i].Type == COSA_DML_DNS_ADDR_SRC_Static){
                g_CountStatic--;
                sprintf(count, "%d", g_CountStatic);
                Utopia_RawSet(&ctx, COSA_DNS_SYSCFG_NAMESPACE, "tr_dns_relay_count", count);
            }else{
                g_CountDhcp--;
                sprintf(count, "%d", g_CountDhcp);
                Utopia_RawSet(&ctx, COSA_DNS_SYSCFG_NAMESPACE, "tr_dns_relay_dhcp_count", count);
            }
            for(j = i; j < g_CountDnsRelays; j++)
            {
                AnscCopyMemory(&g_dns_relay_forwarding[j],
                                &g_dns_relay_forwarding[j+1], sizeof(COSA_DML_DNS_RELAY_ENTRY));
                if(g_CountDhcp == 0){
                    memset(buf, 0, sizeof(buf));
                    sprintf(buf, "tr_dns_relay_forwarding_dhcp_instance_%d", i+1);
                    buf[strlen(buf)] = '\0';
                    Utopia_RawSet(&ctx, COSA_DNS_SYSCFG_NAMESPACE, buf, "");
                    memset(buf, 0, sizeof(buf));
                    sprintf(buf, "tr_dns_relay_forwarding_dhcp_alias_%d", i+1);
                    buf[strlen(buf)] = '\0';
                    Utopia_RawSet(&ctx, COSA_DNS_SYSCFG_NAMESPACE, buf, "");
                }else{
                    memset(buf, 0, sizeof(buf));
                    sprintf(buf, "tr_dns_relay_forwarding_instance_%d", j+1);
                    buf[strlen(buf)] = '\0';
                    sprintf(inst_str, "%d", g_dns_relay_forwarding[j+1].InstanceNumber);
                    Utopia_RawSet(&ctx, COSA_DNS_SYSCFG_NAMESPACE, buf, inst_str);

                    memset(buf, 0, sizeof(buf));
                    sprintf(buf, "tr_dns_relay_forwarding_alias_%d", j+1);
                    buf[strlen(buf)] = '\0';
                    Utopia_RawSet(&ctx, COSA_DNS_SYSCFG_NAMESPACE, buf, g_dns_relay_forwarding[j+1].Alias);
                }
            }
            g_CountDnsRelays--;

            snprintf(cmd, sizeof(cmd), "sysevent set wan-restart");
	    ulogf(ULOG_SYSTEM, UL_DHCP, "%s:cmd %s\n", __FUNCTION__, cmd);
	    system(cmd);   

	    CcspTraceWarning(("\n\n Device.DNS: Deleted one obj... \n\n" ));
            Utopia_Free(&ctx, 1);
            return ANSC_STATUS_SUCCESS;
        }
    }

    Utopia_Free(&ctx, 0);
    return ANSC_STATUS_CANT_FIND;
}
/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsRelaySetServer
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_DNS_RELAY_ENTRY   pEntry
            );

    description:

        This function set the relay server.

    argument:   ANSC_HANDLE                 hContext,
    
                PCOSA_DML_DNS_RELAY_ENTRY   pEntry

    return:     operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlDnsRelaySetServer
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DNS_RELAY_ENTRY   pEntry
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    UtopiaContext ctx;
    INT          rc = -1;
    INT          i = 0;
    CHAR         key_buf[64] = {'\0'};
    CHAR         buf[64] = {'\0'};
    CHAR         cmd[64] = {'\0'};

    if( (!pEntry) )
    {
        CcspTraceWarning(("Invalid input param\n"));
        return ANSC_STATUS_FAILURE;
    }
    if (!Utopia_Init(&ctx))
    {
        CcspTraceWarning(("Failed in initializing context !!!\n"));
        return ANSC_STATUS_FAILURE;
    }

    for(; i < g_CountDnsRelays; i++)
    {
        if ( g_dns_relay_forwarding[i].InstanceNumber == pEntry->InstanceNumber )
        {
            if( pEntry->Type != COSA_DML_DNS_ADDR_SRC_Static){
                g_dns_relay_forwarding[i].bEnabled = pEntry->bEnabled;
                AnscCopyString(g_dns_relay_forwarding[i].Alias, pEntry->Alias);
            }else {
                AnscCopyMemory(&g_dns_relay_forwarding[i], pEntry, sizeof(COSA_DML_DNS_RELAY_ENTRY));
            }

            if(pEntry->bEnabled)
                g_dns_relay_forwarding[i].Status = COSA_DML_DNS_STATUS_Enabled;
            else
                g_dns_relay_forwarding[i].Status = COSA_DML_DNS_STATUS_Disabled;

            CcspTraceWarning(("\n\n ********************set server ********************************************* \n\n"));
            CcspTraceWarning(("Device.DNS:InstanceNumber %d;\n", g_dns_relay_forwarding[i].InstanceNumber ) );
            CcspTraceWarning(("Device.DNS:Alias %s;\n", g_dns_relay_forwarding[i].Alias ) );
            CcspTraceWarning(("Device.DNS:bEnabled %d;\n", g_dns_relay_forwarding[i].bEnabled ) );
            CcspTraceWarning(("Device.DNS:Status %d;\n", g_dns_relay_forwarding[i].Status ) );
            CcspTraceWarning(("Device.DNS:DNSServer %d;\n", g_dns_relay_forwarding[i].DNSServer.Value ) );
            CcspTraceWarning(("Device.DNS:Interface %s;\n", g_dns_relay_forwarding[i].Interface ) );
            CcspTraceWarning(("Device.DNS:Type %d;\n", g_dns_relay_forwarding[i].Type ) );

            rc = Utopia_Set_DeviceDnsRelayForwarding(&ctx, i+1, (void*)&g_dns_relay_forwarding[i]);
            if(rc != 0){
                CcspTraceWarning(("Device.DNS: Utopia_Set_DeviceDnsRelayForwarding Failed !!!\n"));
                Utopia_Free(&ctx, !rc);
                return ANSC_STATUS_FAILURE;
            }
            memset(key_buf, 0, sizeof(key_buf));
            if(i == 0)
                sprintf(key_buf, "tr_dns_relay_forwarding_dhcp_alias_%d", i+1);
            else
                sprintf(key_buf, "tr_dns_relay_forwarding_alias_%d", i+1);
            key_buf[strlen(key_buf)] = '\0';
            Utopia_RawSet(&ctx, COSA_DNS_SYSCFG_NAMESPACE, key_buf, g_dns_relay_forwarding[i].Alias);
    
	    /*snprintf(cmd, sizeof(cmd), "sysevent set wan-restart");
	      ulogf(ULOG_SYSTEM, UL_DHCP, "%s:cmd %s\n", __FUNCTION__, cmd);
	      system(cmd);*/
    
	    Utopia_Free(&ctx, 1);
	    return ANSC_STATUS_SUCCESS;
        }
        else
        {
            if ( g_dns_relay_forwarding[i].DNSServer.Value == pEntry->DNSServer.Value )
            {
                if(g_dns_relay_forwarding[i].bEnabled)
                {
                    pEntry->bEnabled = 0;
                    pEntry->Status   = COSA_DML_DNS_STATUS_Disabled;
                    CcspTraceWarning(("At most one Enabled entry can exist with a given value for DNSServer !!!\n"));
                }
            }
        }
    }

    Utopia_Free(&ctx, 0);
    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlDnsRelayGetServer
            (
                ANSC_HANDLE                           hContext,
                PCOSA_DML_DNS_RELAY_ENTRY   pEntry     
            )



    description:

        This routine is to retrieve the config of DNS servers, which is a dynamic table.

    argument:  
                    ANSC_HANDLE                          hContext,

                    PCOSA_DML_DNS_RELAY_ENTRY  pEntry

    return:    operation status.  

**********************************************************************/

ANSC_STATUS
CosaDmlDnsRelayGetServer
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_DNS_RELAY_ENTRY   pEntry     
    )


{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    int i = 0;

    if ( !pEntry )
    {
        CcspTraceWarning(("Inavlid Input param !!! \n"));
        return ANSC_STATUS_FAILURE;
    }
    for(; i < g_CountDnsRelays; i++)
    {
        if ( g_dns_relay_forwarding[i].InstanceNumber == pEntry->InstanceNumber )
        {
            AnscCopyMemory(pEntry, &g_dns_relay_forwarding[i], sizeof(COSA_DML_DNS_RELAY_ENTRY));
            return ANSC_STATUS_SUCCESS;
        }
    }
    return ANSC_STATUS_CANT_FIND;
}


#endif

