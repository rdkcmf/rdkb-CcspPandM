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

    module: cosa_x_cisco_com_mld_internal.h

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


#ifndef  _COSA_DIAG_INTERNAL_H
#define  _COSA_DIAG_INTERNAL_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "cosa_x_cisco_com_diagnostics_apis.h"

/***********************************
    Actual definition declaration
************************************/

#define   DIAG_MAXINSTANCE                      16    

#define  COSA_DATAMODEL_DIAGNOSTICS_CLASS_CONTENT                                           \
    /* duplication of the base object class content */                                                \
    COSA_BASE_CONTENT                                                                       \
    /* start of NAT object class content */                                                        \
    ULONG                           ulDiagEntryNumber;                                      \
    PCOSA_DML_DIAGNOSTICS_ENTRY     pDiagEntry;                                             \
    ULONG                           ulDiagEventlogNumber;                                   \
    PCOSA_DML_DIAGNOSTICS_ENTRY     pDiagEventlog;                                          \


typedef  struct
_COSA_DATAMODEL_DIAGNOSTICS
{
    COSA_DATAMODEL_DIAGNOSTICS_CLASS_CONTENT
}
COSA_DATAMODEL_DIAGNOSTICS,  *PCOSA_DATAMODEL_DIAGNOSTICS;


/**********************************
    Standard function declaration 
***********************************/
ANSC_HANDLE
CosaDiagnosticsCreate
    (
        VOID
    );

ANSC_STATUS
CosaDiagnosticsInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaDiagnosticsRemove
    (
        ANSC_HANDLE                 hThisObject
    );

#endif
