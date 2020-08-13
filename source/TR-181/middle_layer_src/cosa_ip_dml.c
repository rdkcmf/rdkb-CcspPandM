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

    module: cosa_ip_dml.c

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

        01/17/2011    initial revision.

**************************************************************************/

#include "ansc_platform.h"
#include "ansc_string_util.h"
/*#include "cosa_diagnostic_apis.h"*/
#include "plugin_main_apis.h"
#include "cosa_ip_apis.h"
#include "cosa_ip_apis_multilan.h"
#include "cosa_ip_dml.h"
#include "cosa_ip_internal.h"
#include "safec_lib_common.h"

static ULONG last_tick;
#define REFRESH_INTERVAL 120
#define TIME_NO_NEGATIVE(x) ((long)(x) < 0 ? 0 : (x))


#ifndef _COSA_SIM_
BOOL CosaIpifGetSetSupported(char * pParamName);
#endif

#include <net/if.h>

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

    IP.

    *  IP_GetParamBoolValue
    *  IP_GetParamIntValue
    *  IP_GetParamUlongValue
    *  IP_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IP_GetParamBoolValue
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
IP_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "IPv4Capable", TRUE))
    {
        /* collect value */
        *pBool = TRUE;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "IPv6Capable", TRUE))
    {
        /* collect value */
        *pBool = TRUE;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "IPv4Enable", TRUE))
    {
        /* collect value */
        *pBool = TRUE;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "IPv6Enable", TRUE))
    {
        /* collect value */
        *pBool = TRUE;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IP_GetParamIntValue
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
IP_GetParamIntValue
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
        IP_GetParamUlongValue
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
IP_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "IPv4Status", TRUE))
    {
        /* collect value */
        *puLong = COSA_DML_IPV4V6_STATUS_Enabled;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "IPv6Status", TRUE))
    {
        /* collect value */
        *puLong = COSA_DML_IPV4V6_STATUS_Enabled;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IP_GetParamStringValue
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
IP_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "ULAPrefix", TRUE))
    {
        /* collect value */
        *pValue = 0;
        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IP_SetParamBoolValue
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
IP_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "IPv4Enable", TRUE))
    {
        /* not supported */
        return FALSE;
    }

    if( AnscEqualString(ParamName, "IPv6Enable", TRUE))
    {
        /* not supported */
        return FALSE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IP_SetParamIntValue
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
IP_SetParamIntValue
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
        IP_SetParamUlongValue
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
IP_SetParamUlongValue
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
        IP_SetParamStringValue
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
IP_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "ULAPrefix", TRUE))
    {
        /* not supported */
        return FALSE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IP_Validate
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
IP_Validate
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
        IP_Commit
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
IP_Commit
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
        IP_Rollback
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
IP_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    return 0;
}

/***********************************************************************

 APIs for Object:

    IP.Interface.{i}.

    *  Interface2_GetEntryCount
    *  Interface2_GetEntry
    *  Interface2_AddEntry
    *  Interface2_DelEntry
    *  Interface2_GetParamBoolValue
    *  Interface2_GetParamIntValue
    *  Interface2_GetParamUlongValue
    *  Interface2_GetParamStringValue
    *  Interface2_SetParamBoolValue
    *  Interface2_SetParamIntValue
    *  Interface2_SetParamUlongValue
    *  Interface2_SetParamStringValue
    *  Interface2_Validate
    *  Interface2_Commit
    *  Interface2_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Interface2_GetEntryCount
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
Interface2_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_IP              pMyObject = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP;

    return AnscSListQueryDepth(&pMyObject->InterfaceList);
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Interface2_GetEntry
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
Interface2_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_IP              pMyObject     = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PSLIST_HEADER                   pIPIFHead     = (PSLIST_HEADER)&pMyObject->InterfaceList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pLink         = (PSINGLE_LINK_ENTRY       )NULL;

    pLink = AnscSListGetEntryByIndex(pIPIFHead, nIndex);

    if ( pLink )
    {
        pCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        *pInsNumber = pCosaContext->InstanceNumber;
    }
    
    return pCosaContext;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Interface2_AddEntry
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
Interface2_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_IP              pMyObject     = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PSLIST_HEADER                   pIPIFHead     = (PSLIST_HEADER)&pMyObject->InterfaceList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_IP_IF_FULL2           pIPInterface  = (PCOSA_DML_IP_IF_FULL2)NULL;

#ifndef MULTILAN_FEATURE
#ifndef _COSA_SIM_
    if (!CosaIpifGetSetSupported("addentry"))
    {
        return NULL;
    }
#endif
#endif
    pIPInterface = (PCOSA_DML_IP_IF_FULL2)AnscAllocateMemory(sizeof(COSA_DML_IP_IF_FULL2));

    if ( !pIPInterface )
    {
        return NULL;
    }
#if defined (MULTILAN_FEATURE)
    /*
     * Adding default Interface type
     */
    pIPInterface->Cfg.IfType   = COSA_DML_IP_IF_TYPE_Normal;
#endif

    _ansc_sprintf(pIPInterface->Cfg.Alias, "Interface%d", pMyObject->ulNextInterfaceInsNum);

    AnscSListInitializeHeader(&pIPInterface->IPV4List);
    
    pIPInterface->ulNextIPV4InsNum = 1;
    
    if ( TRUE )
    {
        pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

        if ( !pCosaContext )
        {
            AnscFreeMemory(pIPInterface);
            
            return NULL;
        }

        pCosaContext->InstanceNumber = pIPInterface->Cfg.InstanceNumber = pMyObject->ulNextInterfaceInsNum;

        pMyObject->ulNextInterfaceInsNum++;

        if ( 0 == pMyObject->ulNextInterfaceInsNum )
        {
            pMyObject->ulNextInterfaceInsNum = 1;
        }

        pCosaContext->hContext         = (ANSC_HANDLE)pIPInterface;
        pCosaContext->hParentTable     = NULL;
        pCosaContext->hPoamIrepUpperFo = pMyObject->hIrepFolderIPInterfaceHA;
        pCosaContext->bNew             = TRUE;

        CosaSListPushEntryByInsNum(pIPIFHead, pCosaContext);

        CosaIPRegAddInfo
        (
            (ANSC_HANDLE)pMyObject,
            COSA_DML_RR_NAME_IPIF_NextInsNum,
            pMyObject->ulNextInterfaceInsNum,
            COSA_DML_RR_NAME_IPIF_Prefix,
            0,
            pIPInterface->Cfg.Alias,
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
        Interface2_DelEntry
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
Interface2_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_IP              pMyObject     = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PSLIST_HEADER                   pIPIFHead     = (PSLIST_HEADER)&pMyObject->InterfaceList;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_DML_IP_IF_FULL2           pIPInterface  = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
#ifndef MULTILAN_FEATURE
#ifndef _COSA_SIM_
    if (!CosaIpifGetSetSupported("delentry"))
    {
        return ANSC_STATUS_FAILURE;
    }
#endif
#endif
    CosaDmlIpIfDelEntry(pMyObject->hSbContext, pIPInterface->Cfg.InstanceNumber);

    AnscSListPopEntryByLink(pIPIFHead, &pCosaContext->Linkage);

    if ( pCosaContext->bNew )
    {
        CosaIPRegDelInfo(NULL, (ANSC_HANDLE)pCosaContext);
    }
    
    AnscFreeMemory(pIPInterface);
    AnscFreeMemory(pCosaContext);

    return 0;
    }


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Interface2_GetParamBoolValue
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
Interface2_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
    
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        *pBool = pIPInterface->Cfg.bEnabled;
        /* collect value */
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Reset", TRUE))
    {
        /* collect value */
        *pBool = FALSE;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Loopback", TRUE))
    {
        /* collect value */
        *pBool = pIPInterface->Cfg.Loopback;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "AutoIPEnable", TRUE))
    {
        /* collect value */
        *pBool = pIPInterface->Cfg.AutoIPEnable;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "IPv4Enable", TRUE))
    {
        /* collect value */
        *pBool = TRUE;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "IPv6Enable", TRUE))
    {
        /* collect value */
#if defined (MULTILAN_FEATURE)
        *pBool = pIPInterface->Cfg.bIPv6Enabled;
#else
	*pBool = TRUE;
#endif
        return TRUE;
    }

    if( AnscEqualString(ParamName, "ULAEnable", TRUE))
    {
        /* collect value */
        *pBool = FALSE;
        return TRUE;
    }
    else if ( AnscEqualString(ParamName, "X_CISCO_COM_UPnPIGDEnabled", TRUE) )
    {
        *pBool = pIPInterface->Cfg.UpnpIgdEnabled;
        return  TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Interface2_GetParamIntValue
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
Interface2_GetParamIntValue
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
        Interface2_GetParamUlongValue
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
Interface2_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
    PCOSA_DML_IP_IF_INFO            p            = NULL;

#if defined (_INTEL_MAX_MTU_PROPOSED_FEATURE_)
    PCOSA_DATAMODEL_IP              pMyObject    = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP;
#endif

    p = CosaDmlIpIfGetEntry2(NULL, pIPInterface->Cfg.InstanceNumber);

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Status", TRUE))
    {
        /* Get value every time */
        if (CosaUtilIoctlXXX((char *)pIPInterface->Info.Name, "status", NULL) & IFF_UP) 
        {
            *puLong = COSA_DML_IF_STATUS_Up;
        }
        else
        {
            *puLong = COSA_DML_IF_STATUS_LowerLayerDown;
        }

        return TRUE;
    }

    if( AnscEqualString(ParamName, "LastChange", TRUE))
    {
        /* collect value */
        *puLong = AnscGetTimeIntervalInSeconds(pIPInterface->Info.LastChange, AnscGetTickInSeconds());
        return TRUE;
    }

    if( AnscEqualString(ParamName, "MaxMTUSize", TRUE))
    {
        /* collect value */
        *puLong = pIPInterface->Cfg.MaxMTUSize;

#if defined (_INTEL_MAX_MTU_PROPOSED_FEATURE_)
        CosaDmlIpIfGetCfg (pMyObject->hSbContext, (PCOSA_DML_IP_IF_CFG)&pIPInterface->Cfg);
#endif

        return TRUE;
    }

    if( AnscEqualString(ParamName, "Type", TRUE))
    {
        /* collect value */
        *puLong = pIPInterface->Cfg.IfType;
        return TRUE;
    }

    if (AnscEqualString(ParamName, "X_CISCO_COM_TTL", TRUE))
    {
        *puLong = pIPInterface->Cfg.WanTTL;
        return TRUE;
    }
    else if ( AnscEqualString(ParamName, "X_CISCO_COM_ARPCacheTimeout", TRUE) )
    {
        *puLong = pIPInterface->Cfg.ArpCacheTimeout;
        return TRUE;
    }

    if (AnscEqualString(ParamName, "X_CISCO_COM_IPv6_IANA_T1", TRUE))
    {
        if ( p ){
            *puLong = p->iana_t1;
            return TRUE;
        }
    }

    if (AnscEqualString(ParamName, "X_CISCO_COM_IPv6_IANA_T2", TRUE))
    {
        if ( p ){
            *puLong = p->iana_t2;
            return TRUE;
        }
    }

    if (AnscEqualString(ParamName, "X_CISCO_COM_IPv6_IAPD_T1", TRUE))
    {
        if ( p ){
            *puLong = p->iapd_t1;
            return TRUE;
        }
    }

    if (AnscEqualString(ParamName, "X_CISCO_COM_IPv6_IAPD_T2", TRUE))
    {
        if ( p ){
            *puLong = p->iapd_t2;
            return TRUE;
        }
    }

    if (AnscEqualString(ParamName, "X_CISCO_COM_IPv6_IANA_IAID", TRUE))
    {
        if ( p ){
            *puLong = p->iana_iaid;
            return TRUE;
        }
    }

    if (AnscEqualString(ParamName, "X_CISCO_COM_IPv6_IAPD_IAID", TRUE))
    {
        if ( p ){
            *puLong = p->iapd_iaid;         
            return TRUE;
        }
    }
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Interface2_GetParamStringValue
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
Interface2_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
    PUCHAR                          pLowerLayer             = NULL;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pIPInterface->Cfg.Alias);
        return 0;
    }

    if( AnscEqualString(ParamName, "Name", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pIPInterface->Info.Name);
        return 0;
    }

    if( AnscEqualString(ParamName, "LowerLayers", TRUE))
    {
        if(_ansc_strlen(pIPInterface->Cfg.LowerLayers) == 0)
        {
            CcspTraceDebug
                ((
                    "%s -- LowerLayers, IpIf %d, LinkName is %s, EthLink %d, IfType %d\n",
                    __FUNCTION__,
                    pIPInterface->Cfg.InstanceNumber,
                    pIPInterface->Cfg.LinkName,
                    pIPInterface->Cfg.LinkInstNum,
                    pIPInterface->Cfg.IfType
                ));

            if ( pIPInterface->Cfg.IfType != COSA_DML_IP_IF_TYPE_Normal )
            {
                AnscCopyString(pIPInterface->Cfg.LowerLayers, "");
            }
            else if ( !pIPInterface->Cfg.LinkInstNum )
            {
                /* Link instance number/LinkName is invalid*/
                char* linkTypePath = CosaUtilGetLinkTypePath(pIPInterface->Cfg.LinkType);

                if ( pLowerLayer = CosaUtilGetLowerLayers(linkTypePath, pIPInterface->Cfg.LinkName) )
                    _ansc_strcpy(pIPInterface->Cfg.LowerLayers, pLowerLayer);
                else
                    AnscCopyString(pIPInterface->Cfg.LowerLayers, "");
            }
            else
            {
                char* linkTypePath = CosaUtilGetLinkTypePath(pIPInterface->Cfg.LinkType);

                _ansc_sprintf(pIPInterface->Cfg.LowerLayers, "%s%d", linkTypePath, pIPInterface->Cfg.LinkInstNum);
            }
        }

        CcspTraceDebug
            ((
                "%s -- LowerLayers is %s\n",
                __FUNCTION__,
                pIPInterface->Cfg.LowerLayers
            ));

        AnscCopyString(pValue, pIPInterface->Cfg.LowerLayers);
    }

    /*RDKB-, CID-33002, free unused resource before exit*/
    if(pLowerLayer)
    {
        AnscFreeMemory(pLowerLayer);
    }

    if( AnscEqualString(ParamName, "Router", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pIPInterface->Cfg.RouterName);         
        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Interface2_SetParamBoolValue
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
Interface2_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_DATAMODEL_IP              pMyObject    = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
#ifndef MULTILAN_FEATURE
#ifndef _COSA_SIM_
    if (!CosaIpifGetSetSupported(ParamName))
    {
        return FALSE;
    }
#endif
#endif
    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* save update to backup */
        pIPInterface ->Cfg.bEnabled = bValue;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "Reset", TRUE))
    {
        /* save update to backup */
        if ( TRUE == bValue )
        {
            CosaDmlIpIfReset(pMyObject->hSbContext, pIPInterface->Cfg.InstanceNumber);
        }
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Loopback", TRUE))
    {
        /* save update to backup */
        pIPInterface->Cfg.Loopback = bValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "AutoIPEnable", TRUE))
    {
        /* save update to backup */
        pIPInterface->Cfg.AutoIPEnable = bValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "IPv4Enable", TRUE))
    {
        return FALSE;
    }

    if( AnscEqualString(ParamName, "IPv6Enable", TRUE))
    {
#if defined (MULTILAN_FEATURE)
        /* save update to backup */
        pIPInterface ->Cfg.bIPv6Enabled = bValue;
        return TRUE;
#else
        return FALSE;
#endif
    }

    if( AnscEqualString(ParamName, "ULAEnable", TRUE))
    {
        return FALSE;
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_UPnPIGDEnabled", TRUE))
    {
        /* save update to backup */
        pIPInterface->Cfg.UpnpIgdEnabled = bValue;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Interface2_SetParamIntValue
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
Interface2_SetParamIntValue
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
        Interface2_SetParamUlongValue
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
Interface2_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
#ifndef MULTILAN_FEATURE
#ifndef _COSA_SIM_
    if (!CosaIpifGetSetSupported(ParamName))
    {
        return FALSE;
    }
#endif
#endif
    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "MaxMTUSize", TRUE))
    {
        /* save update to backup */
        pIPInterface->Cfg.MaxMTUSize = uValue;
        return TRUE;
    }

    if (AnscEqualString(ParamName, "X_CISCO_COM_TTL", TRUE))
    {
        pIPInterface->Cfg.WanTTL = uValue;
        return TRUE;
    }
    else if ( AnscEqualString(ParamName, "X_CISCO_COM_ARPCacheTimeout", TRUE) )
    {
        pIPInterface->Cfg.ArpCacheTimeout = uValue;
        return  TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Interface2_SetParamStringValue
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
Interface2_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
#ifndef MULTILAN_FEATURE
#ifndef _COSA_SIM_
    if (!CosaIpifGetSetSupported(ParamName))
    {
        return FALSE;
    }
#endif
#endif
    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE))
    {
        /* save update to backup */
        AnscCopyString(pIPInterface->Cfg.Alias, pString);

        return TRUE;
    }

    if( AnscEqualString(ParamName, "LowerLayers", TRUE))
    {
        if ( pIPInterface->Cfg.IfType != COSA_DML_IP_IF_TYPE_Normal )
        {
            /* Loopback/Tunnel/Tunneled cannot have LowerLayers */
            return  FALSE;
        }
        else
        {
            ULONG                           ulIndex;
            UCHAR                           ucEntryParamName[256]       = {0};
            UCHAR                           ucEntryNameValue[256]       = {0};
#if defined (MULTILAN_FEATURE)
            ULONG                           ulEntryNameLen = 256;
#else
      	    int                             size;
            parameterValStruct_t            varStruct;
#endif
 
           AnscTraceWarning
                ((
                    "%s -- IpIf %d, LinkName is %s, EthLink %d, LowerLayers %s\n",
                    __FUNCTION__,
                    pIPInterface->Cfg.InstanceNumber,
                    pIPInterface->Info.Name,
                    pIPInterface->Cfg.LinkInstNum,
                    pString
                ));

            if ( _ansc_strlen(pString) == 0 )
            {
                pIPInterface->Cfg.LinkType    = COSA_DML_LINK_TYPE_LAST;
                pIPInterface->Cfg.LinkInstNum = 0;
                pIPInterface->Cfg.LinkName[0] = "\0";
            }
            else
            {
                pIPInterface->Cfg.LinkType = CosaUtilGetLinkTypeFromPath(pString);

                /* Normalize the LowerLayer string -- remove the '.' at the end */
                if ( pString[_ansc_strlen(pString) - 1] == '.' )
                {
                    pString[_ansc_strlen(pString) - 1] = '\0';   
                }

                /* Extract Instance Number */
                ulIndex = _ansc_strlen(pString) - 1;
#if defined (MULTILAN_FEATURE)
                while ( (ulIndex != 0) && (pString[ulIndex -1] != '.') )
#else
		while ( (ulIndex != 0) && (pString[ulIndex] != '.') )
#endif
                {
                    ulIndex--;
                }

                if ( ulIndex == 0 )
                {
                    pIPInterface->Cfg.LinkInstNum = 0;
                }
                else
                {
                    pIPInterface->Cfg.LinkInstNum = (ULONG)AnscString2Int(&pString[ulIndex]);
                }

                /* Retrieve LinkName */
                _ansc_sprintf(ucEntryParamName, "%s.%s", pString, "Name");
#if defined (MULTILAN_FEATURE)
                ulEntryNameLen = sizeof(ucEntryNameValue);
                if ( 0 == CosaGetParamValueString(ucEntryParamName, ucEntryNameValue, &ulEntryNameLen))
#else
		varStruct.parameterName  = ucEntryParamName;
                varStruct.parameterValue = ucEntryNameValue;

                if ( ANSC_STATUS_SUCCESS == 
                        COSAGetParamValueByPathName(&varStruct, &size) )
#endif
                {
                    AnscCopyString(pIPInterface->Cfg.LinkName, ucEntryNameValue);
                }
                else
                {
                    pIPInterface->Cfg.LinkName[0] = '\0';
                }
            }

            return  TRUE;
        }
    }

    if( AnscEqualString(ParamName, "Router", TRUE))
    {
        /* save update to backup */
//        AnscCopyString(pIPInterface->Cfg.Router, pString);
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Interface2_Validate
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
Interface2_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_DATAMODEL_IP              pMyObject     = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2 = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_IP_IF_FULL2           pIPInterface  = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface2 = (PCOSA_DML_IP_IF_FULL2)NULL;
    PSLIST_HEADER                   pIPIFHead     = (PSLIST_HEADER)&pMyObject->InterfaceList;
    PSINGLE_LINK_ENTRY              pLink         = (PSINGLE_LINK_ENTRY)NULL;

    pLink = AnscSListGetFirstEntry(pIPIFHead);

    while ( pLink )
    {
        pCosaContext2 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        pLink   = AnscSListGetNextEntry(pLink);

        pIPInterface2 = (PCOSA_DML_IP_IF_FULL2)pCosaContext2->hContext;

        if ( pIPInterface2 && 
            ((ULONG)pIPInterface2 != (ULONG)pIPInterface) &&
             AnscEqualString(pIPInterface2->Cfg.Alias, pIPInterface->Cfg.Alias, TRUE))
        {
            AnscCopyString(pReturnParamName, "Alias");
            *puLength = AnscSizeOfString("Alias");

            CcspTraceWarning(("Interface2_Validate() failed.\n"));
            return FALSE;
        }
    }

    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Interface2_Commit
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
Interface2_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_IP              pMyObject    = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;

    if ( pCosaContext->bNew )
    {
        pCosaContext->bNew = FALSE;

        CosaDmlIpIfAddEntry(pMyObject->hSbContext, (PCOSA_DML_IP_IF_FULL)pIPInterface);

        CosaIPRegDelInfo(NULL, (ANSC_HANDLE)pCosaContext);
    }
    else
    {
        CosaDmlIpIfSetCfg(pMyObject->hSbContext, (PCOSA_DML_IP_IF_CFG)&pIPInterface->Cfg);
    }
    
    return 0;

}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Interface2_Rollback
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
Interface2_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{   
    PCOSA_DATAMODEL_IP              pMyObject    = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2            pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;

    CcspTraceWarning(("Interface2_Rollback()----begin to rollback......\n"));

    CosaDmlIpIfGetCfg (pMyObject->hSbContext, (PCOSA_DML_IP_IF_CFG)&pIPInterface->Cfg);
    CosaDmlIpIfGetInfo(pMyObject->hSbContext, pIPInterface->Cfg.InstanceNumber, (PCOSA_DML_IP_IF_INFO)&pIPInterface->Info);

    return 0; 
}

