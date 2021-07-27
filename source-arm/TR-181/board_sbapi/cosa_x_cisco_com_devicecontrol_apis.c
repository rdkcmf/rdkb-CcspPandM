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
#ifdef _MACSEC_SUPPORT_
#include "ccsp_hal_ethsw.h"
#endif
#include "secure_wrapper.h"
#include "cosa_drg_common.h"
#include "ccsp_psm_helper.h"
#include "safec_lib_common.h"

#if defined (_XB6_PRODUCT_REQ_) || defined (_XB7_PRODUCT_REQ_)
#define LED_SOLID 0
#define LED_BLINK 1
#define FR_BLINK_INTRVL 3
#endif

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
#include "cm_hal.h"

#define HTTPD_CONF      "/var/lighttpd.conf"
#define HTTPD_DEF_CONF  "/etc/lighttpd.conf"
#define HTTPD_PID       "/var/run/lighttpd.pid"
#define RM_L2_PATH "rm -rf /nvram/dl"
#define Device_Config_Ignore_size 1024

static void configBridgeMode(int bEnable);
static int curticket   = 1; /*The thread should be run with the ticket*/
static int totalticket = 0;

extern int commonSyseventFd ;
extern token_t commonSyseventToken;

void* set_mesh_disabled();
BOOL is_mesh_enabled();

#if defined (CONFIG_TI_BBU) || defined (CONFIG_TI_BBU_TI)
    INT mta_hal_BatteryGetPowerSavingModeStatus(ULONG *pValue);
#endif

#if defined (INTEL_PUMA7)
    BOOL moca_factoryReset(void);
#endif

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

/*
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
*/

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
    
    snprintf(buf, sizeof(buf), "%lu", val);
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

void* WebGuiRestart( void *arg )
{
    UNREFERENCED_PARAMETER(arg);
    pthread_detach(pthread_self());
    sleep(30);
    #if defined (_XB6_PRODUCT_REQ_) || defined (_CBR_PRODUCT_REQ_)
        vsystem("/bin/systemctl restart CcspWebUI.service");
    #else
        vsystem("/bin/sh /etc/webgui.sh &");
    #endif
    return NULL;
}
    

#if defined(_PLATFORM_RASPBERRYPI_) || defined(_PLATFORM_TURRIS_)
static int
DmSetBool(const char *param, BOOL value)
{
    parameterValStruct_t val[1];
    char crname[256], *fault = NULL;
    int err;

    val[0].parameterName  = (char*)param;
    val[0].parameterValue = (value ? "true" : "false");
    val[0].type           = ccsp_boolean;

    snprintf(crname, sizeof(crname), "%s%s", g_GetSubsystemPrefix(g_pDslhDmlAgent), CCSP_DBUS_INTERFACE_CR);

    if ((err = CcspBaseIf_SetRemoteParameterValue(g_MessageBusHandle,
                crname, param, g_GetSubsystemPrefix(g_pDslhDmlAgent), 0, 0xFFFF, val, 1, 1, &fault)) != CCSP_SUCCESS)

    if (fault)
        AnscFreeMemory(fault);

    return (err == CCSP_SUCCESS) ? 0 : -1;
}
#endif

void* WebServRestart( void *arg )
{
    UNREFERENCED_PARAMETER(arg);
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
    pthread_detach(pthread_self());
    CcspTraceInfo(("%s vsystem %d \n", __FUNCTION__,__LINE__)); 
    if (vsystem("/bin/sh /etc/webgui.sh") != 0) {
        fprintf(stderr, "%s: fail to restart lighttpd\n", __FUNCTION__);
        return NULL;
    }

    v_secure_system("sysevent set firewall-restart");

    return NULL;
}

