
#if defined(DDNS_BROADBANDFORUM)
#include "cosa_dynamicdns_apis.h"
#include <utctx/utctx.h>
#include <utapi/utapi.h>
#include <utapi/utapi_util.h>
#include <syscfg/syscfg.h>

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
#define  HOST_DISABLED                    5
#define  MAX_HOST_COUNT                   1

typedef struct {
    char ServiceName[64];
    char Name[64];
    char SupportedProtocols[64];
    char Protocol[16];
} DDNS_SERVICE;

DDNS_SERVICE gDdnsServices[] =
{
/*  structure template
    {
        "ServiceName Parameter"
        "Name parameter" //(to be used by UI display)
        "SupportedProtocols"
        "Protocol" //(default)
    },
*/
    {
        "no-ip",
        "No-IP.com",
        "HTTP",
        "HTTP"
    },
    {
        "dyndns",
        "Dyn.com",
        "HTTP",
        "HTTP"
    },
    {
        "duckdns",
        "DuckDNS.org",
        "HTTPS",
        "HTTPS"
    },
    {
        "afraid",
        "FreeDNS.afraid.org",
        "HTTPS",
        "HTTPS"
    },
    {
        "easydns",
        "EasyDNS.com",
        "HTTPS",
        "HTTPS"
    }
};

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

/***********************************************************************
 APIs for Object:

    DyanmicDNS.

    *  CosaDmlDynamicDns_GetEnable
    *  CosaDmlDynamicDns_GetsupportedServices
    *  CosaDmlDynamicDns_SetEnable
***********************************************************************/

