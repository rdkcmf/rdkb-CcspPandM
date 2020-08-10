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
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "cosa_dhcpv4_webconfig_apis.h"
#include "webconfig_framework.h"
#include "plugin_main_apis.h"
#define MACADDR_SZ          18
#define MIN 60
#define HOURS 3600
#define DAYS 86400
#define WEEKS 604800
#define MINSECS 120
#define MAXSECS 999

pthread_mutex_t staticClientsMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lanMutex = PTHREAD_MUTEX_INITIALIZER;

Dhcpv4_Cache_t g_dhcpv4bkup_cache[DHCPV4_CACHE_MAX_NUM_OF_PARAM];
Dhcpv4_Cache_t cache_temp[DHCPV4_CACHE_MAX_NUM_OF_PARAM] = {0};
int g_numOfbkupCacheParam = 0;
int g_numOfReceivedParam = 0;

int Dhcpv4_StaticClients_MutexLock()
{
    return pthread_mutex_lock(&staticClientsMutex);
}

int Dhcpv4_StaticClients_MutexUnLock()
{
    return pthread_mutex_unlock(&staticClientsMutex);
}

int Dhcpv4_StaticClients_MutexTryLock()
{
    return pthread_mutex_trylock(&staticClientsMutex);
}

int Dhcpv4_Lan_MutexLock()
{
    return pthread_mutex_lock(&lanMutex);
}

int Dhcpv4_Lan_MutexUnLock()
{
    return pthread_mutex_unlock(&lanMutex);
}

int Dhcpv4_Lan_MutexTryLock()
{
    return pthread_mutex_trylock(&lanMutex);
}
/*if the address is not in the following range, we think it is a public address
 10.0.0.0    ~ 10.255.255.255
 172.16.0.0  ~ 172.31.255.255
 192.168.0.0 ~ 192.168.255.255.
 */
BOOL Dhcpv4_Lan_Ip_ispublic_addr(ULONG addr)
{
    if((addr >= 0x0A000000)&&(addr <= 0x0AFFFFFF)){
        return FALSE;
    }else if((addr >= 0xAC100000)&&(addr <= 0xAC1FFFFF)){
        return FALSE;
    }else if((addr >= 0xC0A80000)&&(addr <= 0xC0A8FFFF)){
        return FALSE;
    }else
        return TRUE;
}
int CheckIpIsValid( char *ipAddress )
{
    struct sockaddr_in sa;

    if (!ipAddress)
        return INVALID_IP;

    CcspTraceWarning(("%s:IpAddressReceivedIs:%s\n",__FUNCTION__,ipAddress));

    if ( (inet_pton(AF_INET, ipAddress, &(sa.sin_addr)) == 1 ) )
    {
        return VALID_IP;
    }

    return INVALID_IP;
}


int CheckStaticClientIpIsValid( char *ipAddress )
{

    CcspTraceWarning((" %s Enter \n",__FUNCTION__));
    if (VALID_IP == CheckIpIsValid(ipAddress))
    {
        ULONG static_ip = _ansc_inet_addr(ipAddress);
        char buf[24] = {0};
        ULONG dhcp_start = 0;
        ULONG dhcp_end = 0;
        if (0 == syscfg_get(NULL,"dhcp_start",buf,sizeof(buf)))
        {
            dhcp_start = _ansc_inet_addr(buf);
        }
        if (0 == syscfg_get(NULL,"dhcp_end",buf,sizeof(buf)))
        {
            dhcp_end = _ansc_inet_addr(buf);
        }
        if (!dhcp_end || !dhcp_start)
            return VALID_IP;

        if((ntohl(static_ip) < ntohl(dhcp_start)) || (ntohl(static_ip) > ntohl(dhcp_end)))
        {
            return STATIC_IP_NOT_IN_LANRANGE;
        }
        return VALID_IP;        
    }
    return INVALID_IP;
}

