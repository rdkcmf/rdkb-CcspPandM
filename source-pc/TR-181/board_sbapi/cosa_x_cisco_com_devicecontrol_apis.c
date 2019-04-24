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
#include "ccsp_hal_dhcpv4_emu_api.h" 
#include "dmsb_tr181_psm_definitions.h"

#define HTTPD_DEF_CONF  "/etc/lighttpd.conf"

#if 1//LNT_EMU
// for PSM access
static char *PSM_LanMode = "dmsb.X_CISCO_COM_DeviceControl.LanManagementEntry.%d.LanMode";
extern ANSC_HANDLE bus_handle;
extern char g_Subsystem[32];
// PSM access MACRO
#define _PSM_WRITE_PARAM(_PARAM_NAME) { \
        _ansc_sprintf(param_name, _PARAM_NAME, instancenum); \
        retPsmSet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, param_name, ccsp_string, param_value); \
        if (retPsmSet != CCSP_SUCCESS) { \
            AnscTraceFlow(("%s Error %d writing %s %s\n", __FUNCTION__, retPsmSet, param_name, param_value));\
        } \
        else \
        { \
            AnscTraceFlow(("%s: retPsmSet == CCSP_SUCCESS writing %s = %s \n", __FUNCTION__,param_name,param_value)); \
           printf("%s: retPsmSet == CCSP_SUCCESS writing %s = %s \n", __FUNCTION__,param_name,param_value); \
        } \
        _ansc_memset(param_name, 0, sizeof(param_name)); \
        _ansc_memset(param_value, 0, sizeof(param_value)); \
    }
#define _PSM_READ_PARAM(_PARAM_NAME) { \
        _ansc_memset(param_name, 0, sizeof(param_name)); \
        _ansc_sprintf(param_name, _PARAM_NAME, instancenum); \
        retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, param_name, NULL, &param_value); \
        if (retPsmGet != CCSP_SUCCESS) { \
            AnscTraceFlow(("%s Error %d reading %s %s\n", __FUNCTION__, retPsmGet, param_name, param_value));\
        } \
        else { \
            AnscTraceFlow(("%s: retPsmGet == CCSP_SUCCESS reading %s = \n%s\n", __FUNCTION__,param_name, param_value)); \
		printf("param_name (%s) and param_value (%s) \n",param_name, param_value); \
        } \
    }

#endif


int fwSync = 0;

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

#include "ansc_string_util.h"

#define _ERROR_ "NOT SUPPORTED"

/**
 * Dummy data
 */
struct DcWanDns {
    COSA_DML_WanAddrMode    AddrMode;
    ULONG       IPAddr;
    ULONG       SubnetMask;
    ULONG       Gateway;
    ULONG       SecIPAddr;
    BOOL                    SecIPRIPAdv;
    ULONG       BackupGateway;
};

static struct DcWanDns g_DcWanDns = {
    .AddrMode       = COSA_DML_WanAddrMode_Static,
};

static BOOLEAN         g_EnableZeroConfig = FALSE;

