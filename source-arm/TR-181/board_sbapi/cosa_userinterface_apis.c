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

    module: cosa_userinterface_apis.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaDmlRaInit
        *  CosaDmlRaSetCfg
        *  CosaDmlRaGetCfg
	 *   CosaDmlUserInterfaceGetCfg
	 * CosaDmlUserInterfaceSetCfg
        *  CosaDmlRaGetSupportedProtocols
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

#include "cosa_userinterface_apis.h"

#include <utctx/utctx.h>
#include <utctx/utctx_api.h>
#include <utapi/utapi.h>
#include <utapi/utapi_util.h>
#include <syscfg/syscfg.h>
#include "cosa_drg_common.h"
#include "ccsp_custom.h"
#include "safec_lib_common.h"

COSA_DML_RA_CFG                     g_RaCfg = {0};

ANSC_STATUS
CosaDmlRaInit
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
CosaDmlRaSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RA_CFG            pCfg
    )
{
    UtopiaContext ctx;
    char sVal[64];
    UNREFERENCED_PARAMETER(hContext);
    if (!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    /* global flag */
    if (pCfg->bEnabled)
        Utopia_RawSet(&ctx, NULL, "mgmt_wan_access", "1");
    else
        Utopia_RawSet(&ctx, NULL, "mgmt_wan_access", "0");

    /* access ip range */
    if (pCfg->bFromAnyIp)
        Utopia_RawSet(&ctx, NULL, "mgmt_wan_srcany", "1");
    else
        Utopia_RawSet(&ctx, NULL, "mgmt_wan_srcany", "0");

    if (inet_ntop(AF_INET, &pCfg->StartIp.Value, sVal, sizeof(sVal)) != NULL)
        Utopia_RawSet(&ctx, NULL, "mgmt_wan_srcstart_ip", sVal);

    if (inet_ntop(AF_INET, &pCfg->EndIp.Value, sVal, sizeof(sVal)) != NULL)
        Utopia_RawSet(&ctx, NULL, "mgmt_wan_srcend_ip", sVal);

    /* HTTP access */
#if defined(CONFIG_CCSP_WAN_MGMT_ACCESS)
    char *sysCfghttpAccess = "mgmt_wan_httpaccess_ert";
#else
    char *sysCfghttpAccess = "mgmt_wan_httpaccess";
#endif

    if (pCfg->HttpEnable)
        Utopia_RawSet(&ctx, NULL, sysCfghttpAccess, "1");
    else
        Utopia_RawSet(&ctx, NULL, sysCfghttpAccess, "0");

    /*
    snprintf(sVal, sizeof(sVal), "%lu", pCfg->HttpPort);
    Utopia_RawSet(&ctx, NULL, "mgmt_wan_httpport", sVal); 
    */ 

    /* HTTPS access */
    if (pCfg->HttpsEnable)
        Utopia_RawSet(&ctx, NULL, "mgmt_wan_httpsaccess", "1");
    else
        Utopia_RawSet(&ctx, NULL, "mgmt_wan_httpsaccess", "0");
    /*
    snprintf(sVal, sizeof(sVal), "%lu", pCfg->HttpsPort);
    Utopia_RawSet(&ctx, NULL, "mgmt_wan_httpsport", sVal); 
    */ 

    /* SSH access */

    /*
    if (pCfg->SSHEnable)
        Utopia_RawSet(&ctx, NULL, "mgmt_wan_sshaccess", "1");
    else
        Utopia_RawSet(&ctx, NULL, "mgmt_wan_sshaccess", "0");
    */

    snprintf(sVal, sizeof(sVal), "%lu", pCfg->SSHPort);
    Utopia_RawSet(&ctx, NULL, "mgmt_wan_sshport", sVal);

    /* Telnet access */
    /*
    if (pCfg->TelnetEnable)
        Utopia_RawSet(&ctx, NULL, "mgmt_wan_telnetaccess", "1");
    else
        Utopia_RawSet(&ctx, NULL, "mgmt_wan_telnetaccess", "0");
    */

    snprintf(sVal, sizeof(sVal), "%lu", pCfg->TelnetPort);
    Utopia_RawSet(&ctx, NULL, "mgmt_wan_telnetport", sVal);

    /* FW Upgrade */
    if (pCfg->UpgradePermission)
        Utopia_RawSet(&ctx, NULL, "mgmt_wan_fwupgrade", "1");
    else
        Utopia_RawSet(&ctx, NULL, "mgmt_wan_fwupgrade", "0");

    Utopia_RawSet(&ctx, NULL, "mgmt_wan_srcstart_ipv6", pCfg->StartIpV6);
    Utopia_RawSet(&ctx, NULL, "mgmt_wan_srcend_ipv6", pCfg->EndIpV6);

    Utopia_Free(&ctx, 1);

    /*Restarting Firewall */
    if (system("sysevent set firewall-restart") != 0)
    {
        fprintf(stderr, "%s: fail to restart firewall\n", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    fprintf(stderr, "%s: OK !!!!\n", __FUNCTION__);
    return ANSC_STATUS_FAILURE;
}

static int 
Ut_GetInt(UtopiaContext *ctx, const char *key, int *val)
{
    CHAR buf[64] = {'\0'};

    if (!Utopia_RawGet(ctx, NULL, (char*)key, buf, sizeof(buf)))
        return 0;

    *val = atoi(buf);
    return 1;
}

ANSC_STATUS
CosaDmlRaGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RA_CFG            pCfg
    )
{
    UtopiaContext ctx;
    int iVal;
    char sVal[64];
    UNREFERENCED_PARAMETER(hContext);
    if (!Utopia_Init(&ctx))
    {
        CcspTraceWarning(("%s: Utopia_Init error\n", __FUNCTION__));
        fprintf(stderr, "%s: Utopia_Init error\n", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    /* global flag */
    if (!Ut_GetInt(&ctx, "mgmt_wan_access", &iVal))
    {
        fprintf(stderr, "%s: mgmt_wan_access error\n", __FUNCTION__);
        goto errout;
    }
    pCfg->bEnabled = (iVal == 1 ? TRUE : FALSE);

    snprintf(pCfg->SupportedProtocols, sizeof(pCfg->SupportedProtocols), 
            "%s", "HTTP,HTTPS,SSH,Telnet");

    /* access IP range */
    if (!Ut_GetInt(&ctx, "mgmt_wan_srcany", &iVal))
    {
        fprintf(stderr, "%s: mgmt_wan_srcany error\n", __FUNCTION__);
        goto errout;
    }
    pCfg->bFromAnyIp = (iVal == 1 ? TRUE : FALSE);

    if (!Utopia_RawGet(&ctx, NULL, "mgmt_wan_srcstart_ip", sVal, sizeof(sVal)))
    {
        fprintf(stderr, "%s: mgmt_wan_srcstart_ip error\n", __FUNCTION__);
        goto errout;
    }
    if (inet_pton(AF_INET, sVal, &pCfg->StartIp.Value) <= 0)
        pCfg->StartIp.Value = 0;

    if (!Utopia_RawGet(&ctx, NULL, "mgmt_wan_srcend_ip", sVal, sizeof(sVal)))
    {
        fprintf(stderr, "%s: mgmt_wan_srcend_ip error\n", __FUNCTION__);
        goto errout;
    }
    if (inet_pton(AF_INET, sVal, &pCfg->EndIp.Value) <= 0)
        pCfg->EndIp.Value = 0;

    /* HTTP access */
    if (!Ut_GetInt(&ctx, "mgmt_wan_httpaccess", &iVal))
    {
        fprintf(stderr, "%s: mgmt_wan_httpaccess error\n", __FUNCTION__);
        goto errout;
    }
    pCfg->HttpEnable = (iVal == 1 ? TRUE : FALSE);

#if defined(CONFIG_CCSP_WAN_MGMT_ACCESS)
    if (Ut_GetInt(&ctx, "mgmt_wan_httpaccess_ert", &iVal))
        pCfg->HttpEnable = (iVal == 1 ? TRUE : FALSE);
#endif

    if (!Ut_GetInt(&ctx, "mgmt_wan_httpport", &iVal))
    {
        fprintf(stderr, "%s: mgmt_wan_httpport error\n", __FUNCTION__);
        goto errout;
    }
    pCfg->HttpPort = iVal;

    /* HTTPS access */
    if (!Ut_GetInt(&ctx, "mgmt_wan_httpsaccess", &iVal))
    {
        fprintf(stderr, "%s: mgmt_wan_httpsaccess error\n", __FUNCTION__);
        goto errout;
    }
    pCfg->HttpsEnable = (iVal == 1 ? TRUE : FALSE);

    if (!Ut_GetInt(&ctx, "mgmt_wan_httpsport", &iVal))
    {
        fprintf(stderr, "%s: mgmt_wan_httpsport error\n", __FUNCTION__);
        goto errout;
    }
    pCfg->HttpsPort = iVal;

    /* SSH access */
    if (!Ut_GetInt(&ctx, "mgmt_wan_sshaccess", &iVal))
    {
        fprintf(stderr, "%s: mgmt_wan_sshaccess error\n", __FUNCTION__);
        goto errout;
    }
    pCfg->SSHEnable = (iVal == 1 ? TRUE : FALSE);

    if (!Ut_GetInt(&ctx, "mgmt_wan_sshport", &iVal))
    {
        fprintf(stderr, "%s: mgmt_wan_sshport error\n", __FUNCTION__);
        goto errout;
    }
    pCfg->SSHPort = iVal;

    /* Telnet access */
    if (!Ut_GetInt(&ctx, "mgmt_wan_telnetaccess", &iVal))
    {
	#if defined(_CBR_PRODUCT_REQ_) || defined (_BWG_PRODUCT_REQ_)
            return ANSC_STATUS_FAILURE;
    	#endif
        fprintf(stderr, "%s: mgmt_wan_telnetaccess error\n", __FUNCTION__);
        goto errout;
    }
    pCfg->TelnetEnable = (iVal == 1 ? TRUE : FALSE);

    if (!Ut_GetInt(&ctx, "mgmt_wan_telnetport", &iVal))
    {
        fprintf(stderr, "%s: mgmt_wan_telnetport error\n", __FUNCTION__);
        goto errout;
    }
    pCfg->TelnetPort = iVal;

    /* FW Upgrade */
    if (!Ut_GetInt(&ctx, "mgmt_wan_fwupgrade", &iVal))
    {
        fprintf(stderr, "%s: mgmt_wan_fwupgrade error\n", __FUNCTION__);
        goto errout;
    }
    pCfg->UpgradePermission = (iVal == 1 ? TRUE : FALSE);

    Utopia_RawGet(&ctx, NULL, "mgmt_wan_srcstart_ipv6", pCfg->StartIpV6, sizeof(pCfg->StartIpV6));
    Utopia_RawGet(&ctx, NULL, "mgmt_wan_srcend_ipv6", pCfg->EndIpV6, sizeof(pCfg->EndIpV6));

    Utopia_Free(&ctx, 0);
    return ANSC_STATUS_SUCCESS;

errout:
    Utopia_Free(&ctx, 0);

    CcspTraceWarning(("%s: ERROR !!!!\n", __FUNCTION__));
    fprintf(stderr, "%s: ERROR !!!!\n", __FUNCTION__);
    return ANSC_STATUS_FAILURE;
}


ANSC_STATUS
CosaDmlUserInterfaceGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_USERINTERFACE_CFG            pCfg
    )
{
    UNREFERENCED_PARAMETER(hContext);
        char buf[10];
	memset(buf,0,sizeof(buf));
        /* CID: 53702 Array compared against 0*/
        if(!syscfg_get( NULL, "PasswordLockoutEnable", buf, sizeof(buf))) {
                  if (strcmp(buf,"true") == 0)
                       pCfg->bPasswordLockoutEnable = TRUE;
                  else
                       pCfg->bPasswordLockoutEnable = FALSE;
        }
	memset(buf,0,sizeof(buf));
        /* CID: 53702 Array compared against 0*/
        if(!syscfg_get( NULL, "PasswordLockoutAttempts", buf, sizeof(buf))) {
			pCfg->PasswordLockoutAttempts=atoi(buf);
        }

	memset(buf,0,sizeof(buf));
        if(!syscfg_get( NULL, "PasswordLockoutTime", buf, sizeof(buf))) {
			pCfg->PasswordLockoutTime=atoi(buf);
        }

       memset(buf,0,sizeof(buf));
       /* CID: 53702 Array compared against 0*/
       if (!syscfg_get( NULL, "HTTPSecurityHeaderEnable", buf, sizeof(buf)))
       {
          if (strcmp(buf,"true") == 0)
               pCfg->bHTTPSecurityHeaderEnable = TRUE;
          else
               pCfg->bHTTPSecurityHeaderEnable = FALSE;
       }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlUserInterfaceSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_USERINTERFACE_CFG            pCfg
    )
{
    UNREFERENCED_PARAMETER(hContext);
    char buf[10];
    BOOL security_header_enabled = TRUE;

    if (syscfg_set(NULL, "PasswordLockoutEnable", (pCfg->bPasswordLockoutEnable == TRUE) ? "true" : "false") != 0) {
        AnscTraceWarning(("%s :PasswordLockoutEnable syscfg_set failed\n",__FUNCTION__));
    }

    if (syscfg_set_u(NULL, "PasswordLockoutAttempts", pCfg->PasswordLockoutAttempts) != 0) {
        AnscTraceWarning(("%s : PasswordLockoutAttempts syscfg_set failed\n",__FUNCTION__));
    }

    if (syscfg_set_u(NULL, "PasswordLockoutTime", pCfg->PasswordLockoutTime) != 0) {
        AnscTraceWarning(("%s : PasswordLockoutTime syscfg_set failed\n",__FUNCTION__));
    }

    if (syscfg_get(NULL, "HTTPSecurityHeaderEnable", buf, sizeof(buf)) == 0)
    {
        security_header_enabled = (strcmp(buf, "true") == 0) ? TRUE : FALSE;
    }

    if (pCfg->bHTTPSecurityHeaderEnable != security_header_enabled)
    {
        if (syscfg_set(NULL, "HTTPSecurityHeaderEnable", pCfg->bHTTPSecurityHeaderEnable ? "true" : "false") != 0)
        {
            AnscTraceWarning(("%s syscfg_set failed  for HTTPSecurityHeaderEnable\n",__FUNCTION__));
            return ANSC_STATUS_FAILURE;
        }

        system("/bin/sh /etc/webgui.sh &");
    }

    if (syscfg_commit() != 0)
    {
        AnscTraceWarning(("%s syscfg_commit failed\n",__FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}

#define IPRANGE_UTKEY_PREFIX "mgmt_wan_iprange_"
unsigned long iprangeCount = 0;

ULONG
CosaDmlUIIPRangeGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    )
{
    UtopiaContext ctx;
    char countStr[16];
    UNREFERENCED_PARAMETER(hContext);
    if (!Utopia_Init(&ctx)) {
        CcspTraceWarning(("%s: Utopia_Init error\n", __FUNCTION__));
        fprintf(stderr, "%s: Utopia_Init error\n", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }
    
    Utopia_RawGet(&ctx, NULL, IPRANGE_UTKEY_PREFIX"count", countStr, sizeof(countStr));
    iprangeCount = strtoul(countStr, NULL, 10);
    Utopia_Free(&ctx, 0);

    return iprangeCount;
}

ANSC_STATUS
CosaDmlUIIPRangeGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_UI_IPRANGE_ENTRY  pEntry
    )
{ 
    UtopiaContext ctx;
    char utVal[64];
    char utKey[64];
    UNREFERENCED_PARAMETER(hContext);
    if (!Utopia_Init(&ctx)) {
        CcspTraceWarning(("%s: Utopia_Init error\n", __FUNCTION__));
        fprintf(stderr, "%s: Utopia_Init error\n", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    snprintf(utKey, sizeof(utKey), IPRANGE_UTKEY_PREFIX"%lu_ins", ulIndex);
    Utopia_RawGet(&ctx, NULL, utKey, utVal, sizeof(utVal));
    pEntry->InstanceNumber = strtoul(utVal, NULL, 10);
    
    snprintf(utKey, sizeof(utKey), IPRANGE_UTKEY_PREFIX"%lu_alias", ulIndex);
    Utopia_RawGet(&ctx, NULL, utKey, utVal, sizeof(utVal));
    strncpy(pEntry->Alias, utVal, sizeof(pEntry->Alias));

    snprintf(utKey, sizeof(utKey), IPRANGE_UTKEY_PREFIX"%lu_startIP", ulIndex);
    Utopia_RawGet(&ctx, NULL, utKey, utVal, sizeof(utVal));
    strncpy(pEntry->StartIP, utVal, sizeof(pEntry->StartIP));
    
    snprintf(utKey, sizeof(utKey), IPRANGE_UTKEY_PREFIX"%lu_endIP", ulIndex);
    Utopia_RawGet(&ctx, NULL, utKey, utVal, sizeof(utVal));
    strncpy(pEntry->EndIP, utVal, sizeof(pEntry->EndIP));
    
    snprintf(utKey, sizeof(utKey), IPRANGE_UTKEY_PREFIX"%lu_desp", ulIndex);
    Utopia_RawGet(&ctx, NULL, utKey, utVal, sizeof(utVal));
    strncpy(pEntry->Desp, utVal, sizeof(pEntry->Desp));

    Utopia_Free(&ctx, 0);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlUIIPRangeSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    UtopiaContext ctx;
    char utVal[64];
    char utKey[64];
    UNREFERENCED_PARAMETER(hContext);
    if (!Utopia_Init(&ctx)) {
        CcspTraceWarning(("%s: Utopia_Init error\n", __FUNCTION__));
        fprintf(stderr, "%s: Utopia_Init error\n", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    snprintf(utKey, sizeof(utKey), IPRANGE_UTKEY_PREFIX"%lu_ins", ulIndex);
    snprintf(utVal, sizeof(utVal), "%lu", ulInstanceNumber);
    Utopia_RawSet(&ctx, NULL, utKey, utVal);

    snprintf(utKey, sizeof(utKey), IPRANGE_UTKEY_PREFIX"%lu_alias", ulIndex);
    Utopia_RawSet(&ctx, NULL, utKey, pAlias);

    Utopia_Free(&ctx, 1);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlUIIPRangeSetEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_UI_IPRANGE_ENTRY  pEntry
    )
{
    UtopiaContext ctx;
    unsigned long i;
    char utVal[64];
    char utKey[64];
    UNREFERENCED_PARAMETER(hContext);
    if (!Utopia_Init(&ctx)) {
        CcspTraceWarning(("%s: Utopia_Init error\n", __FUNCTION__));
        fprintf(stderr, "%s: Utopia_Init error\n", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    for(i = 0; i < iprangeCount; i++) {
        snprintf(utKey, sizeof(utKey), IPRANGE_UTKEY_PREFIX"%lu_ins", i);
        Utopia_RawGet(&ctx, NULL, utKey, utVal, sizeof(utVal));
        if(pEntry->InstanceNumber == strtoul(utVal, NULL, 10))
            break;
    }
    snprintf(utKey, sizeof(utKey), IPRANGE_UTKEY_PREFIX"%lu_alias", i);
    Utopia_RawSet(&ctx, NULL, utKey, pEntry->Alias);

    snprintf(utKey, sizeof(utKey), IPRANGE_UTKEY_PREFIX"%lu_startIP", i);
    Utopia_RawSet(&ctx, NULL, utKey, pEntry->StartIP);
    
    snprintf(utKey, sizeof(utKey), IPRANGE_UTKEY_PREFIX"%lu_endIP", i);
    Utopia_RawSet(&ctx, NULL, utKey, pEntry->EndIP);
    
    snprintf(utKey, sizeof(utKey), IPRANGE_UTKEY_PREFIX"%lu_desp", i);
    Utopia_RawSet(&ctx, NULL, utKey, pEntry->Desp);

    Utopia_Free(&ctx, 1);

    commonSyseventSet("firewall-restart", "");

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlUIIPRangeAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_UI_IPRANGE_ENTRY  pEntry
    )
{    
    UtopiaContext ctx;
    char utVal[64];
    char utKey[64];

    if (!Utopia_Init(&ctx)) {
        CcspTraceWarning(("%s: Utopia_Init error\n", __FUNCTION__));
        fprintf(stderr, "%s: Utopia_Init error\n", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    iprangeCount++;

    snprintf(utKey, sizeof(utKey), IPRANGE_UTKEY_PREFIX"count");
    snprintf(utVal, sizeof(utVal), "%lu", iprangeCount);
    Utopia_RawSet(&ctx, NULL, utKey, utVal);

    snprintf(utKey, sizeof(utKey), IPRANGE_UTKEY_PREFIX"%lu_ins", iprangeCount-1);
    snprintf(utVal, sizeof(utVal), "%lu", pEntry->InstanceNumber);
    Utopia_RawSet(&ctx, NULL, utKey, utVal);
    Utopia_Free(&ctx, 1);

    CosaDmlUIIPRangeSetEntry(hContext, pEntry);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlUIIPRangeDelEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_UI_IPRANGE_ENTRY  pEntry
    )
{
    UtopiaContext ctx;
    char utVal[64];
    char utKey[64];
    unsigned long i;
    UNREFERENCED_PARAMETER(hContext);
    
    if (!Utopia_Init(&ctx)) {
        CcspTraceWarning(("%s: Utopia_Init error\n", __FUNCTION__));
        fprintf(stderr, "%s: Utopia_Init error\n", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    for(i = 0; i < iprangeCount; i++) {
        snprintf(utKey, sizeof(utKey), IPRANGE_UTKEY_PREFIX"%lu_ins", i);
        Utopia_RawGet(&ctx, NULL, utKey, utVal, sizeof(utVal));
        if(pEntry->InstanceNumber == strtoul(utVal, NULL, 10))
            break;
    }

    for(; i+1 < iprangeCount; i++) {

        //get elements
        snprintf(utKey, sizeof(utKey), IPRANGE_UTKEY_PREFIX"%lu_ins", i+1);
        Utopia_RawGet(&ctx, NULL, utKey, utVal, sizeof(utVal));
        pEntry->InstanceNumber = strtoul(utVal, NULL, 10);

        snprintf(utKey, sizeof(utKey), IPRANGE_UTKEY_PREFIX"%lu_alias", i+1);
        Utopia_RawGet(&ctx, NULL, utKey, utVal, sizeof(utVal));
        strncpy(pEntry->Alias, utVal, sizeof(pEntry->Alias));

        snprintf(utKey, sizeof(utKey), IPRANGE_UTKEY_PREFIX"%lu_startIP", i+1);
        Utopia_RawGet(&ctx, NULL, utKey, utVal, sizeof(utVal));
        strncpy(pEntry->StartIP, utVal, sizeof(pEntry->StartIP));

        snprintf(utKey, sizeof(utKey), IPRANGE_UTKEY_PREFIX"%lu_endIP", i+1);
        Utopia_RawGet(&ctx, NULL, utKey, utVal, sizeof(utVal));
        strncpy(pEntry->EndIP, utVal, sizeof(pEntry->EndIP));

        snprintf(utKey, sizeof(utKey), IPRANGE_UTKEY_PREFIX"%lu_desp", i+1);
        utVal[0] = '\0';
        Utopia_RawGet(&ctx, NULL, utKey, utVal, sizeof(utVal));
        strncpy(pEntry->Desp, utVal, sizeof(pEntry->Desp));

        //move elements
        snprintf(utKey, sizeof(utKey), IPRANGE_UTKEY_PREFIX"%lu_ins", i);
        snprintf(utVal, sizeof(utVal), "%lu", pEntry->InstanceNumber);
        Utopia_RawSet(&ctx, NULL, utKey, utVal);

        snprintf(utKey, sizeof(utKey), IPRANGE_UTKEY_PREFIX"%lu_alias", i);
        Utopia_RawSet(&ctx, NULL, utKey, pEntry->Alias);

        snprintf(utKey, sizeof(utKey), IPRANGE_UTKEY_PREFIX"%lu_startIP", i);
        Utopia_RawSet(&ctx, NULL, utKey, pEntry->StartIP);

        snprintf(utKey, sizeof(utKey), IPRANGE_UTKEY_PREFIX"%lu_endIP", i);
        Utopia_RawSet(&ctx, NULL, utKey, pEntry->EndIP);

        snprintf(utKey, sizeof(utKey), IPRANGE_UTKEY_PREFIX"%lu_desp", i);
        Utopia_RawSet(&ctx, NULL, utKey, pEntry->Desp);
    }

    //delete last element
    snprintf(utKey, sizeof(utKey), IPRANGE_UTKEY_PREFIX"%lu_ins", iprangeCount-1);
    Utopia_RawSet(&ctx, NULL, utKey, 0);

    snprintf(utKey, sizeof(utKey), IPRANGE_UTKEY_PREFIX"%lu_alias", iprangeCount-1);
    Utopia_RawSet(&ctx, NULL, utKey, 0);

    snprintf(utKey, sizeof(utKey), IPRANGE_UTKEY_PREFIX"%lu_startIP", iprangeCount-1);
    Utopia_RawSet(&ctx, NULL, utKey, 0);
    
    snprintf(utKey, sizeof(utKey), IPRANGE_UTKEY_PREFIX"%lu_endIP", iprangeCount-1);
    Utopia_RawSet(&ctx, NULL, utKey, 0);
    
    snprintf(utKey, sizeof(utKey), IPRANGE_UTKEY_PREFIX"%lu_desp", iprangeCount-1);
    Utopia_RawSet(&ctx, NULL, utKey, 0);

    //update counter
    iprangeCount--;
    snprintf(utKey, sizeof(utKey), IPRANGE_UTKEY_PREFIX"count");
    snprintf(utVal, sizeof(utVal), "%lu", iprangeCount);
    Utopia_RawSet(&ctx, NULL, utKey, utVal);

    Utopia_Free(&ctx, 1);
       
    commonSyseventSet("firewall-restart", "");

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlUIIPRangeGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_UI_IPRANGE_ENTRY  pEntry
    )
{
    fprintf(stderr, "%s is not implemented!\n", __FUNCTION__);
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pEntry);
    return ANSC_STATUS_SUCCESS;
}
