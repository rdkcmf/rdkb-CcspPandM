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
#include "plugin_main_apis.h"
#include "cosa_x_cisco_com_hotspot_internal.h"

ANSC_HANDLE
CosaHotspotCreate
    (
        VOID
    )
{
    COSA_DATAMODEL_HOTSPOT      *pMyObject   = NULL;

    pMyObject = AnscAllocateMemory(sizeof(COSA_DATAMODEL_HOTSPOT));
    if (!pMyObject)
    {
        return NULL;
    }

    pMyObject->Oid               = COSA_DATAMODEL_HOTSPOT_OID;
    pMyObject->Create            = CosaHotspotCreate;
    pMyObject->Remove            = CosaHotspotRemove;
    pMyObject->Initialize        = CosaHotspotInitialize;

    pMyObject->Initialize((ANSC_HANDLE)pMyObject);
    return (ANSC_HANDLE)pMyObject;
}

ANSC_STATUS
CosaHotspotInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    COSA_DATAMODEL_HOTSPOT          *hs = (COSA_DATAMODEL_HOTSPOT *)hThisObject;
    int                             i, j;
    COSA_DML_HOTSPOT_SSID           *hsSsid;

    if (CosaDml_HotspotInit() != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;

    memset(hs->HsSsids, 0, sizeof(hs->HsSsids));

    hs->SsidCnt = CosaDml_HsSsidGetNumberOfEntries();

    for (i = 0; i < hs->SsidCnt; i++)
    {
        hsSsid = &hs->HsSsids[i];

        if (CosaDml_HsSsidGetEntryByIndex(i, hsSsid) != ANSC_STATUS_SUCCESS)
            return ANSC_STATUS_FAILURE;
        hsSsid->InstanceNumber = i + 1;

        hsSsid->DevCnt = CosaDml_HsSsidAssoDevGetNumberOfEntries(hsSsid->InstanceNumber);

        for (j = 0; j < hsSsid->DevCnt; j++) {
            if (CosaDml_HsSsidAssoDevGetEntryByIndex(hsSsid->InstanceNumber, j, 
                        &hsSsid->AssoDevs[j]) != ANSC_STATUS_SUCCESS)
                return ANSC_STATUS_FAILURE;
            hsSsid->AssoDevs[j].InstanceNumber = j;
        }

    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaHotspotRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    COSA_DATAMODEL_HOTSPOT          *hs = (COSA_DATAMODEL_HOTSPOT *)hThisObject;

    if (CosaDml_HotspotFinalize() != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;

    if (hs)
        AnscFreeMemory(hs);
    return ANSC_STATUS_SUCCESS;
}


#endif
