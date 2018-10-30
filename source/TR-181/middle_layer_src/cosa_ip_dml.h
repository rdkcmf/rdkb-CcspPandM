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

    module: cosa_ip_dml.h

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


#ifndef  _COSA_IP_DML_H
#define  _COSA_IP_DML_H

/***********************************************************************

 APIs for Object:

    IP.

    *  IP_GetParamBoolValue
    *  IP_GetParamIntValue
    *  IP_GetParamUlongValue
    *  IP_GetParamStringValue

***********************************************************************/
BOOL
IP_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
IP_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
IP_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
IP_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );
BOOL
IP_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
IP_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
IP_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
IP_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
IP_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
IP_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
IP_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    IP.Interface.{i}.

    *  Interface2_GetEntryCount
    *  Interface2_GetEntry
    *  Interface2_AddEntry
    *  Interface2_DelEntry
    *  Interface2_GetParamBoolValue
    *  Interface2_GetParamIntValue
    *  Interface2_GetParamUlongValue
    *  Interface2_GetParamStringValue
    *  Interface2_SetParamBoolValue
    *  Interface2_SetParamIntValue
    *  Interface2_SetParamUlongValue
    *  Interface2_SetParamStringValue
    *  Interface2_Validate
    *  Interface2_Commit
    *  Interface2_Rollback

***********************************************************************/
ULONG
Interface2_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
Interface2_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
Interface2_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
Interface2_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
Interface2_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Interface2_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
Interface2_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Interface2_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
Interface2_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
Interface2_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
Interface2_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
Interface2_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
Interface2_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
Interface2_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
Interface2_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    IP.Interface.{i}.IPv4Address.{i}.

    *  IPv4Address_GetEntryCount
    *  IPv4Address_GetEntry
    *  IPv4Address_AddEntry
    *  IPv4Address_DelEntry
    *  IPv4Address_GetParamBoolValue
    *  IPv4Address_GetParamIntValue
    *  IPv4Address_GetParamUlongValue
    *  IPv4Address_GetParamStringValue
    *  IPv4Address_SetParamBoolValue
    *  IPv4Address_SetParamIntValue
    *  IPv4Address_SetParamUlongValue
    *  IPv4Address_SetParamStringValue
    *  IPv4Address_Validate
    *  IPv4Address_Commit
    *  IPv4Address_Rollback

***********************************************************************/
ULONG
IPv4Address_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
IPv4Address_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
IPv4Address_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
IPv4Address_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
IPv4Address_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
IPv4Address_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
IPv4Address_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
IPv4Address_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
IPv4Address_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
IPv4Address_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
IPv4Address_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
IPv4Address_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
IPv4Address_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
IPv4Address_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
IPv4Address_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    IP.Interface.{i}.IPv6Address.{i}.

    *  IPv6Address_GetEntryCount
    *  IPv6Address_GetEntry
    *  IPv6Address_AddEntry
    *  IPv6Address_DelEntry
    *  IPv6Address_GetParamBoolValue
    *  IPv6Address_GetParamIntValue
    *  IPv6Address_GetParamUlongValue
    *  IPv6Address_GetParamStringValue
    *  IPv6Address_SetParamBoolValue
    *  IPv6Address_SetParamIntValue
    *  IPv6Address_SetParamUlongValue
    *  IPv6Address_SetParamStringValue
    *  IPv6Address_Validate
    *  IPv6Address_Commit
    *  IPv6Address_Rollback

***********************************************************************/
ULONG
IPv6Address_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
IPv6Address_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
IPv6Address_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
IPv6Address_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
IPv6Address_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
IPv6Address_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
IPv6Address_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
IPv6Address_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
IPv6Address_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
IPv6Address_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
IPv6Address_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
IPv6Address_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
IPv6Address_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
IPv6Address_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
IPv6Address_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

BOOL
IPv6Address_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
IPv6Address_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );


