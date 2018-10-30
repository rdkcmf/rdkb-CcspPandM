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

    module: cosa_ipv6rd_apis.c

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

        08/30/2011    initial revision.

**************************************************************************/
#if (defined(_COSA_INTEL_USG_ARM_) || defined(_COSA_DRG_TPG_))
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/if.h>
#include <linux/ip.h>

#include "cosa_apis.h"
#include "syscfg/syscfg.h"
#include "cosa_ipv6rd_apis.h"

#define MAX_LINE                1024

#define TR_6RD_ENABLE           "Device.IPv6rd.Enable"

#define TR_6RD_IF_PRE           "Device.IPv6rd.InterfaceSetting"

#define IFP_ENABLE              "Enable"
#define IFP_ALIAS               "Alias"
#define IFP_BRIPV4ADDRS         "BorderRelayIPv4Addresses"
#define IFP_ALLTRTOBR           "AllTrafficToBorderRelay"
#define IFP_SPV6PREFIX          "SPIPv6Prefix"
#define IFP_IPV4MASKLEN         "IPv4MaskLength"
#define IFP_ADDRSOURCE          "AddressSource"

struct ipv6rd_conf {
    BOOL                        enable;
    int                         ifnum;
    COSA_DML_IPV6RD_IF          ifconfs[MAX_6RDIF_INS];
};

static struct ipv6rd_conf *g_ipv6rd_conf;

#if (defined(_COSA_INTEL_USG_ARM_) || defined(_COSA_DRG_TPG_))
/* 
 * define USE_SYSTEM to use system() function
 */
#define USE_SYSTEM
#endif

/**************************************************************************
 * 6rd DML functions
**************************************************************************/

static int 
syscfg_get_bool(const char *key, BOOL *bvalue)
{
    char value[MAX_LINE];

    bzero(value, sizeof(value));
    if (syscfg_get(NULL, key, value, sizeof(value)) != 0)
        return -1;

    if (atoi(value) == 1) {
        *bvalue = TRUE;
    } else {
        *bvalue = FALSE;
    }

    return 0;
}

static int
syscfg_set_bool(const char *key, BOOL bvalue)
{
    char value[MAX_LINE];

    snprintf(value, sizeof(value), "%d", bvalue ? 1 : 0);
    if (syscfg_set(NULL, key, value) != 0)
        return -1;

    return 0;
}

static int 
syscfg_get_ulong(const char *key, ULONG *ulvalue)
{
    char value[MAX_LINE];

    bzero(value, sizeof(value));
    if (syscfg_get(NULL, key, value, sizeof(value)) != 0)
        return -1;

    *ulvalue = (ULONG)atoi(value);
    return 0;
}

static int
syscfg_set_ulong(const char *key, ULONG ulvalue)
{
    char value[MAX_LINE];

    snprintf(value, sizeof(value), "%u", ulvalue);
    if (syscfg_set(NULL, key, value) != 0)
        return -1;

    return 0;
}

static int 
syscfg_get_string(const char *key, char *buf, unsigned int size)
{
    return syscfg_get(NULL, key, buf, size);
}

static int
syscfg_set_string(const char *key, const char *svalue)
{
    return syscfg_set(NULL, key, svalue);
}

static struct ipv6rd_conf *
load_ipv6rd_conf(void)
{
    struct ipv6rd_conf *conf = NULL;
    int len, ins, i;
    COSA_DML_IPV6RD_IF  *ifconf;
    char key[MAX_LINE];

    if ((conf = AnscAllocateMemory(sizeof(struct ipv6rd_conf))) == NULL)
        return NULL;

    bzero(conf, sizeof(struct ipv6rd_conf));
    if (syscfg_get_bool(TR_6RD_ENABLE, &conf->enable) != 0) {
        syslog(LOG_ERR, "%s: no 6RD info in config file", __FUNCTION__);
        // give a chance to set the config
        conf->enable = FALSE;
        //return conf;
        //return NULL;
    }

    conf->ifnum = 0; /* set to zero first */

    /* NOTE: instance number is counting from 1 */
    for (ins = 1; ins < MAX_6RDIF_INS - 1; ins++) {
        ifconf = &conf->ifconfs[conf->ifnum];

        /* not all instance with num "ins" is exist, 
         * TRY to find the instance{ins} in config file first */
        snprintf(key, sizeof(key), "%s.%d.%s", TR_6RD_IF_PRE, ins, IFP_ENABLE);
        if (syscfg_get_bool(key, &ifconf->Enable) != 0)
            continue; // instance{ins} is not exist

        /* record we found a instance */
        ifconf->InstanceNumber = ins;
        conf->ifnum++;

        /* now get other parameters */
        snprintf(key, sizeof(key), "%s.%d.%s", TR_6RD_IF_PRE, ins, IFP_ALIAS);
        if (syscfg_get_string(key, ifconf->Alias, sizeof(ifconf->Alias)) != 0)
            goto errout;

        snprintf(key, sizeof(key), "%s.%d.%s", TR_6RD_IF_PRE, ins, IFP_BRIPV4ADDRS);
        if (syscfg_get_string(key, ifconf->BorderRelayIPv4Addr, sizeof(ifconf->BorderRelayIPv4Addr)) != 0)
            goto errout;

        snprintf(key, sizeof(key), "%s.%d.%s", TR_6RD_IF_PRE, ins, IFP_ALLTRTOBR);
        if (syscfg_get_bool(key, &ifconf->AllTrafficToBorderRelay) != 0)
            goto errout;

        snprintf(key, sizeof(key), "%s.%d.%s", TR_6RD_IF_PRE, ins, IFP_SPV6PREFIX);
        if (syscfg_get_string(key, ifconf->SPIPv6Prefix, sizeof(ifconf->SPIPv6Prefix)) != 0)
            goto errout;

        snprintf(key, sizeof(key), "%s.%d.%s", TR_6RD_IF_PRE, ins, IFP_IPV4MASKLEN);
        if (syscfg_get_ulong(key, &ifconf->IPv4MaskLength) != 0)
            goto errout;

        snprintf(key, sizeof(key), "%s.%d.%s", TR_6RD_IF_PRE, ins, IFP_ADDRSOURCE);
        if (syscfg_get_string(key, ifconf->AddressSource, sizeof(ifconf->AddressSource)) != 0)
            goto errout;
    }

    /* load success */
    return conf;

errout:
    if (conf) {
        AnscFreeMemory(conf);
    }

    return NULL;
}

