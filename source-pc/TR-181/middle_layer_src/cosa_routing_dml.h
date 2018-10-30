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

    module: cosa_routing_dml.h

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


#ifndef  _COSA_ROUTING_DML_H
#define  _COSA_ROUTING_DML_H

/***********************************************************************

 APIs for Object:

    Routing.

    *  Routing_GetParamBoolValue
    *  Routing_GetParamIntValue
    *  Routing_GetParamUlongValue
    *  Routing_GetParamStringValue

***********************************************************************/
BOOL
Routing_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Routing_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
Routing_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Routing_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    Routing.Router.{i}.

    *  Router_GetEntryCount
    *  Router_GetEntry
    *  Router_AddEntry
    *  Router_DelEntry
    *  Router_GetParamBoolValue
    *  Router_GetParamIntValue
    *  Router_GetParamUlongValue
    *  Router_GetParamStringValue
    *  Router_SetParamBoolValue
    *  Router_SetParamIntValue
    *  Router_SetParamUlongValue
    *  Router_SetParamStringValue
    *  Router_Validate
    *  Router_Commit
    *  Router_Rollback

***********************************************************************/
ULONG
Router_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
Router_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
Router_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
Router_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
Router_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Router_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
Router_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Router_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
Router_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
Router_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
Router_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
Router_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
Router_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
Router_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
Router_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    Routing.Router.{i}.IPv4Forwarding.{i}.

    *  IPv4Forwarding_GetEntryCount
    *  IPv4Forwarding_GetEntry
    *  IPv4Forwarding_AddEntry
    *  IPv4Forwarding_DelEntry
    *  IPv4Forwarding_GetParamBoolValue
    *  IPv4Forwarding_GetParamIntValue
    *  IPv4Forwarding_GetParamUlongValue
    *  IPv4Forwarding_GetParamStringValue
    *  IPv4Forwarding_SetParamBoolValue
    *  IPv4Forwarding_SetParamIntValue
    *  IPv4Forwarding_SetParamUlongValue
    *  IPv4Forwarding_SetParamStringValue
    *  IPv4Forwarding_Validate
    *  IPv4Forwarding_Commit
    *  IPv4Forwarding_Rollback

***********************************************************************/
ULONG
IPv4Forwarding_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
IPv4Forwarding_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
IPv4Forwarding_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
IPv4Forwarding_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
IPv4Forwarding_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
IPv4Forwarding_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
IPv4Forwarding_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
IPv4Forwarding_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
IPv4Forwarding_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
IPv4Forwarding_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
IPv4Forwarding_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
IPv4Forwarding_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
IPv4Forwarding_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
IPv4Forwarding_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
IPv4Forwarding_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    Routing.Router.{i}.IPv6Forwarding.{i}.

    *  IPv6Forwarding_GetEntryCount
    *  IPv6Forwarding_GetEntry
    *  IPv6Forwarding_AddEntry
    *  IPv6Forwarding_DelEntry
    *  IPv6Forwarding_GetParamBoolValue
    *  IPv6Forwarding_GetParamIntValue
    *  IPv6Forwarding_GetParamUlongValue
    *  IPv6Forwarding_GetParamStringValue
    *  IPv6Forwarding_SetParamBoolValue
    *  IPv6Forwarding_SetParamIntValue
    *  IPv6Forwarding_SetParamUlongValue
    *  IPv6Forwarding_SetParamStringValue
    *  IPv6Forwarding_Validate
    *  IPv6Forwarding_Commit
    *  IPv6Forwarding_Rollback

***********************************************************************/
ULONG
IPv6Forwarding_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
IPv6Forwarding_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
IPv6Forwarding_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
IPv6Forwarding_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
IPv6Forwarding_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
IPv6Forwarding_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
IPv6Forwarding_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
IPv6Forwarding_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
IPv6Forwarding_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
IPv6Forwarding_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
IPv6Forwarding_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
IPv6Forwarding_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
IPv6Forwarding_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
IPv6Forwarding_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
IPv6Forwarding_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    Routing.RIP.

    *  RIP_GetParamBoolValue
    *  RIP_GetParamIntValue
    *  RIP_GetParamUlongValue
    *  RIP_GetParamStringValue
    *  RIP_SetParamBoolValue
    *  RIP_SetParamIntValue
    *  RIP_SetParamUlongValue
    *  RIP_SetParamStringValue
    *  RIP_Validate
    *  RIP_Commit
    *  RIP_Rollback

