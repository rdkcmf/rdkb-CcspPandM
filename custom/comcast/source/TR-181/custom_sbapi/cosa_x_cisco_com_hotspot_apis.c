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

#ifdef CONFIG_CISCO_HOTSPOT
#include "cosa_apis.h"
#include "ccsp_psm_helper.h"
#include "ansc_platform.h"
#include "plugin_main_apis.h"

#include "cosa_x_cisco_com_hotspot_apis.h"
#include "dhcpsnooper.h"
#include "cosa_ethernet_dml.h"
#include "safec_lib_common.h"

#define FIRST_SSID_INS  5

#define GRETEST

#if defined(GRETEST)

#ifdef AnscTraceWarning
#undef AnscTraceWarning
#define AnscTraceWarning(a) printf("%s:%d> ", __FUNCTION__, __LINE__); printf a
#endif

#ifdef AnscTraceError
#undef AnscTraceError
#define AnscTraceError(a) printf("%s:%d> ", __FUNCTION__, __LINE__); printf a
#endif

//#ifdef AnscTraceDebug
#undef AnscTraceDebug
#define AnscTraceDebug(a) printf("%s:%d> ", __FUNCTION__, __LINE__); printf a
//#endif

#endif

extern void* g_pDslhDmlAgent;
#if defined(_INTEL_BUG_FIXES_)
extern ANSC_HANDLE bus_handle;
#endif

struct hs_client {
    char                mac[18];
    char                hostname[18];
    int                 rssi;
    char                v4addr[16];
    char                dh4_status[64];
    char                v6addr[64];
    char                v6pref[64];
    char                v6lladdr[64];
    char                dh6_status[64];
};

struct hs_ssid {
    char                path[128];
    char                ssid[128];
    int                 ncli;
    struct hs_client    clis[MAX_HS_CLIS];
};

static int              g_num_hs_ssid = 2;
static struct hs_ssid   g_hs_ssids[MAX_HS_SSIDS];
static snooper_statistics_s *g_snstat = NULL;

ANSC_STATUS
COSAGetParamValueByPathName
    (
        void*                      bus_handle,
        parameterValStruct_t       *val,
        ULONG                      *parameterValueLength
    );

static int
hhs_psm_get(const char *param, char *value, int size)
{
    char *val;
    CCSP_MESSAGE_BUS_INFO *businfo;

    if (PSM_Get_Record_Value2(g_MessageBusHandle, g_GetSubsystemPrefix(g_pDslhDmlAgent),
                (char *)param, NULL, &val) != CCSP_SUCCESS)
        return -1;

    snprintf(value, size, "%s", val);

    businfo = g_MessageBusHandle;
    businfo->freefunc(val);
    return 0;
}

/*
static struct hs_ssid *get_hs_ssid(const char *ssid)
{
    int i;

    for (i = 0; i < g_num_hs_ssid; i++) {
        if (strlen(g_hs_ssids[i].ssid) == 0 
                || strcmp(g_hs_ssids[i].ssid, ssid) == 0)
            return &g_hs_ssids[i];
    }

    return NULL;
}
*/

static int circuit_id_get_ssid(const char *circuit_id, char *ssid, int size)
{
    char *buf, *cp, *cp2;

    if ((buf = strdup(circuit_id)) == NULL)
        return -1;

    if ((cp = strchr(buf, ';')) == NULL) {
        free(buf);
        return -1;
    }
    *cp++ = '\0';

    if ((cp2 = strchr(cp, ';')) == NULL) {
        free(buf);
        return -1;
    }
    *cp2 ='\0';

    snprintf(ssid, size, "%s", cp);
    free(buf);
    return 0;
}

