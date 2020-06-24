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

    module: cosa_nat_internal.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaNatCreate
        *  CosaNatInitialize
        *  CosaNatRemove
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
#include "cosa_nat_apis.h"
#include "cosa_nat_internal.h"
#include "plugin_main_apis.h"
#include "poam_irepfo_interface.h"
#include "sys_definitions.h"
#include "cosa_webconfig_api.h"
#include "cosa_nat_webconfig_apis.h"
extern void * g_pDslhDmlAgent;

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        CosaNatCreate
            (
            );

    description:

        This function constructs cosa nat object and return handle.

    argument:

    return:     newly created nat object.

**********************************************************************/

ANSC_HANDLE
CosaNatCreate
    (
        VOID
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_NAT             pMyObject    = (PCOSA_DATAMODEL_NAT)NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PCOSA_DATAMODEL_NAT)AnscAllocateMemory(sizeof(COSA_DATAMODEL_NAT));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->Oid               = COSA_DATAMODEL_NAT_OID;
    pMyObject->Create            = CosaNatCreate;
    pMyObject->Remove            = CosaNatRemove;
    pMyObject->Initialize        = CosaNatInitialize;

    pMyObject->Initialize   ((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaNatInitialize
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
CosaNatInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_NAT             pMyObject        = (PCOSA_DATAMODEL_NAT)hThisObject;
    PSLAP_VARIABLE                  pSlapVariable    = (PSLAP_VARIABLE             )NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoCOSA  = NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoNat   = NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoNatPt = NULL;

    /* Call Initiation */
    returnStatus = CosaDmlNatInit(NULL, NULL, CosaNatGen);
    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        return returnStatus;
    }

    /* Initiation all functions */
    AnscSListInitializeHeader( &pMyObject->NatPMappingList );
    AnscSListInitializeHeader( &pMyObject->NatPTriggerList );
    pMyObject->MaxInstanceNumber        = 0;
    pMyObject->ulPtNextInstanceNumber   = 1;
    pMyObject->PreviousVisitTime        = 0;

    /*Create NAT folder in configuration */
    pPoamIrepFoCOSA = (PPOAM_IREP_FOLDER_OBJECT)g_GetRegistryRootFolder(g_pDslhDmlAgent);

    if ( !pPoamIrepFoCOSA )
    {
        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT;
    }

    pPoamIrepFoNat =
        (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoCOSA->GetFolder
            (
                (ANSC_HANDLE)pPoamIrepFoCOSA,
                COSA_IREP_FOLDER_NAME_NAT
            );

    if ( !pPoamIrepFoNat )
    {
        pPoamIrepFoCOSA->EnableFileSync((ANSC_HANDLE)pPoamIrepFoCOSA, FALSE);

        pPoamIrepFoNat =
            pPoamIrepFoCOSA->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoCOSA,
                    COSA_IREP_FOLDER_NAME_NAT,
                    0
                );

        pPoamIrepFoCOSA->EnableFileSync((ANSC_HANDLE)pPoamIrepFoCOSA, TRUE);
    }

    if ( !pPoamIrepFoNat )
    {
        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT;
    }
    else
    {
        pMyObject->hIrepFolderNat = (ANSC_HANDLE)pPoamIrepFoNat;
    }

    pPoamIrepFoNatPt =
        (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoNat->GetFolder
            (
                (ANSC_HANDLE)pPoamIrepFoNat,
                COSA_IREP_FOLDER_NAME_PORTTRIGGER
            );

    if ( !pPoamIrepFoNatPt )
    {
        /* pPoamIrepFoCOSA->EnableFileSync((ANSC_HANDLE)pPoamIrepFoCOSA, FALSE); */

        pPoamIrepFoNatPt =
            pPoamIrepFoCOSA->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoNat,
                    COSA_IREP_FOLDER_NAME_PORTTRIGGER,
                    0
                );

        /* pPoamIrepFoCOSA->EnableFileSync((ANSC_HANDLE)pPoamIrepFoCOSA, TRUE); */
    }

    if ( !pPoamIrepFoNatPt )
    {
        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT;
    }
    else
    {
        pMyObject->hIrepFolderNatPt = (ANSC_HANDLE)pPoamIrepFoNatPt;
    }

    /* Retrieve the next instance number for Port Trigger */

    if ( TRUE )
    {
        if ( pPoamIrepFoNatPt )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoNatPt->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoNatPt,
                        COSA_DML_RR_NAME_NATNextInsNumber,
                        NULL
                    );

            if ( pSlapVariable )
            {
                pMyObject->ulPtNextInstanceNumber = pSlapVariable->Variant.varUint32;

                SlapFreeVariable(pSlapVariable);
            }
        }
    }

    CosaDmlNatGetLanIP(NULL);

    /* Sync with backend */
    CosaDmlNatGetDmz(NULL, &pMyObject->Dmz);

    CosaNatSyncPortTrigger((ANSC_HANDLE)pMyObject);

    /* We need get NextInstanceNumber from backend. By the way, the whole tree
            was created. Moreover, we also need get delay-added entry and put them
            into our tree. */
    CosaNatRegGetNatInfo((ANSC_HANDLE)pMyObject);

    /* Intialixing cache first time */

    clear_pf_cache(pf_cache);
    clear_pf_cache(pf_cache_bkup);

    init_pf_cache(pf_cache);

    clear_dmz_cache(dmz_cache);
    clear_dmz_cache(dmz_cache_bkup);

    init_dmz_cache(dmz_cache);

