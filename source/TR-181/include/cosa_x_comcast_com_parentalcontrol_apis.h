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

#ifndef _COSA_X_COMCAST_COM_PARENTALCONTROL_APIS_H
#define _COSA_X_COMCAST_COM_PARENTALCONTROL_APIS_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"

typedef enum
{
    IPADDR_IPV4 = 1,
    IPADDR_IPV6 = 2,
}
COSA_DML_IPADDR_TYPE;

typedef enum
{
    PROTO_TCP = 1,
    PROTO_UDP = 2,
    PROTO_BOTH = 3,
}
COSA_DML_PROTO_TYPE;

typedef enum
{
    BLOCK_METHOD_URL = 1,
    BLOCK_METHOD_KEYWORD = 2,
}
COSA_DML_BLOCK_METHOD;

typedef enum
{
    MD_TYPE_BLOCK = 1,
    MD_TYPE_ALLOW = 2,
}
COSA_DML_MD_TYPE;


/*
 * .ManagedSites.
 */
typedef struct
_COSA_DML_MANAGEDSITES
{
    BOOL            Enable;
}
COSA_DML_MANAGEDSITES;

/*
 * .ManagedSites.BlockedURL.{i}.
 */
typedef struct
_COSA_DML_BLOCKEDURL
{
    ULONG           InstanceNumber;
    char            Alias[256];

    COSA_DML_BLOCK_METHOD BlockMethod;
    char            Site[1025];
    BOOL            AlwaysBlock;
    char            StartTime[64];
    char            EndTime[64];
    BOOL            StartTimeFlg;
    BOOL            EndTimeFlg;
    char            BlockDays[64];
    char            MAC[32];
    char            DeviceName[128];
}
COSA_DML_BLOCKEDURL;

/*
 * .ManagedSites.TrustedUser.{i}.
 */
typedef struct
_COSA_DML_TRUSTEDUSER
{
    ULONG                   InstanceNumber;
    char                    Alias[256];

    char                    HostDescription[64];
    COSA_DML_IPADDR_TYPE    IPAddressType;
    char                    IPAddress[64];
    BOOL                    Trusted;
}
COSA_DML_TRUSTEDUSER;

/*
 * .ManagedServices.
 */
typedef struct
_COSA_DML_MANAGED_SERVS
{
    BOOL            Enable;
}
COSA_DML_MANAGED_SERVS;

/*
 * .ManagedServices.Service.{i}.
 */
typedef struct
_COSA_DML_MS_SERV
{
    ULONG           InstanceNumber;
    char            Alias[256];

    char            Description[64];
    COSA_DML_PROTO_TYPE Protocol;
    ULONG           StartPort;
    ULONG           EndPort;
    BOOL            AlwaysBlock;
    char            StartTime[64];
    char            EndTime[64];
    char            BlockDays[64];
}
COSA_DML_MS_SERV;

/*
 * .ManagedServices.TrustedUser.{i}.
 */
typedef struct
_COSA_DML_MS_TRUSTEDUSER
{
    ULONG           InstanceNumber;
    char            Alias[256];

    char                    HostDescription[64];
    COSA_DML_IPADDR_TYPE    IPAddressType;
    char                    IPAddress[64];
    BOOL                    Trusted;
}
COSA_DML_MS_TRUSTEDUSER;

/*
 * .ManagedDevices.Redirect
 */
typedef struct 
_COSA_DML_MD_RED
{
    char                        HTTP_Server_IP[17];
    char                        HTTPS_Server_IP[17];
    char                        Default_Server_IP[17];
    char                        HTTP_Server_IPv6[64];
    char                        HTTPS_Server_IPv6[64];
    char                        Default_Server_IPv6[64];
} 
COSA_DML_MD_RED;
/*
 * .ManagedDevices.
 */
typedef struct
_COSA_DML_MANAGED_DEVS
{
    BOOL            Enable;
    BOOL            AllowAll;
}
COSA_DML_MANAGED_DEVS;

/*
 * .ManagedDevices.Device.{i}.
 */
typedef struct
_COSA_DML_MD_DEV
{
    ULONG           InstanceNumber;
    char            Alias[256];

    COSA_DML_MD_TYPE Type;
    char            Description[64];
    char            MACAddress[64];
    BOOL            AlwaysBlock;
    char            StartTime[64];
    char            EndTime[64];
    char            BlockDays[64];
}
COSA_DML_MD_DEV;

ANSC_STATUS
CosaDmlParentalControlInit(ANSC_HANDLE hDml, PANSC_HANDLE phContext);

ANSC_STATUS
CosaDmlMngSites_GetConf(COSA_DML_MANAGEDSITES *conf);

ANSC_STATUS
CosaDmlMngSites_SetConf(COSA_DML_MANAGEDSITES *conf);

ANSC_STATUS
CosaDmlMngServs_GetConf(COSA_DML_MANAGED_SERVS *conf);

ANSC_STATUS
CosaDmlMngServs_SetConf(COSA_DML_MANAGED_SERVS *conf);

ANSC_STATUS
CosaDmlMngDevs_GetConf(COSA_DML_MANAGED_DEVS *conf);

ANSC_STATUS
CosaDmlMngDevs_SetConf(COSA_DML_MANAGED_DEVS *conf);

ULONG
CosaDmlBlkURL_GetNumberOfEntries(void);

