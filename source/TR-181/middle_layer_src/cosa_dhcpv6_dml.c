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

    module: cosa_apis_dhcpv6.c

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

        09/07/2011    initial revision.

**************************************************************************/

#include "ansc_platform.h"
#include "plugin_main_apis.h"
#include "cosa_dhcpv6_apis.h"
#include "cosa_dhcpv6_internal.h"

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

    DHCPv6.

    *  DHCPv6_GetParamBoolValue
    *  DHCPv6_GetParamIntValue
    *  DHCPv6_GetParamUlongValue
    *  DHCPv6_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DHCPv6_GetParamBoolValue
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
DHCPv6_GetParamBoolValue
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
        DHCPv6_GetParamIntValue
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
DHCPv6_GetParamIntValue
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
        DHCPv6_GetParamUlongValue
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
DHCPv6_GetParamUlongValue
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
        DHCPv6_GetParamStringValue
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
DHCPv6_GetParamStringValue
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

/***********************************************************************

 APIs for Object:

    DHCPv6.Client.{i}.

    *  Client3_GetEntryCount
    *  Client3_GetEntry
    *  Client3_AddEntry
    *  Client3_DelEntry
    *  Client3_GetParamBoolValue
    *  Client3_GetParamIntValue
    *  Client3_GetParamUlongValue
    *  Client3_GetParamStringValue
    *  Client3_SetParamBoolValue
    *  Client3_SetParamIntValue
    *  Client3_SetParamUlongValue
    *  Client3_SetParamStringValue
    *  Client3_Validate
    *  Client3_Commit
    *  Client3_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Client3_GetEntryCount
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
Client3_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DHCPV6          pDhcpv6           = (PCOSA_DATAMODEL_DHCPV6)g_pCosaBEManager->hDhcpv6;

    return AnscSListQueryDepth( &pDhcpv6->ClientList );
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Client3_GetEntry
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
Client3_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PSINGLE_LINK_ENTRY              pSListEntry       = NULL;
    PCOSA_DATAMODEL_DHCPV6          pDhcpv6           = (PCOSA_DATAMODEL_DHCPV6)g_pCosaBEManager->hDhcpv6;
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtLink          = NULL;

    pSListEntry = AnscSListGetEntryByIndex(&pDhcpv6->ClientList, nIndex);

    if ( pSListEntry )
    {
        pCxtLink          = ACCESS_COSA_CONTEXT_DHCPCV6_LINK_OBJECT(pSListEntry);
        *pInsNumber       = pCxtLink->InstanceNumber;
    }

    return pSListEntry;
    
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Client3_AddEntry
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
Client3_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PSINGLE_LINK_ENTRY              pSListEntry       = NULL;
    PCOSA_DATAMODEL_DHCPV6          pDhcpv6           = (PCOSA_DATAMODEL_DHCPV6)g_pCosaBEManager->hDhcpv6;
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtLink        = NULL;
    PCOSA_DML_DHCPCV6_FULL            pDhcpc          = NULL;

#if (defined _COSA_DRG_CNS_)  || (defined _COSA_DRG_TPG_)
    /*not supported*/
    return NULL;
#endif
    
    pDhcpc  = (PCOSA_DML_DHCPCV6_FULL)AnscAllocateMemory( sizeof(COSA_DML_DHCPCV6_FULL) );
    if ( !pDhcpc )
    {
        goto EXIT2;
    }

    /* Set default value */
    DHCPV6_CLIENT_SET_DEFAULTVALUE(pDhcpc);

    /* Add into our link tree*/    
    pCxtLink = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)AnscAllocateMemory( sizeof(COSA_CONTEXT_DHCPCV6_LINK_OBJECT) );
    if ( !pCxtLink )
    {
        goto EXIT1;
    }

    DHCPV6_CLIENT_INITIATION_CONTEXT(pCxtLink)

    pCxtLink->hContext       = (ANSC_HANDLE)pDhcpc;
    pCxtLink->bNew           = TRUE;
    
    if ( !++pDhcpv6->maxInstanceOfClient )
    {
        pDhcpv6->maxInstanceOfClient = 1;
    }
    pDhcpc->Cfg.InstanceNumber = pDhcpv6->maxInstanceOfClient;
    pCxtLink->InstanceNumber   = pDhcpc->Cfg.InstanceNumber;
    *pInsNumber                = pDhcpc->Cfg.InstanceNumber;

    _ansc_sprintf( pDhcpc->Cfg.Alias, "Client%d", pDhcpc->Cfg.InstanceNumber);

    /* Put into our list */
    CosaSListPushEntryByInsNum(&pDhcpv6->ClientList, (PCOSA_CONTEXT_LINK_OBJECT)pCxtLink);

    /* we recreate the configuration because we has new delay_added entry for dhcpv6 */
    CosaDhcpv6RegSetDhcpv6Info(pDhcpv6);

    return (ANSC_HANDLE)pCxtLink;

EXIT1:
    
    AnscFreeMemory(pDhcpc);

EXIT2:        
    
    return NULL; /* return the handle */
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Client3_DelEntry
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
Client3_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PSINGLE_LINK_ENTRY              pSListEntry       = NULL;
    PCOSA_DATAMODEL_DHCPV6          pDhcpv6           = (PCOSA_DATAMODEL_DHCPV6)g_pCosaBEManager->hDhcpv6;
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtLink        = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)hInstance;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink2         = NULL;
    PCOSA_DML_DHCPCV6_FULL          pDhcpc            = (PCOSA_DML_DHCPCV6_FULL)pCxtLink->hContext;

    /* Normally, two sublinks are empty because our framework will firstly 
            call delEntry for them before coming here. We needn't care them.
         */
#if (defined _COSA_DRG_CNS_)  || (defined _COSA_DRG_TPG_)
    /*not supported*/
    return ANSC_STATUS_FAILURE;
#endif


    if ( !pCxtLink->bNew )
    {
        returnStatus = CosaDmlDhcpv6cDelEntry(NULL, pDhcpc->Cfg.InstanceNumber);
        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            return returnStatus;
        }
    }
    
    if (AnscSListPopEntryByLink(&pDhcpv6->ClientList, &pCxtLink->Linkage) )
    {
        /* Because the current NextInstanceNumber information need be deleted */
        CosaDhcpv6RegSetDhcpv6Info(pDhcpv6);

        AnscFreeMemory(pCxtLink->hContext);
        AnscFreeMemory(pCxtLink);
    }
    
    return returnStatus;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Client3_GetParamBoolValue
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
Client3_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    ANSC_STATUS                       returnStatus    = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtLink        = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPCV6_FULL            pDhcpc          = (PCOSA_DML_DHCPCV6_FULL)pCxtLink->hContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE) )
    {
        /* collect value */
        /**pBool   = pDhcpc->Cfg.bEnabled;*/
        *pBool   =CosaDmlDhcpv6cGetEnabled(NULL);

        return TRUE;
    }

    if( AnscEqualString(ParamName, "RequestAddresses", TRUE) )
    {
        /* collect value */
        *pBool   = pDhcpc->Cfg.RequestAddresses;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "RequestPrefixes", TRUE) )
    {
        /* collect value */
        *pBool   = pDhcpc->Cfg.RequestPrefixes;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "RapidCommit", TRUE) )
    {
        /* collect value */
        *pBool   = pDhcpc->Cfg.RapidCommit;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "Renew", TRUE) )
    {
        /* collect value */
        *pBool   = FALSE;

        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Client3_GetParamIntValue
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
Client3_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtLink          = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPCV6_FULL            pDhcpc            = (PCOSA_DML_DHCPCV6_FULL)pCxtLink->hContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "SuggestedT1", TRUE) )
    {
        /* collect value */        
        *pInt   = pDhcpc->Cfg.SuggestedT1;
        
        return TRUE;
    }

    if( AnscEqualString(ParamName, "SuggestedT2", TRUE) )
    {
        /* collect value */
        *pInt   = pDhcpc->Cfg.SuggestedT2;

        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Client3_GetParamUlongValue
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
Client3_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtLink          = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPCV6_FULL            pDhcpc            = (PCOSA_DML_DHCPCV6_FULL)pCxtLink->hContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Status", TRUE) )
    {
        /* collect value */
        CosaDmlDhcpv6cGetInfo(pDhcpc, pCxtLink->InstanceNumber, &pDhcpc->Info);

        *puLong   = pDhcpc->Info.Status;

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Client3_GetParamStringValue
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
Client3_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtLink          = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPCV6_FULL            pDhcpc            = (PCOSA_DML_DHCPCV6_FULL)pCxtLink->hContext;
    CHAR                            tmpBuff[128]      = {0};
    ULONG                           i                 = 0;
    ULONG                           len               = 0;
    PUCHAR                          pString           = NULL;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pDhcpc->Cfg.Alias) < *pUlSize)
        {
            AnscCopyString(pValue, pDhcpc->Cfg.Alias);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDhcpc->Cfg.Alias)+1;
            return 1;
        }

        return 0;
    }

    if( AnscEqualString(ParamName, "Interface", TRUE) )
    {
        /* collect value */
        pString = CosaUtilGetFullPathNameByKeyword
                    (
                        "Device.IP.Interface.",
                        "Name",
                        pDhcpc->Cfg.Interface
                    );
        if ( pString )
        {
            if ( AnscSizeOfString(pString) < *pUlSize)
            {
                AnscCopyString(pValue, pString);

                AnscFreeMemory(pString);

                return 0;
            }
            else
            {
                *pUlSize = AnscSizeOfString(pString)+1;

                AnscFreeMemory(pString);
                
                return 1;
            }
        }
        else
        {
            return 0;
        }

        return 0;
    }

    if( AnscEqualString(ParamName, "DUID", TRUE) )
    {
        CosaDmlDhcpv6cGetInfo(pDhcpc, pCxtLink->InstanceNumber, &pDhcpc->Info);

        /* collect value */
        if ( AnscSizeOfString(pDhcpc->Info.DUID) < *pUlSize)
        {
            AnscCopyString(pValue, pDhcpc->Info.DUID);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDhcpc->Info.DUID)+1;
            return 1;
        }
    }


    if( AnscEqualString(ParamName, "SupportedOptions", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pDhcpc->Info.SupportedOptions) < *pUlSize)
        {
            AnscCopyString(pValue, pDhcpc->Info.SupportedOptions);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDhcpc->Info.SupportedOptions)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "RequestedOptions", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pDhcpc->Cfg.RequestedOptions) < *pUlSize)
        {
            AnscCopyString(pValue, pDhcpc->Cfg.RequestedOptions);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDhcpc->Cfg.RequestedOptions)+1;
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
        Client3_SetParamBoolValue
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
Client3_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtLink          = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPCV6_FULL            pDhcpc            = (PCOSA_DML_DHCPCV6_FULL)pCxtLink->hContext;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE) )
    {
        /* save update to backup */
        pDhcpc->Cfg.bEnabled = bValue;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "RequestAddresses", TRUE) )
    {
        /* save update to backup */
        pDhcpc->Cfg.RequestAddresses = bValue;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "RequestPrefixes", TRUE) )
    {
        /* save update to backup */
        pDhcpc->Cfg.RequestPrefixes = bValue;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "RapidCommit", TRUE) )
    {
        /* save update to backup */
        pDhcpc->Cfg.RapidCommit = bValue;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "Renew", TRUE) )
    {
        /* save update to backup */
        if ( bValue )
        {
            returnStatus = CosaDmlDhcpv6cRenew(NULL, pDhcpc->Cfg.InstanceNumber);
            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                return  FALSE;
            }
        }
        
        return  TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Client3_SetParamIntValue
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
Client3_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtLink          = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPCV6_FULL            pDhcpc            = (PCOSA_DML_DHCPCV6_FULL)pCxtLink->hContext;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "SuggestedT1", TRUE) )
    {
        /* save update to backup */
        pDhcpc->Cfg.SuggestedT1 = iValue;
        
        return TRUE;
    }

    if( AnscEqualString(ParamName, "SuggestedT2", TRUE) )
    {
        /* save update to backup */
        pDhcpc->Cfg.SuggestedT2 = iValue;

        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Client3_SetParamUlongValue
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
Client3_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Client3_SetParamStringValue
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
Client3_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtLink          = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPCV6_FULL            pDhcpc            = (PCOSA_DML_DHCPCV6_FULL)pCxtLink->hContext;
    PCOSA_DATAMODEL_DHCPV6            pDhcpv6           = (PCOSA_DATAMODEL_DHCPV6)pCxtLink->hParentTable;
        
    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "RequestedOptions", TRUE) )
    {
        /* save update to backup */
        AnscCopyString(pDhcpc->Cfg.RequestedOptions, pString);

        return TRUE;
    }

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        /* save update to backup */
        AnscCopyString(pDhcpv6->AliasOfClient, pDhcpc->Cfg.Alias);

        AnscCopyString(pDhcpc->Cfg.Alias, pString);

        return TRUE;
    }

    if( AnscEqualString(ParamName, "Interface", TRUE) )
    {
#if defined _COSA_DRG_CNS_  || defined _COSA_DRG_TPG_
        /*not supported*/
        return FALSE;
#endif

        /* save update to backup */
        AnscCopyString(pDhcpc->Cfg.Interface, pString);

        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Client3_Validate
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
Client3_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DHCPV6            pDhcpv6           = (PCOSA_DATAMODEL_DHCPV6)g_pCosaBEManager->hDhcpv6;
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtLink          = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPCV6_FULL            pDhcpc            = (PCOSA_DML_DHCPCV6_FULL)pCxtLink->hContext;
    PSINGLE_LINK_ENTRY                pSListEntry       = NULL;
    PCOSA_DML_DHCPCV6_FULL            pDhcpc2           = NULL;
    BOOL                              bFound            = FALSE;

    /*  only for Alias */
    if ( pDhcpv6->AliasOfClient[0] )
    {
        pSListEntry           = AnscSListGetFirstEntry(&pDhcpv6->ClientList);
        while( pSListEntry != NULL)
        {
            pCxtLink          = ACCESS_COSA_CONTEXT_DHCPCV6_LINK_OBJECT(pSListEntry);
            pSListEntry       = AnscSListGetNextEntry(pSListEntry);
        
            pDhcpc2 = (PCOSA_DML_DHCPCV6_FULL)pCxtLink->hContext;
        
            if( DHCPV6_CLIENT_ENTRY_MATCH2(pDhcpc2->Cfg.Alias, pDhcpc->Cfg.Alias) )
            {
                if ( (ANSC_HANDLE)pCxtLink == hInsContext )
                {
                    continue;
                }

                _ansc_strcpy(pReturnParamName, "Alias");

                bFound = TRUE;
                
                break;
            }
        }
        
        if ( bFound )
        {
#if COSA_DHCPV6_ROLLBACK_TEST        
            Client3_Rollback(hInsContext);
#endif
            return FALSE;
        }
    }

    /* some other checking */
    if (pDhcpc->Cfg.bEnabled)
    {
        if (pDhcpc->Cfg.SuggestedT1 > pDhcpc->Cfg.SuggestedT2)
            return FALSE;
    }



    
    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Client3_Commit
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
Client3_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtLink          = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPCV6_FULL            pDhcpc            = (PCOSA_DML_DHCPCV6_FULL)pCxtLink->hContext;
    PCOSA_DATAMODEL_DHCPV6            pDhcpv6           = (PCOSA_DATAMODEL_DHCPV6)g_pCosaBEManager->hDhcpv6;

    if ( pCxtLink->bNew )
    {
        returnStatus = CosaDmlDhcpv6cAddEntry(NULL, pDhcpc );

        if ( returnStatus == ANSC_STATUS_SUCCESS )
        {
            pCxtLink->bNew = FALSE;

            CosaDhcpv6RegSetDhcpv6Info(pDhcpv6);
        }
        else
        {
            DHCPV6_CLIENT_SET_DEFAULTVALUE(pDhcpc);
            
            if ( pDhcpv6->AliasOfClient[0] )
                AnscCopyString( pDhcpc->Cfg.Alias, pDhcpv6->AliasOfClient );
        }
    }
    else
    {
        returnStatus = CosaDmlDhcpv6cSetCfg(NULL, &pDhcpc->Cfg);

        if ( returnStatus != ANSC_STATUS_SUCCESS)
        {
            CosaDmlDhcpv6cGetCfg(NULL, &pDhcpc->Cfg);
        }
    }
    
    AnscZeroMemory( pDhcpv6->AliasOfClient, sizeof(pDhcpv6->AliasOfClient) );

    return returnStatus;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Client3_Rollback
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
Client3_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DHCPV6            pDhcpv6           = (PCOSA_DATAMODEL_DHCPV6)g_pCosaBEManager->hDhcpv6;
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtLink          = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPCV6_FULL            pDhcpc            = (PCOSA_DML_DHCPCV6_FULL)pCxtLink->hContext;

    if ( pDhcpv6->AliasOfClient[0] )
        AnscCopyString( pDhcpc->Cfg.Alias, pDhcpv6->AliasOfClient );

    if ( !pCxtLink->bNew )
    {
        CosaDmlDhcpv6cGetCfg( NULL, &pDhcpc->Cfg );
    }
    else
    {
        DHCPV6_CLIENT_SET_DEFAULTVALUE(pDhcpc);
    }
    
    AnscZeroMemory( pDhcpv6->AliasOfClient, sizeof(pDhcpv6->AliasOfClient) );
    
    return returnStatus;
}

