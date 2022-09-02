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

    module: cosa_deviceinfo_internal.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaDeviceInfoCreate
        *  CosaDeviceInfoInitialize
        *  CosaDeviceInfoRemove
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

#include "cosa_deviceinfo_internal.h"
#include "cosa_deviceinfo_apis_custom.h"
#include "syscfg/syscfg.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        CosaDeviceInfoCreate
            (
            );

    description:

        This function constructs cosa DeviceInfo object and return handle.

    argument:  

    return:     newly created DeviceInfo object.

**********************************************************************/

ANSC_HANDLE
CosaDeviceInfoCreate
    (
        VOID
    )
{
    PCOSA_DATAMODEL_DEVICEINFO  pMyObject    = (PCOSA_DATAMODEL_DEVICEINFO)NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)AnscAllocateMemory(sizeof(COSA_DATAMODEL_DEVICEINFO));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->Oid               = COSA_DATAMODEL_DEVICEINFO_OID;
    pMyObject->Create            = CosaDeviceInfoCreate;
    pMyObject->Remove            = CosaDeviceInfoRemove;
    pMyObject->Initialize        = CosaDeviceInfoInitialize;

    pMyObject->Initialize   ((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDeviceInfoInitialize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate  cosa DeviceInfo object and return handle.

    argument:   ANSC_HANDLE                 hThisObject
            This handle is actually the pointer of this object
            itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaDeviceInfoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject    = (PCOSA_DATAMODEL_DEVICEINFO)hThisObject;
    errno_t rc = -1;

    /* Initiation all functions */
    _ansc_memset(pMyObject->ProvisioningCode, 0, 64);
    CosaDmlDiInit(NULL, NULL);
    CosaDmlDiGetXfinityWiFiEnable(&pMyObject->bxfinitywifiEnable);
	CosaDmlDiGetCloudCapable(&pMyObject->bCloudCapable);
	syscfg_get(NULL, "CloudPersonalizationURL", pMyObject->CloudPersonalizationURL, sizeof(pMyObject->CloudPersonalizationURL));
    printf("%s : bxfinitywifiEnable value is : %d\n",__FUNCTION__,pMyObject->bxfinitywifiEnable);

	CosaDmlDiGetAndProcessDhcpServDetectionFlag( pMyObject, &pMyObject->bDhcpServDetectEnable );
	pMyObject->bMultipleGW	= FALSE;
#ifndef _COSA_FOR_BCI_
#ifndef FEATURE_FWUPGRADE_MANAGER
	CosaDmlDiGet_DeferFWDownloadReboot(&pMyObject->DeferFWDownloadReboot);
#endif
#endif	
        pMyObject->AbortReboot	= FALSE;
	pMyObject->FirmwareDownloadCompletedNotification	= FALSE;
	_ansc_memset(pMyObject->FirmwareDownloadStartedNotification, 0, 64);
	_ansc_memset(pMyObject->DeviceManageableNotification, 0, 64);
	rc = STRCPY_S_NOCLOBBER( pMyObject->FirmwareDownloadStartedNotification, sizeof(pMyObject->FirmwareDownloadStartedNotification), "0" );
	ERR_CHK(rc);
	rc = STRCPY_S_NOCLOBBER( pMyObject->DeviceManageableNotification, sizeof(pMyObject->DeviceManageableNotification), "0" );
	ERR_CHK(rc);

	CosaDmlGiGetCustomDataModelEnabled(NULL, &pMyObject->CustomDataModelEnabled);

	//Get TR69CertLocation
	CosaDmlDiGetSyndicationTR69CertLocation( (ANSC_HANDLE)pMyObject,
											  pMyObject->TR69CertLocation.ActiveValue );
	CosaDeriveSyndicationPartnerID(pMyObject->PartnerID);
    CosaDmlDiUiBrandingInit((ANSC_HANDLE)pMyObject, &pMyObject->UiBrand, &pMyObject->CdlDM);
	CosaDmlDiWiFiTelemetryInit(&pMyObject->WiFi_Telemetry);
	CosaDmlDiUniqueTelemetryIdInit(&pMyObject->UniqueTelemetryId);
    CosaDmlDiSyndicationFlowControlInit(&pMyObject->SyndicatonFlowControl);
    CosaDmlDiRfcDefaultsInit(&pMyObject->pRfcDefaults);
    CosaDmlDiRfcStoreInit(&pMyObject->pRfcStore);
    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDeviceInfoRemove
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate  cosa DeviceInfo object and return handle.

    argument:   ANSC_HANDLE                 hThisObject
            This handle is actually the pointer of this object
            itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaDeviceInfoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject    = (PCOSA_DATAMODEL_DEVICEINFO)hThisObject;

    /* Remove necessary resounce */
    cJSON_Delete(pMyObject->pRfcDefaults);
    cJSON_Delete(pMyObject->pRfcStore);

    /* Remove self */
    AnscFreeMemory((ANSC_HANDLE)pMyObject);

    return returnStatus;
}