ANSC_STATUS
CosaDmlBlkURL_GetEntryByIndex(ULONG index, COSA_DML_BLOCKEDURL *pEntry);

ANSC_STATUS
CosaDmlBlkURL_SetValues(ULONG index, ULONG ins, const char *alias);

ANSC_STATUS
CosaDmlBlkURL_AddEntry(COSA_DML_BLOCKEDURL *pEntry);

ANSC_STATUS
CosaDmlBlkURL_DelEntry(ULONG ins);

ANSC_STATUS
CosaDmlBlkURL_GetConf(ULONG ins, COSA_DML_BLOCKEDURL *pEntry);

ANSC_STATUS
CosaDmlBlkURL_SetConf(ULONG ins, COSA_DML_BLOCKEDURL *pEntry);

ULONG
CosaDmlTrustedUser_GetNumberOfEntries(void);

ANSC_STATUS
CosaDmlTrustedUser_GetEntryByIndex(ULONG index, COSA_DML_TRUSTEDUSER *pEntry);

ANSC_STATUS
CosaDmlTrustedUser_SetValues(ULONG index, ULONG ins, const char *alias);

ANSC_STATUS
CosaDmlTrustedUser_AddEntry(COSA_DML_TRUSTEDUSER *pEntry);

ANSC_STATUS
CosaDmlTrustedUser_DelEntry(ULONG ins);

ANSC_STATUS
CosaDmlTrustedUser_GetConf(ULONG ins, COSA_DML_TRUSTEDUSER *pEntry);

ANSC_STATUS
CosaDmlTrustedUser_SetConf(ULONG ins, COSA_DML_TRUSTEDUSER *pEntry);

ULONG
CosaDmlMSServ_GetNumberOfEntries(void);

ANSC_STATUS
CosaDmlMSServ_GetEntryByIndex(ULONG index, COSA_DML_MS_SERV *pEntry);

ANSC_STATUS
CosaDmlMSServ_SetValues(ULONG index, ULONG ins, const char *alias);

ANSC_STATUS
CosaDmlMSServ_AddEntry(COSA_DML_MS_SERV *pEntry);

ANSC_STATUS
CosaDmlMSServ_DelEntry(ULONG ins);

ANSC_STATUS
CosaDmlMSServ_GetConf(ULONG ins, COSA_DML_MS_SERV *pEntry);

ANSC_STATUS
CosaDmlMSServ_SetConf(ULONG ins, COSA_DML_MS_SERV *pEntry);

ULONG
CosaDmlMSTrustedUser_GetNumberOfEntries(void);

ANSC_STATUS
CosaDmlMSTrustedUser_GetEntryByIndex(ULONG index, COSA_DML_MS_TRUSTEDUSER *pEntry);

ANSC_STATUS
CosaDmlMSTrustedUser_SetValues(ULONG index, ULONG ins, const char *alias);

ANSC_STATUS
CosaDmlMSTrustedUser_AddEntry(COSA_DML_MS_TRUSTEDUSER *pEntry);

ANSC_STATUS
CosaDmlMSTrustedUser_DelEntry(ULONG ins);

ANSC_STATUS
CosaDmlMSTrustedUser_GetConf(ULONG ins, COSA_DML_MS_TRUSTEDUSER *pEntry);

ANSC_STATUS
CosaDmlMSTrustedUser_SetConf(ULONG ins, COSA_DML_MS_TRUSTEDUSER *pEntry);

ULONG
CosaDmlMDDev_GetNumberOfEntries(void);

ANSC_STATUS
CosaDmlMDDev_GetEntryByIndex(ULONG index, COSA_DML_MD_DEV *pEntry);

ANSC_STATUS
CosaDmlMDDev_SetValues(ULONG index, ULONG ins, const char *alias);

ANSC_STATUS
CosaDmlMDDev_AddEntry(COSA_DML_MD_DEV *pEntry);

ANSC_STATUS
CosaDmlMDDev_DelEntry(ULONG ins);

ANSC_STATUS
CosaDmlMDDev_GetConf(ULONG ins, COSA_DML_MD_DEV *pEntry);

ANSC_STATUS
CosaDmlMDDev_SetConf(ULONG ins, COSA_DML_MD_DEV *pEntry);
ANSC_STATUS
CosaDmlMDRed_GetConf(COSA_DML_MD_RED *pEntry);

ANSC_STATUS
CosaDmlMDRed_SetConf(COSA_DML_MD_RED *pEntry);

ANSC_STATUS isValidIP( int type, char *ipAddress, char *wrapped_inputparam, int sizeof_wrapped_inputparam);

int ConvLocalToUTC(char* LocalTime, char* UtcTime);

int ConvUTCToLocal( char* UtcTime, char* LocalTime);

int split_BlockDays(int sRet, int eRet, char *sBDays, char *eBDays);

#ifdef UTC_ENABLE

ANSC_STATUS CosaDmlBlkURL_Migration();

ANSC_STATUS CosaDmlMSServ_Migration();

ANSC_STATUS CosaDmlMDDev_Migration();

ANSC_STATUS CosaDmlBlkURL_RollbackUTCtoLocal();

ANSC_STATUS CosaDmlMSServ_RollbackUTCtoLocal();

ANSC_STATUS CosaDmlMDDev_RollbackUTCtoLocal();
#endif
#endif /* _COSA_X_COMCAST_COM_PARENTALCONTROL_H_ */
