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

    module: cosa_dns_dml.c

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

        01/14/2011    initial revision.

**************************************************************************/

#include "ansc_platform.h"
/*#include "cosa_diagnostic_apis.h"*/
#include "cosa_dns_dml.h"
#include "cosa_dns_internal.h"
#include "safec_lib_common.h"

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

    DNS.



***********************************************************************/

/***********************************************************************

 APIs for Object:

   DNS.Client.

    *  Client1_GetParamBoolValue
    *  Client1_GetParamIntValue
    *  Client1_GetParamUlongValue
    *  Client1_GetParamStringValue
    *  Client1_SetParamBoolValue
    *  Client1_SetParamIntValue
    *  Client1_SetParamUlongValue
    *  Client1_SetParamStringValue
    *  Client1_Validate
    *  Client1_Commit
    *  Client1_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Client1_GetParamBoolValue
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
Client1_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    COSA_DML_DNS_STATUS             eClientStatus = COSA_DML_DNS_STATUS_Disabled;

    /* check the parameter name and return the corresponding value */
    if ( AnscEqualString(ParamName, "Enable", TRUE) )
    {
        /* collect value */
        eClientStatus = CosaDmlIpDnsGetClientStatus(NULL);
        if ( eClientStatus == COSA_DML_DNS_STATUS_Enabled )
        {
            *pBool = TRUE;
        }else
        {
            *pBool = FALSE;
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
        Client1_GetParamIntValue
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
Client1_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pInt);
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Client1_GetParamUlongValue
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
Client1_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and return the corresponding value */
    if ( AnscEqualString(ParamName, "Status", TRUE) )
    {
        /* collect value */
        *puLong = (ULONG)CosaDmlIpDnsGetClientStatus(NULL);
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Client1_GetParamStringValue
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
Client1_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pValue);
    UNREFERENCED_PARAMETER(pUlSize);
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Client1_SetParamBoolValue
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
Client1_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and set the corresponding value */
    if ( AnscEqualString(ParamName, "Enable", TRUE) )
    {
        /* save update to backup */
        CosaDmlDnsEnableClient(NULL, (BOOLEAN)bValue);
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Client1_SetParamIntValue
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
Client1_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(iValue);
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Client1_SetParamUlongValue
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
Client1_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(uValue);
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Client1_SetParamStringValue
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
Client1_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pString);
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Client1_Validate
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
Client1_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pReturnParamName);
    UNREFERENCED_PARAMETER(puLength);
    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Client1_Commit
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
Client1_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Client1_Rollback
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
Client1_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{   
    UNREFERENCED_PARAMETER(hInsContext);
    return 0;
}

/***********************************************************************

 APIs for Object:

    DNS.Client.Server.{i}.

    *  Server1_GetEntryCount
    *  Server1_GetEntry
    *  Server1_AddEntry
    *  Server1_DelEntry
    *  Server1_IsUpdated
    *  Server1_Synchronize
    *  Server1_GetParamBoolValue
    *  Server1_GetParamIntValue
    *  Server1_GetParamUlongValue
    *  Server1_GetParamStringValue
    *  Server1_SetParamBoolValue
    *  Server1_SetParamIntValue
    *  Server1_SetParamUlongValue
    *  Server1_SetParamStringValue
    *  Server1_Validate
    *  Server1_Commit
    *  Server1_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Server1_GetEntryCount
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
Server1_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DNS             pMyObject    = (PCOSA_DATAMODEL_DNS)g_pCosaBEManager->hDNS;     

    return AnscSListQueryDepth(&pMyObject->ServerList);
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Server1_GetEntry
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
Server1_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DNS             pMyObject    = (PCOSA_DATAMODEL_DNS)g_pCosaBEManager->hDNS;     
    PSLIST_HEADER                   pSrvHead     = (PSLIST_HEADER)&pMyObject->ServerList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pLink        = (PSINGLE_LINK_ENTRY       )NULL;

    pLink = AnscSListGetEntryByIndex(pSrvHead, nIndex);

    if ( pLink )
    {
        pCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        *pInsNumber  = pCosaContext->InstanceNumber;
    }
    
    return pCosaContext;
}



/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Server1_AddEntry
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
Server1_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DNS             pMyObject    = (PCOSA_DATAMODEL_DNS)g_pCosaBEManager->hDNS;     
    PSLIST_HEADER                   pSrvHead     = (PSLIST_HEADER)&pMyObject->ServerList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_DNS_CLIENT_SERVER     pDnsServer   = (PCOSA_DML_DNS_CLIENT_SERVER)NULL;
    errno_t                         rc           = -1;

    CcspTraceInfo(("Server1_AddEntry...\n"));

    pDnsServer = (PCOSA_DML_DNS_CLIENT_SERVER)AnscAllocateMemory(sizeof(COSA_DML_DNS_CLIENT_SERVER));

    if ( !pDnsServer )
    {
        return NULL;
    }

    rc = sprintf_s(pDnsServer->Alias, sizeof(pDnsServer->Alias),"Server%lu", pMyObject->ulNextServerInsNum);
    if(rc < EOK)
    {
      ERR_CHK(rc);
      AnscFreeMemory(pDnsServer);
      return NULL;
    }

    pDnsServer->Type = COSA_DML_DNS_ADDR_SRC_Static;
        
    if ( TRUE )
    {
        pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

        if ( !pCosaContext )
        {
            AnscFreeMemory(pDnsServer);
            
            return NULL;
        }

        pCosaContext->InstanceNumber = pDnsServer->InstanceNumber = pMyObject->ulNextServerInsNum;

        pMyObject->ulNextServerInsNum++;

        if ( 0 == pMyObject->ulNextServerInsNum )
        {
            pMyObject->ulNextServerInsNum = 1;
        }

        pCosaContext->hContext         = (ANSC_HANDLE)pDnsServer;
        pCosaContext->hParentTable     = NULL;
        pCosaContext->hPoamIrepUpperFo = pMyObject->hIrepFolderServerHA;
        pCosaContext->bNew             = TRUE;

        CosaSListPushEntryByInsNum(pSrvHead, pCosaContext);

        CosaDNSRegAddInfo
        (
            (ANSC_HANDLE)pMyObject,
            COSA_DML_RR_NAME_SERVER_NextInsNum,
            pMyObject->ulNextServerInsNum,
            COSA_DML_RR_NAME_SERVER_Prefix,
            0,
            pDnsServer->Alias,
            (ANSC_HANDLE)pCosaContext
        );
    }

    *pInsNumber = pCosaContext->InstanceNumber;

    return pCosaContext;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Server1_DelEntry
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
Server1_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DNS             pMyObject    = (PCOSA_DATAMODEL_DNS)g_pCosaBEManager->hDNS;     
    PSLIST_HEADER                   pSrvHead     = (PSLIST_HEADER)&pMyObject->ServerList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_DML_DNS_CLIENT_SERVER     pDnsServer   = (PCOSA_DML_DNS_CLIENT_SERVER)pCosaContext->hContext;

    CcspTraceInfo(("Server1_DelEntry...\n"));

    CosaDmlDnsClientDelServer(NULL, pDnsServer->InstanceNumber);

    AnscSListPopEntryByLink(pSrvHead, &pCosaContext->Linkage);

    if ( pCosaContext->bNew )
    {
        CosaDNSRegDelInfo(NULL, (ANSC_HANDLE)pCosaContext);
    }
    
    AnscFreeMemory(pDnsServer);
    AnscFreeMemory(pCosaContext);

    return 0;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Server1_IsUpdated
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/
BOOL
Server1_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DNS             pMyObject    = (PCOSA_DATAMODEL_DNS)g_pCosaBEManager->hDNS;     
    BOOL                            bIsUpdated   = TRUE;
    ULONG                           ulInterval   = 0;              

    ulInterval =  AnscGetTickInSeconds() - pMyObject->PreVisitSrvTime;

    if ( COSA_DML_DNS_ACCESS_INTERVAL > ulInterval )
    {
        bIsUpdated  = FALSE;
    }
    else
    {
        pMyObject->PreVisitSrvTime =  AnscGetTickInSeconds();
        bIsUpdated  = TRUE;
    }

    CcspTraceInfo(("Server1_IsUpdated -- %s...\n", bIsUpdated ? "Yes" : "No"));

    return bIsUpdated;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Server1_Synchronize
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
Server1_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    ANSC_STATUS                     returnStatus = ANSC_STATUS_FAILURE;
    PCOSA_DATAMODEL_DNS             pMyObject    = (PCOSA_DATAMODEL_DNS)g_pCosaBEManager->hDNS;     
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_DNS_CLIENT_SERVER     pDnsServer   = (PCOSA_DML_DNS_CLIENT_SERVER)NULL;

    PCOSA_DML_DNS_CLIENT_SERVER     pDnsServer2  = (PCOSA_DML_DNS_CLIENT_SERVER)NULL;
        
    PSINGLE_LINK_ENTRY              pLink        = (PSINGLE_LINK_ENTRY       )NULL;
    PSLIST_HEADER                   pSrvHead     = (PSLIST_HEADER)&pMyObject->ServerList;
    PULONG                          pFound       = (PULONG)NULL;
    ULONG                           ulEntryCount = 0;
    ULONG                           i            = 0;

    pDnsServer = CosaDmlDnsClientGetServers(NULL, (PULONG)&ulEntryCount);
    if ( !pDnsServer )
    {
        return ANSC_STATUS_FAILURE;
    }

    pFound = AnscAllocateMemory( ulEntryCount * sizeof(ULONG) );
    if ( !pFound )
    {
        goto EXIT3;
    }

    AnscZeroMemory(pFound, ulEntryCount * sizeof(ULONG));

    pLink = AnscSListGetFirstEntry(pSrvHead);

    while ( pLink )
    {
        pCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        pLink        = AnscSListGetNextEntry(pLink);

        pDnsServer2  = (PCOSA_DML_DNS_CLIENT_SERVER)pCosaContext->hContext;

        for ( i = 0; i < ulEntryCount; i++ )
        {
            if ( ( 0 == pFound[i] ) && ( pDnsServer2->InstanceNumber == pDnsServer[i].InstanceNumber ) )
            {
                pFound[i] = 1;
                /* If found, sync local content too */
                *pDnsServer2       = pDnsServer[i];
                pCosaContext->bNew = FALSE;
                break;
            }
       }

        /* if Not found, remove this old content from loca list */
        if ( (ulEntryCount == i) && (FALSE == pCosaContext->bNew) )
        {
            AnscSListPopEntryByLink(pSrvHead, &pCosaContext->Linkage);
        
            AnscFreeMemory(pDnsServer2);
            AnscFreeMemory(pCosaContext);
        }
    }

    /* Sync new entry into local list */
    for ( i = 0; i < ulEntryCount; i++ )
    {
        if ( 0 == pFound[i] )
        {
            pCosaContext = AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
            if ( !pCosaContext )
            {
                goto EXIT2;
            }
            
            pDnsServer2 = AnscAllocateMemory(sizeof(COSA_DML_DNS_CLIENT_SERVER));
            if ( !pDnsServer2 )
            {
                goto EXIT1;
            }
            
            /* copy new content which should include InstanceNumber and Alias */
            *pDnsServer2                 = pDnsServer[i];

            pCosaContext->bNew           = FALSE;
            pCosaContext->hContext       = (ANSC_HANDLE)pDnsServer2;
            pCosaContext->InstanceNumber = pDnsServer2->InstanceNumber;

            CcspTraceInfo
                ((
                    "Server1_Synchronize -- new entry %s with instance number %d.\n",
                    pDnsServer2->DNSServer,
                    pDnsServer2->InstanceNumber
                ));

            CosaSListPushEntryByInsNum(pSrvHead, pCosaContext);
        }
    }
        
    returnStatus =  ANSC_STATUS_SUCCESS;

    goto EXIT2;

    
