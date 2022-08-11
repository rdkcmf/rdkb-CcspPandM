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

    module: cosa_deviceinfo_internal.h

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

#ifndef  _COSA_DEVICEINFO_INTERNAL_H
#define  _COSA_DEVICEINFO_INTERNAL_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "cosa_deviceinfo_apis.h"

#define  COSA_DATAMODEL_DEVICEINFO_CLASS_CONTENT                            \
    /* duplication of the base object class content */                      \
    COSA_BASE_CONTENT                                                       \
    /* start of DeviceInfo object class content */                          \
    /* CID: 78739 Out-of-bounds access with PCOSA_DATAMODEL_DEVICEINFO */     \
    UCHAR               ProvisioningCode[256];                               \
    

typedef  struct
_COSA_DATAMODEL_DEVICEINFO_CLASS_CONTENT
{
    COSA_DATAMODEL_DEVICEINFO_CLASS_CONTENT
    BOOL            bEnableMoCAforXi5;
#ifdef CONFIG_INTERNET2P0
    BOOL		bWiFiConfigued;	
    BOOL		bCaptivePortalEnable;	
    BOOL		bCloudCapable;	
    BOOL		bCloudEnable;				    
    CHAR		WebURL[64];
#endif
    char        CloudPersonalizationURL[64];
	/*Added for EMS*/
	CHAR 		EMS_ServerURL[64];
	CHAR 		EMS_MobileNo[64];
/*change ends here*/
    BOOL		bxfinitywifiEnable;
	BOOL		bDhcpServDetectEnable;	
	BOOL		bMultipleGW;					
#ifndef FEATURE_FWUPGRADE_MANAGER
	ULONG		DeferFWDownloadReboot;
#endif	
	BOOL		AbortReboot;
	BOOL		FirmwareDownloadCompletedNotification;
	CHAR 		FirmwareDownloadStartedNotification[128];
	CHAR 		DeviceManageableNotification[64];
	BOOL            CustomDataModelEnabled;
	COSA_BOOTSTRAP_STR 		TR69CertLocation;
	COSA_BOOTSTRAP_STR 		MeshRedirectorURL; //Prash
        COSA_BOOTSTRAP_STR              XconfURL;
        COSA_BOOTSTRAP_STR              LogUploadURL;
        COSA_BOOTSTRAP_STR              TelemetryURL;
        COSA_BOOTSTRAP_STR              CrashPortalURL;
	COSA_BOOTSTRAP_STR              AdvsecRedirectorURL;
	CHAR 		PartnerID[64];
	COSA_BOOTSTRAP_BOOL		bWANsideSSHEnable;

    COSA_DATAMODEL_RDKB_UIBRANDING	UiBrand;
    COSA_DATAMODEL_RDKB_CDLDM           CdlDM;
    COSA_DATAMODEL_RDKB_WIFI_TELEMETRY    WiFi_Telemetry;
	COSA_DATAMODEL_RDKB_UNIQUE_TELEMETRY_ID     	UniqueTelemetryId;
    COSA_DATAMODEL_RDKB_SYNDICATIONFLOWCONTROL  SyndicatonFlowControl;
    COSA_DATAMODEL_KICKSTART                    Kickstart;
    cJSON *pRfcDefaults;
    cJSON *pRfcStore;
    COSA_AUTO_REBOOT                            AutoReboot;
    COSA_HHT_PTR                                HwHealtTestPTR;
}
COSA_DATAMODEL_DEVICEINFO, *PCOSA_DATAMODEL_DEVICEINFO;

/*
    Standard function declaration
*/
ANSC_HANDLE
CosaDeviceInfoCreate
    (
        VOID
    );

ANSC_STATUS
CosaDeviceInfoInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaDeviceInfoRemove
    (
        ANSC_HANDLE                 hThisObject
    );

#endif
