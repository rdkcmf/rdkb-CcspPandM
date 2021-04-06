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

    module: cosa_userinterface_internal.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaUserinterfaceCreate
        *  CosaUserinterfaceInitialize
        *  CosaUserinterfaceRemove
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

#include "cosa_userinterface_internal.h"
#include "safec_lib_common.h"

extern void* g_pDslhDmlAgent;

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        CosaUserinterfaceCreate
            (
            );

    description:

        This function constructs cosa Userinterface object and return handle.

    argument:  

    return:     newly created Userinterface object.

**********************************************************************/

ANSC_HANDLE
CosaUserinterfaceCreate
    (
        VOID
    )
{
    PCOSA_DATAMODEL_USERINTERFACE   pMyObject    = (PCOSA_DATAMODEL_USERINTERFACE)NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PCOSA_DATAMODEL_USERINTERFACE)AnscAllocateMemory(sizeof(COSA_DATAMODEL_USERINTERFACE));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->Oid               = COSA_DATAMODEL_USERINTERFACE_OID;
    pMyObject->Create            = CosaUserinterfaceCreate;
    pMyObject->Remove            = CosaUserinterfaceRemove;
    pMyObject->Initialize        = CosaUserinterfaceInitialize;

    pMyObject->Initialize   ((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaUserinterfaceInitialize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate  cosa Userinterface object and return handle.

    argument:   ANSC_HANDLE                 hThisObject
            This handle is actually the pointer of this object
            itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaUserinterfaceInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                         returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_USERINTERFACE       pMyObject    = (PCOSA_DATAMODEL_USERINTERFACE)hThisObject;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoCOSA        = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PPOAM_IREP_FOLDER_OBJECT        hIrepFolderUIIPRange   = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PSLAP_VARIABLE                  pSlapVariable          = (PSLAP_VARIABLE          )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext           = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_UI_IPRANGE_ENTRY      pEntry                 = (PCOSA_DML_UI_IPRANGE_ENTRY)NULL;
    ULONG                           ulEntryCount           = 0;
    ULONG                           ulIndex                = 0;
    errno_t                         rc                     = -1;

    /* Initiation all functions */
    _ansc_memset(&pMyObject->RaCfg, 0, sizeof(COSA_DML_RA_CFG));
    _ansc_memset(&pMyObject->UserInterfaceCfg, 0, sizeof(COSA_DML_USERINTERFACE_CFG));
    
    CosaDmlRaInit(NULL, NULL);

    CosaDmlRaGetCfg(NULL, &pMyObject->RaCfg);
    CosaDmlUserInterfaceGetCfg(NULL, &pMyObject->UserInterfaceCfg);

    pMyObject->hIrepFolderCOSA = g_GetRegistryRootFolder(g_pDslhDmlAgent);


    /* Initiation Device.UserInterface.X_CISCO_COM_RemoteAccess.iprange.{i}. */

    AnscSListInitializeHeader(&pMyObject->iprangeList);

    pMyObject->uliprangeNextInsNum = 1;

    pPoamIrepFoCOSA = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderCOSA;
    if ( !pPoamIrepFoCOSA )
    {
        return ANSC_STATUS_FAILURE;
    }

    hIrepFolderUIIPRange =
        (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoCOSA->GetFolder
            (
                (ANSC_HANDLE)pPoamIrepFoCOSA,
                COSA_IREP_FOLDER_NAME_UserInterface_IPRange
            );

    if ( !hIrepFolderUIIPRange )
    {
        hIrepFolderUIIPRange =
            pPoamIrepFoCOSA->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoCOSA,
                    COSA_IREP_FOLDER_NAME_UserInterface_IPRange,
                    0
                );
    }

    if ( !hIrepFolderUIIPRange )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pMyObject->hIrepFolderUIIPRange = (ANSC_HANDLE)hIrepFolderUIIPRange;
    }

    /* Retrieve Next Instance Number */
    
    if ( TRUE )
    {
        pSlapVariable =
            (PSLAP_VARIABLE)hIrepFolderUIIPRange->GetRecord
                (
                    (ANSC_HANDLE)hIrepFolderUIIPRange,
                    COSA_DML_RR_NAME_UserInterface_IPRangeNextInsNunmber,
                    NULL
                );

        if ( pSlapVariable )
        {
            pMyObject->uliprangeNextInsNum = pSlapVariable->Variant.varUint32;

            SlapFreeVariable(pSlapVariable);
        }
    }

    /* Initialize middle layer */

    ulEntryCount = CosaDmlUIIPRangeGetNumberOfEntries(NULL);

    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        pEntry = (PCOSA_DML_UI_IPRANGE_ENTRY)AnscAllocateMemory(sizeof(COSA_DML_UI_IPRANGE_ENTRY));

        if ( !pEntry )
        {
            return ANSC_STATUS_RESOURCES;
        }
        
        CosaDmlUIIPRangeGetEntry(NULL, ulIndex, pEntry);

        if ( TRUE )
        {
            pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

            if ( !pCosaContext )
            {
                AnscFreeMemory(pEntry);
                
                return ANSC_STATUS_RESOURCES;
            }

            if ( pEntry->InstanceNumber != 0 )
            {
                pCosaContext->InstanceNumber = pEntry->InstanceNumber;

                if ( pMyObject->uliprangeNextInsNum <= pEntry->InstanceNumber )
                {
                    pMyObject->uliprangeNextInsNum = pEntry->InstanceNumber + 1;

                    if ( pMyObject->uliprangeNextInsNum == 0 )
                    {
                        pMyObject->uliprangeNextInsNum = 1;
                    }
                }
            }
            else
            {
                pEntry->InstanceNumber = pCosaContext->InstanceNumber = pMyObject->uliprangeNextInsNum;

                pMyObject->uliprangeNextInsNum++;

                if ( pMyObject->uliprangeNextInsNum == 0 )
                {
                    pMyObject->uliprangeNextInsNum = 1;
                }

                /* Generate Alias */
                rc = sprintf_s(pEntry->Alias, sizeof(pEntry->Alias),"iprange%d", (int)pCosaContext->InstanceNumber);
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                  AnscFreeMemory(pEntry);
                  AnscFreeMemory(pCosaContext);
                  return ANSC_STATUS_FAILURE;
                }

                CosaDmlUIIPRangeSetValues
                (
                    NULL,
                    ulIndex,
                    pCosaContext->InstanceNumber,
                    pEntry->Alias
                );
            }

            pCosaContext->hContext      = (ANSC_HANDLE)pEntry;
            pCosaContext->hParentTable  = NULL;
            pCosaContext->bNew          = FALSE;
            pCosaContext->hPoamIrepUpperFo = NULL;
            pCosaContext->hPoamIrepFo      = NULL;

            CosaSListPushEntryByInsNum(&pMyObject->iprangeList, pCosaContext);
        }
    }
    
    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaUserinterfaceRemove
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate  cosa Userinterface object and return handle.

    argument:   ANSC_HANDLE                 hThisObject
            This handle is actually the pointer of this object
            itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaUserinterfaceRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                         returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_USERINTERFACE       pMyObject    = (PCOSA_DATAMODEL_USERINTERFACE)hThisObject;

    /* Remove necessary resounce */


    /* Remove self */
    AnscFreeMemory((ANSC_HANDLE)pMyObject);

    return returnStatus;
}

