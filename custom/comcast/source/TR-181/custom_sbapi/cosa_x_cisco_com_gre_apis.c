/************************************************************************************
  If not stated otherwise in this file or this component's Licenses.txt file the
  following copyright and licenses apply:

  Copyright 2018 RDK Management

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
**************************************************************************/

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
/**************************************************************************

    module: cosa_x_cisco_com_gre_apis.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

    -------------------------------------------------------------------

    environment:

        platform independent

    -------------------------------------------------------------------

    author:

        leichen2@cisco.com

    -------------------------------------------------------------------

    revision:

        06/25/2013    initial revision.

**************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sysevent/sysevent.h>

#include "cosa_apis.h"
#include "cosa_x_cisco_com_gre_apis.h"
#include "ccsp_psm_helper.h"
#include "ansc_platform.h"
#include "plugin_main_apis.h"

#if 0

#ifdef AnscTraceWarning
#undef AnscTraceWarning
#define AnscTraceWarning(a) printf("%s:%d> ", __FUNCTION__, __LINE__); printf a
#endif

#ifdef AnscTraceError
#undef AnscTraceError
#define AnscTraceError(a) printf("%s:%d> ", __FUNCTION__, __LINE__); printf a
#endif

#endif

#define MAX_CGRE_PSM_REC        127

#define CGRE_OBJ_GREIF          "dmsb.cisco.gre."

#define CGRE_PARAM_ENABLE       CGRE_OBJ_GREIF "%lu.enable"
#define CGRE_PARAM_ALIAS        CGRE_OBJ_GREIF "%lu.alias"
#define CGRE_PARAM_NAME         CGRE_OBJ_GREIF "%lu.name"
#define CGRE_PARAM_LOWERLAYERS  CGRE_OBJ_GREIF "%lu.lowerlayers"
#define CGRE_PARAM_MODE         CGRE_OBJ_GREIF "%lu.mode"
#define CGRE_PARAM_ENDPOINT     CGRE_OBJ_GREIF "%lu.endpoint"
#define CGRE_PARAM_KEYMODE      CGRE_OBJ_GREIF "%lu.keymode"
#define CGRE_PARAM_KEY          CGRE_OBJ_GREIF "%lu.key"
#define CGRE_PARAM_CSUMEN       CGRE_OBJ_GREIF "%lu.csumenabled"
#define CGRE_PARAM_SEQEN        CGRE_OBJ_GREIF "%lu.seqnumenabled"
#define CGRE_PARAM_TOSMODE      CGRE_OBJ_GREIF "%lu.tosmode"
#define CGRE_PARAM_TOS          CGRE_OBJ_GREIF "%lu.tos"

#define WEB_CONF_ENABLE         "eRT.com.cisco.spvtg.ccsp.webpa.WebConfigRfcEnable"
#define WEB_CONF_FILE           "/nvram/hotspot_blob"

extern void* g_pDslhDmlAgent;

static int sysevent_fd;
static token_t sysevent_token;

static int if_get_stat(const char *ifname, int *up)
{
    int sockfd;
    struct ifreq ifr;

    if (!ifname || !up)
        return -1;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        return -1;
    }

    memset(&ifr, 0, sizeof(ifr));
    snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "%s", ifname);

    if (ioctl(sockfd, SIOCGIFFLAGS, &ifr) < 0) {
        perror("socket");
        close(sockfd);
        return -1;
    }

    close(sockfd);
    *up = ifr.ifr_flags & IFF_UP;
    return 0;
}

static COSA_DML_CGRE_STATUS 
CGre_GetStatus(const char *ifname, const char *lowlayers)
{
    char buf[1024];
    char *lowif, *sp;
    int up;

    if (if_get_stat(ifname, &up) != 0)
        return COSA_DML_CGRE_STATUS_ERROR;
    if (!up)
        return COSA_DML_CGRE_STATUS_DOWN;

    snprintf(buf, sizeof(buf), "%s", lowlayers);

    if ((lowif = strtok_r(buf, ",", &sp)) == NULL) /* no lower if */
        return COSA_DML_CGRE_STATUS_ERROR;
    if (if_get_stat(lowif, &up) != 0)
        return COSA_DML_CGRE_STATUS_ERROR;
    if (!up)
        return COSA_DML_CGRE_STATUS_LOWERLAYERDOWN;

    while ((lowif = strtok_r(NULL, ",", &sp)) != NULL) {
        if (if_get_stat(lowif, &up) != 0)
            return COSA_DML_CGRE_STATUS_ERROR;
        if (!up)
            return COSA_DML_CGRE_STATUS_LOWERLAYERDOWN;
    }

    return COSA_DML_CGRE_STATUS_UP;
}

