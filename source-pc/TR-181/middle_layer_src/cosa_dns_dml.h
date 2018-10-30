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

    module: cosa_dns_dml.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        01/14/2011    initial revision.

**************************************************************************/


#ifndef  _COSA_DNS_DML_H
#define  _COSA_DNS_DML_H

/***********************************************************************

 APIs for Object:

    DNS.


***********************************************************************/

/***********************************************************************

 APIs for Object:

    DNS.Client.

    *  Client1_GetParamBoolValue
    *  Client1_GetParamIntValue
    *  Client1_GetParamUlongValue
    *  Client1_GetParamStringValue
    *  Client1_SetParamBoolValue
    *  Client1_SetParamIntValue
    *  Client1_SetParamUlongValue
    *  Client1_SetParamStringValue
    *  Client1_Validate
    *  Client1_Commit
    *  Client1_Rollback

***********************************************************************/
BOOL
Client1_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Client1_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
Client1_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Client1_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
Client1_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
Client1_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
Client1_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
Client1_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
Client1_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
Client1_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
Client1_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    DNS.Client.Server.{i}.

    *  Server1_GetEntryCount
    *  Server1_GetEntry
    *  Server1_AddEntry
    *  Server1_DelEntry
    *  Server1_IsUpdated
    *  Server1_Synchronize
    *  Server1_GetParamBoolValue
    *  Server1_GetParamIntValue
    *  Server1_GetParamUlongValue
    *  Server1_GetParamStringValue
    *  Server1_SetParamBoolValue
    *  Server1_SetParamIntValue
    *  Server1_SetParamUlongValue
    *  Server1_SetParamStringValue
    *  Server1_Validate
    *  Server1_Commit
    *  Server1_Rollback

***********************************************************************/
ULONG
Server1_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
Server1_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
Server1_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
Server1_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
Server1_IsUpdated
    (
        ANSC_HANDLE                  hInsContext
    );

ULONG
Server1_Synchronize
    (
        ANSC_HANDLE                  hInsContext
    );

BOOL
Server1_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Server1_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
Server1_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Server1_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
Server1_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
Server1_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
Server1_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
Server1_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
Server1_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
Server1_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
Server1_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );


/***********************************************************************

 APIs for Object:

    DNS.Relay.

    *  Relay_GetParamBoolValue
    *  Relay_GetParamIntValue
    *  Relay_GetParamUlongValue
    *  Relay_GetParamStringValue
    *  Relay_SetParamBoolValue
    *  Relay_SetParamIntValue
    *  Relay_SetParamUlongValue
    *  Relay_SetParamStringValue
    *  Relay_Validate
    *  Relay_Commit
    *  Relay_Rollback

***********************************************************************/
BOOL
Relay_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Relay_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
Relay_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Relay_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
Relay_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
Relay_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
Relay_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
Relay_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
Relay_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
Relay_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
Relay_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    DNS.Relay.Forwarding.{i}.

    *  Forwarding_GetEntryCount
    *  Forwarding_GetEntry
    *  Forwarding_AddEntry
    *  Forwarding_DelEntry
    *  Forwarding_IsUpdated
    *  Forwarding_Synchrize
    *  Forwarding_GetParamBoolValue
    *  Forwarding_GetParamIntValue
    *  Forwarding_GetParamUlongValue
    *  Forwarding_GetParamStringValue
    *  Forwarding_SetParamBoolValue
    *  Forwarding_SetParamIntValue
    *  Forwarding_SetParamUlongValue
    *  Forwarding_SetParamStringValue
    *  Forwarding_Validate
    *  Forwarding_Commit
    *  Forwarding_Rollback

***********************************************************************/
ULONG
Forwarding_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
Forwarding_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
Forwarding_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
Forwarding_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
Forwarding_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
Forwarding_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

BOOL
Forwarding_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Forwarding_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
Forwarding_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Forwarding_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
Forwarding_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
Forwarding_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
Forwarding_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
Forwarding_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
Forwarding_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
Forwarding_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
Forwarding_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    DNS.Diagnostics.


***********************************************************************/
/***********************************************************************

 APIs for Object:

    DNS.Diagnostics.NSLookupDiagnostics.

    *  NSLookupDiagnostics_GetParamBoolValue
    *  NSLookupDiagnostics_GetParamIntValue
    *  NSLookupDiagnostics_GetParamUlongValue
    *  NSLookupDiagnostics_GetParamStringValue
    *  NSLookupDiagnostics_SetParamBoolValue
    *  NSLookupDiagnostics_SetParamIntValue
    *  NSLookupDiagnostics_SetParamUlongValue
    *  NSLookupDiagnostics_SetParamStringValue
    *  NSLookupDiagnostics_Validate
    *  NSLookupDiagnostics_Commit
    *  NSLookupDiagnostics_Rollback

***********************************************************************/
BOOL
NSLookupDiagnostics_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
NSLookupDiagnostics_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
NSLookupDiagnostics_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
NSLookupDiagnostics_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
NSLookupDiagnostics_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
NSLookupDiagnostics_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
NSLookupDiagnostics_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
NSLookupDiagnostics_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
NSLookupDiagnostics_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
NSLookupDiagnostics_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
NSLookupDiagnostics_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    DNS.Diagnostics.NSLookupDiagnostics.Result.{i}.

    *  Result_GetEntryCount
    *  Result_GetEntry
    *  Result_IsUpdated
    *  Result_Synchronize
    *  Result_GetParamBoolValue
    *  Result_GetParamIntValue
    *  Result_GetParamUlongValue
    *  Result_GetParamStringValue

***********************************************************************/
ULONG
Result_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
Result_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

BOOL
Result_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
Result_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

BOOL
Result_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Result_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
Result_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Result_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

#endif