EXIT1:
    AnscFreeMemory(pCosaContext);
    
EXIT2:
    AnscFreeMemory(pFound);
    
EXIT3:
    AnscFreeMemory(pDnsServer);

    return returnStatus;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Server1_GetParamBoolValue
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
Server1_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DNS_CLIENT_SERVER     pDnsServer   = (PCOSA_DML_DNS_CLIENT_SERVER)pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE) )
    {
        /* collect value */
        *pBool = pDnsServer->bEnabled;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Server1_GetParamIntValue
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
Server1_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pInt);
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Server1_GetParamUlongValue
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
Server1_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DNS_CLIENT_SERVER     pDnsServer   = (PCOSA_DML_DNS_CLIENT_SERVER)pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Status", TRUE) )
    {
        /* collect value */
        *puLong  = pDnsServer->Status;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Type", TRUE) )
    {
        /* collect value */
        *puLong  = pDnsServer->Type;
        return TRUE;
    }

    if (AnscEqualString(ParamName, "X_CISCO_COM_Order", TRUE))
    {
        *puLong  = pDnsServer->Order;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Server1_GetParamStringValue
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
Server1_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(pUlSize);
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DNS_CLIENT_SERVER     pDnsServer   = (PCOSA_DML_DNS_CLIENT_SERVER)pCosaContext->hContext;
    errno_t   rc = -1;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pDnsServer->Alias);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }
        return 0;
    }

    if( AnscEqualString(ParamName, "Interface", TRUE) )
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pDnsServer->Interface);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }
        return 0;
    }

    if( AnscEqualString(ParamName, "DNSServer", TRUE) )
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pDnsServer->DNSServer);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }
        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Server1_SetParamBoolValue
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
Server1_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DNS_CLIENT_SERVER     pDnsServer   = (PCOSA_DML_DNS_CLIENT_SERVER)pCosaContext->hContext;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE) )
    {
        /* save update to backup */
        pDnsServer->bEnabled = bValue;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Server1_SetParamIntValue
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
Server1_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(iValue);
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Server1_SetParamUlongValue
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
Server1_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DNS_CLIENT_SERVER     pDnsServer   = (PCOSA_DML_DNS_CLIENT_SERVER)pCosaContext->hContext;

    /* check the parameter name and set the corresponding value */
    if (AnscEqualString(ParamName, "X_CISCO_COM_Order", TRUE))
    {
        pDnsServer->Order = uValue;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Server1_SetParamStringValue
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
Server1_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DNS_CLIENT_SERVER     pDnsServer   = (PCOSA_DML_DNS_CLIENT_SERVER)pCosaContext->hContext;
    errno_t   rc = -1;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER(pDnsServer->Alias, sizeof(pDnsServer->Alias), pString);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return FALSE;
        }
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Interface", TRUE) )
    {
       /* Note: Interface is only writable when Type is Static; */
        if ( COSA_DML_DNS_ADDR_SRC_Static  == pDnsServer->Type )
        {
            rc = STRCPY_S_NOCLOBBER(pDnsServer->Interface, sizeof(pDnsServer->Interface), pString);
            if(rc != EOK)
            {
               ERR_CHK(rc);
               return FALSE;
            }
            return TRUE;
        }

         CcspTraceWarning(("Interface is only writable when Type is Static \n"));
         return FALSE;
    }

    if( AnscEqualString(ParamName, "DNSServer", TRUE) )
    {
        if(( COSA_DML_DNS_ADDR_SRC_Static  == pDnsServer->Type ) && pDnsServer->InstanceNumber <= 2)
        {
            rc = STRCPY_S_NOCLOBBER(pDnsServer->DNSServer, sizeof(pDnsServer->DNSServer),pString);
            if(rc != EOK)
            {
               ERR_CHK(rc);
               return FALSE;
            }
            return TRUE;
        }

        CcspTraceWarning(("Only two static DNS servers are supported.\n"));
        CcspTraceWarning(("DNSServer is only writable when Type is Static \n"));
        return FALSE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Server1_Validate
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
Server1_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pReturnParamName);
    UNREFERENCED_PARAMETER(puLength);
    /*
    PCOSA_DATAMODEL_DNS             pMyObject     = (PCOSA_DATAMODEL_DNS)g_pCosaBEManager->hDNS;     
    PSLIST_HEADER                   pSrvHead     = (PSLIST_HEADER)&pMyObject->ServerList;
    //PSINGLE_LINK_ENTRY              pLink         = (PSINGLE_LINK_ENTRY)NULL;

    pLink = AnscSListGetFirstEntry(pSrvHead);

	
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DNS_CLIENT_SERVER     pDnsServer    = (PCOSA_DML_DNS_CLIENT_SERVER)pCosaContext->hContext;
    PCOSA_DML_DNS_CLIENT_SERVER     pDnsServer2   = (PCOSA_DML_DNS_CLIENT_SERVER)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2 = (PCOSA_CONTEXT_LINK_OBJECT)NULL;

    while ( pLink )
    {
        pCosaContext2 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        pLink         = AnscSListGetNextEntry(pLink);

        pDnsServer2 = (PCOSA_DML_DNS_CLIENT_SERVER)pCosaContext2->hContext;

        if ( pDnsServer2 && 
            ((ULONG)pDnsServer2 != (ULONG)pDnsServer) &&
             AnscEqualString(pDnsServer2->Alias, pDnsServer->Alias, TRUE))
        {
            AnscCopyString(pReturnParamName, "Alias");
            *puLength = AnscSizeOfString("Alias");

            CcspTraceWarning(("Server1_Validate() failed.\n"));
            return FALSE;
        }
    }
	*/

    return TRUE;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Server1_Commit
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
Server1_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DNS_CLIENT_SERVER     pDnsServer   = (PCOSA_DML_DNS_CLIENT_SERVER)pCosaContext->hContext;

    if ( pCosaContext->bNew )
    {
        pCosaContext->bNew = FALSE;

        CosaDmlDnsClientAddServer(NULL, pDnsServer);

        CosaDNSRegDelInfo(NULL, (ANSC_HANDLE)pCosaContext);
    }
    else
    {
        CosaDmlDnsClientSetServer(NULL, pDnsServer);
    }

    return 0;

}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Server1_Rollback
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
Server1_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{  
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DNS_CLIENT_SERVER     pDnsServer   = (PCOSA_DML_DNS_CLIENT_SERVER)pCosaContext->hContext;

    CcspTraceWarning(("Server1_Rollback()----begin to rollback......\n"));

    CosaDmlDnsClientGetServer(NULL, pDnsServer);
    
    return 0; 
}


