/*
* If not stated otherwise in this file or this component's LICENSE file the
* following copyright and licenses apply:
*
* Copyright 2021 RDK Management
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

#if defined(DDNS_BROADBANDFORUM)
#include "cosa_dynamicdns_apis.h"
#include <utctx/utctx.h>
#include <utapi/utapi.h>
#include <utapi/utapi_util.h>
#include <syscfg/syscfg.h>
#include <sysevent/sysevent.h>
#include "safec_lib_common.h"

      /* MACROS */
#define  SYSCFG_SERVER_ENABLE_KEY         "ddns_server_enable_%lu"
#define  SYSCFG_SERVER_PROTOCOL_KEY       "ddns_server_protocol_%lu"
#define  SYSCFG_SERVER_CHECKINTERVAL_KEY  "ddns_server_checkinterval_%lu"
#define  SYSCFG_SERVER_RETRYINTERVAL_KEY  "ddns_server_retryinterval_%lu"
#define  SYSCFG_SERVER_MAXRETRIES_KEY     "ddns_server_maxretries_%lu"
#define  SYSCFG_SERVER_SERVERPORT_KEY     "ddns_server_serverport_%lu"
#define  SYSCFG_SERVER_SERVERADDRESS_KEY  "ddns_server_serveraddress_%lu"
#define  SYSCFG_SERVER_SERVICENAME_KEY    "ddns_server_servicename_%lu"
#define  DEFAULT_RETRYINTERVAL            660
#define  DEFAULT_MAXRETRIES               10

#define  SYSCFG_HOST_ENABLE_KEY           "ddns_host_enable_%lu"
#define  SYSCFG_HOST_STATUS_KEY           "ddns_host_status_%lu"
#define  SYSCFG_HOST_NAME_KEY             "ddns_host_name_%lu"
#define  HOST_REGISTERED                  1
#define  CLIENT_UPDATED                   3
#define  HOST_DISABLED                    5
#define  MAX_HOST_COUNT                   1

typedef struct {
    char ServiceName[64];
    char Name[64];
    char SupportedProtocols[64];
    char Protocol[16];
    char ServerAddress[64];
} DDNS_SERVICE;

DDNS_SERVICE gDdnsServices[] =
{
/*  structure template
    {
        "ServiceName Parameter"
        "Name parameter" //(to be used by UI display)
        "SupportedProtocols"
        "Protocol" //(default)
        "Server Address"
    },
*/
    {
        "no-ip",
        "No-IP.com",
        "HTTP",
        "HTTP",
        "www.no-ip.com"
    },
    {
        "dyndns",
        "Dyn.com",
        "HTTP",
        "HTTP",
        "www.dyndns.org"
    },
    {
        "duckdns",
        "DuckDNS.org",
        "HTTPS",
        "HTTPS",
        "www.duckdns.org"
    },
    {
        "afraid",
        "FreeDNS.afraid.org",
        "HTTPS",
        "HTTPS",
        "www.freedns.afraid.org"
    },
    {
        "easydns",
        "EasyDNS.com",
        "HTTPS",
        "HTTPS",
        "www.easydns.com"
    },
    {
        "changeip",
        "ChangeIP.com",
        "HTTP",
        "HTTP",
        "www.changeip.com"
    }
};

#ifdef DDNS_SERVICE_BIN
static void RemoveCheckIntervalEntryFromCron(void)
{
    system("sed -i '/#DDNS_CHECK_INTERVAL/d' /var/spool/cron/crontabs/root");
}

static void RemoveRetryIntervalEntryFromCron(void)
{
    system("sed -i '/#DDNS_RETRY_INTERVAL/d' /var/spool/cron/crontabs/root");
}
#endif

/***********************************************************************
 APIs for SYSCFG GET and SET

     *UtGetString
     *UtSetString
     *UtGetUlong
     *UtSetUlong
     *UtGetBool
     *UtSetBool
***********************************************************************/

