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

    module: cosa_users_apis.c

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

        01/11/2011    initial revision.

**************************************************************************/
#include "cosa_apis.h"
#include "cosa_users_apis.h"
#include "cosa_users_internal.h"
#include "plugin_main_apis.h"
#include <openssl/hmac.h>

char SerialNumber[64] = {'\0'};
#if ( defined _COSA_SIM_ )

COSA_DML_USER  g_users_user[] = 
{
    {
        1,
        TRUE,
        TRUE,
        "Sam",
        "XDFSXE",
        "En",
		"0"
    },
    {
        2,
        TRUE,
        TRUE,
        "Jerry",
        "X72SXE",
        "Fr",
		"0"
    }
};



ANSC_STATUS
CosaDmlUserInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;

    return returnStatus;
}

ULONG
CosaDmlUserGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;

    return sizeof(g_users_user)/sizeof(COSA_DML_USER);

}

ANSC_STATUS
CosaDmlUserGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_USER              pEntry
    )
{
    if ( ulIndex+1 > sizeof(g_users_user)/sizeof(COSA_DML_USER))
        return ANSC_STATUS_FAILURE;

    AnscCopyMemory( pEntry, &g_users_user[ulIndex], sizeof(COSA_DML_USER));

    return ANSC_STATUS_SUCCESS;

}

ANSC_STATUS
CosaDmlUserSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber
    )
{
    if ( ulIndex+1 > sizeof(g_users_user)/sizeof(COSA_DML_USER))
        return ANSC_STATUS_SUCCESS;

    g_users_user[ulIndex].InstanceNumber  = ulInstanceNumber;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlUserAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_USER              pEntry
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;

    return returnStatus;

}

ANSC_STATUS
CosaDmlUserDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;

    return returnStatus;

}

ANSC_STATUS
CosaDmlUserSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_USER              pEntry      /* Identified by InstanceNumber */
    )
{
    ULONG                          index      = 0;

    for ( index = 0 ;index < sizeof(g_users_user)/sizeof(COSA_DML_USER ); index++)
    {
        if ( pEntry->InstanceNumber == g_users_user[index].InstanceNumber )
        {
            AnscCopyMemory( &g_users_user[index], pEntry, sizeof(COSA_DML_USER ));
            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlUserGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_USER              pEntry      /* Identified by InstanceNumber */
    )
{
    ULONG                          index      = 0;

    for ( index = 0 ;index < sizeof(g_users_user)/sizeof(COSA_DML_USER ); index++)
    {
        if ( pEntry->InstanceNumber == g_users_user[index].InstanceNumber )
        {
            AnscCopyMemory( pEntry, &g_users_user[index], sizeof(COSA_DML_USER ));
            return ANSC_STATUS_SUCCESS;
        }
    }

    return ANSC_STATUS_FAILURE;

}

  
#elif ( defined _COSA_DRG_TPG_ )


ANSC_STATUS
CosaDmlUserInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    return ANSC_STATUS_SUCCESS;
}

ULONG
CosaDmlUserGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;

    return 0;

}

ANSC_STATUS
CosaDmlUserGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_USER              pEntry
    )
{
    return ANSC_STATUS_SUCCESS;

}

ANSC_STATUS
CosaDmlUserSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlUserAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_USER              pEntry
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlUserDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlUserSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_USER              pEntry      /* Identified by InstanceNumber */
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlUserGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_USER              pEntry      /* Identified by InstanceNumber */
    )
{
    return ANSC_STATUS_SUCCESS;

}
  
  
  
  
  
  
  
  
  
#elif (defined _COSA_INTEL_USG_ARM_) || (defined  _COSA_BCM_MIPS_)
#undef _COSA_SIM_

#include <utctx/utctx_api.h>
#include <ulog/ulog.h>
#include <utapi/utapi_tr_user.h>

COSA_DML_USER  g_users_user[] = 
{
    {
        1,
        TRUE,
        TRUE,
        "mso",
        "pod",
        ""
	"0"
    },
    {
        2,
        TRUE,
        TRUE,
        "cusadmin",
        "Xfinity",
        ""
	"0"
    },
    {
	3,
	TRUE,
        TRUE,
        "admin",
        "password",
        ""
	"0"
    }

};

