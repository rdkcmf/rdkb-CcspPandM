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

    module: cosa_neighdisc_apis.c

        For COSA Data Model Library Development

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
#include "cosa_apis.h"
#include "cosa_neighdisc_apis.h"
#include "cosa_neighdisc_internal.h"

/*
 *    Because we need interface's Alias to build connection request URL addr,
 *   so add these two definition together with _COSA_SIM_ temporarily.
 *   After finish backend integration, pls remove these two definition from here 
 *   like below:
 *   
 *
 */
#define NEIGHDISC_INTERFACE_MAX_NUM 6

COSA_DML_NEIGHDISC_IF_FULL g_neighdisc_interfaces[NEIGHDISC_INTERFACE_MAX_NUM] =
{
    {
        {1, "NeighdiscIf1", TRUE, "Interface.1", 1, 2, 3, TRUE, TRUE}, {COSA_DML_NEIGHDISC_IF_STATUS_Enabled}
    },
    {
        {2, "NeighdiscIf2", TRUE, "Interface.2", 4, 5, 6, TRUE, TRUE}, {COSA_DML_NEIGHDISC_IF_STATUS_Disabled}
    },
};

static ULONG g_NumOfInterfaces = 2;

static BOOLEAN g_neighdisc_enabled = TRUE ;

/*
 *  Neighdisc Interface
 */

/**********************************************************************

    caller:     self

    prototype:

        ULONG
        CosaDmlNeighdiscInit
            (
                ANSC_HANDLE                 hDml,
                PANSC_HANDLE                phContext
             );
    Description:
    
        The API initial the entry of NEIGHDISC interfaces in the system.

    Arguments:
                ANSC_HANDLE                 hDml

                PANSC_HANDLE                phContext

    Return:
                  The operation status..

**********************************************************************/
ANSC_STATUS
CosaDmlNeighdiscInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    return ANSC_STATUS_SUCCESS;
}


ULONG 
CosaDmlNeighdiscGetEnabled
    (
        BOOLEAN * pEnabled
    )
{
    *pEnabled = g_neighdisc_enabled;

    return 0;
}

ULONG 
CosaDmlNeighdiscSetEnabled
    (
        BOOLEAN  bEnabled
    )
{
    g_neighdisc_enabled = bEnabled;

    return 0;
}


/**********************************************************************

    caller:     self

    prototype:

        ULONG
        CosaDmlNeighdiscIfGetNumberOfEntries
            (
                ANSC_HANDLE                 hContext
             );
    Description:
    
        The API retrieves the number of NEIGHDISC interfaces in the system.

    Arguments:
                ANSC_HANDLE                 hContext

    Return:
                The number of entries in the Interface table.

**********************************************************************/
ULONG
CosaDmlNeighdiscIfGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    )
{
    return g_NumOfInterfaces;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNeighdiscIfGetEntry
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulIndex,
                PCOSA_DML_NEIGHDISC_IF_FULL        pEntry
             );

    Description:
           The API retrieves the complete info of the NEIGHDISC interface designated by index. 
           The usual process is the caller gets the total number of entries, then iterate 
           through those by calling this API.
    Arguments:
               ANSC_HANDLE                 hContext,
               
               ULONG                       ulIndex,
               
               PCOSA_DML_NEIGHDISC_IF_FULL        pEntry

    Return:
        The ulIndex entry of the table

**********************************************************************/

ANSC_STATUS
CosaDmlNeighdiscIfGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_NEIGHDISC_IF_FULL        pEntry
    )
{
    AnscCopyMemory(pEntry, &g_neighdisc_interfaces[ulIndex], sizeof(*pEntry));

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlNeighdiscIfSetValues
            (
                ANSC_HANDLE             hContext,
                ULONG                       ulIndex,
                ULONG                       ulInstanceNumber,
                char*                        pAlias
            );

    Description:

        Backend implementation to set back instance number and alias.

    Arguments:    
                   ANSC_HANDLE                 hContext

                   ULONG                           ulIndex

                   ULONG                           ulInstanceNumber

                   char*                             pAlias

    Return:       The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlNeighdiscIfSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    g_neighdisc_interfaces[ulIndex].Cfg.InstanceNumber = ulInstanceNumber;
    AnscCopyString(g_neighdisc_interfaces[ulIndex].Cfg.Alias, pAlias);

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNeighdiscIfAddEntry
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_NEIGHDISC_IF_FULL        pEntry
            );

    Description:
    
        The API adds one NEIGHDISC interface into the system. 
        
    Arguments:   
                ANSC_HANDLE                 hContext,
                
                PCOSA_DML_NEIGHDISC_IF_FULL        pEntry
    Return:
        he pointer to the array of NAT port mappings, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/
