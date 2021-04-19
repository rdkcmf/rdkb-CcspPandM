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
/****************************************************************************
  Copyright 2016-2018 Intel Corporation

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
******************************************************************************/

#ifdef INTEL_GRE_HOTSPOT

#ifndef  _COSA_GRE_INTERNAL_H
#define  _COSA_GRE_INTERNAL_H

#include "cosa_gre_apis.h"

#define COSA_DATAMODEL_GRE_CLASS_CONTENT                \
    COSA_BASE_CONTENT                                   \
    SLIST_HEADER               GRETunnelList;           \
    ULONG                      ulNextGRETunnelInstance; \

typedef  struct
_COSA_DATAMODEL_GRE
{
    COSA_DATAMODEL_GRE_CLASS_CONTENT
}
COSA_DATAMODEL_GRE, *PCOSA_DATAMODEL_GRE;

/*
    Standard function declaration
*/
ANSC_HANDLE
CosaGRECreate
    (
        VOID
    );

ANSC_STATUS
CosaGREInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaGRERemove
    (
        ANSC_HANDLE                 hThisObject
    );

#endif

#endif
