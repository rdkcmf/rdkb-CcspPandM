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

    module: cosa_x_cisco_com_ddns_dml.h

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

#if !defined(DDNS_BROADBANDFORUM)
#ifndef  _COSA_X_CISCO_COM_DDNS_DML_H
#define  _COSA_X_CISCO_COM_DDNS_DML_H

/***********************************************************************

 APIs for Object:

    X_CISCO_COM_DDNS.

    *  X_CISCO_COM_DDNS_GetParamBoolValue
    *  X_CISCO_COM_DDNS_GetParamIntValue
    *  X_CISCO_COM_DDNS_GetParamUlongValue
    *  X_CISCO_COM_DDNS_GetParamStringValue
    *  X_CISCO_COM_DDNS_SetParamBoolValue
    *  X_CISCO_COM_DDNS_SetParamIntValue
    *  X_CISCO_COM_DDNS_SetParamUlongValue
    *  X_CISCO_COM_DDNS_SetParamStringValue
    *  X_CISCO_COM_DDNS_Validate
    *  X_CISCO_COM_DDNS_Commit
    *  X_CISCO_COM_DDNS_Rollback

***********************************************************************/
BOOL
X_CISCO_COM_DDNS_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
X_CISCO_COM_DDNS_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
X_CISCO_COM_DDNS_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
X_CISCO_COM_DDNS_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
X_CISCO_COM_DDNS_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
X_CISCO_COM_DDNS_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
X_CISCO_COM_DDNS_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
X_CISCO_COM_DDNS_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
X_CISCO_COM_DDNS_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
X_CISCO_COM_DDNS_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
X_CISCO_COM_DDNS_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    X_CISCO_COM_DDNS.Service.{i}.

    *  Service_GetEntryCount
    *  Service_GetEntry
    *  Service_AddEntry
    *  Service_DelEntry
    *  Service_GetParamBoolValue
    *  Service_GetParamIntValue
    *  Service_GetParamUlongValue
    *  Service_GetParamStringValue
    *  Service_SetParamBoolValue
    *  Service_SetParamIntValue
    *  Service_SetParamUlongValue
    *  Service_SetParamStringValue
    *  Service_Validate
    *  Service_Commit
    *  Service_Rollback

***********************************************************************/
ULONG
Service_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
Service_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
Service_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
Service_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
Service_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Service_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
Service_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Service_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
Service_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
Service_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
Service_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
Service_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
Service_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
Service_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
Service_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

#endif
#endif
