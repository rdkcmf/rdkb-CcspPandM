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

    module: cosa_interfacestack_internal.h

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


#ifndef  _COSA_INTERFACESTACK_INTERNAL_H
#define  _COSA_INTERFACESTACK_INTERNAL_H

#include "cosa_apis.h"

#define COSA_IFSTACK_NAME_LENGTH               256
#define COSA_IFSTACK_ALIAS_LENGTH              64


/* 
    Object definition
*/

#define  COSA_DATAMODEL_IFSTACK_CLASS_CONTENT                         \
    /* duplication of the base object class content */                         \
    COSA_BASE_CONTENT                                                    \
    /* start of InterfaceStack object class content */                           \
    SLIST_HEADER                    InterfaceStackList;                 \
    ULONG                           ulEntryCount;                       \

typedef  struct
_COSA_DATAMODEL_IFSTACK                                               
{
    COSA_DATAMODEL_IFSTACK_CLASS_CONTENT
}
COSA_DATAMODEL_IFSTACK,  *PCOSA_DATAMODEL_IFSTACK;

/*
 *  Structure definitions for InterfaceStack
 */
typedef  struct
_COSA_DML_IFSTACK_ENTRY
{
    char                            HigherLayer[COSA_IFSTACK_NAME_LENGTH];
    char                            LowerLayer[COSA_IFSTACK_NAME_LENGTH];
    char                            HigherAlias[COSA_IFSTACK_ALIAS_LENGTH];
    char                            LowerAlias[COSA_IFSTACK_ALIAS_LENGTH];
}
COSA_DML_IFSTACK_ENTRY,  *PCOSA_DML_IFSTACK_ENTRY;


/**********************************
    Standard function declaration 
***********************************/
ANSC_HANDLE
CosaIFStackCreate
    (
        VOID
    );

ANSC_STATUS
CosaIFStackInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaIFStackRemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaIFStackAddEntry
    (
    
        ANSC_HANDLE                 hThisObject,
        PCOSA_DML_IFSTACK_ENTRY     pEntry
    );

ANSC_STATUS
CosaIFStackCreateAll 
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaIFStackEmptyTable
    (
        ANSC_HANDLE                 hThisObject
    );

#endif

