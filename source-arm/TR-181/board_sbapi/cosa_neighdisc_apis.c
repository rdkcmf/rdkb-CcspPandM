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
#include "secure_wrapper.h"

/*
 *    Because we need interface's Alias to build connection request URL addr,
 *   so add these two definition together with _COSA_SIM_ temporarily.
 *   After finish backend integration, pls remove these two definition from here 
 *   like below:
 *   
 *
 */
#if (defined(_COSA_SIM_)  ||   defined(_COSA_DRG_TPG_))
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

#elif  defined(_COSA_INTEL_USG_ARM_) || defined(_COSA_BCM_MIPS_)
#include <net/if.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <libgen.h>
#include <utapi.h>
#include <utapi_util.h>
#include "utctx/utctx_api.h"
#include "syscfg/syscfg.h"
#include "cosa_ip_apis.h"


COSA_DML_NEIGHDISC_IF_FULL g_neighdisc_interface;
static BOOLEAN g_neighdisc_enabled = TRUE ;
int safe_strcpy(char * dst, char * src, int dst_size);


#define RetransTimer_SYSCTL_FILE "/proc/sys/net/ipv6/neigh/"COSA_DML_NEIGHDISC_IFNAME"/retrans_time_ms"
#define RtrSolicitationInterval_SYSCTL_FILE "/proc/sys/net/ipv6/conf/"COSA_DML_NEIGHDISC_IFNAME"/router_solicitation_interval"
#define MaxRtrSolicitations_SYSCTL_FILE "/proc/sys/net/ipv6/conf/"COSA_DML_NEIGHDISC_IFNAME"/router_solicitations"

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
    char buf[10];
    buf[0]='\0';
    if(!syscfg_get(NULL, "neigh_disc_enable", buf, sizeof(buf))) {
        *pEnabled = (atoi(buf) ? TRUE : FALSE);
        return ANSC_STATUS_SUCCESS;
    }
    return ANSC_STATUS_FAILURE;

    
}