static int 
unload_ipv6rd_conf(struct ipv6rd_conf *conf)
{
    int i;

    if (!conf)
        return -1;

    AnscFreeMemory(conf);
    return 0;
}

static COSA_DML_IPV6RD_IF *
get_ifconf_by_insnum(int ins)
{
    int i;

    if (!g_ipv6rd_conf) {
        syslog(LOG_ERR, "%s: not initialized", __FUNCTION__);
        return NULL;
    }

    for (i = 0; i < g_ipv6rd_conf->ifnum; i++) {
        if (g_ipv6rd_conf->ifconfs[i].InstanceNumber == ins)
            return &g_ipv6rd_conf->ifconfs[i];
    }

    return NULL;
}

static COSA_DML_IPV6RD_IF *
get_ifconf_by_alias(const char *alias)
{
    int i;

    if (!g_ipv6rd_conf) {
        syslog(LOG_ERR, "%s: not initialized", __FUNCTION__);
        return NULL;
    }

    for (i = 0; i < g_ipv6rd_conf->ifnum; i++) {
        if (strcmp(g_ipv6rd_conf->ifconfs[i].Alias, alias) == 0)
            return &g_ipv6rd_conf->ifconfs[i];
    }

    return NULL;
}

static int
save_ifconf(COSA_DML_IPV6RD_IF *new)
{
    char key[MAX_LINE];
    int ins = new->InstanceNumber;
    COSA_DML_IPV6RD_IF *conf;

    snprintf(key, sizeof(key), "%s.%d.%s", TR_6RD_IF_PRE, ins, IFP_ENABLE);
    if (syscfg_set_bool(key, new->Enable) != 0)
        return -1;

    snprintf(key, sizeof(key), "%s.%d.%s", TR_6RD_IF_PRE, ins, IFP_ALIAS);
    if (syscfg_set_string(key, new->Alias) != 0)
        return -1;

    snprintf(key, sizeof(key), "%s.%d.%s", TR_6RD_IF_PRE, ins, IFP_BRIPV4ADDRS);
    if (syscfg_set_string(key, new->BorderRelayIPv4Addr) != 0)
        return -1;

    snprintf(key, sizeof(key), "%s.%d.%s", TR_6RD_IF_PRE, ins, IFP_ALLTRTOBR);
    if (syscfg_set_bool(key, new->AllTrafficToBorderRelay) != 0)
        return -1;

    snprintf(key, sizeof(key), "%s.%d.%s", TR_6RD_IF_PRE, ins, IFP_SPV6PREFIX);
    if (syscfg_set_string(key, new->SPIPv6Prefix) != 0)
        return -1;

    snprintf(key, sizeof(key), "%s.%d.%s", TR_6RD_IF_PRE, ins, IFP_IPV4MASKLEN);
    if (syscfg_set_ulong(key, new->IPv4MaskLength) != 0)
        return -1;

    snprintf(key, sizeof(key), "%s.%d.%s", TR_6RD_IF_PRE, ins, IFP_ADDRSOURCE);
    if (syscfg_set_string(key, new->AddressSource) != 0)
        return -1;

    if (syscfg_commit() != 0)
        return -1;

    /* not all field need to save */
    if ((conf = get_ifconf_by_insnum(ins)) != NULL) {
        conf->Enable = new->Enable;
        snprintf(conf->Alias, sizeof(conf->Alias), "%s", new->Alias);
        snprintf(conf->BorderRelayIPv4Addr,
                sizeof(conf->BorderRelayIPv4Addr), "%s", new->BorderRelayIPv4Addr);
        conf->AllTrafficToBorderRelay = new->AllTrafficToBorderRelay;
        snprintf(conf->SPIPv6Prefix, sizeof(conf->SPIPv6Prefix), "%s", new->SPIPv6Prefix);
        conf->IPv4MaskLength = new->IPv4MaskLength;
        snprintf(conf->AddressSource, sizeof(conf->AddressSource), "%s", new->AddressSource);
    } else { /* add to memory struct */
        if (g_ipv6rd_conf->ifnum + 1 > MAX_6RDIF_INS)
            return -1;
        g_ipv6rd_conf->ifconfs[g_ipv6rd_conf->ifnum++] = *new;
    }

    return 0;
}

