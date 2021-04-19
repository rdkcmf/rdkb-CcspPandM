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
#ifdef FEATURE_SUPPORT_ONBOARD_LOGGING
#include "cosa_x_rdkcentral_com_xpc_dml.h"
#include "cosa_x_rdkcentral_com_xpc_internal.h"
#include "dml_tr181_custom_cfg.h"
#include "ccsp_trace.h"
#include "ansc_platform.h"
#include "ccsp_base_api.h"
#include "dslh_definitions_database.h"
#include "messagebus_interface_helper.h"

extern void* g_pDslhDmlAgent;
extern ANSC_HANDLE bus_handle;

BOOL
SnmpOnboardReboot_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_XPC       pMyObject     = (PCOSA_DATAMODEL_XPC)g_pCosaBEManager->hXpc;
    if (strcmp(ParamName, "Enable") == 0)
    {
        *pBool = pMyObject->bEnable;
        return TRUE;
    }

    return FALSE;
}

BOOL
SnmpOnboardReboot_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_DATAMODEL_XPC       pMyObject     = (PCOSA_DATAMODEL_XPC)g_pCosaBEManager->hXpc;
    ANSC_STATUS  returnStatus = ANSC_STATUS_SUCCESS;
    UNREFERENCED_PARAMETER(hInsContext);
    if (strcmp(ParamName, "Enable") == 0)
    {
        CcspTraceInfo(("Write ID is %ld \n", GET_CURRENT_WRITE_ENTITY()));
        if( GET_CURRENT_WRITE_ENTITY() == DSLH_MPA_ACCESS_CONTROL_XPC )
        {
            if(bValue == pMyObject->bEnable)
                    return TRUE;
            if( bValue )
                    returnStatus = CosaXpcEnable(pMyObject);
            else
                    returnStatus = CosaXpcDisable(pMyObject);

            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                CcspTraceInfo(("%s EXIT Error\n", __FUNCTION__));
                return  returnStatus;
            }
            return TRUE;
        }
        else
        {
            CcspTraceError(("Operation not allowed\n"));
            return FALSE;
        }
    }

    return FALSE;
}
#endif
