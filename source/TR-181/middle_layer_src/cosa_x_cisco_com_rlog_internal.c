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

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "cosa_x_cisco_com_rlog_apis.h"
#include "cosa_x_cisco_com_rlog_internal.h"

ANSC_HANDLE
CosaRLogCreate(void)
{
    PCOSA_DATAMODEL_RLOG    pRLog;

    pRLog = AnscAllocateMemory(sizeof(COSA_DATAMODEL_RLOG));
    if (!pRLog)
        return (ANSC_HANDLE)NULL;

    pRLog->Oid          = COSA_DATAMODEL_RLOG_OID;
    pRLog->Create       = CosaRLogCreate;
    pRLog->Remove       = CosaRLogRemove;
    pRLog->Initialize   = CosaRLogInitialize;

    if (pRLog->Initialize(pRLog) != ANSC_STATUS_SUCCESS)
    {
        AnscTraceError(("%s: Fail to init remote logging\n", __FUNCTION__));
        AnscFreeMemory(pRLog);
        return (ANSC_HANDLE)NULL;
    }

    return (ANSC_HANDLE)pRLog;
}

ANSC_STATUS
CosaRLogInitialize(ANSC_HANDLE hThisObject)
{
    PCOSA_DATAMODEL_RLOG    pDMRLog = (PCOSA_DATAMODEL_RLOG)hThisObject;

    if (!pDMRLog)
        return ANSC_STATUS_FAILURE;

    if (CosaDmlRLog_Init() != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    if (CosaDmlRLog_GetConf(&pDMRLog->RLog) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaRLogRemove(ANSC_HANDLE hThisObject)
{
    PCOSA_DATAMODEL_RLOG    pRLog = (PCOSA_DATAMODEL_RLOG)hThisObject;

    if (!pRLog)
        return ANSC_STATUS_FAILURE;

    if (CosaDmlRLog_Term() != ANSC_STATUS_FAILURE)
        return ANSC_STATUS_FAILURE;

    AnscFreeMemory(pRLog);
    return ANSC_STATUS_SUCCESS;
}
