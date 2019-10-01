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

/**************************************************************************

    module: cosa_lanmanagement_apis.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaDmlLanManagementInit
        *  CosaDmlLanManagementSetCfg
        *  CosaDmlLanManagementGetCfg
        *  CosaDmlLanMngm_SetLanIpv6Enable
        *  CosaDmlLanMngm_SetLanIpv6UlaEnable
        *  CosaDmlLanMngm_SetLanIpv6Ula

    -------------------------------------------------------------------

    environment:

        platform independent

    -------------------------------------------------------------------

    author:


    -------------------------------------------------------------------

    revision:

        05/9/2019    initial revision.

**************************************************************************/

#ifndef  _COSA_LANMANAGEMENT_API_H
#define  _COSA_LANMANAGEMENT_API_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"

/**********************************************************************
                STRUCTURE AND CONSTANT DEFINITIONS
**********************************************************************/

typedef  struct
_COSA_DML_LANMANAGEMENT_CFG
{
    BOOLEAN                         LanIpv6Enable;
	BOOLEAN                         LanIpv6UlaEnable;
    char                            LanIpv6Ula[64];
}
COSA_DML_LANMANAGEMENT_CFG,  *PCOSA_DML_LANMANAGEMENT_CFG;

/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/

ANSC_STATUS
CosaDmlLanManagementInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

ANSC_STATUS
CosaDmlLanManagementSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_LANMANAGEMENT_CFG          pTimeCfg
    );

ANSC_STATUS
CosaDmlLanManagementGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_LANMANAGEMENT_CFG          pTimeCfg
    );

#endif
