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

    module: cosa_ip_apis.c

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
#include "cosa_apis.h"
#include "cosa_ip_apis.h"
#include "cosa_ip_internal.h"
#include "cosa_ip_apis_multilan.h"
#include "secure_wrapper.h"
#include "safec_lib_common.h"

extern void* g_pDslhDmlAgent;

#if ( defined(_COSA_INTEL_USG_ARM_) || defined(_COSA_BCM_MIPS_))
#include <net/if.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <utapi.h>
#include <utapi_util.h>
#include "utctx/utctx_api.h"
#include "syscfg/syscfg.h"
#include "cosa_dhcpv6_apis.h"
#include "cosa_ip_internal.h"
#include "cosa_drg_common.h"

#if defined(_COSA_BCM_MIPS_) || defined(_ENABLE_DSL_SUPPORT_)
#define INTERFACE "erouter0"
#else
#define INTERFACE "wan0"
#endif
#if defined (MULTILAN_FEATURE)
#define MAX_IPIF_NUM  20
#else
#define MAX_IPIF_NUM  5
#endif
/*this is the real backend buffers, the PCOSA_DATAMODEL_IP structure only stores stuff from WebGui or ACS, we use this buffer to track param changes..*/
COSA_PRI_IP_IF_FULL  g_ipif_be_bufs[MAX_IPIF_NUM];

#define SYSCFG_FORMAT_NORMAL_V6ADDR "tr_%s_v6addr"
#define SYSCFG_FORMAT_NORMAL_V6PREFIX "tr_%s_v6pref_%s"

#define SYSCFG_FORMAT_NAMESPACE_STATIC_V6ADDR "tr_%s_static_v6addr_%lu"
#define SYSCFG_FORMAT_NAMESPACE_STATIC_V6PREFIX "tr_%s_static_v6pref_%lu"
#define SYSCFG_FORMAT_STATIC_V6ADDR "tr_%s_ipv6_static_addresses"
#define SYSCFG_FORMAT_STATIC_V6PREF "tr_%s_ipv6_static_prefixes"
#define PVAL_MAX_SIZE   128

char   g_ipif_names[MAX_IPIF_NUM][64];
int    g_ipif_num;
/*
 *  DH  Temp workaround for the 0.0.0.0 address issue on erouter0
 *      The root source is because _get_addressing_type() uses hardcoded "wan"/"lan" keywords
 *      to differentiate LAN/WAN interfaces.
 *      Anyway, IP.Interface SBAPI is an akward implementation, as hardcoded values are seen everywhere. 
 */
BOOLEAN                 gDmsbIpIfUpstream[MAX_IPIF_NUM] = {0};
ULONG                   gDmsbIpIfLoopbackInstNum        = 0;

static int _invoke_ip_cmd(char * cmd, PCOSA_DML_IP_V6ADDR p_old, PCOSA_DML_IP_V6ADDR p_new, char * ifname, void * extra_args);
static int 
IPIF_getEntry_for_Ipv6Addr
    (
        PCOSA_DML_IP_IF_FULL2       p_ipif,
        ULONG ulIndex
    );
static int
IPIF_getEntry_for_Ipv6Pre
    (
        PCOSA_DML_IP_IF_FULL2       p_ipif,
        ULONG ulIndex
    );



BOOL CosaIpifGetSetSupported(char * pParamName)
{
#if defined _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_
    char * not_supported_param_list[]= {"addentry", "delentry", /*"Enable",*/ "AutoIPEnable", \
        "Loopback", "ipv4addr_addentry", "ipv4addr_delentry", "Router", \
        "Anycast", "Status", "StaticType", "IPAddressStatus", "Prefix", "PrefixStatus", "PreferredLifetime", \
        "ValidLifetime", "ParentPrefix", "PreferredLifetime", ""};
#else
    char * not_supported_param_list[]= {"addentry", "delentry", /*"Enable",*/ "AutoIPEnable", "LowerLayers", "Loopback", "ipv4addr_addentry", "ipv4addr_delentry", "Router", ""};
#endif
    char ** p;

    for (p= &not_supported_param_list[0]; (*p)[0]; p++)
    {
        if (AnscEqualString(pParamName, *p, TRUE))
        {
            return FALSE;
        }
    }
    
    return TRUE;
}

void _get_shell_output(FILE *fp, char *buf, int len)
{
    char * p;

    if (fp)
    {
        if(fgets (buf, len-1, fp) != NULL)
        {
            buf[len-1] = '\0';
            if ((p = strchr(buf, '\n'))) {
                *p = '\0';
            }
        }
    v_secure_pclose(fp); 
    }
}

int _get_shell_output2(FILE *fp, char * dststr)
{
    char   buf[256];
    int   bFound = 0;

    if (fp)
    {
        while( fgets(buf, sizeof(buf), fp) )
        {
            if (strstr(buf, dststr)) 
            {
                bFound = 1;;
                break;
            }
        }
        
        v_secure_pclose(fp);
    }

    return bFound;
}

#define LAN_TIMEOUT 3
#define WAN_TIMEOUT 15

#if 0
static void _wait_for_services(char * serv_name)
{
    char buf[256] = {0};
    int  i = 0;
    FILE *fp;

    if (strcmp(serv_name, "lan") == 0)
    {
        for (i= 0; i< LAN_TIMEOUT; i++)
        {
            fp = v_secure_popen("r","sysevent get lan-status");
            _get_shell_output(fp, buf, sizeof(buf));
            if (strcmp(buf, "started") != 0)
            {
                sleep(1);
                continue;
            }
            else 
                break;
        }
        
        if (i == LAN_TIMEOUT) CcspTraceWarning(("Lan service not fully started, IP.Interface data may be wrong\n"));
    }
    else if (strcmp(serv_name, "wan") == 0)
    {
        for (i= 0; i< WAN_TIMEOUT; i++)
        {
            fp = v_secure_popen("r","sysevent get wan-status");
            _get_shell_output(fp, buf, sizeof(buf));
            if (strcmp(buf, "started") != 0)
            {
                sleep(1);
                continue;
            }
            else 
                break;
        }

        if (i == WAN_TIMEOUT) CcspTraceWarning(("Wan service not fully started, IP.Interface data may be wrong\n"));
    }
    
}

static int _is_primary_mode()
{
    char buf[256] = {0};
    FILE *fp;    

    fp = v_secure_popen("r","sysevent get current_hsd_mode");
    _get_shell_output(fp, buf, sizeof(buf));

    /*current_hsd_mode has 3 possible values: byoi, primary. unknown. from IP interface's view, byoi is different from the other 2 */
    if (strcmp(buf, "byoi") == 0)
        return 0;
    else 
        return 1;
}


static int _is_in_linux_bridge(char * if_name, char * br_name)
{
    char buf[256] = {0};
    FILE *fp;

    fp = v_secure_popen("r", "brctl show %s|grep %s", br_name, if_name);
    _get_shell_output(fp, buf, sizeof(buf));

    if (strstr(buf, if_name))
        return 1;
    else
        return 0;
    
}
#endif


/**********************************************************************

    caller:     self

    prototype:

        ULONG
        CosaDmlIpInit
            (
                ANSC_HANDLE                 hDml,
                PANSC_HANDLE                phContext
             );
    Description:
    
        The API initial the entry of IP interfaces in the system.

    Arguments:
                ANSC_HANDLE                 hDml

                PANSC_HANDLE                phContext

    Return:
                  The operation status..

**********************************************************************/
#if defined _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_

#ifdef _COSA_INTEL_USG_ARM_
    #define COSA_USG_IF_NUM 4
#endif
#if defined(_COSA_BCM_MIPS_) || defined(_PLATFORM_RASPBERRYPI_) || defined(_ENABLE_DSL_SUPPORT_)
    #if defined(COSA_USG_IF_NUM)
        #undef COSA_USG_IF_NUM
    #endif
    #define COSA_USG_IF_NUM 3 // we only have 3 interfaces for XF3
#endif
#ifdef _PLATFORM_TURRIS_
    #if defined(COSA_USG_IF_NUM)
        #undef COSA_USG_IF_NUM
    #endif
    #define COSA_USG_IF_NUM 3
#endif

typedef struct USG_IF_CFG
{
    char                            IfName[64];
    COSA_DML_LINK_TYPE              LinkType;
    /* DH  Temp workaround for the 0.0.0.0 address issue on erouter0 */
    BOOLEAN                         bUpstream;
}USG_IF_CFG_T;

USG_IF_CFG_T g_usg_if_cfg[COSA_USG_IF_NUM] =
{
    {"erouter0",    COSA_DML_LINK_TYPE_EthLink, TRUE},
#if defined(_COSA_INTEL_USG_ARM_) && !defined(_ENABLE_DSL_SUPPORT_)
#ifndef _PLATFORM_RASPBERRYPI_
#ifndef _PLATFORM_TURRIS_
    {"wan0",        COSA_DML_LINK_TYPE_DOCSIS,  TRUE},  /*DH  wan0 should never appear here -- CM extensions are for DOCSIS interfaces */
#endif
#endif
#endif
    {"lo",          COSA_DML_LINK_TYPE_EthLink, FALSE}, /*DH  change the value of gDmsbIpIfLoopbackInstNum too, if "lo" is moved to a different location */
    /*
     *  Multi-LAN -- still have to keep primary LAN interface for IPv6 settings -- overlapping with MLAN configuration
     *  Stay with the name "brlan0" for now.
     */
    {"brlan0",      COSA_DML_LINK_TYPE_Bridge,  FALSE}
};

#define G_USG_IF_NAME(i) g_usg_if_cfg[i].IfName
#define G_USG_IF_LINKTYPE(i) g_usg_if_cfg[i].LinkType 

#if 0
 static int _is_bridge_mode(){
    UtopiaContext utctx = {0};
    bridgeInfo_t bridge_info ={0}; /*RDKB-6840, CID-33568, initialize before use*/
    int ret = TRUE;
    if(Utopia_Init(&utctx)){
        Utopia_GetBridgeSettings(&utctx, &bridge_info);
        ret = bridge_info.mode == BRIDGE_MODE_OFF ? FALSE: TRUE;
        Utopia_Free(&utctx, 0);
    }
    return ret;
}
#endif
#endif

int CosaGetUsgIfNum() {
    return COSA_USG_IF_NUM;
}

ANSC_STATUS
CosaDmlIpInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    errno_t rc = -1;
#if defined _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_
    ANSC_STATUS                     returnStatus;
    int i;
    for(i = 0; i < COSA_USG_IF_NUM; i++)
    {
        rc = strcpy_s((char *)g_ipif_names[i],sizeof(g_ipif_names[i]), G_USG_IF_NAME(i));
        ERR_CHK(rc);
        gDmsbIpIfUpstream[i] = g_usg_if_cfg[i].bUpstream;
    }
    g_ipif_num               = COSA_USG_IF_NUM;
    gDmsbIpIfLoopbackInstNum = 2+1;

    returnStatus = CosaDmlIpMlanInit(hDml, phContext);
    if ( ANSC_STATUS_SUCCESS != returnStatus )
    {
        return  returnStatus;
    }
#endif

    syscfg_init();
    
    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     self

    prototype:

        ULONG
        CosaDmlIpIfGetNumberOfEntries
            (
                ANSC_HANDLE                 hContext
             );
    Description:
    
        The API retrieves the number of IP interfaces in the system.

    Arguments:
                ANSC_HANDLE                 hContext

    Return:
                The number of entries in the Interface table.

**********************************************************************/
ULONG
CosaDmlIpIfGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    )
{
    /* We have a overlapping interface */
    return g_ipif_num + CosaDmlIpIfMlanGetNumberOfEntries(hContext) - 1;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlIpIfGetEntry
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulIndex,
                PCOSA_DML_IP_IF_FULL        pEntry
             );

    Description:
           The API retrieves the complete info of the IP interface designated by index. 
           The usual process is the caller gets the total number of entries, then iterate 
           through those by calling this API.
    Arguments:
               ANSC_HANDLE                 hContext,
               
               ULONG                       ulIndex,
               
               PCOSA_DML_IP_IF_FULL        pEntry

    Return:
        The ulIndex entry of the table

**********************************************************************/
typedef struct 
{
    int onlink;
    int autoconf;
    int pref_len;
    int valid_lft;
    int prefered_lft;
    int addr_16b[8];/*the prefix is printed in 16bit groups, at most 8 prints*/
    char pref[64]; /*the format is like : 2006:cafe:0000:0000:/64, it's the same as ipv6_addr_info_t.v6pre format*/
}exported_ra_info_t;  /*ra = Router Advertisement*/


/*kernel will export RA info at EXPORTED_RA_INFO, this function will parse the file
 pp_info will be realloc() to contain *p_num * sizeof(exportede_ra_info_t)*/
#define EXPORTED_RA_INFO "/proc/net/ipv6_ra_info"
static int _obtain_ra_info(exported_ra_info_t ** pp_info, int * p_num)
{
    FILE * fp = NULL;
    char buf[1024] = {0};
    exported_ra_info_t * p = NULL;
    int  i = 0;
    errno_t  safec_rc = -1;

    if (!pp_info || !p_num)
        return -1;

    *p_num = 0;
    fp = fopen(EXPORTED_RA_INFO, "r");
    if (!fp)
        return -2;

    while (fgets(buf, sizeof(buf), fp))
    {
        (*p_num)++;
        *pp_info = realloc(*pp_info, *p_num * sizeof(exported_ra_info_t));
        if (!*pp_info)
            continue;
        
        /*current ra info*/
        p = *pp_info + *p_num - 1;
        
        if (13 != sscanf(buf, "%d %d %d %x %x %x:%x:%x:%x:%x:%x:%x:%x", 
                         &p->onlink, &p->autoconf,
                         &p->pref_len, &p->valid_lft,
                         &p->prefered_lft,
                         &p->addr_16b[0],&p->addr_16b[1],&p->addr_16b[2],&p->addr_16b[3],
                         &p->addr_16b[4],&p->addr_16b[5],&p->addr_16b[6],&p->addr_16b[7]))
        {
            (*p_num)--;
            continue;
        }

        memset(p->pref, 0, sizeof(p->pref));
        for (i=0; (i< ( p->pref_len%16 ? (p->pref_len/16+1):p->pref_len/16)) && i<8; i++)
        {
            safec_rc = sprintf_s(p->pref + strlen(p->pref), sizeof(p->pref)-strlen(p->pref),"%04x:", p->addr_16b[i]);
            if(safec_rc < EOK)
            {
                ERR_CHK(safec_rc);
            }
        }
        safec_rc = sprintf_s(p->pref+strlen(p->pref), sizeof(p->pref)-strlen(p->pref), ":/%d", p->pref_len);
        if(safec_rc < EOK)
        {
            ERR_CHK(safec_rc);
        }

        memset(buf, 0, sizeof(buf));
    }

    fclose(fp);
    return 0;
}



int _get_datetime_offset(int offset, char * buf, unsigned int len)
{
    time_t t1;
    struct tm t2 = {0};
    errno_t safec_rc = -1;

    if (!buf)
        return -1;
    
    if (offset == (int)0xffffffff)
    {
        /*forever*/
        safec_rc = strcpy_s(buf, len, "9999-12-31T23:59:59Z");
        if(safec_rc != EOK)
        {
            ERR_CHK(safec_rc);
            return -1;
        }
        return 0;
    }

    t1= time(NULL) + offset;
    localtime_r(&t1, &t2);

    snprintf(buf, len, "%d-%d-%dT%02d:%02d:%02dZ", 
             1900+t2.tm_year, t2.tm_mon+1, t2.tm_mday,
             t2.tm_hour, t2.tm_min, t2.tm_sec);

    return 0;
}

/*the format of the record:
3: eth1: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qlen 1000
    inet6 2007:abcd::a00:27ff:fe10:d6ac/64 scope global dynamic 
       valid_lft 2579976sec preferred_lft 592776sec
    inet6 2006:cafe::a00:27ff:fe10:d6ac/64 scope global dynamic 
       valid_lft 2579976sec preferred_lft 592776sec
    inet6 fe80::a00:27ff:fe10:d6ac/64 scope link 
       valid_lft forever preferred_lft forever

lft: lift time
*/
static int _get_2_lfts(char * fn, int * p_valid, int * p_prefer, ipv6_addr_info_t * p_addr)
{
    FILE * fp = NULL;
    char buf[256] = {0};
    struct in6_addr  addr;
    char addr_str[128] = {0};
    char * p = NULL;
    char file_str[128] = {0};
    int  pre_len = 0;
    int  found = 0;
    errno_t safec_rc = -1;

    if (!(fp = fopen(fn, "r")))
        return -1;
        
    while (fgets((char *)buf, sizeof(buf), fp))
    {
        if (strstr((char *)buf, "inet6") && strstr((char *)buf, "scope"))
        {
            inet_pton(AF_INET6, (char *)p_addr->v6addr, &addr);
            inet_ntop(AF_INET6, &addr, addr_str, sizeof(addr_str));
            
            p = strchr((char *)p_addr->v6pre, '/');
            if (!p)
                continue;
            
            sscanf(p, "/%d", &pre_len);

            safec_rc = sprintf_s(addr_str+strlen(addr_str), sizeof(addr_str)-strlen(addr_str), "/%d", pre_len);
            if(safec_rc < EOK)
            {
               ERR_CHK(safec_rc);
            }
            p = (char *)buf;
            while (isblank(*p)) p++;
            
            if (sscanf(p, "inet6 %s", file_str) == 1)
            {
                if (!strncmp((char *)file_str, (char *)addr_str, sizeof(file_str)))
                    found = 1;
            }

            if (!found)
                continue;

            /*the next line is life time info*/
            memset(buf, 0, sizeof(buf));
            fgets((char *)buf, sizeof(buf), fp);

            p = (char *)buf;
            while (isblank(*p)) p++;

            if (!strncmp(p, "valid_lft forever preferred_lft forever", strlen("valid_lft forever preferred_lft forever")))
            {
                *p_valid = *p_prefer = 0xffffffff;
                fclose(fp);
                return 0;
            }


            if (sscanf(p, "valid_lft %dsec preferred_lft %dsec", p_valid, p_prefer) == 2)
            {
                fclose(fp);
                return 0;
            }

            
        }
        memset(buf, 0, sizeof(buf));
    }

    fclose(fp);
    return 0;
}