/***********************************************************************

 APIs for Object:

    DNS.Relay.

    *  Relay_GetParamBoolValue
    *  Relay_GetParamIntValue
    *  Relay_GetParamUlongValue
    *  Relay_GetParamStringValue
    *  Relay_SetParamBoolValue
    *  Relay_SetParamIntValue
    *  Relay_SetParamUlongValue
    *  Relay_SetParamStringValue
    *  Relay_Validate
    *  Relay_Commit
    *  Relay_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Relay_GetParamBoolValue
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
Relay_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    COSA_DML_DNS_STATUS             eRelayStatus     = COSA_DML_DNS_STATUS_Disabled;
    
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* collect value */
        eRelayStatus = CosaDmlIpDnsGetRelayStatus(NULL);
        if ( eRelayStatus == COSA_DML_DNS_STATUS_Enabled )
        {
            *pBool = TRUE;
        }else
        {
            *pBool = FALSE;
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
        Relay_GetParamIntValue
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
Relay_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pInt);
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Relay_GetParamUlongValue
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
Relay_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Status", TRUE))
    {
        /* collect value */
        *puLong = (ULONG)CosaDmlIpDnsGetRelayStatus(NULL);
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Relay_GetParamStringValue
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
Relay_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pValue);
    UNREFERENCED_PARAMETER(pUlSize);
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Relay_SetParamBoolValue
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
Relay_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* save update to backup */
        CosaDmlDnsEnableRelay(NULL, (BOOLEAN)bValue);
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Relay_SetParamIntValue
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
Relay_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(iValue);
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Relay_SetParamUlongValue
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
Relay_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(uValue);
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Relay_SetParamStringValue
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
Relay_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pString);
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Relay_Validate
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
Relay_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pReturnParamName);
    UNREFERENCED_PARAMETER(puLength);
    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Relay_Commit
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
Relay_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    return 0;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Relay_Rollback
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
Relay_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    return 0;
}


