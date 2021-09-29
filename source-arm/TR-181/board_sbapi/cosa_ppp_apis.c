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

    module: cosa_ppp_apis.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaDmlNatGetPortMappingNumber
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
#include "cosa_ppp_apis.h"
#include "cosa_ppp_internal.h"
#include "plugin_main_apis.h"
#include "safec_lib_common.h"

#if ( defined _COSA_SIM_ )

COSA_DML_PPP_IF_FULL                g_PPPIf[MAXINSTANCE] =
{
    {{0, "", TRUE},{0}},
    {{0, "", TRUE},{0}}
};

ULONG                               g_PPPIfNum = 2;

ANSC_STATUS
CosaDmlPppInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    UNREFERENCED_PARAMETER(hDml);
    UNREFERENCED_PARAMETER(phContext);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlPPPGetSupportedNCPs
    (
        ANSC_HANDLE                 hContext,
        PULONG                      puLong
    )
{
    UNREFERENCED_PARAMETER(hContext);
    *puLong = COSA_DML_PPP_SUPPORTED_NCP_ATCP | COSA_DML_PPP_SUPPORTED_NCP_IPCP;
    
    return ANSC_STATUS_SUCCESS;
}

ULONG
CosaDmlPppIfGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    )
{
    UNREFERENCED_PARAMETER(hContext);
    return g_PPPIfNum;
}

ANSC_STATUS
CosaDmlPppIfGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_PPP_IF_FULL       pEntry
    )
{
    if ( !pEntry )
    {
        return ANSC_STATUS_FAILURE;
    }

    if ( ulIndex < g_PPPIfNum )
    {
        AnscCopyMemory(pEntry, &g_PPPIf[ulIndex], sizeof(COSA_DML_PPP_IF_FULL));
    }
    else
    {
        return ANSC_STATUS_FAILURE;
    }


    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlPppIfSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlPppIfAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_PPP_IF_FULL       pEntry
    )
{
    if ( !pEntry )
    {
        return ANSC_STATUS_FAILURE;
    }
    
    if ( g_PPPIfNum< MAXINSTANCE )
    {
        AnscCopyMemory(&g_PPPIf[g_PPPIfNum], pEntry, sizeof(COSA_DML_PPP_IF_FULL));
        
        g_PPPIfNum++;
    }
    else
    {
        return ANSC_STATUS_FAILURE;
    }
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlPppIfDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
    ULONG                           i = 0;
    ULONG                           j = 0;

    for ( i = 0; i < g_PPPIfNum; i++ )
    {
        if ( g_PPPIf[i].Cfg.InstanceNumber == ulInstanceNumber )
        {
            for ( j = i; j < g_PPPIfNum; j++ )
            {
                AnscCopyMemory(&g_PPPIf[j], &g_PPPIf[j+1], sizeof(COSA_DML_PPP_IF_FULL));
            }

            g_PPPIfNum--;

            return ANSC_STATUS_SUCCESS;
        }
    }
    
    return ANSC_STATUS_CANT_FIND;
}

