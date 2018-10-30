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

    module: cosa_ra_dml.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        10/09/2011    initial revision.

**************************************************************************/


#ifndef  _COSA_RA_DML_H
#define  _COSA_RA_DML_H

/***********************************************************************

 APIs for Object:

    RouterAdvertisement.

    *  RouterAdvertisement_GetParamBoolValue
    *  RouterAdvertisement_GetParamIntValue
    *  RouterAdvertisement_GetParamUlongValue
    *  RouterAdvertisement_GetParamStringValue
    *  RouterAdvertisement_SetParamBoolValue
    *  RouterAdvertisement_SetParamIntValue
    *  RouterAdvertisement_SetParamUlongValue
    *  RouterAdvertisement_SetParamStringValue
    *  RouterAdvertisement_Validate
    *  RouterAdvertisement_Commit
    *  RouterAdvertisement_Rollback

***********************************************************************/
BOOL
RouterAdvertisement_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
RouterAdvertisement_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
RouterAdvertisement_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
RouterAdvertisement_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
RouterAdvertisement_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
RouterAdvertisement_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
RouterAdvertisement_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
RouterAdvertisement_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
RouterAdvertisement_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
RouterAdvertisement_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
RouterAdvertisement_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );


/***********************************************************************

 APIs for Object:

    InterfaceSetting.{i}.

    *  InterfaceSetting1_GetEntryCount
    *  InterfaceSetting1_GetEntry
    *  InterfaceSetting1_AddEntry
    *  InterfaceSetting1_DelEntry
    *  InterfaceSetting1_GetParamBoolValue
    *  InterfaceSetting1_GetParamIntValue
    *  InterfaceSetting1_GetParamUlongValue
    *  InterfaceSetting1_GetParamStringValue
    *  InterfaceSetting1_SetParamBoolValue
    *  InterfaceSetting1_SetParamIntValue
    *  InterfaceSetting1_SetParamUlongValue
    *  InterfaceSetting1_SetParamStringValue
    *  InterfaceSetting1_Validate
    *  InterfaceSetting1_Commit
    *  InterfaceSetting1_Rollback

***********************************************************************/
ULONG
InterfaceSetting1_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
InterfaceSetting1_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
InterfaceSetting1_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
InterfaceSetting1_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
InterfaceSetting1_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
InterfaceSetting1_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
InterfaceSetting1_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
InterfaceSetting1_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
InterfaceSetting1_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
InterfaceSetting1_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
InterfaceSetting1_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
InterfaceSetting1_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
InterfaceSetting1_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
InterfaceSetting1_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
InterfaceSetting1_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );


/***********************************************************************

 APIs for Object:

    Option.{i}.

    *  Option5_GetEntryCount
    *  Option5_GetEntry
    *  Option5_AddEntry
    *  Option5_DelEntry
    *  Option5_GetParamBoolValue
    *  Option5_GetParamIntValue
    *  Option5_GetParamUlongValue
    *  Option5_GetParamStringValue
    *  Option5_SetParamBoolValue
    *  Option5_SetParamIntValue
    *  Option5_SetParamUlongValue
    *  Option5_SetParamStringValue
    *  Option5_Validate
    *  Option5_Commit
    *  Option5_Rollback

***********************************************************************/
ULONG
Option5_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
Option5_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
Option5_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
Option5_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
Option5_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Option5_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
Option5_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Option5_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
Option5_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
Option5_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
Option5_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
Option5_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
Option5_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
Option5_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
Option5_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

#endif