/***********************************************************************

 APIs for Object:

    DNS.Relay.Forwarding.{i}.

    *  Forwarding_GetEntryCount
    *  Forwarding_GetEntry
    *  Forwarding_AddEntry
    *  Forwarding_DelEntry
    *  Forwarding_IsUpdated
    *  Forwarding_Synchrize
    *  Forwarding_GetParamBoolValue
    *  Forwarding_GetParamIntValue
    *  Forwarding_GetParamUlongValue
    *  Forwarding_GetParamStringValue
    *  Forwarding_SetParamBoolValue
    *  Forwarding_SetParamIntValue
    *  Forwarding_SetParamUlongValue
    *  Forwarding_SetParamStringValue
    *  Forwarding_Validate
    *  Forwarding_Commit
    *  Forwarding_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Forwarding_GetEntryCount
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
Forwarding_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DNS             pMyObject    = (PCOSA_DATAMODEL_DNS)g_pCosaBEManager->hDNS;     

    return AnscSListQueryDepth(&pMyObject->ForwardList);
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Forwarding_GetEntry
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
Forwarding_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DNS             pMyObject    = (PCOSA_DATAMODEL_DNS)g_pCosaBEManager->hDNS;     
    PSLIST_HEADER                   pForwardHead = (PSLIST_HEADER)&pMyObject->ForwardList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pLink        = (PSINGLE_LINK_ENTRY       )NULL;

    pLink = AnscSListGetEntryByIndex(pForwardHead, nIndex);

    if ( pLink )
    {
        pCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        *pInsNumber  = pCosaContext->InstanceNumber;
    }
    
    return pCosaContext;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Forwarding_AddEntry
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
Forwarding_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DNS             pMyObject    = (PCOSA_DATAMODEL_DNS)g_pCosaBEManager->hDNS;     
    PSLIST_HEADER                   pForwardHead = (PSLIST_HEADER)&pMyObject->ForwardList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_DNS_RELAY_ENTRY       pForward     = (PCOSA_DML_DNS_RELAY_ENTRY)NULL;
    errno_t                         rc           = -1;

    CcspTraceInfo(("Forwarding_AddEntry...\n"));

    pForward = (PCOSA_DML_DNS_RELAY_ENTRY)AnscAllocateMemory(sizeof(COSA_DML_DNS_RELAY_ENTRY));

    if ( !pForward )
    {
        return NULL;
    }

    rc = sprintf_s(pForward->Alias, sizeof(pForward->Alias),"Forwarding%lu", pMyObject->ulNextForwardInsNum);
    if(rc < EOK)
    {
      ERR_CHK(rc);
      AnscFreeMemory(pForward);
      return NULL;
    }

    pForward->Type = COSA_DML_DNS_ADDR_SRC_Static;

    if ( TRUE )
    {
        pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

        if ( !pCosaContext )
        {
            AnscFreeMemory(pForward);
            
            return NULL;
        }

        pCosaContext->InstanceNumber = pForward->InstanceNumber = pMyObject->ulNextForwardInsNum;

        pMyObject->ulNextForwardInsNum++;

        if ( 0 == pMyObject->ulNextForwardInsNum )
        {
            pMyObject->ulNextForwardInsNum = 1;
        }

        pCosaContext->hContext         = (ANSC_HANDLE)pForward;
        pCosaContext->hParentTable     = NULL;
        pCosaContext->hPoamIrepUpperFo = pMyObject->hIrepFolderServerHA;
        pCosaContext->bNew             = TRUE;

        CosaSListPushEntryByInsNum(pForwardHead, pCosaContext);

        CosaDNSRegAddInfo
        (
            (ANSC_HANDLE)pMyObject,
            COSA_DML_RR_NAME_FORWARD_NextInsNum,
            pMyObject->ulNextForwardInsNum,
            COSA_DML_RR_NAME_FORWARD_Prefix,
            0,
            pForward->Alias,
            (ANSC_HANDLE)pCosaContext
        );
    }

    *pInsNumber = pCosaContext->InstanceNumber;

    return pCosaContext;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Forwarding_DelEntry
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
Forwarding_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DNS             pMyObject    = (PCOSA_DATAMODEL_DNS)g_pCosaBEManager->hDNS;     
    PSLIST_HEADER                   pForwardHead = (PSLIST_HEADER)&pMyObject->ForwardList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_DML_DNS_RELAY_ENTRY       pForward     = (PCOSA_DML_DNS_RELAY_ENTRY)pCosaContext->hContext;

    CcspTraceInfo(("Forwarding_DelEntry...\n"));

    CosaDmlDnsRelayDelServer(NULL, pForward->InstanceNumber);

    AnscSListPopEntryByLink(pForwardHead, &pCosaContext->Linkage);

    if ( pCosaContext->bNew )
    {
        CosaDNSRegDelInfo(NULL, (ANSC_HANDLE)pCosaContext);
    }
    
    AnscFreeMemory(pForward);
    AnscFreeMemory(pCosaContext);

    return 0;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Forwarding_IsUpdated
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/
BOOL
Forwarding_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DNS             pMyObject    = (PCOSA_DATAMODEL_DNS)g_pCosaBEManager->hDNS;     
    BOOL                            bIsUpdated   = TRUE;
    ULONG                           ulInterval    = 0;              

    ulInterval =  AnscGetTickInSeconds() - pMyObject->PreVisitForwardTime;

    if ( COSA_DML_DNS_ACCESS_INTERVAL > ulInterval )
    {
        bIsUpdated  = FALSE;
    }
    else
    {
        pMyObject->PreVisitForwardTime =  AnscGetTickInSeconds();
        bIsUpdated  = TRUE;
    }

    CcspTraceInfo(("Forwarding_IsUpdated -- %s...\n", bIsUpdated ? "Yes" : "No"));

    return bIsUpdated;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Forwarding_Synchronize
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
Forwarding_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    ANSC_STATUS                     returnStatus = ANSC_STATUS_FAILURE;
    PCOSA_DATAMODEL_DNS             pMyObject    = (PCOSA_DATAMODEL_DNS)g_pCosaBEManager->hDNS;     
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_DNS_RELAY_ENTRY       pForward     = (PCOSA_DML_DNS_RELAY_ENTRY)NULL;

    PCOSA_DML_DNS_RELAY_ENTRY       pForward2    = (PCOSA_DML_DNS_RELAY_ENTRY)NULL;
        
    PSINGLE_LINK_ENTRY              pLink        = (PSINGLE_LINK_ENTRY       )NULL;
    PSLIST_HEADER                   pForwardHead = (PSLIST_HEADER)&pMyObject->ForwardList;
    PULONG                          pFound       = (PULONG)NULL;
    ULONG                           ulEntryCount = 0;
    ULONG                           i            = 0;

    pForward = CosaDmlDnsRelayGetServers(NULL, (PULONG)&ulEntryCount);
    if ( !pForward )
    {
        return ANSC_STATUS_FAILURE;
    }

    pFound = AnscAllocateMemory( ulEntryCount * sizeof(ULONG) );
    if ( !pFound )
    {
        goto EXIT3;
    }

    AnscZeroMemory(pFound, ulEntryCount * sizeof(ULONG));

    pLink = AnscSListGetFirstEntry(pForwardHead);

    while ( pLink )
    {
        pCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        pLink        = AnscSListGetNextEntry(pLink);

        pForward2 = (PCOSA_DML_DNS_RELAY_ENTRY)pCosaContext->hContext;

        for ( i = 0; i < ulEntryCount; i++ )
        {
            if ( ( 0 == pFound[i] ) && ( pForward2->InstanceNumber == pForward[i].InstanceNumber ) )
            {
                pFound[i] = 1;

                /* If found, sync local content too */
                *pForward2         = pForward[i];
                pCosaContext->bNew = FALSE;

                break;
            }
        }

        /* if Not found, remove this old content from loca list */
        if ( (ulEntryCount == i) && (FALSE == pCosaContext->bNew) )
        {
            AnscSListPopEntryByLink(pForwardHead, &pCosaContext->Linkage);
        
            AnscFreeMemory(pForward2);
            AnscFreeMemory(pCosaContext);
        }
    }

    /* Sync new entry into local list */
    for ( i = 0; i < ulEntryCount; i++ )
    {
        if ( 0 == pFound[i] )
        {
            pCosaContext = AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
            if ( !pCosaContext )
            {
                goto EXIT2;
            }
            
            pForward2 = AnscAllocateMemory(sizeof(COSA_DML_DNS_RELAY_ENTRY));
            if ( !pForward2 )
            {
                goto EXIT1;
            }
            
            /* copy new content which should include InstanceNumber and Alias */
            *pForward2                   = pForward[i];

            pCosaContext->bNew           = FALSE;
            pCosaContext->hContext       = (ANSC_HANDLE)pForward2;
            pCosaContext->InstanceNumber = pForward2->InstanceNumber;

            CcspTraceInfo
                ((
                    "Forwarding_Synchronize -- new entry %d.%d.%d.%d with instance number %d.\n",
                    pForward2->DNSServer.Dot[0], pForward2->DNSServer.Dot[1],
                    pForward2->DNSServer.Dot[2], pForward2->DNSServer.Dot[3],
                    pForward2->InstanceNumber
                ));

            CosaSListPushEntryByInsNum(pForwardHead, pCosaContext);
        }
    }
        
    returnStatus =  ANSC_STATUS_SUCCESS;

    goto EXIT2;

    
