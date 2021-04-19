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
/****************************************************************************
  Copyright 2016-2018 Intel Corporation

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
******************************************************************************/

#ifdef INTEL_GRE_HOTSPOT

#ifndef  _COSA_GRE_DML_H
#define  _COSA_GRE_DML_H

#include "cosa_gre_apis.h"

/***********************************************************************

 APIs for Object:

    GRE

    *  GRE_GetParamUlongValue

***********************************************************************/

BOOL
GRE_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    );

/***********************************************************************

 APIs for Object:

    GRE.Tunnel.{i}.

    *  GRETunnel_GetEntryCount
    *  GRETunnel_GetEntry
    *  GRETunnel_DelEntry
    *  GRETunnel_GetParamBoolValue
    *  GRETunnel_GetParamIntValue
    *  GRETunnel_GetParamUlongValue
    *  GRETunnel_GetParamStringValue
    *  GRETunnel_SetParamBoolValue
    *  GRETunnel_SetParamIntValue
    *  GRETunnel_SetParamUlongValue
    *  GRETunnel_SetParamStringValue
    *  GRETunnel_Validate
    *  GRETunnel_Commit
    *  GRETunnel_Rollback

***********************************************************************/
ULONG
GRETunnel_GetEntryCount
    (
        ANSC_HANDLE hInsContext
    );

ANSC_HANDLE
GRETunnel_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
GRETunnel_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
GRETunnel_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
GRETunnel_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
GRETunnel_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
GRETunnel_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
GRETunnel_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
GRETunnel_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
GRETunnel_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
GRETunnel_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    );

BOOL
GRETunnel_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    );

BOOL
GRETunnel_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
GRETunnel_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
GRETunnel_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

BOOL
GRETunnelStats_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/***********************************************************************

 APIs for Object:

    GRE.Tunnel.{i}.Interface.{i}

    *  GRETunnelIf_GetEntryCount
    *  GRETunnelIf_GetEntry
    *  GRETunnelIf_DelEntry
    *  GRETunnelIf_GetParamBoolValue
    *  GRETunnelIf_GetParamIntValue
    *  GRETunnelIf_GetParamUlongValue
    *  GRETunnelIf_GetParamStringValue
    *  GRETunnelIf_SetParamBoolValue
    *  GRETunnelIf_SetParamIntValue
    *  GRETunnelIf_SetParamUlongValue
    *  GRETunnelIf_SetParamStringValue
    *  GRETunnelIf_Validate
    *  GRETunnelIf_Commit
    *  GRETunnelIf_Rollback

***********************************************************************/

ULONG
GRETunnelIf_GetEntryCount
    (
        ANSC_HANDLE hInsContext
    );

ANSC_HANDLE
GRETunnelIf_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
GRETunnelIf_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
GRETunnelIf_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
GRETunnelIf_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
GRETunnelIf_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
GRETunnelIf_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
GRETunnelIf_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
GRETunnelIf_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
GRETunnelIf_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
GRETunnelIf_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
GRETunnelIf_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    );

BOOL
GRETunnelIf_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
GRETunnelIf_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
GRETunnelIf_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

BOOL
GRETunnelIfStats_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

#endif

#endif
