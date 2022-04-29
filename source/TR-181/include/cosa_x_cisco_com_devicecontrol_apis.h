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

    module: cosa_x_cisco_com_devicecontrol_apis.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        07/15/2011    initial revision.

**************************************************************************/


#ifndef  _COSA_X_CISCO_COM_DEVICECONTROL_APIS_H
#define  _COSA_X_CISCO_COM_DEVICECONTROL_APIS_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"

#include <sys/sysinfo.h>


#define CONFIG_AVAHI_DAEMON_FILENAME    "/var/tmp/avahi/avahi-daemon.conf"

#define CMD_START_AVAHI_DAEMON          "avahi-daemon -D"
#define CMD_START_AVAHI_AUTOIPD         "avahi-autoipd -D --force brlan0"
#define CMD_STOP_AVAHI_DAEMON           "avahi-daemon -k"
#define CMD_STOP_AVAHI_AUTOIPD          "avahi-autoipd -k brlan0"

#define UTOPIA_AVAHI_ENABLED            "ccsp_zeroconf_enabled"



/**********************************************************************
                STRUCTURE AND CONSTANT DEFINITIONS
**********************************************************************/
enum
{
    PRIMARY_MODE=1,
    BYOI_MODE,
    NONE_MODE
};

typedef enum
_COSA_DML_WanAddrMode
{
    COSA_DML_WanAddrMode_DHCP       = 1,
    COSA_DML_WanAddrMode_Static,
    COSA_DML_WanAddrMode_DHALIP,
}
COSA_DML_WanAddrMode, *PCOSA_DML_WanAddrMode;


typedef enum
_COSA_DML_LanMode
{
    COSA_DML_LanMode_BridgeDHCP = 1,
    COSA_DML_LanMode_BridgeStatic = 2,
    COSA_DML_LanMode_Router = 3,
	COSA_DML_LanMode_FullBridgeStatic = 4
}
COSA_DML_LanMode, *PCOSA_DML_LanMode;

typedef enum
_COSA_DML_LanNetworksAllow
{
    COSA_DML_LanNetworksAllow_Default = 0,
    COSA_DML_LanNetworksAllow_AnyPrivateClass,
    COSA_DML_LanNetworksAllow_AnyClass,
}
COSA_DML_LanNetworksAllow, *PCOSA_DML_LanNetworksAllow;

typedef enum
_COSA_DML_LanNapt
{
//    COSA_DML_LanNapt_Disable = 0,
    COSA_DML_LanNapt_DHCP =1,
    COSA_DML_LanNapt_StaticIP,
}COSA_DML_LanNapt, *PCOSA_DML_LanNapt;

typedef struct 
_COSA_DML_LAN_MANAGEMENT
{
    ULONG                       InstanceNumber;
    char                        Alias[COSA_DML_IF_NAME_LENGTH];

    COSA_DML_LanMode            LanMode;
    ANSC_IPV4_ADDRESS           LanNetwork;
    COSA_DML_LanNetworksAllow   LanNetworksAllow;
    ANSC_IPV4_ADDRESS           LanSubnetMask;
    ANSC_IPV4_ADDRESS           LanIPAddress;
    BOOLEAN                     LanDhcpServer;
    COSA_DML_LanNapt            LanNaptType;
    BOOLEAN                     LanNaptEnable;
    ULONG                       LanTos;
    BOOLEAN                     LanDhcp125;
    BOOLEAN                     LanHnap;
    BOOLEAN                     LanUpnp;
}
COSA_DML_LAN_MANAGEMENT, *PCOSA_DML_LAN_MANAGEMENT;

typedef struct
_COSA_NOTIFY_WIFI
{
    int flag;
    int ticket;
}
COSA_NOTIFY_WIFI, *PCOSA_NOTIFY_WIFI;

#define FACTORY_RESET_KEY "factory_reset"
#define FACTORY_RESET_WIFI_VALUE "w"
#define FACTORY_RESET_ROUTER_VALUE "y"

/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/

ANSC_STATUS
CosaDmlDcInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

ANSC_STATUS
CosaDmlDcGetMultiHomedHSDFlag
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

