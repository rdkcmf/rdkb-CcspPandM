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

    module: cosa_ppp_internal.h

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
#ifndef  FEATURE_RDKB_XDSL_PPP_MANAGER

#ifndef  _COSA_PPP_INTERNAL_H
#define  _COSA_PPP_INTERNAL_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "cosa_ppp_apis.h"
#include "poam_irepfo_interface.h"
#include "sys_definitions.h"

#define  COSA_IREP_FOLDER_NAME_PPPIF             "PPPIf"
#define  COSA_DML_RR_NAME_PPPIFAlias             "Alias"
#define  COSA_DML_RR_NAME_PPPIFInsNum            "InstanceNumber"
#define  COSA_DML_RR_NAME_PPPIFNextInsNunmber    "NextInstanceNumber"
#define  COSA_DML_RR_NAME_PPPIFNewlyAdded        "NewlyAdded"

/***********************************
    Actual definition declaration
************************************/
#define  COSA_DATAMODEL_PPP_CLASS_CONTENT                                   \
    /* duplication of the base object class content */                      \
    COSA_BASE_CONTENT                                                       \
    SLIST_HEADER                        IfList;                             \
    ULONG                               ulIfNextInstance;                   \
    ANSC_HANDLE                         hIrepFolderCOSA;                    \
    ANSC_HANDLE                         hIrepFolderPPPIf;                   \


typedef  struct
_COSA_DATAMODEL_PPP
{
    COSA_DATAMODEL_PPP_CLASS_CONTENT
}
COSA_DATAMODEL_PPP,  *PCOSA_DATAMODEL_PPP;

/**********************************
    Standard function declaration 
***********************************/
ANSC_HANDLE
CosaPPPCreate
    (
        VOID
    );

ANSC_STATUS
CosaPPPInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaPPPRemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaPPPIfRegGetInfo
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaPPPIfRegAddInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );

ANSC_STATUS
CosaPPPIfRegDelInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );

#endif
#endif
