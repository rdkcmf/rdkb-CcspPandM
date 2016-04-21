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

    module: cosa_nat_apis.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    copyright:

        Cisco Systems, Inc.
        All Rights Reserved.

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaDmlNatGetPortMappingNumber
        *  CosaDmlNatIsUpdatedPortMappings
        *  CosaDmlNatSynchronizePortMappings
        *  CosaDmlNatGetPortMappings
        *  CosaDmlNatAddPortMapping
        *  CosaDmlNatDelPortMapping
        *  CosaDmlNatSetPortMapping
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
#include "cosa_apis.h"
#include "cosa_nat_apis.h"
#include "cosa_nat_internal.h"
#include "plugin_main_apis.h"

PFN_COSA_DML_NAT_GEN   g_nat_pportmapping_callback = NULL;


COSA_DML_NAT_PMAPPING   g_nat_portmapping[] =
    {
        {
            0,
            "",
            TRUE,
            COSA_DML_NAT_STATUS_Enabled,
            "wan0",
            TRUE,
            232332,
            {
                "\x40\x40\x40\x01"
            },
            {
                "\x40\x40\x40\x01"
            },
            80,
            80,
            22222,
            2,    /* UDP */
            {
                "\xc0\xa8\x01\x01"
            },
            "this is for vod",
            "IntenalClientV6_0"
        },
        {
            0,
            "",
            TRUE,
            COSA_DML_NAT_STATUS_Enabled,
            "wan0",
            TRUE,
            21222,
            {
                "\x40\x40\x40\x02"
            },
            {
                "\x40\x40\x40\x02"
            },
            21,
            21,
            22222,
            1,   /* TCP */
            {
                "\xc0\xa8\x01\x02"
            },
            "this is for ftp",
            "IntenalClientV6_1"
        }
    };

COSA_DML_NAT_DMZ    g_Dmz =
    {
        TRUE,
        "10.10.10.10",
        "10.10.10.11",
        "192.168.1.66"
    };

ANSC_STATUS
CosaDmlNatGetLanIP
    (
        ANSC_HANDLE                 hContext
    )
{
    return ANSC_STATUS_SUCCESS;
}

int _Check_PF_parameter(PCOSA_DML_NAT_PMAPPING pPortMapping)
{
    return TRUE;
}

int _Check_PT_parameter(PCOSA_DML_NAT_PTRIGGER pPortTrigger)
{
    return TRUE;
}
/**********************************************************************

    caller:     self

    prototype:

        BOOL
        CosaDmlNatInit
            (
                ANSC_HANDLE                 hDml,
                PANSC_HANDLE                phContext,
                PFN_COSA_DML_NAT_GEN        pValueGenFn
            );

        Description:
            This is the initialization routine for NAT backend.

        Arguments:
            hDml               Opaque handle from DM adapter. Backend saves this handle for calling pValueGenFn.
             phContext       Opaque handle passed back from backend, needed by CosaDmlNatXyz() routines.
            pValueGenFn    Function pointer to instance number/alias generation callback.

        Return:
            Status of operation.

**********************************************************************/
ANSC_STATUS
CosaDmlNatInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext,
        PFN_COSA_DML_NAT_GEN        pValueGenFn
    )
{
    g_nat_pportmapping_callback = pValueGenFn;

    return ANSC_STATUS_SUCCESS;

}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatGet
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_NAT_DMZ           pDmlDmz
            )

        Description:
            This routine is to retrieve the info of DMZ.

        Arguments:
            hContext  is the reversed context.

        Return:
            The pointer to the info of DMZ.

**********************************************************************/
ANSC_STATUS
CosaDmlNatGet
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT               pDmlNat
    )
{
    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatSet
            (
                ANSC_HANDLE                 hContext,

            )
        Description:
            This routine is to retrieve the info of DMZ.

        Arguments:
            hContext  is the reversed context.

        Return:
            The pointer to the info of DMZ.

**********************************************************************/

ANSC_STATUS
CosaDmlNatSet
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT               pDmlNat
    )
{
    if ( COSA_DML_NAT_CUSTOM_GET_ENABLEPORTMAPPING(pDmlNat) )
    {
        printf("X_Comcast_com_EnablePortMapping is enabled!\n");
    }
    else
    {
        printf("X_Comcast_com_EnablePortMapping is disabled!\n");
    }

    if ( COSA_DML_NAT_CUSTOM_GET_ENABLEHSPORTMAPPING(pDmlNat) )
    {
        printf("X_Comcast_com_EnableHSPortMapping is enabled!\n");
    }
    else
    {
        printf("X_Comcast_com_EnableHSPortMapping is disabled!\n");
    }

    if (COSA_DML_NAT_CUSTOM_GET_ENABLENATMAPPING(pDmlNat) )
    {
        printf("X_Comcast_com_EnableNATMapping is enabled!\n");
    }
    else
    {
        printf("X_Comcast_com_EnableNATMapping is disabled!\n");
    }

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatGetDmz
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_NAT_DMZ           pDmlDmz
            )

        Description:
            This routine is to retrieve the info of DMZ.

        Arguments:
            hContext  is the reversed context.

        Return:
            The pointer to the info of DMZ.