#if 0
static void dump_snoop_stat(const snooper_statistics_s *stat)
{
    int i;

    if (!stat) {
        AnscTraceDebug(("Nothing to dump\n"));
        return;
    }

    AnscTraceDebug(("-------  Snooper Statistics  -------\n"));
    AnscTraceDebug(("%-15s %d\n", "Enabled", stat->snooper_enabled));
    AnscTraceDebug(("%-15s %d\n", "Debug", stat->snooper_debug_enabled));
    AnscTraceDebug(("%-15s %d\n", "Circuit ID", stat->snooper_circuit_id_enabled));
    AnscTraceDebug(("%-15s %d\n", "Remote ID", stat->snooper_remote_id_enabled));
    AnscTraceDebug(("%-15s %d\n", "First Queue", stat->snooper_first_queue));
    AnscTraceDebug(("%-15s %d\n", "Num Queues", stat->snooper_num_queues));
    AnscTraceDebug(("%-15s %d\n", "Max Queues", stat->snooper_max_queues));
    AnscTraceDebug(("%-15s %d\n", "DHCP Pkts", stat->snooper_dhcp_packets));
    AnscTraceDebug(("%-15s %d\n", "Max Clients", stat->snooper_max_clients));
    AnscTraceDebug(("%-15s %d\n", "Num Clients", stat->snooper_num_clients));

    for (i = 0; i < stat->snooper_num_clients; i++) {
        AnscTraceDebug(("  %d %-10s %s\n", i, "Remote ID", stat->snooper_clients[i].remote_id));
        AnscTraceDebug(("  %d %-10s %s\n", i, "Circuit ID", stat->snooper_clients[i].circuit_id));
        AnscTraceDebug(("  %d %-10s %s\n", i, "IP Addr", stat->snooper_clients[i].ipv4_addr));
        AnscTraceDebug(("  %d %-10s %s\n", i, "DHCP Status", stat->snooper_clients[i].dhcp_status));
        AnscTraceDebug(("  %d %-10s %s\n", i, "Hostname", stat->snooper_clients[i].hostname));
    }

    return;
}
#endif

static ULONG HsSsidUpdateTime = 0;
#define TIME_NO_NEGATIVE(x) ((long)(x) < 0 ? 0 : (x))
#define COSA_DML_HS_SSID_ACCESS_INTERVAL     8

BOOL
Hs_Ssid_IsUpdated()
{

	if ( HsSsidUpdateTime == 0 ) 
    {
        HsSsidUpdateTime = AnscGetTickInSeconds();

        return TRUE;
    }
    
    if ( HsSsidUpdateTime >= TIME_NO_NEGATIVE(AnscGetTickInSeconds() - COSA_DML_HS_SSID_ACCESS_INTERVAL ) )
    {
        return FALSE;
    }
    else 
    {
        HsSsidUpdateTime = AnscGetTickInSeconds();

        return TRUE;
    }
    return TRUE;
}

