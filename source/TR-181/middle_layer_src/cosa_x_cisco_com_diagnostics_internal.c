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

    module: cosa_x_cisco_com_diagnostics_internal.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaDiagCreate
        *  CosaDiagInitialize
        *  CosaDiagRemove
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
#include "cosa_x_cisco_com_diagnostics_internal.h"
#include "plugin_main_apis.h"
#include "safec_lib_common.h"

#define MERGED_LOG_FILE_NVRAM "/nvram/log/mergeLog.txt"
#define SORT_MERGE_LOG_FILE_NVRAM "/nvram/log/sortLog.txt"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        CosaDiagCreate
            (
            );

    description:

        This function constructs cosa nat object and return handle.

    argument:  

    return:     newly created nat object.

**********************************************************************/

ANSC_HANDLE
CosaDiagnosticsCreate
    (
        VOID
    )
{
    PCOSA_DATAMODEL_DIAGNOSTICS     pMyObject    = (PCOSA_DATAMODEL_DIAGNOSTICS)NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PCOSA_DATAMODEL_DIAGNOSTICS)AnscAllocateMemory(sizeof(COSA_DATAMODEL_DIAGNOSTICS));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->Oid               = COSA_DATAMODEL_DIAG_OID;
    pMyObject->Create            = CosaDiagnosticsCreate;
    pMyObject->Remove            = CosaDiagnosticsRemove;
    pMyObject->Initialize        = CosaDiagnosticsInitialize;

    pMyObject->Initialize   ((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDiagInitialize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate  cosa nat object and return handle.

    argument:	ANSC_HANDLE                 hThisObject
            This handle is actually the pointer of this object
            itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaDiagnosticsInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DIAGNOSTICS     pMyObject        = (PCOSA_DATAMODEL_DIAGNOSTICS)hThisObject;
	
    /* Call Initiation */
    returnStatus = CosaDmlDiagnosticsInit(NULL, NULL);
    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        return returnStatus;
    }

    /* Initiation all functions */
    pMyObject->ulDiagEntryNumber = 0;
    pMyObject->pDiagEntry        = NULL;

    system("rm -rf " MERGED_LOG_FILE_NVRAM " " SORT_MERGE_LOG_FILE_NVRAM);
	
    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDiagRemove
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate  cosa nat object and return handle.

    argument:   ANSC_HANDLE                 hThisObject
            This handle is actually the pointer of this object
            itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaDiagnosticsRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DIAGNOSTICS     pMyObject    = (PCOSA_DATAMODEL_DIAGNOSTICS)hThisObject;
    ULONG                           index        = 0;
    
    if ( pMyObject->pDiagEntry )
    {
        for( index = 0; index < pMyObject->ulDiagEntryNumber; index++ )
        {
            if ( pMyObject->pDiagEntry[index].pMessage )
            {
                /* We must use free() here. */
                free(pMyObject->pDiagEntry[index].pMessage);
            }
        }

        AnscFreeMemory(pMyObject->pDiagEntry);
    }
    
    /* Remove self */
    AnscFreeMemory((ANSC_HANDLE)pMyObject);

    return returnStatus;
}