EXIT1:
    AnscFreeMemory(pCosaContext);
    
EXIT2:
    AnscFreeMemory(pFound);
    
EXIT3:
    AnscFreeMemory(pForward);

    return returnStatus;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Forwarding_GetParamBoolValue
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
Forwarding_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DNS_RELAY_ENTRY       pForward     = (PCOSA_DML_DNS_RELAY_ENTRY)pCosaContext->hContext;
    
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        *pBool = pForward->bEnabled;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Forwarding_GetParamIntValue
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
Forwarding_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pInt);
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Forwarding_GetParamUlongValue
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
Forwarding_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DNS_RELAY_ENTRY       pForward     = (PCOSA_DML_DNS_RELAY_ENTRY)pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Status", TRUE))
    {
        *puLong = pForward->Status;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "DNSServer", TRUE))
    {
        *puLong = pForward->DNSServer.Value;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Type", TRUE))
    {
        *puLong = pForward->Type;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Forwarding_GetParamStringValue
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
Forwarding_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DNS_RELAY_ENTRY       pForward     = (PCOSA_DML_DNS_RELAY_ENTRY)pCosaContext->hContext;
    errno_t   rc = -1;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE))
    {
        rc = strcpy_s(pValue, *pUlSize, pForward->Alias);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }
        *pUlSize = AnscSizeOfString(pValue)+1;
        return 0;
    }

    if( AnscEqualString(ParamName, "Interface", TRUE))
    {
        rc = strcpy_s(pValue, *pUlSize, pForward->Interface);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }
        *pUlSize = AnscSizeOfString(pValue)+1;
        return 0;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Forwarding_SetParamBoolValue
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
Forwarding_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DNS_RELAY_ENTRY       pForward     = (PCOSA_DML_DNS_RELAY_ENTRY)pCosaContext->hContext;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        pForward->bEnabled = bValue;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Forwarding_SetParamIntValue
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
Forwarding_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(iValue);
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Forwarding_SetParamUlongValue
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
Forwarding_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DNS_RELAY_ENTRY       pForward     = (PCOSA_DML_DNS_RELAY_ENTRY)pCosaContext->hContext;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "DNSServer", TRUE))
    {
        if ( COSA_DML_DNS_ADDR_SRC_Static  == pForward->Type )
        {
            pForward->DNSServer.Value = uValue;
            return TRUE;
        }

         CcspTraceWarning(("DNSServer is only writable when Type is Static \n"));
         return FALSE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Forwarding_SetParamStringValue
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
Forwarding_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DNS_RELAY_ENTRY       pForward     = (PCOSA_DML_DNS_RELAY_ENTRY)pCosaContext->hContext;
    errno_t   rc = -1;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE))
    {
        rc = STRCPY_S_NOCLOBBER(pForward->Alias, sizeof(pForward->Alias), pString);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Interface", TRUE))
    {
       /* Note: Interface is only writable when Type is Static; */
        if ( COSA_DML_DNS_ADDR_SRC_Static  == pForward->Type )
        {
            rc = STRCPY_S_NOCLOBBER(pForward->Interface, sizeof(pForward->Interface), pString);
            if(rc != EOK)
            {
               ERR_CHK(rc);
               return FALSE;
            }
            return TRUE;
        }

         CcspTraceWarning(("Interface is only writable when Type is Static \n"));
         return FALSE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Forwarding_Validate
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
Forwarding_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_DATAMODEL_DNS             pMyObject     = (PCOSA_DATAMODEL_DNS)g_pCosaBEManager->hDNS;     
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2 = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_DNS_RELAY_ENTRY       pForward      = (PCOSA_DML_DNS_RELAY_ENTRY)pCosaContext->hContext;
    PCOSA_DML_DNS_RELAY_ENTRY       pForward2     = (PCOSA_DML_DNS_RELAY_ENTRY)NULL;
    PSLIST_HEADER                   pForwardHead  = (PSLIST_HEADER)&pMyObject->ForwardList;
    PSINGLE_LINK_ENTRY              pLink         = (PSINGLE_LINK_ENTRY)NULL;
    errno_t  rc = -1;

    pLink = AnscSListGetFirstEntry(pForwardHead);

    while ( pLink )
    {
        pCosaContext2 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        pLink         = AnscSListGetNextEntry(pLink);

        pForward2 = (PCOSA_DML_DNS_RELAY_ENTRY)pCosaContext2->hContext;

        if ( pForward2 && 
            ((ULONG)pForward2 != (ULONG)pForward) &&
             AnscEqualString(pForward2->Alias, pForward->Alias, TRUE))
        {
            rc = strcpy_s(pReturnParamName, *puLength, "Alias");
            if(rc != EOK)
            {
               ERR_CHK(rc);
               return FALSE;
            }
            *puLength = AnscSizeOfString("Alias");

            CcspTraceWarning(("Forwarding_Validate() failed.\n"));
            return FALSE;
        }
    }
        
    return TRUE;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Forwarding_Commit
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
Forwarding_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DNS_RELAY_ENTRY       pForward     = (PCOSA_DML_DNS_RELAY_ENTRY)pCosaContext->hContext;

    if ( pCosaContext->bNew )
    {
        pCosaContext->bNew = FALSE;

        CosaDmlDnsRelayAddServer(NULL, pForward);

        CosaDNSRegDelInfo(NULL, (ANSC_HANDLE)pCosaContext);
    }
    else
    {
        CosaDmlDnsRelaySetServer(NULL, pForward);
    }

    return 0;

}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Forwarding_Rollback
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
Forwarding_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{  
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DNS_RELAY_ENTRY       pForward     = (PCOSA_DML_DNS_RELAY_ENTRY)pCosaContext->hContext;

    CcspTraceWarning(("Forwarding_Rollback()----begin to rollback......\n"));

    CosaDmlDnsRelayGetServer(NULL, pForward);
    
    return 0; 
}

#if 0
/***********************************************************************

 APIs for Object:

    DNS.Diagnostics.


***********************************************************************/
/***********************************************************************

 APIs for Object:

    DNS.Diagnostics.NSLookupDiagnostics.

    *  NSLookupDiagnostics_GetParamBoolValue
    *  NSLookupDiagnostics_GetParamIntValue
    *  NSLookupDiagnostics_GetParamUlongValue
    *  NSLookupDiagnostics_GetParamStringValue
    *  NSLookupDiagnostics_SetParamBoolValue
    *  NSLookupDiagnostics_SetParamIntValue
    *  NSLookupDiagnostics_SetParamUlongValue
    *  NSLookupDiagnostics_SetParamStringValue
    *  NSLookupDiagnostics_Validate
    *  NSLookupDiagnostics_Commit
    *  NSLookupDiagnostics_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        NSLookupDiagnostics_GetParamBoolValue
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
NSLookupDiagnostics_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
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
        NSLookupDiagnostics_GetParamIntValue
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
NSLookupDiagnostics_GetParamIntValue
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
        NSLookupDiagnostics_GetParamUlongValue
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
NSLookupDiagnostics_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_NSLOOKUP_INFO             pNSLookupDiagInfo   = pMyObject->hDiagNSLookInfo;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "DiagnosticsState", TRUE))
    {
        pNSLookupDiagInfo = (PDSLH_NSLOOKUP_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_NSLookup);

        if ( pNSLookupDiagInfo )
        {
            *puLong = pNSLookupDiagInfo->DiagnosticState + 1;
        }
        else
        {
            CcspTraceWarning(("Failed to get NSLookup DiagnosticsState parameter\n!"));

            *puLong = 0;

            return FALSE;
        }

        return TRUE;
    }

    if( AnscEqualString(ParamName, "Timeout", TRUE))
    {
        if ( !pNSLookupDiagInfo )
        {
            *puLong = 0;
            
            return FALSE;
        }

        *puLong = pNSLookupDiagInfo->Timeout;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "NumberOfRepetitions", TRUE))
    {
        if ( !pNSLookupDiagInfo )
        {
            *puLong = 0;
            
            return FALSE;
        }

        *puLong = pNSLookupDiagInfo->NumberOfRepetitions;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "SuccessCount", TRUE))
    {
        pNSLookupDiagInfo = (PDSLH_NSLOOKUP_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_NSLookup);

        if ( pNSLookupDiagInfo )
        {
            *puLong = pNSLookupDiagInfo->SuccessCount;
        }
        else
        {
            CcspTraceWarning(("Failed to get NSLookup SuccessCount parameter\n!"));

            *puLong = 0;

            return FALSE;
        }

        return TRUE;
    }


    CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        NSLookupDiagnostics_GetParamStringValue
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
NSLookupDiagnostics_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_NSLOOKUP_INFO             pNSLookupDiagInfo   = pMyObject->hDiagNSLookInfo;
    errno_t rc = -1;

    if ( !pNSLookupDiagInfo )
    {
        return -1;
    }

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Interface", TRUE))
    {
        if ( AnscSizeOfString(pNSLookupDiagInfo->Interface) < *pUlSize )
        {
            rc = strcpy_s(pValue, *pUlSize, pNSLookupDiagInfo->Interface);
            if(rc != EOK)
            {
               ERR_CHK(rc);
               return -1;
            }
        }
        else
        {
            *pUlSize = AnscSizeOfString(pNSLookupDiagInfo->Interface) + 1;

            return 1;
        }

        return 0;
    }

    if( AnscEqualString(ParamName, "HostName", TRUE))
    {
        if ( AnscSizeOfString(pNSLookupDiagInfo->HostName) < *pUlSize )
        {
            rc = strcpy_s(pValue, *pUlSize, pNSLookupDiagInfo->HostName);
            if(rc != EOK)
            {
               ERR_CHK(rc);
               return -1;
            }
        }
        else
        {
            *pUlSize = AnscSizeOfString(pNSLookupDiagInfo->HostName) + 1;

            return 1;
        }

        return 0;
    }

    if( AnscEqualString(ParamName, "DNSServer", TRUE))
    {
        if ( AnscSizeOfString(pNSLookupDiagInfo->DNSServer) < *pUlSize )
        {
            rc = strcpy_s(pValue, *pUlSize, pNSLookupDiagInfo->DNSServer);
            if(rc != EOK)
            {
               ERR_CHK(rc);
               return -1;
            }
        }
        else
        {
            *pUlSize = AnscSizeOfString(pNSLookupDiagInfo->DNSServer) + 1;

            return 1;
        }
        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        NSLookupDiagnostics_SetParamBoolValue
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
NSLookupDiagnostics_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
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
        NSLookupDiagnostics_SetParamIntValue
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
NSLookupDiagnostics_SetParamIntValue
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
        NSLookupDiagnostics_SetParamUlongValue
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
NSLookupDiagnostics_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_NSLOOKUP_INFO             pNSLookupDiagInfo   = pMyObject->hDiagNSLookInfo;
    PDSLH_NSLOOKUP_INFO             pDiagInfo           = NULL;

    if ( !pNSLookupDiagInfo )
    {
        return FALSE;
    }

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "DiagnosticsState", TRUE))
    {
        if ( (uValue - 1) == (ULONG)DSLH_DIAG_STATE_TYPE_Requested )
        {
            pNSLookupDiagInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_Requested;
            pNSLookupDiagInfo->bForced = TRUE;
        }
        else
        {
            pNSLookupDiagInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
            pNSLookupDiagInfo->bForced = FALSE;
            return FALSE;
        }

        return TRUE;
    }

    if( AnscEqualString(ParamName, "Timeout", TRUE))
    {
        pDiagInfo = (PDSLH_NSLOOKUP_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_NSLookup);
        
        if ( pDiagInfo && pDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Inprogress )
        {
            CosaDmlDiagCancelDiagnostic(DSLH_DIAGNOSTIC_TYPE_NSLookup);
            pDiagInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
        }

        pNSLookupDiagInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
        pNSLookupDiagInfo->Timeout = uValue;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "NumberOfRepetitions", TRUE))
    {
        pDiagInfo = (PDSLH_NSLOOKUP_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_NSLookup);
        
        if ( pDiagInfo && pDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Inprogress )
        {
            CosaDmlDiagCancelDiagnostic(DSLH_DIAGNOSTIC_TYPE_NSLookup);
            pDiagInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
        }
        
        pNSLookupDiagInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
        pNSLookupDiagInfo->NumberOfRepetitions = uValue;

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        NSLookupDiagnostics_SetParamStringValue
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
NSLookupDiagnostics_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_NSLOOKUP_INFO             pNSLookupDiagInfo   = pMyObject->hDiagNSLookInfo;
    PDSLH_NSLOOKUP_INFO             pDiagInfo           = NULL;
    errno_t    rc  = -1;

    if ( !pNSLookupDiagInfo )
    {
        return FALSE;
    }

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Interface", TRUE))
    {
        pDiagInfo = (PDSLH_NSLOOKUP_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_NSLookup);
        
        if ( pDiagInfo && pDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Inprogress )
        {
            CosaDmlDiagCancelDiagnostic(DSLH_DIAGNOSTIC_TYPE_NSLookup);
            pDiagInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
        }
        
        pNSLookupDiagInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
        rc = STRCPY_S_NOCLOBBER(pNSLookupDiagInfo->Interface, sizeof(pNSLookupDiagInfo->Interface), pString);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
        return TRUE;
    }

    if( AnscEqualString(ParamName, "HostName", TRUE))
    {
        pDiagInfo = (PDSLH_NSLOOKUP_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_NSLookup);
        
        if ( pDiagInfo && pDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Inprogress )
        {
            CosaDmlDiagCancelDiagnostic(DSLH_DIAGNOSTIC_TYPE_NSLookup);
            pDiagInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
        }
        
        pNSLookupDiagInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
        rc = STRCPY_S_NOCLOBBER(pNSLookupDiagInfo->HostName, pString);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
        return TRUE;
    }

    if( AnscEqualString(ParamName, "DNSServer", TRUE))
    {
        pDiagInfo = (PDSLH_NSLOOKUP_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_NSLookup);
        
        if ( pDiagInfo && pDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Inprogress )
        {
            CosaDmlDiagCancelDiagnostic(DSLH_DIAGNOSTIC_TYPE_NSLookup);
            pDiagInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
        }
        
        pNSLookupDiagInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
        rc = STRCPY_S_NOCLOBBER(pNSLookupDiagInfo->DNSServer, pString);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return FALSE;
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
        NSLookupDiagnostics_Validate
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
NSLookupDiagnostics_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    char*                           p                  = NULL;
    char*                           pDomainName        = NULL;
    ULONG                           ulDNLength         = DSLH_NS_MAX_STRING_LENGTH_Host;
    ULONG                           i                  = 0;
    PCOSA_DATAMODEL_DIAG            pMyObject          = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_NSLOOKUP_INFO             pNSLookupDiagInfo  = pMyObject->hDiagNSLookInfo;
    errno_t  rc = -1;

    if ( !pNSLookupDiagInfo )
    {
        return FALSE;
    }

    //COSAValidateHierarchyInterface doesn't work now due to incomplete lan device management
    if ( pNSLookupDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Requested )//&&
         //COSAValidateHierarchyInterface && AnscSizeOfString(pNSLookupDiagInfo->Interface) > 0 )
    {
        // COSAValidateHierarchyInterface depends on the specific target
        return TRUE;
        /*
        if ( !COSAValidateHierarchyInterface
                (
                    pNSLookupDiagInfo->Interface,
                    DSLH_LAN_LAYER3_INTERFACE | DSLH_WAN_LAYER3_CONNECTION_INTERFACE | DSLH_NULL_STRING_INTERFACE
                ))
        {
            AnscCopyString(pReturnParamName, "Interface");
            return FALSE;
        }
        */
    }

    if ( pNSLookupDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Requested || AnscSizeOfString(pNSLookupDiagInfo->HostName) > 0 )
    {
        p = pNSLookupDiagInfo->HostName;

        //check if there is any illegal character
        for(i = 0; i < AnscSizeOfString(p); i++)
        {
            if ( (p[i] >= '0' && p[i] <= '9') || (p[i] >= 'a' && p[i] <= 'z') ||
                (p[i] >= 'A' && p[i] <= 'Z') || p[i] == '$' || p[i] == '-' ||
                p[i] == '_' || p[i] == '.' || p[i] == '+' || p[i] == '!' ||
                p[i] == '*' || p[i] == 39 || p[i] == '(' || p[i] == ')' ||
                p[i] == ',' || p[i] == '"' )
            {
                continue;
            }
            else
            {
                rc = STRCPY_S_NOCLOBBER(pReturnParamName, *puLength, "HostName");
                ERR_CHK(rc);
                pNSLookupDiagInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
                return FALSE;
            }
        }
            
        if ( pNSLookupDiagInfo->HostName[0] != '.')
        {
            p = _ansc_strstr(pNSLookupDiagInfo->HostName + 1, "..");

            if ( p )
            {
                for(; p < pNSLookupDiagInfo->HostName + AnscSizeOfString(pNSLookupDiagInfo->HostName); p++)
                {
                    if ( *p != '.' )
                    {
                        rc = STRCPY_S_NOCLOBBER(pReturnParamName, *puLength, "HostName");
                        ERR_CHK(rc);
                        pNSLookupDiagInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
                        return FALSE;
                    }
                }
            }
        }
        else
        {
            rc = STRCPY_S_NOCLOBBER(pReturnParamName, *puLength, "HostName");
            ERR_CHK(rc);
            pNSLookupDiagInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
            return FALSE;
        }

        pDomainName = AnscAllocateMemory(DSLH_NS_MAX_STRING_LENGTH_Host);

        if ( !pDomainName )
        {
            rc = STRCPY_S_NOCLOBBER(pReturnParamName, *puLength, "HostName");
            ERR_CHK(rc);
            pNSLookupDiagInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
            return FALSE;
        }

        CosaGetParamValueString
            (
                "Device.DHCPv4.Server.Pool.1.DomainName",
                pDomainName,
                &ulDNLength
            );

        if ( !AnscSizeOfString(pDomainName) )
        {
            if ( pNSLookupDiagInfo->HostName[0] != '.')
            {               
                p = _ansc_strchr(pNSLookupDiagInfo->HostName + 1, '.');

                if ( !p )
                {
                    rc = STRCPY_S_NOCLOBBER(pReturnParamName, *puLength, "HostName");
                    ERR_CHK(rc);
                    pNSLookupDiagInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
                    return FALSE;
                }
                else if ( p[1] == '0' || p[1] == '.' )
                {
                    rc = STRCPY_S_NOCLOBBER(pReturnParamName, *puLength, "HostName");
                    ERR_CHK(rc);
                    pNSLookupDiagInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
                    return FALSE;
                }
            }
            else
            {
                rc = STRCPY_S_NOCLOBBER(pReturnParamName, *puLength, "HostName");
                ERR_CHK(rc);
                pNSLookupDiagInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
                return FALSE;
            }
        }
        else
        {
            if ( pNSLookupDiagInfo->HostName[0] == '.')
            {
                rc = STRCPY_S_NOCLOBBER(pReturnParamName, *puLength, "HostName");
                ERR_CHK(rc);
                pNSLookupDiagInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
                return FALSE;
            }
        }
    }

    if ( pNSLookupDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Requested || AnscSizeOfString(pNSLookupDiagInfo->DNSServer) > 0  )
    {
        //still not clear how to validate dns server
        return TRUE;
    }

    if ( TRUE )
    {
        if ( pNSLookupDiagInfo->Timeout < DSLH_NS_MIN_Timeout )
        {
            rc = STRCPY_S_NOCLOBBER(pReturnParamName, *puLength, "Timeout");
            ERR_CHK(rc);
            pNSLookupDiagInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
            return FALSE;
        }
    }

    if ( TRUE )
    {
        if ( pNSLookupDiagInfo->NumberOfRepetitions < DSLH_NS_MIN_NumberOfRepetitions )
        {
            rc = STRCPY_S_NOCLOBBER(pReturnParamName, *puLength, "NumberOfRepetitions");
            ERR_CHK(rc);
            pNSLookupDiagInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
            return FALSE;
        }
    }

    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        NSLookupDiagnostics_Commit
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
NSLookupDiagnostics_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_FAILURE;
    PCOSA_DATAMODEL_DIAG            pMyObject          = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_NSLOOKUP_INFO             pNSLookupDiagInfo  = pMyObject->hDiagNSLookInfo;

    if ( !pNSLookupDiagInfo )
    {
        return returnStatus;
    }

    if ( pNSLookupDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Requested )
    {
        returnStatus = CosaDmlDiagScheduleDiagnostic(DSLH_DIAGNOSTIC_TYPE_NSLookup, (ANSC_HANDLE)pNSLookupDiagInfo);
        pNSLookupDiagInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
        pNSLookupDiagInfo->bForced = FALSE;
    }

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        NSLookupDiagnostics_Rollback
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
NSLookupDiagnostics_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DIAG            pMyObject          = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_NSLOOKUP_INFO             pNSLookupInfo      = pMyObject->hDiagNSLookInfo;
    PDSLH_NSLOOKUP_INFO             pNSLookupDiagInfo  = NULL;
    errno_t   rc = -1;

    if ( !pNSLookupInfo )
    {
        return ANSC_STATUS_FAILURE;
    }

    pNSLookupDiagInfo = (PDSLH_NSLOOKUP_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_NSLookup);

    if ( pNSLookupDiagInfo )
    {
        DslhInitNSLookupInfo(pNSLookupInfo);

        pNSLookupInfo->StructSize    = sizeof(DSLH_NSLOOKUP_INFO);

        rc = STRCPY_S_NOCLOBBER(pNSLookupInfo->HostName, sizeof(pNSLookupInfo->HostName), pNSLookupDiagInfo->HostName);
        ERR_CHK(rc);
        rc = STRCPY_S_NOCLOBBER(pNSLookupInfo->Interface, sizeof(pNSLookupInfo->Interface), pNSLookupDiagInfo->Interface);
        ERR_CHK(rc);
        rc = STRCPY_S_NOCLOBBER(pNSLookupInfo->DNSServer, sizeof(pNSLookupInfo->DNSServer), pNSLookupDiagInfo->DNSServer);
        ERR_CHK(rc);

        pNSLookupInfo->bForced = FALSE;
        pNSLookupInfo->Timeout = pNSLookupDiagInfo->Timeout;
        pNSLookupInfo->NumberOfRepetitions = pNSLookupDiagInfo->NumberOfRepetitions;
        pNSLookupInfo->UpdatedAt = pNSLookupDiagInfo->UpdatedAt;
    }
    else
    {
        DslhInitNSLookupInfo(pNSLookupInfo);
    }

    return returnStatus;
}