BOOL CheckMacHasValidCharacter( char *pMac)
{
    int index = 0;
    BOOL bvalid = FALSE;  
    if (!pMac)
        return FALSE;

     if (pMac[0]) {
        if(pMac[2] == ':')
            if(pMac[5] == ':')
                if(pMac[8] == ':')
                    if(pMac[11] == ':')
                        if(pMac[14] == ':')
                            bvalid = TRUE;
    }
   
    if (TRUE == bvalid)
    {
        for (index = 0; index <  MACADDR_SZ-1; ++index)
        {
            if ((pMac[index] >= 48 && pMac[index] <= 57) ||
                    (pMac[index] >= 65 && pMac[index] <= 70) ||
                    (pMac[index] >= 97 && pMac[index] <= 102) ||
                    (2 == index || 5 == index || 8 == index || 11 == index || 14 == index))
            {
                continue;
            }
            else
            {
                return FALSE;
            }

        }
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}

BOOL Validate_Mac(char * physAddress)
{
    BOOL bvalid = FALSE;
    if(!physAddress || \
            0 == strcmp(physAddress, "00:00:00:00:00:00"))
    {
        return FALSE;
    }

    if(strlen(physAddress) != MACADDR_SZ-1)
    {
         return FALSE;
    }

    bvalid = CheckMacHasValidCharacter(physAddress);
    
    return bvalid;
}

int Dhcpv4_Reset_Cache()
{
    memset(g_dhcpv4bkup_cache,0,sizeof(g_dhcpv4bkup_cache));
    memset(cache_temp,0,sizeof(cache_temp));
    g_numOfbkupCacheParam = 0;
    g_numOfReceivedParam = 0;
    CcspTraceWarning((" %s done \n",__FUNCTION__));
    return 0;
}

char* ConvertErrCodeToErrMsg(int errCode)
{
    if (errCode == 0)
        return "Success";

    switch(errCode)
    {
	case INVALID_IP:
        {
	    return "Invalid IP";
        }
        break;
        case INVALID_MAC:
        {
            return "Invalid MAC";
        }
        break;	
        case STATIC_IP_NOT_IN_LANRANGE:
        {
            return "Static IP not in Lan Min/Max Range";
        }
        break;
        case LAN_PARAM_IP_RANGE_INVALID:
        {
            return "Lan Min/Max IP Invalid";
        }
        break;
        case LAN_PARAM_NETMASK_INVALID:
        {
            return "Lan NetMask Invalid";
        }
        break;
        case LAN_PARAM_LEASETIME_INVALID:
        {
            return "Lan LeaseTime Invalid";
        }
        break;
        case COSA_SYNCHRONIZE_FAILED:
        {
            return "Cosa Object Sync Failed";
        }
        break;
	case LAN_PARAM_GATEWAY_IP_INVALID:
	{
            return "Lan Gateway IP Invalid";
        }
        break;
        default:
        break;
    }
    return "Invalid Blob Input Parameters";
}

int Dhcpv4_UnsetAllDbValues(Dhcpv4_Cache_t *pCache, int numOfCacheParam)
{
    int i = 0;

    if (!pCache || numOfCacheParam <= 0)
        return -1;

    for(i = 0; i < numOfCacheParam; i++)
    {
        if (syscfg_unset(NULL, pCache[i].cmd) != 0)
        {
            CcspTraceError(("syscfg_set failed for %s %s\n",pCache[i].cmd,pCache[i].val));
            return SYSCFG_FAILURE;       
        }
    }
    
    if (syscfg_commit() != 0)
    {
        CcspTraceError(("syscfg_commit failed unset_ToDB\n"));
        return SYSCFG_FAILURE;
    }
    return 0;
}

int Dhcpv4_Cache_ApplyAllToDb(Dhcpv4_Cache_t *pCache, int numOfCacheParam)
{
    int i = 0;
    if (!pCache || numOfCacheParam <= 0)
        return -1;
     
    CcspTraceWarning((" %s line.no %d numOfParam %d\n",__FUNCTION__,__LINE__,numOfCacheParam));

    for(i = 0;i < numOfCacheParam ; i++)
    {
        if (syscfg_set(NULL, pCache[i].cmd, pCache[i].val) != 0)
        {
            CcspTraceError(("syscfg_set failed for %s %s\n",pCache[i].cmd,pCache[i].val));
            return SYSCFG_FAILURE;                  
        }
    }
    if (syscfg_commit() != 0)
    {
        CcspTraceError(("syscfg_commit failed apply_cache_ToDB\n"));
        return SYSCFG_FAILURE;

    }
    CcspTraceWarning((" %s line.no %d Success \n",__FUNCTION__,__LINE__));
    return 0;
}

int Dhcpv4_StaticClients_UnsetNotUsedParamFromDb(int numOfNewlyReceivedParam, int numOfBackupParam)
{
    int index = 0;

    if (numOfNewlyReceivedParam <= numOfBackupParam)
    {
        return -1;
    }

    for (index = numOfBackupParam; index < numOfNewlyReceivedParam; ++index)
    {
        char cmd[64] = {0};
        char val[256] = {0};
        snprintf(cmd,sizeof(cmd),"%s%d",DHCPV4_STATIC_HOST,index + 1);
        if (syscfg_unset(NULL,cmd) != 0)
        {
            CcspTraceError(("syscfg_unset failed for %s \n",cmd));
            return SYSCFG_FAILURE;
        }
        snprintf(cmd,sizeof(cmd),"%s%d",DHCPV4_STATIC_HOST_ALIAS,index + 1);
        if (syscfg_unset(NULL,cmd) != 0)
        {
            CcspTraceError(("syscfg_unset failed for %s \n",cmd));
            return SYSCFG_FAILURE;
        }
        snprintf(cmd,sizeof(cmd),"%s%d",DHCPV4_STATIC_HOSTS_INSNUM,index + 1);
        if (syscfg_unset(NULL,cmd) != 0)
        {
            CcspTraceError(("syscfg_unset failed for %s \n",cmd));
            return SYSCFG_FAILURE;
        }
    }

    if (syscfg_commit() != 0)
    {
        CcspTraceError(("syscfg_commit failed apply_cache_ToDB\n"));
        return SYSCFG_FAILURE;

    }
    return 0;

}

int Dhcpv4_StaticClients_BackupFromDb(Dhcpv4_Cache_t *pCache, int *pNumOfParam)
{
    int numOfHost = 0;
    int numOfParam = 0;
    int index = 0;
    if (!pCache || !pNumOfParam)
        return -1;
   
 
    snprintf(pCache[numOfParam].cmd,sizeof(pCache[numOfParam].cmd),"%s",DHCPV4_NUM_STATIC_HOSTS);
    syscfg_get(NULL,pCache[numOfParam].cmd,pCache[numOfParam].val,sizeof(pCache[numOfParam].val));
    if (pCache[numOfParam].val[0] != 0)
    {
        numOfHost = atoi(pCache[numOfParam].val);
        ++numOfParam;
    }
    for (index = 0; index < numOfHost; ++index)
    {
        char hostInfo[256] = {0};
        char *pStr = NULL;

        snprintf(pCache[numOfParam].cmd,sizeof(pCache[numOfParam].cmd),"%s%d",DHCPV4_STATIC_HOST,index + 1);
        syscfg_get(NULL,pCache[numOfParam].cmd,pCache[numOfParam].val,sizeof(pCache[numOfParam].val));
        if (pCache[numOfParam].val[0] != 0)
        {
            snprintf(hostInfo, sizeof(hostInfo), "%s", pCache[numOfParam].val);
            pStr = strstr(hostInfo,",");
            if (pStr)
            {
                *pStr = '\0';
            }
            ++numOfParam;
        }

        snprintf(pCache[numOfParam].cmd,sizeof(pCache[numOfParam].cmd),"%s%d",DHCPV4_STATIC_HOSTS_INSNUM,index + 1);
        syscfg_get(NULL,pCache[numOfParam].cmd,pCache[numOfParam].val,sizeof(pCache[numOfParam].val));
        if (pCache[numOfParam].val[0] != 0)
        {
            ++numOfParam;
        }

        snprintf(pCache[numOfParam].cmd,sizeof(pCache[numOfParam].cmd),"%s%d",DHCPV4_STATIC_HOST_ALIAS,index + 1);
        syscfg_get(NULL,pCache[numOfParam].cmd,pCache[numOfParam].val,sizeof(pCache[numOfParam].val));
        if (pCache[numOfParam].val[0] != 0)
        {
            ++numOfParam;
        }

        snprintf(pCache[numOfParam].cmd,sizeof(pCache[numOfParam].cmd),"%s%d",DHCPV4_COMMENTS_LAN_CLIENTS,index + 1);
        syscfg_get(NULL,pCache[numOfParam].cmd,pCache[numOfParam].val,sizeof(pCache[numOfParam].val));
        if (pCache[numOfParam].val[0] != 0)
        {
            ++numOfParam;
        }
     
        snprintf(pCache[numOfParam].cmd,sizeof(pCache[numOfParam].cmd),"%s%s",DHCPV4_COMMENTS,hostInfo);
        syscfg_get(NULL,pCache[numOfParam].cmd,pCache[numOfParam].val,sizeof(pCache[numOfParam].val));
        if (pCache[numOfParam].val[0] != 0)
        {
            ++numOfParam;
        }
    }
    snprintf(pCache[numOfParam].cmd,sizeof(pCache[numOfParam].cmd),"%s",DHCPV4_COMMENTS_LAN_CLIENTS_COUNT);
    syscfg_get(NULL,pCache[numOfParam].cmd,pCache[numOfParam].val,sizeof(pCache[numOfParam].val));
    if (pCache[numOfParam].val[0] != 0)
    {
        ++numOfParam;
    }
    *pNumOfParam = numOfParam;
    CcspTraceWarning((" %s Done NumofParam %d \n",__FUNCTION__,numOfParam));
    return 0;
}

int Dhcpv4_StaticClients_UpdateConfToCache(macbindingdoc_t *pConf,Dhcpv4_Cache_t *pCache,int *pNumOfParam)
{
    int numOfParam = 0;
    int numOfHost = 0;
    int index =0;
    if (!pConf)
        return -1;

    if (!pCache || !pNumOfParam)
        return -1;
    numOfHost = pConf->entries_count;
    snprintf(pCache[numOfParam].cmd,sizeof(pCache[numOfParam].cmd),"%s",DHCPV4_NUM_STATIC_HOSTS);
    snprintf(pCache[numOfParam].val,sizeof(pCache[numOfParam].val),"%d",numOfHost);
    ++numOfParam;
    for (index = 0; index < numOfHost; ++index)
    {
        snprintf(pCache[numOfParam].cmd,sizeof(pCache[numOfParam].cmd),"%s%d",DHCPV4_STATIC_HOST,index + 1);
        snprintf(pCache[numOfParam].val,sizeof(pCache[numOfParam].val),"%s,%s,",pConf->entries[index].chaddr,pConf->entries[index].yiaddr);
        ++numOfParam;

        snprintf(pCache[numOfParam].cmd,sizeof(pCache[numOfParam].cmd),"%s%d",DHCPV4_STATIC_HOSTS_INSNUM,index + 1);
        snprintf(pCache[numOfParam].val,sizeof(pCache[numOfParam].val),"%d",index + 1);
        ++numOfParam;

        snprintf(pCache[numOfParam].cmd,sizeof(pCache[numOfParam].cmd),"%s%d",DHCPV4_STATIC_HOST_ALIAS,index + 1);
        snprintf(pCache[numOfParam].val,sizeof(pCache[numOfParam].val),"StaticAddress%d",index + 1);
        ++numOfParam;

    }
    *pNumOfParam = numOfParam;

    return 0;
}

int Dhcpv4_StaticClients_Validate(macbindingdoc_t *pConf)
{
    int index = 0;
    if (!pConf)
        return -1;

    CcspTraceWarning((" %s Enter \n",__FUNCTION__));
    if (pConf->entries_count <= 0 || !pConf->entries)
        return EMPTY_BLOB;


    for (index = 0; index < pConf->entries_count; ++index)
    {
        int ret_val = 0;
        ret_val = CheckStaticClientIpIsValid(pConf->entries[index].yiaddr);
        if (ret_val != VALID_IP)
        {
            return ret_val;
        }

        if (FALSE == Validate_Mac(pConf->entries[index].chaddr))
        {
            return INVALID_MAC;
        }        
    }
    CcspTraceWarning((" %s Done \n",__FUNCTION__));

    return 0;    
}

int Dhcpv4_StaticClients_SetSyncFlag()
{
    PCOSA_DATAMODEL_DHCPV4 pDhcpv4 = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;
    if (!pDhcpv4)
        return -1;
    CcspTraceWarning((" %s line.no %d Entered \n",__FUNCTION__,__LINE__));
    pDhcpv4->syncStaticClientsTable = TRUE;
    return 0;
}

int Dhcpv4_StaticClients_Synchronize()
{
    PCOSA_DATAMODEL_DHCPV4 pDhcpv4 = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;
    PCOSA_CONTEXT_POOL_LINK_OBJECT pCxtPoolLink = NULL;
    PCOSA_CONTEXT_LINK_OBJECT  pCxtLink = NULL;
    PCOSA_CONTEXT_LINK_OBJECT  pCxtLink2 = NULL;
    PSINGLE_LINK_ENTRY  pSListEntry       = NULL;
    PSINGLE_LINK_ENTRY  pSListEntry2      = NULL;
    int index = 0;
    ULONG count = 0;
    ULONG ulIndex2 = 0;
    int numOfHost = 0;

    if (!pDhcpv4)
        return -1;

    pCxtPoolLink = (PCOSA_CONTEXT_POOL_LINK_OBJECT) CosaSListGetEntryByInsNum(&pDhcpv4->PoolList, 1); // instance 1 is Lan Pool.
    if (!pCxtPoolLink)
        return -1;
    numOfHost = AnscSListQueryDepth(&pCxtPoolLink->StaticAddressList);
    pSListEntry =   AnscSListGetFirstEntry(&pCxtPoolLink->StaticAddressList);      
    while( pSListEntry )
    {
        pCxtLink          = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSListEntry);
        pSListEntry2      = pSListEntry;
        pSListEntry       = AnscSListGetNextEntry(pSListEntry);

        if (pCxtLink && (AnscSListPopEntryByLink(&pCxtPoolLink->StaticAddressList, pSListEntry2)))
        {
            CosaDhcpv4RegSetDhcpv4Info(pDhcpv4);
            AnscFreeMemory(pCxtLink->hContext);
            AnscFreeMemory(pCxtLink);
        }
    }

    numOfHost = AnscSListQueryDepth(&pCxtPoolLink->StaticAddressList);
    /* We begin treat DHCPv4.Server.Pool.{i}.StaticAddress.{i} */
    count = CosaDmlDhcpsGetNumberOfSaddr
        (
         NULL,
         1
        );
    pCxtPoolLink->maxInstanceOfStaticAddress = 0;
    for (ulIndex2 = 0; ulIndex2 < count; ++ulIndex2)
    {
	ANSC_STATUS returnStatus = ANSC_STATUS_SUCCESS;
	PCOSA_DML_DHCPS_SADDR pStaticAddr  = (PCOSA_DML_DHCPS_SADDR)AnscAllocateMemory( sizeof(COSA_DML_DHCPS_SADDR) );
        if ( !pStaticAddr )
        {
            break;
        }

        DHCPV4_STATICADDRESS_SET_DEFAULTVALUE(pStaticAddr);
        returnStatus = CosaDmlDhcpsGetSaddr
            (
             NULL, 
             1, 
             ulIndex2, 
             pStaticAddr
            );
        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            AnscFreeMemory(pStaticAddr);
            break;
        }

        pCxtLink = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory( sizeof(COSA_CONTEXT_LINK_OBJECT) );

        if ( !pCxtLink )
        {
            AnscFreeMemory(pStaticAddr);
            break;
        }

        COSA_CONTEXT_LINK_INITIATION_CONTENT(pCxtLink);
        pCxtLink->hContext       = (ANSC_HANDLE)pStaticAddr;
        pCxtLink->hParentTable   = (ANSC_HANDLE)pCxtPoolLink;
        pCxtLink->bNew           = TRUE;

        if ( !pStaticAddr->InstanceNumber )
        {
            if ( !++pCxtPoolLink->maxInstanceOfStaticAddress )
            {
                pCxtPoolLink->maxInstanceOfStaticAddress = 1;
            }
            pStaticAddr->InstanceNumber = pCxtPoolLink->maxInstanceOfStaticAddress;

            _ansc_sprintf( pStaticAddr->Alias, "StaticAddress%u", pStaticAddr->InstanceNumber );

            returnStatus = CosaDmlDhcpsSetSaddrValues
                (
                 NULL,
                 1,
                 ulIndex2,
                 pStaticAddr->InstanceNumber,
                 pStaticAddr->Alias
                );

            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                AnscFreeMemory(pStaticAddr);
                AnscFreeMemory(pCxtLink);
                break;
            }

            pCxtLink->InstanceNumber = pStaticAddr->InstanceNumber;

            /* Put into our list */
            CosaSListPushEntryByInsNum(&pCxtPoolLink->StaticAddressList, (PCOSA_CONTEXT_LINK_OBJECT)pCxtLink);
        }
        else
        {
            pCxtLink->InstanceNumber = pStaticAddr->InstanceNumber;

            /* This case never happen. Add it just for simulation code run well */
            if ( pCxtPoolLink->maxInstanceOfStaticAddress < pStaticAddr->InstanceNumber )
            {
                pCxtPoolLink->maxInstanceOfStaticAddress = pStaticAddr->InstanceNumber;
            }

            /* if this entry is in link tree already because it's  delay_added table */
            pCxtLink2 = (PCOSA_CONTEXT_LINK_OBJECT)CosaSListGetEntryByInsNum(&pCxtPoolLink->StaticAddressList, pStaticAddr->InstanceNumber);
            if ( !pCxtLink2 )
            {
                CosaSListPushEntryByInsNum(&pCxtPoolLink->StaticAddressList, (PCOSA_CONTEXT_LINK_OBJECT)pCxtLink);
            }
            else
            {
                AnscFreeMemory( pCxtLink2->hContext );
                pCxtLink2->hContext       = (ANSC_HANDLE)pStaticAddr;
                if ( pCxtLink2->bNew )
                {
                    pCxtLink2->bNew       = FALSE;
                }

                AnscFreeMemory(pCxtLink);
                pCxtLink                  = pCxtLink2;
                pCxtLink2                 = NULL;
            }

        }
    }

    CosaDhcpv4RegSetDhcpv4Info(pDhcpv4);
    numOfHost = AnscSListQueryDepth(&pCxtPoolLink->StaticAddressList);
    CcspTraceWarning((" %s line.no %d Success num of host %d\n",__FUNCTION__,__LINE__,numOfHost));
    return 0;
}

