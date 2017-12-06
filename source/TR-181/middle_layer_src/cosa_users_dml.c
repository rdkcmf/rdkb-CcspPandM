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

    module: cosa_users_dml.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    copyright:

        Cisco Systems, Inc.
        All Rights Reserved.

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

    -------------------------------------------------------------------

    environment:

        platform independent

    -------------------------------------------------------------------

    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        01/14/2011    initial revision.

**************************************************************************/

#include "ansc_platform.h"
#include "cosa_users_dml.h"
#include "cosa_users_apis.h"
#include "plugin_main_apis.h"
#include "cosa_users_internal.h"

#include "dml_tr181_custom_cfg.h"

#if     CFG_USE_CCSP_SYSLOG
    #include <ccsp_syslog.h>
#endif


void ResetFailedAttepmts(PCOSA_DML_USER  pEntry)
{
	printf("Inside ResetFailedAttepmts\n");
        pthread_detach(pthread_self());
        char buf[10];
        int lockoutTime =0 ;

        memset(buf,0,sizeof(buf));
        syscfg_get( NULL, "PasswordLockoutTime", buf, sizeof(buf));
        if( buf != NULL )
       {
                lockoutTime=atoi(buf);
       }
        usleep(1000*lockoutTime);
        pEntry->NumOfFailedAttempts=0;

}

/***********************************************************************
 IMPORTANT NOTE:

 According to TR69 spec:
 On successful receipt of a SetParameterValues RPC, the CPE MUST apply 
 the changes to all of the specified Parameters atomically. That is, either 
 all of the value changes are applied together, or none of the changes are 
 applied at all. In the latter case, the CPE MUST return a fault response 
 indicating the reason for the failure to apply the changes. 
 
 The CPE MUST NOT apply any of the specified changes without applying all 
 of them.

 In order to set parameter values correctly, the back-end is required to
 hold the updated values until "Validate" and "Commit" are called. Only after
 all the "Validate" passed in different objects, the "Commit" will be called.
 Otherwise, "Rollback" will be called instead.

 The sequence in COSA Data Model will be:

 SetParamBoolValue/SetParamIntValue/SetParamUlongValue/SetParamStringValue
 -- Backup the updated values;

 if( Validate_XXX())
 {
     Commit_XXX();    -- Commit the update all together in the same object
 }
 else
 {
     Rollback_XXX();  -- Remove the update at backup;
 }
 
***********************************************************************/
/***********************************************************************

 APIs for Object:

    Users.


***********************************************************************/
/***********************************************************************

 APIs for Object:

    Users.User.{i}.

    *  User_GetEntryCount
    *  User_GetEntry
    *  User_AddEntry
    *  User_DelEntry
    *  User_GetParamBoolValue
    *  User_GetParamIntValue
    *  User_GetParamUlongValue
    *  User_GetParamStringValue
    *  User_SetParamBoolValue
    *  User_SetParamIntValue
    *  User_SetParamUlongValue
    *  User_SetParamStringValue
    *  User_Validate
    *  User_Commit
    *  User_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        User_GetEntryCount
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG
User_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_USERS           pUsers           = (PCOSA_DATAMODEL_USERS)g_pCosaBEManager->hUsers;

    return AnscSListQueryDepth( &pUsers->UserList );
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        User_GetEntry
            (
                ANSC_HANDLE                 hInsContext,
                ULONG                       nIndex,
                ULONG*                      pInsNumber
            );

    description:

        This function is called to retrieve the entry specified by the index.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ULONG                       nIndex,
                The index of this entry;

                ULONG*                      pInsNumber
                The output instance number;

    return:     The handle to identify the entry

**********************************************************************/
ANSC_HANDLE
User_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_USERS           pUsers            = (PCOSA_DATAMODEL_USERS)g_pCosaBEManager->hUsers;
    PSINGLE_LINK_ENTRY              pSListEntry       = NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = NULL;

    pSListEntry = AnscSListGetEntryByIndex(&pUsers->UserList, nIndex);

    if ( pSListEntry )
    {
        pCxtLink          = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSListEntry);
        *pInsNumber       = pCxtLink->InstanceNumber;
    }

    return pSListEntry;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        User_AddEntry
            (
                ANSC_HANDLE                 hInsContext,
                ULONG*                      pInsNumber
            );

    description:

        This function is called to add a new entry.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ULONG*                      pInsNumber
                The output instance number;

    return:     The handle of new added entry.

