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

    module: cosa_userinterface_internal.h

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

#ifndef _COSA_USERINTERFACE_INTERNAL_H
#define _COSA_USERINTERFACE_INTERNAL_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "cosa_userinterface_apis.h"
#include "poam_irepfo_interface.h"
#include "sys_definitions.h"

#define  COSA_IREP_FOLDER_NAME_UserInterface_IPRange    "UserInterfaceIPRange"
#define  COSA_DML_RR_NAME_UserInterface_IPRangeAlias    "Alias"
#define  COSA_DML_RR_NAME_UserInterface_IPRangeInsNum   "InstanceNumber"
#define  COSA_DML_RR_NAME_UserInterface_IPRangeNextInsNunmber "NextInstanceNumber"
#define  COSA_DML_RR_NAME_UserInterface_IPRangeNewlyAdded     "NewlyAdded"

#define  COSA_DATAMODEL_USERINTERFACE_CLASS_CONTENT                         \
    /* duplication of the base object class content */                      \
    COSA_BASE_CONTENT                                                       \
    /* start of Time object class content */                                \
    COSA_DML_RA_CFG                 RaCfg;                                  \
    COSA_DML_USERINTERFACE_CFG      UserInterfaceCfg;                       \
    SLIST_HEADER                    iprangeList;                            \
    ULONG                           uliprangeNextInsNum;                    \
    ANSC_HANDLE                     hIrepFolderCOSA;                        \
    ANSC_HANDLE                     hIrepFolderUIIPRange;                   \
    

typedef  struct
_COSA_DATAMODEL_USERINTERFACE_CLASS_CONTENT
{
    COSA_DATAMODEL_USERINTERFACE_CLASS_CONTENT
}
COSA_DATAMODEL_USERINTERFACE, *PCOSA_DATAMODEL_USERINTERFACE;

/*
    Standard function declaration
*/
ANSC_HANDLE
CosaUserinterfaceCreate
    (
        VOID
    );

ANSC_STATUS
CosaUserinterfaceInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaUserinterfaceRemove
    (
        ANSC_HANDLE                 hThisObject
    );

#endif
