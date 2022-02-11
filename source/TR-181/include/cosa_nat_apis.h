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

    module: cosa_nat_apis.h

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


#ifndef  _COSA_NAT_APIS_H
#define  _COSA_NAT_APIS_H

#include "cosa_apis.h"
#include "cosa_nat_apis_custom.h"



#define COSA_NAT_SYSCFG_NAMESPACE "CosaNAT"
#define COSA_NAT_ID_SYSCFG_NAMESPACE COSA_NAT_SYSCFG_NAMESPACE"IDs"
#define PORT_TRIGGER_ENABLED_KEY "port_trigger_enabled" 
#define PORT_FORWARD_ENABLED_KEY "port_forward_enabled" 
#define HS_PORT_FORWARD_ENABLED_KEY "port_hs_forward_enabled" 
#define NAT_FORWARD_ENABLED_KEY "nat_forward_enabled" 


/***********************************
    Actual definition declaration
************************************/

/*
    Nat Part
*/
typedef  enum
_COSA_DML_NAT_STATUS
{
    COSA_DML_NAT_STATUS_Disabled                = 1,
    COSA_DML_NAT_STATUS_Enabled,
    COSA_DML_NAT_STATUS_Error_Misconfigured,
    COSA_DML_NAT_STATUS_Error
}
COSA_DML_NAT_STATUS, *PCOSA_DML_NAT_STATUS;

typedef  struct
_COSA_DML_NAT
{
    ULONG                           X_CISCO_COM_TCPTimeout;
    ULONG                           X_CISCO_COM_UDPTimeout;
    ULONG                           X_CISCO_COM_ICMPTimeout;

    COSA_DML_NAT_CUSTOM
}
COSA_DML_NAT, *PCOSA_DML_NAT;

typedef  struct
_COSA_DML_NAT_DMZ
{
    BOOLEAN                         bEnabled;
    char                            RemoteIPStart[COSA_DML_IP_ADDR_LENGTH ];
    char                            RemoteIPEnd  [COSA_DML_IP_ADDR_LENGTH ];
    char                            InternalIP   [COSA_DML_IP_ADDR_LENGTH ];
    char                            InternalMAC  [COSA_DML_MAC_ADDR_LENGTH];
    char                            IPv6Host     [64];
}
COSA_DML_NAT_DMZ, *PCOSA_DML_NAT_DMZ;

typedef enum
{
    COSA_DML_NAT_PMAPPING_Origin_Static = 1,
    COSA_DML_NAT_PMAPPING_Origin_Dynamic
}
COSA_DML_NAT_PMAPPING_Origin;

typedef  struct
_COSA_DML_NAT_PMAPPING
{
    ULONG                           InstanceNumber;
    CHAR                            Alias[COSA_DML_IF_NAME_LENGTH];

    BOOLEAN                         bEnabled;
    COSA_DML_NAT_STATUS             Status;
    CHAR                            Interface[COSA_DML_IF_NAME_LENGTH]; /* IP interface name */
    BOOLEAN                         AllInterfaces;
    ULONG                           LeaseDuration;
    ANSC_IPV4_ADDRESS               RemoteHost;
    ANSC_IPV4_ADDRESS               PublicIP;
    USHORT                          ExternalPort;
    USHORT                          ExternalPortEndRange;
    USHORT                          InternalPort;
    UCHAR                           Protocol;                           /* IP protocol number for TCP, UDP */
    ANSC_IPV4_ADDRESS               InternalClient;
    char                            Description[256];
    char                            X_CISCO_COM_InternalClientV6[64];
    COSA_DML_NAT_PMAPPING_Origin    X_CISCO_COM_Origin;
}
COSA_DML_NAT_PMAPPING,  *PCOSA_DML_NAT_PMAPPING;

typedef  struct
_COSA_DML_NAT_PTRIGGER
{
    ULONG                           InstanceNumber;
    char                            Alias[COSA_DML_ALIAS_LENGTH];
    char*                           pOriAlias;

    BOOLEAN                         bEnabled;
    UCHAR                           TriggerProtocol;
    USHORT                          TriggerPortStart;
    USHORT                          TriggerPortEnd;
    UCHAR                           ForwardProtocol;
    USHORT                          ForwardPortStart;
    USHORT                          ForwardPortEnd;
    char                            Description[64];
}
COSA_DML_NAT_PTRIGGER,  *PCOSA_DML_NAT_PTRIGGER;

#define COSA_DML_NAT_PTRIGGER_INIT(pPortTrigger)                                            \
{                                                                                           \
    (pPortTrigger)->bEnabled                 = FALSE;                                       \
    (pPortTrigger)->pOriAlias                = NULL;                                        \
    (pPortTrigger)->TriggerProtocol          = 0;                                           \
    (pPortTrigger)->TriggerPortStart         = 0;                                           \
    (pPortTrigger)->TriggerPortEnd           = 0;                                           \
    (pPortTrigger)->ForwardProtocol          = 0;                                           \
    (pPortTrigger)->ForwardPortStart         = 0;                                           \
    (pPortTrigger)->ForwardPortEnd           = 0;                                           \
}                                                                                           \


/*
Description:
    This callback routine is provided for backend to call middle layer to generate instance number/alias, when backend loads a NAT port mapping from persistent configuration and finds out it doesn't contain instance number/alias.
Arguments:
    hDml        Opaque handle passed in from CosaDmlNatInit.
    pEntry        Pointer to NAT port mapping to receive the generated values.
Return:
    Status of operation


*/
typedef ANSC_STATUS
(*PFN_COSA_DML_NAT_GEN)
    (
        ANSC_HANDLE                 hDml,
        PCOSA_DML_NAT_PMAPPING      pEntry
);