ANSC_STATUS
CosaDmlDcGetMultiHomedUIPageControl
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

ANSC_STATUS
CosaDmlDcGetMultiHomedMode
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

ANSC_STATUS
CosaDmlDcGetMultiHomedBridgingStatus
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

ANSC_STATUS
CosaDmlDcSetMultiHomedHSDFlag
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulValue
    );

ANSC_STATUS
CosaDmlDcSetMultiHomedUIPageControl
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulValue
    );

ANSC_STATUS
CosaDmlDcSetMultiHomedMode
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulValue
    );

ANSC_STATUS
CosaDmlDcGetWanAddressMode
    (
        ANSC_HANDLE                 hContext,
        COSA_DML_WanAddrMode        *pMode
    );

ANSC_STATUS
CosaDmlDcGetWanStaticIPAddress
    (
        ANSC_HANDLE                 hContext,
        /* XXX: Why not ANSC_IPV4_ADDRESS? it will build fail */
        ULONG                       *ipAddr
    );

ANSC_STATUS
CosaDmlDcGetWanStaticSubnetMask
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *ipAddr
    );

ANSC_STATUS
CosaDmlDcGetWanStaticGatewayIP
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *ipAddr
    );

ANSC_STATUS
CosaDmlDcGetWanSecondIPAddr
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *ipAddr
    );

ANSC_STATUS
CosaDmlDcGetWanSecondIPRipAdvertised
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    );

ANSC_STATUS
CosaDmlDcGetWanBackupDefaultGateway
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *ipAddr
    );

ANSC_STATUS
CosaDmlDcGetWanNameServer
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *ipAddr,
        int                         nameServerNo
    );

ANSC_STATUS
CosaDmlDcGetWanHostName
    (
        ANSC_HANDLE                 hContext,
        char                        *pHostName
    );

ANSC_STATUS
CosaDmlDcGetWanDomainName
    (
        ANSC_HANDLE                 hContext,
        char                        *pDomainName
    );

ANSC_STATUS
CosaDmlDcGetWanStaticDomainName
    (
        ANSC_HANDLE                 hContext,
        char                        *pStaticDomainName
    );

ANSC_STATUS
CosaDmlDcSetWanAddressMode
    (
        ANSC_HANDLE                 hContext,
        COSA_DML_WanAddrMode        mode
    );

ANSC_STATUS
CosaDmlDcSetWanStaticIPAddress
    (
        ANSC_HANDLE                 hContext,
        uint32_t                    ipAddr
    );

ANSC_STATUS
CosaDmlDcSetWanStaticSubnetMask
    (
        ANSC_HANDLE                 hContext,
        uint32_t                    ipAddr
    );

ANSC_STATUS
CosaDmlDcSetWanStaticGatewayIP
    (
        ANSC_HANDLE                 hContext,
        uint32_t                    ipAddr
    );

ANSC_STATUS
CosaDmlDcSetWanSecondIPAddr
    (
        ANSC_HANDLE                 hContext,
        uint32_t                    ipAddr
    );

ANSC_STATUS
CosaDmlDcSetWanSecondIPRipAdvertised
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     Flag
    );

ANSC_STATUS
CosaDmlDcSetWanBackupDefaultGateway
    (
        ANSC_HANDLE                 hContext,
        uint32_t                    ipAddr
    );

ANSC_STATUS
CosaDmlDcSetWanNameServer
    (
        ANSC_HANDLE                 hContext,
        uint32_t                    ipAddr,
        int                         nameServerNo
    );

ANSC_STATUS
CosaDmlDcGetEnableStaticNameServer
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    );

ANSC_STATUS
CosaDmlDcSetEnableStaticNameServer
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     pFlag
    );

ANSC_STATUS
CosaDmlDcGetReleaseWan
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    );

ANSC_STATUS
CosaDmlDcSetReleaseWan
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     pFlag
    );

ANSC_STATUS
CosaDmlDcGetRenewWan
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    );

ANSC_STATUS
CosaDmlDcSetRenewWan
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     pFlag
    );