/***********************************************************************

 APIs for Object:

    DHCPv6.Client.{i}.Server.{i}.

    *  Server2_GetEntryCount
    *  Server2_GetEntry
    *  Server2_IsUpdated
    *  Server2_Synchronize
    *  Server2_GetParamBoolValue
    *  Server2_GetParamIntValue
    *  Server2_GetParamUlongValue
    *  Server2_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Server2_GetEntryCount
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
Server2_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                       returnStatus    = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtLink        = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)hInsContext;

    return pCxtLink->NumberOfServer;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Server2_GetEntry
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
Server2_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    ANSC_STATUS                       returnStatus    = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtLink        = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)hInsContext;

    *pInsNumber  = nIndex + 1; 

    return (ANSC_HANDLE)&pCxtLink->pServerEntry[nIndex]; /* return the handle */
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Server2_IsUpdated
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
Server2_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtLink        = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)hInsContext;
    BOOL                              bIsUpdated   = TRUE;

    /* 
        We can use one rough granularity interval to get whole table in case 
        that the updating is too frequent.
        */
    if ( ( AnscGetTickInSeconds() - pCxtLink->PreviousVisitTimeOfServer ) < COSA_DML_DHCPV6_ACCESS_INTERVAL_CLIENTSERVER )
    {
        bIsUpdated  = FALSE;
    }
    else
    {
        pCxtLink->PreviousVisitTimeOfServer =  AnscGetTickInSeconds();
        bIsUpdated  = TRUE;
    }

    return bIsUpdated;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Server2_Synchronize
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
Server2_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                       returnStatus    = ANSC_STATUS_SUCCESS;    
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtLink        = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPCV6_SVR             pDhcpcServer    = NULL;
    ULONG                             count           = 0;
    
    if ( pCxtLink->pServerEntry )
    {
        AnscFreeMemory(pCxtLink->pServerEntry);
        pCxtLink->pServerEntry   = NULL;
        pCxtLink->NumberOfServer = 0;
    }

    returnStatus = CosaDmlDhcpv6cGetServerCfg
                    (
                        NULL,
                        pCxtLink->InstanceNumber,
                        &pDhcpcServer,
                        &count
                    );

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        pCxtLink->pServerEntry    = pDhcpcServer;
        pCxtLink->NumberOfServer  = count;
    }

    return returnStatus;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Server2_GetParamBoolValue
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
Server2_GetParamBoolValue
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
        Server2_GetParamIntValue
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
Server2_GetParamIntValue
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
        Server2_GetParamUlongValue
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
Server2_GetParamUlongValue
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
        Server2_GetParamStringValue
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
Server2_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    ANSC_STATUS           returnStatus    = ANSC_STATUS_SUCCESS;    
    PCOSA_DML_DHCPCV6_SVR pDhcpcServer    = (PCOSA_DML_DHCPCV6_SVR)hInsContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "SourceAddress", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pDhcpcServer->SourceAddress) < *pUlSize)
        {
            AnscCopyString(pValue, pDhcpcServer->SourceAddress);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDhcpcServer->SourceAddress)+1;
            return 1;
        }

        return 0;
    }

    if( AnscEqualString(ParamName, "DUID", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pDhcpcServer->DUID) < *pUlSize)
        {
            AnscCopyString(pValue, pDhcpcServer->DUID);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDhcpcServer->DUID)+1;
            return 1;
        }
        return 0;
    }

    if( AnscEqualString(ParamName, "InformationRefreshTime", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pDhcpcServer->InformationRefreshTime) < *pUlSize)
        {
            AnscCopyString(pValue, pDhcpcServer->InformationRefreshTime);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDhcpcServer->InformationRefreshTime)+1;
            return 1;
        }
        return 0;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/***********************************************************************

 APIs for Object:

    DHCPv6.Client.{i}.SentOption.{i}.

    *  SentOption1_GetEntryCount
    *  SentOption1_GetEntry
    *  SentOption1_AddEntry
    *  SentOption1_DelEntry
    *  SentOption1_GetParamBoolValue
    *  SentOption1_GetParamIntValue
    *  SentOption1_GetParamUlongValue
    *  SentOption1_GetParamStringValue
    *  SentOption1_SetParamBoolValue
    *  SentOption1_SetParamIntValue
    *  SentOption1_SetParamUlongValue
    *  SentOption1_SetParamStringValue
    *  SentOption1_Validate
    *  SentOption1_Commit
    *  SentOption1_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        SentOption1_GetEntryCount
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
SentOption1_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtLink          = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPCV6_FULL            pDhcpc            = (PCOSA_DML_DHCPCV6_FULL)pCxtLink->hContext;

    return AnscSListQueryDepth( &pCxtLink->SentOptionList );
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        SentOption1_GetEntry
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
SentOption1_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtDhcpcLink      = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)hInsContext;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink             = NULL;
    PSINGLE_LINK_ENTRY              pSListEntry          = NULL;

    pSListEntry = AnscSListGetEntryByIndex(&pCxtDhcpcLink->SentOptionList, nIndex);

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
        SentOption1_AddEntry
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
SentOption1_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
        ANSC_STATUS                       returnStatus         = ANSC_STATUS_SUCCESS;
        PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtDhcpcLink        = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)hInsContext;
        PCOSA_DML_DHCPCV6_FULL            pDhcpc               = (PCOSA_DML_DHCPCV6_FULL)pCxtDhcpcLink->hContext;
        PCOSA_CONTEXT_LINK_OBJECT         pCxtLink             = NULL;
        PCOSA_DML_DHCPCV6_SENT            pDhcpSentOption      = NULL;
        
        pDhcpSentOption  = (PCOSA_DML_DHCPCV6_SENT)AnscAllocateMemory( sizeof(COSA_DML_DHCPCV6_SENT) );
        if ( !pDhcpSentOption )
        {
            goto EXIT2;
        }
    
        DHCPV6_SENTOPTION_SET_DEFAULTVALUE(pDhcpSentOption);
        
        pCxtLink = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory( sizeof(COSA_CONTEXT_LINK_OBJECT) );
        if ( !pCxtLink )
        {
            goto EXIT1;
        }
    
        pCxtLink->hContext       = (ANSC_HANDLE)pDhcpSentOption;
        pCxtLink->hParentTable   = (ANSC_HANDLE)pCxtDhcpcLink;
        pCxtLink->bNew           = TRUE;
        
        if ( !++pCxtDhcpcLink->maxInstanceOfSent )
        {
            pCxtDhcpcLink->maxInstanceOfSent = 1;
        }
        
        pDhcpSentOption->InstanceNumber = pCxtDhcpcLink->maxInstanceOfSent; 
        pCxtLink->InstanceNumber       = pDhcpSentOption->InstanceNumber;
        *pInsNumber                    = pDhcpSentOption->InstanceNumber;
    
        _ansc_sprintf( pDhcpSentOption->Alias, "SentOption%d", pDhcpSentOption->InstanceNumber);
    
        /* Put into our list */
        CosaSListPushEntryByInsNum(&pCxtDhcpcLink->SentOptionList, (PCOSA_CONTEXT_LINK_OBJECT)pCxtLink);
    
        /* we recreate the configuration */
        CosaDhcpv6RegSetDhcpv6Info((ANSC_HANDLE)g_pCosaBEManager->hDhcpv6);
    
        return (ANSC_HANDLE)pCxtLink;    
           
    EXIT1:
            
        AnscFreeMemory(pDhcpSentOption);
        
    EXIT2:   
            
        return NULL;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        SentOption1_DelEntry
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
SentOption1_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    ANSC_STATUS                       returnStatus         = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtDhcpcLink        = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPCV6_FULL            pDhcpClient          = (PCOSA_DML_DHCPCV6_FULL)pCxtDhcpcLink->hContext;
    PCOSA_CONTEXT_LINK_OBJECT         pCxtLink             = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_DML_DHCPCV6_SENT            pDhcpSentOption      = (PCOSA_DML_DHCPCV6_SENT)pCxtLink->hContext;
    PCOSA_DATAMODEL_DHCPV6            pDhcpv6              = (PCOSA_DATAMODEL_DHCPV6)g_pCosaBEManager->hDhcpv6;

    if ( !pCxtLink->bNew )
    {

        returnStatus = CosaDmlDhcpv6cDelSentOption(NULL, pDhcpClient->Cfg.InstanceNumber, pDhcpSentOption->InstanceNumber);
        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            return returnStatus;
        }
    }

    if ( AnscSListPopEntryByLink(&pCxtDhcpcLink->SentOptionList, &pCxtLink->Linkage) )
    {
        CosaDhcpv6RegSetDhcpv6Info(pDhcpv6);
        
        AnscFreeMemory(pCxtLink->hContext);
        AnscFreeMemory(pCxtLink);
    }

    return returnStatus;    
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        SentOption1_GetParamBoolValue
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
SentOption1_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink             = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPCV6_SENT          pDhcpSentOption      = (PCOSA_DML_DHCPCV6_SENT)pCxtLink->hContext;
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE) )
    {
        /* collect value */
        *pBool  = pDhcpSentOption->bEnabled;

        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        SentOption1_GetParamIntValue
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
SentOption1_GetParamIntValue
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
        SentOption1_GetParamUlongValue
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
SentOption1_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink             = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPCV6_SENT          pDhcpSentOption      = (PCOSA_DML_DHCPCV6_SENT)pCxtLink->hContext;
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Tag", TRUE) )
    {
        /* collect value */
        *puLong = pDhcpSentOption->Tag;

        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        SentOption1_GetParamStringValue
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
SentOption1_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink             = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPCV6_SENT          pDhcpSentOption      = (PCOSA_DML_DHCPCV6_SENT)pCxtLink->hContext;
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pDhcpSentOption->Alias) < *pUlSize)
        {
            AnscCopyString(pValue, pDhcpSentOption->Alias);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDhcpSentOption->Alias)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "Value", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pDhcpSentOption->Value) < *pUlSize)
        {
            AnscCopyString(pValue, pDhcpSentOption->Value);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDhcpSentOption->Value)+1;
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
        SentOption1_SetParamBoolValue
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
SentOption1_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink             = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPCV6_SENT          pDhcpSentOption      = (PCOSA_DML_DHCPCV6_SENT)pCxtLink->hContext;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE) )
    {
        /* save update to backup */
        pDhcpSentOption->bEnabled  = bValue;

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        SentOption1_SetParamIntValue
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
SentOption1_SetParamIntValue
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
        SentOption1_SetParamUlongValue
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
SentOption1_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink             = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPCV6_SENT          pDhcpSentOption      = (PCOSA_DML_DHCPCV6_SENT)pCxtLink->hContext;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Tag", TRUE) )
    {
        /* save update to backup */
        pDhcpSentOption->Tag  = (UCHAR)uValue;

        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        SentOption1_SetParamStringValue
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
SentOption1_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT         pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPCV6_SENT            pDhcpSentOption   = (PCOSA_DML_DHCPCV6_SENT)pCxtLink->hContext;
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtDhcpcLink     = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)pCxtLink->hParentTable;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        /* save update to backup */
        AnscCopyString(pCxtDhcpcLink->AliasOfSent, pDhcpSentOption->Alias);

        AnscCopyString(pDhcpSentOption->Alias, pString);

        return TRUE;
    }

    if( AnscEqualString(ParamName, "Value", TRUE) )
    {
        /* save update to backup */
        AnscCopyString(pDhcpSentOption->Value, pString);

        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        SentOption1_Validate
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
SentOption1_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
        PCOSA_CONTEXT_LINK_OBJECT         pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
        PCOSA_DML_DHCPCV6_SENT            pDhcpSentOption   = (PCOSA_DML_DHCPCV6_SENT)pCxtLink->hContext;
        PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtDhcpcLink     = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)pCxtLink->hParentTable;
        PCOSA_CONTEXT_LINK_OBJECT         pCxtLink2         = NULL;
        PCOSA_DML_DHCPCV6_SENT            pDhcpSentOption2  = NULL;
        PSINGLE_LINK_ENTRY                pSListEntry       = NULL;
        BOOL                              bFound            = FALSE;
    
        /* Parent hasn't set, we don't permit child is set.*/
        if ( pCxtDhcpcLink->bNew )
        {
#if COSA_DHCPV6_ROLLBACK_TEST        
            SentOption1_Rollback(hInsContext);
#endif
            return FALSE;
        }
    
        /* This is for alias */
        if ( TRUE )
        {
            bFound                = FALSE;
            pSListEntry           = AnscSListGetFirstEntry(&pCxtDhcpcLink->SentOptionList);
            while( pSListEntry != NULL)
            {
                pCxtLink2         = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSListEntry);
                pSListEntry       = AnscSListGetNextEntry(pSListEntry);
    
                pDhcpSentOption2  = (PCOSA_DML_DHCPCV6_SENT)pCxtLink2->hContext;
    
                if( DHCPV6_SENDOPTION_ENTRY_MATCH2(pDhcpSentOption->Alias, pDhcpSentOption2->Alias) )
                {
                    if ( (ANSC_HANDLE)pCxtLink2 == hInsContext )
                    {
                        continue;
                    }
    
                    _ansc_strcpy(pReturnParamName, "Alias");
                    *puLength = AnscSizeOfString("Alias");
    
                    bFound = TRUE;
                    
                    break;
                }

                if ( (pDhcpSentOption->bEnabled && pDhcpSentOption2->bEnabled) &&
                     pDhcpSentOption->Tag == pDhcpSentOption2->Tag)
                {
                    if ( (ANSC_HANDLE)pCxtLink2 == hInsContext )
                    {
                        continue;
                    }
    
                    _ansc_strcpy(pReturnParamName, "Tag");
                    *puLength = AnscSizeOfString("Tag");
    
                    bFound = TRUE;
                    
                    break;

                }
                
            }
            
            if ( bFound )
            {
#if COSA_DHCPV6_ROLLBACK_TEST        
                SentOption1_Rollback(hInsContext);
#endif
                return FALSE;
            }
        }
    
        /* For other check */
    
        
        return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        SentOption1_Commit
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
SentOption1_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_LINK_OBJECT         pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPCV6_SENT            pDhcpSentOption   = (PCOSA_DML_DHCPCV6_SENT)pCxtLink->hContext;
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtDhcpcLink     = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)pCxtLink->hParentTable;
    PCOSA_DML_DHCPCV6_FULL            pDhcpClient       = (PCOSA_DML_DHCPCV6_FULL)pCxtDhcpcLink->hContext;
    PCOSA_DATAMODEL_DHCPV6            pDhcpv6           = (PCOSA_DATAMODEL_DHCPV6)g_pCosaBEManager->hDhcpv6;

    if ( pCxtLink->bNew )
    {
        returnStatus = CosaDmlDhcpv6cAddSentOption(NULL, pDhcpClient->Cfg.InstanceNumber, pDhcpSentOption );

        if ( returnStatus == ANSC_STATUS_SUCCESS )
        {
            pCxtLink->bNew = FALSE;

            CosaDhcpv6RegSetDhcpv6Info(pDhcpv6);
        }
        else
        {
            DHCPV6_SENTOPTION_SET_DEFAULTVALUE(pDhcpSentOption);

            if ( pCxtDhcpcLink->AliasOfSent[0] )
                AnscCopyString( pDhcpSentOption->Alias, pCxtDhcpcLink->AliasOfSent );
        }
    }
    else
    {
        returnStatus = CosaDmlDhcpv6cSetSentOption(NULL, pDhcpClient->Cfg.InstanceNumber, pDhcpSentOption);

        if ( returnStatus != ANSC_STATUS_SUCCESS)
        {
            CosaDmlDhcpv6cGetSentOptionbyInsNum(NULL, pDhcpClient->Cfg.InstanceNumber, pDhcpSentOption);
        }
    }
    
    AnscZeroMemory( pCxtDhcpcLink->AliasOfSent, sizeof(pCxtDhcpcLink->AliasOfSent) );
    
    return returnStatus;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        SentOption1_Rollback
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
SentOption1_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_LINK_OBJECT         pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPCV6_SENT            pDhcpSentOption   = (PCOSA_DML_DHCPCV6_SENT)pCxtLink->hContext;
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtDhcpcLink     = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)pCxtLink->hParentTable;
    PCOSA_DML_DHCPCV6_FULL            pDhcpc            = (PCOSA_DML_DHCPCV6_FULL)pCxtDhcpcLink->hContext;

    if ( pCxtDhcpcLink->AliasOfSent[0] )
        AnscCopyString( pDhcpSentOption->Alias, pCxtDhcpcLink->AliasOfSent );

    if ( !pCxtLink->bNew )
    {
        CosaDmlDhcpv6cGetSentOptionbyInsNum(NULL, pDhcpc->Cfg.InstanceNumber, pDhcpSentOption);
    }
    else
    {
        DHCPV6_SENTOPTION_SET_DEFAULTVALUE(pDhcpSentOption);
    }
    
    AnscZeroMemory( pCxtDhcpcLink->AliasOfSent, sizeof(pCxtDhcpcLink->AliasOfSent) );
    
    return returnStatus;
}