void Print_StaticClients_BlobInfo(macbindingdoc_t *pConf)
{
    int index = 0;

    if (!pConf)
        return;
    CcspTraceWarning((" %s enter\n",__FUNCTION__));
    for (index = 0; index < pConf->entries_count; ++index)
    {

        CcspTraceWarning((" ip %s\n",pConf->entries[index].yiaddr));
        CcspTraceWarning((" Mac %s\n",pConf->entries[index].chaddr));
    }

}

void print_cache(Dhcpv4_Cache_t *pCache, int numOfCacheParam)
{
    int i = 0;
        for(i = 0;i < numOfCacheParam ; i++)
        {
            CcspTraceWarning(("Cache[%d].cmd - %s \n",i,pCache[i].cmd));
            CcspTraceWarning(("Cache[%d].val - %s \n",i,pCache[i].val));
        }

}

/* CallBack API to execute StaticClients Blob request */
pErr Process_StaticClients_WebConfigRequest(void *Data)
{
    pErr pErrRetVal = NULL;
    int ret = 0;
    char buf[16] = {0};
    macbindingdoc_t *pMacConf = NULL;
    int cacheSize = 0;

    pErrRetVal = (pErr) malloc (sizeof(Err));
    if (pErrRetVal == NULL )
    {
        CcspTraceError(("%s : malloc failed\n",__FUNCTION__));
        return pErrRetVal;
    }

    memset(pErrRetVal,0,sizeof(Err));
    pErrRetVal->ErrorCode = BLOB_EXEC_SUCCESS;

    ret = syscfg_get(NULL,"bridge_mode",buf,sizeof(buf));
 
    if (0 == ret && (strcmp(buf,"0")))
    {
        CcspTraceWarning((" %s line.no %d bridge mode enabled \n",__FUNCTION__,__LINE__));
        pErrRetVal->ErrorCode = VALIDATION_FALIED;
        strncpy(pErrRetVal->ErrorMsg,"MacBinding Blob Not supported in bridge mode",sizeof(pErrRetVal->ErrorMsg)-1);
        return pErrRetVal;
    }
    pMacConf = (macbindingdoc_t*)Data;
    Dhcpv4_Reset_Cache();
    ret = Dhcpv4_StaticClients_Validate(pMacConf);
    if (EMPTY_BLOB == ret)
    {
        Dhcpv4_StaticClients_MutexLock();
        Dhcpv4_StaticClients_BackupFromDb(g_dhcpv4bkup_cache,&g_numOfbkupCacheParam);
        ret = Dhcpv4_UnsetAllDbValues(g_dhcpv4bkup_cache,g_numOfbkupCacheParam);
        if (SYSCFG_FAILURE == ret)
        {
            CcspTraceWarning((" %s line.no %d syscfg failure \n",__FUNCTION__,__LINE__));
            pErrRetVal->ErrorCode = SYSCFG_FAILURE;
            strncpy(pErrRetVal->ErrorMsg,"sysconfig failure while clearing entries",sizeof(pErrRetVal->ErrorMsg)-1);
        }
        else 
        {
            ret = Dhcpv4_StaticClients_SetSyncFlag();
            if (0 != ret)
            {
                CcspTraceWarning((" %s Sync Failed \n",__FUNCTION__));
                pErrRetVal->ErrorCode = COSA_SYNCHRONIZE_FAILED;
                strncpy(pErrRetVal->ErrorMsg,"Sync with Cosa Object failure",sizeof(pErrRetVal->ErrorMsg)-1);
            }
            system("sysevent set dhcp_server-restart");
        }
        Dhcpv4_StaticClients_MutexUnLock();
        return pErrRetVal;
    }
    else if (0 != ret)
    {
	char *errMsg = ConvertErrCodeToErrMsg(ret);
        CcspTraceWarning((" %s line.no %d validation failed reason %s \n",__FUNCTION__,__LINE__,errMsg));
        pErrRetVal->ErrorCode = VALIDATION_FALIED;
        strncpy(pErrRetVal->ErrorMsg,errMsg,sizeof(pErrRetVal->ErrorMsg)-1);
        return pErrRetVal;
    }
    Dhcpv4_StaticClients_MutexLock();
    Dhcpv4_StaticClients_BackupFromDb(g_dhcpv4bkup_cache,&g_numOfbkupCacheParam);
    ret = Dhcpv4_UnsetAllDbValues(g_dhcpv4bkup_cache,g_numOfbkupCacheParam);
    if (SYSCFG_FAILURE == ret)
    {
        CcspTraceWarning((" %s line.no %d syscfg failed \n",__FUNCTION__,__LINE__));
        pErrRetVal->ErrorCode = SYSCFG_FAILURE;
        strncpy(pErrRetVal->ErrorMsg,"sysconfig failure while clearing entries",sizeof(pErrRetVal->ErrorMsg)-1);
        Dhcpv4_StaticClients_MutexUnLock();
        return pErrRetVal;
    }

    ret = Dhcpv4_StaticClients_UpdateConfToCache(pMacConf,cache_temp,&cacheSize);
    if (ret == 0)
    {
        g_numOfReceivedParam = cacheSize;
        ret = Dhcpv4_Cache_ApplyAllToDb(cache_temp,cacheSize);
        if (SYSCFG_FAILURE == ret)
        {
            CcspTraceWarning((" %s line.no %d syscfg failed \n",__FUNCTION__,__LINE__));
            pErrRetVal->ErrorCode = SYSCFG_FAILURE;
            strncpy(pErrRetVal->ErrorMsg,"sysconfig failure while applying into db",sizeof(pErrRetVal->ErrorMsg)-1);
        }
        else
        {
            ret = Dhcpv4_StaticClients_SetSyncFlag();
            if (0 != ret)
            {
                CcspTraceWarning((" %sSync Failed \n",__FUNCTION__));
                pErrRetVal->ErrorCode = COSA_SYNCHRONIZE_FAILED;
                strncpy(pErrRetVal->ErrorMsg,"Sync with Cosa object failure",sizeof(pErrRetVal->ErrorMsg)-1);
            }
            system("sysevent set dhcp_server-restart");
        }
    }
    Dhcpv4_StaticClients_MutexUnLock();
    return pErrRetVal;
}

