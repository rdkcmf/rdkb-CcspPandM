/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2016 RDK Management
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

/**************************************************************************

    module: cosa_x_rdkcentral_com_xpc_internal.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implements back-end apis for the COSA Data Model Library

**************************************************************************/
#ifdef FEATURE_SUPPORT_ONBOARD_LOGGING
#include "cosa_x_rdkcentral_com_xpc_internal.h"
#include "cosa_x_rdkcentral_com_xpc_dml.h"
#include "ccsp_psm_helper.h"
#include <stdio.h>
#include <stdlib.h>
#include "platform_hal.h"

extern ANSC_HANDLE bus_handle;
extern char g_Subsystem[32];

ANSC_HANDLE
CosaXpcCreate
    (
        VOID
    )
{
    PCOSA_DATAMODEL_XPC       pMyObject    = (PCOSA_DATAMODEL_XPC)NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PCOSA_DATAMODEL_XPC)AnscAllocateMemory(sizeof(COSA_DATAMODEL_XPC));

    if ( !pMyObject )
    {
        CcspTraceInfo(("%s exit ERROR \n", __FUNCTION__));
        return  (ANSC_HANDLE)NULL;
    }

    pMyObject->bEnable = TRUE;

    return  (ANSC_HANDLE)pMyObject;
}

ANSC_STATUS
CosaXpcRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_XPC            pMyObject    = (PCOSA_DATAMODEL_XPC)hThisObject;

    /* Remove self */
    AnscFreeMemory((ANSC_HANDLE)pMyObject);
    CcspTraceInfo(("%s EXIT \n", __FUNCTION__));

    return returnStatus;
}

ANSC_STATUS CosaXpcEnable(ANSC_HANDLE hThisObject)
{
    ANSC_STATUS                 returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_XPC     pMyObject    = (PCOSA_DATAMODEL_XPC)hThisObject;

    system("sysevent set snmp-onboard-reboot enable");
    if ( platform_hal_SetSNMPOnboardRebootEnable("enable") != RETURN_OK )
    {
        return ANSC_STATUS_FAILURE;
    }
    pMyObject->bEnable = TRUE;
    return returnStatus;
}

ANSC_STATUS CosaXpcDisable(ANSC_HANDLE hThisObject)
{
    ANSC_STATUS                 returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_XPC     pMyObject    = (PCOSA_DATAMODEL_XPC)hThisObject;

    system("sysevent set snmp-onboard-reboot disable");
    if ( platform_hal_SetSNMPOnboardRebootEnable("disable") != RETURN_OK )
    {
        return ANSC_STATUS_FAILURE;
    }
    pMyObject->bEnable = FALSE;
    return returnStatus;
}
#endif
