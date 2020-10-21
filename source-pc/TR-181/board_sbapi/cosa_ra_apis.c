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

    module: cosa_ra_apis.c

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
#include "cosa_ra_apis.h"
#include "cosa_ra_internal.h"
#include "secure_wrapper.h"

extern void* g_pDslhDmlAgent;

/*
 *    Because we need interface's Alias to build connection request URL addr,
 *   so add these two definition together with _COSA_SIM_ temporarily.
 *   After finish backend integration, pls remove these two definition from here 
 *   like below:
 *   #ifdef ( _COSA_SIM_ )
 *
 */
#if (defined(_COSA_SIM_)  ||   defined(_COSA_DRG_TPG_) )
#define RA_INTERFACE_MAX_NUM 10

COSA_DML_RA_IF_FULL2 g_ra_interfaces[RA_INTERFACE_MAX_NUM] =
{
    {
        {1, "RaIf1", TRUE, "Interface.1", "ManualPrefix.1", 1, 2, 3, TRUE, TRUE, TRUE, COSA_DML_RA_PREFER_ROUTER_High, FALSE, 4,5,6,7}, 
        {COSA_DML_RA_IF_STATUS_Enabled, "Prefix"}, {{NULL}, 0, 0}, 1
    },
    {
        {2, "RaIf2", TRUE, "Interface.2", "ManualPrefix.2", 1, 2, 3, TRUE, TRUE, TRUE, COSA_DML_RA_PREFER_ROUTER_Low, FALSE, 4,5,6,7}, 
        {COSA_DML_RA_IF_STATUS_Error, "Prefix"}, {{NULL}, 0, 0}, 1
    },
};

static ULONG g_NumOfInterfaces = 2;

static BOOLEAN g_ra_enabled = TRUE ;

/*sometimes it's not safe to call CosaGetParamValueString, you need to reset value_size, use this wrapper*/
static int _safe_CosaGetParamValueString(char * name, char * value, int size)
{
    int value_len = size;
        
    return CosaGetParamValueString(name, value, (PULONG)&value_len);
}

/*
 *  RA Interface
 */

/**********************************************************************

    caller:     self

    prototype:

        ULONG
        CosaDmlRAInit
            (
                ANSC_HANDLE                 hDml,
                PANSC_HANDLE                phContext
             );
    Description:
    
        The API initial the entry of RA interfaces in the system.

    Arguments:
                ANSC_HANDLE                 hDml

                PANSC_HANDLE                phContext

    Return:
                  The operation status..

**********************************************************************/

ANSC_STATUS
CosaDmlRACallback
    (
        ANSC_HANDLE                 hContext
    )
{
    char value[256] = {0};
    int value_len = 256;
    int ret = 0;

    ret = _safe_CosaGetParamValueString("Device.IP.Interface.1.Name", value, sizeof(value));
    printf("test for CosaDmlRACallback ret:value %d:%s\n\n", ret, value);
    
    return 0;
}


ANSC_STATUS
CosaDmlRAInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    DSLHDMAGNT_CALLBACK *  pEntry = NULL;
    
    pEntry = AnscAllocateMemory(sizeof(*pEntry));

    pEntry->func = CosaDmlRACallback;
    
    g_RegisterCallBackAfterInitDml(g_pDslhDmlAgent, pEntry);

    return ANSC_STATUS_SUCCESS;
}


ULONG 
CosaDmlRAGetEnabled
    (
        BOOLEAN * pEnabled
    )
{
    *pEnabled = g_ra_enabled;

    return 0;
}

ULONG 
CosaDmlRASetEnabled
    (
        BOOLEAN  bEnabled
    )
{
    g_ra_enabled = bEnabled;

    return 0;
}


/**********************************************************************

    caller:     self

    prototype:

        ULONG
        CosaDmlRaIfGetNumberOfEntries
            (
                ANSC_HANDLE                 hContext
             );
    Description:
    
        The API retrieves the number of RA interfaces in the system.

    Arguments:
                ANSC_HANDLE                 hContext

    Return:
                The number of entries in the Interface table.

**********************************************************************/
ULONG
CosaDmlRaIfGetNumberOfEntries
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
        CosaDmlRaIfGetEntry
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulIndex,
                PCOSA_DML_RA_IF_FULL        pEntry
             );

    Description:
           The API retrieves the complete info of the RA interface designated by index. 
           The usual process is the caller gets the total number of entries, then iterate 
           through those by calling this API.
    Arguments:
               ANSC_HANDLE                 hContext,
               
               ULONG                       ulIndex,
               
               PCOSA_DML_RA_IF_FULL        pEntry

    Return:
        The ulIndex entry of the table

**********************************************************************/

ANSC_STATUS
CosaDmlRaIfGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_RA_IF_FULL        pEntry
    )
{
    AnscCopyMemory(pEntry, &g_ra_interfaces[ulIndex], sizeof(*pEntry));

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlRaIfSetValues
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
CosaDmlRaIfSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    g_ra_interfaces[ulIndex].Cfg.InstanceNumber = ulInstanceNumber;
    AnscCopyString(g_ra_interfaces[ulIndex].Cfg.Alias, pAlias);

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRaIfAddEntry
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_RA_IF_FULL        pEntry
            );

    Description:
    
        The API adds one RA interface into the system. 
        
    Arguments:   
                ANSC_HANDLE                 hContext,
                
                PCOSA_DML_RA_IF_FULL        pEntry
    Return:
        he pointer to the array of NAT port mappings, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/
