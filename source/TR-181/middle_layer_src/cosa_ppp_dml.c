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

    module: cosa_ppp_dml.c

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

        07/12/2011    initial revision.

**************************************************************************/
#ifndef  FEATURE_RDKB_XDSL_PPP_MANAGER

#include "ansc_platform.h"
#include "cosa_ppp_dml.h"
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

    PPP.

    *  PPP_GetParamBoolValue
    *  PPP_GetParamIntValue
    *  PPP_GetParamUlongValue
    *  PPP_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        PPP_GetParamBoolValue
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
PPP_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pBool);
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        PPP_GetParamIntValue
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
PPP_GetParamIntValue
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
        PPP_GetParamUlongValue
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
PPP_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(puLong);
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        PPP_GetParamStringValue
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
PPP_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pUlSize);
    errno_t      rc  =  -1;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "SupportedNCPs") == 0)
    {
        ULONG flag;
        
        CosaDmlPPPGetSupportedNCPs(NULL, &flag);

        pValue[0] = '\0';

        if (flag & COSA_DML_PPP_SUPPORTED_NCP_ATCP)
        {
            rc = strcat_s(pValue, *pUlSize,  "ATCP,");
            if ( rc != EOK)
            {
                ERR_CHK(rc);
                return -1;
            }

        }

        if (flag & COSA_DML_PPP_SUPPORTED_NCP_IPCP)
        {
            rc = strcat_s(pValue, *pUlSize,  "IPCP,");
            if ( rc != EOK)
            {
                ERR_CHK(rc);
                return -1;
            }

        }

        if (flag & COSA_DML_PPP_SUPPORTED_NCP_IPXCP)
        {
            rc = strcat_s(pValue, *pUlSize,  "IPXCP,");
            if ( rc != EOK)
            {
                ERR_CHK(rc);
                return -1;
            }

        }

        if (flag & COSA_DML_PPP_SUPPORTED_NCP_NBFCP)
        {
            rc = strcat_s(pValue, *pUlSize,  "NBFCP,");
            if ( rc != EOK)
            {
                ERR_CHK(rc);
                return -1;
            }

        }

        if (flag & COSA_DML_PPP_SUPPORTED_NCP_IPv6CP)
        {
            rc = strcat_s(pValue, *pUlSize,  "IPv6CP,");
            if ( rc != EOK)
            {
                ERR_CHK(rc);
                return -1;
            }

        }

        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/***********************************************************************

 APIs for Object:

    PPP.Interface.{i}.

    *  Interface_GetEntryCount
    *  Interface_GetEntry
    *  Interface_AddEntry
    *  Interface_DelEntry
    *  Interface_GetParamBoolValue
    *  Interface_GetParamIntValue
    *  Interface_GetParamUlongValue
    *  Interface_GetParamStringValue
    *  Interface_SetParamBoolValue
    *  Interface_SetParamIntValue
    *  Interface_SetParamUlongValue
    *  Interface_SetParamStringValue
    *  Interface_Validate
    *  Interface_Commit
    *  Interface_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Interface_GetEntryCount
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
Interface3_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_PPP             pMyObject = (PCOSA_DATAMODEL_PPP)g_pCosaBEManager->hPPP;
    
    return AnscSListQueryDepth(&pMyObject->IfList);
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Interface_GetEntry
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
Interface3_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_PPP             pMyObject               = (PCOSA_DATAMODEL_PPP      )g_pCosaBEManager->hPPP;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry             = (PSINGLE_LINK_ENTRY       )NULL;
    PCOSA_DML_PPP_IF_FULL           pEntry                  = (PCOSA_DML_PPP_IF_FULL    )NULL;

    pSLinkEntry = AnscSListGetEntryByIndex(&pMyObject->IfList, nIndex);

    if ( pSLinkEntry )
    {
        pCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);

        *pInsNumber = pCosaContext->InstanceNumber;

        pEntry = pCosaContext->hContext;
        
        CosaDmlPppIfGetCfg(NULL, &pEntry->Cfg);
    }

    return pCosaContext;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Interface_AddEntry
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
Interface3_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_PPP             pMyObject               = (PCOSA_DATAMODEL_PPP      )g_pCosaBEManager->hPPP;
    PSLIST_HEADER                   pListHead               = (PSLIST_HEADER            )&pMyObject->IfList;
    PCOSA_DML_PPP_IF_FULL           pEntry                  = (PCOSA_DML_PPP_IF_FULL    )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    errno_t                         rc                      = -1;


    pEntry = (PCOSA_DML_PPP_IF_FULL)AnscAllocateMemory(sizeof(COSA_DML_PPP_IF_FULL));
    if (!pEntry)
    {
        return NULL;
    }

    rc = sprintf_s(pEntry->Cfg.Alias, sizeof(pEntry->Cfg.Alias),"Interface%lu", pMyObject->ulIfNextInstance);
    if(rc < EOK)
    {
      ERR_CHK(rc);
    }

    /* Update the cache */
    pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
    if (!pCosaContext)
    {
        AnscFreeMemory(pEntry);

        return NULL;
    }

    pCosaContext->InstanceNumber = pEntry->Cfg.InstanceNumber = pMyObject->ulIfNextInstance;

    pMyObject->ulIfNextInstance++;

    if ( pMyObject->ulIfNextInstance == 0 )
    {
        pMyObject->ulIfNextInstance = 1;
    }

    pCosaContext->hContext        = (ANSC_HANDLE)pEntry;
    pCosaContext->hParentTable    = NULL;
    pCosaContext->bNew            = TRUE;

    CosaSListPushEntryByInsNum(pListHead, pCosaContext);

    CosaPPPIfRegAddInfo((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pCosaContext);

    *pInsNumber = pCosaContext->InstanceNumber;

    return pCosaContext;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Interface_DelEntry
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
Interface3_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2           = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_PPP_IF_FULL           pEntry                  = (PCOSA_DML_PPP_IF_FULL    )pCosaContext->hContext;
    PCOSA_DML_PPP_IF_FULL           pNewEntry               = (PCOSA_DML_PPP_IF_FULL    )NULL;
    PCOSA_DATAMODEL_PPP             pMyObject               = (PCOSA_DATAMODEL_PPP      )g_pCosaBEManager->hPPP;
    PSLIST_HEADER                   pListHead               = (PSLIST_HEADER            )&pMyObject->IfList;
    PSINGLE_LINK_ENTRY              pSLinkEntry             = NULL;


    CosaDmlPppIfDelEntry(NULL, pEntry->Cfg.InstanceNumber);

    /* Update the cache */
    if ( TRUE )
    {
        pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->IfList);

        while ( pSLinkEntry )
        {
            pCosaContext2 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
            pSLinkEntry   = AnscSListGetNextEntry(pSLinkEntry);

            pNewEntry = (PCOSA_DML_PPP_IF_FULL)pCosaContext2->hContext;

            if ( pNewEntry && AnscEqualString(pNewEntry->Cfg.Alias, pEntry->Cfg.Alias, FALSE))
            {
                AnscSListPopEntryByLink(pListHead, &pCosaContext2->Linkage);

				if (pCosaContext->bNew)
	                CosaPPPIfRegDelInfo((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pCosaContext2);

                AnscFreeMemory(pNewEntry);
                AnscFreeMemory(pCosaContext2);

                break;
            }
        }
    }
        
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Interface_GetParamBoolValue
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
Interface3_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_PPP_IF_FULL           pEntry                  = (PCOSA_DML_PPP_IF_FULL)pContextLinkObject->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "IPCPEnable") == 0)
    {
        /* collect value */
        *pBool = pEntry->Cfg.bEnabled;
        return TRUE;
    }

    if (strcmp(ParamName, "Reset") == 0)
    {
        /* collect value */
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
        Interface_GetParamIntValue
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
Interface3_GetParamIntValue
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
        Interface_GetParamUlongValue
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
Interface3_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_PPP_IF_FULL           pEntry                  = (PCOSA_DML_PPP_IF_FULL)pContextLinkObject->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Status") == 0)
    {
        /* collect value */
        CosaDmlPppIfGetInfo(NULL, pEntry->Cfg.InstanceNumber, &pEntry->Info);
        *puLong = pEntry->Info.Status;
        return TRUE;
    }

    if (strcmp(ParamName, "LastChange") == 0)
    {
        /* collect value */
        CosaDmlPppIfGetInfo(NULL, pEntry->Cfg.InstanceNumber, &pEntry->Info);
        *puLong = AnscGetTimeIntervalInSeconds(pEntry->Info.LastChange, AnscGetTickInSeconds());
        return TRUE;
    }

    if (strcmp(ParamName, "ConnectionStatus") == 0)
    {
        /* collect value */
        CosaDmlPppIfGetInfo(NULL, pEntry->Cfg.InstanceNumber, &pEntry->Info);
        *puLong = pEntry->Info.ConnectionStatus;
        return TRUE;
    }

    if (strcmp(ParamName, "LastConnectionError") == 0)
    {
        /* collect value */
        CosaDmlPppIfGetInfo(NULL, pEntry->Cfg.InstanceNumber, &pEntry->Info);
        *puLong = pEntry->Info.LastConnectionError;
        return TRUE;
    }

    if (strcmp(ParamName, "AutoDisconnectTime") == 0)
    {
        /* collect value */
        *puLong = pEntry->Cfg.AutoDisconnectTime;
        return TRUE;
    }

    if (strcmp(ParamName, "IdleDisconnectTime") == 0)
    {
        /* collect value */
        *puLong = pEntry->Cfg.IdleDisconnectTime;
        return TRUE;
    }

    if (strcmp(ParamName, "WarnDisconnectDelay") == 0)
    {
        /* collect value */
        *puLong = pEntry->Cfg.WarnDisconnectDelay;
        return TRUE;
    }

    if (strcmp(ParamName, "EncryptionProtocol") == 0)
    {
        /* collect value */
        CosaDmlPppIfGetInfo(NULL, pEntry->Cfg.InstanceNumber, &pEntry->Info);
        *puLong = pEntry->Info.EncryptionProtocol;
        return TRUE;
    }

    if (strcmp(ParamName, "CompressionProtocol") == 0)
    {
        /* collect value */
        CosaDmlPppIfGetInfo(NULL, pEntry->Cfg.InstanceNumber, &pEntry->Info);
        *puLong = pEntry->Info.CompressionProtocol;
        return TRUE;
    }

    if (strcmp(ParamName, "AuthenticationProtocol") == 0)
    {
        /* collect value */
        CosaDmlPppIfGetInfo(NULL, pEntry->Cfg.InstanceNumber, &pEntry->Info);
        *puLong = pEntry->Info.AuthenticationProtocol;
        return TRUE;
    }

    if (strcmp(ParamName, "MaxMRUSize") == 0)
    {
        /* collect value */
        *puLong = pEntry->Cfg.MaxMRUSize;
        return TRUE;
    }

    if (strcmp(ParamName, "CurrentMRUSize") == 0)
    {
        /* collect value */
        CosaDmlPppIfGetInfo(NULL, pEntry->Cfg.InstanceNumber, &pEntry->Info);
        *puLong = pEntry->Info.CurrentMRUSize;
        return TRUE;
    }

    if (strcmp(ParamName, "ConnectionTrigger") == 0)
    {
        /* collect value */
        *puLong = pEntry->Cfg.ConnectionTrigger;
        return TRUE;
    }

    if (strcmp(ParamName, "LCPEcho") == 0)
    {
        /* collect value */
        CosaDmlPppIfGetInfo(NULL, pEntry->Cfg.InstanceNumber, &pEntry->Info);
        *puLong = pEntry->Info.LCPEcho;
        return TRUE;
    }

    if (strcmp(ParamName, "LCPEchoRetry") == 0)
    {
        /* collect value */
        CosaDmlPppIfGetInfo(NULL, pEntry->Cfg.InstanceNumber, &pEntry->Info);
        *puLong = pEntry->Info.LCPEchoRetry;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Interface_GetParamStringValue
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
Interface3_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(pUlSize);
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_PPP_IF_FULL           pEntry                  = (PCOSA_DML_PPP_IF_FULL)pContextLinkObject->hContext;
    PUCHAR                          pString                 = NULL;
    errno_t                         rc                      = -1;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize,  pEntry->Cfg.Alias);
        if ( rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }

        return 0;
    }

    if (strcmp(ParamName, "Name") == 0)
    {
        /* collect value */
        CosaDmlPppIfGetInfo(NULL, pEntry->Cfg.InstanceNumber, &pEntry->Info);
        rc = strcpy_s(pValue, *pUlSize,  pEntry->Info.Name);
        if ( rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }

        return 0;
    }

    if (strcmp(ParamName, "LowerLayers") == 0)
    {
        /* collect value */
        pString = CosaUtilGetFullPathNameByKeyword((PUCHAR)"Device.Ethernet.Link.", (PUCHAR)"Name", (PUCHAR)"wan0");
        
        /*LowerLayers should not end with '.'*/
        if (pString && pString[_ansc_strlen((const char*)pString)-1] == '.')
            pString[_ansc_strlen((const char*)pString)-1] = 0;
        
        if ( pString )
        {
            rc = strcpy_s(pValue, *pUlSize,  (char*)pString);
            AnscFreeMemory(pString);
            if ( rc != EOK)
            {
                ERR_CHK(rc);
                return -1;
            }

        }
        
        return 0;
    }

    if (strcmp(ParamName, "Username") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize,  pEntry->Cfg.Username);
        if ( rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }

        return 0;
    }

    if (strcmp(ParamName, "Password") == 0)
    {
        /* collect value */
        pValue[0] = '\0';

        return 0;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Interface_SetParamBoolValue
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
Interface3_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_PPP_IF_FULL           pEntry                  = (PCOSA_DML_PPP_IF_FULL)pContextLinkObject->hContext;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "IPCPEnable") == 0)
    {
        /* save update to backup */
        pEntry->Cfg.bEnabled = bValue;
        return TRUE;
    }

    if (strcmp(ParamName, "Reset") == 0)
    {
        /* save update to backup */
        if ( pEntry->Cfg.bEnabled && bValue )
        {
            CosaDmlPppIfReset(NULL, pEntry->Cfg.InstanceNumber);
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
        Interface_SetParamIntValue
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
Interface3_SetParamIntValue
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
        Interface_SetParamUlongValue
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
Interface3_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_PPP_IF_FULL           pEntry                  = (PCOSA_DML_PPP_IF_FULL)pContextLinkObject->hContext;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "AutoDisconnectTime") == 0)
    {
        /* save update to backup */
        pEntry->Cfg.AutoDisconnectTime = uValue;
        return TRUE;
    }

    if (strcmp(ParamName, "IdleDisconnectTime") == 0)
    {
        /* save update to backup */
        pEntry->Cfg.IdleDisconnectTime = uValue;
        return TRUE;
    }

    if (strcmp(ParamName, "WarnDisconnectDelay") == 0)
    {
        /* save update to backup */
        pEntry->Cfg.WarnDisconnectDelay = uValue;
        return TRUE;
    }

    if (strcmp(ParamName, "MaxMRUSize") == 0)
    {
        /* save update to backup */


        pEntry->Cfg.MaxMRUSize = uValue;
        return TRUE;
    }

    if (strcmp(ParamName, "ConnectionTrigger") == 0)
    {
        /* save update to backup */
        

        pEntry->Cfg.ConnectionTrigger = uValue;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Interface_SetParamStringValue
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
Interface3_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_PPP_IF_FULL           pEntry                  = (PCOSA_DML_PPP_IF_FULL)pContextLinkObject->hContext;
    errno_t                         rc                      = -1;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* save update to backup */
        rc = strcpy_s(pEntry->Cfg.Alias, sizeof(pEntry->Cfg.Alias), pString);
        if ( rc != EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "LowerLayers") == 0)
    {
        /* save update to backup */
        return TRUE;
    }

    if (strcmp(ParamName, "Username") == 0)
    {
        /* save update to backup */
        rc = strcpy_s(pEntry->Cfg.Username, sizeof(pEntry->Cfg.Username), pString);
        if ( rc != EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "Password") == 0)
    {
        /* save update to backup */
        rc = strcpy_s(pEntry->Cfg.Password, sizeof(pEntry->Cfg.Password), pString);
        if ( rc != EOK)
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
        Interface_Validate
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
Interface3_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_DATAMODEL_PPP             pMyObject         = (PCOSA_DATAMODEL_PPP      )g_pCosaBEManager->hPPP;
    PSLIST_HEADER                   pListHead         = (PSLIST_HEADER            )&pMyObject->IfList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_PPP_IF_FULL           pEntry            = (PCOSA_DML_PPP_IF_FULL    )pCosaContext->hContext;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2     = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_PPP_IF_FULL           pEntry2           = (PCOSA_DML_PPP_IF_FULL    )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY       )NULL;
    errno_t                         rc                      = -1;

    pSLinkEntry = AnscSListGetFirstEntry(pListHead);

    while ( pSLinkEntry )
    {
        pCosaContext2 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        pSLinkEntry   = AnscSListGetNextEntry(pSLinkEntry);

        pEntry2       = (PCOSA_DML_PPP_IF_FULL)pCosaContext2->hContext;

        if ( 
                 pEntry2 && 
                 ((ULONG)pEntry2 != (ULONG)pEntry) && 
                 (strcmp(pEntry->Cfg.Alias, pEntry2->Cfg.Alias) == 0)
           )
        {
            rc = strcpy_s(pReturnParamName, *puLength, "Alias");
            if ( rc != EOK)
            {
                ERR_CHK(rc);
                return FALSE;
            }
            *puLength = AnscSizeOfString("Alias")+1;
            
            return FALSE;
        }
    }
    
    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Interface_Commit
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
Interface3_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_PPP             pMyObject               = (PCOSA_DATAMODEL_PPP      )g_pCosaBEManager->hPPP;
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_PPP_IF_FULL           pEntry                  = (PCOSA_DML_PPP_IF_FULL    )pContextLinkObject->hContext;

    if ( pContextLinkObject->bNew )
    {
        pContextLinkObject->bNew = FALSE;

        CosaDmlPppIfAddEntry(NULL, pEntry);
        CosaPPPIfRegDelInfo((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pContextLinkObject);
    }
    else
    {
        CosaDmlPppIfSetCfg(NULL, &pEntry->Cfg);
    }
    
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Interface_Rollback
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
Interface3_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_PPP_IF_FULL           pEntry                  = (PCOSA_DML_PPP_IF_FULL)pContextLinkObject->hContext;

    CosaDmlPppIfGetCfg(NULL, &pEntry->Cfg);
    
    return 0;
}

