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

    module: cosa_time_internal.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaTimeCreate
        *  CosaTimeInitialize
        *  CosaTimeRemove
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

#include "cosa_time_internal.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        CosaTimeCreate
            (
            );

    description:

        This function constructs cosa Time object and return handle.

    argument:  

    return:     newly created Time object.

**********************************************************************/

ANSC_HANDLE
CosaTimeCreate
    (
        VOID
    )
{
    PCOSA_DATAMODEL_TIME  pMyObject    = (PCOSA_DATAMODEL_TIME)NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PCOSA_DATAMODEL_TIME)AnscAllocateMemory(sizeof(COSA_DATAMODEL_TIME));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->Oid               = COSA_DATAMODEL_TIME_OID;
    pMyObject->Create            = CosaTimeCreate;
    pMyObject->Remove            = CosaTimeRemove;
    pMyObject->Initialize        = CosaTimeInitialize;

    pMyObject->Initialize   ((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaTimeInitialize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate  cosa Time object and return handle.

    argument:   ANSC_HANDLE                 hThisObject
            This handle is actually the pointer of this object
            itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaTimeInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS               returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_TIME      pMyObject    = (PCOSA_DATAMODEL_TIME)hThisObject;

    /* Initiation all functions */
    _ansc_memset(&pMyObject->TimeCfg, 0, sizeof(COSA_DML_TIME_CFG));
    _ansc_memset(&pMyObject->TimeStatus, 0, sizeof(COSA_DML_TIME_STATUS));
    _ansc_memset(&pMyObject->CurrLocalTime, 0, sizeof(ANSC_UNIVERSAL_TIME));

    CosaDmlTimeInit(NULL, (PANSC_HANDLE)&pMyObject->TimeCfg);

    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaTimeRemove
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate  cosa Time object and return handle.

    argument:   ANSC_HANDLE                 hThisObject
            This handle is actually the pointer of this object
            itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaTimeRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS               returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_TIME      pMyObject    = (PCOSA_DATAMODEL_TIME)hThisObject;

    /* Remove necessary resounce */


    /* Remove self */
    AnscFreeMemory((ANSC_HANDLE)pMyObject);

    return returnStatus;
}
