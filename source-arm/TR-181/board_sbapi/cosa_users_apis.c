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
#include "cosa_deviceinfo_apis.h"
#include "secure_wrapper.h"
#include <openssl/hmac.h>
#include <syscfg/syscfg.h>
#include "safec_lib_common.h"

#define SIZE_OF_HASHPASSWORD  32
/* Changing SNO as 256 bytes from 64 bytes due to HAL layer access more than 64 byets*/
char SerialNumber[256] = {'\0'};
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

  
#elif (defined _COSA_INTEL_USG_ARM_) || (defined  _COSA_BCM_MIPS_)
#undef _COSA_SIM_

#include <utctx/utctx_api.h>
#include <ulog/ulog.h>
#include <utapi/utapi_tr_user.h>
/*
COSA_DML_USER  g_users_user[] = 
{
    {
        1,
        TRUE,
        TRUE,
        "mso",
        "pod",
        "",
	"0"
    },
    {
        2,
        TRUE,
        TRUE,
        "cusadmin",
        "Xfinity",
        "",
	"0"
    },
    {
	3,
	TRUE,
        TRUE,
        "admin",
        "password",
        "",
	"0"
    }

};
*/
ANSC_STATUS
CosaDmlUserInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    UNREFERENCED_PARAMETER(hDml);
    UNREFERENCED_PARAMETER(phContext);
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
    int rc = -1;

    UtopiaContext ctx;
