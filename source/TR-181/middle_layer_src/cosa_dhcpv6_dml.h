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

    module: cosa_apis_dhcpv6.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        09/07/2011    initial revision.

**************************************************************************/


#ifndef  _COSA_APIS_DHCPV6_H
#define  _COSA_APIS_DHCPV6_H

/***********************************************************************

 APIs for Object:

    DHCPv6.

    *  DHCPv6_GetParamBoolValue
    *  DHCPv6_GetParamIntValue
    *  DHCPv6_GetParamUlongValue
    *  DHCPv6_GetParamStringValue

***********************************************************************/
BOOL
DHCPv6_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
DHCPv6_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
DHCPv6_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
DHCPv6_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );
#ifndef FEATURE_RDKB_WAN_MANAGER
/***********************************************************************

 APIs for Object:

    DHCPv6.Client.{i}.

    *  Client3_GetEntryCount
    *  Client3_GetEntry
    *  Client3_AddEntry
    *  Client3_DelEntry
    *  Client3_GetParamBoolValue
    *  Client3_GetParamIntValue
    *  Client3_GetParamUlongValue
    *  Client3_GetParamStringValue
    *  Client3_SetParamBoolValue
    *  Client3_SetParamIntValue
    *  Client3_SetParamUlongValue
    *  Client3_SetParamStringValue
    *  Client3_Validate
    *  Client3_Commit
    *  Client3_Rollback

***********************************************************************/
ULONG
Client3_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
Client3_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
Client3_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
Client3_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
Client3_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Client3_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
Client3_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Client3_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
Client3_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
Client3_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
Client3_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
Client3_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
Client3_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
Client3_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
Client3_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    DHCPv6.Client.{i}.Server.{i}.

    *  Server2_GetEntryCount
    *  Server2_GetEntry
    *  Server2_IsUpdated
    *  Server2_Synchronize
    *  Server2_GetParamBoolValue
    *  Server2_GetParamIntValue
    *  Server2_GetParamUlongValue
    *  Server2_GetParamStringValue

***********************************************************************/
ULONG
Server2_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
Server2_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

BOOL
Server2_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
Server2_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

BOOL
Server2_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Server2_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
Server2_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Server2_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    DHCPv6.Client.{i}.SentOption.{i}.

    *  SentOption1_GetEntryCount
    *  SentOption1_GetEntry
    *  SentOption1_AddEntry
    *  SentOption1_DelEntry
    *  SentOption1_GetParamBoolValue
    *  SentOption1_GetParamIntValue
    *  SentOption1_GetParamUlongValue
    *  SentOption1_GetParamStringValue
    *  SentOption1_SetParamBoolValue
    *  SentOption1_SetParamIntValue
    *  SentOption1_SetParamUlongValue
    *  SentOption1_SetParamStringValue
    *  SentOption1_Validate
    *  SentOption1_Commit
    *  SentOption1_Rollback

***********************************************************************/
ULONG
SentOption1_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
SentOption1_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
SentOption1_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
SentOption1_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
SentOption1_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
SentOption1_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
SentOption1_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
SentOption1_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
SentOption1_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
SentOption1_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
SentOption1_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
SentOption1_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
SentOption1_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
SentOption1_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
SentOption1_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    DHCPv6.Client.{i}.ReceivedOption.{i}.

    *  ReceivedOption_GetEntryCount
    *  ReceivedOption_GetEntry
    *  ReceivedOption_IsUpdated
    *  ReceivedOption_Synchronize
    *  ReceivedOption_GetParamBoolValue
    *  ReceivedOption_GetParamIntValue
    *  ReceivedOption_GetParamUlongValue
    *  ReceivedOption_GetParamStringValue

***********************************************************************/
ULONG
ReceivedOption_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
ReceivedOption_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

BOOL
ReceivedOption_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
ReceivedOption_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

BOOL
ReceivedOption_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
ReceivedOption_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
ReceivedOption_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
ReceivedOption_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );
#endif
/***********************************************************************

 APIs for Object:

    DHCPv6.Server.

    *  Server3_GetParamBoolValue
    *  Server3_GetParamIntValue
    *  Server3_GetParamUlongValue
    *  Server3_GetParamStringValue
    *  Server3_SetParamBoolValue
    *  Server3_SetParamIntValue
    *  Server3_SetParamUlongValue
    *  Server3_SetParamStringValue
    *  Server3_Validate
    *  Server3_Commit
    *  Server3_Rollback

***********************************************************************/
BOOL
Server3_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Server3_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
Server3_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Server3_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
Server3_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
Server3_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
Server3_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
Server3_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
Server3_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
Server3_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
Server3_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    DHCPv6.Server.Pool.{i}.

    *  Pool1_GetEntryCount
    *  Pool1_GetEntry
    *  Pool1_AddEntry
    *  Pool1_DelEntry
    *  Pool1_GetParamBoolValue
    *  Pool1_GetParamUlongValue
    *  Pool1_GetParamStringValue
    *  Pool1_SetParamBoolValue
    *  Pool1_SetParamUlongValue
    *  Pool1_SetParamStringValue
    *  Pool1_Validate
    *  Pool1_Commit
    *  Pool1_Rollback