/* Callback function to rollback when StaticClients blob execution fails */
int rollback_StaticClients()
{
    // return 0 to notify framework when rollback is success

    int ret = 0;

    CcspTraceInfo((" Entering %s \n",__FUNCTION__));
    Dhcpv4_StaticClients_MutexLock();
    
    ret = Dhcpv4_Cache_ApplyAllToDb(g_dhcpv4bkup_cache,g_numOfbkupCacheParam);

    Dhcpv4_StaticClients_UnsetNotUsedParamFromDb(g_numOfReceivedParam,g_numOfbkupCacheParam);
    
    ret = Dhcpv4_StaticClients_SetSyncFlag();
    
    Dhcpv4_StaticClients_MutexUnLock();
     
    system("sysevent set dhcp_server-restart");
    CcspTraceWarning((" Success %s \n",__FUNCTION__));

    return ret ;
}

void FreeResources_StaticClients(void *arg)
{
    
    CcspTraceWarning((" Entering %s \n",__FUNCTION__));
    execData *blob_exec_data  = (execData*) arg;
    macbindingdoc_t *pStaticClients = (macbindingdoc_t *) blob_exec_data->user_data ;

    if (pStaticClients != NULL)
    {
        macbindingdoc_destroy(pStaticClients);
        pStaticClients = NULL;
    }

    if (blob_exec_data != NULL)
    {
        free(blob_exec_data);
        blob_exec_data = NULL;
    }
}