static int UtGetString(const char *path, char *pVal, ULONG length)
{
    UtopiaContext ctx;

    if (!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    Utopia_RawGet(&ctx, NULL, (char *)path, pVal, length);

    Utopia_Free(&ctx, 0);
    return ANSC_STATUS_SUCCESS;
}

static int UtSetString(const char *path, char *val)
{
    UtopiaContext ctx;

    if (!Utopia_Init(&ctx))
       return ANSC_STATUS_FAILURE;

    if (val)
        Utopia_RawSet(&ctx, NULL, (char *)path, val);

    Utopia_Free(&ctx, 1);

    return ANSC_STATUS_SUCCESS;
}

static int UtGetUlong(const char *path, ULONG *pVal)
{
    UtopiaContext ctx;
    char buf[64] = {0};

    if (!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    Utopia_RawGet(&ctx, NULL, (char *)path, buf, sizeof(buf));
    if(buf[0] != '\0')
    {
        *pVal = atoi(buf);
    }

    Utopia_Free(&ctx, 0);

    return ANSC_STATUS_SUCCESS;
}

static int UtSetUlong(const char *path, ULONG val)
{
    UtopiaContext ctx;
    char buf[64] = {0};

    if (!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    snprintf(buf, sizeof(buf), "%lu", val);
    Utopia_RawSet(&ctx, NULL, (char *)path, buf);

    Utopia_Free(&ctx, 1);

    return ANSC_STATUS_SUCCESS;
}

static int UtGetBool(const char *path, BOOLEAN *pVal)
{
    UtopiaContext ctx;
    char buf[64] = {0};

    if (!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    Utopia_RawGet(&ctx, NULL, (char *)path, buf, sizeof(buf));
    if (atoi(buf) == 1)
        *pVal = TRUE;
    else
        *pVal = FALSE;

    Utopia_Free(&ctx, 0);

    return ANSC_STATUS_SUCCESS;
}

static int UtSetBool(const char *path, BOOLEAN val)
{
    UtopiaContext ctx;

    if (!Utopia_Init(&ctx))
       return ANSC_STATUS_FAILURE;

    if (val)
        Utopia_RawSet(&ctx, NULL, (char *)path, "1");
    else
        Utopia_RawSet(&ctx, NULL, (char *)path, "0");

    Utopia_Free(&ctx, 1);

    return ANSC_STATUS_SUCCESS;
}

static int resetDynamicDNSStatus(void)
{
    syscfg_set(NULL, "ddns_client_Status", "1"); /* CLIENT_CONNECTING=1 */
    syscfg_set(NULL, "ddns_host_status_1", "2"); /* HOST_UPDATE_NEEDED=2 */

    return ANSC_STATUS_SUCCESS;
}

/***********************************************************************
 APIs for Object:

    DyanmicDNS.

    *  CosaDmlDynamicDns_GetEnable
    *  CosaDmlDynamicDns_GetsupportedServices
    *  CosaDmlDynamicDns_SetEnable
***********************************************************************/

static int g_NrDynamicDnsClient =  0;

BOOL
CosaDmlDynamicDns_GetEnable()
{
   char buf[8] = {0};

   if (!syscfg_get(NULL, "dynamic_dns_enable", buf, sizeof(buf)))
   {
        //return (strcmp(buf, "1") == 0);
        //Returning true by default as dynamic_dns_enable syscfg dependency has been removed as part of removing the DDNS depenedency from Device.DynamicDNS.X_RDK-COM_Enable DM
        return 1;
   }
   return 0;
}

ULONG
CosaDmlDynamicDns_GetsupportedServices
    (
        char*  supportedServices
    )
{
    char buf[128] = {0};
    errno_t rc = -1;

    if (!syscfg_get( NULL, "ddns_service_providers_list", buf, sizeof(buf)))
    {
        /* destination buffer supportedServices is declared as an array size of 1024 bytes in the calling func. */
        rc = strcpy_s(supportedServices, 1024, buf);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    return -1;
}

ULONG
CosaDmlDynamicDns_SetEnable
    (
        BOOL  bValue
    )
{
       if (bValue == TRUE) {
           char buf[4];
           syscfg_get(NULL, "dslite_enable", buf, sizeof(buf));
           if (strcmp(buf, "1") == 0)
               return -1;
       }

       syscfg_set(NULL, "dynamic_dns_enable", (bValue == TRUE) ? "1" : "0");
       syscfg_set("arddnsclient_1", "enable", (bValue == TRUE) ? "1" : "0");
       syscfg_set(NULL, "ddns_host_enable_1", (bValue == TRUE) ? "1" : "0");
       syscfg_commit();

       if (bValue == TRUE && g_NrDynamicDnsClient != 0) {
#ifdef DDNS_SERVICE_BIN
           CcspTraceInfo(("%s Going to invoke ddns service from CosaDmlDynamicDns_SetEnable() \n", __FUNCTION__));
           v_secure_system("service_ddns restart &");
#else
           CcspTraceInfo(("%s Going to invoke script from CosaDmlDynamicDns_SetEnable() \n", __FUNCTION__));
           v_secure_system("/etc/utopia/service.d/service_dynamic_dns.sh dynamic_dns-restart &");
#endif
       }

#ifdef DDNS_SERVICE_BIN
       if (bValue == FALSE) {
           RemoveCheckIntervalEntryFromCron();
           RemoveRetryIntervalEntryFromCron();
       }
#endif

       return 0;
}

/***********************************************************************
 APIs for Object:

    DyanmicDNS.

    *  CosaDmlDynamicDns_Client_GetNumberOfEntries
    *  CosaDmlDynamicDns_Client_GetEntryByIndex
    *  CosaDmlDynamicDns_Client_SetValues
    *  CosaDmlDynamicDns_Client_AddEntry
    *  CosaDmlDynamicDns_Client_DelEntry
    *  CosaDmlDynamicDns_Client_GetConf
    *  CosaDmlDynamicDns_Client_SetConf
***********************************************************************/

static int
DynamicDns_Client_InsGetIndex
    (
        ULONG ins
    )
{
    int i, ins_num, ret = -1;
    UtopiaContext ctx;

    CosaDmlDynamicDns_Client_GetNumberOfEntries();
    if (!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    for (i = 0; i < g_NrDynamicDnsClient; i++)
    {
        Utopia_GetDynamicDnsClientInsNumByIndex(&ctx, i, &ins_num);
        if (ins_num == ins) {
            ret = i;
            break;
        }
    }
    Utopia_Free(&ctx, 0);

    return ret;
}

ULONG
CosaDmlDynamicDns_Client_GetNumberOfEntries(void)
{
    UtopiaContext ctx;

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    Utopia_GetNumberOfDynamicDnsClient(&ctx, &g_NrDynamicDnsClient);
    Utopia_Free(&ctx, 0);
    return g_NrDynamicDnsClient;
}

ANSC_STATUS
CosaDmlDynamicDns_Client_GetEntryByIndex
    (
        ULONG index,
        COSA_DML_DDNS_CLIENT *pEntry
    )
{
    UtopiaContext ctx;
    DynamicDnsClient_t  DDNSclient = {0};

    if (index >= g_NrDynamicDnsClient || !Utopia_Init(&ctx))
    {
        return ANSC_STATUS_FAILURE;
    }

    Utopia_GetDynamicDnsClientByIndex(&ctx, index, &DDNSclient);

    pEntry->InstanceNumber = DDNSclient.InstanceNumber;
    pEntry->Enable = DDNSclient.Enable;
    pEntry->Status = DDNSclient.Status;
    pEntry->LastError = DDNSclient.LastError;
    _ansc_strncpy(pEntry->Alias,    DDNSclient.Alias,    sizeof(pEntry->Alias)-1);
    _ansc_strncpy(pEntry->Username, DDNSclient.Username, sizeof(pEntry->Username)-1);
    _ansc_strncpy(pEntry->Password, DDNSclient.Password, sizeof(pEntry->Password)-1);
    _ansc_strncpy(pEntry->Server,   DDNSclient.Server,   sizeof(pEntry->Server)-1);

    Utopia_Free(&ctx, 0);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDynamicDns_Client_SetValues
    (
        ULONG index,
        ULONG ins,
        const char *alias
    )
{
    int rc = -1;
    UtopiaContext ctx;

    if (index >= g_NrDynamicDnsClient || !Utopia_Init(&ctx))
    {
        return ANSC_STATUS_FAILURE;
    }

    rc = Utopia_SetDynamicDnsClientInsAndAliasByIndex(&ctx, index, ins, alias);
    Utopia_Free(&ctx, !rc);

    return (rc != 0) ? ANSC_STATUS_FAILURE : ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDynamicDns_Client_AddEntry
    (
        COSA_DML_DDNS_CLIENT *pEntry
    )
{
    int rc = -1;
    UtopiaContext ctx;
    DynamicDnsClient_t  DDNSclient = {0};

    if (!Utopia_Init(&ctx))
    {
        return ANSC_STATUS_FAILURE;
    }

    DDNSclient.InstanceNumber = pEntry->InstanceNumber;
    DDNSclient.Enable         = pEntry->Enable;
    _ansc_strncpy(DDNSclient.Alias,    pEntry->Alias,    sizeof(DDNSclient.Alias)-1);
    _ansc_strncpy(DDNSclient.Username, pEntry->Username, sizeof(DDNSclient.Username)-1);
    _ansc_strncpy(DDNSclient.Password, pEntry->Password, sizeof(DDNSclient.Password)-1);
    _ansc_strncpy(DDNSclient.Server,   pEntry->Server,   sizeof(DDNSclient.Server)-1);

    rc = Utopia_AddDynamicDnsClient(&ctx, &DDNSclient);

    Utopia_GetNumberOfDynamicDnsClient(&ctx, &g_NrDynamicDnsClient);
    Utopia_Free(&ctx, !rc);

#if 0
    if (CosaDmlDynamicDns_GetEnable() && pEntry->Enable == TRUE)
    {
        /* reset the DynamicDNS client and host status before restart*/
        resetDynamicDNSStatus();
        CcspTraceInfo(("%s Going to restart dynamic dns service",__FUNCTION__));
#ifdef DDNS_SERVICE_BIN
        if (access("/var/run/updating_ddns_server.txt", F_OK) != 0) {
            v_secure_system("service_ddns restart &");
        }
#else
        v_secure_system("/etc/utopia/service.d/service_dynamic_dns.sh dynamic_dns-restart &");
#endif
    }

#endif

    return (rc != 0) ? ANSC_STATUS_FAILURE : ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDynamicDns_Client_DelEntry
    (
        ULONG ins
    )
{
    int rc = -1;
    UtopiaContext ctx;

    if (!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    rc = Utopia_DelDynamicDnsClient(&ctx, ins);
    Utopia_GetNumberOfDynamicDnsClient(&ctx, &g_NrDynamicDnsClient);
    Utopia_Free(&ctx, !rc);

    return (rc != 0) ? ANSC_STATUS_FAILURE : ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDynamicDns_Client_GetConf
    (
        ULONG ins,
        COSA_DML_DDNS_CLIENT *pEntry
    )
{
    int index;

    if ((index = DynamicDns_Client_InsGetIndex(ins)) == -1)
    {
        return ANSC_STATUS_FAILURE;
    }

    return CosaDmlDynamicDns_Client_GetEntryByIndex(index, pEntry);
}

/* Clear the ddns_return_status{DnIdx} sysevent variable */
static void reset_ddns_return_status (void)
{
    char server[40];

    if (syscfg_get("arddnsclient_1", "Server", server, sizeof(server)) == 0)
    {
        /*
           Assume server is in the form "Device.DynamicDNS.Server.1"
           and index is a single character.
        */
        size_t len = strlen(server);
        if ((len >= 2) && (server[len - 2] == '.'))
        {
            char buf[32];
            char *index = server + len - 1;
            snprintf(buf, sizeof(buf), "ddns_return_status%s", index);
            commonSyseventSet(buf, "");
        }
    }
}

ANSC_STATUS
CosaDmlDynamicDns_Client_SetConf
    (
        ULONG ins,
        COSA_DML_DDNS_CLIENT *pEntry
    )
{
    int index, rc = -1;
    char client_status[12];
    char enable_path[sizeof(SYSCFG_HOST_ENABLE_KEY) + 1] = {0};
    BOOLEAN enable = FALSE, isUserconfChanged = FALSE;
    UtopiaContext ctx;
    DynamicDnsClient_t  DDNSclient = {0};

    ULONG InsNumber;
    ANSC_HANDLE pHostnameInsContext = NULL;
    PCOSA_CONTEXT_LINK_OBJECT pHostLinkObj = NULL;
    COSA_DML_DDNS_HOST *pHostEntry = NULL;
    bool bReadyUpdate = FALSE;

    if ((index = DynamicDns_Client_InsGetIndex(ins)) == -1 || !Utopia_Init(&ctx))
    {
        return ANSC_STATUS_FAILURE;
    }

    if (index >= g_NrDynamicDnsClient)
    {
        return ANSC_STATUS_FAILURE;
    }

    syscfg_get(NULL, "ddns_client_Status", client_status, sizeof(client_status));

    Utopia_GetDynamicDnsClientByIndex(&ctx, index, &DDNSclient);
    if (CosaDmlDynamicDns_GetEnable() && pEntry->Enable == TRUE &&
       ((strcmp(DDNSclient.Username, pEntry->Username) != 0) ||
       (strcmp(DDNSclient.Password, pEntry->Password) != 0) ||
       (strcmp(DDNSclient.Server, pEntry->Server) != 0)) ||
       (atoi(client_status) != CLIENT_UPDATED))
    {
        CcspTraceInfo(("%s UserConf changed \n",__FUNCTION__));
        isUserconfChanged = TRUE;
    }

    DDNSclient.InstanceNumber = pEntry->InstanceNumber;
    DDNSclient.Enable         = pEntry->Enable;
    _ansc_strncpy(DDNSclient.Alias,    pEntry->Alias,    sizeof(DDNSclient.Alias)-1);
    _ansc_strncpy(DDNSclient.Username, pEntry->Username, sizeof(DDNSclient.Username)-1);
    _ansc_strncpy(DDNSclient.Password, pEntry->Password, sizeof(DDNSclient.Password)-1);
    _ansc_strncpy(DDNSclient.Server,   pEntry->Server,   sizeof(DDNSclient.Server)-1);

    rc = Utopia_SetDynamicDnsClientByIndex(&ctx, index, &DDNSclient);
    Utopia_Free(&ctx, !rc);

    snprintf(enable_path, sizeof(enable_path), SYSCFG_HOST_ENABLE_KEY, ins);
    UtGetBool(enable_path, &enable);
    if (enable != pEntry->Enable)
    {
        UtSetBool(enable_path, pEntry->Enable);
        if (pEntry->Enable == TRUE && CosaDmlDynamicDns_GetEnable()) {
            CcspTraceInfo(("%s UserConf changed - Enable changed from false to true \n",__FUNCTION__));
            isUserconfChanged = TRUE;
        }
    }

    pHostnameInsContext = DDNSHostname_GetEntry(NULL, 0, &InsNumber);
    if (pHostnameInsContext != NULL)
    {
        pHostLinkObj = (PCOSA_CONTEXT_LINK_OBJECT)pHostnameInsContext;
        pHostEntry = (COSA_DML_DDNS_HOST *)pHostLinkObj->hContext;
    }

    if ((pEntry->Enable) && (pEntry->Server[0] != '\0') && (pEntry->Username[0] != '\0') && pHostEntry && (pHostEntry->Enable) && (pHostEntry->Name[0]!='\0'))
    {
        if (strstr(pHostEntry->Name, "duckdns") == NULL)
        {
            /* Check whether password is null or not for services other than duckdns */
            if (pEntry->Password[0] != '\0')
            {
                bReadyUpdate = TRUE;
            }
        }
        else
        {
            /* for duckdns no need to check password */
            bReadyUpdate = TRUE;
        }
    }

    if ((isUserconfChanged == TRUE) && (bReadyUpdate == TRUE))
    {
        /* reset the DynamicDNS client and host status before restart*/
        resetDynamicDNSStatus();
        CcspTraceInfo(("%s Going to restart dynamic dns service",__FUNCTION__));
        reset_ddns_return_status();
#ifdef DDNS_SERVICE_BIN
        if (access("/var/run/updating_ddns_server.txt", F_OK) != 0) {
            v_secure_system("service_ddns restart &");
        }
#else
        v_secure_system("/etc/utopia/service.d/service_dynamic_dns.sh dynamic_dns-restart &");
#endif
    }

    return (rc != 0) ? ANSC_STATUS_FAILURE : ANSC_STATUS_SUCCESS;
}

ULONG
CosaDmlDynamicDns_GetClientStatus()
{
   char buf[8] = {0};
   unsigned long pVal = 6;

   if (!syscfg_get(NULL, "ddns_client_Status", buf, sizeof(buf)))
   {
       if(buf[0] != '\0')
       {
           pVal = atoi(buf);
       }
   }
   return pVal;
}

ULONG
CosaDmlDynamicDns_GetClientLastError()
{
   char buf[8] = {0};
   unsigned long pVal = 1;

   if (!syscfg_get(NULL, "ddns_client_Lasterror", buf, sizeof(buf)))
   {
      if(buf[0] != '\0')
       {
           pVal = atoi(buf);
       }
   }
   return pVal;
}


/***********************************************************************
 APIs for Object:

    DyanmicDNS.Client.{i}.Host.{i}

    *  CosaDmlDynamicDns_Host_GetNumberOfEntries
    *  CosaDmlDynamicDns_Host_GetEntryByIndex
    *  CosaDmlDynamicDns_Host_SetValues
    *  CosaDmlDynamicDns_Host_AddEntry
    *  CosaDmlDynamicDns_Host_DelEntry
    *  CosaDmlDynamicDns_Host_GetConf
    *  CosaDmlDynamicDns_Host_SetConf
***********************************************************************/

static int g_NrDynamicDnsHost =  0;
COSA_DML_DDNS_HOST *g_DDNSHost = NULL;

void CosaInitializeTr181DdnsHost()
{
    int i = 0;
    errno_t rc = -1;
    g_NrDynamicDnsHost = MAX_HOST_COUNT;
    if(!g_DDNSHost && (g_NrDynamicDnsHost > 0))
    {
        g_DDNSHost = (COSA_DML_DDNS_HOST *)AnscAllocateMemory(g_NrDynamicDnsHost * sizeof(COSA_DML_DDNS_HOST));
        if(g_DDNSHost)
        {
            for(i=0; i<g_NrDynamicDnsHost; i++)
            {
                g_DDNSHost[i].Enable = FALSE;
                g_DDNSHost[i].InstanceNumber = i+1;
                rc = sprintf_s(g_DDNSHost[i].Alias, sizeof(g_DDNSHost[i].Alias), "cpe-ddns-host_%d", i+1);
                if(rc < EOK)
                {
                    ERR_CHK(rc);
                }
                g_DDNSHost[i].Status = HOST_DISABLED;
            }
        }
    }
}

static int
DynamicDns_Host_InsGetIndex
    (
        ULONG ins
    )
{
   int i, ret = -1;

    CosaDmlDynamicDns_Host_GetNumberOfEntries();

    for (i = 0; i < g_NrDynamicDnsHost; i++)
    {
        if (g_DDNSHost[i].InstanceNumber == ins) {
            ret = i;
            break;
        }
    }

    return ret;
}

ULONG
CosaDmlDynamicDns_Host_GetNumberOfEntries(void)
{
    return g_NrDynamicDnsHost;
}

ANSC_STATUS
CosaDmlDynamicDns_Host_GetEntryByIndex
    (
        ULONG index,
        COSA_DML_DDNS_HOST *pEntry
    )
{
    char enable_path[sizeof(SYSCFG_HOST_ENABLE_KEY) + 1] = {0};
    char status_path[sizeof(SYSCFG_HOST_STATUS_KEY) + 1] = {0};
    char name_path[sizeof(SYSCFG_HOST_NAME_KEY) + 1] = {0};

    BOOLEAN enable = FALSE;
    char name[256] = {0};
    ULONG status = HOST_DISABLED;

    if (index >= g_NrDynamicDnsHost || (!g_DDNSHost))
    {
        return ANSC_STATUS_FAILURE;
    }

    /* Get from Syscfg db if present or return the default values */
    snprintf(enable_path, sizeof(enable_path), SYSCFG_HOST_ENABLE_KEY, index + 1);
    snprintf(status_path, sizeof(status_path), SYSCFG_HOST_STATUS_KEY, index + 1);
    snprintf(name_path, sizeof(name_path), SYSCFG_HOST_NAME_KEY, index + 1);

    if ((UtGetBool(enable_path, &enable) == ANSC_STATUS_SUCCESS) &&
        (UtGetUlong(status_path, &status) == ANSC_STATUS_SUCCESS) &&
        (UtGetString(name_path, name, (sizeof(name) - 1)) == ANSC_STATUS_SUCCESS))
    {
        g_DDNSHost[index].Enable = enable;
        _ansc_strncpy(g_DDNSHost[index].Name, name, sizeof(g_DDNSHost[index].Name)-1);
        g_DDNSHost[index].Status = status;
    }
    pEntry->InstanceNumber = g_DDNSHost[index].InstanceNumber;
    pEntry->Enable = g_DDNSHost[index].Enable;
    _ansc_strncpy(pEntry->Alias, g_DDNSHost[index].Alias, sizeof(pEntry->Alias)-1);
    _ansc_strncpy(pEntry->Name,  g_DDNSHost[index].Name,  sizeof(pEntry->Name)-1);
    pEntry->Status = g_DDNSHost[index].Status;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDynamicDns_Host_SetValues
    (
        ULONG index,
        ULONG ins,
        const char *alias
    )
{

    if (index >= g_NrDynamicDnsHost || !g_DDNSHost)
    {
        return ANSC_STATUS_FAILURE;
    }

    g_DDNSHost[index].InstanceNumber = ins;
    _ansc_strncpy(g_DDNSHost[index].Alias, alias, sizeof(g_DDNSHost[index].Alias)-1);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDynamicDns_Host_AddEntry
    (
        COSA_DML_DDNS_HOST *pEntry
    )
{
    CosaDmlDynamicDns_Host_SetValues(0, pEntry->InstanceNumber, pEntry->Alias);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDynamicDns_Host_DelEntry
    (
        ULONG ins
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDynamicDns_Host_GetConf
    (
        ULONG ins,
        COSA_DML_DDNS_HOST *pEntry
    )
{
    int index;

    if ((index = DynamicDns_Host_InsGetIndex(ins)) == -1)
    {
        return ANSC_STATUS_FAILURE;
    }

    return CosaDmlDynamicDns_Host_GetEntryByIndex(index, pEntry);
}

ANSC_STATUS
CosaDmlDynamicDns_Host_SetConf
    (
        ULONG ins,
        COSA_DML_DDNS_HOST *pEntry
    )
{
    ULONG index;
    BOOLEAN isHostchanged = FALSE;

    char enable_path[sizeof(SYSCFG_HOST_ENABLE_KEY) + 1] = {0};
    char name_path[sizeof(SYSCFG_HOST_NAME_KEY) + 1] = {0};
    char host_status[2];

    ULONG InsNumber;
    ANSC_HANDLE pClientInsContext = NULL;
    PCOSA_CONTEXT_LINK_OBJECT pClientLinkObj = NULL;
    COSA_DML_DDNS_CLIENT *pClientEntry = NULL;
    BOOL bReadyUpdate = FALSE;

    if ((index = DynamicDns_Host_InsGetIndex(ins)) == -1 || (!g_DDNSHost))
    {
        return ANSC_STATUS_FAILURE;
    }

    snprintf(enable_path, sizeof(enable_path), SYSCFG_HOST_ENABLE_KEY, index + 1);
    snprintf(name_path, sizeof(name_path), SYSCFG_HOST_NAME_KEY, index + 1);

    if (g_DDNSHost[index].Enable != pEntry->Enable)
    {
        isHostchanged = TRUE;
        g_DDNSHost[index].Enable = pEntry->Enable;
        UtSetBool(enable_path, g_DDNSHost[index].Enable);
    }

    if(strcmp(g_DDNSHost[index].Name, pEntry->Name) != 0)
    {
        isHostchanged = TRUE;
        _ansc_strncpy(g_DDNSHost[index].Name, pEntry->Name, sizeof(g_DDNSHost[index].Name)-1);
        UtSetString(name_path, g_DDNSHost[index].Name);
    }

    syscfg_get(NULL, "ddns_host_status_1", host_status, sizeof(host_status));
    if (atoi(host_status) != HOST_REGISTERED)
    {
        isHostchanged = TRUE;
    }

    pClientInsContext = DDNSClient_GetEntry(NULL, 0, &InsNumber);
    if (pClientInsContext)
    {
        pClientLinkObj = (PCOSA_CONTEXT_LINK_OBJECT)pClientInsContext;
        pClientEntry = (COSA_DML_DDNS_CLIENT *)pClientLinkObj->hContext;
    }

    if ((pClientInsContext) && (pClientEntry->Enable) && (pClientEntry->Server[0] != '\0') && (pClientEntry->Username[0] != '\0') && (pEntry->Enable) && (pEntry->Name[0]!='\0'))
    {
        if (strstr(pEntry->Name, "duckdns") == NULL)
        {
            /* Check whether password is null or not for services other than duckdns */
            if (pClientEntry->Password[0] != '\0')
            {
                bReadyUpdate = TRUE;
            }
        }
        else
        {
            /* for duckdns no need to check password */
            bReadyUpdate = TRUE;
        }
    }

    if (bReadyUpdate && CosaDmlDynamicDns_GetEnable() && (g_DDNSHost[index].Enable == TRUE) && (isHostchanged == TRUE) && (g_DDNSHost[index].Name[0] != '\0'))
    {
        /* reset the DynamicDNS client and host status before restart*/
        resetDynamicDNSStatus();
        g_DDNSHost[index].Status = 2; /* HOST_UPDATE_NEEDED=2 */
        CcspTraceInfo(("%s Going to restart dynamic dns service",__FUNCTION__));
        reset_ddns_return_status();
#ifdef DDNS_SERVICE_BIN
        if (access("/var/run/updating_ddns_server.txt", F_OK) != 0) {
            v_secure_system("service_ddns restart &");
        }
#else
        v_secure_system("/etc/utopia/service.d/service_dynamic_dns.sh dynamic_dns-restart &");
#endif
    }
    return ANSC_STATUS_SUCCESS;
}

/* To get the Last successful update of this name to the Dynamic DNS Server. */
ULONG
CosaDmlDynamicDns_GetHostLastUpdate
    (
        char*  lastUpdate
    )
{
    char buf[128] = {0};
    errno_t rc = -1;

    if (!syscfg_get( NULL, "ddns_host_lastupdate_1", buf, sizeof(buf)))
    {
        /* destination buffer lastUpdate is defined as array size of 256 bytes in calling func. */
        rc = strcpy_s(lastUpdate, 256, buf);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }
    return -1;
}

/***********************************************************************
 APIs for Object:

    DyanmicDNS.Server.{i}

    *  CosaDmlDynamicDns_Server_GetNumberOfEntries
    *  CosaDmlDynamicDns_Server_GetEntryByIndex
    *  CosaDmlDynamicDns_Server_SetValues
    *  CosaDmlDynamicDns_Server_AddEntry
    *  CosaDmlDynamicDns_Server_DelEntry
    *  CosaDmlDynamicDns_Server_GetConf
    *  CosaDmlDynamicDns_Server_SetConf
***********************************************************************/

static int g_NrDynamicDnsServer =  0;
COSA_DML_DDNS_SERVER  *g_DDNSServer = NULL;

void CosaInitializeTr181DdnsServiceProviderList()
{
    char service_list[128] = {0};
    errno_t rc = -1;

    if (!syscfg_get( NULL, "ddns_service_providers_list", service_list, sizeof(service_list)))
    {
        ULONG index = 0;
        char enable_path[sizeof(SYSCFG_SERVER_ENABLE_KEY) + 1] = {0};
        char servicename_path[sizeof(SYSCFG_SERVER_SERVICENAME_KEY) +1] = {0};
        char serveraddress_path[sizeof(SYSCFG_SERVER_SERVERADDRESS_KEY) +1] = {0};
        g_NrDynamicDnsServer = sizeof(gDdnsServices)/sizeof(DDNS_SERVICE);
        g_DDNSServer = (COSA_DML_DDNS_SERVER *)AnscAllocateMemory(g_NrDynamicDnsServer * sizeof(COSA_DML_DDNS_SERVER));
       for(index = 0; index<g_NrDynamicDnsServer; index++)
        {
            g_DDNSServer[index].Enable = FALSE;
            g_DDNSServer[index].InstanceNumber = index+1;
            rc = sprintf_s(g_DDNSServer[index].Alias, sizeof(g_DDNSServer[index].Alias), "cpe-ddns-server_%lu", index+1);
            if(rc < EOK)
            {
                ERR_CHK(rc);
            }
            if (strstr(service_list, gDdnsServices[index].ServiceName))
            {
                g_DDNSServer[index].Enable = TRUE;
                snprintf(enable_path, sizeof(enable_path), SYSCFG_SERVER_ENABLE_KEY, index+1);
                UtSetBool(enable_path, g_DDNSServer[index].Enable);

                snprintf(g_DDNSServer[index].ServiceName, sizeof(g_DDNSServer[index].ServiceName), gDdnsServices[index].ServiceName);
               snprintf(servicename_path, sizeof(servicename_path), SYSCFG_SERVER_SERVICENAME_KEY, index + 1);
                UtSetString(servicename_path, g_DDNSServer[index].ServiceName);

                snprintf(g_DDNSServer[index].Name, sizeof(g_DDNSServer[index].Name), gDdnsServices[index].Name);
                snprintf(g_DDNSServer[index].SupportedProtocols, sizeof(g_DDNSServer[index].SupportedProtocols), gDdnsServices[index].SupportedProtocols);
                snprintf(g_DDNSServer[index].Protocol, sizeof(g_DDNSServer[index].Protocol), gDdnsServices[index].Protocol);

                snprintf(g_DDNSServer[index].ServerAddress, sizeof(g_DDNSServer[index].ServerAddress), gDdnsServices[index].ServerAddress);
                snprintf(serveraddress_path, sizeof(serveraddress_path), SYSCFG_SERVER_SERVERADDRESS_KEY, index + 1);
                UtSetString(serveraddress_path, g_DDNSServer[index].ServerAddress);
            }
        }
    }
}

static int
DynamicDns_Server_InsGetIndex
    (
        ULONG ins
    )
{
    int i, ret = -1;

    CosaDmlDynamicDns_Server_GetNumberOfEntries();

    for (i = 0; i < g_NrDynamicDnsServer; i++)
    {
        if (g_DDNSServer[i].InstanceNumber == ins) {
            ret = i;
            break;
        }
    }

    return ret;
}

ULONG
CosaDmlDynamicDns_Server_GetNumberOfEntries(void)
{
    return g_NrDynamicDnsServer;
}

ANSC_STATUS
CosaDmlDynamicDns_Server_GetEntryByIndex
    (
        ULONG index,
        COSA_DML_DDNS_SERVER *pEntry
    )
{
    char enable_path[sizeof(SYSCFG_SERVER_ENABLE_KEY) + 1] = {0};
    char protocol_path[sizeof(SYSCFG_SERVER_PROTOCOL_KEY) + 1] = {0};
    char checkinterval_path[sizeof(SYSCFG_SERVER_CHECKINTERVAL_KEY) + 1] = {0};
    char retryinterval_path[sizeof(SYSCFG_SERVER_RETRYINTERVAL_KEY) + 1] = {0};
    char maxretries_path[sizeof(SYSCFG_SERVER_MAXRETRIES_KEY) + 1] = {0};
    char serverport_path[sizeof(SYSCFG_SERVER_SERVERPORT_KEY) + 1] = {0};
    char serveraddress_path[sizeof(SYSCFG_SERVER_SERVERADDRESS_KEY) + 1] = {0};

    char protocol[8] = {0}, serveraddress[256] = {0};
    ULONG checkinterval = 0, retryinterval = DEFAULT_RETRYINTERVAL, maxretries = DEFAULT_MAXRETRIES, serverport = 0;
   BOOLEAN enable = FALSE;

    if (index >= g_NrDynamicDnsServer || (!g_DDNSServer))
    {
        return ANSC_STATUS_FAILURE;
    }

    /* Get from Syscfg db if present or return the default values */
    snprintf(enable_path, sizeof(enable_path), SYSCFG_SERVER_ENABLE_KEY, index + 1);
    snprintf(protocol_path, sizeof(protocol_path), SYSCFG_SERVER_PROTOCOL_KEY, index + 1);
    snprintf(checkinterval_path, sizeof(checkinterval_path), SYSCFG_SERVER_CHECKINTERVAL_KEY, index + 1);
    snprintf(retryinterval_path, sizeof(retryinterval_path), SYSCFG_SERVER_RETRYINTERVAL_KEY, index + 1);
    snprintf(maxretries_path, sizeof(maxretries_path), SYSCFG_SERVER_MAXRETRIES_KEY, index + 1);
    snprintf(serverport_path, sizeof(serverport_path), SYSCFG_SERVER_SERVERPORT_KEY, index + 1);
    snprintf(serveraddress_path, sizeof(serveraddress_path), SYSCFG_SERVER_SERVERADDRESS_KEY, index + 1);

    if ((UtGetBool(enable_path, &enable) == ANSC_STATUS_SUCCESS) &&
        (UtGetString(protocol_path, protocol, (sizeof(protocol) - 1)) == ANSC_STATUS_SUCCESS) &&
        (UtGetString(serveraddress_path, serveraddress, (sizeof(serveraddress) - 1)) == ANSC_STATUS_SUCCESS) &&
        (UtGetUlong(checkinterval_path, &checkinterval) == ANSC_STATUS_SUCCESS) &&
        (UtGetUlong(retryinterval_path, &retryinterval) == ANSC_STATUS_SUCCESS) &&
        (UtGetUlong(maxretries_path, &maxretries) == ANSC_STATUS_SUCCESS) &&
        (UtGetUlong(serverport_path, &serverport) == ANSC_STATUS_SUCCESS))
    {
         g_DDNSServer[index].Enable = enable;
         g_DDNSServer[index].CheckInterval = checkinterval;
         g_DDNSServer[index].RetryInterval = retryinterval;
         g_DDNSServer[index].MaxRetries = maxretries;
         g_DDNSServer[index].ServerPort = serverport;
         if (!AnscEqualString(protocol, "", TRUE))
         {
             _ansc_strncpy(g_DDNSServer[index].Protocol, protocol, sizeof(g_DDNSServer[index].Protocol)-1);
        }
         _ansc_strncpy(g_DDNSServer[index].ServerAddress, serveraddress, sizeof(g_DDNSServer[index].ServerAddress)-1);
    }

    /*Copy the values */
    pEntry->InstanceNumber = g_DDNSServer[index].InstanceNumber;
    pEntry->ServerPort = g_DDNSServer[index].ServerPort;
    pEntry->CheckInterval = g_DDNSServer[index].CheckInterval;
    pEntry->RetryInterval = g_DDNSServer[index].RetryInterval;
    pEntry->MaxRetries = g_DDNSServer[index].MaxRetries;
    pEntry->Enable = g_DDNSServer[index].Enable;
    _ansc_strncpy(pEntry->ServerAddress, g_DDNSServer[index].ServerAddress, sizeof(pEntry->ServerAddress)-1);
    _ansc_strncpy(pEntry->Protocol,      g_DDNSServer[index].Protocol,      sizeof(pEntry->Protocol)-1);
    _ansc_strncpy(pEntry->Alias,         g_DDNSServer[index].Alias,         sizeof(pEntry->Alias)-1);
    _ansc_strncpy(pEntry->Name,          g_DDNSServer[index].Name,          sizeof(pEntry->Name)-1);
    _ansc_strncpy(pEntry->ServiceName,   g_DDNSServer[index].ServiceName,   sizeof(pEntry->ServiceName)-1);
    _ansc_strncpy(pEntry->SupportedProtocols, g_DDNSServer[index].SupportedProtocols, sizeof(pEntry->SupportedProtocols)-1);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDynamicDns_Server_SetValues
    (
        ULONG index,
        ULONG ins,
        const char *alias
    )
{

    if (index >= g_NrDynamicDnsServer || !g_DDNSServer)
    {
        return ANSC_STATUS_FAILURE;
    }

    g_DDNSServer[index].InstanceNumber = ins;
    _ansc_strncpy(g_DDNSServer[index].Alias, alias, sizeof(g_DDNSServer[index].Alias)-1);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDynamicDns_Server_AddEntry
    (
        COSA_DML_DDNS_SERVER *pEntry
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDynamicDns_Server_DelEntry
    (
        ULONG ins
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDynamicDns_Server_GetConf
    (
        ULONG ins,
        COSA_DML_DDNS_SERVER *pEntry
    )
{
    int index;

    if ((index = DynamicDns_Server_InsGetIndex(ins)) == -1)
    {
        return ANSC_STATUS_FAILURE;
    }

    return CosaDmlDynamicDns_Server_GetEntryByIndex(index, pEntry);
}

ANSC_STATUS
CosaDmlDynamicDns_Server_SetConf
    (
        ULONG ins,
        COSA_DML_DDNS_SERVER *pEntry
    )
{
    ULONG index;
    char enable_path[sizeof(SYSCFG_SERVER_ENABLE_KEY) + 1] = {0};
    char protocol_path[sizeof(SYSCFG_SERVER_PROTOCOL_KEY) + 1] = {0};
    char checkinterval_path[sizeof(SYSCFG_SERVER_CHECKINTERVAL_KEY) + 1] = {0};
    char retryinterval_path[sizeof(SYSCFG_SERVER_RETRYINTERVAL_KEY) + 1] = {0};
    char maxretries_path[sizeof(SYSCFG_SERVER_MAXRETRIES_KEY) + 1] = {0};
    char serverport_path[sizeof(SYSCFG_SERVER_SERVERPORT_KEY) + 1] = {0};
    char serveraddress_path[sizeof(SYSCFG_SERVER_SERVERADDRESS_KEY) + 1] = {0};
    char servicename_path[sizeof(SYSCFG_SERVER_SERVICENAME_KEY) +1] = {0};

    if ((index = DynamicDns_Server_InsGetIndex(ins)) == -1 || (!g_DDNSServer))
    {
        return ANSC_STATUS_FAILURE;
    }

#ifndef DDNS_SERVICE_BIN
    if (vsystem("killall -9 ez-ipupdate") != 0)
    {
       fprintf(stderr, "%s: fail to killall ez-ipupdate\n", __FUNCTION__);
    }
#endif

    snprintf(enable_path, sizeof(enable_path), SYSCFG_SERVER_ENABLE_KEY, index + 1);
    snprintf(protocol_path, sizeof(protocol_path), SYSCFG_SERVER_PROTOCOL_KEY, index + 1);
    snprintf(checkinterval_path, sizeof(checkinterval_path), SYSCFG_SERVER_CHECKINTERVAL_KEY, index + 1);
    snprintf(retryinterval_path, sizeof(retryinterval_path), SYSCFG_SERVER_RETRYINTERVAL_KEY, index + 1);
    snprintf(maxretries_path, sizeof(maxretries_path), SYSCFG_SERVER_MAXRETRIES_KEY, index + 1);
    snprintf(serverport_path, sizeof(serverport_path), SYSCFG_SERVER_SERVERPORT_KEY, index + 1);
    snprintf(serveraddress_path, sizeof(serveraddress_path), SYSCFG_SERVER_SERVERADDRESS_KEY, index + 1);
    snprintf(servicename_path, sizeof(servicename_path), SYSCFG_SERVER_SERVICENAME_KEY, index + 1);

    g_DDNSServer[index].ServerPort = pEntry->ServerPort;
    g_DDNSServer[index].CheckInterval = pEntry->CheckInterval;
    g_DDNSServer[index].RetryInterval = pEntry->RetryInterval;
    g_DDNSServer[index].MaxRetries = pEntry->MaxRetries;
    g_DDNSServer[index].InstanceNumber = pEntry->InstanceNumber;
    g_DDNSServer[index].Enable         = pEntry->Enable;
    _ansc_strncpy(g_DDNSServer[index].ServerAddress, pEntry->ServerAddress, sizeof(g_DDNSServer[index].ServerAddress)-1);
    _ansc_strncpy(g_DDNSServer[index].Protocol,      pEntry->Protocol,      sizeof(g_DDNSServer[index].Protocol)-1);
    _ansc_strncpy(g_DDNSServer[index].ServiceName,      pEntry->ServiceName,      sizeof(g_DDNSServer[index].ServiceName)-1);

    /* Set syscfg variable */
    UtSetBool(enable_path, g_DDNSServer[index].Enable);
    UtSetString(protocol_path, g_DDNSServer[index].Protocol);
    UtSetString(serveraddress_path, g_DDNSServer[index].ServerAddress);
    UtSetString(servicename_path, g_DDNSServer[index].ServiceName);
    UtSetUlong(checkinterval_path, g_DDNSServer[index].CheckInterval);
    UtSetUlong(retryinterval_path, g_DDNSServer[index].RetryInterval);
    UtSetUlong(maxretries_path, g_DDNSServer[index].MaxRetries);
    UtSetUlong(serverport_path, g_DDNSServer[index].ServerPort);

#ifdef DDNS_SERVICE_BIN
    if (g_DDNSServer[index].CheckInterval != 0)
    {
        BOOLEAN client_enable = 0;
        UtGetBool("arddnsclient_1::enable", &client_enable);
        if (client_enable)
        {
            char buf[30];
            int server_index = -1;
            UtGetString("arddnsclient_1::Server", buf, sizeof(buf));
            if ((sscanf(buf, "Device.DynamicDNS.Server.%d", &server_index) == 1) &&
                (server_index == (index + 1)))
            {
                int se_fd = -1;
                token_t se_token;
                RemoveCheckIntervalEntryFromCron();
                FILE *cron_fp = fopen("/var/spool/cron/crontabs/root", "a+");
                if (!cron_fp)
                    return ANSC_STATUS_FAILURE;
                fprintf(cron_fp, "* * * * * /usr/bin/service_ddns ddns-check &  #DDNS_CHECK_INTERVAL\n");
                fclose(cron_fp);
                se_fd = sysevent_open("127.0.0.1", SE_SERVER_WELL_KNOWN_PORT, SE_VERSION, "service_ddns", &se_token);
                sysevent_set(se_fd, se_token, "crond-restart", "1", 0);
                sysevent_close(se_fd, se_token);
            }
        }
    }
    else
    {
        /* Delete the DDNS_CHECK_INTERVAL entry from crontab */
        RemoveCheckIntervalEntryFromCron();
    }
#endif

    return ANSC_STATUS_SUCCESS;
}
#endif