ANSC_STATUS
CosaDmlRaIfAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RA_IF_FULL        pEntry
    )
{
    if ( g_NumOfInterfaces >= RA_INTERFACE_MAX_NUM )
    {
        return ANSC_STATUS_FAILURE;
    }
    
    AnscCopyMemory(&g_ra_interfaces[g_NumOfInterfaces], pEntry, sizeof(*pEntry));
    AnscSListInitializeHeader(&g_ra_interfaces[g_NumOfInterfaces].OptionList);
    g_ra_interfaces[g_NumOfInterfaces].ulNextOptionInsNum = 1;

    g_NumOfInterfaces++;

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRaIfDelEntry
            (
                ANSC_HANDLE                 hContext,
               ULONG                            ulInstanceNumber
            );

    Description:
    
        The API delete one RA interface from the system. 
        
    Arguments:
                ANSC_HANDLE                 hContext,
                ULONG                           ulInstanceNumber        

    Return:
        The operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlRaIfDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
    ULONG                           i = 0;
    ULONG                           j = 0;

    for ( i = 0; i < g_NumOfInterfaces; i++ )
    {
        if ( ulInstanceNumber == g_ra_interfaces[i].Cfg.InstanceNumber )
        {
            for ( j = i; j < g_NumOfInterfaces-1; j++ )
            {
                AnscCopyMemory
                (
                    &g_ra_interfaces[j], 
                    &g_ra_interfaces[j + 1], 
                    sizeof(COSA_DML_RA_IF_FULL2)
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
       CosaDmlRaIfSetCfg
          (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_RA_IF_CFG         pCfg
            );

    Description:
    
        The API the RA configuration. 
        
    Arguments:
                ANSC_HANDLE                 hContext,
                PCOSA_DML_RA_IF_CFG         pCfg

    Return:
        The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlRaIfSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RA_IF_CFG         pCfg
    )
{
    ULONG                           i = 0;

    for ( i = 0; i < g_NumOfInterfaces; i++)
    {
        if ( g_ra_interfaces[i].Cfg.InstanceNumber == pCfg->InstanceNumber )
        {
            AnscCopyMemory(&g_ra_interfaces[i].Cfg, pCfg, sizeof(*pCfg));
            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_CANT_FIND;   
}
/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlRaIfGetCfg
           (
                ANSC_HANDLE                 hContext,
                 PCOSA_DML_RA_IF_CFG         pCfg
           );

    Description:
    
         The API adds one RA interface into the system. 
         
    Arguments:
                ANSC_HANDLE                 hContext,
                
                PCOSA_DML_RA_IF_CFG         pCfg
                
    Return:
        The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlRaIfGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RA_IF_CFG         pCfg
    )
{
    ULONG                           i = 0;

    for ( i = 0; i < g_NumOfInterfaces; i++)
    {
        if ( g_ra_interfaces[i].Cfg.InstanceNumber == pCfg->InstanceNumber )
        {
            AnscCopyMemory(pCfg, &g_ra_interfaces[i].Cfg, sizeof(*pCfg));
            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_CANT_FIND;   
}


/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlRaIfGetInfo
           (
                ANSC_HANDLE                 hContext,
                ULONG                           ulInstanceNumber,
                PCOSA_DML_RA_IF_INFO        pInfo
           );

    Description:
    
         The API adds one RA interface into the system. 
         
    Arguments:
                ANSC_HANDLE                 hContext,
                
                ULONG                           ulInstanceNumber,
                
                PCOSA_DML_RA_IF_INFO        pInfo

    Return:
        The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlRaIfGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_RA_IF_INFO        pInfo
    )
{
    ULONG                           i = 0;

    for ( i = 0; i < g_NumOfInterfaces; i++)
    {
        if ( g_ra_interfaces[i].Cfg.InstanceNumber == ulInstanceNumber )
        {
            AnscCopyMemory(pInfo, &g_ra_interfaces[i].Info, sizeof(*pInfo));
            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_CANT_FIND;   
}

/*
 *  RA Interface Options
 */
/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlRaIfGetNumberOfOptions
           (
                ANSC_HANDLE                 hContext,
                ULONG                           ulRaIfInstanceNumber,
           );

    Description:
    
        The API adds one RA interface into the system. 
        
    Arguments:
                ANSC_HANDLE                 hContext,
                
                ULONG                       ulRaIfInstanceNumber

    Return:
        The number of Options

**********************************************************************/
ULONG
CosaDmlRaIfGetNumberOfOptions
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulRaIfInstanceNumber
    )
{
    ULONG                           i = 0;

    for ( i = 0; i < g_NumOfInterfaces; i++ )
    {
        if ( ulRaIfInstanceNumber == g_ra_interfaces[i].Cfg.InstanceNumber )
        {
            return AnscSListQueryDepth(&g_ra_interfaces[i].OptionList);
        }
    }

    return 0;
}

/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlRaIfGetOption
           (
                ANSC_HANDLE                 hContext,
                ULONG                       ulRaIfInstanceNumber,
                ULONG                       ulIndex,
                PCOSA_DML_RA_OPTION         pEntry
           );

    Description:
        
        The API adds one RA interface into the system. 
        
    Arguments:
    
        ANSC_HANDLE                 hContext,
        
        ULONG                       ulRaIfInstanceNumber,
        
        ULONG                       ulIndex,
        
        PCOSA_DML_RA_OPTION         pEntry

    Return:
        The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlRaIfGetOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulRaIfInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_RA_OPTION         pEntry
    )
{
    ULONG                           i = 0;
    PSINGLE_LINK_ENTRY              pSListEntry = NULL;
    PCOSA_DML_RA_OPTION             pOption = NULL;

    for ( i = 0; i < g_NumOfInterfaces; i++ )
    {
        if ( ulRaIfInstanceNumber == g_ra_interfaces[i].Cfg.InstanceNumber )
        {
            pSListEntry =
                AnscSListGetEntryByIndex(&g_ra_interfaces[i].OptionList, ulIndex);
            if (pSListEntry)
            {
                pOption = ACCESS_RA_OPTION_LINK_OBJECT(pSListEntry);
                AnscCopyMemory(pEntry, pOption, sizeof(*pEntry));
                return ANSC_STATUS_SUCCESS;
            }
        }
    }

    return ANSC_STATUS_CANT_FIND;
}

/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlRaIfSetOptionValues
           (
                ANSC_HANDLE                 hContext,
                ULONG                       ulRaIfInstanceNumber,
                ULONG                       ulIndex,
                ULONG                       ulInstanceNumber,
                char*                       pAlias
            );

    Description:
        
        The API set one RA interface into the system. 
        
    Arguments:
    
        ANSC_HANDLE                 hContext,
        
        ULONG                       ulRaIfInstanceNumber,
        
        ULONG                       ulIndex,

        ULONG                       ulInstanceNumber,
        
        char*                       pAlias

    Return:
        The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlRaIfSetOptionValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulRaIfInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    ULONG                           i = 0;
    PSINGLE_LINK_ENTRY              pSListEntry = NULL;
    PCOSA_DML_RA_OPTION             pOption = NULL;


    for ( i = 0; i < g_NumOfInterfaces; i++ )
    {
        if ( ulRaIfInstanceNumber == g_ra_interfaces[i].Cfg.InstanceNumber )
        {
            pSListEntry =
                AnscSListGetEntryByIndex(&g_ra_interfaces[i].OptionList, ulIndex);

            if (pSListEntry)
            {
                pOption = ACCESS_RA_OPTION_LINK_OBJECT(pSListEntry);            

                pOption->InstanceNumber = ulInstanceNumber;
                AnscCopyString(pOption->Alias, pAlias);
                
                return ANSC_STATUS_SUCCESS;
            }
        }
    }

    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlRaIfAddOption
           (
                ANSC_HANDLE                 hContext,
                ULONG                           ulRaIfInstanceNumber,
                PCOSA_DML_RA_OPTION     pEntry
           );

    Description:
    
        The API adds one RA interface into the system. 
        
    Arguments:
                ANSC_HANDLE                 hContext,

                ULONG                            ulRaIfInstanceNumber,
                
                PCOSA_DML_RA_OPTION         pEntry

    Return:
        The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlRaIfAddOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulRaIfInstanceNumber,
        PCOSA_DML_RA_OPTION         pEntry
    )
{
    ULONG                           i = 0;
    PCOSA_DML_RA_OPTION             pOption = NULL;


    for ( i = 0; i < g_NumOfInterfaces; i++ )
    {
        if ( ulRaIfInstanceNumber == g_ra_interfaces[i].Cfg.InstanceNumber )
        {
            pOption = (PCOSA_DML_RA_OPTION)AnscAllocateMemory(sizeof(*pOption));

            if (!pOption)
                return ANSC_STATUS_FAILURE;

            AnscCopyMemory(pOption, pEntry, sizeof(*pOption));

            AnscSListPushEntryAtBack(&g_ra_interfaces[i].OptionList, &pOption->Link);            

            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_CANT_FIND;
}

/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlRaIfDelOption
           (
                ANSC_HANDLE                 hContext,
                ULONG                           ulRaIfInstanceNumber,
                PCOSA_DML_RA_OPTION    pEntry

           );

    Description:
    
        The API adds one RA interface into the system. 
        
    Arguments:
                ANSC_HANDLE                 hContext,
                
                ULONG                                ulRaIfInstanceNumber,
                
                PCOSA_DML_RA_OPTION         pEntry

    Return:
        The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlRaIfDelOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulRaIfInstanceNumber,
        PCOSA_DML_RA_OPTION         pEntry
    )
{
    ULONG                           i = 0;
    PSINGLE_LINK_ENTRY              pLink           = (PSINGLE_LINK_ENTRY )NULL;
    PCOSA_DML_RA_OPTION             pOption = NULL;

    for ( i = 0; i < g_NumOfInterfaces; i++ )
    {
        if ( ulRaIfInstanceNumber == g_ra_interfaces[i].Cfg.InstanceNumber )
        {
            
            pLink = AnscSListGetFirstEntry(&g_ra_interfaces[i].OptionList);

            while ( pLink )
            {
                pOption = ACCESS_RA_OPTION_LINK_OBJECT(pLink);

                
                if (pOption->InstanceNumber == pEntry->InstanceNumber)
                {
                    AnscSListPopEntryByLink(&g_ra_interfaces[i].OptionList, pLink);                    
                    AnscFreeMemory(pOption);
                    break;
                }
                
                pLink   = AnscSListGetNextEntry(pLink);

            }
            
        }
    }

    return ANSC_STATUS_CANT_FIND;
}

/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlRaIfSetOption
           (
                ANSC_HANDLE                 hContext,
                ULONG                           ulRaIfInstanceNumber,
                PCOSA_DML_RA_OPTION         pEntry
           );

    Description:
    
        The API adds one RA interface into the system. 
        
    Arguments:
                ANSC_HANDLE                 hContext,
               ULONG                            ulRaIfInstanceNumber,
                PCOSA_DML_RA_OPTION         pEntry

    Return:
        The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlRaIfSetOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulRaIfInstanceNumber,
        PCOSA_DML_RA_OPTION         pEntry
    )
{
    ULONG                           i       = 0;
    PSINGLE_LINK_ENTRY              pLink           = (PSINGLE_LINK_ENTRY )NULL;
    PCOSA_DML_RA_OPTION             pOption = NULL;

    for ( i = 0; i < g_NumOfInterfaces; i++ )
    {
        if ( ulRaIfInstanceNumber == g_ra_interfaces[i].Cfg.InstanceNumber )
        {

            pLink = AnscSListGetFirstEntry(&g_ra_interfaces[i].OptionList);

            while ( pLink )
            {
                pOption = ACCESS_RA_OPTION_LINK_OBJECT(pLink);
                
                if (pOption->InstanceNumber == pEntry->InstanceNumber)
                {
                    AnscCopyMemory(pOption, pEntry, sizeof(*pEntry));
                    return ANSC_STATUS_SUCCESS;
                }
                
                pLink   = AnscSListGetNextEntry(pLink);

            }
        }
    }

    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlRaIfGetOption2
           (
            ANSC_HANDLE                 hContext,
            ULONG                           ulRaIfInstanceNumber,
            PCOSA_DML_RA_OPTION         pEntry        
        );

    Description:
        
        The API adds one RA interface into the system. 
        
    Arguments:
    
        ANSC_HANDLE                 hContext,
        
        ULONG                       ulRaIfInstanceNumber,
        
        ULONG                       ulIndex,
        
        PCOSA_DML_RA_OPTION         pEntry

    Return:
        The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlRaIfGetOption2
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulRaIfInstanceNumber,
        PCOSA_DML_RA_OPTION         pEntry         
    )
{
    ULONG                           i = 0;
    PSINGLE_LINK_ENTRY              pLink           = (PSINGLE_LINK_ENTRY )NULL;
    PCOSA_DML_RA_OPTION             pOption = NULL;

    for ( i = 0; i < g_NumOfInterfaces; i++ )
    {
        if ( ulRaIfInstanceNumber == g_ra_interfaces[i].Cfg.InstanceNumber )
        {
            pLink = AnscSListGetFirstEntry(&g_ra_interfaces[i].OptionList);

            while ( pLink )
            {
                pOption = ACCESS_RA_OPTION_LINK_OBJECT(pLink);
                
                if (pOption->InstanceNumber == pEntry->InstanceNumber)
                {
                    AnscCopyMemory(pEntry, pOption, sizeof(*pEntry));
                    return ANSC_STATUS_SUCCESS;
                }
                
                pLink   = AnscSListGetNextEntry(pLink);

            }
        }
    }

    return ANSC_STATUS_CANT_FIND;
}

ANSC_STATUS
CosaDmlRaIfBERemove
    (
        void
    )
{
    PSINGLE_LINK_ENTRY              pLink           = (PSINGLE_LINK_ENTRY )NULL;
    PCOSA_DML_RA_OPTION             pOption = NULL;
    

    return ANSC_STATUS_SUCCESS;
}


#elif (defined(_COSA_DRG_CNS_) )
#include <net/if.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <libgen.h>
#include <utapi.h>
#include <utapi_util.h>
#include "utctx/utctx_api.h"
#include "syscfg/syscfg.h"
#include "cosa_ip_apis.h"

#define SYSCFG_FORMAT_RA "tr_RouterAdvertise"
#define SYSCFG_FORMAT_RA_OPTION "tr_RouterAdvertise_option"
#define RADVD_BIN "/usr/sbin/radvd"

int safe_strcpy(char * dst, char * src, int dst_size);

COSA_DML_RA_IF_FULL2 g_ra_interface;

/*if set this flag to FALSE, radvd won't start*/
static BOOLEAN g_ra_enabled = TRUE ;
static int _radvd_operation(char * arg);
static int _gen_radvd_conf(COSA_DML_RA_IF_CFG * pCfg);
static int _gen_ra_option_file(SLIST_HEADER * p_list);

/*sometimes it's not safe to call CosaGetParamValueString, you need to reset value_size, use this wrapper*/
static int _safe_CosaGetParamValueString(char * name, char * value, int size)
{
    int value_len = size;
        
    return CosaGetParamValueString(name, value, (PULONG)&value_len);
}

/*
 *  RA Interface
 */

/**********************************************************************

    caller:     self

    prototype:

        ULONG
        CosaDmlRAInit
            (
                ANSC_HANDLE                 hDml,
                PANSC_HANDLE                phContext
             );
    Description:
    
        The API initial the entry of RA interfaces in the system.

    Arguments:
                ANSC_HANDLE                 hDml

                PANSC_HANDLE                phContext

    Return:
                  The operation status..

**********************************************************************/

ANSC_STATUS
CosaDmlRACallback
    (
        ANSC_HANDLE                 hContext
    )
{
    if (g_ra_enabled && g_ra_interface.Cfg.bEnabled)
    {
        _gen_radvd_conf(&g_ra_interface.Cfg);
        _gen_ra_option_file(&g_ra_interface.OptionList);
        _radvd_operation("restart");
    }
    
    return 0;
}

ANSC_STATUS
CosaDmlRAInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    DSLHDMAGNT_CALLBACK *  pEntry = NULL;
    
    pEntry = AnscAllocateMemory(sizeof(*pEntry));

    pEntry->func = CosaDmlRACallback;
    
    g_RegisterCallBackAfterInitDml(g_pDslhDmlAgent, pEntry);

    AnscSListInitializeHeader(&g_ra_interface.OptionList);

    return ANSC_STATUS_SUCCESS;
}

ULONG 
CosaDmlRAGetEnabled
    (
        BOOLEAN * pEnabled
    )
{
    UtopiaContext utctx = {0};
    char out[256] = {0};

    if (!Utopia_Init(&utctx))
        return ANSC_STATUS_FAILURE;

    memset(out, 0, sizeof(out));
    Utopia_RawGet(&utctx,NULL,SYSCFG_FORMAT_RA"_all_enabled",out,sizeof(out));
    
    g_ra_enabled = (out[0] == '1') ? TRUE:FALSE;

    Utopia_Free(&utctx,0);                    

    *pEnabled = g_ra_enabled;

    return 0;
}


ULONG 
CosaDmlRASetEnabled
    (
        BOOLEAN  bEnabled
    )
{
    UtopiaContext utctx = {0};
    char out[256] = {0};

    if (g_ra_enabled != bEnabled)
    {
        if (!bEnabled )
            _radvd_operation("stop");
        else if (g_ra_interface.Cfg.bEnabled)
            _radvd_operation("restart");
    }
    
    if (!Utopia_Init(&utctx))
        return ANSC_STATUS_FAILURE;

    memset(out, 0, sizeof(out));
    out[0] = bEnabled ? '1':'0';
    
    Utopia_RawSet(&utctx,NULL,SYSCFG_FORMAT_RA"_all_enabled",out);
    
    Utopia_Free(&utctx,1);                    

    g_ra_enabled = bEnabled;

    return 0;
}


/**********************************************************************

    caller:     self

    prototype:

        ULONG
        CosaDmlRaIfGetNumberOfEntries
            (
                ANSC_HANDLE                 hContext
             );
    Description:
    
        The API retrieves the number of RA interfaces in the system.

    Arguments:
                ANSC_HANDLE                 hContext

    Return:
                The number of entries in the Interface table.

**********************************************************************/
ULONG
CosaDmlRaIfGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    )
{
    return 1;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRaIfGetEntry
            (
                ANSC_HANDLE                 hContext,
                ULONG                       ulIndex,
                PCOSA_DML_RA_IF_FULL        pEntry
             );

    Description:
           The API retrieves the complete info of the RA interface designated by index. 
           The usual process is the caller gets the total number of entries, then iterate 
           through those by calling this API.
    Arguments:
               ANSC_HANDLE                 hContext,
               
               ULONG                       ulIndex,
               
               PCOSA_DML_RA_IF_FULL        pEntry

    Return:
        The ulIndex entry of the table

**********************************************************************/

#define RADVD_CONF_FILE "/tmp/radvd/radvd.conf"
#define RADVD_CONF_FILE_TMP "/tmp/radvd/radvd.conf.tmp"
#define RADVD_OPTIONS_FILE "/tmp/radvd/radvd.options"

#define RADVD_IFNAME "brlan0"

static int _radvd_operation(char * arg)
{
    char out[256] = {0};

    if (!strncmp(arg, "stop", 4))
    {
        v_secure_system("killall %s", basename(RADVD_BIN)); 
        sleep(2);      
        v_secure_system("killall -9 %s", basename(RADVD_BIN));
    }
    else if (!strncmp(arg, "start", 5))
    {
        v_secure_system(RADVD_BIN " -C " RADVD_CONF_FILE);
    }
    else if (!strncmp(arg, "restart", 7))
    {
        _radvd_operation("stop");
        _radvd_operation("start");
    }

    return 0;
}

/*out will be this format:
  2001:edff:fe6a:f76::/64,2002:edff:fe6a:f76::/64
 or empty,
*/

#define _COLLECT_VALUE  \
    /*don't forget ',' after each Prefix*/ \
    if (strlen(out)+strlen(value)+1 > out_len-1 ) \
    { \
        out_len = strlen(out) + strlen(value) +1+1; \
        out = realloc(out, out_len); \
        if (!out) \
            goto EXIT; \
    }                       \
    strcat(out, value);     \
    strcat(out, ",");       


static int _get_ra_prefixes(COSA_DML_RA_IF_CFG * pCfg, char ** ppout)
{
    char value[256] = {0};
    char name[256] = {0};
    char * saveptr = NULL;
    char * str = NULL;
    char * p_token = NULL;
    COSA_DML_RA_OPTION * pEntry = NULL;
    int    value_len = sizeof(value);
    char * out = NULL;
    PUCHAR  pString = NULL;
    ULONG ulNumOfEntries = 0, ulEntryInstanceNum = 0;
    int   out_len = 0;
    int   i = 0;
    int ret = 0;
    char * running = NULL;
    
    out_len = 1024;
    out = calloc(1, out_len);
    if (!out)
        return -1;

    /*ManualPrefixes format is:
      "IP.Interface.{i}.IPv6Prefix.1.,IP.Interface.{i}.IPv6Prefix.2. "*/
    running = strdup(pCfg->ManualPrefixes);
    if (!running)
        return -1;

    for (str = running; ; str = NULL) 
    {
        p_token = strtok_r(str, ",", &saveptr);
        if (p_token == NULL)
            break;

        safe_strcpy(name, p_token, sizeof(name));
        strncat(name, "StaticType", sizeof(name)-1-strlen(name));

        /*type should be static or child*/
        value_len = sizeof(value);
        if (_safe_CosaGetParamValueString(name, value, sizeof(value)) || 
            (!AnscEqualString(value, "Static", TRUE) && !AnscEqualString(value, "Child", TRUE)))
            continue;

        snprintf(name, sizeof(name)-1, "%sPrefix", p_token);
        memset(value, 0, sizeof(value));
        printf("dz before get %s\n\n", name);
        value_len = sizeof(value);
        if (ret = _safe_CosaGetParamValueString(name, value, sizeof(value)))
            continue;

        _COLLECT_VALUE;
    }

EXIT:
    *ppout= out;
    free(running);

    return 0;
}
#undef _COLLECT_VALUE
/*interface brlan0
{
        AdvSendAdvert on;
        
        prefix prefix/length {
            list of prefix specific options
       };

}
*/
static int _gen_radvd_conf(COSA_DML_RA_IF_CFG * pCfg)
{
    FILE * fp = fopen(RADVD_CONF_FILE_TMP, "w+");
    char * p_out = NULL;
    char * str = NULL;
    char * p_token = NULL;
    char * saveptr = NULL;

    if (!fp)
        return -1;

    fprintf(fp, "interface %s\n", RADVD_IFNAME);

    fprintf(fp, "{\n");

    fprintf(fp, "    MaxRtrAdvInterval %d;\n",  pCfg->MaxRtrAdvInterval);
    fprintf(fp, "    MinRtrAdvInterval %d;\n",  pCfg->MinRtrAdvInterval);
    fprintf(fp, "    AdvDefaultLifetime %d;\n", pCfg->AdvDefaultLifetime);
    fprintf(fp, "    AdvLinkMTU %d;\n",         pCfg->AdvLinkMTU);
    fprintf(fp, "    AdvReachableTime %d;\n",   pCfg->AdvReachableTime);
    fprintf(fp, "    AdvRetransTimer %d;\n",    pCfg->AdvRetransTimer);
    fprintf(fp, "    AdvCurHopLimit %d;\n",     pCfg->AdvCurHopLimit);

    if(pCfg->bAdvManagedFlag)
        fprintf(fp, "    AdvManagedFlag on;\n");
    else 
        fprintf(fp, "    AdvManagedFlag off;\n");

    if(pCfg->bAdvOtherConfigFlag)
        fprintf(fp, "    AdvOtherConfigFlag on;\n");
    else 
        fprintf(fp, "    AdvOtherConfigFlag off;\n");

    if(pCfg->bAdvMobileAgentFlag)
        fprintf(fp, "    AdvHomeAgentFlag on;\n");
    else 
        fprintf(fp, "    AdvHomeAgentFlag off;\n");
    
    /*bAdvNDProxyFlag not supported by radvd*/


    if (pCfg->AdvPreferredRouterFlag == COSA_DML_RA_PREFER_ROUTER_High)
        fprintf(fp, "    AdvDefaultPreference high;\n");
    else if (pCfg->AdvPreferredRouterFlag == COSA_DML_RA_PREFER_ROUTER_Low)
        fprintf(fp, "    AdvDefaultPreference low;\n");
    else
        fprintf(fp, "    AdvDefaultPreference medium;\n");

    /*the biggest part: prefix*/
    _get_ra_prefixes(pCfg, &p_out);    

    if (p_out && p_out[0])
    {
    
        for (str = p_out; ; str = NULL) 
        {
            p_token = strtok_r(str, ",", &saveptr);
            if (p_token == NULL)
                break;

            /*even our ipv6 prefix is not in consice format, radvd can still work, so don't convert it.*/
            fprintf(fp, "    prefix %s {\n", p_token);
            fprintf(fp, "    };\n");
        }    
        free(p_out);
    }

    fprintf(fp, "};\n");

    if (rename(RADVD_CONF_FILE_TMP, RADVD_CONF_FILE))
        CcspTraceWarning(("%s rename failed %s\n", __FUNCTION__, strerror(errno)));

    /*to make radvd happy*/
    chmod(RADVD_CONF_FILE, S_IRUSR|S_IWUSR);

EXIT:
    fclose(fp);    

    return 0;
}


/*this function will generate option info file to radvd,
 the format of this file:
    option-type:option-len(binary length):option-data*/
static int _gen_ra_option_file(SLIST_HEADER * p_list)
{
    FILE * fp = fopen(RADVD_OPTIONS_FILE, "w+");
    int  i = 0;
    
    if (fp)
    {
        PSINGLE_LINK_ENTRY              pLink = NULL;
        PCOSA_DML_RA_OPTION             pOption = NULL;

        pLink = AnscSListGetFirstEntry(&g_ra_interface.OptionList);

        while ( pLink )
        {
            pOption = ACCESS_RA_OPTION_LINK_OBJECT(pLink);

            if (pOption->bEnabled)
                fprintf(fp, "%d:%d:%s\n", 
                        pOption->Tag,
                        strlen(pOption->Value)/2,
                        pOption->Value);
            
            pLink   = AnscSListGetNextEntry(pLink);
        }

        fclose(fp);
    }

    return 0;
}


ANSC_STATUS
CosaDmlRaIfGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_RA_IF_FULL        pEntry
    )
{
    UtopiaContext utctx = {0};
    char out[256] = {0};
    UCHAR * pString = NULL;


    if (ulIndex)
        return ANSC_STATUS_FAILURE;

    /*handle syscfg stuff*/
    if (!Utopia_Init(&utctx))
        return ANSC_STATUS_FAILURE;

    memset(out, 0, sizeof(out));
    Utopia_RawGet(&utctx,NULL,SYSCFG_FORMAT_RA"_inst_num",out,sizeof(out));
    if (out[0])
        g_ra_interface.Cfg.InstanceNumber = atoi(out);

    Utopia_RawGet(&utctx,NULL,SYSCFG_FORMAT_RA"_alias",g_ra_interface.Cfg.Alias,sizeof(g_ra_interface.Cfg.Alias));
    
    memset(out, 0, sizeof(out));
    Utopia_RawGet(&utctx,NULL,SYSCFG_FORMAT_RA"_enabled",out,sizeof(out));
    g_ra_interface.Cfg.bEnabled = (out[0] == '1') ? TRUE:FALSE;

    /*set default values*/
    g_ra_interface.Cfg.MaxRtrAdvInterval = 600;
    g_ra_interface.Cfg.MinRtrAdvInterval = 200;
    g_ra_interface.Cfg.AdvDefaultLifetime = 1800;
    g_ra_interface.Cfg.AdvPreferredRouterFlag = COSA_DML_RA_PREFER_ROUTER_Medium;
    
    Utopia_RawGet(&utctx,NULL,SYSCFG_FORMAT_RA"_ManualPrefixes",g_ra_interface.Cfg.ManualPrefixes, sizeof(g_ra_interface.Cfg.ManualPrefixes));

    memset(out, 0, sizeof(out));
    Utopia_RawGet(&utctx,NULL,SYSCFG_FORMAT_RA"_MaxRtrAdvInterval",out,sizeof(out));
    if (out[0])
        g_ra_interface.Cfg.MaxRtrAdvInterval = atoi(out);

    memset(out, 0, sizeof(out));
    Utopia_RawGet(&utctx,NULL,SYSCFG_FORMAT_RA"_MinRtrAdvInterval",out,sizeof(out));
    if (out[0])
        g_ra_interface.Cfg.MaxRtrAdvInterval = atoi(out);

    memset(out, 0, sizeof(out));
    Utopia_RawGet(&utctx,NULL,SYSCFG_FORMAT_RA"_AdvDefaultLifetime",out,sizeof(out));
    if (out[0])
        g_ra_interface.Cfg.AdvDefaultLifetime = atoi(out);

    memset(out, 0, sizeof(out));
    Utopia_RawGet(&utctx,NULL,SYSCFG_FORMAT_RA"_AdvLinkMTU",out,sizeof(out));
    if (out[0])
        g_ra_interface.Cfg.AdvLinkMTU = atoi(out);

    memset(out, 0, sizeof(out));
    Utopia_RawGet(&utctx,NULL,SYSCFG_FORMAT_RA"_AdvReachableTime",out,sizeof(out));
    if (out[0])
        g_ra_interface.Cfg.AdvReachableTime = atoi(out);

    memset(out, 0, sizeof(out));
    Utopia_RawGet(&utctx,NULL,SYSCFG_FORMAT_RA"_AdvRetransTimer",out,sizeof(out));
    if (out[0])
        g_ra_interface.Cfg.AdvRetransTimer = atoi(out);

    memset(out, 0, sizeof(out));
    Utopia_RawGet(&utctx,NULL,SYSCFG_FORMAT_RA"_AdvCurHopLimit",out,sizeof(out));
    if (out[0])
        g_ra_interface.Cfg.AdvCurHopLimit = atoi(out);
    
    memset(out, 0, sizeof(out));
    Utopia_RawGet(&utctx,NULL,SYSCFG_FORMAT_RA"_AdvManagedFlag",out,sizeof(out));
    g_ra_interface.Cfg.bAdvManagedFlag = (out[0] == '1') ? TRUE:FALSE;

    memset(out, 0, sizeof(out));
    Utopia_RawGet(&utctx,NULL,SYSCFG_FORMAT_RA"_AdvOtherConfigFlag",out,sizeof(out));
    g_ra_interface.Cfg.bAdvOtherConfigFlag = (out[0] == '1') ? TRUE:FALSE;

    memset(out, 0, sizeof(out));
    Utopia_RawGet(&utctx,NULL,SYSCFG_FORMAT_RA"_AdvMobileAgentFlag",out,sizeof(out));
    g_ra_interface.Cfg.bAdvMobileAgentFlag = (out[0] == '1') ? TRUE:FALSE;

    memset(out, 0, sizeof(out));
    Utopia_RawGet(&utctx,NULL,SYSCFG_FORMAT_RA"_AdvNDProxyFlag",out,sizeof(out));
    g_ra_interface.Cfg.bAdvNDProxyFlag = (out[0] == '1') ? TRUE:FALSE;

    memset(out, 0, sizeof(out));
    Utopia_RawGet(&utctx,NULL,SYSCFG_FORMAT_RA"_AdvPreferredRouterFlag",out, sizeof(out));    
    if (!_ansc_strcmp(out, "High"))
        g_ra_interface.Cfg.AdvPreferredRouterFlag = COSA_DML_RA_PREFER_ROUTER_High;
    else if (!_ansc_strcmp(out, "Low"))
        g_ra_interface.Cfg.AdvPreferredRouterFlag = COSA_DML_RA_PREFER_ROUTER_Low;
    else 
        g_ra_interface.Cfg.AdvPreferredRouterFlag = COSA_DML_RA_PREFER_ROUTER_Medium; /*the default value is medium*/

    Utopia_Free(&utctx,0);                    

    AnscCopyMemory(pEntry, &g_ra_interface, sizeof(*pEntry));


    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     COSA DML

    prototype:

        ANSC_STATUS
        CosaDmlRaIfSetValues
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
CosaDmlRaIfSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    UtopiaContext utctx = {0};
    char out[256] = {0};

    if (ulIndex)
        return ANSC_STATUS_FAILURE;

    g_ra_interface.Cfg.InstanceNumber = ulInstanceNumber;
    safe_strcpy(g_ra_interface.Cfg.Alias, pAlias, sizeof(g_ra_interface.Cfg.Alias));

    if (!Utopia_Init(&utctx))
        return ANSC_STATUS_FAILURE;

    sprintf(out, "%d", ulInstanceNumber);
    Utopia_RawSet(&utctx,NULL,SYSCFG_FORMAT_RA"_inst_num",out);

    Utopia_RawSet(&utctx,NULL,SYSCFG_FORMAT_RA"_alias",pAlias);

    Utopia_Free(&utctx,1);                    

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRaIfAddEntry
            (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_RA_IF_FULL        pEntry
            );

    Description:
    
        The API adds one RA interface into the system. 
        
    Arguments:   
                ANSC_HANDLE                 hContext,
                
                PCOSA_DML_RA_IF_FULL        pEntry
    Return:
        he pointer to the array of NAT port mappings, allocated by callee. If no entry is found, NULL is returned.

**********************************************************************/
ANSC_STATUS
CosaDmlRaIfAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RA_IF_FULL        pEntry
    )
{
    /*no chance to be here*/
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDmlRaIfDelEntry
            (
                ANSC_HANDLE                 hContext,
               ULONG                            ulInstanceNumber
            );

    Description:
    
        The API delete one RA interface from the system. 
        
    Arguments:
                ANSC_HANDLE                 hContext,
                ULONG                           ulInstanceNumber        

    Return:
        The operation status.

**********************************************************************/
ANSC_STATUS
CosaDmlRaIfDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
    /*no chance to get here*/
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlRaIfSetCfg
          (
                ANSC_HANDLE                 hContext,
                PCOSA_DML_RA_IF_CFG         pCfg
            );

    Description:
    
        The API the RA configuration. 
        
    Arguments:
                ANSC_HANDLE                 hContext,
                PCOSA_DML_RA_IF_CFG         pCfg

    Return:
        The status of the operation.

**********************************************************************/
#define SYSCFG_SET_BOOL(ns, name, val )         \
        if (val) \
            strcpy(out, "1"); \
        else  \
            strcpy(out, "0"); \
        Utopia_RawSet(&utctx,ns,name,out);  


#define SYSCFG_SET_ULONG(ns, name, val)          \
        sprintf(out, "%d", val); \
        Utopia_RawSet(&utctx,ns,name,out);           \



ANSC_STATUS
CosaDmlRaIfSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RA_IF_CFG         pCfg
    )
{
    UtopiaContext utctx = {0};
    char out[256] = {0};
    int  need_reconfig = 0;

    if ( !g_ra_interface.Cfg.InstanceNumber == pCfg->InstanceNumber )
        return ANSC_STATUS_CANT_FIND;

    if (!Utopia_Init(&utctx))
        return ANSC_STATUS_FAILURE;

    /*handle syscfg stuff*/
    if (!AnscEqualString(pCfg->Alias, g_ra_interface.Cfg.Alias, TRUE))
        Utopia_RawSet(&utctx,NULL,SYSCFG_FORMAT_RA"_alias",pCfg->Alias);  

    if (!AnscEqualString(pCfg->ManualPrefixes, g_ra_interface.Cfg.ManualPrefixes, TRUE))
    {
        need_reconfig = 1;
        Utopia_RawSet(&utctx,NULL,SYSCFG_FORMAT_RA"_ManualPrefixes",pCfg->ManualPrefixes);  
    }


    if (pCfg->bEnabled != g_ra_interface.Cfg.bEnabled)
    {
        need_reconfig = 1;
        SYSCFG_SET_BOOL(NULL,SYSCFG_FORMAT_RA"_enabled",  pCfg->bEnabled)
    }

    if (pCfg->bAdvManagedFlag != g_ra_interface.Cfg.bAdvManagedFlag)
    {
        need_reconfig = 1;
        SYSCFG_SET_BOOL(NULL,SYSCFG_FORMAT_RA"_AdvManagedFlag",  pCfg->bAdvManagedFlag)
    }

    if (pCfg->bAdvManagedFlag != g_ra_interface.Cfg.bAdvOtherConfigFlag)
    {
        need_reconfig = 1;
        SYSCFG_SET_BOOL(NULL,SYSCFG_FORMAT_RA"_AdvOtherConfigFlag",  pCfg->bAdvOtherConfigFlag)
    }

    if (pCfg->MaxRtrAdvInterval != g_ra_interface.Cfg.MaxRtrAdvInterval)
    {
        need_reconfig = 1;
        SYSCFG_SET_ULONG(NULL,SYSCFG_FORMAT_RA"_MaxRtrAdvInterval", pCfg->MaxRtrAdvInterval);                              
    }
    
    if (pCfg->MinRtrAdvInterval != g_ra_interface.Cfg.MinRtrAdvInterval)
    {
        need_reconfig = 1;
        SYSCFG_SET_ULONG(NULL,SYSCFG_FORMAT_RA"_MinRtrAdvInterval", pCfg->MinRtrAdvInterval);                              
    }

    if (pCfg->AdvDefaultLifetime != g_ra_interface.Cfg.AdvDefaultLifetime)
    {
        need_reconfig = 1;
        SYSCFG_SET_ULONG(NULL,SYSCFG_FORMAT_RA"_AdvDefaultLifetime", pCfg->AdvDefaultLifetime);                              
    }

    if (pCfg->AdvLinkMTU != g_ra_interface.Cfg.AdvLinkMTU)
    {
        need_reconfig = 1;
        SYSCFG_SET_ULONG(NULL,SYSCFG_FORMAT_RA"_AdvLinkMTU", pCfg->AdvLinkMTU);                              
    }

    if (pCfg->AdvReachableTime != g_ra_interface.Cfg.AdvReachableTime)
    {
        need_reconfig = 1;
        SYSCFG_SET_ULONG(NULL,SYSCFG_FORMAT_RA"_AdvReachableTime", pCfg->AdvReachableTime);                              
    }

    if (pCfg->AdvRetransTimer != g_ra_interface.Cfg.AdvRetransTimer)
    {
        need_reconfig = 1;
        SYSCFG_SET_ULONG(NULL,SYSCFG_FORMAT_RA"_AdvRetransTimer", pCfg->AdvRetransTimer);                              
    }

    if (pCfg->AdvCurHopLimit != g_ra_interface.Cfg.AdvCurHopLimit)
    {
        need_reconfig = 1;
        SYSCFG_SET_ULONG(NULL,SYSCFG_FORMAT_RA"_AdvCurHopLimit", pCfg->AdvCurHopLimit);                              
    }

    if (pCfg->AdvPreferredRouterFlag != g_ra_interface.Cfg.AdvPreferredRouterFlag)
    {
        need_reconfig = 1;
        if (pCfg->AdvPreferredRouterFlag == COSA_DML_RA_PREFER_ROUTER_High)
            Utopia_RawSet(&utctx,NULL,SYSCFG_FORMAT_RA"_AdvPreferredRouterFlag","High");              
        else if (pCfg->AdvPreferredRouterFlag == COSA_DML_RA_PREFER_ROUTER_Low)
            Utopia_RawSet(&utctx,NULL,SYSCFG_FORMAT_RA"_AdvPreferredRouterFlag","Low");              
        else 
            Utopia_RawSet(&utctx,NULL,SYSCFG_FORMAT_RA"_AdvPreferredRouterFlag","Medium");                  
    }

    Utopia_Free(&utctx,1);                    

    if (g_ra_enabled)
    {
        if ( g_ra_interface.Cfg.bEnabled && !pCfg->bEnabled )
            _radvd_operation("stop");
        else if (g_ra_interface.Cfg.bEnabled && pCfg->bEnabled && need_reconfig)
        {
            _gen_radvd_conf(pCfg);
            _radvd_operation("restart");
        }
        else if (!g_ra_interface.Cfg.bEnabled && pCfg->bEnabled)
        {
            _gen_radvd_conf(pCfg);
            _radvd_operation("restart");
        }
        else 
        {
            /*both false, don't need to do anything*/
        }
    }
    

    AnscCopyMemory(&g_ra_interface.Cfg, pCfg, sizeof(*pCfg));

    return ANSC_STATUS_SUCCESS;   
}
/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlRaIfGetCfg
           (
                ANSC_HANDLE                 hContext,
                 PCOSA_DML_RA_IF_CFG         pCfg
           );

    Description:
    
         The API adds one RA interface into the system. 
         
    Arguments:
                ANSC_HANDLE                 hContext,
                
                PCOSA_DML_RA_IF_CFG         pCfg
                
    Return:
        The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlRaIfGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RA_IF_CFG         pCfg
    )
{
    ULONG                           i = 0;

    if ( g_ra_interface.Cfg.InstanceNumber == pCfg->InstanceNumber )
    {
        AnscCopyMemory(pCfg, &g_ra_interface.Cfg, sizeof(*pCfg));
        return ANSC_STATUS_SUCCESS;
    }

    return ANSC_STATUS_CANT_FIND;   
}


/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlRaIfGetInfo
           (
                ANSC_HANDLE                 hContext,
                ULONG                           ulInstanceNumber,
                PCOSA_DML_RA_IF_INFO        pInfo
           );

    Description:
    
         The API adds one RA interface into the system. 
         
    Arguments:
                ANSC_HANDLE                 hContext,
                
                ULONG                           ulInstanceNumber,
                
                PCOSA_DML_RA_IF_INFO        pInfo

    Return:
        The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlRaIfGetInfo
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber,
        PCOSA_DML_RA_IF_INFO        pInfo
    )
{
    ULONG                           i = 0;
    PCOSA_DML_RA_IF_FULL2           pRAInterface = (PCOSA_DML_RA_IF_FULL2)hContext;
    char                          * p_prefix = NULL;

    if ( g_ra_interface.Cfg.InstanceNumber == ulInstanceNumber )
    {
        if (g_ra_enabled && pRAInterface->Cfg.bEnabled)
            g_ra_interface.Info.Status = COSA_DML_RA_IF_STATUS_Enabled;
        else
            g_ra_interface.Info.Status = COSA_DML_RA_IF_STATUS_Disabled;

        _get_ra_prefixes(&pRAInterface->Cfg, &p_prefix);
        if (!p_prefix || !p_prefix[0])
        {
            strcpy(g_ra_interface.Info.Prefixes, "");
        }
        else
        {
            safe_strcpy(g_ra_interface.Info.Prefixes, p_prefix, sizeof(g_ra_interface.Info.Prefixes));
            free(p_prefix);
        }

        AnscCopyMemory(pInfo, &g_ra_interface.Info, sizeof(*pInfo));
        return ANSC_STATUS_SUCCESS;
    }

    return ANSC_STATUS_CANT_FIND;   
}

/*
 *  RA Interface Options
 */
/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlRaIfGetNumberOfOptions
           (
                ANSC_HANDLE                 hContext,
                ULONG                           ulRaIfInstanceNumber,
           );

    Description:
    
        The API adds one RA interface into the system. 
        
    Arguments:
                ANSC_HANDLE                 hContext,
                
                ULONG                       ulRaIfInstanceNumber

    Return:
        The number of Options

**********************************************************************/

static int _syscfg_get_ra_options(SLIST_HEADER * p_list)
{
    UtopiaContext utctx = {0};
    char out[256] = {0};
    char * saveptr = NULL;
    char * str = NULL;
    char * p_token = NULL;
    COSA_DML_RA_OPTION * pEntry = NULL;

    /*RA options in syscfg format:
      tr_RouterAdvertise_option_list: "inst_num1,inst_num2"

      namespace: tr_RouterAdvertise_option_inst_num1 
      variable: enable, alias, tag, value
      namespace: tr_RouterAdvertise_option_inst_num1 
      variable: enable, alias, tag, value
      ......
     */
    if (!Utopia_Init(&utctx))
        return ANSC_STATUS_FAILURE;

    memset(out, 0, sizeof(out));
    Utopia_RawGet(&utctx,NULL,SYSCFG_FORMAT_RA_OPTION"_list",out,sizeof(out));
    
    if (!out[0])
        goto EXIT;
    
    for (str = out; ; str = NULL) 
    {
        char namespace[256] = {0};
        char val[256] = {0};

        p_token = strtok_r(str, ",", &saveptr);
        if (p_token == NULL)
            break;

        if (atoi(p_token))
        {
            memset(namespace, 0, sizeof(namespace));
            /*p_token contains instance number*/
            snprintf(namespace, sizeof(namespace)-1, "%s_%d", SYSCFG_FORMAT_RA_OPTION, atoi(p_token));
            memset(val, 0, sizeof(val));
            /*Utopia_RawGet() fail when return 0, which means can't find this namespace, then this optin instance doesn't exist;*/
            if (!Utopia_RawGet(&utctx,namespace,"enable",val,sizeof(val)))
                continue;

            pEntry = AnscAllocateMemory(sizeof(*pEntry));
            if (!pEntry)
                continue;

            pEntry->InstanceNumber = atoi(p_token);
            pEntry->bEnabled = (val[0] == '1' ? TRUE:FALSE );
            
            Utopia_RawGet(&utctx,namespace,"alias",pEntry->Alias,sizeof(pEntry->Alias)-1);
            Utopia_RawGet(&utctx,namespace,"value",pEntry->Value,sizeof(pEntry->Value)-1);
            
            memset(val, 0, sizeof(val));
            Utopia_RawGet(&utctx,namespace,"tag",val,sizeof(val));
            pEntry->Tag = atoi(val);
            
            AnscSListPushEntryAtBack(&g_ra_interface.OptionList, &pEntry->Link);                        
        }
        
    }


EXIT:
    Utopia_Free(&utctx,0);                    
    
    return 0;
}


ULONG
CosaDmlRaIfGetNumberOfOptions
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulRaIfInstanceNumber
    )
{
    ULONG                           i = 0;

    if ( ulRaIfInstanceNumber == g_ra_interface.Cfg.InstanceNumber )
    {
        _syscfg_get_ra_options(&g_ra_interface.OptionList);        
        return AnscSListQueryDepth(&g_ra_interface.OptionList);
    }

    return 0;
}

