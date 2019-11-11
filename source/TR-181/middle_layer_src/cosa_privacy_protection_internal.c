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

    module: cosa_adv_parental_control_internal.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implements back-end apis for the COSA Data Model Library

**************************************************************************/

#include "cosa_privacy_protection_internal.h"
#include "cosa_privacy_protection_dml.h"
#include "ccsp_psm_helper.h"
#include "cosa_advsec_utils.h"
#include <stdio.h>
#include <stdlib.h>

#define PRIV_PROT_FILE_PATH "/tmp/adv_priv_prot_rfc_disabled"

extern ANSC_HANDLE bus_handle;
extern char g_Subsystem[32];
static char *g_PrivacyProtectionEnabled = "Adv_PrivProtRFCEnable";

ANSC_HANDLE
CosaPrivacyProtectionCreate
    (
        VOID
    )
{
    PCOSA_DATAMODEL_PRIVACYPROTECTION       pMyObject    = (PCOSA_DATAMODEL_PRIVACYPROTECTION)NULL;
    int                     retGet  = CCSP_SUCCESS;
    FILE                           *fd;
    ULONG                   syscfgValue = 0;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PCOSA_DATAMODEL_PRIVACYPROTECTION)AnscAllocateMemory(sizeof(COSA_DATAMODEL_PRIVACYPROTECTION));

    if ( !pMyObject )
    {
    	CcspTraceInfo(("%s exit ERROR \n", __FUNCTION__));
        return  (ANSC_HANDLE)NULL;
    }

    retGet = CosaGetSysCfgUlong(g_PrivacyProtectionEnabled, &syscfgValue);
    
    if ( retGet == ANSC_STATUS_SUCCESS )
    {
        if(syscfgValue == 0)
        {
            pMyObject->bEnable = FALSE;
            if ((fd = fopen (PRIV_PROT_FILE_PATH, "w+")) != NULL)
            {
                fclose(fd);
            }            
        }
        else
        {
            pMyObject->bEnable = TRUE;
        }
    }
    else
    {
        retGet = CosaSetSysCfgUlong(g_PrivacyProtectionEnabled, 1);
        if ( retGet == ANSC_STATUS_SUCCESS )
        {
            pMyObject->bEnable = TRUE;
        }
    }

    return  (ANSC_HANDLE)pMyObject;
}

ANSC_STATUS
CosaPrivacyProtectionRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_PRIVACYPROTECTION            pMyObject    = (PCOSA_DATAMODEL_PRIVACYPROTECTION)hThisObject;

    /* Remove self */
    AnscFreeMemory((ANSC_HANDLE)pMyObject);
    CcspTraceInfo(("%s EXIT \n", __FUNCTION__));

    return returnStatus;
}

ANSC_STATUS CosaPrivacyProtectionInit(ANSC_HANDLE hThisObject)
{
    ANSC_STATUS                 returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_PRIVACYPROTECTION     pMyObject    = (PCOSA_DATAMODEL_PRIVACYPROTECTION)hThisObject;
    char cmd[128];

    memset(cmd, 0, sizeof(cmd));

    returnStatus = CosaSetSysCfgUlong(g_PrivacyProtectionEnabled, 1);
    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        remove(PRIV_PROT_FILE_PATH);
        AnscCopyString(cmd, "sysevent set privacy_protection 1");
        system(cmd);
        pMyObject->bEnable = TRUE;
        CcspTraceWarning(("AdTrackerBlockingRFCEnable:TRUE\n"));
    }
    return returnStatus;
}

ANSC_STATUS CosaPrivacyProtectionDeInit(ANSC_HANDLE hThisObject)
{
    ANSC_STATUS                 returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_PRIVACYPROTECTION     pMyObject    = (PCOSA_DATAMODEL_PRIVACYPROTECTION)hThisObject;
    char cmd[128];
    FILE                           *fd;

    memset(cmd, 0, sizeof(cmd));

    returnStatus = CosaSetSysCfgUlong(g_PrivacyProtectionEnabled, 0);
    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        if ((fd = fopen (PRIV_PROT_FILE_PATH, "w+")) != NULL)
        {
            fclose(fd);
        }

        AnscCopyString(cmd, "sysevent set privacy_protection 0");
        system(cmd);
        pMyObject->bEnable = FALSE;
        CcspTraceWarning(("AdTrackerBlockingRFCEnable:FALSE\n"));
    }
    return returnStatus;
}

