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

    module: cosa_bridging_dml.h

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

        03/14/2011    initial revision.

**************************************************************************/


#ifndef  _COSA_BRIDGING_DML_H
#define  _COSA_BRIDGING_DML_H

/***********************************************************************

 APIs for Object:

    Bridging.

    *  Bridging_GetParamBoolValue
    *  Bridging_GetParamIntValue
    *  Bridging_GetParamUlongValue
    *  Bridging_GetParamStringValue

***********************************************************************/
BOOL
Bridging_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Bridging_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
Bridging_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Bridging_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    Bridging.Bridge.{i}.

    *  Bridge_GetEntryCount
    *  Bridge_GetEntry
    *  Bridge_AddEntry
    *  Bridge_DelEntry
    *  Bridge_GetParamBoolValue
    *  Bridge_GetParamIntValue
    *  Bridge_GetParamUlongValue
    *  Bridge_GetParamStringValue
    *  Bridge_SetParamBoolValue
    *  Bridge_SetParamIntValue
    *  Bridge_SetParamUlongValue
    *  Bridge_SetParamStringValue
    *  Bridge_Validate
    *  Bridge_Commit
    *  Bridge_Rollback

***********************************************************************/
ULONG
Bridge_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
Bridge_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
Bridge_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
Bridge_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
Bridge_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Bridge_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
Bridge_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Bridge_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
Bridge_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
Bridge_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
Bridge_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
Bridge_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
Bridge_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
Bridge_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
Bridge_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    Bridging.Bridge.{i}.Port.{i}.

    *  Port_GetEntryCount
    *  Port_GetEntry
    *  Port_AddEntry
    *  Port_DelEntry
    *  Port_GetParamBoolValue
    *  Port_GetParamIntValue
    *  Port_GetParamUlongValue
    *  Port_GetParamStringValue
    *  Port_SetParamBoolValue
    *  Port_SetParamIntValue
    *  Port_SetParamUlongValue
    *  Port_SetParamStringValue
    *  Port_Validate
    *  Port_Commit
    *  Port_Rollback

***********************************************************************/
ULONG
Port_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
Port_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
Port_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
Port_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
Port_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Port_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
Port_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Port_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
Port_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
Port_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
Port_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
Port_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
Port_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
Port_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
Port_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    Bridging.Bridge.{i}.Port.{i}.Stats.

    *  PortStats_GetParamBoolValue
    *  PortStats_GetParamIntValue
    *  PortStats_GetParamUlongValue
    *  PortStats_GetParamStringValue

***********************************************************************/
BOOL
PortStats_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
PortStats_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
PortStats_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
PortStats_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    Bridging.Bridge.{i}.VLAN.{i}.

    *  VLAN_GetEntryCount
    *  VLAN_GetEntry
    *  VLAN_AddEntry
    *  VLAN_DelEntry
    *  VLAN_GetParamBoolValue
    *  VLAN_GetParamIntValue
    *  VLAN_GetParamUlongValue
    *  VLAN_GetParamStringValue
    *  VLAN_SetParamBoolValue
    *  VLAN_SetParamIntValue
    *  VLAN_SetParamUlongValue
    *  VLAN_SetParamStringValue
    *  VLAN_Validate
    *  VLAN_Commit
    *  VLAN_Rollback

***********************************************************************/
ULONG
VLAN_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
VLAN_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
VLAN_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
VLAN_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
VLAN_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
VLAN_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
VLAN_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
VLAN_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
VLAN_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
VLAN_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
VLAN_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
VLAN_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
VLAN_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
VLAN_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
VLAN_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    Bridging.Bridge.{i}.VLANPort.{i}.

    *  VLANPort_GetEntryCount
    *  VLANPort_GetEntry
    *  VLANPort_AddEntry
    *  VLANPort_DelEntry
    *  VLANPort_GetParamBoolValue
    *  VLANPort_GetParamIntValue
    *  VLANPort_GetParamUlongValue
    *  VLANPort_GetParamStringValue
    *  VLANPort_SetParamBoolValue
    *  VLANPort_SetParamIntValue
    *  VLANPort_SetParamUlongValue
    *  VLANPort_SetParamStringValue
    *  VLANPort_Validate
    *  VLANPort_Commit
    *  VLANPort_Rollback

***********************************************************************/
ULONG
VLANPort_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
VLANPort_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
VLANPort_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
VLANPort_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
VLANPort_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
VLANPort_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
VLANPort_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
VLANPort_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
VLANPort_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
VLANPort_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
VLANPort_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
VLANPort_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
VLANPort_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
VLANPort_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
VLANPort_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    Bridging.Filter.{i}.

    *  Filter_GetEntryCount
    *  Filter_GetEntry
    *  Filter_AddEntry
    *  Filter_DelEntry
    *  Filter_GetParamBoolValue
    *  Filter_GetParamIntValue
    *  Filter_GetParamUlongValue
    *  Filter_GetParamStringValue
    *  Filter_SetParamBoolValue
    *  Filter_SetParamIntValue
    *  Filter_SetParamUlongValue
    *  Filter_SetParamStringValue
    *  Filter_Validate
    *  Filter_Commit
    *  Filter_Rollback

***********************************************************************/
ULONG
Filter_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
Filter_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
Filter_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
Filter_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
Filter_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Filter_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
Filter_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Filter_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
Filter_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
Filter_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
Filter_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
Filter_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
Filter_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
Filter_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
Filter_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

#endif