/***********************************************************************

 APIs for Object:

    IP.Interface.{i}.IPv4Address.{i}.

    *  IPv4Address_GetEntryCount
    *  IPv4Address_GetEntry
    *  IPv4Address_AddEntry
    *  IPv4Address_DelEntry
    *  IPv4Address_GetParamBoolValue
    *  IPv4Address_GetParamIntValue
    *  IPv4Address_GetParamUlongValue
    *  IPv4Address_GetParamStringValue
    *  IPv4Address_SetParamBoolValue
    *  IPv4Address_SetParamIntValue
    *  IPv4Address_SetParamUlongValue
    *  IPv4Address_SetParamStringValue
    *  IPv4Address_Validate
    *  IPv4Address_Commit
    *  IPv4Address_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv4Address_GetEntryCount
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
IPv4Address_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;

    return AnscSListQueryDepth(&pIPInterface->IPV4List);
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        IPv4Address_GetEntry
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
IPv4Address_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface     = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
    PSINGLE_LINK_ENTRY              pLink            = (PSINGLE_LINK_ENTRY)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)NULL;

    pLink = AnscSListGetEntryByIndex((PSLIST_HEADER)&pIPInterface->IPV4List, nIndex);

    if ( pLink )
    {
        pSubCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        *pInsNumber = pSubCosaContext->InstanceNumber;
    }
    
    return pSubCosaContext;

}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        IPv4Address_AddEntry
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
IPv4Address_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_IP              pMyObject        = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface     = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_IP_V4ADDR             pIPv4Addr        = (PCOSA_DML_IP_V4ADDR)NULL;
#ifndef MULTILAN_FEATURE
#ifndef _COSA_SIM_
    if (!CosaIpifGetSetSupported("ipv4addr_addentry"))
    {
        return NULL;
    }
#endif
#endif
    pIPv4Addr = (PCOSA_DML_IP_V4ADDR)AnscAllocateMemory(sizeof(COSA_DML_IP_V4ADDR));

    if ( !pIPv4Addr )
    {
        return NULL;
    }

    _ansc_sprintf(pIPv4Addr->Alias, "IPv4Address%d", pIPInterface->ulNextIPV4InsNum);

    /* Update the middle layer data */
    if ( TRUE )
    {
        pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

        if ( !pSubCosaContext )
        {
            AnscFreeMemory(pIPv4Addr);
            
            return NULL;
        }

        pSubCosaContext->InstanceNumber = pIPInterface->ulNextIPV4InsNum;

        pIPInterface->ulNextIPV4InsNum++;

        if ( 0 == pIPInterface->ulNextIPV4InsNum )
        {
            pIPInterface->ulNextIPV4InsNum = 1;
        }

        pSubCosaContext->hContext         = (ANSC_HANDLE)pIPv4Addr;
        pSubCosaContext->hParentTable     = pIPInterface;
        pSubCosaContext->hPoamIrepUpperFo = pMyObject->hIrepFolderIPInterfaceHA;
        pSubCosaContext->bNew             = TRUE;

        CosaSListPushEntryByInsNum((PSLIST_HEADER)&pIPInterface->IPV4List, pSubCosaContext);

        CosaIPRegAddInfo
        (
            (ANSC_HANDLE)pMyObject,
            COSA_DML_RR_NAME_IPIF_IPv4_NextInsNum,
            pIPInterface->ulNextIPV4InsNum,
            COSA_DML_RR_NAME_IPIF_IPV4_Prefix,
            pIPInterface->Cfg.InstanceNumber,
            pIPv4Addr->Alias,
            (ANSC_HANDLE)pSubCosaContext
        );
    }

    *pInsNumber = pSubCosaContext->InstanceNumber;

    return pSubCosaContext;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv4Address_DelEntry
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
IPv4Address_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_IP              pMyObject       = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface    = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_DML_IP_V4ADDR             pIPv4Addr       = (PCOSA_DML_IP_V4ADDR)pSubCosaContext->hContext;
#ifndef MULTILAN_FEATURE
#ifndef _COSA_SIM_
    if (!CosaIpifGetSetSupported("ipv4addr_delentry"))
    {
        return ANSC_STATUS_FAILURE;
    }
#endif
#endif
    CosaDmlIpIfDelV4Addr(pMyObject->hSbContext, pIPInterface->Cfg.InstanceNumber, pIPv4Addr);

    AnscSListPopEntryByLink((PSLIST_HEADER)&pIPInterface->IPV4List, &pSubCosaContext->Linkage);

    if ( pSubCosaContext->bNew )
    {
        CosaIPRegDelInfo(NULL, (ANSC_HANDLE)pSubCosaContext);
    }

    AnscFreeMemory(pIPv4Addr);
    AnscFreeMemory(pSubCosaContext);

    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv4Address_GetParamBoolValue
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
IPv4Address_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V4ADDR             pIPv4Addr    = (PCOSA_DML_IP_V4ADDR)pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* collect value */
        *pBool = pIPv4Addr->bEnabled;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv4Address_GetParamIntValue
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
IPv4Address_GetParamIntValue
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
        IPv4Address_GetParamUlongValue
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
IPv4Address_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V4ADDR             pIPv4Addr    = (PCOSA_DML_IP_V4ADDR)pCosaContext->hContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hParentTable;
    BOOL                                      bridgeMode;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "IPAddress", TRUE))
    {
#if !defined(_COSA_BCM_MIPS_) && !defined(_ENABLE_DSL_SUPPORT_)
        if (pIPv4Addr->AddressingType == COSA_DML_IP_ADDR_TYPE_Static && pIPInterface->Cfg.InstanceNumber >= CosaGetUsgIfNum())
        {
	    CosaDmlIpIfMlanGetIPv4Addr(pIPInterface->Cfg.InstanceNumber, pIPv4Addr);
            *puLong = pIPv4Addr->IPAddress.Value;
        }
        else
#endif /* !_COSA_BCM_MIPS_ */
        {
            /* TBC -- this should be from a SBAPI call */
            *puLong = CosaUtilGetIfAddr(pIPInterface->Cfg.LinkName);
        }
         
         return TRUE;
    }

    if( AnscEqualString(ParamName, "SubnetMask", TRUE))
    {
        //if((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&bridgeMode)) &&
        //   (TRUE == bridgeMode))
        //{
        //    *puLong = 0; 
        //}
#if !defined(_COSA_BCM_MIPS_) && !defined(_ENABLE_DSL_SUPPORT_)
        if (pIPv4Addr->AddressingType == COSA_DML_IP_ADDR_TYPE_Static && pIPInterface->Cfg.InstanceNumber >= CosaGetUsgIfNum())
        {
	    CosaDmlIpIfMlanGetSubnetMask(pIPInterface->Cfg.InstanceNumber, pIPv4Addr);
            *puLong = pIPv4Addr->SubnetMask.Value;
        }
        else
#endif /* !_COSA_BCM_MIPS_ */
        {
            /*
             *  TBC --  Why on earth is platform specific code called in middle layer!
             */
            *puLong = CosaUtilIoctlXXX(pIPInterface->Info.Name, "netmask", NULL);
        }
        return TRUE;

    }

    if( AnscEqualString(ParamName, "AddressingType", TRUE))
    {
        /* collect value */
        *puLong = pIPv4Addr->AddressingType;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Status", TRUE) )
    {
        *puLong = pIPv4Addr->Status;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv4Address_GetParamStringValue
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
IPv4Address_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V4ADDR             pIPv4Addr    = (PCOSA_DML_IP_V4ADDR)pCosaContext->hContext;
    
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pIPv4Addr->Alias);
        return 0;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv4Address_SetParamBoolValue
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
IPv4Address_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V4ADDR             pIPv4Addr    = (PCOSA_DML_IP_V4ADDR)pCosaContext->hContext;
#ifndef MULTILAN_FEATURE
#ifndef _COSA_SIM_
    if (!CosaIpifGetSetSupported(ParamName))
    {
        return FALSE;
    }
#endif
#endif  
    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* save update to backup */
        pIPv4Addr->bEnabled = bValue;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv4Address_SetParamIntValue
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
IPv4Address_SetParamIntValue
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
        IPv4Address_SetParamUlongValue
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
IPv4Address_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V4ADDR             pIPv4Addr    = (PCOSA_DML_IP_V4ADDR)pCosaContext->hContext;
#ifndef MULTILAN_FEATURE
#ifndef _COSA_SIM_
    if (!CosaIpifGetSetSupported(ParamName))
    {
        return FALSE;
    }
#endif
#endif
    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "IPAddress", TRUE))
    {
        /* save update to backup */
        if (pIPv4Addr->AddressingType != COSA_DML_IP_ADDR_TYPE_Static)
        {
            return FALSE;
        }
        else 
        {
            pIPv4Addr->IPAddress.Value = uValue;
            return TRUE;
        }
    }

    if( AnscEqualString(ParamName, "SubnetMask", TRUE))
    {
        /* save update to backup */
        if (pIPv4Addr->AddressingType != COSA_DML_IP_ADDR_TYPE_Static)
        {
            return FALSE;
        }
        else
        {
            pIPv4Addr->SubnetMask.Value = uValue;
            return TRUE;
        }
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv4Address_SetParamStringValue
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
IPv4Address_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V4ADDR             pIPv4Addr    = (PCOSA_DML_IP_V4ADDR)pCosaContext->hContext;
#ifndef MULTILAN_FEATURE
#ifndef _COSA_SIM_
    if (!CosaIpifGetSetSupported(ParamName))
    {
        return FALSE;
    }
#endif
#endif
    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE))
    {
        /* save update to backup */
        AnscCopyString(pIPv4Addr->Alias, pString);
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;

}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv4Address_Validate
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
IPv4Address_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2 = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_IP_IF_FULL2           pIPInterface  = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hParentTable;
    PCOSA_DML_IP_V4ADDR             pIPv4Addr     = (PCOSA_DML_IP_V4ADDR)pCosaContext->hContext;
    PCOSA_DML_IP_V4ADDR             pIPv4Addr2    = (PCOSA_DML_IP_V4ADDR)NULL;
    PSINGLE_LINK_ENTRY              pLink         = (PSINGLE_LINK_ENTRY)NULL;

    pLink = AnscSListGetFirstEntry(&pIPInterface->IPV4List);

    while ( pLink )
    {
        pCosaContext2 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        pLink   = AnscSListGetNextEntry(pLink);

        pIPv4Addr2 = (PCOSA_DML_IP_V4ADDR)pCosaContext2->hContext;

        if ( pIPv4Addr2 &&
            ((ULONG)pIPv4Addr2 != (ULONG)pIPv4Addr) &&
             AnscEqualString(pIPv4Addr2->Alias, pIPv4Addr->Alias, TRUE))
        {
            AnscCopyString(pReturnParamName, "Alias");
            *puLength = AnscSizeOfString("Alias");

            CcspTraceWarning(("IPv4Address_Validate() failed.\n"));            
            return FALSE;
        }
    }

    return TRUE;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv4Address_Commit
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
IPv4Address_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_IP              pMyObject    = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V4ADDR             pIPv4Addr    = (PCOSA_DML_IP_V4ADDR)pCosaContext->hContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hParentTable;

    if ( pCosaContext->bNew )
    {
        pCosaContext->bNew = FALSE;

        CosaDmlIpIfAddV4Addr(pMyObject->hSbContext, pIPInterface->Cfg.InstanceNumber, pIPv4Addr);

        CosaIPRegDelInfo(NULL, (ANSC_HANDLE)pCosaContext);
    }
    else
    {
        CosaDmlIpIfSetV4Addr(pMyObject->hSbContext, pIPInterface->Cfg.InstanceNumber, pIPv4Addr);
    }
    
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv4Address_Rollback
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
IPv4Address_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_IP              pMyObject    = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V4ADDR             pIPv4Addr    = (PCOSA_DML_IP_V4ADDR)pCosaContext->hContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hParentTable;

    CcspTraceWarning(("IPv4Address_Rollback()----begin to rollback......\n"));

    CosaDmlIpIfGetV4Addr2(pMyObject->hSbContext, pIPInterface->Cfg.InstanceNumber, pIPv4Addr);

    return 0;
}

/***********************************************************************

 APIs for Object:

    IP.Interface.{i}.IPv6Address.{i}.

    *  IPv6Address_GetEntryCount
    *  IPv6Address_GetEntry
    *  IPv6Address_AddEntry
    *  IPv6Address_DelEntry
    *  IPv6Address_GetParamBoolValue
    *  IPv6Address_GetParamIntValue
    *  IPv6Address_GetParamUlongValue
    *  IPv6Address_GetParamStringValue
    *  IPv6Address_SetParamBoolValue
    *  IPv6Address_SetParamIntValue
    *  IPv6Address_SetParamUlongValue
    *  IPv6Address_SetParamStringValue
    *  IPv6Address_Validate
    *  IPv6Address_Commit
    *  IPv6Address_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv6Address_GetEntryCount
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
IPv6Address_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{    
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;

    return AnscSListQueryDepth(&pIPInterface->IPV6List);

}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        IPv6Address_GetEntry
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
IPv6Address_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface     = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
    PSINGLE_LINK_ENTRY              pLink            = (PSINGLE_LINK_ENTRY)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)NULL;

    pLink = AnscSListGetEntryByIndex((PSLIST_HEADER)&pIPInterface->IPV6List, nIndex);

    if ( pLink )
    {
        pSubCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        *pInsNumber = pSubCosaContext->InstanceNumber;
    }
    
    return pSubCosaContext;
}

#define IPV6_TABLE_TIMEOUT 30

BOOL
IPv6Address_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface     = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
    BOOL                            bIsUpdated   = TRUE;

    /* 
        We can use one rough granularity interval to get whole table in case 
        that the updating is too frequent.
        */
    if ( ( AnscGetTickInSeconds() - pIPInterface->V6AddrPreviousVisitTime ) < IPV6_TABLE_TIMEOUT )
    {
        bIsUpdated  = FALSE;
    }
    else
    {
        pIPInterface->V6AddrPreviousVisitTime =  AnscGetTickInSeconds();
        bIsUpdated  = TRUE;
    }

    return bIsUpdated;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv6Address_Synchronize
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
IPv6Address_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                           returnStatus      = ANSC_STATUS_FAILURE;
    PCOSA_DATAMODEL_IP                    pIP              = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP;
    PCOSA_CONTEXT_LINK_OBJECT             pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2                 pIPInterface     = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
    PCOSA_CONTEXT_LINK_OBJECT             pCxtLink     = NULL;
    PSINGLE_LINK_ENTRY                    pLink       = NULL;
    PSINGLE_LINK_ENTRY                    pLink2      = NULL;
    PCOSA_DML_IP_V6ADDR                   pIPv6Addr         = (PCOSA_DML_IP_V6ADDR)NULL;
    PCOSA_DML_IP_V6ADDR                   pIPv6Addr2        = (PCOSA_DML_IP_V6ADDR)NULL;
    ULONG                                 entryCount        = 0;
    ULONG                                 i                 = 0;
    PULONG                                pOldInBE          = NULL; /*Old entry in back end*/

    pIPv6Addr         = CosaDmlIPGetIPv6Addresses(pIPInterface,&entryCount);
    if ( !pIPv6Addr )
    {
        /* Get Error, we don't del link because next time, it may be successful */
        return ANSC_STATUS_FAILURE;
    }

    pOldInBE    =   (PULONG)AnscAllocateMemory( entryCount * sizeof(ULONG) );
    if ( !pOldInBE )
    {
        goto EXIT3;
    }
    
    /* Search the whole link and mark bFound of exist entry to FALSE */
    pLink =   AnscSListGetFirstEntry(&pIPInterface->IPV6List);
    while( pLink )
    {

        pCxtLink = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        pLink   = AnscSListGetNextEntry(pLink);

        pIPv6Addr2 = (PCOSA_DML_IP_V6ADDR)pCxtLink->hContext;

        pIPv6Addr2->bFound = FALSE;
    }

    /* go over all new entries, find them in current link table and mark them */
    for ( i = 0; i < entryCount; i++)
    {
        pLink =   AnscSListGetFirstEntry(&pIPInterface->IPV6List);
        while( pLink )
        {
            pCxtLink    = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
            pLink       = AnscSListGetNextEntry(pLink);

            pIPv6Addr2     = (PCOSA_DML_IP_V6ADDR)pCxtLink->hContext;
            if ( pIPv6Addr2->InstanceNumber == pIPv6Addr[i].InstanceNumber )
            {
                pOldInBE[i]         =  1;
                /* If found, update the content also, watch the sequence of below 2 statements, we should set bFound later */
                *pIPv6Addr2    = pIPv6Addr[i];
                pIPv6Addr2->bFound  = TRUE;

                pCxtLink->bNew    = FALSE;
                
                break;
            }
        }
    }

    /* We need del unreferred entry if it's not delay_added entry */
    pLink =   AnscSListGetFirstEntry(&pIPInterface->IPV6List);
    while( pLink )
    {
        pCxtLink = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        pLink2      = pLink;
        pLink       = AnscSListGetNextEntry(pLink);
        pIPv6Addr2     = (PCOSA_DML_IP_V6ADDR)pCxtLink->hContext;

        if ( pIPv6Addr2->bFound  == FALSE && pCxtLink->bNew  == FALSE )
        {
            AnscSListPopEntryByLink(&pIPInterface->IPV6List, pLink2);

            AnscFreeMemory( pCxtLink->hContext );
            AnscFreeMemory( pCxtLink );
        }
    }

    /* We add new entry into our link table */
    for ( i = 0; i < entryCount; i++ )
    {
        if ( pOldInBE[i] == 0 )
        {
            /* Add new one */
            pCxtLink             = (COSA_CONTEXT_LINK_OBJECT *)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
            if ( !pCxtLink )
            {
                goto EXIT2;
            }
            
            pIPv6Addr2        = (COSA_DML_IP_V6ADDR *)AnscAllocateMemory(sizeof(COSA_DML_IP_V6ADDR));
            if ( !pIPv6Addr2 )
            {
                goto EXIT1;
            }
            /* copy new content which should include InstanceNumber and Alias */
            *pIPv6Addr2       = pIPv6Addr[i];

            pCxtLink->hContext   = (ANSC_HANDLE)pIPv6Addr2;
            pCxtLink->InstanceNumber = pIPv6Addr2->InstanceNumber;
            pCxtLink->bNew       = FALSE;
            pCxtLink->hParentTable = pIPInterface;
            /*todo: add parent table*/

            CosaSListPushEntryByInsNum(&pIPInterface->IPV6List, (PCOSA_CONTEXT_LINK_OBJECT)pCxtLink);
        }
    }

    {
        ULONG ret = 0;
        
        ret = AnscSListQueryDepth(&pIPInterface->IPV6List);

    }
        
    returnStatus =  ANSC_STATUS_SUCCESS;

    goto EXIT2;
    