#define TMP_IP_OUTPUT  "/var/.ip_output"
static int _get_datetime_lfts(char * p_pref_datetime, int len1,  char * p_valid_datetime, int len2, ipv6_addr_info_t * p_v6addr, ULONG ulIndex)
{
    /*this is a SLAAC address from RA*/
    int  valid_lft = 0, prefered_lft = 0;
    char iana_pretm[32] = {0};
    char iana_vldtm[32] = {0};
    errno_t safec_rc = -1;

    /*it's hard to use iproute2 C code to obtain 2 lifetimes, here call "ip" directly*/
    v_secure_system("ip -6 addr show dev %s > " TMP_IP_OUTPUT, g_ipif_names[ulIndex]);
       
    _get_2_lfts(TMP_IP_OUTPUT, &valid_lft, &prefered_lft, p_v6addr);
    _get_datetime_offset(valid_lft, p_valid_datetime, len1);
    _get_datetime_offset(prefered_lft, p_pref_datetime, len2);

    safec_rc = sprintf_s(iana_pretm,sizeof(iana_pretm),"%d",prefered_lft);
    if(safec_rc < EOK)
    {
       ERR_CHK(safec_rc);
    }
    safec_rc = sprintf_s(iana_vldtm,sizeof(iana_vldtm),"%d",valid_lft);
    if(safec_rc < EOK)
    {
       ERR_CHK(safec_rc);
    }

    commonSyseventSet(COSA_DML_DHCPV6C_ADDR_PRETM_SYSEVENT_NAME, iana_pretm);
    commonSyseventSet(COSA_DML_DHCPV6C_ADDR_VLDTM_SYSEVENT_NAME, iana_vldtm);

    return 0;
}

int  _is_static_addr(char * ifname, char * v6addr)
{
    char namespace[256] = {0};
    char out[256] = {0};
    char out1[256] = {0};
    char buf[256] = {0};
    char * str = NULL;
    char * p_token = NULL;
    char * saveptr = NULL;
    int found = 0;
    UtopiaContext utctx = {0};
    errno_t safec_rc = -1;
    
    if (!Utopia_Init(&utctx))
        return found ;

    safec_rc = sprintf_s(buf, sizeof(buf), SYSCFG_FORMAT_STATIC_V6ADDR, ifname);
    if(safec_rc < EOK)
    {
       ERR_CHK(safec_rc);
    }
    memset(out, 0, sizeof(out));
    Utopia_RawGet(&utctx,NULL,buf,out,sizeof(out));
    
    if (!out[0])
        goto OUT;
    
    
    for (str = out; ; str = NULL) 
    {
        p_token = strtok_r(str, ",", &saveptr);
        if (p_token == NULL)
                break;

        snprintf(namespace, sizeof(namespace)-1, SYSCFG_FORMAT_STATIC_V6ADDR, ifname);

        memset(out1, 0, sizeof(out1));
        Utopia_RawGet(&utctx,namespace,"IPAddress",out1,sizeof(out1));

        if (!strcmp(out1, v6addr))
        {
            found = 1;
            break;
        }
    }

OUT:
    Utopia_Free(&utctx,0);        

    return found;
}

static int _get_early_pref_len(int * p_pref_len, ULONG ulIndex, char * v6pref_path, char * v6addr_str)
{
    ULONG inst2 = 0;
    int i = 0;
    int found = 0;
    char * pref_str = NULL;

    
    if (!p_pref_len || !v6pref_path || !v6addr_str)
        return -1;

    if (sscanf(v6pref_path, "Device.IP.Interface.%*d.IPv6Prefix.%lu", &inst2) != 1)
        return -2;

    if (!inst2)
        return -3;

    for (i=0; i<g_ipif_be_bufs[ulIndex].ulNumOfV6Pre; i++)
    {
        if (g_ipif_be_bufs[ulIndex].V6PreList[i].InstanceNumber == inst2) 
        {
            found = 1;
            pref_str = (char *)g_ipif_be_bufs[ulIndex].V6PreList[i].Prefix;
            break;
        }
    }

    if (found && !__v6addr_mismatches_v6pre(v6addr_str ,pref_str))
    {
        char * p = strchr(pref_str, '/');
        if (p)
        {
            if (sscanf(p, "/%d", p_pref_len) == 1)
                return 0;

        }
    }

    return -1;
}

static int _load_v6_alias_instNum(UtopiaContext *p_utctx, char * syscfg_instNum, char * syscfg_alias, ULONG * p_inst_num, char * p_alias, unsigned int alias_size)
{
    char out[256] = {0};
    errno_t safec_rc = -1;
    
    memset(out, 0, sizeof(out));
    Utopia_RawGet(p_utctx,NULL,syscfg_instNum,out,sizeof(out));
       
    if (!out[0])
        *p_inst_num = 0;
    else
        *p_inst_num = atoi(out);

    memset(out, 0, sizeof(out));
    Utopia_RawGet(p_utctx,NULL,syscfg_alias,out,sizeof(out));
    safec_rc = STRCPY_S_NOCLOBBER(p_alias, alias_size, out);
    if(safec_rc != EOK)
    {
       ERR_CHK(safec_rc);
    }
    return 0;
}

static int _save_v6_alias_instNum(UtopiaContext *p_utctx, char * syscfg_instNum, char * syscfg_alias, ULONG inst_num, char * p_alias)
{
    char val[256] = {0};
    
    errno_t safec_rc = sprintf_s(val, sizeof(val), "%lu", inst_num);
    if(safec_rc < EOK)
    {
       ERR_CHK(safec_rc);
    }

    Utopia_RawSet(p_utctx,NULL,syscfg_instNum,val);
       
    Utopia_RawSet(p_utctx,NULL,syscfg_alias,p_alias);

    return 0;
}