**********************************************************************/

ANSC_STATUS
CosaDmlNatGetDmz
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_DMZ           pDmlDmz
    )
{
    AnscCopyMemory(pDmlDmz, &g_Dmz, sizeof(COSA_DML_NAT_DMZ));

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatSetDmz
            (
                ANSC_HANDLE                 hContext,

            )
        Description:
            This routine is to retrieve the info of DMZ.

        Arguments:
            hContext  is the reversed context.

        Return:
            The pointer to the info of DMZ.

**********************************************************************/

ANSC_STATUS
CosaDmlNatSetDmz
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_DMZ           pDmz
    )
{
    AnscCopyMemory(&g_Dmz, pDmz, sizeof(COSA_DML_NAT_DMZ));

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     self

    prototype:

        PCOSA_DML_NAT_PMAPPING
        CosaDmlNatGetPortMappings
            (
                ANSC_HANDLE                 hContext,
                PULONG                          pulCount
            )
        Description:
            This routine is to retrieve the complete list of NAT port mappings, which is a dynamic table.

        Arguments:
            pulCount  is to receive the actual number of entries.

        Return:
            The pointer to the array of NAT port mappings, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/
PCOSA_DML_NAT_PMAPPING
CosaDmlNatGetPortMappings
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount
    )
{
    PCOSA_DML_NAT_PMAPPING pNatPMapping = NULL;
    ULONG                         index = 0;

    for(index =0; index < sizeof(g_nat_portmapping)/sizeof(COSA_DML_NAT_PMAPPING); index++)
    {
        if ( g_nat_portmapping[index].InstanceNumber  ==  0 )
        {
            g_nat_pportmapping_callback( NULL, &g_nat_portmapping[index] );

        }
    }

    pNatPMapping = (PCOSA_DML_NAT_PMAPPING)AnscAllocateMemory( sizeof(g_nat_portmapping) );

    if ( pNatPMapping )
    {
         AnscCopyMemory(pNatPMapping, g_nat_portmapping, sizeof(g_nat_portmapping) );

        *pulCount = 2;
    }
    else
    {
        *pulCount = 0;
    }

    return pNatPMapping;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatAddPortMapping
            (
                ANSC_HANDLE                           hContext,
                PCOSA_DML_NAT_PMAPPING      pEntry
            )

    Description:
        The API adds one port mapping entry into NAT.

    Arguments:
        pEntry      Caller does not need to fill in Status or Alias fields. Upon return, callee fills in the generated Alias and associated Status.

    Return:
        he pointer to the array of NAT port mappings, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/
ANSC_STATUS
CosaDmlNatAddPortMapping
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_PMAPPING      pEntry
    )
{

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatDelPortMapping
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulInstanceNumber
            );

    Description:
        The API removes the designated port mapping entry from NAT.
    Arguments:
        pAlias      The entry is identified through Alias.
    Return:
        The pointer to the array of NAT port mappings, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/
ANSC_STATUS
CosaDmlNatDelPortMapping
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatGetPortMappings
            (
                ANSC_HANDLE                 hThisObject
            );

    Description:
        The API re-configures the designated port mapping entry.
    Arguments:
        pAlias      The entry is identified through Alias.
        pEntry      The new configuration is passed through this argument, even Alias field can be changed.

    Return:
    The pointer to the array of NAT port mappings, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/
ANSC_STATUS
CosaDmlNatSetPortMapping
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_PMAPPING      pEntry          /* Identified by InstanceNumber */
    )
{
    ULONG                          index = 0;

    for(index =0; index < sizeof(g_nat_portmapping)/sizeof(COSA_DML_NAT_PMAPPING); index++)
    {
        if ( g_nat_portmapping[index].InstanceNumber  ==  pEntry->InstanceNumber )
        {
            AnscCopyMemory(&g_nat_portmapping[index], pEntry, sizeof(COSA_DML_NAT_PMAPPING));

            break;
        }
    }

    return ANSC_STATUS_SUCCESS;

}

