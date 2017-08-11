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

    module: cosa_x_cisco_com_devicecontrol_apis.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    copyright:

        Cisco Systems, Inc.
        All Rights Reserved.

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaDmlDcInit
        *  CosaDmlDcGetMultiHomedHSDFlag
        *  CosaDmlDcGetMultiHomedUIPageControl
        *  CosaDmlDcGetMultiHomedMode
        *  CosaDmlDcGetMultiHomedBridgingStatus
    -------------------------------------------------------------------

    environment:

        platform independent

    -------------------------------------------------------------------

    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        07/15/2011    initial revision.

**************************************************************************/

#include "cosa_x_cisco_com_devicecontrol_apis.h"
#include "cosa_x_cisco_com_devicecontrol_dml.h"
#include "dml_tr181_custom_cfg.h" 
#include "ccsp_dm_api.h"
#include <arpa/inet.h>
#include "platform_hal.h"

extern void* g_pDslhDmlAgent;

PCHAR g_avahi_daemon_conf[] =
{
    "",
    "[server]",
    "#host-name=foo",
    "#domain-name=local",
    "#browse-domains=0pointer.de, zeroconf.org",
    "use-ipv4=yes",
    "use-ipv6=no",
    "allow-interfaces=brlan0",
    "#deny-interfaces=eth1",
    "#check-response-ttl=no",
    "#use-iff-running=no",
    "enable-dbus=no",
    "#disallow-other-stacks=no",
    "#allow-point-to-point=no",
    "#cache-entries-max=4096",
    "#clients-max=4096",
    "#objects-per-client-max=1024",
    "#entries-per-entry-group-max=32",
    "ratelimit-interval-usec=1000000",
    "ratelimit-burst=1000",
    "",
    "[wide-area]",
    "enable-wide-area=yes",
    "",
    "[publish]",
    "#disable-publishing=no",
    "#disable-user-service-publishing=no",
    "#add-service-cookie=no",
    "#publish-addresses=yes",
    "#publish-hinfo=yes",
    "#publish-workstation=yes",
    "#publish-domain=yes",
    "#publish-dns-servers=192.168.50.1, 192.168.50.2",
    "#publish-resolv-conf-dns-servers=yes",
    "#publish-aaaa-on-ipv4=yes",
    "#publish-a-on-ipv6=no",
    "",
    "[reflector]",
    "#enable-reflector=no",
    "#reflect-ipv=no",
    "",
    "[rlimits]",
    "#rlimit-as=",
    "rlimit-core=0",
    "rlimit-data=4194304",
    "rlimit-fsize=0",
    "rlimit-nofile=768",
    "rlimit-stack=4194304",
    "rlimit-nproc=3",
    NULL
};


#define FR_NONE 1
#define FR_WIFI (1<<1)
#define FR_ROUTER (1<<2)
#define FR_FW (1<<3)
#define FR_OTHER (1<<4)

extern ANSC_HANDLE bus_handle;
char   dst_pathname_cr[64]  =  {0};
static componentStruct_t **        ppComponents = NULL;
extern char        g_Subsystem[32];

int fwSync = 0;

#include "arpa/inet.h"

#include <utctx.h>
#include <utctx_api.h>
#include <utapi.h>
#include <utapi_util.h>
#include <ccsp_syslog.h>
#include "syscfg/syscfg.h"

#include "platform_hal.h"


#define HTTPD_CONF      "/var/lighttpd.conf"
#define HTTPD_DEF_CONF  "/etc/lighttpd.conf"
#define HTTPD_PID       "/var/run/lighttpd.pid"

static void configBridgeMode(int bEnable);
static int curticket   = 1; /*The thread should be run with the ticket*/
static int totalticket = 0;

extern int commonSyseventFd ;
extern token_t commonSyseventToken;

#if 0
void configWifi()
{
	     sleep(2);
             CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
              componentStruct_t **        ppComponents = NULL;
	      char* faultParam = NULL;
	      int size =0;
	      int ret;
	snprintf(dst_pathname_cr, sizeof(dst_pathname_cr), "%s%s", g_Subsystem, CCSP_DBUS_INTERFACE_CR);
	    ret = CcspBaseIf_discComponentSupportingNamespace(bus_handle,
                dst_pathname_cr,
                "Device.DeviceInfo.X_RDKCENTRAL-COM_ConfigureWiFi",
                g_Subsystem,        /* prefix */
                &ppComponents,
                &size);
	   if ( ret != CCSP_SUCCESS )	
		return ANSC_STATUS_FAILURE;

parameterValStruct_t                value = { "Device.DeviceInfo.X_RDKCENTRAL-COM_ConfigureWiFi", "true", ccsp_boolean};
            ret = CcspBaseIf_setParameterValues
                                (
                                        bus_handle,
                                        ppComponents[0]->componentName,
                                        ppComponents[0]->dbusPath,
                                        0, 0x0,   /* session id and write id */
                                        &value,
                                        1,
                                        TRUE,   /* no commit */
                                        &faultParam
                                );

                if (ret != CCSP_SUCCESS && faultParam)
                {
                    AnscTraceError(("Error:Failed to SetValue for param '%s'\n", faultParam));
                    bus_info->freefunc(faultParam);
                }

}

#endif
static int initWifiComp() {
    int size =0 ,ret;
    snprintf(dst_pathname_cr, sizeof(dst_pathname_cr), "%s%s", g_Subsystem, CCSP_DBUS_INTERFACE_CR);

    ret = CcspBaseIf_discComponentSupportingNamespace(bus_handle,
                dst_pathname_cr,
                "Device.WiFi.X_CISCO_COM_FactoryReset",
                g_Subsystem,        /* prefix */
                &ppComponents,
                &size);
  
    return ret == CCSP_SUCCESS ? 0 : ret;
}

