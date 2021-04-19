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
#include "ansc_string_util.h"
#include "safec_lib_common.h"
#include "cosa_drg_common.h"

#define MIN 60
#define HOURS 3600
#define DAYS 86400
#define WEEKS 604800
#define MINSECS 120
#define MAXSECS 999
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

static int update_pValue (char *pValue, PULONG pulSize, char *str)
{
    if (!str)
        return -1;

    size_t len = strlen(str);

    if (len < *pulSize)
    {
        memcpy(pValue, str, len + 1);
        return 0;
    }

    *pulSize = len + 1;
    return 1;
}

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
#ifndef FEATURE_RDKB_WAN_MANAGER
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pBool);
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pInt);
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(puLong);
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pValue);
    UNREFERENCED_PARAMETER(pUlSize);
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DHCPV6          pDhcpv6           = (PCOSA_DATAMODEL_DHCPV6)g_pCosaBEManager->hDhcpv6;
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtLink        = NULL;
    PCOSA_DML_DHCPCV6_FULL            pDhcpc          = NULL;
    errno_t                           rc              = -1;

    
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

    rc = sprintf_s( (char*)pDhcpc->Cfg.Alias,sizeof(pDhcpc->Cfg.Alias),"Client%lu", pDhcpc->Cfg.InstanceNumber);
    if(rc < EOK)
    {
      ERR_CHK(rc);
      AnscFreeMemory(pCxtLink);
      goto EXIT1;
    }

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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(hInstance);
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DHCPV6          pDhcpv6           = (PCOSA_DATAMODEL_DHCPV6)g_pCosaBEManager->hDhcpv6;
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtLink        = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)hInstance;
    PCOSA_DML_DHCPCV6_FULL          pDhcpc            = (PCOSA_DML_DHCPCV6_FULL)pCxtLink->hContext;

    /* Normally, two sublinks are empty because our framework will firstly 
            call delEntry for them before coming here. We needn't care them.
         */


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
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtLink        = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPCV6_FULL            pDhcpc          = (PCOSA_DML_DHCPCV6_FULL)pCxtLink->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        /**pBool   = pDhcpc->Cfg.bEnabled;*/
        *pBool   =CosaDmlDhcpv6cGetEnabled(NULL);

        return TRUE;
    }

    if (strcmp(ParamName, "RequestAddresses") == 0)
    {
        /* collect value */
        *pBool   = pDhcpc->Cfg.RequestAddresses;

        return TRUE;
    }

    if (strcmp(ParamName, "RequestPrefixes") == 0)
    {
        /* collect value */
        *pBool   = pDhcpc->Cfg.RequestPrefixes;

        return TRUE;
    }

    if (strcmp(ParamName, "RapidCommit") == 0)
    {
        /* collect value */
        *pBool   = pDhcpc->Cfg.RapidCommit;

        return TRUE;
    }

    if (strcmp(ParamName, "Renew") == 0)
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
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtLink          = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPCV6_FULL            pDhcpc            = (PCOSA_DML_DHCPCV6_FULL)pCxtLink->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "SuggestedT1") == 0)
    {
        /* collect value */        
        *pInt   = pDhcpc->Cfg.SuggestedT1;
        
        return TRUE;
    }

    if (strcmp(ParamName, "SuggestedT2") == 0)
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
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtLink          = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPCV6_FULL            pDhcpc            = (PCOSA_DML_DHCPCV6_FULL)pCxtLink->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Status") == 0)
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
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtLink          = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPCV6_FULL            pDhcpc            = (PCOSA_DML_DHCPCV6_FULL)pCxtLink->hContext;
    PUCHAR                          pString           = NULL;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* collect value */
        return  update_pValue(pValue,pUlSize, (char*)pDhcpc->Cfg.Alias);
    }

    if (strcmp(ParamName, "Interface") == 0)
    {
        /* collect value */
        pString = CosaUtilGetFullPathNameByKeyword
                    (
                        (PUCHAR)"Device.IP.Interface.",
                        (PUCHAR)"Name",
                        (PUCHAR)pDhcpc->Cfg.Interface
                    );
        if ( pString )
        {
            int ret = update_pValue(pValue,pUlSize, (char*)pString);
            AnscFreeMemory(pString);
            return ret;
        }
        else
        {
            return 0;
        }

        return 0;
    }

    if (strcmp(ParamName, "DUID") == 0)
    {
        CosaDmlDhcpv6cGetInfo(pDhcpc, pCxtLink->InstanceNumber, &pDhcpc->Info);

        /* collect value */
        return  update_pValue(pValue,pUlSize, (char*)pDhcpc->Info.DUID);
    }

    if (strcmp(ParamName, "SupportedOptions") == 0)
    {
        /* collect value */
        return  update_pValue(pValue,pUlSize, (char*)pDhcpc->Info.SupportedOptions);
    }

    if (strcmp(ParamName, "RequestedOptions") == 0)
    {
        /* collect value */
        return  update_pValue(pValue,pUlSize, (char*)pDhcpc->Cfg.RequestedOptions);
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
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* save update to backup */
        pDhcpc->Cfg.bEnabled = bValue;

        return TRUE;
    }

    if (strcmp(ParamName, "RequestAddresses") == 0)
    {
        /* save update to backup */
        pDhcpc->Cfg.RequestAddresses = bValue;

        return TRUE;
    }

    if (strcmp(ParamName, "RequestPrefixes") == 0)
    {
        /* save update to backup */
        pDhcpc->Cfg.RequestPrefixes = bValue;

        return TRUE;
    }

    if (strcmp(ParamName, "RapidCommit") == 0)
    {
        /* save update to backup */
        pDhcpc->Cfg.RapidCommit = bValue;

        return TRUE;
    }

    if (strcmp(ParamName, "Renew") == 0)
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
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtLink          = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPCV6_FULL            pDhcpc            = (PCOSA_DML_DHCPCV6_FULL)pCxtLink->hContext;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "SuggestedT1") == 0)
    {
        /* save update to backup */
        pDhcpc->Cfg.SuggestedT1 = iValue;
        
        return TRUE;
    }

    if (strcmp(ParamName, "SuggestedT2") == 0)
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(uValue);
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
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtLink          = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPCV6_FULL            pDhcpc            = (PCOSA_DML_DHCPCV6_FULL)pCxtLink->hContext;
    PCOSA_DATAMODEL_DHCPV6            pDhcpv6           = (PCOSA_DATAMODEL_DHCPV6)pCxtLink->hParentTable;
    errno_t rc = -1;
    int ind    = -1;
        
    /* check the parameter name and set the corresponding value */
    rc = strcmp_s("RequestedOptions", strlen("RequestedOptions"), ParamName , &ind);
    ERR_CHK(rc);
    if((ind == 0) && (rc == EOK))
    {
        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER((char *)pDhcpc->Cfg.RequestedOptions, sizeof(pDhcpc->Cfg.RequestedOptions), pString);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }

        return TRUE;
    }

    /* check the parameter name and set the corresponding value */
    rc = strcmp_s("Alias", strlen("Alias"), ParamName , &ind);
    ERR_CHK(rc);
    if((ind == 0) && (rc == EOK))
    {
        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER(pDhcpv6->AliasOfClient, sizeof(pDhcpv6->AliasOfClient), (const char *)pDhcpc->Cfg.Alias);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }

        rc = STRCPY_S_NOCLOBBER((char *)pDhcpc->Cfg.Alias, sizeof(pDhcpc->Cfg.Alias), pString);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }

        rc = STRCPY_S_NOCLOBBER((char*)pDhcpc->Cfg.Alias, sizeof(pDhcpc->Cfg.Alias), pString);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
        return TRUE;
    }

    rc = strcmp_s("Interface", strlen("Interface"), ParamName , &ind);
    ERR_CHK(rc);
    if((ind == 0) && (rc == EOK))
    {

        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER((char *)pDhcpc->Cfg.Interface, sizeof(pDhcpc->Cfg.Interface), pString);
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
    PCOSA_DATAMODEL_DHCPV6            pDhcpv6           = (PCOSA_DATAMODEL_DHCPV6)g_pCosaBEManager->hDhcpv6;
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtLink          = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPCV6_FULL            pDhcpc            = (PCOSA_DML_DHCPCV6_FULL)pCxtLink->hContext;
    PSINGLE_LINK_ENTRY                pSListEntry       = NULL;
    PCOSA_DML_DHCPCV6_FULL            pDhcpc2           = NULL;
    BOOL                              bFound            = FALSE;
    errno_t                           rc                = -1;
    UNREFERENCED_PARAMETER(puLength);
    /*  only for Alias */
    if ( pDhcpv6->AliasOfClient[0] )
    {
        pSListEntry           = AnscSListGetFirstEntry(&pDhcpv6->ClientList);
        while( pSListEntry != NULL)
        {
            pCxtLink          = ACCESS_COSA_CONTEXT_DHCPCV6_LINK_OBJECT(pSListEntry);
            pSListEntry       = AnscSListGetNextEntry(pSListEntry);
        
            pDhcpc2 = (PCOSA_DML_DHCPCV6_FULL)pCxtLink->hContext;
        
            if( DHCPV6_CLIENT_ENTRY_MATCH2((char*)pDhcpc2->Cfg.Alias, (char*)pDhcpc->Cfg.Alias) )
            {
                if ( (ANSC_HANDLE)pCxtLink == hInsContext )
                {
                    continue;
                }

                rc = STRCPY_S_NOCLOBBER(pReturnParamName, *puLength, "Alias");
                if(rc != EOK)
                {
                  ERR_CHK(rc);
                  return FALSE;
                }

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
    errno_t    rc = -1;

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
            {
                rc = STRCPY_S_NOCLOBBER( (char*)pDhcpc->Cfg.Alias, sizeof(pDhcpc->Cfg.Alias), pDhcpv6->AliasOfClient );
                ERR_CHK(rc);
            }
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
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtLink          = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)hInsContext;
    PCOSA_DATAMODEL_DHCPV6            pDhcpv6           = (PCOSA_DATAMODEL_DHCPV6)g_pCosaBEManager->hDhcpv6;
    PCOSA_DML_DHCPCV6_FULL            pDhcpc            = (PCOSA_DML_DHCPCV6_FULL)pCxtLink->hContext;
    errno_t     rc = -1;

    if ( pDhcpv6->AliasOfClient[0] )
    {
        rc = STRCPY_S_NOCLOBBER( (char*)pDhcpc->Cfg.Alias, sizeof(pDhcpc->Cfg.Alias), pDhcpv6->AliasOfClient );
        ERR_CHK(rc);
    }

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
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtLink        = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)hInsContext;

    return (pCxtLink->NumberOfServer);
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pBool);
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pInt);
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(puLong);
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

    PCOSA_DML_DHCPCV6_SVR pDhcpcServer    = (PCOSA_DML_DHCPCV6_SVR)hInsContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "SourceAddress") == 0)
    {
        /* collect value */
        return  update_pValue(pValue,pUlSize, (char*)pDhcpcServer->SourceAddress);
    }

    if (strcmp(ParamName, "DUID") == 0)
    {
        /* collect value */
        return  update_pValue(pValue,pUlSize, (char*)pDhcpcServer->DUID);
    }

    if (strcmp(ParamName, "InformationRefreshTime") == 0)
    {
        /* collect value */
        return  update_pValue(pValue,pUlSize, (char*)pDhcpcServer->InformationRefreshTime);
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
    PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtLink          = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)hInsContext;
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
        PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT pCxtDhcpcLink        = (PCOSA_CONTEXT_DHCPCV6_LINK_OBJECT)hInsContext;
        PCOSA_CONTEXT_LINK_OBJECT         pCxtLink             = NULL;
        PCOSA_DML_DHCPCV6_SENT            pDhcpSentOption      = NULL;
        errno_t                           rc                   = -1;
        
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
    
        rc = sprintf_s( (char*)pDhcpSentOption->Alias, sizeof(pDhcpSentOption->Alias), "SentOption%lu", pDhcpSentOption->InstanceNumber);
        if(rc < EOK)
        {
          ERR_CHK(rc);
          AnscFreeMemory(pCxtLink);
          goto EXIT1;
        }
    
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
    
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pInt);
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
    
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Tag") == 0)
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

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* collect value */
        return  update_pValue(pValue,pUlSize, (char*)pDhcpSentOption->Alias);
    }

    if (strcmp(ParamName, "Value") == 0)
    {
        /* collect value */
        return  update_pValue(pValue,pUlSize, (char*)pDhcpSentOption->Value);
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
    if (strcmp(ParamName, "Enable") == 0)
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(iValue);
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
    if (strcmp(ParamName, "Tag") == 0)
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
    errno_t   rc = -1;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER(pCxtDhcpcLink->AliasOfSent, sizeof(pCxtDhcpcLink->AliasOfSent), (char*)pDhcpSentOption->Alias);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }

        rc = STRCPY_S_NOCLOBBER((char*)pDhcpSentOption->Alias, sizeof(pDhcpSentOption->Alias), pString);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }

        return TRUE;
    }

    if (strcmp(ParamName, "Value") == 0)
    {
        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER((char*)pDhcpSentOption->Value, sizeof(pDhcpSentOption->Value), pString);
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
        errno_t                           rc                = -1;
    
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
    
                if( DHCPV6_SENDOPTION_ENTRY_MATCH2((char*)pDhcpSentOption->Alias, (char*)pDhcpSentOption2->Alias) )
                {
                    if ( (ANSC_HANDLE)pCxtLink2 == hInsContext )
                    {
                        continue;
                    }
    
                    rc = STRCPY_S_NOCLOBBER(pReturnParamName, *puLength,"Alias");
                    if(rc != EOK)
                    {
                      ERR_CHK(rc);
                      return FALSE;
                    }
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
    
                    rc = STRCPY_S_NOCLOBBER(pReturnParamName, *puLength, "Tag");
                    if(rc != EOK)
                    {
                      ERR_CHK(rc);
                      return FALSE;
                    }
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
    errno_t  rc = -1;

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
            {
                rc = STRCPY_S_NOCLOBBER( (char*)pDhcpSentOption->Alias, sizeof(pDhcpSentOption->Alias), pCxtDhcpcLink->AliasOfSent );
                ERR_CHK(rc);
            }
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
    errno_t  rc = -1;

    if ( pCxtDhcpcLink->AliasOfSent[0] )
    {
        rc = STRCPY_S_NOCLOBBER( (char*)pDhcpSentOption->Alias, sizeof(pDhcpSentOption->Alias), pCxtDhcpcLink->AliasOfSent );
        ERR_CHK(rc);
    }

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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pBool);
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pInt);
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
    PCOSA_DML_DHCPCV6_RECV pDhcpcRecv      = (PCOSA_DML_DHCPCV6_RECV)hInsContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Tag") == 0)
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
    PCOSA_DML_DHCPCV6_RECV pDhcpcRecv      = (PCOSA_DML_DHCPCV6_RECV)hInsContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Value") == 0)
    {
        /* collect value */
        return  update_pValue(pValue,pUlSize, (char*)pDhcpcRecv->Value);
    }

    if (strcmp(ParamName, "Server") == 0)
    {
        /* collect value */
        return  update_pValue(pValue,pUlSize, (char*)pDhcpcRecv->Server);
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

#ifdef _HUB4_PRODUCT_REQ_
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        dhcp6c_mapt_mape_GetParamBoolValue
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
dhcp6c_mapt_mape_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "MapIsFMR") == 0)
    {
        char temp[32] = {0};
        commonSyseventGet(SYSEVENT_MAP_IS_FMR, temp, sizeof(temp));
        if (strcmp(temp, "TRUE") == 0)
            *pBool  = TRUE;
        else
            *pBool  = FALSE;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        dhcp6c_mapt_mape_GetParamUlongValue
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
dhcp6c_mapt_mape_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    char temp[64] = {0};
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "MapEALen") == 0)
    {
        commonSyseventGet(SYSEVENT_MAP_EA_LEN, temp, sizeof(temp));
        *puLong  = strtoul(temp, NULL, 10);
        return TRUE;
    }

    if (strcmp(ParamName, "MapPSIDOffset") == 0)
    {
        commonSyseventGet(SYSEVENT_MAPT_PSID_OFFSET, temp, sizeof(temp));
        *puLong  = strtoul(temp, NULL, 10);
        return TRUE;
    }

    if (strcmp(ParamName, "MapPSIDLen") == 0)
    {
        commonSyseventGet(SYSEVENT_MAPT_PSID_LENGTH, temp, sizeof(temp));
        *puLong  = strtoul(temp, NULL, 10);
        return TRUE;
    }

    if (strcmp(ParamName, "MapPSID") == 0)
    {
        commonSyseventGet(SYSEVENT_MAPT_PSID_VALUE, temp, sizeof(temp));
        *puLong  = strtoul(temp, NULL, 10);
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        dhcp6c_mapt_mape_GetParamStringValue
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
dhcp6c_mapt_mape_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    char temp[64] = {0};
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "MapTransportMode") == 0)
    {
        commonSyseventGet(SYSEVENT_MAP_TRANSPORT_MODE, temp, sizeof(temp));
        return  update_pValue(pValue,pUlSize, temp);
    }

    if (strcmp(ParamName, "MapBRPrefix") == 0)
    {
        commonSyseventGet(SYSEVENT_MAP_BR_IPV6_PREFIX, temp, sizeof(temp));
        return  update_pValue(pValue,pUlSize, temp);
    }

    if (strcmp(ParamName, "MapRuleIPv4Prefix") == 0)
    {
        commonSyseventGet(SYSEVENT_MAP_RULE_IPADDRESS, temp, sizeof(temp));
        return  update_pValue(pValue,pUlSize, temp);
    }

    if (strcmp(ParamName, "MapRuleIPv6Prefix") == 0)
    {
        commonSyseventGet(SYSEVENT_MAP_RULE_IPV6_ADDRESS, temp, sizeof(temp));
        return  update_pValue(pValue,pUlSize, temp);
    }

    if (strcmp(ParamName, "MapIpv4Address") == 0)
    {
        commonSyseventGet(SYSEVENT_MAPT_IPADDRESS, temp, sizeof(temp));
        return  update_pValue(pValue,pUlSize, temp);
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}
#endif
#endif
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
    UNREFERENCED_PARAMETER(hInsContext);
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pInt);
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
    UNREFERENCED_PARAMETER(hInsContext);

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "X_CISCO_COM_Type") == 0)
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pValue);
    UNREFERENCED_PARAMETER(pUlSize);
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
    UNREFERENCED_PARAMETER(hInsContext);
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(iValue);
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
    UNREFERENCED_PARAMETER(hInsContext);
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "X_CISCO_COM_Type") == 0)
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pString);
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pReturnParamName);
    UNREFERENCED_PARAMETER(puLength);
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    *  Pool1_GetParamUlongValue
    *  Pool1_GetParamStringValue
    *  Pool1_SetParamBoolValue
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DHCPV6            pDhcpv6           = (PCOSA_DATAMODEL_DHCPV6)g_pCosaBEManager->hDhcpv6;
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtLink          = NULL;
    PCOSA_DML_DHCPSV6_POOL_FULL       pPool             = NULL;
    errno_t                           rc                = -1;