static int 
IPIF_getEntry_for_Ipv6Addr
    (
        PCOSA_DML_IP_IF_FULL2       p_ipif,
        ULONG ulIndex
    )
{
    char buf[256]= {0};
    char buf1[256]= {0};
    char out[1024] = {0};
    UtopiaContext utctx = {0};
    ipv6_addr_info_t * p_v6addr = NULL;
    ipv6_addr_info_t * orig_p_v6addr = NULL;
    int  v6addr_num = 0;
    int  i = 0;
    COSA_DML_IP_V6ADDR * p_dml_v6addr = NULL;
    int  ret = 0;
    exported_ra_info_t  * p_ra = NULL;
    int  ra_num = 0;
    int  j = 0;
    char namespace[256] = {0};
    int  need_write = 0;
    errno_t safec_rc = -1;
    
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    CosaUtilGetIpv6AddrInfo((char *)g_ipif_names[ulIndex], &p_v6addr, &v6addr_num);
    _obtain_ra_info(&p_ra, &ra_num);

    g_ipif_be_bufs[ulIndex].ulNumOfV6Addr = 0;

    /*save for free()*/
    orig_p_v6addr = p_v6addr;

    /*the following loop block is on link-local, loopback, RA, DHCPv6 IANA address*/
    /*if we have too many ipv6 addresses, drop them*/


    for (i=0; i<v6addr_num && i<MAX_IPV6_ENTRY_NUM; i++,p_v6addr++)
    {
        p_dml_v6addr = &g_ipif_be_bufs[ulIndex].V6AddrList[g_ipif_be_bufs[ulIndex].ulNumOfV6Addr];

            
        /*for static v6address, it's not done here, just ignore it */
        if (_is_static_addr((char *)g_ipif_names[ulIndex], p_v6addr->v6addr))
            continue;

        //fprintf(stderr, "index:%d, addr:%s, scope:%d\n", i, p_v6addr->v6addr, p_v6addr->scope);
        p_dml_v6addr->Origin = 0;
        /*determine origin*/
        if (p_v6addr->scope == IPV6_ADDR_SCOPE_LINKLOCAL)
            p_dml_v6addr->Origin = COSA_DML_IP6_ORIGIN_AutoConfigured;
        else if (p_v6addr->scope == IPV6_ADDR_SCOPE_LOOPBACK)
            p_dml_v6addr->Origin = COSA_DML_IP6_ORIGIN_WellKnown;
        else if (p_v6addr->scope == IPV6_ADDR_SCOPE_GLOBAL)
        {
            char dhcpv6_addr[64] = {0};
            char global_addr[64] = {0};
            struct sockaddr_in6 sap;

            /*first handle RA type*/
            for (j = 0; j< ra_num ; j++)
            {
                if (!strncmp(p_v6addr->v6pre, p_ra[j].pref, sizeof(p_ra[j].pref)))
                {
                    p_dml_v6addr->Origin = COSA_DML_IP6_ORIGIN_AutoConfigured;
                    break;
                }
            }
            
            commonSyseventGet(COSA_DML_DHCPV6C_ADDR_SYSEVENT_NAME, dhcpv6_addr, sizeof(dhcpv6_addr));
            if (!strncmp(p_v6addr->v6addr, dhcpv6_addr, sizeof(p_v6addr->v6addr))){
                p_dml_v6addr->Origin = COSA_DML_IP6_ORIGIN_DHCPv6;

               if ( _ansc_strstr(g_ipif_names[ulIndex], "erouter0" )  ){
                   if (!commonSyseventGet(COSA_DML_DHCPV6C_ADDR_IAID_SYSEVENT_NAME, out, sizeof(out)) )
                        g_ipif_be_bufs[ulIndex].Info.iana_iaid = atoi(out);
                   else
                        g_ipif_be_bufs[ulIndex].Info.iana_iaid  = 0;

                   if (!commonSyseventGet(COSA_DML_DHCPV6C_ADDR_T1_SYSEVENT_NAME, out, sizeof(out)) )
                        g_ipif_be_bufs[ulIndex].Info.iana_t1 = atoi(out);
                   else
                        g_ipif_be_bufs[ulIndex].Info.iana_t1  = 0;

                   if (!commonSyseventGet(COSA_DML_DHCPV6C_ADDR_T2_SYSEVENT_NAME, out, sizeof(out)) )
                        g_ipif_be_bufs[ulIndex].Info.iana_t2 = atoi(out);
                   else
                        g_ipif_be_bufs[ulIndex].Info.iana_t2  = 0;

                   /*if (!commonSyseventGet(COSA_DML_DHCPV6C_ADDR_PRETM_SYSEVENT_NAME, out, sizeof(out)) )
                        p_dml_v6addr->iana_pretm = atoi(out);
                   else
                        p_dml_v6addr->iana_pretm = 0;

                   if (!commonSyseventGet(COSA_DML_DHCPV6C_ADDR_VLDTM_SYSEVENT_NAME, out, sizeof(out)) )
                        p_dml_v6addr->iana_vldtm = atoi(out);
                   else
                        p_dml_v6addr->iana_vldtm = 0;*/

               }
            }

            commonSyseventGet(COSA_DML_DHCPV6S_ADDR_SYSEVENT_NAME, global_addr, sizeof(global_addr));
            inet_pton(AF_INET6, global_addr, (struct sockaddr *) &sap.sin6_addr);
            sap.sin6_family = AF_INET6;
            inet_ntop(AF_INET6, (struct sockaddr *) &sap.sin6_addr, global_addr, sizeof(global_addr));

            //fprintf(stderr, "changed event addr:%s, p_v6addr->v6addr:%s\n", global_addr, p_v6addr->v6addr);
            if (!strncmp(p_v6addr->v6addr, global_addr, sizeof(p_v6addr->v6addr)))
                p_dml_v6addr->Origin = COSA_DML_IP6_ORIGIN_AutoConfigured;

        }
        
        if (!p_dml_v6addr->Origin)
        {
            continue;
        }

       memset(buf, 0, sizeof(buf));
       memset(buf1, 0, sizeof(buf1));

       if(!strncmp(p_v6addr->v6addr, "fe80", 4)){
               safec_rc = sprintf_s(buf, sizeof(buf), SYSCFG_FORMAT_NORMAL_V6ADDR"_inet6_link_inst_num", g_ipif_names[ulIndex]);
               if(safec_rc < EOK)
               {
                   ERR_CHK(safec_rc);
               }
               safec_rc = sprintf_s(buf1, sizeof(buf1), SYSCFG_FORMAT_NORMAL_V6ADDR"_inet6_link_alias", g_ipif_names[ulIndex]);
               if(safec_rc < EOK)
               {
                   ERR_CHK(safec_rc);
               }
       }else{
               safec_rc = sprintf_s(buf, sizeof(buf), SYSCFG_FORMAT_NORMAL_V6ADDR"_inet6_inst_num", g_ipif_names[ulIndex]);
               if(safec_rc < EOK)
               {
                   ERR_CHK(safec_rc);
               }
               safec_rc = sprintf_s(buf1, sizeof(buf1), SYSCFG_FORMAT_NORMAL_V6ADDR"_inet6_alias", g_ipif_names[ulIndex]);
               if(safec_rc < EOK)
               {
                   ERR_CHK(safec_rc);
               }
       }

        if (Utopia_Init(&utctx))
        {
            need_write = 0;
            _load_v6_alias_instNum(&utctx, buf, buf1, &p_dml_v6addr->InstanceNumber, p_dml_v6addr->Alias, sizeof(p_dml_v6addr->Alias));

            if (!p_dml_v6addr->InstanceNumber)
            {
                need_write = 1;
                CosaIPv6AddrGenInstAlias(p_ipif, p_dml_v6addr);
                _save_v6_alias_instNum(&utctx, buf, buf1, p_dml_v6addr->InstanceNumber, p_dml_v6addr->Alias);
            }
            Utopia_Free(&utctx,need_write);                
        }

        p_dml_v6addr->bEnabled = TRUE;
        
        p_dml_v6addr->Status = COSA_DML_IP_ADDR_STATUS_Enabled;

        p_dml_v6addr->V6Status = COSA_DML_IPV4V6_STATUS_Enabled;

        safec_rc = strcpy_s(p_dml_v6addr->IP6Address, sizeof(p_dml_v6addr->IP6Address), p_v6addr->v6addr);
        if(safec_rc != EOK)
        {
           ERR_CHK(safec_rc);
        }

        if (p_v6addr->scope == IPV6_ADDR_SCOPE_LINKLOCAL ||
            p_v6addr->scope == IPV6_ADDR_SCOPE_LOOPBACK)
        {
            /*we don't have prefix for these 2 types*/
            p_dml_v6addr->Prefix[0] = 0;
            
            safe_strcpy(p_dml_v6addr->PreferredLifetime, "9999-12-31T23:59:59Z", sizeof(p_dml_v6addr->PreferredLifetime));
            safe_strcpy(p_dml_v6addr->ValidLifetime, "9999-12-31T23:59:59Z", sizeof(p_dml_v6addr->ValidLifetime));
        }
        else 
        { 
            /*prefix is calculated when GetParam, not here*/
            
            _get_datetime_lfts((char *)p_dml_v6addr->PreferredLifetime, sizeof(p_dml_v6addr->PreferredLifetime),
                               (char *)p_dml_v6addr->ValidLifetime, sizeof(p_dml_v6addr->ValidLifetime),
                               p_v6addr, ulIndex);
        }
	
	#ifdef _HUB4_PRODUCT_REQ_
	    if ( _ansc_strstr(g_ipif_names[ulIndex], "brlan0" )  ) {
	#else
	    if ( _ansc_strstr(g_ipif_names[ulIndex], "erouter0" )  ) {
	#endif
		if (!commonSyseventGet(COSA_DML_DHCPV6C_ADDR_PRETM_SYSEVENT_NAME, out, sizeof(out)) ) {
			p_dml_v6addr->iana_pretm = atoi(out);
		}
		else {
			p_dml_v6addr->iana_pretm = 0;
		}

		if (!commonSyseventGet(COSA_DML_DHCPV6C_ADDR_VLDTM_SYSEVENT_NAME, out, sizeof(out)) ) {
			p_dml_v6addr->iana_vldtm = atoi(out);
		}
		else {
			p_dml_v6addr->iana_vldtm = 0;
		}
	  }

        p_dml_v6addr->bAnycast = FALSE;

        g_ipif_be_bufs[ulIndex].ulNumOfV6Addr++;        
    }

    
    /*
      the following block is for static address

      for static addresses, we store them in syscfg.
     the syscfg structure:
     Key                        : Value
    SYSCFG_FORMAT_STATIC_V6ADDR : "inst_num1,inst_num2,..."
    
    SYSCFG_FORMAT_NAMESPACE_STATIC_V6ADDR::(enable, alias, prefix, valid_lft, prefered_lft, anycast)    
    */

    if (Utopia_Init(&utctx))
    {
        safec_rc = sprintf_s(buf, sizeof(buf), SYSCFG_FORMAT_STATIC_V6ADDR, (char *)g_ipif_names[ulIndex]);
        if(safec_rc < EOK)
        {
           ERR_CHK(safec_rc);
        }
        memset(out, 0, sizeof(out));
        Utopia_RawGet(&utctx,NULL,buf,out,sizeof(out));
        Utopia_Free(&utctx,0);        
    }
    
    if (out[0])
    {
        char val[256] = {0};
        int  index = g_ipif_be_bufs[ulIndex].ulNumOfV6Addr;
        char * str = NULL;
        char * saveptr = NULL;
        ULONG  inst_num = 0;
        char * p_token = NULL;

        for (str = out; ; str = NULL) 
        {
            p_token = strtok_r(str, ",", &saveptr);
            if (p_token == NULL)
                break;

            inst_num = atoi(p_token);
            if (!inst_num)
                continue;

            if (index >= MAX_IPV6_ENTRY_NUM)
                break;
            else 
                p_dml_v6addr = &g_ipif_be_bufs[ulIndex].V6AddrList[index];

            snprintf(namespace, sizeof(namespace)-1, SYSCFG_FORMAT_NAMESPACE_STATIC_V6ADDR, (char *)g_ipif_names[ulIndex], inst_num);

            if (Utopia_Init(&utctx))
            {
                memset(val, 0, sizeof(val));
                Utopia_RawGet(&utctx,namespace,"IPAddress",val,sizeof(val));
                if (!val[0])
                {
                    Utopia_Free(&utctx,0);        
                    continue;
                }
                safec_rc = strcpy_s(p_dml_v6addr->IP6Address, sizeof(p_dml_v6addr->IP6Address), val);
                if(safec_rc != EOK)
                {
                   ERR_CHK(safec_rc);
                }
                p_dml_v6addr->InstanceNumber = inst_num;

                memset(val, 0, sizeof(val));
                Utopia_RawGet(&utctx,namespace,"Alias",val,sizeof(val));
                safec_rc = strcpy_s(p_dml_v6addr->Alias, sizeof(p_dml_v6addr->Alias), val);
                if(safec_rc != EOK)
                {
                   ERR_CHK(safec_rc);
                }
                memset(val, 0, sizeof(val));
                Utopia_RawGet(&utctx,namespace,"Enable",val,sizeof(val));
                p_dml_v6addr->bEnabled = (atoi(val) == 0) ? FALSE : TRUE;

                if (p_dml_v6addr->bEnabled)
                    p_dml_v6addr->Status = COSA_DML_IP_ADDR_STATUS_Enabled;
                else
                    p_dml_v6addr->Status = COSA_DML_IP_ADDR_STATUS_Disabled;

                p_dml_v6addr->Origin = COSA_DML_IP6_ORIGIN_Static;

                memset(val, 0, sizeof(val));
                Utopia_RawGet(&utctx,namespace,"Prefix",val,sizeof(val));
                safec_rc = strcpy_s(p_dml_v6addr->Prefix, sizeof(p_dml_v6addr->Prefix), val);
                if(safec_rc != EOK)
                {
                  ERR_CHK(safec_rc);
                }

                memset(val, 0, sizeof(val));
                Utopia_RawGet(&utctx,namespace,"PreferredLifetime",val,sizeof(val));
                safec_rc = strcpy_s(p_dml_v6addr->PreferredLifetime, sizeof(p_dml_v6addr->PreferredLifetime), val);
                if(safec_rc != EOK)
                {
                  ERR_CHK(safec_rc);
                }
                memset(val, 0, sizeof(val));
                Utopia_RawGet(&utctx,namespace,"ValidLifetime",val,sizeof(val));
                safec_rc = strcpy_s(p_dml_v6addr->ValidLifetime, sizeof(p_dml_v6addr->ValidLifetime), val);
                if(safec_rc != EOK)
                {
                  ERR_CHK(safec_rc);
                }
                memset(val, 0, sizeof(val));
                Utopia_RawGet(&utctx,namespace,"Anycast",val,sizeof(val));
                p_dml_v6addr->bAnycast = (atoi(val) == 0) ? FALSE:TRUE;

                Utopia_Free(&utctx,0);        
            }
            /*if this entry is enabled, add it to system*/
            if (p_dml_v6addr->bEnabled && p_dml_v6addr->Prefix[0])
            {
                /*in this early stage, we can't get prefix length from Mpr interface, we have to calculate it from g_ipif_be_bufs*/
                int early_pref_len = 0;

                _get_early_pref_len(&early_pref_len, ulIndex, (char *)p_dml_v6addr->Prefix, (char *)p_dml_v6addr->IP6Address);

                CcspTraceInfo(("Adding static IPv6 address, pref_len %s:%d\n", p_dml_v6addr->Prefix, early_pref_len));
                
                _invoke_ip_cmd("add", NULL, p_dml_v6addr, (char *)g_ipif_names[ulIndex], &early_pref_len);
            }
            else
                CcspTraceInfo(("failed to add static V6Address, enable:Prefix %d:%s\n", p_dml_v6addr->bEnabled, p_dml_v6addr->Prefix));

            g_ipif_be_bufs[ulIndex].ulNumOfV6Addr++;

            index++;
        }
    }


    if (orig_p_v6addr)  
        free(orig_p_v6addr);

    if (p_ra)
        free(p_ra);
 
    return ret;
}

/*for non-static v6addr uses this func to find the matched prefix*/
int CosaDmlGetPrefixPathName(char * ifname, int inst1, PCOSA_DML_IP_V6ADDR p_dml_v6addr, char * p_val)
{
    int   i = 0;
    int   inst2 = 0;
    char  name[256] = {0};
    int   num = 0;
    ipv6_addr_info_t * p_v6addr = NULL;
    ipv6_addr_info_t * orig_p_v6addr = NULL;
    int   v6addr_num = 0;
    char  prefix[64] = {0};
    char  param_val[256] = {0};
    ULONG val_len = sizeof(param_val);
    int   found = 0;
    errno_t safec_rc = -1;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    if (!p_dml_v6addr || !p_val)
        return -1;

    /*for DHCPv6 IANA, prefix is empty*/
    if (p_dml_v6addr->Origin == COSA_DML_IP6_ORIGIN_DHCPv6)
    {
        p_val[0] = 0;
        return 0;
    }
    
    found = 0;
    CosaUtilGetIpv6AddrInfo(ifname,  &p_v6addr, &v6addr_num);
    orig_p_v6addr = p_v6addr;
    
    for (i = 0; i < v6addr_num; i++)
    {
        if (!strncmp(p_v6addr->v6addr, p_dml_v6addr->IP6Address, sizeof(p_v6addr->v6addr)))
        {
            safec_rc = strcpy_s(prefix, sizeof(prefix), p_v6addr->v6pre);
            if(safec_rc != EOK)
            {
               ERR_CHK(safec_rc);
            }
            found = 1;
            break;
        }
        p_v6addr++;
    }
    
    if (orig_p_v6addr)
        free(orig_p_v6addr);

    if (!found)
        return -2;

    safec_rc = sprintf_s(name, sizeof(name), "Device.IP.Interface.%d.IPv6PrefixNumberOfEntries", inst1);
    if(safec_rc < EOK)
    {
       ERR_CHK(safec_rc);
    }
	
    num = g_GetParamValueUlong(g_pDslhDmlAgent, name);

    for (i=0; i<num; i++)
    {
        safec_rc = sprintf_s(name, sizeof(name), "Device.IP.Interface.%d.IPv6Prefix.", inst1);
        if(safec_rc < EOK)
        {
           ERR_CHK(safec_rc);
        }
        inst2 = g_GetInstanceNumberByIndex(g_pDslhDmlAgent, name, i);

        if ( inst2 )
        {

            safec_rc = sprintf_s(name, sizeof(name), "Device.IP.Interface.%d.IPv6Prefix.%d.Origin", inst1, inst2);
            if(safec_rc < EOK)
            {
               ERR_CHK(safec_rc);
            }
            val_len = sizeof(param_val);               
            if (g_GetParamValueString(g_pDslhDmlAgent, name, param_val, &val_len) ||
                 AnscEqualString(param_val, "Static", TRUE ) )
                continue;
            
            safec_rc = sprintf_s(name, sizeof(name), "Device.IP.Interface.%d.IPv6Prefix.%d.Prefix", inst1, inst2);
            if(safec_rc < EOK)
            {
               ERR_CHK(safec_rc);
            }
            val_len = sizeof(param_val);               
            if ( ( 0 == g_GetParamValueString(g_pDslhDmlAgent, name, param_val, &val_len)) &&
                 !__v6pref_mismatches(param_val, prefix))
            {
                safec_rc = sprintf_s(p_val, PVAL_MAX_SIZE, "Device.IP.Interface.%d.IPv6Prefix.%d.", inst1, inst2);
                if(safec_rc < EOK)
                {
                   ERR_CHK(safec_rc);
                }
                break;
            }
        }
    }

    

    return 0;
}

static int _get_dynamic_prefix_status(COSA_DML_IP6PREFIX_STATUS_TYPE * p_status, char * ifname, ipv6_addr_info_t * p_v6addr)
{
    int  valid_lft = 0, prefered_lft = 0;

    /*it's hard to use iproute2 C code to obtain 2 lifetimes, here call "ip" directly*/
    v_secure_system("ip -6 addr show dev %s > " TMP_IP_OUTPUT, ifname);
            
    _get_2_lfts(TMP_IP_OUTPUT, &valid_lft, &prefered_lft, p_v6addr);
    
    if (valid_lft <= 0 && (valid_lft != ((int)0xffffffff)))
        *p_status = COSA_DML_IP6PREFIX_STATUS_Invalid;
    else
    {
        if (prefered_lft <= 0 && (prefered_lft != ((int)0xffffffff)))
            *p_status = COSA_DML_IP6PREFIX_STATUS_Deprecated;
        else
            *p_status = COSA_DML_IP6PREFIX_STATUS_Preferred;
    }

    return 0;
}

int __find_iana_addr_info(char * ifname, PCOSA_DML_IP_V6ADDR p_dml_v6addr, ipv6_addr_info_t * p_v6_ai)
{
    ipv6_addr_info_t * p_all_ai = NULL;
    int v6addr_num = 0;
    int found = 0;

    CosaUtilGetIpv6AddrInfo(ifname, &p_all_ai, &v6addr_num);

    if (p_all_ai)
    {
        int i = 0;
        
        for (i=0; i<v6addr_num; i++)
        {
            if (!__v6addr_mismatch(p_all_ai[i].v6addr, p_dml_v6addr->IP6Address, 128))
                break;
        }
        
        if (i < v6addr_num)
        {
            *p_v6_ai = p_all_ai[i];
            found = 1;
        }
        free(p_all_ai);
    }

    return found;
}

ULONG CosaDmlIPv6addrGetV6Status(PCOSA_DML_IP_V6ADDR p_dml_v6addr, PCOSA_DML_IP_IF_FULL2 p_ipif)
{
    int  valid_lft = 0, prefered_lft = 0;
    ipv6_addr_info_t v6_ai;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    errno_t safec_rc = strcpy_s(v6_ai.v6addr, sizeof(v6_ai.v6addr), p_dml_v6addr->IP6Address);
    if(safec_rc != EOK)
    {
       ERR_CHK(safec_rc);
    }

    /*for loopback and link-local, always Preferred*/
    if (p_dml_v6addr->Origin == COSA_DML_IP6_ORIGIN_WellKnown
        || (!strncmp(p_dml_v6addr->IP6Address, "fe80", 4)))
        return COSA_DML_IP6_ADDRSTATUS_Preferred;        
    
    if (!p_dml_v6addr->Prefix[0] && p_dml_v6addr->Origin != COSA_DML_IP6_ORIGIN_Static)
        CosaDmlGetPrefixPathName(p_ipif->Info.Name, p_ipif->Cfg.InstanceNumber, p_dml_v6addr, p_dml_v6addr->Prefix);

    if (p_dml_v6addr->Origin == COSA_DML_IP6_ORIGIN_DHCPv6)
    {
        if (!__find_iana_addr_info(p_ipif->Info.Name, p_dml_v6addr, &v6_ai))
            return COSA_DML_IP6_ADDRSTATUS_Invalid;   
    }
    else if (!p_dml_v6addr->Prefix[0])
        return COSA_DML_IP6_ADDRSTATUS_Invalid;   
    else 
    {
        char name[256] = {0};
        char val[256] = {0};
        ULONG len = sizeof(val);
        snprintf(name, sizeof(name), "%sPrefix", p_dml_v6addr->Prefix);
        if ( 0 == g_GetParamValueString(g_pDslhDmlAgent, name, val, &len))
        {
            safec_rc = strcpy_s(v6_ai.v6pre, sizeof(v6_ai.v6pre), val);
            if(safec_rc != EOK)
            {
               ERR_CHK(safec_rc);
            }
        }
        else
            return COSA_DML_IP6_ADDRSTATUS_Invalid;   
    }

    /*it's hard to use iproute2 C code to obtain 2 lifetimes, here call "ip" directly*/
    v_secure_system("ip -6 addr show dev %s > " TMP_IP_OUTPUT, p_ipif->Info.Name);

    _get_2_lfts(TMP_IP_OUTPUT, &valid_lft, &prefered_lft, &v6_ai);

    if (valid_lft <= 0 && (valid_lft != (int)0xffffffff))
        return COSA_DML_IP6_ADDRSTATUS_Invalid;
    else
    {
        if (prefered_lft <= 0 && (prefered_lft != (int)0xffffffff))
            return COSA_DML_IP6_ADDRSTATUS_Deprecated;
        else
            return COSA_DML_IP6_ADDRSTATUS_Preferred;
    }

    return COSA_DML_IP6_ADDRSTATUS_Invalid;
}
#ifdef CISCO_CONFIG_DHCPV6_PREFIX_DELEGATION
PCOSA_DML_IP_V6ADDR
CosaDmlIPGetIPv6Addresses
    (
        PCOSA_DML_IP_IF_FULL2       p_ipif,
        PULONG                      p_num
    )
{
    int i = 0;
    PCOSA_DML_IP_V6ADDR p_dml_addr = NULL;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    if (!p_ipif || !p_num)
        return NULL;

#if defined (MULTILAN_FEATURE)
    if (p_ipif->Cfg.InstanceNumber > COSA_USG_IF_NUM) {
        /*current only support one global ipv6 addr for multilan*/
        *p_num = 1;
        p_dml_addr = (PCOSA_DML_IP_V6ADDR)AnscAllocateMemory(*p_num * sizeof(COSA_DML_IP_V6ADDR));

        if (!p_dml_addr)
            return NULL;

        p_dml_addr->InstanceNumber = 1;
        CosaDmlIpIfMlanGetV6Addr2(NULL, p_ipif->Cfg.InstanceNumber, p_dml_addr);
    } else
#else
    {

        for (i=0; i<g_ipif_num; i++)
            if (!strncmp(g_ipif_names[i], p_ipif->Info.Name, sizeof(p_ipif->Info.Name)))
                break;

        if (i == g_ipif_num)
            return NULL;

        IPIF_getEntry_for_Ipv6Addr(p_ipif, i);

        *p_num = g_ipif_be_bufs[i].ulNumOfV6Addr;

        if (*p_num)
        {
            p_dml_addr = (PCOSA_DML_IP_V6ADDR)AnscAllocateMemory(*p_num * sizeof(COSA_DML_IP_V6ADDR));

            if (!p_dml_addr)
                return NULL;

            AnscCopyMemory(p_dml_addr, g_ipif_be_bufs[i].V6AddrList, *p_num*sizeof(COSA_DML_IP_V6ADDR));
        }

    }
#endif
    return p_dml_addr;
}
#else
PCOSA_DML_IP_V6ADDR
CosaDmlIPGetIPv6Addresses
    (
        PCOSA_DML_IP_IF_FULL2       p_ipif,
        PULONG                      p_num
    )
{
    int i = 0;
    PCOSA_DML_IP_V6ADDR p_dml_addr = NULL;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    if (!p_ipif || !p_num)
        return NULL;
#if defined (MULTILAN_FEATURE)
    if (p_ipif->Cfg.InstanceNumber > COSA_USG_IF_NUM) {
        /*current only support one global ipv6 addr for multilan*/
        *p_num = 1;
#else
    for (i=0; i<g_ipif_num; i++)
        if (!strncmp(g_ipif_names[i], p_ipif->Info.Name, sizeof(p_ipif->Info.Name)))
            break;

    if (i == g_ipif_num)
        return NULL;
    
    IPIF_getEntry_for_Ipv6Addr(p_ipif, i);

    *p_num = g_ipif_be_bufs[i].ulNumOfV6Addr;
    
    if (*p_num)
    {	
#endif
        p_dml_addr = (PCOSA_DML_IP_V6ADDR)AnscAllocateMemory(*p_num * sizeof(COSA_DML_IP_V6ADDR));

        if (!p_dml_addr)
            return NULL;
#if defined (MULTILAN_FEATURE)
        p_dml_addr->InstanceNumber = 1;
        CosaDmlIpIfMlanGetV6Addr2(NULL, p_ipif->Cfg.InstanceNumber, p_dml_addr);

        _get_datetime_offset(p_dml_addr->iana_pretm, p_dml_addr->PreferredLifetime, sizeof(p_dml_addr->PreferredLifetime));
        _get_datetime_offset(p_dml_addr->iana_vldtm, p_dml_addr->ValidLifetime, sizeof(p_dml_addr->ValidLifetime));

        if(p_ipif->ulNextIPV6InsNum <= p_dml_addr->InstanceNumber)
            p_ipif->ulNextIPV6InsNum = p_dml_addr->InstanceNumber + 1;

    } else {

        for (i=0; i<g_ipif_num; i++) {
            if (!strncmp(g_ipif_names[i], p_ipif->Info.Name, sizeof(p_ipif->Info.Name))) {
                break;
            }
        }

        if (i == g_ipif_num)
            return NULL;
    
        IPIF_getEntry_for_Ipv6Addr(p_ipif, i);

        *p_num = g_ipif_be_bufs[i].ulNumOfV6Addr;
    
        if (*p_num)
        {
            p_dml_addr = (PCOSA_DML_IP_V6ADDR)AnscAllocateMemory(*p_num * sizeof(COSA_DML_IP_V6ADDR));
    
            if (!p_dml_addr)
                return NULL;
#endif
            AnscCopyMemory(p_dml_addr, g_ipif_be_bufs[i].V6AddrList, *p_num*sizeof(COSA_DML_IP_V6ADDR));
        }
#if defined (MULTILAN_FEATURE)
    }
#endif
    return p_dml_addr;
}

#endif


static int
IPIF_getEntry_for_Ipv6Pre
    (
        PCOSA_DML_IP_IF_FULL2       p_ipif,
        ULONG ulIndex
    )
{
    char buf[256]= {0};
    char buf1[256]= {0};
    char out[1024]= {0};
    UtopiaContext utctx = {0};
    ipv6_addr_info_t * p_v6addr = NULL;
    ipv6_addr_info_t * orig_p_v6addr = NULL;
    int  v6addr_num = 0;
    int  i = 0;
    COSA_DML_IP_V6PREFIX * p_dml_v6pre = NULL;
    int  ret = 0;
    exported_ra_info_t  * p_ra = NULL;
    int  ra_num = 0;
    int  j = 0;
    int  ra_index = -1;
    char dhcpv6_pref[64] = {0};
    int  need_write = 0;
    errno_t safec_rc = -1;


    CosaUtilGetIpv6AddrInfo((char *)g_ipif_names[ulIndex], &p_v6addr, &v6addr_num);

    g_ipif_be_bufs[ulIndex].ulNumOfV6Pre = 0;
    _obtain_ra_info(&p_ra, &ra_num);

    /*save for free*/
    orig_p_v6addr = p_v6addr;

    /*for RA prefixes
      note, for DHCPv6 IANA address, the according prefix should be empty, */
    for (i=0; i<v6addr_num && i<MAX_IPV6_ENTRY_NUM; i++, p_v6addr++)
    {
      
        /*according to tr181, we don't need to support WellKnown prefixes.*/
        if (p_v6addr->scope == IPV6_ADDR_SCOPE_LINKLOCAL ||
            p_v6addr->scope == IPV6_ADDR_SCOPE_LOOPBACK)
            continue;

        ra_index = -1;
        
        p_dml_v6pre = &g_ipif_be_bufs[ulIndex].V6PreList[g_ipif_be_bufs[ulIndex].ulNumOfV6Pre];


        p_dml_v6pre->Origin = 0;

        for (j = 0; j< ra_num ; j++)
        {
            if (!__v6pref_mismatches(p_v6addr->v6pre, p_ra[j].pref))
            {
                p_dml_v6pre->Origin = COSA_DML_IP6PREFIX_ORIGIN_RouterAdvertisement;
                ra_index = j;
                break;
            }
        }

        _get_datetime_lfts((char *)p_dml_v6pre->PreferredLifetime, sizeof(p_dml_v6pre->PreferredLifetime),
                           (char *)p_dml_v6pre->ValidLifetime, sizeof(p_dml_v6pre->ValidLifetime),
                           p_v6addr, ulIndex);

        if (!p_dml_v6pre->Origin)
        {
            continue;
        }

        if (Utopia_Init(&utctx))
        {
            need_write = 0;

            safec_rc = sprintf_s(buf, sizeof(buf), SYSCFG_FORMAT_NORMAL_V6PREFIX"_inst_num", g_ipif_names[ulIndex], p_v6addr->v6pre);
            if(safec_rc < EOK)
            {
               ERR_CHK(safec_rc);
            }
            safec_rc = sprintf_s(buf1, sizeof(buf1), SYSCFG_FORMAT_NORMAL_V6PREFIX"_alias", g_ipif_names[ulIndex], p_v6addr->v6pre);
            if(safec_rc < EOK)
            {
                ERR_CHK(safec_rc);
            }
            _load_v6_alias_instNum(&utctx, buf, buf1, &p_dml_v6pre->InstanceNumber, p_dml_v6pre->Alias, sizeof(p_dml_v6pre->Alias));

            if (!p_dml_v6pre->InstanceNumber)
            {
                need_write = 1;
                CosaIPv6PrefGenInstAlias(p_ipif, p_dml_v6pre);
                _save_v6_alias_instNum(&utctx, buf, buf1, p_dml_v6pre->InstanceNumber, p_dml_v6pre->Alias);
            }
            Utopia_Free(&utctx,need_write);        
        }

        p_dml_v6pre->bEnabled = TRUE;
        
        p_dml_v6pre->Status = COSA_DML_PREFIXENTRY_STATUS_Enabled;

        safec_rc = strcpy_s(p_dml_v6pre->Prefix, sizeof(p_dml_v6pre->Prefix), p_v6addr->v6pre);
        if(safec_rc != EOK)
        {
           ERR_CHK(safec_rc);
        }
        _get_dynamic_prefix_status(&p_dml_v6pre->PrefixStatus, (char *)g_ipif_names[ulIndex], p_v6addr);
                    
        p_dml_v6pre->StaticType = COSA_DML_IP6PREFIX_STATICTYPE_Inapplicable;
        
        p_dml_v6pre->ParentPrefix[0] = 0;

        p_dml_v6pre->ChildPrefixBits[0] = 0;

        /*normally these 2 fields will be TRUE, I don't believe it's practical to set these fields to FALSE*/
#ifdef SKY_IPV6
        if (ra_index > -1)
        {
            p_dml_v6pre->bOnlink = p_ra[ra_index].onlink;
            p_dml_v6pre->bAutonomous  = p_ra[ra_index].autoconf;
        }
#else
        p_dml_v6pre->bOnlink = p_ra[ra_index].onlink;
        p_dml_v6pre->bAutonomous  = p_ra[ra_index].autoconf;
#endif

        g_ipif_be_bufs[ulIndex].ulNumOfV6Pre++;        
    }


    /*for dhcpv6 IAPD prefix, IAPD prefix won't show in ifconfig, it only exists in sysevent*/
    do
    {
        if (g_ipif_be_bufs[ulIndex].ulNumOfV6Pre >= MAX_IPV6_ENTRY_NUM)
            break;

#ifdef CISCO_CONFIG_DHCPV6_PREFIX_DELEGATION        
  #if defined _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_
        /* We just put this prefix into erouter0 && brlan0 entry */
        if ( ulIndex > 0 && ulIndex != 3)
            break;
  #endif

        p_dml_v6pre = &g_ipif_be_bufs[ulIndex].V6PreList[g_ipif_be_bufs[ulIndex].ulNumOfV6Pre];

        if (ulIndex == 3)
            commonSyseventGet("ipv6_brlan0-prefix", dhcpv6_pref, sizeof(dhcpv6_pref));
        else
            commonSyseventGet(COSA_DML_DHCPV6C_PREF_SYSEVENT_NAME, dhcpv6_pref, sizeof(dhcpv6_pref));

        if (dhcpv6_pref[0])
            p_dml_v6pre->Origin = (ulIndex == 0) ? COSA_DML_IP6PREFIX_ORIGIN_PrefixDelegation : COSA_DML_IP6PREFIX_ORIGIN_Child;
        else 
            break;
#else
  #ifdef _COSA_INTEL_USG_ARM_
        /* We just put this prefix into erouter0 entry */
        if ( ulIndex > 0 )
            break;
#endif

        p_dml_v6pre = &g_ipif_be_bufs[ulIndex].V6PreList[g_ipif_be_bufs[ulIndex].ulNumOfV6Pre];

        
        commonSyseventGet(COSA_DML_DHCPV6C_PREF_SYSEVENT_NAME, dhcpv6_pref, sizeof(dhcpv6_pref));
        if (dhcpv6_pref[0])
            p_dml_v6pre->Origin = COSA_DML_IP6PREFIX_ORIGIN_PrefixDelegation;
        else 
            break;


#endif        

        if (Utopia_Init(&utctx))
        {
            need_write = 0;             
            safec_rc = sprintf_s(buf, sizeof(buf), SYSCFG_FORMAT_NORMAL_V6PREFIX"_inst_num", g_ipif_names[ulIndex], dhcpv6_pref);
            if(safec_rc < EOK)
            {
               ERR_CHK(safec_rc);
            }
            safec_rc = sprintf_s(buf1, sizeof(buf1), SYSCFG_FORMAT_NORMAL_V6PREFIX"_alias", g_ipif_names[ulIndex], dhcpv6_pref);
            if(safec_rc < EOK)
            {
               ERR_CHK(safec_rc);
            }
            _load_v6_alias_instNum(&utctx, buf, buf1, &p_dml_v6pre->InstanceNumber, p_dml_v6pre->Alias, sizeof(p_dml_v6pre->Alias));

            /*This instance Number should be fixed to 1 because other place will refer it*/
            p_dml_v6pre->InstanceNumber = g_ipif_be_bufs[ulIndex].ulNumOfV6Pre + 1;
            safec_rc = sprintf_s(p_dml_v6pre->Alias, sizeof(p_dml_v6pre->Alias), "IPv6Prefix%lu", p_dml_v6pre->InstanceNumber );
            if(safec_rc < EOK)
            {
               ERR_CHK(safec_rc);
            }
            if ( p_dml_v6pre->InstanceNumber != 1 ) {
                AnscTrace("The Instance Number is not 1, error ,DHCPv6 Server will not work.\n");
                p_dml_v6pre->InstanceNumber = 1;
                safec_rc = sprintf_s(p_dml_v6pre->Alias, sizeof(p_dml_v6pre->Alias), "IPv6Prefix%lu", p_dml_v6pre->InstanceNumber );
                if(safec_rc < EOK)
                {
                   ERR_CHK(safec_rc);
                }
            }

            if (!p_dml_v6pre->InstanceNumber)
            {
                CosaIPv6PrefGenInstAlias(p_ipif, p_dml_v6pre);
                need_write = 1;
                _save_v6_alias_instNum(&utctx, buf, buf1, p_dml_v6pre->InstanceNumber, p_dml_v6pre->Alias);
            }

            Utopia_Free(&utctx,need_write);        
        }

        p_dml_v6pre->bEnabled = TRUE;
        
        safec_rc = strcpy_s(p_dml_v6pre->Prefix, sizeof(p_dml_v6pre->Prefix), dhcpv6_pref);
        if(safec_rc != EOK)
        {
           ERR_CHK(safec_rc);
        }
        if (!commonSyseventGet(COSA_DML_DHCPV6C_PREF_IAID_SYSEVENT_NAME, out, sizeof(out)) )
             g_ipif_be_bufs[ulIndex].Info.iapd_iaid  = atoi(out);
        else
             g_ipif_be_bufs[ulIndex].Info.iapd_iaid  = 0;
    
        if (!commonSyseventGet(COSA_DML_DHCPV6C_PREF_T1_SYSEVENT_NAME, out, sizeof(out)) )
             g_ipif_be_bufs[ulIndex].Info.iapd_t1 = atoi(out);
        else
             g_ipif_be_bufs[ulIndex].Info.iapd_t1  = 0;
    
        if (!commonSyseventGet(COSA_DML_DHCPV6C_PREF_T2_SYSEVENT_NAME, out, sizeof(out)) )
             g_ipif_be_bufs[ulIndex].Info.iapd_t2 = atoi(out);
        else
             g_ipif_be_bufs[ulIndex].Info.iapd_t2  = 0;
    
        if (!commonSyseventGet(COSA_DML_DHCPV6C_PREF_PRETM_SYSEVENT_NAME, out, sizeof(out)) )
             p_dml_v6pre->iapd_pretm = atoi(out);
        else
             p_dml_v6pre->iapd_pretm = 0;
    
        if (!commonSyseventGet(COSA_DML_DHCPV6C_PREF_VLDTM_SYSEVENT_NAME, out, sizeof(out)) )
             p_dml_v6pre->iapd_vldtm = atoi(out);
        else
             p_dml_v6pre->iapd_vldtm = 0;

        p_dml_v6pre->Status = COSA_DML_PREFIXENTRY_STATUS_Enabled;

        /*todo: get correct prefixStatus*/
        p_dml_v6pre->PrefixStatus = COSA_DML_IP6PREFIX_STATUS_Preferred;
        
        p_dml_v6pre->StaticType = COSA_DML_IP6PREFIX_STATICTYPE_Inapplicable;
        
        p_dml_v6pre->ParentPrefix[0] = 0;

        p_dml_v6pre->ChildPrefixBits[0] = 0;

        p_dml_v6pre->bOnlink = TRUE;
        p_dml_v6pre->bAutonomous  = TRUE;

        g_ipif_be_bufs[ulIndex].ulNumOfV6Pre++;        
    }while(0);


    /*
      the following block is for static prefix

      for static prefixes, we store them in syscfg.
     the syscfg structure:
     Key                        : Value
    SYSCFG_FORMAT_STATIC_V6PREF : "inst_num1,inst_num2,..."
    
    SYSCFG_FORMAT_NAMESPACE_STATIC_V6PREFIX::(enable, alias)    
    */

    if (Utopia_Init(&utctx))
    {
        safec_rc = sprintf_s(buf, sizeof(buf), SYSCFG_FORMAT_STATIC_V6PREF, (char *)g_ipif_names[ulIndex]);
        if(safec_rc < EOK)
        {
           ERR_CHK(safec_rc);
        }
        memset(out, 0, sizeof(out));
        Utopia_RawGet(&utctx,NULL,buf,out,sizeof(out));
        Utopia_Free(&utctx,0);                
    }    
    if (out[0])
    {
        char val[256] = {0};
        char namespace[256] = {0};
        int  index = g_ipif_be_bufs[ulIndex].ulNumOfV6Pre;
        char * str = NULL;
        char * saveptr = NULL;
        char * p_token = NULL;
        int  inst_num = 0;

        for (str = out; ; str = NULL) 
        {
            p_token = strtok_r(str, ",", &saveptr);
            if (p_token == NULL)
                break;

            inst_num = atoi(p_token);
            if (!inst_num)
                continue;

            if (index >= MAX_IPV6_ENTRY_NUM)
                break;
            else
                p_dml_v6pre = &g_ipif_be_bufs[ulIndex].V6PreList[index];            

            snprintf(namespace, sizeof(namespace)-1, SYSCFG_FORMAT_NAMESPACE_STATIC_V6PREFIX, (char *)g_ipif_names[ulIndex], (ULONG)inst_num);

            p_dml_v6pre->InstanceNumber = inst_num;

            if (Utopia_Init(&utctx))
            {
                Utopia_RawGet(&utctx,namespace,"Prefix",val,sizeof(val));
                if (!val[0])
                {
                    Utopia_Free(&utctx,0);        
                    continue;
                }
                safec_rc = strcpy_s(p_dml_v6pre->Prefix, sizeof(p_dml_v6pre->Prefix), val);
                if(safec_rc != EOK)
                {
                    ERR_CHK(safec_rc);
                }  
                memset(val, 0, sizeof(val));
                Utopia_RawGet(&utctx,namespace,"Alias",val,sizeof(val));
                safec_rc = strcpy_s(p_dml_v6pre->Alias, sizeof(p_dml_v6pre->Alias), val);
                if(safec_rc != EOK)
                {
                    ERR_CHK(safec_rc);
                }              
                memset(val, 0, sizeof(val));
                Utopia_RawGet(&utctx,namespace,"Enable",val,sizeof(val));
                p_dml_v6pre->bEnabled = (atoi(val) == 1) ? TRUE : FALSE;

                Utopia_Free(&utctx,0);                
            }

            if (p_dml_v6pre->bEnabled)
                p_dml_v6pre->Status = COSA_DML_PREFIXENTRY_STATUS_Enabled;
            else
                p_dml_v6pre->Status = COSA_DML_PREFIXENTRY_STATUS_Disabled;
            
            /*for static IPv6 prefix, prefixStatus is useless, we only count on the corresponding IPv6 address's ipAddressStatus*/
            p_dml_v6pre->PrefixStatus = COSA_DML_IP6PREFIX_STATUS_Preferred;

            p_dml_v6pre->Origin = COSA_DML_IP6PREFIX_ORIGIN_Static;
            
            /*for now, only support pure static prefix, no IA_PD purpose*/
            p_dml_v6pre->StaticType = COSA_DML_IP6PREFIX_STATICTYPE_Static;

            p_dml_v6pre->ParentPrefix[0] = 0;
            
            p_dml_v6pre->ChildPrefixBits[0] = 0;
        
            p_dml_v6pre->bOnlink = TRUE;

            p_dml_v6pre->bAutonomous  = TRUE;

            /*for static IPv6 prefix, the lifetime is useless, we refer to the corresponding IPv6Address's lifetime */
            safec_rc = strcpy_s(p_dml_v6pre->PreferredLifetime, sizeof(p_dml_v6pre->PreferredLifetime), "0001-01-01T00:00:00Z");
            if(safec_rc != EOK)
            {
                ERR_CHK(safec_rc);
            }  
            safec_rc = strcpy_s(p_dml_v6pre->ValidLifetime, sizeof(p_dml_v6pre->ValidLifetime), "0001-01-01T00:00:00Z");
            if(safec_rc != EOK)
            {
                ERR_CHK(safec_rc);
            }  
            g_ipif_be_bufs[ulIndex].ulNumOfV6Pre++;

            index++;
        }
    }


    if (orig_p_v6addr)
        free(orig_p_v6addr);

    if (p_ra)
        free(p_ra);
   
    return ret;
}


PCOSA_DML_IP_IF_INFO
CosaDmlIpIfGetEntry2
    (
        ANSC_HANDLE                 hContext,
        ULONG                       InstanceNumber
    )
{
    UNREFERENCED_PARAMETER(hContext);
    /*It's sure that index is the InstanceNumber - 1*/
    return &g_ipif_be_bufs[InstanceNumber-1].Info;
}

ANSC_STATUS
CosaDmlIpIfGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_IP_IF_FULL        pEntry
    )
{

    int i;
    long uptime2 = 0;
    errno_t safec_rc = -1;
    AnscTraceFlow(("%s...\n", __FUNCTION__));

    if ( ulIndex >= (COSA_USG_IF_NUM - 1) )
    {
        return  CosaDmlIpIfMlanGetEntry(hContext, (ulIndex - (COSA_USG_IF_NUM - 1)), pEntry);
    }

    pEntry->Cfg.InstanceNumber = 0;
    for(i=0; i < COSA_USG_IF_NUM; i++){
        if(strcmp((char *)g_ipif_names[ulIndex], G_USG_IF_NAME(i)) == 0){
            pEntry->Cfg.InstanceNumber = 1 + i;
            break;
        }
    }
    if (i >= COSA_USG_IF_NUM)
    {
        return ANSC_STATUS_FAILURE;
    }

    /*not supported*/
    pEntry->Cfg.bEnabled            = TRUE;

    /*RDKB-6840,CID-33138, InstanceNumber==0 will lead to memory violation*/
    if(pEntry->Cfg.InstanceNumber > 0)
    {
        pEntry->Cfg.LinkType            = G_USG_IF_LINKTYPE(pEntry->Cfg.InstanceNumber - 1);
    }
    else
    {
        pEntry->Cfg.LinkType            = G_USG_IF_LINKTYPE(pEntry->Cfg.InstanceNumber);
    }

    if (strcmp((char *)g_ipif_names[ulIndex], "lo") == 0)
    {
        pEntry->Cfg.IfType   = COSA_DML_IP_IF_TYPE_Loopback;
        pEntry->Cfg.Loopback = TRUE;

    }
    else 
    {
        pEntry->Cfg.IfType   = COSA_DML_IP_IF_TYPE_Normal;
        pEntry->Cfg.Loopback = FALSE;
    }

    pEntry->Cfg.MaxMTUSize   = CosaUtilIoctlXXX((char *)g_ipif_names[ulIndex], "mtu", NULL);
    pEntry->Cfg.AutoIPEnable = FALSE;
    
    if (!pEntry->Cfg.bEnabled)
    {
        pEntry->Info.Status      = COSA_DML_IF_STATUS_Down;
    }
    else 
    {
        if (CosaUtilIoctlXXX((char *)g_ipif_names[ulIndex], "status", NULL) & IFF_UP) 
        {
            pEntry->Info.Status      = COSA_DML_IF_STATUS_Up;
        }
        else
        {
            pEntry->Info.Status      = COSA_DML_IF_STATUS_LowerLayerDown;
        }
    }

    get_uptime(&uptime2);
    pEntry->Info.LastChange  =  uptime2;
    safec_rc = strcpy_s(pEntry->Info.Name, sizeof(pEntry->Info.Name), (char *)g_ipif_names[ulIndex]);
    ERR_CHK(safec_rc);

    pEntry->Cfg.LinkType            = G_USG_IF_LINKTYPE(i);
    safec_rc = strcpy_s(pEntry->Cfg.LinkName, sizeof(pEntry->Cfg.LinkName), (char *)g_ipif_names[ulIndex]);
    ERR_CHK(safec_rc);

    if (TRUE)
    {
        char buf[256]= {0};
        char out[COSA_DML_IF_NAME_LENGTH]= {0};
        UtopiaContext utctx;

        /*we can't use InstanceNumber to set a alias name to IP.Interface, because different interfaces will have a same instanceNumber when in BYOI or Primary mode*/
        /*CID: 54740 Unchecked return value*/
        if (!Utopia_Init(&utctx))
        {
        return ANSC_STATUS_FAILURE;
        }
        safec_rc = sprintf_s(buf, sizeof(buf), "tr_ip_interface_%s_alias", pEntry->Info.Name);
        if(safec_rc < EOK)
        {
           ERR_CHK(safec_rc);
        }
        Utopia_RawGet(&utctx,NULL,buf,out,sizeof(out));
        Utopia_Free(&utctx,0);

        if (out[0]) 
        {
            _ansc_strncpy(pEntry->Cfg.Alias, out, COSA_DML_IF_NAME_LENGTH-1);  
        }
        else 
        {
            /*if the alias is not set before, we initialize it here rather than get a default value from middile layer*/
            Utopia_Init(&utctx);
            safec_rc = sprintf_s(buf, sizeof(buf), "tr_ip_interface_%s_alias", pEntry->Info.Name);
            if(safec_rc < EOK)
            {
               ERR_CHK(safec_rc);
            }
            safec_rc = sprintf_s(out, sizeof(out), "Interface_%s", pEntry->Info.Name);
            if(safec_rc < EOK)
            {
               ERR_CHK(safec_rc);
            }
            Utopia_RawSet(&utctx,NULL,buf,out);
            Utopia_Free(&utctx,1);

            _ansc_strncpy(pEntry->Cfg.Alias, out, COSA_DML_IF_NAME_LENGTH-1);  
        }
    }

    /* not support */
    pEntry->Cfg.RouterName[0] = '\0';

    /*copy middle layer pEntry to backend buffer*/
    AnscCopyMemory(&g_ipif_be_bufs[ulIndex].Cfg, &pEntry->Cfg, sizeof(pEntry->Cfg));
    AnscCopyMemory(&g_ipif_be_bufs[ulIndex].Info, &pEntry->Info, sizeof(pEntry->Info));

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlIpIfSetValues
            (
                ANSC_HANDLE             hContext,
                ULONG                       ulIndex,
                ULONG                       ulInstanceNumber,
                char*                        pAlias
            );

    Description:

        Backend implementation to set back instance number and alias.

    Arguments:    
                   ANSC_HANDLE                 hContext

                   ULONG                           ulIndex

                   ULONG                           ulInstanceNumber

                   char*                             pAlias

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlIpIfSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    if ( ulIndex >= COSA_USG_IF_NUM )
    {
        return  CosaDmlIpIfMlanSetValues(hContext, ulIndex, ulInstanceNumber, pAlias);
    }
    else
    {
        return ANSC_STATUS_SUCCESS;
    }
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlIpIfAddEntry
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_IP_IF_FULL        pEntry
            );

    Description:
    
        The API adds one IP interface into the system. 
        
    Arguments:   
                ANSC_HANDLE                 hContext,
                
                PCOSA_DML_IP_IF_FULL        pEntry
    Return:
        he pointer to the array of NAT port mappings, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/
ANSC_STATUS
CosaDmlIpIfAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_IP_IF_FULL        pEntry
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;

    if ( pEntry->Cfg.InstanceNumber >= COSA_USG_IF_NUM )
    {
        return  CosaDmlIpIfMlanAddEntry(hContext, pEntry);
    }
    else
    {
        return returnStatus;
    }
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlIpIfDelEntry
            (
                ANSC_HANDLE                 hContext,
               ULONG                            ulInstanceNumber
            );

    Description:
    
        The API delete one IP interface from the system. 
        
    Arguments:
                ANSC_HANDLE                 hContext,
                ULONG                           ulInstanceNumber        

    Return:
        The operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlIpIfDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    
    if ( ulInstanceNumber >= COSA_USG_IF_NUM )
    {
        return  CosaDmlIpIfMlanDelEntry(hContext, ulInstanceNumber);
    }
    else
    {
        return returnStatus;
    }
}


