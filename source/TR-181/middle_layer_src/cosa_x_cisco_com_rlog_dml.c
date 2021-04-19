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

#include "cosa_x_cisco_com_rlog_dml.h"

BOOL
RLog_GetParamBoolValue(ANSC_HANDLE hInsContext, char *ParamName, BOOL *pBool)
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_RLOG pDMRLog    = (PCOSA_DATAMODEL_RLOG)g_pCosaBEManager->hRLog;

    if (!pDMRLog || !ParamName || !pBool)
        return FALSE;

    if (strcmp(ParamName, "Enable") == 0)
    {
        *pBool = pDMRLog->RLog.Enable;
        return TRUE;
    }

    return FALSE;
}

BOOL
RLog_GetParamUlongValue(ANSC_HANDLE hInsContext, char *ParamName, ULONG *pUlong)
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_RLOG pDMRLog    = (PCOSA_DATAMODEL_RLOG)g_pCosaBEManager->hRLog;

    if (!pDMRLog || !ParamName || !pUlong)
        return FALSE;

    if (strcmp(ParamName, "Port") == 0)
    {
        *pUlong = pDMRLog->RLog.Port;
        return TRUE;
    }
    else if (strcmp(ParamName, "Protocol") == 0)
    {
        *pUlong = pDMRLog->RLog.Protocol;
        return TRUE;
    }

    return FALSE;
}

ULONG
RLog_GetParamStringValue(ANSC_HANDLE hInsContext, char *ParamName, char *pStr, ULONG *pSize)
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_RLOG pDMRLog    = (PCOSA_DATAMODEL_RLOG)g_pCosaBEManager->hRLog;

    if (!pDMRLog || !ParamName || !pStr || !pSize)
        return -1;

    if (strcmp(ParamName, "Host") == 0)
    {
        snprintf(pStr, *pSize, "%s", pDMRLog->RLog.Host);
        return 0;
    }
    else if (strcmp(ParamName, "Patterns") == 0)
    {
        snprintf(pStr, *pSize, "%s", pDMRLog->RLog.Patterns);
        return 0;
    }

    return -1;
}

BOOL
RLog_SetParamBoolValue(ANSC_HANDLE hInsContext, char *ParamName, BOOL bValue)
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_RLOG pDMRLog    = (PCOSA_DATAMODEL_RLOG)g_pCosaBEManager->hRLog;

    if (!pDMRLog || !ParamName)
        return FALSE;

    if (strcmp(ParamName, "Enable") == 0)
    {
        pDMRLog->RLog.Enable = bValue;
        return TRUE;
    }

    return FALSE;
}

BOOL
RLog_SetParamUlongValue(ANSC_HANDLE hInsContext, char *ParamName, ULONG ulValue)
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_RLOG pDMRLog    = (PCOSA_DATAMODEL_RLOG)g_pCosaBEManager->hRLog;

    if (!pDMRLog || !ParamName)
        return FALSE;

    if (strcmp(ParamName, "Port") == 0)
    {
        pDMRLog->RLog.Port = ulValue;
        return TRUE;
    } else if (strcmp(ParamName, "Protocol") == 0)
    {
        pDMRLog->RLog.Protocol = ulValue;
        return TRUE;
    }

    return FALSE;
}

BOOL
RLog_SetParamStringValue(ANSC_HANDLE hInsContext, char *ParamName, char *sValue)
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_RLOG pDMRLog    = (PCOSA_DATAMODEL_RLOG)g_pCosaBEManager->hRLog;

    if (!pDMRLog || !ParamName || !sValue)
        return FALSE;

    if (strcmp(ParamName, "Host") == 0)
    {
        snprintf(pDMRLog->RLog.Host, sizeof(pDMRLog->RLog.Host), "%s", sValue);
        return TRUE;
    }
    else if (strcmp(ParamName, "Patterns") == 0)
    {
        snprintf(pDMRLog->RLog.Patterns, sizeof(pDMRLog->RLog.Patterns), "%s", sValue);
        return TRUE;
    }

    return FALSE;
}

BOOL
RLog_Validate(ANSC_HANDLE hInsContext, char *pFaultName, ULONG *pLen)
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pFaultName);
    UNREFERENCED_PARAMETER(pLen);
    PCOSA_DATAMODEL_RLOG pDMRLog    = (PCOSA_DATAMODEL_RLOG)g_pCosaBEManager->hRLog;

    if (!pDMRLog)
        return FALSE;

    if (CosaDmlRLog_Validate(&pDMRLog->RLog) != ANSC_STATUS_SUCCESS)
        return FALSE;

    return TRUE;
}

ULONG
RLog_Commit(ANSC_HANDLE hInsContext)
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_RLOG pDMRLog    = (PCOSA_DATAMODEL_RLOG)g_pCosaBEManager->hRLog;

    if (!pDMRLog)
        return FALSE;

    if (CosaDmlRLog_SetConf(&pDMRLog->RLog) != ANSC_STATUS_SUCCESS)
    {
        CosaDmlRLog_GetConf(&pDMRLog->RLog);
        return FALSE;
    }

    return TRUE;

}

ULONG
RLog_Rollback(ANSC_HANDLE hInsContext)
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_RLOG pDMRLog    = (PCOSA_DATAMODEL_RLOG)g_pCosaBEManager->hRLog;

    if (!pDMRLog)
        return FALSE;

    if (CosaDmlRLog_GetConf(&pDMRLog->RLog) != ANSC_STATUS_SUCCESS)
        return FALSE;

    return TRUE;
}
