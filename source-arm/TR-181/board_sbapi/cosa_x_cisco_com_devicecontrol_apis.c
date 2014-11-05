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
#include <arpa/inet.h>

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

    if (!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;
    
    Utopia_RawGet(&ctx, NULL, (char *)path, buf, sizeof(buf));
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
    if (UtGetUlong("mgmt_wan_httpport", &conf->httpport) != ANSC_STATUS_SUCCESS)
        return -1;
    if (UtGetUlong("mgmt_wan_httpsport", &conf->httpsport) != ANSC_STATUS_SUCCESS)
        return -1;

    return 0;
}

static int
SaveWebServConf(const WebServConf_t *conf)
{
    if (UtSetUlong("mgmt_wan_httpport", conf->httpport) != ANSC_STATUS_SUCCESS)
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
        if (system("ps | grep snmp_subagnet | grep -v grep") != 0)
            if (system("/fss/gw/usr/share/snmp/run_subagent.sh tcp:127.0.0.1:705") != 0)
                return -1;
    }
    else
    {
        system("killall snmp_agent_cm");
        system("killall run_subagent.sh");
        system("killall snmp_subagnet");
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

    strncpy(pHostName, buf, sizeof(buf));

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

ANSC_STATUS
CosaDmlDcGetSNMPEnable
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
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

ANSC_STATUS
CosaDmlDcSetRebootDevice
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    int router, wifi, voip, dect, moca, all, delay;

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

    if (router && wifi && voip && dect && moca) {
        all = 1;
    }

    if (all) {
        if(delay) {
            fprintf(stderr, "Device is going to reboot in 5 seconds\n");
            system("(sleep 5 && reboot) &");
        }
        else {
            fprintf(stderr, "Device is going to reboot now\n");
            system("reboot");
        }
    }

    if (router) {
        fprintf(stderr, "Router is going to reboot\n");
        system("sysevent set lan-stop");
        system("sysevent set forwarding-restart"); 
    }
    if (wifi) {
        fprintf(stderr, "WiFi is going to reboot\n");
        int                         ret;
        int                         size = 0;
        componentStruct_t **        ppComponents = NULL;
        char*   faultParam = NULL;
        
        sprintf(dst_pathname_cr, "%s%s", g_Subsystem, CCSP_DBUS_INTERFACE_CR);

        ret = CcspBaseIf_discComponentSupportingNamespace(bus_handle,
                    dst_pathname_cr,
                    "Device.WiFi.Radio.",
                    g_Subsystem,        /* prefix */
                    &ppComponents,
                    &size);

        if ( ret == CCSP_SUCCESS && size == 1)
        {
            parameterValStruct_t val[2] = { { "Device.WiFi.Radio.1.X_CISCO_COM_ApplySetting", "true", ccsp_boolean}, 
                                            { "Device.WiFi.Radio.2.X_CISCO_COM_ApplySetting", "true", ccsp_boolean} };
            
            ret = CcspBaseIf_setParameterValues
				(
					bus_handle, 
					ppComponents[0]->componentName, 
					ppComponents[0]->dbusPath,
					0, 0x0,   /* session id and write id */
					&val, 
					2, 
					TRUE,   /* no commit */
					&faultParam
				);	
                
                if (ret != CCSP_SUCCESS && faultParam)
                {
                    AnscTraceError(("Error:Failed to SetValue for param '%s'\n", faultParam));
                    bus_info->freefunc(faultParam);
                }

            free_componentStruct_t(bus_handle, size, ppComponents);
        }
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
	}

	if (!factory_reset_mask)
		return ANSC_STATUS_BAD_PARAMETER;

    if (factory_reset_mask & FR_FW) {
        int rc = -1;
        UtopiaContext ctx;
        firewall_t fw;

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
		system("restoreAllDBs"); //Perform factory reset on other components
	}

	if (factory_reset_mask & FR_ROUTER) {
		tok = FACTORY_RESET_ROUTER_VALUE;
		if (!Utopia_Init(&utctx))
			return ANSC_STATUS_FAILURE;              
			
		if(!Utopia_RawSet(&utctx, NULL, FACTORY_RESET_KEY, tok)) {
			Utopia_Free(&utctx,0);
			return ANSC_STATUS_FAILURE;
		}
		
		Utopia_Free(&utctx,1);
		system("reboot");
	} else if (factory_reset_mask & FR_WIFI) {
		/*TODO: SEND EVENT TO WIFI PAM  Device.WiFi.X_CISCO_COM_FactoryReset*/
        int                         ret;
        char* faultParam = NULL;

        
            if (ppComponents == NULL && initWifiComp())
                return ANSC_STATUS_FAILURE;
            
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
                    AnscTraceError(("Error:Failed to SetValue for param '%s'\n", faultParam));
                    bus_info->freefunc(faultParam);
                }

//            free_componentStruct_t(bus_handle, size, ppComponents);
            return  ANSC_STATUS_SUCCESS;
        
        //system("reboot");
	} else if (factory_reset_mask & FR_NONE){
		
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
    syscfg_set(NULL, "last_erouter_mode", buf);
    syscfg_commit();
    /*Need to reboot device according to SNMP definition*/
    printf("<<< Reboot device because of the change of rgDevieMode >>>\n");
    CosaDmlDcSetRebootDevice(NULL,"Router,Wifi,VoIP,Dect,MoCA,Device,delay");

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
    BOOLEAN bTelnetEnable;

    if (CosaDmlDcGetTelnetEnable(NULL, &bTelnetEnable) == ANSC_STATUS_FAILURE)
        return ANSC_STATUS_FAILURE;

    if (flag != bTelnetEnable) {
        if (platform_hal_SetTelnetEnable(flag) == RETURN_ERR )
            return ANSC_STATUS_FAILURE;
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
    BOOLEAN bSSHEnable;

    if (CosaDmlDcGetSSHEnable(NULL, &bSSHEnable) == ANSC_STATUS_FAILURE)
            return ANSC_STATUS_FAILURE;

    if (flag != bSSHEnable) {
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
    bridgeInfo_t bridge_info;
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

void* bridge_mode_wifi_notifier_thread(void* arg) {
    char* enableStr = (char*)arg;
    char*   faultParam = NULL;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    ANSC_STATUS ret = ANSC_STATUS_FAILURE;
    
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
    
 parameterValStruct_t           val[] = { {"Device.WiFi.Radio.1.Enable", enableStr, ccsp_boolean}, {"Device.WiFi.Radio.2.Enable", enableStr, ccsp_boolean},
#ifdef CONFIG_CISCO_FEATURE_CISCOCONNECT
 {guestnetDM, guestEnableStr, ccsp_boolean},
#endif
 {"Device.WiFi.Radio.1.X_CISCO_COM_ApplySetting", "true", ccsp_boolean}, {"Device.WiFi.Radio.2.X_CISCO_COM_ApplySetting", "true", ccsp_boolean} };
            
        ret = CcspBaseIf_setParameterValues
                        (
                                bus_handle, 
                                ppComponents[0]->componentName, 
                                ppComponents[0]->dbusPath,
                                0, 0x0,   /* session id and write id */
                                val, 
                                sizeof(val)/sizeof(val[0]), 
                                TRUE,   /* no commit */
                                &faultParam
                        );      
                
        if (ret != CCSP_SUCCESS && faultParam)
        {
            AnscTraceError(("Error:Failed to SetValue for param '%s'\n", faultParam));
            bus_info->freefunc(faultParam);
        }   
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
    
    COSA_DML_LAN_MANAGEMENT orgLanMngm;

    if (Utopia_Init(&utctx))
    {
        _Get_LanMngm_Setting(&utctx, ins, &orgLanMngm);
        Utopia_SetLanMngmAlias(&utctx, pLanMngm->Alias);
        Utopia_SetLanMngmInsNum(&utctx, pLanMngm->InstanceNumber);
        bridge_info.mode = pLanMngm->LanMode == COSA_DML_LanMode_BridgeStatic ? BRIDGE_MODE_STATIC : BRIDGE_MODE_OFF; 
        Utopia_SetBridgeSettings(&utctx,&bridge_info);
        
        memset(&lan, 0 ,sizeof(lan));
        inet_ntop(AF_INET, &(pLanMngm->LanIPAddress), str, sizeof(str));
        memcpy(&(lan.ipaddr), str, sizeof(str));
        inet_ntop(AF_INET, &(pLanMngm->LanSubnetMask), str, sizeof(str));
        memcpy(&(lan.netmask), str, sizeof(str));
        Utopia_SetLanSettings(&utctx, &lan);

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
        
        if(bridge_info.mode == BRIDGE_MODE_OFF)
        {
            syslog_systemlog("Local Network", LOG_NOTICE, "Status change: IP %s mask %s", lan.ipaddr, lan.netmask);
            enableStr = "true";
        }
        else
        {
            syslog_systemlog("Local Network", LOG_NOTICE, "Status change: Bridge mode");
            enableStr = "false";
        }
        
        char primaryl2inst[5];
        char primarybrp[5];
        char brpdm[50];
        commonSyseventGet("primary_lan_l2net", primaryl2inst, sizeof(primaryl2inst));
        commonSyseventGet("primary_lan_brport", primarybrp, sizeof(primaryl2inst));
        
        snprintf(brpdm, sizeof(brpdm), "Device.Bridging.Bridge.%s.Port.%s.Enable", primaryl2inst, primarybrp);
//         varstruct.parameterName = brpdm;
//         varstruct.parameterValue = enableStr;
//         varstruct.type = ccsp_boolean;
        g_SetParamValueBool(brpdm, (bridge_info.mode != BRIDGE_MODE_OFF));

        vsystem("/bin/sh /etc/webgui.sh");

        if (ppComponents == NULL && initWifiComp()) {
            syslog_systemlog("Local Network", LOG_NOTICE, "Bridge mode transition: Failed to acquire wifi component.");
            return ANSC_STATUS_SUCCESS;
        }
            
        AnscCreateTask(bridge_mode_wifi_notifier_thread, USER_DEFAULT_TASK_STACK_SIZE, USER_DEFAULT_TASK_PRIORITY, (void*)enableStr, "BridgeModeWifiNotifierThread");
        
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

