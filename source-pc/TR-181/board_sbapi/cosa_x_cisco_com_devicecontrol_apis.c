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
    AnscCopyString(pValue, "Router");
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcGetFactoryReset
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    AnscCopyString(pValue, "Router");
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

ANSC_STATUS
CosaDmlDcSetRebootDevice
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDcSetFactoryReset
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
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

ANSC_STATUS
CosaDmlDcGetHTTPPort
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *pValue
    )
{
    *pValue = g_DevCtrl.httpPort;
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

ANSC_STATUS
CosaDmlDcSetWebServer(BOOL httpEn, BOOL httpsEn, ULONG httpPort, ULONG httpsPort)
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
    if (index >= g_lanMngmCnt)
        return ANSC_STATUS_FAILURE;

    *pLanMngm = g_lanMngmTab[index];
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

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlLanMngm_SetConf(ULONG ins, PCOSA_DML_LAN_MANAGEMENT pLanMngm)
{
    int i;

    if ((i = CosaDmlLanMngm_InsGetIndex(ins)) == -1)
        return ANSC_STATUS_FAILURE;

    g_lanMngmTab[i] = *pLanMngm;
    g_lanMngmTab[i].InstanceNumber = ins; /* just in case */

    return ANSC_STATUS_SUCCESS;
}

