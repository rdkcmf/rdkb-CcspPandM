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

    module: cosa_ethernet_internal.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaEthernetCreate
        *  CosaEthernetInitialize
        *  CosaEthernetRemove
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

#include "cosa_ethernet_internal.h"
#include <syscfg/syscfg.h>
#include "safec_lib_common.h"

#define ONE_HR 60*60

static void CosaEthWanTelementryLogger(void);

extern void * g_pDslhDmlAgent;


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        CosaEthernetCreate
            (
            );

    description:

        This function constructs cosa Ethernet object and return handle.

    argument:  

    return:     newly created Ethernet object.

**********************************************************************/

ANSC_HANDLE
CosaEthernetCreate
    (
        VOID
    )
{
    PCOSA_DATAMODEL_ETHERNET    pMyObject    = (PCOSA_DATAMODEL_ETHERNET)NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PCOSA_DATAMODEL_ETHERNET)AnscAllocateMemory(sizeof(COSA_DATAMODEL_ETHERNET));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->Oid               = COSA_DATAMODEL_ETHERNET_OID;
    pMyObject->Create            = CosaEthernetCreate;
    pMyObject->Remove            = CosaEthernetRemove;
    pMyObject->Initialize        = CosaEthernetInitialize;

    pMyObject->Initialize   ((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaEthernetInitialize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate  cosa Ethernet object and return handle.

    argument:   ANSC_HANDLE                 hThisObject
            This handle is actually the pointer of this object
            itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaEthernetInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_ETHERNET        pMyObject           = (PCOSA_DATAMODEL_ETHERNET)hThisObject;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoCOSA     = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoEthLink  = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoEthVlanTermination = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PSLAP_VARIABLE                  pSlapVariable       = (PSLAP_VARIABLE          )NULL;
    PCOSA_DML_ETH_LINK_FULL         pEntry              = (PCOSA_DML_ETH_LINK_FULL )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext        = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    ULONG                           ulEntryCount        = 0;
    ULONG                           ulIndex             = 0;
    ULONG                           ulNextInsNum        = 0;
    errno_t                         rc                  = -1;

    /* Initiation all functions */
    CosaDmlEthInit(NULL, &pMyObject->hSbContext);
    
    _ansc_memset(pMyObject->EthernetPortFullTable, 0, sizeof(COSA_DML_ETH_PORT_FULL) * MAXINSTANCE);

    /* Initiation Device.Ethernet.Interface */

    ulEntryCount = CosaDmlEthPortGetNumberOfEntries(NULL);

    ulNextInsNum = 1;

    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        CosaDmlEthPortGetEntry(NULL, ulIndex, &pMyObject->EthernetPortFullTable[ulIndex]);

        if ( pMyObject->EthernetPortFullTable[ulIndex].Cfg.InstanceNumber == 0 )
        {
            pMyObject->EthernetPortFullTable[ulIndex].Cfg.InstanceNumber = ulNextInsNum;

            rc = sprintf_s(pMyObject->EthernetPortFullTable[ulIndex].Cfg.Alias, sizeof(pMyObject->EthernetPortFullTable[ulIndex].Cfg.Alias),"Interface%d", (int)ulNextInsNum);
            if(rc < EOK)
            {
              ERR_CHK(rc);
              return ANSC_STATUS_FAILURE;
            }

            CosaDmlEthPortSetValues(NULL, ulIndex, ulNextInsNum, pMyObject->EthernetPortFullTable[ulIndex].Cfg.Alias);

            ulNextInsNum++;
        }
        else
        {
            ulNextInsNum = pMyObject->EthernetPortFullTable[ulIndex].Cfg.InstanceNumber + 1;
        }
    }

    /* Initiation Device.Ethernet.Link */

    AnscSListInitializeHeader(&pMyObject->EthernetLinkList);

    pMyObject->ulEthernetLinkNextInstance = 1;

    pMyObject->hIrepFolderCOSA = g_GetRegistryRootFolder(g_pDslhDmlAgent);

    pPoamIrepFoCOSA = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderCOSA;
    if ( !pPoamIrepFoCOSA )
    {
        return ANSC_STATUS_FAILURE;
    }

    pPoamIrepFoEthLink =
        (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoCOSA->GetFolder
            (
                (ANSC_HANDLE)pPoamIrepFoCOSA,
                COSA_IREP_FOLDER_NAME_ETHLINK
            );

    if ( !pPoamIrepFoEthLink )
    {
        pPoamIrepFoEthLink =
            pPoamIrepFoCOSA->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoCOSA,
                    COSA_IREP_FOLDER_NAME_ETHLINK,
                    0
                );
    }

    if ( !pPoamIrepFoEthLink )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pMyObject->hIrepFolderEthLink = (ANSC_HANDLE)pPoamIrepFoEthLink;
    }

    /* Retrieve Next Instance Number */
    
    if ( TRUE )
    {
        pSlapVariable =
            (PSLAP_VARIABLE)pPoamIrepFoEthLink->GetRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoEthLink,
                    COSA_DML_RR_NAME_EthernetLinkNextInsNunmber,
                    NULL
                );

        if ( pSlapVariable )
        {
            pMyObject->ulEthernetLinkNextInstance = pSlapVariable->Variant.varUint32;

            SlapFreeVariable(pSlapVariable);
        }
    }    

    /* Initialize Device.Ethernet.VLANTermination */

    AnscSListInitializeHeader(&pMyObject->EthernetVlanTerminationList);

    pMyObject->ulEthernetVlanTerminationNextInstance = 1;

    pPoamIrepFoEthVlanTermination =
        (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoCOSA->GetFolder
            (
                (ANSC_HANDLE)pPoamIrepFoCOSA,
                COSA_IREP_FOLDER_NAME_ETHVLANTERMINATION
            );

    if ( !pPoamIrepFoEthVlanTermination )
    {
        pPoamIrepFoEthVlanTermination =
            pPoamIrepFoCOSA->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoCOSA,
                    COSA_IREP_FOLDER_NAME_ETHVLANTERMINATION,
                    0
                );
    }

    if ( !pPoamIrepFoEthVlanTermination )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pMyObject->hIrepFolderEthVlanTermination = (ANSC_HANDLE)pPoamIrepFoEthVlanTermination;
    }

    /* Retrieve Next Instance Number */
    
    if ( TRUE )
    {
        pSlapVariable =
            (PSLAP_VARIABLE)pPoamIrepFoEthVlanTermination->GetRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoEthVlanTermination,
                    COSA_DML_RR_NAME_EthernetVlanTerminationNextInsNunmber,
                    NULL
                );

        if ( pSlapVariable )
        {
            pMyObject->ulEthernetVlanTerminationNextInstance = pSlapVariable->Variant.varUint32;

            SlapFreeVariable(pSlapVariable);
        }
    }    
    /* Initialize middle layer */

    ulEntryCount = CosaDmlEthLinkGetNumberOfEntries(pMyObject->hSbContext);

    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        pEntry = (PCOSA_DML_ETH_LINK_FULL)AnscAllocateMemory(sizeof(COSA_DML_ETH_LINK_FULL));

        if ( !pEntry )
        {
            return ANSC_STATUS_RESOURCES;
        }
        
        CosaDmlEthLinkGetEntry(pMyObject->hSbContext, ulIndex, pEntry);

        if ( TRUE )
        {
            pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

            if ( !pCosaContext )
            {
                AnscFreeMemory(pEntry);
                
                return ANSC_STATUS_RESOURCES;
            }

            if ( pEntry->Cfg.InstanceNumber != 0 )
            {
                pCosaContext->InstanceNumber = pEntry->Cfg.InstanceNumber;

                if ( pMyObject->ulEthernetLinkNextInstance <= pEntry->Cfg.InstanceNumber )
                {
                    pMyObject->ulEthernetLinkNextInstance = pEntry->Cfg.InstanceNumber + 1;

                    if ( pMyObject->ulEthernetLinkNextInstance == 0 )
                    {
                        pMyObject->ulEthernetLinkNextInstance = 1;
                    }
                }
            }
            else
            {
                pEntry->Cfg.InstanceNumber = pCosaContext->InstanceNumber = pMyObject->ulEthernetLinkNextInstance;

                pMyObject->ulEthernetLinkNextInstance++;

                if ( pMyObject->ulEthernetLinkNextInstance == 0 )
                {
                    pMyObject->ulEthernetLinkNextInstance = 1;
                }

                /* Generate Alias */
                rc = sprintf_s(pEntry->Cfg.Alias, sizeof(pEntry->Cfg.Alias),"Link%d", (int)pCosaContext->InstanceNumber);
                if(rc < EOK)
                {
                  ERR_CHK(rc);
                  AnscFreeMemory(pCosaContext);
                  AnscFreeMemory(pEntry);
                  return ANSC_STATUS_FAILURE;
                }

                CosaDmlEthLinkSetValues
                (
                    pMyObject->hSbContext,
                    ulIndex,
                    pCosaContext->InstanceNumber,
                    pEntry->Cfg.Alias
                );
            }

            pCosaContext->hContext      = (ANSC_HANDLE)pEntry;
            pCosaContext->hParentTable  = NULL;
            pCosaContext->bNew          = FALSE;

            CosaSListPushEntryByInsNum(&pMyObject->EthernetLinkList, pCosaContext);
        }
    }

    /* Load the newly added but not yet commited entries, if exist */

    CosaEthLinkRegGetInfo((ANSC_HANDLE)pMyObject);
    
    CosaEthVlanTerminationRegGetInfo((ANSC_HANDLE)pMyObject);

    CosaEthWanTelementryLogger();
    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaEthernetRemove
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate  cosa Ethernet object and return handle.

    argument:   ANSC_HANDLE                 hThisObject
            This handle is actually the pointer of this object
            itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaEthernetRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus            = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_ETHERNET        pMyObject               = (PCOSA_DATAMODEL_ETHERNET)hThisObject;
    PSINGLE_LINK_ENTRY              pSLinkEntry             = NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = NULL;

    /* Remove necessary resounce */
    if (AnscSListQueryDepth(&pMyObject->EthernetLinkList) != 0)
    {
        pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->EthernetLinkList);

        while ( pSLinkEntry )
        {
            pCosaContext            = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
            pSLinkEntry             = AnscSListGetNextEntry(pSLinkEntry);

            AnscSListPopEntryByLink(&pMyObject->EthernetLinkList, &pCosaContext->Linkage);           

            AnscFreeMemory(pCosaContext->hContext);
            AnscFreeMemory(pCosaContext);
        }
    }

    if (AnscSListQueryDepth(&pMyObject->EthernetVlanTerminationList) != 0)
    {
        pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->EthernetVlanTerminationList);

        while ( pSLinkEntry )
        {
            pCosaContext            = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
            pSLinkEntry             = AnscSListGetNextEntry(pSLinkEntry);

            AnscSListPopEntryByLink(&pMyObject->EthernetVlanTerminationList, &pCosaContext->Linkage);           

            AnscFreeMemory(pCosaContext->hContext);
            AnscFreeMemory(pCosaContext);
        }
    }
    /* Remove self */
    AnscFreeMemory((ANSC_HANDLE)pMyObject);

    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        CosaEthLinkRegGetInfo
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve Dslm policy parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hDdnsInfo
                Specifies the Dslm policy parameters to be filled.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CosaEthLinkRegGetInfo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PCOSA_DATAMODEL_ETHERNET        pMyObject               = (PCOSA_DATAMODEL_ETHERNET )hThisObject;
    PSLIST_HEADER                   pListHead               = (PSLIST_HEADER            )&pMyObject->EthernetLinkList;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoEthLink      = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderEthLink;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoEthLinkSp    = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PCOSA_DML_ETH_LINK_FULL         pEntry                  = (PCOSA_DML_ETH_LINK_FULL  )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSLAP_VARIABLE                  pSlapVariable           = (PSLAP_VARIABLE           )NULL;
    ULONG                           ulEntryCount            = 0;
    ULONG                           ulIndex                 = 0;
    ULONG                           ulInstanceNumber        = 0;
    char*                           pFolderName             = NULL;
    char*                           pAlias                  = NULL;
    errno_t                         rc                      = -1;

    if ( !pPoamIrepFoEthLink )
    {
        return ANSC_STATUS_FAILURE;
    }

    /* Load the newly added but not yet commited entries */
    
    ulEntryCount = pPoamIrepFoEthLink->GetFolderCount((ANSC_HANDLE)pPoamIrepFoEthLink);

    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        pFolderName =
            pPoamIrepFoEthLink->EnumFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoEthLink,
                    ulIndex
                );

        if ( !pFolderName )
        {
            continue;
        }

        pPoamIrepFoEthLinkSp = pPoamIrepFoEthLink->GetFolder((ANSC_HANDLE)pPoamIrepFoEthLink, pFolderName);

        AnscFreeMemory(pFolderName);

        if ( !pPoamIrepFoEthLinkSp )
        {
            continue;
        }

        if ( TRUE )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoEthLinkSp->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEthLinkSp,
                        COSA_DML_RR_NAME_EthernetLinkInsNum,
                        NULL
                    );

            if ( pSlapVariable )
            {
                ulInstanceNumber = pSlapVariable->Variant.varUint32;

                SlapFreeVariable(pSlapVariable);
            }
        }

        if ( TRUE )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoEthLinkSp->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEthLinkSp,
                        COSA_DML_RR_NAME_EthernetLinkAlias,
                        NULL
                    );

            if ( pSlapVariable )
            {
                pAlias = AnscCloneString(pSlapVariable->Variant.varString);

                SlapFreeVariable(pSlapVariable);
            }
        }

        pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

        if ( !pCosaContext )
        {
            AnscFreeMemory(pAlias);

            return ANSC_STATUS_RESOURCES;
        }

        pEntry = (PCOSA_DML_ETH_LINK_FULL)AnscAllocateMemory(sizeof(COSA_DML_ETH_LINK_FULL));

        if ( !pEntry )
        {
            AnscFreeMemory(pAlias);
            AnscFreeMemory(pCosaContext);

            return ANSC_STATUS_RESOURCES;
        }
        rc = strcpy_s(pEntry->Cfg.Alias,sizeof(pEntry->Cfg.Alias), pAlias ? pAlias : "");
        ERR_CHK(rc);

        pEntry->Cfg.InstanceNumber = ulInstanceNumber;

        pCosaContext->InstanceNumber        = ulInstanceNumber;
        pCosaContext->bNew                  = TRUE;
        pCosaContext->hContext              = (ANSC_HANDLE)pEntry;
        pCosaContext->hParentTable          = NULL;
        pCosaContext->hPoamIrepUpperFo      = (ANSC_HANDLE)pPoamIrepFoEthLink;
        pCosaContext->hPoamIrepFo           = (ANSC_HANDLE)pPoamIrepFoEthLinkSp;

        CosaSListPushEntryByInsNum(pListHead, pCosaContext);

        if ( pAlias )
        {
            AnscFreeMemory(pAlias);

            pAlias = NULL;
        }
    }
    
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        CosaEthLinkRegAddInfo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCosaContext
            );

    description:

        This function is called to configure Dslm policy parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hDdnsInfo
                Specifies the Dslm policy parameters to be filled.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CosaEthLinkRegAddInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    )
{
    ANSC_STATUS                     returnStatus            = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_ETHERNET        pMyObject               = (PCOSA_DATAMODEL_ETHERNET )hThisObject;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoEthLink      = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderEthLink;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoEthLinkSp    = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)hCosaContext;
    PCOSA_DML_ETH_LINK_FULL         pEntry                  = (PCOSA_DML_ETH_LINK_FULL  )pCosaContext->hContext;
    PSLAP_VARIABLE                  pSlapVariable           = (PSLAP_VARIABLE           )NULL;

    if ( !pPoamIrepFoEthLink )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pPoamIrepFoEthLink->EnableFileSync((ANSC_HANDLE)pPoamIrepFoEthLink, FALSE);
    }

    if ( TRUE )
    {
        SlapAllocVariable(pSlapVariable);

        if ( !pSlapVariable )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT1;
        }
    }

    if ( TRUE )
    {
        returnStatus = 
            pPoamIrepFoEthLink->DelRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoEthLink,
                    COSA_DML_RR_NAME_EthernetLinkNextInsNunmber
                );

        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
        pSlapVariable->Variant.varUint32 = pMyObject->ulEthernetLinkNextInstance;

        returnStatus =
            pPoamIrepFoEthLink->AddRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoEthLink,
                    COSA_DML_RR_NAME_EthernetLinkNextInsNunmber,
                    SYS_REP_RECORD_TYPE_UINT,
                    SYS_RECORD_CONTENT_DEFAULT,
                    pSlapVariable,
                    0
                );

        SlapCleanVariable(pSlapVariable);
        SlapInitVariable (pSlapVariable);
    }

    if ( TRUE )
    {
        pPoamIrepFoEthLinkSp =
            pPoamIrepFoEthLink->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoEthLink,
                    pEntry->Cfg.Alias,
                    0
                );

        if ( !pPoamIrepFoEthLinkSp )
        {
            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT1;
        }

        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
            pSlapVariable->Variant.varUint32 = pEntry->Cfg.InstanceNumber;

            returnStatus =
                pPoamIrepFoEthLinkSp->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEthLinkSp,
                        COSA_DML_RR_NAME_EthernetLinkInsNum,
                        SYS_REP_RECORD_TYPE_UINT,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );

            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }

        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;
            pSlapVariable->Variant.varString = AnscCloneString(pEntry->Cfg.Alias);

            returnStatus =
                pPoamIrepFoEthLinkSp->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEthLinkSp,
                        COSA_DML_RR_NAME_EthernetLinkAlias,
                        SYS_REP_RECORD_TYPE_ASTR,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );

            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }

        pCosaContext->hPoamIrepUpperFo = (ANSC_HANDLE)pPoamIrepFoEthLink;
        pCosaContext->hPoamIrepFo      = (ANSC_HANDLE)pPoamIrepFoEthLinkSp;
    }