ULONG 
CosaDmlNeighdiscSetEnabled
    (
        BOOLEAN  bEnabled
    )
{
    g_neighdisc_enabled = bEnabled;

    if( syscfg_set(NULL, "neigh_disc_enable", bEnabled ? "1" : "0") == 0)
    {
        syscfg_commit();
        return ANSC_STATUS_SUCCESS;
    }
    return ANSC_STATUS_FAILURE;
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
    return 1;
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
#define SYSCFG_FORMAT_NEIGHDISC_IF "tr_Neighdisc_If"

static int _apply_kernel_params(COSA_DML_NEIGHDISC_IF_CFG * pCfg)
{
    _write_sysctl_file(RetransTimer_SYSCTL_FILE, pCfg->RetransTimer);
    _write_sysctl_file(RtrSolicitationInterval_SYSCTL_FILE, 
                       (pCfg->RtrSolicitationInterval/1000 ? pCfg->RtrSolicitationInterval/1000:1));
    _write_sysctl_file(MaxRtrSolicitations_SYSCTL_FILE, pCfg->MaxRtrSolicitations);

    return 0;
}

ANSC_STATUS
CosaDmlNeighdiscIfGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_NEIGHDISC_IF_FULL        pEntry
    )
{
    UtopiaContext utctx = {0};
    char out[256] = {0};
    int  need_write = 0;

    if (!Utopia_Init(&utctx))
        return ANSC_STATUS_FAILURE;

    memset(out, 0, sizeof(out));
    Utopia_RawGet(&utctx,NULL,SYSCFG_FORMAT_NEIGHDISC_IF"_inst_num",out,sizeof(out));
    if (!out[0] || !(atoi(out)))
    {
        /*the first time system boots up*/
        need_write = 1;
        strcpy(out, "1");
        Utopia_RawSet(&utctx,NULL,SYSCFG_FORMAT_NEIGHDISC_IF"_inst_num",out);
    }
    g_neighdisc_interface.Cfg.InstanceNumber = atoi(out);

    memset(out, 0, sizeof(out));
    Utopia_RawGet(&utctx,NULL,SYSCFG_FORMAT_NEIGHDISC_IF"_alias",out,sizeof(out));
    if (!out[0])
    {
        /*the first time system boots up*/
        need_write = 1;
        strcpy(out, "InterfaceSetting1");
        Utopia_RawSet(&utctx,NULL,SYSCFG_FORMAT_NEIGHDISC_IF"_alias",out);
    }
    safe_strcpy(g_neighdisc_interface.Cfg.Alias, out, sizeof(g_neighdisc_interface.Cfg.Alias));

    /*we don't support writing to these variables*/
    memset(out, 0, sizeof(out));
    Utopia_RawGet(&utctx,NULL,SYSCFG_FORMAT_NEIGHDISC_IF"_enable",out,sizeof(out));
    if (!out[0])
        g_neighdisc_interface.Cfg.bEnabled = TRUE;
    else
        g_neighdisc_interface.Cfg.bEnabled = atoi(out);

    memset(out, 0, sizeof(out));
    Utopia_RawGet(&utctx,NULL,SYSCFG_FORMAT_NEIGHDISC_IF"_interface",out,sizeof(out));
    if (!out[0])
    {
        need_write = 1;
        strcpy(out, "erouter0");
        Utopia_RawSet(&utctx,NULL,SYSCFG_FORMAT_NEIGHDISC_IF"_interface",out);
    }
    safe_strcpy(g_neighdisc_interface.Cfg.Interface, out, sizeof(g_neighdisc_interface.Cfg.Interface));
    g_neighdisc_interface.Cfg.bNUDEnable = TRUE;
    g_neighdisc_interface.Cfg.bRSEnable  = TRUE;

    Utopia_RawGet(&utctx,NULL,SYSCFG_FORMAT_NEIGHDISC_IF"_RetransTimer",out,sizeof(out));
    if (!out[0] || !(atoi(out)))
        g_neighdisc_interface.Cfg.RetransTimer = 1000;
    else
        g_neighdisc_interface.Cfg.RetransTimer = atoi(out);

    Utopia_RawGet(&utctx,NULL,SYSCFG_FORMAT_NEIGHDISC_IF"_RtrSolicitationInterval",out,sizeof(out));
    if (!out[0] || !(atoi(out)))
        g_neighdisc_interface.Cfg.RtrSolicitationInterval = 4000;
    else
        g_neighdisc_interface.Cfg.RtrSolicitationInterval = atoi(out);

    Utopia_RawGet(&utctx,NULL,SYSCFG_FORMAT_NEIGHDISC_IF"_MaxRtrSolicitations",out,sizeof(out));
    if (!out[0] || !(atoi(out)))
        g_neighdisc_interface.Cfg.MaxRtrSolicitations = 3;
    else
        g_neighdisc_interface.Cfg.MaxRtrSolicitations = atoi(out);

    Utopia_Free(&utctx,need_write);                    

    AnscCopyMemory(pEntry, &g_neighdisc_interface, sizeof(*pEntry));

    /*handle backend*/
    _apply_kernel_params(&pEntry->Cfg);

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
    /*can't be called*/
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
    return ANSC_STATUS_SUCCESS;
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
    UtopiaContext utctx = {0};
    char out[256] = {0};

    if ( g_neighdisc_interface.Cfg.InstanceNumber != pCfg->InstanceNumber )
        return ANSC_STATUS_CANT_FIND;   

    /*handle syscfg stuff*/
    if (!Utopia_Init(&utctx))
        return ANSC_STATUS_FAILURE;

    if (pCfg->bEnabled != g_neighdisc_interface.Cfg.bEnabled)
    {
        sprintf(out, "%d", pCfg->bEnabled);
        Utopia_RawSet(&utctx,NULL,SYSCFG_FORMAT_NEIGHDISC_IF"_enable",out);
    }

    if (!AnscEqualString(pCfg->Interface, g_neighdisc_interface.Cfg.Interface, TRUE))
        Utopia_RawSet(&utctx,NULL,SYSCFG_FORMAT_NEIGHDISC_IF"_interface",pCfg->Interface);
 
    if (!AnscEqualString(pCfg->Alias, g_neighdisc_interface.Cfg.Alias, TRUE))
        Utopia_RawSet(&utctx,NULL,SYSCFG_FORMAT_NEIGHDISC_IF"_alias",pCfg->Alias);        

    if (pCfg->RetransTimer != g_neighdisc_interface.Cfg.RetransTimer)
    {
        sprintf(out, "%d", pCfg->RetransTimer);
        Utopia_RawSet(&utctx,NULL,SYSCFG_FORMAT_NEIGHDISC_IF"_RetransTimer",out);        

        /*this is so called backend implementation*/
        _write_sysctl_file(RetransTimer_SYSCTL_FILE, pCfg->RetransTimer);
    }

    if (pCfg->RtrSolicitationInterval != g_neighdisc_interface.Cfg.RtrSolicitationInterval)
    {
        sprintf(out, "%d", pCfg->RtrSolicitationInterval);
        Utopia_RawSet(&utctx,NULL,SYSCFG_FORMAT_NEIGHDISC_IF"_RtrSolicitationInterval",out);        

        /*this is so called backend implementation*/
        /*RtrSolicitationInterval is in the unit of millisecond, but kernel wants seconds.*/
        _write_sysctl_file(RtrSolicitationInterval_SYSCTL_FILE, 
                           (pCfg->RtrSolicitationInterval/1000 ? pCfg->RtrSolicitationInterval/1000:1));
    }

    if (pCfg->MaxRtrSolicitations != g_neighdisc_interface.Cfg.MaxRtrSolicitations)
    {
        sprintf(out, "%d", pCfg->MaxRtrSolicitations);
        Utopia_RawSet(&utctx,NULL,SYSCFG_FORMAT_NEIGHDISC_IF"_MaxRtrSolicitations",out);        

        /*this is so called backend implementation*/
        _write_sysctl_file(MaxRtrSolicitations_SYSCTL_FILE, pCfg->MaxRtrSolicitations);
    }

    Utopia_Free(&utctx,1);                    


    AnscCopyMemory(&g_neighdisc_interface.Cfg, pCfg, sizeof(*pCfg));

    return ANSC_STATUS_SUCCESS;   
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
    if ( g_neighdisc_interface.Cfg.InstanceNumber == pCfg->InstanceNumber )
    {
        AnscCopyMemory(pCfg, &g_neighdisc_interface.Cfg, sizeof(*pCfg));
        return ANSC_STATUS_SUCCESS;
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
    if ( g_neighdisc_interface.Cfg.InstanceNumber == ulInstanceNumber )
    {
	#if 0
        /*we always enable it*/
        g_neighdisc_interface.Info.Status = COSA_DML_NEIGHDISC_IF_STATUS_Enabled;
        #endif
	g_neighdisc_interface.Info.Status = (g_neighdisc_interface.Cfg.bEnabled ? COSA_DML_NEIGHDISC_IF_STATUS_Enabled : COSA_DML_NEIGHDISC_IF_STATUS_Disabled);
        AnscCopyMemory(pInfo, &g_neighdisc_interface.Info, sizeof(*pInfo));
        return ANSC_STATUS_SUCCESS;
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

#define NEIGHBOR_TABLE_RESULT_FILE        "/tmp/nbtbl.txt"
#define NEIGHBOR_TABLE_LINENUM_FILE       "/tmp/nbtbl_linenumber.txt"

void CosaDmlNeighborTableGetEntry
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulInstanceNumber,
        PCOSA_DML_NEIGHTABLE_INFO*  ppNbTbl
    )
{
    FILE*   fp         = NULL;
    char    buf[128]   = {0};
    int     counter    = 0;
    int     i          = 0;
    char   *p          = NULL;
    char   *p1         = NULL;

    if ( *ppNbTbl ){
        free(*ppNbTbl);
        *ppNbTbl = NULL;
        *pulInstanceNumber = 0;
    }

    v_secure_system("ip -6 neigh show |grep -E 'brlan0' > " NEIGHBOR_TABLE_RESULT_FILE );
    v_secure_system("grep lladdr -c " NEIGHBOR_TABLE_RESULT_FILE "> " NEIGHBOR_TABLE_LINENUM_FILE );
    
    fp = fopen(NEIGHBOR_TABLE_LINENUM_FILE, "r");
    if (NULL != fp) {
        fgets(buf, sizeof(buf), fp);
        
        counter = atoi(buf);
        fprintf(stderr, "%s -- %d %d\n", __FUNCTION__, __LINE__, counter);

        fclose(fp);
    }

    if ( counter <= 0 ){
        return;
    }

    *ppNbTbl = malloc(sizeof(COSA_DML_NEIGHTABLE_INFO)*counter);
    if ( *ppNbTbl == NULL ){
        return;
    }

    /* 
        2040:cafe::5850:5ff9:d425:f21c dev brlan0 lladdr 3c:97:0e:44:50:95 REACHABLE
        fe80::9490:e980:ba75:3632 dev brlan0 lladdr 3c:97:0e:44:50:95 REACHABLE
        fe80::21f:caff:fe5e:4f55 dev erouter0 lladdr 00:1f:ca:5e:4f:55 router STALE
        */    
    fp = fopen(NEIGHBOR_TABLE_RESULT_FILE, "r");
    if (NULL != fp) {
        
        i = 0;
        while( fgets(buf, sizeof(buf), fp) ) {
            p   = &buf[0];
            p1  = p;

            //get address
            while(*p != ' ') p++;
            *p = 0;

            _ansc_strcpy((*ppNbTbl)[i].Address, p1);
            p++;

            //get interface
            p = _ansc_strstr(p, "dev ");
            if ( p == NULL ){
                *pulInstanceNumber = i;
                fclose(fp);/*RDKB-6843, CID-33223, free unused resource befor exit*/
                return;
            }

            p  += _ansc_strlen("dev ");
            while(*p == ' ') p++;

            p1 = p;
            while(*p != ' ') p++;
            *p = 0;
            
            _ansc_strcpy((*ppNbTbl)[i].Interface, p1);
            p++;
            
            //get mac
            p = _ansc_strstr(p, "lladdr ");
            if ( p == NULL ){
                *pulInstanceNumber = i;
                fclose(fp);/*RDKB-6843, CID-33223, free unused resource befor exit*/
                return;
            }

            p  += _ansc_strlen("lladdr ");
            while(*p == ' ') p++;

            p1 = p;
            while(*p != ' ') p++;
            *p = 0;
            
            _ansc_strcpy((*ppNbTbl)[i].MACAddress, p1);
            p++;

            //get status
            if ( _ansc_strstr(p, "INCOMPLETE" )){
                (*ppNbTbl)[i].Status = NEIGHBOR_STATUS_INCOMPLETE;
            }else if ( _ansc_strstr(p, "REACHABLE" )){
                (*ppNbTbl)[i].Status = NEIGHBOR_STATUS_REACHABLE;
            }else if ( _ansc_strstr(p, "STALE" )){
                (*ppNbTbl)[i].Status = NEIGHBOR_STATUS_STALE;
            }else if ( _ansc_strstr(p, "DELAY" )){
                (*ppNbTbl)[i].Status = NEIGHBOR_STATUS_DELAY;
            }else if ( _ansc_strstr(p, "PROBE" )){
                (*ppNbTbl)[i].Status = NEIGHBOR_STATUS_PROBE;
            }else{
                *pulInstanceNumber = i;
                fclose(fp);/*RDKB-6843, CID-33223, free unused resource befor exit*/
                return;
            }

            i++;
        }
        
        *pulInstanceNumber = i;
        fclose(fp);
    }

    return;
}

#endif