static int CGre_PsmGetInsNums(const char *obj, unsigned int insList[], unsigned int *insCnt)
{
    unsigned int *list, cnt;
    CCSP_MESSAGE_BUS_INFO *businfo;

    if (PsmGetNextLevelInstances(g_MessageBusHandle, g_GetSubsystemPrefix(g_pDslhDmlAgent), 
                (char *)obj, &cnt, &list) != CCSP_SUCCESS)
        return -1;

    if (cnt > *insCnt) {
        businfo = g_MessageBusHandle;
        businfo->freefunc(list);
        return -1;
    }

    *insCnt = cnt;
    memcpy(insList, list, sizeof(unsigned int) * cnt);

    businfo = g_MessageBusHandle;
    businfo->freefunc(list);
    return 0;
}

static int
CGre_PsmGet(const char *param, char *value, int size)
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

static int
CGre_PsmSet(const char *param, const char *value)
{
    if (PSM_Set_Record_Value2(g_MessageBusHandle, g_GetSubsystemPrefix(g_pDslhDmlAgent),
                (char *)param, ccsp_string, (char *)value) != CCSP_SUCCESS)
        return -1;
    return 0;
}

static int
CGre_LoadConf(ULONG ins, COSA_DML_CGRE_IF *cfg)
{
    char rec[128], val[1025];

    //memset(cfg, 0, sizeof(COSA_DML_CGRE_IF));

    cfg->InstanceNumber = ins;

    snprintf(rec, sizeof(rec), CGRE_PARAM_ENABLE, ins);
    if (CGre_PsmGet(rec, val, sizeof(val)) != 0)
        return -1;
    cfg->Enable = atoi(val);

    snprintf(rec, sizeof(rec), CGRE_PARAM_ALIAS, ins);
    if (CGre_PsmGet(rec, cfg->Alias, sizeof(cfg->Alias)) != 0)
        return -1;

    snprintf(rec, sizeof(rec), CGRE_PARAM_NAME, ins);
    if (CGre_PsmGet(rec, cfg->Name, sizeof(cfg->Name)) != 0)
        return -1;

    snprintf(rec, sizeof(rec), CGRE_PARAM_LOWERLAYERS, ins);
    if (CGre_PsmGet(rec, cfg->LowerLayers, sizeof(cfg->LowerLayers)) != 0)
        return -1;

    snprintf(rec, sizeof(rec), CGRE_PARAM_MODE, ins);
    if (CGre_PsmGet(rec, val, sizeof(val)) != 0)
        return -1;
    cfg->Mode = atoi(val);

    snprintf(rec, sizeof(rec), CGRE_PARAM_ENDPOINT, ins);
    if (CGre_PsmGet(rec, cfg->RemoteEndpoint, sizeof(cfg->RemoteEndpoint)) != 0)
        return -1;

    snprintf(rec, sizeof(rec), CGRE_PARAM_KEYMODE, ins);
    if (CGre_PsmGet(rec, val, sizeof(val)) != 0)
        return -1;
    cfg->KeyMode = atoi(val);

    snprintf(rec, sizeof(rec), CGRE_PARAM_KEY, ins);
    if (CGre_PsmGet(rec, cfg->Key, sizeof(cfg->Key)) != 0)
        return -1;

    snprintf(rec, sizeof(rec), CGRE_PARAM_CSUMEN, ins);
    if (CGre_PsmGet(rec, val, sizeof(val)) != 0)
        return -1;
    cfg->ChecksumEnabled = atoi(val);

    snprintf(rec, sizeof(rec), CGRE_PARAM_SEQEN, ins);
    if (CGre_PsmGet(rec, val, sizeof(val)) != 0)
        return -1;
    cfg->SequenceNumberEnabled = atoi(val);

    snprintf(rec, sizeof(rec), CGRE_PARAM_TOSMODE, ins);
    if (CGre_PsmGet(rec, val, sizeof(val)) != 0)
        return -1;
    cfg->TOSMode = atoi(val);

    snprintf(rec, sizeof(rec), CGRE_PARAM_TOS, ins);
    if (CGre_PsmGet(rec, val, sizeof(val)) != 0)
        return -1;
    sscanf(val, "%x", (UINT*)&cfg->TOS);
    //cfg->TOS = atoi(val);

#if 0
    cfg->Status = CGre_GetStatus(cfg->Name, cfg->LowerLayers);

    /* We have to remember the setting as an workaround */
    cfg->LastChange = (ULONG)(time(NULL) - cfg->LastSet);
#endif

    return 0;
}

