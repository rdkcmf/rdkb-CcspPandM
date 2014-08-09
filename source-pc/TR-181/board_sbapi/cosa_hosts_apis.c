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


/*
 *  Worst kind of practice to share a global variable between DM middle layer code and SBAPI!
 */
LmObjectHosts lmHosts = {
    .pHostBoolParaName = {"Active"},
    .pHostIntParaName = {"X_CISCO_COM_ActiveTime", "X_CISCO_COM_InactiveTime", "X_CISCO_COM_RSSI"},
    .pHostUlongParaName = {"X_CISCO_COM_DeviceType", "X_CISCO_COM_NetworkInterface", "X_CISCO_COM_ConnectionStatus", "X_CISCO_COM_OSType"},
    .pHostStringParaName = {"Alias", "PhysAddress", "IPAddress", "DHCPClient", "AssociatedDevice", "Layer1Interface", "Layer3Interface", "HostName",
                                        "X_CISCO_COM_UPnPDevice", "X_CISCO_COM_HNAPDevice", "X_CISCO_COM_DNSRecords", "X_CISCO_COM_HardwareVendor",
                                        "X_CISCO_COM_SoftwareVendor", "X_CISCO_COM_SerialNumbre", "X_CISCO_COM_DefinedDeviceType",
                                        "X_CISCO_COM_DefinedHWVendor", "X_CISCO_COM_DefinedSWVendor", "AddressSource", "Comments"},
    .pIPv4AddressStringParaName = {"IPAddress"},
    .pIPv6AddressStringParaName = {"IPAddress"}
};


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

        ANSC_STATUS
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

ANSC_STATUS
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


ANSC_STATUS
CosaDmlHostsSetHostComment
    (
        ANSC_HANDLE                 hContext,
        char*                       pMac,
        char*                       pComment
    )
{
    return ANSC_STATUS_SUCCESS;
}

ULONG CosaDmlHostsGetOnline()
{
    return 2;
}