EXIT1:
    
    if ( pSlapVariable )
    {
        SlapFreeVariable(pSlapVariable);
    }

    pPoamIrepFoEthLink->EnableFileSync((ANSC_HANDLE)pPoamIrepFoEthLink, TRUE);

    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        CosaEthLinkRegDelInfo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCosaContext
            );

    description:

        This function is called to configure Dslm policy parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hDdnsInfo
                Specifies the Dslm policy parameters to be filled.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CosaEthLinkRegDelInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hCosaContext;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepUpperFo  = (PPOAM_IREP_FOLDER_OBJECT )pCosaContext->hPoamIrepUpperFo;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFo       = (PPOAM_IREP_FOLDER_OBJECT )pCosaContext->hPoamIrepFo;

    if ( !pPoamIrepUpperFo || !pPoamIrepFo )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pPoamIrepUpperFo->EnableFileSync((ANSC_HANDLE)pPoamIrepUpperFo, FALSE);
    }

    if ( TRUE )
    {
        pPoamIrepFo->Close((ANSC_HANDLE)pPoamIrepFo);
        
        pPoamIrepUpperFo->DelFolder
            (
                (ANSC_HANDLE)pPoamIrepUpperFo, 
                pPoamIrepFo->GetFolderName((ANSC_HANDLE)pPoamIrepFo)
            );

        pPoamIrepUpperFo->EnableFileSync((ANSC_HANDLE)pPoamIrepUpperFo, TRUE);

        AnscFreeMemory(pPoamIrepFo);
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaEthPortGetAssocDevices
    (
        UCHAR       *mac,
        CHAR        *maclist,
        int         numMacAddr
    )
{
    int i;

    *maclist = '\0';

    for (i = 0; i < numMacAddr; i++)
    {
        if (i > 0)
        {
            *maclist++ = ',';
        }

        maclist += sprintf (maclist, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

        mac += MAC_SZ;
    }

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        CosaEthVlanTerminationRegGetInfo
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve Dslm policy parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hDdnsInfo
                Specifies the Dslm policy parameters to be filled.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CosaEthVlanTerminationRegGetInfo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PCOSA_DATAMODEL_ETHERNET        pMyObject               = (PCOSA_DATAMODEL_ETHERNET )hThisObject;
    PSLIST_HEADER                   pListHead               = (PSLIST_HEADER            )&pMyObject->EthernetVlanTerminationList;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoEthVlan      = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderEthVlanTermination;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoEthVlanSp    = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PCOSA_DML_ETH_VLAN_TERMINATION_FULL pEntry              = (PCOSA_DML_ETH_VLAN_TERMINATION_FULL)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSLAP_VARIABLE                  pSlapVariable           = (PSLAP_VARIABLE           )NULL;
    ULONG                           ulEntryCount            = 0;
    ULONG                           ulIndex                 = 0;
    ULONG                           ulInstanceNumber        = 0;
    char*                           pFolderName             = NULL;
    char*                           pAlias                  = NULL;
    errno_t                         rc                      = -1;

    if ( !pPoamIrepFoEthVlan )
    {
        return ANSC_STATUS_FAILURE;
    }

    /* Load the newly added but not yet commited entries */
    
    ulEntryCount = pPoamIrepFoEthVlan->GetFolderCount((ANSC_HANDLE)pPoamIrepFoEthVlan);

    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        pFolderName =
            pPoamIrepFoEthVlan->EnumFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoEthVlan,
                    ulIndex
                );

        if ( !pFolderName )
        {
            continue;
        }

        pPoamIrepFoEthVlanSp = pPoamIrepFoEthVlan->GetFolder((ANSC_HANDLE)pPoamIrepFoEthVlan, pFolderName);

        AnscFreeMemory(pFolderName);

        if ( !pPoamIrepFoEthVlanSp )
        {
            continue;
        }

        if ( TRUE )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoEthVlanSp->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEthVlanSp,
                        COSA_DML_RR_NAME_EthernetVlanTerminationInsNum,
                        NULL
                    );

            if ( pSlapVariable )
            {
                ulInstanceNumber = pSlapVariable->Variant.varUint32;

                SlapFreeVariable(pSlapVariable);
            }
        }

        if ( TRUE )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoEthVlanSp->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEthVlanSp,
                        COSA_DML_RR_NAME_EthernetVlanTerminationAlias,
                        NULL
                    );

            if ( pSlapVariable )
            {
                pAlias = AnscCloneString(pSlapVariable->Variant.varString);

                SlapFreeVariable(pSlapVariable);
            }
        }

        pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

        if ( !pCosaContext )
        {
            AnscFreeMemory(pAlias);

            return ANSC_STATUS_RESOURCES;
        }

        pEntry = (PCOSA_DML_ETH_VLAN_TERMINATION_FULL)AnscAllocateMemory(sizeof(COSA_DML_ETH_VLAN_TERMINATION_FULL));

        if ( !pEntry )
        {
            AnscFreeMemory(pAlias);
            AnscFreeMemory(pCosaContext);

            return ANSC_STATUS_RESOURCES;
        }
        rc = strcpy_s(pEntry->Cfg.Alias,sizeof(pEntry->Cfg.Alias), pAlias ? pAlias : "");
        ERR_CHK(rc);

        pEntry->Cfg.InstanceNumber = ulInstanceNumber;

        pCosaContext->InstanceNumber        = ulInstanceNumber;
        pCosaContext->bNew                  = TRUE;
        pCosaContext->hContext              = (ANSC_HANDLE)pEntry;
        pCosaContext->hParentTable          = NULL;
        pCosaContext->hPoamIrepUpperFo      = (ANSC_HANDLE)pPoamIrepFoEthVlan;
        pCosaContext->hPoamIrepFo           = (ANSC_HANDLE)pPoamIrepFoEthVlanSp;

        CosaSListPushEntryByInsNum(pListHead, pCosaContext);

        if ( pAlias )
        {
            AnscFreeMemory(pAlias);

            pAlias = NULL;
        }
    }
    
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        CosaEthVlanTerminationRegAddInfo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCosaContext
            );

    description:

        This function is called to configure Dslm policy parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hDdnsInfo
                Specifies the Dslm policy parameters to be filled.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CosaEthVlanTerminationRegAddInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    )
{
    ANSC_STATUS                     returnStatus            = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_ETHERNET        pMyObject               = (PCOSA_DATAMODEL_ETHERNET )hThisObject;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoEthVlan      = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderEthVlanTermination;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoEthVlanSp    = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)hCosaContext;
    PCOSA_DML_ETH_VLAN_TERMINATION_FULL pEntry              = (PCOSA_DML_ETH_VLAN_TERMINATION_FULL)pCosaContext->hContext;
    PSLAP_VARIABLE                  pSlapVariable           = (PSLAP_VARIABLE           )NULL;

    if ( !pPoamIrepFoEthVlan )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pPoamIrepFoEthVlan->EnableFileSync((ANSC_HANDLE)pPoamIrepFoEthVlan, FALSE);
    }

    if ( TRUE )
    {
        SlapAllocVariable(pSlapVariable);

        if ( !pSlapVariable )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT1;
        }
    }

    if ( TRUE )
    {
        returnStatus = 
            pPoamIrepFoEthVlan->DelRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoEthVlan,
                    COSA_DML_RR_NAME_EthernetVlanTerminationNextInsNunmber
                );

        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
        pSlapVariable->Variant.varUint32 = pMyObject->ulEthernetVlanTerminationNextInstance;

        returnStatus =
            pPoamIrepFoEthVlan->AddRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoEthVlan,
                    COSA_DML_RR_NAME_EthernetVlanTerminationNextInsNunmber,
                    SYS_REP_RECORD_TYPE_UINT,
                    SYS_RECORD_CONTENT_DEFAULT,
                    pSlapVariable,
                    0
                );

        SlapCleanVariable(pSlapVariable);
        SlapInitVariable (pSlapVariable);
    }

    if ( TRUE )
    {
        pPoamIrepFoEthVlanSp =
            pPoamIrepFoEthVlan->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoEthVlan,
                    pEntry->Cfg.Alias,
                    0
                );

        if ( !pPoamIrepFoEthVlanSp )
        {
            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT1;
        }

        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
            pSlapVariable->Variant.varUint32 = pEntry->Cfg.InstanceNumber;

            returnStatus =
                pPoamIrepFoEthVlanSp->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEthVlanSp,
                        COSA_DML_RR_NAME_EthernetVlanTerminationInsNum,
                        SYS_REP_RECORD_TYPE_UINT,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );

            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }

        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;
            pSlapVariable->Variant.varString = AnscCloneString(pEntry->Cfg.Alias);

            returnStatus =
                pPoamIrepFoEthVlanSp->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEthVlanSp,
                        COSA_DML_RR_NAME_EthernetVlanTerminationAlias,
                        SYS_REP_RECORD_TYPE_ASTR,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );

            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }

        pCosaContext->hPoamIrepUpperFo = (ANSC_HANDLE)pPoamIrepFoEthVlan;
        pCosaContext->hPoamIrepFo      = (ANSC_HANDLE)pPoamIrepFoEthVlanSp;
    }