int Dhcpv4_Lan_BackupFromDb(Dhcpv4_Cache_t *pCache,int *pNumOfParam)
{
    int numOfParam = 0;
    int index = 0;
    if (!pCache || !pNumOfParam)
        return -1;

    CcspTraceWarning((" Entering %s \n",__FUNCTION__));
    snprintf(pCache[numOfParam].cmd,sizeof(pCache[numOfParam].cmd),"%s",DHCPV4_LAN_IP);
    syscfg_get(NULL,pCache[numOfParam].cmd,pCache[numOfParam].val,sizeof(pCache[numOfParam].val));
    if (pCache[numOfParam].val[0] != 0)
    {
        ++numOfParam;
    }

    snprintf(pCache[numOfParam].cmd,sizeof(pCache[numOfParam].cmd),"%s",DHCPV4_LAN_NETMASK);
    syscfg_get(NULL,pCache[numOfParam].cmd,pCache[numOfParam].val,sizeof(pCache[numOfParam].val));
    if (pCache[numOfParam].val[0] != 0)
    {
        ++numOfParam;
    }   

    snprintf(pCache[numOfParam].cmd,sizeof(pCache[numOfParam].cmd),"%s",DHCPV4_SERVER_ENABLED);
    syscfg_get(NULL,pCache[numOfParam].cmd,pCache[numOfParam].val,sizeof(pCache[numOfParam].val));
    if (pCache[numOfParam].val[0] != 0)
    {
        ++numOfParam;
    }

    snprintf(pCache[numOfParam].cmd,sizeof(pCache[numOfParam].cmd),"%s",DHCPV4_SERVER_START_IP);
    syscfg_get(NULL,pCache[numOfParam].cmd,pCache[numOfParam].val,sizeof(pCache[numOfParam].val));
    if (pCache[numOfParam].val[0] != 0)
    {
        ++numOfParam;
    }

    snprintf(pCache[numOfParam].cmd,sizeof(pCache[numOfParam].cmd),"%s",DHCPV4_SERVER_END_IP);
    syscfg_get(NULL,pCache[numOfParam].cmd,pCache[numOfParam].val,sizeof(pCache[numOfParam].val));
    if (pCache[numOfParam].val[0] != 0)
    {
        ++numOfParam;
    }

    snprintf(pCache[numOfParam].cmd,sizeof(pCache[numOfParam].cmd),"%s",DHCPV4_LEASE_TIME);
    syscfg_get(NULL,pCache[numOfParam].cmd,pCache[numOfParam].val,sizeof(pCache[numOfParam].val));
    if (pCache[numOfParam].val[0] != 0)
    {
        ++numOfParam;
    }

    *pNumOfParam = numOfParam;
     return 0;
}


