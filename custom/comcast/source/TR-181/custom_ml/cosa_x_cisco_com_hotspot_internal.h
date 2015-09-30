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

#ifdef CONFIG_CISCO_HOTSPOT
#ifndef _COSA_X_CISCO_COM_HOTSPOT_INTERNAL_H
#define _COSA_X_CISCO_COM_HOTSPOT_INTERNAL_H
#include "cosa_apis.h"
#include "cosa_x_cisco_com_hotspot_apis.h"

typedef struct {
    COSA_BASE_CONTENT

    COSA_DML_HOTSPOT_SSID   HsSsids[MAX_HS_SSIDS];
    int                     SsidCnt;
} COSA_DATAMODEL_HOTSPOT;

ANSC_HANDLE
CosaHotspotCreate
    (
        VOID
    );

ANSC_STATUS
CosaHotspotInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaHotspotRemove
    (
        ANSC_HANDLE                 hThisObject
    );

#endif
#endif