/* TBC      -- need to be normalized */
BOOL CosaDmlNatChkEnableFlg(PCOSA_DML_NAT_PMAPPING pPortMapping)
{
    return  TRUE;
}

BOOL CosaDmlNatChkPortMappingClient(ULONG client)
{
    return  TRUE;
}

/**********************************************************************

    caller:     self

    prototype:

        PCOSA_DML_NAT_PTRIGGER
        CosaDmlNatGetPortTriggers
            (
                ANSC_HANDLE                 hContext,
                PULONG                      pulCount,
		BOOLEAN                     bCommit
            )
        Description:
            This routine is to retrieve the complete list of NAT port triggers, which is a table.

        Arguments:
            pulCount  is to receive the actual number of entries.

        Return:
            The pointer to the array of NAT port triggers, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/

PCOSA_DML_NAT_PTRIGGER
CosaDmlNatGetPortTriggers
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
	BOOLEAN                     bCommit
    )
{
    *pulCount = 0;
    return NULL;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatAddPortTrigger
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_NAT_PTRIGGER      pEntry
            )

    Description:
        The API adds one port trigger entry into NAT.

    Arguments:
        pEntry      Caller does not need to fill in Status or Alias fields. Upon return, callee fills in the generated Alias and associated Status.

    Return:
        Status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlNatAddPortTrigger
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_PTRIGGER      pEntry
    )
{
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatDelPortTrigger
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_NAT_PTRIGGER      pEntry
            );

    Description:
        The API removes the designated port trigger entry from NAT.
    Arguments:
        pAlias      The entry is identified through Alias.
    Return:
        Status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlNatDelPortTrigger
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_PTRIGGER      pEntry
    )
{
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNatSetPortTrigger
            (
                ANSC_HANDLE                 hThisObject,
                PCOSA_DML_NAT_PTRIGGER      pEntry
            );

    Description:
        The API re-configures the designated port mapping entry.
    Arguments:
        pAlias      The entry is identified through Alias.
        pEntry      The new configuration is passed through this argument, even Alias field can be changed.

    Return:
        Status of the operation

**********************************************************************/

ANSC_STATUS
CosaDmlNatSetPortTrigger
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NAT_PTRIGGER      pEntry          /* Identified by InstanceNumber */
    )
{
    return ANSC_STATUS_SUCCESS;
}



static BOOL g_NatPTTriggerEnable = TRUE;

ANSC_STATUS
CosaDmlNatGetPortTriggerEnable(BOOL *pBool)
{
    *pBool = g_NatPTTriggerEnable;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlNatSetPortTriggerEnable(BOOL vBool)
{
    g_NatPTTriggerEnable = vBool;
    return ANSC_STATUS_SUCCESS;
}


int CosaDmlNatChkPortRange(ULONG InstanceNumber, BOOLEAN enabled, int start, int end, int protocol, int is_trigger)
{
    return TRUE;
}

int CosaDmlNatChkTriggerPortRange(ULONG InstanceNumber, BOOLEAN enabled, int start, int end, int protocol, int is_trigger)
{
    return TRUE;
}

ANSC_STATUS
CosaDmlNatGetPortMapping
    (
        ANSC_HANDLE                 hContext,
        ULONG                      InstanceNumber,
        PCOSA_DML_NAT_PMAPPING      pNatPMapping
    )
{
    memset(pNatPMapping, 0, sizeof(*pNatPMapping));
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlNatGetPortTrigger
    (
        ANSC_HANDLE                 hContext,
        ULONG                       InstanceNum,
        PCOSA_DML_NAT_PTRIGGER      pNatPTrigger
    )
{
    memset(pNatPTrigger, 0, sizeof(*pNatPTrigger));
    return ANSC_STATUS_SUCCESS;
}
 

BOOL CosaDmlNatChkPortMappingMaxRuleNum(PCOSA_DML_NAT_PMAPPING pEntry)
{
    return  TRUE;
}

int CosaDmlChkDesp(char *desp)
{
    if(desp == NULL ||
       desp[0] == '\0' || 
       desp[0] == ' ')
        return FALSE;
    return TRUE;
} 