ANSC_STATUS
CosaDmlUserInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
	/*
    unsigned short index = 0;
    if(0 != CosaDmlUserGetNumberOfEntries(hDml))
        return ANSC_STATUS_SUCCESS;
    for(index=0; index<3; ++index)
    {
	CosaDmlUserAddEntry(hDml, &g_users_user[index]);
    }
	*/
    return ANSC_STATUS_SUCCESS;
}

ULONG
CosaDmlUserGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    int count = 0;
    UtopiaContext ctx;

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;
    count = Utopia_GetNumOfUsers(&ctx);

    /* Free Utopia Context */
    Utopia_Free(&ctx,0);

    return count;

}

ANSC_STATUS
CosaDmlUserGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_USER              pEntry
    )
{
    int rc = -1;
    UtopiaContext ctx;

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    rc = Utopia_GetUserEntry(&ctx,ulIndex,pEntry);
    
    /* Free Utopia Context */
    Utopia_Free(&ctx,0);

    if (rc != 0)
       return ANSC_STATUS_FAILURE;
    else
       return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlUserSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber
    )
{
    int rc = -1;
    UtopiaContext ctx;

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    rc = Utopia_SetUserValues(&ctx,ulIndex,ulInstanceNumber);

    /* Free Utopia Context */
    Utopia_Free(&ctx,!rc);

    if (rc != 0)
       return ANSC_STATUS_FAILURE;
    else
       return ANSC_STATUS_SUCCESS;

}

ANSC_STATUS
CosaDmlUserAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_USER              pEntry
    )
{
    int rc = -1;
    UtopiaContext ctx;

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    rc = Utopia_AddUser(&ctx,pEntry);

    /* Free Utopia Context */
    Utopia_Free(&ctx,!rc);

    if (rc != 0)
       return ANSC_STATUS_FAILURE;
    else
       return ANSC_STATUS_SUCCESS;

}

ANSC_STATUS
CosaDmlUserDelEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulInstanceNumber
    )
{
    int rc = -1;
    UtopiaContext ctx;

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;
    
    rc = Utopia_DelUser(&ctx,ulInstanceNumber);

    /* Free Utopia Context */
    Utopia_Free(&ctx,!rc);

    if (rc != 0)
       return ANSC_STATUS_FAILURE;
    else
       return ANSC_STATUS_SUCCESS;

}


ANSC_STATUS
CosaDmlUserSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_USER              pEntry      /* Identified by InstanceNumber */
    )
{
    int rc = -1;

    UtopiaContext ctx;
#if 0
    /*if (pEntry && (!strcmp(pEntry->Username, "mso"))){
        int len = -1;
        if (pEntry->Password){
            len = strlen(pEntry->Password);
            if ((0 <= len) && (len <= 40)){
		// need to get a fix from Cisco
                SaRemoteAccessDb_RetrieveAccess();
                SaRemoteAccessDb_SetWASuperpassLen(len);
                SaRemoteAccessDb_SetWASuperpass(pEntry->Password);
                SaRemoteAccessDb_SetWASuperpassSource(2);   /* WEBUI */
                SaRemoteAccessDb_SaveToNvram();
                system("sysevent set potd-restart");
            }
        }
        return ANSC_STATUS_SUCCESS;
    }
#endif
    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;
    
    rc = Utopia_SetUserCfg(&ctx,pEntry);
        
    /* Free Utopia Context */
    Utopia_Free(&ctx,!rc);

 

    if (rc != 0)
       return ANSC_STATUS_FAILURE;
    else
       return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlUserGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_USER              pEntry      /* Identified by InstanceNumber */
    )
{
    int rc = -1;
    UtopiaContext ctx;

    if(!Utopia_Init(&ctx))
        return ANSC_STATUS_FAILURE;

    rc = Utopia_GetUserCfg(&ctx,pEntry);

    /* Free Utopia Context */
    Utopia_Free(&ctx,0);

    if (rc != 0)
       return ANSC_STATUS_FAILURE;
    else
       return ANSC_STATUS_SUCCESS;
}
#endif

