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

    module: cosa_dhcpv4_internal.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**************************************************************************/


#ifndef  _COSA_DHCPV4_INTERNAL_H
#define  _COSA_DHCPV4_INTERNAL_H

#include "cosa_apis.h"


/*
  *   This is cosa middle layer definition 
  *
  */
#define  COSA_IREP_FOLDER_NAME_DHCPV4                      "Dhcpv4"
#ifndef FEATURE_RDKB_WAN_MANAGER
#define  COSA_IREP_FOLDER_NAME_DHCPV4_CLIENT               "Client"
#define  COSA_IREP_FOLDER_NAME_DHCPV4_REQOPTION            "ReqOption"
#define  COSA_IREP_FOLDER_NAME_DHCPV4_SENTOPTION           "SentOption"
#endif
#define  COSA_IREP_FOLDER_NAME_DHCPV4_X_CISCO_COM_SADDR    "X_CISCO_COM_SAddr"
#define  COSA_IREP_FOLDER_NAME_DHCPV4_POOL                 "Pool"
#define  COSA_IREP_FOLDER_NAME_DHCPV4_STATICADDRESS        "StaticAddress"
#define  COSA_IREP_FOLDER_NAME_DHCPV4_OPTION               "Option"
#define  COSA_DML_RR_NAME_Dhcpv4Alias                      "Alias"
#define  COSA_DML_RR_NAME_Dhcpv4bNew                       "bNew"
#define  COSA_DML_RR_NAME_Dhcpv4NextInsNunmber             "NextInstanceNumber"

#define  COSA_DML_DHCPV4_ALIAS                             64

#define  COSA_NAT_ROLLBACK_TEST                            0       /* This is just for test purpose */
#define  COSA_DML_DHCPV4_ACCESS_INTERVAL_POOLCLIENT        10 /* seconds*/

/*
*  This struct is only for dhcpc because it have two sub tables.
*  For the two table, they just use common link struct because they havenot sub tables.
*/
#define  COSA_CONTEXT_DHCPC_LINK_CLASS_CONTENT                                              \
        COSA_CONTEXT_LINK_CLASS_CONTENT                                                     \
        SLIST_HEADER                      SendOptionList;                                   \
        SLIST_HEADER                      ReqOptionList;                                    \
        ULONG                             maxInstanceOfSend;                                \
        ULONG                             maxInstanceOfReq;                                 \
        CHAR                              AliasOfReq[COSA_DML_DHCPV4_ALIAS];                \
        CHAR                              AliasOfSend[COSA_DML_DHCPV4_ALIAS];               \

typedef  struct
_COSA_CONTEXT_DHCPC_LINK_OBJECT
{
    COSA_CONTEXT_DHCPC_LINK_CLASS_CONTENT
}
COSA_CONTEXT_DHCPC_LINK_OBJECT, *PCOSA_CONTEXT_DHCPC_LINK_OBJECT;

#define  ACCESS_COSA_CONTEXT_DHCPC_LINK_OBJECT(p)                                \
         ACCESS_CONTAINER(p, COSA_CONTEXT_DHCPC_LINK_OBJECT, Linkage)            \

/*
*  This struct is only for dhcps.pool because it have one sub tables.
*  For the two table, they just use common link struct because they havenot sub tables.
*/
#define  COSA_CONTEXT_POOL_LINK_CLASS_CONTENT                                              \
        COSA_CONTEXT_LINK_CLASS_CONTENT                                                    \
        SLIST_HEADER                      StaticAddressList;                               \
        ULONG                             maxInstanceOfStaticAddress;                      \
        CHAR                              AliasOfStaAddr[COSA_DML_DHCPV4_ALIAS];           \
        SLIST_HEADER                      OptionList;                                      \
        ULONG                             maxInstanceOfOption;                             \
        CHAR                              AliasOfOption[COSA_DML_DHCPV4_ALIAS];            \
        PCOSA_DML_DHCPSV4_CLIENT          pClientList;                                     \
        PCOSA_DML_DHCPSV4_CLIENTCONTENT   pClientContentList;                              \
        ULONG                             NumberOfClient;                                  \
        ULONG                             PreviousVisitTimeOfClient;                       \


typedef  struct
_COSA_CONTEXT_POOL_LINK_OBJECT
{
    COSA_CONTEXT_POOL_LINK_CLASS_CONTENT
}
COSA_CONTEXT_POOL_LINK_OBJECT, *PCOSA_CONTEXT_POOL_LINK_OBJECT;

#define  ACCESS_COSA_CONTEXT_POOL_LINK_OBJECT(p)              \
         ACCESS_CONTAINER(p, COSA_CONTEXT_POOL_LINK_OBJECT, Linkage)

