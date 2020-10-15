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

    module: cosa_ra_apis.c

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
        leichen2@cisco.com

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.
        23/08/2012    partial param (readonly) support for USGv2.
                      the GNU zebra is used for USGv2 RA, 
                      but we can only read the config now without modifyinig.

**************************************************************************/
#include "cosa_apis.h"
#include "cosa_ra_internal.h"
#include "cosa_ra_apis.h"
#include <ctype.h>
#include <utapi.h>
#include <utapi_util.h>
#include "utctx/utctx_api.h"

#define MAX_PREF        8       /* according to TR-181 */
#define ZEBRA_CONF      "/etc/zebra.conf"

#define RA_CONF_START   "# Based on prefix"
#define RA_CONF_IF      "interface"

typedef enum {
    RT_PREFER_HIGH = 1,
    RT_PREFER_MEDIUM,
    RT_PREFER_LOW,
} RtPrefer_t;

typedef struct RaPrefix_s {
    char    prefix[64];         /* XX::XX/M, including length */
    int     preLen;             /* prefix length */
    int     validLifetime;      /* -1 for infinite */
    int     preferLifetime;     /* -1 for infinite */
} RaPrefix_t;

typedef struct ZebraRaConf_s {
    /* struct ZebraRaConf_s  *next; */
    char            interface[17];
    RaPrefix_t      prefixes[MAX_PREF];
    int             preCnt;
    int             interval;
    int             lifetime;
	int             managedFlag;
	int             otherFlag;
    RtPrefer_t      preference;
} ZebraRaConf_t;

#ifndef DEBUG
#define DumpZebraRaConf(x)  
#else
static void DumpZebraRaConf(const ZebraRaConf_t *conf)
{
    int i;

    fprintf(stderr, "------- Zebra RA Config -------\n");
    fprintf(stderr, "%s %s\n", "Interface", conf->interface);
    for (i = 0; i < conf->preCnt; i++) {
        fprintf(stderr, "  prefix[%d]\n    %s length %d valid %d prefer %d\n", 
                i + 1, conf->prefixes[i].prefix, 
                conf->prefixes[i].preLen, 
                conf->prefixes[i].validLifetime, 
                conf->prefixes[i].preferLifetime);
        fprintf(stderr, "  interval %d\n", conf->interval);
        fprintf(stderr, "  lifetime %d\n", conf->lifetime);
        fprintf(stderr, "  preference %d\n", conf->preference);
    }

    return;
}
#endif

static void trim_leading_space(char *line)
{
    char *cp;
    int len;

    for (cp = line; *cp != '\0' && (*cp == ' ' || *cp == '\t'); cp++)
        ;

    if (cp != line) {
        len = strlen(line) - (cp - line);
        memmove(line, cp, len);
        line[len] = '\0';
    }
    return;
}

/*
 * # cat /etc/zebra.conf 
 * hostname zebra
 * password zebra
 * !enable password admin
 * !log stdout
 * log syslog
 * log file /var/log/zebra.log
 * # Based on prefix=2040:cafe:0:2::/64, old_previous=, LAN IPv6 address=
 * interface brlan0
 *    no ipv6 nd suppress-ra
 *    ipv6 nd prefix 2040:cafe:0:2::/64 300 300
 *    ipv6 nd ra-interval 60
 *    ipv6 nd ra-lifetime 180
 *    ipv6 nd router-preference medium
 *    ipv6 nd rdnss 2018:cafe::20c:29ff:fe97:fccc 300
 * interface brlan0
 *    ip irdp multicast
 * # 
 */