EXIT1:
    AnscFreeMemory(pCxtLink);
    
EXIT2:
    AnscFreeMemory(pOldInBE);
    
EXIT3:
    AnscFreeMemory(pIPv6Addr);
    return returnStatus;



}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        IPv6Address_AddEntry
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
IPv6Address_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_IP              pMyObject        = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface     = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_IP_V6ADDR             pIPv6Addr        = (PCOSA_DML_IP_V6ADDR)NULL;

    pIPv6Addr = (PCOSA_DML_IP_V6ADDR)AnscAllocateMemory(sizeof(COSA_DML_IP_V6ADDR));
    /*RDKB-, CID-33520, perform null check before use */
    if ( !pIPv6Addr )
    {
        return NULL;
    }

    pIPv6Addr->Origin = COSA_DML_IP6_ORIGIN_Static;
    AnscCopyString(pIPv6Addr->PreferredLifetime, "9999-12-31T23:59:59Z");
    AnscCopyString(pIPv6Addr->ValidLifetime, "9999-12-31T23:59:59Z");


    _ansc_sprintf(pIPv6Addr->Alias, "IPv6Address%d", pIPInterface->ulNextIPV6InsNum);

    /* Update the middle layer data */
    if ( TRUE )
    {
        pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

        if ( !pSubCosaContext )
        {
            AnscFreeMemory(pIPv6Addr);
            
            return NULL;
        }

        pSubCosaContext->InstanceNumber = pIPv6Addr->InstanceNumber = pIPInterface->ulNextIPV6InsNum;

        pIPInterface->ulNextIPV6InsNum++;

        if ( 0 == pIPInterface->ulNextIPV6InsNum )
        {
            pIPInterface->ulNextIPV6InsNum = 1;
        }

        pSubCosaContext->hContext         = (ANSC_HANDLE)pIPv6Addr;
        pSubCosaContext->hParentTable     = pIPInterface;
        pSubCosaContext->hPoamIrepUpperFo = pMyObject->hIrepFolderIPInterfaceHA;
        pSubCosaContext->bNew             = TRUE;

        CosaSListPushEntryByInsNum((PSLIST_HEADER)&pIPInterface->IPV6List, pSubCosaContext);

        CosaIPRegAddInfo
        (
            (ANSC_HANDLE)pMyObject,
            COSA_DML_RR_NAME_IPIF_IPv6_NextInsNum,
            pIPInterface->ulNextIPV6InsNum,
            COSA_DML_RR_NAME_IPIF_IPV6_Prefix,
            pIPInterface->Cfg.InstanceNumber,
            pIPv6Addr->Alias,
            (ANSC_HANDLE)pSubCosaContext
        );
    }

    *pInsNumber = pSubCosaContext->InstanceNumber;

    return pSubCosaContext;

}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv6Address_DelEntry
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
IPv6Address_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_IP              pMyObject       = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface    = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_DML_IP_V6ADDR             pIPv6Addr       = (PCOSA_DML_IP_V6ADDR)pSubCosaContext->hContext;

    if (pIPv6Addr->Origin != COSA_DML_IP6_ORIGIN_Static)
        return ANSC_STATUS_FAILURE;

    CosaDmlIpIfDelV6Addr(pMyObject->hSbContext, pIPInterface->Cfg.InstanceNumber, pIPv6Addr);

    AnscSListPopEntryByLink((PSLIST_HEADER)&pIPInterface->IPV6List, &pSubCosaContext->Linkage);

    if ( pSubCosaContext->bNew )
    {
        CosaIPRegDelInfo(NULL, (ANSC_HANDLE)pSubCosaContext);
    }

    AnscFreeMemory(pIPv6Addr);
    AnscFreeMemory(pSubCosaContext);

    return 0;

}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Address_GetParamBoolValue
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
IPv6Address_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V6ADDR             pIPv6Addr    = (PCOSA_DML_IP_V6ADDR)pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* collect value */
        *pBool = pIPv6Addr->bEnabled;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Anycast", TRUE))
    {
        /* collect value */
        *pBool = pIPv6Addr->bAnycast;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Address_GetParamIntValue
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
IPv6Address_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V6ADDR             pIPv6Addr    = (PCOSA_DML_IP_V6ADDR)pCosaContext->hContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hParentTable;

    /* check the parameter name and return the corresponding value */

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Address_GetParamUlongValue
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
IPv6Address_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V6ADDR             pIPv6Addr    = (PCOSA_DML_IP_V6ADDR)pCosaContext->hContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hParentTable;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Status", TRUE))
    {
        /* collect value */
        *puLong = pIPv6Addr->Status;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "IPAddressStatus", TRUE))
    {
        /* collect value */
#ifndef _COSA_SIM_
        *puLong = CosaDmlIPv6addrGetV6Status(pIPv6Addr, pIPInterface);
        return TRUE;
#endif

		*puLong = pIPv6Addr->V6Status;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Origin", TRUE))
    {
        /* collect value */
        *puLong = pIPv6Addr->Origin;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_PreferredLifetime", TRUE) )
    {
        /* collect value */
        *puLong  = pIPv6Addr->iana_pretm;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_ValidLifetime", TRUE) )
    {
        /* collect value */
        *puLong  = pIPv6Addr->iana_vldtm;

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv6Address_GetParamStringValue
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
IPv6Address_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V6ADDR             pIPv6Addr    = (PCOSA_DML_IP_V6ADDR)pCosaContext->hContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hParentTable;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pIPv6Addr->Alias);
        return 0;
    }

    if( AnscEqualString(ParamName, "IPAddress", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pIPv6Addr->IP6Address);
        return 0;
    }

    if( AnscEqualString(ParamName, "Prefix", TRUE))
    {
        /* collect value */
#ifndef _COSA_SIM_

        if (pIPv6Addr->Origin != COSA_DML_IP6_ORIGIN_Static)        
            CosaDmlGetPrefixPathName(pIPInterface->Info.Name, pIPInterface->Cfg.InstanceNumber, pIPv6Addr, pValue);
        else 
            AnscCopyString(pValue, pIPv6Addr->Prefix);
        return 0;
#endif


		AnscCopyString(pValue, pIPv6Addr->Prefix);
        return 0;
    }

    if( AnscEqualString(ParamName, "PreferredLifetime", TRUE))
    {
        /* collect value */
		AnscCopyString(pValue, pIPv6Addr->PreferredLifetime);
        return 0;
    }

    if( AnscEqualString(ParamName, "ValidLifetime", TRUE))
    {
        /* collect value */
		AnscCopyString(pValue, pIPv6Addr->ValidLifetime);
        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Address_SetParamBoolValue
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
IPv6Address_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V6ADDR             pIPv6Addr    = (PCOSA_DML_IP_V6ADDR)pCosaContext->hContext;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* save update to backup */
#ifndef _COSA_SIM_
        /*not supported*/
        if (pIPv6Addr->Origin != COSA_DML_IP6_ORIGIN_Static)
            return FALSE;
#endif

		pIPv6Addr->bEnabled = bValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Anycast", TRUE))
    {
        /* save update to backup */
        if (pIPv6Addr->Origin != COSA_DML_IP6_ORIGIN_Static)
            return FALSE;

		pIPv6Addr->bAnycast = bValue;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Address_SetParamIntValue
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
IPv6Address_SetParamIntValue
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
        IPv6Address_SetParamUlongValue
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
IPv6Address_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V6ADDR             pIPv6Addr    = (PCOSA_DML_IP_V6ADDR)pCosaContext->hContext;

    if (pIPv6Addr->Origin != COSA_DML_IP6_ORIGIN_Static)
        return FALSE;
        
    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Status", TRUE))
    {
        /* save update to backup */
        pIPv6Addr->Status = uValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "IPAddressStatus", TRUE))
    {
        /* save update to backup */
        pIPv6Addr->V6Status = uValue;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Address_SetParamStringValue
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
IPv6Address_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V6ADDR             pIPv6Addr    = (PCOSA_DML_IP_V6ADDR)pCosaContext->hContext;
    errno_t     rc =  -1;
    int ind = -1;

    /* check the parameter name and set the corresponding value */
    rc = strcmp_s("Alias", strlen("Alias"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
       /* save update to backup */
       rc = STRCPY_S_NOCLOBBER(pIPv6Addr->Alias, sizeof(pIPv6Addr->Alias), pString);
       if(rc != EOK)
       {
          ERR_CHK(rc);
          return FALSE;
       }
       return TRUE;
    }

    if (pIPv6Addr->Origin != COSA_DML_IP6_ORIGIN_Static)
        return FALSE;
    
    rc = strcmp_s("IPAddress", strlen("IPAddress"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
       /* save update to backup */
       rc = STRCPY_S_NOCLOBBER(pIPv6Addr->IP6Address, sizeof(pIPv6Addr->IP6Address), pString);
       if(rc != EOK)
       {
          ERR_CHK(rc);
          return FALSE;
       }
       return TRUE;
    }

    rc = strcmp_s("Prefix", strlen("Prefix"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
       /* save update to backup */
       rc = STRCPY_S_NOCLOBBER(pIPv6Addr->Prefix, sizeof(pIPv6Addr->Prefix), pString);
       if(rc != EOK)
       {
          ERR_CHK(rc);
          return FALSE;
       }
       return TRUE;
    }

    rc = strcmp_s("PreferredLifetime", strlen("PreferredLifetime"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
       /* save update to backup */
       rc = STRCPY_S_NOCLOBBER(pIPv6Addr->PreferredLifetime, sizeof(pIPv6Addr->PreferredLifetime), pString);
       if(rc != EOK)
       {
          ERR_CHK(rc);
          return FALSE;
       }
       return TRUE;
    }

    rc = strcmp_s("ValidLifetime", strlen("ValidLifetime"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
       /* save update to backup */
       rc = STRCPY_S_NOCLOBBER(pIPv6Addr->ValidLifetime, sizeof(pIPv6Addr->ValidLifetime), pString);
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
        IPv6Address_Validate
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
IPv6Address_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2 = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_IP_IF_FULL2           pIPInterface  = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hParentTable;
    PCOSA_DML_IP_V6ADDR             pIPv6Addr     = (PCOSA_DML_IP_V6ADDR)pCosaContext->hContext;
    PCOSA_DML_IP_V6ADDR             pIPv6Addr2    = (PCOSA_DML_IP_V6ADDR)NULL;
    PSINGLE_LINK_ENTRY              pLink         = (PSINGLE_LINK_ENTRY)NULL;
    struct in6_addr                 in6           = {0};

    pLink = AnscSListGetFirstEntry(&pIPInterface->IPV6List);

    while ( pLink )
    {
        pCosaContext2 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        pLink   = AnscSListGetNextEntry(pLink);

        pIPv6Addr2 = (PCOSA_DML_IP_V6ADDR)pCosaContext2->hContext;

        if ( pIPv6Addr2 &&
            ((ULONG)pIPv6Addr2 != (ULONG)pIPv6Addr ) &&
             AnscEqualString(pIPv6Addr2->Alias, pIPv6Addr->Alias, TRUE))
        {
            AnscCopyString(pReturnParamName, "Alias");
            *puLength = AnscSizeOfString("Alias");

            CcspTraceWarning(("IPv6Address_Validate() on Alias failed.\n"));            
            return FALSE;
        }

        if ( pIPv6Addr2 &&
            (pIPv6Addr2->bEnabled && pIPv6Addr->bEnabled) &&
             ((ULONG)pIPv6Addr2 != (ULONG)pIPv6Addr ) &&
             CosaDmlV6AddrIsEqual(pIPv6Addr2->IP6Address, pIPv6Addr->IP6Address))
        {
            _ansc_strcpy(pReturnParamName, "Address");
            *puLength = AnscSizeOfString("Address");
    
            CcspTraceWarning(("IPv6Address_Validate() on Address failed.\n"));            
            return FALSE;
        }
    }

    /*validate for Prefix*/
    if (pIPv6Addr->Prefix[0] &&
        !CosaDmlIpv6AddrMatchesPrefix(pIPv6Addr->Prefix, pIPv6Addr->IP6Address, pIPInterface->Cfg.InstanceNumber))
    {
        AnscCopyString(pReturnParamName, "Prefix");
        *puLength = AnscSizeOfString("Prefix");

        CcspTraceWarning(("IPv6Address_Validate() on Prefix failed.\n"));            
        return FALSE;
    }

    /*validate 2 lifetimes*/
    if (pIPv6Addr->PreferredLifetime && pIPv6Addr->ValidLifetime &&
        CosaDmlDateTimeCompare(pIPv6Addr->PreferredLifetime, pIPv6Addr->ValidLifetime) > 0
        )
    {
        AnscCopyString(pReturnParamName, "PreferredLifetime/ValidLifetime");
        *puLength = AnscSizeOfString("PreferredLifetime/ValidLifetime");

        CcspTraceWarning(("IPv6Address_Validate() on Lifetime failed.\n"));            
        return FALSE;
    }


    if (inet_pton(AF_INET6, pIPv6Addr->IP6Address, &in6) != 1)
    {
        _ansc_strcpy(pReturnParamName, "Address");
        *puLength = AnscSizeOfString("Address");
    
        CcspTraceWarning(("IPv6Address_Validate() on Address failed.\n"));
        return FALSE;
    }


    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv6Address_Commit
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
IPv6Address_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_IP              pMyObject    = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V6ADDR             pIPv6Addr    = (PCOSA_DML_IP_V6ADDR)pCosaContext->hContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hParentTable;

    if ( pCosaContext->bNew )
    {
        pCosaContext->bNew = FALSE;

        CosaDmlIpIfAddV6Addr(pMyObject->hSbContext, pIPInterface->Cfg.InstanceNumber, pIPv6Addr);

        CosaIPRegDelInfo(NULL, (ANSC_HANDLE)pCosaContext);
    }
    else
    {
        CosaDmlIpIfSetV6Addr(pMyObject->hSbContext, pIPInterface->Cfg.InstanceNumber, pIPv6Addr);
    }
    
    return 0;

}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv6Address_Rollback
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
IPv6Address_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_IP              pMyObject    = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V6ADDR             pIPv6Addr    = (PCOSA_DML_IP_V6ADDR)pCosaContext->hContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hParentTable;

    CcspTraceWarning(("IPv6Address_Rollback()----begin to rollback......\n"));

    CosaDmlIpIfGetV6Addr2(pMyObject->hSbContext, pIPInterface->Cfg.InstanceNumber, pIPv6Addr);

    return ANSC_STATUS_SUCCESS;
}

/***********************************************************************

 APIs for Object:

    IP.Interface.{i}.IPv6Prefix.{i}.

    *  IPv6Prefix_GetEntryCount
    *  IPv6Prefix_GetEntry
    *  IPv6Prefix_AddEntry
    *  IPv6Prefix_DelEntry
    *  IPv6Prefix_GetParamBoolValue
    *  IPv6Prefix_GetParamIntValue
    *  IPv6Prefix_GetParamUlongValue
    *  IPv6Prefix_GetParamStringValue
    *  IPv6Prefix_SetParamBoolValue
    *  IPv6Prefix_SetParamIntValue
    *  IPv6Prefix_SetParamUlongValue
    *  IPv6Prefix_SetParamStringValue
    *  IPv6Prefix_Validate
    *  IPv6Prefix_Commit
    *  IPv6Prefix_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv6Prefix_GetEntryCount
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
IPv6Prefix_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;

    return AnscSListQueryDepth(&pIPInterface->IPV6PrefixList);
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        IPv6Prefix_GetEntry
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
IPv6Prefix_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface     = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
    PSINGLE_LINK_ENTRY              pLink            = (PSINGLE_LINK_ENTRY)NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)NULL;

    pLink = AnscSListGetEntryByIndex((PSLIST_HEADER)&pIPInterface->IPV6PrefixList, nIndex);

    if ( pLink )
    {
        pSubCosaContext = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        *pInsNumber = pSubCosaContext->InstanceNumber;
    }
    
    return pSubCosaContext;
}

BOOL
IPv6Prefix_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface     = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
    BOOL                            bIsUpdated       = TRUE;

    if (!AnscSListQueryDepth(&pIPInterface->IPV6PrefixList)) {
        return TRUE;
    }
    /* 
        We can use one rough granularity interval to get whole table in case 
        that the updating is too frequent.
        */
    if ( ( AnscGetTickInSeconds() - pIPInterface->V6PrefPreviousVisitTime ) < IPV6_TABLE_TIMEOUT )
    {
        bIsUpdated  = FALSE;
    }
    else
    {
        pIPInterface->V6PrefPreviousVisitTime =  AnscGetTickInSeconds();
        bIsUpdated  = TRUE;
    }

#ifdef _HUB4_PRODUCT_REQ_
    /* To update the ipv6_prefix value for dibbler configuration, the DML should be syncronzed in time. So time delay check removed */
    bIsUpdated  = TRUE;
#endif
    return bIsUpdated;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv6Prefix_Synchronize
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
IPv6Prefix_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                           returnStatus      = ANSC_STATUS_FAILURE;
    PCOSA_DATAMODEL_IP                    pIP              = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP;
    PCOSA_CONTEXT_LINK_OBJECT             pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2                 pIPInterface     = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
    PCOSA_CONTEXT_LINK_OBJECT             pCxtLink     = NULL;
    PSINGLE_LINK_ENTRY                    pLink       = NULL;
    PSINGLE_LINK_ENTRY                    pLink2      = NULL;
    PCOSA_DML_IP_V6PREFIX                 pIPv6Pre         = (PCOSA_DML_IP_V6PREFIX)NULL;
    PCOSA_DML_IP_V6PREFIX                 pIPv6Pre2        = (PCOSA_DML_IP_V6PREFIX)NULL;
    ULONG                                 entryCount        = 0;
    ULONG                                 i                 = 0;
    PULONG                                pOldInBE       = 0; /*Old entry in back end*/

    pIPv6Pre         = CosaDmlIPGetIPv6Prefixes(pIPInterface,&entryCount);
    if ( !pIPv6Pre )
        return ANSC_STATUS_FAILURE;

    pOldInBE    =   (PULONG)AnscAllocateMemory( entryCount * sizeof(ULONG) );
    if ( !pOldInBE )
    {
        goto EXIT3;
    }
    
    /* Search the whole link and mark bFound of exist entry to FALSE */
    pLink =   AnscSListGetFirstEntry(&pIPInterface->IPV6PrefixList);
    while( pLink )
    {

        pCxtLink = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        pLink   = AnscSListGetNextEntry(pLink);

        pIPv6Pre2 = (PCOSA_DML_IP_V6PREFIX)pCxtLink->hContext;

        pIPv6Pre2->bFound = FALSE;
    }

    /* go over all old entries, find them in current link table and mark them */
    for ( i = 0; i < entryCount; i++)
    {
        pLink =   AnscSListGetFirstEntry(&pIPInterface->IPV6PrefixList);
        while( pLink )
        {
            pCxtLink    = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
            pLink       = AnscSListGetNextEntry(pLink);

            pIPv6Pre2     = (PCOSA_DML_IP_V6PREFIX)pCxtLink->hContext;
            if ( pIPv6Pre2->InstanceNumber == pIPv6Pre[i].InstanceNumber )
            {
                pOldInBE[i]         =  1;

                /* If found, update the content also, watch the sequence of below 2 statements, we should set bFound later  */
                *pIPv6Pre2    = pIPv6Pre[i];
                pIPv6Pre2->bFound  = TRUE;

                pCxtLink->bNew    = FALSE;
                
                break;
            }
        }
    }

    /* We need del unreferred entry if it's not delay_added entry */
    pLink =   AnscSListGetFirstEntry(&pIPInterface->IPV6PrefixList);
    while( pLink )
    {
        pCxtLink = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        pLink2      = pLink;
        pLink       = AnscSListGetNextEntry(pLink);
        pIPv6Pre2     = (PCOSA_DML_IP_V6PREFIX)pCxtLink->hContext;

        if ( pIPv6Pre2->bFound  == FALSE && pCxtLink->bNew  == FALSE )
        {
            AnscSListPopEntryByLink(&pIPInterface->IPV6PrefixList, pLink2);

            AnscFreeMemory( pCxtLink->hContext );
            AnscFreeMemory( pCxtLink );
        }
    }

    /* We add new entry into our link table */
    for ( i = 0; i < entryCount; i++ )
    {
        if ( pOldInBE[i] == 0 )
        {
            /* Add new one */
            pCxtLink             = (COSA_CONTEXT_LINK_OBJECT *)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));
            if ( !pCxtLink )
            {
                goto EXIT2;
            }
            
            pIPv6Pre2        = (COSA_DML_IP_V6PREFIX *)AnscAllocateMemory(sizeof(COSA_DML_IP_V6PREFIX));
            if ( !pIPv6Pre2 )
            {
                goto EXIT1;
            }
            /* copy new content which should include InstanceNumber and Alias */
            *pIPv6Pre2       = pIPv6Pre[i];

            pCxtLink->hContext   = (ANSC_HANDLE)pIPv6Pre2;
            pCxtLink->hParentTable   = (ANSC_HANDLE)pIPInterface;
            pCxtLink->InstanceNumber = pIPv6Pre2->InstanceNumber;
            pCxtLink->bNew       = FALSE;

            CosaSListPushEntryByInsNum(&pIPInterface->IPV6PrefixList, (PCOSA_CONTEXT_LINK_OBJECT)pCxtLink);
        }
    }
        
    returnStatus =  ANSC_STATUS_SUCCESS;

    goto EXIT2;
    
EXIT1:
    AnscFreeMemory(pCxtLink);
    
EXIT2:
    AnscFreeMemory(pOldInBE);
    
EXIT3:
    AnscFreeMemory(pIPv6Pre);
    return returnStatus;



}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        IPv6Prefix_AddEntry
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
IPv6Prefix_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_IP              pMyObject        = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext     = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface     = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_IP_V6PREFIX           pIPv6Pre         = (PCOSA_DML_IP_V6PREFIX)NULL;

    pIPv6Pre = (PCOSA_DML_IP_V6PREFIX)AnscAllocateMemory(sizeof(COSA_DML_IP_V6PREFIX));

    if ( !pIPv6Pre )
    {
        return NULL;
    }

    /*we only support adding static prefix*/
    pIPv6Pre->Origin = COSA_DML_IP6PREFIX_ORIGIN_Static;
    pIPv6Pre->StaticType = COSA_DML_IP6PREFIX_STATICTYPE_Static;
    pIPv6Pre->bEnabled = COSA_DML_PREFIXENTRY_STATUS_Enabled;
    AnscCopyString(pIPv6Pre->PreferredLifetime, "0001-01-01T00:00:00Z");
    AnscCopyString(pIPv6Pre->ValidLifetime, "0001-01-01T00:00:00Z");

    _ansc_sprintf(pIPv6Pre->Alias, "IPv6Prefix%d", pIPInterface->ulNextIPV6PreInsNum);

    /* Update the middle layer data */
    if ( TRUE )
    {
        pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory(sizeof(COSA_CONTEXT_LINK_OBJECT));

        if ( !pSubCosaContext )
        {
            AnscFreeMemory(pIPv6Pre);
            
            return NULL;
        }

        pSubCosaContext->InstanceNumber = pIPv6Pre->InstanceNumber =  pIPInterface->ulNextIPV6PreInsNum;

        pIPInterface->ulNextIPV6PreInsNum++;

        if ( 0 == pIPInterface->ulNextIPV6PreInsNum )
        {
            pIPInterface->ulNextIPV6PreInsNum = 1;
        }

        pSubCosaContext->hContext         = (ANSC_HANDLE)pIPv6Pre;
        pSubCosaContext->hParentTable     = pIPInterface;
        pSubCosaContext->hPoamIrepUpperFo = pMyObject->hIrepFolderIPInterfaceHA;
        pSubCosaContext->bNew             = TRUE;

        CosaSListPushEntryByInsNum((PSLIST_HEADER)&pIPInterface->IPV6PrefixList, pSubCosaContext);

        CosaIPRegAddInfo
        (
            (ANSC_HANDLE)pMyObject,
            COSA_DML_RR_NAME_IPIF_IPv6Pre_NextInsNum,
            pIPInterface->ulNextIPV6PreInsNum,
            COSA_DML_RR_NAME_IPIF_IPV6Pre_Prefix,
            pIPInterface->Cfg.InstanceNumber,
            pIPv6Pre->Alias,
            (ANSC_HANDLE)pSubCosaContext
        );
    }

    *pInsNumber = pSubCosaContext->InstanceNumber;

    return pSubCosaContext;

}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv6Prefix_DelEntry
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
IPv6Prefix_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_IP              pMyObject       = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext    = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface    = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;
    PCOSA_CONTEXT_LINK_OBJECT       pSubCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_DML_IP_V6PREFIX           pIPv6Pre        = (PCOSA_DML_IP_V6PREFIX)pSubCosaContext->hContext;

    if (pIPv6Pre->Origin != COSA_DML_IP6PREFIX_ORIGIN_Static)
        return ANSC_STATUS_FAILURE;

    CosaDmlIpIfDelV6Prefix(pMyObject->hSbContext, pIPInterface->Cfg.InstanceNumber, pIPv6Pre);

    AnscSListPopEntryByLink((PSLIST_HEADER)&pIPInterface->IPV6PrefixList, &pSubCosaContext->Linkage);

    if ( pSubCosaContext->bNew )
    {
        CosaIPRegDelInfo(NULL, (ANSC_HANDLE)pSubCosaContext);
    }

    AnscFreeMemory(pIPv6Pre);
    AnscFreeMemory(pSubCosaContext);

    return 0; /* succeeded */
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Prefix_GetParamBoolValue
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
IPv6Prefix_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V6PREFIX           pIPv6Pre     = (PCOSA_DML_IP_V6PREFIX)pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* collect value */
        *pBool = pIPv6Pre->bEnabled;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "OnLink", TRUE))
    {
        /* collect value */
        *pBool = pIPv6Pre->bOnlink;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Autonomous", TRUE))
    {
        /* collect value */
        *pBool = pIPv6Pre->bAutonomous;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Prefix_GetParamIntValue
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
IPv6Prefix_GetParamIntValue
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
        IPv6Prefix_GetParamUlongValue
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
IPv6Prefix_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V6PREFIX           pIPv6Pre     = (PCOSA_DML_IP_V6PREFIX)pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Status", TRUE))
    {
        /* collect value */
        *puLong = pIPv6Pre->Status;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "PrefixStatus", TRUE))
    {
        /* collect value */
        *puLong = pIPv6Pre->PrefixStatus;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Origin", TRUE))
    {
        /* collect value */
        *puLong = pIPv6Pre->Origin;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "StaticType", TRUE))
    {
        /* collect value */
        *puLong = pIPv6Pre->StaticType;
        return TRUE;
    }


    if( AnscEqualString(ParamName, "X_CISCO_COM_PreferredLifetime", TRUE) )
    {
        /* collect value */
        *puLong  = pIPv6Pre->iapd_pretm;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_ValidLifetime", TRUE) )
    {
        /* collect value */
        *puLong  = pIPv6Pre->iapd_vldtm;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv6Prefix_GetParamStringValue
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
IPv6Prefix_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V6PREFIX           pIPv6Pre     = (PCOSA_DML_IP_V6PREFIX)pCosaContext->hContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pIPv6Pre->Alias);
        return 0;
    }

    if( AnscEqualString(ParamName, "Prefix", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pIPv6Pre->Prefix);
        return 0;
    }

    if( AnscEqualString(ParamName, "ParentPrefix", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pIPv6Pre->ParentPrefix);
        return 0;
    }

    if( AnscEqualString(ParamName, "ChildPrefixBits", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pIPv6Pre->ChildPrefixBits);
        return 0;
    }

    if( AnscEqualString(ParamName, "PreferredLifetime", TRUE))
    {
        /* collect value */
		AnscCopyString(pValue, pIPv6Pre->PreferredLifetime);
        return 0;
    }

    if( AnscEqualString(ParamName, "ValidLifetime", TRUE))
    {
        /* collect value */
		AnscCopyString(pValue, pIPv6Pre->ValidLifetime);
        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Prefix_SetParamBoolValue
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
IPv6Prefix_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V6PREFIX           pIPv6Pre     = (PCOSA_DML_IP_V6PREFIX)pCosaContext->hContext;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* save update to backup */
#ifndef _COSA_SIM_
        /*not supported, even for static Prefix, we set Enable always true when adding one entry*/
        return FALSE;
    
#endif

		pIPv6Pre->bEnabled = bValue;
        return TRUE;
    }

    if (pIPv6Pre->Origin != COSA_DML_IP6PREFIX_ORIGIN_Static)
        return FALSE;

    if( AnscEqualString(ParamName, "OnLink", TRUE))
    {
        /* save update to backup */
#ifndef _COSA_SIM_
        /*not supported, even for static Prefix, */
        return FALSE;
    
#endif

		pIPv6Pre->bOnlink = bValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Autonomous", TRUE))
    {
        /* save update to backup */
#ifndef _COSA_SIM_
        /*not supported, even for static Prefix, */
        return FALSE;
    
#endif

		pIPv6Pre->bAutonomous = bValue;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Prefix_SetParamIntValue
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
IPv6Prefix_SetParamIntValue
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
        IPv6Prefix_SetParamUlongValue
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
IPv6Prefix_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V6PREFIX           pIPv6Pre     = (PCOSA_DML_IP_V6PREFIX)pCosaContext->hContext;

    if (pIPv6Pre->Origin != COSA_DML_IP6PREFIX_ORIGIN_Static)
        return FALSE;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Status", TRUE))
    {
        /* save update to backup */
        pIPv6Pre->Status = uValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "PrefixStatus", TRUE))
    {
        /* save update to backup */
        pIPv6Pre->PrefixStatus = uValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "StaticType", TRUE))
    {
#ifndef _COSA_SIM_
        /*not supported*/
        return FALSE;

#endif
        /* save update to backup */
        pIPv6Pre->StaticType = uValue;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Prefix_SetParamStringValue
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
IPv6Prefix_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V6PREFIX           pIPv6Pre     = (PCOSA_DML_IP_V6PREFIX)pCosaContext->hContext;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE))
    {
        /* save update to backup */
        AnscCopyString(pIPv6Pre->Alias, pString);
        return TRUE;
    }

    if (pIPv6Pre->Origin != COSA_DML_IP6PREFIX_ORIGIN_Static)
        return FALSE;

    if( AnscEqualString(ParamName, "Prefix", TRUE))
    {
        /* save update to backup */
        AnscCopyString(pIPv6Pre->Prefix, pString);
        return TRUE;
    }

    if( AnscEqualString(ParamName, "ParentPrefix", TRUE))
    {
        /* save update to backup */
#ifndef _COSA_SIM_
        return FALSE;
#endif
		AnscCopyString(pIPv6Pre->ParentPrefix, pString);
        return TRUE;
    }

    if( AnscEqualString(ParamName, "ChildPrefixBits", TRUE))
    {
        /* save update to backup */
#ifndef _COSA_SIM_
        return FALSE;
#endif
		AnscCopyString(pIPv6Pre->ChildPrefixBits, pString);
        return TRUE;
    }

    if( AnscEqualString(ParamName, "PreferredLifetime", TRUE))
    {
        /* save update to backup */
		AnscCopyString(pIPv6Pre->PreferredLifetime, pString);
        return TRUE;
    }

    if( AnscEqualString(ParamName, "ValidLifetime", TRUE))
    {
        /* save update to backup */
		AnscCopyString(pIPv6Pre->ValidLifetime, pString);
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv6Prefix_Validate
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
IPv6Prefix_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext  = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext2 = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PCOSA_DML_IP_IF_FULL2           pIPInterface  = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hParentTable;
    PCOSA_DML_IP_V6PREFIX           pIPv6Pre      = (PCOSA_DML_IP_V6PREFIX)pCosaContext->hContext;
    PCOSA_DML_IP_V6PREFIX           pIPv6Pre2     = (PCOSA_DML_IP_V6PREFIX)NULL;
    PSINGLE_LINK_ENTRY              pLink         = (PSINGLE_LINK_ENTRY)NULL;
    struct in6_addr                 in6           = {0};
    char *                          p             =  NULL;

    pLink = AnscSListGetFirstEntry(&pIPInterface->IPV6PrefixList);

    while ( pLink )
    {
        pCosaContext2 = ACCESS_COSA_CONTEXT_LINK_OBJECT(pLink);
        pLink   = AnscSListGetNextEntry(pLink);

        pIPv6Pre2 = (PCOSA_DML_IP_V6PREFIX)pCosaContext2->hContext;

        if ( pIPv6Pre2 &&
            ((ULONG)pIPv6Pre2 != (ULONG)pIPv6Pre ) &&
             AnscEqualString(pIPv6Pre2->Alias, pIPv6Pre->Alias, TRUE))
        {
            AnscCopyString(pReturnParamName, "Alias");
            *puLength = AnscSizeOfString("Alias");

            CcspTraceWarning(("IPv6Prefix_Validate() on Alias failed.\n"));            
            return FALSE;
        }

        if ( pIPv6Pre2 &&
            (pIPv6Pre2->bEnabled && pIPv6Pre->bEnabled) &&
             ((ULONG)pIPv6Pre2 != (ULONG)pIPv6Pre ) &&
             CosaDmlV6PrefIsEqual(pIPv6Pre2->Prefix, pIPv6Pre->Prefix))
        {
            _ansc_strcpy(pReturnParamName, "Prefix");
            *puLength = AnscSizeOfString("Prefix");
    
            CcspTraceWarning(("IPv6Prefix_Validate() on Prefix failed.\n"));            
            return FALSE;
        }
    }

    if (!(p= strchr(pIPv6Pre->Prefix, '/')))
    {
        _ansc_strcpy(pReturnParamName, "Prefix");
        *puLength = AnscSizeOfString("Prefix");
    
        CcspTraceWarning(("IPv6Prefix_Validate() on Prefix failed.\n"));
        return FALSE;
    }
    else 
    {
        char * dup = AnscCloneString(pIPv6Pre->Prefix);
        
        p = strchr(dup, '/');
        if (p)
        {
            *p = 0;
        
            if (inet_pton(AF_INET6, dup, &in6) != 1)
            {
                _ansc_strcpy(pReturnParamName, "Prefix");
                *puLength = AnscSizeOfString("Prefix");

                CcspTraceWarning(("IPv6Prefix_Validate() on Prefix failed.\n"));
                AnscFreeMemory(dup);/*RDKB-, CID-33094, free unused resources before exit*/
                return FALSE;
            }
        }
        AnscFreeMemory(dup);
        
    }



    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv6Prefix_Commit
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
IPv6Prefix_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_IP              pMyObject    = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V6PREFIX           pIPv6Pre     = (PCOSA_DML_IP_V6PREFIX)pCosaContext->hContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hParentTable;

    if ( pCosaContext->bNew )
    {
        pCosaContext->bNew = FALSE;

        CosaDmlIpIfAddV6Prefix(pMyObject->hSbContext, pIPInterface->Cfg.InstanceNumber, pIPv6Pre);

        CosaIPRegDelInfo(NULL, (ANSC_HANDLE)pCosaContext);
    }
    else
    {
        CosaDmlIpIfSetV6Prefix(pMyObject->hSbContext, pIPInterface->Cfg.InstanceNumber, pIPv6Pre);
    }

    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv6Prefix_Rollback
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
IPv6Prefix_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_IP              pMyObject    = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_V6PREFIX           pIPv6Pre     = (PCOSA_DML_IP_V6PREFIX)pCosaContext->hContext;
    PCOSA_DML_IP_IF_FULL2           pIPInterface = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hParentTable;

    CcspTraceWarning(("IPv6Prefix_Rollback()----begin to rollback......\n"));

    CosaDmlIpIfGetV6Prefix2(pMyObject->hSbContext, pIPInterface->Cfg.InstanceNumber, pIPv6Pre);

    return 0;
}

/***********************************************************************

 APIs for Object:

    IP.Interface.{i}.Stats.

    *  Stats5_GetParamBoolValue
    *  Stats5_GetParamIntValue
    *  Stats5_GetParamUlongValue
    *  Stats5_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Stats5_GetParamBoolValue
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
Stats5_GetParamBoolValue
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
        Stats5_GetParamIntValue
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
Stats5_GetParamIntValue
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
        Stats5_GetParamUlongValue
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
Stats5_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DATAMODEL_IP              pMyObject    = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP; 
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PCOSA_DML_IP_STATS              pIPStats      = (PCOSA_DML_IP_STATS)AnscAllocateMemory(sizeof(COSA_DML_IP_STATS));
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContext = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_IP_IF_FULL2           pIfFull = (PCOSA_DML_IP_IF_FULL2)pCosaContext->hContext;

    if( !pIPStats )
    {
        return FALSE;
    }
   
    returnStatus = CosaDmlIpIfGetStats(pMyObject->hSbContext, pIfFull->Cfg.InstanceNumber, pIPStats);

    if( returnStatus != ANSC_STATUS_SUCCESS )
    {
        goto FAILED;
    }

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "BytesSent", TRUE))
    {
        /* collect value */
        *puLong = pIPStats->BytesSent;
        goto SUCCESS;
    }

    if( AnscEqualString(ParamName, "BytesReceived", TRUE))
    {
        /* collect value */
        *puLong = pIPStats->BytesReceived;
        goto SUCCESS;
    }

    if( AnscEqualString(ParamName, "PacketsSent", TRUE))
    {
        /* collect value */
        *puLong = pIPStats->PacketsSent;
        goto SUCCESS;
    }

    if( AnscEqualString(ParamName, "PacketsReceived", TRUE))
    {
        /* collect value */
        *puLong = pIPStats->PacketsReceived;
        goto SUCCESS;
    }

    if( AnscEqualString(ParamName, "ErrorsSent", TRUE))
    {
        /* collect value */
        *puLong = pIPStats->ErrorsSent;
        goto SUCCESS;
    }

    if( AnscEqualString(ParamName, "ErrorsReceived", TRUE))
    {
        /* collect value */
        *puLong = pIPStats->ErrorsReceived;
        goto SUCCESS;
    }

    if( AnscEqualString(ParamName, "UnicastPacketsSent", TRUE))
    {
        /* collect value */
        *puLong = pIPStats->UnicastPacketsSent;
        goto SUCCESS;
    }

    if( AnscEqualString(ParamName, "UnicastPacketsReceived", TRUE))
    {
        /* collect value */
        *puLong = pIPStats->UnicastPacketsReceived;
        goto SUCCESS;
    }

    if( AnscEqualString(ParamName, "DiscardPacketsSent", TRUE))
    {
        /* collect value */
        *puLong = pIPStats->DiscardPacketsSent;
        goto SUCCESS;
    }

    if( AnscEqualString(ParamName, "DiscardPacketsReceived", TRUE))
    {
        /* collect value */
        *puLong = pIPStats->DiscardPacketsReceived;
        goto SUCCESS;
    }

    if( AnscEqualString(ParamName, "MulticastPacketsSent", TRUE))
    {
        /* collect value */
        *puLong = pIPStats->MulticastPacketsSent;
        goto SUCCESS;
    }

    if( AnscEqualString(ParamName, "MulticastPacketsReceived", TRUE))
    {
        /* collect value */
        *puLong = pIPStats->MulticastPacketsReceived;
        goto SUCCESS;
    }

    if( AnscEqualString(ParamName, "BroadcastPacketsSent", TRUE))
    {
        /* collect value */
        *puLong = pIPStats->BroadcastPacketsSent;
        goto SUCCESS;
    }

    if( AnscEqualString(ParamName, "BroadcastPacketsReceived", TRUE))
    {
        /* collect value */
        *puLong = pIPStats->BroadcastPacketsReceived;
        goto SUCCESS;
    }

    if( AnscEqualString(ParamName, "UnknownProtoPacketsReceived", TRUE))
    {
        /* collect value */
        *puLong = pIPStats->UnknownProtoPacketsReceived;
        goto SUCCESS;
    }

SUCCESS:
    AnscFreeMemory(pIPStats);
    return TRUE;

FAILED:
    AnscFreeMemory(pIPStats);
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Stats5_GetParamStringValue
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
Stats5_GetParamStringValue
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

    IP.ActivePort.{i}.

    *  ActivePort_GetEntryCount
    *  ActivePort_GetEntry
    *  ActivePort_IsUpdated
    *  ActivePort_Synchronize
    *  ActivePort_GetParamBoolValue
    *  ActivePort_GetParamIntValue
    *  ActivePort_GetParamUlongValue
    *  ActivePort_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        ActivePort_GetEntryCount
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
ActivePort_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_IP              pMyObject     = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP;
    ULONG                           entryCount    = 0;
    
    entryCount = pMyObject->activePortCount;
    
    return entryCount;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        ActivePort_GetEntry
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
ActivePort_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_IP              pMyObject     = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP;
    ULONG                           entryCount    = 0;
    PCOSA_DML_IP_ACTIVE_PORT        pActivePort   = (PCOSA_DML_IP_ACTIVE_PORT)pMyObject->pActivePort;
    
    entryCount   = pMyObject->activePortCount;
    *pInsNumber  = nIndex + 1; 

    if( nIndex > (entryCount - 1))
    {
        return NULL;
    }
    
    return (ANSC_HANDLE)&pActivePort[nIndex];
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        ActivePort_IsUpdated
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
ActivePort_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    )
{
    if (!last_tick) 
    {
        last_tick = AnscGetTickInSeconds();
        return TRUE;
    }
    
    if (last_tick >= TIME_NO_NEGATIVE(AnscGetTickInSeconds() - REFRESH_INTERVAL))
    {
        return FALSE;
    }
    else 
    {
        last_tick = AnscGetTickInSeconds();
        return TRUE;
    }
    
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        ActivePort_Synchronize
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
ActivePort_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_IP              pMyObject     = (PCOSA_DATAMODEL_IP)g_pCosaBEManager->hIP;
    PCOSA_DML_IP_ACTIVE_PORT        pActivePort   = (PCOSA_DML_IP_ACTIVE_PORT)NULL;
    ULONG                           activePortCount 
                                                  = 0;
    
    /*pActivePort                = CosaDmlIpGetActivePorts(pMyObject->hSbContext,&activePortCount);*/
    pMyObject->activePortCount = 0; /*activePortCount;*/

    if( pMyObject->pActivePort )
    {
        AnscFreeMemory(pMyObject->pActivePort);
        pMyObject->pActivePort = NULL;
    }
    
    return returnStatus;

    pMyObject->pActivePort     = (PCOSA_DML_IP_ACTIVE_PORT)
                                 AnscAllocateMemory(sizeof(COSA_DML_IP_ACTIVE_PORT)*activePortCount);
    
    if( !pMyObject->pActivePort )
    {
        AnscFreeMemory(pActivePort);
        return ANSC_STATUS_RESOURCES;
    }
    
    AnscCopyMemory(pMyObject->pActivePort,pActivePort,sizeof(COSA_DML_IP_ACTIVE_PORT)*activePortCount);

    AnscFreeMemory(pActivePort);
    return returnStatus;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        ActivePort_GetParamBoolValue
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
ActivePort_GetParamBoolValue
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
        ActivePort_GetParamIntValue
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
ActivePort_GetParamIntValue
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
        ActivePort_GetParamUlongValue
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
ActivePort_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DML_IP_ACTIVE_PORT        pActivePort   = (PCOSA_DML_IP_ACTIVE_PORT)hInsContext;
    
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "LocalPort", TRUE))
    {
        /* collect value */
        *puLong = pActivePort->LocalPort;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "RemotePort", TRUE))
    {
        /* collect value */
        *puLong = pActivePort->RemotePort;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Status", TRUE))
    {
        /* collect value */
        *puLong = pActivePort->Status;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        ActivePort_GetParamStringValue
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
ActivePort_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_DML_IP_ACTIVE_PORT        pActivePort   = (PCOSA_DML_IP_ACTIVE_PORT)hInsContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "LocalIPAddress", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pActivePort->LocalIPAddress);
        return 0;
    }

    if( AnscEqualString(ParamName, "RemoteIPAddress", TRUE))
    {
        /* collect value */
        AnscCopyString(pValue, pActivePort->RemoteIPAddress);
        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

#if 0
/***********************************************************************

 APIs for Object:

    IP.Diagnostics.


***********************************************************************/
/***********************************************************************

 APIs for Object:

    IP.Diagnostics.X_CISCO_COM_ARP.

    *  X_CISCO_COM_ARP_GetParamBoolValue
    *  X_CISCO_COM_ARP_GetParamIntValue
    *  X_CISCO_COM_ARP_GetParamUlongValue
    *  X_CISCO_COM_ARP_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_ARP_GetParamBoolValue
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
X_CISCO_COM_ARP_GetParamBoolValue
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
        X_CISCO_COM_ARP_GetParamIntValue
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
X_CISCO_COM_ARP_GetParamIntValue
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
        X_CISCO_COM_ARP_GetParamUlongValue
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
X_CISCO_COM_ARP_GetParamUlongValue
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
        X_CISCO_COM_ARP_GetParamStringValue
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
X_CISCO_COM_ARP_GetParamStringValue
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

    IP.Diagnostics.X_CISCO_COM_ARP.Table.{i}.

    *  ARPTable_GetEntryCount
    *  ARPTable_GetEntry
    *  ARPTable_GetParamBoolValue
    *  ARPTable_GetParamIntValue
    *  ARPTable_GetParamUlongValue
    *  ARPTable_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        ARPTable_GetEntryCount
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
ARPTable_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PCOSA_DML_DIAG_ARP_TABLE        pArpTable           = (PCOSA_DML_DIAG_ARP_TABLE)pMyObject->pArpTable;
    ULONG                           entryCount          = pMyObject->ArpEntryCount;

    return entryCount;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        ARPTable_GetEntry
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
ARPTable_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PCOSA_DML_DIAG_ARP_TABLE        pArpTable           = (PCOSA_DML_DIAG_ARP_TABLE)pMyObject->pArpTable;
    ULONG                           entryCount          = pMyObject->ArpEntryCount;

    *pInsNumber  = nIndex + 1; 
    
    return (ANSC_HANDLE)&pArpTable[nIndex]; /* return the handle */
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        ARPTable_IsUpdated
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
ARPTable_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PCOSA_DML_DIAG_ARP_TABLE        pArpTable           = (PCOSA_DML_DIAG_ARP_TABLE)pMyObject->pArpTable;
    ULONG                           entryCount          = pMyObject->ArpEntryCount;
    BOOL                            bIsUpdated   = TRUE;

    /* 
        We can use one rough granularity interval to get whole table in case 
        that the updating is too frequent.
        */
    if ( ( AnscGetTickInSeconds() - pMyObject->PreviousVisitTime ) < COSA_DML_DIAG_ARP_TABLE_ACCESS_INTERVAL )
    {
        bIsUpdated  = FALSE;
    }
    else
    {
        pMyObject->PreviousVisitTime =  AnscGetTickInSeconds();
        bIsUpdated  = TRUE;
    }

    return bIsUpdated;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        ARPTable_Synchronize
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
ARPTable_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_FAILURE;
    PCOSA_DATAMODEL_DIAG            pMyObject         = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PCOSA_DML_DIAG_ARP_TABLE        pArpTable         = (PCOSA_DML_DIAG_ARP_TABLE)pMyObject->pArpTable;
    ULONG                           entryCount        = pMyObject->ArpEntryCount;
    PCOSA_DML_DIAG_ARP_TABLE        pArpTable2        = NULL;

    pArpTable2         = CosaDmlDiagGetARPTable(NULL,&entryCount);
    if ( !pArpTable2 )
    {
        /* Get Error, we don't del link because next time, it may be successful */
        return ANSC_STATUS_FAILURE;
    }

    if ( pArpTable )
    {
        AnscFreeMemory(pArpTable);
    }

    pMyObject->pArpTable     = pArpTable2;
    pMyObject->ArpEntryCount = entryCount;

    returnStatus =  ANSC_STATUS_SUCCESS;

    return returnStatus;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        ARPTable_GetParamBoolValue
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
ARPTable_GetParamBoolValue
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
        ARPTable_GetParamIntValue
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
ARPTable_GetParamIntValue
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
        ARPTable_GetParamUlongValue
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
ARPTable_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DML_DIAG_ARP_TABLE        pArpTable           = (PCOSA_DML_DIAG_ARP_TABLE)hInsContext;

    /* check the parameter name and return the corresponding value */

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "IPAddress", TRUE))
    {
        /* collect value */
        *puLong    =  pArpTable->IPAddress.Value;
        
        return TRUE;
    }


    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        ARPTable_GetParamStringValue
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
ARPTable_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_DML_DIAG_ARP_TABLE        pArpTable           = (PCOSA_DML_DIAG_ARP_TABLE)hInsContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "MACAddress", TRUE))
    {
        /* collect value */
        if ( sizeof(pArpTable->MACAddress) <= *pUlSize)
        {
            _ansc_sprintf
                (
                    pValue, 
                    "%02x:%02x:%02x:%02x:%02x:%02x", 
                    pArpTable->MACAddress[0],
                    pArpTable->MACAddress[1],
                    pArpTable->MACAddress[2],
                    pArpTable->MACAddress[3],
                    pArpTable->MACAddress[4],
                    pArpTable->MACAddress[5]
                );
            
            return 0;
        }
        else
        {
            *pUlSize = sizeof(pArpTable->MACAddress);
            return 1;
        }
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/***********************************************************************

 APIs for Object:

    IP.Diagnostics.IPPing.

    *  IPPing_GetParamBoolValue
    *  IPPing_GetParamIntValue
    *  IPPing_GetParamUlongValue
    *  IPPing_GetParamStringValue
    *  IPPing_SetParamBoolValue
    *  IPPing_SetParamIntValue
    *  IPPing_SetParamUlongValue
    *  IPPing_SetParamStringValue
    *  IPPing_Validate
    *  IPPing_Commit
    *  IPPing_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPPing_GetParamBoolValue
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
IPPing_GetParamBoolValue
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
        IPPing_GetParamIntValue
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
IPPing_GetParamIntValue
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
        IPPing_GetParamUlongValue
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
IPPing_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_PING_INFO                 pDiagPingInfo       = pMyObject->hDiagPingInfo;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "DiagnosticsState", TRUE))
    {
        pDiagPingInfo = (PDSLH_PING_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Ping);

        if ( pDiagPingInfo )
        {
            *puLong = pDiagPingInfo->DiagnosticState + 1;
        }
        else
        {
            CcspTraceWarning(("Failed to get Ping DiagnosticsState parameter!\n"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if( AnscEqualString(ParamName, "NumberOfRepetitions", TRUE))
    {
        if ( pDiagPingInfo )
        {
            *puLong = pDiagPingInfo->NumberOfRepetitions;
        }
        else
        {
            CcspTraceWarning(("Failed to get Ping NumberOfRepetitions parameter!\n"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if( AnscEqualString(ParamName, "Timeout", TRUE))
    {
        if ( pDiagPingInfo )
        {
            *puLong = pDiagPingInfo->Timeout;
        }
        else
        {
            CcspTraceWarning(("Failed to get Ping Timeout parameter!\n"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if( AnscEqualString(ParamName, "DataBlockSize", TRUE))
    {
        if ( pDiagPingInfo )
        {
            *puLong = pDiagPingInfo->DataBlockSize;
        }
        else
        {
            CcspTraceWarning(("Failed to get Ping DataBlockSize parameter!\n"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if( AnscEqualString(ParamName, "DSCP", TRUE))
    {
        if ( pDiagPingInfo )
        {
            *puLong = pDiagPingInfo->DSCP;
        }
        else
        {
            CcspTraceWarning(("Failed to get Ping DSCP parameter!\n"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if( AnscEqualString(ParamName, "SuccessCount", TRUE))
    {
        pDiagPingInfo = (PDSLH_PING_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Ping);

        if ( pDiagPingInfo )
        {
            *puLong = pDiagPingInfo->SuccessCount;
        }
        else
        {
            CcspTraceWarning(("Failed to get Ping SuccessCount parameter!\n"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if( AnscEqualString(ParamName, "FailureCount", TRUE))
    {
        pDiagPingInfo = (PDSLH_PING_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Ping);

        if ( pDiagPingInfo )
        {
            *puLong = pDiagPingInfo->FailureCount;
        }
        else
        {
            CcspTraceWarning(("Failed to get Ping FailureCount parameter!\n"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if( AnscEqualString(ParamName, "AverageResponseTime", TRUE))
    {
        pDiagPingInfo = (PDSLH_PING_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Ping);

        if ( pDiagPingInfo )
        {
            *puLong = pDiagPingInfo->AverageResponseTime;
        }
        else
        {
            CcspTraceWarning(("Failed to get Ping AverageResponseTime parameter!\n"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if( AnscEqualString(ParamName, "MinimumResponseTime", TRUE))
    {
        pDiagPingInfo = (PDSLH_PING_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Ping);

        if ( pDiagPingInfo )
        {
            *puLong = pDiagPingInfo->MinimumResponseTime;
        }
        else
        {
            CcspTraceWarning(("Failed to get Ping MinimumResponseTime parameter!\n"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if( AnscEqualString(ParamName, "MaximumResponseTime", TRUE))
    {
        pDiagPingInfo = (PDSLH_PING_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Ping);

        if ( pDiagPingInfo )
        {
            *puLong = pDiagPingInfo->MaximumResponseTime;
        }
        else
        {
            CcspTraceWarning(("Failed to get Ping MaximumResponseTime parameter!\n"));
            *puLong = 0;
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

        ULONG
        IPPing_GetParamStringValue
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
IPPing_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_PING_INFO                 pDiagPingInfo       = pMyObject->hDiagPingInfo;

    if( AnscEqualString(ParamName, "Interface", TRUE))
    {
        if ( pDiagPingInfo && AnscSizeOfString(pDiagPingInfo->Interface) < *pUlSize )
        {
            AnscCopyString(pValue, pDiagPingInfo->Interface);
        }
        else
        {
            if ( AnscSizeOfString(pDiagPingInfo->Interface) >= *pUlSize )
            {
                *pUlSize = AnscSizeOfString(pDiagPingInfo->Interface) + 1;
            }

            CcspTraceWarning(("Failed to get Ping Interface parameter!\n"));

            return -1;
        }
        return 0;
    }

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Host", TRUE))
    {
        if ( pDiagPingInfo && AnscSizeOfString(pDiagPingInfo->Host) < *pUlSize )
        {
            AnscCopyString(pValue, pDiagPingInfo->Host);
        }
        else
        {
            if ( AnscSizeOfString(pDiagPingInfo->Host) >= *pUlSize )
            {
                *pUlSize = AnscSizeOfString(pDiagPingInfo->Host) + 1;
            }

            CcspTraceWarning(("Failed to get Ping Host parameter!\n"));
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
        IPPing_SetParamBoolValue
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
IPPing_SetParamBoolValue
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
        IPPing_SetParamIntValue
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
IPPing_SetParamIntValue
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
        IPPing_SetParamUlongValue
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
IPPing_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_PING_INFO                 pDiagPingInfo       = pMyObject->hDiagPingInfo;
    PDSLH_PING_INFO                 pDiagInfo           = NULL;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "DiagnosticsState", TRUE))
    {
        if ( (uValue - 1) != (ULONG)DSLH_DIAG_STATE_TYPE_Requested )
        {
            return FALSE;
        }

        pDiagPingInfo->DiagnosticState = uValue - 1;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "NumberOfRepetitions", TRUE))
    {
        pDiagPingInfo->NumberOfRepetitions = uValue;

        pDiagInfo = (PDSLH_PING_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Ping);

        if ( pDiagInfo && pDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Inprogress )
        {
            pDiagPingInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_Canceled;
        }
        else
        {
            pDiagPingInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
        }

        return TRUE;
    }

    if( AnscEqualString(ParamName, "Timeout", TRUE))
    {
        pDiagPingInfo->Timeout             = uValue;

        pDiagInfo = (PDSLH_PING_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Ping);

        if ( pDiagInfo && pDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Inprogress )
        {
            pDiagPingInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_Canceled;
        }
        else
        {
            pDiagPingInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
        }

        return TRUE;
    }

    if( AnscEqualString(ParamName, "DataBlockSize", TRUE))
    {
        pDiagPingInfo->DataBlockSize       = uValue;

        pDiagInfo = (PDSLH_PING_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Ping);

        if ( pDiagInfo && pDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Inprogress )
        {
            pDiagPingInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_Canceled;
        }
        else
        {
            pDiagPingInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
        }

        return TRUE;
    }

    if( AnscEqualString(ParamName, "DSCP", TRUE))
    {
        pDiagPingInfo->DSCP                = uValue;

        pDiagInfo = (PDSLH_PING_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Ping);

        if ( pDiagInfo && pDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Inprogress )
        {
            pDiagPingInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_Canceled;
        }
        else
        {
            pDiagPingInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
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
        IPPing_SetParamStringValue
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
IPPing_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_PING_INFO                 pDiagPingInfo       = pMyObject->hDiagPingInfo;
    PDSLH_PING_INFO                 pDiagInfo           = NULL;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Interface", TRUE))
    {
        AnscCopyString(pDiagPingInfo->Interface, pString);

        pDiagInfo = (PDSLH_PING_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Ping);

        if ( pDiagInfo && pDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Inprogress )
        {
            pDiagPingInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_Canceled;
        }
        else
        {
            pDiagPingInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
        }

        return TRUE;
    }

    if( AnscEqualString(ParamName, "Host", TRUE))
    {
        AnscCopyString(pDiagPingInfo->Host, pString);

        pDiagInfo = (PDSLH_PING_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Ping);

        if ( pDiagInfo && pDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Inprogress )
        {
            pDiagPingInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_Canceled;
        }
        else
        {
            pDiagPingInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
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
        IPPing_Validate
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
IPPing_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_PING_INFO                 pDiagPingInfo       = pMyObject->hDiagPingInfo;

    if ( !pDiagPingInfo )
    {
        return FALSE;
    }

    if ( pDiagPingInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Requested )
    {
        if ( AnscSizeOfString(pDiagPingInfo->Host) == 0 )
        {
            AnscCopyString(pReturnParamName, "Host");

            pDiagPingInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;

            return FALSE;
        }
    }

    if ( TRUE ) 
    {
        if ( pDiagPingInfo->NumberOfRepetitions < DSLH_PING_MIN_NumberOfRepetitions )
        {
            AnscCopyString(pReturnParamName, "NumberOfRepetitions");

            return FALSE;
        }
    }

    if ( TRUE ) 
    {
        if ( pDiagPingInfo->Timeout < DSLH_PING_MIN_Timeout )
        {
            AnscCopyString(pReturnParamName, "Timeout");

            return FALSE;
        }
    }

    if ( TRUE ) 
    {
        if ( pDiagPingInfo->DataBlockSize < DSLH_PING_MIN_DataBlockSize || pDiagPingInfo->DataBlockSize > DSLH_PING_MAX_DataBlockSize )
        {
            AnscCopyString(pReturnParamName, "DataBlockSize");

            return FALSE;
        }
    }

    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPPing_Commit
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
IPPing_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_PING_INFO                 pDiagPingInfo       = pMyObject->hDiagPingInfo;
    PDSLH_PING_INFO                 pDiagInfo           = NULL;

    if ( !pDiagPingInfo )
    {
        return ANSC_STATUS_FAILURE;
    }

    pDiagInfo = (PDSLH_PING_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Ping);

    if ( pDiagPingInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Requested )
    {
        if ( pDiagInfo && pDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Inprogress )
        {
            pDiagPingInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_Canceled;
            CosaDmlDiagCancelDiagnostic(DSLH_DIAGNOSTIC_TYPE_Ping);
            pDiagPingInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_Requested;
            AnscSleep(1000);
        }

        CosaDmlDiagScheduleDiagnostic(DSLH_DIAGNOSTIC_TYPE_Ping, (ANSC_HANDLE)pDiagPingInfo);

        pDiagPingInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
    }

    if ( pDiagPingInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Canceled )
    {
        CosaDmlDiagCancelDiagnostic(DSLH_DIAGNOSTIC_TYPE_Ping);

        pDiagPingInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
    }

    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPPing_Rollback
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
IPPing_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_PING_INFO                 pDiagPingInfo       = pMyObject->hDiagPingInfo;
    PDSLH_PING_INFO                 pDiagInfo           = NULL;

    if ( !pDiagPingInfo )
    {
        return ANSC_STATUS_FAILURE;
    }

    pDiagInfo = (PDSLH_PING_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Ping);

    if ( pDiagInfo )
    {
        DslhInitPingInfo(pDiagPingInfo);
        pDiagPingInfo->StructSize                    = sizeof(DSLH_PING_INFO);
        AnscCopyString(pDiagPingInfo->Host           , pDiagInfo->Host     );
        AnscCopyString(pDiagPingInfo->Interface      , pDiagInfo->Interface);
        pDiagPingInfo->Timeout                       = pDiagInfo->Timeout;
        pDiagPingInfo->NumberOfRepetitions           = pDiagInfo->NumberOfRepetitions;
        pDiagPingInfo->DataBlockSize                 = pDiagInfo->DataBlockSize;
    }
    else
    {
        DslhInitPingInfo(pDiagPingInfo);
    }

    return 0;
}

/***********************************************************************

 APIs for Object:

    IP.Diagnostics.TraceRoute.

    *  TraceRoute_GetParamBoolValue
    *  TraceRoute_GetParamIntValue
    *  TraceRoute_GetParamUlongValue
    *  TraceRoute_GetParamStringValue
    *  TraceRoute_SetParamBoolValue
    *  TraceRoute_SetParamIntValue
    *  TraceRoute_SetParamUlongValue
    *  TraceRoute_SetParamStringValue
    *  TraceRoute_Validate
    *  TraceRoute_Commit
    *  TraceRoute_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        TraceRoute_GetParamBoolValue
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
TraceRoute_GetParamBoolValue
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
        TraceRoute_GetParamIntValue
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
TraceRoute_GetParamIntValue
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
        TraceRoute_GetParamUlongValue
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
TraceRoute_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TRACEROUTE_INFO           pDiagTracerouteInfo = pMyObject->hDiagTracerouteInfo;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "DiagnosticsState", TRUE))
    {
        pDiagTracerouteInfo = (PDSLH_TRACEROUTE_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Traceroute);

        if ( pDiagTracerouteInfo )
        {
            *puLong = pDiagTracerouteInfo->DiagnosticState + 1;
        }
        else
        {
            CcspTraceWarning(("Failed to get Traceroute DiagnosticsState parameter!\n"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if( AnscEqualString(ParamName, "NumberOfTries", TRUE))
    {
        if ( pDiagTracerouteInfo )
        {
            *puLong = pDiagTracerouteInfo->NumberOfTries;
        }
        else
        {
            CcspTraceWarning(("Failed to get Traceroute NumberOfTries parameter!\n"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if( AnscEqualString(ParamName, "Timeout", TRUE))
    {
        if ( pDiagTracerouteInfo )
        {
            *puLong = pDiagTracerouteInfo->Timeout;
        }
        else
        {
            CcspTraceWarning(("Failed to get Traceroute Timeout parameter!\n"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if( AnscEqualString(ParamName, "DataBlockSize", TRUE))
    {
        if ( pDiagTracerouteInfo )
        {
            *puLong = pDiagTracerouteInfo->DataBlockSize;
        }
        else
        {
            CcspTraceWarning(("Failed to get Traceroute DataBlockSize parameter!\n"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if( AnscEqualString(ParamName, "DSCP", TRUE))
    {
        if ( pDiagTracerouteInfo )
        {
            *puLong = pDiagTracerouteInfo->DSCP;
        }
        else
        {
            CcspTraceWarning(("Failed to get Traceroute DSCP parameter!\n"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if( AnscEqualString(ParamName, "MaxHopCount", TRUE))
    {
        if ( pDiagTracerouteInfo )
        {
            *puLong = pDiagTracerouteInfo->MaxHopCount;
        }
        else
        {
            CcspTraceWarning(("Failed to get Traceroute MaxHopCount parameter!\n"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if( AnscEqualString(ParamName, "ResponseTime", TRUE))
    {
        pDiagTracerouteInfo = (PDSLH_TRACEROUTE_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Traceroute);

        if ( pDiagTracerouteInfo )
        {
            *puLong = pDiagTracerouteInfo->ResponseTime;
        }
        else
        {
            CcspTraceWarning(("Failed to get Traceroute ResponseTime parameter!\n"));
            *puLong = 0;
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

        ULONG
        TraceRoute_GetParamStringValue
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
TraceRoute_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TRACEROUTE_INFO           pDiagTracerouteInfo = pMyObject->hDiagTracerouteInfo;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Interface", TRUE))
    {
        if ( pDiagTracerouteInfo && AnscSizeOfString(pDiagTracerouteInfo->Interface) < *pUlSize )
        {
            AnscCopyString(pValue, pDiagTracerouteInfo->Interface);
        }
        else
        {
            if ( AnscSizeOfString(pDiagTracerouteInfo->Interface) >= *pUlSize )
            {
                *pUlSize = AnscSizeOfString(pDiagTracerouteInfo->Interface) + 1;
            }

            CcspTraceWarning(("Failed to get Traceroute Interface parameter!\n"));
            return -1;
        }

        return 0;
    }

    if( AnscEqualString(ParamName, "Host", TRUE))
    {
        if ( pDiagTracerouteInfo && AnscSizeOfString(pDiagTracerouteInfo->Host) < *pUlSize )
        {
            AnscCopyString(pValue, pDiagTracerouteInfo->Host);
        }
        else
        {
            if ( AnscSizeOfString(pDiagTracerouteInfo->Host) >= *pUlSize )
            {
                *pUlSize = AnscSizeOfString(pDiagTracerouteInfo->Host) + 1;
            }

            CcspTraceWarning(("Failed to get Traceroute Host parameter!\n"));
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
        TraceRoute_SetParamBoolValue
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
TraceRoute_SetParamBoolValue
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
        TraceRoute_SetParamIntValue
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
TraceRoute_SetParamIntValue
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
        TraceRoute_SetParamUlongValue
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
TraceRoute_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TRACEROUTE_INFO           pDiagTracerouteInfo = pMyObject->hDiagTracerouteInfo;
    PDSLH_TRACEROUTE_INFO           pDiagInfo           = NULL;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "DiagnosticsState", TRUE))
    {
        if ( (uValue - 1) != (ULONG)DSLH_DIAG_STATE_TYPE_Requested )
        {
            return FALSE;
        }

        pDiagTracerouteInfo->DiagnosticState = uValue - 1;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "NumberOfTries", TRUE))
    {
        pDiagTracerouteInfo->NumberOfTries = uValue;

        pDiagInfo = (PDSLH_TRACEROUTE_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Traceroute);

        if ( pDiagInfo && pDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Inprogress )
        {
            pDiagTracerouteInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_Canceled;
        }
        else
        {
            pDiagTracerouteInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
        }

        return TRUE;
    }

    if( AnscEqualString(ParamName, "Timeout", TRUE))
    {
        pDiagTracerouteInfo->Timeout = uValue;

        pDiagInfo = (PDSLH_TRACEROUTE_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Traceroute);

        if ( pDiagInfo && pDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Inprogress )
        {
            pDiagTracerouteInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_Canceled;
        }
        else
        {
            pDiagTracerouteInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
        }

        return TRUE;
    }

    if( AnscEqualString(ParamName, "DataBlockSize", TRUE))
    {
        pDiagTracerouteInfo->DataBlockSize = uValue;

        pDiagInfo = (PDSLH_TRACEROUTE_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Traceroute);

        if ( pDiagInfo && pDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Inprogress )
        {
            pDiagTracerouteInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_Canceled;
        }
        else
        {
            pDiagTracerouteInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
        }

        return TRUE;
    }

    if( AnscEqualString(ParamName, "DSCP", TRUE))
    {
        pDiagTracerouteInfo->DSCP = uValue;

        pDiagInfo = (PDSLH_TRACEROUTE_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Traceroute);

        if ( pDiagInfo && pDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Inprogress )
        {
            pDiagTracerouteInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_Canceled;
        }
        else
        {
            pDiagTracerouteInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
        }

        return TRUE;
    }

    if( AnscEqualString(ParamName, "MaxHopCount", TRUE))
    {
        pDiagTracerouteInfo->MaxHopCount = uValue;

        pDiagInfo = (PDSLH_TRACEROUTE_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Traceroute);

        if ( pDiagInfo && pDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Inprogress )
        {
            pDiagTracerouteInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_Canceled;
        }
        else
        {
            pDiagTracerouteInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
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
        TraceRoute_SetParamStringValue
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
TraceRoute_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TRACEROUTE_INFO           pDiagTracerouteInfo = pMyObject->hDiagTracerouteInfo;
    PDSLH_TRACEROUTE_INFO           pDiagInfo           = NULL;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Interface", TRUE))
    {
        AnscCopyString(pDiagTracerouteInfo->Interface, pString);

        pDiagInfo = (PDSLH_TRACEROUTE_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Traceroute);

        if ( pDiagInfo && pDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Inprogress )
        {
            pDiagTracerouteInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_Canceled;
        }
        else
        {
            pDiagTracerouteInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
        }

        return TRUE;
    }

    if( AnscEqualString(ParamName, "Host", TRUE))
    {
        AnscCopyString(pDiagTracerouteInfo->Host, pString);

        pDiagInfo = (PDSLH_TRACEROUTE_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Traceroute);

        if ( pDiagInfo && pDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Inprogress )
        {
            pDiagTracerouteInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_Canceled;
        }
        else
        {
            pDiagTracerouteInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
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
        TraceRoute_Validate
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
TraceRoute_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TRACEROUTE_INFO           pDiagTracerouteInfo = pMyObject->hDiagTracerouteInfo;

    if ( !pDiagTracerouteInfo )
    {
        return ANSC_STATUS_FAILURE;
    }

    if ( pDiagTracerouteInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Requested )
    {
        if ( AnscSizeOfString(pDiagTracerouteInfo->Host) == 0 )
        {
            AnscCopyString(pReturnParamName, "Host");

            return FALSE;
        }
    }

    if ( TRUE ) 
    {
        if ( pDiagTracerouteInfo->NumberOfTries < DSLH_TRACEROUTE_MIN_NumberOfTries )
        {
            AnscCopyString(pReturnParamName, "NumberOfTries");

            return FALSE;
        }
    }

    if ( TRUE ) 
    {
        if ( pDiagTracerouteInfo->Timeout < DSLH_TRACEROUTE_MIN_Timeout )
        {
            AnscCopyString(pReturnParamName, "Timeout");

            return FALSE;
        }
    }

    if ( TRUE ) 
    {
        if ( pDiagTracerouteInfo->DataBlockSize < DSLH_TRACEROUTE_MIN_DataBlockSize || pDiagTracerouteInfo->DataBlockSize > DSLH_TRACEROUTE_MAX_DataBlockSize )
        {
            AnscCopyString(pReturnParamName, "DataBlockSize");

            return FALSE;
        }
    }

    if ( TRUE ) 
    {
        if ( pDiagTracerouteInfo->MaxHopCount < DSLH_TRACEROUTE_MIN_MaxHopCount || pDiagTracerouteInfo->MaxHopCount > DSLH_TRACEROUTE_MAX_HOPS_COUNT )
        {
            AnscCopyString(pReturnParamName, "MaxHopCount");

            return FALSE;
        }
    }

    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        TraceRoute_Commit
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
TraceRoute_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TRACEROUTE_INFO           pDiagTracerouteInfo = pMyObject->hDiagTracerouteInfo;
    PDSLH_TRACEROUTE_INFO           pDiagInfo           = NULL;

    if ( !pDiagTracerouteInfo )
    {
        return ANSC_STATUS_FAILURE;
    }

    pDiagInfo = (PDSLH_TRACEROUTE_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Traceroute);

    if ( pDiagTracerouteInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Requested )
    {
        if ( pDiagInfo && pDiagInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Inprogress )
        {
            pDiagTracerouteInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_Canceled;
            CosaDmlDiagCancelDiagnostic(DSLH_DIAGNOSTIC_TYPE_Traceroute);
            pDiagTracerouteInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_Requested;
            AnscSleep(1000);
        }

        CosaDmlDiagScheduleDiagnostic(DSLH_DIAGNOSTIC_TYPE_Traceroute, (ANSC_HANDLE)pDiagTracerouteInfo);

        pDiagTracerouteInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
    }

    if ( pDiagTracerouteInfo->DiagnosticState == DSLH_DIAG_STATE_TYPE_Canceled )
    {
        CosaDmlDiagCancelDiagnostic(DSLH_DIAGNOSTIC_TYPE_Traceroute);

        pDiagTracerouteInfo->DiagnosticState = DSLH_DIAG_STATE_TYPE_None;
    }

    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        TraceRoute_Rollback
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
TraceRoute_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TRACEROUTE_INFO           pDiagTracerouteInfo = pMyObject->hDiagTracerouteInfo;
    PDSLH_TRACEROUTE_INFO           pDiagInfo           = NULL;

    if ( !pDiagTracerouteInfo )
    {
        return ANSC_STATUS_FAILURE;
    }

    pDiagInfo = (PDSLH_TRACEROUTE_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Traceroute);

    if ( pDiagInfo )
    {
        DslhInitTracerouteInfo(pDiagTracerouteInfo);
        pDiagTracerouteInfo->StructSize                = sizeof(DSLH_TRACEROUTE_INFO);
        AnscCopyString(pDiagTracerouteInfo->Host     , pDiagInfo->Host     );
        AnscCopyString(pDiagTracerouteInfo->Interface, pDiagInfo->Interface);
        pDiagTracerouteInfo->Timeout                   = pDiagInfo->Timeout;
        pDiagTracerouteInfo->MaxHopCount               = pDiagInfo->MaxHopCount;
        pDiagTracerouteInfo->NumberOfTries             = pDiagInfo->NumberOfTries;
        pDiagTracerouteInfo->DataBlockSize             = pDiagInfo->DataBlockSize;
        pDiagTracerouteInfo->UpdatedAt                 = pDiagInfo->UpdatedAt;
    }
    else
    {
        DslhInitTracerouteInfo(pDiagTracerouteInfo);
    }

    return 0;
}

/***********************************************************************

 APIs for Object:

    IP.Diagnostics.TraceRoute.RouteHops.{i}.

    *  RouteHops_GetEntryCount
    *  RouteHops_GetEntry
    *  RouteHops_IsUpdated
    *  RouteHops_Synchronize
    *  RouteHops_GetParamBoolValue
    *  RouteHops_GetParamIntValue
    *  RouteHops_GetParamUlongValue
    *  RouteHops_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        RouteHops_GetEntryCount
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
RouteHops_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PDSLH_TRACEROUTE_INFO           pDiagTracerouteInfo = NULL;

    pDiagTracerouteInfo = (PDSLH_TRACEROUTE_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Traceroute);

    if ( pDiagTracerouteInfo && pDiagTracerouteInfo->DiagnosticState != DSLH_DIAG_STATE_TYPE_None 
         && pDiagTracerouteInfo->DiagnosticState != DSLH_DIAG_STATE_TYPE_Requested )
    {
        return pDiagTracerouteInfo->RouteHopsNumberOfEntries;
    }

    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        RouteHops_GetEntry
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
RouteHops_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PDSLH_TRACEROUTE_INFO           pDiagTracerouteInfo= NULL;
    PDSLH_ROUTEHOPS_INFO            pRouteHop          = (PDSLH_ROUTEHOPS_INFO       )NULL;

    pDiagTracerouteInfo = (PDSLH_TRACEROUTE_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Traceroute);

    if ( pDiagTracerouteInfo )
    {
        pRouteHop = (PDSLH_ROUTEHOPS_INFO)&pDiagTracerouteInfo->hDiagRouteHops[nIndex];
    }

    if ( !pRouteHop )
    {
        return  (ANSC_HANDLE)NULL;
    }

    *pInsNumber  = nIndex + 1; 
    return pRouteHop;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        RouteHops_IsUpdated
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
RouteHops_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TRACEROUTE_INFO           pDiagTracerouteInfo = pMyObject->hDiagTracerouteInfo;
    PDSLH_TRACEROUTE_INFO           pDiagInfo           = NULL;

    if ( !pDiagTracerouteInfo )
    {
        return FALSE;
    }

    pDiagInfo = (PDSLH_TRACEROUTE_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Traceroute);

    if ( pDiagInfo && pDiagTracerouteInfo->UpdatedAt != pDiagInfo->UpdatedAt )
    {
        return  TRUE;
    }

    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        RouteHops_Synchronize
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
RouteHops_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject           = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TRACEROUTE_INFO           pDiagTracerouteInfo = pMyObject->hDiagTracerouteInfo;
    PDSLH_TRACEROUTE_INFO           pDiagInfo           = NULL;

    if ( !pDiagTracerouteInfo )
    {
        return ANSC_STATUS_FAILURE;
    }

    pDiagInfo = (PDSLH_TRACEROUTE_INFO)CosaDmlDiagGetResults(DSLH_DIAGNOSTIC_TYPE_Traceroute);

    if ( !pDiagInfo )
    {
        CcspTraceWarning(("Failed to get Traceroute backend information!\n"));

        return  ANSC_STATUS_FAILURE;
    }

    pDiagTracerouteInfo->UpdatedAt = pDiagInfo->UpdatedAt;

    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        RouteHops_GetParamBoolValue
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
RouteHops_GetParamBoolValue
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
        RouteHops_GetParamIntValue
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
RouteHops_GetParamIntValue
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
        RouteHops_GetParamUlongValue
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
RouteHops_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PDSLH_ROUTEHOPS_INFO            pRouteHop          = (PDSLH_ROUTEHOPS_INFO       )hInsContext;

    if ( !pRouteHop )
    {
        CcspTraceWarning(("Failed to get route hops parameters hInsContext!\n"));

        return FALSE;
    }

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "ErrorCode", TRUE))
    {
        *puLong = pRouteHop->HopErrorCode;

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        RouteHops_GetParamStringValue
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
RouteHops_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PDSLH_ROUTEHOPS_INFO            pRouteHop          = (PDSLH_ROUTEHOPS_INFO       )hInsContext;

    if ( !pRouteHop )
    {
        CcspTraceWarning(("Failed to get route hops parameters hInsContext!\n"));

        return FALSE;
    }

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Host", TRUE))
    {
        if ( AnscSizeOfString(pRouteHop->HopHost) < *pUlSize )
        {
            AnscCopyString(pValue, pRouteHop->HopHost);

            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pRouteHop->HopHost) + 1;

            return -1;
        }
    }

    if( AnscEqualString(ParamName, "HostAddress", TRUE))
    {
        if ( AnscSizeOfString(pRouteHop->HopHostAddress) < *pUlSize )
        {
            AnscCopyString(pValue, pRouteHop->HopHostAddress);

            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pRouteHop->HopHostAddress) + 1;

            return -1;
        }
    }

    if( AnscEqualString(ParamName, "RTTimes", TRUE))
    {
        if ( AnscSizeOfString(pRouteHop->HopRTTimes) < *pUlSize )
        {
            AnscCopyString(pValue, pRouteHop->HopRTTimes);

            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pRouteHop->HopRTTimes) + 1;

            return -1;
        }
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}
/***********************************************************************
   

 APIs for Object:

    IP.Diagnostics.DownloadDiagnostics.

    *  DownloadDiagnostics_GetParamBoolValue
    *  DownloadDiagnostics_GetParamIntValue
    *  DownloadDiagnostics_GetParamUlongValue
    *  DownloadDiagnostics_GetParamStringValue
    *  DownloadDiagnostics_SetParamBoolValue
    *  DownloadDiagnostics_SetParamIntValue
    *  DownloadDiagnostics_SetParamUlongValue
    *  DownloadDiagnostics_SetParamStringValue
    *  DownloadDiagnostics_Validate
    *  DownloadDiagnostics_Commit
    *  DownloadDiagnostics_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DownloadDiagnostics_GetParamBoolValue
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
DownloadDiagnostics_GetParamBoolValue
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
        DownloadDiagnostics_GetParamIntValue
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
DownloadDiagnostics_GetParamIntValue
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
        DownloadDiagnostics_GetParamUlongValue
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
DownloadDiagnostics_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject          = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_DOWNLOAD_DIAG_INFO  pDownloadInfo      = pMyObject->hDiagDownloadInfo;
    PDSLH_TR143_DOWNLOAD_DIAG_STATS pDownloadDiagStats = NULL;


    /* check the parameter name and return the corresponding value */
    if ( AnscEqualString(ParamName, "DiagnosticsState", TRUE))
    {
        pDownloadDiagStats = (PDSLH_TR143_DOWNLOAD_DIAG_STATS)CosaDmlDiagGetResults
                                (
                                    DSLH_DIAGNOSTIC_TYPE_Download
                                );

        if ( pDownloadDiagStats )
        {
            *puLong = pDownloadDiagStats->DiagStates + 1;
        }
        else
        {
            CcspTraceWarning(("Download Diagnostics---Failed to get DiagnosticsState\n!"));

            *puLong = 0;
            return FALSE;
        }
        
        return TRUE;
    }

    if ( AnscEqualString(ParamName, "DSCP", TRUE))
    {
        if ( pDownloadInfo )
        {
            *puLong = pDownloadInfo->DSCP;
        }
        else
        {
            CcspTraceWarning(("Download Diagnostics---Failed to get DSCP \n!"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if ( AnscEqualString(ParamName, "EthernetPriority", TRUE))
    {
        if ( pDownloadInfo )
        {
            *puLong = pDownloadInfo->EthernetPriority;
        }
        else
        {
            CcspTraceWarning(("Download Diagnostics---Failed to get EthernetPriority \n!"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if ( AnscEqualString(ParamName, "TestBytesReceived", TRUE))
    {
        pDownloadDiagStats = (PDSLH_TR143_DOWNLOAD_DIAG_STATS)CosaDmlDiagGetResults
                                (
                                    DSLH_DIAGNOSTIC_TYPE_Download
                                );        

        if ( pDownloadDiagStats )
        {
            *puLong = pDownloadDiagStats->TestBytesReceived;
        }
        else
        {
            CcspTraceWarning(("Download Diagnostics---Failed to get TestBytesReceived\n!"));

            *puLong = 0;
            return FALSE;
        }
        
        return TRUE;
    }

    if ( AnscEqualString(ParamName, "TotalBytesReceived", TRUE))
    {
        pDownloadDiagStats = (PDSLH_TR143_DOWNLOAD_DIAG_STATS)CosaDmlDiagGetResults
                                (
                                    DSLH_DIAGNOSTIC_TYPE_Download
                                );


        if ( pDownloadDiagStats )
        {
            *puLong = pDownloadDiagStats->TotalBytesReceived;
        }
        else
        {
            CcspTraceWarning(("Download Diagnostics---Failed to get TotalBytesReceived\n!"));

            *puLong = 0;
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

        ULONG
        DownloadDiagnostics_GetParamStringValue
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
DownloadDiagnostics_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject          = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_DOWNLOAD_DIAG_INFO  pDownloadInfo      = pMyObject->hDiagDownloadInfo;
    PDSLH_TR143_DOWNLOAD_DIAG_STATS pDownloadDiagStats = NULL;
    PANSC_UNIVERSAL_TIME            pTime              = NULL;
    char                            pBuf[128]          = { 0 };


    /* check the parameter name and return the corresponding value */
    if ( AnscEqualString(ParamName, "Interface", TRUE))
    {
        if ( pDownloadInfo )
        {
            AnscCopyString(pValue, pDownloadInfo->Interface);
        }
        else
        {
            CcspTraceWarning(("Download Diagnostics---Failed to get Interface\n!"));
            return -1;
        }
        
        return 0;
    }
    
    if ( AnscEqualString(ParamName, "DownloadURL", TRUE))
    {
        if ( pDownloadInfo )
        {
            AnscCopyString(pValue, pDownloadInfo->DownloadURL);
        }
        else
        {
            CcspTraceWarning(("Download Diagnostics---Failed to get DownloadURL \n!"));
            return -1;
        }
        
        return 0;
    }

    if( AnscEqualString(ParamName, "ROMTime", TRUE))
    {
        pDownloadDiagStats = (PDSLH_TR143_DOWNLOAD_DIAG_STATS)CosaDmlDiagGetResults
                                (
                                    DSLH_DIAGNOSTIC_TYPE_Download
                                );
        
        if ( pDownloadDiagStats )
        {
            pTime = (PANSC_UNIVERSAL_TIME)&pDownloadDiagStats->ROMTime;

            _ansc_sprintf
            (
                pBuf,
                "%.4d-%.2d-%.2dT%.2d:%.2d:%.2d.%.3d000",
                pTime->Year,
                pTime->Month,
                pTime->DayOfMonth,
                pTime->Hour,
                pTime->Minute,
                pTime->Second,
                pTime->MilliSecond
            );
            
            AnscCopyString(pValue, pBuf);
        }
        else
        {
            CcspTraceWarning(("Download Diagnostics---Failed to get ROMTime\n!"));

            return -1;
        }
        
        return 0;
    }

    if ( AnscEqualString(ParamName, "BOMTime", TRUE))
    {
        pDownloadDiagStats = (PDSLH_TR143_DOWNLOAD_DIAG_STATS)CosaDmlDiagGetResults
                                (
                                    DSLH_DIAGNOSTIC_TYPE_Download
                                );
        
        if ( pDownloadDiagStats )
        {
            pTime = (PANSC_UNIVERSAL_TIME)&pDownloadDiagStats->BOMTime;

            _ansc_sprintf
            (
                pBuf,
                "%.4d-%.2d-%.2dT%.2d:%.2d:%.2d.%.3d000",
                pTime->Year,
                pTime->Month,
                pTime->DayOfMonth,
                pTime->Hour,
                pTime->Minute,
                pTime->Second,
                pTime->MilliSecond
            );
            
            AnscCopyString(pValue, pBuf);
        }
        else
        {
            CcspTraceWarning(("Download Diagnostics---Failed to get BOMTime\n!"));

            return -1;
        }
        
        return 0;
    }

    if ( AnscEqualString(ParamName, "EOMTime", TRUE))
    {
        pDownloadDiagStats = (PDSLH_TR143_DOWNLOAD_DIAG_STATS)CosaDmlDiagGetResults
                                (
                                    DSLH_DIAGNOSTIC_TYPE_Download
                                );
                                
        if ( pDownloadDiagStats )
        {
            pTime = (PANSC_UNIVERSAL_TIME)&pDownloadDiagStats->EOMTime;

            _ansc_sprintf
            (
                pBuf,
                "%.4d-%.2d-%.2dT%.2d:%.2d:%.2d.%.3d000",
                pTime->Year,
                pTime->Month,
                pTime->DayOfMonth,
                pTime->Hour,
                pTime->Minute,
                pTime->Second,
                pTime->MilliSecond
            );

            AnscCopyString(pValue, pBuf);
        }
        else
        {
            CcspTraceWarning(("Download Diagnostics---Failed to get EOMTime\n!"));

            return -1;
        }
        
        return 0;
    }

    if ( AnscEqualString(ParamName, "TCPOpenRequestTime", TRUE))
    {
        pDownloadDiagStats = (PDSLH_TR143_DOWNLOAD_DIAG_STATS)CosaDmlDiagGetResults
                                (
                                    DSLH_DIAGNOSTIC_TYPE_Download
                                );
                                        
        if ( pDownloadDiagStats )
        {
            pTime = (PANSC_UNIVERSAL_TIME)&pDownloadDiagStats->TCPOpenRequestTime;

            _ansc_sprintf
            (
                pBuf,
                "%.4d-%.2d-%.2dT%.2d:%.2d:%.2d.%.3d000",
                pTime->Year,
                pTime->Month,
                pTime->DayOfMonth,
                pTime->Hour,
                pTime->Minute,
                pTime->Second,
                pTime->MilliSecond
            );

            AnscCopyString(pValue, pBuf);
        }
        else
        {
            CcspTraceWarning(("Download Diagnostics---Failed to get TCPOpenRequestTime\n!"));

            return -1;
        }
        
        return 0;
    }

    if ( AnscEqualString(ParamName, "TCPOpenResponseTime", TRUE))
    {
        pDownloadDiagStats = (PDSLH_TR143_DOWNLOAD_DIAG_STATS)CosaDmlDiagGetResults
                                (
                                    DSLH_DIAGNOSTIC_TYPE_Download
                                );
        if ( pDownloadDiagStats )
        {
            pTime = (PANSC_UNIVERSAL_TIME)&pDownloadDiagStats->TCPOpenResponseTime;

            _ansc_sprintf
            (
                pBuf,
                "%.4d-%.2d-%.2dT%.2d:%.2d:%.2d.%.3d000",
                pTime->Year,
                pTime->Month,
                pTime->DayOfMonth,
                pTime->Hour,
                pTime->Minute,
                pTime->Second,
                pTime->MilliSecond
            );

            AnscCopyString(pValue, pBuf);
        }
        else
        {
            CcspTraceWarning(("Download Diagnostics---Failed to get TCPOpenResponseTime\n!"));

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
        DownloadDiagnostics_SetParamBoolValue
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
DownloadDiagnostics_SetParamBoolValue
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
        DownloadDiagnostics_SetParamIntValue
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
DownloadDiagnostics_SetParamIntValue
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
        DownloadDiagnostics_SetParamUlongValue
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
DownloadDiagnostics_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject          = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_DOWNLOAD_DIAG_INFO  pDownloadInfo      = pMyObject->hDiagDownloadInfo;
    PDSLH_TR143_DOWNLOAD_DIAG_STATS pDownloadDiagStats = NULL;

    /* check the parameter name and set the corresponding value */
    if ( AnscEqualString(ParamName, "DiagnosticsState", TRUE))
    {
        uValue--;
        if ( uValue != (ULONG)DSLH_TR143_DIAGNOSTIC_Requested )
        {
            return FALSE;
        }
        
        pDownloadInfo->DiagnosticsState = DSLH_TR143_DIAGNOSTIC_Requested;
        return TRUE;        
    }

    if ( AnscEqualString(ParamName, "DSCP", TRUE))
    {
        pDownloadInfo->DSCP= uValue;
        return TRUE;
    }

    if ( AnscEqualString(ParamName, "EthernetPriority", TRUE))
    {
        pDownloadInfo->EthernetPriority = uValue;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DownloadDiagnostics_SetParamStringValue
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
DownloadDiagnostics_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject          = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_DOWNLOAD_DIAG_INFO  pDownloadInfo      = pMyObject->hDiagDownloadInfo;


    /* check the parameter name and set the corresponding value */
    if ( AnscEqualString(ParamName, "Interface", TRUE))
    {
     /* Because not work across library, so disable this for later solution.*/ 
#if 0
       if ( !g_ValidateInterface(
            pString, 
            DSLH_WAN_LAYER3_CONNECTION_INTERFACE | DSLH_NULL_STRING_INTERFACE
            ))
        {
            return FALSE;
        }
#endif

        AnscCopyString(pDownloadInfo->Interface, pString);
        return TRUE;
    }

    if ( AnscEqualString(ParamName, "DownloadURL", TRUE))
    {
        if ( !pString || !(*pString) )
        {
            return FALSE;   
        }
 
        AnscCopyString(pDownloadInfo->DownloadURL, pString);
        return TRUE;
    }

    /*CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DownloadDiagnostics_Validate
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
DownloadDiagnostics_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject          = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_DOWNLOAD_DIAG_INFO  pDownloadInfo      = pMyObject->hDiagDownloadInfo;

    if ( pDownloadInfo->DiagnosticsState == DSLH_TR143_DIAGNOSTIC_Requested 
      && !AnscSizeOfString(pDownloadInfo->DownloadURL) )
    {
        AnscCopyString(pReturnParamName, "DownloadURL");
        return FALSE;
    }

    return  TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        DownloadDiagnostics_Commit
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
DownloadDiagnostics_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_FAILURE;
    PCOSA_DATAMODEL_DIAG            pMyObject          = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_DOWNLOAD_DIAG_INFO  pDownloadInfo      = pMyObject->hDiagDownloadInfo;


    if ( pDownloadInfo->DiagnosticsState != DSLH_TR143_DIAGNOSTIC_Requested )
    {
        pDownloadInfo->DiagnosticsState = DSLH_TR143_DIAGNOSTIC_None;
    }

    returnStatus = CosaDmlDiagScheduleDiagnostic
                    (
                        DSLH_DIAGNOSTIC_TYPE_Download, 
                        (ANSC_HANDLE)pDownloadInfo
                    );
 
    return 0;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        DownloadDiagnostics_Rollback
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
DownloadDiagnostics_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject          = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_DOWNLOAD_DIAG_INFO  pDownloadInfo      = pMyObject->hDiagDownloadInfo;
    PDSLH_TR143_DOWNLOAD_DIAG_INFO  pDownloadPreInfo   = NULL; 

    if ( !pDownloadInfo )
    {
        return ANSC_STATUS_FAILURE;
    }
    
    DslhInitDownloadDiagInfo(pDownloadInfo);

    pDownloadPreInfo = (PDSLH_TR143_DOWNLOAD_DIAG_INFO)CosaDmlDiagGetConfigs
                        (
                            DSLH_DIAGNOSTIC_TYPE_Download
                        );

    if ( pDownloadPreInfo )
    {
        AnscCopyString(pDownloadInfo->Interface, pDownloadPreInfo->Interface);
        AnscCopyString(pDownloadInfo->DownloadURL, pDownloadPreInfo->DownloadURL);
        pDownloadInfo->DSCP             = pDownloadPreInfo->DSCP;
        pDownloadInfo->EthernetPriority = pDownloadPreInfo->EthernetPriority;
        pDownloadInfo->DiagnosticsState = pDownloadPreInfo->DiagnosticsState;
    }
    else
    {
       CcspTraceWarning(("Download Diagnostics---Failed to get previous configuration!\n")); 
    }

    return 0;
}


/***********************************************************************

 APIs for Object:

    IP.Diagnostics.UploadDiagnostics.

    *  UploadDiagnostics_GetParamBoolValue
    *  UploadDiagnostics_GetParamIntValue
    *  UploadDiagnostics_GetParamUlongValue
    *  UploadDiagnostics_GetParamStringValue
    *  UploadDiagnostics_SetParamBoolValue
    *  UploadDiagnostics_SetParamIntValue
    *  UploadDiagnostics_SetParamUlongValue
    *  UploadDiagnostics_SetParamStringValue
    *  UploadDiagnostics_Validate
    *  UploadDiagnostics_Commit
    *  UploadDiagnostics_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        UploadDiagnostics_GetParamBoolValue
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
UploadDiagnostics_GetParamBoolValue
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
        UploadDiagnostics_GetParamIntValue
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
UploadDiagnostics_GetParamIntValue
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
        UploadDiagnostics_GetParamUlongValue
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
UploadDiagnostics_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject        = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_UPLOAD_DIAG_INFO    pUploadInfo      = pMyObject->hDiagUploadInfo;
    PDSLH_TR143_UPLOAD_DIAG_STATS   pUploadDiagStats = NULL;

  
    /* check the parameter name and return the corresponding value */
    if ( AnscEqualString(ParamName, "DiagnosticsState", TRUE))
    {
        pUploadDiagStats = (PDSLH_TR143_UPLOAD_DIAG_STATS)CosaDmlDiagGetResults
                            (
                                DSLH_DIAGNOSTIC_TYPE_Upload
                            );

        if ( pUploadDiagStats )
        {
            *puLong = pUploadDiagStats->DiagStates + 1;
        }
        else
        {
            CcspTraceWarning(("Upload Diagnostics---Failed to get DiagnosticsState\n!"));

            *puLong = 0;
            return FALSE;
        }
        
        return TRUE;
    }

    if( AnscEqualString(ParamName, "DSCP", TRUE))
    {
        if ( pUploadInfo )
        {
            *puLong = pUploadInfo->DSCP;
        }
        else
        {
            CcspTraceWarning(("Upload Diagnostics---Failed to get DSCP \n!"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if( AnscEqualString(ParamName, "EthernetPriority", TRUE))
    {
        if ( pUploadInfo )
        {
            *puLong = pUploadInfo->EthernetPriority;
        }
        else
        {
            CcspTraceWarning(("Upload Diagnostics---Failed to get EthernetPriority \n!"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if( AnscEqualString(ParamName, "TestFileLength", TRUE))
    {
        if ( pUploadInfo )
        {
            *puLong = pUploadInfo->TestFileLength;
        }
        else
        {
            CcspTraceWarning(("Upload Diagnostics---Failed to get TestFileLength \n!"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if( AnscEqualString(ParamName, "TotalBytesSent", TRUE))
    {
        pUploadDiagStats = (PDSLH_TR143_UPLOAD_DIAG_STATS)CosaDmlDiagGetResults
                            (
                                DSLH_DIAGNOSTIC_TYPE_Upload
                            );

        if ( pUploadDiagStats )
        {
            *puLong = pUploadDiagStats->TotalBytesSent;
        }
        else
        {
            CcspTraceWarning(("Upload Diagnostics---Failed to get TotalBytesSent\n!"));

            *puLong = 0;
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

        ULONG
        UploadDiagnostics_GetParamStringValue
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
UploadDiagnostics_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject        = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_UPLOAD_DIAG_INFO    pUploadInfo      = pMyObject->hDiagUploadInfo;
    PDSLH_TR143_UPLOAD_DIAG_STATS   pUploadDiagStats = NULL;
    PANSC_UNIVERSAL_TIME            pTime            = NULL;
    char                            pBuf[128]        = { 0 };


    /* check the parameter name and return the corresponding value */
    if ( AnscEqualString(ParamName, "Interface", TRUE))
    {
        if ( pUploadInfo )
        {
            AnscCopyString(pValue, pUploadInfo->Interface);
        }
        else
        {
            CcspTraceWarning(("Upload Diagnostics---Failed to get Interface \n!"));
            return -1;
        }
        
        return 0;
    }

    if ( AnscEqualString(ParamName, "UploadURL", TRUE))
    {
        if ( pUploadInfo )
        {
            AnscCopyString(pValue, pUploadInfo->UploadURL);
        }
        else
        {
            CcspTraceWarning(("Upload Diagnostics---Failed to get UploadURL \n!"));
            return -1;
        }
        
        return 0;
    }

    if ( AnscEqualString(ParamName, "ROMTime", TRUE))
    {
        pUploadDiagStats = (PDSLH_TR143_UPLOAD_DIAG_STATS)CosaDmlDiagGetResults
                            (
                                DSLH_DIAGNOSTIC_TYPE_Upload
                            );
        
        if ( pUploadDiagStats )
        {
            pTime = (PANSC_UNIVERSAL_TIME)&pUploadDiagStats->ROMTime;

            _ansc_sprintf
            (
                pBuf,
                "%.4d-%.2d-%.2dT%.2d:%.2d:%.2d.%.3d000",
                pTime->Year,
                pTime->Month,
                pTime->DayOfMonth,
                pTime->Hour,
                pTime->Minute,
                pTime->Second,
                pTime->MilliSecond
            );
           
           AnscCopyString(pValue, pBuf);
        }
       else
       {
            CcspTraceWarning(("Upload Diagnostics---Failed to get ROMTime\n!"));

            return -1;
       }
       
       return 0;
    }
    
    if ( AnscEqualString(ParamName, "BOMTime", TRUE))
    {
        pUploadDiagStats = (PDSLH_TR143_UPLOAD_DIAG_STATS)CosaDmlDiagGetResults
                            (
                                DSLH_DIAGNOSTIC_TYPE_Upload
                            );

        if ( pUploadDiagStats )
        {
            pTime = (PANSC_UNIVERSAL_TIME)&pUploadDiagStats->BOMTime;

            _ansc_sprintf
            (
                pBuf,
                "%.4d-%.2d-%.2dT%.2d:%.2d:%.2d.%.3d000",
                pTime->Year,
                pTime->Month,
                pTime->DayOfMonth,
                pTime->Hour,
                pTime->Minute,
                pTime->Second,
                pTime->MilliSecond
            );

            AnscCopyString(pValue, pBuf);
        }
        else
        {
            CcspTraceWarning(("Upload Diagnostics---Failed to get BOMTime\n!"));

            return -1;
        }

        return 0;
    }
    
    if ( AnscEqualString(ParamName, "EOMTime", TRUE))
    {
        pUploadDiagStats = (PDSLH_TR143_UPLOAD_DIAG_STATS)CosaDmlDiagGetResults
                            (
                                DSLH_DIAGNOSTIC_TYPE_Upload
                            );
                            
        if ( pUploadDiagStats )
        {
            pTime = (PANSC_UNIVERSAL_TIME)&pUploadDiagStats->EOMTime;

            _ansc_sprintf
            (
                pBuf,
                "%.4d-%.2d-%.2dT%.2d:%.2d:%.2d.%.3d000",
                pTime->Year,
                pTime->Month,
                pTime->DayOfMonth,
                pTime->Hour,
                pTime->Minute,
                pTime->Second,
                pTime->MilliSecond
            );

            AnscCopyString(pValue, pBuf);
        }
        else
        {
            CcspTraceWarning(("Upload Diagnostics---Failed to get EOMTime\n!"));

            return -1;
        }

        return 0;
    }
    
    if ( AnscEqualString(ParamName, "TCPOpenRequestTime", TRUE))
    {
        pUploadDiagStats = (PDSLH_TR143_UPLOAD_DIAG_STATS)CosaDmlDiagGetResults
                            (
                                DSLH_DIAGNOSTIC_TYPE_Upload
                            );
                            
        if ( pUploadDiagStats )
        {
            pTime = (PANSC_UNIVERSAL_TIME)&pUploadDiagStats->TCPOpenRequestTime;

            _ansc_sprintf
            (
                pBuf,
                "%.4d-%.2d-%.2dT%.2d:%.2d:%.2d.%.3d000",
                pTime->Year,
                pTime->Month,
                pTime->DayOfMonth,
                pTime->Hour,
                pTime->Minute,
                pTime->Second,
                pTime->MilliSecond
            );

            AnscCopyString(pValue, pBuf);
        }
        else
        {
            CcspTraceWarning(("Upload Diagnostics---Failed to get TCPOpenRequestTime\n!"));

            return -1;
        }

        return 0;
    }
    
    if ( AnscEqualString(ParamName, "TCPOpenResponseTime", TRUE))
    {
        pUploadDiagStats = (PDSLH_TR143_UPLOAD_DIAG_STATS)CosaDmlDiagGetResults
                            (
                                DSLH_DIAGNOSTIC_TYPE_Upload
                            );
        
        if ( pUploadDiagStats )
        {
            pTime = (PANSC_UNIVERSAL_TIME)&pUploadDiagStats->TCPOpenResponseTime;

            _ansc_sprintf
            (
                pBuf,
                "%.4d-%.2d-%.2dT%.2d:%.2d:%.2d.%.3d000",
                pTime->Year,
                pTime->Month,
                pTime->DayOfMonth,
                pTime->Hour,
                pTime->Minute,
                pTime->Second,
                pTime->MilliSecond
            );

            AnscCopyString(pValue, pBuf);
        }
        else
        {
            CcspTraceWarning(("Upload Diagnostics---Failed to get TCPOpenResponseTime\n!"));

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
        UploadDiagnostics_SetParamBoolValue
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
UploadDiagnostics_SetParamBoolValue
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
        UploadDiagnostics_SetParamIntValue
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
UploadDiagnostics_SetParamIntValue
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
        UploadDiagnostics_SetParamUlongValue
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
UploadDiagnostics_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject        = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_UPLOAD_DIAG_INFO    pUploadInfo      = pMyObject->hDiagUploadInfo;
    PDSLH_TR143_UPLOAD_DIAG_STATS   pUploadDiagStats = NULL;

    /* check the parameter name and set the corresponding value */
    if ( AnscEqualString(ParamName, "DiagnosticsState", TRUE))
    {
        uValue--;
        if ( uValue != (ULONG)DSLH_TR143_DIAGNOSTIC_Requested )
        {
            return FALSE;
        }
        
        pUploadInfo->DiagnosticsState = DSLH_TR143_DIAGNOSTIC_Requested;
        return TRUE;        
    }

    if ( AnscEqualString(ParamName, "DSCP", TRUE))
    {
        pUploadInfo->DSCP = uValue;
        return TRUE;
    }

    if ( AnscEqualString(ParamName, "EthernetPriority", TRUE))
    {
        pUploadInfo->EthernetPriority = uValue;
        return TRUE;
    }

    if ( AnscEqualString(ParamName, "TestFileLength", TRUE))
    {
        if ( uValue == 0 )
        {
            return FALSE;
        }
        
        pUploadInfo->TestFileLength = uValue;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        UploadDiagnostics_SetParamStringValue
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
UploadDiagnostics_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject        = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_UPLOAD_DIAG_INFO    pUploadInfo      = pMyObject->hDiagUploadInfo;
    PDSLH_TR143_UPLOAD_DIAG_STATS   pUploadDiagStats = NULL;

    /* check the parameter name and set the corresponding value */
    if ( AnscEqualString(ParamName, "Interface", TRUE))
    {
     /* Because not work across library, so disable this for later solution.*/ 
#if 0
        if ( !g_ValidateInterface(
            pString, 
            DSLH_WAN_LAYER3_CONNECTION_INTERFACE | DSLH_NULL_STRING_INTERFACE
            ))
        {
            return FALSE;
        }
#endif        

        AnscCopyString(pUploadInfo->Interface, pString);
        return TRUE;
    }

    if ( AnscEqualString(ParamName, "UploadURL", TRUE))
    {
        if ( !pString || !(*pString) )
        {
            return FALSE;   
        }
        
        AnscCopyString(pUploadInfo->UploadURL, pString);
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        UploadDiagnostics_Validate
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
UploadDiagnostics_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject    = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_UPLOAD_DIAG_INFO    pUploadInfo  = pMyObject->hDiagUploadInfo;

    if ( pUploadInfo->DiagnosticsState == DSLH_TR143_DIAGNOSTIC_Requested 
       && !AnscSizeOfString(pUploadInfo->UploadURL) )
    {
        AnscCopyString(pReturnParamName, "UploadURL");
        return FALSE;
    }
    return  TRUE;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        UploadDiagnostics_Commit
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
UploadDiagnostics_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_FAILURE;
    PCOSA_DATAMODEL_DIAG            pMyObject    = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_UPLOAD_DIAG_INFO    pUploadInfo  = pMyObject->hDiagUploadInfo;


    if ( pUploadInfo->DiagnosticsState != DSLH_TR143_DIAGNOSTIC_Requested )
    {
        pUploadInfo->DiagnosticsState = DSLH_TR143_DIAGNOSTIC_None;
    }

    returnStatus = CosaDmlDiagScheduleDiagnostic
                    (
                        DSLH_DIAGNOSTIC_TYPE_Upload, 
                        (ANSC_HANDLE)pUploadInfo
                    );

    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        UploadDiagnostics_Rollback
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
UploadDiagnostics_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject        = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_UPLOAD_DIAG_INFO    pUploadInfo      = pMyObject->hDiagUploadInfo;
    PDSLH_TR143_UPLOAD_DIAG_INFO    pUploadPreInfo   = NULL; 

    if ( !pUploadInfo )
    {
        return ANSC_STATUS_FAILURE;
    }

    DslhInitUploadDiagInfo(pUploadInfo);

    pUploadPreInfo = (PDSLH_TR143_UPLOAD_DIAG_INFO)CosaDmlDiagGetConfigs
                        (
                            DSLH_DIAGNOSTIC_TYPE_Upload
                        );

    if ( pUploadPreInfo )
    {
        AnscCopyString(pUploadInfo->Interface, pUploadPreInfo->Interface);
        AnscCopyString(pUploadInfo->UploadURL, pUploadPreInfo->UploadURL);
        pUploadInfo->DSCP                 = pUploadPreInfo->DSCP;
        pUploadInfo->EthernetPriority     = pUploadPreInfo->EthernetPriority;
        pUploadInfo->TestFileLength       = pUploadPreInfo->TestFileLength;
        pUploadInfo->DiagnosticsState     = pUploadPreInfo->DiagnosticsState;
    }
    else
    {
        CcspTraceWarning(("Upload Diagnostics---Failed to get previous configuration!\n")); 
    }

    return 0;
}


/***********************************************************************

 APIs for Object:

    IP.Diagnostics.UDPEchoConfig.

    *  UDPEchoConfig_GetParamBoolValue
    *  UDPEchoConfig_GetParamIntValue
    *  UDPEchoConfig_GetParamUlongValue
    *  UDPEchoConfig_GetParamStringValue
    *  UDPEchoConfig_SetParamBoolValue
    *  UDPEchoConfig_SetParamIntValue
    *  UDPEchoConfig_SetParamUlongValue
    *  UDPEchoConfig_SetParamStringValue
    *  UDPEchoConfig_Validate
    *  UDPEchoConfig_Commit
    *  UDPEchoConfig_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        UDPEchoConfig_GetParamBoolValue
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
UDPEchoConfig_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject     = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_UDP_ECHO_CONFIG     pUdpEchoInfo  = pMyObject->hDiagUdpechoSrvInfo;
    PDSLH_UDP_ECHO_SERVER_STATS     pUdpEchoStats = NULL;

    /* check the parameter name and return the corresponding value */
    if ( AnscEqualString(ParamName, "Enable", TRUE))
    {
        if ( pUdpEchoInfo )
        {
            *pBool = pUdpEchoInfo->Enable;
        }
        else
        {
            CcspTraceWarning(("UDP echo Diagnostics---Failed to get Enable \n!"));
            *pBool = FALSE;
            return FALSE;
        }

        return TRUE;
    }
    
    if ( AnscEqualString(ParamName, "EchoPlusEnabled", TRUE))
    {
        if ( pUdpEchoInfo )
        {
            *pBool = pUdpEchoInfo->EchoPlusEnabled;
        }
        else
        {
            CcspTraceWarning(("UDP echo Diagnostics---Failed to get EchoPlusEnabled \n!"));
            *pBool = FALSE;
            return FALSE;
        }

        return TRUE;
    }

    if ( AnscEqualString(ParamName, "EchoPlusSupported", TRUE))
    {
        if ( pUdpEchoInfo )
        {
            *pBool = pUdpEchoInfo->EchoPlusSupported;
        }
        else
        {
            CcspTraceWarning(("UDP echo Diagnostics---Failed to get EchoPlusSupported \n!"));
            *pBool = FALSE;
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
        UDPEchoConfig_GetParamIntValue
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
UDPEchoConfig_GetParamIntValue
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
        UDPEchoConfig_GetParamUlongValue
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
UDPEchoConfig_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject     = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_UDP_ECHO_CONFIG     pUdpEchoInfo  = pMyObject->hDiagUdpechoSrvInfo;
    PDSLH_UDP_ECHO_SERVER_STATS     pUdpEchoStats = NULL;

    /* check the parameter name and return the corresponding value */
    if ( AnscEqualString(ParamName, "SourceIPAddress", TRUE))
    {
        if ( pUdpEchoInfo )
        {
            *puLong = pUdpEchoInfo->SourceIPAddress;
        }
        else
        {
            CcspTraceWarning(("UDP echo Diagnostics---Failed to get SourceIPAddress \n!"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if ( AnscEqualString(ParamName, "UDPPort", TRUE))
    {
        if ( pUdpEchoInfo )
        {
            *puLong = pUdpEchoInfo->UDPPort;
        }
        else
        {
            CcspTraceWarning(("UDP echo Diagnostics---Failed to get UDPPort \n!"));
            *puLong = 0;
            return FALSE;
        }

        return TRUE;
    }

    if ( AnscEqualString(ParamName, "PacketsReceived", TRUE))
    {
        pUdpEchoStats = (PDSLH_UDP_ECHO_SERVER_STATS)CosaDmlDiagGetResults
                        (
                            DSLH_DIAGNOSTIC_TYPE_UdpEcho
                        );

        if ( pUdpEchoStats )
        {
            *puLong = pUdpEchoStats->PacketsReceived;
        }
        else
        {
            CcspTraceWarning(("UDP echo Diagnostics---Failed to get PacketsReceived\n!"));

            *puLong = 0;
            return FALSE;
        }
        
        return TRUE;
    }

    if ( AnscEqualString(ParamName, "PacketsResponded", TRUE))
    {
        pUdpEchoStats = (PDSLH_UDP_ECHO_SERVER_STATS)CosaDmlDiagGetResults
                        (
                            DSLH_DIAGNOSTIC_TYPE_UdpEcho
                        );        

        if ( pUdpEchoStats )
        {
            *puLong = pUdpEchoStats->PacketsResponded;
        }
        else
        {
            CcspTraceWarning(("UDP echo Diagnostics---Failed to get PacketsResponded\n!"));

            *puLong = 0;
            return FALSE;
        }
        
        return TRUE;
    }

    if ( AnscEqualString(ParamName, "BytesReceived", TRUE))
    {
        pUdpEchoStats = (PDSLH_UDP_ECHO_SERVER_STATS)CosaDmlDiagGetResults
                        (
                            DSLH_DIAGNOSTIC_TYPE_UdpEcho
                        );

        if ( pUdpEchoStats )
        {
            *puLong = pUdpEchoStats->BytesReceived;
        }
        else
        {
            CcspTraceWarning(("UDP echo Diagnostics---Failed to get BytesReceived\n!"));

            *puLong = 0;
            return FALSE;
        }
        
        return TRUE;
    }

    if ( AnscEqualString(ParamName, "BytesResponded", TRUE))
    {
        pUdpEchoStats = (PDSLH_UDP_ECHO_SERVER_STATS)CosaDmlDiagGetResults
                        (
                            DSLH_DIAGNOSTIC_TYPE_UdpEcho
                        );

        if ( pUdpEchoStats )
        {
            *puLong = pUdpEchoStats->BytesResponded;
        }
        else
        {
            CcspTraceWarning(("UDP echo Diagnostics---Failed to get BytesResponded\n!"));

            *puLong = 0;
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

        ULONG
        UDPEchoConfig_GetParamStringValue
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
UDPEchoConfig_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject     = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_UDP_ECHO_CONFIG     pUdpEchoInfo  = pMyObject->hDiagUdpechoSrvInfo;
    PDSLH_UDP_ECHO_SERVER_STATS     pUdpEchoStats = NULL;
    PANSC_UNIVERSAL_TIME            pTime         = NULL;
    char                            pBuf[128]     = { 0 };


    /* check the parameter name and return the corresponding value */
    if ( AnscEqualString(ParamName, "Interface", TRUE))
    {
        if ( pUdpEchoInfo )
        {
            AnscCopyString(pValue, pUdpEchoInfo->Interface);
        }
        else
        {
            CcspTraceWarning(("UDP echo Diagnostics---Failed to get Interface\n!"));
            return -1;
        }
        
        return 0;
    }

    if ( AnscEqualString(ParamName, "TimeFirstPacketReceived", TRUE))
    {
        pUdpEchoStats = (PDSLH_UDP_ECHO_SERVER_STATS)CosaDmlDiagGetResults
                            (
                                DSLH_DIAGNOSTIC_TYPE_UdpEcho
                            );
        
        if ( pUdpEchoStats )
        {
            pTime = (PANSC_UNIVERSAL_TIME)&pUdpEchoStats->TimeFirstPacketReceived;

            _ansc_sprintf
            (
                pBuf,
                "%.4d-%.2d-%.2dT%.2d:%.2d:%.2d.%.3d000",
                pTime->Year,
                pTime->Month,
                pTime->DayOfMonth,
                pTime->Hour,
                pTime->Minute,
                pTime->Second,
                pTime->MilliSecond
            );
            
            AnscCopyString(pValue, pBuf);
        }
        else
        {
            CcspTraceWarning(("UDP echo Diagnostics---Failed to get TimeFirstPacketReceived\n!"));

            return -1;
        }
        
        return 0;
    }

    if ( AnscEqualString(ParamName, "TimeLastPacketReceived", TRUE))
    {
        pUdpEchoStats = (PDSLH_UDP_ECHO_SERVER_STATS)CosaDmlDiagGetResults
                            (
                                DSLH_DIAGNOSTIC_TYPE_UdpEcho
                            );
        
        if ( pUdpEchoStats )
        {
            pTime = (PANSC_UNIVERSAL_TIME)&pUdpEchoStats->TimeLastPacketReceived;

            _ansc_sprintf
            (
                pBuf,
                "%.4d-%.2d-%.2dT%.2d:%.2d:%.2d.%.3d000",
                pTime->Year,
                pTime->Month,
                pTime->DayOfMonth,
                pTime->Hour,
                pTime->Minute,
                pTime->Second,
                pTime->MilliSecond
            );
            
            AnscCopyString(pValue, pBuf);
        }
        else
        {
            CcspTraceWarning(("UDP echo Diagnostics---Failed to get TimeLastPacketReceived\n!"));

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
        UDPEchoConfig_SetParamBoolValue
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
UDPEchoConfig_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject     = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_UDP_ECHO_CONFIG     pUdpEchoInfo  = pMyObject->hDiagUdpechoSrvInfo;
    PDSLH_UDP_ECHO_SERVER_STATS     pUdpEchoStats = NULL;

    /* check the parameter name and set the corresponding value */
    if ( AnscEqualString(ParamName, "Enable", TRUE))
    {
        pUdpEchoInfo->Enable = bValue;
        return TRUE;
    }

    if ( AnscEqualString(ParamName, "EchoPlusEnabled", TRUE))
    {
        pUdpEchoInfo->EchoPlusEnabled = bValue;
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        UDPEchoConfig_SetParamIntValue
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
UDPEchoConfig_SetParamIntValue
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
        UDPEchoConfig_SetParamUlongValue
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
UDPEchoConfig_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject     = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_UDP_ECHO_CONFIG     pUdpEchoInfo  = pMyObject->hDiagUdpechoSrvInfo;
    PDSLH_UDP_ECHO_SERVER_STATS     pUdpEchoStats = NULL;

    /* check the parameter name and set the corresponding value */
    if ( AnscEqualString(ParamName, "SourceIPAddress", TRUE))
    {
        if ( uValue == 0 )
        {
            return FALSE;
        }
            
        pUdpEchoInfo->SourceIPAddress = uValue;
        return TRUE;
    }

    if ( AnscEqualString(ParamName, "UDPPort", TRUE))
    {
        if ( uValue == 0 )
        {
            return FALSE;
        } 

        pUdpEchoInfo->UDPPort = uValue;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        UDPEchoConfig_SetParamStringValue
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
UDPEchoConfig_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject     = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_UDP_ECHO_CONFIG     pUdpEchoInfo  = pMyObject->hDiagUdpechoSrvInfo;


    /* check the parameter name and set the corresponding value */
    if ( AnscEqualString(ParamName, "Interface", TRUE))
    {
     /* Because not work across library, so disable this for later solution.*/ 
#if 0
       if ( !g_ValidateInterface(
            pString, 
            DSLH_WAN_LAYER3_CONNECTION_INTERFACE | DSLH_NULL_STRING_INTERFACE
            ))
        {
            return FALSE;
        }
#endif

        AnscCopyString(pUdpEchoInfo->Interface, pString);
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        UDPEchoConfig_Validate
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
UDPEchoConfig_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject     = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_UDP_ECHO_CONFIG     pUdpEchoInfo  = pMyObject->hDiagUdpechoSrvInfo;


    if ( pUdpEchoInfo->EchoPlusEnabled && !pUdpEchoInfo->EchoPlusSupported )
    {
        AnscCopyString(pReturnParamName, "EchoPlusEnabled");
        return FALSE;
    }

    if ( pUdpEchoInfo->Enable && (pUdpEchoInfo->SourceIPAddress == 0) )
    {
        AnscCopyString(pReturnParamName, "SourceIPAddress");
        return FALSE;
    }
        
    if ( pUdpEchoInfo->Enable && (pUdpEchoInfo->UDPPort == 0) )
    {
        AnscCopyString(pReturnParamName, "UDPPort");
        return FALSE;
    }

    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        UDPEchoConfig_Commit
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
UDPEchoConfig_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_FAILURE;
    PCOSA_DATAMODEL_DIAG            pMyObject     = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_UDP_ECHO_CONFIG     pUdpEchoInfo  = pMyObject->hDiagUdpechoSrvInfo;
    PDSLH_UDP_ECHO_SERVER_STATS     pUdpEchoStats = NULL;

   returnStatus = CosaDmlDiagScheduleDiagnostic
                (
                    DSLH_DIAGNOSTIC_TYPE_UdpEcho, 
                    (ANSC_HANDLE)pUdpEchoInfo
                );
    
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        UDPEchoConfig_Rollback
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
UDPEchoConfig_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DIAG            pMyObject       = (PCOSA_DATAMODEL_DIAG)g_pCosaBEManager->hDiag;
    PDSLH_TR143_UDP_ECHO_CONFIG     pUdpEchoInfo    = pMyObject->hDiagUdpechoSrvInfo;
    PDSLH_TR143_UDP_ECHO_CONFIG     pUdpEchoPreInfo = NULL; 

    if ( !pUdpEchoInfo )
    {
        return ANSC_STATUS_FAILURE;
    }

    DslhInitUDPEchoConfig(pUdpEchoInfo);

    pUdpEchoPreInfo = (PDSLH_TR143_UDP_ECHO_CONFIG)CosaDmlDiagGetConfigs
                    (
                        DSLH_DIAGNOSTIC_TYPE_UdpEcho
                    );

    if ( pUdpEchoPreInfo )
    {
        AnscCopyString(pUdpEchoInfo->Interface, pUdpEchoPreInfo->Interface);
        pUdpEchoInfo->Enable               = pUdpEchoPreInfo->Enable;
        pUdpEchoInfo->SourceIPAddress      = pUdpEchoPreInfo->SourceIPAddress;
        pUdpEchoInfo->UDPPort              = pUdpEchoPreInfo->UDPPort;
        pUdpEchoInfo->EchoPlusEnabled      = pUdpEchoPreInfo->EchoPlusEnabled;
        pUdpEchoInfo->EchoPlusSupported    = pUdpEchoPreInfo->EchoPlusSupported;
    }
    else
    {
        CcspTraceWarning(("UDP echo Diagnostics---Failed to get previous configuration!\n")); 
    }

    return 0;
}
#endif