/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlIpIfSetCfg
          (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_IP_IF_CFG         pCfg
            );

    Description:
    
        The API the IP configuration. 
        
    Arguments:
                ANSC_HANDLE                 hContext,
                PCOSA_DML_IP_IF_CFG         pCfg

    Return:
        The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlIpIfSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_IP_IF_CFG         pCfg
    )
{
    if ( pCfg->InstanceNumber >= COSA_USG_IF_NUM )
    {
        return  CosaDmlIpIfMlanSetCfg(hContext, pCfg);
    }
    else
    {
        ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
        PCOSA_DML_IP_IF_CFG             p_be_buf_cfg;
        char                            buf[256] = {0};
        UtopiaContext                   utctx;
        errno_t safec_rc = -1;

        AnscTraceFlow(("%s...\n", __FUNCTION__));

        p_be_buf_cfg = &g_ipif_be_bufs[pCfg->InstanceNumber-1].Cfg;
        
        /*TODO: Enabled is not supported*/
        if (pCfg->bEnabled != p_be_buf_cfg->bEnabled)
        {
        }
        
        if (strcmp(pCfg->Alias, p_be_buf_cfg->Alias) != 0)
        {
            /*CID: 68667 Unchecked return value*/
            if(!Utopia_Init(&utctx))
               return ANSC_STATUS_FAILURE;
            safec_rc = sprintf_s(buf, sizeof(buf), "tr_ip_interface_%s_alias", g_ipif_names[pCfg->InstanceNumber-1]);
            if(safec_rc < EOK)
            {
               ERR_CHK(safec_rc);
            }
            Utopia_RawSet(&utctx,NULL,buf,pCfg->Alias);
            Utopia_Free(&utctx,1);

            safec_rc = strcpy_s(p_be_buf_cfg->Alias,sizeof(p_be_buf_cfg->Alias), pCfg->Alias);
            ERR_CHK(safec_rc);
        }
    #if defined _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_
        if (pCfg->MaxMTUSize != p_be_buf_cfg->MaxMTUSize)
        {
            /*
             *  TBC -- Why do we have to use interface name to tell the role of an interface?
             */
            if (strstr(pCfg->LinkName, "erouter0"))
            {
                if(0 == pCfg->MaxMTUSize)
                    pCfg->MaxMTUSize = 1500;
                CosaUtilIoctlXXX(pCfg->LinkName, "setmtu", &pCfg->MaxMTUSize);
                /*Utopia has wan_mtu support, should turn to Utopia api*/
                UtopiaContext utctx;

                Utopia_Init(&utctx);
                safec_rc = sprintf_s(buf, sizeof(buf), "%lu", pCfg->MaxMTUSize);
                if(safec_rc < EOK)
                {
                   ERR_CHK(safec_rc);
                }
                Utopia_Set(&utctx, UtopiaValue_WAN_MTU, buf);
                Utopia_Free(&utctx, 1);
            }
            else
            {
                /*no Utopia support, do it myself */
                CosaUtilIoctlXXX(pCfg->LinkName, "setmtu", &pCfg->MaxMTUSize);
            }
            
            p_be_buf_cfg->MaxMTUSize = pCfg->MaxMTUSize;
        }
    #else
        if (pCfg->MaxMTUSize != p_be_buf_cfg->MaxMTUSize)
        {
            /*
             *  TBC -- Why do we have to use interface name to tell the role of an interface?
             */
            if (strstr(pIPInterface->Info.Name, "wan"))
            {
                /*Utopia has wan_mtu support, should turn to Utopia api*/
                UtopiaContext utctx;

                Utopia_Init(&utctx);
                safec_rc = sprintf_s(buf, sizeof(buf), "%d", pCfg->MaxMTUSize);
                if(safec_rc < EOK)
                {
                   ERR_CHK(safec_rc);
                }
                UTOPIA_SET(&utctx, UtopiaValue_WAN_MTU, buf);
                Utopia_Free(&utctx, 1);
            }
            else
            {
                /*no Utopia support, do it myself */
                CosaUtilIoctlXXX(pIPInterface->Info.Name, "setmtu", &pCfg->MaxMTUSize);
            }
            
            p_be_buf_cfg->MaxMTUSize = pCfg->MaxMTUSize;
        }
    #endif    
        return returnStatus;
    }
}
/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlIpIfGetCfg
           (
                ANSC_HANDLE                 hContext,
                 PCOSA_DML_IP_IF_CFG         pCfg
           );

    Description:
    
         The API adds one IP interface into the system. 
         
    Arguments:
                ANSC_HANDLE                 hContext,
                
                PCOSA_DML_IP_IF_CFG         pCfg
                
    Return:
        The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlIpIfGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_IP_IF_CFG         pCfg
    )
{

#if defined (_INTEL_MAX_MTU_PROPOSED_FEATURE_)
    char buf[256]= {0};
    UtopiaContext utctx;
#endif

if ( pCfg->InstanceNumber >= COSA_USG_IF_NUM )
    {
        return  CosaDmlIpIfMlanGetCfg(hContext, pCfg);
    }
    else
    {
        ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;

        AnscTraceFlow(("%s...\n", __FUNCTION__));

        /*the reason we can safely use pIPInerface.Cfg.InstanceNumber to refer to real backend buffer is we can't add/del IP.Interface. table, the InstanceNumber is fixed*/
        AnscCopyMemory(pCfg, &g_ipif_be_bufs[pCfg->InstanceNumber-1].Cfg, sizeof(COSA_DML_IP_IF_CFG));
        
#if defined (_INTEL_MAX_MTU_PROPOSED_FEATURE_)
        if (strstr(pCfg->LinkName, "erouter0"))
        {
            Utopia_Init(&utctx);
            Utopia_Get(&utctx, UtopiaValue_WAN_MTU, buf, sizeof(buf));
            Utopia_Free(&utctx,1);
            if (atoi(buf) > 0)
            {
                pCfg->MaxMTUSize = atoi(buf);
            }
        }
        else
        {
            pCfg->MaxMTUSize = CosaUtilIoctlXXX(pCfg->LinkName, "mtu", NULL);
        }
#endif

        return returnStatus;
    }
}
/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlIpIfGetInfo
           (
                ANSC_HANDLE                 hContext,
                ULONG                           ulInstanceNumber,
                PCOSA_DML_IP_IF_INFO        pInfo
           );

    Description:
    
         The API adds one IP interface into the system. 
         
    Arguments:
                ANSC_HANDLE                 hContext,
                
                ULONG                           ulInstanceNumber,
                
                PCOSA_DML_IP_IF_INFO        pInfo

    Return:
        The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlIpIfGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_IP_IF_INFO        pInfo
    )
{
    if ( ulInstanceNumber >= COSA_USG_IF_NUM )
    {
        return  CosaDmlIpIfMlanGetInfo(hContext, ulInstanceNumber, pInfo);
    }
    else
    {
        ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;

        AnscTraceFlow(("%s...\n", __FUNCTION__));

        /*the reason we can safely use pIPInerface.Cfg.InstanceNumber to refer to real backend buffer is we can't add/del IP.Interface. table, the InstanceNumber is fixed*/
        AnscCopyMemory(pInfo, &g_ipif_be_bufs[ulInstanceNumber-1].Info, sizeof(COSA_DML_IP_IF_INFO));
        
        return returnStatus;
    }
}
/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlIpIfReset
           (
                ANSC_HANDLE                 hContext,
                ULONG                       ulInstanceNumber
           );

    Description:
    
        The API initiates a reset of IP interface - the IP connection is to be torn down and reestablished. 
        
    Arguments:
                ANSC_HANDLE                 hContext,
                
                ULONG                           ulInstanceNumber

    Return:
        The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlIpIfReset
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
    if ( ulInstanceNumber >= COSA_USG_IF_NUM )
    {
        return  CosaDmlIpIfMlanReset(hContext, ulInstanceNumber);
    }
    else
    {
    #ifdef _COSA_INTEL_USG_ARM_
        return ANSC_STATUS_FAILURE;
    #else
        ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
        /*TODO: TR181 requires to delay this operation after the end of current CWMP session, not supported*/
        
        AnscTraceFlow(("%s...\n", __FUNCTION__));

        if (strstr((char *)g_ipif_names[ulInstanceNumber-1], "lan0"))
        {
	    CcspTraceWarning(("%s: setting lan-restart\n", __FUNCTION__));
            v_secure_system("sysevent set lan-restart");
        }
        else if (strstr((char *)g_ipif_names[ulInstanceNumber-1],INTERFACE))
        {
            v_secure_system("sysevent set wan-restart");        
        }
        /*we do nothing when the interface is loopback*/

        
        return returnStatus;
    #endif
    }
}