ANSC_STATUS
CosaDmlPppIfSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_PPP_IF_CFG        pCfg        /* Identified by InstanceNumber */
    )
{
    ULONG                           i = 0;

    if ( !pCfg )
    {
        return ANSC_STATUS_FAILURE;
    }
    
    for ( i = 0; i < g_PPPIfNum; i++)
    {
        if ( pCfg->InstanceNumber == g_PPPIf[i].Cfg.InstanceNumber )
        {
            AnscCopyMemory(&g_PPPIf[i].Cfg, pCfg, sizeof(COSA_DML_PPP_IF_CFG));

            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_CANT_FIND;
}

ANSC_STATUS
CosaDmlPppIfGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_PPP_IF_CFG        pCfg        /* Identified by InstanceNumber */
    )
{
    ULONG                           i = 0;

    if ( !pCfg )
    {
        return ANSC_STATUS_FAILURE;
    }

    for ( i = 0; i < g_PPPIfNum; i++ )
    {
        if ( pCfg->InstanceNumber == g_PPPIf[i].Cfg.InstanceNumber )
        {
            AnscCopyMemory(pCfg, &g_PPPIf[i].Cfg, sizeof(COSA_DML_PPP_IF_CFG));

            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_CANT_FIND;
}

ANSC_STATUS
CosaDmlPppIfGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_PPP_IF_INFO       pInfo
    )
{
    ULONG                           i = 0;

    if ( !pInfo )
    {
        return ANSC_STATUS_FAILURE;
    }

    for ( i = 0; i < g_PPPIfNum; i++ )
    {
        if ( ulInstanceNumber == g_PPPIf[i].Cfg.InstanceNumber )
        {
            AnscCopyMemory(pInfo, &g_PPPIf[i].Info, sizeof(COSA_DML_PPP_IF_INFO));

            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_CANT_FIND;
}

ANSC_STATUS
CosaDmlPppIfReset
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlPppIfGetStats
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPppIfInstanceNumber,
        PCOSA_DML_IF_STATS          pStats
)
{
    if (!pStats)
    {
        return ANSC_STATUS_FAILURE;
    }

    _ansc_memset(pStats, 0, sizeof(COSA_DML_IF_STATS));

    pStats->BytesSent = ulPppIfInstanceNumber;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaPPPApiRemove(ANSC_HANDLE  hContext)
{
    return ANSC_STATUS_SUCCESS;
}

#elif ( defined _COSA_INTEL_USG_ARM_ ) || ( defined _COSA_BCM_MIPS_ )
#include <utctx.h>
#include <utctx_api.h>
#include <utapi.h>
#include <utapi_util.h>
#include "syscfg/syscfg.h"
#include "cosa_drg_common.h"
#include "ansc_string_util.h"
#include <net/if.h>
#include <sys/ioctl.h>
#include <ctype.h>

#if defined(_COSA_BCM_MIPS_) || defined(_ENABLE_DSL_SUPPORT_)
#define INTERFACE "erouter0"
#else
#define INTERFACE "wan0"
#endif

int safe_strcpy(char * dst, char * src, int dst_size);

struct 
{
    wanInfo_t             wan;
    char                  ifname[256];
    char                  lower_ifname[256];
    COSA_DML_IF_STATS     last_stats;   
    ULONG                 last_change;
    char                  ut_alias_name[256]; /*Utopia alias name for syscfg*/
    wanProto_t            last_wan_proto;     /*when disable pppoe, should return to this wan protocol*/
    
}g_ppp_info;

static void get_wan_proto(wanProto_t * p_wan_proto)
{
    char          output[64] =  {0};

    if (!p_wan_proto)
        return;
    
    *p_wan_proto = DHCP;

    if ( !syscfg_get(NULL, "wan_proto", output, sizeof(output)) )
    {
        if (!strncmp(output, "pppoe", 5))
            *p_wan_proto = PPPOE;
    }
}

/*this API is used by bbhm to get pppoe IF address*/
ULONG get_ppp_ip_addr(void)
{
    wanProto_t proto = 0;
    char buf[128]    = {0};
    
    /*ppp0 ip address is valid with 3 conditions: ppp0 ip address, ppp_status is "up"; wan_proto is pppoe*/
    
    get_wan_proto(&proto);
    if (proto != PPPOE) 
        return 0;
    
    if (!commonSyseventGet("ppp_status", buf, sizeof(buf)))
    {
        if (strncmp(buf, "up", 2) != 0)
            return 0;
    }
    else
        return 0;

    return CosaUtilGetIfAddr("ppp0");

    return 0;
}

ANSC_STATUS
CosaDmlPppInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    UNREFERENCED_PARAMETER(hDml);
    UNREFERENCED_PARAMETER(phContext);
    errno_t safec_rc = -1;
    safec_rc = strcpy_s(g_ppp_info.ifname, sizeof(g_ppp_info.ifname), "ppp0");
    if(safec_rc != EOK)
    {
       ERR_CHK(safec_rc);
    }
    safec_rc = strcpy_s(g_ppp_info.lower_ifname, sizeof(g_ppp_info.lower_ifname),INTERFACE);
    if(safec_rc != EOK)
    {
       ERR_CHK(safec_rc);
    }
    safec_rc = strcpy_s(g_ppp_info.ut_alias_name, sizeof(g_ppp_info.ut_alias_name), "PPP_Interface_tr_alias");
    if(safec_rc != EOK)
    {
       ERR_CHK(safec_rc);
    }

    get_wan_proto(&g_ppp_info.last_wan_proto);

    syscfg_init();
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlPPPGetSupportedNCPs
    (
        ANSC_HANDLE                 hContext,
        PULONG                      puLong
    )
{
    UNREFERENCED_PARAMETER(hContext);
    *puLong = COSA_DML_PPP_SUPPORTED_NCP_IPCP;
    return ANSC_STATUS_SUCCESS;
}

ULONG
CosaDmlPppIfGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    )
{
    UNREFERENCED_PARAMETER(hContext);
    return 1;
}

/*Back End struct to/from middle layer struct routines*/
static int ml_cfg_2_be_struct(UtopiaContext * p_ctx, PCOSA_DML_PPP_IF_CFG p_in, wanInfo_t * p_out)
{
    char buf[128] = {0};
    errno_t safec_rc = -1;
    
    /*No chance to set InstanceNumber*/
    wan_ppp_t  * p_ppp = &p_out->ppp;

    Utopia_RawSet(p_ctx, NULL, g_ppp_info.ut_alias_name, p_in->Alias);

    if (p_in->bEnabled && (p_out->wan_proto != PPPOE))
    {
        /*enable PPPoE*/
        get_wan_proto(&g_ppp_info.last_wan_proto);
        p_out->wan_proto = PPPOE;

        memset(buf, 0, sizeof(buf));
        if (!commonSyseventGet("byoi_bridge_mode", buf, sizeof(buf)))
        {
            if (!strncmp(buf, "1", 1))
            {
                int i = 0;

                CcspTraceWarning(("in BYOI bridge mode, the steps are tricky\n"));

                commonSyseventSet("bridge-stop", "");
                
                while(i<5)
                {
                    memset(buf, 0, sizeof(buf));
                    if (!commonSyseventGet("bridge-status", buf, sizeof(buf)))
                    {
                        if (!strncmp(buf, "stopped", strlen("stopped")))
                            break;
                    }
                    sleep(1);
                    i++;
                }
                
                if (i == 5)
                {
                    CcspTraceWarning(("Warning: bridge service is not stopped\n"));
                }
                else 
                {
                    CcspTraceWarning(("OK,  bridge service was stopped\n"));
                }

#if defined (_COSA_BCM_MIPS_) || defined(_ENABLE_DSL_SUPPORT_)
                system("ip link set erouter0 up");
#else
                system("ip link set wan0 up");
#endif
                commonSyseventSet("lan-start", "");
                /*how strange, lan-start won't ifup lan0!!!*/
                system("ip link set lan0 up");
            }
        }
    }
    else if (!p_in->bEnabled && (p_out->wan_proto == PPPOE))
    {
        /*disable PPPOE, if we don't know the last wan protocol, we force it to DHCP*/
        if (g_ppp_info.last_wan_proto != PPPOE )
            p_out->wan_proto = g_ppp_info.last_wan_proto;
        else 
            p_out->wan_proto = DHCP;
    }
    
    p_ppp->max_idle_time = p_in->IdleDisconnectTime;
    
    safec_rc = strcpy_s(p_ppp->username, sizeof(p_ppp->username), p_in->Username);    
    if(safec_rc != EOK)
    {
       ERR_CHK(safec_rc);
    }
    safec_rc = strcpy_s(p_ppp->password, sizeof(p_ppp->password), p_in->Password);    
    if(safec_rc != EOK)
    {
       ERR_CHK(safec_rc);
    }
    
    if (p_in->ConnectionTrigger == COSA_DML_PPP_CONN_TRIGGER_OnDemand)
        p_ppp->conn_method = CONNECT_ON_DEMAND;
    else if (p_in->ConnectionTrigger == COSA_DML_PPP_CONN_TRIGGER_AlwaysOn)
        p_ppp->conn_method = KEEP_ALIVE;
    /*backend don't support manual*/
    
    UTOPIA_SET(p_ctx, UtopiaValue_WAN_PPPoEAccessConcentratorName, p_in->ACName);    
    safec_rc = strcpy_s(p_ppp->service_name, sizeof(p_ppp->service_name), p_in->ServiceName);    
    if(safec_rc != EOK)
    {
       ERR_CHK(safec_rc);
    }

    return 0;
}

static int be_struct_2_ml_cfg(UtopiaContext * p_ctx,  wanInfo_t * p_in, PCOSA_DML_PPP_IF_CFG p_out)
{
    wan_ppp_t * p_ppp = &p_in->ppp;
    errno_t safec_rc = -1;
    
    Utopia_RawGet(p_ctx, NULL, g_ppp_info.ut_alias_name, p_out->Alias, sizeof(p_out->Alias));

    if (!strlen(p_out->Alias)) {
        /*if alias is empty, set a default one*/
        safec_rc = strcpy_s(p_out->Alias, sizeof(p_out->Alias), "Interface1");
        if(safec_rc != EOK)
        {
           ERR_CHK(safec_rc);
        }
        Utopia_RawSet(p_ctx, NULL, g_ppp_info.ut_alias_name, p_out->Alias);        
    }

    /*wan Setting is already got, just map them from p_in*/
    if (p_in->wan_proto == PPPOE)
        p_out->bEnabled = TRUE;
    else 
        p_out->bEnabled = FALSE;

    p_out->LinkType =    COSA_DML_PPP_LINK_TYPE_PPP;
    safec_rc = strcpy_s(p_out->LinkName, sizeof(p_out->LinkName), g_ppp_info.ifname);
    if(safec_rc != EOK)
    {
       ERR_CHK(safec_rc);
    }
    
    /*not supported*/
    p_out->AutoDisconnectTime = p_out->WarnDisconnectDelay = 0;

    p_out->IdleDisconnectTime = p_ppp->max_idle_time;
    
    safec_rc = strcpy_s(p_out->Username, sizeof(p_out->Username), p_ppp->username );
    if(safec_rc != EOK)
    {
       ERR_CHK(safec_rc);
    }
    safec_rc = strcpy_s(p_out->Password, sizeof(p_out->Password), p_ppp->password );
    if(safec_rc != EOK)
    {
       ERR_CHK(safec_rc);
    }
    
    /*default*/
    p_out->MaxMRUSize = 1492;

    if (p_ppp->conn_method == CONNECT_ON_DEMAND)
        p_out->ConnectionTrigger = COSA_DML_PPP_CONN_TRIGGER_OnDemand;
    else if (p_ppp->conn_method == KEEP_ALIVE)
        p_out->ConnectionTrigger = COSA_DML_PPP_CONN_TRIGGER_AlwaysOn;
    else
    {
        /*give a default value when system starts up*/
        p_out->ConnectionTrigger = COSA_DML_PPP_CONN_TRIGGER_AlwaysOn;
    }

    memset(p_out->ACName, 0, sizeof(p_out->ACName));
    UTOPIA_GET(p_ctx, UtopiaValue_WAN_PPPoEAccessConcentratorName, p_out->ACName, sizeof(p_out->ACName));        

    safec_rc = strcpy_s(p_out->ServiceName, sizeof(p_out->ServiceName), p_ppp->service_name);
    if(safec_rc != EOK)
    {
       ERR_CHK(safec_rc);
    }   
    return 0;
}

#define LOG_FILE "/var/log/messages"
static int get_auth_proto(int * p_proto)
{
    FILE * fp;
    char buf[1024] = {0};
    char result[1024] = {0};

    if ((fp = fopen(LOG_FILE, "r+")))
    {
        while (fgets(buf, sizeof(buf)-1, fp))
        {
            if (strstr(buf, "pppd"))
            {
                if (strstr(buf, "authentication succeeded") || strstr(buf, "authentication failed"))
                {
                    memset(result, 0, sizeof(result));
                    memcpy(result, buf, sizeof(result)-1);
                }
            }
            memset(buf, 0, sizeof(buf));
        }

        /*result stores the last line for pppd authenticatoin log*/
        if (result[0])
        {
            if (strstr(result, "CHAP")) 
                *p_proto = COSA_DML_PPP_AUTH_CHAP;
            else if (strstr(result, "PAP"))
                *p_proto = COSA_DML_PPP_AUTH_PAP;
            else if (strstr(result, "MS-CHAP"))
                *p_proto = COSA_DML_PPP_AUTH_MS_CHAP;
        }

        fclose(fp);
    }

    return 0;
}

static int get_session_id(ULONG * p_id)
{

    FILE * fp;
    char buf[1024] = {0};
    char result[1024] = {0};

    /*session id is only valid when ppp0 link is up*/
    if (!get_ppp_ip_addr())
        return 0;

    if ((fp = fopen(LOG_FILE, "r+")))
    {
        while (fgets(buf, sizeof(buf)-1, fp))
        {
            if (strstr(buf, "pppd"))
            {
                if (strstr(buf, "PPP session is") )
                {
                    memset(result, 0, sizeof(result));
                    memcpy(result, buf, sizeof(result)-1);
                }
            }
            memset(buf, 0, sizeof(buf));
        }

        /*result stores the last line for pppd session id*/
        if (result[0])
        {
            char * p = strstr(result, "PPP session is");
            int    id = 0;
            
            if (p)
            {
                if (sscanf(p, "PPP session is %d", &id) == 1)
                    *p_id = id;
            }
        }

        fclose(fp);
    }

    return 0;
    
}

#define DNS_FILE "/var/run/ppp/resolv.conf"
static void be_struct_2_ml_info(UtopiaContext * p_ctx, wanInfo_t * p_in, PCOSA_DML_PPP_IF_INFO p_out)
{
    UNREFERENCED_PARAMETER(p_ctx);
    char buf[128] = {0};
    FILE * fp = NULL;
    errno_t safec_rc = -1;
        
    if (p_in->wan_proto == PPPOE)
    {
        if (!( CosaUtilIoctlXXX(g_ppp_info.lower_ifname, "status", NULL) & IFF_UP) )
            p_out->Status = COSA_DML_IF_STATUS_LowerLayerDown;
        else 
        {
            if (CosaUtilIoctlXXX(g_ppp_info.ifname, "status", NULL) & IFF_UP)
            {
                /*even ppp0 has an ip address, ppp_status could be "preup"  */
                memset(buf, 0, sizeof(buf));
                if (!commonSyseventGet("ppp_status", buf, sizeof(buf)))
                {
                    if (!strncmp(buf, "up", 2))
                        p_out->Status = COSA_DML_IF_STATUS_Up;
                    else if (!strncmp(buf, "down", 4))
                        p_out->Status = COSA_DML_IF_STATUS_Down;            
                    else if (!strncmp(buf, "preup", 5))
                        p_out->Status = COSA_DML_IF_STATUS_Dormant;            
                }
                else 
                    p_out->Status = COSA_DML_IF_STATUS_Down;
            }
            else 
                p_out->Status = COSA_DML_IF_STATUS_Down;
        }
    }
    else 
        p_out->Status = COSA_DML_IF_STATUS_Down;

    safec_rc = strcpy_s(p_out->Name, sizeof(p_out->Name), g_ppp_info.ifname);
    if(safec_rc != EOK)
    {
       ERR_CHK(safec_rc);
    }
    
    p_out->LastChange = g_ppp_info.last_change;

    if (p_in->wan_proto == PPPOE)
    {
        if (!CosaUtilGetIfAddr(g_ppp_info.ifname))
            p_out->ConnectionStatus = COSA_DML_PPP_CONN_STATUS_Disconnected;            
        else
        {
            /*sometimes event ppp0 is there, ppp_status could be preup, but can't be "down" normally*/
            memset(buf, 0, sizeof(buf));
            if (!commonSyseventGet("ppp_status", buf, sizeof(buf)))
            {
                if (!strncmp(buf, "up", 2))
                    p_out->ConnectionStatus = COSA_DML_PPP_CONN_STATUS_Connected;            
                else if (!strncmp(buf, "down", 4))
                    p_out->ConnectionStatus = COSA_DML_PPP_CONN_STATUS_Disconnected;            
                else if (!strncmp(buf, "preup", 5))
                    p_out->ConnectionStatus = COSA_DML_PPP_CONN_STATUS_Connecting;            
            }
            else
                p_out->ConnectionStatus = COSA_DML_PPP_CONN_STATUS_Disconnected;            
        }
    }        
    else
        p_out->ConnectionStatus = COSA_DML_PPP_CONN_STATUS_Unconfigured;

    /*not supported*/
    p_out->LastConnectionError = COSA_DML_PPP_CONN_ERROR_NONE;
    
    p_out->EncryptionProtocol  =  COSA_DML_PPP_ENCRYPTION_None;

    p_out->CompressionProtocol = COSA_DML_PPP_COMPRESSION_None;

    /*the data model doesn't have "None" auth, here use PAP as default. */
    p_out->AuthenticationProtocol = COSA_DML_PPP_AUTH_PAP;
    get_auth_proto((int*)&p_out->AuthenticationProtocol);

    p_out->CurrentMRUSize         = 1492;

    p_out->LCPEcho      = p_in->ppp.redial_period ? p_in->ppp.redial_period : 30;
    /*hardcoded by backend*/
    p_out->LCPEchoRetry = 1;

    p_out->SessionID    = 0;
    get_session_id(&p_out->SessionID);

    p_out->LocalIPAddress.Value = p_out->RemoteIPAddress.Value = 0;
    p_out->DNSServers[0].Value  = p_out->DNSServers[1].Value   = 0;
    
    memset(buf, 0, sizeof(buf));
    if (!commonSyseventGet("ppp_status", buf, sizeof(buf)))
    {
        /*these fields are only meaningful when ppp_status is up*/
        if (!strncmp(buf, "up", 2))
        {
            memset(buf, 0, sizeof(buf));
            if (!commonSyseventGet("ppp_local_ipaddr", buf, sizeof(buf)))
                p_out->LocalIPAddress.Value  = _ansc_inet_addr(buf);        
    
            memset(buf, 0, sizeof(buf));
            if (!commonSyseventGet("ppp_remote_ipaddr", buf, sizeof(buf)))
                p_out->RemoteIPAddress.Value = _ansc_inet_addr(buf);        

            fp = fopen(DNS_FILE, "r+");
            if (fp)
            {
                char ip[128] = {0};
                int  i = 0;

                memset(buf, 0, sizeof(buf));        
                while(fgets(buf, sizeof(buf)-1, fp))
                {
                    memset(ip, 0, sizeof(ip));
            
                    if (sscanf(buf, "nameserver %s", ip) == 1)
                    {
                        p_out->DNSServers[i++].Value = _ansc_inet_addr(ip);       
                        /*we support at most 2 dns server*/
                        if (i ==  2)
                            break;
                    }
                }

                fclose(fp);
            }
        }
    }

    
    return;
}


static void be_struct_2_ml(UtopiaContext * p_ctx, wanInfo_t * p_in, PCOSA_DML_PPP_IF_FULL p_out)
{
    be_struct_2_ml_cfg (p_ctx, p_in, &p_out->Cfg);
    be_struct_2_ml_info(p_ctx, p_in, &p_out->Info);

    return ;
}

ANSC_STATUS
CosaDmlPppIfGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_PPP_IF_FULL       pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    /*we only have one PPP interface*/
    if (ulIndex == 0)
    {
        UtopiaContext utctx = {0};

        if (Utopia_Init(&utctx))
        {
            if (Utopia_GetWANSettings(&utctx, &g_ppp_info.wan))
            {
                Utopia_Free(&utctx, 0);
                return ANSC_STATUS_FAILURE;
            }

            be_struct_2_ml(&utctx, &g_ppp_info.wan, pEntry);            

            g_ppp_info.last_change = pEntry->Info.LastChange = AnscGetTickInSeconds();
            
            pEntry->Cfg.InstanceNumber = 1;
            
            Utopia_Free(&utctx, 0);
        }
    }
    else
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlPppIfSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(ulIndex);
    UNREFERENCED_PARAMETER(ulInstanceNumber);
    UNREFERENCED_PARAMETER(pAlias);
    /*set calculated instanceNumber & alias, no chance to run here*/
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlPppIfAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_PPP_IF_FULL       pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pEntry);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlPppIfDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(ulInstanceNumber);
    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlPppIfSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_PPP_IF_CFG        pCfg        /* Identified by InstanceNumber */
    )
{
    UNREFERENCED_PARAMETER(hContext);
    wanInfo_t *  p_wi = &g_ppp_info.wan;    
	UtopiaContext ctx = {0};
    int           rc  = 0;

    if (pCfg->InstanceNumber != 1)
        return ANSC_STATUS_FAILURE;

    if (!Utopia_Init(&ctx)) 
        return ANSC_STATUS_FAILURE;

    /*when Enable/Disable PPP interface, save current stats, and LastChange*/
    if ((pCfg->bEnabled && p_wi->wan_proto != PPPOE)
        || (!pCfg->bEnabled && p_wi->wan_proto == PPPOE)) 
    {
        CosaUtilGetIfStats((char *)g_ppp_info.ifname,  &g_ppp_info.last_stats);
        g_ppp_info.last_change = AnscGetTickInSeconds();
    }

    ml_cfg_2_be_struct(&ctx, pCfg, p_wi);
    

	rc = Utopia_SetWANSettings(&ctx, p_wi);

	if (rc) 
        CcspTraceWarning(("Utopia_SetWANSettings for PPP failed\n"));

	Utopia_Free(&ctx, !rc);    
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlPppIfGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_PPP_IF_CFG        pCfg        /* Identified by InstanceNumber */
    )
{
    UNREFERENCED_PARAMETER(hContext);
    wanInfo_t *  p_wi = &g_ppp_info.wan;    
	UtopiaContext ctx = {0};

    if (pCfg->InstanceNumber != 1)
        return ANSC_STATUS_FAILURE;

    if (!Utopia_Init(&ctx)) 
        return ANSC_STATUS_FAILURE;

    if (!Utopia_GetWANSettings(&ctx, p_wi))
    {
        /*GetWANSettings succeeds*/
        be_struct_2_ml_cfg(&ctx, p_wi, pCfg);
    }

	Utopia_Free(&ctx, 0);    

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlPppIfGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_PPP_IF_INFO       pInfo
    )
{
    UNREFERENCED_PARAMETER(hContext);
    wanInfo_t *  p_wi = &g_ppp_info.wan;    
	UtopiaContext ctx = {0};

    if (ulInstanceNumber != 1)
        return ANSC_STATUS_FAILURE;

    if (!Utopia_Init(&ctx)) 
        return ANSC_STATUS_FAILURE;

    if (!Utopia_GetWANSettings(&ctx, p_wi))
    {
        /*GetWANSettings succeeds*/
        be_struct_2_ml_info(&ctx, p_wi, pInfo);
    }

	Utopia_Free(&ctx, 0);    

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlPppIfReset
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(ulInstanceNumber);
    commonSyseventSet("wan-restart", "");
    return ANSC_STATUS_SUCCESS;
}

static void update_stats(PCOSA_DML_IF_STATS p_out, PCOSA_DML_IF_STATS p_curr, PCOSA_DML_IF_STATS p_last)
{
    p_out->BytesReceived = 
        (p_curr->BytesReceived - p_last->BytesReceived >0) ? (p_curr->BytesReceived - p_last->BytesReceived) : 0;
    p_out->PacketsReceived = 
        (p_curr->PacketsReceived - p_last->PacketsReceived >0) ? (p_curr->PacketsReceived - p_last->PacketsReceived) : 0;
    p_out->ErrorsReceived = 
        (p_curr->ErrorsReceived - p_last->ErrorsReceived >0) ? (p_curr->ErrorsReceived - p_last->ErrorsReceived) : 0;
    p_out->DiscardPacketsReceived = 
        (p_curr->DiscardPacketsReceived - p_last->DiscardPacketsReceived >0) ? (p_curr->DiscardPacketsReceived - p_last->DiscardPacketsReceived) : 0;
    p_out->BytesSent = 
        (p_curr->BytesSent - p_last->BytesSent >0) ? (p_curr->BytesSent - p_last->BytesSent) : 0;
    p_out->PacketsSent = 
        (p_curr->PacketsSent - p_last->PacketsSent >0) ? (p_curr->PacketsSent - p_last->PacketsSent) : 0;
    p_out->ErrorsSent = 
        (p_curr->ErrorsSent - p_last->ErrorsSent >0) ? (p_curr->ErrorsSent - p_last->ErrorsSent) : 0;
    p_out->DiscardPacketsSent = 
        (p_curr->DiscardPacketsSent - p_last->DiscardPacketsSent >0) ? (p_curr->DiscardPacketsSent - p_last->DiscardPacketsSent) : 0;

    return;
}

ANSC_STATUS
CosaDmlPppIfGetStats
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulPppIfInstanceNumber,
        PCOSA_DML_IF_STATS          pStats
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(ulPppIfInstanceNumber);
    COSA_DML_IF_STATS          curr_stats = {0};

    if (CosaUtilGetIfStats((char *)g_ppp_info.ifname,  &curr_stats))
    {
        update_stats(pStats, &curr_stats, &g_ppp_info.last_stats);
        return ANSC_STATUS_SUCCESS;
    }
    else 
    {
        return ANSC_STATUS_FAILURE;
    }
}

ANSC_STATUS
CosaPPPApiRemove(ANSC_HANDLE  hContext)
{
    UNREFERENCED_PARAMETER(hContext);
    commonSyseventClose();
    return ANSC_STATUS_SUCCESS;
}

#endif
