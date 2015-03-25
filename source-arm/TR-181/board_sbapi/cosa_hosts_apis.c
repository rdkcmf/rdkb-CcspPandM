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

    module: cosa_hosts_apis.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    copyright:

        Cisco Systems, Inc.
        All Rights Reserved.

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
#include "plugin_main_apis.h"
#include "cosa_hosts_apis.h"
#include "cosa_hosts_internal.h"
#include "pacm_manager_utilities.h"

extern void* g_pDslhDmlAgent;

#define NAME_DM_LEN  257

typedef struct _Name_DM 
{
    char name[NAME_DM_LEN];
    char dm[NAME_DM_LEN];
}Name_DM_t;

int g_IPIfNameDMListNum = 0;
Name_DM_t *g_pIPIfNameDMList = NULL;

int g_MoCAADListNum = 0;
Name_DM_t *g_pMoCAADList = NULL;

int g_DHCPv4ListNum = 0;
Name_DM_t *g_pDHCPv4List = NULL;

inline int _mac_string_to_array(char *pStr, unsigned char array[6])
{
    int tmp[6],n,i;
	if(pStr == NULL)
		return -1;
		
    memset(array,0,6);
    n = sscanf(pStr,"%02x:%02x:%02x:%02x:%02x:%02x",&tmp[0],&tmp[1],&tmp[2],&tmp[3],&tmp[4],&tmp[5]);
    if(n==6){
        for(i=0;i<n;i++)
            array[i] = (unsigned char)tmp[i];
        return 0;
    }

    return -1;
}

#if ( defined _COSA_SIM_ )

COSA_DML_HOST_ENTRY  g_user_entrys1 = 
    {
        "33-33-33-33-33-33",
        {
            "\x40\x40\x40\x01"
        },
        COSA_DML_HOST_ADDR_SOURCE_DHCP,
        2322,
        "Layer1Interface1",
        "Layer3Interface1",
        "Jerry",
        TRUE,
        7,   /* Vendor ID size */
        7,   /* Client ID offset */
        11,   /* Client ID size */
        18,  /* User ID offset */
        10  /* User ID size */
          /*"VerizonJerryServerUser123456"*/
    };

COSA_DML_HOST_ENTRY  g_user_entrys2 = 
    {
        "33-33-33-33-33-33",
        {
            "\x40\x40\x40\x02"
        },
        COSA_DML_HOST_ADDR_SOURCE_Static,
        2322,
        "Layer1Interface1",
        "Layer3Interface2",
        "Sam",
        TRUE,
        5,   /* Vendor ID size */
        5,   /* Client ID offset */
        6,   /* Client ID size */
        11,  /* User ID offset */
        14  /* User ID size */
         /*"CiscoSamaPCUser1234567890"*/
    };

UCHAR       g_user_Entrys[1024]      = {0};
ULONG       g_user_pEntrys_len       = sizeof(g_user_Entrys);


ANSC_STATUS
CosaDmlHostsInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DML_HOST_ENTRY            pEntry       = NULL;

    pEntry = (PCOSA_DML_HOST_ENTRY)g_user_Entrys;

    /*Copy first entry */
    AnscCopyMemory(pEntry, &g_user_entrys1, sizeof(COSA_DML_HOST_ENTRY));
    pEntry++;
    /* Copy flexible string */
    AnscCopyString( (PUCHAR)pEntry, "VerizonJerryServerUser123456" );

    /* move pEntry */
    pEntry = (PCOSA_DML_HOST_ENTRY)((PUCHAR)pEntry + _ansc_strlen("VerizonJerryServerUser123456"));

    /* Copy second entry */
    AnscCopyMemory(pEntry, &g_user_entrys2, sizeof(COSA_DML_HOST_ENTRY));
    pEntry++;
    /* Copy flexible string */
    AnscCopyString( (PUCHAR)pEntry, "CiscoSamaPCUser1234567890" );

    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        PCOSA_DML_HOST_ENTRY
        CosaDmlHostsGetHosts
            (
                ANSC_HANDLE                 hContext,
                PULONG                      pulCount
            );

    description:

        This routine is to retrieve the complete list of current hosts, which is a dynamic table.

    argument:   ANSC_HANDLE                 hContext,
                PULONG                      pulCount
                To receive the actual number of entries.

    return:     The pointer to the array of hosts, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/
