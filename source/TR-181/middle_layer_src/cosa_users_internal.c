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

    module: cosa_users_internal.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    copyright:

        Cisco Systems, Inc.
        All Rights Reserved.

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaUsersCreate
        *  CosaUsersInitialize
        *  CosaUsersRemove
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
#include "cosa_users_apis.h"
#include "cosa_users_internal.h"
#include "plugin_main_apis.h"
#include "poam_irepfo_interface.h"
#include "sys_definitions.h"

extern void * g_pDslhDmlAgent;

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        CosaUsersCreate
            (
            );

    description:

        This function constructs cosa nat object and return handle.

    argument:  

    return:     newly created nat object.

**********************************************************************/

ANSC_HANDLE
CosaUsersCreate
    (
        VOID
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_USERS           pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PCOSA_DATAMODEL_USERS)AnscAllocateMemory(sizeof(COSA_DATAMODEL_USERS));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->Oid               = COSA_DATAMODEL_USERS_OID;
    pMyObject->Create            = CosaUsersCreate;
    pMyObject->Remove            = CosaUsersRemove;
    pMyObject->Initialize        = CosaUsersInitialize;

    pMyObject->Initialize   ((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaUsersInitialize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate  cosa nat object and return handle.

    argument:    ANSC_HANDLE                 hThisObject
            This handle is actually the pointer of this object
            itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaUsersInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_USERS           pMyObject         = (PCOSA_DATAMODEL_USERS)hThisObject;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoCOSA   = NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoUser   = NULL;
 
    /* We need call the initiation function of backend firstly */
    returnStatus = CosaDmlUserInit( NULL, NULL );
    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        return returnStatus;
    }

    /* Initiation all functions */
    AnscSListInitializeHeader( &pMyObject->UserList );
    pMyObject->maxInstanceOfUser    = 0;
    pMyObject->hIrepFolderUser      = NULL;
    AnscZeroMemory(pMyObject->AliasOfUser, sizeof(pMyObject->AliasOfUser));

    /*We need to get Instance Info from cosa configuration*/
    pPoamIrepFoCOSA = (PPOAM_IREP_FOLDER_OBJECT)g_GetRegistryRootFolder(g_pDslhDmlAgent);

    if ( !pPoamIrepFoCOSA )
    {
        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT;
    }

    pPoamIrepFoUser = 
        (PPOAM_IREP_FOLDER_OBJECT)pPoamIrepFoCOSA->GetFolder
            (
                (ANSC_HANDLE)pPoamIrepFoCOSA,
                COSA_IREP_FOLDER_NAME_USER
            );

    if ( !pPoamIrepFoUser )
    {
        pPoamIrepFoCOSA->EnableFileSync((ANSC_HANDLE)pPoamIrepFoCOSA, FALSE);
    
        pPoamIrepFoUser =
            pPoamIrepFoCOSA->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoCOSA,
                    COSA_IREP_FOLDER_NAME_USER,
                    0
                );

        pPoamIrepFoCOSA->EnableFileSync((ANSC_HANDLE)pPoamIrepFoCOSA, TRUE);

    }

    if ( !pPoamIrepFoUser )
    {
        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT;
    }
    else
    {
        pMyObject->hIrepFolderUser = (ANSC_HANDLE)pPoamIrepFoUser;
    }

    /* We need get NextInstanceNumber from backend. By the way, the whole tree 
            was created. Moreover, we also need get delay-added entry and put them
            into our tree. */
    CosaUsersRegGetUserInfo((ANSC_HANDLE)pMyObject);

    /* Firstly we create the whole system from backend */
    CosaUsersBackendGetUserInfo((ANSC_HANDLE)pMyObject);

