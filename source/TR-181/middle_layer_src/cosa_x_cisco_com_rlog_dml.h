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

#ifndef _COSA_RLOG_DML_H
#define _COSA_RLOG_DML_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "cosa_x_cisco_com_rlog_internal.h"
#include "cosa_x_cisco_com_rlog_apis.h"

BOOL
RLog_GetParamBoolValue(ANSC_HANDLE hInsContext, char *ParamName, BOOL *pBool);

BOOL
RLog_GetParamUlongValue(ANSC_HANDLE hInsContext, char *ParamName, ULONG *pUlong);

ULONG
RLog_GetParamStringValue(ANSC_HANDLE hInsContext, char *ParamName, char *pStr, ULONG *pSize);

BOOL
RLog_SetParamBoolValue(ANSC_HANDLE hInsContext, char *ParamName, BOOL bValue);

BOOL
RLog_SetParamUlongValue(ANSC_HANDLE hInsContext, char *ParamName, ULONG ulValue);

BOOL
RLog_SetParamStringValue(ANSC_HANDLE hInsContext, char *ParamName, char * sValue);

BOOL
RLog_Validate(ANSC_HANDLE hInsContext, char *pFaultName, ULONG *pLen);

ULONG
RLog_Commit(ANSC_HANDLE hInsContext);

ULONG
RLog_Rollback(ANSC_HANDLE hInsContext);

#endif /* _COSA_RLOG_DML_H */
