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

#include "cosa_lanmanagement_apis.h"

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

#define PSM_LANMANAGEMENTENTRY_LAN_ULA_ENABLE  "dmsb.lanmanagemententry.lanulaenable"
#define PSM_LANMANAGEMENTENTRY_LAN_IPV6_ENABLE "dmsb.lanmanagemententry.lanipv6enable"
#define PSM_LANMANAGEMENTENTRY_LAN_ULA  "dmsb.lanmanagemententry.lanula"
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
        commonSyseventFd = s_sysevent_connect(&commonSyseventToken);
    }
    return 0;
}

static int setLanIpv6ULA(char *ula_prefix) {

    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    if(CCSP_SUCCESS != PSM_Set_Record_Value2(bus_info, g_GetSubsystemPrefix(g_pDslhDmlAgent), PSM_LANMANAGEMENTENTRY_LAN_ULA, NULL, ula_prefix) )
    {
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}


static ANSC_STATUS setLanIpv6Enable(BOOLEAN ipv6_enable) {

    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;

    if(CCSP_SUCCESS != PSM_Set_Record_Value2(bus_info, g_GetSubsystemPrefix(g_pDslhDmlAgent), PSM_LANMANAGEMENTENTRY_LAN_IPV6_ENABLE, NULL, ( ipv6_enable ) ? "TRUE"  : "FALSE"))
    {
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}

static ANSC_STATUS setLanIpv6UlaEnable(BOOLEAN ula_enable) {

    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;

    if(CCSP_SUCCESS != PSM_Set_Record_Value2(bus_info, "eRT.", PSM_LANMANAGEMENTENTRY_LAN_ULA_ENABLE, NULL, ( ula_enable ) ? "TRUE"  : "FALSE"))
    {
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
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

ANSC_STATUS CosaDmlLanMngm_SetLanIpv6Ula(char *ula_address) {
   
    char ula_prefix[64] = {0};
    int count = 0;
    int i;

    if(commonSyseventFd == -1) {
        openCommonSyseventConnection();
    }

    for(i=0; i<strlen(ula_address); i++ ) { 
        if(ula_address[i] == ':') 
            count++; 
        if(count == 4) 
            break; 
        else 
            ula_prefix[i] = ula_address[i]; 
    } 
    strcat(ula_prefix, "::/64");

    sysevent_set(commonSyseventFd, commonSyseventToken, SYSEVENT_ULA_ADDRESS, ula_address, 0);
    sysevent_set(commonSyseventFd, commonSyseventToken, SYSEVENT_ULA_PREFIX, ula_prefix, 0);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS CosaDmlLanManagement_SetLanIpv6Ula(char *lan_ula, int lan_ula_len) {
    unsigned int mac[8] = {0};
    char macStr[32] = {0};
    char serial_num[32] = {0};
    char command[1024] = {0};
    FILE *pf = NULL;
    char tmp[8] = {0};
    int idx =0;
    int idj = 0;
    unsigned char ula[64] = {0};
    char ula_address[64] = {0};
    char ula_prefix[64] = {0};
    int count = 0;
    int i;

    if(strncmp(lan_ula, "undefined", lan_ula_len) == 0) {
        if(platform_hal_GetSerialNumber(serial_num) != 0) {
            AnscTraceWarning(("%s platform_hal_GetSerialNumber failure \n", __FUNCTION__));
            return ANSC_STATUS_FAILURE;
        }

        if (iface_get_hwaddr(LAN_BRIDGE, macStr, sizeof(macStr)) != 0) {
            fprintf(stderr, "get the mac of %s error!\n", LAN_BRIDGE);
            return ANSC_STATUS_FAILURE;
        }

        while(macStr[idx] != '\0') {
            tmp[0] = '\0';
            strncpy(tmp, &macStr[idx], 2);
            mac[idj] = strtol(tmp, NULL, 16);
            idx+=3;
            idj+=1;
        }

        ula[15] = 0xfd;
        ula[7] = mac[0] ^ 0x02;
        ula[6] = mac[1];
        ula[5] = mac[2];
        ula[4] = 0xff;
        ula[3] = 0xfe;
        ula[2] = mac[3];
        ula[1] = mac[4];
        ula[0] = mac[5];

        /* extract binary data from string */
        for (idx = 0; idx < 7; idx++) {
            tmp[0] = '\0';
            strncpy(tmp, &serial_num[ 0 + (idx * 2) ], 2);
            ula[14 - idx] = strtol(tmp, NULL, 16);
        }

        snprintf(ula_prefix, 64, "%02x%02x:%02x%02x:"
           "%02x%02x:%02x%02x::/64",
        ula[15],
        ula[14],
        ula[13],
        ula[12],
        ula[11],
        ula[10],
        ula[9],
        ula[8]);

		snprintf(ula_address, 64, "%02x%02x:%02x%02x:"
           "%02x%02x:%02x%02x:"
           "%02x%02x:%02x%02x:"
           "%02x%02x:%02x%02x",
        ula[15],
        ula[14],
        ula[13],
        ula[12],
        ula[11],
        ula[10],
        ula[9],
        ula[8],
        ula[7],
        ula[6],
        ula[5],
        ula[4],
        ula[3],
        ula[2],
        ula[1],
        ula[0]);

        strncpy(lan_ula, ula_address, lan_ula_len);

       CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
       if(CCSP_SUCCESS != PSM_Set_Record_Value2(bus_info, g_GetSubsystemPrefix(g_pDslhDmlAgent),
                       PSM_LANMANAGEMENTENTRY_LAN_ULA, NULL, lan_ula) ) {
           return ANSC_STATUS_FAILURE;
       }
    }

    memset(command, '\0', sizeof(command));

    strncpy(ula_address, lan_ula, sizeof(ula_address));
    memset(ula_prefix, '\0', sizeof(ula_prefix));
    for(i=0; i<strlen(ula_address); i++ ) {
        if(ula_address[i] == ':')
            count++;
        if(count == 4)
            break;
        else
            ula_prefix[i] = ula_address[i];
    }
    strcat(ula_prefix, "::/64");

    if(commonSyseventFd == -1) {
        openCommonSyseventConnection();
    }
    sysevent_set(commonSyseventFd, commonSyseventToken, SYSEVENT_ULA_PREFIX, ula_prefix, 0);
    sysevent_set(commonSyseventFd, commonSyseventToken, SYSEVENT_ULA_ADDRESS, ula_address, 0);

    return ANSC_STATUS_SUCCESS;
}

static ANSC_STATUS CosaDmlLanManagement_GetCfg(ANSC_HANDLE hContext, PCOSA_DML_LANMANAGEMENT_CFG pLanMngmCfg) {
    char *pIpv6_enable = NULL;
    char *pUla_enable  = NULL;
    char *pUla_prefix  = NULL;

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

    if(CCSP_SUCCESS != PSM_Get_Record_Value2(bus_info, g_GetSubsystemPrefix(g_pDslhDmlAgent), PSM_LANMANAGEMENTENTRY_LAN_ULA, NULL, &pUla_prefix))
    {
        if(pUla_prefix != NULL)
            bus_info->freefunc(pUla_prefix);
        return ANSC_STATUS_FAILURE;
    }

    if(pLanMngmCfg == NULL)
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
        strncpy(pLanMngmCfg->LanIpv6Ula , pUla_prefix, 64);
        bus_info->freefunc(pUla_prefix);
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
    PCOSA_DML_LANMANAGEMENT_CFG pLanMngmCfg = (PCOSA_DML_LANMANAGEMENT_CFG) phContext;
    if(CosaDmlLanManagement_GetCfg(NULL, pLanMngmCfg) == ANSC_STATUS_FAILURE) {
        AnscTraceWarning(("%s CosaDmlLanManagementGetCfg() failure \n", __FUNCTION__));
    }

    CosaDmlLanManagement_SetLanIpv6Ula(pLanMngmCfg->LanIpv6Ula, sizeof(pLanMngmCfg->LanIpv6Ula));

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
    if (pLanMngmCfg == NULL) {
        return ANSC_STATUS_FAILURE;
    }

    if(strcmp(g_LanMngmCfg.LanIpv6Ula, pLanMngmCfg->LanIpv6Ula) != 0) {
        if(setLanIpv6ULA(pLanMngmCfg->LanIpv6Ula) != ANSC_STATUS_SUCCESS) {
            AnscTraceWarning(("%s: setLanIpv6ULA() failure \n", __FUNCTION__));
            return ANSC_STATUS_FAILURE;
        }
        CosaDmlLanMngm_SetLanIpv6Ula(pLanMngmCfg->LanIpv6Ula);
    }
    if(g_LanMngmCfg.LanIpv6UlaEnable != pLanMngmCfg->LanIpv6UlaEnable) {
        if(setLanIpv6UlaEnable(pLanMngmCfg->LanIpv6UlaEnable) != ANSC_STATUS_SUCCESS) {
            AnscTraceWarning(("%s: LanIpv6UlaEnable() failure \n", __FUNCTION__));
            return ANSC_STATUS_FAILURE;
        }
        CosaDmlLanMngm_SetLanIpv6UlaEnable(pLanMngmCfg->LanIpv6UlaEnable);
    }
    if(g_LanMngmCfg.LanIpv6Enable != pLanMngmCfg->LanIpv6Enable) {
        if(setLanIpv6Enable(pLanMngmCfg->LanIpv6Enable) != ANSC_STATUS_SUCCESS) {
            AnscTraceWarning(("%s: setLanIpv6Enable() failure \n", __FUNCTION__));
            return ANSC_STATUS_FAILURE;
        }
        CosaDmlLanMngm_SetLanIpv6Enable(pLanMngmCfg->LanIpv6Enable);
    }

    AnscCopyMemory(&g_LanMngmCfg, pLanMngmCfg, sizeof(COSA_DML_LANMANAGEMENT_CFG));
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
    if (pLanMngmCfg)
        AnscCopyMemory(pLanMngmCfg, &g_LanMngmCfg, sizeof(COSA_DML_LANMANAGEMENT_CFG));

    return ANSC_STATUS_SUCCESS;
}
