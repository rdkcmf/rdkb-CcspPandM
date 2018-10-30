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

    module: cosa_apis_neighbordiscovery.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        10/11/2011    initial revision.

**************************************************************************/


#ifndef  _COSA_APIS_NEIGHBORDISCOVERY_H
#define  _COSA_APIS_NEIGHBORDISCOVERY_H

/***********************************************************************

 APIs for Object:

    NeighborDiscovery.

    *  NeighborDiscovery_GetParamBoolValue
    *  NeighborDiscovery_GetParamIntValue
    *  NeighborDiscovery_GetParamUlongValue
    *  NeighborDiscovery_GetParamStringValue
    *  NeighborDiscovery_SetParamBoolValue
    *  NeighborDiscovery_SetParamIntValue
    *  NeighborDiscovery_SetParamUlongValue
    *  NeighborDiscovery_SetParamStringValue
    *  NeighborDiscovery_Validate
    *  NeighborDiscovery_Commit
    *  NeighborDiscovery_Rollback

***********************************************************************/
BOOL
NeighborDiscovery_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
NeighborDiscovery_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
NeighborDiscovery_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
NeighborDiscovery_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
NeighborDiscovery_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
NeighborDiscovery_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
NeighborDiscovery_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
NeighborDiscovery_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
NeighborDiscovery_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
NeighborDiscovery_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
NeighborDiscovery_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    NeighborDiscovery.InterfaceSetting.{i}.

    *  InterfaceSetting2_GetEntryCount
    *  InterfaceSetting2_GetEntry
    *  InterfaceSetting2_AddEntry
    *  InterfaceSetting2_DelEntry
    *  InterfaceSetting2_GetParamBoolValue
    *  InterfaceSetting2_GetParamIntValue
    *  InterfaceSetting2_GetParamUlongValue
    *  InterfaceSetting2_GetParamStringValue
    *  InterfaceSetting2_SetParamBoolValue
    *  InterfaceSetting2_SetParamIntValue
    *  InterfaceSetting2_SetParamUlongValue
    *  InterfaceSetting2_SetParamStringValue
    *  InterfaceSetting2_Validate
    *  InterfaceSetting2_Commit
    *  InterfaceSetting2_Rollback

***********************************************************************/
ULONG
InterfaceSetting2_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
InterfaceSetting2_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
InterfaceSetting2_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
InterfaceSetting2_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
InterfaceSetting2_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
InterfaceSetting2_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
InterfaceSetting2_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
InterfaceSetting2_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
InterfaceSetting2_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
InterfaceSetting2_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
InterfaceSetting2_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
InterfaceSetting2_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
InterfaceSetting2_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
InterfaceSetting2_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
InterfaceSetting2_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

#endif
