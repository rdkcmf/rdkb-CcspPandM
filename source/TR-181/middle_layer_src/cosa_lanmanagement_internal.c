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

    module: cosa_lanmanagement_internal.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaLanManagementCreate
        *  CosaLanManagementInitialize
        *  CosaLanManagementRemove
    -------------------------------------------------------------------

    environment:

        platform independent

    -------------------------------------------------------------------

    author:


    -------------------------------------------------------------------

    revision:

        05/09/2019    initial revision.

**************************************************************************/
#if defined(CUSTOM_ULA)
#include "cosa_lanmanagement_internal.h"
#include "cosa_lanmanagement_apis.h"

ANSC_HANDLE
CosaLanManagementCreate
    (
        VOID
    )
{
    PCOSA_DATAMODEL_LANMANAGEMENT  pMyObject = (PCOSA_DATAMODEL_LANMANAGEMENT) NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PCOSA_DATAMODEL_LANMANAGEMENT)AnscAllocateMemory(sizeof(COSA_DATAMODEL_LANMANAGEMENT));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->Oid               = COSA_DATAMODEL_LANMANAGEMENT_OID;
    pMyObject->Create            = CosaLanManagementCreate;
    pMyObject->Remove            = CosaLanManagementRemove;
    pMyObject->Initialize        = CosaLanManagementInitialize;

    pMyObject->Initialize   ((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}

ANSC_STATUS
CosaLanManagementInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS               returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_LANMANAGEMENT      pMyObject    = (PCOSA_DATAMODEL_LANMANAGEMENT)hThisObject;

    /* Initiation all functions */
    _ansc_memset(&pMyObject->LanMngmCfg, 0, sizeof(COSA_DML_LANMANAGEMENT_CFG));

    CosaDmlLanManagementInit(NULL, (PANSC_HANDLE)&pMyObject->LanMngmCfg);

    return returnStatus;
}

ANSC_STATUS
CosaLanManagementRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS               returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_LANMANAGEMENT      pMyObject    = (PCOSA_DATAMODEL_LANMANAGEMENT)hThisObject;

    /* Remove necessary resounce */


    /* Remove self */
    AnscFreeMemory((ANSC_HANDLE)pMyObject);

    return returnStatus;
}
#endif
