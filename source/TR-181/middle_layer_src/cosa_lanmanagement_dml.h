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

/**************************************************************************

    module: cosa_lanmanagement_dml.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        
    -------------------------------------------------------------------

    revision:

        05/09/2019    initial revision.

**************************************************************************/

#ifndef  _COSA_LANMANAGEMENT_DML_H
#define  _COSA_LANMANAGEMENT_DML_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "cosa_lanmanagement_internal.h"

/***********************************************************************

 APIs for Object:

    DeviceInfo.

    *  LanManagement_GetParamBoolValue
    *  LanManagement_GetParamStringValue
    *  LanManagement_SetParamBoolValue
    *  LanManagement_SetParamStringValue
    *  LanManagement_Validate
    *  LanManagement_Commit
    *  LanManagement_Rollback

***********************************************************************/

BOOL
LanManagement_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

ULONG
LanManagement_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
LanManagement_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
LanManagement_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
LanManagement_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
LanManagement_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
LanManagement_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

#endif
