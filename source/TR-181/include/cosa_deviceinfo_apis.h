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

    module: cosa_deviceinfo_apis.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**************************************************************************/


#ifndef  _COSA_DEVICEINFO_APIS_H
#define  _COSA_DEVICEINFO_APIS_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "cJSON.h"
#ifdef FEATURE_SUPPORT_ONBOARD_LOGGING

#define LOGGING_MODULE           "PAM"
#define OnboardLog(...)          rdk_log_onboard(LOGGING_MODULE, __VA_ARGS__)
#else
#define OnboardLog(...)
#endif
#if defined(_ANSC_LINUX)
    #include <sys/sysinfo.h>
#endif

#include "cm_hal.h"

#define MAX_SECURITYNUM_LEN         128
#define MAX_SECURITYNAME_LEN        18      // includes room for NULL terminator

/**********************************************************************
                STRUCTURE AND CONSTANT DEFINITIONS
**********************************************************************/

enum
{
    CNS_FLASH_SerialNo = 1,
    CNS_FLASH_Description,
    CNS_FLASH_ModelNo,
    CNS_FLASH_NP_ID,
    CNS_FLASH_Version,
    CNS_FLASH_MFG_Date,
    CNS_FLASH_HW_Feature
};

enum
{
    COSA_DML_PROC_STATUS_Running = 1,
    COSA_DML_PROC_STATUS_Sleeping,
    COSA_DML_PROC_STATUS_Stopped,
    COSA_DML_PROC_STATUS_Idle,
    COSA_DML_PROC_STATUS_Uninterruptible,
    COSA_DML_PROC_STATUS_Zombie,
};

typedef  struct
_COSA_PROCESS_ENTRY
{
    ULONG                           Pid;
    char                            Command[256];
    ULONG                           Size;
    ULONG                           Priority;
    ULONG                           CPUTime;
    ULONG                           State;
}
COSA_PROCESS_ENTRY, *PCOSA_PROCESS_ENTRY;

#define  COSA_DATAMODEL_PROCESS_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    COSA_BASE_CONTENT                                                                       \
    ULONG                           ProcessNumberOfEntries;                             \
    ULONG                           CPUUsage;                                           \
    PCOSA_PROCESS_ENTRY             pProcTable;                                       \

typedef  struct
_COSA_DATAMODEL_PROCESS                                               
{
    COSA_DATAMODEL_PROCESS_CLASS_CONTENT
}
COSA_DATAMODEL_PROCSTATUS,  *PCOSA_DATAMODEL_PROCSTATUS;

typedef  struct
_COSA_BOOTSTRAP_STR
{
    CHAR                    ActiveValue[512];
    CHAR		    UpdateSource[16];
}
COSA_BOOTSTRAP_STR;

typedef  struct
_COSA_BOOTSTRAP_BOOL
{
    BOOL                    ActiveValue;
    CHAR		    UpdateSource[16];
}
COSA_BOOTSTRAP_BOOL;

typedef  struct
COSA_AUTO_REBOOT
{
    BOOL            Enable;
    INT	            UpTime;
}
COSA_AUTO_REBOOT;

typedef  struct
_COSA_DATAMODEL_RDKB_FOOTER_CLASS_CONTENT
{
	COSA_BOOTSTRAP_STR 		PartnerLink;
	COSA_BOOTSTRAP_STR 		UserGuideLink;
	COSA_BOOTSTRAP_STR 		CustomerCentralLink;
	COSA_BOOTSTRAP_STR		PartnerText;
	COSA_BOOTSTRAP_STR		UserGuideText;
	COSA_BOOTSTRAP_STR		CustomerCentralText;
}
COSA_DATAMODEL_RDKB_FOOTER, *PCOSA_DATAMODEL_RDKB_FOOTER;

typedef  struct
_COSA_DATAMODEL_RDKB_CONNECTION_CLASS_CONTENT
{
	COSA_BOOTSTRAP_STR 		MSOmenu;
	COSA_BOOTSTRAP_STR 		MSOinfo;
	COSA_BOOTSTRAP_STR 		StatusTitle;
	COSA_BOOTSTRAP_STR 		StatusInfo;
}
COSA_DATAMODEL_RDKB_CONNECTION, *PCOSA_DATAMODEL_RDKB_CONNECTION;