/*************************************
    The actual function declaration
**************************************/

/*
Description:
    This is the initialization routine for NAT backend.
Arguments:
    hDml        Opaque handle from DM adapter. Backend saves this handle for calling pValueGenFn.
    phContext       Opaque handle passed back from backend, needed by CosaDmlNatXyz() routines.
    pValueGenFn Function pointer to instance number/alias generation callback.
Return:
Status of operation.

*/
ANSC_STATUS
CosaDmlNatInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext,
        PFN_COSA_DML_NAT_GEN        pValueGenFn
    );

ANSC_STATUS
CosaDmlNatGet
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT               pDmlNat
    );

ANSC_STATUS
CosaDmlNatSet
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT               pDmlNat
    );

ANSC_STATUS
CosaDmlNatGetDmz
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_DMZ           pDmlDmz
    );

ANSC_STATUS
CosaDmlNatSetDmz
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_DMZ           pDmlDmz
    );

ANSC_STATUS
CosaDmlNatGetLanIP
    (
        ANSC_HANDLE                 hContext
    );

ANSC_STATUS
CosaDmlNatGetPortMapping
    (
        ANSC_HANDLE                 hContext,
        ULONG                      InstanceNumber,
        PCOSA_DML_NAT_PMAPPING      pNatPMapping
    );
/*
Description:
    This routine is to retrieve the complete list of NAT port mappings, which is a dynamic table.
Arguments:
    pulCount        To receive the actual number of entries.
Return:
The pointer to the array of NAT port mappings, allocated by callee. If no entry is found, NULL is returned.
*/
PCOSA_DML_NAT_PMAPPING
CosaDmlNatGetPortMappings
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount
    );

/*
    Description:
    The API adds one port mapping entry into NAT.
Arguments:
    pEntry      Caller does not need to fill in Status fields. Upon return, callee fills in associated Status.
*/
ANSC_STATUS
CosaDmlNatAddPortMapping
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_PMAPPING      pEntry
    );


ANSC_STATUS
CosaDmlNatDelPortMapping
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    );

#ifdef _BWG_PRODUCT_REQ_
//CGWTDETS-8737 : Usable Statics will no longer support 1-1 NAT :: START
ANSC_STATUS UpdateList(ANSC_HANDLE hContext);
//CGWTDETS-8737 : Usable Statics will no longer support 1-1 NAT :: END
#endif

/*
Description:
    The API re-configures the designated port mapping entry.
Arguments:
    pEntry      The new configuration is passed through this argument. The entry is identified through InstanceNumber field.

*/
ANSC_STATUS
CosaDmlNatSetPortMapping
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_PMAPPING      pEntry          /* Identified by InstanceNumber */
    );


/* APIs for PortTrigger */

PCOSA_DML_NAT_PTRIGGER
CosaDmlNatGetPortTriggers
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
        BOOLEAN                     bCommit
    );

ANSC_STATUS
CosaDmlNatAddPortTrigger
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_PTRIGGER      pEntry
    );

ANSC_STATUS
CosaDmlNatGetPortTrigger
    (
        ANSC_HANDLE                 hContext,
        ULONG                       InstanceNum,
        PCOSA_DML_NAT_PTRIGGER      pNatPTrigger
    );
    
ANSC_STATUS
CosaDmlNatDelPortTrigger
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_PTRIGGER      pEntry
    );

ANSC_STATUS
CosaDmlNatSetPortTrigger
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_PTRIGGER      pEntry          /* Identified by InstanceNumber */
    );

ANSC_STATUS
CosaDmlNatGetPortTriggerEnable(BOOL *pBool);

ANSC_STATUS
CosaDmlNatSetPortTriggerEnable(BOOL vBool);

BOOL 
CosaDmlNatChkPortMappingIPV6Address(char* address);

BOOL 
CosaDmlNatChkPortMappingClient(ULONG client);

int
CosaDmlNatChkPortRange(ULONG InstanceNumber, BOOLEAN enabled, int start, int end, int protocol, int is_trigger);

int
CosaDmlNatChkTriggerPortRange(ULONG InstanceNumber, BOOLEAN enabled, int start, int end, int protocol, int is_trigger);

int
CosaDmlChkDesp(char *desp);

BOOL 
CosaDmlNatChkEnableFlg(PCOSA_DML_NAT_PMAPPING pPortMapping);

int 
_Check_PF_parameter(PCOSA_DML_NAT_PMAPPING pPortMapping);

int 
_Check_PT_parameter(PCOSA_DML_NAT_PTRIGGER pPortTrigger);

BOOL 
CosaDmlNatChkPortMappingMaxRuleNum(PCOSA_DML_NAT_PMAPPING pEntry);

#if defined(FEATURE_MAPT) || defined(FEATURE_SUPPORT_MAPT_NAT46)
//if defined (_XB6_PRODUCT_REQ_) || defined (_XB7_PRODUCT_REQ_)

ANSC_STATUS
CosaDmlNatGetActiveIPv4UdpInternalPorts
    (
        int*                       nports
    );

ANSC_STATUS
CosaDmlNatGetActiveIPv4TcpInternalPorts
    (
        int*                       nports        
    );
#endif
#endif
