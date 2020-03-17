/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2016 RDK Management
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

#ifndef  _COSA_RABID_INTERNAL_H
#define  _COSA_RABID_INTERNAL_H

#include "ansc_platform.h"
#include "ansc_string_util.h"

typedef  struct
_COSA_DATAMODEL_RABID
{
    BOOL                        bEnable;
    ULONG                       uMemoryLimit;
    ULONG                       uMacCacheSize;
    int         	iStatus;
    int             iState;
}
COSA_DATAMODEL_RABID,  *PCOSA_DATAMODEL_RABID;

/*
    Standard function declaration 
*/
ANSC_HANDLE
CosaRabidCreate
    (
        VOID
    );

ANSC_STATUS
CosaRabidInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaRabidRemove
    (
        ANSC_HANDLE                 hThisObject
    );
#endif 
