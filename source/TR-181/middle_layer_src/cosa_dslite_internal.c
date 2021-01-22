/**********************************************************************
 * Copyright 2017-2019 ARRIS Enterprises, LLC.
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
**********************************************************************/
#ifdef DSLITE_FEATURE_SUPPORT
#include "cosa_apis.h"
#include "cosa_dslite_apis.h"
#include "cosa_dslite_internal.h"
#include "plugin_main_apis.h"
#include "poam_irepfo_interface.h"
#include "sys_definitions.h"
#include "slap_vho_exported_api.h"
#include "safec_lib_common.h"
#include "cosa_x_cisco_com_devicecontrol_internal.h"

extern void * g_pDslhDmlAgent;

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        CosaDsliteCreate
            (
            );

    description:

        This function constructs cosa Dslite object and return handle.

    argument:

    return:     newly created Dslite object.

**********************************************************************/

ANSC_HANDLE
CosaDsliteCreate
    (
        VOID
    )
{
    PCOSA_DATAMODEL_DSLITE          pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PCOSA_DATAMODEL_DSLITE)AnscAllocateMemory(sizeof(COSA_DATAMODEL_DSLITE));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->Oid               = COSA_DATAMODEL_DSLITE_OID;
    pMyObject->Create            = CosaDsliteCreate;
    pMyObject->Remove            = CosaDsliteRemove;
    pMyObject->Initialize        = CosaDsliteInitialize;

    pMyObject->Initialize   ((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDsliteInitialize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate  cosa Dslite object and return handle.

            argument:    ANSC_HANDLE                 hThisObject
            This handle is actually the pointer of this object
            itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaDsliteInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DSLITE          pMyObject         = (PCOSA_DATAMODEL_DSLITE)hThisObject;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoCOSA   = NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoDslite = NULL;

    /* We need call the initiation function of backend firstly */
    returnStatus = CosaDmlDsliteInit( NULL, NULL );
    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        return returnStatus;
    }

    /* Initiation all functions */
    AnscSListInitializeHeader( &pMyObject->DsliteList );
    pMyObject->maxInstanceOfDslite    = 0;
    pMyObject->hIrepFolderDslite      = NULL;
    AnscZeroMemory(pMyObject->Alias, sizeof(pMyObject->Alias));

    /*We need to get Instance Info from cosa configuration*/
    pPoamIrepFoCOSA = (PPOAM_IREP_FOLDER_OBJECT)g_GetRegistryRootFolder(g_pDslhDmlAgent);

    if ( !pPoamIrepFoCOSA )
    {
        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT;
    }

    pPoamIrepFoDslite =
        (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoCOSA->GetFolder
            (
                (ANSC_HANDLE)pPoamIrepFoCOSA,
                COSA_IREP_FOLDER_NAME_DSLITE_IFS
            );

    if ( !pPoamIrepFoDslite )
    {
        pPoamIrepFoCOSA->EnableFileSync((ANSC_HANDLE)pPoamIrepFoCOSA, FALSE);

        pPoamIrepFoDslite =
            pPoamIrepFoCOSA->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoCOSA,
                    COSA_IREP_FOLDER_NAME_DSLITE_IFS,
                    0
                );

        pPoamIrepFoCOSA->EnableFileSync((ANSC_HANDLE)pPoamIrepFoCOSA, TRUE);

    }

    if ( !pPoamIrepFoDslite )
    {
        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT;
    }
    else
    {
        pMyObject->hIrepFolderDslite = (ANSC_HANDLE)pPoamIrepFoDslite;
    }

    /* We need get NextInstanceNumber from backend. By the way, the whole tree
            was created. Moreover, we also need get delay-added entry and put them
            into our tree. */
    CosaDsliteRegGetDsliteInfo((ANSC_HANDLE)pMyObject);
    /* Firstly we create the whole system from backend */
    CosaDsliteBackendGetInfo((ANSC_HANDLE)pMyObject);

EXIT:

    return returnStatus;

}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaDsliteRemove
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate  cosa Dslite object and return handle.

            argument:   ANSC_HANDLE                 hThisObject
            This handle is actually the pointer of this object
            itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaDsliteRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DSLITE          pMyObject           = (PCOSA_DATAMODEL_DSLITE)hThisObject;
    PSINGLE_LINK_ENTRY              pSListEntry         = NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink            = NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFo         = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderDslite;

    /* Remove necessary resource */
    pSListEntry         = AnscSListPopEntry(&pMyObject->DsliteList);
    while( pSListEntry != NULL)
    {
        pCxtLink         = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSListEntry);
        pSListEntry      = AnscSListGetNextEntry(pSListEntry);

        AnscFreeMemory(pCxtLink->hContext);
        AnscFreeMemory(pCxtLink);
    }

    if ( pPoamIrepFo )
    {
        pPoamIrepFo->Remove( (ANSC_HANDLE)pPoamIrepFo);
    }

    /* Remove self */
    AnscFreeMemory((ANSC_HANDLE)pMyObject);

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        CosaDsliteBackendGetInfo
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the NextInstanceNumber for every table, Create
        the link tree. For delay_added entry, we also need create them.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of Dslite.
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CosaDsliteBackendGetInfo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DSLITE          pMyObject         = (PCOSA_DATAMODEL_DSLITE)hThisObject;
    PCOSA_DML_DSLITE                pCosaDslite       = NULL;
    ULONG                           tunnelCount       = 0;
    ULONG                           ulIndex           = 0;

    PCOSA_CONTEXT_LINK_OBJECT       pDsliteCxtLink    = NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pDsliteCxtLink2   = NULL;
    BOOL                            bNeedSave         = FALSE;
    errno_t                         rc                = -1;

    /* Get Device.DSLite.InterfaceSetting.{i} */
    CosaDmlDsliteGetNumberOfEntries(NULL, &tunnelCount);

    for ( ulIndex = 0; ulIndex < tunnelCount; ulIndex++ )
    {
        pCosaDslite  = (PCOSA_DML_DSLITE)AnscAllocateMemory( sizeof(COSA_DML_DSLITE) );
        if ( !pCosaDslite )
        {
            break;
        }

        DSLITE_SET_DEFAULTVALUE(pCosaDslite);
        returnStatus = CosaDmlDsliteGetEntry(NULL, ulIndex, pCosaDslite);
        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            AnscFreeMemory(pCosaDslite);
            break;
        }

        ULONG   deviceMode;
        if (CosaDmlDcGetDeviceMode(NULL, &deviceMode) == ANSC_STATUS_SUCCESS) {
         if(deviceMode != COSA_DML_DEVICE_MODE_Ipv6) {
          CosaDmlSetDsliteEnable(NULL, FALSE);
          pCosaDslite->active = FALSE;
          CosaDmlDsliteSetCfg(NULL, pCosaDslite);
         }
        }  
        pDsliteCxtLink = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory( sizeof(COSA_CONTEXT_LINK_OBJECT) );
        if ( !pDsliteCxtLink )
        {
            AnscFreeMemory(pCosaDslite);
            break;
        }

        COSA_CONTEXT_LINK_INITIATION_CONTENT(pDsliteCxtLink)
        pDsliteCxtLink->hContext       = (ANSC_HANDLE)pCosaDslite;
        pDsliteCxtLink->bNew           = FALSE;

        if ( !pCosaDslite->InstanceNumber )
        {
            if ( !++pMyObject->maxInstanceOfDslite )
            {
                pMyObject->maxInstanceOfDslite = 1;
            }
            bNeedSave                    = TRUE;

            pCosaDslite->InstanceNumber    = pMyObject->maxInstanceOfDslite;
            pDsliteCxtLink->InstanceNumber = pCosaDslite->InstanceNumber;
            rc = sprintf_s( pCosaDslite->alias, sizeof(pCosaDslite->alias),"Dslite.Tunnel.%lu", pCosaDslite->InstanceNumber);
            if(rc < EOK)
            {
              ERR_CHK(rc);
              AnscFreeMemory(pCosaDslite);
              AnscFreeMemory(pDsliteCxtLink);
              break;
            }

            returnStatus = CosaDmlDsliteSetInsNum
                            (
                                NULL,
                                ulIndex,
                                pCosaDslite->InstanceNumber
                            );

            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                AnscFreeMemory(pCosaDslite);
                AnscFreeMemory(pDsliteCxtLink);
                break;
            }

            /* Put into our list */
            CosaSListPushEntryByInsNum(&pMyObject->DsliteList, pDsliteCxtLink);
        }
        else
        {
            pDsliteCxtLink->InstanceNumber = pCosaDslite->InstanceNumber;

            if ( pMyObject->maxInstanceOfDslite < pDsliteCxtLink->InstanceNumber )
            {
                pMyObject->maxInstanceOfDslite  = pDsliteCxtLink->InstanceNumber;
                bNeedSave                       = TRUE;
            }

            /* if this entry is in link tree already because it's the parent of delay_added table.*/
            pDsliteCxtLink2 = (PCOSA_CONTEXT_LINK_OBJECT)CosaSListGetEntryByInsNum(&pMyObject->DsliteList, pDsliteCxtLink->InstanceNumber);
            if ( !pDsliteCxtLink2 )
            {
                CosaSListPushEntryByInsNum(&pMyObject->DsliteList, pDsliteCxtLink);
            }
            else
            {
                AnscFreeMemory( pDsliteCxtLink2->hContext );
                pDsliteCxtLink2->hContext       = (ANSC_HANDLE)pCosaDslite;
                if ( pDsliteCxtLink2->bNew )
                {
                    pDsliteCxtLink2->bNew       = FALSE;
                    bNeedSave                 = TRUE;
                }

                AnscFreeMemory(pDsliteCxtLink);
                pDsliteCxtLink                  = pDsliteCxtLink2;
                pDsliteCxtLink2                 = NULL;
            }
        }
    }

    /* Max InstanceNumber is changed. Save now.*/
    if (bNeedSave)
    {
        CosaDsliteRegSetDsliteInfo(pMyObject);
    }

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        CosaDsliteRegGetDsliteInfo
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve backend inform and put them into our trees.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of Dslite
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CosaDsliteRegGetDsliteInfo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DSLITE          pMyObject         = ( PCOSA_DATAMODEL_DSLITE )hThisObject;

    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoDslite    = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderDslite;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoEnumDslite  = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaDsliteContext     = NULL;
    PCOSA_DML_DSLITE                pCosaDslite          = NULL;

    PSLAP_VARIABLE                  pSlapVariable     = NULL;
    ULONG                           ulEntryCount      = 0;
    ULONG                           ulIndex           = 0;
    ULONG                           uInstanceNumber   = 0;
    BOOL                            bNew              = FALSE;
    char*                           pAliasDslite      = NULL;
    char*                           pFolderName       = NULL;
    errno_t                         rc                = -1;


    if ( !pPoamIrepFoDslite )
    {
        return ANSC_STATUS_FAILURE;
    }

    /* This is saved structure for DSLite.InterfaceSetting
        *****************************************
              <InterfaceSetting>
                  <NextInstanceNumber> xxx </>
                  <1>
                       <alias>xxx</>
                       <bNew>false</>
                 </1>
                 <2>
                      <alias>xxx</>
                      <bNew>false</>
                </2>
            </InterfaceSetting>
      ****************************************************
    */

    /* Get Maximum number */
    if ( TRUE )
    {
        pSlapVariable =
            (PSLAP_VARIABLE)pPoamIrepFoDslite->GetRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoDslite,
                    COSA_DML_RR_NAME_DsliteNextInsNumber,
                    NULL
                );

        if ( pSlapVariable )
        {
            pMyObject->maxInstanceOfDslite = pSlapVariable->Variant.varUint32;

            SlapFreeVariable(pSlapVariable);
        }
    }

    /* enumerate DSLite.InterfaceSetting.{i} */
    ulEntryCount = pPoamIrepFoDslite->GetFolderCount((ANSC_HANDLE)pPoamIrepFoDslite);
    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        /* Get i in DSLite.InterfaceSetting.{i}. */
        pFolderName =
            pPoamIrepFoDslite->EnumFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoDslite,
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

        pPoamIrepFoEnumDslite = pPoamIrepFoDslite->GetFolder((ANSC_HANDLE)pPoamIrepFoDslite, pFolderName);

        AnscFreeMemory(pFolderName);

        if ( !pPoamIrepFoEnumDslite )
        {
            continue;
        }

        /* Get InterfaceSetting.{i}.Alias value*/
        if ( TRUE )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoEnumDslite->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEnumDslite,
                        COSA_DML_RR_NAME_DsliteAlias,
                        NULL
                    );

            if ( pSlapVariable )
            {
                pAliasDslite = AnscCloneString(pSlapVariable->Variant.varString);

                SlapFreeVariable(pSlapVariable);
            }
        }

        /* Get InterfaceSetting.{i}.bNew value*/
        if ( TRUE )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoEnumDslite->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEnumDslite,
                        COSA_DML_RR_NAME_DslitebNew,
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
        pCosaDslite = (PCOSA_DML_DSLITE)AnscAllocateMemory(sizeof(COSA_DML_DSLITE));
        if ( !pCosaDslite )
        {
            returnStatus = ANSC_STATUS_FAILURE;
            goto EXIT1;
        }

        /* set some default value firstly */
        DSLITE_SET_DEFAULTVALUE(pCosaDslite);

        /* save alias and instanceNumber */
        pCosaDslite->InstanceNumber = uInstanceNumber;
        rc = strcpy_s( pCosaDslite->alias, sizeof(pCosaDslite->alias), pAliasDslite ? pAliasDslite : "");
        ERR_CHK(rc);

        /* Create one link point */
        pCosaDsliteContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
        if ( !pCosaDsliteContext )
        {
            returnStatus = ANSC_STATUS_FAILURE;
            goto EXIT2;
        }

        COSA_CONTEXT_LINK_INITIATION_CONTENT(pCosaDsliteContext)

        pCosaDsliteContext->InstanceNumber = uInstanceNumber;
        pCosaDsliteContext->hContext       = (ANSC_HANDLE)pCosaDslite;
        pCosaDslite                        = 0;
        pCosaDsliteContext->bNew           = bNew;

        CosaSListPushEntryByInsNum(&pMyObject->DsliteList, (PCOSA_CONTEXT_LINK_OBJECT)pCosaDsliteContext);

        /* release some memory */
        if (pAliasDslite)
        {
            AnscFreeMemory(pAliasDslite);
            pAliasDslite = NULL;
        }
        pPoamIrepFoEnumDslite->Remove((ANSC_HANDLE)pPoamIrepFoEnumDslite);
        pPoamIrepFoEnumDslite = NULL;
    }



