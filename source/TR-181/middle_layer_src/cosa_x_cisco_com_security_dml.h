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

    module: cosa_x_cisco_com_security_dml.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    copyright:

        Cisco Systems, Inc.
        All Rights Reserved.

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


#ifndef  _COSA_X_CISCO_COM_SECURITY_DML_H
#define  _COSA_X_CISCO_COM_SECURITY_DML_H

/***********************************************************************

 APIs for Object:

    X_CISCO_COM_Security.


***********************************************************************/
/***********************************************************************

 APIs for Object:

    X_CISCO_COM_Security.Firewall.

    *  X_CISCO_COM_Security_GetParamBoolValue
    *  X_CISCO_COM_Security_GetParamIntValue
    *  X_CISCO_COM_Security_GetParamUlongValue
    *  X_CISCO_COM_Security_GetParamStringValue
    *  X_CISCO_COM_Security_SetParamBoolValue
    *  X_CISCO_COM_Security_SetParamIntValue
    *  X_CISCO_COM_Security_SetParamUlongValue
    *  X_CISCO_COM_Security_SetParamStringValue
    *  X_CISCO_COM_Security_Validate
    *  X_CISCO_COM_Security_Commit
    *  X_CISCO_COM_Security_Rollback

***********************************************************************/
BOOL
X_CISCO_COM_Security_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
X_CISCO_COM_Security_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
X_CISCO_COM_Security_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
X_CISCO_COM_Security_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
X_CISCO_COM_Security_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
X_CISCO_COM_Security_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
X_CISCO_COM_Security_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
X_CISCO_COM_Security_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
X_CISCO_COM_Security_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
X_CISCO_COM_Security_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
X_CISCO_COM_Security_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    X_CISCO_COM_Security.Firewall.

    *  Firewall1_GetParamBoolValue
    *  Firewall1_GetParamIntValue
    *  Firewall1_GetParamUlongValue
    *  Firewall1_GetParamStringValue
    *  Firewall1_SetParamBoolValue
    *  Firewall1_SetParamIntValue
    *  Firewall1_SetParamUlongValue
    *  Firewall1_SetParamStringValue
    *  Firewall1_Validate
    *  Firewall1_Commit
    *  Firewall1_Rollback

***********************************************************************/
BOOL
Firewall1_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Firewall1_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
Firewall1_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Firewall1_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
Firewall1_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
Firewall1_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
Firewall1_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
Firewall1_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
Firewall1_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
Firewall1_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
Firewall1_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    X_CISCO_COM_Security.InternetAccess.

    *  InternetAccess_GetParamBoolValue
    *  InternetAccess_GetParamIntValue
    *  InternetAccess_GetParamUlongValue
    *  InternetAccess_GetParamStringValue

***********************************************************************/
BOOL
InternetAccess_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
InternetAccess_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
InternetAccess_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
InternetAccess_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    X_CISCO_COM_Security.InternetAccess.AccessPolicy.{i}.

    *  AccessPolicy_GetEntryCount
    *  AccessPolicy_GetEntry
    *  AccessPolicy_AddEntry
    *  AccessPolicy_DelEntry
    *  AccessPolicy_GetParamBoolValue
    *  AccessPolicy_GetParamIntValue
    *  AccessPolicy_GetParamUlongValue
    *  AccessPolicy_GetParamStringValue
    *  AccessPolicy_SetParamBoolValue
    *  AccessPolicy_SetParamIntValue
    *  AccessPolicy_SetParamUlongValue
    *  AccessPolicy_SetParamStringValue
    *  AccessPolicy_Validate
    *  AccessPolicy_Commit
    *  AccessPolicy_Rollback

***********************************************************************/
ULONG
AccessPolicy_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
AccessPolicy_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
AccessPolicy_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
AccessPolicy_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
AccessPolicy_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
AccessPolicy_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
AccessPolicy_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
AccessPolicy_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
AccessPolicy_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
AccessPolicy_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
AccessPolicy_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
AccessPolicy_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
AccessPolicy_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
AccessPolicy_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
AccessPolicy_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    X_CISCO_COM_Security.InternetAccess.AccessPolicy.{i}.Schedule.

    *  Schedule_GetParamBoolValue
    *  Schedule_GetParamIntValue
    *  Schedule_GetParamUlongValue
    *  Schedule_GetParamStringValue
    *  Schedule_SetParamBoolValue
    *  Schedule_SetParamIntValue
    *  Schedule_SetParamUlongValue
    *  Schedule_SetParamStringValue
    *  Schedule_Validate
    *  Schedule_Commit
    *  Schedule_Rollback

