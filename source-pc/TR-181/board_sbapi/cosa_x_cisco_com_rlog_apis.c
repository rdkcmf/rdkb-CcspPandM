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

#include "cosa_x_cisco_com_rlog_apis.h"

static COSA_DML_RLOG g_RLog;

ANSC_STATUS 
CosaDmlRLog_Init(void)
{
    g_RLog.Enable       = FALSE;
    snprintf(g_RLog.Host, sizeof(g_RLog.Host), "192.168.1.121");
    g_RLog.Port         = 514;
    g_RLog.Protocol     = COSA_DML_RLOG_PROTO_UDP;
    snprintf(g_RLog.Patterns, sizeof(g_RLog.Patterns), "*.*");

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS 
CosaDmlRLog_Term()
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS 
CosaDmlRLog_Validate(PCOSA_DML_RLOG pRLog)
{
    if (!pRLog)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS 
CosaDmlRLog_SetConf(PCOSA_DML_RLOG pRLog)
{
    if (!pRLog)
        return ANSC_STATUS_FAILURE;

    memcpy(&g_RLog, pRLog, sizeof(COSA_DML_RLOG));
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS 
CosaDmlRLog_GetConf(PCOSA_DML_RLOG pRLog)
{
    if (!pRLog)
        return ANSC_STATUS_FAILURE;

    memcpy(pRLog, &g_RLog, sizeof(COSA_DML_RLOG));
    return ANSC_STATUS_SUCCESS;
}