int Dhcpv4_Lan_UpdateConfToCache(landoc_t *pConf,Dhcpv4_Cache_t *pCache,int *pNumOfParam)
{
    int numOfParam = 0;
    int index = 0;
    if (!pCache || !pNumOfParam || !pConf || !pConf->param)
        return -1;

    snprintf(pCache[numOfParam].cmd,sizeof(pCache[numOfParam].cmd),"%s",DHCPV4_LAN_IP);
    snprintf(pCache[numOfParam].val,sizeof(pCache[numOfParam].val),"%s",pConf->param->lan_ip_address);
    ++numOfParam;

    snprintf(pCache[numOfParam].cmd,sizeof(pCache[numOfParam].cmd),"%s",DHCPV4_LAN_NETMASK);
    snprintf(pCache[numOfParam].val,sizeof(pCache[numOfParam].val),"%s",pConf->param->lan_subnet_mask);
    ++numOfParam;

    snprintf(pCache[numOfParam].cmd,sizeof(pCache[numOfParam].cmd),"%s",DHCPV4_SERVER_ENABLED);
    snprintf(pCache[numOfParam].val,sizeof(pCache[numOfParam].val),"%d",pConf->param->dhcp_server_enable);
    ++numOfParam;

    snprintf(pCache[numOfParam].cmd,sizeof(pCache[numOfParam].cmd),"%s",DHCPV4_SERVER_START_IP);
    snprintf(pCache[numOfParam].val,sizeof(pCache[numOfParam].val),"%s",pConf->param->dhcp_start_ip_address);
    ++numOfParam;

    snprintf(pCache[numOfParam].cmd,sizeof(pCache[numOfParam].cmd),"%s",DHCPV4_SERVER_END_IP);
    snprintf(pCache[numOfParam].val,sizeof(pCache[numOfParam].val),"%s",pConf->param->dhcp_end_ip_address);
    ++numOfParam;

    snprintf(pCache[numOfParam].cmd,sizeof(pCache[numOfParam].cmd),"%s",DHCPV4_LEASE_TIME);
    snprintf(pCache[numOfParam].val,sizeof(pCache[numOfParam].val),"%u",pConf->param->leasetime);
    ++numOfParam;

    *pNumOfParam = numOfParam;

    return 0;
}
   
