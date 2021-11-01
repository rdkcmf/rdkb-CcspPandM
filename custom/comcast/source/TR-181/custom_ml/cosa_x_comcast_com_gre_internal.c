/************************************************************************************
  If not stated otherwise in this file or this component's Licenses.txt file the
  following copyright and licenses apply:

  Copyright 2018 RDK Management

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
**************************************************************************/

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

/*
 *  Where is the file header section? Taking shortcut?...
 */

#if !defined(HOTSPOT_DISABLE)
#include "dml_tr181_custom_cfg.h"

//#ifdef CONFIG_CISCO_HOTSPOT
#include "plugin_main_apis.h"
#include "cosa_x_comcast_com_gre_internal.h"

ANSC_HANDLE
CosaGreCreate
    (
        VOID
    )
{
    COSA_DATAMODEL_GRE          *pMyObject   = NULL;

    pMyObject = AnscAllocateMemory(sizeof(COSA_DATAMODEL_GRE));
    if (!pMyObject)
    {
        return NULL;
    }

    pMyObject->Oid               = COSA_DATAMODEL_GRE_OID;
    pMyObject->Create            = CosaGreCreate;
    pMyObject->Remove            = CosaGreRemove;
    pMyObject->Initialize        = CosaGreInitialize;

    pMyObject->Initialize((ANSC_HANDLE)pMyObject);
    return (ANSC_HANDLE)pMyObject;
}
ANSC_STATUS
CosaGreInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    COSA_DATAMODEL_GRE              *gre = (COSA_DATAMODEL_GRE *)hThisObject;
    ULONG                           ifCnt, i, nextIns;

	CcspTraceInfo(( "%s %d - Entry\n", __FUNCTION__, __LINE__ ));

    if (CosaDml_GreInit() != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;

    memset(gre->GreIf, 0, sizeof(COSA_DML_GRE_IF) * MAX_GRE_IF);

    ifCnt = CosaDml_GreIfGetNumberOfEntries();
    nextIns = 1;

	CcspTraceInfo(( "%s %d - GreIfCount:%lu\n", __FUNCTION__, __LINE__, ifCnt ));

    for (i = 0; i < ifCnt; i++)
    {
        if (CosaDml_GreIfGetEntryByIndex(i, &gre->GreIf[i]) != 0)
            return ANSC_STATUS_FAILURE;

        if (gre->GreIf[i].InstanceNumber == 0)
        {
            gre->GreIf[i].InstanceNumber = nextIns;
            CosaDml_GreIfSetIns(i, nextIns);
            nextIns++;
        }
        else if (nextIns <= gre->GreIf[i].InstanceNumber)
        {
            nextIns = gre->GreIf[i].InstanceNumber + 1;
        }
    }

	CcspTraceInfo(( "%s %d - Exit\n", __FUNCTION__, __LINE__ ));

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaGreRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    COSA_DATAMODEL_GRE              *gre = (COSA_DATAMODEL_GRE *)hThisObject;

    if (CosaDml_GreFinalize() != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;

    if (gre)
        AnscFreeMemory(gre);
    return ANSC_STATUS_SUCCESS;
}
//#endif
#endif