BOOL
CosaDmlDynamicDns_GetEnable()
{
   char buf[8] = {0};

   if ((!syscfg_init()) &&
       (!syscfg_get(NULL, "dynamic_dns_enable", buf, sizeof(buf))))
   {
       return (strcmp(buf, "1") == 0);
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

    if ((!syscfg_init()) &&
        (!syscfg_get( NULL, "ddns_service_providers_list", buf, sizeof(buf))))
    {
         _ansc_sprintf(supportedServices, "%s", buf);
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
   char buf[8] = {0};

   if (!syscfg_init())
   {
       syscfg_get(NULL, "dslite_enable", buf, sizeof(buf));
       if((strcmp(buf, "1") == 0) && bValue == TRUE){
           return -1;
       }
       _ansc_sprintf(buf, "%s", ((bValue == FALSE) ? "0" : "1"));
       syscfg_set(NULL, "dynamic_dns_enable", buf);
       if(bValue == FALSE)
       {
          syscfg_set(NULL, "arddnsclient_1::enable", buf);
          syscfg_set(NULL, "ddns_host_enable_1", buf);
       }
       syscfg_commit();
       return 0;
   }
   return -1;
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

static int g_NrDynamicDnsClient =  0;

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
    if (CosaDmlDynamicDns_GetEnable() && pEntry->Enable == TRUE)
    {
        CcspTraceInfo(("%s Going to restart dynamic dns service",__FUNCTION__));
        vsystem("/etc/utopia/service.d/service_dynamic_dns.sh dynamic_dns-restart");
    }

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

ANSC_STATUS
CosaDmlDynamicDns_Client_SetConf
    (
        ULONG ins,
        COSA_DML_DDNS_CLIENT *pEntry
    )
{
    int index, rc = -1;
    char enable_path[sizeof(SYSCFG_HOST_ENABLE_KEY) + 1] = {0};
    BOOLEAN enable = FALSE, isUserconfChanged = FALSE;
    UtopiaContext ctx;
    DynamicDnsClient_t  DDNSclient = {0};

    if ((index = DynamicDns_Client_InsGetIndex(ins)) == -1 || !Utopia_Init(&ctx))
    {
        return ANSC_STATUS_FAILURE;
    }

    if (index >= g_NrDynamicDnsClient)
    {
        return ANSC_STATUS_FAILURE;
    }

    Utopia_GetDynamicDnsClientByIndex(&ctx, index, &DDNSclient);
    if (CosaDmlDynamicDns_GetEnable() && pEntry->Enable == TRUE &&
       ((strcmp(DDNSclient.Username, pEntry->Username) != 0) ||
       (strcmp(DDNSclient.Password, pEntry->Password) != 0) ||
       (strcmp(DDNSclient.Server, pEntry->Server) != 0)))
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

    if (isUserconfChanged == TRUE)
    {
        CcspTraceInfo(("%s Going to restart dynamic dns service",__FUNCTION__));
        vsystem("/etc/utopia/service.d/service_dynamic_dns.sh dynamic_dns-restart");
    }

    return (rc != 0) ? ANSC_STATUS_FAILURE : ANSC_STATUS_SUCCESS;
}

ULONG
CosaDmlDynamicDns_GetClientStatus()
{
   char buf[8] = {0};
   unsigned long pVal = 6;

   if ((!syscfg_init()) &&
       (!syscfg_get(NULL, "ddns_client_Status", buf, sizeof(buf))))
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

   if ((!syscfg_init()) &&
       (!syscfg_get(NULL, "ddns_client_Lasterror", buf, sizeof(buf))))
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
                sprintf(g_DDNSHost[i].Alias, "cpe-ddns-host_%d", i+1);
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
    char status_path[sizeof(SYSCFG_HOST_STATUS_KEY) + 1] = {0};
    char name_path[sizeof(SYSCFG_HOST_NAME_KEY) + 1] = {0};

    if ((index = DynamicDns_Host_InsGetIndex(ins)) == -1 || (!g_DDNSHost))
    {
        return ANSC_STATUS_FAILURE;
    }

    snprintf(enable_path, sizeof(enable_path), SYSCFG_HOST_ENABLE_KEY, index + 1);
    snprintf(status_path, sizeof(status_path), SYSCFG_HOST_STATUS_KEY, index + 1);
    snprintf(name_path, sizeof(name_path), SYSCFG_HOST_NAME_KEY, index + 1);

    g_DDNSHost[index].Status = pEntry->Status;
    g_DDNSHost[index].Enable         = pEntry->Enable;
    if(strcmp(g_DDNSHost[index].Name, pEntry->Name) != 0)
    {
        isHostchanged = TRUE;
    }
    _ansc_strncpy(g_DDNSHost[index].Name,       pEntry->Name,       sizeof(g_DDNSHost[index].Name)-1);

   /* Set syscfg variable */
    UtSetBool(enable_path, g_DDNSHost[index].Enable);
    UtSetString(name_path, g_DDNSHost[index].Name);
    UtSetUlong(status_path, g_DDNSHost[index].Status);

    if (CosaDmlDynamicDns_GetEnable() && (g_DDNSHost[index].Enable == TRUE) && (isHostchanged == TRUE))
    {
        CcspTraceInfo(("%s Going to restart dynamic dns service",__FUNCTION__));
        vsystem("/etc/utopia/service.d/service_dynamic_dns.sh dynamic_dns-restart");
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

    if ((!syscfg_init()) &&
        (!syscfg_get( NULL, "ddns_host_lastupdate_1", buf, sizeof(buf))))
    {
         _ansc_sprintf(lastUpdate, "%s", buf);
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

    if ((!syscfg_init()) &&
        (!syscfg_get( NULL, "ddns_service_providers_list", service_list, sizeof(service_list))))
    {
        ULONG index = 0;
        char enable_path[sizeof(SYSCFG_SERVER_ENABLE_KEY) + 1] = {0};
        char servicename_path[sizeof(SYSCFG_SERVER_SERVICENAME_KEY) +1] = {0};
        g_NrDynamicDnsServer = sizeof(gDdnsServices)/sizeof(DDNS_SERVICE);
        g_DDNSServer = (COSA_DML_DDNS_SERVER *)AnscAllocateMemory(g_NrDynamicDnsServer * sizeof(COSA_DML_DDNS_SERVER));
       for(index = 0; index<g_NrDynamicDnsServer; index++)
        {
            g_DDNSServer[index].Enable = FALSE;
            g_DDNSServer[index].InstanceNumber = index+1;
            sprintf(g_DDNSServer[index].Alias, "cpe-ddns-server_%lu", index+1);
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

    if (vsystem("killall -9 ez-ipupdate") != 0)
    {
       fprintf(stderr, "%s: fail to killall ez-ipupdate\n", __FUNCTION__);
    }

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

    return ANSC_STATUS_SUCCESS;
}
#endif