typedef  struct
_COSA_DATAMODEL_RDKB_NETWORKDIAGNOSTICTOOLS_CLASS_CONTENT
{
	COSA_BOOTSTRAP_STR 		ConnectivityTestURL;
}
COSA_DATAMODEL_RDKB_NETWORKDIAGNOSTICTOOLS, *PCOSA_DATAMODEL_RDKB_NETWORKDIAGNOSTICTOOLS;

typedef  struct
_COSA_DATAMODEL_RDKB_WIFIPERSONALIZATION_CLASS_CONTENT
{
	COSA_BOOTSTRAP_BOOL			Support;
	COSA_BOOTSTRAP_STR			PartnerHelpLink;
	COSA_BOOTSTRAP_BOOL			SMSsupport;
	COSA_BOOTSTRAP_BOOL			MyAccountAppSupport;
	COSA_BOOTSTRAP_STR			MSOLogo;
	COSA_BOOTSTRAP_STR			Title;
	COSA_BOOTSTRAP_STR			WelcomeMessage;
	COSA_BOOTSTRAP_STR			WelcomeMessage_fre;
}

COSA_DATAMODEL_RDKB_WIFIPERSONALIZATION, *PCOSA_DATAMODEL_RDKB_WIFIPERSONALIZATION;
typedef  struct
_COSA_DATAMODEL_RDKB_LOCALUI_CLASS_CONTENT
{
	COSA_BOOTSTRAP_STR			MSOLogo;
	COSA_BOOTSTRAP_STR			DefaultLoginUsername;
	COSA_BOOTSTRAP_STR			DefaultLoginPassword;
	COSA_BOOTSTRAP_STR			MSOLogoTitle;
	COSA_BOOTSTRAP_BOOL			HomeNetworkControl;
}
COSA_DATAMODEL_RDKB_LOCALUI, *PCOSA_DATAMODEL_RDKB_LOCALUI;

typedef  struct
_COSA_DATAMODEL_RDKB_HELPTIP_CLASS_CONTENT
{
	COSA_BOOTSTRAP_STR			NetworkName;
}
COSA_DATAMODEL_RDKB_HELPTIP, *PCOSA_DATAMODEL_RDKB_HELPTIP;

typedef  struct
_COSA_DATAMODEL_RDKB_CLOUDUI_CLASS_CONTENT
{
    COSA_BOOTSTRAP_STR            brandname;
    COSA_BOOTSTRAP_STR            productname;
    COSA_BOOTSTRAP_STR            link;
}
COSA_DATAMODEL_RDKB_CLOUDUI, *PCOSA_DATAMODEL_RDKB_CLOUDUI;

typedef  struct
_COSA_DATAMODEL_RDKB_UIBRANDING_CLASS_CONTENT
{
	COSA_DATAMODEL_RDKB_FOOTER			Footer;
	COSA_DATAMODEL_RDKB_CONNECTION			Connection;
	COSA_DATAMODEL_RDKB_NETWORKDIAGNOSTICTOOLS	NDiagTool;
	COSA_DATAMODEL_RDKB_WIFIPERSONALIZATION		WifiPersonal;
	COSA_DATAMODEL_RDKB_LOCALUI			LocalUI;
	COSA_DATAMODEL_RDKB_HELPTIP			HelpTip;
	COSA_DATAMODEL_RDKB_CLOUDUI			CloudUI;
	COSA_BOOTSTRAP_STR				DefaultAdminIP;
	COSA_BOOTSTRAP_STR				DefaultLocalIPv4SubnetRange;
	COSA_BOOTSTRAP_STR                        	DefaultLanguage;
	COSA_BOOTSTRAP_STR				PauseScreenFileLocation;
	COSA_BOOTSTRAP_BOOL				AllowEthernetWAN;
}
COSA_DATAMODEL_RDKB_UIBRANDING, *PCOSA_DATAMODEL_RDKB_UIBRANDING;