PCOSA_DML_HOST_ENTRY
CosaDmlHostsGetHosts
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount
    )
{
    PCOSA_DML_HOST_ENTRY            pHostEntry    = NULL;

    pHostEntry = AnscAllocateMemory( g_user_pEntrys_len );
    if ( !pHostEntry )
    {
        *pulCount = 0;
        return NULL;
    }

    AnscCopyMemory(pHostEntry, g_user_Entrys, g_user_pEntrys_len );

    *pulCount = 2;

    return pHostEntry;
}

#elif ( defined _COSA_DRG_TPG_ )


ANSC_STATUS
CosaDmlHostsInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     self

    prototype:

        PCOSA_DML_HOST_ENTRY
        CosaDmlHostsGetHosts
            (
                ANSC_HANDLE                 hContext,
                PULONG                      pulCount
            );

    description:

        This routine is to retrieve the complete list of current hosts, which is a dynamic table.

    argument:   ANSC_HANDLE                 hContext,
                PULONG                      pulCount
                To receive the actual number of entries.

    return:     The pointer to the array of hosts, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/
PCOSA_DML_HOST_ENTRY
CosaDmlHostsGetHosts
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount
    )
{
    *pulCount = 0;

    return NULL;
}


#elif ( defined _COSA_INTEL_USG_ARM_ )
#include <sys/socket.h>
#include "lm_api.h"
#include "ccsp_dm_api.h"

/* BBU */
#if defined (CONFIG_TI_BBU) || defined (CONFIG_TI_BBU_TI)
#include "bbu_api.h"
#include "bbu_interface.h"
#endif

#ifdef CONFIG_TI_PACM
/* PACM */
#include "pacm_config_utilities.h"
#include "pacm_ctx.h"
#include "pacm_manager_utilities.h"
#include "pacm_msm_api.h"
#endif

LmObjectHosts lmHosts = {
    .pHostBoolParaName = {"Active"},
    .pHostIntParaName = {"X_CISCO_COM_ActiveTime", "X_CISCO_COM_InactiveTime", "X_CISCO_COM_RSSI"},
    .pHostUlongParaName = {"X_CISCO_COM_DeviceType", "X_CISCO_COM_NetworkInterface", "X_CISCO_COM_ConnectionStatus", "X_CISCO_COM_OSType","X_COMCAST-COM_LastChange"},
    .pHostStringParaName = {"Alias", "PhysAddress", "IPAddress", "DHCPClient", "AssociatedDevice", "Layer1Interface", "Layer3Interface", "HostName",
                                        "X_CISCO_COM_UPnPDevice", "X_CISCO_COM_HNAPDevice", "X_CISCO_COM_DNSRecords", "X_CISCO_COM_HardwareVendor",
                                        "X_CISCO_COM_SoftwareVendor", "X_CISCO_COM_SerialNumbre", "X_CISCO_COM_DefinedDeviceType",
                                        "X_CISCO_COM_DefinedHWVendor", "X_CISCO_COM_DefinedSWVendor", "AddressSource", "Comments"},
    .pIPv4AddressStringParaName = {"IPAddress"},
    .pIPv6AddressStringParaName = {"IPAddress"}
};
#define MACADDR_SZ 18
#define LM_HOST_OBJECT_NAME_HEADER  "Device.Hosts.Host."
#define STRNCPY_NULL_CHK(dest, src) { if((dest) != NULL ) AnscFreeMemory((dest));\
                                           (dest) = _CloneString((src));}
#define STRSET_NULL_CHK(dest, src)  { if((dest) != NULL) AnscFreeMemory((dest)); \
                                            (dest) = (src);}

char _g_atom_if_ip[4];

char * _CloneString
    (
    const char * src
    )
{
    if(src == NULL) return NULL;
    size_t len = strlen(src) + 1;
    if(len <= 1) return NULL;
    char * dest = AnscAllocateMemory(len);
    if ( dest )
    {
        strncpy(dest, src, len);
        dest[len - 1] = 0;
    }
    return dest;
}