/***********************************************************************

 APIs for Object:

    DHCPv6.Client.{i}.ReceivedOption.{i}.

    *  ReceivedOption_GetEntryCount
    *  ReceivedOption_GetEntry
    *  ReceivedOption_IsUpdated
    *  ReceivedOption_Synchronize
    *  ReceivedOption_GetParamBoolValue
    *  ReceivedOption_GetParamIntValue
    *  ReceivedOption_GetParamUlongValue
    *  ReceivedOption_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        ReceivedOption_GetEntryCount
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
ReceivedOption_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                       returnStatus    = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtLink        = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)hInsContext;

    return pCxtLink->NumberOfRecv;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        ReceivedOption_GetEntry
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
ReceivedOption_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    ANSC_STATUS                       returnStatus    = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtLink        = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)hInsContext;

    *pInsNumber  = nIndex + 1; 

    return (ANSC_HANDLE)&pCxtLink->pRecvEntry[nIndex]; /* return the handle */
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        ReceivedOption_IsUpdated
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
ReceivedOption_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtLink        = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)hInsContext;
    BOOL                              bIsUpdated   = TRUE;

    /* 
        We can use one rough granularity interval to get whole table in case 
        that the updating is too frequent.
        */
    if ( ( AnscGetTickInSeconds() - pCxtLink->PreviousVisitTimeOfRecv ) < COSA_DML_DHCPV6_ACCESS_INTERVAL_CLIENTRECV )
    {
        bIsUpdated  = FALSE;
    }
    else
    {
        pCxtLink->PreviousVisitTimeOfRecv =  AnscGetTickInSeconds();
        bIsUpdated  = TRUE;
    }

    return bIsUpdated;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        ReceivedOption_Synchronize
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
ReceivedOption_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                       returnStatus    = ANSC_STATUS_SUCCESS;    
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtLink        = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPCV6_RECV            pDhcpcRecv      = NULL;
    ULONG                             count           = 0;
    
    if ( pCxtLink->pRecvEntry )
    {
        AnscFreeMemory(pCxtLink->pRecvEntry);
        pCxtLink->pRecvEntry   = NULL;
        pCxtLink->NumberOfRecv = 0;
    }

    returnStatus = CosaDmlDhcpv6cGetReceivedOptionCfg
                    (
                        NULL,
                        pCxtLink->InstanceNumber,
                        &pDhcpcRecv,
                        &count
                    );

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        pCxtLink->pRecvEntry    = pDhcpcRecv;
        pCxtLink->NumberOfRecv  = count;
    }

    return returnStatus;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        ReceivedOption_GetParamBoolValue
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
ReceivedOption_GetParamBoolValue
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
        ReceivedOption_GetParamIntValue
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
ReceivedOption_GetParamIntValue
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
        ReceivedOption_GetParamUlongValue
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
ReceivedOption_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    ANSC_STATUS            returnStatus    = ANSC_STATUS_SUCCESS;    
    PCOSA_DML_DHCPCV6_RECV pDhcpcRecv      = (PCOSA_DML_DHCPCV6_RECV)hInsContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Tag", TRUE) )
    {
        /* collect value */
        *puLong  = pDhcpcRecv->Tag;
        
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        ReceivedOption_GetParamStringValue
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
ReceivedOption_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    ANSC_STATUS            returnStatus    = ANSC_STATUS_SUCCESS;    
    PCOSA_DML_DHCPCV6_RECV pDhcpcRecv      = (PCOSA_DML_DHCPCV6_RECV)hInsContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Value", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pDhcpcRecv->Value) < *pUlSize)
        {
            AnscCopyString(pValue, pDhcpcRecv->Value);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDhcpcRecv->Value)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "Server", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pDhcpcRecv->Server) < *pUlSize)
        {
            AnscCopyString(pValue, pDhcpcRecv->Server);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDhcpcRecv->Server)+1;
            return 1;
        }
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/***********************************************************************

 APIs for Object:

    DHCPv6.Server.

    *  Server3_GetParamBoolValue
    *  Server3_GetParamIntValue
    *  Server3_GetParamUlongValue
    *  Server3_GetParamStringValue
    *  Server3_SetParamBoolValue
    *  Server3_SetParamIntValue
    *  Server3_SetParamUlongValue
    *  Server3_SetParamStringValue
    *  Server3_Validate
    *  Server3_Commit
    *  Server3_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Server3_GetParamBoolValue
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
Server3_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_DATAMODEL_DHCPV6          pDhcpv6           = (PCOSA_DATAMODEL_DHCPV6)g_pCosaBEManager->hDhcpv6;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE) )
    {
        /* collect value */
        *pBool  =  CosaDmlDhcpv6sGetState(NULL);
        
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Server3_GetParamIntValue
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
Server3_GetParamIntValue
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
        Server3_GetParamUlongValue
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
Server3_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DHCPV6          pDhcpv6           = (PCOSA_DATAMODEL_DHCPV6)g_pCosaBEManager->hDhcpv6;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "X_CISCO_COM_Type", TRUE) )
    {
        /* collect value */
        *puLong  =  CosaDmlDhcpv6sGetType(NULL);
		
        return TRUE;
    }	
	
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Server3_GetParamStringValue
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
Server3_GetParamStringValue
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

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Server3_SetParamBoolValue
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
Server3_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE) )
    {
        /* save update to backup */
        returnStatus = CosaDmlDhcpv6sEnable((ANSC_HANDLE)NULL, bValue );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
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
        Server3_SetParamIntValue
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
Server3_SetParamIntValue
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
        Server3_SetParamUlongValue
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
Server3_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "X_CISCO_COM_Type", TRUE) )
    {
        /* save update to backup */
        returnStatus = CosaDmlDhcpv6sSetType((ANSC_HANDLE)NULL, uValue );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
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
        Server3_SetParamStringValue
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
Server3_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
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
        Server3_Validate
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
Server3_Validate
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
        Server3_Commit
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
Server3_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Server3_Rollback
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
Server3_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    return 0;
}