#if 0
static int TelnetdConfig(int enable)
{
#if defined(_CBR_PRODUCT_REQ_) || defined (_BWG_PRODUCT_REQ_)
    return 0;
#endif
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
#endif

static int detect_process(char *process_name)
{
    FILE *ptr;
    char buff[512];

    if ((ptr=v_secure_popen("r", "ps | grep -v grep | grep -c %s", process_name))!=NULL)
    {
        while (fgets(buff,512,ptr)!=NULL)
        {
            if (atoi(buff)>=1)
            {
                v_secure_pclose(ptr);
                return 1;
            }
        }

        v_secure_pclose(ptr);
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
    UNREFERENCED_PARAMETER(hDml);
    UNREFERENCED_PARAMETER(phContext);
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
	/*
    else
    {
        //stop avahi two applications
        _CosaDmlDcStopZeroConfig();
    }*/

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
    UNREFERENCED_PARAMETER(hContext);
    errno_t rc = -1;
    rc = strcpy_s(pValue,*pulSize, "Cisco");
    ERR_CHK(rc);
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
    UNREFERENCED_PARAMETER(hContext);
    errno_t rc = -1;
    rc = strcpy_s(pValue, *pulSize, "000000");
    ERR_CHK(rc);
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
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pValue);
    UNREFERENCED_PARAMETER(pulSize);
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
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pValue);
    UNREFERENCED_PARAMETER(pulSize);
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
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(ulValue);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetMultiHomedHSDFlag
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulValue

    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(ulValue);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetMultiHomedUIPageControl
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulValue

    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(ulValue);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetWanAddressMode
    (
        ANSC_HANDLE                 hContext,
        COSA_DML_WanAddrMode        *pMode
    )
{
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(ipAddr);
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
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pFlag);
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
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(ipAddr);
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
    errno_t safec_rc = -1;
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

    safec_rc = strcpy_s(pHostName, sizeof(g_HostName), buf);
    if(safec_rc != EOK)
    {
       ERR_CHK(safec_rc);
    }
    safec_rc = strcpy_s(g_HostName, sizeof(g_HostName), buf);
    if(safec_rc != EOK)
    {
       ERR_CHK(safec_rc);
    }

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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
    UtopiaContext utctx;
    char buf[256];
    errno_t safec_rc = -1;

    if (!Utopia_Init(&utctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }

    safec_rc = sprintf_s(buf, sizeof(buf), "%d.%d.%d.%d", ((ANSC_IPV4_ADDRESS)ipAddr).Dot[0], ((ANSC_IPV4_ADDRESS)ipAddr).Dot[1], ((ANSC_IPV4_ADDRESS)ipAddr).Dot[2], ((ANSC_IPV4_ADDRESS)ipAddr).Dot[3] );
    if(safec_rc < EOK)
    {
       ERR_CHK(safec_rc);
    }

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
    UNREFERENCED_PARAMETER(hContext);
    UtopiaContext utctx;
    char buf[256];
    errno_t safec_rc = -1;

    if (!Utopia_Init(&utctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }

    safec_rc = sprintf_s(buf, sizeof(buf),"%d.%d.%d.%d", ((ANSC_IPV4_ADDRESS)ipAddr).Dot[0], ((ANSC_IPV4_ADDRESS)ipAddr).Dot[1], ((ANSC_IPV4_ADDRESS)ipAddr).Dot[2], ((ANSC_IPV4_ADDRESS)ipAddr).Dot[3] );
    if(safec_rc < EOK)
    {
       ERR_CHK(safec_rc);
    }

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
    UNREFERENCED_PARAMETER(hContext);
    UtopiaContext utctx;
    char buf[256];
    errno_t  safec_rc = -1;

    if (!Utopia_Init(&utctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }

    safec_rc = sprintf_s(buf, sizeof(buf), "%d.%d.%d.%d", ((ANSC_IPV4_ADDRESS)ipAddr).Dot[0], ((ANSC_IPV4_ADDRESS)ipAddr).Dot[1], ((ANSC_IPV4_ADDRESS)ipAddr).Dot[2], ((ANSC_IPV4_ADDRESS)ipAddr).Dot[3] );
    if(safec_rc < EOK)
    {
       ERR_CHK(safec_rc);
    }

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
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(ipAddr);
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
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(Flag);
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
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(ipAddr);
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
    UNREFERENCED_PARAMETER(hContext);
    UtopiaContext utctx;
    char buf[256];
    errno_t safec_rc = -1;

    if (!Utopia_Init(&utctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }

    safec_rc = sprintf_s(buf, sizeof(buf), "%d.%d.%d.%d", ((ANSC_IPV4_ADDRESS)ipAddr).Dot[0], ((ANSC_IPV4_ADDRESS)ipAddr).Dot[1], ((ANSC_IPV4_ADDRESS)ipAddr).Dot[2], ((ANSC_IPV4_ADDRESS)ipAddr).Dot[3] );
    if(safec_rc < EOK)
    {
       ERR_CHK(safec_rc);
    }

    Utopia_Set(&utctx, nameServerNo == 1 ? UtopiaValue_WAN_NameServer1 : UtopiaValue_WAN_NameServer2, buf);
	
    Utopia_Free(&utctx, 1);
    
    CcspTraceInfo(("%s vsystem %d \n", __FUNCTION__,__LINE__)); 
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

    CcspTraceInfo(("%s vsystem %d \n", __FUNCTION__,__LINE__)); 
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
    UNREFERENCED_PARAMETER(hContext);
    UtopiaContext utctx;

    if (!Utopia_Init(&utctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }
	
    Utopia_Set(&utctx, UtopiaValue_WAN_ProtoAuthDomain, pValue);
	
    Utopia_Free(&utctx,1);

    CcspTraceInfo(("%s vsystem %d \n", __FUNCTION__,__LINE__)); 
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);

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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
    pValue[0] = '\0';
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetFactoryReset
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    UNREFERENCED_PARAMETER(hContext);
    pValue[0] = '\0';
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetUserChangedFlags
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    UNREFERENCED_PARAMETER(hContext);
    pValue[0] = '0';
    pValue[1] = '\0';
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetDeviceConfigStatus
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
    errno_t rc = -1;
	/* Below pValue size is 1024 bytes having from calling function */
    rc = strcpy_s(pValue, Device_Config_Ignore_size, " notRequire");/*need to modfiy @ivan*/
    ERR_CHK(rc);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetDeviceConfigIgnore
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pValue);
    return ANSC_STATUS_SUCCESS;
}

void*
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
	errno_t  safec_rc = -1;

	safec_rc = sprintf_s(dst_pathname_cr, sizeof(dst_pathname_cr), "%s%s", g_Subsystem, CCSP_DBUS_INTERFACE_CR);
	if(safec_rc < EOK)
	{
		ERR_CHK(safec_rc);
	}

	ret = CcspBaseIf_discComponentSupportingNamespace(bus_handle,
		    dst_pathname_cr,
//		    "Device.WiFi.Radio.",
		    "Device.WiFi.X_CISCO_COM_ResetRadios",
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
					(void*)&val, 
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
			safec_rc = strcpy_s(buf, sizeof(buf), "date");
			if(safec_rc != EOK)
			{
				ERR_CHK(safec_rc);
			}
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
	return hContext;
}

ANSC_STATUS
CosaDmlDcResetBr0(char *ip, char *sub) {
	int ret;
	
	char objName[256]="Device.WiFi.X_RDKCENTRAL-COM_Br0_Sync";
	char objValue[256]={0};
	parameterValStruct_t  value[1] = {{ objName, objValue, ccsp_string}};
	
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
				(void*)&value,
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

#ifdef _XF3_PRODUCT_REQ_
static int openCommonSyseventConnection() {
    if (commonSyseventFd == -1) {
        commonSyseventFd = s_sysevent_connect(&commonSyseventToken);
    }
    return 0;
}
#endif

void* CosaDmlDcRestartRouter(void* arg)
{
    pthread_detach(pthread_self());
    v_secure_system("sysevent set lan-stop");

	sleep(3);
    v_secure_system("sysevent set lan_restarted true");
    v_secure_system("sysevent set forwarding-restart");
#if 0
    int count = 0;
    char statusValue[256] = {0};
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
    return arg;      
}

ANSC_STATUS
CosaDmlDcSetRebootDevice
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    UNREFERENCED_PARAMETER(hContext);
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
		char buffer[50] = {0};
        fp = popen("date", "r");
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
#if defined (_CBR_PRODUCT_REQ_) || defined (_BWG_PRODUCT_REQ_)
//TCCBR-4087
            int                         ret = -1;
            int                         size = 0;
            componentStruct_t **        ppComponents = NULL;
            char*   faultParam = NULL;
            extern char        g_Subsystem[32];
            char   dst_pathname_cr[64]  =  {0};
            errno_t safec_rc = -1;
            CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;

            safec_rc = sprintf_s(dst_pathname_cr, sizeof(dst_pathname_cr), "%s%s", g_Subsystem, CCSP_DBUS_INTERFACE_CR);
            if(safec_rc < EOK)
            {
               ERR_CHK(safec_rc);
            }

            ret = CcspBaseIf_discComponentSupportingNamespace(bus_handle,
                    dst_pathname_cr,
                    "Device.WiFi.Radio.",
                    g_Subsystem,        /* prefix */
                    &ppComponents,
                    &size);

            if ( ret == CCSP_SUCCESS && size == 1)
            {

                parameterValStruct_t val[ ] = { { "Device.WiFi.Radio.1.X_CISCO_COM_ApplySetting", "true", ccsp_boolean},
			{ "Device.WiFi.Radio.1.X_CISCO_COM_ApplySettingSSID", "1", ccsp_int},
			{ "Device.WiFi.Radio.2.X_CISCO_COM_ApplySetting", "true", ccsp_boolean},
			{ "Device.WiFi.Radio.2.X_CISCO_COM_ApplySettingSSID", "1", ccsp_int} };

                ret = CcspBaseIf_setParameterValues
                    (
                     bus_handle,
                     ppComponents[0]->componentName,
                     ppComponents[0]->dbusPath,
                     0, 0x0,  
                     val,
                     2,
                     TRUE,   
                     &faultParam
                    );



                if (ret != CCSP_SUCCESS && faultParam)
                {   
                    AnscTraceError(("Error:Failed to SetValue for param '%s'\n", faultParam));
                    bus_info->freefunc(faultParam);
                }

                free_componentStruct_t(bus_handle, size, ppComponents);
                } /*TCCBR-4087*/
#endif
            CcspTraceWarning(("RebootDevice:Device is going to reboot to restore configuration \n"));
        }
		
		CcspTraceWarning(("REBOOT_COUNT : %d Time : %s  \n",rebootcount,buffer));

        if(delay) {
            if(delay_time)
            {
            	fprintf(stderr, "Device is going to reboot in %d seconds\n", delay_time);
		CcspTraceWarning(("RebootDevice:Device is going to reboot after taking log backups \n"));
            	//system("(sleep 5 && reboot) &");
            	CosaDmlDcSaveWiFiHealthStatusintoNVRAM( );
            	sleep (delay_time);
            	v_secure_system("/rdklogger/backupLogs.sh &");
        	}
        	else
            {
                fprintf(stderr, "Device is going to reboot in 5 seconds\n");
		CcspTraceWarning(("RebootDevice:Device is going to reboot after taking log backups \n"));
                //system("(sleep 5 && reboot) &");
				CosaDmlDcSaveWiFiHealthStatusintoNVRAM( );
				sleep(5);
				v_secure_system("/rdklogger/backupLogs.sh &");
            }
		}
		else {
	        fprintf(stderr, "Device is going to reboot now\n");
			CcspTraceWarning(("RebootDevice:Device is going to reboot after taking log backups \n"));
	         //system("reboot");
 			 CosaDmlDcSaveWiFiHealthStatusintoNVRAM( );
	         v_secure_system("/rdklogger/backupLogs.sh &");
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

void* restoreAllDBs(void* arg)
{
    UNREFERENCED_PARAMETER(arg);
    pthread_detach(pthread_self());
    CcspTraceWarning(("FactoryReset:%s in thread  Restoring all the DBs to factory defaults  ...\n",__FUNCTION__));
    v_secure_system("rm -f /nvram/TLVData.bin"); //Need to remove TR69 TLV data.
    v_secure_system("rm -f /nvram/reverted"); //Need to remove redirection reverted flag
    //Need to remove the encrypted shared keys
    v_secure_system("rm -f /nvram/.keys/*");
    v_secure_system("rm -f /nvram/partners_defaults.json");
    v_secure_system("rm -f /nvram/bootstrap.json");
    v_secure_system( "touch /nvram/.apply_partner_defaults" );

    //Need to remove the custom maintenance window
    v_secure_system("rm -f /nvram/.FirmwareUpgradeEndTime");
    v_secure_system("rm -f /nvram/.FirmwareUpgradeStartTime");

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
            return NULL;
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
#ifdef ARRIS_XB3_PLATFORM_CHANGES
                char *args[] = {"rpcclient", urlPtr, "/bin/rm -f /nvram/syscfg.db /nvram/.keys/vyinerkyo.wyr", (char *) 0 };
#else
                char *args[] = {"rpcclient", urlPtr, "/bin/rm -f /nvram/syscfg.db", (char *) 0 };
#endif
                execv(args[0], args);
                _exit(EXIT_FAILURE);   // exec never returns
            }
        }
    }
#endif

        // Need to remove lxy database
#if defined(_LXY_CXB3_ATOM_IP_)
        #define ATOM_IP "169.254.101.2"
        v_secure_system("/usr/bin/rpcclient "ATOM_IP" \""RM_L2_PATH"\"");
#endif
#if defined(_LXY_AXB3_ATOM_IP_)
        #define ATOM_IP "192.168.254.254"
        v_secure_system("/usr/bin/rpcclient "ATOM_IP" \""RM_L2_PATH"\"");
#endif
#if defined(_ARRIS_XB6_PRODUCT_REQ_)
        #define PEER_INTERFACE_IP "192.168.254.253"
        #define ID "/tmp/elxrretyt-lxy.swr"
        v_secure_system("/usr/bin/GetConfigFile "ID"");
        v_secure_system("ssh -i "ID" root@"PEER_INTERFACE_IP" "RM_L2_PATH"");
        v_secure_system("rm -f "ID"");
#endif

#if defined (_CBR_PRODUCT_REQ_) || (defined (_XB7_PRODUCT_REQ_) && defined (_COSA_BCM_ARM_))
	v_secure_system("rm -f /data/nvram /data/nvram_bak");
	v_secure_system("touch /tmp/wifi_factory_reset");
        /* Remove maintenance window data on factory reset */
        v_secure_system("rm -f /nvram/.FirmwareUpgradeEndTime");
        v_secure_system("rm -f /nvram/.FirmwareUpgradeStartTime");
#endif
#if (defined (_XB7_PRODUCT_REQ_) && defined (_COSA_BCM_ARM_)) || defined (_CBR2_PRODUCT_REQ_)
        v_secure_system("rm -f /data/macaddress_all_updated");
        v_secure_system("rm -f /nvram/.bcmwifi_primary /nvram/.bcmwifi_rmCrashLogs /nvram/.bcmwifi_xhs_lnf_enabled");
        v_secure_system("touch /nvram/brcm_wifi_factory_reset");
#endif
#if defined (_COSA_BCM_ARM_) && !defined (_HUB4_PRODUCT_REQ_)
	/* Clear cable modem's dynamic nonvol settings */
	v_secure_system("latticecli -n \"set Cm.ResetNonvolNoReboot 1\"");
#endif
#if defined (_XB6_PRODUCT_REQ_) && defined (_COSA_BCM_ARM_)
        CcspTraceWarning(("FactoryReset:%s in thread  Restoring moca to factory defaults  ...\n",__FUNCTION__));
        v_secure_system("rm -f /nvram/moca.conf.default"); //TCXB6-1028
        v_secure_system("rm -f /nvram/*.moca0"); //TCXB6-1028
#endif
        v_secure_system("rm -f /nvram/mesh_enabled"); //RDKB-23900
#if defined (_ARRIS_XB6_PRODUCT_REQ_)
	v_secure_system("rm -f /nvram/etc/passwd"); //ARRISXB6-7330
	v_secure_system( "arris_rpc_client arm nvm_reset" ); //ARRISXB6-7323
#elif defined(_COSA_BCM_MIPS_)
        v_secure_system("xf3_erase_nvram");
#elif defined(_HUB4_PRODUCT_REQ_)
	v_secure_system("rm -f /nvram/ETH_WAN_PORT_RECLAIMED");
	v_secure_system("rm -rf /nvram/lxy");
        v_secure_system("rm -rf /nvram/certs");
        v_secure_system("/bin/sh /etc/sky/restore_settings.sh factory_reset");
#else
	v_secure_system("restoreAllDBs"); //Perform factory reset on other components
#endif
#if defined (_XB6_PRODUCT_REQ_) && defined (_COSA_BCM_ARM_)
	v_secure_system("rm -rf /opt/secure/Apparmor_blocklist");
#endif
	return NULL;
}

void* backuplogs(void *thread)
{
	void *ret;
	int s;
	if(thread != NULL)
	{
		pthread_t thread_id = (pthread_t) thread;
		CcspTraceWarning(("FactoryReset:%s Wait for WiFi reset to complete\n",__FUNCTION__));
		s = pthread_join(thread_id, &ret);
		if (!s)
			CcspTraceWarning(("FactoryReset:%s WiFi reset is now completed\n",__FUNCTION__));
	}
	pthread_detach(pthread_self());

	v_secure_system("/rdklogger/backupLogs.sh &");
    return NULL;
}

void* resetWiFi(void* arg)
{
    UNREFERENCED_PARAMETER(arg);
	CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
	/*TODO: SEND EVENT TO WIFI PAM  Device.WiFi.X_CISCO_COM_FactoryReset*/
	int ret;
	char* faultParam = NULL;

	CcspTraceWarning(("FactoryReset:%s Restoring WiFi to factory defaults  ...\n",__FUNCTION__));     
	if (ppComponents == NULL && initWifiComp())
	{
		CcspTraceError(("FactoryReset:%s Restoring WiFi to factory defaults returned error  ...\n",__FUNCTION__));     
		return NULL;
	}
	parameterValStruct_t	val = { "Device.WiFi.X_CISCO_COM_FactoryReset", "true", ccsp_boolean};

#if defined (_XB6_PRODUCT_REQ_) && !defined (_XB7_PRODUCT_REQ_)
	/* In Reset Factory Settings, the system is rebooted  before WiFi gets reset.
	* Add this line to indicate WiFi module to restore after boot up.
	* This will be removed iff CCSP layer adds sufficient delay during reboot. */
	v_secure_system("echo 2 >/nvram/qtn_wifi_reset_indicator");
#endif

#if defined (_INTEL_WAV_)
	v_secure_system("touch /nvram/wifi_reset_indicator");
#endif

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
#if defined (_XB6_PRODUCT_REQ_) || defined(_COSA_BCM_MIPS_) || (defined(_HUB4_PRODUCT_REQ_) && !defined(_SR300_PRODUCT_REQ_))
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
    return NULL;
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
    UNREFERENCED_PARAMETER(hContext);
	char* tok;
	char* sv;
    char value[50];
	int factory_reset_mask = 0;
	UtopiaContext utctx = {0};
	static pthread_t wifiThread;
	int wifiThreadStarted=0;
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
		
#if defined (_XB6_PRODUCT_REQ_) || defined (_XB7_PRODUCT_REQ_)
                if(factory_reset_mask & FR_ROUTER){
			LEDMGMT_PARAMS ledMgmt;
			memset(&ledMgmt, 0, sizeof(LEDMGMT_PARAMS));
			ledMgmt.LedColor = LED_GREEN;
			ledMgmt.State	 = LED_BLINK;
			ledMgmt.Interval = FR_BLINK_INTRVL;

			if(0 == platform_hal_setLed(&ledMgmt)) {
                        	CcspTraceInfo(("Front LED Transition: GREEN LED will blink, Reason: Factory Reset\n"));
				v_secure_system("touch /tmp/.FRBLINKGREEN");
			}
			else {
				CcspTraceError(("[%s]:Setting Front LED to Blink Green Failed\n",__FUNCTION__));
			}
		}
#endif
		
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
#if (defined (_XB6_PRODUCT_REQ_) || defined (_CBR_PRODUCT_REQ_)) && defined (_COSA_BCM_ARM_) || defined (_HUB4_PRODUCT_REQ_) || defined (_PLATFORM_RASPBERRYPI_)
                {
                        unsigned int dbValue = 0;
                        FILE *pdbFile = NULL;
                        char buf[128]={0};
                        #define FACTORY_RESET_COUNT_FILE "/nvram/.factory_reset_count"
                        pdbFile = fopen(FACTORY_RESET_COUNT_FILE, "r");
                        if(pdbFile != NULL){
                           fread(buf,sizeof(buf),1,pdbFile);
                           fclose(pdbFile);
                           dbValue = atoi(buf);
                        }
                        dbValue++;
                        pdbFile = fopen(FACTORY_RESET_COUNT_FILE, "w+");
                        if(pdbFile != NULL){
                                fprintf(pdbFile,"%d\n",dbValue);
                                fclose(pdbFile);
                        }
                }
                if(factory_reset_mask & FR_ROUTER){
                        unsigned int dbValue = 0;
                        FILE *pdbFile = NULL;
                        char buf[128]={0};
                        #define ROUTER_RESET_COUNT_FILE "/nvram/.router_reset_count"
                        pdbFile = fopen(ROUTER_RESET_COUNT_FILE, "r");
                        if(pdbFile != NULL){
                           fread(buf,sizeof(buf),1,pdbFile);
                           fclose(pdbFile);
                           dbValue = atoi(buf);
                        }
                        dbValue++;
                        pdbFile = fopen(ROUTER_RESET_COUNT_FILE, "w+");
                        if(pdbFile != NULL){
                                fprintf(pdbFile,"%d\n",dbValue);
                                fclose(pdbFile);
                        }
                }
#endif
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
#if defined (_HUB4_PRODUCT_REQ_) || defined(_SR300_PRODUCT_REQ_)
	CcspTraceWarning(("[%s]:Restoring Voice to factory default ...\n",__FUNCTION__));
	int ret = -1;
	char* faultParam = NULL;
	errno_t safec_rc = -1;
	componentStruct_t **        ppVoiceComponent = NULL;
	int                         size = 0;
#ifdef FEATURE_RDKB_TELCOVOICE_MANAGER
        parameterValStruct_t    val = { "Device.Services.VoiceService.1.X_RDK_FactoryReset", "true", ccsp_boolean};
#else
	parameterValStruct_t    val = { "Device.Services.VoiceService.1.X_RDK-Central_COM_VoiceProcessState", "FactoryDefault", ccsp_string};
#endif
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
	safec_rc = sprintf_s(dst_pathname_cr, sizeof(dst_pathname_cr),"%s%s", g_Subsystem, CCSP_DBUS_INTERFACE_CR);
	if(safec_rc < EOK)
	{
		ERR_CHK(safec_rc);
	}

	ret = CcspBaseIf_discComponentSupportingNamespace(bus_handle,
	                                               dst_pathname_cr,
	                                               "Device.Services.VoiceService.",
	                                               g_Subsystem,
	                                               &ppVoiceComponent,
	                                               &size);

	if ( ret == CCSP_SUCCESS && size == 1)
	{
	    ret = CcspBaseIf_setParameterValues
		(
			bus_handle,
			ppVoiceComponent[0]->componentName,
			ppVoiceComponent[0]->dbusPath,
			0, 0x0,   /* session id and write id */
			&val,
			1,
			TRUE,   /* no commit */
			&faultParam
		);

	   if (ret != CCSP_SUCCESS && faultParam)
	   {
#ifdef FEATURE_RDKB_TELCOVOICE_MANAGER
		CcspTraceError(("[%s]:Setting X_RDK_FactoryReset returned error '%s'  ...\n",__FUNCTION__,faultParam));
#else
		CcspTraceError(("[%s]:Setting X_RDK-Central_COM_VoiceProcessState returned error '%s'  ...\n",__FUNCTION__,faultParam));
#endif
		bus_info->freefunc(faultParam);
	   }
	}
#endif

#if defined (INTEL_PUMA7)
	CcspTraceWarning (("***** New API call to MOCA FactoryReset: Restoring MOCA to factory defaults  ...\n")); //ARRISXB6-7326
	moca_factoryReset();
#endif
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
		//Set LastRebootReason before device bootup
		CcspTraceWarning(("FactoryReset:%s Set LastRebootReason to factory-reset ...\n",__FUNCTION__));
		if ((syscfg_set(NULL, "X_RDKCENTRAL-COM_LastRebootReason", "factory-reset") != 0))
		{
                        AnscTraceWarning(("syscfg_set failed\n"));
			return -1;
		}
		else
		{
                        if (syscfg_commit() != 0)
			{
				AnscTraceWarning(("syscfg_commit failed\n"));
				return -1;
			}
		}

#ifdef _MACSEC_SUPPORT_
                //TCXB7-1453
                char partnerId[20];
                BOOLEAN currentMacSecRequired = FALSE;

                if(RETURN_OK == platform_hal_GetMACsecEnable( ETHWAN_DEF_INTF_NUM, &currentMacSecRequired ))
                {
                   if(!syscfg_get(NULL, "PartnerID", partnerId, sizeof(partnerId))) 
                   {
                        if (strcmp( "comcast", partnerId ) == 0 ) 
                        {
                           if( currentMacSecRequired != TRUE) 
                           {
                              CcspTraceInfo(("MACsec Required enabled by factory reset\n"));
                              platform_hal_SetMACsecEnable( ETHWAN_DEF_INTF_NUM, true );
                           }
                           else
                           {
                              CcspTraceInfo(("MACsec Required already enabled not calling API"));
                           }
                        } 
                        else 
                        {
                           if( currentMacSecRequired != FALSE) 
                           {
                              CcspTraceInfo(("MACsec Required disabled by factory reset\n"));
                              platform_hal_SetMACsecEnable( ETHWAN_DEF_INTF_NUM, false);
                          }
                           else
                           {
                              CcspTraceInfo(("MACsec Required already disabled not calling API"));
                           }
                        }
                   }
                }
                else
                {
                   CcspTraceInfo((" Unable to retrieve current MACsec Required config\n"));
                }
#endif //_MACSEC_SUPPORT_
#ifdef COLUMBO_HWTEST
                /*
                 * RDKB-31738: HHT Enhancements
                 * As per AC 5, if there is no space in /tmp to write the HHT results,
                 * the results should be saved into /nvram.
                 * This results file will get removed in the next HHT run if there is
                 * space in /tmp directory OR if a FactoryReset is performed.
                 */
                FILE *resFile = fopen("/nvram/hwselftest.results","r");
                if(resFile)
                {
                    fclose(resFile);
                    if(remove("/nvram/hwselftest.results") != 0)
                    {
                        CcspTraceWarning(("FactoryReset:Could not remove HardwareHealthTest results file from nvram\n"));
                    }
                }
#endif
                if ((syscfg_set(NULL, "X_RDKCENTRAL-COM_LastRebootCounter", "1") != 0))
                {
                        AnscTraceWarning(("syscfg_set failed\n"));
                        return -1;
                }
                else
                {
                        if (syscfg_commit() != 0)
                        {
                                AnscTraceWarning(("syscfg_commit failed\n"));
                                return -1;
                        }
                }
	}
	if (factory_reset_mask & FR_WIFI) {
           
           if (!pthread_create(&wifiThread, NULL, &resetWiFi, NULL))
		   wifiThreadStarted=1;
	}
	if (factory_reset_mask & FR_NONE){
	}
	// do backup logs 
	if (factory_reset_mask & FR_ROUTER) {
           pthread_t logs;
	      if (wifiThreadStarted){
		//if wifithread staretd, pass wifi thread id so backulogs thread waits for it to complete before starting backup
		pthread_create(&logs, NULL, &backuplogs, (void*)wifiThread);
	      }else
		pthread_create(&logs, NULL, &backuplogs, NULL);
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
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pValue);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetResetDefaultEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     pFlag
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pFlag);
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
    v_secure_system(CMD_START_AVAHI_DAEMON);
    v_secure_system(CMD_START_AVAHI_AUTOIPD);

    return;
}