**********************************************************************/
ANSC_HANDLE
User_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PSINGLE_LINK_ENTRY              pSListEntry       = NULL;
    PCOSA_DATAMODEL_USERS           pUsers            = (PCOSA_DATAMODEL_USERS)g_pCosaBEManager->hUsers;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = NULL;
    PCOSA_DML_USER                  pUser             = NULL;
    CHAR                            tmpBuff[64]       = {0};
    ULONG                           i                 = 0;
    
    pUser  = (PCOSA_DML_USER)AnscAllocateMemory( sizeof(COSA_DML_USER) );
    if ( !pUser )
    {
        goto EXIT2;
    }

    /* Set default value */
    USERS_USER_SET_DEFAULTVALUE(pUser);

    pCxtLink = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory( sizeof(COSA_CONTEXT_LINK_OBJECT) );
    if ( !pUser )
    {
        goto EXIT1;
    }
    
    pCxtLink->hContext       = (ANSC_HANDLE)pUser;
    pCxtLink->bNew           =  TRUE;

    if ( !++pUsers->maxInstanceOfUser )
    {
        pUsers->maxInstanceOfUser   = 1;
    }    

    pUser->InstanceNumber    = pUsers->maxInstanceOfUser;
    pCxtLink->InstanceNumber = pUser->InstanceNumber;
    *pInsNumber              = pUser->InstanceNumber;

    _ansc_sprintf( pUser->Username, "User%d", pUser->InstanceNumber);

    /* Put into our list */
    CosaSListPushEntryByInsNum(&pUsers->UserList, pCxtLink);
    
    /* we recreate the configuration because we has new delay_added entry for users */
    CosaUsersRegSetUserInfo(pUsers);

    return (ANSC_HANDLE)pCxtLink;


EXIT1:
    
    AnscFreeMemory(pUser);

EXIT2:        
    
    return NULL; /* return the handle */
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        User_DelEntry
            (
                ANSC_HANDLE                 hInsContext,
                ANSC_HANDLE                 hInstance
            );

    description:

        This function is called to delete an exist entry.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ANSC_HANDLE                 hInstance
                The exist entry handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
User_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PSINGLE_LINK_ENTRY              pSListEntry       = NULL;
    PCOSA_DATAMODEL_USERS           pUsers            = (PCOSA_DATAMODEL_USERS)g_pCosaBEManager->hUsers;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_DML_USER                  pUser             = (PCOSA_DML_USER)pCxtLink->hContext;
    
    if ( !pCxtLink->bNew )
    {
        returnStatus = CosaDmlUserDelEntry(NULL, pUser->InstanceNumber);
        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            return returnStatus;
        }
    }
    
    /* Firstly we del this link point from our list */
    if (AnscSListPopEntryByLink(&pUsers->UserList, &pCxtLink->Linkage) )
    {
        CosaUsersRegSetUserInfo(pUsers);
        
        AnscFreeMemory(pCxtLink->hContext);
        AnscFreeMemory(pCxtLink);
    }

    return returnStatus;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        User_GetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL*                       pBool
            );

    description:

        This function is called to retrieve Boolean parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL*                       pBool
                The buffer of returned boolean value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
User_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_USER                  pUser             = (PCOSA_DML_USER)pCxtLink->hContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /*
         * Always read password from backend
         */
        CosaDmlUserGetCfg(NULL, pUser);

        /* collect value */
        *pBool  =  pUser->bEnabled;
        
        return TRUE;
    }

    if( AnscEqualString(ParamName, "RemoteAccessCapable", TRUE))
    {
        /* collect value */
        *pBool  =  pUser->RemoteAccessCapable;
        
        return TRUE;
    }
    if ( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_PasswordReset", TRUE))
    {
        *pBool = FALSE;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        User_GetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int*                        pInt
            );

    description:

        This function is called to retrieve integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The buffer of returned integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
User_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        User_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

    description:

        This function is called to retrieve ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
User_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_USER                  pUser             = (PCOSA_DML_USER)pCxtLink->hContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "X_CISCO_COM_AccessPermission", TRUE))
    {
        /* collect value */
        *puLong = pUser->AccessPermission;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "NumOfFailedAttempts", TRUE))
    {
        /* collect value */
        *puLong = pUser->NumOfFailedAttempts;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        User_GetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pValue,
                ULONG*                      pUlSize
            );

    description:

        This function is called to retrieve string parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue,
                The string value buffer;

                ULONG*                      pUlSize
                The buffer of length of string value;
                Usually size of 1023 will be used.
                If it's not big enough, put required size here and return 1;

    return:     0 if succeeded;
                1 if short of buffer size; (*pUlSize = required size)
                -1 if not supported.