EXIT:

    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaNatRemove
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
CosaNatRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_NAT             pMyObject    = (PCOSA_DATAMODEL_NAT)hThisObject;
    PSINGLE_LINK_ENTRY              pLink        = NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pNat         = NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFo  = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderNat;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepPt  = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderNatPt;



    /* Remove resource of writable entry link */
    for( pLink = AnscSListPopEntry(&pMyObject->NatPMappingList); pLink; )
    {
        pNat = (PCOSA_CONTEXT_LINK_OBJECT)ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        pLink = AnscSListGetNextEntry(pLink);

        AnscFreeMemory(pNat->hContext);
        AnscFreeMemory(pNat);
    }

    for( pLink = AnscSListPopEntry(&pMyObject->NatPTriggerList); pLink; )
    {
        pNat = (PCOSA_CONTEXT_LINK_OBJECT)ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        pLink = AnscSListGetNextEntry(pLink);

        AnscFreeMemory(pNat->hContext);
        AnscFreeMemory(pNat);
    }

    if ( pPoamIrepPt )
    {
        pPoamIrepPt->Remove( (ANSC_HANDLE)pPoamIrepPt);
    }

    if ( pPoamIrepFo )
    {
        pPoamIrepFo->Remove( (ANSC_HANDLE)pPoamIrepFo);
    }

    /* Remove self */
    AnscFreeMemory((ANSC_HANDLE)pMyObject);

    return returnStatus;
}

ANSC_STATUS
CosaNatGen
    (
        ANSC_HANDLE                 hDml,
        PCOSA_DML_NAT_PMAPPING      pEntry
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_NAT             pNat              = (PCOSA_DATAMODEL_NAT)g_pCosaBEManager->hNat;

    /*
            For dynamic and writable table, we don't keep the Maximum InstanceNumber.
            If there is delay_added entry, we just jump that InstanceNumber.
        */
    do
    {
        pNat->MaxInstanceNumber++;

        if ( pNat->MaxInstanceNumber <= 0 )
        {
            pNat->MaxInstanceNumber   = 1;
        }

        if ( !CosaSListGetEntryByInsNum(&pNat->NatPMappingList, pNat->MaxInstanceNumber) )
        {
            break;
        }
    }while(1);

    pEntry->InstanceNumber            = pNat->MaxInstanceNumber;
    _ansc_sprintf( pEntry->Alias, "PortMapping%d", pEntry->InstanceNumber );

    /* keep new MaxInstanceNumber */
    returnStatus = CosaNatRegSetNatInfo(pNat);

    return returnStatus;
}