void _CosaDmlDcStopZeroConfig()
{
    /* kill two daemon no matter whether it's successful. */
    AnscTraceWarning(("_CosaDmlDcStopZeroConfig -- stop avahi.\n"));
    
    v_secure_system(CMD_STOP_AVAHI_DAEMON);
    v_secure_system(CMD_STOP_AVAHI_AUTOIPD);
    
    return;
}

ANSC_STATUS
CosaDmlDcGetEnableStaticNameServer
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                    *pFlag
    )
{
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
    UtopiaContext ctx;
    boolean_t bEnabled = bFlag;

    if (!Utopia_Init(&ctx))
    {
        CcspTraceWarning(("X_CISCO_COM_DeviceControl: Error in initializing context!!! \n" ));
        return ANSC_STATUS_FAILURE;
    }

    Utopia_SetBool(&ctx, UtopiaValue_WAN_EnableStaticNameServer, bEnabled);
    Utopia_Free(&ctx, 1);
    
    //get nameservers by DHCP if static nameservers not enabled on GUI
    if(!bFlag)
    {
	// Call set_resolv_conf to delete static dns entries from dns server
        vsystem("/bin/sh /etc/utopia/service.d/set_resolv_conf.sh");
        commonSyseventSet("wan-restart", "");
    }
    else{
        CcspTraceInfo(("%s vsystem %d \n", __FUNCTION__,__LINE__)); 
        if (vsystem("/bin/sh /etc/utopia/service.d/set_resolv_conf.sh") != 0) {
            fprintf(stderr, "%s: fail to set resolv.conf\n", __FUNCTION__);
            return ANSC_STATUS_FAILURE;
        }
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetReleaseWan
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                    *pFlag
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pFlag);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetReleaseWan
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bFlag
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(bFlag);
    v_secure_system("sysevent set dhcp_client-release");
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetRenewWan
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                    *pFlag
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pFlag);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetRenewWan
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bFlag
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(bFlag);
    char status[10] = {'\0'};
    int timeOut = 15;

    v_secure_system("sysevent set dhcp_client-renew");
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
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
		/*
        else
        {
            //stop avahi two applications
            _CosaDmlDcStopZeroConfig();
        }*/

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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
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
    snprintf(buf,sizeof(buf),"%lu",value);
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
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
            vsystem("igmpproxy &");
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
    if (pFlag)
    {
        if ( detect_process("dnsproxy") == 0 )
        {
            vsystem("dnsproxy &");
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
    UNREFERENCED_PARAMETER(hContext);
    #if defined(_CBR_PRODUCT_REQ_) || defined (_BWG_PRODUCT_REQ_)
        return ANSC_STATUS_FAILURE;
    #endif

    char buf[5];
    //printf("Got value mgmt_wan_telnetaccess = %d\n", *pFlag);
    /*CID: 70419 Array compared against 0*/
    if(!syscfg_get( NULL, "mgmt_wan_telnetaccess", buf, sizeof(buf)))
    {
        //printf("%s buf = %s\n", __FUNCTION__, buf);
        if (strcmp(buf,"1") == 0)
            *pFlag = 1;
        else
            *pFlag = 0;
    } else
        return ANSC_STATUS_FAILURE;
 
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
    UNREFERENCED_PARAMETER(hContext);
    char buf[5];
    /*CID: 58920 Array compared against 0*/
    if(!syscfg_get( NULL, "mgmt_wan_sshaccess", buf, sizeof(buf)))
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
    #if defined(_CBR_PRODUCT_REQ_) || defined (_BWG_PRODUCT_REQ_)
        return ANSC_STATUS_FAILURE;
    #endif
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
        v_secure_system("sysevent set firewall-restart");

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
    UNREFERENCED_PARAMETER(hContext);
    char buf[5];
    BOOLEAN bSSHEnable;

    if (CosaDmlDcGetSSHEnable(NULL, &bSSHEnable) == ANSC_STATUS_FAILURE)
            return ANSC_STATUS_FAILURE;

    if (flag != bSSHEnable) {
        snprintf(buf,sizeof(buf),"%d",flag);
        syscfg_set( NULL, "mgmt_wan_sshaccess", buf);
        syscfg_commit();
        v_secure_system("sysevent set firewall-restart");
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
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(flag);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetHTTPEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pValue
    )
{
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
    WebServConf_t conf;

    if (LoadWebServConf(&conf) != 0)
        return ANSC_STATUS_FAILURE;

    *pValue = conf.httpsport;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetReinitMacThreshold
    (
        ANSC_HANDLE                 hContext,
        ULONG                       value
    )
{
#ifdef ARRIS_XB3_PLATFORM_CHANGES
    UNREFERENCED_PARAMETER(hContext);
    if ( cm_hal_set_ReinitMacThreshold(value) != RETURN_OK )
        return ANSC_STATUS_FAILURE;
    else
        return ANSC_STATUS_SUCCESS;

#else
    UNREFERENCED_PARAMETER(hContext);
    char buf[12];
    errno_t safec_rc = -1;
    safec_rc = sprintf_s(buf, sizeof(buf), "%lu",value);
    if(safec_rc < EOK)
    {
       ERR_CHK(safec_rc);
    }
	
    if((syscfg_set( NULL, "rdkbReinitMacThreshold", buf)) != 0 )
    {
    	AnscTraceWarning(("setrdkbReinitMacThreshold : syscfg_set failed\n"));
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        if (syscfg_commit() != 0)
        {
            AnscTraceWarning(("setrdkbReinitMacThreshold : syscfg_commit failed\n"));
            return ANSC_STATUS_FAILURE;
        }
        return ANSC_STATUS_SUCCESS;
    }
#endif
}

ANSC_STATUS
CosaDmlDcGetReinitMacThreshold
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *pValue
    )
{
#ifdef ARRIS_XB3_PLATFORM_CHANGES
    UNREFERENCED_PARAMETER(hContext);
    if ( cm_hal_get_ReinitMacThreshold(pValue) != RETURN_OK )
        return ANSC_STATUS_FAILURE;
    else
        return ANSC_STATUS_SUCCESS;

#else
    char buf[12];

    if( (syscfg_get( NULL, "rdkbReinitMacThreshold", buf, sizeof(buf))) == 0 )
    {
    	*pValue = atoi(buf);
    }
    else
    {
    	*pValue = 5;
    	CosaDmlDcSetReinitMacThreshold(hContext, *pValue);
    }
    return ANSC_STATUS_SUCCESS;
#endif
}

ANSC_STATUS
CosaDmlDcSetWebServer(BOOL httpEn, BOOL httpsEn, ULONG httpPort, ULONG httpsPort)
{
    UNREFERENCED_PARAMETER(httpEn);
    UNREFERENCED_PARAMETER(httpsEn);
    WebServConf_t conf;
    pthread_t tid;

    /* do not support disable HTTP/HTTPS */
    conf.httpport = httpPort;
    conf.httpsport = httpsPort;

    if (SaveWebServConf(&conf) != 0)
        return ANSC_STATUS_FAILURE;
   
    //Needs to start as thread to avoid PAM Hung issue
    pthread_create( &tid, NULL, &WebServRestart, NULL);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetMsoRemoteMgmtEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    )
{
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
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
    
    Cdm_GetParamBool(cmdBuf, (bool*)pFlagcpv);
    
    return ANSC_STATUS_SUCCESS;
    
}

ANSC_STATUS
CosaDmlDcSetHSEthernetPortEnable
    (
        ANSC_HANDLE                hContext,
        BOOLEAN                    pFlag
    )
{
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
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

    v_secure_system("sysevent set firewall-restart");

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

    snprintf(maxNum, sizeof(maxNum), "%lu", uVal);
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
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

    v_secure_system("sysevent set firewall-restart");

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetParConQuestion
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
    UtopiaContext utctx;

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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(index);
    lanSetting_t lan;
    ANSC_IPV4_ADDRESS network, netmask, ipaddr;
    bridgeInfo_t bridge_info = {0}; /*RDKB-6845, CID-33087, initialize before use*/
    int int_tmp;
    napt_mode_t napt_mode; 
    /* Till now,just support only one lan interface */
    /* ignor the index */
    Utopia_GetLanMngmInsNum(utctx, &(pLanMngm->InstanceNumber));
    Utopia_GetLanMngmAlias(utctx, pLanMngm->Alias, sizeof(pLanMngm->Alias));
    Utopia_GetBridgeSettings(utctx, &bridge_info);

	/* 
	  * Configure Bridge Static Mode Configuration 
	  * if COSA_DML_LanMode_BridgeStatic then BridgeStaticMode then "Advanced Bridge" 2
	  * if COSA_DML_LanMode_FullBridgeStatic then BridgeStaticMode then "Primary Bridge" 4		  
	  */
	
	switch( bridge_info.mode )
	{
		case BRIDGE_MODE_STATIC:
		{
			pLanMngm->LanMode = COSA_DML_LanMode_BridgeStatic;
		}
		break; /* BRIDGE_MODE_STATIC */
	
		case BRIDGE_MODE_FULL_STATIC:
		{
			pLanMngm->LanMode = COSA_DML_LanMode_FullBridgeStatic;
		}
		break; /* BRIDGE_MODE_FULL_STATIC */
	
		case BRIDGE_MODE_OFF:
		{
			pLanMngm->LanMode = COSA_DML_LanMode_Router;
		}
		break; /* BRIDGE_MODE_OFF */

		default:
		{
			pLanMngm->LanMode = COSA_DML_LanMode_Router; 
		}
		break;
	}

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
        if(index < (ULONG)num ){
            _Get_LanMngm_Setting(&utctx, index, pLanMngm);
            ret = ANSC_STATUS_SUCCESS;
        }
        Utopia_Free(&utctx, 0);
    }
    return ret;
}

ANSC_STATUS
CosaDmlLanMngm_SetValues(ULONG index, ULONG ins, const char *alias)
{
    UNREFERENCED_PARAMETER(index);
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
    UNREFERENCED_PARAMETER(pLanMngm);
    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDmlLanMngm_DelEntry(ULONG ins)
{
    UNREFERENCED_PARAMETER(ins);
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
    char*   faultParam = NULL;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    ANSC_STATUS ret = ANSC_STATUS_FAILURE;
	char acBridgeMode[ 4 ],
		 acSetRadioString[ 8 ],
		 acSetSSIDString[ 8 ];
	errno_t safec_rc = -1;

    checkTicket(pNotify->ticket);

	/* 
	  * Configure Bridge Static Mode Configuration 
	  * if BridgeStaticMode then "Advanced Bridge" 2 then COSA_DML_LanMode_BridgeStatic
	  * if BridgeStaticMode then "Primary Bridge" 4 then COSA_DML_LanMode_FullBridgeStatic
	  * 
	  * if "Advanced Bridge" then disable Private SSIDs only
	  * if "Primary Bridge" then disable both Radios and Private SSIDs too
	  */

	memset( acBridgeMode, 0 ,sizeof( acBridgeMode ) );
	syscfg_get( NULL, "bridge_mode", acBridgeMode, sizeof(acBridgeMode));

	switch( atoi( acBridgeMode ) )
	{
		case BRIDGE_MODE_STATIC:
		{
			safec_rc = strcpy_s( acSetRadioString, sizeof(acSetRadioString), "true" );
			if(safec_rc != EOK)
			{
				ERR_CHK(safec_rc);
			}
			safec_rc = strcpy_s( acSetSSIDString,sizeof(acSetSSIDString), "false" );
			if(safec_rc != EOK)
			{
				ERR_CHK(safec_rc);
			}
		}
		break; /* BRIDGE_MODE_STATIC */

		case BRIDGE_MODE_FULL_STATIC:
		{
			safec_rc = strcpy_s( acSetRadioString, sizeof( acSetRadioString ), "false" );
			if(safec_rc != EOK)
			{
				ERR_CHK(safec_rc);
			}
			safec_rc = strcpy_s( acSetSSIDString,sizeof( acSetSSIDString ), "false" );
			if(safec_rc != EOK)
			{
				ERR_CHK(safec_rc);
			}
		}
		break; /* BRIDGE_MODE_FULL_STATIC */

		default: /* BRIDGE_MODE_OFF */
		{
			safec_rc = strcpy_s( acSetRadioString,sizeof( acSetRadioString ), "true" );
			if(safec_rc != EOK)
			{
				ERR_CHK(safec_rc);
			}
			safec_rc = strcpy_s( acSetSSIDString,sizeof( acSetSSIDString ), "true" );
			if(safec_rc != EOK)
			{
				ERR_CHK(safec_rc);
			}
		}
		break;
	}

	AnscTraceInfo(("%s - Mode:%d Radio:%s SSID:%s\n", __FUNCTION__, atoi( acBridgeMode ), acSetRadioString, acSetSSIDString ));
    
#ifdef CONFIG_CISCO_FEATURE_CISCOCONNECT
    char param[50];
    char* enVal = NULL;
    char* guestnetDM = NULL;
    char* guestEnableStr = NULL;
    char* enableStr = (char*)(pNotify->flag?"false" : "true");
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
 {"Device.WiFi.Radio.1.Enable", acSetRadioString, ccsp_boolean}, 
 {"Device.WiFi.Radio.2.Enable", acSetRadioString, ccsp_boolean},
#if !defined (_CBR_PRODUCT_REQ_) && !defined (_BWG_PRODUCT_REQ_) // CBR and BWG don't have XHS don't force here
 {"Device.WiFi.SSID.3.Enable", acSetRadioString, ccsp_boolean}
#endif
};
 
// Pseudo bridge
 parameterValStruct_t val2[] = { 
 {"Device.WiFi.SSID.1.Enable", acSetSSIDString, ccsp_boolean}, 
 {"Device.WiFi.SSID.2.Enable", acSetSSIDString, ccsp_boolean}};

parameterValStruct_t valCommit1[] = {
 {"Device.WiFi.Radio.1.X_CISCO_COM_ApplySetting", "true", ccsp_boolean}, {"Device.WiFi.Radio.2.X_CISCO_COM_ApplySetting", "true", ccsp_boolean} };

#ifdef _XF3_PRODUCT_REQ_
 parameterValStruct_t valCommit2[] = { {"Device.WiFi.X_CISCO_COM_ResetRadios", "true", ccsp_boolean} };
#endif 
 
    // All the cases Radio should get update since transition will happen during full - psedo - router
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
            
	// All the cases Radio should get update since transition will happen during full - psedo - router
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
                    faultParam = NULL;
                }  
        
               
      ret = CcspBaseIf_setParameterValues
                        (
                                bus_handle,
                                ppComponents[0]->componentName,
                                ppComponents[0]->dbusPath,
                                0, 0x0,   /* session id and write id */
                                valCommit1,
                                2,
                                TRUE,   /* no commit */
                                &faultParam
                        );
                if (ret != CCSP_SUCCESS && faultParam)
                {
                    AnscTraceError(("Error:Failed to SetValue for param '%s'\n", faultParam));
                    bus_info->freefunc(faultParam);
                    faultParam = NULL;
                }  
  #ifdef _XF3_PRODUCT_REQ_
      ret = CcspBaseIf_setParameterValues
                        (
                                bus_handle,
                                ppComponents[0]->componentName,
                                ppComponents[0]->dbusPath,
                                0, 0x0,   /* session id and write id */
                                valCommit2,
                                1,
                                TRUE,   /* no commit */
                                &faultParam
                        );
 #endif
        if (ret != CCSP_SUCCESS && faultParam)
        {
            AnscTraceError(("Error:Failed to SetValue for param '%s'\n", faultParam));
            bus_info->freefunc(faultParam);
            faultParam = NULL;
        } 

	// All the cases Radio should get update since transition will happen during full - psedo - router
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
                    faultParam = NULL;
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
    napt_mode_t napt;
#if !defined(_CBR_PRODUCT_REQ_) && !defined(_PLATFORM_RASPBERRYPI_) && !defined(_HUB4_PRODUCT_REQ_) && !defined(_PLATFORM_TURRIS_)// MOCA is not present for TCCBR environment, HUB4 and RaspberryPi environment
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

		/* 
		  * Configure Bridge Static Mode Configuration 
		  * if COSA_DML_LanMode_BridgeStatic then BridgeStaticMode then "Advanced Bridge" 2
		  * if COSA_DML_LanMode_FullBridgeStatic then BridgeStaticMode then "Primary Bridge" 4		  
		  */

		switch( pLanMngm->LanMode )
		{
			case COSA_DML_LanMode_BridgeStatic:
			{
				bridge_info.mode = BRIDGE_MODE_STATIC;
				CcspTraceInfo(("LanMode:Adv_Bridge_Mode_selected\n"));				
			}
			break; /* COSA_DML_LanMode_BridgeStatic */

			case COSA_DML_LanMode_FullBridgeStatic:
			{
				bridge_info.mode = BRIDGE_MODE_FULL_STATIC;
				CcspTraceInfo(("LanMode:Basic_Bridge_Mode_selected\n"));								
			}
			break; /* COSA_DML_LanMode_BridgeStatic */

			case COSA_DML_LanMode_Router:
			{
				bridge_info.mode = BRIDGE_MODE_OFF;
				CcspTraceInfo(("LanMode:Router_Mode_selected\n"));								
			}
			break; /* COSA_DML_LanMode_Router */

			default:
			{
				bridge_info.mode = BRIDGE_MODE_OFF;	
			}
			break;
		}

        Utopia_SetBridgeSettings(&utctx,&bridge_info);
#if !defined(_CBR_PRODUCT_REQ_) && !defined(_PLATFORM_RASPBERRYPI_) && !defined(_HUB4_PRODUCT_REQ_) && !defined(_PLATFORM_TURRIS_)// MOCA is not present for TCCBR environment, HUB4 and RaspberryPi environment
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
                            Utopia_Free(&utctx, 0);
                            CcspTraceWarning(("syscfg_set failed\n"));
                            return -1;
                        }
                        else
                        {
                             if (syscfg_commit() != 0)
                             {
                                    Utopia_Free(&utctx, 0);
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

#if defined(_COSA_INTEL_USG_ARM_) && !defined(INTEL_PUMA7) && defined(ENABLE_FEATURE_MESHWIFI)
        // Send subnet change message to ATOM so that MESH is notified.
        {
            #define DATA_SIZE 1024
            FILE *fp1;
            char buf[DATA_SIZE] = {0};
            char *urlPtr = NULL;
            errno_t safec_rc = -1;

            // Grab the ATOM RPC IP address
            // sprintf(cmd1, "cat /etc/device.properties | grep ATOM_ARPING_IP | cut -f 2 -d\"=\"");

            fp1 = fopen("/etc/device.properties", "r");
            if (fp1 == NULL) {
                CcspTraceError(("Error opening properties file! \n"));
                Utopia_Free(&utctx, 0);
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
                    safec_rc = sprintf_s(cmd, sizeof(cmd), "/usr/bin/sysevent set subnet_change \"RDK|%s|%s\"",
                            lan.ipaddr,lan.netmask);
                    if(safec_rc < EOK)
                    {
                        ERR_CHK(safec_rc);
                    }
                    char *args[] = {"rpcclient", urlPtr, cmd, (char *) 0 };
                    execv(args[0], args);
                    _exit(EXIT_FAILURE);   // exec never returns
                }
            }
        }
#elif defined(ENABLE_FEATURE_MESHWIFI)
        // In all the other platforms XB6, XF3, etc. PandM is running on the same processor as on Mesh, so we just need to
        // send the sysevent call directly.
        {
            #define DATA_SIZE 1024

			pid_t pid = fork();
			errno_t safec_rc = -1;

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
				CcspTraceInfo(("Sending subnet_change notification \n"));
				safec_rc = sprintf_s(cmd, sizeof(cmd), "RDK|%s|%s", lan.ipaddr, lan.netmask);
				if(safec_rc < EOK)
				{
					ERR_CHK(safec_rc);
				}
				char *args[] = {"/usr/bin/sysevent", "set", "subnet_change", cmd, (char *) 0 };
				execv(args[0], args);
				_exit(EXIT_FAILURE);   // exec never returns
			}
        }