static int
delete_ifconf(int ins)
{
    int i;
    COSA_DML_IPV6RD_IF *ifconf;
    char key[MAX_LINE];

    for (i = 0; i < g_ipv6rd_conf->ifnum; i++) {
        if (g_ipv6rd_conf->ifconfs[i].InstanceNumber == ins) {
            ifconf = &g_ipv6rd_conf->ifconfs[i];
            break;
        }
    }
    if (i == g_ipv6rd_conf->ifnum) {
        syslog(LOG_ERR, "%s: instance not exist", __FUNCTION__);
        return -1;
    }

    memmove(&g_ipv6rd_conf->ifconfs[i], &g_ipv6rd_conf->ifconfs[i+1], 
            (g_ipv6rd_conf->ifnum - i - 1) * sizeof(COSA_DML_IPV6RD_IF));
    g_ipv6rd_conf->ifnum--;

    snprintf(key, sizeof(key), "%s.%d.%s", TR_6RD_IF_PRE, ins, IFP_ENABLE);
    if (syscfg_unset(NULL, key) != 0) {
        syslog(LOG_ERR, "%s: syscfg_unset %s", __FUNCTION__, key);
        return -1;
    }

    snprintf(key, sizeof(key), "%s.%d.%s", TR_6RD_IF_PRE, ins, IFP_ALIAS);
    if (syscfg_unset(NULL, key) != 0) {
        syslog(LOG_ERR, "%s: syscfg_unset %s", __FUNCTION__, key);
        return -1;
    }

    snprintf(key, sizeof(key), "%s.%d.%s", TR_6RD_IF_PRE, ins, IFP_BRIPV4ADDRS);
    if (syscfg_unset(NULL, key) != 0) {
        syslog(LOG_ERR, "%s: syscfg_unset %s", __FUNCTION__, key);
        return -1;
    }

    snprintf(key, sizeof(key), "%s.%d.%s", TR_6RD_IF_PRE, ins, IFP_ALLTRTOBR);
    if (syscfg_unset(NULL, key) != 0) {
        syslog(LOG_ERR, "%s: syscfg_unset %s", __FUNCTION__, key);
        return -1;
    }

    snprintf(key, sizeof(key), "%s.%d.%s", TR_6RD_IF_PRE, ins, IFP_SPV6PREFIX);
    if (syscfg_unset(NULL, key) != 0) {
        syslog(LOG_ERR, "%s: syscfg_unset %s", __FUNCTION__, key);
        return -1;
    }

    snprintf(key, sizeof(key), "%s.%d.%s", TR_6RD_IF_PRE, ins, IFP_IPV4MASKLEN);
    if (syscfg_unset(NULL, key) != 0) {
        syslog(LOG_ERR, "%s: syscfg_unset %s", __FUNCTION__, key);
        return -1;
    }

    snprintf(key, sizeof(key), "%s.%d.%s", TR_6RD_IF_PRE, ins, IFP_ADDRSOURCE);
    if (syscfg_unset(NULL, key) != 0) {
        syslog(LOG_ERR, "%s: syscfg_unset %s", __FUNCTION__, key);
        return -1;
    }

    if (syscfg_commit() != 0) {
        syslog(LOG_ERR, "%s: syscfg_commit error", __FUNCTION__);
        return -1;
    }

    return 0;
}

static int
extract_first_addr(const char *addrlist, char *buf, unsigned int size)
{
    char *end;
    int len;

    if (addrlist[0] == '\0')
        return -1; // no address

    bzero(buf, size);

    /* strtok() will modify original data */
    if ((end = strchr(addrlist, ',')) != NULL) {
        len = end - addrlist;
    } else {
        len = strlen(addrlist);
    }

    if (len > size - 1)
        return -1;
    memcpy(buf, addrlist, len);

    return 0;
}

static int 
ifname_to_ipaddr(const char *ifname, char *addr, unsigned int size)
{
	struct ifreq ifr;
	int sockfd;
	struct sockaddr_in *sin;

	bzero(&ifr, sizeof(ifr));
	strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name) - 1);

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		return -1;

	if (ioctl(sockfd, SIOCGIFADDR, &ifr) < 0) {
		close(sockfd);
		return -1;
	}

	close(sockfd);

	sin = (struct sockaddr_in *)&ifr.ifr_addr;
	if (inet_ntop(AF_INET, &sin->sin_addr, addr, size) == NULL)
		return -1;

	return 0;
}

static int
generate_6rd_delegated_prefix(
        struct in6_addr *v6rdpref,
        unsigned int v6rdmasklen,
        struct in_addr *v4addr,
        unsigned int v4masklen,
        struct in6_addr *out)
{
    int o, b, m, n;
    unsigned int uiv4addr;
    unsigned char ucv4addr[4];

    bzero(out, sizeof(struct in6_addr));

    if (v4masklen > 32 || v6rdmasklen + 32 - v4masklen > 64)
        return -1;

    o = v6rdmasklen >> 3;
    b = v6rdmasklen & 0x7;
    uiv4addr = ntohl((unsigned int)v4addr->s_addr);
    uiv4addr <<= v4masklen;

    ucv4addr[0] = (uiv4addr >> 24) & 0xff;
    ucv4addr[1] = (uiv4addr >> 16) & 0xff;
    ucv4addr[2] = (uiv4addr >> 8) & 0xff;
    ucv4addr[3] = uiv4addr & 0xff;

    memcpy(out->s6_addr, v6rdpref->s6_addr, o);
    if (b != 0) {
        out->s6_addr[o] = (v6rdpref->s6_addr[o] & (0xff00 >> b)) \
                            | (ucv4addr[0] >> b);
        o++;
    }

    if (32 - v4masklen - b > 0) {
        m = (32 - v4masklen - b) >> 3;
        n = (32 - v4masklen - b) & 0x7;

        uiv4addr <<= b;

        ucv4addr[0] = (uiv4addr >> 24) & 0xff;
        ucv4addr[1] = (uiv4addr >> 16) & 0xff;
        ucv4addr[2] = (uiv4addr >> 8) & 0xff;
        ucv4addr[3] = uiv4addr & 0xff;

        memcpy(&out->s6_addr[o], ucv4addr, m);
        if (n != 0) {
            out->s6_addr[o + m] = ucv4addr[m] & (0xff00 >> n);
        }
    }

    return 0;
}