***********************************************************************/
ULONG
Pool1_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
Pool1_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
Pool1_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
Pool1_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
Pool1_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Pool1_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Pool1_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
Pool1_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
Pool1_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
Pool1_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
Pool1_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
Pool1_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
Pool1_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    DHCPv6.Server.Pool.{i}.Client.{i}.

    *  Client4_GetEntryCount
    *  Client4_GetEntry
    *  Client4_IsUpdated
    *  Client4_Synchronize
    *  Client4_GetParamBoolValue
    *  Client4_GetParamIntValue
    *  Client4_GetParamUlongValue
    *  Client4_GetParamStringValue
    *  Client4_SetParamBoolValue
    *  Client4_SetParamIntValue
    *  Client4_SetParamUlongValue
    *  Client4_SetParamStringValue
    *  Client4_Validate
    *  Client4_Commit
    *  Client4_Rollback

***********************************************************************/
ULONG
Client4_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
Client4_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

BOOL
Client4_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
Client4_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

BOOL
Client4_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Client4_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
Client4_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Client4_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
Client4_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
Client4_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
Client4_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
Client4_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
Client4_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
Client4_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
Client4_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    DHCPv6.Server.Pool.{i}.Client.{i}.IPv6Address.{i}.

    *  IPv6Address2_GetEntryCount
    *  IPv6Address2_GetEntry
    *  IPv6Address2_IsUpdated
    *  IPv6Address2_Synchronize
    *  IPv6Address2_GetParamBoolValue
    *  IPv6Address2_GetParamIntValue
    *  IPv6Address2_GetParamUlongValue
    *  IPv6Address2_GetParamStringValue

***********************************************************************/
ULONG
IPv6Address2_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
IPv6Address2_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

BOOL
IPv6Address2_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
IPv6Address2_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

BOOL
IPv6Address2_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
IPv6Address2_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
IPv6Address2_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
IPv6Address2_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    DHCPv6.Server.Pool.{i}.Client.{i}.IPv6Prefix.{i}.

    *  IPv6Prefix1_GetEntryCount
    *  IPv6Prefix1_GetEntry
    *  IPv6Prefix1_IsUpdated
    *  IPv6Prefix1_Synchronize
    *  IPv6Prefix1_GetParamBoolValue
    *  IPv6Prefix1_GetParamIntValue
    *  IPv6Prefix1_GetParamUlongValue
    *  IPv6Prefix1_GetParamStringValue

***********************************************************************/
ULONG
IPv6Prefix1_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
IPv6Prefix1_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

BOOL
IPv6Prefix1_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
IPv6Prefix1_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

BOOL
IPv6Prefix1_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
IPv6Prefix1_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
IPv6Prefix1_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
IPv6Prefix1_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    DHCPv6.Server.Pool.{i}.Client.{i}.Option.{i}.

    *  Option3_GetEntryCount
    *  Option3_GetEntry
    *  Option3_IsUpdated
    *  Option3_Synchronize
    *  Option3_GetParamBoolValue
    *  Option3_GetParamIntValue
    *  Option3_GetParamUlongValue
    *  Option3_GetParamStringValue

***********************************************************************/
ULONG
Option3_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
Option3_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

BOOL
Option3_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
Option3_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

BOOL
Option3_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Option3_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
Option3_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Option3_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    DHCPv6.Server.Pool.{i}.Option.{i}.

    *  Option4_GetEntryCount
    *  Option4_GetEntry
    *  Option4_AddEntry
    *  Option4_DelEntry
    *  Option4_GetParamBoolValue
    *  Option4_GetParamIntValue
    *  Option4_GetParamUlongValue
    *  Option4_GetParamStringValue
    *  Option4_SetParamBoolValue
    *  Option4_SetParamIntValue
    *  Option4_SetParamUlongValue
    *  Option4_SetParamStringValue
    *  Option4_Validate
    *  Option4_Commit
    *  Option4_Rollback

***********************************************************************/
ULONG
Option4_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
Option4_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
Option4_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
Option4_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
Option4_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Option4_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
Option4_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Option4_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
Option4_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
Option4_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
Option4_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
Option4_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
Option4_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
Option4_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
Option4_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

#endif
