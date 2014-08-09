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

    module: cosa_upnp_apis.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    copyright:

        Cisco Systems, Inc.
        All Rights Reserved.

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaDmlUpnpInit
        *  CosaDmlUpnpDevEnable
        *  CosaDmlUpnpDevGetState
        *  CosaDmlUpnpDevEnableMediaServer
        *  CosaDmlUpnpDevGetMediaServerState
        *  CosaDmlUpnpDevEnableIgd
        *  CosaDmlUpnpDevGetIgdState
        *  CosaDmlUpnpDevGetArchVer
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

#include "cosa_upnp_apis.h"

#ifdef _COSA_SIM_

BOOLEAN         g_UpnpDevEnable             = TRUE;
BOOLEAN         g_UpnpDevMediaServerEnable  = TRUE;
BOOLEAN         g_UpnpDevIgdEnable          = TRUE;
BOOLEAN         g_UpnpDiscoveryEnable       = TRUE;
ULONG           g_UpnpDiscoveryPollingInterval = 60;
ULONG           g_AdvPeriod                 = 1800;
ULONG           g_TTL                       = 5;

ANSC_STATUS
CosaDmlUpnpInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlUpnpDevEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bEnabled
    )
{
    g_UpnpDevEnable = bEnabled;
    
    return ANSC_STATUS_SUCCESS;
}

BOOLEAN
CosaDmlUpnpDevGetState
    (
        ANSC_HANDLE                 hContext
    )
{
    return g_UpnpDevEnable;
}

