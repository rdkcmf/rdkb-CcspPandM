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

    module: cosa_dhcpv4_dml.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        01/18/2011    initial revision.

**************************************************************************/


#ifndef  _COSA_DHCPV4_DML_H
#define  _COSA_DHCPV4_DML_H

#include "cosa_apis.h"

/***********************************************************************

 APIs for Object:

    DHCPv4.

    *  DHCPv4_GetParamBoolValue
    *  DHCPv4_GetParamIntValue
    *  DHCPv4_GetParamUlongValue
    *  DHCPv4_GetParamStringValue

***********************************************************************/
BOOL
DHCPv4_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
DHCPv4_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
DHCPv4_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
DHCPv4_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );
#ifndef FEATURE_RDKB_WAN_MANAGER
/***********************************************************************

 APIs for Object:

    DHCPv4.Client.{i}.

    *  Client_GetEntryCount
    *  Client_GetEntry
    *  Client_AddEntry
    *  Client_DelEntry
    *  Client_GetParamBoolValue
    *  Client_GetParamIntValue
    *  Client_GetParamUlongValue
    *  Client_GetParamStringValue
    *  Client_SetParamBoolValue
    *  Client_SetParamIntValue
    *  Client_SetParamUlongValue
    *  Client_SetParamStringValue
    *  Client_Validate
    *  Client_Commit
    *  Client_Rollback

***********************************************************************/
ULONG
Client_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
Client_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
Client_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
Client_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
Client_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Client_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
Client_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Client_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
Client_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
Client_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
Client_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
Client_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
Client_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
Client_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
Client_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    DHCPv4.Client.{i}.SentOption.{i}.

    *  SentOption_GetEntryCount
    *  SentOption_GetEntry
    *  SentOption_AddEntry
    *  SentOption_DelEntry
    *  SentOption_GetParamBoolValue
    *  SentOption_GetParamIntValue
    *  SentOption_GetParamUlongValue
    *  SentOption_GetParamStringValue
    *  SentOption_SetParamBoolValue
    *  SentOption_SetParamIntValue
    *  SentOption_SetParamUlongValue
    *  SentOption_SetParamStringValue
    *  SentOption_Validate
    *  SentOption_Commit
    *  SentOption_Rollback

***********************************************************************/
ULONG
SentOption_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
SentOption_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
SentOption_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
SentOption_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
SentOption_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
SentOption_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
SentOption_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
SentOption_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
SentOption_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
SentOption_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
SentOption_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
SentOption_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
SentOption_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
SentOption_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
SentOption_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    DHCPv4.Client.{i}.ReqOption.{i}.

    *  ReqOption_GetEntryCount
    *  ReqOption_GetEntry
    *  ReqOption_AddEntry
    *  ReqOption_DelEntry
    *  ReqOption_GetParamBoolValue
    *  ReqOption_GetParamIntValue
    *  ReqOption_GetParamUlongValue
    *  ReqOption_GetParamStringValue
    *  ReqOption_SetParamBoolValue
    *  ReqOption_SetParamIntValue
    *  ReqOption_SetParamUlongValue
    *  ReqOption_SetParamStringValue
    *  ReqOption_Validate
    *  ReqOption_Commit
    *  ReqOption_Rollback

***********************************************************************/
ULONG
ReqOption_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
ReqOption_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
ReqOption_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
ReqOption_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
ReqOption_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
ReqOption_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
ReqOption_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
ReqOption_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
ReqOption_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
ReqOption_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
ReqOption_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
ReqOption_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
ReqOption_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
ReqOption_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
ReqOption_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );
#endif
/***********************************************************************

 APIs for Object:

    DHCPv4.Server.

    *  Server_GetParamBoolValue
    *  Server_GetParamIntValue
    *  Server_GetParamUlongValue
    *  Server_GetParamStringValue
    *  Server_SetParamBoolValue
    *  Server_SetParamIntValue
    *  Server_SetParamUlongValue
    *  Server_SetParamStringValue
    *  Server_Validate
    *  Server_Commit
    *  Server_Rollback

***********************************************************************/
BOOL
Server_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Server_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
Server_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Server_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
Server_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
Server_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
Server_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
Server_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
Server_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
Server_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
Server_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );


/***********************************************************************

 APIs for Object:

    DHCPv4.Server.X_COM_CISCO_StaticAddress.{i}.

    *  X_COM_CISCO_StaticAddress_GetEntryCount
    *  X_COM_CISCO_StaticAddress_GetEntry
    *  X_COM_CISCO_StaticAddress_AddEntry
    *  X_COM_CISCO_StaticAddress_DelEntry
    *  X_COM_CISCO_StaticAddress_GetParamBoolValue
    *  X_COM_CISCO_StaticAddress_GetParamIntValue
    *  X_COM_CISCO_StaticAddress_GetParamUlongValue
    *  X_COM_CISCO_StaticAddress_GetParamStringValue
    *  X_COM_CISCO_StaticAddress_SetParamBoolValue
    *  X_COM_CISCO_StaticAddress_SetParamIntValue
    *  X_COM_CISCO_StaticAddress_SetParamUlongValue
    *  X_COM_CISCO_StaticAddress_SetParamStringValue
    *  X_COM_CISCO_StaticAddress_Validate
    *  X_COM_CISCO_StaticAddress_Commit
    *  X_COM_CISCO_StaticAddress_Rollback

***********************************************************************/
ULONG
X_COM_CISCO_StaticAddress_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
X_COM_CISCO_StaticAddress_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
X_COM_CISCO_StaticAddress_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
X_COM_CISCO_StaticAddress_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
X_COM_CISCO_StaticAddress_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
X_COM_CISCO_StaticAddress_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
X_COM_CISCO_StaticAddress_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
X_COM_CISCO_StaticAddress_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
X_COM_CISCO_StaticAddress_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
X_COM_CISCO_StaticAddress_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
X_COM_CISCO_StaticAddress_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
X_COM_CISCO_StaticAddress_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
X_COM_CISCO_StaticAddress_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
X_COM_CISCO_StaticAddress_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
X_COM_CISCO_StaticAddress_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    DHCPv4.Server.Pool.{i}.

    *  Pool_GetEntryCount
    *  Pool_GetEntry
    *  Pool_AddEntry
    *  Pool_DelEntry
    *  Pool_GetParamBoolValue
    *  Pool_GetParamIntValue
    *  Pool_GetParamUlongValue
    *  Pool_GetParamStringValue
    *  Pool_SetParamBoolValue
    *  Pool_SetParamIntValue
    *  Pool_SetParamUlongValue
    *  Pool_SetParamStringValue
    *  Pool_Validate
    *  Pool_Commit
    *  Pool_Rollback