#if 0
    if (pEntry && (!strcmp(pEntry->Username, "mso"))){
        int len = -1;
        if (pEntry->Password){
            len = strlen(pEntry->Password);
            if ((0 <= len) && (len <= 40)){
		// need to get a fix from Cisco
                SaRemoteAccessDb_RetrieveAccess();
                SaRemoteAccessDb_SetWASuperpassLen(len);
                SaRemoteAccessDb_SetWASuperpass(pEntry->Password);
                SaRemoteAccessDb_SetWASuperpassSource(2);
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
    UNREFERENCED_PARAMETER(hContext);
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
	if (SerialNumber[0] =='\0' )
	{
          /* CID: 79484 Out-of-bounds access - updated global decl*/
          CosaDmlDiGetSerialNumber(NULL,SerialNumber,&SerialNumberLength);
	}
        char *convertTo = NULL;
        char saltText[128] = {'\0'}, hashedmd[128] = {'\0'};
        int  iIndex = 0, Key_len = 0, salt_len = 0, hashedmd_len = 0;
        errno_t safec_rc = -1;
        int hashedpassword_size = 128;
			
#if (OPENSSL_VERSION_NUMBER < 0x10100000L)
        HMAC_CTX ctx;
#else
        HMAC_CTX *pctx = HMAC_CTX_new();
#endif
        safec_rc = strcpy_s(saltText, sizeof(saltText), SerialNumber);
        if(safec_rc != EOK)
        {
           ERR_CHK(safec_rc);
        }

        Key_len = strlen(pString);
	
        salt_len = strlen(saltText);
#if (OPENSSL_VERSION_NUMBER < 0x10100000L)
        HMAC_CTX_init( &ctx);
        HMAC_Init(	 &ctx, pString,  Key_len, EVP_sha256());
        HMAC_Update( &ctx, (unsigned char *)saltText, salt_len);
        HMAC_Final(  &ctx, (unsigned char *)hashedmd, (unsigned int *)&hashedmd_len );
#else
        HMAC_CTX_reset (pctx);
        HMAC_Init (pctx, pString, Key_len, EVP_sha256());
        HMAC_Update (pctx, (unsigned char *) saltText, salt_len);
        HMAC_Final (pctx, (unsigned char *) hashedmd, (unsigned int *) &hashedmd_len);
#endif
        convertTo = hashedpassword;
        for (iIndex = 0; iIndex < hashedmd_len; iIndex++) 
        {
          // Here hashedpassword size is 128 from calling function
          safec_rc = sprintf_s(convertTo, hashedpassword_size,"%02x", hashedmd[iIndex] & 0xff);
          if(safec_rc < EOK)
          {
            ERR_CHK(safec_rc);
          }
          convertTo += 2;
          hashedpassword_size -= 2;
        }
#if (OPENSSL_VERSION_NUMBER < 0x10100000L)
        HMAC_CTX_cleanup( &ctx );
#else
        HMAC_CTX_free (pctx);
#endif
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
   char getHash[128]= {'\0'};
   int isDefault=0;
   errno_t safec_rc = -1;
   char *v;
   if(!strcmp(pEntry->Username,"admin"))
   {

   syscfg_get( NULL, "hash_password_3",fromDB, sizeof(fromDB));

   if(fromDB[0] == '\0')
   {
     #if defined(_HUB4_PRODUCT_REQ_) || defined(_PLATFORM_RASPBERRYPI_) || defined(_PLATFORM_TURRIS_)
         user_hashandsavepwd(hContext,pEntry->Password,pEntry);
     #else
         FILE *ptr;
         char buff[10];
         if ((ptr=v_secure_popen("r", "/usr/bin/configparamgen jx lkiprgpkmqfk:3"))!=NULL)
         if (NULL == ptr) {
             return ANSC_STATUS_FAILURE;
         }
         if (NULL == fgets(buff, 9, ptr)) {
             v_secure_pclose(ptr);
             return ANSC_STATUS_FAILURE;
         }
         v_secure_pclose(ptr);
         user_hashandsavepwd(hContext,buff,pEntry);
     #endif
   }
#if !defined(_HUB4_PRODUCT_REQ_)
   //TODO: Avoid the hardcoded password.
   if (!strcmp("password",pString))
   { 
     isDefault=1;
   }
#endif
   hash_userPassword(pString,getHash); 
   CcspTraceWarning(("%s, Compare passwords\n",__FUNCTION__));
   
    v = strcmp(getHash, pEntry->HashedPassword) ? "Invalid_PWD" : (isDefault == 1 ? "Default_PWD" : "Good_PWD");
    safec_rc = strcpy_s(hashpassword, SIZE_OF_HASHPASSWORD, v);
    if(safec_rc != EOK)
    {
       ERR_CHK(safec_rc);
    }
   }
#if defined(_COSA_FOR_BCI_)
   if(!strcmp(pEntry->Username,"cusadmin"))
   {

   syscfg_get( NULL, "hash_password_2",fromDB, sizeof(fromDB));

   if(fromDB[0] == '\0')
   {
         FILE *fptr;
         char outbuff[10];
         if ((fptr=v_secure_popen("r", "/usr/bin/configparamgen jx jtxpybrepjab:3"))!=NULL)
         if (NULL == fptr) {
             return ANSC_STATUS_FAILURE;
         }
         if (NULL == fgets(outbuff, 10, fptr)) {
             v_secure_pclose(fptr);
             return ANSC_STATUS_FAILURE;
         }
         v_secure_pclose(fptr);
         user_hashandsavepwd(hContext,outbuff,pEntry);
   }
   if (!strcmp("highspeed",pString))
   {
     isDefault=1;
   }
   hash_userPassword(pString,getHash);
   CcspTraceWarning(("%s, Compare passwords\n",__FUNCTION__));

    v = strcmp(getHash, pEntry->HashedPassword) ? "Invalid_PWD" : (isDefault == 1 ? "Default_PWD" : "Good_PWD");
    safec_rc = strcpy_s(hashpassword, SIZE_OF_HASHPASSWORD, v);
    if(safec_rc != EOK)
    {
       ERR_CHK(safec_rc);
    }
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
  UNREFERENCED_PARAMETER(hContext);
  char setHash[128]= {'\0'};
  errno_t rc = -1;
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
          rc = strcpy_s(pEntry->HashedPassword,sizeof(pEntry->HashedPassword),setHash);
          ERR_CHK(rc);
          CcspTraceWarning(("%s, Hash value is saved to syscfg\n",__FUNCTION__));
	  syscfg_unset(NULL, "user_password_3");
	  syscfg_commit();
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
          rc = strcpy_s(pEntry->HashedPassword,sizeof(pEntry->HashedPassword),setHash);
          ERR_CHK(rc);
          CcspTraceWarning(("%s, Hash value is saved to syscfg\n",__FUNCTION__));
          syscfg_unset(NULL, "user_password_2");
          syscfg_commit();
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
   UNREFERENCED_PARAMETER(bValue);
   CcspTraceWarning(("%s, Entered Reset function\n",__FUNCTION__));
   char defPassword[10];
   
   if(!strcmp(pEntry->Username,"admin"))
   {
     #if defined(_HUB4_PRODUCT_REQ_) || defined(_PLATFORM_RASPBERRYPI_) || defined(_PLATFORM_TURRIS_)
         //TODO: Avoid the hardcoded password.
         errno_t safec_rc = -1;
         safec_rc = strcpy_s(defPassword,sizeof(defPassword),"password");
         if(safec_rc != EOK)
         {
            ERR_CHK(safec_rc);
         }
     #else
         FILE *ptr;
         if ((ptr=v_secure_popen("r", "/usr/bin/configparamgen jx lkiprgpkmqfk:3"))!=NULL)
         if (NULL == ptr) {
             return ANSC_STATUS_FAILURE;
         }
         if (NULL == fgets(defPassword, 9, ptr)) {
             v_secure_pclose(ptr);
             return ANSC_STATUS_FAILURE;
         }
         v_secure_pclose(ptr);
     #endif
   } 
#if defined(_COSA_FOR_BCI_)
   else if(!strcmp(pEntry->Username,"cusadmin"))
   {
         FILE *fptr;
         if ((fptr=v_secure_popen("r", "/usr/bin/configparamgen jx jtxpybrepjab:3"))!=NULL)
         if (NULL == fptr) {
             return ANSC_STATUS_FAILURE;
         }
         if (NULL == fgets(defPassword, 10, fptr)) {
             v_secure_pclose(fptr);
             return ANSC_STATUS_FAILURE;
         }
         v_secure_pclose(fptr);
   }
#endif
   else
   {
     return ANSC_STATUS_FAILURE;
   }
   if(!strcmp(pEntry->Username,"admin"))
   {
     user_hashandsavepwd(NULL,defPassword,pEntry);
     
     //AnscCopyString(pEntry->Password, defPassword);
     return ANSC_STATUS_SUCCESS;
   }
#if defined(_COSA_FOR_BCI_)
   if(!strcmp(pEntry->Username,"cusadmin"))
   {
     user_hashandsavepwd(NULL,defPassword,pEntry);
     //AnscCopyString(pEntry->Password, defPassword);
     CcspTraceWarning(("%s, Returning Success\n",__FUNCTION__));
     return ANSC_STATUS_SUCCESS;
   }
#endif
   CcspTraceWarning(("%s, Returning Failure\n",__FUNCTION__));
   return ANSC_STATUS_FAILURE;
} 