void _get_unwelcome_list()
{
   char *pStr = NULL;
   char ParaName[50];
   int ins = 0;
   memset(_g_atom_if_ip, 0, sizeof(_g_atom_if_ip));
   if(PSM_Get_Record_Value2(g_MessageBusHandle, g_GetSubsystemPrefix(g_pDslhDmlAgent), "dmsb.MultiLAN.PrimaryLAN_l3net", NULL, &pStr) == CCSP_SUCCESS &&
      pStr != NULL){
      ins = atoi(pStr);
      sprintf(ParaName, "dmsb.atom.l3net.%d.V4Addr", ins);
      AnscFreeMemory(pStr);
      pStr = NULL;
      if(PSM_Get_Record_Value2(g_MessageBusHandle, g_GetSubsystemPrefix(g_pDslhDmlAgent), ParaName, NULL, &pStr) == CCSP_SUCCESS && 
         pStr != NULL){
         inet_pton(AF_INET, pStr, _g_atom_if_ip);
         AnscFreeMemory(pStr);
      } 
   } 
}

int host_filter(LM_host_t *host)
{
    int i;
    LM_ip_addr_t *pIp;
    if(0 == *(int*)(_g_atom_if_ip))
        return FALSE;
    for(i = 0; i < host->ipv4AddrAmount; i++){
        pIp = &(host->ipv4AddrList[i]);
        if(0 == memcmp(pIp->addr, _g_atom_if_ip, sizeof(_g_atom_if_ip)))
            return TRUE;
    }
    return FALSE; 
}

void Hosts_FreeHost(PLmObjectHost pHost){
    int i;
    if(pHost == NULL)
        return;
    for(i=0; i<LM_HOST_NumStringPara; i++) 
        if(NULL != pHost->pStringParaValue[i])
            AnscFreeMemory(pHost->pStringParaValue[i]);
    if(pHost->objectName != NULL)
        AnscFreeMemory(pHost->objectName);

    if(pHost->ipv4AddrArray != NULL){
        for(i = 0; i < pHost->numIPv4Addr; i++){
            if(pHost->ipv4AddrArray[i] == NULL)
                continue;
            if (pHost->ipv4AddrArray[i]->pStringParaValue[LM_HOST_IPv4Address_IPAddressId])
            {
                AnscFreeMemory(pHost->ipv4AddrArray[i]->pStringParaValue[LM_HOST_IPv4Address_IPAddressId]);
            }
            AnscFreeMemory(pHost->ipv4AddrArray[i]);
        }
        AnscFreeMemory(pHost->ipv4AddrArray);
    }

    if(pHost->ipv6AddrArray != NULL){
        for(i = 0; i < pHost->numIPv4Addr; i++){
            if(pHost->ipv6AddrArray[i] == NULL)
                continue;
            if (pHost->ipv6AddrArray[i]->pStringParaValue[LM_HOST_IPv4Address_IPAddressId])
            {
                AnscFreeMemory(pHost->ipv6AddrArray[i]->pStringParaValue[LM_HOST_IPv4Address_IPAddressId]);
            }
            AnscFreeMemory(pHost->ipv6AddrArray[i]);
        }
        AnscFreeMemory(pHost->ipv6AddrArray);
    }
    
    AnscFreeMemory(pHost);
}

void Hosts_RmHosts(){
    int i;  
    
    if(lmHosts.numHost == 0)
        return;

    for(i = 0; i < lmHosts.numHost; i++){
        Hosts_FreeHost(lmHosts.hostArray[i]);
    }
    AnscFreeMemory(lmHosts.hostArray); 
    lmHosts.availableInstanceNum = 1;
    lmHosts.hostArray = NULL;
    lmHosts.numHost = 0;
    lmHosts.sizeHost = 0;
    return;
}