/***********************************************************************

 APIs for Object:

    DNS.Diagnostics.NSLookupDiagnostics.Result.{i}.

    *  Result_GetEntryCount
    *  Result_GetEntry
    *  Result_IsUpdated
    *  Result_Synchronize
    *  Result_GetParamBoolValue
    *  Result_GetParamIntValue
    *  Result_GetParamUlongValue
    *  Result_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Result_GetEntryCount
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
Result_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PDSLH_NSLOOKUP_INFO             pNSLookupDiagInfo  =  NULL;

    pNSLookupDiagInfo = (PDSLH_NSLOOKUP_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_NSLookup);

    if ( pNSLookupDiagInfo && pNSLookupDiagInfo->DiagnosticState != DSLH_DIAG_STATE_TYPE_None 
         && pNSLookupDiagInfo->DiagnosticState != DSLH_DIAG_STATE_TYPE_Requested )
    {
        return pNSLookupDiagInfo->ResultNumberOfEntries;
    }

    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Result_GetEntry
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
Result_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PDSLH_NSLOOKUP_INFO             pNSLookupDiagInfo  = NULL;
    PBBHM_NS_LOOKUP_ECHO_ENTRY      pEchoInfo          = (PBBHM_NS_LOOKUP_ECHO_ENTRY   )NULL;

    pNSLookupDiagInfo = (PDSLH_NSLOOKUP_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_NSLookup);
    
    if ( pNSLookupDiagInfo )
    {
        pEchoInfo = (PBBHM_NS_LOOKUP_ECHO_ENTRY)pNSLookupDiagInfo->hDiaginfo;
    }   

    if ( !pEchoInfo )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        *pInsNumber  = nIndex + 1;

        return &pEchoInfo[nIndex];
    }

    return NULL; /* return the handle */
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Result_IsUpdated
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/
BOOL
Result_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject          = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_NSLOOKUP_INFO             pNSLookupInfo      = pMyObject->hDiagNSLookInfo;
    PDSLH_NSLOOKUP_INFO             pNSLookupDiagInfo  = NULL;

    if ( !pNSLookupInfo )
    {
        return FALSE;
    }

    pNSLookupDiagInfo = (PDSLH_NSLOOKUP_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_NSLookup);

    if ( pNSLookupDiagInfo && pNSLookupDiagInfo->UpdatedAt != pNSLookupInfo->UpdatedAt )
    {
        return  TRUE;
    }

    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Result_Synchronize
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
Result_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject          = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_NSLOOKUP_INFO             pNSLookupInfo      = pMyObject->hDiagNSLookInfo;
    PDSLH_NSLOOKUP_INFO             pNSLookupDiagInfo  = NULL;

    if ( !pNSLookupInfo )
    {
        return ANSC_STATUS_FAILURE;
    }

    pNSLookupDiagInfo = (PDSLH_NSLOOKUP_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_NSLookup);

    if ( !pNSLookupDiagInfo )
    {
        CcspTraceWarning(("Failed to get NSLookup backend information!\n"));

        return  ANSC_STATUS_FAILURE;
    }

    pNSLookupInfo->UpdatedAt = pNSLookupDiagInfo->UpdatedAt;

    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Result_GetParamBoolValue
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
Result_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
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
        Result_GetParamIntValue
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
Result_GetParamIntValue
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
        Result_GetParamUlongValue
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
Result_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PBBHM_NS_LOOKUP_ECHO_ENTRY      pEchoInfo          = (PBBHM_NS_LOOKUP_ECHO_ENTRY)hInsContext;

    if ( !pEchoInfo )
    {
        CcspTraceWarning(("Fail to get result parameters hInsContext!\n"));

        return FALSE;
    }

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Status", TRUE))
    {
        *puLong = pEchoInfo->Status + 1;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "AnswerType", TRUE))
    {
        *puLong = pEchoInfo->AnswerType + 1;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "DNSServerIP", TRUE))
    {
        *puLong = pEchoInfo->DNSServerIP.Value;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "ResponseTime", TRUE))
    {
        *puLong = pEchoInfo->ResponsTime;

        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Result_GetParamStringValue
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
Result_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PBBHM_NS_LOOKUP_ECHO_ENTRY      pEchoInfo          = (PBBHM_NS_LOOKUP_ECHO_ENTRY)hInsContext;
    errno_t   rc = -1;

    if ( !pEchoInfo )
    {
        CcspTraceWarning(("Fail to get result parameters hInsContext!\n"));

        return ANSC_STATUS_FAILURE;
    }

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "HostNameReturned", TRUE))
    {
        if ( pEchoInfo->HostNameReturned )
        {
            if ( AnscSizeOfString(pEchoInfo->HostNameReturned) < *pUlSize )
            {
                rc = strcpy_s(pValue, *pUlSize, pEchoInfo->HostNameReturned);
                if(rc != EOK)
                {
                   ERR_CHK(rc);
                   return -1;
                }

                return 0;
            }
            else
            {
                *pUlSize = AnscSizeOfString(pEchoInfo->HostNameReturned) + 1;

                return 1;
            }
        }

        return ANSC_STATUS_FAILURE;
    }

    if( AnscEqualString(ParamName, "IPAddresses", TRUE))
    {
        if ( pEchoInfo->IPAddresses )
        {
            if ( AnscSizeOfString(pEchoInfo->IPAddresses) < *pUlSize )
            {
                rc = strcpy_s(pValue, *pUlSize, pEchoInfo->IPAddresses);
                if(rc != EOK)
                {
                   ERR_CHK(rc);
                   return -1;
                }

                return 0;
            }
            else
            {
                *pUlSize = AnscSizeOfString(pEchoInfo->IPAddresses) + 1;

                return 1;
            }
        }
        
        return ANSC_STATUS_FAILURE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}
#endif