BOOL Dhcpv4_Lan_CheckIsValidLeaseTime(uint32_t leasetime)
{
     /*  enter only valid values 
            UNITS:
            seconds=iValue;(min-120 max-999)
            minutes=iValue/60;
            hours=iValue/3600;
            days=iValue/86400;
            weeks=iValue/604800;
            forever=-1;
        */
        if((leasetime%WEEKS==0)  ||
            (leasetime%DAYS==0)  ||
            (leasetime%HOURS==0) ||
            (leasetime%MIN==0)   ||
            ((leasetime>=MINSECS)&& (leasetime<=MAXSECS)) ||
            (leasetime == (uint32_t)-1))
            {
                return TRUE;
            }

    return FALSE;
}

BOOL Dhcpv4_Lan_CheckIpIsValidUnicast(unsigned int gw, unsigned int mask, unsigned int ipaddr)
{
    unsigned int subnet, bcast;

    /*Check whether ipaddr is zero, or a multicast address , or a loopback address*/
    if((ipaddr==0)||(ipaddr>=0xE0000000)||((ipaddr & 0xFF000000)==0x7F000000))
        return FALSE;
    subnet = gw & mask;
    bcast = subnet | (~mask);
    /*Check if ip addr is subnet or subnet broadcast address, or it is the same as gw*/
    if((ipaddr<=subnet)||(ipaddr>=bcast)||(ipaddr==gw))
        return FALSE;
    return TRUE;
}

int Get_LanParameters_ValidityCheckStatus(lanparam_t *pLanParam)
{
    ULONG minaddr = 0;
    ULONG maxaddr = 0;
    ULONG netmask = 0;
    ULONG gateway = 0;

    if (!pLanParam
            || !pLanParam->dhcp_start_ip_address
            || !pLanParam->dhcp_end_ip_address
            || !pLanParam->lan_ip_address
            || !pLanParam->lan_subnet_mask
       )
    {
        return -1;
    }

    if (VALID_IP != CheckIpIsValid(pLanParam->lan_ip_address))
    {
        return LAN_PARAM_GATEWAY_IP_INVALID;
    }

    if (VALID_IP != CheckIpIsValid(pLanParam->dhcp_start_ip_address)
        || VALID_IP != CheckIpIsValid(pLanParam->dhcp_end_ip_address)
        )
    {
        return LAN_PARAM_IP_RANGE_INVALID;
    }

    minaddr = ntohl(_ansc_inet_addr(pLanParam->dhcp_start_ip_address));
    maxaddr = ntohl(_ansc_inet_addr(pLanParam->dhcp_end_ip_address));

    if(minaddr > maxaddr)
    {
        return LAN_PARAM_IP_RANGE_INVALID;
    }

    gateway = ntohl(_ansc_inet_addr(pLanParam->lan_ip_address));
    
    if (Dhcpv4_Lan_Ip_ispublic_addr(gateway))
    {
        return LAN_PARAM_GATEWAY_IP_INVALID;
    }

    if ((gateway & 0x000000FF) != 1) // last digit in lanip should equal to 1.
    {
        return LAN_PARAM_GATEWAY_IP_INVALID;
    }

    netmask = ntohl(_ansc_inet_addr(pLanParam->lan_subnet_mask));
    if (netmask == 0 || netmask == 0xFFFFFFFF)
        return LAN_PARAM_NETMASK_INVALID;
    if(!Dhcpv4_Lan_CheckIpIsValidUnicast(gateway,netmask,minaddr) || !Dhcpv4_Lan_CheckIpIsValidUnicast(gateway,netmask,maxaddr))
        return LAN_PARAM_IP_RANGE_INVALID;

    if (FALSE == Dhcpv4_Lan_CheckIsValidLeaseTime(pLanParam->leasetime))
        return LAN_PARAM_LEASETIME_INVALID;
  
    return 0;
}
 
int Dhcpv4_Lan_Validate(landoc_t *pConf)
{
    int index = 0;
    if (!pConf)
        return -1;

    if (pConf->entries_count <= 0 || !pConf->param)
        return EMPTY_BLOB;

    for (index = 0; index < pConf->entries_count; ++index)
    {
        int ret_val = 0;
        ret_val = Get_LanParameters_ValidityCheckStatus(pConf->param);
        if (0 != ret_val)
        {
            return ret_val;
        }
    }

    return 0;
}

int Dhcpv4_Lan_Synchronize()
{
    PCOSA_DATAMODEL_DHCPV4 pDhcpv4 = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;
    PCOSA_DATAMODEL_DEVICECONTROL   pDevCtrl    = (PCOSA_DATAMODEL_DEVICECONTROL)g_pCosaBEManager->hDeviceControl;
    PCOSA_CONTEXT_POOL_LINK_OBJECT pCxtPoolLink = NULL;
    PCOSA_DML_DHCPS_POOL_FULL pPool = NULL;
    PCOSA_CONTEXT_LINK_OBJECT pCxtLink = NULL;
    PCOSA_DML_LAN_MANAGEMENT  pLanMngm = NULL;
    ANSC_STATUS returnStatus = ANSC_STATUS_SUCCESS;
    if (!pDhcpv4 || !pDevCtrl)
        return -1;

    pCxtLink  = (PCOSA_CONTEXT_LINK_OBJECT) CosaSListGetEntryByInsNum(&pDevCtrl->LanMngmList, 1); // instance 1 is Lan.
    if (!pCxtLink)
        return -1;

    pLanMngm = (PCOSA_DML_LAN_MANAGEMENT) pCxtLink->hContext;
    if (!pLanMngm)
        return -1;
    if (CosaDmlLanMngm_GetEntryByIndex(0, pLanMngm) != ANSC_STATUS_SUCCESS)
    {
        CcspTraceError(("%s: CosaDmlLanMngm_GetEntryByIndex error\n", __FUNCTION__));
        return -1;
    }
    pCxtPoolLink = (PCOSA_CONTEXT_POOL_LINK_OBJECT) CosaSListGetEntryByInsNum(&pDhcpv4->PoolList, 1); // instance 1 is Lan Pool.
    if (!pCxtPoolLink)
        return -1;

    pPool = (PCOSA_DML_DHCPS_POOL_FULL) pCxtPoolLink->hContext;
    if (!pPool)
        return -1;
    returnStatus = CosaDmlDhcpsGetPool(NULL, 0, pPool);
    pPool->Cfg.X_CISCO_COM_TimeOffset = 0;
    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        return -1;
    }
    return 0;
}

