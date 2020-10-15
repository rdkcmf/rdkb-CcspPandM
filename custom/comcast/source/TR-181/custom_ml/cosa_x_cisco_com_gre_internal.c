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
#include "cosa_x_cisco_com_gre_internal.h"
#include "cosa_x_cisco_com_gre_apis.h"

ANSC_HANDLE
CosaCGreCreate
    (
        VOID
    )
{
    COSA_DATAMODEL_CGRE         *pMyObject   = NULL;

    pMyObject = AnscAllocateMemory(sizeof(COSA_DATAMODEL_CGRE));
    if (!pMyObject)
    {
        return NULL;
    }

    pMyObject->Oid               = COSA_DATAMODEL_CGRE_OID;
    pMyObject->Create            = CosaCGreCreate;
    pMyObject->Remove            = CosaCGreRemove;
    pMyObject->Initialize        = CosaCGreInitialize;

    pMyObject->Initialize((ANSC_HANDLE)pMyObject);
    return (ANSC_HANDLE)pMyObject;
}

ANSC_STATUS
CosaCGreInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    COSA_DATAMODEL_CGRE             *cgre = (COSA_DATAMODEL_CGRE *)hThisObject;
    ULONG                           nextIns;
    char                            alias[65];
    int                             i;

    if (CosaDml_CGreInit() != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;

    memset(cgre->GreIf, 0, sizeof(cgre->GreIf));

    cgre->IfCnt = CosaDml_CGreIfGetNumberOfEntries();
    nextIns = 1;

    for (i = 0; i < cgre->IfCnt; i++)
    {
        if (CosaDml_CGreIfGetEntryByIndex(i, &cgre->GreIf[i]) != 0)
            return ANSC_STATUS_FAILURE;

        if (cgre->GreIf[i].InstanceNumber == 0)
        {
            cgre->GreIf[i].InstanceNumber = nextIns;
            snprintf(alias, sizeof(alias), "cpe-cgreif-%lu", nextIns);
            CosaDml_CGreIfSetInsAlias(i, nextIns, alias);
            nextIns++;
        }
        else if (nextIns <= cgre->GreIf[i].InstanceNumber)
        {
            nextIns = cgre->GreIf[i].InstanceNumber + 1;
        }
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaCGreRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    COSA_DATAMODEL_CGRE             *cgre = (COSA_DATAMODEL_CGRE *)hThisObject;

    if (CosaDml_CGreFinalize() != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;

    if (cgre)
        AnscFreeMemory(cgre);
    return ANSC_STATUS_SUCCESS;
}


#endif