static int
CGre_SaveConf(ULONG ins, COSA_DML_CGRE_IF *cfg)
{
    char rec[128], val[1025];

    if (cfg->ChangeFlag & COSA_DML_CGRE_CF_ENABLE) {
    snprintf(rec, sizeof(rec), CGRE_PARAM_ENABLE, ins);
    snprintf(val, sizeof(val), "%d", cfg->Enable);
    if (CGre_PsmSet(rec, val) != 0)
        return -1;
    }

    if (cfg->ChangeFlag & COSA_DML_CGRE_CF_ALIAS) {
    snprintf(rec, sizeof(rec), CGRE_PARAM_ALIAS, ins);
    if (CGre_PsmSet(rec, cfg->Alias) != 0)
        return -1;
    }

    if (cfg->ChangeFlag & COSA_DML_CGRE_CF_NAME) {
    snprintf(rec, sizeof(rec), CGRE_PARAM_NAME, ins);
    if (CGre_PsmSet(rec, cfg->Name) != 0)
        return -1;
    }

    if (cfg->ChangeFlag & COSA_DML_CGRE_CF_LOWLAY) {
    snprintf(rec, sizeof(rec), CGRE_PARAM_LOWERLAYERS, ins);
    if (CGre_PsmSet(rec, cfg->LowerLayers) != 0)
        return -1;
    }

    if (cfg->ChangeFlag & COSA_DML_CGRE_CF_MODE) {
    snprintf(rec, sizeof(rec), CGRE_PARAM_MODE, ins);
    snprintf(val, sizeof(val), "%d", cfg->Mode);
    if (CGre_PsmSet(rec, val) != 0)
        return -1;
    }

    if (cfg->ChangeFlag & COSA_DML_CGRE_CF_EP) {
    snprintf(rec, sizeof(rec), CGRE_PARAM_ENDPOINT, ins);
    if (CGre_PsmSet(rec, cfg->RemoteEndpoint) != 0)
        return -1;
    }

    if (cfg->ChangeFlag & COSA_DML_CGRE_CF_KEYMOD) {
    snprintf(rec, sizeof(rec), CGRE_PARAM_KEYMODE, ins);
    snprintf(val, sizeof(val), "%d", cfg->KeyMode);
    if (CGre_PsmSet(rec, val) != 0)
        return -1;
    }

    if (cfg->ChangeFlag & COSA_DML_CGRE_CF_KEY) {
    snprintf(rec, sizeof(rec), CGRE_PARAM_KEY, ins);
    if (CGre_PsmSet(rec, cfg->Key) != 0)
        return -1;
    }

    if (cfg->ChangeFlag & COSA_DML_CGRE_CF_CSUMEN) {
    snprintf(rec, sizeof(rec), CGRE_PARAM_CSUMEN, ins);
    snprintf(val, sizeof(val), "%d", cfg->ChecksumEnabled);
    if (CGre_PsmSet(rec, val) != 0)
        return -1;
    }

    if (cfg->ChangeFlag & COSA_DML_CGRE_CF_SEQEN) {
    snprintf(rec, sizeof(rec), CGRE_PARAM_SEQEN, ins);
    snprintf(val, sizeof(val), "%d", cfg->SequenceNumberEnabled);
    if (CGre_PsmSet(rec, val) != 0)
        return -1;
    }

    if (cfg->ChangeFlag & COSA_DML_CGRE_CF_TOSMOD) {
    snprintf(rec, sizeof(rec), CGRE_PARAM_TOSMODE, ins);
    snprintf(val, sizeof(val), "%d", cfg->TOSMode);
    if (CGre_PsmSet(rec, val) != 0)
        return -1;
    }

    if (cfg->ChangeFlag & COSA_DML_CGRE_CF_TOS) {
    snprintf(rec, sizeof(rec), CGRE_PARAM_TOS, ins);
    snprintf(val, sizeof(val), "%x", (UINT)cfg->TOS);
    if (CGre_PsmSet(rec, val) != 0)
        return -1;
    }

    cfg->LastSet = time(NULL);

    return 0;
}