/***********************************************************************

 APIs for Object:

    DHCPv6.Server.Pool.{i}.

    *  Pool1_GetEntryCount
    *  Pool1_GetEntry
    *  Pool1_AddEntry
    *  Pool1_DelEntry
    *  Pool1_GetParamBoolValue
    *  Pool1_GetParamIntValue
    *  Pool1_GetParamUlongValue
    *  Pool1_GetParamStringValue
    *  Pool1_SetParamBoolValue
    *  Pool1_SetParamIntValue
    *  Pool1_SetParamUlongValue
    *  Pool1_SetParamStringValue
    *  Pool1_Validate
    *  Pool1_Commit
    *  Pool1_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Pool1_GetEntryCount
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
Pool1_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DHCPV6          pDhcpv6           = (PCOSA_DATAMODEL_DHCPV6)g_pCosaBEManager->hDhcpv6;

    return AnscSListQueryDepth( &pDhcpv6->PoolList );
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Pool1_GetEntry
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
Pool1_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_DHCPV6            pDhcpv6           = (PCOSA_DATAMODEL_DHCPV6)g_pCosaBEManager->hDhcpv6;
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtLink          = NULL;
    PSINGLE_LINK_ENTRY                pSListEntry       = NULL;

    pSListEntry = AnscSListGetEntryByIndex(&pDhcpv6->PoolList, nIndex);

    if ( pSListEntry )
    {
        pCxtLink          = ACCESS_COSA_CONTEXT_POOLV6_LINK_OBJECT(pSListEntry);
        *pInsNumber       = pCxtLink->InstanceNumber;
    }

    return pSListEntry;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Pool1_AddEntry
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
Pool1_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PSINGLE_LINK_ENTRY                pSListEntry       = NULL;
    PCOSA_DATAMODEL_DHCPV6            pDhcpv6           = (PCOSA_DATAMODEL_DHCPV6)g_pCosaBEManager->hDhcpv6;
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtLink          = NULL;
    PCOSA_DML_DHCPSV6_POOL_FULL       pPool             = NULL;
    CHAR                              tmpBuff[64]       = {0};
#ifndef MULTILAN_FEATURE
        /* We just have one Pool. Not permit to add/delete. */
        return NULL;
#endif
#if defined _COSA_DRG_CNS_  || defined _COSA_DRG_TPG_
    return NULL;
#endif
    
    pPool  = (PCOSA_DML_DHCPSV6_POOL_FULL)AnscAllocateMemory( sizeof(COSA_DML_DHCPSV6_POOL_FULL) );
    if ( !pPool )
    {
        goto EXIT2;
    }

    /* Set default value */
    DHCPV6_POOL_SET_DEFAULTVALUE(pPool);

    pCxtLink = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)AnscAllocateMemory( sizeof(COSA_CONTEXT_POOLV6_LINK_OBJECT) );
    if ( !pPool )
    {
        goto EXIT1;
    }
    
    DHCPV6_POOL_INITIATION_CONTEXT(pCxtLink)

    pCxtLink->hContext       = (ANSC_HANDLE)pPool;
    pCxtLink->bNew           = TRUE;

    if ( !++pDhcpv6->maxInstanceOfPool )
    {
        pDhcpv6->maxInstanceOfPool = 1;
    }

    pPool->Cfg.InstanceNumber = pDhcpv6->maxInstanceOfPool;
    pCxtLink->InstanceNumber  = pPool->Cfg.InstanceNumber;
    *pInsNumber               = pPool->Cfg.InstanceNumber;

    _ansc_sprintf( pPool->Cfg.Alias, "Pool%d", pPool->Cfg.InstanceNumber);

    /* Put into our list */
    CosaSListPushEntryByInsNum(&pDhcpv6->PoolList, (PCOSA_CONTEXT_LINK_OBJECT)pCxtLink);

    /* we recreate the configuration */
    CosaDhcpv6RegSetDhcpv6Info(pDhcpv6);

    return (ANSC_HANDLE)pCxtLink;

EXIT1:
    
    AnscFreeMemory(pPool);

EXIT2:        
    
    return NULL; /* return the handle */
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Pool1_DelEntry
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
Pool1_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DHCPV6            pDhcpv6           = (PCOSA_DATAMODEL_DHCPV6)g_pCosaBEManager->hDhcpv6;
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)hInstance;
    PCOSA_DML_DHCPSV6_POOL_FULL       pPool             = (PCOSA_DML_DHCPSV6_POOL_FULL)pCxtLink->hContext;
#ifndef MULTILAN_FEATURE
	/* We just have one Pool. Not permit to add/delete. */
	return ANSC_STATUS_FAILURE;
#endif
#if defined _COSA_DRG_CNS_  || defined _COSA_DRG_TPG_
    return ANSC_STATUS_FAILURE;