#endif
        /* TODO: CID: 54391 Useless call Utopia_SetLanMngmLanNetworksAllow , no definition*/
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
        char l_cSecWebUI_Enabled[8] = {0};
        syscfg_get(NULL, "SecureWebUI_Enable", l_cSecWebUI_Enabled, sizeof(l_cSecWebUI_Enabled));
        if (!strncmp(l_cSecWebUI_Enabled, "true", 4)) { 
        /* If lan settings are changed, restart the webgui.sh */
             if(orgLanMngm.LanIPAddress.Value != pLanMngm->LanIPAddress.Value)
             {
                 pthread_t tid;
                 pthread_create( &tid, NULL, &WebGuiRestart, NULL);
             }
         }    
        

#ifdef _HUB4_PRODUCT_REQ_
	/* If lan settings(gw-ip or subnet-mask) not change, skip refreshing lan_prefix */
	if( (orgLanMngm.LanIPAddress.Value != pLanMngm->LanIPAddress.Value) ||
	    (orgLanMngm.LanSubnetMask.Value != pLanMngm->LanSubnetMask.Value) )
	{
	    /* SKYH4-1780 : This will help to set Down state to
	     * sysevent 'ipv6_connection_state' */
	    CcspTraceInfo(("lan_prefix_clear is setting\n"));
	    commonSyseventSet("lan_prefix_clear", "");
	}