#ifndef MULTILAN_FEATURE
        /* We just have one Pool. Not permit to add/delete. */
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

    rc = sprintf_s( (char*)pPool->Cfg.Alias, sizeof(pPool->Cfg.Alias),"Pool%lu", pPool->Cfg.InstanceNumber);
    if(rc < EOK)
    {
      ERR_CHK(rc);
      AnscFreeMemory(pCxtLink);
      goto EXIT1;
    }

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

    UNREFERENCED_PARAMETER(hInsContext);

#ifndef MULTILAN_FEATURE
	/* We just have one Pool. Not permit to add/delete. */
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
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV6_POOL_FULL       pDhcpc            = (PCOSA_DML_DHCPSV6_POOL_FULL)pCxtLink->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* collect value */
        *pBool   =  pDhcpc->Cfg.bEnabled;

        return TRUE;
    }

    if (strcmp(ParamName, "DUIDExclude") == 0)
    {
        /* collect value */
        *pBool   =  pDhcpc->Cfg.DUIDExclude;
        
        return TRUE;
    }

    if (strcmp(ParamName, "VendorClassIDExclude") == 0)
    {
        /* collect value */
        *pBool   =  pDhcpc->Cfg.VendorClassIDExclude;

        return TRUE;
    }

    if (strcmp(ParamName, "UserClassIDExclude") == 0)
    {
        /* collect value */
        *pBool   =  pDhcpc->Cfg.UserClassIDExclude;

        return TRUE;
    }

    if (strcmp(ParamName, "SourceAddressExclude") == 0)
    {
        /* collect value */
        *pBool   =  pDhcpc->Cfg.SourceAddressExclude;

        return TRUE;
    }

    if (strcmp(ParamName, "IANAEnable") == 0)
    {
        /* collect value */
        *pBool   =  pDhcpc->Cfg.IANAEnable;

        return TRUE;
    }

    if (strcmp(ParamName, "IAPDEnable") == 0)
    {
        /* collect value */
        *pBool   =  pDhcpc->Cfg.IAPDEnable;

        return TRUE;
    }

    if (strcmp(ParamName, "X_CISCO_RapidEnable") == 0)
    {
        /* collect value */
        *pBool   =  pDhcpc->Cfg.RapidEnable;

        return TRUE;
    }

    if (strcmp(ParamName, "X_CISCO_UnicastEnable") == 0)
    {
        /* collect value */
        *pBool   =  pDhcpc->Cfg.UnicastEnable;

        return TRUE;
    }

    if (strcmp(ParamName, "X_CISCO_EUI64Enable") == 0)
    {
        /* collect value */
        *pBool   =  pDhcpc->Cfg.EUI64Enable;

        return TRUE;
    }

    if (strcmp(ParamName, "X_RDKCENTRAL-COM_DNSServersEnabled") == 0)
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
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV6_POOL_FULL       pPool             = (PCOSA_DML_DHCPSV6_POOL_FULL)pCxtLink->hContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Status") == 0)
    {
        /* collect value */
        CosaDmlDhcpv6sGetPoolInfo( NULL, pPool->Cfg.InstanceNumber, &pPool->Info );

        *puLong  = pPool->Info.Status;

        return TRUE;
    }

    if (strcmp(ParamName, "Order") == 0)
    {
        /* collect value */
        *puLong  = pPool->Cfg.Order;

        return TRUE;
    }

    if (strcmp(ParamName, "IAPDAddLength") == 0)
    {
        /* collect value */
        *puLong  = pPool->Cfg.IAPDAddLength;

        return TRUE;
    }

    if (strcmp(ParamName, "X_CISCO_IANAAmount") == 0)
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
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV6_POOL_FULL       pPool             = (PCOSA_DML_DHCPSV6_POOL_FULL)pCxtLink->hContext;
    PUCHAR                            pString           = NULL;
    errno_t   rc = -1;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* collect value */
        return  update_pValue(pValue,pUlSize, (char*)pPool->Cfg.Alias);
    }

    if (strcmp(ParamName, "Interface") == 0)
    {
        /* collect value */
     #ifdef CISCO_CONFIG_DHCPV6_PREFIX_DELEGATION
        pString = CosaUtilGetFullPathNameByKeyword
                    (
                        (PUCHAR)"Device.IP.Interface.",
                        (PUCHAR)"Name",
                        (PUCHAR)pPool->Cfg.Interface /* When brlan0 works ,change to "brlan0"*/
                    );
     #elif defined (MULTILAN_FEATURE)
        pString = (PUCHAR)pPool->Cfg.Interface;
     #else
        pString = CosaUtilGetFullPathNameByKeyword
                    (
                        (PUCHAR)"Device.IP.Interface.",
                        (PUCHAR)"Name",
                        (PUCHAR)"brlan0" /* When brlan0 works ,change to "brlan0"*/
                    );
     #endif
     
        if ( pString )
        {
            if ( AnscSizeOfString((const char*)pString) < *pUlSize)
            {
                rc = strcpy_s(pValue, *pUlSize, (char*) pString);
                ERR_CHK(rc);
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
                *pUlSize = AnscSizeOfString((const char*)pString)+1;
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

    if (strcmp(ParamName, "DUID") == 0)
    {
        /* collect value */
        return  update_pValue(pValue,pUlSize, (char*)pPool->Cfg.DUID);
    }

    if (strcmp(ParamName, "VendorClassID") == 0)
    {
        /* collect value */
        return  update_pValue(pValue,pUlSize, (char*)pPool->Cfg.VendorClassID);
    }

    if (strcmp(ParamName, "UserClassID") == 0)
    {
        /* collect value */
        return  update_pValue(pValue,pUlSize, (char*)pPool->Cfg.UserClassID);
    }

    if (strcmp(ParamName, "SourceAddress") == 0)
    {
        /* collect value */
        return  update_pValue(pValue,pUlSize,  (char*)pPool->Cfg.SourceAddress);
    }

    if (strcmp(ParamName, "SourceAddressMask") == 0)
    {
        /* collect value */
        return  update_pValue(pValue,pUlSize,  (char*)pPool->Cfg.SourceAddressMask);
    }

    if (strcmp(ParamName, "IANAManualPrefixes") == 0)
    {
        /* collect value */
        return  update_pValue(pValue,pUlSize, (char*)pPool->Cfg.IANAManualPrefixes); 
    }

    if (strcmp(ParamName, "IANAPrefixes") == 0)
    {
        /* collect value */
        return  update_pValue(pValue,pUlSize, (char*)pPool->Info.IANAPrefixes);
    }

    if (strcmp(ParamName, "IAPDManualPrefixes") == 0)
    {
        /* collect value */
        return  update_pValue(pValue,pUlSize, (char*)pPool->Cfg.IAPDManualPrefixes);
    }

    if (strcmp(ParamName, "IAPDPrefixes") == 0)
    {
        /* collect value */
#ifdef CISCO_CONFIG_DHCPV6_PREFIX_DELEGATION
	        return CosaDmlDhcpv6sGetIAPDPrefixes2(&pPool->Cfg, pValue, pUlSize);
#else
#endif

        return  update_pValue(pValue,pUlSize, (char*)pPool->Info.IAPDPrefixes);
    }

    if (strcmp(ParamName, "PrefixRangeBegin") == 0)
    {
        /* collect value */
        return  update_pValue(pValue,pUlSize, (char*)pPool->Cfg.PrefixRangeBegin);
    }

    if (strcmp(ParamName, "PrefixRangeEnd") == 0)
    {
        /* collect value */
        return  update_pValue(pValue,pUlSize, (char*)pPool->Cfg.PrefixRangeEnd);
    }

    if (strcmp(ParamName, "X_CISCO_StartAddress") == 0)
    {
        char   addr[64] = {0};

        CosaDmlDhcpv6sGetStartAddress(NULL, addr, sizeof(addr));
        /* collect value */
        return  update_pValue(pValue,pUlSize, addr);
    }

    if (strcmp(ParamName, "X_RDKCENTRAL-COM_DNSServers") == 0)
    {
        /* collect value */
        return  update_pValue(pValue,pUlSize, (char*)pPool->Cfg.X_RDKCENTRAL_COM_DNSServers);
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
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV6_POOL_FULL       pPool            = (PCOSA_DML_DHCPSV6_POOL_FULL)pCxtLink->hContext;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        /* save update to backup */
        pPool->Cfg.bEnabled   = bValue;

        return TRUE;
    }

    if (strcmp(ParamName, "DUIDExclude") == 0)
    {
        /* save update to backup */
        pPool->Cfg.DUIDExclude   = bValue;

        /* We just support same mechnism */
        pPool->Cfg.SourceAddressExclude   = bValue;

        return TRUE;
    }

    if (strcmp(ParamName, "VendorClassIDExclude") == 0)
    {
        /* save update to backup */

        pPool->Cfg.VendorClassIDExclude   = bValue;

        return TRUE;
    }

    if (strcmp(ParamName, "UserClassIDExclude") == 0)
    {
        /* save update to backup */

        pPool->Cfg.UserClassIDExclude   = bValue;

        return TRUE;
    }

    if (strcmp(ParamName, "SourceAddressExclude") == 0)
    {
        /* save update to backup */
        pPool->Cfg.SourceAddressExclude   = bValue;

        pPool->Cfg.DUIDExclude   = bValue;

        return TRUE;
    }

    if (strcmp(ParamName, "IANAEnable") == 0)
    {
        /* save update to backup */
        pPool->Cfg.IANAEnable   = bValue;

        return TRUE;
    }

    if (strcmp(ParamName, "IAPDEnable") == 0)
    {
        /* save update to backup */
        pPool->Cfg.IAPDEnable   = bValue;

        return TRUE;
    }

    if (strcmp(ParamName, "X_CISCO_UnicastEnable") == 0)
    {
        /* save update to backup */
        pPool->Cfg.UnicastEnable= bValue;

        return TRUE;
    }

    if (strcmp(ParamName, "X_CISCO_RapidEnable") == 0)
    {
        /* save update to backup */
        pPool->Cfg.RapidEnable  = bValue;

        return TRUE;
    }

    if (strcmp(ParamName, "X_CISCO_EUI64Enable") == 0)
    {
        /* save update to backup */
        pPool->Cfg.EUI64Enable = bValue;

        return TRUE;
    }

    if (strcmp(ParamName, "X_RDKCENTRAL-COM_DNSServersEnabled") == 0)
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
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV6_POOL_FULL       pPool             = (PCOSA_DML_DHCPSV6_POOL_FULL)pCxtLink->hContext;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Order") == 0)
    {
        /* save update to backup */
        pPool->Cfg.Order  = uValue;

        return TRUE;
    }

    if (strcmp(ParamName, "IAPDAddLength") == 0)
    {
        /* save update to backup */
        pPool->Cfg.IAPDAddLength  = uValue;

        return TRUE;
    }

    if (strcmp(ParamName, "X_CISCO_IANAAmount") == 0)
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
    PCOSA_DATAMODEL_DHCPV6            pDhcpv6           = (PCOSA_DATAMODEL_DHCPV6)g_pCosaBEManager->hDhcpv6;
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV6_POOL_FULL       pPool             = (PCOSA_DML_DHCPSV6_POOL_FULL)pCxtLink->hContext;
    
    int a[4]={0};
    char dump;
    errno_t     rc =  -1;
    int ind = -1;

    /* check the parameter name and set the corresponding value */
    rc = strcmp_s("Alias", strlen("Alias"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
       /* save update to backup */
       rc = strcpy_s(pDhcpv6->AliasOfPool,sizeof(pDhcpv6->AliasOfPool), (const char *)pPool->Cfg.Alias);
       if(rc != EOK)
       {
          ERR_CHK(rc);
          return FALSE;
       }
       rc = STRCPY_S_NOCLOBBER((char *)pPool->Cfg.Alias,sizeof(pPool->Cfg.Alias), pString);
       if(rc != EOK)
       {
          ERR_CHK(rc);
          return FALSE;
       }

       return TRUE;
    }

    rc = strcmp_s("Interface", strlen("Interface"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
       /* save update to backup */
#if defined (MULTILAN_FEATURE) 
       rc = sprintf_s(pPool->Cfg.Interface,sizeof(pPool->Cfg.Interface), "%s.", pString);
       if(rc < EOK)
       {
          ERR_CHK(rc);
          return FALSE;
       }
#else
       rc = STRCPY_S_NOCLOBBER(pPool->Cfg.Interface,sizeof(pPool->Cfg.Interface), pString);
       if(rc != EOK)
       {
          ERR_CHK(rc);
          return FALSE;
       }
#endif
       return TRUE;
    }

    rc = strcmp_s("DUID", strlen("DUID"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
       /* save update to backup */
       rc = STRCPY_S_NOCLOBBER((char *)pPool->Cfg.DUID,sizeof(pPool->Cfg.DUID), pString);
       if(rc != EOK)
       {
         ERR_CHK(rc);
         return FALSE;
       }
       return TRUE;
    }

    rc = strcmp_s("VendorClassID", strlen("VendorClassID"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
       /* save update to backup */
       rc = STRCPY_S_NOCLOBBER(pPool->Cfg.VendorClassID,sizeof(pPool->Cfg.VendorClassID), pString);
       if(rc != EOK)
       {
          ERR_CHK(rc);
          return FALSE;
       }
       return TRUE;
    }

    rc = strcmp_s("UserClassID", strlen("UserClassID"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
       /* save update to backup */

      rc = STRCPY_S_NOCLOBBER((char *)pPool->Cfg.UserClassID,sizeof(pPool->Cfg.UserClassID), pString);
      if(rc != EOK)
      {
        ERR_CHK(rc);
        return FALSE;
      }
      return TRUE;
    }

    rc = strcmp_s("IANAManualPrefixes", strlen("IANAManualPrefixes"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
       /* save update to backup */
       rc = STRCPY_S_NOCLOBBER((char *)pPool->Cfg.IANAManualPrefixes,sizeof(pPool->Cfg.IANAManualPrefixes), pString);
       if(rc != EOK)
       {
         ERR_CHK(rc);
         return FALSE;
       }
       rc = memcpy_s(pPool->Info.IANAPrefixes, sizeof(pPool->Info.IANAPrefixes), pPool->Cfg.IANAManualPrefixes, sizeof(pPool->Cfg.IANAManualPrefixes));
       if(rc != EOK)
       {
         ERR_CHK(rc);
         return FALSE;
       }

       return TRUE;
    }

    rc = strcmp_s("IAPDManualPrefixes", strlen("IAPDManualPrefixes"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
       /* save update to backup */
       rc = STRCPY_S_NOCLOBBER((char *)pPool->Cfg.IAPDManualPrefixes,sizeof(pPool->Cfg.IAPDManualPrefixes), pString);
       if(rc != EOK)
       {
         ERR_CHK(rc);
         return FALSE;
       }

       return TRUE;
    }

    rc = strcmp_s("SourceAddress", strlen("SourceAddress"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
       /* save update to backup */
       rc = STRCPY_S_NOCLOBBER((char *)pPool->Cfg.SourceAddress,sizeof(pPool->Cfg.SourceAddress), pString);
       if(rc != EOK)
       {
         ERR_CHK(rc);
         return FALSE;
       }

       return TRUE;
    }

    rc = strcmp_s("SourceAddressMask", strlen("SourceAddressMask"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
       /* save update to backup */
       rc = STRCPY_S_NOCLOBBER((char *)pPool->Cfg.SourceAddressMask,sizeof(pPool->Cfg.SourceAddressMask), pString);
       if(rc != EOK)
       {
         ERR_CHK(rc);
         return FALSE;
       }

       return TRUE;
    }

    rc = strcmp_s("PrefixRangeBegin", strlen("PrefixRangeBegin"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        strcmp_s(pString, strlen(pString),(const char *)pPool->Cfg.PrefixRangeEnd, &ind);
        if(ind>0)
            return FALSE;
        if( sscanf(pString, "%x:%x:%x:%x %c", &a[0], &a[1], &a[2], &a[3], &dump) == 4
            && a[0] <= 0xFFFF
            && a[1] <= 0xFFFF
            && a[2] <= 0xFFFF
            && a[3] <= 0xFFFF )
            {
               /* save update to backup */
               rc = STRCPY_S_NOCLOBBER((char *)pPool->Cfg.PrefixRangeBegin,sizeof(pPool->Cfg.PrefixRangeBegin), pString);
               if(rc != EOK)
               {
                 ERR_CHK(rc);
                 return FALSE;
               }
               return TRUE;
            }

    }

    rc = strcmp_s("PrefixRangeEnd", strlen("PrefixRangeEnd"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        strcmp_s((const char *)pPool->Cfg.PrefixRangeBegin, strlen((const char *)pPool->Cfg.PrefixRangeBegin),pString, &ind);
        if(ind>0)
            return FALSE;
        if( sscanf(pString, "%x:%x:%x:%x %c", &a[0], &a[1], &a[2], &a[3], &dump) == 4
            && a[0] <= 0xFFFF
            && a[1] <= 0xFFFF
            && a[2] <= 0xFFFF
            && a[3] <= 0xFFFF )
            {
               /* save update to backup */
               rc = STRCPY_S_NOCLOBBER((char *)pPool->Cfg.PrefixRangeEnd,sizeof(pPool->Cfg.PrefixRangeEnd), pString);
               if(rc != EOK)
               {
                 ERR_CHK(rc);
                 return FALSE;
               }
               return TRUE;
            }

    }

    rc = strcmp_s("X_CISCO_StartAddress", strlen("X_CISCO_StartAddress"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER((char *)pPool->Cfg.StartAddress,sizeof(pPool->Cfg.StartAddress), pString);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return FALSE;
        }
        return TRUE;
    }

    rc = strcmp_s("X_RDKCENTRAL-COM_DNSServers", strlen("X_RDKCENTRAL-COM_DNSServers"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER((char *)pPool->Cfg.X_RDKCENTRAL_COM_DNSServers,sizeof(pPool->Cfg.X_RDKCENTRAL_COM_DNSServers), pString);
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
        PCOSA_DATAMODEL_DHCPV6            pDhcpv6           = (PCOSA_DATAMODEL_DHCPV6)g_pCosaBEManager->hDhcpv6;
        PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)hInsContext;
        PCOSA_DML_DHCPSV6_POOL_FULL       pPool             = (PCOSA_DML_DHCPSV6_POOL_FULL)pCxtLink->hContext;
        PSINGLE_LINK_ENTRY                pSListEntry       = NULL;
        PCOSA_DML_DHCPSV6_POOL_FULL       pPool2            = NULL;
        BOOL                              bFound            = FALSE;
        errno_t                           rc                = -1;

        UNREFERENCED_PARAMETER(puLength);           
        /*  only for Alias */
        if ( pDhcpv6->AliasOfPool[0] )
        {
            pSListEntry           = AnscSListGetFirstEntry(&pDhcpv6->PoolList);
            while( pSListEntry != NULL)
            {
                pCxtLink          = ACCESS_COSA_CONTEXT_POOLV6_LINK_OBJECT(pSListEntry);
                pSListEntry       = AnscSListGetNextEntry(pSListEntry);
            
                pPool2 = (PCOSA_DML_DHCPSV6_POOL_FULL)pCxtLink->hContext;
            
                if( DHCPV6_POOL_ENTRY_MATCH2((char*)pPool2->Cfg.Alias, (char*)pPool->Cfg.Alias) )
                {
                    if ( (ANSC_HANDLE)pCxtLink == hInsContext )
                    {
                        continue;
                    }
    
                    rc = STRCPY_S_NOCLOBBER(pReturnParamName, *puLength, "Alias");
                    if(rc != EOK)
                    {
                      ERR_CHK(rc);
                      return FALSE;
                    }
                    
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
    errno_t   rc = -1;

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
            {
                rc = STRCPY_S_NOCLOBBER( (char*)pPool->Cfg.Alias, sizeof(pPool->Cfg.Alias), pDhcpv6->AliasOfPool );
                ERR_CHK(rc);
            }
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
    errno_t   rc = -1;

    if ( pDhcpv6->AliasOfPool[0] )
    {
        rc = STRCPY_S_NOCLOBBER( (char*)pPool->Cfg.Alias, sizeof(pPool->Cfg.Alias), pDhcpv6->AliasOfPool );
        ERR_CHK(rc);
    }

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
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)hInsContext;
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
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)hInsContext;
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
             /* CID: 54474 Dereference after null check*/
             return ANSC_STATUS_FAILURE;
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
                CcspTraceWarning(( "Client4_Synchronize -- CosaDmlDhcpv6sGetIPv6Address() error %lu.\n", returnStatus));
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
                CcspTraceWarning(( "Client4_Synchronize -- CosaDmlDhcpv6sGetIPv6Prefix() error %lu.\n", returnStatus));
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
                CcspTraceWarning(( "Client4_Synchronize -- CosaDmlDhcpv6sGetIPv6Option() error %lu.\n", returnStatus));
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
    if (strcmp(ParamName, "Active") == 0)
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pInt);
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(puLong);
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

    PCOSA_DML_DHCPSV6_CLIENT        pDhcpsClient    = (PCOSA_DML_DHCPSV6_CLIENT)hInsContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* collect value */
        return  update_pValue(pValue,pUlSize, pDhcpsClient->Alias);
    }

    if (strcmp(ParamName, "SourceAddress") == 0)
    {
        /* collect value */
        return  update_pValue(pValue,pUlSize, (char*)pDhcpsClient->SourceAddress);
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(bValue);
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(iValue);
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(uValue);
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
    PCOSA_DML_DHCPSV6_CLIENT        pDhcpsClient    = (PCOSA_DML_DHCPSV6_CLIENT)hInsContext;
    errno_t   rc = -1;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* save update to backup */
        rc = STRCPY_S_NOCLOBBER(pDhcpsClient->Alias, sizeof(pDhcpsClient->Alias), pString);
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pReturnParamName);
    UNREFERENCED_PARAMETER(puLength);
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pBool);
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pInt);
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(puLong);
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
    PCOSA_DML_DHCPSV6_CLIENT_IPV6ADDRESS   pIPv6Address    = (PCOSA_DML_DHCPSV6_CLIENT_IPV6ADDRESS)hInsContext;
   
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "IPAddress") == 0)
    {
        /* collect value */
        return  update_pValue(pValue,pUlSize, (char*)pIPv6Address->IPAddress);
    }

    if (strcmp(ParamName, "PreferredLifetime") == 0)
    {
        /* collect value */
        return  update_pValue(pValue,pUlSize, (char*)pIPv6Address->PreferredLifetime);
    }

    if (strcmp(ParamName, "ValidLifetime") == 0)
    {
        /* collect value */
        return  update_pValue(pValue,pUlSize,  (char*)pIPv6Address->ValidLifetime);
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pBool);
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pInt);
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(puLong);
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
    PCOSA_DML_DHCPSV6_CLIENT_IPV6PREFIX    pIPv6Prefix    = (PCOSA_DML_DHCPSV6_CLIENT_IPV6PREFIX)hInsContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Prefix") == 0)
    {
        /* collect value */
        return  update_pValue(pValue,pUlSize, (char*)pIPv6Prefix->Prefix);
    }

    if (strcmp(ParamName, "PreferredLifetime") == 0)
    {
        /* collect value */
        return  update_pValue(pValue,pUlSize, (char*)pIPv6Prefix->PreferredLifetime);
    }

    if (strcmp(ParamName, "ValidLifetime") == 0)
    {
        /* collect value */
        return  update_pValue(pValue,pUlSize, (char*)pIPv6Prefix->ValidLifetime);
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pBool);
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pInt);
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
    PCOSA_DML_DHCPSV6_CLIENT_OPTION        pIPv6Option     = (PCOSA_DML_DHCPSV6_CLIENT_OPTION)hInsContext;


    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Tag") == 0)
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
    PCOSA_DML_DHCPSV6_CLIENT_OPTION        pIPv6Option     = (PCOSA_DML_DHCPSV6_CLIENT_OPTION)hInsContext;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Value") == 0)
    {
        /* collect value */
        return  update_pValue(pValue,pUlSize, (char*)pIPv6Option->Value);
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
    PCOSA_CONTEXT_POOLV6_LINK_OBJECT  pCxtPoolLink         = (PCOSA_CONTEXT_POOLV6_LINK_OBJECT)hInsContext;
    PCOSA_CONTEXT_LINK_OBJECT         pCxtLink             = NULL;
    PCOSA_DML_DHCPSV6_POOL_OPTION     pDhcpOption          = NULL;
    PCOSA_DATAMODEL_DHCPV6            pDhcpv6              = (PCOSA_DATAMODEL_DHCPV6)g_pCosaBEManager->hDhcpv6;
    errno_t                           rc                   = -1;
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

    rc = sprintf_s( (char*)pDhcpOption->Alias, sizeof(pDhcpOption->Alias), "Option%lu", pDhcpOption->InstanceNumber);
    if(rc < EOK)
    {
      ERR_CHK(rc);
      AnscFreeMemory(pCxtLink);
      goto EXIT1;
    }

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
    if (strcmp(ParamName, "Enable") == 0)
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pInt);
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
    if (strcmp(ParamName, "Tag") == 0)
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
    if (strcmp(ParamName, "Alias") == 0)
    {
        /* collect value */
        return  update_pValue(pValue,pUlSize, (char*)pDhcpOption->Alias);
    }

    if (strcmp(ParamName, "Value") == 0)
    {
        /* collect value */
        return  update_pValue(pValue,pUlSize, (char*)pDhcpOption->Value);
    }

    if (strcmp(ParamName, "PassthroughClient") == 0)
    {
        /* collect value */
        return  update_pValue(pValue,pUlSize, (char*)pDhcpOption->PassthroughClient);
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
    if (strcmp(ParamName, "Enable") == 0)
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
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(iValue);
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
    if (strcmp(ParamName, "Tag") == 0)
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
    errno_t     rc =  -1;
    int ind = -1;

    /* check the parameter name and set the corresponding value */
    rc = strcmp_s("Alias", strlen("Alias"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
       /* save update to backup */
       rc = strcpy_s(pPoolLink->AliasOfOption,sizeof(pPoolLink->AliasOfOption), (const char *)pDhcpOption->Alias);
       if(rc != EOK)
       {
          ERR_CHK(rc);
          return FALSE;
       }
       rc = STRCPY_S_NOCLOBBER((char *)pDhcpOption->Alias,sizeof(pDhcpOption->Alias), pString);
       if(rc != EOK)
       {
          ERR_CHK(rc);
          return FALSE;
       }

       return TRUE;
    }


    rc = strcmp_s("Value", strlen("Value"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
       /* save update to backup */
       rc = STRCPY_S_NOCLOBBER((char *)pDhcpOption->Value,sizeof(pDhcpOption->Value), pString);
       if(rc != EOK)
       {
          ERR_CHK(rc);
          return FALSE;
       }

       return TRUE;
    }

    rc = strcmp_s("PassthroughClient", strlen("PassthroughClient"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {

	   /* save update to backup */
       rc = STRCPY_S_NOCLOBBER((char *)pDhcpOption->PassthroughClient,sizeof(pDhcpOption->PassthroughClient), pString);
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
    PCOSA_CONTEXT_LINK_OBJECT         pCxtLink2         = NULL;
    PCOSA_DML_DHCPSV6_POOL_OPTION     pDhcpPoolOption2  = NULL;
    PSINGLE_LINK_ENTRY                pSListEntry       = NULL;
    BOOL                              bFound            = FALSE;
    errno_t                           rc                = -1;

    UNREFERENCED_PARAMETER(puLength);

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

            if( DHCPV6_POOLOPTION_ENTRY_MATCH2((char*)pDhcpPoolOption->Alias, (char*)pDhcpPoolOption2->Alias )) 
            {
                if ( (ANSC_HANDLE)pCxtLink2 == hInsContext )
                {
                    continue;
                }

                rc = STRCPY_S_NOCLOBBER(pReturnParamName, *puLength,"Alias");
                if(rc != EOK)
                {
                  ERR_CHK(rc);
                  return FALSE;
                }

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

                rc = STRCPY_S_NOCLOBBER(pReturnParamName, *puLength,"Tag");
                if(rc != EOK)
                {
                  ERR_CHK(rc);
                  return FALSE;
                }

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
    errno_t   rc = -1;
    
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
            {
                rc = STRCPY_S_NOCLOBBER( (char*)pDhcpOption->Alias, sizeof(pDhcpOption->Alias), pCxtPoolLink->AliasOfOption );
                ERR_CHK(rc);
            }
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
    errno_t   rc = -1;

    if ( pCxtPoolLink->AliasOfOption[0] )
    {
        rc = STRCPY_S_NOCLOBBER( (char*)pDhcpPoolOption->Alias, sizeof(pDhcpPoolOption->Alias), pCxtPoolLink->AliasOfOption );
        ERR_CHK(rc);
    }

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

