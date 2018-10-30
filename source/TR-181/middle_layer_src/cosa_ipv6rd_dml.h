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

/**************************************************************************

    module: cosa_ipv6rd_dml.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        leichen2@cisco.com

    -------------------------------------------------------------------

    revision:

        05/08/2011    initial revision.

**************************************************************************/

#ifndef _COSA_IPV6RD_DML_H_
#define _COSA_IPV6RD_DML_H_

#include "cosa_apis.h"
#include "plugin_main_apis.h"

BOOL
IPv6rd_GetParamBoolValue(
        ANSC_HANDLE hInsContext, 
        char *ParamName, 
        BOOL *pBool);

BOOL
IPv6rd_GetParamUlongValue(
        ANSC_HANDLE hInsContext, 
        char *ParamName, 
        ULONG *pUlong);

BOOL
IPv6rd_SetParamBoolValue(
        ANSC_HANDLE hInsContext,
        char        *ParamName,
        BOOL        bValue);

BOOL
IPv6rd_SetParamUlongValue(
        ANSC_HANDLE hInsContext,
        char        *ParamName,
        ULONG       ulValue);
     
BOOL
IPv6rd_Validate(
        ANSC_HANDLE hInsContext,
        char        *pReturnParamName,
        ULONG       *puLength);

ULONG
IPv6rd_Commit(
        ANSC_HANDLE hInsContext);

ULONG
IPv6rd_Rollback(
        ANSC_HANDLE hInsContext);

ULONG
IPv6rdIF_GetEntryCount(
        ANSC_HANDLE hInsContext);

ANSC_HANDLE
IPv6rdIF_GetEntry(
        ANSC_HANDLE hInsContext,
        ULONG       nIndex,
        ULONG       *pInsNumber);

ANSC_HANDLE
IPv6rdIF_AddEntry(
        ANSC_HANDLE hInsContext,
        ULONG       *pInsNumber);

ULONG
IPv6rdIF_DelEntry(
        ANSC_HANDLE hInsContext,
        ULONG       hInstance);

BOOL
IPv6rdIF_GetParamBoolValue(
        ANSC_HANDLE hInsContext, 
        char *ParamName, 
        BOOL *pBool);

ULONG
IPv6rdIF_GetParamStringValue(
        ANSC_HANDLE     hInsContext,
        char            *ParamName,
        char            *pValue,
        ULONG           *pSize);

BOOL
IPv6rdIF_GetParamUlongValue(
        ANSC_HANDLE hInsContext, 
        char *ParamName, 
        ULONG *pUlong);

BOOL
IPv6rdIF_SetParamBoolValue(
        ANSC_HANDLE hInsContext,
        char        *ParamName,
        BOOL        bValue);

BOOL
IPv6rdIF_SetParamStringValue(
        ANSC_HANDLE hInsContext,
        char        *ParamName,
        char        *pString);

BOOL
IPv6rdIF_SetParamUlongValue(
        ANSC_HANDLE hInsContext,
        char        *ParamName,
        ULONG       bValue);

BOOL
IPv6rdIF_Validate(
        ANSC_HANDLE hInsContext,
        char        *pReturnParamName,
        ULONG       *puLength);

ULONG
IPv6rdIF_Commit(
        ANSC_HANDLE hInsContext);

ULONG
IPv6rdIF_Rollback(
        ANSC_HANDLE hInsContext);

#endif /* _COSA_IPV6RD_DML_H_ */