#endif
        if (pLanMngm->LanMode == orgLanMngm.LanMode) {
            return ANSC_STATUS_SUCCESS;
        }
        
        
        
        //Bridge mode has changed, so we need to report the change and toggle wifi accordingly
        //TODO: move this to a thread
#ifdef _XF3_PRODUCT_REQ_
        int bEnable;
#endif
        
        if(bridge_info.mode == BRIDGE_MODE_OFF)
        {
            syslog_systemlog("Local Network", LOG_NOTICE, "Status change: IP %s mask %s", lan.ipaddr, lan.netmask);
            #ifdef _XF3_PRODUCT_REQ_            
                bEnable = 0;
            #endif
        }
        else
        {
            syslog_systemlog("Local Network", LOG_NOTICE, "Status change: Bridge mode");
#ifndef _XF3_PRODUCT_REQ_
            // stop lan when it is bridge mode
		    commonSyseventSet("lan-stop", "");
#endif
#ifdef _XF3_PRODUCT_REQ_
            bEnable = 3;
        }

        char buf[7] = {0};
        snprintf(buf,sizeof(buf),"%d",bEnable);
        openCommonSyseventConnection();
        sysevent_set(commonSyseventFd, commonSyseventToken, "bridge_mode",buf,0);
        configBridgeMode(bEnable);