PLmObjectHost Hosts_AddHost(int instanceNum)
{
    //printf("in Hosts_AddHost %d \n", instanceNum);
    PLmObjectHost pHost = AnscAllocateMemory(sizeof(LmObjectHost));
    if(pHost == NULL)
    {
        return NULL;
    }
    pHost->instanceNum = instanceNum;
    /* Compose Host object name. */
    char objectName[100] = LM_HOST_OBJECT_NAME_HEADER;
    char instanceNumStr[50] = {0};
    _ansc_itoa(pHost->instanceNum, instanceNumStr, 10);
    strcat(instanceNumStr, ".");
    strcat(objectName, instanceNumStr);
    pHost->objectName = _CloneString(objectName);

    pHost->l3unReachableCnt = 0;
    pHost->l1unReachableCnt = 0;
    pHost->ipv4AddrArray = NULL;
    pHost->sizeIPv4Addr = 0;
    pHost->numIPv4Addr = 0;
    pHost->ipv6AddrArray = NULL;
    pHost->sizeIPv6Addr = 0;
    pHost->numIPv6Addr = 0;
    
    /* Default it is inactive. */
    pHost->bBoolParaValue[LM_HOST_ActiveId] = FALSE;
    pHost->ipv4Active = FALSE;
    pHost->ipv6Active = FALSE;
    pHost->availableInstanceNumIPv4Address = 1;
    pHost->availableInstanceNumIPv6Address = 1;
    //$HL 9/5/2013
    ANSC_UNIVERSAL_TIME currentTime = {0};
    AnscGetLocalTime(&currentTime);
    pHost->activityChangeTime  = AnscCalendarToSecond(&currentTime);

    pHost->iIntParaValue[LM_HOST_X_CISCO_COM_ActiveTimeId] = -1;
    pHost->iIntParaValue[LM_HOST_X_CISCO_COM_RSSIId] = INT_MAX;

    int i;
    for(i=0; i<LM_HOST_NumStringPara; i++) pHost->pStringParaValue[i] = NULL;

    if(lmHosts.numHost >= lmHosts.sizeHost){
        lmHosts.sizeHost += LM_HOST_ARRAY_STEP;
        PLmObjectHost *newArray = AnscAllocateMemory(lmHosts.sizeHost * sizeof(PLmObjectHost));
        for(i=0; i<lmHosts.numHost; i++){
            newArray[i] = lmHosts.hostArray[i];
        }
        PLmObjectHost *backupArray = lmHosts.hostArray;
        lmHosts.hostArray = newArray;
        if(backupArray) AnscFreeMemory(backupArray);
    }
    pHost->id = lmHosts.numHost;
    lmHosts.hostArray[pHost->id] = pHost;
    lmHosts.numHost++;
    return pHost;
}

PLmObjectHost Hosts_FindHostByPhysAddress(char * physAddress)
{
    int i = 0;
    for(; i<lmHosts.numHost; i++){
        if(AnscEqualString(lmHosts.hostArray[i]->pStringParaValue[LM_HOST_PhysAddressId], physAddress, FALSE)){
            return lmHosts.hostArray[i];
        }
    }
    return NULL;
}

PLmObjectHost Hosts_AddHostByPhysAddress(char * physAddress)
{
    char comments[256] = {0};

    if(!physAddress) return NULL;
    if(strlen(physAddress) != MACADDR_SZ-1) return NULL;
    PLmObjectHost pHost = Hosts_FindHostByPhysAddress(physAddress);

    if(pHost) return pHost;

    pHost = Hosts_AddHost(lmHosts.availableInstanceNum);
    if(pHost){
        pHost->pStringParaValue[LM_HOST_PhysAddressId] = _CloneString(physAddress);
        //pHost->pStringParaValue[LM_HOST_HostNameId] = _CloneString(physAddress);
        //_getLanHostComments(physAddress, comments);
        //if ( comments[0] != 0 )
        //{
        //    pHost->pStringParaValue[LM_HOST_Comments] = _CloneString(comments);
        //}
        pHost->pStringParaValue[LM_HOST_Layer1InterfaceId] = _CloneString("Ethernet");
        lmHosts.availableInstanceNum++;
    }
    return pHost;
}