static int 
IPv6rd_TunnelAdd(const COSA_DML_IPV6RD_IF *ifconf)
{
    struct in6_addr v6rdpref;
    unsigned int v6rdmasklen;
    struct in_addr v4addr;
    unsigned int v4masklen;
    struct in6_addr v6rdaddr;
    char buf[64];
    char *ptr;
    char addrsource[16];
#if defined(USE_SYSTEM)
    char cmd[MAX_LINE];
#endif

    /* if AddressSource is not specified, use wan0's ipv4 address (which is from ISP) */
    snprintf(addrsource, sizeof(addrsource), "%s", ifconf->AddressSource);
    if (addrsource[0] == '\0') {
        if (ifname_to_ipaddr("wan0", addrsource, sizeof(addrsource)) != 0) {
            syslog(LOG_ERR, "%s: Local address can't determined", __FUNCTION__);
            return -1;
        }
    }

    /* create 6rd tunnel and set 6rd-prefix */
#if defined(USE_SYSTEM)
    if (extract_first_addr(ifconf->BorderRelayIPv4Addr, buf, sizeof(buf)) == 0) {
        snprintf(cmd, sizeof(cmd), "ip tunnel add %s mode sit local %s remote %s", 
                ifconf->Alias, addrsource, buf);
    } else {
        snprintf(cmd, sizeof(cmd), "ip tunnel add %s mode sit local %s", 
                ifconf->Alias, addrsource);
    }
    if (system(cmd) != 0) {
        syslog(LOG_ERR, "%s: Fail to add tunnel: %s", __FUNCTION__, ifconf->Alias);
        return -1;
    }

    snprintf(cmd, sizeof(cmd), "ip tunnel 6rd dev %s 6rd-prefix %s", 
            ifconf->Alias, ifconf->SPIPv6Prefix);
    if (system(cmd) != 0) {
        syslog(LOG_ERR, "%s: Fail to set prefix for %s", __FUNCTION__, ifconf->Alias);
        return -1;
    }
#endif

    /* to generate 6rd address for 6rd tunnel */
	snprintf(buf, sizeof(buf), "%s", ifconf->SPIPv6Prefix);
	if ((ptr = strchr(buf, '/')) == NULL) {
		syslog(LOG_ERR, "%s: invalid SPIPv6Prefix", __FUNCTION__);
		return -1;
	}
	*ptr++ = '\0';
	v6rdmasklen = atoi(ptr);

	if (inet_pton(AF_INET6, buf, &v6rdpref) <= 0) {
		syslog(LOG_ERR, "%s: invalid SPIPv6Prefix", __FUNCTION__);
		return -1;
	}

	if (inet_pton(AF_INET, addrsource, &v4addr) <= 0) {
		syslog(LOG_ERR, "%s: invalid source IPv4 address", __FUNCTION__);
		return -1;
	}
	v4masklen = ifconf->IPv4MaskLength;

	if (generate_6rd_delegated_prefix(
			&v6rdpref,
			v6rdmasklen,
			&v4addr,
			v4masklen,
			&v6rdaddr) != 0) {
		syslog(LOG_ERR, "%s: Fail to generate 6rd address", __FUNCTION__);
		return -1;
	}

	/* set 6rd delegated prefix as a valid address */
	v6rdaddr.s6_addr32[3] = htonl(0x01);

	if (inet_ntop(AF_INET6, &v6rdaddr, buf, sizeof(buf)) == NULL) {
		syslog(LOG_ERR, "%s: Fail to generate 6rd address", __FUNCTION__);
		return -1;
	}
	snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), "/%d", v6rdmasklen);

    /* add 6rd interface's address and set tunnel interface up */
#if defined(USE_SYSTEM)
    snprintf(cmd, sizeof(cmd), "ip addr add %s dev %s", buf, ifconf->Alias);
    if (system(cmd) != 0) {
        syslog(LOG_ERR, "%s: Fail to generate 6rd address", __FUNCTION__);
        return -1;
    }

    snprintf(cmd, sizeof(cmd), "ip link set %s up", ifconf->Alias);
    if (system(cmd) != 0) {
        syslog(LOG_ERR, "%s: Fail to set %s up", __FUNCTION__, ifconf->Alias);
        return -1;
    }
#endif

    /* add default route if need */
    if (extract_first_addr(ifconf->BorderRelayIPv4Addr, buf, sizeof(buf)) == 0) {
#if defined(USE_SYSTEM)
        //snprintf(cmd, sizeof(cmd), "ip route add ::/0 via ::%s dev %s", buf, ifconf->Alias);
        snprintf(cmd, sizeof(cmd), "ip route add ::/0 dev %s", ifconf->Alias);
        if (system(cmd) != 0) {
            syslog(LOG_ERR, "%s: Fail to set default route", __FUNCTION__, ifconf->Alias);
            return -1;
        }
#endif
    }

    /* if all traffic to some border relay delete direct route */
    if (ifconf->AllTrafficToBorderRelay) {
#if defined(USE_SYSTEM)
        snprintf(cmd, sizeof(cmd), "ip route del %s dev %s", ifconf->SPIPv6Prefix, ifconf->Alias);
        system(cmd);
#endif
        /* failure is acceptable */
    }

    return 0;
}