/***********************************************************************

 APIs for Object:

    PPP.Interface.{i}.PPPoE.

    *  PPPoE_GetParamBoolValue
    *  PPPoE_GetParamIntValue
    *  PPPoE_GetParamUlongValue
    *  PPPoE_GetParamStringValue
    *  PPPoE_SetParamBoolValue
    *  PPPoE_SetParamIntValue
    *  PPPoE_SetParamUlongValue
    *  PPPoE_SetParamStringValue
    *  PPPoE_Validate
    *  PPPoE_Commit
    *  PPPoE_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        PPPoE_GetParamBoolValue
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
PPPoE_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pBool);
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        PPPoE_GetParamIntValue
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
PPPoE_GetParamIntValue
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
        PPPoE_GetParamUlongValue
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
PPPoE_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_PPP_IF_FULL           pEntry                  = (PCOSA_DML_PPP_IF_FULL)pContextLinkObject->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "SessionID") == 0)
    {
        /* collect value */
        CosaDmlPppIfGetInfo(NULL, pEntry->Cfg.InstanceNumber, &pEntry->Info);
        *puLong = pEntry->Info.SessionID;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        PPPoE_GetParamStringValue
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
PPPoE_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(pUlSize);
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_PPP_IF_FULL           pEntry                  = (PCOSA_DML_PPP_IF_FULL)pContextLinkObject->hContext;
    errno_t                         rc                      = -1;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "ACName") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pEntry->Cfg.ACName);
        if ( rc != EOK)
        {
            ERR_CHK(rc);
            return -1;
        }
        return 0;
    }

    if (strcmp(ParamName, "ServiceName") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pEntry->Cfg.ServiceName);
        if ( rc != EOK)
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
        PPPoE_SetParamBoolValue
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
PPPoE_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(bValue);
    /* check the parameter name and set the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        PPPoE_SetParamIntValue
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
PPPoE_SetParamIntValue
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
        PPPoE_SetParamUlongValue
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
PPPoE_SetParamUlongValue
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
        PPPoE_SetParamStringValue
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
PPPoE_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_PPP_IF_FULL           pEntry                  = (PCOSA_DML_PPP_IF_FULL)pContextLinkObject->hContext;
    errno_t                         rc                      = -1;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "ACName") == 0)
    {
        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER(pEntry->Cfg.ACName, sizeof(pEntry->Cfg.ACName), pString);
        if ( rc != EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
        return TRUE;
    }

    if (strcmp(ParamName, "ServiceName") == 0)
    {
        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER(pEntry->Cfg.ServiceName, sizeof(pEntry->Cfg.ServiceName), pString);
        if ( rc != EOK)
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
        PPPoE_Validate
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
PPPoE_Validate
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
        PPPoE_Commit
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
PPPoE_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_PPP             pMyObject               = (PCOSA_DATAMODEL_PPP      )g_pCosaBEManager->hPPP;
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_PPP_IF_FULL           pEntry                  = (PCOSA_DML_PPP_IF_FULL    )pContextLinkObject->hContext;

    if ( pContextLinkObject->bNew )
    {
        pContextLinkObject->bNew = FALSE;

        CosaDmlPppIfAddEntry(NULL, pEntry);
        CosaPPPIfRegDelInfo((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pContextLinkObject);
    }
    else
    {
        CosaDmlPppIfSetCfg(NULL, &pEntry->Cfg);
    }
    
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        PPPoE_Rollback
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
PPPoE_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_PPP_IF_FULL           pEntry                  = (PCOSA_DML_PPP_IF_FULL)pContextLinkObject->hContext;

    CosaDmlPppIfGetCfg(NULL, &pEntry->Cfg);
    
    return 0;
}

/***********************************************************************

 APIs for Object:

    PPP.Interface.{i}.IPCP.

    *  IPCP_GetParamBoolValue
    *  IPCP_GetParamIntValue
    *  IPCP_GetParamUlongValue
    *  IPCP_GetParamStringValue
    *  IPCP_SetParamBoolValue
    *  IPCP_SetParamIntValue
    *  IPCP_SetParamUlongValue
    *  IPCP_SetParamStringValue
    *  IPCP_Validate
    *  IPCP_Commit
    *  IPCP_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPCP_GetParamBoolValue
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
IPCP_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_PPP_IF_FULL           pEntry                  = (PCOSA_DML_PPP_IF_FULL)pContextLinkObject->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "PassthroughEnable") == 0)
    {
        /* collect value */
        *pBool = pEntry->Cfg.PassthroughEnable;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPCP_GetParamIntValue
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
IPCP_GetParamIntValue
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
        IPCP_GetParamUlongValue
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
IPCP_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(puLong);
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPCP_GetParamStringValue
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
IPCP_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(pUlSize);
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_PPP_IF_FULL           pEntry                  = (PCOSA_DML_PPP_IF_FULL)pContextLinkObject->hContext;
    errno_t                         rc                      = -1;

    CosaDmlPppIfGetInfo(NULL, pEntry->Cfg.InstanceNumber, &pEntry->Info);
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "LocalIPAddress") == 0)
    {
        /* collect value */
        rc = sprintf_s(pValue,
                       *pUlSize,
                        "%u.%u.%u.%u",
                        pEntry->Info.LocalIPAddress.Dot[0],
                        pEntry->Info.LocalIPAddress.Dot[1],
                        pEntry->Info.LocalIPAddress.Dot[2],
                        pEntry->Info.LocalIPAddress.Dot[3]
                     );
        if(rc < EOK)
        {
          ERR_CHK(rc);
          return -1;
        }
        return 0;
    }

    if (strcmp(ParamName, "RemoteIPAddress") == 0)
    {
        /* collect value */
        rc = sprintf_s(pValue,
                       *pUlSize,
                        "%u.%u.%u.%u",
                        pEntry->Info.RemoteIPAddress.Dot[0],
                        pEntry->Info.RemoteIPAddress.Dot[1],
                        pEntry->Info.RemoteIPAddress.Dot[2],
                        pEntry->Info.RemoteIPAddress.Dot[3]
                     );
        if(rc < EOK)
        {
          ERR_CHK(rc);
          return -1;
        }
        return 0;
    }

    if (strcmp(ParamName, "DNSServers") == 0)
    {
        /* collect value */
        if (!pEntry->Info.DNSServers[1].Value)
        {
            rc = sprintf_s(pValue,
                          *pUlSize,
                          "%u.%u.%u.%u",
                          pEntry->Info.DNSServers[0].Dot[0],
                          pEntry->Info.DNSServers[0].Dot[1],
                          pEntry->Info.DNSServers[0].Dot[2],
                          pEntry->Info.DNSServers[0].Dot[3]
                          );
            if(rc < EOK)
            {
               ERR_CHK(rc);
               return -1;
            }
        }
        else
        {
            rc = sprintf_s(pValue,
                          *pUlSize,
                          "%u.%u.%u.%u,%u.%u.%u.%u",
                          pEntry->Info.DNSServers[0].Dot[0],
                          pEntry->Info.DNSServers[0].Dot[1],
                          pEntry->Info.DNSServers[0].Dot[2],
                          pEntry->Info.DNSServers[0].Dot[3],
                          pEntry->Info.DNSServers[1].Dot[0],
                          pEntry->Info.DNSServers[1].Dot[1],
                          pEntry->Info.DNSServers[1].Dot[2],
                          pEntry->Info.DNSServers[1].Dot[3]
                          );
            if(rc < EOK)
            {
               ERR_CHK(rc);
               return -1;
            }
        }
        return 0;
    }

    if (strcmp(ParamName, "PassthroughDHCPPool") == 0)
    {
        /* collect value */
        rc = strcpy_s(pValue, *pUlSize, pEntry->Cfg.PassthroughDHCPPool);
        if ( rc != EOK)
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
        IPCP_SetParamBoolValue
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
IPCP_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_PPP_IF_FULL           pEntry                  = (PCOSA_DML_PPP_IF_FULL)pContextLinkObject->hContext;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "PassthroughEnable") == 0)
    {
        /* save update to backup */
        pEntry->Cfg.PassthroughEnable = bValue;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPCP_SetParamIntValue
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
IPCP_SetParamIntValue
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
        IPCP_SetParamUlongValue
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
IPCP_SetParamUlongValue
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
        IPCP_SetParamStringValue
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
IPCP_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_PPP_IF_FULL           pEntry                  = (PCOSA_DML_PPP_IF_FULL)pContextLinkObject->hContext;
    errno_t                         rc                      = -1;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "PassthroughDHCPPool") == 0)
    {
        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER(pEntry->Cfg.PassthroughDHCPPool, sizeof(pEntry->Cfg.PassthroughDHCPPool), pString);
        if ( rc != EOK)
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
        IPCP_Validate
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
IPCP_Validate
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
        IPCP_Commit
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
IPCP_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_PPP             pMyObject               = (PCOSA_DATAMODEL_PPP      )g_pCosaBEManager->hPPP;
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_PPP_IF_FULL           pEntry                  = (PCOSA_DML_PPP_IF_FULL    )pContextLinkObject->hContext;

    if ( pContextLinkObject->bNew )
    {
        pContextLinkObject->bNew = FALSE;

        CosaDmlPppIfAddEntry(NULL, pEntry);
        CosaPPPIfRegDelInfo((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pContextLinkObject);
    }
    else
    {
        CosaDmlPppIfSetCfg(NULL, &pEntry->Cfg);
    }
    
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPCP_Rollback
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
IPCP_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_PPP_IF_FULL           pEntry                  = (PCOSA_DML_PPP_IF_FULL)pContextLinkObject->hContext;

    CosaDmlPppIfGetCfg(NULL, &pEntry->Cfg);
    
    return 0;
}

/***********************************************************************

 APIs for Object:

    PPP.Interface.{i}.Stats.

    *  Stats_GetParamBoolValue
    *  Stats_GetParamIntValue
    *  Stats_GetParamUlongValue
    *  Stats_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Stats_GetParamBoolValue
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
Stats6_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pBool);
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Stats_GetParamIntValue
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
Stats6_GetParamIntValue
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
        Stats_GetParamUlongValue
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
Stats6_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_PPP_IF_FULL           pEntry                  = (PCOSA_DML_PPP_IF_FULL)pContextLinkObject->hContext;
    COSA_DML_IF_STATS               Stats = {0};

    CosaDmlPppIfGetStats(NULL, pEntry->Cfg.InstanceNumber, &Stats);

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "BytesSent") == 0)
    {
        /* collect value */
        *puLong = Stats.BytesSent;
        return TRUE;
    }

    if (strcmp(ParamName, "BytesReceived") == 0)
    {
        /* collect value */
        *puLong = Stats.BytesReceived;
        return TRUE;
    }

    if (strcmp(ParamName, "PacketsSent") == 0)
    {
        /* collect value */
        *puLong = Stats.PacketsSent;
        return TRUE;
    }

    if (strcmp(ParamName, "PacketsReceived") == 0)
    {
        /* collect value */
        *puLong = Stats.PacketsReceived;
        return TRUE;
    }

    if (strcmp(ParamName, "ErrorsSent") == 0)
    {
        /* collect value */
        *puLong = Stats.ErrorsSent;
        return TRUE;
    }

    if (strcmp(ParamName, "ErrorsReceived") == 0)
    {
        /* collect value */
        *puLong = Stats.ErrorsReceived;
        return TRUE;
    }

    if (strcmp(ParamName, "UnicastPacketsSent") == 0)
    {
        /* collect value */
        *puLong = Stats.UnicastPacketsSent;
        return TRUE;
    }

    if (strcmp(ParamName, "UnicastPacketsReceived") == 0)
    {
        /* collect value */
        *puLong = Stats.UnicastPacketsReceived;
        return TRUE;
    }

    if (strcmp(ParamName, "DiscardPacketsSent") == 0)
    {
        /* collect value */
        *puLong = Stats.DiscardPacketsSent;
        return TRUE;
    }

    if (strcmp(ParamName, "DiscardPacketsReceived") == 0)
    {
        /* collect value */
        *puLong = Stats.DiscardPacketsReceived;
        return TRUE;
    }

    if (strcmp(ParamName, "MulticastPacketsSent") == 0)
    {
        /* collect value */
        *puLong = Stats.MulticastPacketsSent;
        return TRUE;
    }

    if (strcmp(ParamName, "MulticastPacketsReceived") == 0)
    {
        /* collect value */
        *puLong = Stats.MulticastPacketsReceived;
        return TRUE;
    }

    if (strcmp(ParamName, "BroadcastPacketsSent") == 0)
    {
        /* collect value */
        *puLong = Stats.BroadcastPacketsSent;
        return TRUE;
    }

    if (strcmp(ParamName, "BroadcastPacketsReceived") == 0)
    {
        /* collect value */
        *puLong = Stats.BroadcastPacketsReceived;
        return TRUE;
    }

    if (strcmp(ParamName, "UnknownProtoPacketsReceived") == 0)
    {
        /* collect value */
        *puLong = Stats.UnknownProtoPacketsReceived;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Stats_GetParamStringValue
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
Stats6_GetParamStringValue
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

#endif