**********************************************************************/
ULONG
User_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_USER                  pUser             = (PCOSA_DML_USER)pCxtLink->hContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Username", TRUE))
    {
        /*
         * Always read password from backend
         */
        CosaDmlUserGetCfg(NULL, pUser);

        /* collect value */
        if ( AnscSizeOfString(pUser->Username) < *pUlSize)
        {
            AnscCopyString(pValue, pUser->Username);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pUser->Username)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "Password", TRUE))
    {
        /* collect value */
        /* This is a hidden parameter, so return EMPTY */
        AnscCopyString(pValue,"");
        return 0;
    }

    if( AnscEqualString(ParamName, "Language", TRUE))
    {
        /* collect value */
        if ( AnscSizeOfString(pUser->Language) < *pUlSize)
        {
            AnscCopyString(pValue, pUser->Language);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pUser->Language)+1;
            return 1;
        }
    }
    if( AnscEqualString(ParamName, "X_CISCO_COM_Password", TRUE))
    {
        /* Collect Value */
        /* This is an extn parameter to display password */
        
        /*
         * Always read password from backend
         */
        CosaDmlUserGetCfg(NULL, pUser);
        
        if ( AnscSizeOfString(pUser->Password) < *pUlSize)
        {
            ANSC_STATUS returnStatus = ANSC_STATUS_SUCCESS;
            if( AnscEqualString(pUser->Username, "admin",TRUE) )
            {
               AnscCopyString(pValue,pUser->HashedPassword);
               return 0;
            }
#if defined(_COSA_FOR_BCI_)
            if( AnscEqualString(pUser->Username, "cusadmin",TRUE) )
            {
               AnscCopyString(pValue,pUser->HashedPassword);
               return 0;
            }
#endif
            AnscCopyString(pValue, pUser->Password);
            if( AnscEqualString(pUser->Username, "mso", TRUE) )
            {
            AnscCopyString(pUser->Password, "Invalid_PWD");
            returnStatus = CosaDmlUserSetCfg(NULL, pUser);

               if ( returnStatus != ANSC_STATUS_SUCCESS)
               {
                  CosaDmlUserGetCfg(NULL, pUser);
               }
            }
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pUser->Password)+1;
            return 1;
        }
     }
     if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_ComparePassword", TRUE))
     {
        if ( AnscSizeOfString(pUser->X_RDKCENTRAL_COM_ComparePassword) < *pUlSize)
        {
            AnscCopyString(pValue, pUser->X_RDKCENTRAL_COM_ComparePassword);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pUser->X_RDKCENTRAL_COM_ComparePassword)+1;
            return 1;
        }
     }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        User_SetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL                        bValue
            );

    description:

        This function is called to set BOOL parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL                        bValue
                The updated BOOL value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
User_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_USER                  pUser             = (PCOSA_DML_USER)pCxtLink->hContext;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* save update to backup */
        pUser->bEnabled   =  bValue;
        
        return TRUE;
    }

    if( AnscEqualString(ParamName, "RemoteAccessCapable", TRUE))
    {
        /* save update to backup */
        pUser->RemoteAccessCapable   =  bValue;
        
        return TRUE;
    }
    if(AnscEqualString(ParamName,"X_RDKCENTRAL-COM_PasswordReset", TRUE))
    {
        CosaDmlUserResetPassword(bValue,pUser);
        CcspTraceInfo(("Password reset done for %s user\n",pUser->Username));
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        User_SetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int                         iValue
            );

    description:

        This function is called to set integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int                         iValue
                The updated integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
User_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        User_SetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG                       uValue
            );

    description:

        This function is called to set ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue
                The updated ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