/*
*  This struct is for dhcp.
*/
#define  COSA_DATAMODEL_DHCPV4_CLASS_CONTENT                                              \
    /* duplication of the base object class content */                                    \
    COSA_BASE_CONTENT                                                                     \
    /* start of NAT object class content */                                               \
    SLIST_HEADER                    X_CISCO_COM_StaticAddressList;                        \
    SLIST_HEADER                    ClientList;    /* This is for entry added */          \
    SLIST_HEADER                    PoolList;    /* This is for entry added */            \
    ULONG                           maxInstanceX_CISCO_COM_SAddr;                         \
    ULONG                           maxInstanceOfClient;                                  \
    ULONG                           PreviousVisitTime;                                    \
    ULONG                           maxInstanceOfPool;                                    \
    CHAR                            AliasOfX_CISCO_COM_SAddr[COSA_DML_DHCPV4_ALIAS];      \
    CHAR                            AliasOfClient[COSA_DML_DHCPV4_ALIAS];                 \
    CHAR                            AliasOfPool[COSA_DML_DHCPV4_ALIAS];                   \
    ANSC_HANDLE                     hIrepFolderDhcpv4;                                    \
    BOOL                            syncStaticClientsTable;                                \

typedef  struct
_COSA_DATAMODEL_DHCPV4                                               
{
    COSA_DATAMODEL_DHCPV4_CLASS_CONTENT
}
COSA_DATAMODEL_DHCPV4,  *PCOSA_DATAMODEL_DHCPV4;


#define   DHCPV4_CLIENT_ENTRY_MATCH(src,dst)                       \
    (strcmp((src)->Alias, (dst)->Alias) == 0)

#define   DHCPV4_CLIENT_ENTRY_MATCH2(src,dst)                      \
    (strcmp((src), (dst)) == 0)

#define   DHCPV4_SENDOPTION_ENTRY_MATCH(src,dst)                   \
    (strcmp((src)->Alias, (dst)->Alias) == 0)
        
#define   DHCPV4_SENDOPTION_ENTRY_MATCH2(src,dst)                  \
    (strcmp((src), (dst)) == 0)

#define   DHCPV4_REQOPTION_ENTRY_MATCH(src,dst)                    \
    (strcmp((src)->Alias, (dst)->Alias) == 0)

#define   DHCPV4_REQOPTION_ENTRY_MATCH2(src,dst)                   \
    (strcmp((src), (dst)) == 0)

#define   DHCPV4_POOL_ENTRY_MATCH(src,dst)                         \
    (strcmp((src)->Alias, (dst)->Alias) == 0)

#define   DHCPV4_POOL_ENTRY_MATCH2(src,dst)                        \
    (strcmp((src), (dst)) == 0)

#define   DHCPV4_STATICADDRESS_ENTRY_MATCH(src,dst)                \
    (strcmp((src)->Alias, (dst)->Alias) == 0)
    
#define   DHCPV4_STATICADDRESS_ENTRY_MATCH2(src,dst)               \
    (strcmp((src), (dst)) == 0)

#define   DHCPV4_CLIENT_INITIATION_CONTEXT(pDhcpc)                 \
    COSA_CONTEXT_LINK_INITIATION_CONTENT(((PCOSA_CONTEXT_LINK_OBJECT)(pDhcpc)))  \
    AnscSListInitializeHeader(&(pDhcpc)->SendOptionList);          \
    AnscSListInitializeHeader(&(pDhcpc)->ReqOptionList);           \
    (pDhcpc)->maxInstanceOfSend               = 0;                 \
    (pDhcpc)->maxInstanceOfReq                = 0;                 \
    AnscZeroMemory((pDhcpc)->AliasOfReq,  sizeof((pDhcpc)->AliasOfReq) );  \
    AnscZeroMemory((pDhcpc)->AliasOfSend, sizeof((pDhcpc)->AliasOfSend) ); \

#define   DHCPV4_POOL_INITIATION_CONTEXT(pPool)                            \
    COSA_CONTEXT_LINK_INITIATION_CONTENT(((PCOSA_CONTEXT_LINK_OBJECT)(pPool)))   \
    AnscSListInitializeHeader(&(pPool)->StaticAddressList);        \
    AnscSListInitializeHeader(&(pPool)->OptionList);                   \
    (pPool)->maxInstanceOfStaticAddress       = 0;                 \
    (pPool)->maxInstanceOfOption       = 0;                 \
    AnscZeroMemory((pPool)->AliasOfStaAddr,  sizeof((pPool)->AliasOfStaAddr) );  \
    AnscZeroMemory((pPool)->AliasOfOption, sizeof((pPool)->AliasOfOption) ); \
    (pPool)->pClientList       = NULL;                 \
    (pPool)->pClientContentList       = NULL;                 \
    (pPool)->NumberOfClient       = 0;                 \
    (pPool)->PreviousVisitTimeOfClient       = 0;                 \