static int load_hs_ssid(void)
{
    struct hs_ssid *hsssid;
    char ssid[128];
    snooper_client_list *sncli;
    struct hs_client *cli;
    int i;
    char ifs[1024];
    char *tok, *delim, *sp, *from;

    extern ANSC_HANDLE bus_handle;
    char param[128];
    parameterValStruct_t valStru;
    ULONG valSize;

	if(!Hs_Ssid_IsUpdated()) {
		return 0;
	}

    if (!g_snstat) {
        AnscTraceError(("Snooper shm not ready\n"));
        return -1;
    }
    //dump_snoop_stat(g_snstat);

    g_num_hs_ssid = 0;
    memset(g_hs_ssids, 0, sizeof(g_hs_ssids));
    
    if (hhs_psm_get("dmsb.hotspot.gre.1.LocalInterfaces", ifs, sizeof(ifs)) != 0) {
        AnscTraceError(("fail to get LocalInterfaces in PSM\n"));
        return -1;
    }

    for (from = ifs, delim = ","; 
            (tok = strtok_r(from, delim, &sp)) != NULL; from = NULL) {
        if (g_num_hs_ssid == MAX_HS_SSIDS) {
            AnscTraceError(("%s max SSID reached\n", ssid));
            break;
        }

        hsssid = &g_hs_ssids[g_num_hs_ssid];

        snprintf(hsssid->path, sizeof(hsssid->path), "%s", tok);

        /* not using CDM API since this change will be merged to R1.5 */
        snprintf(param, sizeof(param), "%sSSID", tok);
        valStru.parameterName = param;
        valStru.parameterValue = hsssid->ssid;
        valSize = sizeof(hsssid->ssid);
        if (COSAGetParamValueByPathName(bus_handle, &valStru, &valSize) != ANSC_STATUS_SUCCESS) {
            AnscTraceError(("Fail to get DM: %s\n", param));
            continue;
        }
        hsssid->ncli = 0;
        g_num_hs_ssid++;

        for (i = 0; i < g_snstat->snooper_num_clients; i++) {
            if (hsssid->ncli == MAX_HS_CLIS) {
                AnscTraceError(("%s max client reached\n", ssid));
                break;
            }

            sncli = &g_snstat->snooper_clients[i];

            if (circuit_id_get_ssid(sncli->circuit_id, ssid, sizeof(ssid)) != 0) {
                AnscTraceError(("invalid circuit id: %s\n", sncli->circuit_id));
                continue;
            }

            if (strcmp(ssid, hsssid->ssid) == 0) {
                cli = &hsssid->clis[hsssid->ncli++];

                snprintf(cli->mac, sizeof(cli->mac), "%s", sncli->remote_id);
                snprintf(cli->hostname, sizeof(cli->hostname), "%s", sncli->hostname);
                cli->rssi = sncli->rssi;
                snprintf(cli->v4addr, sizeof(cli->v4addr), "%s", sncli->ipv4_addr);
                snprintf(cli->dh4_status, sizeof(cli->dh4_status), "%s", sncli->dhcp_status);
            }
        }
    }

    /*
    AnscTraceDebug((">>>>>> SSID List:\n"));
    for (i = 0; i < g_num_hs_ssid; i++) {
        AnscTraceDebug(("  [%d] SSID %s, Client Num %d\n",
                    i, g_hs_ssids[i].ssid, g_hs_ssids[i].ncli));
        hsssid = &g_hs_ssids[i];

        for (j = 0; j < hsssid->ncli; j++) {
            cli = &hsssid[i].clis[j];
            AnscTraceDebug(("  client[%d].mac %s\n", j, cli->mac));
            AnscTraceDebug(("  client[%d].hostname %s\n", j, cli->hostname));
            AnscTraceDebug(("  client[%d].v4addr %s\n", j, cli->v4addr));
        }
    }
    */

    return 0;
}

