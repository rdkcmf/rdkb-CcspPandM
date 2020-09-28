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

    module: cosa_x_cisco_com_ddns_internal.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the internal apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        Tom Chang

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**************************************************************************/
#if !defined(DDNS_BROADBANDFORUM)
#ifndef  _COSA_X_CISCO_COM_DDNS_INTERNAL_H
#define  _COSA_X_CISCO_COM_DDNS_INTERNAL_H

#include "cosa_x_cisco_com_ddns_apis.h"

#define  COSA_IREP_FOLDER_NAME_DDNS                      "DDNS"
#define  COSA_DML_RR_NAME_DdnsServiceAlias               "Alias"
#define  COSA_DML_RR_NAME_DdnsServiceInsNum              "InstanceNumber"
#define  COSA_DML_RR_NAME_DdnsServiceNextInsNumber       "NextInstanceNumber"
#define  COSA_DML_RR_NAME_DdnsServiceNewlyAdded          "NewlyAdded"

#define  COSA_DATAMODEL_DDNS_CLASS_CONTENT                                                  \
    /* duplication of the base object class content */                                      \
    COSA_BASE_CONTENT                                                                       \
    /* start of DDNS object class content */                                                \
    ULONG                           ulNextInstance;                                         \
    SLIST_HEADER                    ContextHead;                                            \
    COSA_DML_DDNS_CFG               DdnsConfig;                                             \
    ANSC_HANDLE                     hIrepFolderCOSA;                                        \
    ANSC_HANDLE                     hIrepFolderDdns;                                        \


typedef  struct
_COSA_DATAMODEL_DDNS
{
	COSA_DATAMODEL_DDNS_CLASS_CONTENT
}
COSA_DATAMODEL_DDNS,  *PCOSA_DATAMODEL_DDNS;


/*
 *  Standard function declaration 
 */

ANSC_HANDLE
CosaDdnsCreate
    (
        VOID
    );

ANSC_STATUS
CosaDdnsInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaDdnsRemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaDdnsRegGetDdnsInfo
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaDdnsRegAddDdnsInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );

ANSC_STATUS
CosaDdnsRegDelDdnsInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    );

#endif
#endif