typedef  struct
_COSA_DATAMODEL_RDKB_WIFI_TELEMETRY_CLASS_CONTENT
{
    INT                 LogInterval;
    INT                 ChUtilityLogInterval;
    CHAR            NormalizedRssiList[256];
    CHAR            CliStatList[256];
    CHAR            TxRxRateList[256];
    CHAR            SNRList[256];
}
COSA_DATAMODEL_RDKB_WIFI_TELEMETRY, *PCOSA_DATAMODEL_RDKB_WIFI_TELEMETRY;

typedef  struct
_COSA_DATAMODEL_RDKB_UNIQUE_TELEMETRY_ID_CLASS_CONTENT
{
    BOOL		Enable;
    CHAR            	TagString[256];
    INT                 TimingInterval;

}
COSA_DATAMODEL_RDKB_UNIQUE_TELEMETRY_ID, *PCOSA_DATAMODEL_RDKB_UNIQUE_TELEMETRY_ID;

typedef  struct
_COSA_DATAMODEL_RDKB_SYNDICATIONFLOWCONTROL_CLASS_CONTENT
{
    COSA_BOOTSTRAP_BOOL    Enable;
    COSA_BOOTSTRAP_STR    InitialForwardedMark;
    COSA_BOOTSTRAP_STR    InitialOutputMark;
}
COSA_DATAMODEL_RDKB_SYNDICATIONFLOWCONTROL, *PCOSA_DATAMODEL_RDKB_SYNDICATIONFLOWCONTROL;


typedef  struct
    _COSA_DATAMODEL_KICKSTARTTABLE
{
    UINT    SecurityNumberLen;
    uint8_t SecurityNumber[MAX_SECURITYNUM_LEN];
    CHAR    SecurityName[MAX_SECURITYNAME_LEN];
}
COSA_DATAMODEL_KICKSTARTTABLE, *PCOSA_DATAMODEL_KICKSTARTTABLE;

#define COSA_DATAMODEL_KICKSTART_CLASS_CONTENT                      \
    /* duplication of the base object class content */              \
    /* COSA_BASE_CONTENT */                                         \
    BOOL                            TableUpdated;                   \
    BOOL                            Enabled;                        \
    UINT                            KickstartTotal;                 \
    UINT                            TableNumberOfEntries;           \
    COSA_DATAMODEL_KICKSTARTTABLE   KickstartTable[MAX_KICKSTART_ROWS];   

typedef  struct
    _COSA_DATAMODEL_KICKSTART
{
    COSA_DATAMODEL_KICKSTART_CLASS_CONTENT
}
COSA_DATAMODEL_KICKSTART,   *PCOSA_DATAMODEL_KICKSTART;

typedef struct
    _COSA_HHT_PTR
{
    UINT CPUThreshold;
    UINT DRAMThreshold;
    UINT Frequency;
    BOOL PTREnable;
}
COSA_HHT_PTR;

/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/
int getRebootCounter();
int setRebootCounter();
int setUnknownRebootReason();
ANSC_HANDLE CosaProcStatusCreate();
void COSADmlRemoveProcessInfo(PCOSA_DATAMODEL_PROCSTATUS pObj);
void COSADmlGetProcessInfo(PCOSA_DATAMODEL_PROCSTATUS p_info);
ULONG COSADmlGetCpuUsage();
ULONG COSADmlGetMemoryStatus(char * ParamName);
ULONG COSADmlGetMaxWindowSize();
int COSADmlSetMemoryStatus(char * ParamName, ULONG val);

ANSC_STATUS
CosaDmlDiInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

ANSC_STATUS
CosaDmlDiGetManufacturer
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

ANSC_STATUS
CosaDmlDiGetManufacturerOUI
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

ANSC_STATUS
CosaDmlDiGetModelName
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

ANSC_STATUS
CosaDmlDiGetDescription
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

ANSC_STATUS
CosaDmlDiGetProductClass
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

ANSC_STATUS
CosaDmlDiGetSerialNumber
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

ANSC_STATUS
CosaDmlDiGetHardwareVersion
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

ANSC_STATUS
CosaDmlDiGetSoftwareVersion
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

ANSC_STATUS
CosaDmlDiGetAdditionalHardwareVersion
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

ANSC_STATUS
CosaDmlDiGetAdditionalSoftwareVersion
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

