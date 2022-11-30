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

/**************************************************************************

    module: cosa_lanmanagement_apis.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaDmlLanManagementInit
        *  CosaDmlLanManagementSetCfg
        *  CosaDmlLanManagementGetCfg
        *  CosaDmlLanMngm_SetLanIpv6Enable
        *  CosaDmlLanMngm_SetLanIpv6UlaEnable
        *  CosaDmlLanMngm_SetLanIpv6Ula

    -------------------------------------------------------------------

    environment:

        platform independent

    -------------------------------------------------------------------

    author:


    -------------------------------------------------------------------

    revision:

        05/9/2019    initial revision.

**************************************************************************/
#if defined(CUSTOM_ULA)
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include "cosa_lanmanagement_apis.h"
#include "syscfg/syscfg.h"
#include "utapi/utapi.h"
#include "utapi/utapi_util.h"
#include "ccsp_psm_helper.h"
#ifdef _HUB4_PRODUCT_REQ_
#include "platform_hal.h"
#endif
#include "safec_lib_common.h"

#define PSM_LANMANAGEMENTENTRY_LAN_ULA_ENABLE  "dmsb.lanmanagemententry.lanulaenable"
#define PSM_LANMANAGEMENTENTRY_LAN_IPV6_ENABLE "dmsb.lanmanagemententry.lanipv6enable"
#define PSM_LANMANAGEMENTENTRY_LAN_ULA  "dmsb.lanmanagemententry.lanula"
#define PSM_LANMANAGEMENTENTRY_LAN_ULA_PREFIX  "dmsb.lanmanagemententry.lanulaprefix"
#define SYSEVENT_ULA_ADDRESS "ula_address"
#define SYSEVENT_ULA_PREFIX "ula_prefix"
#define SYSEVENT_IPV6_ENABLE "lan_ipv6_enable"
#define SYSEVENT_ULA_ENABLE  "lan_ula_enable"
#define LAN_BRIDGE "brlan0"

extern  ANSC_HANDLE  bus_handle;
extern void* g_pDslhDmlAgent;
extern ANSC_HANDLE bus_handle;
extern int commonSyseventFd ;
extern int commonSyseventToken;

COSA_DML_LANMANAGEMENT_CFG    g_LanMngmCfg = {0};

static int openCommonSyseventConnection() {
    if (commonSyseventFd == -1) {
        commonSyseventFd = s_sysevent_connect((token_t*)&commonSyseventToken);
    }
    return 0;
}

static int iface_get_hwaddr(const char *ifname, char *mac, size_t size)
{
    int sockfd;
    struct ifreq ifr;
    unsigned char *ptr;

    if (!ifname || !mac || size < sizeof("00:00:00:00:00:00"))
        return -1;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        return -1;
    }

    snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "%s", ifname);
    if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) == -1)
    {
        if (ENODEV == errno)
        {
            fprintf(stderr, "%s interface is not present, cannot get MAC Address\n", ifname);
        }
        else
        {
            fprintf(stderr, "%s interface is present, but got an error:%d while getting MAC Address\n",
                    ifname, errno);
        }
	fprintf(stderr," %s interface not present :",ifname);
	perror("ioctl");
        close(sockfd);
        return -1;
    }

    ptr = (unsigned char *)ifr.ifr_hwaddr.sa_data;
    snprintf(mac, size, "%02x:%02x:%02x:%02x:%02x:%02x",
            ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]);

    close(sockfd);
    return 0;
}

int generate_ipv6_eui_address(char *eui, int eui_len) {
    char macStr[32] = {0};
    unsigned int mac[8] = {0};
    char tmp[8] = {0};
    int idx = 0;
    int idj = 0;
#ifdef _HUB4_PRODUCT_REQ_
    BOOLEAN baseMac = FALSE;
#endif

    if (iface_get_hwaddr(LAN_BRIDGE, macStr, sizeof(macStr)) != 0) {
        fprintf(stderr, "get the mac of %s error!\n", LAN_BRIDGE);
#ifdef _HUB4_PRODUCT_REQ_
        if (platform_hal_GetBaseMacAddress(macStr) == 0)
            baseMac = TRUE;
        else
#endif
        return ANSC_STATUS_FAILURE;
    }

    while(macStr[idx] != '\0') {
        tmp[0] = '\0';
        strncpy(tmp, &macStr[idx], 2);
        mac[idj] = strtol(tmp, NULL, 16);
#ifdef _HUB4_PRODUCT_REQ_
        if(baseMac)
        {
            idx+=2;
            if(idj == 5)
                mac[5]+=1;
        }
        else
            idx+=3;
#else
        idx+=3;
#endif
        idj+=1;
    }
   
    snprintf(eui, eui_len, "%02x%02x:%02x%02x:"
        "%02x%02x:%02x%02x",
        (mac[0] ^ 0x02),
        mac[1],
        mac[2],
        0xff,
        0xfe,
        mac[3],
        mac[4],
        mac[5]);
	
    return ANSC_STATUS_SUCCESS;
}