PLmObjectHostIPv4Address
Host_AddIPv4Address
    (
        PLmObjectHost pHost,
        int instanceNum,
        char * ipv4Address
    )
{
    /* check if the address has already exist. */
    int i;
    for(i=0; i<pHost->numIPv4Addr; i++){
        /* If IP address already exists, return. */
        if(AnscEqualString(pHost->ipv4AddrArray[i]->pStringParaValue[LM_HOST_IPv4Address_IPAddressId],ipv4Address, FALSE))
            return pHost->ipv4AddrArray[i]; 
    }

    for(i=0; i<pHost->numIPv4Addr; i++){
        /* If instance number is occuppied, assign a new instance number. It may not happen in DHCP mode. */
        if(pHost->ipv4AddrArray[i]->instanceNum == instanceNum){
            instanceNum = pHost->availableInstanceNumIPv4Address;
            pHost->availableInstanceNumIPv4Address++;
        }
    }

    PLmObjectHostIPv4Address pIPv4Address = AnscAllocateMemory(sizeof(LmObjectHostIPv4Address));
    pIPv4Address->instanceNum = instanceNum;
    pIPv4Address->pStringParaValue[LM_HOST_IPv4Address_IPAddressId] = _CloneString(ipv4Address);
    if(pHost->availableInstanceNumIPv4Address <= pIPv4Address->instanceNum)
        pHost->availableInstanceNumIPv4Address = pIPv4Address->instanceNum + 1;

    if(pHost->numIPv4Addr >= pHost->sizeIPv4Addr){
        pHost->sizeIPv4Addr += LM_HOST_ARRAY_STEP;
        PLmObjectHostIPv4Address *newArray = AnscAllocateMemory(pHost->sizeIPv4Addr * sizeof(PLmObjectHostIPv4Address));
        for(i=0; i<pHost->numIPv4Addr; i++){
            newArray[i] = pHost->ipv4AddrArray[i];
        }
        PLmObjectHostIPv4Address *backupArray = pHost->ipv4AddrArray;
        pHost->ipv4AddrArray = newArray;
        if(backupArray) AnscFreeMemory(backupArray);
    }
    pIPv4Address->id = pHost->numIPv4Addr;
    pHost->ipv4AddrArray[pIPv4Address->id] = pIPv4Address;
    pHost->numIPv4Addr++;
    return pIPv4Address;
}

PLmObjectHostIPv6Address
Host_AddIPv6Address
    (
        PLmObjectHost pHost,
        int instanceNum,
        char * ipv6Address
    )
{
    /* check if the address has already exist. */
    int i = 0;
    for(i=0; i<pHost->numIPv6Addr; i++){
        /* If IP address already exists, return. */
        if(AnscEqualString(pHost->ipv6AddrArray[i]->pStringParaValue[LM_HOST_IPv6Address_IPAddressId],ipv6Address, FALSE))
            return pHost->ipv6AddrArray[i]; 
    }

    for(i=0; i<pHost->numIPv6Addr; i++){
        /* If instance number is occuppied, assign a new instance number. It may not happen in DHCP mode. */
        if(pHost->ipv6AddrArray[i]->instanceNum == instanceNum){
            instanceNum = pHost->availableInstanceNumIPv6Address;
            pHost->availableInstanceNumIPv6Address++;
        }
    }

    PLmObjectHostIPv6Address pIPv6Address = AnscAllocateMemory(sizeof(LmObjectHostIPv6Address));
    pIPv6Address->instanceNum = instanceNum;
    pIPv6Address->pStringParaValue[LM_HOST_IPv6Address_IPAddressId] = _CloneString(ipv6Address);
    if(pHost->availableInstanceNumIPv6Address <= pIPv6Address->instanceNum)
        pHost->availableInstanceNumIPv6Address = pIPv6Address->instanceNum + 1;

    if(pHost->numIPv6Addr >= pHost->sizeIPv6Addr){
        pHost->sizeIPv6Addr += LM_HOST_ARRAY_STEP;
        PLmObjectHostIPv6Address *newArray = AnscAllocateMemory(pHost->sizeIPv6Addr * sizeof(PLmObjectHostIPv6Address));
        for(i=0; i<pHost->numIPv6Addr; i++){
            newArray[i] = pHost->ipv6AddrArray[i];
        }
        PLmObjectHostIPv6Address *backupArray = pHost->ipv6AddrArray;
        pHost->ipv6AddrArray = newArray;
        if(backupArray) AnscFreeMemory(backupArray);
    }
    pIPv6Address->id = pHost->numIPv6Addr;
    pHost->ipv6AddrArray[pIPv6Address->id] = pIPv6Address;
    pHost->numIPv6Addr++;
    return pIPv6Address;
}

