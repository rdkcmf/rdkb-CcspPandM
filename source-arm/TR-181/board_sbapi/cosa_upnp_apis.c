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

    module: cosa_upnp_apis.c

        For COSA Data Model Library Development

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
#include <stdlib.h>
#include "cosa_drg_common.h"

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
    UNREFERENCED_PARAMETER(hDml);
    UNREFERENCED_PARAMETER(phContext);
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
    UNREFERENCED_PARAMETER(hContext);    
    return ANSC_STATUS_SUCCESS;
}

BOOLEAN
CosaDmlUpnpDevGetState
    (
        ANSC_HANDLE                 hContext
    )
{
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);    
    return ANSC_STATUS_SUCCESS;
}

BOOLEAN
CosaDmlUpnpDevGetMediaServerState
    (
        ANSC_HANDLE                 hContext
    )
{
    UNREFERENCED_PARAMETER(hContext);
    return g_UpnpDevMediaServerEnable;
}

ANSC_STATUS
CosaDmlUpnpDevEnableIgd
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bEnabled
    )
{
    UNREFERENCED_PARAMETER(hContext);
    g_UpnpDevIgdEnable = bEnabled;
    
    return ANSC_STATUS_SUCCESS;
}

BOOLEAN
CosaDmlUpnpDevGetIgdState
    (
        ANSC_HANDLE                 hContext
    )
{
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
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

#elif (defined _COSA_INTEL_USG_ARM_) || (defined  _COSA_BCM_MIPS_)
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
    UNREFERENCED_PARAMETER(phContext);
    UNREFERENCED_PARAMETER(hDml);
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
    UNREFERENCED_PARAMETER(hContext);

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
    UNREFERENCED_PARAMETER(hContext);

    if (!Utopia_Init(&pCtx))
    {
        CcspTraceWarning(("Device.UPnP: Error in initializing context!!! \n" ));
        return (BOOLEAN)ANSC_STATUS_FAILURE;
    }
    
    Utopia_RawGet(&pCtx, NULL, "tr_device_upnp_enable", buf, sizeof(buf));
    if (strcmp(buf, "true") == 0)
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
    UNREFERENCED_PARAMETER(hContext);

	if (!Utopia_Init(&utctx)){
		CcspTraceWarning(("Device.UPnP: Error in initializing utctx!!! \n"));
		return ANSC_STATUS_FAILURE;
	}

	Utopia_RawGet(&utctx, NULL, "upnp_igd_advr_expire", buf, sizeof(buf));
    
	if (AnscSizeOfString(buf)){
		*val = (PULONG)_ansc_atoi(buf);
	}else{
		*val = (PULONG)g_AdvPeriod; // use default value
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
    UNREFERENCED_PARAMETER(hContext);

    syslog_systemlog("UPnP", LOG_NOTICE, "Advertisement Period: %lu", val);

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
    UNREFERENCED_PARAMETER(hContext);
	if (!Utopia_Init(&utctx)){
		CcspTraceWarning(("Device.UPnP: Error in initializing utctx!!! \n"));
		return ANSC_STATUS_FAILURE;
	}

	Utopia_RawGet(&utctx, NULL, "upnp_igd_advr_ttl", buf, sizeof(buf));
    
	if (AnscSizeOfString(buf)){
		*val = (PULONG)_ansc_atoi(buf);
	}else{
		*val = (PULONG)g_TTL; // use default value
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
    UNREFERENCED_PARAMETER(hContext);

    syslog_systemlog("UPnP", LOG_NOTICE, "Time To Live: %lu", val);

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
    UNREFERENCED_PARAMETER(hContext);
    g_UpnpDevMediaServerEnable = bEnabled;
    
    return ANSC_STATUS_SUCCESS;
}

BOOLEAN
CosaDmlUpnpDevGetMediaServerState
    (
        ANSC_HANDLE                 hContext
    )
{
    UNREFERENCED_PARAMETER(hContext);
    return g_UpnpDevMediaServerEnable;
}

ANSC_STATUS
CosaDmlUpnpDevEnableIgd
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bEnabled
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UtopiaContext pCtx;
    igdconf_t igd;
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
    CcspTraceWarning(("Device.UPnP is Transitoning from false to true or viceversa \n" ));
    commonSyseventSet("firewall-restart", "");
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
    UNREFERENCED_PARAMETER(hContext);

    if (!Utopia_Init(&pCtx))
    {
        CcspTraceWarning(("Device.UPnP: Error in initializing context!!! \n" ));
        return (BOOLEAN)ANSC_STATUS_FAILURE;
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(bEnabled);
    return ANSC_STATUS_SUCCESS;
}

BOOLEAN
CosaDmlUpnpGetDiscoveryEnable
    (
        ANSC_HANDLE                 hContext
    )
{
    UNREFERENCED_PARAMETER(hContext);
    return FALSE;
}

ANSC_STATUS
CosaDmlUpnpSetDiscoveryPollingInterval
    (
        ANSC_HANDLE                 hContext,
        ULONG                       PollingInterval
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(PollingInterval);
    return ANSC_STATUS_SUCCESS;
}

ULONG
CosaDmlUpnpGetDiscoveryPollingInterval
    (
        ANSC_HANDLE                 hContext
    )
{
    UNREFERENCED_PARAMETER(hContext);
    return 0;
}

#endif
