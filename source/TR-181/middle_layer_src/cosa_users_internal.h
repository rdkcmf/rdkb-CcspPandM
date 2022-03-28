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

    module: cosa_users_internal.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**************************************************************************/


#ifndef  _COSA_USERS_INTERNAL_H
#define  _COSA_USERS_INTERNAL_H

#include "cosa_apis.h"
#include "cosa_users_dml.h"



#define  COSA_IREP_FOLDER_NAME_USER                       "User"
#define  COSA_IREP_FOLDER_NAME_USERS                      "Users"
#define  COSA_DML_RR_NAME_UserNextInsNunmber              "NextInstanceNumber"
#define  COSA_DML_RR_NAME_UserAlias                       "Username"
#define  COSA_DML_RR_NAME_UserbNew                        "bNew"

#define  COSA_DML_USER_ALIAS                             64
#define  COSA_USERS_ROLLBACK_TEST                        0
/*
*  This struct is for dhcp.
*/
#define  COSA_DATAMODEL_USERS_CLASS_CONTENT                                     \
    /* duplication of the base object class content */                                    \
    COSA_BASE_CONTENT                                                           \
    /* start of NAT object class content */                                             \
    SLIST_HEADER                    UserList;    /* This is for entry added */        \
    ULONG                           maxInstanceOfUser;                           \
    CHAR                            AliasOfUser[COSA_DML_USER_ALIAS];            \
    ANSC_HANDLE                     hIrepFolderUser;                             \

typedef  struct
_COSA_DATAMODEL_USERS                                               
{
    COSA_DATAMODEL_USERS_CLASS_CONTENT
}
COSA_DATAMODEL_USERS,  *PCOSA_DATAMODEL_USERS;

#define USERS_USER_SET_DEFAULTVALUE(pUser)                         \
    (pUser)->bEnabled                               = FALSE;       \
    (pUser)->RemoteAccessCapable                    = FALSE;       \
    AnscZeroMemory((pUser)->Password, sizeof((pUser)->Password) ); \
    AnscZeroMemory((pUser)->Language, sizeof((pUser)->Language) ); \
    (pUser)->NumOfFailedAttempts                    = '\0';       \
    AnscZeroMemory((pUser)->X_RDKCENTRAL_COM_ComparePassword, sizeof((pUser)->X_RDKCENTRAL_COM_ComparePassword) ); \
    AnscZeroMemory((pUser)->HashedPassword, sizeof((pUser)->HashedPassword) ); \
    (pUser)->AccessPermission = COSA_DML_USER_PERMISSION_HomeUser; \

/*
    Function declaration 
*/ 

ANSC_HANDLE
CosaUsersCreate
    (
        VOID
    );

ANSC_STATUS
CosaUsersInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaUsersRemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaUsersBackendGetUserInfo
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaUsersRegGetUserInfo
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
CosaUsersRegSetUserInfo
    (
        ANSC_HANDLE                 hThisObject
    );








#endif