static int UtGetBool(const char *path, BOOLEAN *pVal)
{
    UtopiaContext ctx;
    char buf[64] = {0};

    if (!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;
    
    Utopia_RawGet(&ctx, NULL, (char *)path, buf, sizeof(buf));
    if (atoi(buf) == 1)
        *pVal = TRUE;
    else
        *pVal = FALSE;

    Utopia_Free(&ctx, 1);

    return ANSC_STATUS_SUCCESS;
}

static int UtSetBool(const char *path, BOOLEAN val)
{
    UtopiaContext ctx;

    if (!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;
    
    if (val)
        Utopia_RawSet(&ctx, NULL, (char *)path, "1");
    else
        Utopia_RawSet(&ctx, NULL, (char *)path, "0");

    Utopia_Free(&ctx, 1);

    return ANSC_STATUS_SUCCESS;
}

static int UtGetUlong(const char *path, ULONG *pVal)
{
    UtopiaContext ctx;
    char buf[64] = {0};
    int ret;

    if (!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;
    
    if((ret = Utopia_RawGet(&ctx, NULL, (char *)path, buf, sizeof(buf))) == 0){
        Utopia_Free(&ctx, 1);
        return ANSC_STATUS_FAILURE;
    }

    *pVal = atoi(buf);

    Utopia_Free(&ctx, 1);

    return ANSC_STATUS_SUCCESS;
}

static int UtSetUlong(const char *path, ULONG val)
{
    UtopiaContext ctx;
    char buf[64];

    if (!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;
    
    snprintf(buf, sizeof(buf), "%d", val);
    Utopia_RawSet(&ctx, NULL, (char *)path, buf);

    Utopia_Free(&ctx, 1);

    return ANSC_STATUS_SUCCESS;
}

typedef struct WebServConf {
    ULONG       httpport;
    ULONG       httpsport;
} WebServConf_t;

static int
LoadWebServConf(WebServConf_t *conf)
{
    ULONG val;
    
    if (UtGetUlong("mgmt_wan_httpport", &conf->httpport) != ANSC_STATUS_SUCCESS)
        return -1;
    if (UtGetUlong("mgmt_wan_httpsport", &conf->httpsport) != ANSC_STATUS_SUCCESS)
        return -1;

#if defined(CONFIG_CCSP_WAN_MGMT_PORT)
    if (UtGetUlong("mgmt_wan_httpport_ert", &val) == ANSC_STATUS_SUCCESS)
        conf->httpport = val;
#endif

    return 0;
}

static int
SaveWebServConf(const WebServConf_t *conf)
{
#if defined(CONFIG_CCSP_WAN_MGMT_PORT)
    const char *sysCfghttpPort = "mgmt_wan_httpport_ert";
#else
    const char *sysCfghttpPort = "mgmt_wan_httpport";
#endif

    if (UtSetUlong(sysCfghttpPort, conf->httpport) != ANSC_STATUS_SUCCESS)
        return -1;
    if (UtSetUlong("mgmt_wan_httpsport", conf->httpsport) != ANSC_STATUS_SUCCESS)
        return -1;

    return 0;
}

static int 
WebServRestart(const WebServConf_t *conf)
{
#if 0
    if (access(HTTPD_CONF, F_OK) != 0) {
        if (vsystem("cp %s %s", HTTPD_DEF_CONF, HTTPD_CONF) != 0) {
            fprintf(stderr, "%s: no config file\n", __FUNCTION__);
            return -1;
        }
    }

    if (vsystem("sed -i ':a;N;$!ba;s#[ \\t]*server.port[ \\t]*=[ 0-9]*#server.port = %d#' %s", 
                conf->httpport, HTTPD_CONF) != 0
            || vsystem("sed -i ':a;N;$!ba;s#\\$SERVER\\[[^]]*\\] == \"[^\"]*#$SERVER[\"socket\"] == \":%d\"#' %s", 
                conf->httpsport, HTTPD_CONF) != 0) {
        fprintf(stderr, "%s: fail to set config file\n", __FUNCTION__);
        return -1;
    }

    if (vsystem("lighttpd -t -f %s", HTTPD_CONF) != 0) {
        fprintf(stderr, "%s: bad config file format\n", __FUNCTION__);
        return -1;
    }

    if (access(HTTPD_PID, F_OK) == 0) {
        if (vsystem("kill `cat %s`", HTTPD_PID) != 0) {
            fprintf(stderr, "%s: fail to stop lighttpd\n", __FUNCTION__);
            return -1;
        }
    }

    if (vsystem("lighttpd -f %s", HTTPD_CONF) != 0) {
        fprintf(stderr, "%s: fail to start lighttpd\n", __FUNCTION__);
        return -1;
    }
#endif
    if (vsystem("/bin/sh /etc/webgui.sh") != 0) {
        fprintf(stderr, "%s: fail to restart lighttpd\n", __FUNCTION__);
        return -1;
    }

    system("sysevent set firewall-restart");

    return 0;
}

static int TelnetdConfig(int enable)
{
    if (enable)
    {
        /* start the server if service not started */
        if (system("ps | grep telnetd | grep -v grep") != 0)
            if (system("/usr/sbin/telnetd -l /usr/sbin/cli") != 0)
                return -1;
    }
    else
    {
        system("killall telnetd");
    }

    system("sysevent set firewall-restart");

    return 0;
}

static int SshdConfig(int enable)
{
    if (enable)
    {
        /* start the server if service not started */
        if (system("ps | grep dropbear | grep -v grep") != 0)
            if (system("dropbear -r /etc/rsa_key.priv") != 0)
                return -1;
    }
    else
    {
        system("killall dropbear");
    }

    system("sysevent set firewall-restart");

    return 0;
}

static int SnmpdConfig(int enable)
{
#if 1
    // Not For Now
#else
    if (enable)
    {
        /* start the server if service not started */
        if (system("ps | grep snmp_agent_cm | grep -v grep") != 0)
            if (system("/usr/sbin/snmp_agent_cm -c /etc/agent_cm.cnf -n") != 0)
                return -1;
        if (system("ps | grep snmp_subagent | grep -v grep") != 0)
            if (system("/fss/gw/usr/share/snmp/run_subagent.sh tcp:127.0.0.1:705") != 0)
                return -1;
    }
    else
    {
        system("killall snmp_agent_cm");
        system("killall run_subagent.sh");
        system("killall snmp_subagent");
    }
#endif

    return 0;
}

static int detect_process(char *process_name)
{
    FILE *ptr;
    char buff[512];
    char ps[128];

    sprintf(ps, "ps | grep -v grep | grep -c %s", process_name);

    if ((ptr=popen(ps, "r"))!=NULL)
    {
        while (fgets(buff,512,ptr)!=NULL)
        {
            if (atoi(buff)>=1)
            {
                pclose(ptr);
                return 1;
            }
        }

        pclose(ptr);
    }

    return 0;
}

static BOOLEAN         g_EnableZeroConfig = FALSE;

ANSC_STATUS
CosaDmlDcInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    char     buf[256] = {0};
    UtopiaContext utctx = {0};

    if (Utopia_Init(&utctx))
    {
        Utopia_RawGet(&utctx,NULL,UTOPIA_AVAHI_ENABLED,buf,sizeof(buf)-1);
        Utopia_Free(&utctx,0);                
    }

    if ( buf[0] == '1' )
    {
        g_EnableZeroConfig = TRUE;
    }
    else
    {
        g_EnableZeroConfig = FALSE;
    }

    if ( g_EnableZeroConfig )
    {
        /*start avahi two applications*/
        _CosaDmlDcStartZeroConfig();
    }
    else
    {
        /*stop avahi two applications*/
        _CosaDmlDcStopZeroConfig();
    }

    platform_hal_DocsisParamsDBInit();
    syscfg_init();
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetMultiHomedHSDFlag
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    AnscCopyString(pValue, "Cisco");
    *pulSize = AnscSizeOfString(pValue);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetMultiHomedUIPageControl
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    AnscCopyString(pValue, "000000");
    *pulSize = AnscSizeOfString(pValue);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetMultiHomedMode
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    )
{    
    //AnscCopyString(pValue, _ERROR_);
    //*pulSize = AnscSizeOfString(pValue);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetMultiHomedBridgingStatus
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    )
{
    //AnscCopyString(pValue, _ERROR_);
    //*pulSize = AnscSizeOfString(pValue);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetMultiHomedMode
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulValue
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetMultiHomedHSDFlag
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulValue

    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetMultiHomedUIPageControl
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulValue

    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetWanAddressMode
    (
        ANSC_HANDLE                 hContext,
        COSA_DML_WanAddrMode        *pMode
    )
{
    UtopiaContext ctx;
    char wanProto[32] = {'\0'};

    if (!Utopia_Init(&ctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }
    
    Utopia_Get(&ctx, UtopiaValue_WAN_Proto, wanProto, sizeof(wanProto));

    if(!_ansc_strcmp(wanProto, "dhcp"))
        *pMode = COSA_DML_WanAddrMode_DHCP;
    else if(!_ansc_strcmp(wanProto, "static"))
        *pMode = COSA_DML_WanAddrMode_Static;
    /*
    else if(!_ansc_strcmp(wanProto, "dhalip"))
        *pMode = COSA_DML_WanAddrMode_DHALIP;
    */
    
    Utopia_Free(&ctx, 0);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetWanStaticIPAddress
    (
        ANSC_HANDLE                 hContext,
        ULONG           *ipAddr
    )
{
    UtopiaContext ctx;
    char buf[16];

    if (!Utopia_Init(&ctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }

    Utopia_Get(&ctx, UtopiaValue_WAN_IPAddr, buf, sizeof(buf));
    inet_pton(AF_INET, buf, ipAddr);

    Utopia_Free(&ctx, 0);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS 
CosaDmlDcSetReInitCmMac ()
{

    if(cm_hal_ReinitMac() == 0)
    {
       return ANSC_STATUS_SUCCESS;
    }
    else
    {
       return ANSC_STATUS_FAILURE;
    }
    
}
ANSC_STATUS
CosaDmlDcGetWanStaticSubnetMask
    (
        ANSC_HANDLE                 hContext,
        ULONG           *ipAddr
    )
{
    UtopiaContext ctx;
    char buf[16];

    if (!Utopia_Init(&ctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }

    Utopia_Get(&ctx, UtopiaValue_WAN_Netmask, buf, sizeof(buf));
    inet_pton(AF_INET, buf, ipAddr);

    Utopia_Free(&ctx, 0);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetWanStaticGatewayIP
    (
        ANSC_HANDLE                 hContext,
        ULONG           *ipAddr
    )
{
    UtopiaContext ctx;
    char buf[16];

    if (!Utopia_Init(&ctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }

    Utopia_Get(&ctx, UtopiaValue_WAN_DefaultGateway, buf, sizeof(buf));
    inet_pton(AF_INET, buf, ipAddr);

    Utopia_Free(&ctx, 0);

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlDcGetWanSecondIPAddr
    (
        ANSC_HANDLE                 hContext,
        ULONG           *ipAddr
    )
{
//    *ipAddr = g_DcWanDns.SecIPAddr;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetWanSecondIPRipAdvertised
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    )
{
//    *pFlag = g_DcWanDns.SecIPRIPAdv;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetWanBackupDefaultGateway
    (
        ANSC_HANDLE                 hContext,
        ULONG           *ipAddr
    )
{
//    *ipAddr = g_DcWanDns.BackupGateway;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetWanNameServer
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *ipAddr,
        int                         nameServerNo
    )
{
    UtopiaContext ctx;
    char buf[16];

    if (!Utopia_Init(&ctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }

    Utopia_Get(&ctx, nameServerNo == 1 ? UtopiaValue_WAN_NameServer1 : UtopiaValue_WAN_NameServer2, buf, sizeof(buf));
    inet_pton(AF_INET, buf, ipAddr);

    Utopia_Free(&ctx, 0);

    return ANSC_STATUS_SUCCESS;
}

#include <string.h>
#include <ctype.h>

static char   g_HostName[1024] = {0};

ANSC_STATUS
CosaDmlDcGetWanHostName
    (
        ANSC_HANDLE                 hContext,
        char                        *pHostName
    )
{

#if 0
    UtopiaContext ctx;
    PCOSA_DATAMODEL_DEVICECONTROL pDc;

    if (!Utopia_Init(&ctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }
    
    Utopia_Get(&ctx, UtopiaValue_HostName, pHostName, sizeof(pDc->HostName));
    
    Utopia_Free(&ctx, 0);
#endif

    FILE *hostName = fopen("/etc/hostname", "r");
    char buf[1024] = {'\0'};

    if(hostName == NULL) {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Failed to open /etc/hostname !!!\n" ));
        return ANSC_STATUS_FAILURE;
    }

    fgets(buf, sizeof(buf), hostName);

    if(buf[strlen(buf)-1] == '\n')
        buf[strlen(buf)-1] = '\0';

    strcpy(pHostName, buf);

    strcpy(g_HostName, buf);

    fclose(hostName);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetWanDomainName
    (
        ANSC_HANDLE                 hContext,
        char                        *pDomainName
    )
{
#if 0
    UtopiaContext ctx;
    PCOSA_DATAMODEL_DEVICECONTROL pDc;

    if (!Utopia_Init(&ctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }
    
    Utopia_Get(&ctx, UtopiaValue_WAN_ProtoAuthDomain, pDomainName, sizeof(pDc->DomainName));
    
    Utopia_Free(&ctx, 0);
#endif

    FILE *resolvConf = fopen("/etc/resolv.conf", "r");
    char buf[1024] = {'\0'};
    char *domainName;

    if(resolvConf == NULL) {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Failed to open /etc/resolv.conf !!!\n" ));
        return ANSC_STATUS_FAILURE;
    }

    while(fgets(buf, sizeof(buf), resolvConf) != NULL) {
        if((domainName = strstr(buf, "search")) != NULL) {
            domainName += strlen("search");

            while(isspace(*domainName))
                domainName++;

            if(domainName[strlen(domainName)-1] == '\n')
                domainName[strlen(domainName)-1] = '\0';

            strncpy(pDomainName, domainName, strlen(domainName));
            pDomainName[strlen(domainName)] = '\0';
        }
    }

    fclose(resolvConf);
    return ANSC_STATUS_SUCCESS;
}

/*This interface is just used for getting wan static domain name which is set by CosaDmlDcSetDomainName().
 *CosaDmlDcGetDomainName() gets the current validated domain name*/
ANSC_STATUS
CosaDmlDcGetWanStaticDomainName
    (
        ANSC_HANDLE                 hContext,
        char                        *pStaticDomainName
    )
{
    UtopiaContext ctx;
    PCOSA_DATAMODEL_DEVICECONTROL pDc;

    if (!Utopia_Init(&ctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }
    
    Utopia_Get(&ctx, UtopiaValue_WAN_ProtoAuthDomain, pStaticDomainName, sizeof(pDc->StaticDomainName));
    
    Utopia_Free(&ctx, 0);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetWanAddressMode
    (
        ANSC_HANDLE                 hContext,
        COSA_DML_WanAddrMode        mode
    )
{
    UtopiaContext utctx;

    if (!Utopia_Init(&utctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }

    switch(mode)
    {
        case COSA_DML_WanAddrMode_DHCP:
            Utopia_Set(&utctx, UtopiaValue_WAN_Proto, "dhcp");
            break;
        case COSA_DML_WanAddrMode_Static:
            Utopia_Set(&utctx, UtopiaValue_WAN_Proto, "static");
            break;
        /*
        case COSA_DML_WanAddrMode_DHALIP:
            Utopia_Set(&utctx, UtopiaValue_WAN_Proto, "dhalip");
            break;
        */
        default:
            Utopia_Set(&utctx, UtopiaValue_WAN_Proto, "dhcp");
            break;
    }
    
    Utopia_Free(&utctx, 1);
	
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetWanStaticIPAddress
    (
        ANSC_HANDLE                 hContext,
        ULONG           ipAddr
    )
{
    UtopiaContext utctx;
    char buf[256];

    if (!Utopia_Init(&utctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }

    _ansc_sprintf(buf, "%d.%d.%d.%d", ((ANSC_IPV4_ADDRESS)ipAddr).Dot[0], ((ANSC_IPV4_ADDRESS)ipAddr).Dot[1], ((ANSC_IPV4_ADDRESS)ipAddr).Dot[2], ((ANSC_IPV4_ADDRESS)ipAddr).Dot[3] );

    Utopia_Set(&utctx, UtopiaValue_WAN_IPAddr, buf);

    Utopia_Free(&utctx, 1);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetWanStaticSubnetMask
    (
        ANSC_HANDLE                 hContext,
        ULONG           ipAddr
    )
{
    UtopiaContext utctx;
    char buf[256];

    if (!Utopia_Init(&utctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }

    _ansc_sprintf(buf, "%d.%d.%d.%d", ((ANSC_IPV4_ADDRESS)ipAddr).Dot[0], ((ANSC_IPV4_ADDRESS)ipAddr).Dot[1], ((ANSC_IPV4_ADDRESS)ipAddr).Dot[2], ((ANSC_IPV4_ADDRESS)ipAddr).Dot[3] );

    Utopia_Set(&utctx, UtopiaValue_WAN_Netmask, buf);
	
    Utopia_Free(&utctx, 1);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetWanStaticGatewayIP
    (
        ANSC_HANDLE                 hContext,
        ULONG           ipAddr
    )
{
    UtopiaContext utctx;
    char buf[256];

    if (!Utopia_Init(&utctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }

    _ansc_sprintf(buf, "%d.%d.%d.%d", ((ANSC_IPV4_ADDRESS)ipAddr).Dot[0], ((ANSC_IPV4_ADDRESS)ipAddr).Dot[1], ((ANSC_IPV4_ADDRESS)ipAddr).Dot[2], ((ANSC_IPV4_ADDRESS)ipAddr).Dot[3] );

    Utopia_Set(&utctx, UtopiaValue_WAN_DefaultGateway, buf);

    Utopia_Free(&utctx, 1);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetWanSecondIPAddr
    (
        ANSC_HANDLE                 hContext,
        ULONG           ipAddr
    )
{
//    g_DcWanDns.SecIPAddr = ipAddr;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetWanSecondIPRipAdvertised
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     Flag
    )
{
//    g_DcWanDns.SecIPRIPAdv = Flag;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetWanBackupDefaultGateway
    (
        ANSC_HANDLE                 hContext,
        ULONG           ipAddr
    )
{
//    g_DcWanDns.BackupGateway = ipAddr;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetWanNameServer
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ipAddr,
        int                         nameServerNo
    )
{
    UtopiaContext utctx;
    char buf[256];

    if (!Utopia_Init(&utctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }

    _ansc_sprintf(buf, "%d.%d.%d.%d", ((ANSC_IPV4_ADDRESS)ipAddr).Dot[0], ((ANSC_IPV4_ADDRESS)ipAddr).Dot[1], ((ANSC_IPV4_ADDRESS)ipAddr).Dot[2], ((ANSC_IPV4_ADDRESS)ipAddr).Dot[3] );

    Utopia_Set(&utctx, nameServerNo == 1 ? UtopiaValue_WAN_NameServer1 : UtopiaValue_WAN_NameServer2, buf);
	
    Utopia_Free(&utctx, 1);
    
    if (vsystem("/bin/sh /etc/utopia/service.d/set_resolv_conf.sh") != 0) {
        fprintf(stderr, "%s: fail to set resolv.conf\n", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetHostName
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    UtopiaContext utctx;

    if (!Utopia_Init(&utctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }
	
    Utopia_Set(&utctx, UtopiaValue_HostName, pValue);
	
    Utopia_Free(&utctx,1);

    if (vsystem("/bin/sh /etc/utopia/service.d/set_hostname.sh") != 0) {
        fprintf(stderr, "%s: fail to set resolv.conf\n", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    if ( !g_HostName[0] )
    {
        CosaDmlDcGetWanHostName(hContext, g_HostName);
    }

    if (g_HostName[0] && strcmp(g_HostName, pValue))
    {
        commonSyseventSet("wan-restart", "");
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetDomainName
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    UtopiaContext utctx;

    if (!Utopia_Init(&utctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }
	
    Utopia_Set(&utctx, UtopiaValue_WAN_ProtoAuthDomain, pValue);
	
    Utopia_Free(&utctx,1);

    if (vsystem("/bin/sh /etc/utopia/service.d/set_wandomain.sh") != 0) {
        fprintf(stderr, "%s: fail to set wan domain name\n", __FUNCTION__);
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetResetDefaultEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    )
{
    *pFlag = FALSE;
    return ANSC_STATUS_SUCCESS;
}

/* saRgDeviceConfigSnmpEnable */
enum snmpenable_e {
    RG_WAN = 0,
    RG_DUALIP,
    RG_LANIP
};

const char *snmpenable_str[] = {
    [RG_WAN]    = "rgWan",
    [RG_DUALIP] = "rgDualIp",
    [RG_LANIP]  = "rgLanIp"
};


ANSC_STATUS
CosaDmlDcGetSNMPEnable
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    int bAllowed = 0;

    if (pValue == NULL) {
        AnscTraceError(("[SNMPEnable]: pValue pointer is null.\n"));
        return ANSC_STATUS_FAILURE;
    }

    if ( platform_hal_GetSNMPEnable(pValue) != RETURN_OK )
        return ANSC_STATUS_FAILURE;
    else    
        return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetSNMPEnable
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    if (pValue == NULL) {
        AnscTraceError(("[SNMPEnable]: pValue pointer is null.\n"));
        return ANSC_STATUS_FAILURE;
    }

    if ( platform_hal_SetSNMPEnable(pValue) != RETURN_OK )
        return ANSC_STATUS_FAILURE;
    else    
        return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetRebootDevice
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    AnscCopyString(pValue, "");
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetFactoryReset
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    AnscCopyString(pValue, "");
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetUserChangedFlags
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    AnscCopyString(pValue, "0");
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetDeviceConfigStatus
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    if(platform_hal_GetDeviceConfigStatus(pValue) != RETURN_OK)
        return ANSC_STATUS_FAILURE;
    else
        return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetDeviceConfigIgnore
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
   AnscCopyString(pValue, " notRequire");/*need to modfiy @ivan*/
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetDeviceConfigIgnore
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    return ANSC_STATUS_SUCCESS;
}

static ANSC_STATUS
CosaDmlDcRebootWifi(ANSC_HANDLE   hContext)
{
	CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
	fprintf(stderr, "WiFi is going to reboot\n");
	pthread_detach(pthread_self());
	CcspTraceWarning(("RebootDevice:WiFi is going to reboot now\n"));
	int                         ret;
	int                         size = 0;
	componentStruct_t **        ppComponents = NULL;
	char*   faultParam = NULL;

	sprintf(dst_pathname_cr, "%s%s", g_Subsystem, CCSP_DBUS_INTERFACE_CR);

	ret = CcspBaseIf_discComponentSupportingNamespace(bus_handle,
		    dst_pathname_cr,
//		    "Device.WiFi.Radio.",
                    "Device.WiFi.",
		    g_Subsystem,        /* prefix */
		    &ppComponents,
		    &size);

	if ( ret == CCSP_SUCCESS && size == 1)
	{
//	    parameterValStruct_t val[2] = { { "Device.WiFi.Radio.1.X_CISCO_COM_ApplySetting", "true", ccsp_boolean}, 
//				            { "Device.WiFi.Radio.2.X_CISCO_COM_ApplySetting", "true", ccsp_boolean} };
            parameterValStruct_t val[1] = { { "Device.WiFi.X_CISCO_COM_ResetRadios", "true", ccsp_boolean}};
 
	    ret = CcspBaseIf_setParameterValues
				(
					bus_handle, 
					ppComponents[0]->componentName, 
					ppComponents[0]->dbusPath,
					0, 0x0,   /* session id and write id */
					&val, 
//					2,
                                        1, 
					TRUE,   /* no commit */
					&faultParam
				);	

		if (ret != CCSP_SUCCESS && faultParam)
		{
		    CcspTraceError(("RebootDevice:%s Failed to SetValue for param '%s'\n",__FUNCTION__,faultParam));
		    bus_info->freefunc(faultParam);
		} else {

			char buf[7] = {0};
			int wifiresetcount = 0;
			syscfg_get( NULL, "wifi_reset_count", buf, sizeof(buf));
			wifiresetcount = atoi(buf);
			wifiresetcount++;
			memset(buf,0,sizeof(buf));
			snprintf(buf,sizeof(buf),"%d",wifiresetcount);
			syscfg_set(NULL, "wifi_reset_count", buf);

			FILE *fp = NULL;
			memset(buf,0,sizeof(buf));
			sprintf(buf, "date");
			char buffer[50] = {0};
			memset(buffer,0,sizeof(buffer));
			fp = popen(buf, "r");
			if( fp != NULL) {         
			while(fgets(buffer, sizeof(buffer), fp)!=NULL){
				buffer[strlen(buffer) - 1] = '\0';
				syscfg_set(NULL, "latest_wifi_reset_time", buffer);
			}
				pclose(fp);
			}

			if (syscfg_commit() != 0) 
			{
				CcspTraceWarning(("syscfg_commit failed\n"));
			}
		    CcspTraceWarning(("WIFI_RESET_COUNT : %d Time : %s  \n",wifiresetcount,buffer));
		}
	    free_componentStruct_t(bus_handle, size, ppComponents);
	}
	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcResetBr0(char *ip, char *sub) {
	int ret;
	
	char objName[256]="Device.WiFi.X_RDKCENTRAL-COM_Br0_Sync";
	char objValue[256]={0};
	parameterValStruct_t  value[1] = { objName, objValue, ccsp_string};
	
	char dst_pathname_cr[64]  =  {0};
	componentStruct_t **        ppComponents = NULL;
	int size =0;
	
	CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
	char* faultParam = NULL;
    snprintf(objValue, sizeof(objValue), "%s,%s", ip, sub);
	fprintf(stderr, "-- %s %d %s\n", __func__, __LINE__, objValue);
	
	snprintf(dst_pathname_cr, sizeof(dst_pathname_cr), "%s%s", g_Subsystem, CCSP_DBUS_INTERFACE_CR);
	ret = CcspBaseIf_discComponentSupportingNamespace(
				bus_handle,
				dst_pathname_cr,
				objName,
				g_Subsystem,        /* prefix */
				&ppComponents,
				&size);
	if ( ret != CCSP_SUCCESS ) {
		fprintf(stderr, "Error:'%s' is not exist\n", objName);
		return ANSC_STATUS_FAILURE;
	}	

	ret = CcspBaseIf_setParameterValues(
				bus_handle,
				ppComponents[0]->componentName,
				ppComponents[0]->dbusPath,
				0, 0x0,   /* session id and write id */
				&value,
				1,
				TRUE,   /* no commit */
				&faultParam
			);

	if (ret != CCSP_SUCCESS && faultParam) {
		fprintf(stderr, "Error:Failed to SetValue for param '%s'\n", faultParam);
		bus_info->freefunc(faultParam);
	}
	free_componentStruct_t(bus_handle, 1, ppComponents);
	return ANSC_STATUS_SUCCESS;
}

static int openCommonSyseventConnection() {
    if (commonSyseventFd == -1) {
        commonSyseventFd = s_sysevent_connect(&commonSyseventToken);
    }
    return 0;
}

static ANSC_STATUS CosaDmlDcRestartRouter()
{
    char statusValue[256] = {0};
    int count = 0;
    pthread_detach(pthread_self());
    system("sysevent set lan-stop");

	sleep(3);
    system("sysevent set lan_restarted true");
	system("sysevent set forwarding-restart");
#if 0
    while(1) {

        if(commonSyseventFd == -1) {
            openCommonSyseventConnection();
        }
        sleep(3);
        /*lan-status event*/
        if( 0 == sysevent_get(commonSyseventFd, commonSyseventToken, "lan-status", statusValue, sizeof(statusValue)) && '\0' != statusValue[0] )
        {
            if (0 == strncmp(statusValue, "stopped", strlen("stopped"))) {
                CcspTraceWarning(("Lan is restarting \n"));
                system("sysevent set forwarding-restart");
                break;
            }
        }
        if(count > 5) {
            CcspTraceWarning(("Lan Restart Time Out\n"));
            break;
        }
        count++;            
    } 
#endif 
    return ANSC_STATUS_SUCCESS;      
}

ANSC_STATUS
CosaDmlDcSetRebootDevice
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    int router, wifi, voip, dect, moca, all, delay;
    int delay_time = 0;

    router = wifi = voip = dect = moca = all = delay = 0;
    if (strstr(pValue, "Router") != NULL) {
        router = 1;
    }
    if (strstr(pValue, "Wifi") != NULL) {
        wifi = 1;
    }
    if (strstr(pValue, "VoIP") != NULL) {
        voip = 1;
    }
    if (strstr(pValue, "Dect") != NULL) {
        dect = 1;
    }
    if (strstr(pValue, "MoCA") != NULL) {
        moca = 1;
    }
    if (strstr(pValue, "Device") != NULL) {
        all = 1;
    }
    if (strstr(pValue, "delay") != NULL) {
        delay = 1;
    }
    if (strstr(pValue, "delay=") != NULL) {
        delay_time = atoi(strstr(pValue, "delay=") + strlen("delay="));
    }

    if (router && wifi && voip && dect && moca) {
        all = 1;
    }

    if (all) {

		char buf[7] = {0};
		int rebootcount = 0;
    	syscfg_get( NULL, "reboot_count", buf, sizeof(buf));
		rebootcount = atoi(buf);
		rebootcount++;
		memset(buf,0,sizeof(buf));
		snprintf(buf,sizeof(buf),"%d",rebootcount);
	syscfg_set(NULL, "reboot_count", buf);

		FILE *fp = NULL;
		memset(buf,0,sizeof(buf));
		sprintf(buf, "date");
		char buffer[50] = {0};
		memset(buffer,0,sizeof(buffer));
        fp = popen(buf, "r");
		if( fp != NULL) {         
		    while(fgets(buffer, sizeof(buffer), fp)!=NULL){
			    buffer[strlen(buffer) - 1] = '\0';
				syscfg_set(NULL, "latest_reboot_time", buffer);
			}
			pclose(fp);
		}

		char tmp[7] = {0};
		syscfg_get(NULL, "restore_reboot", tmp, sizeof(tmp));

		if(strcmp(tmp,"true") != 0)
		{
			if (syscfg_commit() != 0)
			{
				CcspTraceWarning(("syscfg_commit failed\n"));
			}
		}
		else
		{
			CcspTraceWarning(("RebootDevice:Device is going to reboot to restore configuration \n"));
		}
		
		CcspTraceWarning(("REBOOT_COUNT : %d Time : %s  \n",rebootcount,buffer));

        if(delay) {
            if(delay_time)
            {
            	fprintf(stderr, "Device is going to reboot in %d seconds\n", delay_time);
		CcspTraceWarning(("RebootDevice:Device is going to reboot after taking log backups \n"));
            	//system("(sleep 5 && reboot) &");
            	sleep (delay_time);
            	system("/fss/gw/rdklogger/backupLogs.sh &");
        	}
        	else
            {
                fprintf(stderr, "Device is going to reboot in 5 seconds\n");
		CcspTraceWarning(("RebootDevice:Device is going to reboot after taking log backups \n"));
                //system("(sleep 5 && reboot) &");
				sleep(5);
				system("/fss/gw/rdklogger/backupLogs.sh &");
            }
		}
		else {
	        fprintf(stderr, "Device is going to reboot now\n");
			CcspTraceWarning(("RebootDevice:Device is going to reboot after taking log backups \n"));
	         //system("reboot");
	         system("/fss/gw/rdklogger/backupLogs.sh &");
	    }
    }

    if (router) {
        fprintf(stderr, "Router is going to reboot\n");
		CcspTraceWarning(("RebootDevice:Router is going to reboot\n"));
		
		/*Since two sysevents are issued simultaneously , lan is stopped but forwarding-restart is not bringing up. 
		Fix added is for waiting till lan-stop is done , then issuing command to bring up*/
		//system("sysevent set lan-stop");
		//system("sysevent set forwarding-restart");
		pthread_t tid2;
		pthread_create(&tid2, NULL, &CosaDmlDcRestartRouter, NULL); 
    }

    if (wifi) {
        pthread_t tid;
	CcspTraceWarning(("RebootDevice:CosaDmlDcRebootWifi thread called to reboot WiFi\n"));
   	pthread_create(&tid, NULL, &CosaDmlDcRebootWifi, NULL);
    }
    
    if (voip) {
        fprintf(stderr, "VoIP is going to reboot\n");
        // TODO: 
    }
    if (dect) {
        fprintf(stderr, "Dect is going to reboot\n");
        // TODO: 
    }
    if (moca) {
        fprintf(stderr, "MoCA is going to reboot\n");
        // TODO: 
    }
    
    return ANSC_STATUS_SUCCESS;
}

void restoreAllDBs()
{

	pthread_detach(pthread_self());
	CcspTraceWarning(("FactoryReset:%s in thread  Restoring all the DBs to factory defaults  ...\n",__FUNCTION__));
	system("rm -f /nvram/TLVData.bin"); //Need to remove TR69 TLV data.
	system("rm -f /nvram/reverted"); //Need to remove redirection reverted flag

	// We have syscfg running on the ATOM side when mesh is running. We need to clear out the
    // syscfg.db on the ATOM side during factory reset.
#if defined(_COSA_INTEL_USG_ARM_)
    {
        #define DATA_SIZE 1024
        FILE *fp1;
        char buf[DATA_SIZE] = {0};
        char *urlPtr = NULL;

        // Grab the ATOM RPC IP address
        // sprintf(cmd1, "cat /etc/device.properties | grep ATOM_ARPING_IP | cut -f 2 -d\"=\"");

        fp1 = fopen("/etc/device.properties", "r");
        if (fp1 == NULL) {
            CcspTraceError(("Error opening properties file! \n"));
            return FALSE;
        }

        while (fgets(buf, DATA_SIZE, fp1) != NULL) {
            // Look for ATOM_ARPING_IP
            if (strstr(buf, "ATOM_ARPING_IP") != NULL) {
                buf[strcspn(buf, "\r\n")] = 0; // Strip off any carriage returns

                // grab URL from string
                urlPtr = strstr(buf, "=");
                urlPtr++;
                break;
            }
        }

        if (fclose(fp1) != 0) {
            /* Error reported by pclose() */
            CcspTraceError(("Error closing properties file! \n"));
        }

        if (urlPtr != NULL && urlPtr[0] != 0 && strlen(urlPtr) > 0) {
            CcspTraceInfo(("Reported an ATOM IP of %s \n", urlPtr));
            pid_t pid = fork();

            if (pid == -1)
            {
                // error, failed to fork()
            }
            else if (pid > 0)
            {
                int status;
                waitpid(pid, &status, 0); // wait here until the child completes
            }
            else
            {
                // we are the child

                char *args[] = {"/fss/gw/usr/bin/rpcclient", urlPtr, "/bin/rm -f /nvram/syscfg.db", (char *) 0 };
                execv(args[0], args);
                _exit(EXIT_FAILURE);   // exec never returns
            }
        }
    }
#endif

#if defined (_XB6_PRODUCT_REQ_)
	system("rm -f /nvram/syscfg.db");
#endif
#if defined (_CBR_PRODUCT_REQ_)
	system("rm -f /data/nvram");
	system("rm -f /data/country_code_*");
	system("touch /tmp/wifi_factory_reset");
        /* Remove maintenance window data on factory reset */
        system("rm -f /nvram/.FirmwareUpgradeEndTime");
        system("rm -f /nvram/.FirmwareUpgradeStartTime");
#endif
#if defined (_COSA_BCM_ARM_)
	/* Clear cable modem's dynamic nonvol settings */
	system("rm -f /mnt/cmnonvol/cm_dyn.bin");
#endif
	system("restoreAllDBs"); //Perform factory reset on other components
	return;
}

void backuplogs()
{
	pthread_detach(pthread_self());
	system("/fss/gw/rdklogger/backupLogs.sh &");
}

/*****************************************
*
*  pValue - Comma delimited string indicating which RG modules
*           should be returned to their factory default settings.
*           Modified during the call.
*
*           Values: "Router", "Wifi", "" or NULL
*
*  Returns ANSC_STATUS_BAD_PARAMETER if a valid value is not given in pValue
*
*  DEPENDENCY: UTCTX
*
*  Algorithm: Set syscfg value and perform factory reset
*             on next boot.
******************************************/
ANSC_STATUS
CosaDmlDcSetFactoryReset
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
	char* tok;
	char* sv;
    char value[50];
	int factory_reset_mask = 0;
	UtopiaContext utctx = {0};
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
	
#if defined (_XB6_PRODUCT_REQ_)
	int delay_time = 0;
	int delay = 0;
#endif

	if (pValue == NULL || pValue[0] == '\0')
		factory_reset_mask |= FR_NONE;
	else {
		strncpy(value, pValue, sizeof(value));
		tok = strtok_r(value, ",", &sv);

		while (tok) {
			if (strcmp("Router", tok) == 0) {
				factory_reset_mask |= FR_ROUTER;
			} else if (strcmp("Wifi", tok) == 0) {
				factory_reset_mask |= FR_WIFI;
			} else if(strcmp("Firewall", tok) == 0) {
				factory_reset_mask |= FR_FW;
			} else if(strcmp("VoIP", tok) == 0 || strcmp("Docsis", tok) == 0) {
				factory_reset_mask |= FR_OTHER;
			}

			tok = strtok_r(NULL, ",", &sv);
		}
		
#if defined (_XB6_PRODUCT_REQ_)
		//XB6 Needs Time for SNMP packet to get out of device from Atom 
		if (strstr(pValue, "delay") != NULL) {
			delay = 1;
		}
		if (strstr(pValue, "delay=") != NULL) {
			delay_time = atoi(strstr(pValue, "delay=") + strlen("delay="));
		}
		
		if(delay) {
			if(delay_time)
			{
				fprintf(stderr, "CosaDmlDcSetFactoryReset: is going to wait for %d seconds\n", delay_time);
				CcspTraceWarning(("CosaDmlDcSetFactoryReset: is going to wait for %d seconds\n", delay_time));
            	sleep (delay_time);
			}
			else
			{
				fprintf(stderr, "CosaDmlDcSetFactoryReset: is going to wait for 5 seconds\n");
				CcspTraceWarning(("CosaDmlDcSetFactoryReset: is going to wait for 5 seconds\n"));
				sleep(5);
			}
		}
#endif
		
	}

	if (!factory_reset_mask)
	    {
	   	CcspTraceError(("FactoryReset:%s BAD parameter passed to factory defaults parameter ...\n",__FUNCTION__));
		return ANSC_STATUS_BAD_PARAMETER;
	    }

    if (factory_reset_mask & FR_FW) {
        int rc = -1;
        UtopiaContext ctx;
        firewall_t fw;
   		CcspTraceWarning(("FactoryReset:%s Resetting Firewall to factory defaults ...\n",__FUNCTION__));
        fwSync = 1; //inform middle layer to get data from backend not cache

        if (!Utopia_Init(&ctx))
        {
            CcspTraceWarning(("X_CISCO_SECURITY: Error in initializing context!!! \n" ));
            return ANSC_STATUS_FAILURE;
        }

        fw.filter_ident = 0;
        fw.filter_multicast = 0;
        fw.filter_anon_req = 0;
        fw.filter_p2p_from_wan = 0;
        fw.filter_http_from_wan = 0;

        Utopia_Set(&ctx, UtopiaValue_Firewall_Level, "Low");
        fw.spi_protection = 1;

        rc = Utopia_SetFirewallSettings(&ctx, fw);

        Utopia_Free(&ctx, !rc);

        if(rc != 0)
            return ANSC_STATUS_FAILURE;
        else
            commonSyseventSet("firewall-restart", "");
    }

	if (factory_reset_mask & FR_OTHER ) {
   	//	CcspTraceWarning(("FactoryReset:%s Restoring all the DBs to factory defaults  ...\n",__FUNCTION__));
       // system("rm -f /nvram/TLVData.bin"); //Need to remove TR69 TLV data.
	//	system("rm -f /nvram/reverted"); //Need to remove redirection reverted flag
	//	system("restoreAllDBs"); //Perform factory reset on other components
	pthread_t other;
        pthread_create(&other, NULL, &restoreAllDBs, NULL);
	}

	if (factory_reset_mask & FR_ROUTER) {
   		CcspTraceWarning(("FactoryReset:%s Restoring router to factory defaults  ...\n",__FUNCTION__));
		tok = FACTORY_RESET_ROUTER_VALUE;
		if (!Utopia_Init(&utctx))
		{
        		CcspTraceWarning(("FactoryReset: Error in initializing context during router factory reset!!! \n" ));
			return ANSC_STATUS_FAILURE;              
		}
	
		if(!Utopia_RawSet(&utctx, NULL, FACTORY_RESET_KEY, tok)) {
			Utopia_Free(&utctx,0);
      		CcspTraceWarning(("FactoryReset: Error in setting FACTORY_RESET_KEY!!! \n" ));
			return ANSC_STATUS_FAILURE;
		}
		
		Utopia_Free(&utctx,1);
		//system("reboot");i
		pthread_t logs;
        pthread_create(&logs, NULL, &backuplogs, NULL);
	//	system("/fss/gw/rdklogger/backupLogs.sh");
	} 
	if (factory_reset_mask & FR_WIFI) {
		/*TODO: SEND EVENT TO WIFI PAM  Device.WiFi.X_CISCO_COM_FactoryReset*/
        int                         ret;
        char* faultParam = NULL;

      	CcspTraceWarning(("FactoryReset:%s Restoring WiFi to factory defaults  ...\n",__FUNCTION__));     
            if (ppComponents == NULL && initWifiComp())
		{
    		  	CcspTraceError(("FactoryReset:%s Restoring WiFi to factory defaults returned error  ...\n",__FUNCTION__));     
			
                	return ANSC_STATUS_FAILURE;
            	}
            parameterValStruct_t		val = { "Device.WiFi.X_CISCO_COM_FactoryReset", "true", ccsp_boolean};
            
            ret = CcspBaseIf_setParameterValues
				(
					bus_handle, 
					ppComponents[0]->componentName, 
					ppComponents[0]->dbusPath,
					0, 0x0,   /* session id and write id */
					&val, 
					1, 
					TRUE,   /* no commit */
					&faultParam
				);	
                
                if (ret != CCSP_SUCCESS && faultParam)
                {
					CcspTraceError(("FactoryReset:%s SettingX_CISCO_COM_FactoryReset returned error for param '%s'  ...\n",__FUNCTION__,faultParam));  
                    bus_info->freefunc(faultParam);
                }
#if defined (_XB6_PRODUCT_REQ_)
            faultParam = NULL;
            parameterValStruct_t val1 = { "Device.WiFi.X_CISCO_COM_FactoryResetRadioAndAp", "1,2;1,2", ccsp_string};
            ret = CcspBaseIf_setParameterValues
                                (
                                        bus_handle,
                                        ppComponents[0]->componentName,
                                        ppComponents[0]->dbusPath,
                                        0, 0x0,   /* session id and write id */
                                        &val1,
                                        1,
                                        TRUE,   /* no commit */
                                        &faultParam
                                );

             if (ret != CCSP_SUCCESS && faultParam)
             {
                    CcspTraceError(("FactoryReset:%s Setting X_CISCO_COM_FactoryResetRadioAndAp returned error for param '%s'  ...\n",__FUNCTION__,faultParam));
                    bus_info->freefunc(faultParam);
             }
#endif
#if 0
		FILE *fp;
		char command[30];

		 memset(command,0,sizeof(command));
		sprintf(command, "ls /tmp/*walledgarden*");
		char buffer[50];
		 memset(buffer,0,sizeof(buffer));
                if(!(fp = popen(command, "r"))){
                           exit(1);
                    }
	        while(fgets(buffer, sizeof(buffer), fp)!=NULL){
		        buffer[strlen(buffer) - 1] = '\0';
    }

if ( strlen(buffer) == 0 )
{
		pthread_t captive;
		pthread_create(&captive, NULL, &configWifi, NULL);

}
pclose(fp);
#endif
//            free_componentStruct_t(bus_handle, size, ppComponents);
            return  ANSC_STATUS_SUCCESS;
        
        //system("reboot");
	} 
	if (factory_reset_mask & FR_NONE){
		
	}	

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetUserChangedFlags
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetResetDefaultEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     pFlag
    )
{
    return ANSC_STATUS_SUCCESS;
}

void _CosaDmlDcStartZeroConfig()
{
    FILE    *fileHandle  = NULL;
    int      i           = 0;

    AnscTraceWarning(("_CosaDmlDcStartZeroConfig -- start avahi.\n"));

    /* If configuration file doesn't exist, create it firstly. */
    fileHandle = fopen(CONFIG_AVAHI_DAEMON_FILENAME, "r" );
    /*fileHandle = fopen("/home/yali3/avahi-daemon.conf", "r" );*/

    if ( !fileHandle )
    {
        fileHandle = fopen(CONFIG_AVAHI_DAEMON_FILENAME, "w+" );
        /*fileHandle = fopen("/home/yali3/avahi-daemon.conf", "w+" );*/

        if (!fileHandle)
        {
            AnscTraceWarning(("_CosaDmlDcStartZeroConfig -- create file:%s, error.\n", CONFIG_AVAHI_DAEMON_FILENAME));
            return;
        }

        do
        {
            fputs(g_avahi_daemon_conf[i], fileHandle);
            fputs("\n", fileHandle);
        }while(g_avahi_daemon_conf[++i]);
    }

    fclose(fileHandle);

    /* Start two daemon */
    system(CMD_START_AVAHI_DAEMON);
    system(CMD_START_AVAHI_AUTOIPD);

    return;
}

void _CosaDmlDcStopZeroConfig()
{
    /* kill two daemon no matter whether it's successful. */
    AnscTraceWarning(("_CosaDmlDcStopZeroConfig -- stop avahi.\n"));
    
    system(CMD_STOP_AVAHI_DAEMON);
    system(CMD_STOP_AVAHI_AUTOIPD);
    
    return;
}

ANSC_STATUS
CosaDmlDcGetEnableStaticNameServer
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                    *pFlag
    )
{
    UtopiaContext ctx;
    boolean_t bEnabled;

    if (!Utopia_Init(&ctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }
    
    Utopia_GetBool(&ctx, UtopiaValue_WAN_EnableStaticNameServer, &bEnabled);
    *pFlag = bEnabled ? TRUE : FALSE;

    Utopia_Free(&ctx, 0);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetEnableStaticNameServer
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bFlag
    )
{
    UtopiaContext ctx;
    boolean_t bEnabled = bFlag;

    if (!Utopia_Init(&ctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }

    Utopia_SetBool(&ctx, UtopiaValue_WAN_EnableStaticNameServer, bEnabled);

    //get nameservers by DHCP if static nameservers not enabled on GUI
    if(!bFlag)
        commonSyseventSet("wan-restart", "");
    else
        if (vsystem("/bin/sh /etc/utopia/service.d/set_resolv_conf.sh") != 0) {
            fprintf(stderr, "%s: fail to set resolv.conf\n", __FUNCTION__);
            return ANSC_STATUS_FAILURE;
        }

    Utopia_Free(&ctx, 1);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetReleaseWan
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                    *pFlag
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetReleaseWan
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bFlag
    )
{
    system("sysevent set dhcp_client-release");
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetRenewWan
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                    *pFlag
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetRenewWan
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bFlag
    )
{
    char status[10] = {'\0'};
    int timeOut = 15;

    system("sysevent set dhcp_client-renew");
    sleep(3);
    
    do {
        sleep(1);
        commonSyseventGet("wan-status", status, sizeof(status));
        timeOut--;
    } while(strcmp(status, "started") != 0 && timeOut != 0);

    if(timeOut == 0 && strcmp(status, "started") != 0) {
        AnscTraceError(("Error:Timeout in CosaDmlDcSetRenewWan\n"));
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetEnableZeroConfig
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                    *pFlag
    )
{
    *pFlag = g_EnableZeroConfig;

    AnscTraceWarning(("CosaDmlDcSetEnableZeroConfig -- *pFlag:%d.\n", *pFlag));

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetEnableZeroConfig
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bFlag
    )
{
    char     buf[256] = {0};

    UtopiaContext utctx = {0};

    AnscTraceWarning(("CosaDmlDcSetEnableZeroConfig -- bFlag:%d.\n", bFlag));

    if ( g_EnableZeroConfig != bFlag )
    {
        if ( bFlag )
        {
            buf[0] = '1';
        }
        else
        {
            buf[0] = '0';
        }

        if (Utopia_Init(&utctx))
        {
            Utopia_RawSet(&utctx, NULL, UTOPIA_AVAHI_ENABLED, buf);
            Utopia_Free(&utctx,1);                
        }  

        if ( bFlag )
        {
            /*start avahi two applications*/
            _CosaDmlDcStartZeroConfig();

        }
        else
        {
            /*stop avahi two applications*/
            _CosaDmlDcStopZeroConfig();
        }

        g_EnableZeroConfig = bFlag;
    }
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetDeviceMode
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *pValue
    )
{
    char buf[10];
    UtopiaContext utctx = {0};
    
    if (!Utopia_Init(&utctx))
        return ANSC_STATUS_FAILURE;
    
    Utopia_RawGet(&utctx,NULL,"last_erouter_mode",buf,sizeof(buf)-1);
    
    Utopia_Free(&utctx, 0);

    //eRouter mode is 0 based, but maps in same sequence to 1 based data model
    *pValue = atoi(buf) + 1;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS CosaDmlDcSetDeviceMode
    (
        ANSC_HANDLE                 hContext,
        ULONG                       value
    )
{
    char buf[8];
    ULONG oldValue;
   
    /*If TLV202 & saRgDeviceMode are both present in CM config file, only TLV202 can take effect*/
    buf[0] = 0;
    commonSyseventGet("ert_im",buf,sizeof(buf));
    if(atoi(buf)){
        return(ANSC_STATUS_SUCCESS);
    }

    if(CosaDmlDcGetDeviceMode(NULL,&oldValue)!=ANSC_STATUS_SUCCESS)
        return(ANSC_STATUS_FAILURE);
    if(oldValue==value)
        return(ANSC_STATUS_SUCCESS);
    value--;
    snprintf(buf,sizeof(buf),"%d",value);
//     syscfg_set(NULL, "last_erouter_mode", buf);
//     syscfg_commit();
    commonSyseventSet("erouter_mode", buf);
    /*Need to reboot device according to SNMP definition*/
//     printf("<<< Reboot device because of the change of rgDevieMode >>>\n");
//     CosaDmlDcSetRebootDevice(NULL,"Router,Wifi,VoIP,Dect,MoCA,Device,delay");

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetIGMPProxyEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    )
{
    if ( detect_process("igmpproxy") == 0 )
    {
        *pFlag = FALSE;
    }
    else
    {
        *pFlag = TRUE;
    }

    fprintf(stderr, "%s: igmpproxy is %s\n", __FUNCTION__, (*pFlag)==TRUE?"alive":"dead");
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetIGMPProxyEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     pFlag
    )
{
    UtopiaContext utctx = {0};
     
    if (Utopia_Init(&utctx))
    {
        Utopia_RawSet(&utctx, NULL, "igmpproxy_enabled", pFlag?"1":"0");
        
        Utopia_Free(&utctx, 1);
    }

    if (pFlag)
    {
        if ( detect_process("igmpproxy") == 0 )
        {
            vsystem("/fss/gw/bin/igmpproxy &");
        }
    }
    else
    {
        vsystem("killall igmpproxy");
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetDNSProxyEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    )
{
    if ( detect_process("dnsproxy") == 0 )
    {
        *pFlag = FALSE;
    }
    else
    {
        *pFlag = TRUE;
    }

    fprintf(stderr, "%s: dnsproxy is %s\n", __FUNCTION__, (*pFlag)==TRUE?"alive":"dead");
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetDNSProxyEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     pFlag
    )
{
    if (pFlag)
    {
        if ( detect_process("dnsproxy") == 0 )
        {
            vsystem("/fss/gw/bin/dnsproxy &");
        }
    }
    else
    {
        vsystem("killall dnsproxy");
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetTelnetEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    )
{
    char buf[5];
    //printf("Got value mgmt_wan_telnetaccess = %d\n", *pFlag);
    syscfg_get( NULL, "mgmt_wan_telnetaccess", buf, sizeof(buf));
    if( buf != NULL )
    {
        //printf("%s buf = %s\n", __FUNCTION__, buf);
        if (strcmp(buf,"1") == 0)
            *pFlag = 1;
        else
            *pFlag = 0;
    }
 
    if (platform_hal_GetTelnetEnable(pFlag) != RETURN_OK )
        return ANSC_STATUS_FAILURE;
    else
        return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetSSHEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    )
{
    char buf[5];
    syscfg_get( NULL, "mgmt_wan_sshaccess", buf, sizeof(buf));
    if( buf != NULL )
    {
        if (strcmp(buf,"1") == 0) 
            *pFlag = 1;
        else
            *pFlag = 0;
    }
    if (platform_hal_GetSSHEnable(pFlag) != RETURN_OK )
        return ANSC_STATUS_FAILURE;
    else
        return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetHNAPEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    )
{
    *pFlag = TRUE;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetTelnetEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     flag
    )
{
    char buf[5];
    BOOLEAN bTelnetEnable;
    printf("%s \n", __FUNCTION__);
    //printf("%s got flag = %d\n", __FUNCTION__, flag);
    if (CosaDmlDcGetTelnetEnable(NULL, &bTelnetEnable) == ANSC_STATUS_FAILURE)
        return ANSC_STATUS_FAILURE;

    if (flag != bTelnetEnable) {
        snprintf(buf,sizeof(buf),"%d",flag);
        syscfg_set( NULL, "mgmt_wan_telnetaccess", buf);
        syscfg_commit();
        system("sysevent set firewall-restart");

        if (platform_hal_SetTelnetEnable(flag) == RETURN_ERR )
            return ANSC_STATUS_SUCCESS;
    }
    return ANSC_STATUS_SUCCESS;

}

ANSC_STATUS
CosaDmlDcSetSSHEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     flag
    )
{
    char buf[5];
    BOOLEAN bSSHEnable;

    if (CosaDmlDcGetSSHEnable(NULL, &bSSHEnable) == ANSC_STATUS_FAILURE)
            return ANSC_STATUS_FAILURE;

    if (flag != bSSHEnable) {
        snprintf(buf,sizeof(buf),"%d",flag);
        syscfg_set( NULL, "mgmt_wan_sshaccess", buf);
        syscfg_commit();
        system("sysevent set firewall-restart");
        if (platform_hal_SetSSHEnable(flag) == RETURN_ERR )
            return ANSC_STATUS_FAILURE;
    }   
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetHNAPEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     flag
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetHTTPEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pValue
    )
{
    *pValue = TRUE;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetHTTPSEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pValue
    )
{
    *pValue = TRUE;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetHTTPPort
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *pValue
    )
{
    WebServConf_t conf;

    if (LoadWebServConf(&conf) != 0)
        return ANSC_STATUS_FAILURE;

    *pValue = conf.httpport;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetHTTPSPort
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *pValue
    )
{
    WebServConf_t conf;

    if (LoadWebServConf(&conf) != 0)
        return ANSC_STATUS_FAILURE;

    *pValue = conf.httpsport;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetWebServer(BOOL httpEn, BOOL httpsEn, ULONG httpPort, ULONG httpsPort)
{
    WebServConf_t conf;
    
    /* do not support disable HTTP/HTTPS */
    conf.httpport = httpPort;
    conf.httpsport = httpsPort;

    if (SaveWebServConf(&conf) != 0)
        return ANSC_STATUS_FAILURE;

    if (WebServRestart(&conf) != 0)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetMsoRemoteMgmtEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    )
{
    UtopiaContext ctx;
    boolean_t bEnabled;

    if (!Utopia_Init(&ctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }
    
    Utopia_GetBool(&ctx, UtopiaValue_Mgmt_MsoAccess, &bEnabled);
    *pFlag = bEnabled ? TRUE : FALSE;

    Utopia_Free(&ctx, 0);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetMsoRemoteMgmtEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     pFlag
    )
{
    UtopiaContext ctx;
    boolean_t bEnabled = pFlag;

    if (!Utopia_Init(&ctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }

    Utopia_SetBool(&ctx, UtopiaValue_Mgmt_MsoAccess, bEnabled);

    Utopia_Free(&ctx, 1);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetCusadminRemoteMgmtEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    )
{
    UtopiaContext ctx;
    boolean_t bEnabled;

    if (!Utopia_Init(&ctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }
    
    Utopia_GetBool(&ctx, UtopiaValue_Mgmt_CusadminAccess, &bEnabled);
    *pFlag = bEnabled ? TRUE : FALSE;

    Utopia_Free(&ctx, 0);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetCusadminRemoteMgmtEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     pFlag
    )
{
    UtopiaContext ctx;
    boolean_t bEnabled = pFlag;

    if (!Utopia_Init(&ctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }

    Utopia_SetBool(&ctx, UtopiaValue_Mgmt_CusadminAccess, bEnabled);

    Utopia_Free(&ctx, 1);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetHSEthernetPortEnable
    (
        ANSC_HANDLE                hContext,
        BOOLEAN                    *pFlagcpv
    )
{
    char cmdBuf[100];
    char *XHSNetPort, *XHSL2;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
   
    if(CCSP_SUCCESS != PSM_Get_Record_Value2(bus_info, g_GetSubsystemPrefix(g_pDslhDmlAgent), "dmsb.MultiLAN.HomeSecurity_HsPorts", NULL, &XHSNetPort))
    {
        return ANSC_STATUS_FAILURE;
    }
    
    if(CCSP_SUCCESS != PSM_Get_Record_Value2(bus_info, g_GetSubsystemPrefix(g_pDslhDmlAgent), "dmsb.MultiLAN.HomeSecurity_l2net", NULL, &XHSL2))
    {
        bus_info->freefunc(XHSNetPort);
        return ANSC_STATUS_FAILURE;
    }
    
    snprintf(cmdBuf, sizeof(cmdBuf), "Device.Bridging.Bridge.%s.Port.%s.Enable", XHSL2, XHSNetPort);
    bus_info->freefunc(XHSNetPort);
    bus_info->freefunc(XHSL2);
    
    Cdm_GetParamBool(cmdBuf, pFlagcpv);
    
    return ANSC_STATUS_SUCCESS;
    
}

ANSC_STATUS
CosaDmlDcSetHSEthernetPortEnable
    (
        ANSC_HANDLE                hContext,
        BOOLEAN                    pFlag
    )
{
    char* primNetXHSPort, *XHSNetPort, *primL2, *XHSL2;
    char* disableL2, *disablePort, *enableL2, *enablePort;
    char cmdBuf[100];
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    //PSM get of primary XHS port and XHS XHS port
    if(CCSP_SUCCESS != PSM_Get_Record_Value2(bus_info, g_GetSubsystemPrefix(g_pDslhDmlAgent), "dmsb.MultiLAN.PrimaryLAN_HsPorts", NULL, &primNetXHSPort))
        return ANSC_STATUS_FAILURE;
    if(CCSP_SUCCESS != PSM_Get_Record_Value2(bus_info, g_GetSubsystemPrefix(g_pDslhDmlAgent), "dmsb.MultiLAN.HomeSecurity_HsPorts", NULL, &XHSNetPort))
    {
        bus_info->freefunc(primNetXHSPort);
        return ANSC_STATUS_FAILURE;
    }
    
    if(CCSP_SUCCESS != PSM_Get_Record_Value2(bus_info, g_GetSubsystemPrefix(g_pDslhDmlAgent), "dmsb.MultiLAN.PrimaryLAN_l2net", NULL, &primL2))
    {
        bus_info->freefunc(XHSNetPort);
        bus_info->freefunc(primNetXHSPort);
        return ANSC_STATUS_FAILURE;
    }
    
    if(CCSP_SUCCESS != PSM_Get_Record_Value2(bus_info, g_GetSubsystemPrefix(g_pDslhDmlAgent), "dmsb.MultiLAN.HomeSecurity_l2net", NULL, &XHSL2))
    {
        bus_info->freefunc(XHSNetPort);
        bus_info->freefunc(primNetXHSPort);
        bus_info->freefunc(primL2);
        return ANSC_STATUS_FAILURE;
    }
    
    //Disable appropriate port
    
    if (pFlag) {
        enableL2 = XHSL2;
        enablePort = XHSNetPort;
        disableL2 = primL2;
        disablePort = primNetXHSPort;
    } else {
        enableL2 = primL2;
        enablePort = primNetXHSPort;
        disableL2 = XHSL2;
        disablePort = XHSNetPort;
    }
    
    snprintf(cmdBuf, sizeof(cmdBuf), "Device.Bridging.Bridge.%s.Port.%s.Enable", disableL2, disablePort);
    printf("running disable on this DM: %s", cmdBuf); fflush(stdout);
    Cdm_SetParamBool(cmdBuf, 0,1);
    
    snprintf(cmdBuf, sizeof(cmdBuf), "Device.Bridging.Bridge.%s.Port.%s.Enable", enableL2, enablePort);
    printf("running enable on this DM: %s", cmdBuf); fflush(stdout);
    Cdm_SetParamBool(cmdBuf, 1,1);
    
    bus_info->freefunc(XHSL2);
    bus_info->freefunc(primNetXHSPort);
    bus_info->freefunc(XHSNetPort);
    bus_info->freefunc(primL2);
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetGuestPassword
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    UtopiaContext ctx;
    PCOSA_DATAMODEL_DEVICECONTROL pDc;

    if (!Utopia_Init(&ctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }
    
    Utopia_RawGet(&ctx, NULL, "guest_password", pValue, sizeof(pDc->GuestPassword));
    
    Utopia_Free(&ctx, 0);

    return ANSC_STATUS_SUCCESS;
}

#define GUEST_ALLOW_LIST "/var/.guest_allow_list"
ANSC_STATUS
CosaDmlDcSetGuestPassword
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    UtopiaContext utctx;

    if (!Utopia_Init(&utctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }
	
    Utopia_RawSet(&utctx, NULL, "guest_password", pValue);
	
    Utopia_Free(&utctx,1);

    FILE* fp = fopen(GUEST_ALLOW_LIST, "w");
    if(fp != NULL)
        fclose(fp);

    system("sysevent set firewall-restart");

    return ANSC_STATUS_SUCCESS;
}

ULONG
CosaDmlDcGetNoOfGuests
    ()
{

    UtopiaContext utctx;
    int ret;
    char maxNum[16];

    if (!Utopia_Init(&utctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }

    Utopia_RawGet(&utctx, NULL, "guest_max_num", maxNum, sizeof(maxNum));
    ret = atoi(maxNum);

    Utopia_Free(&utctx,0);

    return ret;
}

ANSC_STATUS
CosaDmlDcSetNoOfGuests
    (
        ULONG                       uVal
    )
{
    UtopiaContext utctx;
    char maxNum[16];

    if (!Utopia_Init(&utctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }

    snprintf(maxNum, sizeof(maxNum), "%u", uVal);
    Utopia_RawSet(&utctx, NULL, "guest_max_num", maxNum);

    Utopia_Free(&utctx,1);
    
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetParConPassword
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    UtopiaContext ctx;
    PCOSA_DATAMODEL_DEVICECONTROL pDc;

    if (!Utopia_Init(&ctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }
    
    Utopia_RawGet(&ctx, NULL, "parcon_password", pValue, sizeof(pDc->ParentalControlPassword));
    
    Utopia_Free(&ctx, 0);

    return ANSC_STATUS_SUCCESS;
}

#define PARCON_ALLOW_LIST "/var/.parcon_allow_list"
ANSC_STATUS
CosaDmlDcSetParConPassword
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    UtopiaContext utctx;

    if (!Utopia_Init(&utctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }
	
    Utopia_RawSet(&utctx, NULL, "parcon_password", pValue);
	
    Utopia_Free(&utctx,1);

    FILE* fp = fopen(PARCON_ALLOW_LIST, "w");
    if(fp != NULL)
        fclose(fp);

    system("sysevent set firewall-restart");

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetParConQuestion
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    UtopiaContext ctx;
    PCOSA_DATAMODEL_DEVICECONTROL pDc;

    if (!Utopia_Init(&ctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }
    
    Utopia_RawGet(&ctx, NULL, "parcon_question", pValue, sizeof(pDc->ParentalControlQuestion));
    
    Utopia_Free(&ctx, 0);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetParConQuestion
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    UtopiaContext utctx;
    char pwd[33];

    if (!Utopia_Init(&utctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }
	
    Utopia_RawSet(&utctx, NULL, "parcon_question", pValue);
    
    Utopia_Free(&utctx,1);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetParConAnswer
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    UtopiaContext ctx;
    PCOSA_DATAMODEL_DEVICECONTROL pDc;

    if (!Utopia_Init(&ctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }
    
    Utopia_RawGet(&ctx, NULL, "parcon_answer", pValue, sizeof(pDc->ParentalControlAnswer));
    
    Utopia_Free(&ctx, 0);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetParConAnswer
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    UtopiaContext utctx;

    if (!Utopia_Init(&utctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }
	
    Utopia_RawSet(&utctx, NULL, "parcon_answer", pValue);
	
    Utopia_Free(&utctx,1);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetDefaultParConPassword
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    UtopiaContext ctx;
    PCOSA_DATAMODEL_DEVICECONTROL pDc;

    if (!Utopia_Init(&ctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }
    
    Utopia_RawGet(&ctx, NULL, "parcon_default_password", pValue, sizeof(pDc->ParentalControlDefaultPwd));
    
    Utopia_Free(&ctx, 0);

    return ANSC_STATUS_SUCCESS;
}

/*return 1 or 2 
  1 means moca hardware is Available
  2 means moca hardware is NotAvailable
  */

ULONG
CosaDmlGetMocaHardwareStatus
    (
        ANSC_HANDLE                 hContext
    )
{
    char out[256] = {0};
    static int ret = 0;

    if (ret) {
        return ret;
    }

    if (moca_HardwareEquipped()){
        ret = 1;
    }else{
        ret = 2;
    }
    
    return ret;
}

#define _CALC_NETWORK(ip, mask) ((ULONG)(ip) & (ULONG)(mask))

static void getLanMgmtUpnp(UtopiaContext *utctx, BOOLEAN *enable)
{
    int bEnabled;

	if (utctx == NULL || enable == NULL)
		return;
    
    Utopia_GetBool(utctx, UtopiaValue_Mgmt_IGDEnabled, &bEnabled);

    if (bEnabled){
        *enable = TRUE;
    }else{
        *enable = FALSE;
    }

}

static void setLanMgmtUpnp(UtopiaContext *utctx, BOOLEAN enable)
{
    int bEnabled = (enable == TRUE) ? 1 : 0;

	if (utctx == NULL)
		return;

    Utopia_SetBool(utctx, UtopiaValue_Mgmt_IGDEnabled, bEnabled);
}

static 
void _Get_LanMngm_Setting(UtopiaContext *utctx, ULONG index, PCOSA_DML_LAN_MANAGEMENT pLanMngm){
    lanSetting_t lan;
    ANSC_IPV4_ADDRESS network, netmask, ipaddr;
    int temp;
    bridgeInfo_t bridge_info = {0}; /*RDKB-6845, CID-33087, initialize before use*/
    boolean_t bool_tmp;
    int int_tmp;
    napt_mode_t napt_mode; 
    /* Till now,just support only one lan interface */
    /* ignor the index */
    Utopia_GetLanMngmInsNum(utctx, &(pLanMngm->InstanceNumber));
    Utopia_GetLanMngmAlias(utctx, pLanMngm->Alias, sizeof(pLanMngm->Alias));
    Utopia_GetBridgeSettings(utctx, &bridge_info);
    pLanMngm->LanMode = (bridge_info.mode == BRIDGE_MODE_OFF ? COSA_DML_LanMode_Router : COSA_DML_LanMode_BridgeStatic);
    
    Utopia_GetLanSettings(utctx, &lan);
    inet_pton(AF_INET, lan.ipaddr, &ipaddr);
    memcpy(&(pLanMngm->LanIPAddress), &(ipaddr), sizeof(ANSC_IPV4_ADDRESS));
    inet_pton(AF_INET, lan.netmask, &netmask);
    memcpy(&(pLanMngm->LanSubnetMask), &(netmask), sizeof(ANSC_IPV4_ADDRESS));
    network.Value = _CALC_NETWORK(ipaddr.Value, netmask.Value);
    memcpy(&(pLanMngm->LanNetwork), &(network), sizeof(ANSC_IPV4_ADDRESS));
    
    Utopia_GetLanMngmLanNetworksAllow(utctx, &int_tmp);
    pLanMngm->LanNetworksAllow = (COSA_DML_LanNetworksAllow)int_tmp;
    
    /* TO-DO */
    /* LanDhcpServer; */
    Utopia_GetLanMngmLanNapt(utctx, &napt_mode);
    switch (napt_mode){
        default:
            pLanMngm->LanNaptEnable = TRUE;
            pLanMngm->LanNaptType = 1;//COSA_DML_LanNapt_DHCP;
            break;
        case NAPT_MODE_DISABLE_STATIC:
            pLanMngm->LanNaptEnable = FALSE;
            pLanMngm->LanNaptType = 0;//COSA_DML_LanNapt_StaticIP;
            break;
        case NAPT_MODE_DISABLE_DHCP:
            pLanMngm->LanNaptEnable = FALSE;
            pLanMngm->LanNaptType = 1;//COSA_DML_LanNapt_DHCP;
            break;
        case NAPT_MODE_DHCP:
            pLanMngm->LanNaptEnable = TRUE;
            pLanMngm->LanNaptType = 1;//COSA_DML_LanNapt_DHCP;
            break;
        case NAPT_MODE_STATICIP:
            pLanMngm->LanNaptEnable = TRUE;
            pLanMngm->LanNaptType = 0;//COSA_DML_LanNapt_StaticIP;
            break;
    }
    
    /* TO-DO */
    /* LanTos;
     */

	getLanMgmtUpnp(utctx, &pLanMngm->LanUpnp);
}

ULONG
CosaDmlLanMngm_GetNumberOfEntries(void)
{
    UtopiaContext utctx = {0};
    int num = -1;
    if (Utopia_Init(&utctx))
    {
        Utopia_GetLanMngmCount(&utctx, &num);
        Utopia_Free(&utctx, 0);
    }
    return (ULONG)num;
}

ANSC_STATUS
CosaDmlLanMngm_GetEntryByIndex(ULONG index, PCOSA_DML_LAN_MANAGEMENT pLanMngm)
{
    
    UtopiaContext utctx = {0};
    int num = -1;
    ANSC_STATUS ret = ANSC_STATUS_FAILURE;
    
    if (Utopia_Init(&utctx))
    {
        Utopia_GetLanMngmCount(&utctx, &num);
        if(index < num ){
            _Get_LanMngm_Setting(&utctx, index, pLanMngm);
            Utopia_Free(&utctx, 0);
            ret = ANSC_STATUS_SUCCESS;
        }
    }
    return ret;
}

ANSC_STATUS
CosaDmlLanMngm_SetValues(ULONG index, ULONG ins, const char *alias)
{
    UtopiaContext utctx = {0};
    ANSC_STATUS ret = ANSC_STATUS_FAILURE;
    if (Utopia_Init(&utctx))
    {
        Utopia_SetLanMngmInsNum(&utctx, ins);
        Utopia_SetLanMngmAlias(&utctx, alias);
        Utopia_Free(&utctx, 1);
        ret = ANSC_STATUS_SUCCESS;
    }
    return ret;
}

ANSC_STATUS
CosaDmlLanMngm_AddEntry(PCOSA_DML_LAN_MANAGEMENT pLanMngm)
{
    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDmlLanMngm_DelEntry(ULONG ins)
{
    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDmlLanMngm_GetConf(ULONG ins, PCOSA_DML_LAN_MANAGEMENT pLanMngm)
{
    UtopiaContext utctx = {0};
    ANSC_STATUS ret = ANSC_STATUS_FAILURE;
     
    if (Utopia_Init(&utctx))
    {
        _Get_LanMngm_Setting(&utctx, ins, pLanMngm);
        Utopia_Free(&utctx, 0);
        ret = ANSC_STATUS_SUCCESS;
    }
    return ret;
}

/*To make multi thread to exec sequentially*/
static void  checkTicket(int ticket)
{
    while(1)
    {
        if(ticket != curticket)
            sleep(5);
        else
            break;
    }    
}

void* bridge_mode_wifi_notifier_thread(void* arg) {
    PCOSA_NOTIFY_WIFI pNotify = (PCOSA_NOTIFY_WIFI)arg;
    char* enableStr = (char*)(pNotify->flag?"false" : "true");
    char*   faultParam = NULL;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    ANSC_STATUS ret = ANSC_STATUS_FAILURE;
    parameterValStruct_t* theVals;
    int numVals;

    checkTicket(pNotify->ticket);
    
#ifdef CONFIG_CISCO_FEATURE_CISCOCONNECT
    char param[50];
    char* enVal = NULL;
    char* guestnetDM = NULL;
    char* guestEnableStr = NULL;
    snprintf(param, sizeof(param), "dmsb.CiscoConnect.guestEnabled");
    if (PSM_Get_Record_Value2(bus_info, g_GetSubsystemPrefix(g_pDslhDmlAgent), (char *)param, NULL, &enVal) == CCSP_SUCCESS) {
        if ( enVal[0] == '1' && enableStr[0] == 't') {
            guestEnableStr = "true";
        } else {
            guestEnableStr = "false";
        }
        //businfo = g_MessageBusHandle;
        bus_info->freefunc(enVal);
    } else {
        guestEnableStr = "false";
    }
    guestnetDM = "Device.WiFi.SSID.5.Enable";
    
#endif
    
    //Full bridge
 parameterValStruct_t           val[] = { 
#ifdef CONFIG_CISCO_FEATURE_CISCOCONNECT
 {guestnetDM, guestEnableStr, ccsp_boolean},
#endif 
 {"Device.WiFi.Radio.1.Enable", enableStr, ccsp_boolean}, 
 {"Device.WiFi.Radio.2.Enable", enableStr, ccsp_boolean}
#ifdef CONFIG_CISCO_XHS
 ,{"Device.WiFi.SSID.3.Enable", enableStr, ccsp_boolean}
#endif
};
 
// Pseudo bridge
 parameterValStruct_t val2[] = { 
 {"Device.WiFi.SSID.1.Enable", enableStr, ccsp_boolean}, 
 {"Device.WiFi.SSID.2.Enable", enableStr, ccsp_boolean}};
 
 parameterValStruct_t valCommit[] = { 
 {"Device.WiFi.Radio.1.X_CISCO_COM_ApplySetting", "true", ccsp_boolean}, {"Device.WiFi.Radio.2.X_CISCO_COM_ApplySetting", "true", ccsp_boolean} };
 
            //Run the full bridge radio disable params only on router and full bridge transitions
            if (!pNotify->flag || pNotify->flag != 3) {
                ret = CcspBaseIf_setParameterValues
                                (
                                        bus_handle, 
                                        ppComponents[0]->componentName, 
                                        ppComponents[0]->dbusPath,
                                        0, 0x0,   /* session id and write id */
                                        val, 
                                        sizeof(val)/sizeof(*val), 
                                        TRUE,   /* no commit */
                                        &faultParam
                                );      
                        
                if (ret != CCSP_SUCCESS && faultParam)
                {
                    AnscTraceError(("Error:Failed to SetValue for param '%s'\n", faultParam));
                    bus_info->freefunc(faultParam);
                }  
            }
            
            //Run the pseudo bridge ssid disable params only on router and pseudofs bridge transitions
            if (!pNotify->flag || pNotify->flag == 3) {
                ret = CcspBaseIf_setParameterValues
                                (
                                        bus_handle, 
                                        ppComponents[0]->componentName, 
                                        ppComponents[0]->dbusPath,
                                        0, 0x0,   /* session id and write id */
                                        val2, 
                                        sizeof(val2)/sizeof(*val2), 
                                        TRUE,   /* no commit */
                                        &faultParam
                                );      
                        
                if (ret != CCSP_SUCCESS && faultParam)
                {
                    AnscTraceError(("Error:Failed to SetValue for param '%s'\n", faultParam));
                    bus_info->freefunc(faultParam);
                }  
            }
        
        ret = CcspBaseIf_setParameterValues
                        (
                                bus_handle, 
                                ppComponents[0]->componentName, 
                                ppComponents[0]->dbusPath,
                                0, 0x0,   /* session id and write id */
                                valCommit, 
                                2, 
                                TRUE,   /* no commit */
                                &faultParam
                        );      
                
        if (ret != CCSP_SUCCESS && faultParam)
        {
            AnscTraceError(("Error:Failed to SetValue for param '%s'\n", faultParam));
            bus_info->freefunc(faultParam);
        } 

        //Reset Radios only when Enabled router mode
        if (!pNotify->flag)
        {
                parameterValStruct_t resetRadio[] = {{"Device.WiFi.X_CISCO_COM_ResetRadios", "true", ccsp_boolean}};

                ret = CcspBaseIf_setParameterValues
                        (
                                bus_handle,
                                ppComponents[0]->componentName,
                                ppComponents[0]->dbusPath,
                                0, 0x0,   /* session id and write id */
                                resetRadio,
                                1,
                                TRUE,   /* no commit */
                                &faultParam
                        );

                if (ret != CCSP_SUCCESS && faultParam)
                {
                    AnscTraceError(("Error:Failed to SetValue for param '%s'\n", faultParam));
                    bus_info->freefunc(faultParam);
                }
        }

        curticket++;
        AnscFreeMemory(arg);
        return NULL;
}

ANSC_STATUS
CosaDmlLanMngm_SetConf(ULONG ins, PCOSA_DML_LAN_MANAGEMENT pLanMngm)
{
    UtopiaContext utctx = {0};
    lanSetting_t  lan;
    ANSC_STATUS ret = ANSC_STATUS_FAILURE;
    bridgeInfo_t bridge_info;
    char str[IFNAME_SZ];    
    char *enableStr;
    napt_mode_t napt;
#if !defined(_CBR_PRODUCT_REQ_) && !defined(_PLATFORM_RASPBERRYPI_) // MOCA is not present for TCCBR environment and RaspberryPi environment
	parameterValStruct_t **valMoCAstatus;
	char pMoCAComponentName[64]="eRT.com.cisco.spvtg.ccsp.moca";
	char pComponentPath[64]="/com/cisco/spvtg/ccsp/moca";
	char *paramNames[]={"Device.MoCA.Interface.1.Enable"};
	int nval;
	char buf[16];
	int MoCAstate;
#endif
    
    COSA_DML_LAN_MANAGEMENT orgLanMngm;

    if (Utopia_Init(&utctx))
    {
        _Get_LanMngm_Setting(&utctx, ins, &orgLanMngm);
        Utopia_SetLanMngmAlias(&utctx, pLanMngm->Alias);
        Utopia_SetLanMngmInsNum(&utctx, pLanMngm->InstanceNumber);
        bridge_info.mode = pLanMngm->LanMode == COSA_DML_LanMode_BridgeStatic ? BRIDGE_MODE_STATIC : BRIDGE_MODE_OFF; 
        Utopia_SetBridgeSettings(&utctx,&bridge_info);
#if !defined(_CBR_PRODUCT_REQ_) && !defined(_PLATFORM_RASPBERRYPI_) // MOCA is not present for TCCBR environment and RaspberryPi environment
		ret = CcspBaseIf_getParameterValues(
		    bus_handle,
		    pMoCAComponentName,
		    pComponentPath,
		    paramNames,
		    1,
		    &nval,
		    &valMoCAstatus);
                 if( CCSP_SUCCESS == ret ){
			CcspTraceWarning(("valMoCAstatus[0]->parameterValue = %s\n",valMoCAstatus[0]->parameterValue));
			if(strcmp("true", valMoCAstatus[0]->parameterValue)==0)
					MoCAstate=1;
			else
					MoCAstate=0;
			snprintf(buf,sizeof(buf),"%d",MoCAstate);
			if ((syscfg_set(NULL, "MoCA_current_status", buf) != 0)) 
		        {
                            CcspTraceWarning(("syscfg_set failed\n"));
                            return -1;
                        }
                        else
                        {
                             if (syscfg_commit() != 0)
                             {
                                    CcspTraceWarning(("syscfg_commit failed\n"));
				    return -1;
                             }
                        }
                   }
                   else
                   {
                       CcspTraceError(("CcspBaseIf_getParameterValues failed to get MoCA status return vaule = %d\n",ret));
		   }
		   if(valMoCAstatus){
			free_parameterValStruct_t (bus_handle, nval, valMoCAstatus);
		}
#endif
		
        
        memset(&lan, 0 ,sizeof(lan));
        inet_ntop(AF_INET, &(pLanMngm->LanIPAddress), str, sizeof(str));
        memcpy(&(lan.ipaddr), str, sizeof(str));
        inet_ntop(AF_INET, &(pLanMngm->LanSubnetMask), str, sizeof(str));
        memcpy(&(lan.netmask), str, sizeof(str));
        Utopia_SetLanSettings(&utctx, &lan);

#if defined(_COSA_INTEL_USG_ARM_) && defined(ENABLE_FEATURE_MESHWIFI)
        // Send subnet change message to ATOM so that MESH is notified.
        {
            #define DATA_SIZE 1024
            FILE *fp1;
            char buf[DATA_SIZE] = {0};
            char *urlPtr = NULL;

            // Grab the ATOM RPC IP address
            // sprintf(cmd1, "cat /etc/device.properties | grep ATOM_ARPING_IP | cut -f 2 -d\"=\"");

            fp1 = fopen("/etc/device.properties", "r");
            if (fp1 == NULL) {
                CcspTraceError(("Error opening properties file! \n"));
                return FALSE;
            }

            while (fgets(buf, DATA_SIZE, fp1) != NULL) {
                // Look for ATOM_ARPING_IP
                if (strstr(buf, "ATOM_ARPING_IP") != NULL) {
                    buf[strcspn(buf, "\r\n")] = 0; // Strip off any carriage returns

                    // grab URL from string
                    urlPtr = strstr(buf, "=");
                    urlPtr++;
                    break;
                }
            }

            if (fclose(fp1) != 0) {
                /* Error reported by pclose() */
                CcspTraceError(("Error closing properties file! \n"));
            }

            if (urlPtr != NULL && urlPtr[0] != 0 && strlen(urlPtr) > 0) {
                CcspTraceInfo(("Reported an ATOM IP of %s \n", urlPtr));
                pid_t pid = fork();

                if (pid == -1)
                {
                    // error, failed to fork()
                }
                else if (pid > 0)
                {
                    int status;
                    waitpid(pid, &status, 0); // wait here until the child completes
                }
                else
                {
                    // we are the child
                    char cmd[DATA_SIZE] = {0};
                    CcspTraceInfo(("Sending subnet_change notification to ATOM IP %s \n", urlPtr));
                    sprintf(cmd, "/usr/bin/sysevent set subnet_change \"RDK|%s|%s\"",
                            lan.ipaddr,lan.netmask);
                    char *args[] = {"/fss/gw/usr/bin/rpcclient", urlPtr, cmd, (char *) 0 };
                    execv(args[0], args);
                    _exit(EXIT_FAILURE);   // exec never returns
                }
            }
        }
#endif

        Utopia_SetLanMngmLanNetworksAllow(&utctx, pLanMngm->LanNetworksAllow);
#if 0 
        if(pLanMngm->LanNaptType == COSA_DML_LanNapt_DHCP && pLanMngm->LanNaptEnable == TRUE)        
            napt = NAPT_MODE_DHCP;
        else if(pLanMngm->LanNaptType == COSA_DML_LanNapt_DHCP && pLanMngm->LanNaptEnable == FALSE)        
            napt = NAPT_MODE_DISABLE_DHCP;
        else if(pLanMngm->LanNaptType == COSA_DML_LanNapt_StaticIP && pLanMngm->LanNaptEnable == TRUE)        
            napt = NAPT_MODE_STATICIP;
        else if(pLanMngm->LanNaptType == COSA_DML_LanNapt_StaticIP && pLanMngm->LanNaptEnable == FALSE)        
            napt = NAPT_MODE_DISABLE_STATIC;
        else 
            napt = NAPT_MODE_DHCP;
#endif
        if(pLanMngm->LanNaptType == 1 && pLanMngm->LanNaptEnable == TRUE)        
            napt = NAPT_MODE_DHCP;
        else if(pLanMngm->LanNaptType == 1 && pLanMngm->LanNaptEnable == FALSE)        
            napt = NAPT_MODE_DISABLE_DHCP;
        else if(pLanMngm->LanNaptType == 0 && pLanMngm->LanNaptEnable == TRUE)        
            napt = NAPT_MODE_STATICIP;
        else if(pLanMngm->LanNaptType == 0 && pLanMngm->LanNaptEnable == FALSE)        
            napt = NAPT_MODE_DISABLE_STATIC;
        else 
            napt = NAPT_MODE_DHCP;

        Utopia_SetLanMngmLanNapt(&utctx, napt);
		setLanMgmtUpnp(&utctx, pLanMngm->LanUpnp);
        Utopia_Free(&utctx, 1);
        pLanMngm->LanNetwork.Value = _CALC_NETWORK(pLanMngm->LanIPAddress.Value, pLanMngm->LanSubnetMask.Value);
        
        /* If lan settings not change, skip refreshing wifi module */
//         if(orgLanMngm.LanIPAddress.Value == pLanMngm->LanIPAddress.Value &&
//            orgLanMngm.LanSubnetMask.Value == pLanMngm->LanSubnetMask.Value)
//         {
//             return ANSC_STATUS_SUCCESS;
//         }

        
        if (pLanMngm->LanMode == orgLanMngm.LanMode) {
            return ANSC_STATUS_SUCCESS;
        }
        
        
        
        //Bridge mode has changed, so we need to report the change and toggle wifi accordingly
        //TODO: move this to a thread
        int bEnable;
        
        if(bridge_info.mode == BRIDGE_MODE_OFF)
        {
            syslog_systemlog("Local Network", LOG_NOTICE, "Status change: IP %s mask %s", lan.ipaddr, lan.netmask);
            bEnable = 0;
        }
        else
        {
            syslog_systemlog("Local Network", LOG_NOTICE, "Status change: Bridge mode");
            bEnable = 1;
        }
        
        //configBridgeMode(bEnable);
        
        ret = ANSC_STATUS_SUCCESS;
    }
    return ret;
}

ANSC_STATUS
CosaDmlDcGetIGMPSnoopingEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pValue
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetIGMPSnoopingEnable
    (
        ANSC_HANDLE                 hContext,
        ULONG                     value
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetWebUITimeout
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *pValue
    )
{
    if (pValue == NULL)
        return ANSC_STATUS_FAILURE;

    if (platform_hal_GetWebUITimeout(pValue) != RETURN_OK )
        return ANSC_STATUS_FAILURE;
    else
        return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetPowerSavingModeStatus
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *pValue
    )
{
#if defined (CONFIG_TI_BBU) || defined (CONFIG_TI_BBU_TI)

#include "mta_hal.h"
 
    if (mta_hal_BatteryGetPowerSavingModeStatus(pValue) != RETURN_OK )
        return ANSC_STATUS_FAILURE;
    else
        return ANSC_STATUS_SUCCESS;

#endif

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetWebUITimeout
    (
        ANSC_HANDLE                 hContext,
        ULONG                       value
    )
{
    if ((value == 0) || (value >= 30 && value <= 86400)){
        if (platform_hal_SetWebUITimeout(value) != RETURN_OK )
            return ANSC_STATUS_FAILURE;
        else
            return ANSC_STATUS_SUCCESS;    
    }else
        return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDmlDcGetWebAccessLevel
    (
        ANSC_HANDLE                 hContext,
        int                         userIndex,
        int                         ifIndex,
        ULONG                       *pValue
    )
{
    if (pValue == NULL)
        return ANSC_STATUS_FAILURE;

    if (platform_hal_GetWebAccessLevel(userIndex, ifIndex, pValue) != RETURN_OK )
        return ANSC_STATUS_FAILURE;
    else
        return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetWebAccessLevel
    (
        ANSC_HANDLE                 hContext,
        int                         userIndex,
        int                         ifIndex,
        ULONG                       value
    )
{
    if (platform_hal_SetWebAccessLevel(userIndex, ifIndex, value) != RETURN_OK )
        return ANSC_STATUS_FAILURE;
    else
        return ANSC_STATUS_SUCCESS;
}

static void configBridgeMode(int bEnable) {
    char primaryl2inst[5];
        char primarybrp[5];
        char brpdm[50];
        char brmode[5];
        PCOSA_NOTIFY_WIFI pnotifypara = (PCOSA_NOTIFY_WIFI)AnscAllocateMemory(sizeof(COSA_NOTIFY_WIFI)); /*RDKB-6845, CID-32969, free unused resource before return */

        memset(pnotifypara, 0, sizeof(COSA_NOTIFY_WIFI));
        commonSyseventGet("primary_lan_l2net", primaryl2inst, sizeof(primaryl2inst));
        commonSyseventGet("primary_lan_brport", primarybrp, sizeof(primaryl2inst));
        commonSyseventGet("bridge_mode", brmode, sizeof(brmode));
        snprintf(brpdm, sizeof(brpdm), "Device.Bridging.Bridge.%s.Port.%s.Enable", primaryl2inst, primarybrp);
//         varstruct.parameterName = brpdm;
//         varstruct.parameterValue = enableStr;
//         varstruct.type = ccsp_boolean;
        g_SetParamValueBool(brpdm, bEnable);

        vsystem("/bin/sh /etc/webgui.sh &");

        if (ppComponents == NULL && initWifiComp()) {
            syslog_systemlog("Local Network", LOG_NOTICE, "Bridge mode transition: Failed to acquire wifi component.");
            AnscFreeMemory( pnotifypara ); /*RDKB-6845, CID-33015, free unused resource before return */
            return ANSC_STATUS_SUCCESS;
        }
        totalticket += 1;
        pnotifypara->flag = brmode[0] == '3' ? 3 : bEnable;
        pnotifypara->ticket = totalticket;
        AnscCreateTask(bridge_mode_wifi_notifier_thread, USER_DEFAULT_TASK_STACK_SIZE, USER_DEFAULT_TASK_PRIORITY, (void *)pnotifypara, "BridgeModeWifiNotifierThread");
}

ANSC_STATUS
CosaDmlDcGetErouterEnabled
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    )
{
    char buf[10];
    buf[0]='\0';
    if(!syscfg_get(NULL, "last_erouter_mode", buf, sizeof(buf))) {
        *pFlag = (atoi(buf) ? TRUE : FALSE);
        return ANSC_STATUS_SUCCESS;
    } else {
        return ANSC_STATUS_FAILURE;
    }
}

ANSC_STATUS
CosaDmlDcSetErouterEnabled
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bFlag
    )
{
    configBridgeMode(!(bFlag));
    return ANSC_STATUS_SUCCESS;
}