/***********************************************************************

 APIs for Object:

    IP.Interface.{i}.IPv6Prefix.{i}.

    *  IPv6Prefix_GetEntryCount
    *  IPv6Prefix_GetEntry
    *  IPv6Prefix_AddEntry
    *  IPv6Prefix_DelEntry
    *  IPv6Prefix_GetParamBoolValue
    *  IPv6Prefix_GetParamIntValue
    *  IPv6Prefix_GetParamUlongValue
    *  IPv6Prefix_GetParamStringValue
    *  IPv6Prefix_SetParamBoolValue
    *  IPv6Prefix_SetParamIntValue
    *  IPv6Prefix_SetParamUlongValue
    *  IPv6Prefix_SetParamStringValue
    *  IPv6Prefix_Validate
    *  IPv6Prefix_Commit
    *  IPv6Prefix_Rollback

***********************************************************************/
BOOL
IPv6Prefix_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
IPv6Prefix_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );


ULONG
IPv6Prefix_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
IPv6Prefix_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
IPv6Prefix_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
IPv6Prefix_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
IPv6Prefix_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
IPv6Prefix_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
IPv6Prefix_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
IPv6Prefix_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
IPv6Prefix_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
IPv6Prefix_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
IPv6Prefix_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
IPv6Prefix_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
IPv6Prefix_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
IPv6Prefix_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
IPv6Prefix_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    IP.Interface.{i}.Stats.

    *  Stats5_GetParamBoolValue
    *  Stats5_GetParamIntValue
    *  Stats5_GetParamUlongValue
    *  Stats5_GetParamStringValue

***********************************************************************/
BOOL
Stats5_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Stats5_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
Stats5_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Stats5_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    IP.ActivePort.{i}.

    *  ActivePort_GetEntryCount
    *  ActivePort_GetEntry
    *  ActivePort_IsUpdated
    *  ActivePort_Synchronize
    *  ActivePort_GetParamBoolValue
    *  ActivePort_GetParamIntValue
    *  ActivePort_GetParamUlongValue
    *  ActivePort_GetParamStringValue

***********************************************************************/
ULONG
ActivePort_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
ActivePort_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

BOOL
ActivePort_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
ActivePort_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

BOOL
ActivePort_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
ActivePort_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
ActivePort_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
ActivePort_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    IP.Diagnostics.


***********************************************************************/
/***********************************************************************

 APIs for Object:

    IP.Diagnostics.X_CISCO_COM_ARP.

    *  X_CISCO_COM_ARP_GetParamBoolValue
    *  X_CISCO_COM_ARP_GetParamIntValue
    *  X_CISCO_COM_ARP_GetParamUlongValue
    *  X_CISCO_COM_ARP_GetParamStringValue

***********************************************************************/
BOOL
X_CISCO_COM_ARP_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
X_CISCO_COM_ARP_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
X_CISCO_COM_ARP_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
X_CISCO_COM_ARP_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    IP.Diagnostics.X_CISCO_COM_ARP.Table.{i}.

    *  ARPTable_GetEntryCount
    *  ARPTable_GetEntry
    *  ARPTable_IsUpdated
    *  ARPTable_Synchronize
    *  ARPTable_GetParamBoolValue
    *  ARPTable_GetParamIntValue
    *  ARPTable_GetParamUlongValue
    *  ARPTable_GetParamStringValue

***********************************************************************/
ULONG
ARPTable_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
ARPTable_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

BOOL
ARPTable_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
ARPTable_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

BOOL
ARPTable_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
ARPTable_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
ARPTable_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
ARPTable_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    IP.Diagnostics.IPPing.

    *  IPPing_GetParamBoolValue
    *  IPPing_GetParamIntValue
    *  IPPing_GetParamUlongValue
    *  IPPing_GetParamStringValue
    *  IPPing_SetParamBoolValue
    *  IPPing_SetParamIntValue
    *  IPPing_SetParamUlongValue
    *  IPPing_SetParamStringValue
    *  IPPing_Validate
    *  IPPing_Commit
    *  IPPing_Rollback

***********************************************************************/
BOOL
IPPing_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
IPPing_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
IPPing_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
IPPing_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
IPPing_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
IPPing_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
IPPing_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
IPPing_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
IPPing_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
IPPing_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
IPPing_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    IP.Diagnostics.TraceRoute.

    *  TraceRoute_GetParamBoolValue
    *  TraceRoute_GetParamIntValue
    *  TraceRoute_GetParamUlongValue
    *  TraceRoute_GetParamStringValue
    *  TraceRoute_SetParamBoolValue
    *  TraceRoute_SetParamIntValue
    *  TraceRoute_SetParamUlongValue
    *  TraceRoute_SetParamStringValue
    *  TraceRoute_Validate
    *  TraceRoute_Commit
    *  TraceRoute_Rollback

