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

#ifndef  _COSA_DHCPV4_WEBCONFIG_APIS_H
#define  _COSA_DHCPV4_WEBCONFIG_APIS_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include "ansc_status.h"
#include "cosa_apis.h"
#include "cosa_dhcpv4_apis.h"
#include "cosa_dhcpv4_internal.h"
#include "cosa_lanmanagement_apis.h"
#include "cosa_lanmanagement_internal.h"
#include "cosa_x_cisco_com_devicecontrol_apis.h"
#include "cosa_x_cisco_com_devicecontrol_internal.h"
#include "webconfig_framework.h"
#include "macbinding_webconfig_param.h"
#include "lan_webconfig_param.h"

#define EMPTY_BLOB  199
#define NAME_MAX_SIZE 32
#define VAL_MAX_SIZE  256
#define DHCPV4_CACHE_MAX_NUM_OF_PARAM 1024

#define DHCPV4_NUM_STATIC_HOSTS "dhcp_num_static_hosts"
#define DHCPV4_STATIC_HOSTS_INSNUM "dhcp_static_host_insNum_"
#define DHCPV4_STATIC_HOST "dhcp_static_host_"
#define DHCPV4_COMMENTS_LAN_CLIENTS "lan_clients_"
#define DHCPV4_COMMENTS_LAN_CLIENTS_COUNT "lan_clients_count"
#define DHCPV4_COMMENTS "lan_clients_mac_"
#define DHCPV4_STATIC_HOST_ALIAS "dhcp_static_host_alias_"


#define DHCPV4_SERVER_ENABLED "dhcp_server_enabled"
#define DHCPV4_SERVER_START_IP "dhcp_start"
#define DHCPV4_SERVER_END_IP "dhcp_end"
#define DHCPV4_LEASE_TIME "dhcp_lease_time"
#define DHCPV4_LAN_IP "lan_ipaddr"
#define DHCPV4_LAN_NETMASK "lan_netmask"


typedef struct Dhcpv4Cache
{
    char cmd[NAME_MAX_SIZE];
    char val[VAL_MAX_SIZE];
}Dhcpv4_Cache_t;

int CheckIpIsValid( char *ipAddress );
char* ConvertErrCodeToErrMsg(int errCode);
int Dhcpv4_StaticClients_Synchronize();
pErr Process_StaticClients_WebConfigRequest(void *Data);
int rollback_StaticClients();
void FreeResources_StaticClients(void *arg);
pErr Process_Lan_WebConfigRequest(void *Data);
int rollback_Lan();
void FreeResources_Lan(void *arg);
int Dhcpv4_StaticClients_MutexLock();
int Dhcpv4_StaticClients_MutexUnLock();
int Dhcpv4_StaticClients_MutexTryLock();
int Dhcpv4_Lan_MutexLock();
int Dhcpv4_Lan_MutexUnLock();
int Dhcpv4_Lan_MutexTryLock();
BOOL Dhcpv4_Lan_CheckIsValidLeaseTime(uint32_t leasetime);
#endif
