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

#include "cosa_adv_parental_control_internal.h"
#include "cosa_adv_parental_control_dml.h"
#include "ccsp_psm_helper.h"
#include "cosa_advsec_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include "sysevent/sysevent.h"
#include "utapi/utapi.h"
#include "utapi/utapi_util.h"

#define ADV_PC_FILE_PATH "/tmp/adv_pc_rfc_disabled"

extern ANSC_HANDLE bus_handle;
extern char g_Subsystem[32];

ANSC_HANDLE
CosaAdvPCCreate
    (
        VOID
    )
{
    PCOSA_DATAMODEL_ADVPC       pMyObject    = (PCOSA_DATAMODEL_ADVPC)NULL;
    FILE                           *fd;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PCOSA_DATAMODEL_ADVPC)AnscAllocateMemory(sizeof(COSA_DATAMODEL_ADVPC));

    if ( !pMyObject )
    {
    	CcspTraceInfo(("%s exit ERROR \n", __FUNCTION__));
        return  (ANSC_HANDLE)NULL;
    }

    if ((fd = fopen (ADV_PC_FILE_PATH, "r")) != NULL)
    {
        fclose(fd);
        pMyObject->bEnable = FALSE;
    }
    else
    {
        pMyObject->bEnable = TRUE;
    }

    return  (ANSC_HANDLE)pMyObject;
}

ANSC_STATUS
CosaAdvPCRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_ADVPC            pMyObject    = (PCOSA_DATAMODEL_ADVPC)hThisObject;

    /* Remove self */
    AnscFreeMemory((ANSC_HANDLE)pMyObject);
    CcspTraceInfo(("%s EXIT \n", __FUNCTION__));

    return returnStatus;
}

ANSC_STATUS
CosaAdvPCInit
    (
             ANSC_HANDLE                 hThisObject
    )
{
    PCOSA_DATAMODEL_ADVPC     pMyObject    = (PCOSA_DATAMODEL_ADVPC)hThisObject;
    token_t  se_token;
    int se_fd = -1;
    remove (ADV_PC_FILE_PATH);
    se_fd = s_sysevent_connect(&se_token);
    if (0 > se_fd)
    {
        CcspTraceWarning (("%s: Unable to register with sysevent daemon.", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
    if (sysevent_set (se_fd, se_token, "adv_parental_control", "1", 0))
    {
        CcspTraceWarning (("%s: sysevent_set failure.", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
    pMyObject->bEnable = TRUE;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaAdvPCDeInit
    (
             ANSC_HANDLE                 hThisObject
    )
{
    PCOSA_DATAMODEL_ADVPC     pMyObject    = (PCOSA_DATAMODEL_ADVPC)hThisObject;
    token_t  se_token;
    int se_fd = -1;
    FILE                           *fd = NULL;
    if ((fd = fopen (ADV_PC_FILE_PATH, "w+")) != NULL)
    {
        fclose (fd);
    }
    se_fd = s_sysevent_connect(&se_token);
    if (0 > se_fd)
    {
        CcspTraceWarning (("%s: Unable to register with sysevent daemon.", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
    if (sysevent_set (se_fd, se_token, "adv_parental_control", "0", 0))
    {
        CcspTraceWarning (("%s: sysevent_set failure.", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
    pMyObject->bEnable = FALSE;
    return ANSC_STATUS_SUCCESS;
}