ANSC_STATUS
CosaNatGenForTriggerEntry
    (
        ANSC_HANDLE                 hDml,
        PCOSA_DML_NAT_PTRIGGER      pEntry
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_NAT             pNat              = (PCOSA_DATAMODEL_NAT)g_pCosaBEManager->hNat;

    /*
            For dynamic and writable table, we don't keep the Maximum InstanceNumber.
            If there is delay_added entry, we just jump that InstanceNumber.
        */
    do
    {
        if ( pNat->ulPtNextInstanceNumber == 0 )
        {
            pNat->ulPtNextInstanceNumber   = 1;
        }

        if ( !CosaSListGetEntryByInsNum(&pNat->NatPTriggerList, pNat->ulPtNextInstanceNumber) )
        {
            break;
        }
        else
        {
            pNat->ulPtNextInstanceNumber++;
        }
    }while(1);

    pEntry->InstanceNumber            = pNat->ulPtNextInstanceNumber;

    if ( pEntry->Alias[0] == '\0' )
    {
        _ansc_sprintf( pEntry->Alias, "PortTrigger%d", pEntry->InstanceNumber );
    }

    pNat->ulPtNextInstanceNumber++;

    /* keep new ulPtInstanceNumber */
    returnStatus = CosaNatRegSetNatInfo(pNat);

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        CosaNatRegGetNatInfo
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve backend inform and put them into our trees.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of nat
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CosaNatRegGetNatInfo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_NAT             pMyObject         = ( PCOSA_DATAMODEL_NAT )hThisObject;

    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoNat      = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderNat;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoNatPt    = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderNatPt;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoEnumNat  = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PCOSA_CONTEXT_PMAPPING_LINK_OBJECT     pCosaNatContext   = NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaNatPtContext   = NULL;
    PCOSA_DML_NAT_PMAPPING          pCosaNat          = NULL;
    PCOSA_DML_NAT_PTRIGGER          pCosaNatPt        = NULL;
    PSLAP_VARIABLE                  pSlapVariable     = NULL;
    ULONG                           ulEntryCount      = 0;
    ULONG                           ulIndex           = 0;
    ULONG                           uInstanceNumber   = 0;
    BOOL                            bNew              = FALSE;
    char*                           pAliasNat         = NULL;
    char*                           pFolderName       = NULL;

    CcspTraceWarning(("!!!!!! In CosaNatRegGetNatInfo !!!!!!\n"));

    if ( !pPoamIrepFoNat || !pPoamIrepFoNatPt )
    {
        CcspTraceWarning(("!!!!!! pPoamIrepFoNat: %p, pPoamIrepFoNatPt: %p !!!!!!\n", pPoamIrepFoNat, pPoamIrepFoNatPt));
        return ANSC_STATUS_FAILURE;
    }

    /* This is saved structure for nat
        *****************************************
              <Nat>
                  <1>
                       <alias>xxx</>
                       <bNew>false</>
                 </1>
                 <2>
                      <alias>xxx</>
                      <bNew>false</>
                 </2>
            </Nat>
      ****************************************************
      */

    /* enumerate Portmapping.{i} */
    ulEntryCount = pPoamIrepFoNat->GetFolderCount((ANSC_HANDLE)pPoamIrepFoNat);
    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        /* Get i in Portmapping.{i} */
        pFolderName =
            pPoamIrepFoNat->EnumFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoNat,
                    ulIndex
                );

        if ( !pFolderName )
        {
            continue;
        }

        uInstanceNumber = _ansc_atol(pFolderName);

        if ( uInstanceNumber == 0 )
        {
            AnscFreeMemory(pFolderName);
            continue;
        }

        pPoamIrepFoEnumNat = pPoamIrepFoNat->GetFolder((ANSC_HANDLE)pPoamIrepFoNat, pFolderName);

        AnscFreeMemory(pFolderName);

        if ( !pPoamIrepFoEnumNat )
        {
            continue;
        }

        /* Get PortMapping.{i}.Alias value*/
        if ( TRUE )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoEnumNat->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEnumNat,
                        COSA_DML_RR_NAME_NATAlias,
                        NULL
                    );

            if ( pSlapVariable )
            {
                pAliasNat = AnscCloneString(pSlapVariable->Variant.varString);

                SlapFreeVariable(pSlapVariable);
            }
        }

        /* Get PortMapping.{i}.bNew value*/
        if ( TRUE )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoEnumNat->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEnumNat,
                        COSA_DML_RR_NAME_NATbNew,
                        NULL
                    );

            if ( pSlapVariable )
            {
                bNew = pSlapVariable->Variant.varBool;

                SlapFreeVariable(pSlapVariable);
            }
            else
            {
                bNew = TRUE;
            }
        }

        /* Create one entry and keep this delay_added entry */
        pCosaNat = (PCOSA_DML_NAT_PMAPPING)AnscAllocateMemory(sizeof(COSA_DML_NAT_PMAPPING));
        if ( !pCosaNat )
        {
            returnStatus = ANSC_STATUS_FAILURE;
            goto EXIT1;
        }

        /* set some default value firstly */
        NAT_PORTMAPPING_SET_DEFAULTVALUE(pCosaNat);

        /* save alias and instanceNumber */
        pCosaNat->InstanceNumber = uInstanceNumber;
        AnscCopyString( pCosaNat->Alias, pAliasNat );

        /* Create one link point */
        pCosaNatContext = (PCOSA_CONTEXT_PMAPPING_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_PMAPPING_LINK_OBJECT));
        if ( !pCosaNatContext )
        {
            returnStatus = ANSC_STATUS_FAILURE;
            goto EXIT2;
        }

        NAT_PMAPPING_INITIATION_CONTEXT(pCosaNatContext)

        pCosaNatContext->InstanceNumber = pCosaNat->InstanceNumber;
        pCosaNatContext->hContext       = (ANSC_HANDLE)pCosaNat;
        pCosaNat                        = 0;
        pCosaNatContext->bNew           = bNew;

        CosaSListPushEntryByInsNum(&pMyObject->NatPMappingList, (PCOSA_CONTEXT_LINK_OBJECT)pCosaNatContext);

        /* release some memory */
        if (pAliasNat)
        {
            AnscFreeMemory(pAliasNat);
            pAliasNat = NULL;
        }

        pPoamIrepFoEnumNat->Remove((ANSC_HANDLE)pPoamIrepFoEnumNat);
        pPoamIrepFoEnumNat = NULL;
    }

    /* This is saved structure for nat port trigger
        *****************************************
              <Nat>
                <PortTrigger>
                  <Record>NextInsNumber</Record>
                  <1>
                       <alias>xxx</>
                       <bNew>true</>
                 </1>
                 <2>
                      <alias>xxx</>
                      <bNew>true</>
                 </2>
                 </PortTrigger>
            </Nat>
      ****************************************************
      */

    /* enumerate PortTrigger.{i} */
    ulEntryCount = pPoamIrepFoNatPt->GetFolderCount((ANSC_HANDLE)pPoamIrepFoNatPt);
    CcspTraceWarning(("!!!!!! ulEntryCount for PortTrigger: %d !!!!!!\n", ulEntryCount));

    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        /* Get i in PortTrigger.{i} */
        pFolderName =
            pPoamIrepFoNatPt->EnumFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoNatPt,
                    ulIndex
                );
        CcspTraceWarning(("!!!!!! pFolderName: %s !!!!!!\n", pFolderName));

        if ( !pFolderName )
        {
            continue;
        }

        uInstanceNumber = _ansc_atol(pFolderName);

        if ( uInstanceNumber == 0 )
        {
            AnscFreeMemory(pFolderName);
            continue;
        }

        pPoamIrepFoEnumNat = pPoamIrepFoNatPt->GetFolder((ANSC_HANDLE)pPoamIrepFoNatPt, pFolderName);

        AnscFreeMemory(pFolderName);

        if ( !pPoamIrepFoEnumNat )
        {
            continue;
        }

        /* Get PortTrigger.{i}.Alias value*/
        if ( TRUE )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoEnumNat->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEnumNat,
                        COSA_DML_RR_NAME_NATAlias,
                        NULL
                    );

            if ( pSlapVariable )
            {
                pAliasNat = AnscCloneString(pSlapVariable->Variant.varString);

                SlapFreeVariable(pSlapVariable);
            }
        }

        /* Get PortTrigger.{i}.bNew value*/
        if ( TRUE )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoEnumNat->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEnumNat,
                        COSA_DML_RR_NAME_NATbNew,
                        NULL
                    );

            if ( pSlapVariable )
            {
                bNew = pSlapVariable->Variant.varBool;

                SlapFreeVariable(pSlapVariable);
            }
            else
            {
                bNew = TRUE;
            }
        }

        /* Create one entry and keep this delay_added entry */
        pCosaNatPt = (PCOSA_DML_NAT_PTRIGGER)AnscAllocateMemory(sizeof(COSA_DML_NAT_PTRIGGER));

        if ( !pCosaNatPt )
        {
            returnStatus = ANSC_STATUS_RESOURCES;
            goto EXIT1;
        }

        /* set some default value firstly */
        COSA_DML_NAT_PTRIGGER_INIT(pCosaNatPt);

        /* save alias and instanceNumber */
        pCosaNatPt->InstanceNumber = uInstanceNumber;
        AnscCopyString( pCosaNatPt->Alias, pAliasNat );

        CcspTraceWarning(("!!!!!! pCosaNatPt->InstanceNumber: %d, pCosaNatPt->Alias: %s !!!!!!\n", pCosaNatPt->InstanceNumber, pCosaNatPt->Alias));

        /* Create one link point */
        pCosaNatPtContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
        if ( !pCosaNatPtContext )
        {
            AnscFreeMemory(pCosaNatPt); /*RDKB-6742, CID-33206, Free unused resources after use*/
            pCosaNatPt = NULL;
            returnStatus = ANSC_STATUS_FAILURE;
            goto EXIT2;
        }

         COSA_CONTEXT_LINK_INITIATION_CONTENT(pCosaNatPtContext)

        pCosaNatPtContext->InstanceNumber = pCosaNatPt->InstanceNumber;
        pCosaNatPtContext->hContext       = (ANSC_HANDLE)pCosaNatPt;
        pCosaNatPtContext->bNew           = bNew;

        CosaSListPushEntryByInsNum(&pMyObject->NatPTriggerList, (PCOSA_CONTEXT_LINK_OBJECT)pCosaNatPtContext);

        /* release some memory */
        if (pAliasNat)
        {
            AnscFreeMemory(pAliasNat);
            pAliasNat = NULL;
        }

        pPoamIrepFoEnumNat->Remove((ANSC_HANDLE)pPoamIrepFoEnumNat);
        pPoamIrepFoEnumNat = NULL;
    }