inline char* _get_host_mediaType(enum LM_MEDIA_TYPE m_type)
{
    switch (m_type){
        case LM_MEDIA_TYPE_MOCA:
            return _CloneString("MoCA");
        case LM_MEDIA_TYPE_WIFI:
            return _CloneString("WiFi");
        case LM_MEDIA_TYPE_ETHERNET:
        default:
            return _CloneString("Ethernet");
    }
}

inline char* _get_addr_source(enum LM_ADDR_SOURCE source )
{
    switch (source){
        case LM_ADDRESS_SOURCE_DHCP:
            return _CloneString("DHCP");
        case LM_ADDRESS_SOURCE_STATIC:
            return _CloneString("Static");
        case LM_ADDRESS_SOURCE_RESERVED:
            return _CloneString("ReservedIP");
        case LM_ADDRESS_SOURCE_NONE:
        default:
            return _CloneString("None");
    }
}

inline void _get_host_ipaddress(LM_host_t *pSrcHost, PLmObjectHost pHost)
{
    int i;    
    LM_ip_addr_t *pIp;
    char str[100]; 
    for(i = 0; i < pSrcHost->ipv4AddrAmount ;i++){
        pIp = &(pSrcHost->ipv4AddrList[i]);
        inet_ntop(AF_INET, pIp->addr, str, sizeof(str));
        Host_AddIPv4Address(pHost, 1, str);
        if(i == 0){
            STRNCPY_NULL_CHK(pHost->pStringParaValue[LM_HOST_IPAddressId], str);
            // !!!!!!!!!!!!!!!! Lm struct not support mutli address source
            /* Mark TrueStaticIP client */
            if ( 0 == strncmp(str, "192.168", 7) ||
                 0 == strncmp(str, "10.", 3) ||
                 0 == strncmp(str, "172.", 4)
               )
            {
                pHost->bTrueStaticIPClient = FALSE;
            }
            else
            {
                pHost->bTrueStaticIPClient = TRUE;
            }
            STRSET_NULL_CHK(pHost->pStringParaValue[LM_HOST_AddressSource], _get_addr_source(pIp->addrSource));
            if(pIp->addrSource == LM_ADDRESS_SOURCE_DHCP){
                _get_dmbyname(g_DHCPv4ListNum, g_pDHCPv4List, &(pHost->pStringParaValue[LM_HOST_DHCPClientId]), pHost->pStringParaValue[LM_HOST_PhysAddressId]);
            }else if(pHost->pStringParaValue[LM_HOST_DHCPClientId] != NULL){
                LanManager_Free(pHost->pStringParaValue[LM_HOST_DHCPClientId]);
                pHost->pStringParaValue[LM_HOST_DHCPClientId] = NULL;
            }
            pHost->LeaseTime = pIp->LeaseTime;
        }
    }
    for(i = 0; i < pSrcHost->ipv6AddrAmount ;i++){
        pIp = &(pSrcHost->ipv6AddrList[i]);
        inet_ntop(AF_INET6, pIp->addr, str, sizeof(str));
        Host_AddIPv6Address(pHost, 1, str);
    }
}
void _init_DM_List(int *num, Name_DM_t **pList, char *path, char *name)
{
    int i;
    char dm[200];
    char (*dmnames)[CDM_PATH_SZ];
    int nname = 0;
    int dmlen;
    
    if(*pList != NULL){
        AnscFreeMemory(*pList);
        *pList = NULL;
    }
 
    if((CCSP_SUCCESS == Cdm_GetNames(path, 0, &dmnames, &nname)) && \
            (nname > 0))
    {
        *pList = AnscAllocateMemory(sizeof(Name_DM_t) * nname);
        if(NULL != *pList){
            for(i = 0; i < nname; i++){
                dmlen = strlen(dmnames[i]) -1;
                snprintf((*pList)[i].dm ,NAME_DM_LEN -1, "%s%s", dmnames[i], name);
                (*pList)[i].dm[NAME_DM_LEN-1] = '\0';
                if(CCSP_SUCCESS == Cdm_GetParamString((*pList)[i].dm, (*pList)[i].name, NAME_DM_LEN)){
                    (*pList)[i].dm[dmlen] = '\0';
                }
                else
                    (*pList)[i].dm[0] = '\0';
            }
        }
        Cdm_FreeNames(dmnames); 
    }
    *num = nname;
}