ANSC_STATUS
        hash_userPassword
	(
	        PCHAR              pString,
        	char*              hashedpassword      /* Identified by InstanceNumber */
	)
{
        CcspTraceWarning(("%s, Entered to hash password\n",__FUNCTION__));
	ULONG SerialNumberLength = 0;
	char password[128] = {'\0'};
	if (SerialNumber[0] =='\0' )
	{
          CosaDmlDiGetSerialNumber(NULL,SerialNumber,&SerialNumberLength);
	}
        ANSC_CRYPTO_KEY	key	= {0};
        ANSC_CRYPTO_HASH hash	= {0};
        ULONG	hashLength	= 0;
        char *tmp = NULL, *convertTo = NULL;
        char cmp[128] = {'\0'};
        char saltText[128] = {'\0'}, hashedmd[128] = {'\0'};
        int  iIndex = 0, Key_len = 0, salt_len = 0, hashedmd_len = 0;
        HMAC_CTX ctx;
			
        _ansc_sprintf(saltText, "%s", SerialNumber);

        Key_len = strlen(pString);
	
        salt_len = strlen(saltText);
        HMAC_CTX_init( &ctx);
        HMAC_Init(	 &ctx, pString,  Key_len, EVP_sha256());
        HMAC_Update( &ctx, (unsigned char *)saltText, salt_len);
        HMAC_Final(  &ctx, (unsigned char *)hashedmd, (unsigned int *)&hashedmd_len );
        convertTo = cmp;
        for (iIndex = 0; iIndex < hashedmd_len; iIndex++) 
        {
          sprintf(convertTo,"%02x", hashedmd[iIndex] & 0xff);
          convertTo += 2;
        }
        HMAC_CTX_cleanup( &ctx );
        AnscCopyString(hashedpassword,cmp);
        CcspTraceWarning(("%s, Returning success\n",__FUNCTION__));	
        return ANSC_STATUS_SUCCESS ;

} 

ANSC_STATUS
user_validatepwd
	(
            ANSC_HANDLE                 hContext,
            PCHAR                       pString,
            PCOSA_DML_USER              pEntry,
            char*                       hashpassword
	
	)
{
   CcspTraceWarning(("%s, Entered to validate password\n",__FUNCTION__));
   char fromDB[128]={'\0'};
   char val[32] = {'\0'};
   char getHash[128]= {'\0'};
   int isDefault=0;
   if(!strcmp(pEntry->Username,"admin"))
   {

   syscfg_get( NULL, "hash_password_3",fromDB, sizeof(fromDB));

   if(fromDB[0] == '\0')
   {
     user_hashandsavepwd(hContext,pEntry->Password,pEntry);
   }
   if (!strcmp("password",pString))
   { 
     isDefault=1;
   }
   hash_userPassword(pString,getHash); 
   CcspTraceWarning(("%s, Compare passwords\n",__FUNCTION__));
   
   if (strcmp(getHash, pEntry->HashedPassword) == 0)
   {
     if(isDefault == 1)
     {
        strcpy(val,"Default_PWD");
     }
     else
     {
        strcpy(val,"Good_PWD");
     }
   }
   else
   {
     strcpy(val,"Invalid_PWD");

   }
   AnscCopyString(hashpassword,val);
   }
#if defined(_COSA_FOR_BCI_)
   if(!strcmp(pEntry->Username,"cusadmin"))
   {

   syscfg_get( NULL, "hash_password_2",fromDB, sizeof(fromDB));

   if(fromDB[0] == '\0')
   {
     user_hashandsavepwd(hContext,pEntry->Password,pEntry);
   }
   if (!strcmp("highspeed",pString))
   {
     isDefault=1;
   }
   hash_userPassword(pString,getHash);
   CcspTraceWarning(("%s, Compare passwords\n",__FUNCTION__));

   if (strcmp(getHash, pEntry->HashedPassword) == 0)
   {
     if(isDefault == 1)
     {
        strcpy(val,"Default_PWD");
     }
     else
     {
        strcpy(val,"Good_PWD");
     }
   }
   else
   {
     strcpy(val,"Invalid_PWD");

   }
   AnscCopyString(hashpassword,val);
   }
#endif

   CcspTraceWarning(("%s, Comparison result: %s\n",__FUNCTION__,hashpassword));
   return ANSC_STATUS_SUCCESS ;
} 