EXIT2:
    if(pCosaNat)
        AnscFreeMemory(pCosaNat);

EXIT1:

    if(pAliasNat)
        AnscFreeMemory(pAliasNat);

    if ( pPoamIrepFoEnumNat )
        pPoamIrepFoEnumNat->Remove((ANSC_HANDLE)pPoamIrepFoEnumNat);

    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        CosaNatRegSetNatInfo
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to save current NextInstanceNumber and Delay_added
        entry into sysregistry.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of nat
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CosaNatRegSetNatInfo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_NAT             pMyObject           = (PCOSA_DATAMODEL_NAT     )hThisObject;

    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoNat      = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderNat;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoNatPt    = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoEnumNat  = (PPOAM_IREP_FOLDER_OBJECT)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaNatContext     = NULL;
    PCOSA_DML_NAT_PMAPPING          pCosaNat            = NULL;
    PCOSA_DML_NAT_PTRIGGER          pCosaNatPt          = NULL;

    PSINGLE_LINK_ENTRY              pSLinkEntry         = (PSINGLE_LINK_ENTRY      )NULL;
    PSLAP_VARIABLE                  pSlapVariable       = NULL;
    ULONG                           ulEntryCount        = 0;
    ULONG                           ulIndex             = 0;
    ULONG                           uInstanceNumber     = 0;
    char*                           pAliasNat           = NULL;
    char*                           pFolderName         = NULL;
    char                            FolderName[16]      = {0};


    if ( !pPoamIrepFoNat )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pPoamIrepFoNat->EnableFileSync((ANSC_HANDLE)pPoamIrepFoNat, FALSE);
    }

    if ( TRUE )
    {
        pPoamIrepFoNat->Clear((ANSC_HANDLE)pPoamIrepFoNat);

        SlapAllocVariable(pSlapVariable);

        if ( !pSlapVariable )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT1;
        }
    }

    /* This is saved structure for Nat
        *****************************************
              <Nat>
                  <PortMapping>
                      <1>
                           <alias>xxx</>
                           <bNew>false</>
                     </1>
                     <2>
                          <alias>xxx</>
                          <bNew>false</>
                     </2>
                </PortMapping>
            </Nat>
      ******************************************/

    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->NatPMappingList);

    while ( pSLinkEntry )
    {
        /* create nat.{i} */

        pCosaNatContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        pSLinkEntry       = AnscSListGetNextEntry(pSLinkEntry);

        pCosaNat = (PCOSA_DML_NAT_PMAPPING)pCosaNatContext->hContext;

        if ( !pCosaNatContext->bNew )
        {
            continue;
        }

        _ansc_sprintf(FolderName, "%d", pCosaNat->InstanceNumber);

        pPoamIrepFoEnumNat =
            pPoamIrepFoNat->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoNat,
                    FolderName,
                    0
                );

        if ( !pPoamIrepFoEnumNat )
        {
            continue;
        }

        /* add  nat.{i}.Alias */
        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;
            pSlapVariable->Variant.varString = AnscCloneString(pCosaNat->Alias);

            returnStatus =
                pPoamIrepFoEnumNat->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEnumNat,
                        COSA_DML_RR_NAME_NATAlias,
                        SYS_REP_RECORD_TYPE_ASTR,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );

            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }

        /* add  nat.{i}.bNew */
        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_bool;
            pSlapVariable->Variant.varBool   = pCosaNatContext->bNew;

            returnStatus =
                pPoamIrepFoEnumNat->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEnumNat,
                        COSA_DML_RR_NAME_NATbNew,
                        SYS_REP_RECORD_TYPE_BOOL,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );

            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }


        /*release some resource */
        pPoamIrepFoEnumNat->Remove((ANSC_HANDLE)pPoamIrepFoEnumNat);
        pPoamIrepFoEnumNat = NULL;
    }

    /* For PortTrigger */

    /* This is saved structure for Nat
        *****************************************
              <Nat>
                  <PortTrigger>
                      <NextInsNumber>xxx</>
                      <1>
                           <alias>xxx</>
                           <bNew>true</>
                     </1>
                     <2>
                          <alias>xxx</>
                          <bNew>true</>
                     </2>
                </PortTrigger>
            </Nat>
      ******************************************/

    pPoamIrepFoNatPt =
            pPoamIrepFoNat->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoNat,
                    COSA_IREP_FOLDER_NAME_PORTTRIGGER,
                    0
                );

    if ( !pPoamIrepFoNatPt )
    {
        returnStatus = ANSC_STATUS_FAILURE;

        goto EXIT1;
    }

    /* Save next instance number for port trigger */
    if ( TRUE )
    {
        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
        pSlapVariable->Variant.varUint32 = pMyObject->ulPtNextInstanceNumber;

        returnStatus =
            pPoamIrepFoNatPt->AddRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoNatPt,
                    COSA_DML_RR_NAME_NATNextInsNumber,
                    SYS_REP_RECORD_TYPE_UINT,
                    SYS_RECORD_CONTENT_DEFAULT,
                    pSlapVariable,
                    0
                );

        SlapCleanVariable(pSlapVariable);
        SlapInitVariable (pSlapVariable);
    }

    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->NatPTriggerList);

    while ( pSLinkEntry )
    {
        /* create nat.PortTrigger.{i} */

        pCosaNatContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        pSLinkEntry       = AnscSListGetNextEntry(pSLinkEntry);

        pCosaNatPt = (PCOSA_DML_NAT_PTRIGGER)pCosaNatContext->hContext;

        if ( !pCosaNatContext->bNew )
        {
            continue;
        }

        _ansc_sprintf(FolderName, "%d", pCosaNatPt->InstanceNumber);

        pPoamIrepFoEnumNat =
            pPoamIrepFoNatPt->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoNatPt,
                    FolderName,
                    0
                );

        if ( !pPoamIrepFoEnumNat )
        {
            continue;
        }

        /* add  PortTrigger.{i}.Alias */
        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;
            pSlapVariable->Variant.varString = AnscCloneString(pCosaNatPt->Alias);

            returnStatus =
                pPoamIrepFoEnumNat->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEnumNat,
                        COSA_DML_RR_NAME_NATAlias,
                        SYS_REP_RECORD_TYPE_ASTR,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );

            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }

        /* add  PortTrigger.{i}.bNew */
        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_bool;
            pSlapVariable->Variant.varBool   = pCosaNatContext->bNew;

            returnStatus =
                pPoamIrepFoEnumNat->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEnumNat,
                        COSA_DML_RR_NAME_NATbNew,
                        SYS_REP_RECORD_TYPE_BOOL,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );

            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }


        /*release some resource */
        pPoamIrepFoEnumNat->Remove((ANSC_HANDLE)pPoamIrepFoEnumNat);
        pPoamIrepFoEnumNat = NULL;
    }