#else
            //bEnable = 1;
        }
#endif
        
#if defined(_PLATFORM_RASPBERRYPI_) || defined(_PLATFORM_TURRIS_)
	char buf[7] = {0};
	BOOL value;
	snprintf(buf,sizeof(buf),"%d",bridge_info.mode);
	if ((syscfg_set(NULL, "bridge_mode", buf) != 0))
                        {
                            Utopia_Free(&utctx, 0);
                            CcspTraceWarning(("syscfg_set failed\n"));
                            return -1;
                        }
	if(bridge_info.mode == 0)
	{
		value = 1;
		commonSyseventSet("bridge-stop", "");
		commonSyseventSet("lan-start", "");
		if (DmSetBool("Device.WiFi.SSID.1.Enable", value) != ANSC_STATUS_SUCCESS) {
	            fprintf(stderr, "%s: set WiFi.SSID.1 Enable error\n", __FUNCTION__);
        	} else {
            	    fprintf(stderr, "%s: set WiFi.SSID.1 Enable OK\n", __FUNCTION__);
       	        }
		if (DmSetBool("Device.WiFi.SSID.2.Enable", value) != ANSC_STATUS_SUCCESS) {
	            fprintf(stderr, "%s: set WiFi.SSID.2 Enable error\n", __FUNCTION__);
        	} else {
            	    fprintf(stderr, "%s: set WiFi.SSID.2 Enable OK\n", __FUNCTION__);
       	        }
	}
	else if(bridge_info.mode == 2)
	{
		value = 0;
		commonSyseventSet("bridge-start", "");
		commonSyseventSet("lan-stop", "");
		if (DmSetBool("Device.WiFi.SSID.1.Enable", value) != ANSC_STATUS_SUCCESS) {
	            fprintf(stderr, "%s: set WiFi.SSID.1 Disable error\n", __FUNCTION__);
        	} else {
            	    fprintf(stderr, "%s: set WiFi.SSID.1 Disable OK\n", __FUNCTION__);
       	        }
		if (DmSetBool("Device.WiFi.SSID.2.Enable", value) != ANSC_STATUS_SUCCESS) {
	            fprintf(stderr, "%s: set WiFi.SSID.2 Disable error\n", __FUNCTION__);
        	} else {
            	    fprintf(stderr, "%s: set WiFi.SSID.2 Disable OK\n", __FUNCTION__);
       	        }
	}
	else
	{
		fprintf(stderr,"Running in different Modes \n");
	}
	sleep(1);
	vsystem("/bin/sh /etc/webgui.sh &");
