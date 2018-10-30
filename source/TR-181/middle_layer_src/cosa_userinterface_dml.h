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

    module: cosa_userinterface_dml.h

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


#ifndef  _COSA_USERINTERFACE_DML_H
#define  _COSA_USERINTERFACE_DML_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "cosa_userinterface_internal.h"

/***********************************************************************

 APIs for Object:

    UserInterface.


***********************************************************************/
/***********************************************************************

 APIs for Object:

    UserInterface..

    *  UserInterface_GetParamBoolValue
    *  UserInterface_GetParamUlongValue
    *  UserInterface_SetParamBoolValue
    *  UserInterface_SetParamUlongValue
    *  RemoteAccess_Validate
    *  RemoteAccess_Commit
    *  RemoteAccess_Rollback

***********************************************************************/
BOOL
UserInterface_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );
BOOL
UserInterface_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
UserInterface_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    );

BOOL
UserInterface_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    );

BOOL
UserInterface_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
UserInterface_Commit
    (
        ANSC_HANDLE                 hInsContext
    );


/***********************************************************************

 APIs for Object:

    UserInterface.RemoteAccess.

    *  RemoteAccess_GetParamBoolValue
    *  RemoteAccess_GetParamIntValue
    *  RemoteAccess_GetParamUlongValue
    *  RemoteAccess_GetParamStringValue
    *  RemoteAccess_SetParamBoolValue
    *  RemoteAccess_SetParamIntValue
    *  RemoteAccess_SetParamUlongValue
    *  RemoteAccess_SetParamStringValue
    *  RemoteAccess_Validate
    *  RemoteAccess_Commit
    *  RemoteAccess_Rollback

***********************************************************************/
BOOL
RemoteAccess_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
RemoteAccess_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
RemoteAccess_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
RemoteAccess_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
RemoteAccess_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
RemoteAccess_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
RemoteAccess_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
RemoteAccess_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
RemoteAccess_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
RemoteAccess_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
RemoteAccess_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

#endif