/*
 *  IP Interface IPv4Address
 */
/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlIpIfGetNumberOfV4Addrs
           (
                ANSC_HANDLE                 hContext,
                ULONG                           ulIpIfInstanceNumber,
           );

    Description:
    
        The API adds one IP interface into the system. 
        
    Arguments:
                ANSC_HANDLE                 hContext,
                
                ULONG                       ulIpIfInstanceNumber

    Return:
        The number of IPv4 address.

**********************************************************************/
ULONG
CosaDmlIpIfGetNumberOfV4Addrs
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber
    )
{
    if ( ulIpIfInstanceNumber >= COSA_USG_IF_NUM )
    {
        return  CosaDmlIpIfMlanGetNumberOfV4Addrs(hContext, ulIpIfInstanceNumber);
    }
    else
    {
        return 1;
    }
}

/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlIpIfGetV4Addr
           (
                ANSC_HANDLE                 hContext,
                ULONG                       ulIpIfInstanceNumber,
                ULONG                       ulIndex,
                PCOSA_DML_IP_V4ADDR         pEntry
           );

    Description:
        
        The API adds one IP interface into the system. 
        
    Arguments:
    
        ANSC_HANDLE                 hContext,
        
        ULONG                       ulIpIfInstanceNumber,
        
        ULONG                       ulIndex,
        
        PCOSA_DML_IP_V4ADDR         pEntry

    Return:
        The status of the operation.

**********************************************************************/

static ULONG _get_addressing_type(ULONG  ulInstanceNumber)
{
    char output[256] = {0};
    
    if ( ulInstanceNumber == gDmsbIpIfLoopbackInstNum )
    {
        /*loopback interface*/
        return COSA_DML_IP_ADDR_TYPE_Static;
    }
    else if (gDmsbIpIfUpstream[ulInstanceNumber - 1] == FALSE) 
    {
        /*utopia use lan_dhcp_client to indicate lan addressing type*/
        if ( !syscfg_get(NULL, "lan_dhcp_client", output, sizeof(output)) )
        {
            if (output[0] == '0')
                return COSA_DML_IP_ADDR_TYPE_Static;
            else 
                return COSA_DML_IP_ADDR_TYPE_DHCP;
        }
        else
            return COSA_DML_IP_ADDR_TYPE_DHCP;
    }
    else
    {
        /*utopia use wan_proto to indicate wan addressing type*/
        if ( !syscfg_get(NULL, "wan_proto", output, sizeof(output)) )
        {
            if (!strncmp(output, "dhcp", 4))
                return COSA_DML_IP_ADDR_TYPE_DHCP;
            else 
                return COSA_DML_IP_ADDR_TYPE_Static;
        }
        else 
            return COSA_DML_IP_ADDR_TYPE_DHCP;
    }
}

ANSC_STATUS
CosaDmlIpIfGetV4Addr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_IP_V4ADDR         pEntry
    )
{
    if ( ulIpIfInstanceNumber >= COSA_USG_IF_NUM )
    {
        return  CosaDmlIpIfMlanGetV4Addr(hContext, ulIpIfInstanceNumber, ulIndex, pEntry);
    }
    else
    {
        ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
       
        AnscTraceFlow(("%s...\n", __FUNCTION__));

        pEntry->InstanceNumber   = ulIndex + 1;
        /*Not supported now*/
        pEntry->bEnabled         = TRUE;
        pEntry->Status           = COSA_DML_IP_ADDR_STATUS_Enabled;
        
        pEntry->IPAddress.Value  = CosaUtilGetIfAddr((char *)g_ipif_names[ulIpIfInstanceNumber-1]);
        pEntry->SubnetMask.Value = CosaUtilIoctlXXX((char *)g_ipif_names[ulIpIfInstanceNumber-1], "netmask", NULL);
                
        pEntry->AddressingType   = _get_addressing_type(ulIpIfInstanceNumber);

        if (TRUE)
        {
            char buf[256]= {0};
            char out[COSA_DML_IF_NAME_LENGTH]= {0};
            UtopiaContext utctx;
            errno_t safec_rc = -1;
            /*CID: 53826 Unchecked return value*/
            if(!Utopia_Init(&utctx))
               return ANSC_STATUS_FAILURE;


            safec_rc = sprintf_s(buf, sizeof(buf), "tr_ip_interface_%s_v4addr_alias", g_ipif_names[ulIpIfInstanceNumber-1]);
            if(safec_rc < EOK)
            {
               ERR_CHK(safec_rc);
            }
            Utopia_RawGet(&utctx,NULL,buf,out,sizeof(out));
            Utopia_Free(&utctx,0);

            if (out[0]) 
            {
                _ansc_strncpy(pEntry->Alias, out, COSA_DML_IF_NAME_LENGTH-1);  
            }
            else 
            {
                /*if the alias is not set before, we initialize it here rather than get a default value from middile layer*/
                Utopia_Init(&utctx);
                safec_rc = sprintf_s(buf, sizeof(buf), "tr_ip_interface_%s_v4addr_alias", g_ipif_names[ulIpIfInstanceNumber-1]);
                if(safec_rc < EOK)
                {
                   ERR_CHK(safec_rc);
                }
                safec_rc = sprintf_s(out, sizeof(out), "Interface_%lu", pEntry->InstanceNumber);
                if(safec_rc < EOK)
                {
                   ERR_CHK(safec_rc);
                }
                Utopia_RawSet(&utctx,NULL,buf,out);
                Utopia_Free(&utctx,1);

                _ansc_strncpy(pEntry->Alias, out, COSA_DML_IF_NAME_LENGTH-1);  
            }
        }

        AnscCopyMemory(&g_ipif_be_bufs[ulIpIfInstanceNumber-1].V4AddrList, pEntry, sizeof(COSA_DML_IP_V4ADDR));
        
        return returnStatus;
    }
}
/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlIpIfSetV4AddrValues
           (
                ANSC_HANDLE                 hContext,
                ULONG                       ulIpIfInstanceNumber,
                ULONG                       ulIndex,
                ULONG                       ulInstanceNumber,
                char*                       pAlias
            );

    Description:
        
        The API set one IP interface into the system. 
        
    Arguments:
    
        ANSC_HANDLE                 hContext,
        
        ULONG                       ulIpIfInstanceNumber,
        
        ULONG                       ulIndex,

        ULONG                       ulInstanceNumber,
        
        char*                       pAlias

    Return:
        The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlIpIfSetV4AddrValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    if ( ulIpIfInstanceNumber >= COSA_USG_IF_NUM )
    {
        return  CosaDmlIpIfMlanSetV4AddrValues(hContext, ulIpIfInstanceNumber, ulIndex, ulInstanceNumber, pAlias);
    }
    else
    {
    #if defined _COSA_INTEL_USG_ARM_ || _COSA_BCM_MIPS_
        return ANSC_STATUS_FAILURE;
    #else
        /*this API will never be called*/
        return ANSC_STATUS_SUCCESS;
    #endif
    }
}

