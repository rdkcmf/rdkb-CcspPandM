/*
* If not stated otherwise in this file or this component's LICENSE file the
* following copyright and licenses apply:
*
* Copyright 2021 RDK Management
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

#if defined(DDNS_BROADBANDFORUM)


#ifndef  _COSA_DYNAMICDNS_DML_H
#define  _COSA_DYNAMICDNS_DML_H

#include  "cosa_dynamicdns_apis.h"

/***********************************************************************

 APIs for Object:

    DynamicDNS.

    *  DynamicDNS_GetParamBoolValue
    *  DynamicDNS_GetParamStringValue
    *  DynamicDNS_SetParamBoolValue

***********************************************************************/

BOOL
DynamicDNS_GetParamBoolValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    BOOL*                       pBool
);

ULONG
DynamicDNS_GetParamStringValue
(
    ANSC_HANDLE                     hInsContext,
    char*                           ParamName,
    char*                           pValue,
    ULONG*                          puLong
);

BOOL
DynamicDNS_SetParamBoolValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    BOOL                        bValue
);
/***********************************************************************

 APIs for Object:

    Device.DynamicDNS.Client.{i}


    *  DDNSClient_GetEntryCount
    *  DDNSClient_GetEntry
    *  DDNSClient_AddEntry
    *  DDNSClient_DelEntry
    *  DDNSClient_GetParamBoolValue
    *  DDNSClient_GetParamUlongValue
    *  DDNSClient_GetParamStringValue
    *  DDNSClient_SetParamBoolValue
    *  DDNSClient_SetParamStringValue
    *  DDNSClient_Validate
    *  DDNSClient_Commit
    *  DDNSClient_Rollback

***********************************************************************/

ULONG
DDNSClient_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    );

ANSC_HANDLE
DDNSClient_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
DDNSClient_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
DDNSClient_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
DDNSClient_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
DDNSClient_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    );

ULONG
DDNSClient_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
DDNSClient_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
DDNSClient_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
DDNSClient_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
DDNSClient_Commit
    (
        ANSC_HANDLE                 hInsContext
    );
ULONG
DDNSClient_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    Device.DynamicDNS.Client.{i}.Hostname.{i}

    *  DDNSHostname_GetEntryCount
    *  DDNSHostname_GetEntry
    *  DDNSHostname_AddEntry
    *  DDNSHostname_DelEntry
    *  DDNSHostname_GetParamBoolValue
    *  DDNSHostname_GetParamUlongValue
    *  DDNSHostname_GetParamStringValue
    *  DDNSHostname_SetParamBoolValue
    *  DDNSHostname_SetParamStringValue
    *  DDNSHostname_Validate
    *  DDNSHostname_Commit
    *  DDNSHostname_Rollback

***********************************************************************/

ULONG
DDNSHostname_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    );

ANSC_HANDLE
DDNSHostname_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
DDNSHostname_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
DDNSHostname_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
DDNSHostname_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
DDNSHostname_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    );

ULONG
DDNSHostname_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
DDNSHostname_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
DDNSHostname_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
DDNSHostname_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
DDNSHostname_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
DDNSHostname_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    Device.DynamicDNS.Server.{i}

    *  DDNSServer_GetEntryCount
    *  DDNSServer_GetEntry
    *  DDNSServer_AddEntry
    *  DDNSServer_DelEntry
    *  DDNSServer_GetParamBoolValue
    *  DDNSServer_GetParamUlongValue
    *  DDNSServer_GetParamStringValue
    *  DDNSServer_SetParamBoolValue
    *  DDNSServer_SetParamStringValue
    *  DDNSServer_Validate
    *  DDNSServer_Commit
    *  DDNSServer_Rollback

***********************************************************************/
ULONG
DDNSServer_GetEntryCount
    (
         ANSC_HANDLE                 hInsContext
    );

ANSC_HANDLE
DDNSServer_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
DDNSServer_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
DDNSServer_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
DDNSServer_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

ULONG
DDNSServer_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
DDNSServer_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        pBool
    );

BOOL
DDNSServer_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
DDNSServer_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
DDNSServer_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
DDNSServer_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

#endif  //_COSA_DYNAMICDNS_DML_H


#endif