ANSC_STATUS
CosaDmlDiGetProvisioningCode
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

ANSC_STATUS
CosaDmlDiSetProvisioningCode
    (
        ANSC_HANDLE                 hContext,
        char*                       pProvisioningCode
    );

ULONG
CosaDmlDiGetUpTime
    (
        ANSC_HANDLE                 hContext
    );

ANSC_STATUS
CosaDmlDiGetFirstUseDate
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    );

ANSC_STATUS
CosaDmlDiGetHardware
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    );

ANSC_STATUS
CosaDmlDiGetBootloaderVersion
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    );

ANSC_STATUS
CosaDmlDiGetFirmwareName
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    );

ANSC_STATUS
CosaDmlDiGetFirmwareBuildTime
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    );

ANSC_STATUS
CosaDmlDiGetBaseMacAddress
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    );
    
ANSC_STATUS
CosaDmlGetTCPImplementation
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize    
    );

ANSC_STATUS
CosaDmlDiGetAdvancedServices
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

ANSC_STATUS
CosaDmlDiGetProcessorSpeed
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );
ANSC_STATUS
isValidInput
    (
        char                       *inputparam,
        char                       *wrapped_inputparam,
    	int							lengthof_inputparam,
    	int							sizeof_wrapped_inputparam    	
    );


ANSC_STATUS
CosaDmlDiGetFirmwareUpgradeStartTime
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

ANSC_STATUS
CosaDmlDiGetFirmwareUpgradeEndTime
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

ANSC_STATUS
CosaDmlDiSetFirmwareUpgradeStartTime (char* pString);

ANSC_STATUS
CosaDmlDiSetFirmwareUpgradeEndTime (char* pString);

ULONG
CosaDmlGiGetCustomDataModelEnabled
(
    ANSC_HANDLE                 hContext,
    BOOL                        *pValue
);

ULONG
CosaDmlGiSetCustomDataModelEnabled
(
    ANSC_HANDLE                 hContext,
    BOOL                        bValue
);

ANSC_STATUS
CosaDmlDiGetAndProcessDhcpServDetectionFlag
   (
	   ANSC_HANDLE				   hContext,
	   BOOLEAN*			   		   pValue
   );

ANSC_STATUS
CosaDmlDiSetAndProcessDhcpServDetectionFlag
   (
	   ANSC_HANDLE				   hContext,
	   BOOLEAN*					   pValue,
	   BOOLEAN*					   pDhcpServDetectEnable
   );

int setXOpsReverseSshTrigger(char *input); 
int setXOpsReverseSshArgs(char* pString);
ANSC_STATUS getXOpsReverseSshArgs
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );
int isRevSshActive(void);


ANSC_STATUS
CosaDmlDiGetSyndicationPartnerId
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    );

ANSC_STATUS
CosaDmlDiGetSyndicationTR69CertLocation
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

ANSC_STATUS
CosaDmlDiSetSyndicationTR69CertLocation
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

void* CosaDmlDiPartnerIDChangeHandling();

ANSC_STATUS
CosaDmlDiGetSyndicationLocalUIBrandingTable
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    );
ANSC_STATUS
CosaDmlDiGetSyndicationWifiUIBrandingTable
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    );

#ifndef FEATURE_FWUPGRADE_MANAGER
ANSC_STATUS
CosaDmlDiSetFirmwareDownloadAndFactoryReset(void);

void CosaDmlDiGet_DeferFWDownloadReboot(ULONG* puLong);
void CosaDmlDiSet_DeferFWDownloadReboot(ULONG* DeferFWDownloadReboot , ULONG uValue);
#endif
void CosaDmlDiSet_RebootDevice(char* pValue);
BOOL CosaDmlDi_ValidateRebootDeviceParam(char *pValue);

ANSC_STATUS
CosaDmlDiUiBrandingInit
  (
	ANSC_HANDLE                 hContext,
	PCOSA_DATAMODEL_RDKB_UIBRANDING	PUiBrand
  );

void FillPartnerIDValues(cJSON *json , char *partnerID , PCOSA_DATAMODEL_RDKB_UIBRANDING PUiBrand, ANSC_HANDLE hContext);