/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlIpIfAddV4Addr
           (
                ANSC_HANDLE                 hContext,
                ULONG                           ulIpIfInstanceNumber,
                PCOSA_DML_IP_V4ADDR     pEntry
           );

    Description:
    
        The API adds one IP interface into the system. 
        
    Arguments:
                ANSC_HANDLE                 hContext,

                ULONG                            ulIpIfInstanceNumber,
                
                PCOSA_DML_IP_V4ADDR         pEntry

    Return:
        The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlIpIfAddV4Addr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V4ADDR         pEntry
    )
{
    if ( ulIpIfInstanceNumber >= COSA_USG_IF_NUM )
    {
        return  CosaDmlIpIfMlanAddV4Addr(hContext, ulIpIfInstanceNumber, pEntry);
    }
    else
    {    
        return  ANSC_STATUS_FAILURE;
    }
}
/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlIpIfDelV4Addr
           (
                ANSC_HANDLE                 hContext,
                ULONG                           ulIpIfInstanceNumber,
                PCOSA_DML_IP_V4ADDR    pEntry

           );

    Description:
    
        The API adds one IP interface into the system. 
        
    Arguments:
                ANSC_HANDLE                 hContext,
                
                ULONG                                ulIpIfInstanceNumber,
                
                PCOSA_DML_IP_V4ADDR         pEntry

    Return:
        The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlIpIfDelV4Addr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V4ADDR         pEntry
    )
{
    if ( ulIpIfInstanceNumber >= COSA_USG_IF_NUM )
    {
        return  CosaDmlIpIfMlanDelV4Addr(hContext, ulIpIfInstanceNumber, pEntry);
    }
    else
    {    
        return  ANSC_STATUS_FAILURE;
    }
}
/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlIpIfSetV4Addr
           (
                ANSC_HANDLE                 hContext,
                ULONG                           ulIpIfInstanceNumber,
                PCOSA_DML_IP_V4ADDR         pEntry
           );

    Description:
    
        The API adds one IP interface into the system. 
        
    Arguments:
                ANSC_HANDLE                 hContext,
               ULONG                            ulIpIfInstanceNumber,
                PCOSA_DML_IP_V4ADDR         pEntry

    Return:
        The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlIpIfSetV4Addr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V4ADDR         pEntry
    )
{
    if ( ulIpIfInstanceNumber >= COSA_USG_IF_NUM )
    {
        return  CosaDmlIpIfMlanSetV4Addr(hContext, ulIpIfInstanceNumber, pEntry);
    }
    else
    {    
        ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
        PCOSA_DML_IP_V4ADDR             p_be_buf = NULL; 
        char                            buf[256];
        UtopiaContext                   utctx;
        errno_t safec_rc = -1;
        
        AnscTraceFlow(("%s...\n", __FUNCTION__));

        p_be_buf = &g_ipif_be_bufs[ulIpIfInstanceNumber-1].V4AddrList[0];

        if (pEntry->bEnabled != p_be_buf->bEnabled)
        {
            /*not supported now*/
        }
        
        if (strcmp(pEntry->Alias, p_be_buf->Alias) != 0)
        {
            Utopia_Init(&utctx);
            safec_rc = sprintf_s(buf, sizeof(buf), "tr_ip_interface_%s_v4addr_alias", g_ipif_names[ulIpIfInstanceNumber-1]);
            if(safec_rc < EOK)
            {
               ERR_CHK(safec_rc);
            }
            Utopia_RawSet(&utctx,NULL,buf,pEntry->Alias);
            Utopia_Free(&utctx,1);

            safec_rc = strcpy_s(p_be_buf->Alias,sizeof(p_be_buf->Alias), pEntry->Alias);
            ERR_CHK(safec_rc);
        }

        /*for IPaddr and netmask, it's middle layer's responsibility to check if the AddressingType is Static. Remember we only set these params in Static AddressingType*/
        if (pEntry->IPAddress.Value != p_be_buf->IPAddress.Value)
        {

             /*CID: 74354 Unchecked return value*/
            if(!Utopia_Init(&utctx))
               return ANSC_STATUS_FAILURE;

            safec_rc = sprintf_s(buf, sizeof(buf), "%d.%d.%d.%d", 
                    pEntry->IPAddress.Dot[0],pEntry->IPAddress.Dot[1],pEntry->IPAddress.Dot[2],pEntry->IPAddress.Dot[3] );
            if(safec_rc < EOK)
            {
               ERR_CHK(safec_rc);
            }

            UTOPIA_SET(&utctx, UtopiaValue_LAN_IPAddr, buf);
            Utopia_Free(&utctx, 1);
            
            p_be_buf->IPAddress.Value = pEntry->IPAddress.Value;
        }

        if (pEntry->SubnetMask.Value != p_be_buf->SubnetMask.Value)
        {
            if (strstr((char *)g_ipif_names[ulIpIfInstanceNumber-1], "lan"))
            {
                /*CID: 74354 Unchecked return value*/
                if(!Utopia_Init(&utctx))
                    return ANSC_STATUS_FAILURE;
                safec_rc = sprintf_s(buf, sizeof(buf), "%d.%d.%d.%d", 
                        pEntry->SubnetMask.Dot[0],pEntry->SubnetMask.Dot[1],pEntry->SubnetMask.Dot[2],pEntry->SubnetMask.Dot[3] );
                if(safec_rc < EOK)
                {
                    ERR_CHK(safec_rc);
                }

                UTOPIA_SET(&utctx, UtopiaValue_LAN_Netmask, buf);
                Utopia_Free(&utctx, 1);
            }
            else if (strcmp((char *)g_ipif_names[ulIpIfInstanceNumber-1], "lo") == 0)
            {
                CosaUtilIoctlXXX((char *)g_ipif_names[ulIpIfInstanceNumber-1], "set_netmask", &pEntry->SubnetMask.Value);
            }

            p_be_buf->SubnetMask.Value = pEntry->SubnetMask.Value;
        }

        return returnStatus;
    }
}


/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlIpIfGetV4Addr2
           (
            ANSC_HANDLE                 hContext,
            ULONG                           ulIpIfInstanceNumber,
            PCOSA_DML_IP_V4ADDR         pEntry        
        );

    Description:
        
        The API adds one IP interface into the system. 
        
    Arguments:
    
        ANSC_HANDLE                 hContext,
        
        ULONG                       ulIpIfInstanceNumber,
        
        ULONG                       ulIndex,
        
        PCOSA_DML_IP_V4ADDR         pEntry

    Return:
        The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlIpIfGetV4Addr2
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V4ADDR         pEntry         
    )
{
    if ( ulIpIfInstanceNumber >= COSA_USG_IF_NUM )
    {
        return  CosaDmlIpIfMlanGetV4Addr2(hContext, ulIpIfInstanceNumber, pEntry);
    }
    else
    {    
        ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
        
        AnscCopyMemory(pEntry, &g_ipif_be_bufs[ulIpIfInstanceNumber-1].V4AddrList[0], sizeof(COSA_DML_IP_V4ADDR));
        return returnStatus;
    }
}


/*
 *  IP Interface IPv6Address
 */

/*this function will change datetime formatted string to seconds compared to now*/
int _datetime_to_secs(char * p_dt)
{
    time_t t1;
    struct tm t2 = {0};


    if (!p_dt)
        return 0;

    if (sscanf(p_dt, "%d-%d-%d%*c%02d:%02d:%02d%*c",
               &t2.tm_year, &t2.tm_mon, &t2.tm_mday,
               &t2.tm_hour, &t2.tm_min, &t2.tm_sec
            )!= 6 )
        return 0;
    
    t2.tm_year -= 1900;
    t2.tm_mon-- ;

    /*todo: what about time zone?*/
    t1 = mktime(&t2);

    return (t1-time(NULL));
}

/* ret:
   -1 : p_dt1<p_dt2
   0: p_dt1 == p_dt2
   1: p_dt1 > p_dt2*/
int CosaDmlDateTimeCompare(char *p_dt1, char *p_dt2)
{
    struct tm t1 = {0};
    struct tm t2 = {0};

    if (!p_dt1 || !p_dt2)
        return 0;

    if (AnscEqualString(p_dt1, p_dt2, TRUE))
        return 0;

    if (sscanf(p_dt1, "%d-%d-%d%*c%02d:%02d:%02d%*c",
               &t1.tm_year, &t1.tm_mon, &t1.tm_mday,
               &t1.tm_hour, &t1.tm_min, &t1.tm_sec
            )!= 6 )
        return 0;

    if (sscanf(p_dt2, "%d-%d-%d%*c%02d:%02d:%02d%*c",
               &t2.tm_year, &t2.tm_mon, &t2.tm_mday,
               &t2.tm_hour, &t2.tm_min, &t2.tm_sec
            )!= 6 )
        return 0;

    if (t1.tm_year != t2.tm_year)
        return (t1.tm_year > t2.tm_year ? 1:-1);
    else if (t1.tm_mon != t2.tm_mon)
        return (t1.tm_mon > t2.tm_mon ? 1:-1);
    else if (t1.tm_mday != t2.tm_mday)
        return (t1.tm_mday > t2.tm_mday ? 1:-1);
    else if (t1.tm_hour != t2.tm_hour)
        return (t1.tm_hour > t2.tm_hour ? 1:-1);
    else if (t1.tm_min != t2.tm_min)
        return (t1.tm_min > t2.tm_min ? 1:-1);
    else if (t1.tm_sec != t2.tm_sec)
        return (t1.tm_sec > t2.tm_sec ? 1:-1);
                
    return 0;
}

static int _invoke_ip_cmd(char * cmd, PCOSA_DML_IP_V6ADDR p_old, PCOSA_DML_IP_V6ADDR p_new, char * ifname, void * extra_args)
{
    int pref_len = 0;
    char buf[256] = {0};
    char name[256] = {0};
    ULONG buf_len = 256;
    char * p = NULL;
    struct in6_addr addr;
    errno_t safec_rc = -1;

    if (!cmd || !ifname)
        return -1;

    /*only support 3 cmd*/
    if (strcmp(cmd, "add") && strcmp(cmd, "del") && strcmp(cmd, "modify"))
        return -2;

    if (!strcmp(cmd, "add"))
    {
        int prefered_lft = 0;
        int valid_lft = 0;
        
        if (!p_new)
            return -1;

        if (!p_new->bEnabled)
            return 0;

        if (!p_new->Prefix[0])
            return -2;

        if (extra_args)
            pref_len = *(int *)extra_args;
        else
        {
            safec_rc = sprintf_s(name, sizeof(name),"%sPrefix", p_new->Prefix);
            if(safec_rc < EOK)
            {
               ERR_CHK(safec_rc);
            }
            if ( 0 == g_GetParamValueString(g_pDslhDmlAgent, name, buf, &buf_len))
            {
                if ((p = strchr(buf, '/')))
                    sscanf(p, "/%d", &pref_len);
            }
        }

        if (!pref_len)
            return -1;

        memset(buf, 0, sizeof(buf));
        inet_pton(AF_INET6, (char *)p_new->IP6Address, &addr);
        inet_ntop(AF_INET6, &addr, buf, sizeof(buf));
        snprintf(buf+strlen(buf), sizeof(buf)-strlen(buf),  "/%d", pref_len);

        /*if we don't set lft, it's forever*/
        if (strcmp(p_new->PreferredLifetime, "0001-01-01T00:00:00Z") && strcmp(p_new->PreferredLifetime, "9999-12-31T23:59:59Z")) 
            prefered_lft = _datetime_to_secs(p_new->PreferredLifetime);

        if (strcmp(p_new->ValidLifetime, "0001-01-01T00:00:00Z") && strcmp(p_new->ValidLifetime, "9999-12-31T23:59:59Z")) 
            valid_lft = _datetime_to_secs(p_new->ValidLifetime);

        if (!valid_lft && !prefered_lft)
            v_secure_system("ip -6 addr add %s dev %s", buf, ifname);
        else if (valid_lft && prefered_lft)
            v_secure_system("ip -6 addr add %s dev %s valid_lft %d preferred_lft %d", buf, ifname, valid_lft, prefered_lft);
        else
        {
            /*if only has one lft, we can only use prefered_lft, cause valid_lft should be bigger*/
            v_secure_system("ip -6 addr add %s dev %s preferred_lft %d", buf, ifname, prefered_lft);
        }
    }
    else if (!strcmp(cmd, "del"))
    {
        if (!p_old)
            return -1;

        sprintf(name, "%sPrefix", p_old->Prefix);
        if ( 0 == g_GetParamValueString(g_pDslhDmlAgent, name, buf, &buf_len))
        {
            if ((p = strchr(buf, '/')))
                sscanf(p, "/%d", &pref_len);
        }

        if (!pref_len)
            return -1;

        memset(buf, 0, sizeof(buf));
        inet_pton(AF_INET6, (char *)p_old->IP6Address, &addr);
        inet_ntop(AF_INET6, &addr, buf, sizeof(buf));
        snprintf(buf+strlen(buf), sizeof(buf)-strlen(buf),  "/%d", pref_len);

        v_secure_system("ip -6 addr del %s dev %s", buf, ifname);
    }
    else if (!strcmp(cmd, "modify"))
    {
        ULONG mask = 0;
        
        if (!extra_args)
            return -1;
        
        mask = *((int *)extra_args);

        if (mask & _DML_V6ADDR_ENABLE_CHANGE)
        {
            if (!p_new->bEnabled)
                _invoke_ip_cmd("del", p_old, NULL, ifname, NULL);
            else 
            {
                _invoke_ip_cmd("del", p_old, NULL, ifname, NULL);
                _invoke_ip_cmd("add", NULL, p_new, ifname, NULL);
            }
        }
        else 
        {
            _invoke_ip_cmd("del", p_old, NULL, ifname, NULL);
            _invoke_ip_cmd("add", NULL, p_new, ifname, NULL);
        }
        
    }

    return 0;
        
    
}

/*pref_path is a Prefix full path name, the input path name is empty,  we should fill in it. */
int _find_matched_prefix(char * pref_path, char * v6addr, int ipif_inst_num)
{
    char   param_val[128] = {0};
    ULONG  val_len = sizeof(param_val);
    char   name[128] = {0};
    int    i = 0; 
    int    inst2 = 0;
    int    found = 0;
    int    num = 0;
    errno_t safec_rc = -1;
    

    if (!pref_path)
        return -1;
    
    safec_rc = sprintf_s(name, sizeof(name), "Device.IP.Interface.%d.IPv6PrefixNumberOfEntries", ipif_inst_num);
    if(safec_rc < EOK)
    {
       ERR_CHK(safec_rc);
    }
	
    num = g_GetParamValueUlong(g_pDslhDmlAgent, name);
    for (i=0; i<num; i++)
    {
        safec_rc = sprintf_s(name, sizeof(name), "Device.IP.Interface.%d.IPv6Prefix.", ipif_inst_num);
        if(safec_rc < EOK)
        {
           ERR_CHK(safec_rc);
        }
        inst2 = g_GetInstanceNumberByIndex(g_pDslhDmlAgent, name, i);

        if ( inst2 )
        {
            safec_rc = sprintf_s(name, sizeof(name), "Device.IP.Interface.%d.IPv6Prefix.%d.Prefix", ipif_inst_num, inst2);
            if(safec_rc < EOK)
            {
               ERR_CHK(safec_rc);
            }               
            memset(param_val, 0, sizeof(param_val));

            val_len = sizeof(param_val);
            if ( ( 0 == g_GetParamValueString(g_pDslhDmlAgent, name, param_val, &val_len))  
                 && !__v6addr_mismatches_v6pre(v6addr, param_val))
            {
                found = 1;
                sprintf(pref_path, "Device.IP.Interface.%d.IPv6Prefix.%d.", ipif_inst_num, inst2);
                break;
            }
        }

    }

    
    if (found)
        return 0;
    else 
        return -2;
}

int CosaDmlIpv6AddrMatchesPrefix(char * pref_path, char * v6addr, int ipif_inst_num)
{
    char   param_val[128] = {0};
    ULONG  val_len = sizeof(param_val);
    char   name[256] = {0};
    UNREFERENCED_PARAMETER(ipif_inst_num);

    errno_t safec_rc = sprintf_s(name, sizeof(name), "%sPrefix", pref_path);
    if(safec_rc < EOK)
    {
       ERR_CHK(safec_rc);
    }
    
    if ( ( 0 == g_GetParamValueString(g_pDslhDmlAgent, name, param_val, &val_len)) &&
          !__v6addr_mismatches_v6pre(v6addr, param_val))    
        return TRUE;
    else
        return FALSE;
}
/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlIpIfAddV6Addr
           (
                ANSC_HANDLE                 hContext,
                ULONG                           ulIpIfInstanceNumber,
                PCOSA_DML_IP_V6ADDR     pEntry
           );

    Description:
    
        The API adds one IP interface into the system. 
        
    Arguments:
                ANSC_HANDLE                 hContext,

                ULONG                            ulIpIfInstanceNumber,
                
                PCOSA_DML_IP_V6ADDR         pEntry

    Return:
        The status of the operation.

**********************************************************************/
static int _syscfg_set_v6addr(char * ifname, UtopiaContext * p_utctx, PCOSA_DML_IP_V6ADDR  pEntry)
{

    char                            val[1024]= {0};
    char                            namespace[256] = {0};
    errno_t  safec_rc = -1;
    snprintf(namespace, sizeof(namespace)-1, SYSCFG_FORMAT_NAMESPACE_STATIC_V6ADDR, ifname, pEntry->InstanceNumber);

    safec_rc = strcpy_s(val, sizeof(val), pEntry->IP6Address);
    if(safec_rc != EOK)
    {
       ERR_CHK(safec_rc);
    }
    Utopia_RawSet(p_utctx,namespace,"IPAddress",val);

    safec_rc = strcpy_s(val, sizeof(val), pEntry->Alias);
    if(safec_rc != EOK)
    {
       ERR_CHK(safec_rc);
    }
    Utopia_RawSet(p_utctx,namespace,"Alias",val);
            
    memset(val, 0, sizeof(val));
    val[0] = (pEntry->bEnabled) ? '1':'0';
    Utopia_RawSet(p_utctx,namespace,"Enable",val);

    safec_rc = strcpy_s(val, sizeof(val), pEntry->PreferredLifetime);
    if(safec_rc != EOK)
    {
       ERR_CHK(safec_rc);
    }
    Utopia_RawSet(p_utctx,namespace,"PreferredLifetime",val);

    safec_rc = strcpy_s(val, sizeof(val), pEntry->ValidLifetime);
    if(safec_rc != EOK)
    {
       ERR_CHK(safec_rc);
    }
    Utopia_RawSet(p_utctx,namespace,"ValidLifetime",val);

    memset(val, 0, sizeof(val));
    val[0] = (pEntry->bAnycast) ? '1':'0';
    Utopia_RawSet(p_utctx,namespace,"Anycast",val);

    return 0;
}