EXIT1:
    if ( pSlapVariable )
    {
        SlapFreeVariable(pSlapVariable);
        pSlapVariable = NULL;
    }

    if ( pPoamIrepFoEnumNat )
        pPoamIrepFoEnumNat->Remove((ANSC_HANDLE)pPoamIrepFoEnumNat);

    pPoamIrepFoNat->EnableFileSync((ANSC_HANDLE)pPoamIrepFoNat, TRUE);

    return returnStatus;
}


ANSC_STATUS
CosaNatSyncPortTrigger
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_NAT             pMyObject           = (PCOSA_DATAMODEL_NAT     )hThisObject;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink            = NULL;
    PSINGLE_LINK_ENTRY              pSListEntry         = NULL;
    PCOSA_DML_NAT_PTRIGGER          pPortTrigger        = NULL;
    PCOSA_DML_NAT_PTRIGGER          pPtEntry            = NULL;
    ULONG                           entryCount          = 0;
    ULONG                           i                   = 0;
    BOOL*                           pbFoundPt           = NULL;

    CcspTraceWarning(("!!!!!!!! in CosaNatSyncPortTrigger !!!!!!!!\n"));

    pPortTrigger = CosaDmlNatGetPortTriggers(NULL,&entryCount,TRUE);

    if ( !pPortTrigger )
    {
        /* Get Error, we don't del link because next time, it may be successful */
        return ANSC_STATUS_FAILURE;
    }

    pbFoundPt     = (BOOL*)AnscAllocateMemory(sizeof(BOOL) * entryCount);
    AnscZeroMemory(pbFoundPt, (sizeof(BOOL) * entryCount));

    if ( !pbFoundPt )
    {
        AnscFreeMemory(pPortTrigger); /*RDKB-6742, CID-32996, free unused resources before exit*/
        return ANSC_STATUS_RESOURCES;
    }

    pSListEntry =   AnscSListGetFirstEntry(&pMyObject->NatPTriggerList);

    while( pSListEntry )
    {
        pCxtLink          = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSListEntry);
        pSListEntry       = AnscSListGetNextEntry(pSListEntry);

        pPtEntry          = (PCOSA_DML_NAT_PTRIGGER)pCxtLink->hContext;

        for ( i = 0; i < entryCount; i++)
        {
            if ( pPtEntry->InstanceNumber == pPortTrigger[i].InstanceNumber )
            {
                /* If found, update the content also */
                CcspTraceWarning(("!!!!!!!! Ready to copy 1 !!!!!!!!\n"));
                AnscCopyMemory(pPtEntry, &pPortTrigger[i], sizeof(COSA_DML_NAT_PTRIGGER)); /* pOriAlias is NULL, no deep copy needed */

                pCxtLink->bNew = FALSE;
                pbFoundPt[i]   = TRUE;

                break;
            }
        }
    }

    /* Check those in backend but not yet in middle layer */

    for ( i = 0; i < entryCount; i++ )
    {
        if ( pbFoundPt[i] == FALSE )
        {
            /* We add new entry into our link table */
            pCxtLink = AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
            if ( !pCxtLink )
            {
                goto EXIT1;
            }

            pPtEntry = AnscAllocateMemory(sizeof(COSA_DML_NAT_PTRIGGER));
            if ( !pPtEntry )
            {
                goto EXIT2;
            }

            /* copy new content which should include InstanceNumber and Alias */
            CcspTraceWarning(("!!!!!!!! Ready to copy 2 &pPortTrigger[%d]->InsNumber: %d !!!!!!!!\n", i, pPortTrigger[i].InstanceNumber));
            AnscCopyMemory(pPtEntry, &pPortTrigger[i], sizeof(COSA_DML_NAT_PTRIGGER)); /* pOriAlias is NULL, no deep copy needed */
            CcspTraceWarning(("!!!!!!!! After copy 2 pPtEntry->InsNumber: %d !!!!!!!!\n", pPtEntry->InstanceNumber));

            pCxtLink->hContext       = (ANSC_HANDLE)pPtEntry;
            pCxtLink->InstanceNumber = pPtEntry->InstanceNumber;
            pCxtLink->bNew           = FALSE;

            CosaSListPushEntryByInsNum(&pMyObject->NatPTriggerList, (PCOSA_CONTEXT_LINK_OBJECT)pCxtLink);
        }
    }

    returnStatus = ANSC_STATUS_SUCCESS;

    goto EXIT1;


EXIT2:

    AnscFreeMemory(pCxtLink);

EXIT1:

    AnscFreeMemory(pbFoundPt);
    AnscFreeMemory(pPortTrigger);

    CcspTraceWarning(("!!!!!!!! out CosaNatSyncPortTrigger !!!!!!!!\n"));

    return returnStatus;
}