#endif

    /* Normally, two sublinks are empty because our framework will firstly 
              call delEntry for them before coming here. We needn't care them.
           */
    if ( !pCxtLink->bNew )
    {
        returnStatus = CosaDmlDhcpv6sDelPool(NULL, pPool->Cfg.InstanceNumber);
        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            return returnStatus;
        }
    }

    if (AnscSListPopEntryByLink(&pDhcpv6->PoolList, &pCxtLink->Linkage) )
    {
        CosaDhcpv6RegSetDhcpv6Info(pDhcpv6);

        AnscFreeMemory(pCxtLink->hContext);
        AnscFreeMemory(pCxtLink);
    }
    
    return returnStatus;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Pool1_GetParamBoolValue
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
Pool1_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV6_POOL_FULL       pDhcpc            = (PCOSA_DML_DHCPSV6_POOL_FULL)pCxtLink->hContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE) )
    {
        /* collect value */
        *pBool   =  pDhcpc->Cfg.bEnabled;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "DUIDExclude", TRUE) )
    {
        /* collect value */
        *pBool   =  pDhcpc->Cfg.DUIDExclude;
        
        return TRUE;
    }

    if( AnscEqualString(ParamName, "VendorClassIDExclude", TRUE) )
    {
        /* collect value */
        *pBool   =  pDhcpc->Cfg.VendorClassIDExclude;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "UserClassIDExclude", TRUE) )
    {
        /* collect value */
        *pBool   =  pDhcpc->Cfg.UserClassIDExclude;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "SourceAddressExclude", TRUE) )
    {
        /* collect value */
        *pBool   =  pDhcpc->Cfg.SourceAddressExclude;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "IANAEnable", TRUE) )
    {
        /* collect value */
        *pBool   =  pDhcpc->Cfg.IANAEnable;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "IAPDEnable", TRUE) )
    {
        /* collect value */
        *pBool   =  pDhcpc->Cfg.IAPDEnable;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_CISCO_RapidEnable", TRUE) )
    {
        /* collect value */
        *pBool   =  pDhcpc->Cfg.RapidEnable;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_CISCO_UnicastEnable", TRUE) )
    {
        /* collect value */
        *pBool   =  pDhcpc->Cfg.UnicastEnable;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_CISCO_EUI64Enable", TRUE) )
    {
        /* collect value */
        *pBool   =  pDhcpc->Cfg.EUI64Enable;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_DNSServersEnabled", TRUE) )
    {
        /* collect value */
        *pBool   =  pDhcpc->Cfg.X_RDKCENTRAL_COM_DNSServersEnabled;

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Pool1_GetParamIntValue
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
Pool1_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV6_POOL_FULL       pPool             = (PCOSA_DML_DHCPSV6_POOL_FULL)pCxtLink->hContext;

    /* check the parameter name and return the corresponding value */

    if( AnscEqualString(ParamName, "LeaseTime", TRUE) )
    {
        /* collect value */
        *pInt  = pPool->Cfg.LeaseTime;

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Pool1_GetParamUlongValue
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
Pool1_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV6_POOL_FULL       pPool             = (PCOSA_DML_DHCPSV6_POOL_FULL)pCxtLink->hContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Status", TRUE) )
    {
        /* collect value */
        CosaDmlDhcpv6sGetPoolInfo( NULL, pPool->Cfg.InstanceNumber, &pPool->Info );

        *puLong  = pPool->Info.Status;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "Order", TRUE) )
    {
        /* collect value */
        *puLong  = pPool->Cfg.Order;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "IAPDAddLength", TRUE) )
    {
        /* collect value */
        *puLong  = pPool->Cfg.IAPDAddLength;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_CISCO_IANAAmount", TRUE) )
    {
        /* collect value */
        *puLong  = pPool->Cfg.IANAAmount;

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Pool1_GetParamStringValue
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
Pool1_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV6_POOL_FULL       pPool             = (PCOSA_DML_DHCPSV6_POOL_FULL)pCxtLink->hContext;
    CHAR                             *pTmpString        = NULL;
    ULONG                             i                 = 0;
    ULONG                             j                 = 0;
    ULONG                             n                 = 0;
    PUCHAR                            pString           = NULL;
    

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pPool->Cfg.Alias) < *pUlSize)
        {
            AnscCopyString(pValue, pPool->Cfg.Alias);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pPool->Cfg.Alias)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "Interface", TRUE) )
    {
        /* collect value */
     #ifdef CISCO_CONFIG_DHCPV6_PREFIX_DELEGATION
        pString = CosaUtilGetFullPathNameByKeyword
                    (
                        "Device.IP.Interface.",
                        "Name",
                        pPool->Cfg.Interface /* When brlan0 works ,change to "brlan0"*/
                    );
     #elif defined (MULTILAN_FEATURE)
        pString = pPool->Cfg.Interface;
     #else
        pString = CosaUtilGetFullPathNameByKeyword
                    (
                        "Device.IP.Interface.",
                        "Name",
                        "brlan0" /* When brlan0 works ,change to "brlan0"*/
                    );
     #endif
     
        if ( pString )
        {
            if ( AnscSizeOfString(pString) < *pUlSize)
            {
                AnscCopyString(pValue, pString);
#if defined (MULTILAN_FEATURE)
#ifdef CISCO_CONFIG_DHCPV6_PREFIX_DELEGATION
                AnscFreeMemory(pString);
#endif
#else
                AnscFreeMemory(pString);
#endif
                return 0;
            }
            else
            {
                *pUlSize = AnscSizeOfString(pString)+1;
#if defined (MULTILAN_FEATURE)
#ifdef CISCO_CONFIG_DHCPV6_PREFIX_DELEGATION
                AnscFreeMemory(pString);
#endif
#else
                AnscFreeMemory(pString);
#endif
                return 1;
            }
        }
        else
        {
            return 0;
        }
    }

    if( AnscEqualString(ParamName, "DUID", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pPool->Cfg.DUID) < *pUlSize)
        {
            AnscCopyString(pValue, pPool->Cfg.DUID);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pPool->Cfg.DUID)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "VendorClassID", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pPool->Cfg.VendorClassID) < *pUlSize)
        {
            AnscCopyString(pValue, pPool->Cfg.VendorClassID);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pPool->Cfg.VendorClassID)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "UserClassID", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pPool->Cfg.UserClassID) < *pUlSize)
        {
            AnscCopyString(pValue, pPool->Cfg.UserClassID);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pPool->Cfg.UserClassID)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "SourceAddress", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pPool->Cfg.SourceAddress) < *pUlSize)
        {
            AnscCopyString(pValue, pPool->Cfg.SourceAddress);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pPool->Cfg.SourceAddress)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "SourceAddressMask", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pPool->Cfg.SourceAddressMask) < *pUlSize)
        {
            AnscCopyString(pValue, pPool->Cfg.SourceAddressMask);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pPool->Cfg.SourceAddressMask)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "IANAManualPrefixes", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pPool->Cfg.IANAManualPrefixes) < *pUlSize)
        {
            AnscCopyString(pValue, pPool->Cfg.IANAManualPrefixes);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pPool->Cfg.IANAManualPrefixes)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "IANAPrefixes", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pPool->Info.IANAPrefixes) < *pUlSize)
        {
            AnscCopyString(pValue, pPool->Info.IANAPrefixes);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pPool->Info.IANAPrefixes)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "IAPDManualPrefixes", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pPool->Cfg.IAPDManualPrefixes) < *pUlSize)
        {
            AnscCopyString(pValue, pPool->Cfg.IAPDManualPrefixes);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pPool->Cfg.IAPDManualPrefixes)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "IAPDPrefixes", TRUE) )
    {
        /* collect value */
#ifdef CISCO_CONFIG_DHCPV6_PREFIX_DELEGATION
	#ifdef _COSA_DRG_CNS_
	        return CosaDmlDhcpv6sGetIAPDPrefixes(&pPool->Cfg, pValue, pUlSize);
	#else
	        return CosaDmlDhcpv6sGetIAPDPrefixes2(&pPool->Cfg, pValue, pUlSize);
	#endif
#else
	#ifdef _COSA_DRG_CNS_
	        return CosaDmlDhcpv6sGetIAPDPrefixes(&pPool->Cfg, pValue, pUlSize);
	#endif
#endif

        if ( AnscSizeOfString(pPool->Info.IAPDPrefixes) < *pUlSize)
        {
            AnscCopyString(pValue, pPool->Info.IAPDPrefixes);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pPool->Info.IAPDPrefixes)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "PrefixRangeBegin", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pPool->Cfg.PrefixRangeBegin) < *pUlSize)
        {
            AnscCopyString(pValue, pPool->Cfg.PrefixRangeBegin);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pPool->Cfg.PrefixRangeBegin)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "PrefixRangeEnd", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pPool->Cfg.PrefixRangeEnd) < *pUlSize)
        {
            AnscCopyString(pValue, pPool->Cfg.PrefixRangeEnd);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pPool->Cfg.PrefixRangeEnd)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "X_CISCO_StartAddress", TRUE) )
    {
        char   addr[64] = {0};

        CosaDmlDhcpv6sGetStartAddress(NULL, addr, sizeof(addr));
        /* collect value */
        if ( AnscSizeOfString(addr) < *pUlSize)
        {
            AnscCopyString(pValue, addr);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(addr)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_DNSServers", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pPool->Cfg.X_RDKCENTRAL_COM_DNSServers) < *pUlSize)
        {
            AnscCopyString(pValue, pPool->Cfg.X_RDKCENTRAL_COM_DNSServers);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pPool->Cfg.X_RDKCENTRAL_COM_DNSServers)+1;
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
        Pool1_SetParamBoolValue
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
Pool1_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV6_POOL_FULL       pPool            = (PCOSA_DML_DHCPSV6_POOL_FULL)pCxtLink->hContext;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE) )
    {
        /* save update to backup */
        pPool->Cfg.bEnabled   = bValue;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "DUIDExclude", TRUE) )
    {
        /* save update to backup */
        pPool->Cfg.DUIDExclude   = bValue;

        /* We just support same mechnism */
        pPool->Cfg.SourceAddressExclude   = bValue;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "VendorClassIDExclude", TRUE) )
    {
        /* save update to backup */
#if defined _COSA_DRG_CNS_  || defined _COSA_DRG_TPG_
        return FALSE;
#endif

        pPool->Cfg.VendorClassIDExclude   = bValue;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "UserClassIDExclude", TRUE) )
    {
        /* save update to backup */
#if defined _COSA_DRG_CNS_  || defined _COSA_DRG_TPG_
        return FALSE;
#endif

        pPool->Cfg.UserClassIDExclude   = bValue;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "SourceAddressExclude", TRUE) )
    {
        /* save update to backup */
        pPool->Cfg.SourceAddressExclude   = bValue;

        pPool->Cfg.DUIDExclude   = bValue;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "IANAEnable", TRUE) )
    {
        /* save update to backup */
        pPool->Cfg.IANAEnable   = bValue;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "IAPDEnable", TRUE) )
    {
        /* save update to backup */
        pPool->Cfg.IAPDEnable   = bValue;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_CISCO_UnicastEnable", TRUE) )
    {
        /* save update to backup */
        pPool->Cfg.UnicastEnable= bValue;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_CISCO_RapidEnable", TRUE) )
    {
        /* save update to backup */
        pPool->Cfg.RapidEnable  = bValue;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_CISCO_EUI64Enable", TRUE) )
    {
        /* save update to backup */
        pPool->Cfg.EUI64Enable = bValue;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_DNSServersEnabled", TRUE) )
    {
        /* save update to backup */
        pPool->Cfg.X_RDKCENTRAL_COM_DNSServersEnabled = bValue;

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Pool1_SetParamIntValue
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
Pool1_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV6_POOL_FULL       pPool             = (PCOSA_DML_DHCPSV6_POOL_FULL)pCxtLink->hContext;

    /* check the parameter name and set the corresponding value */

    if( AnscEqualString(ParamName, "LeaseTime", TRUE) )
    {
        /* save update to backup */
        pPool->Cfg.LeaseTime= iValue;

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Pool1_SetParamUlongValue
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
Pool1_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV6_POOL_FULL       pPool             = (PCOSA_DML_DHCPSV6_POOL_FULL)pCxtLink->hContext;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Order", TRUE) )
    {
        /* save update to backup */
        pPool->Cfg.Order  = uValue;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "IAPDAddLength", TRUE) )
    {
        /* save update to backup */
        pPool->Cfg.IAPDAddLength  = uValue;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_CISCO_IANAAmount", TRUE) )
    {
        /* save update to backup */
        pPool->Cfg.IANAAmount = uValue;

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Pool1_SetParamStringValue
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
Pool1_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DHCPV6            pDhcpv6           = (PCOSA_DATAMODEL_DHCPV6)g_pCosaBEManager->hDhcpv6;
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV6_POOL_FULL       pPool             = (PCOSA_DML_DHCPSV6_POOL_FULL)pCxtLink->hContext;
    PSINGLE_LINK_ENTRY                pSListEntry       = NULL;
    PCOSA_DML_DHCPSV6_POOL_FULL       pPool2            = NULL;
    BOOL                              bFound            = FALSE;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        /* save update to backup */
        AnscCopyString(pDhcpv6->AliasOfPool, pPool->Cfg.Alias);

        AnscCopyString(pPool->Cfg.Alias, pString);

        return TRUE;
    }

    if( AnscEqualString(ParamName, "Interface", TRUE) )
    {
        /* save update to backup */
#if defined _COSA_DRG_CNS_  || defined _COSA_DRG_TPG_
        /*not supported*/
        return FALSE;
#endif
#if defined (MULTILAN_FEATURE)        
        _ansc_snprintf(pPool->Cfg.Interface, sizeof(pPool->Cfg.Interface), "%s.", pString);
#else
        AnscCopyString(pPool->Cfg.Interface, pString);
#endif

        return TRUE;
    }

    if( AnscEqualString(ParamName, "DUID", TRUE) )
    {
        /* save update to backup */
        AnscCopyString(pPool->Cfg.DUID, pString);

        return TRUE;
    }

    if( AnscEqualString(ParamName, "VendorClassID", TRUE) )
    {
        /* save update to backup */
#if defined _COSA_DRG_CNS_  || defined _COSA_DRG_TPG_
        return FALSE;
#endif
        AnscCopyString(pPool->Cfg.VendorClassID, pString);

        return TRUE;
    }

    if( AnscEqualString(ParamName, "UserClassID", TRUE) )
    {
        /* save update to backup */
#if defined _COSA_DRG_CNS_  || defined _COSA_DRG_TPG_
        return FALSE;
#endif
        AnscCopyString(pPool->Cfg.UserClassID, pString);

        return TRUE;
    }

    if( AnscEqualString(ParamName, "IANAManualPrefixes", TRUE) )
    {
        /* save update to backup */
        AnscCopyString(pPool->Cfg.IANAManualPrefixes, pString);
        AnscCopyMemory(pPool->Info.IANAPrefixes, pPool->Cfg.IANAManualPrefixes, sizeof(pPool->Cfg.IANAManualPrefixes) );

        return TRUE;
    }

    if( AnscEqualString(ParamName, "IAPDManualPrefixes", TRUE) )
    {
        /* save update to backup */
        AnscCopyString(pPool->Cfg.IAPDManualPrefixes, pString);

        return TRUE;
    }

    if( AnscEqualString(ParamName, "SourceAddress", TRUE) )
    {
        /* save update to backup */
        AnscCopyString(pPool->Cfg.SourceAddress, pString);

        return TRUE;
    }

    if( AnscEqualString(ParamName, "SourceAddressMask", TRUE) )
    {
        /* save update to backup */
        AnscCopyString(pPool->Cfg.SourceAddressMask, pString);

        return TRUE;
    }
    
    if( AnscEqualString(ParamName, "PrefixRangeBegin", TRUE) )
    {
        /* save update to backup */
        AnscCopyString(pPool->Cfg.PrefixRangeBegin, pString);

        return TRUE;
    }
    
    if( AnscEqualString(ParamName, "PrefixRangeEnd", TRUE) )
    {
        /* save update to backup */
        AnscCopyString(pPool->Cfg.PrefixRangeEnd, pString);

        return TRUE;
    }
    
    if( AnscEqualString(ParamName, "X_CISCO_StartAddress", TRUE) )
    {
        /* save update to backup */
        AnscCopyString(pPool->Cfg.StartAddress, pString);

        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_RDKCENTRAL-COM_DNSServers", TRUE) )
    {
        /* save update to backup */
        AnscCopyString(pPool->Cfg.X_RDKCENTRAL_COM_DNSServers, pString);

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Pool1_Validate
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
Pool1_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
        ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
        PCOSA_DATAMODEL_DHCPV6            pDhcpv6           = (PCOSA_DATAMODEL_DHCPV6)g_pCosaBEManager->hDhcpv6;
        PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)hInsContext;
        PCOSA_DML_DHCPSV6_POOL_FULL       pPool             = (PCOSA_DML_DHCPSV6_POOL_FULL)pCxtLink->hContext;
        PSINGLE_LINK_ENTRY                pSListEntry       = NULL;
        PCOSA_DML_DHCPSV6_POOL_FULL       pPool2            = NULL;
        BOOL                              bFound            = FALSE;
        UCHAR                             strIP[32]         = {'\0'};
        UINT                              ip[4]             = {0};
        int                               rc                = -1;
        int                               i                 = 0;
    
        /*  only for Alias */
        if ( pDhcpv6->AliasOfPool[0] )
        {
            pSListEntry           = AnscSListGetFirstEntry(&pDhcpv6->PoolList);
            while( pSListEntry != NULL)
            {
                pCxtLink          = ACCESS_COSA_CONTEXT_POOLV6_LINK_OBJECT(pSListEntry);
                pSListEntry       = AnscSListGetNextEntry(pSListEntry);
            
                pPool2 = (PCOSA_DML_DHCPSV6_POOL_FULL)pCxtLink->hContext;
            
                if( DHCPV6_POOL_ENTRY_MATCH2(pPool2->Cfg.Alias, pPool->Cfg.Alias) )
                {
                    if ( (ANSC_HANDLE)pCxtLink == hInsContext )
                    {
                        continue;
                    }
    
                    _ansc_strcpy(pReturnParamName, "Alias");
                    
                    bFound = TRUE;
                    
                    break;
                }
            }
            
            if ( bFound )
            {
#if COSA_DHCPV4_ROLLBACK_TEST        
                Pool1_Rollback(hInsContext);
#endif
                return FALSE;
            }
        }
    
        /* some other checking */
    
        return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Pool1_Commit
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
Pool1_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV6_POOL_FULL       pPool             = (PCOSA_DML_DHCPSV6_POOL_FULL)pCxtLink->hContext;
    PCOSA_DATAMODEL_DHCPV6            pDhcpv6           = (PCOSA_DATAMODEL_DHCPV6)g_pCosaBEManager->hDhcpv6;

    if ( pCxtLink->bNew )
    {
        returnStatus = CosaDmlDhcpv6sAddPool(NULL, pPool );

        if ( returnStatus == ANSC_STATUS_SUCCESS )
        {
            pCxtLink->bNew = FALSE;

			/*This table is not saved into psm. All necessary come from syscfg */
            /*CosaDhcpv6RegSetDhcpv6Info(pDhcpv6);*/
        }
        else
        {
            DHCPV6_POOL_SET_DEFAULTVALUE(pPool);
            
            if ( pDhcpv6->AliasOfClient[0] )
                AnscCopyString( pPool->Cfg.Alias, pDhcpv6->AliasOfPool );
        }
    }
    else
    {
        returnStatus = CosaDmlDhcpv6sSetPoolCfg(NULL, &pPool->Cfg);

        if ( returnStatus != ANSC_STATUS_SUCCESS)
        {
            CosaDmlDhcpv6sGetPoolCfg(NULL, &pPool->Cfg);
        }
    }
    
    AnscZeroMemory( pDhcpv6->AliasOfPool, sizeof(pDhcpv6->AliasOfPool) );

    return returnStatus;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Pool1_Rollback
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
Pool1_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DHCPV6            pDhcpv6           = (PCOSA_DATAMODEL_DHCPV6)g_pCosaBEManager->hDhcpv6;
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV6_POOL_FULL       pPool             = (PCOSA_DML_DHCPSV6_POOL_FULL)pCxtLink->hContext;

    if ( pDhcpv6->AliasOfPool[0] )
        AnscCopyString( pPool->Cfg.Alias, pDhcpv6->AliasOfPool );

    if ( !pCxtLink->bNew )
    {
        CosaDmlDhcpv6sGetPoolCfg( NULL, &pPool->Cfg );

        AnscCopyMemory(pPool->Info.IANAPrefixes, pPool->Cfg.IANAManualPrefixes, sizeof(pPool->Info.IANAPrefixes) );
        AnscCopyMemory(pPool->Info.IAPDPrefixes, pPool->Cfg.IAPDManualPrefixes, sizeof(pPool->Info.IAPDPrefixes) );

    }
    else
    {
        DHCPV6_POOL_SET_DEFAULTVALUE(pPool);
    }
    
    AnscZeroMemory( pDhcpv6->AliasOfPool, sizeof(pDhcpv6->AliasOfPool) );
    
    return returnStatus;
}

/***********************************************************************

 APIs for Object:

    DHCPv6.Server.Pool.{i}.Client.{i}.

    *  Client4_GetEntryCount
    *  Client4_GetEntry
    *  Client4_IsUpdated
    *  Client4_Synchronize
    *  Client4_GetParamBoolValue
    *  Client4_GetParamIntValue
    *  Client4_GetParamUlongValue
    *  Client4_GetParamStringValue
    *  Client4_SetParamBoolValue
    *  Client4_SetParamIntValue
    *  Client4_SetParamUlongValue
    *  Client4_SetParamStringValue
    *  Client4_Validate
    *  Client4_Commit
    *  Client4_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Client4_GetEntryCount
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
Client4_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV6_CLIENT          pClient           = pCxtLink->pClientList;

    return pCxtLink->NumberOfClient;


}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Client4_GetEntry
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
Client4_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV6_CLIENT          pClient           = pCxtLink->pClientList;

    if ( nIndex >= pCxtLink->NumberOfClient )
        return NULL;
    
    *pInsNumber  = nIndex + 1; 
    
    return (ANSC_HANDLE)&pCxtLink->pClientList[nIndex];
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Client4_IsUpdated
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
Client4_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtLink        = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)hInsContext;
    BOOL                              bIsUpdated   = TRUE;

    /* 
        We can use one rough granularity interval to get whole table in case 
        that the updating is too frequent.
        */
    if ( ( AnscGetTickInSeconds() - pCxtLink->PreviousVisitTimeOfClient ) < COSA_DML_DHCPV6_ACCESS_INTERVAL_POOLCLIENT )
    {
        bIsUpdated  = FALSE;
    }
    else
    {
        pCxtLink->PreviousVisitTimeOfClient =  AnscGetTickInSeconds();
        bIsUpdated  = TRUE;
    }

    return bIsUpdated;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Client4_Synchronize
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
Client4_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                       returnStatus    = ANSC_STATUS_SUCCESS;    
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtLink        = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV6_CLIENT          pDhcpsClient    = NULL;
    PCOSA_DML_DHCPSV6_CLIENTCONTENT   pClientContentList = NULL;
    ULONG                             uIndex           = 0;
    ULONG                             count           = 0;

    /* Free all */
    if ( pCxtLink->pClientList )
    {
        AnscFreeMemory(pCxtLink->pClientList);
        pCxtLink->pClientList    = NULL;

        pClientContentList = pCxtLink->pClientContentList;
        for( uIndex =0; uIndex < pCxtLink->NumberOfClient; uIndex++ )
        {
            if ( pClientContentList[uIndex].pIPv6Address )
            {
                AnscFreeMemory(pClientContentList[uIndex].pIPv6Address);
                pClientContentList[uIndex].pIPv6Address = NULL;
            }
            
            if ( pClientContentList[uIndex].pIPv6Prefix )
            {
                AnscFreeMemory(pClientContentList[uIndex].pIPv6Prefix);
                pClientContentList[uIndex].pIPv6Prefix = NULL;
            }
            
            if ( pClientContentList[uIndex].pOption )
            {
                AnscFreeMemory(pClientContentList[uIndex].pOption);            
                pClientContentList[uIndex].pOption = NULL;
            }
        }
        
        AnscFreeMemory(pCxtLink->pClientContentList);
        pCxtLink->pClientContentList    = NULL;

        pCxtLink->NumberOfClient = 0;

    }

    /* Get all again */
    returnStatus = CosaDmlDhcpv6sGetClient
                    (
                        NULL,
                        pCxtLink->InstanceNumber,
                        &pDhcpsClient,
                        &count
                    );

    if ( (returnStatus == ANSC_STATUS_SUCCESS) && count)
    {
        pCxtLink->pClientList     = pDhcpsClient;
        pCxtLink->NumberOfClient  = count;

        pCxtLink->pClientContentList = (PCOSA_DML_DHCPSV6_CLIENTCONTENT)AnscAllocateMemory( count * sizeof(COSA_DML_DHCPSV6_CLIENTCONTENT) );
        if (  !pCxtLink->pClientContentList )
        {
            CcspTraceWarning(( "Client4_Synchronize -- AnscAllocateMemory error.\n"));
        }

        pClientContentList = pCxtLink->pClientContentList;
        for( uIndex =0; uIndex < pCxtLink->NumberOfClient; uIndex++ )
        {
            returnStatus = CosaDmlDhcpv6sGetIPv6Address
                            (
                                NULL,
                                pCxtLink->InstanceNumber,
                                uIndex,
                                &pClientContentList[uIndex].pIPv6Address,
                                &pClientContentList[uIndex].NumberofIPv6Address
                            );

            if ( returnStatus )
            {
                CcspTraceWarning(( "Client4_Synchronize -- CosaDmlDhcpv6sGetIPv6Address() error %d.\n", returnStatus));
            }
            
            returnStatus = CosaDmlDhcpv6sGetIPv6Prefix
                            (
                                NULL,
                                pCxtLink->InstanceNumber,
                                uIndex,
                                &pClientContentList[uIndex].pIPv6Prefix,
                                &pClientContentList[uIndex].NumberofIPv6Prefix
                            );
            
            if ( returnStatus )
            {
                CcspTraceWarning(( "Client4_Synchronize -- CosaDmlDhcpv6sGetIPv6Prefix() error %d.\n", returnStatus));
            }

            returnStatus = CosaDmlDhcpv6sGetIPv6Option
                            (
                                NULL,
                                pCxtLink->InstanceNumber,
                                uIndex,
                                &pClientContentList[uIndex].pOption,
                                &pClientContentList[uIndex].NumberofOption
                            );
            
            if ( returnStatus )
            {
                CcspTraceWarning(( "Client4_Synchronize -- CosaDmlDhcpv6sGetIPv6Option() error %d.\n", returnStatus));
            }
            
        }

    }
    else /*RDKB-6736, CID-33320, free unsed resource before return*/
    {
        if(pDhcpsClient)
        {
            AnscFreeMemory(pDhcpsClient);
        }
    }

    return returnStatus;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Client4_GetParamBoolValue
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
Client4_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;    
    PCOSA_DML_DHCPSV6_CLIENT        pDhcpsClient    = (PCOSA_DML_DHCPSV6_CLIENT)hInsContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Active", TRUE) )
    {
        /* collect value */
        returnStatus = CosaDmlDhcpv6sPing(pDhcpsClient);
        if ( returnStatus == ANSC_STATUS_SUCCESS )
        {
            *pBool = TRUE;
        }
        else
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
        Client4_GetParamIntValue
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
Client4_GetParamIntValue
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
        Client4_GetParamUlongValue
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
Client4_GetParamUlongValue
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
        Client4_GetParamStringValue
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
Client4_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;    
    PCOSA_DML_DHCPSV6_CLIENT        pDhcpsClient    = (PCOSA_DML_DHCPSV6_CLIENT)hInsContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pDhcpsClient->Alias) < *pUlSize)
        {
            AnscCopyString(pValue, pDhcpsClient->Alias);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDhcpsClient->Alias)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "SourceAddress", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pDhcpsClient->SourceAddress) < *pUlSize)
        {
            AnscCopyString(pValue, pDhcpsClient->SourceAddress);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDhcpsClient->SourceAddress)+1;
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
        Client4_SetParamBoolValue
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
Client4_SetParamBoolValue
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
        Client4_SetParamIntValue
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
Client4_SetParamIntValue
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
        Client4_SetParamUlongValue
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
Client4_SetParamUlongValue
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
        Client4_SetParamStringValue
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
Client4_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;    
    PCOSA_DML_DHCPSV6_CLIENT        pDhcpsClient    = (PCOSA_DML_DHCPSV6_CLIENT)hInsContext;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        /* save update to backup */
        AnscCopyString(pDhcpsClient->Alias, pString);

        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Client4_Validate
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
Client4_Validate
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
        Client4_Commit
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
Client4_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Client4_Rollback
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
Client4_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    return 0;
}

/***********************************************************************

 APIs for Object:

    DHCPv6.Server.Pool.{i}.Client.{i}.IPv6Address.{i}.

    *  IPv6Address2_GetEntryCount
    *  IPv6Address2_GetEntry
    *  IPv6Address2_IsUpdated
    *  IPv6Address2_Synchronize
    *  IPv6Address2_GetParamBoolValue
    *  IPv6Address2_GetParamIntValue
    *  IPv6Address2_GetParamUlongValue
    *  IPv6Address2_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv6Address2_GetEntryCount
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
IPv6Address2_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DML_DHCPSV6_CLIENTCONTENT   PClientContent    = NULL;
    
    PClientContent = (PCOSA_DML_DHCPSV6_CLIENTCONTENT)CosaDhcpv6GetClientContentbyClient(hInsContext);

    if ( PClientContent )
        return PClientContent->NumberofIPv6Address;
    else
        return 0;    
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        IPv6Address2_GetEntry
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
IPv6Address2_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DML_DHCPSV6_CLIENTCONTENT   PClientContent    = NULL;
    
    PClientContent = (PCOSA_DML_DHCPSV6_CLIENTCONTENT)CosaDhcpv6GetClientContentbyClient(hInsContext);

    if ( PClientContent )
    {
        *pInsNumber  = nIndex + 1; 
        return &PClientContent->pIPv6Address[nIndex];
    }
    else
        return NULL;    

}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Address2_GetParamBoolValue
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
IPv6Address2_GetParamBoolValue
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
        IPv6Address2_GetParamIntValue
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
IPv6Address2_GetParamIntValue
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
        IPv6Address2_GetParamUlongValue
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
IPv6Address2_GetParamUlongValue
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
        IPv6Address2_GetParamStringValue
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
IPv6Address2_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    ANSC_STATUS                            returnStatus    = ANSC_STATUS_SUCCESS;
    PCOSA_DML_DHCPSV6_CLIENT_IPV6ADDRESS   pIPv6Address    = (PCOSA_DML_DHCPSV6_CLIENT_IPV6ADDRESS)hInsContext;
   
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "IPAddress", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pIPv6Address->IPAddress) < *pUlSize)
        {
            AnscCopyString(pValue, pIPv6Address->IPAddress);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pIPv6Address->IPAddress)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "PreferredLifetime", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pIPv6Address->PreferredLifetime) < *pUlSize)
        {
            AnscCopyString(pValue, pIPv6Address->PreferredLifetime);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pIPv6Address->PreferredLifetime)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "ValidLifetime", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pIPv6Address->ValidLifetime) < *pUlSize)
        {
            AnscCopyString(pValue, pIPv6Address->ValidLifetime);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pIPv6Address->ValidLifetime)+1;
            return 1;
        }
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/***********************************************************************

 APIs for Object:

    DHCPv6.Server.Pool.{i}.Client.{i}.IPv6Prefix.{i}.

    *  IPv6Prefix1_GetEntryCount
    *  IPv6Prefix1_GetEntry
    *  IPv6Prefix1_IsUpdated
    *  IPv6Prefix1_Synchronize
    *  IPv6Prefix1_GetParamBoolValue
    *  IPv6Prefix1_GetParamIntValue
    *  IPv6Prefix1_GetParamUlongValue
    *  IPv6Prefix1_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv6Prefix1_GetEntryCount
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
IPv6Prefix1_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DML_DHCPSV6_CLIENTCONTENT   PClientContent    = NULL;
    
    PClientContent = (PCOSA_DML_DHCPSV6_CLIENTCONTENT)CosaDhcpv6GetClientContentbyClient(hInsContext);

    if ( PClientContent )
        return PClientContent->NumberofIPv6Prefix;
    else
        return 0;    
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        IPv6Prefix1_GetEntry
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
IPv6Prefix1_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DML_DHCPSV6_CLIENTCONTENT   PClientContent    = NULL;
    
    PClientContent = (PCOSA_DML_DHCPSV6_CLIENTCONTENT)CosaDhcpv6GetClientContentbyClient(hInsContext);

    if ( PClientContent )
    {
        *pInsNumber  = nIndex + 1; 
        return &PClientContent->pIPv6Prefix[nIndex];
    }
    else
        return NULL;    
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Prefix1_GetParamBoolValue
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
IPv6Prefix1_GetParamBoolValue
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
        IPv6Prefix1_GetParamIntValue
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
IPv6Prefix1_GetParamIntValue
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
        IPv6Prefix1_GetParamUlongValue
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
IPv6Prefix1_GetParamUlongValue
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
        IPv6Prefix1_GetParamStringValue
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
IPv6Prefix1_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    ANSC_STATUS                            returnStatus    = ANSC_STATUS_SUCCESS;
    PCOSA_DML_DHCPSV6_CLIENT_IPV6PREFIX    pIPv6Prefix    = (PCOSA_DML_DHCPSV6_CLIENT_IPV6PREFIX)hInsContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Prefix", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pIPv6Prefix->Prefix) < *pUlSize)
        {
            AnscCopyString(pValue, pIPv6Prefix->Prefix);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pIPv6Prefix->Prefix)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "PreferredLifetime", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pIPv6Prefix->PreferredLifetime) < *pUlSize)
        {
            AnscCopyString(pValue, pIPv6Prefix->PreferredLifetime);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pIPv6Prefix->PreferredLifetime)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "ValidLifetime", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pIPv6Prefix->ValidLifetime) < *pUlSize)
        {
            AnscCopyString(pValue, pIPv6Prefix->ValidLifetime);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pIPv6Prefix->ValidLifetime)+1;
            return 1;
        }
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/***********************************************************************

 APIs for Object:

    DHCPv6.Server.Pool.{i}.Client.{i}.Option.{i}.

    *  Option3_GetEntryCount
    *  Option3_GetEntry
    *  Option3_IsUpdated
    *  Option3_Synchronize
    *  Option3_GetParamBoolValue
    *  Option3_GetParamIntValue
    *  Option3_GetParamUlongValue
    *  Option3_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Option3_GetEntryCount
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
Option3_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DML_DHCPSV6_CLIENTCONTENT   PClientContent    = NULL;
    
    PClientContent = CosaDhcpv6GetClientContentbyClient(hInsContext);

    if ( PClientContent )
        return PClientContent->NumberofOption;
    else
        return 0;    
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Option3_GetEntry
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
Option3_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DML_DHCPSV6_CLIENTCONTENT   PClientContent    = NULL;
    
    PClientContent = CosaDhcpv6GetClientContentbyClient(hInsContext);

    if ( PClientContent )
    {
        *pInsNumber  = nIndex + 1; 
        return &PClientContent->pOption[nIndex];
    }
    else
        return NULL;    
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Option3_GetParamBoolValue
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
Option3_GetParamBoolValue
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
        Option3_GetParamIntValue
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
Option3_GetParamIntValue
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
        Option3_GetParamUlongValue
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
Option3_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    ANSC_STATUS                            returnStatus    = ANSC_STATUS_SUCCESS;
    PCOSA_DML_DHCPSV6_CLIENT_OPTION        pIPv6Option     = (PCOSA_DML_DHCPSV6_CLIENT_OPTION)hInsContext;


    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Tag", TRUE) )
    {
        /* collect value */
        *puLong  = pIPv6Option->Tag;
        
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Option3_GetParamStringValue
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
Option3_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    ANSC_STATUS                            returnStatus    = ANSC_STATUS_SUCCESS;
    PCOSA_DML_DHCPSV6_CLIENT_OPTION        pIPv6Option     = (PCOSA_DML_DHCPSV6_CLIENT_OPTION)hInsContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Value", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pIPv6Option->Value) < *pUlSize)
        {
            AnscCopyString(pValue, pIPv6Option->Value);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pIPv6Option->Value)+1;
            return 1;
        }
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/***********************************************************************

 APIs for Object:

    DHCPv6.Server.Pool.{i}.Option.{i}.

    *  Option4_GetEntryCount
    *  Option4_GetEntry
    *  Option4_AddEntry
    *  Option4_DelEntry
    *  Option4_GetParamBoolValue
    *  Option4_GetParamIntValue
    *  Option4_GetParamUlongValue
    *  Option4_GetParamStringValue
    *  Option4_SetParamBoolValue
    *  Option4_SetParamIntValue
    *  Option4_SetParamUlongValue
    *  Option4_SetParamStringValue
    *  Option4_Validate
    *  Option4_Commit
    *  Option4_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Option4_GetEntryCount
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
Option4_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)hInsContext;

    return AnscSListQueryDepth( &pCxtLink->OptionList );
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Option4_GetEntry
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
Option4_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtPoolLink         = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)hInsContext;
    PCOSA_CONTEXT_LINK_OBJECT         pCxtLink             = NULL;
    PSINGLE_LINK_ENTRY                pSListEntry          = NULL;

    pSListEntry = AnscSListGetEntryByIndex(&pCxtPoolLink->OptionList, nIndex);

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
        Option4_AddEntry
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
Option4_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    ANSC_STATUS                       returnStatus         = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtPoolLink         = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV6_POOL_FULL       pPool                = (PCOSA_DML_DHCPSV6_POOL_FULL)pCxtPoolLink->hContext;
    PCOSA_CONTEXT_LINK_OBJECT         pCxtLink             = NULL;
    PCOSA_DML_DHCPSV6_POOL_OPTION     pDhcpOption          = NULL;
    CHAR                              tmpBuff[64]          = {0};
    PCOSA_DATAMODEL_DHCPV6            pDhcpv6              = (PCOSA_DATAMODEL_DHCPV6)g_pCosaBEManager->hDhcpv6;
#ifndef MULTILAN_FEATURE
	/* We just have two option:DNS, domain. Not permit to add/delete. */
	return NULL;
#endif
    /* We need ask from backend */
    pDhcpOption  = (PCOSA_DML_DHCPSV6_POOL_OPTION)AnscAllocateMemory( sizeof(COSA_DML_DHCPSV6_POOL_OPTION) );
    if ( !pDhcpOption )
    {
        goto EXIT2;
    }

    DHCPV6_POOLOPTION_SET_DEFAULTVALUE(pDhcpOption);

    pCxtLink = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory( sizeof(COSA_CONTEXT_LINK_OBJECT) );
    if ( !pCxtLink )
    {
        goto EXIT1;
    }

    pCxtLink->hContext       = (ANSC_HANDLE)pDhcpOption;
    pCxtLink->hParentTable   = (ANSC_HANDLE)pCxtPoolLink;
    pCxtLink->bNew           = TRUE;
    
    if ( !++pCxtPoolLink->maxInstanceOfOption )
    {
        pCxtPoolLink->maxInstanceOfOption = 1;
    }
    pDhcpOption->InstanceNumber = pCxtPoolLink->maxInstanceOfOption;
    pCxtLink->InstanceNumber           = pDhcpOption->InstanceNumber; 
    *pInsNumber                        = pDhcpOption->InstanceNumber;

    _ansc_sprintf( pDhcpOption->Alias, "Option%d", pDhcpOption->InstanceNumber);

    /* Put into our list */
    CosaSListPushEntryByInsNum(&pCxtPoolLink->OptionList, (PCOSA_CONTEXT_LINK_OBJECT)pCxtLink);

    /* we recreate the configuration */
    CosaDhcpv6RegSetDhcpv6Info(pDhcpv6);

    return (ANSC_HANDLE)pCxtLink;    

EXIT1:
        
    AnscFreeMemory(pDhcpOption);
    
EXIT2:   
        
    return NULL;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Option4_DelEntry
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
Option4_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    ANSC_STATUS                       returnStatus         = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtPoolLink         = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV6_POOL_FULL       pDhcpPool           = (PCOSA_DML_DHCPSV6_POOL_FULL)pCxtPoolLink->hContext;
    PCOSA_CONTEXT_LINK_OBJECT         pCxtLink             = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_DML_DHCPSV6_POOL_OPTION     pDhcpOption          = (PCOSA_DML_DHCPSV6_POOL_OPTION)pCxtLink->hContext;
    PCOSA_DATAMODEL_DHCPV6            pDhcpv6              = (PCOSA_DATAMODEL_DHCPV6)g_pCosaBEManager->hDhcpv6;
#ifndef MULTILAN_FEATURE
	/* We just have two option:DNS, domain. Not permit to add/delete. */
	return ANSC_STATUS_FAILURE;
#endif
    if ( !pCxtLink->bNew )
    {
        returnStatus = CosaDmlDhcpv6sDelOption( NULL, pDhcpPool->Cfg.InstanceNumber, pDhcpOption->InstanceNumber );
        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            return returnStatus;
        }
    }

    /* Firstly we del this entry */
    if ( AnscSListPopEntryByLink(&pCxtPoolLink->OptionList, &pCxtLink->Linkage) )
    {
        CosaDhcpv6RegSetDhcpv6Info(pDhcpv6);
        AnscFreeMemory(pCxtLink->hContext);
        AnscFreeMemory(pCxtLink);
    }
    
    return returnStatus;
        
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Option4_GetParamBoolValue
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
Option4_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV6_POOL_OPTION   pDhcpOption   = (PCOSA_DML_DHCPSV6_POOL_OPTION)pCxtLink->hContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* collect value */
        *pBool  =  pDhcpOption->bEnabled;
        
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Option4_GetParamIntValue
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
Option4_GetParamIntValue
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
        Option4_GetParamUlongValue
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
Option4_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV6_POOL_OPTION   pDhcpOption   = (PCOSA_DML_DHCPSV6_POOL_OPTION)pCxtLink->hContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Tag", TRUE) )
    {
        /* collect value */
        *puLong    =  pDhcpOption->Tag;

        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Option4_GetParamStringValue
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
Option4_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV6_POOL_OPTION   pDhcpOption   = (PCOSA_DML_DHCPSV6_POOL_OPTION)pCxtLink->hContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pDhcpOption->Alias) < *pUlSize)
        {
            AnscCopyString(pValue, pDhcpOption->Alias);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDhcpOption->Alias)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "Value", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pDhcpOption->Value) < *pUlSize)
        {
            AnscCopyString(pValue, pDhcpOption->Value);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDhcpOption->Value)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "PassthroughClient", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pDhcpOption->PassthroughClient) < *pUlSize)
        {
            AnscCopyString(pValue, pDhcpOption->PassthroughClient);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDhcpOption->PassthroughClient)+1;
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
        Option4_SetParamBoolValue
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
Option4_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT         pCxtLink        = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV6_POOL_OPTION     pDhcpOption     = (PCOSA_DML_DHCPSV6_POOL_OPTION)pCxtLink->hContext;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* save update to backup */
        pDhcpOption->bEnabled  = bValue;
        
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Option4_SetParamIntValue
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
Option4_SetParamIntValue
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
        Option4_SetParamUlongValue
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
Option4_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV6_POOL_OPTION   pDhcpOption   = (PCOSA_DML_DHCPSV6_POOL_OPTION)pCxtLink->hContext;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Tag", TRUE) )
    {
        /* save update to backup */
        if ( !tagPermitted( uValue ) )
        {
            return FALSE;
        }
        
        pDhcpOption->Tag =  uValue;

        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Option4_SetParamStringValue
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
Option4_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT         pCxtLink             = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV6_POOL_OPTION     pDhcpOption          = (PCOSA_DML_DHCPSV6_POOL_OPTION)pCxtLink->hContext;
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pPoolLink            = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)pCxtLink->hParentTable;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        AnscCopyString(pPoolLink->AliasOfOption, pDhcpOption->Alias);

        AnscCopyString(pDhcpOption->Alias, pString);

        return TRUE;
    }

    if( AnscEqualString(ParamName, "Value", TRUE) )
    {
        /* save update to backup */
        AnscCopyString(pDhcpOption->Value,pString);

        return TRUE;
    }

    if( AnscEqualString(ParamName, "PassthroughClient", TRUE) )
    {
#if defined _COSA_DRG_CNS_  || defined _COSA_DRG_TPG_
        /*not supported*/
        return FALSE;
#endif

        /* save update to backup */
        AnscCopyString(pDhcpOption->PassthroughClient,pString);

        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Option4_Validate
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
Option4_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_CONTEXT_LINK_OBJECT         pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV6_POOL_OPTION     pDhcpPoolOption   = (PCOSA_DML_DHCPSV6_POOL_OPTION)pCxtLink->hContext;
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtPoolLink      = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)pCxtLink->hParentTable;
    PCOSA_DML_DHCPSV6_POOL_FULL       pPool             = (PCOSA_DML_DHCPSV6_POOL_FULL)pCxtPoolLink->hContext;
    PCOSA_CONTEXT_LINK_OBJECT         pCxtLink2         = NULL;
    PCOSA_DML_DHCPSV6_POOL_OPTION     pDhcpPoolOption2  = NULL;
    PSINGLE_LINK_ENTRY                pSListEntry       = NULL;
    BOOL                              bFound            = FALSE;

    /* Parent hasn't set, we don't permit child is set.*/
    if ( pCxtPoolLink->bNew )
    {
#if COSA_DHCPV6_ROLLBACK_TEST        
        Option4_Rollback(hInsContext);
#endif
        return FALSE;
    }

    if ( 1 )
    {
        /* save update to backup */
        bFound                = FALSE;
        pSListEntry           = AnscSListGetFirstEntry(&pCxtPoolLink->OptionList);
        while( pSListEntry != NULL)
        {
            pCxtLink2         = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSListEntry);
            pSListEntry       = AnscSListGetNextEntry(pSListEntry);

            pDhcpPoolOption2  = (PCOSA_DML_DHCPSV6_POOL_OPTION)pCxtLink2->hContext;

            if( DHCPV6_POOLOPTION_ENTRY_MATCH2(pDhcpPoolOption->Alias, pDhcpPoolOption2->Alias )) 
            {
                if ( (ANSC_HANDLE)pCxtLink2 == hInsContext )
                {
                    continue;
                }

                _ansc_strcpy(pReturnParamName, "Alias");

                bFound = TRUE;

                break;
            }

            if ( (pDhcpPoolOption->bEnabled && pDhcpPoolOption2->bEnabled) &&
                  ( pDhcpPoolOption->Tag == pDhcpPoolOption2->Tag ) )
            {
                if ( (ANSC_HANDLE)pCxtLink2 == hInsContext )
                {
                    continue;
                }

                _ansc_strcpy(pReturnParamName, "Tag");

                bFound = TRUE;

                break;
            }
        }

        if ( bFound )
        {
#if COSA_DHCPV6_ROLLBACK_TEST
            Option4_Rollback(hInsContext);
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
        Option4_Commit
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
Option4_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                       returnStatus         = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_LINK_OBJECT         pCxtLink             = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV6_POOL_OPTION     pDhcpOption          = (PCOSA_DML_DHCPSV6_POOL_OPTION)pCxtLink->hContext;
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtPoolLink         = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)pCxtLink->hParentTable;
    PCOSA_DML_DHCPSV6_POOL_FULL       pPool                = (PCOSA_DML_DHCPSV6_POOL_FULL)pCxtPoolLink->hContext;
    PCOSA_DATAMODEL_DHCPV6            pDhcpv6              = (PCOSA_DATAMODEL_DHCPV6)g_pCosaBEManager->hDhcpv6;

    if ( pCxtLink->bNew )
    {
        returnStatus = CosaDmlDhcpv6sAddOption(NULL, pPool->Cfg.InstanceNumber, pDhcpOption );

        if ( returnStatus == ANSC_STATUS_SUCCESS )
        {
            pCxtLink->bNew = FALSE;

			/*This table is not saved into psm. All necessary come from syscfg */
            /*CosaDhcpv6RegSetDhcpv6Info(pDhcpv6);*/
        }
        else
        {
            DHCPV6_POOLOPTION_SET_DEFAULTVALUE(pDhcpOption);

            if ( pCxtPoolLink->AliasOfOption[0] )
                AnscCopyString( pDhcpOption->Alias, pCxtPoolLink->AliasOfOption );
        }
    }
    else
    {
        returnStatus = CosaDmlDhcpv6sSetOption(NULL, pPool->Cfg.InstanceNumber, pDhcpOption);

        if ( returnStatus != ANSC_STATUS_SUCCESS)
        {
            CosaDmlDhcpv6sGetOptionbyInsNum(NULL, pPool->Cfg.InstanceNumber, pDhcpOption);
        }
    }
    
    AnscZeroMemory( pCxtPoolLink->AliasOfOption, sizeof(pCxtPoolLink->AliasOfOption) );
    
    return returnStatus;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Option4_Rollback
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
Option4_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_LINK_OBJECT         pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV6_POOL_OPTION     pDhcpPoolOption   = (PCOSA_DML_DHCPSV6_POOL_OPTION)pCxtLink->hContext;
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtPoolLink      = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)pCxtLink->hParentTable;
    PCOSA_DML_DHCPSV6_POOL_FULL       pPool             = (PCOSA_DML_DHCPSV6_POOL_FULL)pCxtPoolLink->hContext;

    if ( pCxtPoolLink->AliasOfOption[0] )
        AnscCopyString( pDhcpPoolOption->Alias, pCxtPoolLink->AliasOfOption );

    if ( !pCxtLink->bNew )
    {
        CosaDmlDhcpv6sGetOptionbyInsNum(NULL, pPool->Cfg.InstanceNumber, pDhcpPoolOption);
    }
    else
    {
        DHCPV6_POOLOPTION_SET_DEFAULTVALUE(pDhcpPoolOption);
    }

    AnscZeroMemory( pCxtPoolLink->AliasOfOption, sizeof(pCxtPoolLink->AliasOfOption) );
    
    return returnStatus;
}