ANSC_STATUS
CosaDml_HotspotInit(void)
{
    int shmid;

    if ((shmid = shmget(kSnooper_Statistics, kSnooper_SharedMemSize, IPC_CREAT | 0666)) < 0) {
        AnscTraceError(("shmget: %s\n", strerror(errno)));
        return ANSC_STATUS_FAILURE;
    }

    if ((g_snstat = shmat(shmid, NULL, 0)) == (snooper_statistics_s *)-1) {
        AnscTraceError(("shmat: %s\n", strerror(errno)));
        g_snstat = NULL;
        return ANSC_STATUS_FAILURE;
    }

    AnscTraceDebug(("Init Hotspot Done\n"));

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_HotspotFinalize(void)
{
    return ANSC_STATUS_SUCCESS;
}

ULONG
CosaDml_HsSsidGetNumberOfEntries(void)
{
    if (load_hs_ssid() != 0) {
        AnscTraceError(("Fail to load HHS SSID list\n"));
        return 0;
    }

    return g_num_hs_ssid;
}

ANSC_STATUS
CosaDml_HsSsidGetEntryByIndex(ULONG idx, COSA_DML_HOTSPOT_SSID *entry)
{

    if (load_hs_ssid() != 0) {
        AnscTraceError(("Fail to load HHS SSID list\n"));
        return ANSC_STATUS_FAILURE;
    }

    if (idx >= (ULONG)g_num_hs_ssid) {
        AnscTraceError(("Invalid index\n"));
        return ANSC_STATUS_FAILURE;
    }

    entry->InstanceNumber = idx + 1;
    snprintf(entry->Alias, sizeof(entry->Alias), "cpe-HsSsid-%lu", idx + 1);
    snprintf(entry->SSID, sizeof(entry->SSID), "%s", g_hs_ssids[idx].path);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_HsSsidGetCfg(ULONG ins, COSA_DML_HOTSPOT_SSID *entry)
{
    return CosaDml_HsSsidGetEntryByIndex(ins - 1, entry);
}

ULONG
CosaDml_HsSsidAssoDevGetNumberOfEntries(ULONG ssidIns)
{
    struct hs_ssid *hsssid;

    if (load_hs_ssid() != 0) {
        AnscTraceError(("fail to loa hotspot ssid info\n"));
        return 0;
    }

    if (ssidIns > (ULONG)g_num_hs_ssid) {
        AnscTraceError(("ssid ins: %lu is not exist\n", ssidIns));
        return 0;
    }
    hsssid = &g_hs_ssids[ssidIns - 1];

    return hsssid->ncli;
}

ANSC_STATUS
CosaDml_HsSsidAssoDevGetEntryByIndex(ULONG ssidIns, ULONG idx, COSA_DML_HOTSPOT_ASSODEV *entry)
{
    struct hs_ssid *hsssid;
    struct hs_client *cli;

#if defined(_INTEL_BUG_FIXES_) || defined(DUAL_CORE_XB3)
    char curInt[256] = {0};
    int inst;
    ULONG size;
    int rssi = 0;
    char paramname[60];
    char outdata[80];
    parameterValStruct_t varStruct;
    varStruct.parameterName = paramname;
    varStruct.parameterValue = outdata;
#endif

    if (load_hs_ssid() != 0) {
        AnscTraceError(("fail to loa hotspot ssid info\n"));
        return ANSC_STATUS_FAILURE;
    }

    if (ssidIns > (ULONG)g_num_hs_ssid) {
        AnscTraceError(("ssid %lu not exist\n", ssidIns));
        return 0;
    }
    hsssid = &g_hs_ssids[ssidIns - 1];

    if (idx >= (ULONG)hsssid->ncli || !entry) {
        AnscTraceError(("bad parameter\n"));
        return ANSC_STATUS_FAILURE;
    }
    cli = &hsssid->clis[idx];

#if defined(_INTEL_BUG_FIXES_) || defined(DUAL_CORE_XB3)
    errno_t rc = -1;
    strncpy(curInt,hsssid->path,sizeof(curInt));
    //Trim off the trailing dot if it exists
    size = strnlen(curInt,sizeof(curInt));
    if (curInt[size - 1] == '.')
        curInt[size - 1] = '\0';
    inst = atoi(strrchr(curInt,'.')+1);

    size = sizeof(outdata);
    rc = sprintf_s(paramname, sizeof(paramname), "Device.WiFi.AccessPoint.%d.AssociatedDevice.%lu.SignalStrength", inst, idx+1);
    if(rc < EOK)  ERR_CHK(rc);
    if (ANSC_STATUS_SUCCESS == COSAGetParamValueByPathName(bus_handle, &varStruct, &size)) {
        rssi = atoi(varStruct.parameterValue);
    } else {
        rssi = cli->rssi;
    }
#endif

    entry->InstanceNumber = idx + 1;
    entry->SsidIns = ssidIns;
    snprintf(entry->Alias, sizeof(entry->Alias), "cpe-HsAssoDev-%lu", idx + 1);
    snprintf(entry->MACAddress, sizeof(entry->MACAddress), "%s", cli->mac);
    snprintf(entry->Hostname, sizeof(entry->Hostname), "%s", cli->hostname);
#if defined(_INTEL_BUG_FIXES_) || defined(DUAL_CORE_XB3)
    entry->RSSILevel = rssi;
#else
    entry->RSSILevel = cli->rssi;
#endif
    snprintf(entry->IPv4Address, sizeof(entry->IPv4Address), "%s", cli->v4addr);
    snprintf(entry->DHCPv4Status, sizeof(entry->DHCPv4Status), "%s", cli->dh4_status);
    snprintf(entry->IPv6Address, sizeof(entry->IPv6Address), "%s", cli->v6addr);
    snprintf(entry->IPv6Prefix, sizeof(entry->IPv6Prefix), "%s", cli->v6pref);
    snprintf(entry->DHCPv6Status, sizeof(entry->DHCPv6Status), "%s", cli->dh6_status);
    snprintf(entry->IPv6LinkLocalAddress, sizeof(entry->IPv6LinkLocalAddress), "%s", cli->v6lladdr);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_HsSsidAssoDevGetCfg(ULONG ssidIns, ULONG ins, COSA_DML_HOTSPOT_ASSODEV *entry)
{
    return CosaDml_HsSsidAssoDevGetEntryByIndex(ssidIns, ins - 1, entry);
}
#endif