EXIT2:
    if(pCosaDslite)
        AnscFreeMemory(pCosaDslite);

EXIT1:

    if(pAliasDslite)
        AnscFreeMemory(pAliasDslite);

    if ( pPoamIrepFoEnumDslite )
        pPoamIrepFoEnumDslite->Remove((ANSC_HANDLE)pPoamIrepFoEnumDslite);

    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        CosaDsliteRegSetDsliteInfo
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to save current NextInstanceNumber and Delay_added
        entry into sysregistry.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of Dslite
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CosaDsliteRegSetDsliteInfo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DSLITE          pMyObject         = (PCOSA_DATAMODEL_DSLITE)hThisObject;

    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoDslite          = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderDslite;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoEnumDslite      = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaDsliteContext         = NULL;
    PCOSA_DML_DSLITE                pCosaDslite                = NULL;

    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY)NULL;
    PSLAP_VARIABLE                  pSlapVariable     = NULL;
    char                            FolderName[16]    = {0};
    errno_t                         rc                = -1;


    if ( !pPoamIrepFoDslite )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pPoamIrepFoDslite->EnableFileSync((ANSC_HANDLE)pPoamIrepFoDslite, FALSE);
    }

    if ( TRUE )
    {
        pPoamIrepFoDslite->Clear((ANSC_HANDLE)pPoamIrepFoDslite);

        SlapAllocVariable(pSlapVariable);

        if ( !pSlapVariable )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT1;
        }
    }

    /* This is saved structure for DSLite.InterfaceSetting
        *****************************************
              <InterfaceSetting>
                  <NextInstanceNumber> xxx </>
                  <1>
                       <alias>xxx</>
                       <bNew>false</>
                 </1>
                 <2>
                      <alias>xxx</>
                      <bNew>false</>
                </2>
            </InterfaceSetting>
      ****************************************************
    */

    if ( TRUE )
    {
        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
        pSlapVariable->Variant.varUint32 = pMyObject->maxInstanceOfDslite;

        returnStatus =
            pPoamIrepFoDslite->AddRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoDslite,
                    COSA_DML_RR_NAME_DsliteNextInsNumber,
                    SYS_REP_RECORD_TYPE_UINT,
                    SYS_RECORD_CONTENT_DEFAULT,
                    pSlapVariable,
                    0
                );

        SlapCleanVariable(pSlapVariable);
        SlapInitVariable (pSlapVariable);
    }

    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->DsliteList);

    while ( pSLinkEntry )
    {
        /* create InterfaceSetting.{i} */

        pCosaDsliteContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        pSLinkEntry       = AnscSListGetNextEntry(pSLinkEntry);

        pCosaDslite = (PCOSA_DML_DSLITE)pCosaDsliteContext->hContext;

        if ( !pCosaDsliteContext->bNew)
        {
            continue;
        }

        rc = sprintf_s(FolderName, sizeof(FolderName),"%lu", pCosaDslite->InstanceNumber);
        if(rc < EOK)
        {
          ERR_CHK(rc);
          returnStatus = ANSC_STATUS_FAILURE;
          goto  EXIT1;
        }

        pPoamIrepFoEnumDslite =
            pPoamIrepFoDslite->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoDslite,
                    FolderName,
                    0
                );

        if ( !pPoamIrepFoEnumDslite )
        {
            continue;
        }

        /* add  InterfaceSetting.{i}.Alias */
        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;
            pSlapVariable->Variant.varString = AnscCloneString(pCosaDslite->alias);

            returnStatus =
                pPoamIrepFoEnumDslite->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEnumDslite,
                        COSA_DML_RR_NAME_DsliteAlias,
                        SYS_REP_RECORD_TYPE_ASTR,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );

            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }

        /* add  InterfaceSetting.{i}.bNew */
        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_bool;
            pSlapVariable->Variant.varBool   = pCosaDsliteContext->bNew;

            returnStatus =
                pPoamIrepFoEnumDslite->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEnumDslite,
                        COSA_DML_RR_NAME_DslitebNew,
                        SYS_REP_RECORD_TYPE_BOOL,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );

            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }

        /*release some resource */
        pPoamIrepFoEnumDslite->Remove((ANSC_HANDLE)pPoamIrepFoEnumDslite);
        pPoamIrepFoEnumDslite = NULL;
    }


EXIT1:
    if ( pSlapVariable )
    {
        SlapFreeVariable(pSlapVariable);
        pSlapVariable = NULL;
    }

    if ( pPoamIrepFoEnumDslite )
        pPoamIrepFoEnumDslite->Remove((ANSC_HANDLE)pPoamIrepFoEnumDslite);

    pPoamIrepFoDslite->EnableFileSync((ANSC_HANDLE)pPoamIrepFoDslite, TRUE);

    return returnStatus;
}
#endif
