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

#elif (_COSA_INTEL_USG_ARM_)
#include <utctx_api.h>
#include <utapi.h>
#include <utapi_util.h>
#include <ulog.h>
#include <ccsp_syslog.h>

BOOLEAN         g_UpnpDevEnable             = TRUE;
BOOLEAN         g_UpnpDevMediaServerEnable  = TRUE;
BOOLEAN         g_UpnpDevIgdEnable          = TRUE;
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
CosaDmlUpnpDevGetAdvPeriod
    (
        ANSC_HANDLE                 hContext,
        PULONG                     *val
    )
{
	UtopiaContext utctx;
	CHAR buf[64] = {'\0'};

	if (!Utopia_Init(&utctx)){
		CcspTraceWarning(("Device.UPnP: Error in initializing utctx!!! \n"));
		return ANSC_STATUS_FAILURE;
	}

	Utopia_RawGet(&utctx, NULL, "upnp_igd_advr_expire", buf, sizeof(buf));
    
	if (AnscSizeOfString(buf)){
		*val = _ansc_atoi(buf);
	}else{
		*val = g_AdvPeriod; // use default value
	}

	Utopia_Free(&utctx, 0);
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlUpnpDevSetAdvPeriod
    (
        ANSC_HANDLE                 hContext,
        ULONG                       val
    )
{
    UtopiaContext utctx;
	CHAR buf[64] = {'\0'};

    syslog_systemlog("UPnP", LOG_NOTICE, "Advertisement Period: %d", val);

	if (!Utopia_Init(&utctx)){
        CcspTraceWarning(("Device.UPnP: Error in initializing utctx!!! \n"));
        return ANSC_STATUS_FAILURE;
	}

    _ansc_itoa(val, buf, 10);
	Utopia_RawSet(&utctx, NULL, "upnp_igd_advr_expire", buf);

	system("sysevent set igd-restart");
	Utopia_Free(&utctx, 1);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlUpnpDevGetTTL
    (
        ANSC_HANDLE                 hContext,
        PULONG                     *val
    )
{
	UtopiaContext utctx;
	CHAR buf[64] = {'\0'};

	if (!Utopia_Init(&utctx)){
		CcspTraceWarning(("Device.UPnP: Error in initializing utctx!!! \n"));
		return ANSC_STATUS_FAILURE;
	}

	Utopia_RawGet(&utctx, NULL, "upnp_igd_advr_ttl", buf, sizeof(buf));
    
	if (AnscSizeOfString(buf)){
		*val = _ansc_atoi(buf);
	}else{
		*val = g_TTL; // use default value
	}

	Utopia_Free(&utctx, 0);
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlUpnpDevSetTTL
    (
        ANSC_HANDLE                 hContext,
        ULONG                       val
    )
{
    UtopiaContext utctx;
	CHAR buf[64] = {'\0'};

    syslog_systemlog("UPnP", LOG_NOTICE, "Time To Live: %d", val);

	if (!Utopia_Init(&utctx)){
        CcspTraceWarning(("Device.UPnP: Error in initializing utctx!!! \n"));
        return ANSC_STATUS_FAILURE;
	}

    _ansc_itoa(val, buf, 10);
	Utopia_RawSet(&utctx, NULL, "upnp_igd_advr_ttl", buf);

	system("sysevent set igd-restart");
	Utopia_Free(&utctx, 1);

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
    UtopiaContext pCtx;
    igdconf_t igd;
    CHAR buf[64] = {'\0'};
    g_UpnpDevIgdEnable = bEnabled;

    syslog_systemlog("UPnP", LOG_NOTICE, "%s", (bEnabled==TRUE)?"enable":"disable");

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
    //system("sysevent set igd-restart");

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

ANSC_STATUS
CosaDmlUpnpSetDiscoveryEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bEnabled
    )
{
    return ANSC_STATUS_SUCCESS;
}

BOOLEAN
CosaDmlUpnpGetDiscoveryEnable
    (
        ANSC_HANDLE                 hContext
    )
{
    return FALSE;
}

ANSC_STATUS
CosaDmlUpnpSetDiscoveryPollingInterval
    (
        ANSC_HANDLE                 hContext,
        ULONG                       PollingInterval
    )
{
    return ANSC_STATUS_SUCCESS;
}

ULONG
CosaDmlUpnpGetDiscoveryPollingInterval
    (
        ANSC_HANDLE                 hContext
    )
{
    return 0;
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