ANSC_STATUS
CosaDmlIpIfAddV6Addr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6ADDR         pEntry
    )
{
    if ( ulIpIfInstanceNumber > COSA_USG_IF_NUM )
    {
        return  CosaDmlIpIfMlanAddV6Addr(hContext, ulIpIfInstanceNumber, pEntry);
    }
    else
    {
        int                             i = 0;
        ULONG                           ulIndex = 0;
        char                            name[256]= {0};
        char                            val[1024]= {0};
        UtopiaContext                   utctx = {0};
        char                            namespace[256] = {0};
        errno_t  safec_rc = -1;
        
        AnscTraceFlow(("%s...\n", __FUNCTION__));
        
        for ( i = 0; i < g_ipif_num; i++ )
        {
            if ( ulIpIfInstanceNumber == g_ipif_be_bufs[i].Cfg.InstanceNumber )
            {
                ulIndex = g_ipif_be_bufs[i].ulNumOfV6Addr;
                if ( ulIndex >= MAX_IPV6_ENTRY_NUM )
                    return ANSC_STATUS_FAILURE;

                if (!Utopia_Init(&utctx))
                    return ANSC_STATUS_FAILURE;

                /*first handle syscfg*/
                safec_rc = sprintf_s(name, sizeof(name), SYSCFG_FORMAT_STATIC_V6ADDR, (char *)g_ipif_names[i]);
                if(safec_rc < EOK)
                {
                   ERR_CHK(safec_rc);
                }
                memset(val, 0, sizeof(val));
                Utopia_RawGet(&utctx,NULL,name,val,sizeof(val));            

                /* CID: 75004 Out-of-bounds access*/
                snprintf(val+strlen(val), sizeof(val)-1,  "%lu,", pEntry->InstanceNumber );

                Utopia_RawSet(&utctx,NULL,name,val);

                _syscfg_set_v6addr((char *)g_ipif_names[i], &utctx, pEntry);
                
                Utopia_Free(&utctx,1);                    

                /*
                  don't do this 'cause it may be wrong.
                if (!pEntry->Prefix[0])
                {
                    if (_find_matched_prefix(pEntry->Prefix, pEntry->IP6Address ,ulIpIfInstanceNumber))
                    {
                        CcspTraceWarning(("%s prefix is empty but can't find a matched Prefix\n", __FUNCTION__));
                    }
                }
                */
                if (!Utopia_Init(&utctx))
                    return ANSC_STATUS_FAILURE;
                
                /*should record prefix in syscfg after it's adjusted*/
                snprintf(namespace, sizeof(namespace)-1, SYSCFG_FORMAT_NAMESPACE_STATIC_V6ADDR, (char *)g_ipif_names[i], pEntry->InstanceNumber);
                safec_rc = strcpy_s(val, sizeof(val), pEntry->Prefix);
                if(safec_rc != EOK)
                {
                   ERR_CHK(safec_rc);
                }
                Utopia_RawSet(&utctx,namespace,"Prefix",val);

                Utopia_Free(&utctx,1);                    

                _invoke_ip_cmd("add", NULL , pEntry, (char *)g_ipif_names[i], NULL);

    			g_ipif_be_bufs[i].V6AddrList[ulIndex] = *pEntry;
                
                g_ipif_be_bufs[i].ulNumOfV6Addr++;

                return ANSC_STATUS_SUCCESS;
            }
        }

        return ANSC_STATUS_CANT_FIND;
    }
}

/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlIpIfDelV6Addr
           (
                ANSC_HANDLE                 hContext,
                ULONG                           ulIpIfInstanceNumber,
                PCOSA_DML_IP_V6ADDR    pEntry

           );

    Description:
    
        The API adds one IP interface into the system. 
        
    Arguments:
                ANSC_HANDLE                 hContext,
                
                ULONG                                ulIpIfInstanceNumber,
                
                PCOSA_DML_IP_V6ADDR         pEntry

    Return:
        The status of the operation.

**********************************************************************/
static int _syscfg_unset_v6addr(char * ifname, char * v6addr, int inst_num)
{
    char                            namespace[256]= {0};

    UNREFERENCED_PARAMETER(v6addr);

    snprintf(namespace, sizeof(namespace)-1, SYSCFG_FORMAT_NAMESPACE_STATIC_V6ADDR, ifname, (ULONG)inst_num);

    syscfg_unset(namespace, "IPAddress");
    syscfg_unset(namespace, "Alias");
    syscfg_unset(namespace, "Enable");
    syscfg_unset(namespace, "Prefix");
    syscfg_unset(namespace, "PreferredLifetime");
    syscfg_unset(namespace, "ValidLifetime");
    syscfg_unset(namespace, "Anycast");

    return 0;
}

/*buf format:
  num1,num2,num3,...
  inst_num is number I want to remove from buf*/
static int _del_one_token(char * buf, int inst_num, int buf_len)
{
    int * num_array = NULL;
    int size = 0;
    char * str = NULL;
    char * p_token = NULL;
    char * saveptr = NULL;
    int  i = 0;

    for (str = buf; ; str = NULL) 
    {
        p_token = strtok_r(str, ",", &saveptr);
        if (p_token == NULL)
            break;

        if (atoi(p_token) == inst_num)
            continue;
        
        size++;
        num_array = (int *)realloc(num_array, sizeof(int)*size);
        if (num_array)
            num_array[size-1] = atoi(p_token);
    }
    
    /*buf is in & out*/
    /* CID:72709 Wrong sizeof argument*/
    memset(buf, 0, buf_len);
    for (i=0; i<size; i++)
        sprintf(buf+strlen(buf), "%d,", num_array[i]);

    free(num_array); /*RDKB-6840,CID-33498, free unused resource before exit*/
    return 0;
}

ANSC_STATUS
CosaDmlIpIfDelV6Addr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6ADDR         pEntry
    )
{
    if ( ulIpIfInstanceNumber > COSA_USG_IF_NUM )
    {
        return  CosaDmlIpIfMlanDelV6Addr(hContext, ulIpIfInstanceNumber, pEntry);
    }
    else
    {
        int                             i = 0;
        ULONG                           j = 0;
        int                             k = 0;
        char                            name[256]= {0};
        char                            val[1024]= {0};
        UtopiaContext                   utctx = {0};
        errno_t safec_rc = -1;

        AnscTraceFlow(("%s...\n", __FUNCTION__));

        for ( i = 0; i < g_ipif_num; i++ )
        {
            if ( ulIpIfInstanceNumber == g_ipif_be_bufs[i].Cfg.InstanceNumber )
            {
                for ( j = 0; j < g_ipif_be_bufs[i].ulNumOfV6Addr; j++ )
                {
                    if ( g_ipif_be_bufs[i].V6AddrList[j].InstanceNumber == pEntry->InstanceNumber )
                    {

                        if (!Utopia_Init(&utctx))
                            return ANSC_STATUS_FAILURE;

                        /*first handle syscfg stuff*/
                        safec_rc = sprintf_s(name, sizeof(name), SYSCFG_FORMAT_STATIC_V6ADDR, (char *)g_ipif_names[i]);
                        if(safec_rc < EOK)
                        {
                           ERR_CHK(safec_rc);
                        }
                        memset(val, 0, sizeof(val));
                        Utopia_RawGet(&utctx,NULL,name,val,sizeof(val));

                        /*val is updated with the removal of v6addr*/
                        _del_one_token(val, pEntry->InstanceNumber,sizeof(val));
                        Utopia_RawSet(&utctx,NULL,name,val);

                        _syscfg_unset_v6addr((char *)g_ipif_names[i], pEntry->IP6Address, pEntry->InstanceNumber);

                        Utopia_Free(&utctx,1);
                        
                        _invoke_ip_cmd("del", &g_ipif_be_bufs[i].V6AddrList[j], NULL, (char *)g_ipif_names[i], NULL);

                        for ( k = j; k < g_ipif_be_bufs[i].ulNumOfV6Addr-1; k++ )
                        {
                            AnscCopyMemory
                                (
                                    &g_ipif_be_bufs[i].V6AddrList[k], 
                                    &g_ipif_be_bufs[i].V6AddrList[k+1],
                                    sizeof(COSA_DML_IP_V6ADDR)
                                    );
                        }
                        
                        g_ipif_be_bufs[i].ulNumOfV6Addr--;

                        return ANSC_STATUS_SUCCESS;
                    }
                }
            }
        }


        return ANSC_STATUS_CANT_FIND;
    }
}

/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlIpIfSetV6Addr
           (
                ANSC_HANDLE                 hContext,
                ULONG                           ulIpIfInstanceNumber,
                PCOSA_DML_IP_V6ADDR         pEntry
           );

    Description:
    
        The API adds one IP interface into the system. 
        
    Arguments:
                ANSC_HANDLE                 hContext,
               ULONG                            ulIpIfInstanceNumber,
                PCOSA_DML_IP_V6ADDR         pEntry

    Return:
        The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlIpIfSetV6Addr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6ADDR         pEntry
    )
{
    if ( ulIpIfInstanceNumber > COSA_USG_IF_NUM )
    {
        return  CosaDmlIpIfMlanSetV6Addr(hContext, ulIpIfInstanceNumber, pEntry);
    }
    else
    {
        int                             i       = 0;
        ULONG                           j       = 0;
        ULONG                           mask    = 0;
        char                            namespace[256]= {0};
        char                            val[1024]= {0};
        UtopiaContext                   utctx = {0};
        UNREFERENCED_PARAMETER(hContext);
        errno_t safec_rc = -1;

        AnscTraceFlow(("%s...\n", __FUNCTION__));

        for ( i = 0; i < g_ipif_num; i++ )
        {
            if ( ulIpIfInstanceNumber == g_ipif_be_bufs[i].Cfg.InstanceNumber )
            {
                for ( j = 0; j < g_ipif_be_bufs[i].ulNumOfV6Addr; j++ )
                {
                    if ( g_ipif_be_bufs[i].V6AddrList[j].InstanceNumber == pEntry->InstanceNumber )
                    {

                        if (!Utopia_Init(&utctx))
                            return ANSC_STATUS_FAILURE;

                        /*first handle syscfg*/
                        _syscfg_set_v6addr((char *)g_ipif_names[i], &utctx, pEntry);
                            
                        if (!AnscEqualString(pEntry->IP6Address, g_ipif_be_bufs[i].V6AddrList[j].IP6Address, TRUE))
                            mask |= _DML_V6ADDR_ADDR_CHANGE;
                
                        if (pEntry->bEnabled != g_ipif_be_bufs[i].V6AddrList[j].bEnabled)
                        {
                            if (pEntry->bEnabled)
                                pEntry->Status = COSA_DML_IP_ADDR_STATUS_Enabled;
                            else
                                pEntry->Status = COSA_DML_IP_ADDR_STATUS_Disabled;

                            mask |= _DML_V6ADDR_ENABLE_CHANGE;
                        }

                        if (!AnscEqualString(pEntry->Prefix, g_ipif_be_bufs[i].V6AddrList[j].Prefix, TRUE))
                            mask |= _DML_V6ADDR_PREFIX_CHANGE;

                        if (!AnscEqualString(pEntry->PreferredLifetime, g_ipif_be_bufs[i].V6AddrList[j].PreferredLifetime, TRUE))
                            mask |= _DML_V6ADDR_PREFERED_LFT_CHANGE;
                        
                        if (!AnscEqualString(pEntry->ValidLifetime, g_ipif_be_bufs[i].V6AddrList[j].ValidLifetime, TRUE))
                            mask |= _DML_V6ADDR_VALID_LFT_CHANGE;

                        if (pEntry->bAnycast != g_ipif_be_bufs[i].V6AddrList[j].bAnycast)
                        {                    
                            /*"ip" cmd doesn't support anycast for now*/
                        }

                        snprintf(namespace, sizeof(namespace)-1, SYSCFG_FORMAT_NAMESPACE_STATIC_V6ADDR, g_ipif_be_bufs[i].Info.Name, pEntry->InstanceNumber);
    /*
      I don't want to calcute the Prefix param, 'cause sometimes we could be wrong, it's the inputer's responsibilty to fill in correct Prefix field
                        if (!pEntry->Prefix[0])
                        {
                            if (_find_matched_prefix(pEntry->Prefix, pEntry->IP6Address ,ulIpIfInstanceNumber))
                            {
                                CcspTraceWarning(("%s prefix is empty but can't find a matched Prefix\n", __FUNCTION__));
                            }
                            else 
                            {
                                safe_strcpy(val, pEntry->Prefix, sizeof(val));
                                Utopia_RawSet(&utctx,namespace,"Prefix",val);
                            }
                        }
                        else
    */
                        if (pEntry->Prefix[0])
                        {
                            /*in some corner case, we need to rewrite prefix into syscfg*/
                            safec_rc = strcpy_s(val, sizeof(val), pEntry->Prefix);
                            if(safec_rc != EOK)
                            {
                               ERR_CHK(safec_rc);
                            }
                            Utopia_RawSet(&utctx,namespace,"Prefix",val);
                        }

                        Utopia_Free(&utctx,1);
                        
                        /*if no critical change, won't invoke "ip" cmd*/
                        if (mask)
                            _invoke_ip_cmd("modify", &g_ipif_be_bufs[i].V6AddrList[j], pEntry, (char *)g_ipif_names[i], &mask);

    					g_ipif_be_bufs[i].V6AddrList[j] = *pEntry;
                        
                        return ANSC_STATUS_SUCCESS;
                    }
                }
            }
        }

        return ANSC_STATUS_CANT_FIND;
    }
}


/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlIpIfGetV6Addr2
           (
            ANSC_HANDLE                 hContext,
            ULONG                           ulIpIfInstanceNumber,
            PCOSA_DML_IP_V6ADDR         pEntry        
        );

    Description:
        
        The API adds one IP interface into the system. 
        
    Arguments:
    
        ANSC_HANDLE                 hContext,
        
        ULONG                       ulIpIfInstanceNumber,
        
        ULONG                       ulIndex,
        
        PCOSA_DML_IP_V6ADDR         pEntry

    Return:
        The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlIpIfGetV6Addr2
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6ADDR         pEntry         
    )
{
    if ( ulIpIfInstanceNumber > COSA_USG_IF_NUM )
    {
        return  CosaDmlIpIfMlanGetV6Addr2(hContext, ulIpIfInstanceNumber, pEntry);
    }
    else
    {
        ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
        int                             i       = 0;
        ULONG                           j       = 0;

        AnscTraceFlow(("%s...\n", __FUNCTION__));

        for ( i = 0; i < g_ipif_num; i++ )
        {
            if ( ulIpIfInstanceNumber == g_ipif_be_bufs[i].Cfg.InstanceNumber )
            {
                for ( j = 0; j < g_ipif_be_bufs[i].ulNumOfV6Addr; j++ )
                {
                    if ( g_ipif_be_bufs[i].V6AddrList[j].InstanceNumber == pEntry->InstanceNumber )
                    {
                        *pEntry = g_ipif_be_bufs[i].V6AddrList[j];
                        return ANSC_STATUS_SUCCESS;
                    }
                }
            }
        }
        
        return returnStatus;
    }
}

/*
 *  IP Interface IPv6Prefix
 */
#ifdef CISCO_CONFIG_DHCPV6_PREFIX_DELEGATION
PCOSA_DML_IP_V6PREFIX
CosaDmlIPGetIPv6Prefixes
    (
        PCOSA_DML_IP_IF_FULL2       p_ipif,
        PULONG                      p_num
    )
{       
    int i = 0;
    PCOSA_DML_IP_V6PREFIX p_dml_pref = NULL;

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    if (!p_ipif || !p_num)
        return NULL;

    if ( p_ipif->Cfg.InstanceNumber > COSA_USG_IF_NUM )
    {
        *p_num = 1; /*current only support only one prefix for multinet*/

        p_dml_pref = (PCOSA_DML_IP_V6PREFIX)AnscAllocateMemory(*p_num * sizeof(COSA_DML_IP_V6PREFIX));

        if (!p_dml_pref)
            return NULL;

        p_dml_pref->InstanceNumber = 1;
        CosaDmlIpIfMlanGetV6Prefix2(NULL, p_ipif->Cfg.InstanceNumber, p_dml_pref);
    }
    else
    {
        for (i=0; i<g_ipif_num; i++)
            if (!strncmp(g_ipif_names[i], p_ipif->Info.Name, sizeof(p_ipif->Info.Name)))
                break;

        if (i == g_ipif_num)
            return NULL;
        
        IPIF_getEntry_for_Ipv6Pre(p_ipif, i);

        *p_num = g_ipif_be_bufs[i].ulNumOfV6Pre;
        
        if (*p_num)
        {
            p_dml_pref = (PCOSA_DML_IP_V6PREFIX)AnscAllocateMemory(*p_num * sizeof(COSA_DML_IP_V6PREFIX));
        
            if (!p_dml_pref)
                return NULL;
            
            AnscCopyMemory(p_dml_pref, g_ipif_be_bufs[i].V6PreList, *p_num*sizeof(COSA_DML_IP_V6PREFIX));
        }
    }

    return p_dml_pref;
}
#else
PCOSA_DML_IP_V6PREFIX
CosaDmlIPGetIPv6Prefixes
    (
        PCOSA_DML_IP_IF_FULL2       p_ipif,
        PULONG                      p_num
    )
{
#if defined (MULTILAN_FEATURE)
    int i = 0;
    PCOSA_DML_IP_V6PREFIX p_dml_pref = NULL;
#endif
    /*RDKB-6840, CID-33130, null check before use*/
    if (!p_ipif || !p_num)
        return NULL;

    if ( p_ipif->Cfg.InstanceNumber >= COSA_USG_IF_NUM )
    {
#if defined (MULTILAN_FEATURE)
        *p_num = 1; /*current only support only one prefix for multinet*/

        p_dml_pref = (PCOSA_DML_IP_V6PREFIX)AnscAllocateMemory(*p_num * sizeof(COSA_DML_IP_V6PREFIX));

        if (!p_dml_pref)
            return NULL;

        p_dml_pref->InstanceNumber = 1;
        CosaDmlIpIfMlanGetV6Prefix2(NULL, p_ipif->Cfg.InstanceNumber, p_dml_pref);

        for (i=0; i<g_ipif_num; i++)
            if (!strncmp(g_ipif_names[i], COSA_DML_DHCPV6_CLIENT_IFNAME, strlen(COSA_DML_DHCPV6_CLIENT_IFNAME)))
                break;
        _ansc_snprintf( p_dml_pref->ParentPrefix, sizeof(p_dml_pref->ParentPrefix), "Device.IP.Interface.%d.IPv6Prefix.1.", i+1);

        _get_datetime_offset(p_dml_pref->iapd_pretm, p_dml_pref->PreferredLifetime, sizeof(p_dml_pref->PreferredLifetime));
        _get_datetime_offset(p_dml_pref->iapd_vldtm, p_dml_pref->ValidLifetime, sizeof(p_dml_pref->ValidLifetime));

        if (p_ipif->ulNextIPV6PreInsNum <= p_dml_pref->InstanceNumber)
            p_ipif->ulNextIPV6PreInsNum = p_dml_pref->InstanceNumber + 1;
#else
        /*return  CosaDmlIpIfMlanGetIPv6Prefixes(hContext, ulIpIfInstanceNumber, pEntry);*/
        return  NULL;
#endif
    }
    else
    {
#ifndef MULTILAN_FEATURE
	int i = 0;
        PCOSA_DML_IP_V6PREFIX p_dml_pref = NULL;
#endif
        AnscTraceFlow(("%s...\n", __FUNCTION__));

        for (i=0; i<g_ipif_num; i++)
            if (!strncmp(g_ipif_names[i], p_ipif->Info.Name, sizeof(p_ipif->Info.Name)))
                break;

        if (i == g_ipif_num)
            return NULL;
        
        IPIF_getEntry_for_Ipv6Pre(p_ipif, i);

        *p_num = g_ipif_be_bufs[i].ulNumOfV6Pre;
        
        if (*p_num)
        {
            p_dml_pref = (PCOSA_DML_IP_V6PREFIX)AnscAllocateMemory(*p_num * sizeof(COSA_DML_IP_V6PREFIX));
        
            if (!p_dml_pref)
                return NULL;
            
            AnscCopyMemory(p_dml_pref, g_ipif_be_bufs[i].V6PreList, *p_num*sizeof(COSA_DML_IP_V6PREFIX));
        }
#if defined (MULTILAN_FEATURE)
    }
    return p_dml_pref;
#else
        return p_dml_pref;
    }