static int generateIpv6LanPrefix(char *lanPrefix, int lanPrefixLen) {
    char serial_num[255] = {0};
    char tmp[8] = {0};
    int idx =0;
    unsigned char ula[16] = {0};
    /* CID: 118948 Out-of-bounds access - HAL layer access SNO as 255*/
    if(platform_hal_GetSerialNumber(serial_num) != 0) {
        AnscTraceWarning(("%s platform_hal_GetSerialNumber failure \n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    ula[7] = 0xfd;

    /* extract binary data from string */
    for (idx = 0; idx < 7; idx++) {
        tmp[0] = '\0';
        strncpy(tmp, &serial_num[ 0 + (idx * 2) ], 2);
        ula[6 - idx] = strtol(tmp, NULL, 16);
    }

    snprintf(lanPrefix, lanPrefixLen, "%02x%02x:%02x%02x:"
       "%02x%02x:%02x%02x",
        ula[7],
        ula[6],
        ula[5],
        ula[4],
        ula[3],
        ula[2],
        ula[1],
        ula[0]);

    return ANSC_STATUS_SUCCESS;
}

static int setLanIpv6ULAPrefix(char *ula_prefix) {

    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    if(CCSP_SUCCESS != PSM_Set_Record_Value2(bus_info, g_GetSubsystemPrefix(g_pDslhDmlAgent), PSM_LANMANAGEMENTENTRY_LAN_ULA_PREFIX, (UINT)NULL, ula_prefix) )
    {
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}

static int setLanIpv6ULA(char *ula_prefix) {

    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    if(CCSP_SUCCESS != PSM_Set_Record_Value2(bus_info, g_GetSubsystemPrefix(g_pDslhDmlAgent), PSM_LANMANAGEMENTENTRY_LAN_ULA, (UINT)NULL, ula_prefix) )
    {
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}


static ANSC_STATUS setLanIpv6Enable(BOOLEAN ipv6_enable) {

    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;

    if(CCSP_SUCCESS != PSM_Set_Record_Value2(bus_info, g_GetSubsystemPrefix(g_pDslhDmlAgent), PSM_LANMANAGEMENTENTRY_LAN_IPV6_ENABLE, (UINT)NULL, ( ipv6_enable ) ? "TRUE"  : "FALSE"))
    {
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}

static ANSC_STATUS setLanIpv6UlaEnable(BOOLEAN ula_enable) {

    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;

    if(CCSP_SUCCESS != PSM_Set_Record_Value2(bus_info, "eRT.", PSM_LANMANAGEMENTENTRY_LAN_ULA_ENABLE, (UINT)NULL, ( ula_enable ) ? "TRUE"  : "FALSE"))
    {
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS CosaDmlLanMngm_SetLanIpv6Enable(BOOLEAN value) {

    if(commonSyseventFd == -1) {
        openCommonSyseventConnection();
    }

    if(value == TRUE) {
        sysevent_set(commonSyseventFd, commonSyseventToken, SYSEVENT_IPV6_ENABLE, "true", 0);
    }
    else {
        sysevent_set(commonSyseventFd, commonSyseventToken, SYSEVENT_IPV6_ENABLE, "false", 0);
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS CosaDmlLanMngm_SetLanIpv6UlaEnable(BOOLEAN value) {

    if(commonSyseventFd == -1) {
        openCommonSyseventConnection();
    }

    if(value == TRUE) {
        sysevent_set(commonSyseventFd, commonSyseventToken, SYSEVENT_ULA_ENABLE, "true", 0);
    }
    else {
        sysevent_set(commonSyseventFd, commonSyseventToken, SYSEVENT_ULA_ENABLE, "false", 0);
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS CosaDmlLanMngm_SetLanIpv6Ula(char *ula_prefix, char *ula) {
   
    char eui_address[64] = {0};
    char lan_prefix[64]  = {0};
    char lan_address[64] = {0};
    char generated_lan_prefix[64]  = {0};
    errno_t rc = -1;

    if(commonSyseventFd == -1) {
        openCommonSyseventConnection();
    }

    if ((ula_prefix == NULL) || (ula == NULL))
    {
        AnscTraceWarning(("%s:NULL value seen for ula_prefix or ula \n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    if( generate_ipv6_eui_address(eui_address, sizeof(eui_address)) != 0 )
    {
        AnscTraceWarning(("%s generate_ipv6_eui_address failure \n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    if((strlen(ula_prefix) == 0) || strncmp(ula_prefix, "undefined",strlen("undefined")) == 0)
    {
        if (generateIpv6LanPrefix(generated_lan_prefix, sizeof(generated_lan_prefix)) != 0)
        {
            AnscTraceWarning(("%s generateIpv6LanPrefix failure \n", __FUNCTION__));
            return ANSC_STATUS_FAILURE;
        }
        rc = sprintf_s(lan_address, sizeof(lan_address), "%s:%s", generated_lan_prefix, eui_address);
        if(rc < EOK) ERR_CHK(rc);
        strncpy(lan_prefix, generated_lan_prefix, sizeof(lan_prefix));
        strncpy(ula_prefix, generated_lan_prefix, strlen(generated_lan_prefix));
    }
    else
    {
        rc = sprintf_s(lan_address, sizeof(lan_address), "%s:%s", ula_prefix, eui_address);
        if(rc < EOK) ERR_CHK(rc);
        strncpy(lan_prefix, ula_prefix, sizeof(lan_prefix));
    }
    if ((IsValid_ULAAddress(lan_address) == 1))
    {
        strncpy(lan_prefix, ula_prefix, sizeof(lan_prefix));
        strcat(lan_prefix, "::/64");
        strncpy(ula, lan_address, strlen(lan_address)+1);
        sysevent_set(commonSyseventFd, commonSyseventToken, SYSEVENT_ULA_ADDRESS, lan_address, 0);
        sysevent_set(commonSyseventFd, commonSyseventToken, SYSEVENT_ULA_PREFIX, lan_prefix, 0);
#if defined (_HUB4_PRODUCT_REQ_)
        sysevent_set(commonSyseventFd, commonSyseventToken, SYSEVENT_ULA_ENABLE, "true", 0);
        sysevent_set(commonSyseventFd, commonSyseventToken, SYSEVENT_IPV6_ENABLE, "true", 0);	
#endif
    }
    else
    {
        sysevent_set(commonSyseventFd, commonSyseventToken, SYSEVENT_ULA_ADDRESS, "", 0);
        sysevent_set(commonSyseventFd, commonSyseventToken, SYSEVENT_ULA_PREFIX, "", 0);
#if defined (_HUB4_PRODUCT_REQ_)
        sysevent_set(commonSyseventFd, commonSyseventToken, SYSEVENT_ULA_ENABLE, "false", 0);
        sysevent_set(commonSyseventFd, commonSyseventToken, SYSEVENT_IPV6_ENABLE, "false", 0);	
#endif
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS CosaDmlLanManagement_SetLanIpv6Ula(char *lan_prefix, int lan_prefix_len, char *lan_ula, int lan_ula_len) {
    char prefix[64] = {0};
    char lan_eui[64] = {0};

    if((strlen(lan_prefix) == 0) || (strncmp(lan_prefix, "undefined", lan_prefix_len) == 0)) {
        if (generateIpv6LanPrefix(lan_prefix, lan_prefix_len) != 0)
        {
            AnscTraceWarning(("%s generateIpv6LanPrefix failure \n", __FUNCTION__));
            return ANSC_STATUS_FAILURE;
        }
        if (generate_ipv6_eui_address(lan_eui, sizeof(lan_eui)) != 0) 
        {
            AnscTraceWarning(("%s generate_ipv6_eui_address failure \n", __FUNCTION__));
            return ANSC_STATUS_FAILURE;
        }
        snprintf(lan_ula, lan_ula_len, "%s:%s", lan_prefix, lan_eui);

        CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
        if(CCSP_SUCCESS != PSM_Set_Record_Value2(bus_info, g_GetSubsystemPrefix(g_pDslhDmlAgent),
                       PSM_LANMANAGEMENTENTRY_LAN_ULA_PREFIX, (UINT)NULL, lan_prefix) ) {
            return ANSC_STATUS_FAILURE;
        }
        if(CCSP_SUCCESS != PSM_Set_Record_Value2(bus_info, g_GetSubsystemPrefix(g_pDslhDmlAgent),
                       PSM_LANMANAGEMENTENTRY_LAN_ULA, (UINT)NULL, lan_ula) ) {
            return ANSC_STATUS_FAILURE;
        }
    }

    strncpy(prefix, lan_prefix, sizeof(prefix));
    strcat(prefix, "::/64");

    if(commonSyseventFd == -1) {
        openCommonSyseventConnection();
    }
    sysevent_set(commonSyseventFd, commonSyseventToken, SYSEVENT_ULA_PREFIX, prefix, 0);
    sysevent_set(commonSyseventFd, commonSyseventToken, SYSEVENT_ULA_ADDRESS, lan_ula, 0);
#if defined (_HUB4_PRODUCT_REQ_)
    sysevent_set(commonSyseventFd, commonSyseventToken, SYSEVENT_ULA_ENABLE, "true", 0);
    sysevent_set(commonSyseventFd, commonSyseventToken, SYSEVENT_IPV6_ENABLE, "true", 0);    
#endif
    return ANSC_STATUS_SUCCESS;
}

static ANSC_STATUS CosaDmlLanManagement_GetCfg(ANSC_HANDLE hContext, PCOSA_DML_LANMANAGEMENT_CFG pLanMngmCfg) {
    UNREFERENCED_PARAMETER(hContext);
    char *pIpv6_enable = NULL;
    char *pUla_enable  = NULL;
    char *pUla_prefix  = NULL;
    char *pUla = NULL;

    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;

    if(CCSP_SUCCESS != PSM_Get_Record_Value2(bus_info, g_GetSubsystemPrefix(g_pDslhDmlAgent), PSM_LANMANAGEMENTENTRY_LAN_IPV6_ENABLE, NULL, &pIpv6_enable))
    {
        bus_info->freefunc(pIpv6_enable);
        return ANSC_STATUS_FAILURE;
    }

    if(CCSP_SUCCESS != PSM_Get_Record_Value2(bus_info, g_GetSubsystemPrefix(g_pDslhDmlAgent), PSM_LANMANAGEMENTENTRY_LAN_ULA_ENABLE, NULL, &pUla_enable))
    {
        bus_info->freefunc(pUla_enable);
        return ANSC_STATUS_FAILURE;
    }

    if(CCSP_SUCCESS != PSM_Get_Record_Value2(bus_info, g_GetSubsystemPrefix(g_pDslhDmlAgent), PSM_LANMANAGEMENTENTRY_LAN_ULA, NULL, &pUla))
    {
        if(pUla != NULL)
            bus_info->freefunc(pUla);
        return ANSC_STATUS_FAILURE;
    }

    if(CCSP_SUCCESS != PSM_Get_Record_Value2(bus_info, g_GetSubsystemPrefix(g_pDslhDmlAgent), PSM_LANMANAGEMENTENTRY_LAN_ULA_PREFIX, NULL, &pUla_prefix))
    {
        if(pUla_prefix != NULL)
            bus_info->freefunc(pUla_prefix);
        return ANSC_STATUS_FAILURE;
    }

    /* CID:58531 Dereference before null check*/
    if(pLanMngmCfg == NULL || pIpv6_enable == NULL || pUla_enable == NULL)
        return ANSC_STATUS_FAILURE;

    if ( strncmp(pIpv6_enable, "TRUE", 4 ) == 0) {
        pLanMngmCfg->LanIpv6Enable = TRUE;
    }
    else {
        pLanMngmCfg->LanIpv6Enable = FALSE;
    }

    if ( strncmp(pUla_enable, "TRUE", 4 ) == 0) {
        pLanMngmCfg->LanIpv6UlaEnable = TRUE;
    }
    else {
        pLanMngmCfg->LanIpv6UlaEnable = FALSE;
    }

    if(pIpv6_enable != NULL)
        bus_info->freefunc(pIpv6_enable);

    if(pUla_enable != NULL)
        bus_info->freefunc(pUla_enable);

    if(pUla_prefix != NULL) {
        strncpy(pLanMngmCfg->LanIpv6UlaPrefix , pUla_prefix, 64);
        bus_info->freefunc(pUla_prefix);
    }

    if(pUla != NULL) {
        strncpy(pLanMngmCfg->LanIpv6Ula , pUla, 64);
        bus_info->freefunc(pUla);
    }

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlLanManagementInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    UNREFERENCED_PARAMETER(hDml);
    PCOSA_DML_LANMANAGEMENT_CFG pLanMngmCfg = (PCOSA_DML_LANMANAGEMENT_CFG) phContext;
    if(CosaDmlLanManagement_GetCfg(NULL, pLanMngmCfg) == ANSC_STATUS_FAILURE) {
        AnscTraceWarning(("%s CosaDmlLanManagementGetCfg() failure \n", __FUNCTION__));
    }

    CosaDmlLanManagement_SetLanIpv6Ula(pLanMngmCfg->LanIpv6UlaPrefix, sizeof(pLanMngmCfg->LanIpv6UlaPrefix),
                                       pLanMngmCfg->LanIpv6Ula, sizeof(pLanMngmCfg->LanIpv6Ula));

    AnscCopyMemory(&g_LanMngmCfg, pLanMngmCfg, sizeof(COSA_DML_LANMANAGEMENT_CFG));

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlLanManagementSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_LANMANAGEMENT_CFG pLanMngmCfg
    )
{
    UNREFERENCED_PARAMETER(hContext);
    BOOLEAN isUlaPrefixChanged = FALSE, isIpv6UlaEnableChanged = FALSE, isIpv6EnableChanged = FALSE;

    if (pLanMngmCfg == NULL) {
        return ANSC_STATUS_FAILURE;
    }

    if(strcmp(g_LanMngmCfg.LanIpv6UlaPrefix, pLanMngmCfg->LanIpv6UlaPrefix) != 0) {
        if(setLanIpv6ULAPrefix(pLanMngmCfg->LanIpv6UlaPrefix) != ANSC_STATUS_SUCCESS) {
            AnscTraceWarning(("%s: setLanIpv6ULAPrefix() failure \n", __FUNCTION__));
            return ANSC_STATUS_FAILURE;
        }

        CosaDmlLanMngm_SetLanIpv6Ula(pLanMngmCfg->LanIpv6UlaPrefix, pLanMngmCfg->LanIpv6Ula);

        if(setLanIpv6ULA(pLanMngmCfg->LanIpv6Ula) != ANSC_STATUS_SUCCESS) {
            AnscTraceWarning(("%s: setLanIpv6ULA() failure \n", __FUNCTION__));
            return ANSC_STATUS_FAILURE;
        }
        isUlaPrefixChanged = TRUE;
    }

    if(g_LanMngmCfg.LanIpv6UlaEnable != pLanMngmCfg->LanIpv6UlaEnable) {
        if(setLanIpv6UlaEnable(pLanMngmCfg->LanIpv6UlaEnable) != ANSC_STATUS_SUCCESS) {
            AnscTraceWarning(("%s: LanIpv6UlaEnable() failure \n", __FUNCTION__));
            return ANSC_STATUS_FAILURE;
        }
        CosaDmlLanMngm_SetLanIpv6UlaEnable(pLanMngmCfg->LanIpv6UlaEnable);
        if(!pLanMngmCfg->LanIpv6UlaEnable)
        {
            system("killall zebra");
        }
        isIpv6UlaEnableChanged = TRUE;
    }
    if(g_LanMngmCfg.LanIpv6Enable != pLanMngmCfg->LanIpv6Enable) {
        if(setLanIpv6Enable(pLanMngmCfg->LanIpv6Enable) != ANSC_STATUS_SUCCESS) {
            AnscTraceWarning(("%s: setLanIpv6Enable() failure \n", __FUNCTION__));
            return ANSC_STATUS_FAILURE;
        }
        CosaDmlLanMngm_SetLanIpv6Enable(pLanMngmCfg->LanIpv6Enable);
        isIpv6EnableChanged = TRUE;
    }

    AnscCopyMemory(&g_LanMngmCfg, pLanMngmCfg, sizeof(COSA_DML_LANMANAGEMENT_CFG));

    /* Restarts entire LAN services */
    if( ( TRUE == isUlaPrefixChanged ) || ( TRUE == isIpv6UlaEnableChanged ) || ( TRUE == isIpv6EnableChanged ) )
    {
       sysevent_set(commonSyseventFd, commonSyseventToken, "lan-stop",  NULL, 0);
       sysevent_set(commonSyseventFd, commonSyseventToken, "lan-start", NULL, 0);
    }

    if (isUlaPrefixChanged && pLanMngmCfg->LanIpv6UlaEnable)
    {
        system("killall zebra; sysevent set zebra-restart");
    }
    system("gw_lan_refresh");

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlLanManagementGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_LANMANAGEMENT_CFG          pLanMngmCfg
    )
{
    UNREFERENCED_PARAMETER(hContext);
    if (pLanMngmCfg)
        AnscCopyMemory(pLanMngmCfg, &g_LanMngmCfg, sizeof(COSA_DML_LANMANAGEMENT_CFG));

    return ANSC_STATUS_SUCCESS;
}
#endif