/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlRaIfGetOption
           (
                ANSC_HANDLE                 hContext,
                ULONG                       ulRaIfInstanceNumber,
                ULONG                       ulIndex,
                PCOSA_DML_RA_OPTION         pEntry
           );

    Description:
        
        The API adds one RA interface into the system. 
        
    Arguments:
    
        ANSC_HANDLE                 hContext,
        
        ULONG                       ulRaIfInstanceNumber,
        
        ULONG                       ulIndex,
        
        PCOSA_DML_RA_OPTION         pEntry

    Return:
        The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlRaIfGetOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulRaIfInstanceNumber,
        ULONG                       ulIndex,
        PCOSA_DML_RA_OPTION         pEntry
    )
{
    ULONG                           i = 0;
    PSINGLE_LINK_ENTRY              pSListEntry = NULL;
    PCOSA_DML_RA_OPTION             pOption = NULL;

    if ( ulRaIfInstanceNumber == g_ra_interface.Cfg.InstanceNumber )
    {
        pSListEntry =
            AnscSListGetEntryByIndex(&g_ra_interface.OptionList, ulIndex);
        if (pSListEntry)
        {
            pOption = ACCESS_RA_OPTION_LINK_OBJECT(pSListEntry);
            AnscCopyMemory(pEntry, pOption, sizeof(*pEntry));
            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_CANT_FIND;
}

/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlRaIfSetOptionValues
           (
                ANSC_HANDLE                 hContext,
                ULONG                       ulRaIfInstanceNumber,
                ULONG                       ulIndex,
                ULONG                       ulInstanceNumber,
                char*                       pAlias
            );

    Description:
        
        The API set one RA interface into the system. 
        
    Arguments:
    
        ANSC_HANDLE                 hContext,
        
        ULONG                       ulRaIfInstanceNumber,
        
        ULONG                       ulIndex,

        ULONG                       ulInstanceNumber,
        
        char*                       pAlias

    Return:
        The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlRaIfSetOptionValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulRaIfInstanceNumber,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    ULONG                           i = 0;
    PSINGLE_LINK_ENTRY              pSListEntry = NULL;
    PCOSA_DML_RA_OPTION             pOption = NULL;

    if ( ulRaIfInstanceNumber == g_ra_interface.Cfg.InstanceNumber )
    {
        pSListEntry =
            AnscSListGetEntryByIndex(&g_ra_interface.OptionList, ulIndex);

        if (pSListEntry)
        {
            pOption = ACCESS_RA_OPTION_LINK_OBJECT(pSListEntry);            

            pOption->InstanceNumber = ulInstanceNumber;
            AnscCopyString(pOption->Alias, pAlias);
            
            CcspTraceWarning(("CosaDmlRaIfSetOptionValues is called, some instance nubmer is 0, this couldn't happen\n\n"));
                
            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlRaIfAddOption
           (
                ANSC_HANDLE                 hContext,
                ULONG                           ulRaIfInstanceNumber,
                PCOSA_DML_RA_OPTION     pEntry
           );

    Description:
    
        The API adds one RA interface into the system. 
        
    Arguments:
                ANSC_HANDLE                 hContext,

                ULONG                            ulRaIfInstanceNumber,
                
                PCOSA_DML_RA_OPTION         pEntry

    Return:
        The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlRaIfAddOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulRaIfInstanceNumber,
        PCOSA_DML_RA_OPTION         pEntry
    )
{
    UtopiaContext                   utctx = {0};
    ULONG                           i = 0;
    PCOSA_DML_RA_OPTION             pOption = NULL;
    char                            out[256] = {0};
    char                            namespace[256] = {0};
    char                          * saveptr = NULL;
    char                          * str = NULL;
    char                          * p_token = NULL;
    PCOSA_DML_RA_IF_FULL2           pRAInterface = (PCOSA_DML_RA_IF_FULL2)hContext;

    if ( ulRaIfInstanceNumber == g_ra_interface.Cfg.InstanceNumber )
    {
        pOption = (PCOSA_DML_RA_OPTION)AnscAllocateMemory(sizeof(*pOption));

        if (!pOption)
            return ANSC_STATUS_FAILURE;

        if (!Utopia_Init(&utctx))
            return ANSC_STATUS_FAILURE;

        memset(out, 0, sizeof(out));
        Utopia_RawGet(&utctx,NULL,SYSCFG_FORMAT_RA_OPTION"_list",out,sizeof(out));
        
        /*sanity check for adding duplicate option instance*/
        if (out[0])
        {
            int  found = 0;

            for (str = out; ; str = NULL) 
            {
                p_token = strtok_r(str, ",", &saveptr);
                if (atoi(p_token) == pEntry->InstanceNumber)
                {
                    found = 1;
                    break;
                }
            }
            
            if (found)
            {
                CcspTraceWarning(("we already has this instanceNumbered option, something is wrong\n"));
                Utopia_Free(&utctx,0);                    
                return ANSC_STATUS_FAILURE;                
            }
        }        
        
        snprintf(namespace, sizeof(namespace)-1, "%s_%d", SYSCFG_FORMAT_RA_OPTION, pEntry->InstanceNumber);
        SYSCFG_SET_BOOL(namespace,"enable", pEntry->bEnabled );
        SYSCFG_SET_ULONG(namespace,"tag", pEntry->Tag );

        Utopia_RawSet(&utctx,namespace,"alias",pEntry->Alias);                          
        Utopia_RawSet(&utctx,namespace,"value",pEntry->Value);                          
        
        Utopia_Free(&utctx,1);                    

        AnscCopyMemory(pOption, pEntry, sizeof(*pOption));

        AnscSListPushEntryAtBack(&g_ra_interface.OptionList, &pOption->Link);            

        if (g_ra_enabled &&  pRAInterface->Cfg.bEnabled)
        {
            _gen_ra_option_file(&g_ra_interface.OptionList);
            _radvd_operation("restart");
        }

        return ANSC_STATUS_SUCCESS;
    }

    return ANSC_STATUS_CANT_FIND;
}

/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlRaIfDelOption
           (
                ANSC_HANDLE                 hContext,
                ULONG                           ulRaIfInstanceNumber,
                PCOSA_DML_RA_OPTION    pEntry

           );

    Description:
    
        The API adds one RA interface into the system. 
        
    Arguments:
                ANSC_HANDLE                 hContext,
                
                ULONG                                ulRaIfInstanceNumber,
                
                PCOSA_DML_RA_OPTION         pEntry

    Return:
        The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlRaIfDelOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulRaIfInstanceNumber,
        PCOSA_DML_RA_OPTION         pEntry
    )
{
    ULONG                           i = 0;
    PSINGLE_LINK_ENTRY              pLink           = (PSINGLE_LINK_ENTRY )NULL;
    PCOSA_DML_RA_OPTION             pOption = NULL;
    UtopiaContext                   utctx = {0};
    char                            namespace[256] = {0};
    char                          * saveptr = NULL;
    char                          * str = NULL;
    char                          * p_token = NULL;
    char                            inst_list[512] = {0};
    PCOSA_DML_RA_IF_FULL2           pRAInterface = (PCOSA_DML_RA_IF_FULL2)hContext;

    if ( ulRaIfInstanceNumber == g_ra_interface.Cfg.InstanceNumber )
    {
        pLink = AnscSListGetFirstEntry(&g_ra_interface.OptionList);

        while ( pLink )
        {
            pOption = ACCESS_RA_OPTION_LINK_OBJECT(pLink);

                
            if (pOption->InstanceNumber == pEntry->InstanceNumber)
            {
                if (!Utopia_Init(&utctx))
                    return ANSC_STATUS_FAILURE;

                snprintf(namespace, sizeof(namespace)-1, "%s_%d", SYSCFG_FORMAT_RA_OPTION, pEntry->InstanceNumber);        
                syscfg_unset(namespace, "enable");
                syscfg_unset(namespace, "alias");
                syscfg_unset(namespace, "tag");
                syscfg_unset(namespace, "value");

                Utopia_Free(&utctx,1);                                    

                AnscSListPopEntryByLink(&g_ra_interface.OptionList, pLink);                    
                AnscFreeMemory(pOption);
                break;
            }
                
            pLink   = AnscSListGetNextEntry(pLink);

        }
        
        /*now update the instance number list in syscfg*/
        pLink = AnscSListGetFirstEntry(&g_ra_interface.OptionList);
        while ( pLink )
        {
            pOption = ACCESS_RA_OPTION_LINK_OBJECT(pLink);

            snprintf(inst_list+strlen(inst_list), sizeof(inst_list)-1, "%d,", pOption->InstanceNumber);
            
            pLink   = AnscSListGetNextEntry(pLink);
        }        
        CcspTraceInfo(("CosaDmlRaIfDelOption, now inst_list is %s\n", inst_list));

        if (!Utopia_Init(&utctx))
                    return ANSC_STATUS_FAILURE;
        
        Utopia_RawSet(&utctx,NULL,SYSCFG_FORMAT_RA_OPTION"_list",inst_list);                          

        Utopia_Free(&utctx,1);                                    

        if (g_ra_enabled && pRAInterface->Cfg.bEnabled)
        {
            _gen_ra_option_file(&g_ra_interface.OptionList);
            _radvd_operation("restart");
        }
    }

    return ANSC_STATUS_CANT_FIND;
}

/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlRaIfSetOption
           (
                ANSC_HANDLE                 hContext,
                ULONG                           ulRaIfInstanceNumber,
                PCOSA_DML_RA_OPTION         pEntry
           );

    Description:
    
        The API adds one RA interface into the system. 
        
    Arguments:
                ANSC_HANDLE                 hContext,
               ULONG                            ulRaIfInstanceNumber,
                PCOSA_DML_RA_OPTION         pEntry

    Return:
        The status of the operation.

**********************************************************************/
ANSC_STATUS
CosaDmlRaIfSetOption
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulRaIfInstanceNumber,
        PCOSA_DML_RA_OPTION         pEntry
    )
{
    ULONG                           i       = 0;
    PSINGLE_LINK_ENTRY              pLink           = (PSINGLE_LINK_ENTRY )NULL;
    PCOSA_DML_RA_OPTION             pOption = NULL;
    UtopiaContext                   utctx = {0};
    char                            namespace[256] = {0};
    char                            out[256] = {0};
    char                          * saveptr = NULL;
    char                          * str = NULL;
    int                             need_reconfig = 0;
    PCOSA_DML_RA_IF_FULL2           pRAInterface = (PCOSA_DML_RA_IF_FULL2)hContext;

    if ( ulRaIfInstanceNumber == g_ra_interface.Cfg.InstanceNumber )
    {
        pLink = AnscSListGetFirstEntry(&g_ra_interface.OptionList);

        while ( pLink )
        {
            pOption = ACCESS_RA_OPTION_LINK_OBJECT(pLink);
                
            if (pOption->InstanceNumber == pEntry->InstanceNumber)
            {

                if (!Utopia_Init(&utctx))
                    return ANSC_STATUS_FAILURE;
                
                snprintf(namespace, sizeof(namespace)-1, "%s_%d", SYSCFG_FORMAT_RA_OPTION, pEntry->InstanceNumber);                        

                if (!AnscEqualString(pEntry->Alias, pOption->Alias, TRUE))
                    Utopia_RawSet(&utctx,namespace,"alias",pEntry->Alias);                          

                if (pEntry->bEnabled != pOption->bEnabled)
                {
                    need_reconfig = 1;
                    SYSCFG_SET_BOOL(namespace,"enable", pEntry->bEnabled );                    
                }

                if (!AnscEqualString(pEntry->Value, pOption->Value, TRUE))
                {
                    need_reconfig = 1;
                    Utopia_RawSet(&utctx,namespace,"value",pEntry->Value);                          
                }
                
                if (pEntry->Tag != pOption->Tag)
                {
                    need_reconfig = 1;
                    SYSCFG_SET_ULONG(namespace,"tag", pEntry->Tag );                    
                }
                
                Utopia_Free(&utctx,1);                                    

                AnscCopyMemory(pOption, pEntry, sizeof(*pEntry));
                
                if (g_ra_enabled && pRAInterface->Cfg.bEnabled)
                {
                    _gen_ra_option_file(&g_ra_interface.OptionList);
                    _radvd_operation("restart");
                }

                return ANSC_STATUS_SUCCESS;
            }
                
            pLink   = AnscSListGetNextEntry(pLink);

        }
    }

    return ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     self

    prototype:

       ANSC_STATUS
       CosaDmlRaIfGetOption2
           (
            ANSC_HANDLE                 hContext,
            ULONG                           ulRaIfInstanceNumber,
            PCOSA_DML_RA_OPTION         pEntry        
        );

    Description:
        
        The API adds one RA interface into the system. 
        
    Arguments:
    
        ANSC_HANDLE                 hContext,
        
        ULONG                       ulRaIfInstanceNumber,
        
        ULONG                       ulIndex,
        
        PCOSA_DML_RA_OPTION         pEntry

    Return:
        The status of the operation.

**********************************************************************/

ANSC_STATUS
CosaDmlRaIfGetOption2
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulRaIfInstanceNumber,
        PCOSA_DML_RA_OPTION         pEntry         
    )
{
    ULONG                           i = 0;
    PSINGLE_LINK_ENTRY              pLink           = (PSINGLE_LINK_ENTRY )NULL;
    PCOSA_DML_RA_OPTION             pOption = NULL;

    if ( ulRaIfInstanceNumber == g_ra_interface.Cfg.InstanceNumber )
    {
        pLink = AnscSListGetFirstEntry(&g_ra_interface.OptionList);

        while ( pLink )
        {
            pOption = ACCESS_RA_OPTION_LINK_OBJECT(pLink);
                
            if (pOption->InstanceNumber == pEntry->InstanceNumber)
            {
                AnscCopyMemory(pEntry, pOption, sizeof(*pEntry));
                return ANSC_STATUS_SUCCESS;
            }
                
            pLink   = AnscSListGetNextEntry(pLink);

        }
    }

    return ANSC_STATUS_CANT_FIND;
}

ANSC_STATUS
CosaDmlRaIfBERemove
    (
        void
    )
{
    PSINGLE_LINK_ENTRY              pLink           = (PSINGLE_LINK_ENTRY )NULL;
    PCOSA_DML_RA_OPTION             pOption = NULL;
    
    pLink = AnscSListGetFirstEntry(&g_ra_interface.OptionList);

    while ( pLink )
    {
        pOption = ACCESS_RA_OPTION_LINK_OBJECT(pLink);
        pLink   = AnscSListGetNextEntry(pLink);
        
        AnscFreeMemory(pOption);
    }

    return ANSC_STATUS_SUCCESS;
}


#endif