***********************************************************************/
BOOL
TraceRoute_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
TraceRoute_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
TraceRoute_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
TraceRoute_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
TraceRoute_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
TraceRoute_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
TraceRoute_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
TraceRoute_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
TraceRoute_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
TraceRoute_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
TraceRoute_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    IP.Diagnostics.TraceRoute.RouteHops.{i}.

    *  RouteHops_GetEntryCount
    *  RouteHops_GetEntry
    *  RouteHops_IsUpdated
    *  RouteHops_Synchronize
    *  RouteHops_GetParamBoolValue
    *  RouteHops_GetParamIntValue
    *  RouteHops_GetParamUlongValue
    *  RouteHops_GetParamStringValue

***********************************************************************/
ULONG
RouteHops_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
RouteHops_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

BOOL
RouteHops_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
RouteHops_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

BOOL
RouteHops_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
RouteHops_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
RouteHops_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
RouteHops_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    IP.Diagnostics.DownloadDiagnostics.

    *  DownloadDiagnostics_GetParamBoolValue
    *  DownloadDiagnostics_GetParamIntValue
    *  DownloadDiagnostics_GetParamUlongValue
    *  DownloadDiagnostics_GetParamStringValue
    *  DownloadDiagnostics_SetParamBoolValue
    *  DownloadDiagnostics_SetParamIntValue
    *  DownloadDiagnostics_SetParamUlongValue
    *  DownloadDiagnostics_SetParamStringValue
    *  DownloadDiagnostics_Validate
    *  DownloadDiagnostics_Commit
    *  DownloadDiagnostics_Rollback

***********************************************************************/
BOOL
DownloadDiagnostics_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
DownloadDiagnostics_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
DownloadDiagnostics_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
DownloadDiagnostics_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
DownloadDiagnostics_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
DownloadDiagnostics_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
DownloadDiagnostics_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
DownloadDiagnostics_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
DownloadDiagnostics_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
DownloadDiagnostics_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
DownloadDiagnostics_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    IP.Diagnostics.UploadDiagnostics.

    *  UploadDiagnostics_GetParamBoolValue
    *  UploadDiagnostics_GetParamIntValue
    *  UploadDiagnostics_GetParamUlongValue
    *  UploadDiagnostics_GetParamStringValue
    *  UploadDiagnostics_SetParamBoolValue
    *  UploadDiagnostics_SetParamIntValue
    *  UploadDiagnostics_SetParamUlongValue
    *  UploadDiagnostics_SetParamStringValue
    *  UploadDiagnostics_Validate
    *  UploadDiagnostics_Commit
    *  UploadDiagnostics_Rollback

***********************************************************************/
BOOL
UploadDiagnostics_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
UploadDiagnostics_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
UploadDiagnostics_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
UploadDiagnostics_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
UploadDiagnostics_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
UploadDiagnostics_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
UploadDiagnostics_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
UploadDiagnostics_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
UploadDiagnostics_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
UploadDiagnostics_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
UploadDiagnostics_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    IP.Diagnostics.UDPEchoConfig.

    *  UDPEchoConfig_GetParamBoolValue
    *  UDPEchoConfig_GetParamIntValue
    *  UDPEchoConfig_GetParamUlongValue
    *  UDPEchoConfig_GetParamStringValue
    *  UDPEchoConfig_SetParamBoolValue
    *  UDPEchoConfig_SetParamIntValue
    *  UDPEchoConfig_SetParamUlongValue
    *  UDPEchoConfig_SetParamStringValue
    *  UDPEchoConfig_Validate
    *  UDPEchoConfig_Commit
    *  UDPEchoConfig_Rollback

***********************************************************************/
BOOL
UDPEchoConfig_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
UDPEchoConfig_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
UDPEchoConfig_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
UDPEchoConfig_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
UDPEchoConfig_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
UDPEchoConfig_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
UDPEchoConfig_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
UDPEchoConfig_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
UDPEchoConfig_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
UDPEchoConfig_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
UDPEchoConfig_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

#endif
