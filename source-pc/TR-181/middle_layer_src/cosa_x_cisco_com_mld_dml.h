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

    module: cosa_x_cisco_com_mld_dml.h

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


#ifndef  _COSA_MLD_DML_H
#define  _COSA_MLD_DML_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "cosa_x_cisco_com_mld_internal.h"

/***********************************************************************

 APIs for Object:

    X_CISCO_COM_IGMP.

    *  X_CISCO_COM_MLD_GetParamBoolValue
    *  X_CISCO_COM_MLD_GetParamIntValue
    *  X_CISCO_COM_MLD_GetParamUlongValue
    *  X_CISCO_COM_MLD_GetParamStringValue
    *  X_CISCO_COM_MLD_SetParamBoolValue
    *  X_CISCO_COM_MLD_SetParamIntValue
    *  X_CISCO_COM_MLD_SetParamUlongValue
    *  X_CISCO_COM_MLD_SetParamStringValue
    *  X_CISCO_COM_MLD_Validate
    *  X_CISCO_COM_MLD_Commit
    *  X_CISCO_COM_MLD_Rollback

***********************************************************************/
BOOL
X_CISCO_COM_MLD_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
X_CISCO_COM_MLD_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
X_CISCO_COM_MLD_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
X_CISCO_COM_MLD_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
X_CISCO_COM_MLD_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
X_CISCO_COM_MLD_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
X_CISCO_COM_MLD_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
X_CISCO_COM_MLD_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
X_CISCO_COM_MLD_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
X_CISCO_COM_MLD_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
X_CISCO_COM_MLD_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    X_CISCO_COM_MLD.Group.{i}.

    *  Group1_GetEntryCount
    *  Group1_GetEntry
    *  Group1_IsUpdated
    *  Group1_Synchronize
    *  Group1_GetParamBoolValue
    *  Group1_GetParamIntValue
    *  Group1_GetParamUlongValue
    *  Group1_GetParamStringValue

***********************************************************************/
ULONG
Group1_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
Group1_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

BOOL
Group1_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
Group1_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

BOOL
Group1_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Group1_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
Group1_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Group1_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

#endif