ANSC_STATUS
user_hashandsavepwd
        (
            ANSC_HANDLE                 hContext,
            PCHAR                       pString,
            PCOSA_DML_USER              pEntry

        )
{
  char setHash[128]= {'\0'};
  CcspTraceWarning(("%s, Hash Password using the passed string\n",__FUNCTION__));

  hash_userPassword(pString,setHash);
  if(!strcmp(pEntry->Username,"admin"))
  {
  if(setHash[0] != '\0')
  {
     CcspTraceWarning(("%s, Set hash value to syscfg\n",__FUNCTION__));
     if(syscfg_set(NULL,"hash_password_3",setHash) != 0)
     {
        AnscTraceWarning(("syscfg_set failed\n"));
     } 
     else
     {
        if (syscfg_commit() != 0) 
        {
           AnscTraceWarning(("syscfg_commit failed\n"));
        }
        else
        {
          AnscCopyString(pEntry->HashedPassword,setHash);
          CcspTraceWarning(("%s, Hash value is saved to syscfg\n",__FUNCTION__));
          return ANSC_STATUS_SUCCESS;
        }
     }
  }
  }
#if defined(_COSA_FOR_BCI_)
  if(!strcmp(pEntry->Username,"cusadmin"))
  {
  if(setHash[0] != '\0')
  {
     CcspTraceWarning(("%s, Set hash value to syscfg\n",__FUNCTION__));
     if(syscfg_set(NULL,"hash_password_2",setHash) != 0)
     {
        AnscTraceWarning(("syscfg_set failed\n"));
     }
     else
     {
        if (syscfg_commit() != 0)
        {
           AnscTraceWarning(("syscfg_commit failed\n"));
        }
        else
        {
          AnscCopyString(pEntry->HashedPassword,setHash);
          CcspTraceWarning(("%s, Hash value is saved to syscfg\n",__FUNCTION__));
          return ANSC_STATUS_SUCCESS;
        }
     }
  }
  }
#endif
  CcspTraceWarning(("%s, Returning failure\n",__FUNCTION__));
  return ANSC_STATUS_FAILURE;

}

ANSC_STATUS
CosaDmlUserResetPassword
      (
          BOOL                        bValue,
          PCOSA_DML_USER              pEntry
      )
{
   CcspTraceWarning(("%s, Entered Reset function\n",__FUNCTION__));
   char* defPassword = NULL;
   if(!strcmp(pEntry->Username,"admin"))
   {
     defPassword = "password";   
   } 
#if defined(_COSA_FOR_BCI_)
   else if(!strcmp(pEntry->Username,"cusadmin"))
   {
     defPassword = "highspeed";
   }
#endif
   else
   {
     return ANSC_STATUS_FAILURE;
   }
   if(!strcmp(pEntry->Username,"admin"))
   {
   user_hashandsavepwd(NULL,defPassword,pEntry);
   CcspTraceWarning(("%s, Set default password to syscfg\n",__FUNCTION__));
   if(syscfg_set(NULL, "user_password_3", defPassword) != 0)
   {
      AnscTraceWarning(("syscfg_set failed\n"));
   }
   else
   {
     if(syscfg_commit() != 0)
     {
       AnscTraceWarning(("syscfg_commit failed\n"));
     }
     else
     {
       AnscCopyString(pEntry->Password, defPassword);
       CcspTraceWarning(("%s, Returning Success\n",__FUNCTION__));
       return ANSC_STATUS_SUCCESS;
     }
   }
   }
#if defined(_COSA_FOR_BCI_)
   if(!strcmp(pEntry->Username,"cusadmin"))
   {
   user_hashandsavepwd(NULL,defPassword,pEntry);
   CcspTraceWarning(("%s, Set default password to syscfg\n",__FUNCTION__));
   if(syscfg_set(NULL, "user_password_2", defPassword) != 0)
   {
      AnscTraceWarning(("syscfg_set failed\n"));
   }
   else
   {
     if(syscfg_commit() != 0)
     {
       AnscTraceWarning(("syscfg_commit failed\n"));
     }
     else
     {
       AnscCopyString(pEntry->Password, defPassword);
       CcspTraceWarning(("%s, Returning Success\n",__FUNCTION__));
       return ANSC_STATUS_SUCCESS;
     }
   }
   }
#endif
   CcspTraceWarning(("%s, Returning Failure\n",__FUNCTION__));
   return ANSC_STATUS_FAILURE;
} 