ANSC_STATUS
CosaDmlDcInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
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
    AnscCopyString(pValue, _ERROR_);
    *pulSize = AnscSizeOfString(pValue);
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
    AnscCopyString(pValue, _ERROR_);
    *pulSize = AnscSizeOfString(pValue);
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
	//RDKB-EMU
	if(CcspHalGetWanAddressMode())
		g_DcWanDns.AddrMode = COSA_DML_WanAddrMode_DHCP;
	else
		g_DcWanDns.AddrMode = COSA_DML_WanAddrMode_Static;

	*pMode = g_DcWanDns.AddrMode;
	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetWanStaticIPAddress
    (
        ANSC_HANDLE                 hContext,
        ULONG           *ipAddr
    )
{
    *ipAddr = g_DcWanDns.IPAddr;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetWanStaticSubnetMask
    (
        ANSC_HANDLE                 hContext,
        ULONG           *ipAddr
    )
{
    *ipAddr = g_DcWanDns.SubnetMask;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetWanStaticGatewayIP
    (
        ANSC_HANDLE                 hContext,
        ULONG           *ipAddr
    )
{
    *ipAddr = g_DcWanDns.Gateway;
    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlDcGetWanSecondIPAddr
    (
        ANSC_HANDLE                 hContext,
        ULONG           *ipAddr
    )
{
    *ipAddr = g_DcWanDns.SecIPAddr;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetWanSecondIPRipAdvertised
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    )
{
    *pFlag = g_DcWanDns.SecIPRIPAdv;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetWanBackupDefaultGateway
    (
        ANSC_HANDLE                 hContext,
        ULONG           *ipAddr
    )
{
    *ipAddr = g_DcWanDns.BackupGateway;
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
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetWanHostName
    (
        ANSC_HANDLE                 hContext,
        char                        *pHostName
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetWanDomainName
    (
        ANSC_HANDLE                 hContext,
        char                        *pDomainName
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetWanAddressMode
    (
        ANSC_HANDLE                 hContext,
        COSA_DML_WanAddrMode        mode
    )
{
    g_DcWanDns.AddrMode = mode;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetWanStaticIPAddress
    (
        ANSC_HANDLE                 hContext,
        ULONG           ipAddr
    )
{
    g_DcWanDns.IPAddr = ipAddr;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetWanStaticSubnetMask
    (
        ANSC_HANDLE                 hContext,
        ULONG           ipAddr
    )
{
    g_DcWanDns.SubnetMask = ipAddr;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetWanStaticGatewayIP
    (
        ANSC_HANDLE                 hContext,
        ULONG           ipAddr
    )
{
    g_DcWanDns.Gateway = ipAddr;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetWanSecondIPAddr
    (
        ANSC_HANDLE                 hContext,
        ULONG           ipAddr
    )
{
    g_DcWanDns.SecIPAddr = ipAddr;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetWanSecondIPRipAdvertised
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     Flag
    )
{
    g_DcWanDns.SecIPRIPAdv = Flag;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetWanBackupDefaultGateway
    (
        ANSC_HANDLE                 hContext,
        ULONG           ipAddr
    )
{
    g_DcWanDns.BackupGateway = ipAddr;
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
    _ansc_strcpy(pValue, "WAN, LAN,Both");
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetSNMPEnable
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlDcSetHostName
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetDomainName
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlDcGetRebootDevice
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
	if (strstr(pValue, "Router") != NULL) {
		AnscCopyString(pValue, "Router");
	}
	else if (strstr(pValue, "Wifi") != NULL) {
		AnscCopyString(pValue, "Wifi");
	}
	else if (strstr(pValue, "Router,Wifi") != NULL) {
		AnscCopyString(pValue, "Router,Wifi");
	}
	else {
		AnscCopyString(pValue, "All");
	}

	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetFactoryReset
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
	if (strstr(pValue, "Router") != NULL) {
		AnscCopyString(pValue, "Router");
	}
	else if (strstr(pValue, "Wifi") != NULL) {
		AnscCopyString(pValue, "Wifi");
	}
	else if (strstr(pValue, "Router,Wifi") != NULL) {
		AnscCopyString(pValue, "Router,Wifi");
	}
	else {
		AnscCopyString(pValue, "All");
	}

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
    AnscCopyString(pValue, "Dummy");
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetDeviceConfigIgnore
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    AnscCopyString(pValue, "Dummy");
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
{//RDKB_EMULATOR
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
                    "Device.WiFi.",
		    g_Subsystem,        /* prefix */
		    &ppComponents,
		    &size);

	if ( ret == CCSP_SUCCESS && size == 1)
	{
	    parameterValStruct_t val[2] = { { "Device.WiFi.Radio.1.X_CISCO_COM_ApplySetting", "true", ccsp_boolean}, 
				            { "Device.WiFi.Radio.5.X_CISCO_COM_ApplySetting", "true", ccsp_boolean} };
 
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
		    CcspTraceError(("RebootDevice:%s Failed to SetValue for param '%s'\n",__FUNCTION__,faultParam));
		    bus_info->freefunc(faultParam);
		} else {
			return 0;
			}
		}
	    free_componentStruct_t(bus_handle, size, ppComponents);
	
	return ANSC_STATUS_SUCCESS;
}
ANSC_STATUS
CosaDmlDcSetRebootDevice
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{//RDKB_EMULATOR
	int router, wifi, all;
	router = wifi = all = 0;

	if (strstr(pValue, "Router") != NULL) {
		router = 1;
	}
	if (strstr(pValue, "Wifi") != NULL) {
		wifi = 1;
	}
	if (router && wifi) {
		all = 1;
	}

	if ( all ) {
		pthread_t tid1;
		CcspTraceWarning(("RebootDevice:CosaDmlDcRebootWifi thread called to reboot WiFi\n"));
		pthread_create(&tid1, NULL, &CosaDmlDcRebootWifi, NULL);
		system("sleep 20");
		system("systemctl restart network-setup.service");
	}
	else  if (wifi) {
		pthread_t tid;
		CcspTraceWarning(("RebootDevice:CosaDmlDcRebootWifi thread called to reboot WiFi\n"));
		pthread_create(&tid, NULL, &CosaDmlDcRebootWifi, NULL);
	}

	else if (router) {
		fprintf(stderr, "Router is going to reboot\n");
		CcspTraceWarning(("RebootDevice:Router is going to reboot\n"));
		system("(sleep 5 && reboot) &");
	}

	return ANSC_STATUS_SUCCESS;
}

static int initWifiComp() {//RDKB_EMULATOR
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

ANSC_STATUS
CosaDmlDcSetFactoryReset
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{//RDKB_EMULATOR
	char* tok;
	char* sv;
	char value[50];
	int factory_reset_mask = 0;
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
			} else if(strcmp("VoIP", tok) == 0 || strcmp("Docsis", tok) == 0) {
				factory_reset_mask |= FR_OTHER;
			}

			tok = strtok_r(NULL, ",", &sv);
		}
	}
	if (!factory_reset_mask)
	{
		CcspTraceError(("FactoryReset:%s BAD parameter passed to factory defaults parameter ...\n",__FUNCTION__));
		return ANSC_STATUS_BAD_PARAMETER;
	}
	if (factory_reset_mask & FR_OTHER ) {
	}
	if(factory_reset_mask & FR_WIFI) {
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

	}

	if (factory_reset_mask & FR_ROUTER) {
		/*RESTORE DEFAULT LAN SETTINGS*/
		system("/lib/rdk/restore-bridgemode-to-routermode.sh");
		system("/lib/rdk/restore-lan-conf.sh");
		system("sleep 10");
		system("/lib/rdk/restore-rules.sh");
		system("sleep 5");
		system("iptables -t nat -F && iptables -t mangle -F && iptables -F");
		system("(sleep 5 && reboot) &");

	}
	else if (factory_reset_mask & FR_NONE){

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
    /*fileHandle = fopen(CONFIG_AVAHI_DAEMON_FILENAME, "r" );*/
    fileHandle = fopen("/tmp/avahi-daemon.conf", "r" );
    
    if ( !fileHandle )
    {
        /*fileHandle = fopen(CONFIG_AVAHI_DAEMON_FILENAME, "w+" );*/
        fileHandle = fopen("/tmp/avahi-daemon.conf", "w+" );

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
    AnscTraceWarning(("CosaDmlDcSetEnableZeroConfig -- bFlag:%d.\n", bFlag));

    if ( g_EnableZeroConfig != bFlag )
    {
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
CosaDmlDcGetEnableStaticNameServer
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                    *pFlag
    )
{
    *pFlag = FALSE;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetEnableStaticNameServer
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bFlag
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetReleaseWan
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                    *pFlag
    )
{
    *pFlag = FALSE;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetReleaseWan
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bFlag
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetRenewWan
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                    *pFlag
    )
{
    *pFlag = FALSE;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetRenewWan
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bFlag
    )
{
    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlDcGetDeviceMode
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *pValue
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS CosaDmlDcSetDeviceMode
    (
        ANSC_HANDLE                 hContext,
        ULONG                       value
    )
{
    return  ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlDcGetIGMPProxyEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    )
{
    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlDcSetIGMPProxyEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     pFlag
    )
{
    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlDcGetDNSProxyEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    )
{
    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlDcSetDNSProxyEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     pFlag
    )
{
    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlDcGetTelnetEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    )
{
    *pFlag = TRUE;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetSSHEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    )
{
    *pFlag = TRUE;
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
        BOOLEAN                     pFlag
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetSSHEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     pFlag
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetHNAPEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     pFlag
    )
{
    return ANSC_STATUS_SUCCESS;
}

struct DevCtrl {
    BOOL        httpEnable;
    ULONG       httpPort;
    BOOL        httpsEnable;
    ULONG       httpsPort;
    BOOL        IGMPSnoopingEnable;
    ULONG       WebUITimeout;
};

static struct DevCtrl g_DevCtrl = {
    .httpEnable     = TRUE,
    .httpPort       = 80,
    .httpsEnable    = TRUE,
    .httpsPort      = 443,
    .IGMPSnoopingEnable    = FALSE,
    .WebUITimeout   = 900,
};

ANSC_STATUS
CosaDmlDcGetHTTPEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pValue
    )
{
    *pValue = g_DevCtrl.httpEnable;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetHTTPSEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pValue
    )
{
    *pValue = g_DevCtrl.httpsEnable;
    return ANSC_STATUS_SUCCESS;
}

void _get_shell_output(char * cmd, char * out, int len)
{
    FILE * fp;
    char   buf[256];
    char * p;

    fp = popen(cmd, "r");

    if (fp)
    {
        fgets(buf, sizeof(buf), fp);

        /*we need to remove the \n char in buf*/
        if ((p = strchr(buf, '\n'))) *p = 0;

        strncpy(out, buf, len-1);

        pclose(fp);
    }

}
ANSC_STATUS
CosaDmlDcGetHTTPPort
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *pValue
    )
{
	char out[128] = {0};
	char cmd[100];
	memset(out,0,sizeof(out));
        memset(cmd,0,sizeof(cmd));
        sprintf(cmd, "syscfg get mgmt_wan_httpport");
        _get_shell_output(cmd, out, sizeof(out));

    *pValue = atoi(out);
    return ANSC_STATUS_SUCCESS; 

}

ANSC_STATUS
CosaDmlDcGetHTTPSPort
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *pValue
    )
{
    *pValue = g_DevCtrl.httpsPort;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetIGMPSnoopingEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pValue
    )
{
    *pValue = g_DevCtrl.IGMPSnoopingEnable;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetWebUITimeout
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *pValue
    )
{
    *pValue = g_DevCtrl.WebUITimeout;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetPowerSavingModeStatus
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *pValue
    )
{
    *pValue = 2;
    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlDcSetHTTPEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     value
    )
{
    g_DevCtrl.httpEnable = value;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetHTTPSEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     value
    )
{
    g_DevCtrl.httpsEnable = value;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetHTTPPort
    (
        ANSC_HANDLE                 hContext,
        ULONG                     value
    )
{
    g_DevCtrl.httpPort = value;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetHTTPSPort
    (
        ANSC_HANDLE                 hContext,
        ULONG                     value
    )
{
    g_DevCtrl.httpsPort = value;
    return ANSC_STATUS_SUCCESS;
}

static int
WebServRestart(ULONG Port)
{
	char buf[128];
	snprintf(buf, sizeof(buf), "sed -i 's/.*SERVER\\[\"socket\"\\] == \":.*/\\$SERVER\\[\"socket\"\\] == \":%d\"  { }/1' %s",Port,HTTPD_DEF_CONF);
        system(buf);

        CcspTraceInfo(("%s vsystem %d \n", __FUNCTION__,__LINE__));
        if (vsystem("systemctl restart lighttpd") != 0) {
            fprintf(stderr, "%s: fail to restart lighttpd\n", __FUNCTION__);
            return -1;
        } 

    return 0;
}
typedef struct WebServConf {
    ULONG       httpport;
    ULONG       httpsport;
} WebServConf_t;

ANSC_STATUS
CosaDmlDcSetWebServer(BOOL httpEn, BOOL httpsEn, ULONG httpPort, ULONG httpsPort)
{
	WebServConf_t conf;

    /* do not support disable HTTP/HTTPS */
        conf.httpport = httpPort;
        conf.httpsport = httpsPort;

	char cmd[50];
	const char *sysCfghttpPort = "mgmt_wan_httpport";
	snprintf(cmd, sizeof(cmd), "syscfg set mgmt_wan_httpport %ld",httpPort);
	system(cmd);
	system("syscfg commit");
	WebServRestart(httpPort);

    return ANSC_STATUS_SUCCESS;

}
ANSC_STATUS
CosaDmlDcSetIGMPSnoopingEnable
    (
        ANSC_HANDLE                 hContext,
        ULONG                     value
    )
{
    g_DevCtrl.IGMPSnoopingEnable = value;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetWebUITimeout
    (
        ANSC_HANDLE                 hContext,
        ULONG                     value
    )
{
    g_DevCtrl.WebUITimeout = value;
    return ANSC_STATUS_SUCCESS;
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
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetWebAccessLevel
    (
        ANSC_HANDLE                 hContext,
        int                         userIndex,
        int                         ifIndex,
        ULONG                     value
    )
{
    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlDcGetMsoRemoteMgmtEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    )
{
    *pFlag = TRUE;
    return  ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlDcSetMsoRemoteMgmtEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     pFlag
    )
{
    return  ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlDcGetCusadminRemoteMgmtEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    )
{
    *pFlag = TRUE;
    return  ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlDcSetCusadminRemoteMgmtEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     pFlag
    )
{
    return  ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlDcGetGuestPassword
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    return  ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlDcSetGuestPassword
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    return  ANSC_STATUS_SUCCESS;
}


ULONG
CosaDmlDcGetNoOfGuests
    (
        void    
    )
{
    return  0;
}


ANSC_STATUS
CosaDmlDcSetNoOfGuests
    (
        ULONG                      uVal 
    )
{
    return  ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetParConPassword
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetParConPassword
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetParConQuestion
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetParConQuestion
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetParConAnswer
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetParConAnswer
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    return  ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetDefaultParConPassword
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    return  ANSC_STATUS_SUCCESS;
}

/*
 *  DH  copied from ARM code -- this implementation method is not a good practice
 */
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
    return 2; 
}

/*
 * Device.X_CISCO_COM_DeviceControl.LanManagementEntry.{i}.
 */
#define MAX_LANMNGM_ENTRY       16
static int                      g_lanMngmCnt = 1;
static COSA_DML_LAN_MANAGEMENT  g_lanMngmTab[MAX_LANMNGM_ENTRY] = {
    {
        .InstanceNumber         = 1,
        .Alias                  = "cpe-LanManagementEntry-1",
        .LanMode                = COSA_DML_LanMode_Router,
        .LanNetwork.Dot         = {192, 168, 0, 100},
        .LanNetworksAllow       = COSA_DML_LanNetworksAllow_Default,
        .LanSubnetMask.Dot      = {255, 255, 255, 0},
        .LanIPAddress.Dot         = {192, 168, 0, 1},
        .LanDhcpServer          = TRUE,
        .LanNaptEnable          = TRUE,
        .LanNaptType            = COSA_DML_LanNapt_StaticIP,
        .LanTos                 = 64,
        .LanDhcp125             = FALSE,
        .LanHnap                = FALSE,
        .LanUpnp                = FALSE,
    }
};

int PSMGetDhcpv4RecordValues(PCOSA_DML_LAN_MANAGEMENT pLanMngt,ULONG instancenum)//LNT_EMU
{
        int retPsmGet = CCSP_SUCCESS;
        char param_name[256] = {0};
        char *param_value = NULL;
	instancenum = instancenum - 1;
        _PSM_READ_PARAM(PSM_DHCPV4_SERVER_POOL_IPROUTERS);
                pLanMngt->LanIPAddress.Value = inet_addr(param_value);
        _PSM_READ_PARAM(PSM_DHCPV4_SERVER_POOL_SUBNETMASK);
                pLanMngt->LanSubnetMask.Value = inet_addr(param_value);
        return 0;
}

int PSMSettDHCPV4RecordValues(PCOSA_DML_LAN_MANAGEMENT pNewCfg,ULONG instancenum)//LNT_EMU
{
        int retPsmSet = CCSP_SUCCESS;
        char param_name[256] = {0};
        char param_value[256] = {0};
        _ansc_sprintf(param_value, "%d.%d.%d.%d",
            pNewCfg->LanIPAddress.Dot[0], pNewCfg->LanIPAddress.Dot[1], pNewCfg->LanIPAddress.Dot[2], pNewCfg->LanIPAddress.Dot[3]);
         _PSM_WRITE_PARAM(PSM_DHCPV4_SERVER_POOL_IPROUTERS);
        _ansc_sprintf(param_value, "%d.%d.%d.%d",
            pNewCfg->LanSubnetMask.Dot[0], pNewCfg->LanSubnetMask.Dot[1],pNewCfg->LanSubnetMask.Dot[2],pNewCfg->LanSubnetMask.Dot[3]);
        _PSM_WRITE_PARAM(PSM_DHCPV4_SERVER_POOL_SUBNETMASK);
        return 0;
}

static int
CosaDmlLanMngm_InsGetIndex(ULONG ins)
{
    int i;

    for (i = 0; i < g_lanMngmCnt; i++)
        if (g_lanMngmTab[i].InstanceNumber == ins)
            return i;

    return -1;
}

ULONG
CosaDmlLanMngm_GetNumberOfEntries(void)
{
    return g_lanMngmCnt;
}

ANSC_STATUS
CosaDmlLanMngm_GetEntryByIndex(ULONG index, PCOSA_DML_LAN_MANAGEMENT pLanMngm)
{
	uint32_t ip_integer;
	uint32_t  netmask;
    	char lan_ip[256] = {0};
    	char lan_subnet[256] = {0};
	char *param_value = NULL;
        char param_name[256] = {0};
	PCOSA_DML_LAN_MANAGEMENT pLanMngt = NULL;//RDKB-EMULATOR
        pLanMngt = (PCOSA_DML_LAN_MANAGEMENT) AnscAllocateMemory( sizeof(COSA_DML_LAN_MANAGEMENT));
	if (index >= g_lanMngmCnt)
		return ANSC_STATUS_FAILURE;

	*pLanMngm = g_lanMngmTab[index];
	netmask=CosaUtilIoctlXXX("brlan0","netmask",NULL);
	ip_integer=CosaUtilGetIfAddr("brlan0");

	*(uint32_t*)pLanMngm->LanIPAddress.Dot = ip_integer;
	*(uint32_t*)pLanMngm->LanSubnetMask.Dot = netmask;
	
	_ansc_sprintf(lan_ip, "%d.%d.%d.%d",
            pLanMngm->LanIPAddress.Dot[0], pLanMngm->LanIPAddress.Dot[1], pLanMngm->LanIPAddress.Dot[2], pLanMngm->LanIPAddress.Dot[3]);
	_ansc_sprintf(lan_subnet, "%d.%d.%d.%d",
            pLanMngm->LanSubnetMask.Dot[0], pLanMngm->LanSubnetMask.Dot[1],pLanMngm->LanSubnetMask.Dot[2], pLanMngm->LanSubnetMask.Dot[3]);
	if(strcmp(lan_ip,"0.0.0.0") == 0)
        {
        PSMGetDhcpv4RecordValues(pLanMngt,pLanMngm->InstanceNumber);
        pLanMngm->LanIPAddress.Dot[0] = pLanMngt->LanIPAddress.Dot[0];
        pLanMngm->LanIPAddress.Dot[1] = pLanMngt->LanIPAddress.Dot[1];
        pLanMngm->LanIPAddress.Dot[2] = pLanMngt->LanIPAddress.Dot[2];
        pLanMngm->LanIPAddress.Dot[3] = pLanMngt->LanIPAddress.Dot[3];
        }
        if(strcmp(lan_subnet,"0.0.0.0") == 0)
        {
        PSMGetDhcpv4RecordValues(pLanMngt,pLanMngm->InstanceNumber);
        pLanMngm->LanSubnetMask.Dot[0] = pLanMngt->LanSubnetMask.Dot[0];
        pLanMngm->LanSubnetMask.Dot[1] = pLanMngt->LanSubnetMask.Dot[1];
        pLanMngm->LanSubnetMask.Dot[2] = pLanMngt->LanSubnetMask.Dot[2];
        pLanMngm->LanSubnetMask.Dot[3] = pLanMngt->LanSubnetMask.Dot[3];
        }
//PSM_ACCESS
        memset(param_name, 0, sizeof(param_name));//RDKB-EMULATOR
        sprintf(param_name, PSM_LanMode, pLanMngm->InstanceNumber);
        PSM_Get_Record_Value2(bus_handle,g_Subsystem, param_name, NULL, &param_value);
        if(strcmp(param_value,"router") == 0)
                pLanMngm->LanMode = COSA_DML_LanMode_Router;
        else
                pLanMngm->LanMode = COSA_DML_LanMode_BridgeStatic;

	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlLanMngm_SetValues(ULONG index, ULONG ins, const char *alias)
{
    if (index >= g_lanMngmCnt)
        return ANSC_STATUS_FAILURE;

    g_lanMngmTab[index].InstanceNumber = ins;
    snprintf(g_lanMngmTab[index].Alias, sizeof(g_lanMngmTab[index].Alias),
            "%s", alias);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlLanMngm_AddEntry(PCOSA_DML_LAN_MANAGEMENT pLanMngm)
{
    if (g_lanMngmCnt >= MAX_LANMNGM_ENTRY)
        return ANSC_STATUS_FAILURE;

    g_lanMngmTab[g_lanMngmCnt++] = *pLanMngm;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlLanMngm_DelEntry(ULONG ins)
{
    int i;

    if ((i = CosaDmlLanMngm_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;

    memmove(&g_lanMngmTab[i], &g_lanMngmTab[i+1], 
            (g_lanMngmCnt - i - 1) * sizeof(COSA_DML_LAN_MANAGEMENT));
    g_lanMngmCnt--;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlLanMngm_GetConf(ULONG ins, PCOSA_DML_LAN_MANAGEMENT pLanMngm)
{
    int i;

    if ((i = CosaDmlLanMngm_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;

    *pLanMngm = g_lanMngmTab[i];
    pLanMngm->InstanceNumber = ins; /* just in case */
//RDKB-EMU
        uint32_t ip_integer;
        uint32_t  netmask;
        netmask=CosaUtilIoctlXXX("brlan0","netmask",NULL);
        ip_integer=CosaUtilGetIfAddr("brlan0");

        *(uint32_t*)pLanMngm->LanIPAddress.Dot = ip_integer;
        *(uint32_t*)pLanMngm->LanSubnetMask.Dot = netmask;

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlLanMngm_SetConf(ULONG ins, PCOSA_DML_LAN_MANAGEMENT pLanMngm)
{
	int i;
	char str[INET_ADDRSTRLEN];
	char str1[INET_ADDRSTRLEN];
        char recName[256] = {0};
        char string[100] ={0};
#if 1//RDKB-EMULATOR
        PCOSA_DML_LAN_MANAGEMENT pSLanCfg = NULL;
        pSLanCfg = (PCOSA_DML_LAN_MANAGEMENT)AnscAllocateMemory( sizeof(COSA_DML_LAN_MANAGEMENT));
        pSLanCfg->LanIPAddress.Dot[0] = pLanMngm->LanIPAddress.Dot[0];
        pSLanCfg->LanIPAddress.Dot[1] = pLanMngm->LanIPAddress.Dot[1];
        pSLanCfg->LanIPAddress.Dot[2] = pLanMngm->LanIPAddress.Dot[2];
        pSLanCfg->LanIPAddress.Dot[3] = pLanMngm->LanIPAddress.Dot[3];
        pSLanCfg->LanSubnetMask.Dot[0] = pLanMngm->LanSubnetMask.Dot[0];
        pSLanCfg->LanSubnetMask.Dot[1] = pLanMngm->LanSubnetMask.Dot[1];
        pSLanCfg->LanSubnetMask.Dot[2] = pLanMngm->LanSubnetMask.Dot[2];
        pSLanCfg->LanSubnetMask.Dot[3] = pLanMngm->LanSubnetMask.Dot[3];
        PSMSettDHCPV4RecordValues(pSLanCfg,pSLanCfg->InstanceNumber);
#endif

	inet_ntop(AF_INET, pLanMngm->LanIPAddress.Dot, str, INET_ADDRSTRLEN);
	inet_ntop(AF_INET, pLanMngm->LanSubnetMask.Dot, str1, INET_ADDRSTRLEN);
	/*ConfigValues config_values;
	config_values.gateway = str;
	config_values.subnet = str1;*/
	if ((i = CosaDmlLanMngm_InsGetIndex(ins)) == -1)
		return ANSC_STATUS_FAILURE;
/*	if(CcspHalSetDHCPConfigValues(GATEWAY, &config_values)==-1)//LNT_EMU
		printf("SetDHCPConfigValues failed\n");
	if(CcspHalSetDHCPConfigValues(GATEWAY|SUBNET_MASK, &config_values)==-1)//LNT_EMU
		printf("SetDHCPConfigValues failed\n");*/
	CcspHalSetDHCPConfigValues(SUBNET_MASK,str1);
	CcspHalInterfacesetval("brlan0",str);//RDKB-EMULATOR
	CcspHalNetmasksetvalue("brlan0",str1);//LNT_EMU
	CcspHalUpdateInterfaceval(str);//LNT_EMU

//PSM-ACCESS - To Store BridgeMode values in PSM DataBase.
        memset(recName, 0, sizeof(recName));
        sprintf(recName, PSM_LanMode, pLanMngm->InstanceNumber);
        if(pLanMngm->LanMode == COSA_DML_LanMode_BridgeStatic)
                strcpy(string,"bridge-static");
        else if(pLanMngm->LanMode == COSA_DML_LanMode_Router)
                strcpy(string,"router");
        PSM_Set_Record_Value2(bus_handle,g_Subsystem, recName, ccsp_string, string);

	if(pLanMngm->LanMode == COSA_DML_LanMode_BridgeStatic)
                system("killall dnsmasq");

	g_lanMngmTab[i] = *pLanMngm;
	g_lanMngmTab[i].InstanceNumber = ins; /* just in case */

	return ANSC_STATUS_SUCCESS;
}

BOOL is_mesh_enabled()
{
    int ret = ANSC_STATUS_FAILURE;
    parameterValStruct_t    **valStructs = NULL;
    char dstComponent[64]="eRT.com.cisco.spvtg.ccsp.meshagent";
    char dstPath[64]="/com/cisco/spvtg/ccsp/meshagent";
    char *paramNames[]={"Device.DeviceInfo.X_RDKCENTRAL-COM_xOpsDeviceMgmt.Mesh.Enable"};
    int  valNum = 0;

    ret = CcspBaseIf_getParameterValues(
            bus_handle,
            dstComponent,
            dstPath,
            paramNames,
            1,
            &valNum,
            &valStructs);

    if(CCSP_Message_Bus_OK != ret)
    {
         CcspTraceError(("%s CcspBaseIf_getParameterValues %s error %d\n", __FUNCTION__,paramNames[0],ret));
         free_parameterValStruct_t(bus_handle, valNum, valStructs);
         return FALSE;
    }

    CcspTraceWarning(("valStructs[0]->parameterValue = %s\n",valStructs[0]->parameterValue));

    if(strncmp("true", valStructs[0]->parameterValue,4)==0)
    {
         free_parameterValStruct_t(bus_handle, valNum, valStructs);
         return TRUE;
    }
    else
    {
         free_parameterValStruct_t(bus_handle, valNum, valStructs);
         return FALSE;
    }
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
    WebServConf_t conf;

        if ((port == conf.httpport) || (port == conf.httpsport))
            return  FALSE;

    for (i=0; i<2; i++)
    {
        sprintf(path,"/proc/net/%s",proto[i]);

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