void _get_dmbyname(int num, Name_DM_t *list, char** dm, char* name)
{
    int i;

    for(i = 0; i < num; i++){
        if(NULL != strcasestr(list[i].name, name)){
            STRNCPY_NULL_CHK((*dm), list[i].dm);
            break;
        }
    }
}

inline void _get_host_info(LM_host_t *pDestHost, PLmObjectHost pHost)
{
        pHost->bBoolParaValue[LM_HOST_ActiveId]= pDestHost->online;
        
        STRNCPY_NULL_CHK(pHost->pStringParaValue[LM_HOST_HostNameId], pDestHost->hostName);
        strstr(pDestHost->l1IfName, "Ethernet.");
        if(NULL != strstr(pDestHost->l1IfName, "Ethernet."))
        {
           int port;
           sscanf(pDestHost->l1IfName,"Ethernet.%d", &port);
           char tmpstr[100];
           if(port != 0){
                snprintf(tmpstr, sizeof(tmpstr), "Device.Ethernet.Interface.%d", port);
                STRNCPY_NULL_CHK(pHost->pStringParaValue[LM_HOST_Layer1InterfaceId], tmpstr);
                if(pHost->pStringParaValue[LM_HOST_AssociatedDeviceId] != NULL){
                    AnscFreeMemory(pHost->pStringParaValue[LM_HOST_AssociatedDeviceId]);
                    pHost->pStringParaValue[LM_HOST_AssociatedDeviceId] = NULL;
                }
           }
        }else if(strstr(pDestHost->l1IfName, "MoCA") != NULL){
            _get_dmbyname(g_MoCAADListNum, g_pMoCAADList, &(pHost->pStringParaValue[LM_HOST_AssociatedDeviceId]), pHost->pStringParaValue[LM_HOST_PhysAddressId]);
            STRNCPY_NULL_CHK(pHost->pStringParaValue[LM_HOST_Layer1InterfaceId], pDestHost->l1IfName);
        }else if(strstr(pDestHost->l1IfName, "WiFi") != NULL){
            STRNCPY_NULL_CHK(pHost->pStringParaValue[LM_HOST_Layer1InterfaceId], pDestHost->l1IfName);
            STRNCPY_NULL_CHK(pHost->pStringParaValue[LM_HOST_AssociatedDeviceId], pDestHost->AssociatedDevice)
        }else{
            STRNCPY_NULL_CHK(pHost->pStringParaValue[LM_HOST_Layer1InterfaceId], pDestHost->l1IfName);
            STRNCPY_NULL_CHK(pHost->pStringParaValue[LM_HOST_AssociatedDeviceId], NULL);
        }

        _get_dmbyname(g_IPIfNameDMListNum, g_pIPIfNameDMList, &(pHost->pStringParaValue[LM_HOST_Layer3InterfaceId]), pDestHost->l3IfName);
        STRNCPY_NULL_CHK(pHost->pStringParaValue[LM_HOST_Comments], pDestHost->comments);
        pHost->iIntParaValue[LM_HOST_X_CISCO_COM_RSSIId] = pDestHost->RSSI;
        pHost->activityChangeTime = pDestHost->activityChangeTime;
        _get_host_ipaddress(pDestHost, pHost);
        
}
int _get_PSM()
{
#if defined (CONFIG_TI_BBU) || defined (CONFIG_TI_BBU_TI)
    int  powerState = 0;
    Bool psmNvramFlag = FALSE;
    Bbu_GetUpsSecondsOnBattery(&powerState);
    psmNvramFlag = PACM_MANAGER_GET_PARAM_VALUE(PACM_MANAGER_PSM_ENABLE);

    AnscTrace("The UPS seconds on battery return:%d\n", powerState);
    AnscTrace("The PACM manager psm enable value is:%d\n", psmNvramFlag);
    if ( powerState &&  psmNvramFlag ) {
        return 1; /* Enabled */
    }
    else{
        return 0; /* Disabled */
    }
#endif
    return 0;
}