static int ParseZebraRaConf(ZebraRaConf_t *conf)
{
    FILE *fp;
    char line[256];
    RaPrefix_t *prefix;
    char sVal[2][64];

    memset(conf, 0, sizeof(ZebraRaConf_t));

    if ((fp = fopen(ZEBRA_CONF, "rb")) == NULL) {
        fprintf(stderr, "%s: fail to open file %s\n", __FUNCTION__, ZEBRA_CONF);
        return -1;
    }

    /* skip the configs before RA part */
    while (fgets(line, sizeof(line), fp) != NULL
            && strncmp(line, RA_CONF_START, strlen(RA_CONF_START)) != 0)
        continue;

    /* the first "interface" line */
    if (fgets(line, sizeof(line), fp) == NULL
            || sscanf(line, "interface %s", conf->interface) != 1)
        goto BAD_FORMAT;

    while (fgets(line, sizeof(line), fp) != NULL) {
        /* only support one interface for now */
        if (strncmp(line, RA_CONF_IF, strlen(RA_CONF_IF)) == 0)
            break;

        trim_leading_space(line);
        if (strstr(line, "prefix") != NULL) {
            if (conf->preCnt == MAX_PREF)
                continue;

            prefix = &conf->prefixes[conf->preCnt];

            if (sscanf(line, "ipv6 nd prefix %s %s %s", 
                        prefix->prefix, sVal[0], sVal[1]) != 3)
                goto BAD_FORMAT;

            if (strcmp(sVal[0], "infinite") == 0)
                prefix->validLifetime = -1;
            else
                prefix->validLifetime = atoi(sVal[0]);

            if (strcmp(sVal[1], "infinite") == 0)
                prefix->preferLifetime = -1;
            else
                prefix->preferLifetime = atoi(sVal[1]);

            if (sscanf(prefix->prefix, "%[:0-9a-fA-F]/%d", 
                        sVal[0], &prefix->preLen) != 2)
                goto BAD_FORMAT;

            conf->preCnt++;
        } else if (strstr(line, "ra-interval") != NULL) {
            if (sscanf(line, "ipv6 nd ra-interval %d", &conf->interval) != 1)
                goto BAD_FORMAT;
        } else if (strstr(line, "ra-lifetime") != NULL) {
            if (sscanf(line, "ipv6 nd ra-lifetime %d", &conf->lifetime) != 1)
                goto BAD_FORMAT;
		} else if (strstr(line, "managed-config-flag") != NULL) {
			conf->managedFlag = 1;
		} else if (strstr(line, "other-config-flag") != NULL) {
			conf->otherFlag = 1;
        } else if (strstr(line, "router-preference") != NULL) {
            if (sscanf(line, "ipv6 nd router-preference %s", sVal[0]) != 1)
                goto BAD_FORMAT;

            if (strcmp(sVal[0], "high") == 0)
                conf->preference = RT_PREFER_HIGH;
            else if (strcmp(sVal[0], "medium") == 0)
                conf->preference = RT_PREFER_MEDIUM;
            else if (strcmp(sVal[0], "low") == 0)
                conf->preference = RT_PREFER_LOW;
            else
                goto BAD_FORMAT;
        }
    }

    DumpZebraRaConf(conf);
    fclose(fp);
    return 0;

BAD_FORMAT:
    fprintf(stderr, "%s: bad format\n", __FUNCTION__);
    fclose(fp);
    return -1;
}

