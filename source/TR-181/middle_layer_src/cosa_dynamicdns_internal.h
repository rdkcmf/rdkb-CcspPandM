#ifndef  _COSA_DYNAMICDNS_INTERNAL_H
#define  _COSA_DYNAMICDNS_INTERNAL_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "cosa_dynamicdns_apis.h"

#define COSA_IREP_FOLDER_NAME_DDNS_CLIENT           "DynamicDnsClient"
#define COSA_DML_RR_NAME_DDNS_CLIENT_Alias          "DynamicDnsClientAlias"
#define COSA_DML_RR_NAME_DDNS_CLIENT_InsNum         "DynamicDnsClientInstanceNumber"
#define COSA_DML_RR_NAME_DDNS_CLIENT_NextInsNumber  "DynamicDnsClientNextInstanceNumber"
#define COSA_DML_RR_NAME_DDNS_CLIENT_NewlyAdded     "DynamicDnsClientNewlyAdded"
#define COSA_IREP_FOLDER_NAME_DDNS_HOST             "DynamicDnsHost"
#define COSA_DML_RR_NAME_DDNS_HOST_Alias            "DynamicDnsHostAlias"
#define COSA_DML_RR_NAME_DDNS_HOST_InsNum           "DynamicDnsHostInstanceNumber"
#define COSA_DML_RR_NAME_DDNS_HOST_NextInsNumber    "DynamicDnsHostNextInstanceNumber"
#define COSA_IREP_FOLDER_NAME_DDNS_SERVER           "DynamicDnsServer"
#define COSA_DML_RR_NAME_DDNS_SERVER_Alias          "DynamicDnsServerAlias"
#define COSA_DML_RR_NAME_DDNS_SERVER_InsNum         "DynamicDnsServerInstanceNumber"
#define COSA_DML_RR_NAME_DDNS_SERVER_NextInsNumber  "DynamicDnsServerNextInstanceNumber"

#define  COSA_DATAMODEL_TR181_DDNS_CLASS_CONTENT                                              \
    /* duplication of the base object class content */                                             \
    COSA_BASE_CONTENT                                                                   \
    /* start of DynamicDNS.client object class content */                                          \
    SLIST_HEADER                    DDNSClientList;                                     \
    ULONG                           DDNSClientNextInsNum;                               \
    ANSC_HANDLE                     hIrepFolderDDNSClient;                              \
    /* start of DynamicDNS.client.{i}.hostname object class content */                             \
    SLIST_HEADER                    DDNSHostList;                                       \
    ULONG                           DDNSHostNextInsNum;                                 \
    ANSC_HANDLE                     hIrepFolderDDNSHost;                                \
    /* start of DynamicDNS.Server.{i}. */                                                          \
    SLIST_HEADER                    DDNSServerList;                                     \
    ULONG                           DDNSServerNextInsNum;                               \
    ANSC_HANDLE                     hIrepFolderDDNSServer;                              \

typedef  struct
_COSA_DATAMODEL_TR181_DDNS
{
    COSA_DATAMODEL_TR181_DDNS_CLASS_CONTENT
}
COSA_DATAMODEL_TR181_DDNS,  *PCOSA_DATAMODEL_TR181_DDNS;

/**********************************
    Standard function declaration
***********************************/

ANSC_HANDLE
CosaDynamicDnsCreate
    (
        VOID
    );

ANSC_STATUS
CosaDynamicDnsInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaDynamicDnsRemove
    (
        ANSC_HANDLE                 hThisObject
    );

//Dynamic DNS Client
ANSC_STATUS
CosaDynamicDns_ClientGetInfo
    (
        ANSC_HANDLE                 hThisObject
    );
ANSC_STATUS
CosaDynamicDns_ClientAddInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );
ANSC_STATUS
CosaDynamicDns_ClientDelInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );

//Dynamic DNS Host
ANSC_STATUS
CosaDynamicDns_HostGetInfo
    (
       ANSC_HANDLE                 hThisObject
    );
ANSC_STATUS
CosaDynamicDns_HostAddInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );
ANSC_STATUS
CosaDynamicDns_HostDelInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );

//Dynamic DNS Server
ANSC_STATUS
CosaDynamicDns_ServerGetInfo
    (
        ANSC_HANDLE                 hThisObject
    );
ANSC_STATUS
CosaDynamicDns_ServerAddInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );
ANSC_STATUS
CosaDynamicDns_ServerDelInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );
#endif //_COSA_DYNAMICDNS_INTERNAL_H
