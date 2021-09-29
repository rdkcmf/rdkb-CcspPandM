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

/**************************************************************************

    module: cosa_advsec_utils.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implements back-end apis for the COSA Data Model Library

**************************************************************************/

#include "cosa_advsec_utils.h"
#include "syscfg/syscfg.h"
#include "safec_lib_common.h"

ANSC_STATUS CosaGetSysCfgUlong(char* setting, ULONG* value)
{
    ANSC_STATUS         ret = ANSC_STATUS_SUCCESS;
    char buf[32];
    memset(buf, 0, sizeof(buf));
    if(ANSC_STATUS_SUCCESS != (ret=syscfg_get( NULL, setting, buf, sizeof(buf))))
    {
        CcspTraceWarning(("syscfg_get failed\n"));
    }
    else
    {
        *value = atoi(buf);
    }
    return ret;
}

ANSC_STATUS CosaSetSysCfgUlong(char* setting, ULONG value)
{
    ANSC_STATUS         ret = ANSC_STATUS_SUCCESS;
    char buf[32];
    errno_t rc = -1;

    rc = sprintf_s(buf, sizeof(buf), "%lu",value);
    if(rc < EOK)
    {
       ERR_CHK(rc);
       return ANSC_STATUS_FAILURE;
    }

    if(ANSC_STATUS_SUCCESS != (ret=syscfg_set_commit( NULL, setting, buf)))
    {
        CcspTraceWarning(("syscfg_set failed\n"));
    }

    return ret;
}

ANSC_STATUS CosaGetSysCfgString(char* setting, char* pValue, PULONG pulSize )
{
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    if(ANSC_STATUS_SUCCESS == syscfg_get( NULL, setting, buf, sizeof(buf)))
    {
      /*CID: 59107 Array compared against 0*/
      /*CID: 59338 Logically dead code*/
       strncpy(pValue ,buf,strlen(buf));
       *pulSize = AnscSizeOfString(pValue);
       return ANSC_STATUS_SUCCESS;
    }
    else
            return ANSC_STATUS_FAILURE;
}

ANSC_STATUS CosaSetSysCfgString( char* setting, char* pValue )
{
        if ((syscfg_set_commit(NULL, setting, pValue) != 0))
        {
            AnscTraceWarning(("syscfg_set failed\n"));
            return ANSC_STATUS_FAILURE;
        }
        else
        {
            return ANSC_STATUS_SUCCESS;
        }
}