static int LoadRaInterface(PCOSA_DML_RA_IF_FULL raif)
{
    ZebraRaConf_t raConf;
    int left, i;
    char *fmt;
    
    if (ParseZebraRaConf(&raConf) != 0)
        return -1;

    memset(raif, 0, sizeof(COSA_DML_RA_IF_FULL));

    /* 
     * XXX: hard-coded the configs not support for now 
     */
    raif->Cfg.InstanceNumber        = 1;
    raif->Cfg.bEnabled              = TRUE;
    raif->Cfg.ManualPrefixes[0]     = '\0';
    raif->Cfg.MaxRtrAdvInterval     = raConf.interval * 2;
    raif->Cfg.MinRtrAdvInterval     = raConf.interval;
    raif->Cfg.AdvDefaultLifetime    = raConf.lifetime;
    raif->Cfg.bAdvManagedFlag       = raConf.managedFlag;
    raif->Cfg.bAdvOtherConfigFlag   = raConf.otherFlag;
    raif->Cfg.bAdvMobileAgentFlag   = FALSE;
    raif->Cfg.bAdvNDProxyFlag       = FALSE;
    raif->Cfg.AdvLinkMTU            = 0;
    raif->Cfg.AdvReachableTime      = 0;
    raif->Cfg.AdvRetransTimer       = 0;
    raif->Cfg.AdvCurHopLimit        = 0;
    snprintf(raif->Cfg.Alias, sizeof(raif->Cfg.Alias), "cpe-RA-Interface-1");
    snprintf(raif->Cfg.Interface, sizeof(raif->Cfg.Interface), "%s", raConf.interface);

    switch (raConf.preference) {
    case RT_PREFER_HIGH:
        raif->Cfg.AdvPreferredRouterFlag = COSA_DML_RA_PREFER_ROUTER_High;
        break;
    case RT_PREFER_MEDIUM:
        raif->Cfg.AdvPreferredRouterFlag = COSA_DML_RA_PREFER_ROUTER_Medium;
        break;
    case RT_PREFER_LOW:
    default:
        raif->Cfg.AdvPreferredRouterFlag = COSA_DML_RA_PREFER_ROUTER_Low;
        break;
    }

    raif->Info.Status = COSA_DML_RA_IF_STATUS_Enabled;

    raif->Info.Prefixes[0] = '\0';
    left = sizeof(raif->Info.Prefixes);
    for (i = 0; i < raConf.preCnt && left > 0; i++) {
        if (i == 0)
            fmt = "%s";
        else
            fmt = ",%s";

        left -= snprintf(raif->Info.Prefixes + strlen(raif->Info.Prefixes), 
                left, fmt, raConf.prefixes[i].prefix);
    }

    return 0;
}

ANSC_STATUS
CosaDmlRAInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    UNREFERENCED_PARAMETER(hDml);
    UNREFERENCED_PARAMETER(phContext);
    return ANSC_STATUS_SUCCESS;
}

ULONG 
CosaDmlRAGetEnabled
    (
        BOOLEAN * pEnabled
    )
{
    COSA_DML_RA_IF_FULL raif;

    if (!pEnabled)
        return ANSC_STATUS_FAILURE;

    if (LoadRaInterface(&raif) != 0)
        return ANSC_STATUS_FAILURE;

    *pEnabled = raif.Cfg.bEnabled;
    return ANSC_STATUS_SUCCESS;
}


ULONG 
CosaDmlRASetEnabled
    (
        BOOLEAN  bEnabled
    )
{
    UNREFERENCED_PARAMETER(bEnabled);
    fprintf(stderr, "%s: NOT SUPPORTED FOR NOW!!\n", __FUNCTION__);
    return ANSC_STATUS_FAILURE;
}

ULONG
CosaDmlRaIfGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    )
{
    UNREFERENCED_PARAMETER(hContext);
    return 1;
}

