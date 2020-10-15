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

    module: cosa_hosts_internal.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaHostsCreate
        *  CosaHostsInitialize
        *  CosaHostsRemove
        *  
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
#include "plugin_main_apis.h"
#include "cosa_hosts_apis.h"
#include "cosa_hosts_internal.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        CosaHostsCreate
            (
            );

    description:

        This function constructs cosa nat object and return handle.

    argument:  

    return:     newly created nat object.

**********************************************************************/

ANSC_HANDLE
CosaHostsCreate
    (
        VOID
    )
{
    PCOSA_DATAMODEL_HOSTS             pMyObject    = (PCOSA_DATAMODEL_HOSTS)NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PCOSA_DATAMODEL_HOSTS)AnscAllocateMemory(sizeof(COSA_DATAMODEL_HOSTS));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->Oid               = COSA_DATAMODEL_HOSTS_OID;
    pMyObject->Create            = CosaHostsCreate;
    pMyObject->Remove            = CosaHostsRemove;
    pMyObject->Initialize        = CosaHostsInitialize;

    pMyObject->Initialize   ((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaHostsInitialize
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
CosaHostsInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus =  ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_HOSTS           pMyObject    =  (PCOSA_DATAMODEL_HOSTS)hThisObject;

    returnStatus = CosaDmlHostsInit(NULL, NULL);
    if ( returnStatus != ANSC_STATUS_SUCCESS)
    {
        return returnStatus;
    }

    /* Initiation all functions */
    pMyObject->pHostEntry           = NULL;
    pMyObject->HostEntryCount       = 0;
    pMyObject->PreviousVisitTime    = 0;


    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaHostsRemove
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
CosaHostsRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_HOSTS             pMyObject    = (PCOSA_DATAMODEL_HOSTS)hThisObject;

    /* Remove necessary resounce */
    if( pMyObject->pHostEntry )
    {
        AnscFreeMemory(pMyObject->pHostEntry);
    }

    /* Remove self */
    AnscFreeMemory((ANSC_HANDLE)pMyObject);

    return returnStatus;
}

void * LanManager_Allocate
(
    size_t size
)
{
    //return AnscAllocateMemoryCountSize(pComponentName, size);
    return AnscAllocateMemory(size);
    //return malloc(size);
}

void LanManager_Free
(
    void *p
)
{
    //return AnscFreeMemoryCountSize(pComponentName, p);
    AnscFreeMemory(p);
    //if(p) return free(p);
}

char * LanManager_CloneString
    (
    const char * src
    )
{
    if(src == NULL) return NULL;
    size_t len = strlen(src) + 1;
    if(len <= 1) return NULL;
    char * dest = LanManager_Allocate(len);
    if ( dest )
    {
        strncpy(dest, src, len);
        dest[len - 1] = 0;
    }
    return dest;
}
