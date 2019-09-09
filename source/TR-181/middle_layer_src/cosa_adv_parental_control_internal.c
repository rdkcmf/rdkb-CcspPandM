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
    int                     retGet  = CCSP_SUCCESS;
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

#if !defined (_XB7_PRODUCT_REQ_)
    if ((fd = fopen (ADV_PC_FILE_PATH, "r")) != NULL)
    {
        fclose(fd);
        pMyObject->bEnable = FALSE;
    }
    else
    {
        pMyObject->bEnable = TRUE;
    }
#else
    pMyObject->bEnable = FALSE;
#endif

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

ANSC_STATUS CosaAdvPCInit(ANSC_HANDLE hThisObject)
{
    ANSC_STATUS                 returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_ADVPC     pMyObject    = (PCOSA_DATAMODEL_ADVPC)hThisObject;
    char cmd[128];

    memset(cmd, 0, sizeof(cmd));

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        remove(ADV_PC_FILE_PATH);
        AnscCopyString(cmd, "sysevent set adv_parental_control 1");
        system(cmd);
        pMyObject->bEnable = TRUE;
    }
    return returnStatus;
}

ANSC_STATUS CosaAdvPCDeInit(ANSC_HANDLE hThisObject)
{
    ANSC_STATUS                 returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_ADVPC     pMyObject    = (PCOSA_DATAMODEL_ADVPC)hThisObject;
    char cmd[128];
    FILE                           *fd;

    memset(cmd, 0, sizeof(cmd));

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        if ((fd = fopen (ADV_PC_FILE_PATH, "w+")) != NULL)
        {
            fclose(fd);
        }
        AnscCopyString(cmd, "sysevent set adv_parental_control 0");
        system(cmd);
        pMyObject->bEnable = FALSE;
    }
    return returnStatus;
}