EXIT1:
    
    if ( pSlapVariable )
    {
        SlapFreeVariable(pSlapVariable);
    }

    pPoamIrepFoEthVlan->EnableFileSync((ANSC_HANDLE)pPoamIrepFoEthVlan, TRUE);

    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        CosaEthVlanTerminationRegDelInfo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCosaContext
            );

    description:

        This function is called to configure Dslm policy parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hDdnsInfo
                Specifies the Dslm policy parameters to be filled.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CosaEthVlanTerminationRegDelInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCosaContext
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hCosaContext;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepUpperFo  = (PPOAM_IREP_FOLDER_OBJECT )pCosaContext->hPoamIrepUpperFo;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFo       = (PPOAM_IREP_FOLDER_OBJECT )pCosaContext->hPoamIrepFo;

    if ( !pPoamIrepUpperFo || !pPoamIrepFo )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pPoamIrepUpperFo->EnableFileSync((ANSC_HANDLE)pPoamIrepUpperFo, FALSE);
    }

    if ( TRUE )
    {
        pPoamIrepFo->Close((ANSC_HANDLE)pPoamIrepFo);
        
        pPoamIrepUpperFo->DelFolder
            (
                (ANSC_HANDLE)pPoamIrepUpperFo, 
                pPoamIrepFo->GetFolderName((ANSC_HANDLE)pPoamIrepFo)
            );

        pPoamIrepUpperFo->EnableFileSync((ANSC_HANDLE)pPoamIrepUpperFo, TRUE);

        AnscFreeMemory(pPoamIrepFo);
    }

    return ANSC_STATUS_SUCCESS;
}

void * EthWan_TelementryLogger_Thread(void *data)
{
    UNREFERENCED_PARAMETER(data);
    pthread_detach(pthread_self());

    while (1)
    {
        CcspTraceInfo(("RDK_LOG_INFO , Ethernet WAN is enabled\n"));
        sleep(ONE_HR);
    }
}


static void CosaEthWanTelementryLogger(void)
{
    pthread_t ethwantelementrylogger_tid;
    char isEthEnabled[64]={'\0'};
    int res;

        if( 0 == syscfg_get( NULL, "eth_wan_enabled", isEthEnabled, sizeof(isEthEnabled)) && (isEthEnabled[0] != '\0' && strncmp(isEthEnabled, "true", strlen("true")) == 0))
        {
            res = pthread_create(&ethwantelementrylogger_tid, NULL, EthWan_TelementryLogger_Thread, NULL);
            if (res != 0)
            {
                AnscTraceWarning(("CosaEthWanTelementryLogger Create EthWan_TelementryLogger_Thread error %d\n", res));
            }
            else
            {
                AnscTraceWarning(("CosaEthWanTelementryLogger EthWan_TelementryLogger_Thread Created\n"));
            }
        }

}