***********************************************************************/
ULONG
Pool_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
Pool_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
Pool_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
Pool_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
Pool_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Pool_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
Pool_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Pool_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
Pool_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
Pool_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
Pool_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
Pool_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
Pool_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
Pool_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
Pool_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    DHCPv4.Server.Pool.{i}.StaticAddress.{i}.

    *  StaticAddress_GetEntryCount
    *  StaticAddress_GetEntry
    *  StaticAddress_AddEntry
    *  StaticAddress_IsUpdated
    *  StaticAddress_Synchronize
    *  StaticAddress_DelEntry
    *  StaticAddress_GetParamBoolValue
    *  StaticAddress_GetParamIntValue
    *  StaticAddress_GetParamUlongValue
    *  StaticAddress_GetParamStringValue
    *  StaticAddress_SetParamBoolValue
    *  StaticAddress_SetParamIntValue
    *  StaticAddress_SetParamUlongValue
    *  StaticAddress_SetParamStringValue
    *  StaticAddress_Validate
    *  StaticAddress_Commit
    *  StaticAddress_Rollback

***********************************************************************/
ULONG
StaticAddress_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
StaticAddress_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

BOOL
StaticAddress_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
StaticAddress_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );


ANSC_HANDLE
StaticAddress_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
StaticAddress_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
StaticAddress_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
StaticAddress_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
StaticAddress_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
StaticAddress_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
StaticAddress_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
StaticAddress_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
StaticAddress_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
StaticAddress_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
StaticAddress_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
StaticAddress_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
StaticAddress_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );


/***********************************************************************

 APIs for Object:

    DHCPv4.Server.Pool.{i}.Option.{i}.

    *  Option1_GetEntryCount
    *  Option1_GetEntry
    *  Option1_AddEntry
    *  Option1_DelEntry
    *  Option1_GetParamBoolValue
    *  Option1_GetParamIntValue
    *  Option1_GetParamUlongValue
    *  Option1_GetParamStringValue
    *  Option1_SetParamBoolValue
    *  Option1_SetParamIntValue
    *  Option1_SetParamUlongValue
    *  Option1_SetParamStringValue
    *  Option1_Validate
    *  Option1_Commit
    *  Option1_Rollback

***********************************************************************/
ULONG
Option1_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
Option1_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
Option1_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
Option1_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
Option1_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Option1_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
Option1_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Option1_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
Option1_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
Option1_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
Option1_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
Option1_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
Option1_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
Option1_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
Option1_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );


/***********************************************************************

 APIs for Object:

    DHCPv4.Server.Pool.{i}.Client.{i}.

    *  Client2_GetEntryCount
    *  Client2_GetEntry
    *  Client2_IsUpdated
    *  Client2_Synchronize
    *  Client2_GetParamBoolValue
    *  Client2_GetParamIntValue
    *  Client2_GetParamUlongValue
    *  Client2_GetParamStringValue
    *  Client2_SetParamBoolValue
    *  Client2_SetParamIntValue
    *  Client2_SetParamUlongValue
    *  Client2_SetParamStringValue
    *  Client2_Validate
    *  Client2_Commit
    *  Client2_Rollback

***********************************************************************/
ULONG
Client2_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
Client2_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

BOOL
Client2_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
Client2_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

BOOL
Client2_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Client2_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
Client2_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Client2_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
Client2_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
Client2_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
Client2_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
Client2_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
Client2_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
Client2_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
Client2_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    DHCPv4.Server.Pool.{i}.Client.{i}.IPv4Address.{i}.

    *  IPv4Address2_GetEntryCount
    *  IPv4Address2_GetEntry
    *  IPv4Address2_IsUpdated
    *  IPv4Address2_Synchronize
    *  IPv4Address2_GetParamBoolValue
    *  IPv4Address2_GetParamIntValue
    *  IPv4Address2_GetParamUlongValue
    *  IPv4Address2_GetParamStringValue

***********************************************************************/
ULONG
IPv4Address2_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
IPv4Address2_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

BOOL
IPv4Address2_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
IPv4Address2_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

BOOL
IPv4Address2_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
IPv4Address2_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
IPv4Address2_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
IPv4Address2_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    DHCPv4.Server.Pool.{i}.Client.{i}.Option.{i}.

    *  Option2_GetEntryCount
    *  Option2_GetEntry
    *  Option2_IsUpdated
    *  Option2_Synchronize
    *  Option2_GetParamBoolValue
    *  Option2_GetParamIntValue
    *  Option2_GetParamUlongValue
    *  Option2_GetParamStringValue

***********************************************************************/
ULONG
Option2_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
Option2_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

BOOL
Option2_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
Option2_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

BOOL
Option2_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Option2_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
Option2_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Option2_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

#endif
