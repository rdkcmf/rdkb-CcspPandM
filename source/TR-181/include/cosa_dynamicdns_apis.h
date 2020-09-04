#ifndef  _COSA_DYNAMICDNS_APIS_H
#define  _COSA_DYNAMICDNS_APIS_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "poam_irepfo_interface.h"
#include "sys_definitions.h"

typedef struct
_COSA_DML_DDNS_CLIENT
{
   ULONG          InstanceNumber;
   char           Alias[64];
   int            Status;
   int            LastError;
   char           Server[256];
   char           Interface[256];
   char           Username[256];
   char           Password[256];
   BOOL           Enable;
}
COSA_DML_DDNS_CLIENT;
typedef struct
_COSA_DML_DDNS_HOST
{
   ULONG          InstanceNumber;
   char           Alias[64];
   int            Status;
   char           Name[256];
   char           LastUpdate[256];
   BOOL           Enable;
}
COSA_DML_DDNS_HOST;

typedef struct
_COSA_DML_DDNS_SERVER
{
   ULONG          InstanceNumber;
   ULONG          ServerPort;
   int            CheckInterval;
   int            RetryInterval;
   int            MaxRetries;
   char           Alias[64];
   char           Name[64];
   char           ServiceName[256];
   char           ServerAddress[256];
   char           SupportedProtocols[64];
   char           Protocol[16];
   BOOL           Enable;
}
COSA_DML_DDNS_SERVER;
/***************************
*  FUNCTION DECLARATION ***
***************************/

//DynamicDNS.
BOOL  CosaDmlDynamicDns_GetEnable();
ULONG CosaDmlDynamicDns_GetsupportedServices(char *supportedServices);
ULONG CosaDmlDynamicDns_SetEnable(BOOL bValue);

//DynamicDNS.Client.{i}
ULONG CosaDmlDynamicDns_Client_GetNumberOfEntries(void);
ANSC_STATUS CosaDmlDynamicDns_Client_GetEntryByIndex(ULONG index, COSA_DML_DDNS_CLIENT *pEntry);
ANSC_STATUS CosaDmlDynamicDns_Client_SetValues(ULONG index, ULONG ins, const char *alias);
ANSC_STATUS CosaDmlDynamicDns_Client_AddEntry(COSA_DML_DDNS_CLIENT *pEntry);
ANSC_STATUS CosaDmlDynamicDns_Client_DelEntry(ULONG ins);
ANSC_STATUS CosaDmlDynamicDns_Client_GetConf(ULONG ins, COSA_DML_DDNS_CLIENT *pEntry);
ANSC_STATUS CosaDmlDynamicDns_Client_SetConf(ULONG ins, COSA_DML_DDNS_CLIENT *pEntry);
ULONG CosaDmlDynamicDns_GetClientStatus();
ULONG CosaDmlDynamicDns_GetClientLastError();

//DynamicDNS.Client.{i}.Host.{i}
ULONG CosaDmlDynamicDns_Host_GetNumberOfEntries(void);
ANSC_STATUS CosaDmlDynamicDns_Host_GetEntryByIndex(ULONG index, COSA_DML_DDNS_HOST *pEntry);
ANSC_STATUS CosaDmlDynamicDns_Host_SetValues(ULONG index, ULONG ins, const char *alias);
ANSC_STATUS CosaDmlDynamicDns_Host_AddEntry(COSA_DML_DDNS_HOST *pEntry);
ANSC_STATUS CosaDmlDynamicDns_Host_DelEntry(ULONG ins);
ANSC_STATUS CosaDmlDynamicDns_Host_GetConf(ULONG ins, COSA_DML_DDNS_HOST *pEntry);
ANSC_STATUS CosaDmlDynamicDns_Host_SetConf(ULONG ins, COSA_DML_DDNS_HOST *pEntry);
/* To get the Last successful update of this name to the Dynamic DNS Server. */
ULONG CosaDmlDynamicDns_GetHostLastUpdate(char*  lastUpdate);
void CosaInitializeTr181DdnsHost();

//DynamicDNS.Server.{i}.
ULONG CosaDmlDynamicDns_Server_GetNumberOfEntries(void);
ANSC_STATUS CosaDmlDynamicDns_Server_GetEntryByIndex(ULONG index, COSA_DML_DDNS_SERVER *pEntry);
ANSC_STATUS CosaDmlDynamicDns_Server_SetValues(ULONG index, ULONG ins, const char *alias);
ANSC_STATUS CosaDmlDynamicDns_Server_AddEntry(COSA_DML_DDNS_SERVER *pEntry);
ANSC_STATUS CosaDmlDynamicDns_Server_DelEntry(ULONG ins);
ANSC_STATUS CosaDmlDynamicDns_Server_GetConf(ULONG ins, COSA_DML_DDNS_SERVER *pEntry);
ANSC_STATUS CosaDmlDynamicDns_Server_SetConf(ULONG ins, COSA_DML_DDNS_SERVER *pEntry);

void CosaInitializeTr181DdnsServiceProviderList();
#endif  //_COSA_DYNAMICDNS_APIS_H
