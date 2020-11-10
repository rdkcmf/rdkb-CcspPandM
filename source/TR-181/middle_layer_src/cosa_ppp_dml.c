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
#ifndef  FEATURE_RDKB_WAN_MANAGER

#include "ansc_platform.h"
#include "cosa_ppp_dml.h"

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
    /* check the parameter name and return the corresponding value */
    if (AnscEqualString(ParamName, "SupportedNCPs", TRUE))
    {
        ULONG flag;
        
        CosaDmlPPPGetSupportedNCPs(NULL, &flag);

        AnscCopyString(pValue, "");

        if (flag & COSA_DML_PPP_SUPPORTED_NCP_ATCP)
        {
            AnscCatString(pValue, "ATCP,");
        }

        if (flag & COSA_DML_PPP_SUPPORTED_NCP_IPCP)
        {
            AnscCatString(pValue, "IPCP,");
        }

        if (flag & COSA_DML_PPP_SUPPORTED_NCP_IPXCP)
        {
            AnscCatString(pValue, "IPXCP,");
        }

        if (flag & COSA_DML_PPP_SUPPORTED_NCP_NBFCP)
        {
            AnscCatString(pValue, "NBFCP,");
        }

        if (flag & COSA_DML_PPP_SUPPORTED_NCP_IPv6CP)
        {
            AnscCatString(pValue, "IPv6CP,");
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
    PCOSA_DATAMODEL_PPP             pMyObject               = (PCOSA_DATAMODEL_PPP      )g_pCosaBEManager->hPPP;
    PSLIST_HEADER                   pListHead               = (PSLIST_HEADER            )&pMyObject->IfList;
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
    PCOSA_DATAMODEL_PPP             pMyObject               = (PCOSA_DATAMODEL_PPP      )g_pCosaBEManager->hPPP;
    PSLIST_HEADER                   pListHead               = (PSLIST_HEADER            )&pMyObject->IfList;
    PCOSA_DML_PPP_IF_FULL           pEntry                  = (PCOSA_DML_PPP_IF_FULL    )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry             = (PSINGLE_LINK_ENTRY       )NULL;

#ifdef _COSA_DRG_CNS_
    return NULL;
#endif

    pEntry = (PCOSA_DML_PPP_IF_FULL)AnscAllocateMemory(sizeof(COSA_DML_PPP_IF_FULL));
    if (!pEntry)
    {
        return NULL;
    }

    _ansc_sprintf(pEntry->Cfg.Alias, "Interface%d", pMyObject->ulIfNextInstance);

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
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext            = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2           = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_PPP_IF_FULL           pEntry                  = (PCOSA_DML_PPP_IF_FULL    )pCosaContext->hContext;
    PCOSA_DML_PPP_IF_FULL           pNewEntry               = (PCOSA_DML_PPP_IF_FULL    )NULL;
    PCOSA_DATAMODEL_PPP             pMyObject               = (PCOSA_DATAMODEL_PPP      )g_pCosaBEManager->hPPP;
    PSLIST_HEADER                   pListHead               = (PSLIST_HEADER            )&pMyObject->IfList;
    PSINGLE_LINK_ENTRY              pSLinkEntry             = NULL;

#ifdef _COSA_DRG_CNS_
    return ANSC_STATUS_FAILURE;
#endif

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
    if( AnscEqualString(ParamName, "IPCPEnable", TRUE))
    {
        /* collect value */
        *pBool = pEntry->Cfg.bEnabled;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Reset", TRUE))
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
    if( AnscEqualString(ParamName, "Status", TRUE))
    {
        /* collect value */
        CosaDmlPppIfGetInfo(NULL, pEntry->Cfg.InstanceNumber, &pEntry->Info);
        *puLong = pEntry->Info.Status;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "LastChange", TRUE))
    {
        /* collect value */
        CosaDmlPppIfGetInfo(NULL, pEntry->Cfg.InstanceNumber, &pEntry->Info);
        *puLong = AnscGetTimeIntervalInSeconds(pEntry->Info.LastChange, AnscGetTickInSeconds());
        return TRUE;
    }

    if( AnscEqualString(ParamName, "ConnectionStatus", TRUE))
    {
        /* collect value */
        CosaDmlPppIfGetInfo(NULL, pEntry->Cfg.InstanceNumber, &pEntry->Info);
        *puLong = pEntry->Info.ConnectionStatus;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "LastConnectionError", TRUE))
    {
        /* collect value */
        CosaDmlPppIfGetInfo(NULL, pEntry->Cfg.InstanceNumber, &pEntry->Info);
        *puLong = pEntry->Info.LastConnectionError;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "AutoDisconnectTime", TRUE))
    {
        /* collect value */
        *puLong = pEntry->Cfg.AutoDisconnectTime;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "IdleDisconnectTime", TRUE))
    {
        /* collect value */
        *puLong = pEntry->Cfg.IdleDisconnectTime;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "WarnDisconnectDelay", TRUE))
    {
        /* collect value */
        *puLong = pEntry->Cfg.WarnDisconnectDelay;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "EncryptionProtocol", TRUE))
    {
        /* collect value */
        CosaDmlPppIfGetInfo(NULL, pEntry->Cfg.InstanceNumber, &pEntry->Info);
        *puLong = pEntry->Info.EncryptionProtocol;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "CompressionProtocol", TRUE))
    {
        /* collect value */
        CosaDmlPppIfGetInfo(NULL, pEntry->Cfg.InstanceNumber, &pEntry->Info);
        *puLong = pEntry->Info.CompressionProtocol;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "AuthenticationProtocol", TRUE))
    {
        /* collect value */
        CosaDmlPppIfGetInfo(NULL, pEntry->Cfg.InstanceNumber, &pEntry->Info);
        *puLong = pEntry->Info.AuthenticationProtocol;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "MaxMRUSize", TRUE))
    {
        /* collect value */
        *puLong = pEntry->Cfg.MaxMRUSize;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "CurrentMRUSize", TRUE))
    {
        /* collect value */
        CosaDmlPppIfGetInfo(NULL, pEntry->Cfg.InstanceNumber, &pEntry->Info);
        *puLong = pEntry->Info.CurrentMRUSize;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "ConnectionTrigger", TRUE))
    {
        /* collect value */
        *puLong = pEntry->Cfg.ConnectionTrigger;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "LCPEcho", TRUE))
    {
        /* collect value */
        CosaDmlPppIfGetInfo(NULL, pEntry->Cfg.InstanceNumber, &pEntry->Info);
        *puLong = pEntry->Info.LCPEcho;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "LCPEchoRetry", TRUE))
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
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_PPP_IF_FULL           pEntry                  = (PCOSA_DML_PPP_IF_FULL)pContextLinkObject->hContext;
    PUCHAR                          pString                 = NULL;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pEntry->Cfg.Alias);
        return 0;
    }

    if( AnscEqualString(ParamName, "Name", TRUE))
    {
        /* collect value */
        CosaDmlPppIfGetInfo(NULL, pEntry->Cfg.InstanceNumber, &pEntry->Info);
        AnscCopyString(pValue, pEntry->Info.Name);
        return 0;
    }

    if( AnscEqualString(ParamName, "LowerLayers", TRUE))
    {
        /* collect value */
        pString = CosaUtilGetFullPathNameByKeyword("Device.Ethernet.Link.", "Name", "wan0");
        
        /*LowerLayers should not end with '.'*/
        if (pString && pString[_ansc_strlen(pString)-1] == '.')
            pString[_ansc_strlen(pString)-1] = 0;
        
        if ( pString )
        {
            AnscCopyString(pValue, pString);
            AnscFreeMemory(pString);
        }
        
        return 0;
    }

    if( AnscEqualString(ParamName, "Username", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pEntry->Cfg.Username);
        return 0;
    }

    if( AnscEqualString(ParamName, "Password", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, "");
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
    if( AnscEqualString(ParamName, "IPCPEnable", TRUE))
    {
        /* save update to backup */
        pEntry->Cfg.bEnabled = bValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Reset", TRUE))
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
    if( AnscEqualString(ParamName, "AutoDisconnectTime", TRUE))
    {
        /* save update to backup */
        pEntry->Cfg.AutoDisconnectTime = uValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "IdleDisconnectTime", TRUE))
    {
        /* save update to backup */
        pEntry->Cfg.IdleDisconnectTime = uValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "WarnDisconnectDelay", TRUE))
    {
        /* save update to backup */
        pEntry->Cfg.WarnDisconnectDelay = uValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "MaxMRUSize", TRUE))
    {
        /* save update to backup */
#ifdef _COSA_DRG_CNS_
        /*not supported*/
        return FALSE;
#endif


        pEntry->Cfg.MaxMRUSize = uValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "ConnectionTrigger", TRUE))
    {
        /* save update to backup */
#ifdef _COSA_DRG_CNS_
        /*don't support manual*/
        if (uValue == COSA_DML_PPP_CONN_TRIGGER_Manual) 
            return FALSE;
#endif
        

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

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE))
    {
        /* save update to backup */
        AnscCopyString(pEntry->Cfg.Alias, pString);
        return TRUE;
    }

    if( AnscEqualString(ParamName, "LowerLayers", TRUE))
    {
        /* save update to backup */
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Username", TRUE))
    {
        /* save update to backup */
        AnscCopyString(pEntry->Cfg.Username, pString);
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Password", TRUE))
    {
        /* save update to backup */
        AnscCopyString(pEntry->Cfg.Password, pString);
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

    pSLinkEntry = AnscSListGetFirstEntry(pListHead);

    while ( pSLinkEntry )
    {
        pCosaContext2 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        pSLinkEntry   = AnscSListGetNextEntry(pSLinkEntry);

        pEntry2       = (PCOSA_DML_PPP_IF_FULL)pCosaContext2->hContext;

        if ( 
                 pEntry2 && 
                 ((ULONG)pEntry2 != (ULONG)pEntry) && 
                 AnscEqualString(pEntry->Cfg.Alias, pEntry2->Cfg.Alias, TRUE) 
           )
        {
            AnscCopyString(pReturnParamName, "Alias");

            *puLength = AnscSizeOfString("Alias");
            
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
    if( AnscEqualString(ParamName, "SessionID", TRUE))
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
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_PPP_IF_FULL           pEntry                  = (PCOSA_DML_PPP_IF_FULL)pContextLinkObject->hContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "ACName", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pEntry->Cfg.ACName);
        return 0;
    }

    if( AnscEqualString(ParamName, "ServiceName", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pEntry->Cfg.ServiceName);
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

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "ACName", TRUE))
    {
        /* save update to backup */
        AnscCopyString(pEntry->Cfg.ACName, pString);
        return TRUE;
    }

    if( AnscEqualString(ParamName, "ServiceName", TRUE))
    {
        /* save update to backup */
        AnscCopyString(pEntry->Cfg.ServiceName, pString);
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
    if( AnscEqualString(ParamName, "PassthroughEnable", TRUE))
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
    PCOSA_CONTEXT_LINK_OBJECT       pContextLinkObject      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_PPP_IF_FULL           pEntry                  = (PCOSA_DML_PPP_IF_FULL)pContextLinkObject->hContext;

    CosaDmlPppIfGetInfo(NULL, pEntry->Cfg.InstanceNumber, &pEntry->Info);
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "LocalIPAddress", TRUE))
    {
        /* collect value */
        _ansc_sprintf(pValue,
                        "%u.%u.%u.%u",
                        pEntry->Info.LocalIPAddress.Dot[0],
                        pEntry->Info.LocalIPAddress.Dot[1],
                        pEntry->Info.LocalIPAddress.Dot[2],
                        pEntry->Info.LocalIPAddress.Dot[3]
                     );
        return 0;
    }

    if( AnscEqualString(ParamName, "RemoteIPAddress", TRUE))
    {
        /* collect value */
        _ansc_sprintf(pValue,
                        "%u.%u.%u.%u",
                        pEntry->Info.RemoteIPAddress.Dot[0],
                        pEntry->Info.RemoteIPAddress.Dot[1],
                        pEntry->Info.RemoteIPAddress.Dot[2],
                        pEntry->Info.RemoteIPAddress.Dot[3]
                     );
        return 0;
    }

    if( AnscEqualString(ParamName, "DNSServers", TRUE))
    {
        /* collect value */
        if (!pEntry->Info.DNSServers[1].Value)
            _ansc_sprintf(pValue,
                          "%u.%u.%u.%u",
                          pEntry->Info.DNSServers[0].Dot[0],
                          pEntry->Info.DNSServers[0].Dot[1],
                          pEntry->Info.DNSServers[0].Dot[2],
                          pEntry->Info.DNSServers[0].Dot[3]
                          );
        else
            _ansc_sprintf(pValue,
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
        return 0;
    }

    if( AnscEqualString(ParamName, "PassthroughDHCPPool", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pEntry->Cfg.PassthroughDHCPPool);
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
    if( AnscEqualString(ParamName, "PassthroughEnable", TRUE))
    {
        /* save update to backup */
#ifdef _COSA_DRG_CNS_
        /*not supported*/
        return FALSE;
#endif
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

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "PassthroughDHCPPool", TRUE))
    {
        /* save update to backup */
#ifdef _COSA_DRG_CNS_
        /*not supported*/
        return FALSE;
#endif
        AnscCopyString(pEntry->Cfg.PassthroughDHCPPool, pString);
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
    if( AnscEqualString(ParamName, "BytesSent", TRUE))
    {
        /* collect value */
        *puLong = Stats.BytesSent;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "BytesReceived", TRUE))
    {
        /* collect value */
        *puLong = Stats.BytesReceived;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "PacketsSent", TRUE))
    {
        /* collect value */
        *puLong = Stats.PacketsSent;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "PacketsReceived", TRUE))
    {
        /* collect value */
        *puLong = Stats.PacketsReceived;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "ErrorsSent", TRUE))
    {
        /* collect value */
        *puLong = Stats.ErrorsSent;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "ErrorsReceived", TRUE))
    {
        /* collect value */
        *puLong = Stats.ErrorsReceived;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "UnicastPacketsSent", TRUE))
    {
        /* collect value */
        *puLong = Stats.UnicastPacketsSent;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "UnicastPacketsReceived", TRUE))
    {
        /* collect value */
        *puLong = Stats.UnicastPacketsReceived;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "DiscardPacketsSent", TRUE))
    {
        /* collect value */
        *puLong = Stats.DiscardPacketsSent;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "DiscardPacketsReceived", TRUE))
    {
        /* collect value */
        *puLong = Stats.DiscardPacketsReceived;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "MulticastPacketsSent", TRUE))
    {
        /* collect value */
        *puLong = Stats.MulticastPacketsSent;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "MulticastPacketsReceived", TRUE))
    {
        /* collect value */
        *puLong = Stats.MulticastPacketsReceived;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "BroadcastPacketsSent", TRUE))
    {
        /* collect value */
        *puLong = Stats.BroadcastPacketsSent;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "BroadcastPacketsReceived", TRUE))
    {
        /* collect value */
        *puLong = Stats.BroadcastPacketsReceived;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "UnknownProtoPacketsReceived", TRUE))
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
    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

#endif
