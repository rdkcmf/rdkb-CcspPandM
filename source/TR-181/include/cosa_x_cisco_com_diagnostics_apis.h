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

/**************************************************************************

    module: cosa_x_cisco_com_Diag_apis.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**************************************************************************/


#ifndef  _COSA_DIAGNOSTICS_APIS_H
#define  _COSA_DIAGNOSTICS_APIS_H

#include "cosa_apis.h"


/***********************************
    Actual definition declaration
************************************/

/*
    Nat Part
*/
typedef  enum
_COSA_DML_DIAGNOSTICS_LEVEL
{
    COSA_DML_DIAGNOSTICS_LEVEL_EMERGENCY                = 1,
    COSA_DML_DIAGNOSTICS_LEVEL_ALERT,
    COSA_DML_DIAGNOSTICS_LEVEL_CRITICAL,
    COSA_DML_DIAGNOSTICS_LEVEL_ERROR,
    COSA_DML_DIAGNOSTICS_LEVEL_WARNING,
    COSA_DML_DIAGNOSTICS_LEVEL_NOTICE,
    COSA_DML_DIAGNOSTICS_LEVEL_INFORMATIONAL,
    COSA_DML_DIAGNOSTICS_LEVEL_DEBUG
}
COSA_DML_DIAGNOSTICS_LEVEL, *PCOSA_DML_DIAGNOSTICS_LEVEL;

typedef  struct
_COSA_DML_DIAGNOSTICS_ENTRY
{ 
    CHAR                            Time[64];
    CHAR                            Tag[16];
    COSA_DML_DIAGNOSTICS_LEVEL      Level;
    PCHAR                           pMessage;     /* The memory is supposed to be created by system malloc(). 
                                                                                            So it will be freed by offical system free() by caller. */
}
COSA_DML_DIAGNOSTICS_ENTRY,  *PCOSA_DML_DIAGNOSTICS_ENTRY;

#if 0
typedef  struct
_COSA_DML_DIAGNOSTICS_EVENTLOG
{ 
    CHAR                            Time[64];
    CHAR                            Tag[16];
    COSA_DML_DIAGNOSTICS_LEVEL      Level;
    PCHAR                           pMessage;     /* The memory is supposed to be created by system malloc(). 
                                                                                            So it will be freed by offical system free() by caller. */
}
COSA_DML_DIAGNOSTICS_EVENTLOG,  *PCOSA_DML_DIAGNOSTICS_EVENTLOG;
#endif

/*************************************
    The actual function declaration 
**************************************/
ANSC_STATUS
CosaDmlDiagnosticsInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

ANSC_STATUS
CosaDmlDiagnosticsGetEntry
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
        PCOSA_DML_DIAGNOSTICS_ENTRY *ppDiagEntry        
    );

ANSC_STATUS
CosaDmlDiagnosticsGetEventlog
    (
        ANSC_HANDLE                    hContext,
        PULONG                         pulCount,
        PCOSA_DML_DIAGNOSTICS_ENTRY *ppDiagnosticsEntry        
    );

ANSC_STATUS
CosaDmlDiagnosticsGetAllEventlog
    (
        char*                          pValue,
        ULONG*                         pUlSize
    );    

ANSC_STATUS
CosaDmlDiagnosticsGetAllSyslog
    (
        char*                          pValue,
        ULONG*                         pUlSize
    );    
#endif