ANSC_STATUS
CosaDmlUpnpDevGetAdvPeriod
    (
        ANSC_HANDLE                 hContext,
        PULONG                     *val
    )
{
    *val = g_AdvPeriod;
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlUpnpDevSetAdvPeriod
    (
        ANSC_HANDLE                 hContext,
        ULONG                       val
    )
{
    g_AdvPeriod = val;
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlUpnpDevGetTTL
    (
        ANSC_HANDLE                 hContext,
        PULONG                     *val
    )
{
    *val = g_TTL;
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlUpnpDevSetTTL
    (
        ANSC_HANDLE                 hContext,
        ULONG                       val
    )
{
    g_TTL = val;
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlUpnpDevEnableMediaServer
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bEnabled
    )
{
    g_UpnpDevMediaServerEnable = bEnabled;
    
    return ANSC_STATUS_SUCCESS;
}

BOOLEAN
CosaDmlUpnpDevGetMediaServerState
    (
        ANSC_HANDLE                 hContext
    )
{
    return g_UpnpDevMediaServerEnable;
}

ANSC_STATUS
CosaDmlUpnpDevEnableIgd
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bEnabled
    )
{
    g_UpnpDevIgdEnable = bEnabled;
    
    return ANSC_STATUS_SUCCESS;
}

BOOLEAN
CosaDmlUpnpDevGetIgdState
    (
        ANSC_HANDLE                 hContext
    )
{
    return g_UpnpDevIgdEnable;
}

ANSC_STATUS
CosaDmlUpnpDevGetArchVer
    (
        ANSC_HANDLE                 hContext,
        ULONG*                      pMajorVer,
        ULONG*                      pMinorVer
    )
{
    if ( pMajorVer )
    {
        *pMajorVer = 2;
    }

    if ( pMinorVer )
    {
        *pMinorVer = 0;
    }
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlUpnpSetDiscoveryEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bEnabled
    )
{
    g_UpnpDiscoveryEnable = bEnabled;
    
    return ANSC_STATUS_SUCCESS;
}

BOOLEAN
CosaDmlUpnpGetDiscoveryEnable
    (
        ANSC_HANDLE                 hContext
    )
{
    return g_UpnpDiscoveryEnable;
}

ANSC_STATUS
CosaDmlUpnpSetDiscoveryPollingInterval
    (
        ANSC_HANDLE                 hContext,
        ULONG                       PollingInterval
    )
{
    g_UpnpDiscoveryPollingInterval = PollingInterval;
    
    return ANSC_STATUS_SUCCESS;
}

ULONG
CosaDmlUpnpGetDiscoveryPollingInterval
    (
        ANSC_HANDLE                 hContext
    )
{
    return g_UpnpDiscoveryPollingInterval;
}

#elif (_COSA_DRG_CNS_)
#include <utctx_api.h>
#include <utapi.h>
#include <utapi_util.h>
#include <ulog.h>

BOOLEAN         g_UpnpDevEnable             = TRUE;
BOOLEAN         g_UpnpDevMediaServerEnable  = TRUE;
BOOLEAN         g_UpnpDevIgdEnable          = TRUE;

ANSC_STATUS
CosaDmlUpnpInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlUpnpDevEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bEnabled
    )
{
    UtopiaContext pCtx;
    g_UpnpDevEnable = bEnabled;

    if (!Utopia_Init(&pCtx))
    {
        CcspTraceWarning(("Device.UPnP: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }
    if ( bEnabled )
    {
        Utopia_RawSet(&pCtx, NULL, "tr_device_upnp_enable", "true");
    }
    else
    {
        Utopia_RawSet(&pCtx, NULL, "tr_device_upnp_enable", "false");
    }
    system("sysevent set igd-restart");
    Utopia_Free(&pCtx, 1);
    
    return ANSC_STATUS_SUCCESS;
}

BOOLEAN
CosaDmlUpnpDevGetState
    (
        ANSC_HANDLE                 hContext
    )
{
    UtopiaContext pCtx;
    CHAR buf[64] = {'\0'};

    if (!Utopia_Init(&pCtx))
    {
        CcspTraceWarning(("Device.UPnP: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }
    
    Utopia_RawGet(&pCtx, NULL, "tr_device_upnp_enable", buf, sizeof(buf));
    if ( AnscEqualString(buf, "true", TRUE) )
    {
        g_UpnpDevEnable = TRUE;
    }
    else
    {
        g_UpnpDevEnable = FALSE;
    }
   
    Utopia_Free(&pCtx, 0);
    return g_UpnpDevEnable;
}

ANSC_STATUS
CosaDmlUpnpDevEnableMediaServer
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bEnabled
    )
{
    g_UpnpDevMediaServerEnable = bEnabled;
    
    return ANSC_STATUS_SUCCESS;
}

BOOLEAN
CosaDmlUpnpDevGetMediaServerState
    (
        ANSC_HANDLE                 hContext
    )
{
    return g_UpnpDevMediaServerEnable;
}

ANSC_STATUS
CosaDmlUpnpDevEnableIgd
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bEnabled
    )
{
    UtopiaContext pCtx;
    igdconf_t igd;
    CHAR buf[64] = {'\0'};
    g_UpnpDevIgdEnable = bEnabled;
 
    if (!Utopia_Init(&pCtx))
    {
        CcspTraceWarning(("Device.UPnP: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }

    if ( bEnabled )
    {
        igd.enable = 1;
    }
    else
    {
        igd.enable = 0;
    }
    UTOPIA_SETBOOL(&pCtx, UtopiaValue_Mgmt_IGDEnabled, igd.enable);

    Utopia_Free(&pCtx, 1); 
    return ANSC_STATUS_SUCCESS;
}

BOOLEAN
CosaDmlUpnpDevGetIgdState
    (
        ANSC_HANDLE                 hContext
    )
{
    UtopiaContext pCtx;
    igdconf_t igd;

    if (!Utopia_Init(&pCtx))
    {
        CcspTraceWarning(("Device.UPnP: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }
    Utopia_GetBool(&pCtx, UtopiaValue_Mgmt_IGDEnabled, &igd.enable);
    g_UpnpDevIgdEnable = (igd.enable == 0)? FALSE : TRUE;

    Utopia_Free(&pCtx, 0);
    return g_UpnpDevIgdEnable;
}

ANSC_STATUS
CosaDmlUpnpDevGetArchVer
    (
        ANSC_HANDLE                 hContext,
        ULONG*                      pMajorVer,
        ULONG*                      pMinorVer
    )
{
    if ( pMajorVer )
    {
        *pMajorVer = 2;
    }

    if ( pMinorVer )
    {
        *pMinorVer = 0;
    }
    
    return ANSC_STATUS_SUCCESS;
}


#elif (_COSA_DRG_TPG_)

BOOLEAN         g_UpnpDevEnable             = TRUE;
BOOLEAN         g_UpnpDevMediaServerEnable  = TRUE;
BOOLEAN         g_UpnpDevIgdEnable          = TRUE;

ANSC_STATUS
CosaDmlUpnpInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlUpnpDevEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bEnabled
    )
{
    g_UpnpDevEnable = bEnabled;
    
    return ANSC_STATUS_SUCCESS;
}

BOOLEAN
CosaDmlUpnpDevGetState
    (
        ANSC_HANDLE                 hContext
    )
{
    return g_UpnpDevEnable;
}

ANSC_STATUS
CosaDmlUpnpDevEnableMediaServer
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bEnabled
    )
{
    g_UpnpDevMediaServerEnable = bEnabled;
    
    return ANSC_STATUS_SUCCESS;
}

BOOLEAN
CosaDmlUpnpDevGetMediaServerState
    (
        ANSC_HANDLE                 hContext
    )
{
    return g_UpnpDevMediaServerEnable;
}

ANSC_STATUS
CosaDmlUpnpDevEnableIgd
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bEnabled
    )
{
    g_UpnpDevIgdEnable = bEnabled;
    
    return ANSC_STATUS_SUCCESS;
}

BOOLEAN
CosaDmlUpnpDevGetIgdState
    (
        ANSC_HANDLE                 hContext
    )
{
    return g_UpnpDevIgdEnable;
}

ANSC_STATUS
CosaDmlUpnpDevGetArchVer
    (
        ANSC_HANDLE                 hContext,
        ULONG*                      pMajorVer,
        ULONG*                      pMinorVer
    )
{
    if ( pMajorVer )
    {
        *pMajorVer = 2;
    }

    if ( pMinorVer )
    {
        *pMinorVer = 0;
    }
    
    return ANSC_STATUS_SUCCESS;
}

#endif