***********************************************************************/
BOOL
Schedule_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Schedule_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
Schedule_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Schedule_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
Schedule_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
Schedule_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
Schedule_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
Schedule_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
Schedule_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
Schedule_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
Schedule_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );


/***********************************************************************

 APIs for Object:

    X_CISCO_COM_Security.InternetAccess.AccessPolicy.{i}.BlockedURL.{i}.

    *  BlockedURL_GetEntryCount
    *  BlockedURL_GetEntry
    *  BlockedURL_GetParamBoolValue
    *  BlockedURL_GetParamIntValue
    *  BlockedURL_GetParamUlongValue
    *  BlockedURL_GetParamStringValue
    *  BlockedURL_SetParamBoolValue
    *  BlockedURL_SetParamIntValue
    *  BlockedURL_SetParamUlongValue
    *  BlockedURL_SetParamStringValue
    *  BlockedURL_Validate
    *  BlockedURL_Commit
    *  BlockedURL_Rollback

***********************************************************************/
ULONG
BlockedURL_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
BlockedURL_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
BlockedURL_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
BlockedURL_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
BlockedURL_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
BlockedURL_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
BlockedURL_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
BlockedURL_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
BlockedURL_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
BlockedURL_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
BlockedURL_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
BlockedURL_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
BlockedURL_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
BlockedURL_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
BlockedURL_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    X_CISCO_COM_Security.InternetAccess.AccessPolicy.{i}.BlockedKeyword.{i}.

    *  BlockedKeyword_GetEntryCount
    *  BlockedKeyword_GetEntry
    *  BlockedKeyword_GetParamBoolValue
    *  BlockedKeyword_GetParamIntValue
    *  BlockedKeyword_GetParamUlongValue
    *  BlockedKeyword_GetParamStringValue
    *  BlockedKeyword_SetParamBoolValue
    *  BlockedKeyword_SetParamIntValue
    *  BlockedKeyword_SetParamUlongValue
    *  BlockedKeyword_SetParamStringValue
    *  BlockedKeyword_Validate
    *  BlockedKeyword_Commit
    *  BlockedKeyword_Rollback

***********************************************************************/
ULONG
BlockedKeyword_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
BlockedKeyword_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
BlockedKeyword_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
BlockedKeyword_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
BlockedKeyword_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
BlockedKeyword_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
BlockedKeyword_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
BlockedKeyword_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
BlockedKeyword_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
BlockedKeyword_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
BlockedKeyword_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
BlockedKeyword_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
BlockedKeyword_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
BlockedKeyword_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
BlockedKeyword_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    X_CISCO_COM_Security.InternetAccess.AccessPolicy.{i}.BlockedApplication.{i}.

    *  BlockedApplication_GetEntryCount
    *  BlockedApplication_GetEntry
    *  BlockedApplication_GetParamBoolValue
    *  BlockedApplication_GetParamIntValue
    *  BlockedApplication_GetParamUlongValue
    *  BlockedApplication_GetParamStringValue
    *  BlockedApplication_SetParamBoolValue
    *  BlockedApplication_SetParamIntValue
    *  BlockedApplication_SetParamUlongValue
    *  BlockedApplication_SetParamStringValue
    *  BlockedApplication_Validate
    *  BlockedApplication_Commit
    *  BlockedApplication_Rollback

***********************************************************************/
ULONG
BlockedApplication_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
BlockedApplication_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
BlockedApplication_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
BlockedApplication_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
BlockedApplication_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
BlockedApplication_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
BlockedApplication_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
BlockedApplication_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
BlockedApplication_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
BlockedApplication_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
BlockedApplication_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
BlockedApplication_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
BlockedApplication_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
BlockedApplication_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
BlockedApplication_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    X_CISCO_COM_Security.InternetAccess.LogEntry.{i}.

    *  LogEntry_GetEntryCount
    *  LogEntry_GetEntry
    *  LogEntry_IsUpdated
    *  LogEntry_Synchronize
    *  LogEntry_GetParamBoolValue
    *  LogEntry_GetParamIntValue
    *  LogEntry_GetParamUlongValue
    *  LogEntry_GetParamStringValue

***********************************************************************/
ULONG
LogEntry_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
LogEntry_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

BOOL
LogEntry_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
LogEntry_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

BOOL
LogEntry_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
LogEntry_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
LogEntry_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
LogEntry_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

#endif
