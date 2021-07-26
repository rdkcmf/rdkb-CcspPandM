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

    module: cosa_ethernet_dml.h

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


#ifndef  _COSA_ETHERNET_DML_H
#define  _COSA_ETHERNET_DML_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "cosa_ethernet_internal.h"

/***********************************************************************

 APIs for Object:

    Ethernet.

    *  Ethernet_GetParamBoolValue
    *  Ethernet_GetParamIntValue
    *  Ethernet_GetParamUlongValue
    *  Ethernet_GetParamStringValue

***********************************************************************/
BOOL
Ethernet_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Ethernet_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
Ethernet_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Ethernet_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );
#ifndef FEATURE_RDKB_WAN_MANAGER
/***********************************************************************

 APIs for Object:

    Ethernet.Interface.{i}.

    *  Interface_GetEntryCount
    *  Interface_GetEntry
    *  Interface_GetParamBoolValue
    *  Interface_GetParamIntValue
    *  Interface_GetParamUlongValue
    *  Interface_GetParamStringValue
    *  Interface_SetParamBoolValue
    *  Interface_SetParamIntValue
    *  Interface_SetParamUlongValue
    *  Interface_SetParamStringValue
    *  Interface_Validate
    *  Interface_Commit
    *  Interface_Rollback

***********************************************************************/
ULONG
Interface_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
Interface_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

BOOL
Interface_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Interface_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
Interface_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Interface_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
Interface_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
Interface_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
Interface_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
Interface_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
Interface_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
Interface_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
Interface_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    Ethernet.Interface.{i}.Stats.

    *  Stats_GetParamBoolValue
    *  Stats_GetParamIntValue
    *  Stats_GetParamUlongValue
    *  Stats_GetParamStringValue

***********************************************************************/
BOOL
Stats_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Stats_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
Stats_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Stats_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );
#endif // #ifndef FEATURE_RDKB_WAN_MANAGER
/***********************************************************************

 APIs for Object:

    Ethernet.Link.{i}.

    *  Link_GetEntryCount
    *  Link_GetEntry
    *  Link_AddEntry
    *  Link_DelEntry
    *  Link_GetParamBoolValue
    *  Link_GetParamIntValue
    *  Link_GetParamUlongValue
    *  Link_GetParamStringValue
    *  Link_SetParamBoolValue
    *  Link_SetParamIntValue
    *  Link_SetParamUlongValue
    *  Link_SetParamStringValue
    *  Link_Validate
    *  Link_Commit
    *  Link_Rollback

***********************************************************************/
ULONG
Link_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
Link_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
Link_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
Link_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
Link_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Link_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
Link_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Link_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
Link_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
Link_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
Link_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
Link_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
Link_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
Link_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
Link_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    Ethernet.Link.{i}.Stats.

    *  Stats1_GetParamBoolValue
    *  Stats1_GetParamIntValue
    *  Stats1_GetParamUlongValue
    *  Stats1_GetParamStringValue

***********************************************************************/
BOOL
Stats1_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
Stats1_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
Stats1_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
Stats1_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/***********************************************************************

 APIs for Object:

    Ethernet.VLANTermination.{i}.

    *  VLANTermination_GetEntryCount
    *  VLANTermination_GetEntry
    *  VLANTermination_AddEntry
    *  VLANTermination_DelEntry
    *  VLANTermination_GetParamBoolValue
    *  VLANTermination_GetParamIntValue
    *  VLANTermination_GetParamUlongValue
    *  VLANTermination_GetParamStringValue
    *  VLANTermination_SetParamBoolValue
    *  VLANTermination_SetParamIntValue
    *  VLANTermination_SetParamUlongValue
    *  VLANTermination_SetParamStringValue
    *  VLANTermination_Validate
    *  VLANTermination_Commit
    *  VLANTermination_Rollback

***********************************************************************/
ULONG
VLANTermination_GetEntryCount
    (
        ANSC_HANDLE
    );

ANSC_HANDLE
VLANTermination_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

ANSC_HANDLE
VLANTermination_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    );

ULONG
VLANTermination_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );

BOOL
VLANTermination_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
VLANTermination_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
VLANTermination_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
VLANTermination_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
VLANTermination_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
VLANTermination_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

BOOL
VLANTermination_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

BOOL
VLANTermination_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

BOOL
VLANTermination_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
VLANTermination_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
VLANTermination_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/***********************************************************************

 APIs for Object:

    Ethernet.VLANTermination.{i}.Stats.

    *  VLANTermination_Stats_GetParamBoolValue
    *  VLANTermination_Stats_GetParamIntValue
    *  VLANTermination_Stats_GetParamUlongValue
    *  VLANTermination_Stats_GetParamStringValue

***********************************************************************/
BOOL
VLANTermination_Stats_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
VLANTermination_Stats_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

BOOL
VLANTermination_Stats_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

ULONG
VLANTermination_Stats_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

int
_getMac
    (
        char*                      ifName, 
        char*                      mac
    );

ANSC_STATUS
CosaDmlEthLinkUpdateStaticMac
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_ETH_LINK_CFG      pCfg,
        PCOSA_DML_ETH_LINK_FULL     pEntry
    );

#endif