ANSC_STATUS UpdateJsonParam
	(
		char*           pKey,
		char*			PartnerId,
		char*			pValue,
		char*                   pSource,
		char*			pCurrentTime
    );

ANSC_STATUS
CosaDmlDiWiFiTelemetryInit
  (
	PCOSA_DATAMODEL_RDKB_WIFI_TELEMETRY PWiFi_Telemetry
  );

ANSC_STATUS
CosaDmlDiUniqueTelemetryIdInit
  (
	PCOSA_DATAMODEL_RDKB_UNIQUE_TELEMETRY_ID PUniqueTelemetryId
  );

void ConvertTime(int time, char day[], char hour[], char mins[]);

void UniqueTelemetryCronJob(BOOL enable, INT timeInterval, char* tagString);

ULONG
CosaDmlDiGetBootTime
    (
        ANSC_HANDLE                 Context
    );

ANSC_STATUS
CosaDmlDiGetFactoryResetCount
    (
        ANSC_HANDLE                 hContext,
        ULONG                       *pValue
    );

ANSC_STATUS
CosaDmlDiGetHardware_MemUsed
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    );

ANSC_STATUS
CosaDmlDiGetHardware_MemFree
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        PULONG                      pulSize
    );

ANSC_STATUS
CosaDmlDiGetCMTSMac
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue,
        ULONG*                      pulSize
    );

ANSC_STATUS CosaDmlDiClearResetCount
    (
        ANSC_HANDLE                 hContext,
        BOOL                        bValue
   );

ANSC_STATUS
CosaDmlSetnewNTPEnable(BOOL bValue);

BOOL CosaDmlGetInternetStatus();

ANSC_STATUS
COSADmlUploadLogsStatus
    (
        ANSC_HANDLE                 Context,
        char*   pValue,
        ULONG*  pUlSize
    );

ANSC_STATUS
COSADmlUploadLogsNow
        (
                ANSC_HANDLE                 hContext,
                BOOL                        bEnable
        );

void CosaDmlPresenceEnable(BOOL enable);

ANSC_STATUS
StartRfcProcessing();

ANSC_STATUS
EndRfcProcessing(cJSON **pRfcStore);

ANSC_STATUS
CosaDmlDiSet_SyndicationFlowControl_Enable
    (
        char bValue
    );

ANSC_STATUS
CosaDmlDiSet_SyndicationFlowControl_InitialForwardedMark
    (
        char *pString
    );

ANSC_STATUS
CosaDmlDiSet_SyndicationFlowControl_InitialOutputMark
    (
        char *pString
    );

ANSC_STATUS 
setTempPartnerId
    (
        char*                       pValue
    );

ANSC_STATUS 
getFactoryPartnerId
    (
        char*                       pValue,
        PULONG                      pulSize
    );

ANSC_STATUS
CosaDeriveSyndicationPartnerID
    (
        char *Partner_ID
    );

ANSC_STATUS
CosaDmlDiSyndicationFlowControlInit
    (
        PCOSA_DATAMODEL_RDKB_SYNDICATIONFLOWCONTROL pSyndicatonFlowControl
    );

ANSC_STATUS
CosaDmlDiRfcDefaultsInit
    (
        cJSON **pRfcDefaults
    );

ANSC_STATUS
CosaDmlDiRfcStoreInit
    (
        cJSON **pRfcStore
    );

ANSC_STATUS 
activatePartnerId();

ANSC_STATUS
ProcessRfcSet(cJSON **pRfcStore, BOOL clearDB, char *paramFullName, char *value, char *pSource, char *pCurrentTime);

ANSC_STATUS
CosaDmlScheduleAutoReboot(int ConfiguredUpTime, BOOL bValue);

int 
setMultiProfileXdnsConfig(BOOL bValue);

#if defined (FEATURE_SUPPORT_RADIUSGREYLIST)
BOOL
CosaDmlSetRadiusGreyListEnable
    (
        BOOL        bValue
    );
#endif
#if defined(FEATURE_HOSTAP_AUTHENTICATOR)
BOOL
CosaDmlSetNativeHostapdState
    (
        BOOL        bValue
    );
#endif //FEATURE_HOSTAP_AUTHENTICATOR

#endif