ANSC_STATUS 
CosaDml_CGreInit(void)
{
    sysevent_fd = sysevent_open("127.0.0.1", SE_SERVER_WELL_KNOWN_PORT, 
            SE_VERSION, "cisco-gre-dm", &sysevent_token);
    if (sysevent_fd < 0) {
        AnscTraceError(("sysevent_open failed\n"));
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS 
CosaDml_CGreFinalize(void)
{
    return ANSC_STATUS_SUCCESS;
}

ULONG 
CosaDml_CGreIfGetNumberOfEntries(void)
{
    unsigned int ifList[MAX_CGRE_IFS], ifCnt = MAX_CGRE_IFS;

    if (CGre_PsmGetInsNums(CGRE_OBJ_GREIF, ifList, &ifCnt) != 0)
        return 0;

    return ifCnt;
}

ANSC_STATUS
CosaDml_CGreIfGetEntryByIndex(ULONG idx, COSA_DML_CGRE_IF *greIf)
{
    unsigned int ifList[MAX_CGRE_IFS], ifCnt = MAX_CGRE_IFS;

    if (CGre_PsmGetInsNums(CGRE_OBJ_GREIF, ifList, &ifCnt) != 0)
        return ANSC_STATUS_FAILURE;

    if (idx >= ifCnt || !greIf)
        return ANSC_STATUS_FAILURE;

    if (CGre_LoadConf(ifList[idx], greIf) != 0)
        return ANSC_STATUS_FAILURE;

    greIf->LastSet = time(NULL);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_CGreIfSetInsAlias(ULONG idx, ULONG ins, const char *alias)
{
    UNREFERENCED_PARAMETER(idx);
    UNREFERENCED_PARAMETER(ins);
    UNREFERENCED_PARAMETER(alias);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_CGreIfGetCfg(ULONG ins, COSA_DML_CGRE_IF *greIf)
{
    if (CGre_LoadConf(ins, greIf) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_CGreIfGetStatus(const COSA_DML_CGRE_IF *greIf, 
        COSA_DML_CGRE_STATUS *status)
{
    if (!greIf || !status)
        return ANSC_STATUS_FAILURE;

    *status = CGre_GetStatus(greIf->Name, greIf->LowerLayers);
    return ANSC_STATUS_SUCCESS;
    }


ANSC_STATUS
CosaDml_CGreIfGetLastChange(const COSA_DML_CGRE_IF *greIf, 
        ULONG *lastChange)
{
    if (!greIf || !lastChange)
        return ANSC_STATUS_FAILURE;

    *lastChange = (ULONG)(time(NULL) - greIf->LastSet);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_CGreIfSetCfg(ULONG ins, COSA_DML_CGRE_IF *greIf)
{
    const char *evt = "gre-forceRestart";
    char val[64];
    char rec[128], old[16];

    FILE *fptr = NULL;

    /* 
     * "force restart"
     * 1. Enable is set to TRUE
     * 2. Enable was already TRUE
     */
    snprintf(rec, sizeof(rec), "%s",  WEB_CONF_ENABLE);
    if((CGre_PsmGet(rec, val, sizeof(val)) != 0)){
        AnscTraceError(("%s Failed to get psm value for Webconfig \n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    fptr = fopen(WEB_CONF_FILE, "r");
    
    if((strcmp(val, "true") == 0) && (NULL != fptr)){
        AnscTraceWarning(("%s: webconfig enabled...legacy hotspot cannot be used...\n", __FUNCTION__));
        fclose(fptr);
        return ANSC_STATUS_SUCCESS;
    }
    if(fptr != NULL)
        fclose(fptr);

    memset(rec, '\0', sizeof(rec));
    memset(val, '\0', sizeof(val));
    if ((greIf->ChangeFlag & COSA_DML_CGRE_CF_ENABLE) && greIf->Enable == TRUE) {
        snprintf(rec, sizeof(rec), CGRE_PARAM_ENABLE, ins);
        if (CGre_PsmGet(rec, old, sizeof(old)) == 0 && atoi(old) == 1)
            evt = "gre-forceRestart";
    }

    if (greIf->ChangeFlag) {
        if (CGre_SaveConf(ins, greIf) != 0)
            return ANSC_STATUS_FAILURE;
        greIf->ChangeFlag = 0;
    }

    snprintf(val, sizeof(val), "%lu", ins);
    if (sysevent_set(sysevent_fd, sysevent_token, (char *)evt, val, 0) != 0) {
        AnscTraceError(("Fail to set sysevent: %s %s\n", evt, val));
        return ANSC_STATUS_FAILURE;
    }

    fprintf(stderr, "Set sysevent: %s %s OK !\n", evt, val);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_CGreIfGetStats(ULONG ins, COSA_DML_IF_STATS *stats)
{
    char ifname[65];
    char rec[128];

    memset(stats, 0, sizeof(COSA_DML_IF_STATS));

    snprintf(rec, sizeof(rec), CGRE_PARAM_NAME, ins);
    if (CGre_PsmGet(rec, ifname, sizeof(ifname)) != 0)
        return ANSC_STATUS_SUCCESS;

    if (CosaUtilGetIfStats(ifname, stats) != 0)
        return ANSC_STATUS_SUCCESS;

    return ANSC_STATUS_SUCCESS;
}
#endif