ANSC_STATUS
CosaDmlHostsInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    lmHosts.availableInstanceNum = 1;
    lmHosts.hostArray = NULL;
    lmHosts.numHost = 0;
    lmHosts.sizeHost = 0;
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     self

    prototype:

        PCOSA_DML_HOST_ENTRY
        CosaDmlHostsGetHosts
            (
                ANSC_HANDLE                 hContext,
                PULONG                      pulCount
            );

    description:

        This routine is to retrieve the complete list of current hosts, which is a dynamic table.

    argument:   ANSC_HANDLE                 hContext,
                PULONG                      pulCount
                To receive the actual number of entries.

    return:     The pointer to the array of hosts, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/
/* This is a workaround, don't get host information when PandM bootup, dm api will crash because this time dm api has not been initialized yet */
static int firstFlg = 0;
ANSC_STATUS
CosaDmlHostsGetHosts
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount
    )
{
    LM_hosts_t hosts;
    LM_host_t *plmHost;
    PLmObjectHost pHost;
    char str[100];
    int i;
    int ret;
    BOOL                                      bridgeMode;
     
    Hosts_RmHosts();
    if(firstFlg == 0){
        firstFlg = 1;
        *pulCount = 0;
        return ANSC_STATUS_SUCCESS;
    }

    /* Lan Hosts should not show in bridge mode */  
    if(((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&bridgeMode)) &&
        (TRUE == bridgeMode)) ||
        (_get_PSM()))
    {
        //Hosts_RmHosts();
        *pulCount = 0;
        return ANSC_STATUS_SUCCESS;
    }
    /* we don't want some device show in lan host list, get those unwelcome device list */
    _get_unwelcome_list();

    if(LM_RET_SUCCESS == lm_get_all_hosts(&hosts))
    {
        *pulCount = hosts.count;
        _init_DM_List(&g_IPIfNameDMListNum, &g_pIPIfNameDMList, "Device.IP.Interface.", "Name");
        _init_DM_List(&g_MoCAADListNum, &g_pMoCAADList, "Device.MoCA.Interface.1.AssociatedDevice.", "MACAddress");
        _init_DM_List(&g_DHCPv4ListNum, &g_pDHCPv4List, "Device.DHCPv4.Server.Pool.1.Client.", "Chaddr");
        for(i = 0; i < hosts.count; i++){
            plmHost = &(hosts.hosts[i]);
            /* filter unwelcome device */
            if(host_filter(plmHost))
                continue;
            sprintf(str,"%02x:%02x:%02x:%02x:%02x:%02x", plmHost->phyAddr[0], plmHost->phyAddr[1], plmHost->phyAddr[2], plmHost->phyAddr[3], plmHost->phyAddr[4], plmHost->phyAddr[5]); 
            pHost = Hosts_AddHostByPhysAddress(str);
            if(pHost == NULL)
                continue;
            _get_host_info(plmHost, pHost);
        }
    }else{
        *pulCount = 0;
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlHostsSetHostComment
    (
        ANSC_HANDLE                 hContext,
        char*                       pMac,
        char*                       pComment
    )
{
    int ret;
    char mac[6];

    ret = _mac_string_to_array(pMac, mac);
    if(ret == 0){
        if(LM_RET_SUCCESS != lm_set_host_comments(mac,pComment))
            return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}

ULONG CosaDmlHostsGetOnline()
{
    int num;
    ULONG rVal = 0;
    int ret;
    BOOL                                      bridgeMode;

    if(((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&bridgeMode)) &&
        (TRUE == bridgeMode)) ||
        (_get_PSM()))
    {
        return 0;
    }
   
    if(LM_RET_SUCCESS == lm_get_online_device(&num)){
        rVal = num;
    }
    return rVal;
}
#endif