User_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_USER                  pUser             = (PCOSA_DML_USER)pCxtLink->hContext;

    /* check the parameter name and set the corresponding value */

    if( AnscEqualString(ParamName, "X_CISCO_COM_AccessPermission", TRUE))
    {
        /* save update to backup */
        pUser->AccessPermission   =  uValue;

        return TRUE;
    }


    if( AnscEqualString(ParamName, "NumOfFailedAttempts", TRUE))
    {
        /* collect value */
    	char buf[10];
 	int MaxFailureAttempts = 0;
	int lockoutState=0;
        pUser->NumOfFailedAttempts = uValue;
    	memset(buf,0,sizeof(buf));
        syscfg_get( NULL, "PasswordLockoutAttempts", buf, sizeof(buf));
        if( buf != NULL )
       {
		MaxFailureAttempts=atoi(buf);
		
       }
         
	if ( MaxFailureAttempts == pUser->NumOfFailedAttempts )
	{
		//action required
			lockoutState=1;
			pthread_t rstattempt;
			pthread_create(&rstattempt, NULL, &ResetFailedAttepmts, (void *)pUser);
		CcspTraceWarning(("WebUI Login:  Num of invalid attempt is %d, WebUI is locked out for %s User\n", pUser->NumOfFailedAttempts, pUser->Username));
	} 
	else if ( MaxFailureAttempts <= pUser->NumOfFailedAttempts)
	{
		CcspTraceWarning(("WebUI Login: Num of invalid attempt is %d, WebUI is locked out for %s User \n", pUser->NumOfFailedAttempts, pUser->Username));
		lockoutState=1;
	}
	else
	{
	
        	CcspTraceWarning(("WebUI Login: Num of invalid attempt is %d, WebUI is not locked out for %s User\n", pUser->NumOfFailedAttempts,pUser->Username));
	}	

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        User_SetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pString
            );

    description:

        This function is called to set string parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pString
                The updated string value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
