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

    module: cosa_upnp_internal.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaUpnpCreate
        *  CosaUpnpInitialize
        *  CosaUpnpRemove
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

#include "cosa_upnp_internal.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        CosaUpnpCreate
            (
            );

    description:

        This function constructs cosa Time object and return handle.

    argument:  

    return:     newly created Time object.

**********************************************************************/

ANSC_HANDLE
CosaUpnpCreate
    (
        VOID
    )
{
    PCOSA_DATAMODEL_UPNP  pMyObject    = (PCOSA_DATAMODEL_UPNP)NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PCOSA_DATAMODEL_UPNP)AnscAllocateMemory(sizeof(COSA_DATAMODEL_UPNP));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->Oid               = COSA_DATAMODEL_UPNP_OID;
    pMyObject->Create            = CosaUpnpCreate;
    pMyObject->Remove            = CosaUpnpRemove;
    pMyObject->Initialize        = CosaUpnpInitialize;

    pMyObject->Initialize   ((ANSC_HANDLE)pMyObject);

    CosaDmlUpnpInit(NULL, NULL);

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
CosaUpnpInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS               returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_UPNP      pMyObject    = (PCOSA_DATAMODEL_UPNP)hThisObject;
    ULONG                     val          = 1;

    /* Initiation all functions */
    CosaDmlUpnpInit(NULL, NULL);
    
    pMyObject->bUpnpDevEnable               = CosaDmlUpnpDevGetState(NULL);
    pMyObject->bUpnpDevMediaServerEnable    = CosaDmlUpnpDevGetMediaServerState(NULL);
    pMyObject->bUpnpDevIgdEnable            = CosaDmlUpnpDevGetIgdState(NULL);
    pMyObject->uUpnpIgd                     = val;

    CosaDmlUpnpDevGetArchVer(NULL, &pMyObject->uMajorVer, &pMyObject->uMinorVer);
    
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
CosaUpnpRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS               returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_UPNP      pMyObject    = (PCOSA_DATAMODEL_UPNP)hThisObject;

    /* Remove necessary resounce */


    /* Remove self */
    AnscFreeMemory((ANSC_HANDLE)pMyObject);

    return returnStatus;
}