ANSC_STATUS
CosaDmlDcGetResetDefaultEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    );

ANSC_STATUS
CosaDmlDcGetSNMPEnable
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

ANSC_STATUS
CosaDmlDcSetSNMPEnable
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

ANSC_STATUS
CosaDmlDcSetHostName
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

ANSC_STATUS
CosaDmlDcSetDomainName
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

ANSC_STATUS
CosaDmlDcGetRebootDevice
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

ANSC_STATUS
CosaDmlDcGetFactoryReset
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

ANSC_STATUS
CosaDmlDcGetUserChangedFlags
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

ANSC_STATUS 
CosaDmlDcSetReInitCmMac ();

ANSC_STATUS
CosaDmlDcGetDeviceConfigStatus
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

ANSC_STATUS
CosaDmlDcGetDeviceConfigIgnore
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

ANSC_STATUS
CosaDmlDcSetDeviceConfigIgnore
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

ANSC_STATUS
CosaDmlDcSetRebootDevice
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

ANSC_STATUS
CosaDmlDcGetFactoryReset
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

ANSC_STATUS
CosaDmlDcSetUserChangedFlags
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

ANSC_STATUS
CosaDmlDcSetResetDefaultEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     pFlag
    );

ANSC_STATUS
CosaDmlDcGetDeviceMode
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *pValue
    );

ANSC_STATUS
CosaDmlDcGetTelnetEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    );

ANSC_STATUS
CosaDmlDcGetSSHEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    );

ANSC_STATUS
CosaDmlDcGetHNAPEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    );

ANSC_STATUS
CosaDmlDcSetTelnetEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     pFlag
    );

ANSC_STATUS
CosaDmlDcSetSSHEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     pFlag
    );

ANSC_STATUS
CosaDmlDcSetHNAPEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     pFlag
    );


ANSC_STATUS
CosaDmlDcGetMsoRemoteMgmtEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    );

ANSC_STATUS
CosaDmlDcSetMsoRemoteMgmtEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     pFlag
    );

ANSC_STATUS
CosaDmlDcGetCusadminRemoteMgmtEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    );

ANSC_STATUS
CosaDmlDcSetCusadminRemoteMgmtEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     pFlag
    );
    
ANSC_STATUS
CosaDmlDcGetHSEthernetPortEnable
    (
        ANSC_HANDLE                hContext,
        BOOLEAN                    *pFlag
    );
    
ANSC_STATUS
CosaDmlDcSetHSEthernetPortEnable
    (
        ANSC_HANDLE                hContext,
        BOOLEAN                    pFlag
    );

ANSC_STATUS
CosaDmlDcGetGuestPassword
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

ANSC_STATUS
CosaDmlDcSetGuestPassword
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

ULONG
CosaDmlDcGetNoOfGuests
    (
    );

ANSC_STATUS
CosaDmlDcSetNoOfGuests
    (
        ULONG                      uVal 
    );

ULONG
CosaDmlLanMngm_GetNumberOfEntries
    (
        void
    );

ANSC_STATUS
CosaDmlLanMngm_GetEntryByIndex
    (
        ULONG index, 
        PCOSA_DML_LAN_MANAGEMENT pLanMngm
    );

ANSC_STATUS
CosaDmlLanMngm_SetValues
    (
        ULONG index, 
        ULONG ins, 
        const char *alias
    );

ANSC_STATUS
CosaDmlLanMngm_AddEntry
    (
        PCOSA_DML_LAN_MANAGEMENT pLanMngm
    );

ANSC_STATUS
CosaDmlLanMngm_DelEntry
    (
        ULONG ins
    );

ANSC_STATUS
CosaDmlLanMngm_GetConf
    (
        ULONG ins, 
        PCOSA_DML_LAN_MANAGEMENT pLanMngm
    );

ANSC_STATUS
CosaDmlLanMngm_SetConf
    (
        ULONG ins, 
        PCOSA_DML_LAN_MANAGEMENT pLanMngm
    );

ANSC_STATUS CosaDmlDcResetBr0(char *ip, char *sub);
	

