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

    module: cosa_x_cisco_com_mld_apis.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    copyright:

        Cisco Systems, Inc.
        All Rights Reserved.

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaDmlNatGetPortMappingNumber
    -------------------------------------------------------------------

    environment:

        platform independent

    -------------------------------------------------------------------

    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**************************************************************************/
#include "cosa_apis.h"
#include "cosa_x_cisco_com_diagnostics_apis.h"
#include "cosa_x_cisco_com_diagnostics_internal.h"
#include "plugin_main_apis.h"

#define COSA_DML_DIAGNOSTICS_WARNING       "jRecord/dslh_objro_management.c:265 com.cisco.spvtg.ccsp.pam End of the repopulation in 'Client'"
#define COSA_DML_DIAGNOSTICS_ERROR         "com.cisco.spvtg.ccsp.pam CcspCcMbi_GetParameterValues -- Something has screwed up. Failure status returned"
#define COSA_DML_DIAGNOSTICS_NOTICE        "cmd deconfig interface erouter0 ip broadcast subnet router."

COSA_DML_DIAGNOSTICS_ENTRY  g_Entry[3] = 
    {
        {"2012-08-10T10:36:20", "1233", COSA_DML_DIAGNOSTICS_LEVEL_WARNING, NULL},
        {"2012-08-10T10:36:21", "1234", COSA_DML_DIAGNOSTICS_LEVEL_ERROR  , NULL},
        {"2012-08-10T10:36:22", "1235", COSA_DML_DIAGNOSTICS_LEVEL_NOTICE , NULL}
    };


ANSC_STATUS
CosaDmlDiagnosticsInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlDiagnosticsGetEntry
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
        PCOSA_DML_DIAGNOSTICS_ENTRY *ppDiagnosticsEntry        
    )    
{
    ULONG                           index = 0;
    
    *pulCount                = sizeof(g_Entry)/sizeof(COSA_DML_DIAGNOSTICS_ENTRY);
    *ppDiagnosticsEntry      = AnscAllocateMemory(sizeof(g_Entry));
    AnscCopyMemory(*ppDiagnosticsEntry, &g_Entry, sizeof(g_Entry));

    for(index =0; index < *pulCount; index++)
    {
        if ( (*ppDiagnosticsEntry)[index].Level == COSA_DML_DIAGNOSTICS_LEVEL_WARNING )
        {
            (*ppDiagnosticsEntry)[index].pMessage = strdup(COSA_DML_DIAGNOSTICS_WARNING);
        }
        else if ( (*ppDiagnosticsEntry)[index].Level == COSA_DML_DIAGNOSTICS_LEVEL_ERROR )
        {
            (*ppDiagnosticsEntry)[index].pMessage = strdup(COSA_DML_DIAGNOSTICS_ERROR);
        }
        else if ( (*ppDiagnosticsEntry)[index].Level == COSA_DML_DIAGNOSTICS_LEVEL_NOTICE )
        {
            (*ppDiagnosticsEntry)[index].pMessage = strdup(COSA_DML_DIAGNOSTICS_NOTICE);
        }
    }

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
CosaDmlDiagnosticsGetEventlog
    (
        ANSC_HANDLE                    hContext,
        PULONG                         pulCount,
        PCOSA_DML_DIAGNOSTICS_EVENTLOG *ppDiagnosticsEntry        
    )    
{
    *pulCount = 0;

    return  ANSC_STATUS_SUCCESS;
}
