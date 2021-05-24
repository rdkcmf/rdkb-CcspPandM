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

    module: cosa_firewall_internal.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the internal apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        Tom Chang

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**************************************************************************/

#include "cosa_firewall_internal.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        CosaFirewallCreate
            (
                VOID
            );

    description:

        This function constructs cosa Firewall object and return handle.

    argument:  

    return:     newly created Firewall object.

**********************************************************************/

ANSC_HANDLE
CosaFirewallCreate
    (
        VOID
    )
{
    PCOSA_DATAMODEL_FIREWALL        pMyObject    = (PCOSA_DATAMODEL_FIREWALL)NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PCOSA_DATAMODEL_FIREWALL)AnscAllocateMemory(sizeof(COSA_DATAMODEL_FIREWALL));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->Oid               = COSA_DATAMODEL_FIREWALL_OID;
    pMyObject->Create            = CosaFirewallCreate;
    pMyObject->Remove            = CosaFirewallRemove;
    pMyObject->Initialize        = CosaFirewallInitialize;

    pMyObject->Initialize   ((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    prototype:

        ANSC_STATUS
        CosaFirewallInitialize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate  cosa Firewall object and return handle.

    argument:	ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaFirewallInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_FIREWALL        pMyObject       = (PCOSA_DATAMODEL_FIREWALL)hThisObject;
    PCOSA_DML_FIREWALL_CFG          pFirewallCfg    = NULL;
    errno_t                         rc              = -1;

    returnStatus = CosaDmlFirewallInit(NULL, NULL);

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        return returnStatus;
    }

    /* Initiation all functions */
    pFirewallCfg = AnscAllocateMemory(sizeof(COSA_DML_FIREWALL_CFG));

    if ( !pFirewallCfg )
    {
        CcspTraceWarning(("CosaFirewallInitialize -- Resource allocation error\n"));

        return FALSE;
    }

    returnStatus = CosaDmlFirewallGetConfig(NULL, pFirewallCfg);

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        pMyObject->FirewallConfig.FirewallLevel = pFirewallCfg->FirewallLevel;

        rc = strcpy_s(pMyObject->FirewallConfig.Version, sizeof(pMyObject->FirewallConfig.Version),pFirewallCfg->Version);
        ERR_CHK(rc);
        rc = strcpy_s(pMyObject->FirewallConfig.LastChange, sizeof(pMyObject->FirewallConfig.LastChange),pFirewallCfg->LastChange);
        ERR_CHK(rc);
    }

    AnscFreeMemory(pFirewallCfg);

    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaFirewallRemove
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate  cosa Firewall object and return handle.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaFirewallRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_FIREWALL        pMyObject    = (PCOSA_DATAMODEL_FIREWALL)hThisObject;

	/* Remove necessary resounce */


    /* Remove self */
    AnscFreeMemory((ANSC_HANDLE)pMyObject);

	return returnStatus;
}
