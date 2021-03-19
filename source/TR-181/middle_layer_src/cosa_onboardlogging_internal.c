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

    module: cosa_onboardlogging_internal.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implements back-end apis for the COSA Data Model Library

**************************************************************************/
#ifdef FEATURE_SUPPORT_ONBOARD_LOGGING
#include "cosa_onboardlogging_internal.h"
#include "cosa_onboardlogging_dml.h"
#include "ccsp_psm_helper.h"
#include "secure_wrapper.h"
#define DEVICE_PROPS_FILE  "/etc/device.properties"

extern ANSC_HANDLE bus_handle;
extern char g_Subsystem[32];

static int getValueFromDevicePropsFile(char *str, char **value);

ANSC_HANDLE
CosaOnboardLoggingCreate
    (
        VOID
    )
{
    PCOSA_DATAMODEL_ONBOARDLOGGING       pMyObject    = (PCOSA_DATAMODEL_ONBOARDLOGGING)NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PCOSA_DATAMODEL_ONBOARDLOGGING)AnscAllocateMemory(sizeof(COSA_DATAMODEL_ONBOARDLOGGING));

    if ( !pMyObject )
    {
    	CcspTraceInfo(("%s exit ERROR \n", __FUNCTION__));
        return  (ANSC_HANDLE)NULL;
    }

    if(access("/nvram/DISABLE_ONBOARD_LOGGING", F_OK) != -1)
    {
        pMyObject->bEnable = false;
    }
    else
    {
        pMyObject->bEnable = true;
    }

    return  (ANSC_HANDLE)pMyObject;
}

ANSC_STATUS
CosaOnboardLoggingRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_ONBOARDLOGGING            pMyObject    = (PCOSA_DATAMODEL_ONBOARDLOGGING)hThisObject;

    /* Remove self */
    AnscFreeMemory((ANSC_HANDLE)pMyObject);
    CcspTraceInfo(("%s EXIT \n", __FUNCTION__));

    return returnStatus;
}

ANSC_STATUS DisableOnboardLogging(ANSC_HANDLE hThisObject)
{
    ANSC_STATUS                 returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_ONBOARDLOGGING     pMyObject    = (PCOSA_DATAMODEL_ONBOARDLOGGING)hThisObject;
    v_secure_system("touch /nvram/DISABLE_ONBOARD_LOGGING");
    char *boxType = NULL, *atomIp = NULL;
    if(getValueFromDevicePropsFile("BOX_TYPE", &boxType) == 0)
    {
        if(strcmp(boxType, "XB3") == 0)
        {
            if(getValueFromDevicePropsFile("ATOM_ARPING_IP", &atomIp) == 0)
            {
                v_secure_system("/usr/bin/rpcclient %s 'touch /nvram/DISABLE_ONBOARD_LOGGING'", atomIp);
            }
        }
    }
    pMyObject->bEnable = FALSE;
    return returnStatus;
}

ANSC_STATUS EnableOnboardLogging(ANSC_HANDLE hThisObject)
{
    ANSC_STATUS                 returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_ONBOARDLOGGING     pMyObject    = (PCOSA_DATAMODEL_ONBOARDLOGGING)hThisObject;
    v_secure_system("rm -rf /nvram/DISABLE_ONBOARD_LOGGING");
    char *boxType = NULL, *atomIp = NULL;
    if(getValueFromDevicePropsFile("BOX_TYPE", &boxType) == 0)
    {
        if(strcmp(boxType, "XB3") == 0)
        {
            if(getValueFromDevicePropsFile("ATOM_ARPING_IP", &atomIp) == 0)
            {
                v_secure_system("/usr/bin/rpcclient %s 'rm -rf /nvram/DISABLE_ONBOARD_LOGGING'", atomIp);
            }
        }
    }
    pMyObject->bEnable = TRUE;
    return returnStatus;
}

static int getValueFromDevicePropsFile(char *str, char **value)
{
    FILE *fp = fopen(DEVICE_PROPS_FILE, "r");
    char         buf[ 1024 ] = { 0 };
    char *tempStr = NULL;
    if( NULL != fp )
    {
        while ( fgets( buf, sizeof( buf ), fp ) != NULL )
        {
            if ( strstr( buf, str ) != NULL )
            {
                buf[strcspn( buf, "\r\n" )] = 0; // Strip off any carriage returns
                tempStr = strstr( buf, "=" );
                tempStr++;
                *value = tempStr;
                return 0;
            }
        }
        if( NULL == *value)
        {
            CcspTraceError(("%s is not present in device.properties file\n",str));
            return -1;
        }
    }
    else
    {
        CcspTraceError(("Failed to open file:%s\n", DEVICE_PROPS_FILE));
        return -1;
    }
    return 0;
}
#endif