#endif
        //configBridgeMode(bEnable);

        if( ( ( bridge_info.mode == BRIDGE_MODE_STATIC ) || \
			   ( bridge_info.mode == BRIDGE_MODE_FULL_STATIC ) ) && \
			 ( is_mesh_enabled( ) )
		   )
        {
            CcspTraceWarning(("Setting MESH to disabled as LanMode is changed to Bridge mode\n"));
            pthread_t tid;
            pthread_create(&tid, NULL, &set_mesh_disabled, NULL);
        }

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
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pValue);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetIGMPSnoopingEnable
    (
        ANSC_HANDLE                 hContext,
        ULONG                     value
    )
{
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(value);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetWebUITimeout
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *pValue
    )
{
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
#if !defined (_BWG_PRODUCT_REQ_)
#if defined (CONFIG_TI_BBU) || defined (CONFIG_TI_BBU_TI)

    if (mta_hal_BatteryGetPowerSavingModeStatus(pValue) != RETURN_OK )
        return ANSC_STATUS_FAILURE;
    else
        return ANSC_STATUS_SUCCESS;

#endif
#endif

    UNREFERENCED_PARAMETER(pValue);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetWebUITimeout
    (
        ANSC_HANDLE                 hContext,
        ULONG                       value
    )
{
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
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
#if defined (_XB7_PRODUCT_REQ_) && defined (_COSA_BCM_ARM_)
        g_SetParamValueBool(brpdm, bEnable);
        if (brmode[0] == '0') {
            vsystem("/bin/sh /etc/webgui.sh &");
        }
#elif (!defined _XF3_PRODUCT_REQ_)
        g_SetParamValueBool(brpdm, bEnable);
	 /* RDKB-32831 Trigger only router mode from here, 
	  *  webgui.sh will be triggeed from Gwprovapp after bridge is started.
	  */
        if (brmode[0] == '0')
	{
           vsystem("/bin/sh /etc/webgui.sh &");
	}
#elif defined( _XF3_PRODUCT_REQ_)
        g_SetParamValueBool(brpdm, (bEnable>0?true:false));
#endif

        if (ppComponents == NULL && initWifiComp()) {
            syslog_systemlog("Local Network", LOG_NOTICE, "Bridge mode transition: Failed to acquire wifi component.");
            AnscFreeMemory( pnotifypara ); /*RDKB-6845, CID-33015, free unused resource before return */
            return;
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
    configBridgeMode(!(bFlag));
    return ANSC_STATUS_SUCCESS;
}

BOOL is_mesh_enabled()
{
    char buf[10] = {0};

    if(!syscfg_get(NULL, "mesh_enable", buf, sizeof(buf)))
    {
        if ((strcmp(buf,"true") == 0))
        {
            return TRUE;
        }
    }
    return FALSE;
}

void* set_mesh_disabled(void* arg)
{
    UNREFERENCED_PARAMETER(arg);
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    parameterValStruct_t   param_val[1];
    char  component[256]  = "eRT.com.cisco.spvtg.ccsp.meshagent";
    char  bus[256]        = "/com/cisco/spvtg/ccsp/meshagent";
    char* faultParam      = NULL;
    int   ret             = 0;

    param_val[0].parameterName="Device.DeviceInfo.X_RDKCENTRAL-COM_xOpsDeviceMgmt.Mesh.Enable";
    param_val[0].parameterValue="false";
    param_val[0].type = ccsp_boolean;

    ret = CcspBaseIf_setParameterValues(
            bus_handle,
            component,
            bus,
            0,
            0,
            (void*)&param_val,
            1,
            TRUE,
            &faultParam
            );

    if( ( ret != CCSP_SUCCESS ) && ( faultParam!=NULL )) {
        CcspTraceError(("%s-%d Failed to set Mesh Enable to false\n",__FUNCTION__,__LINE__));
        bus_info->freefunc( faultParam );
        return NULL;
    }
    return NULL;

}

void CosaDmlDcSaveWiFiHealthStatusintoNVRAM( void  )
{
	char acBoxType[ 16 ] = { 0 };

	// Get BOX TYPE from device properties
	if( 0 == CheckAndGetDevicePropertiesEntry( acBoxType, sizeof( acBoxType ),"BOX_TYPE" ) )
	{
		CcspTraceInfo(("%s - Box Type is %s \n",__FUNCTION__, acBoxType));

		// If it is XB3 then we need to do RPC client operation to do further
		// If it is non-XB3 then we need to do operation here itself
		if( ( acBoxType[ 0 ] != '\0' ) && \
			( 0 == strcmp( acBoxType, "XB3" ) )
		  )
		{
			char acAtomArpingIP[ 64 ] = { 0 };
			
			if( 0 == CheckAndGetDevicePropertiesEntry( acAtomArpingIP, sizeof( acAtomArpingIP ),"ATOM_ARPING_IP" ) )
			{
				if ( acAtomArpingIP[ 0 ] != '\0' )
				{
					CcspTraceInfo(("%s Reported an ATOM IP of %s \n", __FUNCTION__, acAtomArpingIP));
					
					pid_t pid = fork( );
				
					if ( pid == -1 )
					{
						// error, failed to fork()
					}
					else if ( pid > 0 )
					{
						int status;
						waitpid( pid, &status, 0 ); // wait here until the child completes
					}
					else
					{
						// we are the child
						char *args[ ] = {"/usr/bin/rpcclient", acAtomArpingIP, "/bin/sh /usr/ccsp/wifi/wifivAPPercentage.sh", (char *) 0 };

						CcspTraceInfo(("%s - Taking Backup of wifivAPPercentage\n",__FUNCTION__));
						
						execv( args[ 0 ], args );
						_exit(EXIT_FAILURE);   // exec never returns
					}
				}
			}
		}
		else
		{
			CcspTraceInfo(("%s - Taking Backup of wifivAPPercentage\n",__FUNCTION__));
			v_secure_system( "sh /usr/ccsp/wifi/wifivAPPercentage.sh" );
		}
	}
}

/* CheckAndGetDevicePropertiesEntry() */
int CheckAndGetDevicePropertiesEntry( char *pOutput, int size, char *sDevicePropContent )
{
    FILE 	*fp1 		 = NULL;
    char 	 buf[ 1024 ] = { 0 },
	  		*urlPtr 	 = NULL;
    int 	 ret		 = -1;

    // Read the device.properties file 
    fp1 = fopen( "/etc/device.properties", "r" );
	
    if ( NULL == fp1 ) 
	{
        CcspTraceError(("Error opening properties file! \n"));
        return -1;
    }

    while ( fgets( buf, sizeof( buf ), fp1 ) != NULL ) 
	{
        // Look for Device Properties Passed Content
        if ( strstr( buf, sDevicePropContent ) != NULL ) 
		{
            buf[strcspn( buf, "\r\n" )] = 0; // Strip off any carriage returns

            // grab content from string(entry)
            urlPtr = strstr( buf, "=" );
            urlPtr++;
			
            strncpy( pOutput, urlPtr, size );
			
          ret=0;
		  
          break;
        }
    }

    fclose( fp1 );
    return ret;
}

BOOL IsPortInUse(unsigned int port)
{
    char path[sizeof("/proc/net/tcp6")] = {0};
    FILE *f = NULL;
    const char *fmt = "%*d: %64[0-9A-Fa-f]:%x %*x:%*x %*x "
                      "%*x:%*x %*x:%*x %*x %*d %*d %llu";
    char line[256], addr[68];
    char *proto[2] = {"tcp","tcp6"};
    unsigned int tmpPort;
    long long uint64Inode;
    int r,i;
    errno_t safec_rc = -1;
    WebServConf_t conf;

    if (LoadWebServConf(&conf) == 0)
    {
        if ((port == conf.httpport) || (port == conf.httpsport))
            return  FALSE;
    }

    for (i=0; i<2; i++)
    {
        safec_rc = sprintf_s(path, sizeof(path), "/proc/net/%s",proto[i]);
        if(safec_rc < EOK)
        {
           ERR_CHK(safec_rc);
        }
        f = fopen (path,"r");
        if (f != NULL)
        {
            while (fgets(line, 256, f))
            {
                r = sscanf(line, fmt, addr, &tmpPort, &uint64Inode);
                if (r != 3)
                    continue;

                if (tmpPort == port)
                {
                    fclose(f);
                    return TRUE;
                }
            }
            fclose(f);
            f = NULL;
        }
    }

    return FALSE;
}