#define DHCPV4_CLIENT_SET_DEFAULTVALUE(pDhcpc)                                         \
    (pDhcpc)->Cfg.bEnabled                    = FALSE;                                 \
    AnscZeroMemory((pDhcpc)->Cfg.Interface, sizeof((pDhcpc)->Cfg.Interface));                     \
    (pDhcpc)->Info.Status                     = COSA_DML_DHCP_STATUS_Disabled;         \
    (pDhcpc)->Info.DHCPStatus                 = COSA_DML_DHCPC_STATUS_Init;            \

#define DHCPV4_SENDOPTION_SET_DEFAULTVALUE(pSendOption)                                \
    (pSendOption)->bEnabled                   = FALSE;                                 \
     (pSendOption)->Tag                        = 0;                                    \
     AnscZeroMemory( (pSendOption)->Value, sizeof( (pSendOption)->Value ) );           \
    

#define DHCPV4_REQOPTION_SET_DEFAULTVALUE(pReqOption)                                  \
    (pReqOption)->bEnabled                    = FALSE;                                 \
    (pReqOption)->Order                       = 0;                                     \
    (pReqOption)->Tag                         = 0;                                     \
    AnscZeroMemory( (pReqOption)->Value, sizeof( (pReqOption)->Value ) );              \

#define DHCPV4_POOL_SET_DEFAULTVALUE(pPool)                                            \
    (pPool)->Cfg.bEnabled                     = FALSE;                                 \
    (pPool)->Cfg.VendorClassIDExclude         = FALSE;                                 \
    (pPool)->Cfg.VendorClassIDMode            = COSA_DML_DHCPS_VID_MODE_Exact;         \
    (pPool)->Cfg.ClientIDExclude              = FALSE;                                 \
    (pPool)->Cfg.VendorClassIDMode            = FALSE;                                 \
    (pPool)->Cfg.UserClassIDExclude           = FALSE;                                 \
    (pPool)->Cfg.ChaddrExclude                = FALSE;                                 \
    (pPool)->Cfg.DomainName[0]                = 0;                                     \
    (pPool)->Cfg.LeaseTime                    = 86400;                                 \
    (pPool)->Info.Status                      = COSA_DML_DHCP_STATUS_Disabled;         \
    (pPool)->Cfg.bAllowDelete                 = TRUE;                                  \

#define DHCPV4_X_COM_CISCO_SADDR_SET_DEFAULTVALUE(pSAddr)                              \
    (pSAddr)->Yiaddr.Value                    = 0;                                     \
    AnscZeroMemory( (pSAddr)->Chaddr, sizeof( (pSAddr)->Chaddr ) );                    \
    AnscZeroMemory( (pSAddr)->DeviceName, sizeof( (pSAddr)->DeviceName ) );            \

#define DHCPV4_STATICADDRESS_SET_DEFAULTVALUE(pSAddr)                                  \
    (pSAddr)->ActiveFlag                      = FALSE;                                 \
    (pSAddr)->bEnabled                        = TRUE;                                  \
    (pSAddr)->Yiaddr.Value                    = 0;                                     \
    AnscZeroMemory( (pSAddr)->Chaddr, sizeof( (pSAddr)->Chaddr ) );                    \

#define DHCPV4_OPTION_SET_DEFAULTVALUE(pOption)                                        \
    (pOption)->bEnabled                        = FALSE;                                \


#define DHCPV4_POOLOPTION_SET_DEFAULTVALUE(pPool)                                      \
    (pPool)->bEnabled                        = FALSE;                                  \
    AnscZeroMemory( (pPool)->Value, sizeof( (pPool)->Value ) );                        \

#define   DHCPV4_POOLOPTION_ENTRY_MATCH(src,dst)                                       \
    (strcmp((src), (dst)) == 0)

/*
    Function declaration 
*/ 

ANSC_HANDLE
CosaDhcpv4Create
    (
        VOID
    );

ANSC_STATUS
CosaDhcpv4Initialize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaDhcpv4Remove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaDhcpv4RegGetDhcpv4Info
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaDhcpv4RegSetDhcpv4Info
    (
        ANSC_HANDLE                 hThisObject
    );
#ifndef FEATURE_RDKB_WAN_MANAGER
BOOL
CosaDhcpv4ClientHasDelayAddedChild
    (
        PCOSA_CONTEXT_DHCPC_LINK_OBJECT     hContext
    );

#endif
BOOL
CosaDhcpv4PoolHasDelayAddedChild
    (
        PCOSA_CONTEXT_POOL_LINK_OBJECT      hContext
    );

ANSC_STATUS
CosaDhcpv4BackendGetDhcpv4Info
    (
        ANSC_HANDLE                 hThisObject
    );

BOOL
CosaDmlSetIpaddr
    (
        PULONG pIPAddr, 
        PCHAR pString, 
        ULONG MaxNumber 
    );

BOOL 
CosaDmlGetIpaddrString
    (
        char *pString,
        ULONG *pulStrLength,
        uint32_t *pIPAddr,
        ULONG MaxNumber
    );

ANSC_HANDLE
CosaDhcpv4GetClientContentbyClient
    (
        ANSC_HANDLE                 hClientContext
    );


#endif