ANSC_STATUS
CosaDmlNeighdiscIfAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NEIGHDISC_IF_FULL        pEntry
    )
{
    if ( g_NumOfInterfaces >= NEIGHDISC_INTERFACE_MAX_NUM )
    {
        return ANSC_STATUS_FAILURE;
    }

    AnscCopyMemory(&g_neighdisc_interfaces[g_NumOfInterfaces], pEntry, sizeof(*pEntry));

    g_NumOfInterfaces++;

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlNeighdiscIfDelEntry
            (
                ANSC_HANDLE                 hContext,
               ULONG                            ulInstanceNumber
            );

    Description:
    
        The API delete one NEIGHDISC interface from the system. 
        
    Arguments:
                ANSC_HANDLE                 hContext,
                ULONG                           ulInstanceNumber        

    Return:
        The operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlNeighdiscIfDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
    ULONG                           i = 0;
    ULONG                           j = 0;

    for ( i = 0; i < g_NumOfInterfaces; i++ )
    {
        if ( ulInstanceNumber == g_neighdisc_interfaces[i].Cfg.InstanceNumber )
        {
            for ( j = i; j < g_NumOfInterfaces-1; j++ )
            {
                AnscCopyMemory
                (
                    &g_neighdisc_interfaces[j], 
                    &g_neighdisc_interfaces[j + 1], 
                    sizeof(COSA_DML_NEIGHDISC_IF_FULL)
                );

            }

            g_NumOfInterfaces--;

            return ANSC_STATUS_SUCCESS;;
        }
    }

    return ANSC_STATUS_CANT_FIND;
}

/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlNeighdiscIfSetCfg
          (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_NEIGHDISC_IF_CFG         pCfg
            );

    Description:
    
        The API the NEIGHDISC configuration. 
        
    Arguments:
                ANSC_HANDLE                 hContext,
                PCOSA_DML_NEIGHDISC_IF_CFG         pCfg

    Return:
        The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlNeighdiscIfSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NEIGHDISC_IF_CFG         pCfg
    )
{
    ULONG                           i = 0;

    for ( i = 0; i < g_NumOfInterfaces; i++)
    {
        if ( g_neighdisc_interfaces[i].Cfg.InstanceNumber == pCfg->InstanceNumber )
        {
            AnscCopyMemory(&g_neighdisc_interfaces[i].Cfg, pCfg, sizeof(*pCfg));
            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_CANT_FIND;   
}
/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlNeighdiscIfGetCfg
           (
                ANSC_HANDLE                 hContext,
                 PCOSA_DML_NEIGHDISC_IF_CFG         pCfg
           );

    Description:
    
         The API adds one NEIGHDISC interface into the system. 
         
    Arguments:
                ANSC_HANDLE                 hContext,
                
                PCOSA_DML_NEIGHDISC_IF_CFG         pCfg
                
    Return:
        The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlNeighdiscIfGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_NEIGHDISC_IF_CFG         pCfg
    )
{
    ULONG                           i = 0;

    for ( i = 0; i < g_NumOfInterfaces; i++)
    {
        if ( g_neighdisc_interfaces[i].Cfg.InstanceNumber == pCfg->InstanceNumber )
        {
            AnscCopyMemory(pCfg, &g_neighdisc_interfaces[i].Cfg, sizeof(*pCfg));
            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_CANT_FIND;   
}


/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlNeighdiscIfGetInfo
           (
                ANSC_HANDLE                 hContext,
                ULONG                           ulInstanceNumber,
                PCOSA_DML_NEIGHDISC_IF_INFO        pInfo
           );

    Description:
    
         The API adds one NEIGHDISC interface into the system. 
         
    Arguments:
                ANSC_HANDLE                 hContext,
                
                ULONG                           ulInstanceNumber,
                
                PCOSA_DML_NEIGHDISC_IF_INFO        pInfo

    Return:
        The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlNeighdiscIfGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_NEIGHDISC_IF_INFO        pInfo
    )
{
    ULONG                           i = 0;

    for ( i = 0; i < g_NumOfInterfaces; i++)
    {
        if ( g_neighdisc_interfaces[i].Cfg.InstanceNumber == ulInstanceNumber )
        {
            AnscCopyMemory(pInfo, &g_neighdisc_interfaces[i].Info, sizeof(*pInfo));
            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_CANT_FIND;   
}


ANSC_STATUS
CosaDmlNeighdiscIfBERemove
    (
        void
    )
{
    return ANSC_STATUS_SUCCESS;
}

void CosaDmlNeighborTableGetEntry
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulInstanceNumber,
        PCOSA_DML_NEIGHTABLE_INFO*  ppNbTbl
    )
{
    *pulInstanceNumber = 0;
    *ppNbTbl           = NULL;

    return;
}
