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

    module: cosa_nat_dml.h

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


#ifndef  _COSA_NAT_DML_H
#define  _COSA_NAT_DML_H



#include "webconfig_framework.h"
#include "cosa_webconfig_api.h"
#include "cosa_nat_webconfig_apis.h"
#include "cosa_nat_apis.h"

/***********************************************************************

 APIs for Object:

    NAT.

    *  NAT_GetParamBoolValue
    *  NAT_GetParamIntValue
    *  NAT_GetParamUlongValue
    *  NAT_GetParamStringValue

***********************************************************************/
BOOL
NAT_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
NAT_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
NAT_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
NAT_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
NAT_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/***********************************************************************

 APIs for Object:

    NAT.X_RDK_PortMapping.

    *  X_RDK_PortMapping_GetParamStringValue
    *  X_RDK_PortMapping_SetParamStringValue

***********************************************************************/

ULONG
X_RDK_PortMapping_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
X_RDK_PortMapping_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    );

/***********************************************************************

 APIs for Object:

    NAT.X_CISCO_COM_DMZ.

    *  X_CISCO_COM_DMZ_GetParamBoolValue
    *  X_CISCO_COM_DMZ_GetParamStringValue
    *  X_CISCO_COM_DMZ_SetParamBoolValue
    *  X_CISCO_COM_DMZ_SetParamStringValue
    *  X_CISCO_COM_DMZ_Validate
    *  X_CISCO_COM_DMZ_Commit
    *  X_CISCO_COM_DMZ_Rollback

***********************************************************************/

BOOL
X_CISCO_COM_DMZ_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

ULONG
X_CISCO_COM_DMZ_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
X_CISCO_COM_DMZ_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
X_CISCO_COM_DMZ_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    );

BOOL
X_CISCO_COM_DMZ_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
X_CISCO_COM_DMZ_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
X_CISCO_COM_DMZ_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    NAT.PortMapping.{i}.

    *  PortMapping_GetEntryCount
    *  PortMapping_GetEntry
    *  PortMapping_IsUpdated
    *  PortMapping_Synchronize
    *  PortMapping_GetParamBoolValue
    *  PortMapping_GetParamIntValue
    *  PortMapping_GetParamUlongValue
    *  PortMapping_GetParamStringValue
    *  PortMapping_SetParamBoolValue
    *  PortMapping_SetParamIntValue
    *  PortMapping_SetParamUlongValue
    *  PortMapping_SetParamStringValue
    *  PortMapping_Validate
    *  PortMapping_Commit
    *  PortMapping_Rollback

***********************************************************************/
ULONG
PortMapping_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
PortMapping_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

BOOL
PortMapping_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
PortMapping_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

ANSC_HANDLE
PortMapping_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
PortMapping_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
PortMapping_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
PortMapping_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
PortMapping_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
PortMapping_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
PortMapping_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
PortMapping_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
PortMapping_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
PortMapping_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
PortMapping_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
PortMapping_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
PortMapping_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    NAT.X_CISCO_COM_PortTriggers.Trigger.{i}.

    *  PortTrigger_GetEntryCount
    *  PortTrigger_GetEntry
    *  PortTrigger_AddEntry
    *  PortTrigger_DelEntry
    *  PortTrigger_GetParamBoolValue
    *  PortTrigger_GetParamUlongValue
    *  PortTrigger_GetParamStringValue
    *  PortTrigger_SetParamBoolValue
    *  PortTrigger_SetParamUlongValue
    *  PortTrigger_SetParamStringValue
    *  PortTrigger_Validate
    *  PortTrigger_Commit
    *  PortTrigger_Rollback

***********************************************************************/

ULONG
PortTrigger_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    );

ANSC_HANDLE
PortTrigger_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
PortTrigger_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
PortTrigger_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
PortTrigger_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
PortTrigger_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    );

ULONG
PortTrigger_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
PortTrigger_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
PortTrigger_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    );

BOOL
PortTrigger_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    );

BOOL
PortTrigger_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
PortTrigger_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
PortTrigger_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

BOOL
NatPortTrigger_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
NatPortTrigger_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );


#endif