***********************************************************************/
BOOL
RIP_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
RIP_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
RIP_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
RIP_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
RIP_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
RIP_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
RIP_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
RIP_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
RIP_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
RIP_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
RIP_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    Routing.RIP.InterfaceSetting.{i}.

    *  InterfaceSetting_GetEntryCount
    *  InterfaceSetting_GetEntry
    *  InterfaceSetting_AddEntry
    *  InterfaceSetting_DelEntry
    *  InterfaceSetting_GetParamBoolValue
    *  InterfaceSetting_GetParamIntValue
    *  InterfaceSetting_GetParamUlongValue
    *  InterfaceSetting_GetParamStringValue
    *  InterfaceSetting_SetParamBoolValue
    *  InterfaceSetting_SetParamIntValue
    *  InterfaceSetting_SetParamUlongValue
    *  InterfaceSetting_SetParamStringValue
    *  InterfaceSetting_Validate
    *  InterfaceSetting_Commit
    *  InterfaceSetting_Rollback

***********************************************************************/
ULONG
InterfaceSetting_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
InterfaceSetting_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
InterfaceSetting_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
InterfaceSetting_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
InterfaceSetting_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
InterfaceSetting_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
InterfaceSetting_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
InterfaceSetting_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
InterfaceSetting_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
InterfaceSetting_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
InterfaceSetting_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
InterfaceSetting_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
InterfaceSetting_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
InterfaceSetting_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
InterfaceSetting_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    Routing.RouteInformation.

    *  RouteInformation_GetParamBoolValue
    *  RouteInformation_GetParamIntValue
    *  RouteInformation_GetParamUlongValue
    *  RouteInformation_GetParamStringValue
    *  RouteInformation_SetParamBoolValue
    *  RouteInformation_SetParamIntValue
    *  RouteInformation_SetParamUlongValue
    *  RouteInformation_SetParamStringValue
    *  RouteInformation_Validate
    *  RouteInformation_Commit
    *  RouteInformation_Rollback

***********************************************************************/
BOOL
RouteInformation_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
RouteInformation_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
RouteInformation_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
RouteInformation_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
RouteInformation_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
RouteInformation_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
RouteInformation_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
RouteInformation_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
RouteInformation_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
RouteInformation_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
RouteInformation_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    Routing.RouteInformation.InterfaceSetting.{i}.

    *  InterfaceSetting3_GetEntryCount
    *  InterfaceSetting3_GetEntry
    *  InterfaceSetting3_IsUpdated
    *  InterfaceSetting3_Synchronize
    *  InterfaceSetting3_GetParamBoolValue
    *  InterfaceSetting3_GetParamIntValue
    *  InterfaceSetting3_GetParamUlongValue
    *  InterfaceSetting3_GetParamStringValue

***********************************************************************/
ULONG
InterfaceSetting3_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
InterfaceSetting3_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

BOOL
InterfaceSetting3_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
InterfaceSetting3_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

BOOL
InterfaceSetting3_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
InterfaceSetting3_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
InterfaceSetting3_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
InterfaceSetting3_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    Routing.RouteInformation.

    *  RIPv2_GetParamBoolValue
    *  RIPv2_GetParamIntValue
    *  RIPv2_GetParamUlongValue
    *  RIPv2_GetParamStringValue
    *  RIPv2_SetParamBoolValue
    *  RIPv2_SetParamIntValue
    *  RIPv2_SetParamUlongValue
    *  RIPv2_SetParamStringValue
    *  RIPv2_Validate
    *  RIPv2_Commit
    *  RIPv2_Rollback

***********************************************************************/
BOOL
RIPv2_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
RIPv2_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
RIPv2_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
RIPv2_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
RIPv2_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
RIPv2_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
RIPv2_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
RIPv2_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
RIPv2_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
RIPv2_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
RIPv2_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );


#endif