void _CosaDmlDcStartZeroConfig();

void _CosaDmlDcStopZeroConfig();

ANSC_STATUS
CosaDmlDcGetErouterEnabled
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    );

ANSC_STATUS
CosaDmlDcSetErouterEnabled
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bFlag
    );

void* CosaDmlDcRebootWifi(ANSC_HANDLE   hContext);
void* CosaDmlDcRestartRouter(void* arg);
void CosaDmlDcSaveWiFiHealthStatusintoNVRAM( void  );
int CheckAndGetDevicePropertiesEntry( char *pOutput, int size, char *sDevicePropContent );
INT cm_hal_ReinitMac();
BOOL moca_HardwareEquipped(void);

ANSC_STATUS
CosaDmlDcGetHTTPPort
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *pValue
    );

ANSC_STATUS
CosaDmlDcGetHTTPSPort
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *pValue
    );

ANSC_STATUS
CosaDmlDcGetReinitMacThreshold
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *pValue
    );

ANSC_STATUS
CosaDmlDcGetIGMPProxyEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    );

ANSC_STATUS
CosaDmlDcGetDNSProxyEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pFlag
    );

ANSC_STATUS
CosaDmlDcGetEnableZeroConfig
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                    *pFlag
    );

ANSC_STATUS
CosaDmlDcGetHTTPEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pValue
    );

ANSC_STATUS
CosaDmlDcGetHTTPSEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pValue
    );
ANSC_STATUS
CosaDmlDcGetIGMPSnoopingEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     *pValue
    );

ANSC_STATUS
CosaDmlDcGetWebUITimeout
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *pValue
    );

ANSC_STATUS
CosaDmlDcGetPowerSavingModeStatus
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *pValue
    );

ULONG
CosaDmlGetMocaHardwareStatus
    (
        ANSC_HANDLE                 hContext
    );

ANSC_STATUS
CosaDmlDcGetParConPassword
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

ANSC_STATUS
CosaDmlDcGetDefaultParConPassword
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

ANSC_STATUS
CosaDmlDcGetParConQuestion
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

ANSC_STATUS
CosaDmlDcGetParConAnswer
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

ANSC_STATUS
CosaDmlDcSetIGMPProxyEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     pFlag
    );

ANSC_STATUS
CosaDmlDcSetDNSProxyEnable
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     pFlag
    );

ANSC_STATUS
CosaDmlDcSetEnableZeroConfig
    (
        ANSC_HANDLE                 hContext,
        BOOLEAN                     bFlag
    );

ANSC_STATUS
CosaDmlDcSetIGMPSnoopingEnable
    (
        ANSC_HANDLE                 hContext,
        ULONG                     value
    );

ANSC_STATUS CosaDmlDcSetDeviceMode
    (
        ANSC_HANDLE                 hContext,
        ULONG                       value
    );

ANSC_STATUS
CosaDmlDcSetParConPassword
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

ANSC_STATUS
CosaDmlDcSetParConQuestion
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

ANSC_STATUS
CosaDmlDcSetParConAnswer
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

ANSC_STATUS
CosaDmlDcSetFactoryReset
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

ANSC_STATUS
CosaDmlDcSetWebServer(BOOL httpEn, BOOL httpsEn, ULONG httpPort, ULONG httpsPort);


ANSC_STATUS
CosaDmlDcSetWebAccessLevel
    (
        ANSC_HANDLE                 hContext,
        int                         userIndex,
        int                         ifIndex,
        ULONG                       value
    );

ANSC_STATUS
CosaDmlDcGetWebAccessLevel
    (
        ANSC_HANDLE                 hContext,
        int                         userIndex,
        int                         ifIndex,
        ULONG                       *pValue
    );

BOOL 
IsPortInUse
(
    unsigned int port
);

ANSC_STATUS
CosaDmlDcSetReinitMacThreshold
    (
        ANSC_HANDLE                 hContext,
        ULONG                       value
    );

ANSC_STATUS
CosaDmlDcSetWebUITimeout
    (
        ANSC_HANDLE                 hContext,
        ULONG                       value
    );
       
#endif