void Print_Lan_BlobInfo(landoc_t *pConf)
{

    if (!pConf)
        return;
    CcspTraceWarning((" %s enter\n",__FUNCTION__));
    {

        CcspTraceWarning((" Lan ip %s\n",pConf->param->lan_ip_address));
        CcspTraceWarning((" NetMask %s\n",pConf->param->lan_subnet_mask));
        CcspTraceWarning((" MinAddress %s Max Address %s\n",pConf->param->dhcp_start_ip_address,pConf->param->dhcp_end_ip_address));
        CcspTraceWarning((" Lease %u ServerEnabled %d \n",pConf->param->leasetime,pConf->param->dhcp_server_enable));
    }

}
/* CallBack API to execute Lan Blob request */
pErr Process_Lan_WebConfigRequest(void *Data)
{
    pErr pErrRetVal = NULL;
    int ret = 0;
    char buf[16] = {0};
    landoc_t *pLanConf = NULL;
    int cacheSize = 0;

    pErrRetVal = (pErr) malloc (sizeof(Err));
    if (pErrRetVal == NULL )
    {
        CcspTraceError(("%s : malloc failed\n",__FUNCTION__));
        return pErrRetVal;
    }

    memset(pErrRetVal,0,sizeof(Err));
    pErrRetVal->ErrorCode = BLOB_EXEC_SUCCESS; 
    ret = syscfg_get(NULL,"bridge_mode",buf,sizeof(buf));
  
    if (0 == ret && (strcmp(buf,"0")))
    {
        CcspTraceWarning((" %s line.no %d bridge mode enabled \n",__FUNCTION__,__LINE__));
        pErrRetVal->ErrorCode = VALIDATION_FALIED;
        strncpy(pErrRetVal->ErrorMsg,"Lan Blob Not supported in bridge mode",sizeof(pErrRetVal->ErrorMsg)-1);
        return pErrRetVal;
    }

    pLanConf = (landoc_t*)Data;
    Dhcpv4_Reset_Cache();
    ret = Dhcpv4_Lan_Validate(pLanConf);
    if (0 != ret)
    {
        char *errMsg = ConvertErrCodeToErrMsg(ret);

        CcspTraceWarning((" validation failed %s  Reason : %s \n",__FUNCTION__,errMsg));
        pErrRetVal->ErrorCode = VALIDATION_FALIED;
        strncpy(pErrRetVal->ErrorMsg,errMsg,sizeof(pErrRetVal->ErrorMsg)-1);
        return pErrRetVal;

    }
    Dhcpv4_Lan_MutexLock();
    Dhcpv4_Lan_BackupFromDb(g_dhcpv4bkup_cache,&g_numOfbkupCacheParam);
    ret = Dhcpv4_UnsetAllDbValues(g_dhcpv4bkup_cache,g_numOfbkupCacheParam);
    if (SYSCFG_FAILURE == ret)
    {
        pErrRetVal->ErrorCode = SYSCFG_FAILURE;
        strncpy(pErrRetVal->ErrorMsg,"sysconfig failure while clearing entries",sizeof(pErrRetVal->ErrorMsg)-1);
        Dhcpv4_Lan_MutexUnLock();
        return pErrRetVal;
    }

    ret = Dhcpv4_Lan_UpdateConfToCache(pLanConf,cache_temp,&cacheSize);
    if (ret == 0)
    {
        g_numOfReceivedParam = cacheSize;
        ret = Dhcpv4_Cache_ApplyAllToDb(cache_temp,cacheSize);
        if (SYSCFG_FAILURE == ret)
        {
            pErrRetVal->ErrorCode = SYSCFG_FAILURE;
            strncpy(pErrRetVal->ErrorMsg,"sysconfig failure while applying into db",sizeof(pErrRetVal->ErrorMsg)-1);
        }
        else
        {
            ret = Dhcpv4_Lan_Synchronize();
            if (0 != ret)
            {
                CcspTraceWarning((" %s Sync Failed \n",__FUNCTION__));
                pErrRetVal->ErrorCode = COSA_SYNCHRONIZE_FAILED;
                strncpy(pErrRetVal->ErrorMsg,"Sync with Cosa object failure",sizeof(pErrRetVal->ErrorMsg)-1);
            }
            system("sysevent set lan-restart");
            system("sysevent set dhcp_server-restart");
        }
    }
    Dhcpv4_Lan_MutexUnLock();
    return pErrRetVal;
}

/* Callback function to rollback when Lan blob execution fails */
int rollback_Lan()
{
    // return 0 to notify framework when rollback is success
    CcspTraceInfo((" Entering %s \n",__FUNCTION__));

    int ret = 0;
    Dhcpv4_Lan_MutexLock();
    ret = Dhcpv4_Cache_ApplyAllToDb(g_dhcpv4bkup_cache,g_numOfbkupCacheParam);
    ret = Dhcpv4_Lan_Synchronize();
    Dhcpv4_Lan_MutexUnLock();
    system("sysevent set lan-restart");
    system("sysevent set dhcp_server-restart");
    CcspTraceWarning((" Success %s \n",__FUNCTION__));

    return ret ;
}

void FreeResources_Lan(void *arg)
{
    
    CcspTraceWarning((" Entering %s \n",__FUNCTION__));
    execData *blob_exec_data  = (execData*) arg;
    landoc_t *pLanConf = (landoc_t *) blob_exec_data->user_data ;

    if (pLanConf != NULL)
    {
        landoc_destroy(pLanConf);
        pLanConf = NULL;
    }

    if (blob_exec_data != NULL)
    {
        free(blob_exec_data);
        blob_exec_data = NULL;
    }
}