static int 
IPv6rd_TunnelDel(const COSA_DML_IPV6RD_IF *ifconf)
{
    char braddr[16];
#if defined(USE_SYSTEM)
    char cmd[MAX_LINE];
#endif

    /* if default IPv6 routing is to of this IF's Border Relay,
     * we also need to delete it. */
    if (extract_first_addr(ifconf->BorderRelayIPv4Addr, braddr, sizeof(braddr)) == 0) {
        // XXX: don't delete it has any problem ?
    }

#if defined(USE_SYSTEM)
    snprintf(cmd, sizeof(cmd), "ip link set %s down", ifconf->Alias);
    if (system(cmd) != 0)
        syslog(LOG_ERR, "%s: fail to set link down", __FUNCTION__);
#endif

    /* delete tunnel interface and it's direct routing */
#if defined(USE_SYSTEM)
    snprintf(cmd, sizeof(cmd), "ip tunnel del %s", ifconf->Alias);
    if (system(cmd) != 0) {
        syslog(LOG_ERR, "%s: fail to delete tunnel", __FUNCTION__);
        return -1;
    }
#endif

    return 0;
}

ANSC_STATUS
CosaDml_IPv6rdInit(
        ANSC_HANDLE     hDml,
        PANSC_HANDLE    phContext
        )
{
    int i;
    COSA_DML_IPV6RD_IF *ifconf;

    if (g_ipv6rd_conf) {
        syslog(LOG_ERR, "%s: already initialized", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    if (syscfg_init() != 0) {
        syslog(LOG_ERR, "%s: syscfg_init error", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    /* load config from config file (PSM for coas2.0) */
    if ((g_ipv6rd_conf = load_ipv6rd_conf()) == NULL) {
        syslog(LOG_ERR, "%s: load config error", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    /* do setting */
    if (g_ipv6rd_conf->enable) {
        for (i = 0; i < g_ipv6rd_conf->ifnum; i++) {
            ifconf = &g_ipv6rd_conf->ifconfs[i];

            if (ifconf->Enable) {
                if (IPv6rd_TunnelAdd(ifconf) != 0) {
                    syslog(LOG_ERR, "%s: fail to add %s", __FUNCTION__, ifconf->Alias);
                    continue;
                }
            }
        }
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_IPv6rdFinalize(
        ANSC_HANDLE     hDml,
        PANSC_HANDLE    phContext
        )
{
    int i;
    COSA_DML_IPV6RD_IF *ifconf;

    if (!g_ipv6rd_conf) {
        syslog(LOG_ERR, "%s: not initialized", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    if (g_ipv6rd_conf->enable) {
        for (i = 0; i < g_ipv6rd_conf->ifnum; i++) {
            ifconf = &g_ipv6rd_conf->ifconfs[i];

            if (ifconf->Enable) {
                if(IPv6rd_TunnelDel(ifconf) != 0) {
                    syslog(LOG_ERR, "%s: fail to delete %s", __FUNCTION__, ifconf->Alias);
                    continue;
                }
            }
        }
    }

    if (unload_ipv6rd_conf(g_ipv6rd_conf) != 0)
        syslog(LOG_ERR, "%s: unload config error", __FUNCTION__);
    g_ipv6rd_conf = NULL;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_IPv6rdGetEnable(
        ANSC_HANDLE     hDml,
        PANSC_HANDLE    phContext,
        BOOL            *bEnable
        )
{
    if (!g_ipv6rd_conf) {
        syslog(LOG_ERR, "%s: not initialized", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    *bEnable = g_ipv6rd_conf->enable;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_IPv6rdSetEnable(
        ANSC_HANDLE     hDml,
        PANSC_HANDLE    phContext,
        BOOL            bEnable
        )
{
    int i;
    COSA_DML_IPV6RD_IF *ifconf;
    char key[MAX_LINE];

    if (!g_ipv6rd_conf) {
        syslog(LOG_ERR, "%s: not initialized", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    if ((bEnable && g_ipv6rd_conf->enable)
            || (!bEnable && !g_ipv6rd_conf->enable))
        return ANSC_STATUS_SUCCESS; /* status not change */

    g_ipv6rd_conf->enable = bEnable;

    for (i = 0; i < g_ipv6rd_conf->ifnum; i++) {
        ifconf = &g_ipv6rd_conf->ifconfs[i];

        if (bEnable && ifconf->Enable) {
            if (IPv6rd_TunnelAdd(ifconf) != 0) {
                syslog(LOG_ERR, "%s: fail to add %s", __FUNCTION__, ifconf->Alias);
                continue;
            }
        } else if (!bEnable && ifconf->Enable) {
            if (IPv6rd_TunnelDel(ifconf) != 0) {
                syslog(LOG_ERR, "%s: fail to delete %s", __FUNCTION__, ifconf->Alias);
                continue;
            }
        }
    }

    if (syscfg_set_bool(TR_6RD_ENABLE, bEnable) != 0
            || syscfg_commit() != 0) {
        syslog(LOG_ERR, "%s: save config file", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_IPv6rdGetNumberOfEntries(
        ANSC_HANDLE     hContext,
        ULONG           *NumEntries, /* input-output */
        ULONG           InsNumArray[]
        )
{
    int i;

    if (!g_ipv6rd_conf) {
        syslog(LOG_ERR, "%s: not initialized", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    if (!NumEntries || !InsNumArray) {
        syslog(LOG_ERR, "%s: bad parameters", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    if (*NumEntries < g_ipv6rd_conf->ifnum) {
        syslog(LOG_ERR, "%s: space not enough", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    *NumEntries = g_ipv6rd_conf->ifnum;
    for (i = 0; i < g_ipv6rd_conf->ifnum; i++) {
        InsNumArray[i] = g_ipv6rd_conf->ifconfs[i].InstanceNumber;
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_IPv6rdGetEntry(
        ANSC_HANDLE             hContext,
        ULONG                   ulInstanceNumber,
        PCOSA_DML_IPV6RD_IF     pEntry
        )
{
    COSA_DML_IPV6RD_IF *ifconf;
#if defined(USE_SYSTEM)
    char cmd[MAX_LINE];
    int err;
#endif

    if ((ifconf = get_ifconf_by_insnum(ulInstanceNumber)) == NULL) {
        syslog(LOG_ERR, "%s: instance [%d] is not exist", __FUNCTION__, ulInstanceNumber);
        return ANSC_STATUS_FAILURE;
    }

    if (!pEntry) {
        syslog(LOG_ERR, "%s: bad parameter", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    /* configurable parameters */
    bzero(pEntry, sizeof(COSA_DML_IPV6RD_IF));
    pEntry->InstanceNumber = ifconf->InstanceNumber;
    pEntry->Enable = ifconf->Enable;
    snprintf(pEntry->Alias, sizeof(pEntry->Alias), "%s", ifconf->Alias); 
    snprintf(pEntry->BorderRelayIPv4Addr, sizeof(pEntry->BorderRelayIPv4Addr), "%s", ifconf->BorderRelayIPv4Addr); 
    pEntry->AllTrafficToBorderRelay = ifconf->AllTrafficToBorderRelay;
    snprintf(pEntry->SPIPv6Prefix, sizeof(pEntry->SPIPv6Prefix), "%s", ifconf->SPIPv6Prefix); 
    pEntry->IPv4MaskLength = ifconf->IPv4MaskLength;
    snprintf(pEntry->AddressSource, sizeof(pEntry->AddressSource), "%s", ifconf->AddressSource); 

#if defined(USE_SYSTEM)
    /* it should be exist and UP */
    err = snprintf(cmd, sizeof(cmd), "cat /proc/net/dev | grep %s > /dev/null", ifconf->Alias);
    err |= snprintf(cmd, sizeof(cmd), "ip link show %s | grep UP > /dev/null", ifconf->Alias);
    if (system(cmd) == 0)
#endif
        snprintf(pEntry->Status, sizeof(pEntry->Status), "%s", "Enabled");
    else
        snprintf(pEntry->Status, sizeof(pEntry->Status), "%s", "Disabled");

    // XXX: what's the defination of Tunnel(ed)Interface in Linux 6RD model ?
    snprintf(pEntry->TunnelInterface, 
            sizeof(pEntry->TunnelInterface), "%s", ifconf->Alias);
    snprintf(pEntry->TunneledInterface, 
            sizeof(pEntry->TunneledInterface), "%s", "wan0");

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_IPv6rdSetEntry(
        ANSC_HANDLE             hContext,
        ULONG                   ulInstanceNumber,
        PCOSA_DML_IPV6RD_IF     pEntry
        )
{
    COSA_DML_IPV6RD_IF *ifconf;
    int delold, addnew;

    if ((ifconf = get_ifconf_by_insnum(ulInstanceNumber)) == NULL) {
		syslog(LOG_ERR, "%s: instance [%d] is not exist", __FUNCTION__, ulInstanceNumber);
        return ANSC_STATUS_FAILURE;
    }

    if (!pEntry || ulInstanceNumber != pEntry->InstanceNumber) {
        syslog(LOG_ERR, "%s: bad parameter", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    delold = addnew = 0;
    if (g_ipv6rd_conf->enable) {
        if (ifconf->Enable && !pEntry->Enable) { 
            /* disable a 6RD tunnel */
            delold++;
        } else if (ifconf->Enable && pEntry->Enable) { 
            /* change a 6RD tunnel's config */

            /* since if Alias is changed, we have to del/add,
             * so we always do del/add as change. */
            delold++, addnew++;
        } else if (!ifconf->Enable && pEntry->Enable) { 
            /* enable a 6RD tunnel */
            addnew++;
        } else { 
            /* from disable to disable, 
             * just save parameters for later use ? */
            ;
        }
    }

    /*
     * do real setting to system
     */
    if (delold && IPv6rd_TunnelDel(ifconf) != 0) {
        syslog(LOG_ERR, "%s: fail to delete 6RD tunnel", __FUNCTION__);
        /* return ANSC_STATUS_FAILURE; */
    }
    if (addnew && IPv6rd_TunnelAdd(pEntry) != 0) {
        syslog(LOG_ERR, "%s: fail to add 6RD tunnel", __FUNCTION__);
        /* return ANSC_STATUS_FAILURE; */
    }

    /* 
     * save ifconf to both memory struct and config file (PSM for cosa2.0) 
     */
    if (save_ifconf(pEntry) != 0) {
        syslog(LOG_ERR, "%s: Fail to save if's config", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_IPv6rdAddEntry(
        ANSC_HANDLE             hContext,
        PCOSA_DML_IPV6RD_IF     pEntry
        )
{
    if (!g_ipv6rd_conf) {
        syslog(LOG_ERR, "%s: not initialized", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    if (!pEntry) {
        syslog(LOG_ERR, "%s: bad parameter", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    /* InstanceNumber and Alias must be unique */
    if (get_ifconf_by_insnum(pEntry->InstanceNumber) != NULL
            || get_ifconf_by_alias(pEntry->Alias) != NULL) {
        syslog(LOG_ERR, "%s: Instance already exist", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    /* If and Alias is not provide try to generate a default one */
    if (pEntry->Alias[0] == '\0') {
        snprintf(pEntry->Alias, sizeof(pEntry->Alias), "tun6rd%d", pEntry->InstanceNumber);
        if (get_ifconf_by_alias(pEntry->Alias) != NULL) {
            syslog(LOG_ERR, "%s: Default alias is occupied", __FUNCTION__);
            return ANSC_STATUS_FAILURE;
        }
    }

    /* Too many instance ?*/
    if (g_ipv6rd_conf->ifnum + 1 > MAX_6RDIF_INS) {
        syslog(LOG_ERR, "%s: Too many instance", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    if (g_ipv6rd_conf->enable && pEntry->Enable) {
        if (IPv6rd_TunnelAdd(pEntry) != 0) {
            syslog(LOG_ERR, "%s: IPv6rd_TunnelAdd error", __FUNCTION__);
            /* return ANSC_STATUS_FAILURE; */
        }
    }
    
    if (save_ifconf(pEntry) != 0) {
        syslog(LOG_ERR, "%s: save_ifconf error", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_IPv6rdDelEntry(
        ANSC_HANDLE             hContext,
        ULONG                   ulInstanceNumber
        )
{
    COSA_DML_IPV6RD_IF *ifconf;
    int i;

    if ((ifconf = get_ifconf_by_insnum(ulInstanceNumber)) == NULL) {
		syslog(LOG_ERR, "%s: instance [%d] is not exist", __FUNCTION__, ulInstanceNumber);
        return ANSC_STATUS_FAILURE;
    }

    if (g_ipv6rd_conf->enable && ifconf->Enable) {
        if (IPv6rd_TunnelDel(ifconf) != 0) {
            syslog(LOG_ERR, "%s: IPv6rd_TunnelDel error", __FUNCTION__);
            /* return ANSC_STATUS_FAILURE; */
        }
    }

    /* delete from both memory struct and config file */
    if (delete_ifconf(ulInstanceNumber) != 0) {
        syslog(LOG_ERR, "%s: delete_ifconf error", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_Ifname2Addr(const char *ifname, char *addr, unsigned int size)
{
	if (ifname_to_ipaddr(ifname, addr, size) != 0)
		return ANSC_STATUS_FAILURE;

	return ANSC_STATUS_SUCCESS;
}


#elif defined(_COSA_SIM_)

#include <stdio.h>
#include <string.h>
#include "cosa_apis.h"
#include "cosa_ipv6rd_apis.h"

struct ipv6rd_sim_conf {
    BOOL                        enable;
    int                         ifnum;
    COSA_DML_IPV6RD_IF          ifconfs[MAX_6RDIF_INS];
};

static struct ipv6rd_sim_conf *g_ipv6rd_sim_conf;

static COSA_DML_IPV6RD_IF *
get_ifconf_by_insnum(int ins)
{
    int i;

    if (!g_ipv6rd_sim_conf) {
        return NULL;
    }

    for (i = 0; i < g_ipv6rd_sim_conf->ifnum; i++) {
        if (g_ipv6rd_sim_conf->ifconfs[i].InstanceNumber == ins)
            return &g_ipv6rd_sim_conf->ifconfs[i];
    }

    return NULL;
}

static COSA_DML_IPV6RD_IF *
get_ifconf_by_alias(const char *alias)
{
    int i;

    if (!g_ipv6rd_sim_conf) {
        return NULL;
    }

    for (i = 0; i < g_ipv6rd_sim_conf->ifnum; i++) {
        if (strcmp(g_ipv6rd_sim_conf->ifconfs[i].Alias, alias) == 0)
            return &g_ipv6rd_sim_conf->ifconfs[i];
    }

    return NULL;
}


ANSC_STATUS
CosaDml_IPv6rdInit(
        ANSC_HANDLE     hDml,
        PANSC_HANDLE    phContext
        )
{
    COSA_DML_IPV6RD_IF *ifconf;

    if (g_ipv6rd_sim_conf) {
        return ANSC_STATUS_FAILURE;
    }

    if ((g_ipv6rd_sim_conf = AnscAllocateMemory(sizeof(struct ipv6rd_sim_conf))) == NULL) {
        return ANSC_STATUS_FAILURE;
    }

    memset(g_ipv6rd_sim_conf, 0, sizeof(struct ipv6rd_sim_conf));
    g_ipv6rd_sim_conf->enable = TRUE;
    g_ipv6rd_sim_conf->ifnum = 1;

    ifconf = &g_ipv6rd_sim_conf->ifconfs[0];
    ifconf->InstanceNumber = 1;
    ifconf->Enable = TRUE;
    _ansc_sprintf(ifconf->Alias, "tun6rd1");
    _ansc_sprintf(ifconf->BorderRelayIPv4Addr, "100.1.24.2");
    ifconf->AllTrafficToBorderRelay = FALSE;
    _ansc_sprintf(ifconf->SPIPv6Prefix, "2001:db8::/32");
    ifconf->IPv4MaskLength = 8;
    _ansc_sprintf(ifconf->AddressSource, ""); // internal choose

    _ansc_sprintf(ifconf->Status, "Enabled");
    _ansc_sprintf(ifconf->TunnelInterface, "");
    _ansc_sprintf(ifconf->TunneledInterface, "");

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_IPv6rdFinalize(
        ANSC_HANDLE     hDml,
        PANSC_HANDLE    phContext
        )
{
    if (!g_ipv6rd_sim_conf) {
        return ANSC_STATUS_FAILURE;
    }

    AnscFreeMemory(g_ipv6rd_sim_conf);
    g_ipv6rd_sim_conf = NULL;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_IPv6rdGetEnable(
        ANSC_HANDLE     hDml,
        PANSC_HANDLE    phContext,
        BOOL            *bEnable
        )
{
    if (!g_ipv6rd_sim_conf) {
        return ANSC_STATUS_FAILURE;
    }

    *bEnable = g_ipv6rd_sim_conf->enable;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_IPv6rdSetEnable(
        ANSC_HANDLE     hDml,
        PANSC_HANDLE    phContext,
        BOOL            bEnable
        )
{
    if (!g_ipv6rd_sim_conf) {
        return ANSC_STATUS_FAILURE;
    }

    g_ipv6rd_sim_conf->enable = bEnable;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_IPv6rdGetNumberOfEntries(
        ANSC_HANDLE     hContext,
        ULONG           *NumEntries, /* input-output */
        ULONG           InsNumArray[]
        )
{
    int i;

    if (!g_ipv6rd_sim_conf) {
        return ANSC_STATUS_FAILURE;
    }

    if (!NumEntries || !InsNumArray)
        return ANSC_STATUS_FAILURE;

	if (*NumEntries < g_ipv6rd_sim_conf->ifnum) {
		return ANSC_STATUS_FAILURE;
	}

    *NumEntries = g_ipv6rd_sim_conf->ifnum;
    for (i = 0; i < g_ipv6rd_sim_conf->ifnum; i++) {
        InsNumArray[i] = g_ipv6rd_sim_conf->ifconfs[i].InstanceNumber;
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_IPv6rdGetEntry(
        ANSC_HANDLE             hContext,
        ULONG                   ulInstanceNumber,
        PCOSA_DML_IPV6RD_IF     pEntry
        )
{
    COSA_DML_IPV6RD_IF *ifconf;

    if ((ifconf = get_ifconf_by_insnum(ulInstanceNumber)) == NULL) {
        return ANSC_STATUS_FAILURE;
    }

    if (!pEntry) {
        return ANSC_STATUS_FAILURE;
    }
    
    /* configurable parameters */
    memset(pEntry, 0, sizeof(COSA_DML_IPV6RD_IF));
    pEntry->InstanceNumber = ifconf->InstanceNumber;
    pEntry->Enable = ifconf->Enable;
    _ansc_sprintf(pEntry->Alias, "%s", ifconf->Alias); 
    _ansc_sprintf(pEntry->BorderRelayIPv4Addr, "%s", ifconf->BorderRelayIPv4Addr); 
    pEntry->AllTrafficToBorderRelay = ifconf->AllTrafficToBorderRelay;
    _ansc_sprintf(pEntry->SPIPv6Prefix, "%s", ifconf->SPIPv6Prefix); 
    pEntry->IPv4MaskLength = ifconf->IPv4MaskLength;
    _ansc_sprintf(pEntry->AddressSource, "%s", ifconf->AddressSource); 

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_IPv6rdSetEntry(
        ANSC_HANDLE             hContext,
        ULONG                   ulInstanceNumber,
        PCOSA_DML_IPV6RD_IF     pEntry
        )
{
    COSA_DML_IPV6RD_IF *ifconf;

    if ((ifconf = get_ifconf_by_insnum(ulInstanceNumber)) == NULL) {
        return ANSC_STATUS_FAILURE;
    }

    if (!pEntry || ulInstanceNumber != pEntry->InstanceNumber) {
        return ANSC_STATUS_FAILURE;
    }

    /* not all field need to save */
    ifconf->Enable = pEntry->Enable;
    _ansc_sprintf(ifconf->Alias, "%s", pEntry->Alias);
    _ansc_sprintf(ifconf->BorderRelayIPv4Addr, "%s", pEntry->BorderRelayIPv4Addr);
    ifconf->AllTrafficToBorderRelay = pEntry->AllTrafficToBorderRelay;
    _ansc_sprintf(ifconf->SPIPv6Prefix, "%s", pEntry->SPIPv6Prefix);
    ifconf->IPv4MaskLength = pEntry->IPv4MaskLength;
    _ansc_sprintf(ifconf->AddressSource, "%s", pEntry->AddressSource);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_IPv6rdAddEntry(
        ANSC_HANDLE             hContext,
        PCOSA_DML_IPV6RD_IF     pEntry
        )
{
    if (!g_ipv6rd_sim_conf) {
        return ANSC_STATUS_FAILURE;
    }

    if (!pEntry) {
        return ANSC_STATUS_FAILURE;
    }

    /* InstanceNumber and Alias must be unique */
    if (get_ifconf_by_insnum(pEntry->InstanceNumber) != NULL
            || get_ifconf_by_alias(pEntry->Alias) != NULL) {
        return ANSC_STATUS_FAILURE;
    }

    /* If and Alias is not provide try to generate a default one */
    if (pEntry->Alias[0] == '\0') {
        _ansc_sprintf(pEntry->Alias, "tun6rd%d", pEntry->InstanceNumber);
        if (get_ifconf_by_alias(pEntry->Alias) != NULL) {
            return ANSC_STATUS_FAILURE;
        }
    }

    /* Too many instance ?*/
    if (g_ipv6rd_sim_conf->ifnum + 1 == MAX_6RDIF_INS) {
        return ANSC_STATUS_FAILURE;
    }

    g_ipv6rd_sim_conf->ifconfs[g_ipv6rd_sim_conf->ifnum++] = *pEntry;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDml_IPv6rdDelEntry(
        ANSC_HANDLE             hContext,
        ULONG                   ulInstanceNumber
        )
{
    COSA_DML_IPV6RD_IF *ifconf;
    int i;

    for (i = 0; i < g_ipv6rd_sim_conf->ifnum; i++) {
        if (g_ipv6rd_sim_conf->ifconfs[i].InstanceNumber == ulInstanceNumber) {
            ifconf = &g_ipv6rd_sim_conf->ifconfs[i];
            break;
        }
    }
    if (i == g_ipv6rd_sim_conf->ifnum) {
        return ANSC_STATUS_FAILURE;
    }

    memmove(&g_ipv6rd_sim_conf->ifconfs[i], &g_ipv6rd_sim_conf->ifconfs[i+1], 
            (g_ipv6rd_sim_conf->ifnum - i - 1) * sizeof(COSA_DML_IPV6RD_IF));
    g_ipv6rd_sim_conf->ifnum--;

    return ANSC_STATUS_SUCCESS;
}
#endif