EXIT:
    
    return returnStatus;

}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaUsersRemove
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
CosaUsersRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_USERS           pMyObject           = (PCOSA_DATAMODEL_USERS)hThisObject;
    PSINGLE_LINK_ENTRY              pSListEntry         = NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink            = NULL;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFo         = (PPOAM_IREP_FOLDER_OBJECT)pMyObject->hIrepFolderUser;

    /* Remove necessary resource */
    pSListEntry         = AnscSListPopEntry(&pMyObject->UserList);
    while( pSListEntry != NULL)
    {
        pCxtLink         = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSListEntry);
        pSListEntry       = AnscSListGetNextEntry(pSListEntry);

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
        CosaUsersBackendGetUsernfo
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the NextInstanceNumber for every table, Create
        the link tree. For delay_added entry, we also need create them.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of user.
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CosaUsersBackendGetUserInfo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;    
    PCOSA_DATAMODEL_USERS           pMyObject         = (PCOSA_DATAMODEL_USERS)hThisObject;
    PCOSA_DML_USER                  pCosaUser         = NULL;
    ULONG                           clientCount       = 0;
    ULONG                           count             = 0;
    ULONG                           ulIndex           = 0;

    PCOSA_CONTEXT_LINK_OBJECT       pUserCxtLink      = NULL;    
    PCOSA_CONTEXT_LINK_OBJECT       pUserCxtLink2     = NULL;    
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = NULL;
    BOOL                            bNeedSave         = FALSE;

    /* Get Users.user.{i} */
    clientCount = CosaDmlUserGetNumberOfEntries(NULL);
    for ( ulIndex = 0; ulIndex < clientCount; ulIndex++ )
    {
        pCosaUser  = (PCOSA_DML_USER)AnscAllocateMemory( sizeof(COSA_DML_USER) );
        if ( !pCosaUser )
        {
            break;
        }

        USERS_USER_SET_DEFAULTVALUE(pCosaUser);
        pCosaUser->NumOfFailedAttempts=0;     
        returnStatus = CosaDmlUserGetEntry(NULL, ulIndex, pCosaUser);
        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            AnscFreeMemory(pCosaUser);
            break;
        }
        if (ulIndex == 2 && pCosaUser->HashedPassword[0]== '\0')
        {
          char buff[128]= {'\0'};
          hash_adminPassword(pCosaUser->Password,buff);

          syscfg_set(NULL, "hash_password_3", buff);
          syscfg_commit();
          _ansc_strncpy(pCosaUser->HashedPassword,buff,sizeof(pCosaUser->HashedPassword));
        }
        pUserCxtLink = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory( sizeof(COSA_CONTEXT_LINK_OBJECT) );
        if ( !pUserCxtLink )
        {
            AnscFreeMemory(pCosaUser);
            break;
        }

        COSA_CONTEXT_LINK_INITIATION_CONTENT(pUserCxtLink)
        pUserCxtLink->hContext       = (ANSC_HANDLE)pCosaUser;
        pUserCxtLink->bNew           = FALSE;

        if ( !pCosaUser->InstanceNumber )
        {
            if ( !++pMyObject->maxInstanceOfUser )
            {
                pMyObject->maxInstanceOfUser = 1;
            }
            bNeedSave                    = TRUE;

            pCosaUser->InstanceNumber    = pMyObject->maxInstanceOfUser;
            pUserCxtLink->InstanceNumber = pCosaUser->InstanceNumber;

            _ansc_sprintf(pCosaUser->Username, "Username%d", pCosaUser->InstanceNumber);

            returnStatus = CosaDmlUserSetValues
                            (
                                NULL,
                                ulIndex,
                                pCosaUser->InstanceNumber
                            );

            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                AnscFreeMemory(pCosaUser);
                AnscFreeMemory(pUserCxtLink);
                break;
            }

            /* Put into our list */
            CosaSListPushEntryByInsNum(&pMyObject->UserList, pUserCxtLink);
        }
        else
        {
            pUserCxtLink->InstanceNumber = pCosaUser->InstanceNumber;

            if ( pMyObject->maxInstanceOfUser < pUserCxtLink->InstanceNumber )
            {
                pMyObject->maxInstanceOfUser  = pUserCxtLink->InstanceNumber;                
                bNeedSave                     = TRUE;
            }
            
            /* if this entry is in link tree already because it's the parent of delay_added table.
                      */
            pUserCxtLink2 = (PCOSA_CONTEXT_LINK_OBJECT)CosaSListGetEntryByInsNum(&pMyObject->UserList, pUserCxtLink->InstanceNumber);
            if ( !pUserCxtLink2 )
            {
                CosaSListPushEntryByInsNum(&pMyObject->UserList, pUserCxtLink);
            }
            else
            {
                AnscFreeMemory( pUserCxtLink2->hContext );
                pUserCxtLink2->hContext       = (ANSC_HANDLE)pCosaUser;
                if ( pUserCxtLink2->bNew )
                {
                    pUserCxtLink2->bNew       = FALSE;
                    bNeedSave                 = TRUE;
                }

                AnscFreeMemory(pUserCxtLink);
                pUserCxtLink                  = pUserCxtLink2;
                pUserCxtLink2                 = NULL;
            }            
        }
    }
    
    /* Max InstanceNumber is changed. Save now.*/
    if (bNeedSave)
    {
        CosaUsersRegSetUserInfo(pMyObject);
    }

    return returnStatus;    
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        CosaUsersRegGetUserInfo
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve backend inform and put them into our trees.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of user
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CosaUsersRegGetUserInfo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_USERS           pMyObject         = ( PCOSA_DATAMODEL_USERS )hThisObject;

    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoUser      = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderUser;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoEnumUser  = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaUserContext  = NULL;
    PCOSA_DML_USER                  pCosaUser         = NULL;
    
    PSLAP_VARIABLE                  pSlapVariable     = NULL;
    ULONG                           ulEntryCount      = 0;
    ULONG                           ulIndex           = 0;
    ULONG                           uInstanceNumber   = 0;
    BOOL                            bNew              = FALSE;
    char*                           pAliasUser        = NULL;
    char*                           pFolderName       = NULL;


    if ( !pPoamIrepFoUser )
    {
        return ANSC_STATUS_FAILURE;
    }

    /* This is saved structure for user
        *****************************************
              <User>
                  <NextInstanceNumber> xxx </>
                  <1>
                       <alias>xxx</>
                       <bNew>false</>
                 </1>
                 <2>
                      <alias>xxx</>
                      <bNew>false</>
                </2>
            </User>
      ****************************************************
      */

    /* Get Maximum number */
    if ( TRUE )
    {
        pSlapVariable =
            (PSLAP_VARIABLE)pPoamIrepFoUser->GetRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoUser,
                    COSA_DML_RR_NAME_UserNextInsNunmber,
                    NULL
                );

        if ( pSlapVariable )
        {
            pMyObject->maxInstanceOfUser = pSlapVariable->Variant.varUint32;

            SlapFreeVariable(pSlapVariable);
        }
    }

    /* enumerate user.{i} */
    ulEntryCount = pPoamIrepFoUser->GetFolderCount((ANSC_HANDLE)pPoamIrepFoUser);
    for ( ulIndex = 0; ulIndex < ulEntryCount; ulIndex++ )
    {
        /* Get i in user.{i} */
        pFolderName =
            pPoamIrepFoUser->EnumFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoUser,
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

        pPoamIrepFoEnumUser = pPoamIrepFoUser->GetFolder((ANSC_HANDLE)pPoamIrepFoUser, pFolderName);

        AnscFreeMemory(pFolderName);

        if ( !pPoamIrepFoEnumUser )
        {
            continue;
        }

        /* Get user.{i}.Username value*/
        if ( TRUE )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoEnumUser->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEnumUser,
                        COSA_DML_RR_NAME_UserAlias,
                        NULL
                    );

            if ( pSlapVariable )
            {
                pAliasUser = AnscCloneString(pSlapVariable->Variant.varString);

                SlapFreeVariable(pSlapVariable);
            }
        }

        /* Get user.{i}.bNew value*/
        if ( TRUE )
        {
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoEnumUser->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEnumUser,
                        COSA_DML_RR_NAME_UserbNew,
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
        pCosaUser = (PCOSA_DML_USER)AnscAllocateMemory(sizeof(COSA_DML_USER));
        if ( !pCosaUser )
        {
            returnStatus = ANSC_STATUS_FAILURE;            
            goto EXIT1;
        }

        /* set some default value firstly */
        USERS_USER_SET_DEFAULTVALUE(pCosaUser);

        /* save alias and instanceNumber */
        pCosaUser->InstanceNumber = uInstanceNumber;
        AnscCopyString( pCosaUser->Username, pAliasUser );

        /* Create one link point */
        pCosaUserContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
        if ( !pCosaUserContext )
        {
            returnStatus = ANSC_STATUS_FAILURE;
            goto EXIT2;
        }

        COSA_CONTEXT_LINK_INITIATION_CONTENT(pCosaUserContext)

        pCosaUserContext->InstanceNumber = uInstanceNumber;
        pCosaUserContext->hContext       = (ANSC_HANDLE)pCosaUser;
        pCosaUser                        = 0;
        pCosaUserContext->bNew           = bNew;

        CosaSListPushEntryByInsNum(&pMyObject->UserList, (PCOSA_CONTEXT_LINK_OBJECT)pCosaUserContext);

        /* release some memory */
        if (pAliasUser)
        {
            AnscFreeMemory(pAliasUser);
            pAliasUser = NULL;
        }
        
        pPoamIrepFoEnumUser->Remove((ANSC_HANDLE)pPoamIrepFoEnumUser);
        pPoamIrepFoEnumUser = NULL;
    }


    