#endif
}

#endif

/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlIpIfAddV6Prefix
           (
                ANSC_HANDLE                 hContext,
                ULONG                           ulIpIfInstanceNumber,
                PCOSA_DML_IP_V6PREFIX       pEntry
           );

    Description:
    
        The API adds one IP interface into the system. 
        
    Arguments:
                ANSC_HANDLE                 hContext,

                ULONG                            ulIpIfInstanceNumber,
                
                PCOSA_DML_IP_V6PREFIX         pEntry

    Return:
        The status of the operation.

**********************************************************************/
static int _syscfg_set_v6pref(char * ifname, UtopiaContext * p_utctx, PCOSA_DML_IP_V6PREFIX pEntry)
{
    char                            namespace[256] = {0};
    char                            val[1024] = {0};
    errno_t  safec_rc = -1;

    snprintf(namespace, sizeof(namespace)-1, SYSCFG_FORMAT_NAMESPACE_STATIC_V6PREFIX, ifname, pEntry->InstanceNumber);
            
    memset(val, 0, sizeof(val));
    val[0] = (pEntry->bEnabled) ? '1':'0';
    Utopia_RawSet(p_utctx,namespace,"Enable",val);
                
    safec_rc = strcpy_s(val, sizeof(val), pEntry->Alias);
    if(safec_rc != EOK)
    {
       ERR_CHK(safec_rc);
    }
    Utopia_RawSet(p_utctx,namespace,"Alias",val);

    safec_rc = strcpy_s(val, sizeof(val), pEntry->Prefix);
    if(safec_rc != EOK)
    {
       ERR_CHK(safec_rc);
    }
    Utopia_RawSet(p_utctx,namespace,"Prefix",val);

    return 0;
}

ANSC_STATUS
CosaDmlIpIfAddV6Prefix
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6PREFIX       pEntry
    )
{
    if ( ulIpIfInstanceNumber > COSA_USG_IF_NUM )
    {
        return  CosaDmlIpIfMlanAddV6Prefix(hContext, ulIpIfInstanceNumber, pEntry);
    }
    else
    {
        int                             i = 0;
        ULONG                           ulIndex = 0;
        char                            name[256] = {0};
        char                            val[1024] = {0};
        UtopiaContext                   utctx = {0};
        errno_t safec_rc = -1;

        AnscTraceFlow(("%s...\n", __FUNCTION__));

        for ( i = 0; i < g_ipif_num; i++ )
        {
            if ( ulIpIfInstanceNumber == g_ipif_be_bufs[i].Cfg.InstanceNumber )
            {
                ulIndex = g_ipif_be_bufs[i].ulNumOfV6Pre;
                if ( ulIndex >= MAX_IPV6_ENTRY_NUM )
                    return ANSC_STATUS_FAILURE;

                if (!Utopia_Init(&utctx))
                    return ANSC_STATUS_FAILURE;

    			g_ipif_be_bufs[i].V6PreList[ulIndex] = *pEntry;

                safec_rc = sprintf_s(name, sizeof(name), SYSCFG_FORMAT_STATIC_V6PREF, (char *)g_ipif_names[i]);
                if(safec_rc < EOK)
                {
                   ERR_CHK(safec_rc);
                }
                memset(val, 0, sizeof(val));
                Utopia_RawGet(&utctx,NULL,name,val,sizeof(val));

                snprintf(val+strlen(val), sizeof(val)-strlen(val), "%lu,", pEntry->InstanceNumber);

                Utopia_RawSet(&utctx,NULL,name,val);

                _syscfg_set_v6pref((char *)g_ipif_names[i], &utctx, pEntry);

                Utopia_Free(&utctx,1);            

                g_ipif_be_bufs[i].ulNumOfV6Pre++;

                return ANSC_STATUS_SUCCESS;
            }
        }

        return ANSC_STATUS_CANT_FIND;
    }
}

/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlIpIfDelV6Prefix
           (
                ANSC_HANDLE                 hContext,
                ULONG                           ulIpIfInstanceNumber,
                PCOSA_DML_IP_V6PREFIX       pEntry

           );

    Description:
    
        The API adds one IP interface into the system. 
        
    Arguments:
                ANSC_HANDLE                 hContext,
                
                ULONG                                ulIpIfInstanceNumber,
                
                PCOSA_DML_IP_V6PREFIX         pEntry

    Return:
        The status of the operation.

**********************************************************************/

static int _syscfg_unset_v6prefix(char * ifname, int inst_num)
{
    char                            namespace[256] = {0};

    snprintf(namespace, sizeof(namespace)-1, SYSCFG_FORMAT_NAMESPACE_STATIC_V6PREFIX, ifname, (ULONG)inst_num);
    
    syscfg_unset(namespace, "Enable");
    syscfg_unset(namespace, "Alias");
    syscfg_unset(namespace, "Prefix");

    return 0;
}

ANSC_STATUS
CosaDmlIpIfDelV6Prefix
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6PREFIX       pEntry
    )
{
    if ( ulIpIfInstanceNumber > COSA_USG_IF_NUM )
    {
        return  CosaDmlIpIfMlanDelV6Prefix(hContext, ulIpIfInstanceNumber, pEntry);
    }
    else
    {
        int                             i = 0;
        ULONG                           j = 0;
        int                             k = 0;
        char                            name[256] = {0};
        char                            val[1024] = {0};
        UtopiaContext                   utctx = {0};
        errno_t safec_rc = -1;

        AnscTraceFlow(("%s...\n", __FUNCTION__));

        for ( i = 0; i < g_ipif_num; i++ )
        {
            if ( ulIpIfInstanceNumber == g_ipif_be_bufs[i].Cfg.InstanceNumber )
            {
                for ( j = 0; j < g_ipif_be_bufs[i].ulNumOfV6Pre; j++ )
                {
                    if ( g_ipif_be_bufs[i].V6PreList[j].InstanceNumber == pEntry->InstanceNumber )
                    {
                        for ( k = j; k < g_ipif_be_bufs[i].ulNumOfV6Pre-1; k++ )
                        {
                            AnscCopyMemory
                                (
                                    &g_ipif_be_bufs[i].V6PreList[k], 
                                    &g_ipif_be_bufs[i].V6PreList[k+1],
                                    sizeof(COSA_DML_IP_V6PREFIX)
                                    );
                        }


                        if (!Utopia_Init(&utctx))
                            return ANSC_STATUS_FAILURE;
                
                        safec_rc = sprintf_s(name, sizeof(name), SYSCFG_FORMAT_STATIC_V6PREF, (char *)g_ipif_names[i]);
                        if(safec_rc < EOK)
                        {
                            ERR_CHK(safec_rc);
                        }
                        memset(val, 0, sizeof(val));
                        Utopia_RawGet(&utctx,NULL,name,val,sizeof(val));

                        /*val is updated with the removal of Prefix*/
                        _del_one_token(val, pEntry->InstanceNumber,sizeof(val));
                        Utopia_RawSet(&utctx,NULL,name,val);

                        _syscfg_unset_v6prefix((char *)g_ipif_names[i], pEntry->InstanceNumber);

                        Utopia_Free(&utctx,1);            
                        
                        g_ipif_be_bufs[i].ulNumOfV6Pre--;

                        return ANSC_STATUS_SUCCESS;
                    }
                }
            }
        }

        return ANSC_STATUS_CANT_FIND;
    }
}

/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlIpIfSetV6Prefix
           (
                ANSC_HANDLE                 hContext,
                ULONG                           ulIpIfInstanceNumber,
                PCOSA_DML_IP_V6PREFIX       pEntry
           );

    Description:
    
        The API adds one IP interface into the system. 
        
    Arguments:
                ANSC_HANDLE                 hContext,
               ULONG                            ulIpIfInstanceNumber,
                PCOSA_DML_IP_V6PREFIX         pEntry

    Return:
        The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlIpIfSetV6Prefix
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6PREFIX       pEntry
    )
{
    if ( ulIpIfInstanceNumber > COSA_USG_IF_NUM )
    {
        return  CosaDmlIpIfMlanSetV6Prefix(hContext, ulIpIfInstanceNumber, pEntry);
    }
    else
    {
        int                             i       = 0;
        ULONG                           j       = 0;
        UtopiaContext                   utctx = {0};

        AnscTraceFlow(("%s...\n", __FUNCTION__));

        for ( i = 0; i < g_ipif_num; i++ )
        {
            if ( ulIpIfInstanceNumber == g_ipif_be_bufs[i].Cfg.InstanceNumber )
            {
                for ( j = 0; j < g_ipif_be_bufs[i].ulNumOfV6Pre; j++ )
                {
                    if ( g_ipif_be_bufs[i].V6PreList[j].InstanceNumber == pEntry->InstanceNumber )
                    {

                        if (!Utopia_Init(&utctx))
                            return ANSC_STATUS_FAILURE;

                        _syscfg_set_v6pref((char *)g_ipif_names[i], &utctx, pEntry);

                        Utopia_Free(&utctx,1);

    					g_ipif_be_bufs[i].V6PreList[j] = *pEntry;
                        
                        return ANSC_STATUS_SUCCESS;
                    }
                }
            }
        }

        return ANSC_STATUS_CANT_FIND;
    }
}


/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlIpIfGetV6Prefix2
           (
            ANSC_HANDLE                 hContext,
            ULONG                           ulIpIfInstanceNumber,
            PCOSA_DML_IP_V6PREFIX         pEntry        
        );

    Description:
        
        The API adds one IP interface into the system. 
        
    Arguments:
    
        ANSC_HANDLE                 hContext,
        
        ULONG                       ulIpIfInstanceNumber,
        
        ULONG                       ulIndex,
        
        PCOSA_DML_IP_V6PREFIX         pEntry

    Return:
        The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlIpIfGetV6Prefix2 
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_V6PREFIX       pEntry         
    )
{
    if ( ulIpIfInstanceNumber > COSA_USG_IF_NUM )
    {
        return  CosaDmlIpIfMlanGetV6Prefix2(hContext, ulIpIfInstanceNumber, pEntry);
    }
    else
    {
        ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
        int                             i       = 0;
        ULONG                           j       = 0;

        AnscTraceFlow(("%s...\n", __FUNCTION__));

        for ( i = 0; i < g_ipif_num; i++ )
        {
            if ( ulIpIfInstanceNumber == g_ipif_be_bufs[i].Cfg.InstanceNumber )
            {
                for ( j = 0; j < g_ipif_be_bufs[i].ulNumOfV6Pre; j++ )
                {
                    if ( g_ipif_be_bufs[i].V6PreList[j].InstanceNumber == pEntry->InstanceNumber )
                    {
                        *pEntry = g_ipif_be_bufs[i].V6PreList[j];
                        return ANSC_STATUS_SUCCESS;
                    }
                }
            }
        }
        
        return returnStatus;
    }
}

/*
 *  IP Interface statistics
 */
/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlIpIfGetStats
           (
                ANSC_HANDLE                 hContext,
               ULONG                       ulIpIfInstanceNumber,
                COSA_DML_IP_STATS           pStats
           );

    Description:
    
        Get the IP interface status.
        
    Arguments:
                ANSC_HANDLE                 hContext,
                
               ULONG                       ulIpIfInstanceNumber
                
                COSA_DML_IP_STATS           pStats
    Return:
        The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlIpIfGetStats
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIpIfInstanceNumber,
        PCOSA_DML_IP_STATS          pStats
    )
{
    if ( ulIpIfInstanceNumber >= COSA_USG_IF_NUM )
    {
        return  CosaDmlIpIfMlanGetStats(hContext, ulIpIfInstanceNumber, pStats);
    }
    else
    {
       
        AnscTraceFlow(("%s...\n", __FUNCTION__));

        if (CosaUtilGetIfStats((char *)g_ipif_names[ulIpIfInstanceNumber-1],  (PCOSA_DML_IF_STATS)pStats))
            return ANSC_STATUS_SUCCESS;
        else 
            return ANSC_STATUS_FAILURE;
    }
}

/*
 *  IP Active Ports
 */
/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlIpGetActivePorts
           (
                ANSC_HANDLE                 hContext,
                PULONG                      pulCount

           );

    Description:
    
        This routine is to retrieve the complete list of currently active ports,
        which is a dynamic table.
        
    Arguments:
                ANSC_HANDLE                 hContext,
                
                PULONG                      pulCount
    Return:
        The pointer to the array of IP active ports, allocated by callee. 
        If no entry is found, NULL is returned.

**********************************************************************/
PCOSA_DML_IP_ACTIVE_PORT
CosaDmlIpGetActivePorts
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount
    )
{
    UNREFERENCED_PARAMETER(hContext);
    PCOSA_DML_IP_ACTIVE_PORT        pActivePort     = (PCOSA_DML_IP_ACTIVE_PORT)NULL;
    PCHAR                           pBegin          = NULL;    
    PCHAR                           pEnd            = NULL;  
    PCHAR                           pEnd2           = NULL;  
    FILE                            *pFile          = NULL;
    ULONG                           i               = 0;
    ULONG                           j               = 0;
    ULONG                           ulCount         = 0;
    ULONG                           ulLength        = 0;  
    ULONG                           ulSubLength     = 0;  
    CHAR                            ucLineBuf[256]  = {0};
    CHAR                            ucTempBuf[128]  = {0};

    AnscTraceFlow(("%s...\n", __FUNCTION__));

    v_secure_system("netstat -nt | grep -E  'ESTABLISHED|LISTEN' > /tmp/.netstat_tcp");

    pFile = fopen("/tmp/.netstat_tcp", "r");
    
    if (!pFile) 
    {
        *pulCount = 0;
        
        return NULL;
    }
    
    while ( NULL != fgets(ucLineBuf, sizeof(ucLineBuf), pFile) )
    {
        ulCount++;
    }

    pActivePort = (COSA_DML_IP_ACTIVE_PORT *)AnscAllocateMemory(sizeof(COSA_DML_IP_ACTIVE_PORT) * ulCount);

    if ( !pActivePort )
    {
        fclose(pFile);

        *pulCount = 0;
        
        return NULL;
    }
    else
    {
        *pulCount = ulCount;
    }
    
    /* Seek to the beginning of file */
    fseek(pFile, 0, SEEK_SET);
    
    for (i = 0; i < ulCount && (fgets(ucLineBuf, sizeof(ucLineBuf), pFile) != NULL); i++) 
    {
        pBegin   = ucLineBuf;
        ulLength = strcspn((const char*)pBegin, " ");
        
        /* Skip the first three columns as "Proto" "Recv-Q" "Send-Q" */
        for (j = 0; j < 3; j++) 
        {
            pBegin += ulLength;
            pBegin += strspn((const char*)pBegin, " ");
            ulLength = strcspn((const char*)pBegin, " ");
        }

        for (j = 0; j < 2; j++)
        {
            pEnd   = _ansc_strchr((const char*)pBegin, ':');

            if ( NULL != pEnd )
            {
                ulSubLength = (ULONG)(pEnd - pBegin);
                
                /* Special handle for IPv6 address */
                pEnd2 = _ansc_strchr((const char*)++pEnd, ':');
                
                if ( pEnd2 && (ulLength > (ULONG)(pEnd2 - pBegin)) )
                {
                    AnscZeroMemory(ucTempBuf, 128);  
                    AnscCopyMemory(ucTempBuf, pBegin, ulLength);

                    pEnd2 = strrchr((const char*)ucTempBuf, ':');

                    ulSubLength = (ULONG)(pEnd2 - ucTempBuf);               
                }

                /* To get local/remote ip address from this column */
                if ( 0 == j )
                {
                    AnscCopyMemory(pActivePort[i].LocalIPAddress, pBegin, ulSubLength);
                }
                else 
                {
                    AnscCopyMemory(pActivePort[i].RemoteIPAddress, pBegin, ulSubLength);
                }
                
                /* To get ip port from this column */
                pEnd = pBegin + ulSubLength;

                ulSubLength =  ulLength - ulSubLength - 1;
                
                AnscZeroMemory(ucTempBuf, 128);  
                AnscCopyMemory(ucTempBuf, ++pEnd, ulSubLength);
                
                if ( 0 == j )
                {
                    pActivePort[i].LocalPort = _ansc_atoi((const char*)ucTempBuf);
                }
                else 
                {
                    pActivePort[i].RemotePort = _ansc_atoi((const char*)ucTempBuf);
                }
            }

             
            /* To get the next column */
            pBegin += ulLength;
            pBegin += strspn((const char*)pBegin, " ");
            ulLength = strcspn((const char*)pBegin, " ");
        }

        /* To get the column "State" */
        if ( 0 == _ansc_strncmp((const char*)pBegin, "ESTABLISHED", ulLength))
        {
            pActivePort[i].Status = COSA_DML_IP_PORT_STATUS_Established;
        }
        else if ( 0 == _ansc_strncmp((const char*)pBegin, "LISTEN", ulLength))
        {
            pActivePort[i].Status = COSA_DML_IP_PORT_STATUS_Listen;
        }
    }
    
    fclose(pFile);

    return pActivePort;
}


#endif