User_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_USER                  pUser             = (PCOSA_DML_USER)pCxtLink->hContext;
    PCOSA_DATAMODEL_USERS           pUsers            = (PCOSA_DATAMODEL_USERS)g_pCosaBEManager->hUsers;
    PSINGLE_LINK_ENTRY              pSListEntry       = NULL;
    PCOSA_DML_USER                  pUser2            = NULL;
    BOOL                            bFound            = FALSE;


    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Username", TRUE))
    {
        return FALSE;    /* In USG, webgui username is not allowed to change */
#if 0
        AnscCopyString(pUsers->AliasOfUser, pUser->Username);
        AnscCopyString(pUser->Username, pString);
        
        return TRUE;
#endif
    }

    if( AnscEqualString(ParamName, "Password", TRUE)
        || AnscEqualString(ParamName, "X_CISCO_COM_Password", TRUE) )
    {
	if( AnscEqualString(pUser->Username, "mso", TRUE) )
	{
		unsigned int ret=0;

		ret = mso_validatepwd(pString);

		if ( ret == Invalid_PWD )
		{
			AnscCopyString(pUser->Password, "Invalid_PWD");
		}
		else if ( ret == Good_PWD )
		{
			AnscCopyString(pUser->Password, "Good_PWD");
		}
		else if ( ret == Unique_PWD )
		{
			AnscCopyString(pUser->Password, "Unique_PWD");
		}
		else if ( ret == Expired_PWD )
		{
			AnscCopyString(pUser->Password, "Expired_PWD");
		}
		else
		{
			AnscCopyString(pUser->Password, "TimeError");
		}

	}
        else if( AnscEqualString(pUser->Username, "admin", TRUE) )
	{
		unsigned int ret=0;
		char resultBuffer[32]= {'\0'};
		user_hashandsavepwd(NULL,pString,pUser);
                AnscCopyString(pUser->Password, pString);
                CcspTraceInfo(("WebUi admin password is changed\n"));
	}
#if defined(_COSA_FOR_BCI_)
        else if( AnscEqualString(pUser->Username, "cusadmin", TRUE) )
        {
                unsigned int ret=0;
                char resultBuffer[32]= {'\0'};
                user_hashandsavepwd(NULL,pString,pUser);
                AnscCopyString(pUser->Password, pString);
                CcspTraceInfo(("WebUi admin password is changed\n"));
        }
#endif
	else
	{
        	/* save update to backup */
       		 AnscCopyString(pUser->Password, pString);
	}
    #if CFG_USE_CCSP_SYSLOG
        /* Bad practice to use platform dependent and will be rectified -- CCSP_TRACE should be used */
        if( AnscEqualString(pUser->Username, "admin", TRUE) )
        {    syslog_systemlog("Password change", LOG_NOTICE, "Account %s's password changed", pUser->Username);
             return TRUE;
        }  
    #endif

        return TRUE;
    }

    if(AnscEqualString(ParamName,"X_RDKCENTRAL-COM_ComparePassword",TRUE))
    {

       unsigned int ret=0;
       char resultBuffer[32]= {'\0'};
       user_validatepwd(NULL,pString,pUser,resultBuffer);
       AnscCopyString(pUser->X_RDKCENTRAL_COM_ComparePassword, resultBuffer);
       return TRUE;
    }

    if( AnscEqualString(ParamName, "Language", TRUE))
    {
        /* save update to backup */
        AnscCopyString(pUser->Language, pString);

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        User_Validate
            (
                ANSC_HANDLE                 hInsContext,
                char*                       pReturnParamName,
                ULONG*                      puLength
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       pReturnParamName,
                The buffer (128 bytes) of parameter name if there's a validation. 

                ULONG*                      puLength
                The output length of the param name. 

    return:     TRUE if there's no validation.

**********************************************************************/
BOOL
User_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_USERS           pUsers            = (PCOSA_DATAMODEL_USERS)g_pCosaBEManager->hUsers;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_USER                  pUser             = (PCOSA_DML_USER)pCxtLink->hContext;
    PSINGLE_LINK_ENTRY              pSListEntry       = NULL;
    PCOSA_DML_USER                  pUser2            = NULL;
    BOOL                            bFound            = FALSE;

    if ( pUsers->AliasOfUser[0] )
    {
        bFound                = FALSE;
        pSListEntry           = AnscSListGetFirstEntry(&pUsers->UserList);
        while( pSListEntry != NULL)
        {
            pCxtLink          = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSListEntry);
            pSListEntry       = AnscSListGetNextEntry(pSListEntry);
        
            pUser2 = (PCOSA_DML_USER)pCxtLink->hContext;
        
            if( AnscEqualString(pUser2->Username, pUser->Username, TRUE) )
            {
                if ( (ANSC_HANDLE)pCxtLink == hInsContext )
                {
                    continue;
                }
        
                bFound = TRUE;
                
                break;
            }
        }
        
        if ( bFound )
        {
#if COSA_USERS_ROLLBACK_TEST        
            User_Rollback(hInsContext);
#endif
            return FALSE;
        }
    }



    
    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        User_Commit
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
User_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_USER                  pUser             = (PCOSA_DML_USER)pCxtLink->hContext;
    PCOSA_DATAMODEL_USERS           pUsers            = (PCOSA_DATAMODEL_USERS)g_pCosaBEManager->hUsers;

    if ( pCxtLink->bNew )
    {
        returnStatus = CosaDmlUserAddEntry(NULL, pUser );

        if ( returnStatus == ANSC_STATUS_SUCCESS )
        {
            pCxtLink->bNew = FALSE;

            CosaUsersRegSetUserInfo(pUsers);
        }
        else
        {
            USERS_USER_SET_DEFAULTVALUE(pUser);
            
            if ( pUsers->AliasOfUser[0] )
                AnscCopyString( pUser->Username, pUsers->AliasOfUser );
        }
    }
    else
    {
        returnStatus = CosaDmlUserSetCfg(NULL, pUser);

        if ( returnStatus != ANSC_STATUS_SUCCESS)
        {
            CosaDmlUserGetCfg(NULL, pUser);
        }
    }

    AnscZeroMemory( pUsers->AliasOfUser, sizeof(pUsers->AliasOfUser) );

    return returnStatus;

}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        User_Rollback
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to roll back the update whenever there's a 
        validation found.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
User_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_USERS           pUsers            = (PCOSA_DATAMODEL_USERS)g_pCosaBEManager->hUsers;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_USER                  pUser             = (PCOSA_DML_USER)pCxtLink->hContext;

    if ( pUsers->AliasOfUser[0] )
        AnscCopyString( pUser->Username, pUsers->AliasOfUser );

    if ( !pCxtLink->bNew )
    {
        CosaDmlUserGetCfg( NULL, pUser );
    }
    else
    {
        USERS_USER_SET_DEFAULTVALUE(pUser);
    }

    AnscZeroMemory( pUsers->AliasOfUser, sizeof(pUsers->AliasOfUser) );

    return returnStatus;

}

