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

    module: cosa_neighdisc_internal.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**************************************************************************/


#ifndef  _COSA_NEIGHDISC_INTERNAL_H
#define  _COSA_NEIGHDISC_INTERNAL_H

#include "cosa_apis.h"

#define  COSA_IREP_FOLDER_NAME_NeighdiscIF                 "NeighdiscInterface"
#define  COSA_IREP_FOLDER_NAME_NeighdiscIF_HA              "NeighdiscInterfaceHalfAdded"

#define  COSA_DML_RR_NAME_NeighdiscIF_Prefix               "NeighdiscInterface"

#define  COSA_DML_RR_NAME_NeighdiscIF_Upper_InsNum         "UpperInstanceNumber"
#define  COSA_DML_RR_NAME_NeighdiscIF_InsNum               "InstanceNumber"
#define  COSA_DML_RR_NAME_NeighdiscIF_Alias                "Alias"

#define  COSA_DML_RR_NAME_NeighdiscIF_NextInsNum           "NextInstanceNumber"
#define  COSA_DML_RR_NAME_NeighdiscIF_OPTION_NextInsNum    "NextOptionInstanceNum"



/***********************************
    Actual definition declaration
************************************/

/* 
    Object definition
*/

#define  COSA_DATAMODEL_NEIGHDISC_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    COSA_BASE_CONTENT                                                                       \
    /* start of NAT object class content */                                                 \
    BOOL                            bEnabled;                                               \
    SLIST_HEADER                    InterfaceList;                                          \
    ULONG                           ulInterfaceCount;                                     \
    ULONG                           ulNextInterfaceInsNum;                                \
    ANSC_HANDLE                     hIrepFolderCOSA;                                       \
    ANSC_HANDLE                     hIrepFolderNeighdiscInterface;                                 \
    ANSC_HANDLE                     hIrepFolderNeighdiscInterfaceHA;                                 \
    ULONG                           ulNbTblNumber;                                           \
    PCOSA_DML_NEIGHTABLE_INFO       pNbTbl;                                                  \

typedef  struct
_COSA_DATAMODEL_NEIGHDISC                                               
{
    COSA_DATAMODEL_NEIGHDISC_CLASS_CONTENT
}
COSA_DATAMODEL_NEIGHDISC,  *PCOSA_DATAMODEL_NEIGHDISC;

/**********************************
    Standard function declaration 
***********************************/
ANSC_HANDLE
CosaNeighdiscCreate
    (
        VOID
    );

ANSC_STATUS
CosaNeighdiscInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaNeighdiscRemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaNeighdiscRegGetInfo

    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaNeighdiscRegAddInfo
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pNextInsNumName,
        ULONG                       ulNextInsNum,
        char*                       pPreffix,
        ULONG                       ulUpperInsNum,
        char*                       pAlias,
        ANSC_HANDLE                 hCosaContext
    );

ANSC_STATUS
CosaNeighdiscRegDelInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );





#endif