ANSC_STATUS
CosaDmlRaIfGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_RA_IF_FULL        pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    /* only one instance */
    if (ulIndex != 0 || !pEntry)
        return ANSC_STATUS_FAILURE;

    if (LoadRaInterface(pEntry) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlRaIfSetValues
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
    /* let it return success , even we don't support it */ 
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlRaIfAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RA_IF_FULL        pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pEntry);
    fprintf(stderr, "%s: NOT SUPPORTED FOR NOW!!\n", __FUNCTION__);
    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDmlRaIfDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(ulInstanceNumber);
    fprintf(stderr, "%s: NOT SUPPORTED FOR NOW!!\n", __FUNCTION__);
    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDmlRaIfSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RA_IF_CFG         pCfg
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UtopiaContext utctx = {0};
    char out[8] = {0};
	unsigned int  managedFlag = 0;
	unsigned int  otherFlag   = 0;
	
    fprintf(stderr, "%s: Only support O/M flags. NOT SUPPORTED other flags FOR NOW!!\n", __FUNCTION__);
	
	if (Utopia_Init(&utctx))
	{
		Utopia_RawGet(&utctx,NULL,"router_managed_flag",out,sizeof(out));
		if ( out[0] == '1' )
			managedFlag = 1;

		out[0] = 0;
		Utopia_RawGet(&utctx,NULL,"router_other_flag",out,sizeof(out));
		if ( out[0] == '1' )
			otherFlag = 1;

		if ( ( !(pCfg->bAdvManagedFlag)     == !managedFlag ) && 
			 ( !(pCfg->bAdvOtherConfigFlag) == !otherFlag   ) )
                {
                    Utopia_Free(&utctx,0);
                    return ANSC_STATUS_FAILURE;
                }

		out[0] = pCfg->bAdvManagedFlag?'1':'0';
		Utopia_RawSet(&utctx,NULL,"router_managed_flag",out);
		
		out[0] = pCfg->bAdvOtherConfigFlag?'1':'0';
		Utopia_RawSet(&utctx,NULL,"router_other_flag",out);

		Utopia_Free(&utctx,1);
		
		system("sysevent set zebra-restart");
		
	    return ANSC_STATUS_SUCCESS;

	}
	
    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDmlRaIfGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RA_IF_CFG         pCfg
    )
{
    UNREFERENCED_PARAMETER(hContext);
    COSA_DML_RA_IF_FULL             raif;

    if (!pCfg)
        return ANSC_STATUS_FAILURE;

    if (LoadRaInterface(&raif) != 0)
        return ANSC_STATUS_FAILURE;

    memcpy(pCfg, &raif.Cfg, sizeof(COSA_DML_RA_IF_CFG));
    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlRaIfGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_RA_IF_INFO        pInfo
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(ulInstanceNumber);
    COSA_DML_RA_IF_FULL             raif;

    if (!pInfo)
        return ANSC_STATUS_FAILURE;

    if (LoadRaInterface(&raif) != 0)
        return ANSC_STATUS_FAILURE;

    memcpy(pInfo, &raif.Info, sizeof(COSA_DML_RA_IF_INFO));
    return ANSC_STATUS_SUCCESS;
}

ULONG
CosaDmlRaIfGetNumberOfOptions
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulRaIfInstanceNumber
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(ulRaIfInstanceNumber);
    return 0;
}

ANSC_STATUS
CosaDmlRaIfGetOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulRaIfInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_RA_OPTION         pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(ulRaIfInstanceNumber);
    UNREFERENCED_PARAMETER(ulIndex);
    UNREFERENCED_PARAMETER(pEntry);
    fprintf(stderr, "%s: NOT SUPPORTED FOR NOW!!\n", __FUNCTION__);
    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDmlRaIfSetOptionValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulRaIfInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(ulRaIfInstanceNumber);
    UNREFERENCED_PARAMETER(ulIndex);
    UNREFERENCED_PARAMETER(ulInstanceNumber);
    UNREFERENCED_PARAMETER(pAlias);
    fprintf(stderr, "%s: NOT SUPPORTED FOR NOW!!\n", __FUNCTION__);
    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDmlRaIfAddOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulRaIfInstanceNumber,
        PCOSA_DML_RA_OPTION         pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(ulRaIfInstanceNumber);
    UNREFERENCED_PARAMETER(pEntry);
    fprintf(stderr, "%s: NOT SUPPORTED FOR NOW!!\n", __FUNCTION__);
    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDmlRaIfDelOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulRaIfInstanceNumber,
        PCOSA_DML_RA_OPTION         pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(ulRaIfInstanceNumber);
    UNREFERENCED_PARAMETER(pEntry);
    fprintf(stderr, "%s: NOT SUPPORTED FOR NOW!!\n", __FUNCTION__);
    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDmlRaIfSetOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulRaIfInstanceNumber,
        PCOSA_DML_RA_OPTION         pEntry
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(ulRaIfInstanceNumber);
    UNREFERENCED_PARAMETER(pEntry);
    fprintf(stderr, "%s: NOT SUPPORTED FOR NOW!!\n", __FUNCTION__);
    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDmlRaIfGetOption2
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulRaIfInstanceNumber,
        PCOSA_DML_RA_OPTION         pEntry         
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(ulRaIfInstanceNumber);
    UNREFERENCED_PARAMETER(pEntry);
    fprintf(stderr, "%s: NOT SUPPORTED FOR NOW!!\n", __FUNCTION__);
    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDmlRaIfBERemove
    (
        void
    )
{
    fprintf(stderr, "%s: NOT SUPPORTED FOR NOW!!\n", __FUNCTION__);
    return ANSC_STATUS_FAILURE;
}