EXIT2:
    if(pCosaUser)
        AnscFreeMemory(pCosaUser);
        
EXIT1:
    
    if(pAliasUser)
        AnscFreeMemory(pAliasUser);
        
    if ( pPoamIrepFoEnumUser )
        pPoamIrepFoEnumUser->Remove((ANSC_HANDLE)pPoamIrepFoEnumUser);

    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        CosaUsersRegSetUserInfo
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to save current NextInstanceNumber and Delay_added
        entry into sysregistry.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of user
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CosaUsersRegSetUserInfo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_USERS           pMyObject         = (PCOSA_DATAMODEL_USERS   )hThisObject;
    
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoUser          = (PPOAM_IREP_FOLDER_OBJECT )pMyObject->hIrepFolderUser;
    PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoEnumUser      = (PPOAM_IREP_FOLDER_OBJECT )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaUserContext  = NULL;
    PCOSA_DML_USER                  pCosaUser         = NULL;

    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY       )NULL;    
    PSLAP_VARIABLE                  pSlapVariable     = NULL;
    ULONG                           ulEntryCount      = 0;
    ULONG                           ulIndex           = 0;
    ULONG                           uInstanceNumber   = 0;
    char*                           pAliasUser        = NULL;
    char*                           pFolderName       = NULL;
    char                            FolderName[16]    = {0};

    
    if ( !pPoamIrepFoUser )
    {
        return ANSC_STATUS_FAILURE;
    }
    else
    {
        pPoamIrepFoUser->EnableFileSync((ANSC_HANDLE)pPoamIrepFoUser, FALSE);
    }

    if ( TRUE )
    {
        pPoamIrepFoUser->Clear((ANSC_HANDLE)pPoamIrepFoUser);

        SlapAllocVariable(pSlapVariable);

        if ( !pSlapVariable )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT1;
        }
    }

    /* This is saved structure for user
        *****************************************
              <User>
                  <NextInstanceNumber> xxx </>
                  <1>
                       <alias>xxx</>
                       <bNew>false</>
                 </1>
                 <2>
                      <alias>xxx</>
                      <bNew>false</>
                </2>
            </User>
      ****************************************************
      */

    /* add client.{i}.maxInstanceNumber  */
    if ( TRUE )
    {
        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_uint32;
        pSlapVariable->Variant.varUint32 = pMyObject->maxInstanceOfUser;
    
        returnStatus =
            pPoamIrepFoUser->AddRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoUser,
                    COSA_DML_RR_NAME_UserNextInsNunmber,
                    SYS_REP_RECORD_TYPE_UINT,
                    SYS_RECORD_CONTENT_DEFAULT,
                    pSlapVariable,
                    0
                );
    
        SlapCleanVariable(pSlapVariable);
        SlapInitVariable (pSlapVariable);
    }
    
    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->UserList);

    while ( pSLinkEntry )
    {
        /* create user.{i} */
        
        pCosaUserContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        pSLinkEntry       = AnscSListGetNextEntry(pSLinkEntry);

        pCosaUser = (PCOSA_DML_USER)pCosaUserContext->hContext;

        if ( !pCosaUserContext->bNew)
        {
            continue;
        }

        _ansc_sprintf(FolderName, "%d", pCosaUser->InstanceNumber);

        pPoamIrepFoEnumUser =
            pPoamIrepFoUser->AddFolder
                (
                    (ANSC_HANDLE)pPoamIrepFoUser,
                    FolderName,
                    0
                );

        if ( !pPoamIrepFoEnumUser )
        {
            continue;
        }

        /* add  user.{i}.Username */
        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;
            pSlapVariable->Variant.varString = AnscCloneString(pCosaUser->Username);

            returnStatus =
                pPoamIrepFoEnumUser->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEnumUser,
                        COSA_DML_RR_NAME_UserAlias,
                        SYS_REP_RECORD_TYPE_ASTR,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );

            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }

        /* add  user.{i}.bNew */
        if ( TRUE )
        {
            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_bool;
            pSlapVariable->Variant.varBool   = pCosaUserContext->bNew;

            returnStatus =
                pPoamIrepFoEnumUser->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoEnumUser,
                        COSA_DML_RR_NAME_UserbNew,
                        SYS_REP_RECORD_TYPE_BOOL,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pSlapVariable,
                        0
                    );

            SlapCleanVariable(pSlapVariable);
            SlapInitVariable (pSlapVariable);
        }
       

        /*release some resource */        
        pPoamIrepFoEnumUser->Remove((ANSC_HANDLE)pPoamIrepFoEnumUser);
        pPoamIrepFoEnumUser = NULL;
    }
        

EXIT1:
    if ( pSlapVariable )
    {
        SlapFreeVariable(pSlapVariable);
        pSlapVariable = NULL;
    }

    if ( pPoamIrepFoEnumUser )
        pPoamIrepFoEnumUser->Remove((ANSC_HANDLE)pPoamIrepFoEnumUser);

    pPoamIrepFoUser->EnableFileSync((ANSC_HANDLE)pPoamIrepFoUser, TRUE);

    return returnStatus;
}



