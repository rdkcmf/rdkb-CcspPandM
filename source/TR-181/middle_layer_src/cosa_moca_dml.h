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

    module: cosa_moca_dml.h

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


#ifndef  _COSA_MOCA_DML_H
#define  _COSA_MOCA_DML_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "cosa_moca_internal.h"

/***********************************************************************

 APIs for Object:

    MoCA.

    *  MoCA_GetParamBoolValue
    *  MoCA_GetParamIntValue
    *  MoCA_GetParamUlongValue
    *  MoCA_GetParamStringValue

***********************************************************************/
BOOL
MoCA_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
MoCA_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
MoCA_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
MoCA_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    MoCA.Interface.{i}.

    *  Interface1_GetEntryCount
    *  Interface1_GetEntry
    *  Interface1_GetParamBoolValue
    *  Interface1_GetParamIntValue
    *  Interface1_GetParamUlongValue
    *  Interface1_GetParamStringValue
    *  Interface1_SetParamBoolValue
    *  Interface1_SetParamIntValue
    *  Interface1_SetParamUlongValue
    *  Interface1_SetParamStringValue
    *  Interface1_Validate
    *  Interface1_Commit
    *  Interface1_Rollback

***********************************************************************/
ULONG
Interface1_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
Interface1_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

BOOL
Interface1_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Interface1_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
Interface1_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Interface1_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
Interface1_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
Interface1_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
Interface1_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
Interface1_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
Interface1_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
Interface1_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
Interface1_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    MoCA.Interface.{i}.Stats.

    *  Stats2_GetParamBoolValue
    *  Stats2_GetParamIntValue
    *  Stats2_GetParamUlongValue
    *  Stats2_GetParamStringValue

***********************************************************************/
BOOL
Stats2_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Stats2_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
Stats2_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Stats2_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    MoCA.Interface.{i}.QoS.

    *  QoS_GetParamBoolValue
    *  QoS_GetParamIntValue
    *  QoS_GetParamUlongValue
    *  QoS_GetParamStringValue

***********************************************************************/
BOOL
QoS_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
QoS_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
QoS_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
QoS_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    MoCA.Interface.{i}.QoS.FlowStats.{i}.

    *  FlowStats_GetEntryCount
    *  FlowStats_GetEntry
    *  FlowStats_IsUpdated
    *  FlowStats_Synchronize
    *  FlowStats_GetParamBoolValue
    *  FlowStats_GetParamIntValue
    *  FlowStats_GetParamUlongValue
    *  FlowStats_GetParamStringValue

***********************************************************************/
ULONG
FlowStats_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
FlowStats_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

BOOL
FlowStats_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
FlowStats_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

BOOL
FlowStats_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
FlowStats_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
FlowStats_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
FlowStats_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    MoCA.Interface.{i}.Associated{i}.

    *  AssociatedDevice_GetEntryCount
    *  AssociatedDevice_GetEntry
    *  AssociatedDevice_IsUpdated
    *  AssociatedDevice_Synchronize
    *  AssociatedDevice_GetParamBoolValue
    *  AssociatedDevice_GetParamIntValue
    *  AssociatedDevice_GetParamUlongValue
    *  AssociatedDevice_GetParamStringValue

***********************************************************************/
ULONG
AssociatedDevice_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
AssociatedDevice_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

BOOL
AssociatedDevice_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
AssociatedDevice_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

BOOL
AssociatedDevice_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
AssociatedDevice_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
AssociatedDevice_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
AssociatedDevice_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

#endif
