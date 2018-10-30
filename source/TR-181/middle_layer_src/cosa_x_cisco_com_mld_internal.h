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


#ifndef  _COSA_MLD_INTERNAL_H
#define  _COSA_MLD_INTERNAL_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "cosa_x_cisco_com_mld_apis.h"

/***********************************
    Actual definition declaration
************************************/

#define   MLD_MAXINSTANCE                      16    

#define  COSA_DATAMODEL_MLD_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                                \
    COSA_BASE_CONTENT                                                                       \
    /* start of NAT object class content */                                                        \
    COSA_DML_MLD                    MldFull;                                                \
    ULONG                           ulMldGroupNumber;                                       \
    COSA_DML_MLD_GROUP              MldGroup[MLD_MAXINSTANCE];                               \


typedef  struct
_COSA_DATAMODEL_MLD
{
    COSA_DATAMODEL_MLD_CLASS_CONTENT
}
COSA_DATAMODEL_MLD,  *PCOSA_DATAMODEL_MLD;


/**********************************
    Standard function declaration 
***********************************/
ANSC_HANDLE
CosaMldCreate
    (
        VOID
    );

ANSC_STATUS
CosaMldInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaMldRemove
    (
        ANSC_HANDLE                 hThisObject
    );

#endif
