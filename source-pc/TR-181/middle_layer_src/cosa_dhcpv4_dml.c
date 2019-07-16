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

    module: cosa_dhcpv4_dml.c

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

        01/18/2011    initial revision.

**************************************************************************/

#include "ansc_platform.h"
#include "cosa_dhcpv4_dml.h"
#include "cosa_dhcpv4_apis.h"
#include "cosa_dhcpv4_internal.h"
#include "plugin_main_apis.h"
#if 1//LNT_EMU
#include "ccsp_hal_dhcpv4_emu_api.h"
#include "dmsb_tr181_psm_definitions.h"
// for PSM access
extern ANSC_HANDLE bus_handle;
extern char g_Subsystem[32];
// PSM access MACRO
#define _PSM_WRITE_PARAM(_PARAM_NAME) { \
        _ansc_sprintf(param_name, _PARAM_NAME, instancenum); \
        retPsmSet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, param_name, ccsp_string, param_value); \
        if (retPsmSet != CCSP_SUCCESS) { \
            AnscTraceFlow(("%s Error %d writing %s %s\n", __FUNCTION__, retPsmSet, param_name, param_value));\
        } \
        else \
        { \
            AnscTraceFlow(("%s: retPsmSet == CCSP_SUCCESS writing %s = %s \n", __FUNCTION__,param_name,param_value)); \
        } \
        _ansc_memset(param_name, 0, sizeof(param_name)); \
        _ansc_memset(param_value, 0, sizeof(param_value)); \
    }

#define _PSM_READ_PARAM(_PARAM_NAME) { \
        _ansc_memset(param_name, 0, sizeof(param_name)); \
        _ansc_sprintf(param_name, _PARAM_NAME, instancenum); \
        retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, param_name, NULL, &param_value); \
        if (retPsmGet != CCSP_SUCCESS) { \
            AnscTraceFlow(("%s Error %d reading %s %s\n", __FUNCTION__, retPsmGet, param_name, param_value));\
        } \
        else { \
            AnscTraceFlow(("%s: retPsmGet == CCSP_SUCCESS reading %s = \n%s\n", __FUNCTION__,param_name, param_value)); \
        } \
    }
#define _PSM_DEL_PARAM(_PARAM_NAME) { \
        _ansc_sprintf(param_name,_PARAM_NAME,ins); \
        AnscTraceFlow(("%s ins = %ld param_name=%s\n", __FUNCTION__,\
                                ins,param_name));\
        PSM_Del_Record(bus_handle, g_Subsystem, param_name); \
        _ansc_memset(param_name, 0, sizeof(param_name)); \
}
#endif
extern void* g_pDslhDmlAgent;
//PSM-ACCESS
void PSM_Set_ReversedIP_RecordValues(PCOSA_DML_DHCPS_SADDR pDhcpStaticAddress)
{
	int retPsmSet = CCSP_SUCCESS;
	char param_name[256] = {0};
	char param_value[256] = {0};
	ULONG instancenum;
	instancenum = pDhcpStaticAddress->InstanceNumber;
	sprintf(param_value,"%02x:%02x:%02x:%02x:%02x:%02x",pDhcpStaticAddress->Chaddr[0],pDhcpStaticAddress->Chaddr[1],pDhcpStaticAddress->Chaddr[2],pDhcpStaticAddress->Chaddr[3],pDhcpStaticAddress->Chaddr[4],pDhcpStaticAddress->Chaddr[5]);
	_PSM_WRITE_PARAM(PSM_DHCPV4_SERVER_POOL_STATICADDRESS_CHADDR);
	struct in_addr ip_addr;
	ip_addr.s_addr = pDhcpStaticAddress->Yiaddr.Value;
	inet_ntop(AF_INET, &ip_addr, param_value, sizeof param_value);
	_PSM_WRITE_PARAM(PSM_DHCPV4_SERVER_POOL_STATICADDRESS_YIADDR);
	sprintf(param_value,pDhcpStaticAddress->DeviceName);
	_PSM_WRITE_PARAM(PSM_DHCPV4_SERVER_POOL_STATICADDRESS_X_CISCO_COM_DEVICENAME);
	if(pDhcpStaticAddress->bEnabled == TRUE)
		AnscCopyString(param_value,"TRUE");
	else
		AnscCopyString(param_value,"FALSE");
	_PSM_WRITE_PARAM(PSM_DHCPV4_SERVER_POOL_STATICADDRESS_ENABLE);
	sprintf(param_value,pDhcpStaticAddress->Alias);
	_PSM_WRITE_PARAM(PSM_DHCPV4_SERVER_POOL_STATICADDRESS_ALIAS);
	sprintf(param_value,pDhcpStaticAddress->Comment);
	_PSM_WRITE_PARAM(PSM_DHCPV4_SERVER_POOL_STATICADDRESS_X_CISCO_COM_COMMENT);
}
void PSM_Del_ReversedIP_RecordValues(ULONG ins)
{
	char param_name[256] = {0};
	_PSM_DEL_PARAM(PSM_DHCPV4_SERVER_POOL_STATICADDRESS_CHADDR);
	_PSM_DEL_PARAM(PSM_DHCPV4_SERVER_POOL_STATICADDRESS_YIADDR);
	_PSM_DEL_PARAM(PSM_DHCPV4_SERVER_POOL_STATICADDRESS_X_CISCO_COM_DEVICENAME);
	_PSM_DEL_PARAM(PSM_DHCPV4_SERVER_POOL_STATICADDRESS_X_CISCO_COM_COMMENT);
	_PSM_DEL_PARAM(PSM_DHCPV4_SERVER_POOL_STATICADDRESS_ENABLE);
	_PSM_DEL_PARAM(PSM_DHCPV4_SERVER_POOL_STATICADDRESS_ALIAS);
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

    DHCPv4.

    *  DHCPv4_GetParamBoolValue
    *  DHCPv4_GetParamIntValue
    *  DHCPv4_GetParamUlongValue
    *  DHCPv4_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DHCPv4_GetParamBoolValue
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
DHCPv4_GetParamBoolValue
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
        DHCPv4_GetParamIntValue
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
DHCPv4_GetParamIntValue
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
        DHCPv4_GetParamUlongValue
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
DHCPv4_GetParamUlongValue
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
        DHCPv4_GetParamStringValue
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
DHCPv4_GetParamStringValue
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

    DHCPv4.Client.{i}.

    *  Client_GetEntryCount
    *  Client_GetEntry
    *  Client_AddEntry
    *  Client_DelEntry
    *  Client_GetParamBoolValue
    *  Client_GetParamIntValue
    *  Client_GetParamUlongValue
    *  Client_GetParamStringValue
    *  Client_SetParamBoolValue
    *  Client_SetParamIntValue
    *  Client_SetParamUlongValue
    *  Client_SetParamStringValue
    *  Client_Validate
    *  Client_Commit
    *  Client_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Client_GetEntryCount
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
Client_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DHCPV4          pDhcpv4           = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;

    return AnscSListQueryDepth( &pDhcpv4->ClientList );
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Client_GetEntry
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
Client_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PSINGLE_LINK_ENTRY              pSListEntry       = NULL;
    PCOSA_DATAMODEL_DHCPV4          pDhcpv4           = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;
    PCOSA_CONTEXT_DHCPC_LINK_OBJECT pCxtLink          = NULL;

    pSListEntry = AnscSListGetEntryByIndex(&pDhcpv4->ClientList, nIndex);

    if ( pSListEntry )
    {
        pCxtLink          = ACCESS_COSA_CONTEXT_DHCPC_LINK_OBJECT(pSListEntry);
        *pInsNumber       = pCxtLink->InstanceNumber;
    }

    return pSListEntry;
    
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Client_AddEntry
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
Client_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PSINGLE_LINK_ENTRY              pSListEntry       = NULL;
    PCOSA_DATAMODEL_DHCPV4          pDhcpv4           = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;
    PCOSA_CONTEXT_DHCPC_LINK_OBJECT pCxtLink          = NULL;
    PCOSA_DML_DHCPC_FULL            pDhcpc            = NULL;
    ULONG                           i                 = 0;
    
    pDhcpc  = (PCOSA_DML_DHCPC_FULL)AnscAllocateMemory( sizeof(COSA_DML_DHCPC_FULL) );
    if ( !pDhcpc )
    {
        goto EXIT2;
    }

    /* Set default value */
    DHCPV4_CLIENT_SET_DEFAULTVALUE(pDhcpc);

    /* Add into our link tree*/    
    pCxtLink = (PCOSA_CONTEXT_DHCPC_LINK_OBJECT)AnscAllocateMemory( sizeof(COSA_CONTEXT_DHCPC_LINK_OBJECT) );
    if ( !pDhcpc )
    {
        goto EXIT1;
    }

    DHCPV4_CLIENT_INITIATION_CONTEXT(pCxtLink)

    pCxtLink->hContext       = (ANSC_HANDLE)pDhcpc;
    pCxtLink->bNew           = TRUE;
    
    if ( !++pDhcpv4->maxInstanceOfClient )
    {
        pDhcpv4->maxInstanceOfClient = 1;
    }
    pDhcpc->Cfg.InstanceNumber = pDhcpv4->maxInstanceOfClient;
    pCxtLink->InstanceNumber   = pDhcpc->Cfg.InstanceNumber;
    *pInsNumber                = pDhcpc->Cfg.InstanceNumber;

    _ansc_sprintf( pDhcpc->Cfg.Alias, "Client%d", pDhcpc->Cfg.InstanceNumber);

    /* Put into our list */
    CosaSListPushEntryByInsNum(&pDhcpv4->ClientList, (PCOSA_CONTEXT_LINK_OBJECT)pCxtLink);

    /* we recreate the configuration because we has new delay_added entry for dhcpv4 */
    CosaDhcpv4RegSetDhcpv4Info(pDhcpv4);

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
        Client_DelEntry
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
Client_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PSINGLE_LINK_ENTRY              pSListEntry       = NULL;
    PCOSA_DATAMODEL_DHCPV4          pDhcpv4           = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;
    PCOSA_CONTEXT_DHCPC_LINK_OBJECT pCxtLink          = (PCOSA_CONTEXT_DHCPC_LINK_OBJECT)hInstance;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink2         = NULL;
    PCOSA_DML_DHCPC_FULL            pDhcpc            = (PCOSA_DML_DHCPC_FULL)pCxtLink->hContext;

    /* Normally, two sublinks are empty because our framework will firstly 
            call delEntry for them before coming here. We needn't care them.
         */
    if ( !pCxtLink->bNew )
    {
        returnStatus = CosaDmlDhcpcDelEntry(NULL, pDhcpc->Cfg.InstanceNumber);
        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            return returnStatus;
        }
    }
    
    if (AnscSListPopEntryByLink(&pDhcpv4->ClientList, &pCxtLink->Linkage) )
    {
        /* Because the current NextInstanceNumber information need be deleted */
        CosaDhcpv4RegSetDhcpv4Info(pDhcpv4);
        
        AnscFreeMemory(pCxtLink->hContext);
        AnscFreeMemory(pCxtLink);
    }
    
    return returnStatus;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Client_GetParamBoolValue
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
Client_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_DHCPC_LINK_OBJECT pCxtLink          = (PCOSA_CONTEXT_DHCPC_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPC_FULL            pDhcpc            = (PCOSA_DML_DHCPC_FULL)pCxtLink->hContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* collect value */
        *pBool   = pDhcpc->Cfg.bEnabled;
        
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Renew", TRUE))
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
        Client_GetParamIntValue
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
Client_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_DHCPC_LINK_OBJECT pCxtLink          = (PCOSA_CONTEXT_DHCPC_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPC_FULL            pDhcpc            = (PCOSA_DML_DHCPC_FULL)pCxtLink->hContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "LeaseTimeRemaining", TRUE))
    {
        /* collect value */
        CosaDmlDhcpcGetInfo(NULL, pCxtLink->InstanceNumber, &pDhcpc->Info);
        
        *pInt   = pDhcpc->Info.LeaseTimeRemaining;
        
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Client_GetParamUlongValue
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
Client_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_DHCPC_LINK_OBJECT pCxtLink          = (PCOSA_CONTEXT_DHCPC_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPC_FULL            pDhcpc            = (PCOSA_DML_DHCPC_FULL)pCxtLink->hContext;


    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Status", TRUE))
    {
        /* collect value */
        CosaDmlDhcpcGetInfo(NULL, pCxtLink->InstanceNumber, &pDhcpc->Info);

        *puLong   = pDhcpc->Info.Status;
        
        return TRUE;
    }

    if( AnscEqualString(ParamName, "DHCPStatus", TRUE))
    {
        /* collect value */
        CosaDmlDhcpcGetInfo(NULL, pCxtLink->InstanceNumber, &pDhcpc->Info);

        *puLong   = pDhcpc->Info.DHCPStatus;
        
        return TRUE;
    }

    if( AnscEqualString(ParamName, "IPAddress", TRUE))
    {
        /* collect value */
        if ( pDhcpc->Info.DHCPStatus == COSA_DML_DHCPC_STATUS_Bound )
        {
            CosaDmlDhcpcGetInfo(NULL, pCxtLink->InstanceNumber, &pDhcpc->Info);
            *puLong = pDhcpc->Info.IPAddress.Value;
        }
        else
        {
            *puLong = 0;
        }
        
        
        return TRUE;
    }

    if( AnscEqualString(ParamName, "SubnetMask", TRUE))
    {
        /* collect value */
        if ( pDhcpc->Info.DHCPStatus == COSA_DML_DHCPC_STATUS_Bound )
        {
            CosaDmlDhcpcGetInfo(NULL, pCxtLink->InstanceNumber, &pDhcpc->Info);
            *puLong = pDhcpc->Info.SubnetMask.Value;
        }
        else
        {
            *puLong = 0;
        }
        
        return TRUE;
    }

    if( AnscEqualString(ParamName, "DHCPServer", TRUE))
    {
        /* collect value */
        if ( !pDhcpc->Info.DHCPServer.Value )
        {
            CosaDmlDhcpcGetInfo(NULL, pCxtLink->InstanceNumber, &pDhcpc->Info);
        }
        
        *puLong = pDhcpc->Info.DHCPServer.Value;
        
        return TRUE;
    }
    
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Client_GetParamStringValue
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
Client_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_DHCPC_LINK_OBJECT pCxtLink          = (PCOSA_CONTEXT_DHCPC_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPC_FULL            pDhcpc            = (PCOSA_DML_DHCPC_FULL)pCxtLink->hContext;
    CHAR                            tmpBuff[128]      = {0};
    ULONG                           i                 = 0;
    ULONG                           len               = 0;
    PUCHAR                          pString           = NULL;
    
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE))
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
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_BootFileName", TRUE))
    {
        /* collect value */
        if ( AnscSizeOfString(pDhcpc->Cfg.X_CISCO_COM_BootFileName) < *pUlSize)
        {
            AnscCopyString(pValue, pDhcpc->Cfg.X_CISCO_COM_BootFileName);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDhcpc->Cfg.X_CISCO_COM_BootFileName)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "Interface", TRUE))
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

    }

    if( AnscEqualString(ParamName, "IPRouters", TRUE))
    {
        /* collect value */
        CosaDmlDhcpcGetInfo(NULL, pCxtLink->InstanceNumber, &pDhcpc->Info);
        if ( pDhcpc->Info.DHCPStatus != COSA_DML_DHCPC_STATUS_Bound )
        {
            *pValue    = '\0';
            return 0;
        }
        
        AnscZeroMemory(tmpBuff, sizeof(tmpBuff));
        for( i=0; i<pDhcpc->Info.NumIPRouters && i<COSA_DML_DHCP_MAX_ENTRIES; i++)
        {
            len = AnscSizeOfString(tmpBuff);
            
            if(i > 0)
                tmpBuff[len++] = ',';
          
            AnscCopyString( &tmpBuff[len], _ansc_inet_ntoa(*((struct in_addr *)(&pDhcpc->Info.IPRouters[i]))) );
        }
       	CcspHaldhcpv4cGetGw(tmpBuff);//RDKB-EMU 
        if ( AnscSizeOfString(tmpBuff) < *pUlSize)
        {
            AnscCopyString(pValue, tmpBuff);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDhcpc->Cfg.Interface)+1;
            return 1;
        }

        return 0;
    }

    if( AnscEqualString(ParamName, "DNSServers", TRUE))
    {
        /* collect value */
        
        if ( pDhcpc->Info.DHCPStatus == COSA_DML_DHCPC_STATUS_Bound )
        {
            CosaDmlDhcpcGetInfo(NULL, pCxtLink->InstanceNumber, &pDhcpc->Info);
        }
        else
        {
            *pValue    = '\0';
            return 0;
        }
        
        AnscZeroMemory(tmpBuff, sizeof(tmpBuff));
        for( i=0; i<pDhcpc->Info.NumDnsServers && i<COSA_DML_DHCP_MAX_ENTRIES; i++)
        {
            len = AnscSizeOfString(tmpBuff);
            
            if(i > 0)
                tmpBuff[len++] = ',';
          
            AnscCopyString( &tmpBuff[len], _ansc_inet_ntoa(*((struct in_addr *)(&pDhcpc->Info.DNSServers[i]))) );
        }
        
        if ( AnscSizeOfString(tmpBuff) < *pUlSize)
        {
            AnscCopyString(pValue, tmpBuff);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDhcpc->Cfg.Interface)+1;
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
        Client_SetParamBoolValue
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
Client_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_DHCPC_LINK_OBJECT pCxtLink          = (PCOSA_CONTEXT_DHCPC_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPC_FULL            pDhcpc            = (PCOSA_DML_DHCPC_FULL)pCxtLink->hContext;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* save update to backup */
        pDhcpc->Cfg.bEnabled = bValue;
        
        return  TRUE;
    }

    if( AnscEqualString(ParamName, "Renew", TRUE))
    {
        /* save update to backup */
        if ( bValue )
        {
            returnStatus = CosaDmlDhcpcRenew(NULL, pDhcpc->Cfg.InstanceNumber);
            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                return  FALSE;
            }
        }
        
        return  TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return  FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Client_SetParamIntValue
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
Client_SetParamIntValue
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
        Client_SetParamUlongValue
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
Client_SetParamUlongValue
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
        Client_SetParamStringValue
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
Client_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DHCPV4          pDhcpv4           = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;
    PCOSA_CONTEXT_DHCPC_LINK_OBJECT pCxtLink          = (PCOSA_CONTEXT_DHCPC_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPC_FULL            pDhcpc            = (PCOSA_DML_DHCPC_FULL)pCxtLink->hContext;
    PSINGLE_LINK_ENTRY              pSListEntry       = NULL;
    PCOSA_DML_DHCPC_FULL            pDhcpc2           = NULL;
    BOOL                            bFound            = FALSE;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE))
    {
        AnscCopyString(pDhcpv4->AliasOfClient, pDhcpc->Cfg.Alias);

        AnscCopyString(pDhcpc->Cfg.Alias, pString);
        
        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_BootFileName", TRUE))
    {
        AnscCopyString(pDhcpc->Cfg.X_CISCO_COM_BootFileName, pString);
        
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Interface", TRUE))
    {
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
        Client_Validate
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
Client_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DHCPV4          pDhcpv4           = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;
    PCOSA_CONTEXT_DHCPC_LINK_OBJECT pCxtLink          = (PCOSA_CONTEXT_DHCPC_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPC_FULL            pDhcpc            = (PCOSA_DML_DHCPC_FULL)pCxtLink->hContext;
    PSINGLE_LINK_ENTRY              pSListEntry       = NULL;
    PCOSA_DML_DHCPC_FULL            pDhcpc2           = NULL;
    BOOL                            bFound            = FALSE;

    /*  only for Alias */
    if ( pDhcpv4->AliasOfClient[0] )
    {
        pSListEntry           = AnscSListGetFirstEntry(&pDhcpv4->ClientList);
        while( pSListEntry != NULL)
        {
            pCxtLink          = ACCESS_COSA_CONTEXT_DHCPC_LINK_OBJECT(pSListEntry);
            pSListEntry       = AnscSListGetNextEntry(pSListEntry);
        
            pDhcpc2 = (PCOSA_DML_DHCPC_FULL)pCxtLink->hContext;
        
            if( DHCPV4_CLIENT_ENTRY_MATCH2(pDhcpc2->Cfg.Alias, pDhcpc->Cfg.Alias) )
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
            Client_Rollback(hInsContext);
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
        Client_Commit
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
Client_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_DHCPC_LINK_OBJECT pCxtLink          = (PCOSA_CONTEXT_DHCPC_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPC_FULL            pDhcpc            = (PCOSA_DML_DHCPC_FULL)pCxtLink->hContext;
    PCOSA_DATAMODEL_DHCPV4          pDhcpv4           = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;

    if ( pCxtLink->bNew )
    {
        returnStatus = CosaDmlDhcpcAddEntry(NULL, pDhcpc );

        if ( returnStatus == ANSC_STATUS_SUCCESS )
        {
            pCxtLink->bNew = FALSE;

            CosaDhcpv4RegSetDhcpv4Info(pDhcpv4);
        }
        else
        {
            DHCPV4_CLIENT_SET_DEFAULTVALUE(pDhcpc);
            
            if ( pDhcpv4->AliasOfClient[0] )
                AnscCopyString( pDhcpc->Cfg.Alias, pDhcpv4->AliasOfClient );
        }
    }
    else
    {
        returnStatus = CosaDmlDhcpcSetCfg(NULL, &pDhcpc->Cfg);

        if ( returnStatus != ANSC_STATUS_SUCCESS)
        {
            CosaDmlDhcpcGetCfg(NULL, &pDhcpc->Cfg);
        }
    }
    
    AnscZeroMemory( pDhcpv4->AliasOfClient, sizeof(pDhcpv4->AliasOfClient) );

    return returnStatus;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Client_Rollback
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
Client_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DHCPV4          pDhcpv4           = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;
    PCOSA_CONTEXT_DHCPC_LINK_OBJECT pCxtLink          = (PCOSA_CONTEXT_DHCPC_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPC_FULL            pDhcpc            = (PCOSA_DML_DHCPC_FULL)pCxtLink->hContext;

    if ( pDhcpv4->AliasOfClient[0] )
        AnscCopyString( pDhcpc->Cfg.Alias, pDhcpv4->AliasOfClient );

    if ( !pCxtLink->bNew )
    {
        CosaDmlDhcpcGetCfg( NULL, &pDhcpc->Cfg );
    }
    else
    {
        DHCPV4_CLIENT_SET_DEFAULTVALUE(pDhcpc);
    }
    
    AnscZeroMemory( pDhcpv4->AliasOfClient, sizeof(pDhcpv4->AliasOfClient) );
    
    return returnStatus;
}

/***********************************************************************

 APIs for Object:

    DHCPv4.Client.{i}.SentOption.{i}.

    *  SentOption_GetEntryCount
    *  SentOption_GetEntry
    *  SentOption_AddEntry
    *  SentOption_DelEntry
    *  SentOption_GetParamBoolValue
    *  SentOption_GetParamIntValue
    *  SentOption_GetParamUlongValue
    *  SentOption_GetParamStringValue
    *  SentOption_SetParamBoolValue
    *  SentOption_SetParamIntValue
    *  SentOption_SetParamUlongValue
    *  SentOption_SetParamStringValue
    *  SentOption_Validate
    *  SentOption_Commit
    *  SentOption_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        SentOption_GetEntryCount
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
SentOption_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_DHCPC_LINK_OBJECT pCxtLink          = (PCOSA_CONTEXT_DHCPC_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPC_FULL            pDhcpc            = (PCOSA_DML_DHCPC_FULL)pCxtLink->hContext;

    return AnscSListQueryDepth( &pCxtLink->SendOptionList );
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        SentOption_GetEntryStatus
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
BOOL
SentOption_GetEntryStatus
    (
        ANSC_HANDLE                 hInsContext,
        PCHAR                       StatusName
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_DHCPC_LINK_OBJECT pCxtLink          = (PCOSA_CONTEXT_DHCPC_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPC_FULL            pDhcpc            = (PCOSA_DML_DHCPC_FULL)pCxtLink->hContext;

    if( AnscEqualString(StatusName, "Committed", TRUE))
    {
        /* collect value */
        if ( pCxtLink->bNew )
            return FALSE;
        
    }

    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        SentOption_GetEntry
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
SentOption_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_CONTEXT_DHCPC_LINK_OBJECT pCxtDhcpcLink        = (PCOSA_CONTEXT_DHCPC_LINK_OBJECT)hInsContext;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink             = NULL;
    PSINGLE_LINK_ENTRY              pSListEntry          = NULL;

    pSListEntry = AnscSListGetEntryByIndex(&pCxtDhcpcLink->SendOptionList, nIndex);

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
        SentOption_AddEntry
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
SentOption_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_DHCPC_LINK_OBJECT pCxtDhcpcLink        = (PCOSA_CONTEXT_DHCPC_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPC_FULL            pDhcpc               = (PCOSA_DML_DHCPC_FULL)pCxtDhcpcLink->hContext;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink             = NULL;
    PCOSA_DML_DHCP_OPT              pDhcpSendOption      = NULL;
    
    pDhcpSendOption  = (PCOSA_DML_DHCP_OPT)AnscAllocateMemory( sizeof(COSA_DML_DHCP_OPT) );
    if ( !pDhcpSendOption )
    {
        goto EXIT2;
    }

    DHCPV4_SENDOPTION_SET_DEFAULTVALUE(pDhcpSendOption);
    
    pCxtLink = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory( sizeof(COSA_CONTEXT_LINK_OBJECT) );
    if ( !pCxtLink )
    {
        goto EXIT1;
    }

    pCxtLink->hContext       = (ANSC_HANDLE)pDhcpSendOption;
    pCxtLink->hParentTable   = (ANSC_HANDLE)pCxtDhcpcLink;
    pCxtLink->bNew           = TRUE;
    
    if ( !++pCxtDhcpcLink->maxInstanceOfSend )
    {
        pCxtDhcpcLink->maxInstanceOfSend = 1;
    }
    
    pDhcpSendOption->InstanceNumber = pCxtDhcpcLink->maxInstanceOfSend; 
    pCxtLink->InstanceNumber       = pDhcpSendOption->InstanceNumber;
    *pInsNumber                    = pDhcpSendOption->InstanceNumber;

    _ansc_sprintf( pDhcpSendOption->Alias, "SentOption%d", pDhcpSendOption->InstanceNumber);

    /* Put into our list */
    CosaSListPushEntryByInsNum(&pCxtDhcpcLink->SendOptionList, (PCOSA_CONTEXT_LINK_OBJECT)pCxtLink);

    /* we recreate the configuration */
    CosaDhcpv4RegSetDhcpv4Info((ANSC_HANDLE)g_pCosaBEManager->hDhcpv4);

    return (ANSC_HANDLE)pCxtLink;    
       
EXIT1:
        
    AnscFreeMemory(pDhcpSendOption);
    
EXIT2:   
        
    return NULL;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        SentOption_DelEntry
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
SentOption_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_DHCPC_LINK_OBJECT pCxtDhcpcLink        = (PCOSA_CONTEXT_DHCPC_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPC_FULL            pDhcpClient          = (PCOSA_DML_DHCPC_FULL)pCxtDhcpcLink->hContext;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink             = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_DML_DHCP_OPT              pDhcpSendOption      = (PCOSA_DML_DHCP_OPT)pCxtLink->hContext;
    PCOSA_DATAMODEL_DHCPV4          pDhcpv4              = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;

    if ( !pCxtLink->bNew )
    {
        returnStatus = CosaDmlDhcpcDelSentOption(NULL, pDhcpClient->Cfg.InstanceNumber, pDhcpSendOption->InstanceNumber);
        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            return returnStatus;
        }
    }

    if ( AnscSListPopEntryByLink(&pCxtDhcpcLink->SendOptionList, &pCxtLink->Linkage) )
    {
        CosaDhcpv4RegSetDhcpv4Info(pDhcpv4);
        
        AnscFreeMemory(pCxtLink->hContext);
        AnscFreeMemory(pCxtLink);
    }

    return returnStatus;    

}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        SentOption_GetParamBoolValue
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
SentOption_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink             = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCP_OPT              pDhcpSendOption      = (PCOSA_DML_DHCP_OPT)pCxtLink->hContext;
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* collect value */
        *pBool  = pDhcpSendOption->bEnabled;
        
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        SentOption_GetParamIntValue
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
SentOption_GetParamIntValue
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
        SentOption_GetParamUlongValue
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
SentOption_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink             = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCP_OPT              pDhcpSendOption      = (PCOSA_DML_DHCP_OPT)pCxtLink->hContext;
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Tag", TRUE))
    {
        /* collect value */
        *puLong = pDhcpSendOption->Tag;
        
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        SentOption_GetParamStringValue
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
SentOption_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink             = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCP_OPT              pDhcpSendOption      = (PCOSA_DML_DHCP_OPT)pCxtLink->hContext;
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE))
    {
        /* collect value */
        if ( AnscSizeOfString(pDhcpSendOption->Alias) < *pUlSize)
        {
            AnscCopyString(pValue, pDhcpSendOption->Alias);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDhcpSendOption->Alias)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "Value", TRUE))
    {
        /* collect value */
        if ( AnscSizeOfString(pDhcpSendOption->Value) < *pUlSize)
        {
            AnscCopyString(pValue, pDhcpSendOption->Value);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDhcpSendOption->Value)+1;
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
        SentOption_SetParamBoolValue
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
SentOption_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink             = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCP_OPT              pDhcpSendOption      = (PCOSA_DML_DHCP_OPT)pCxtLink->hContext;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* save update to backup */
        pDhcpSendOption->bEnabled  = bValue;
        
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        SentOption_SetParamIntValue
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
SentOption_SetParamIntValue
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
        SentOption_SetParamUlongValue
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
SentOption_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink             = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCP_OPT              pDhcpSendOption      = (PCOSA_DML_DHCP_OPT)pCxtLink->hContext;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Tag", TRUE))
    {
        /* save update to backup */
        pDhcpSendOption->Tag  = (UCHAR)uValue;
        
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        SentOption_SetParamStringValue
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
SentOption_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCP_OPT              pDhcpSendOption   = (PCOSA_DML_DHCP_OPT)pCxtLink->hContext;
    PCOSA_CONTEXT_DHCPC_LINK_OBJECT pCxtDhcpcLink     = (PCOSA_CONTEXT_DHCPC_LINK_OBJECT)pCxtLink->hParentTable;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink2         = NULL;
    PCOSA_DML_DHCP_OPT              pDhcpSendOption2  = NULL;
    PSINGLE_LINK_ENTRY              pSListEntry       = NULL;
    BOOL                            bFound            = FALSE;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE))
    {
        AnscCopyString(pCxtDhcpcLink->AliasOfSend, pDhcpSendOption->Alias);

        AnscCopyString(pDhcpSendOption->Alias, pString);
        
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Value", TRUE))
    {
        /* save update to backup */
        AnscCopyString(pDhcpSendOption->Value, pString);

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        SentOption_Validate
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
SentOption_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCP_OPT              pDhcpSendOption   = (PCOSA_DML_DHCP_OPT)pCxtLink->hContext;
    PCOSA_CONTEXT_DHCPC_LINK_OBJECT pCxtDhcpcLink     = (PCOSA_CONTEXT_DHCPC_LINK_OBJECT)pCxtLink->hParentTable;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink2         = NULL;
    PCOSA_DML_DHCP_OPT              pDhcpSendOption2  = NULL;
    PSINGLE_LINK_ENTRY              pSListEntry       = NULL;
    BOOL                            bFound            = FALSE;

    /* Parent hasn't set, we don't permit child is set.*/
    if ( pCxtDhcpcLink->bNew )
    {
#if COSA_DHCPV4_ROLLBACK_TEST        
        SentOption_Rollback(hInsContext);
#endif
        return FALSE;
    }

    /* This is for alias */
    if ( pCxtDhcpcLink->AliasOfSend[0] )
    {
        bFound                = FALSE;
        pSListEntry           = AnscSListGetFirstEntry(&pCxtDhcpcLink->SendOptionList);
        while( pSListEntry != NULL)
        {
            pCxtLink2         = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSListEntry);
            pSListEntry       = AnscSListGetNextEntry(pSListEntry);

            pDhcpSendOption2  = (PCOSA_DML_DHCP_OPT)pCxtLink2->hContext;

            if( DHCPV4_SENDOPTION_ENTRY_MATCH2(pDhcpSendOption->Alias, pDhcpSendOption2->Alias) )
            {
                if ( (ANSC_HANDLE)pCxtLink2 == hInsContext )
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
            SentOption_Rollback(hInsContext);
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
        SentOption_Commit
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
SentOption_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCP_OPT              pDhcpSendOption   = (PCOSA_DML_DHCP_OPT)pCxtLink->hContext;
    PCOSA_CONTEXT_DHCPC_LINK_OBJECT pCxtDhcpcLink     = (PCOSA_CONTEXT_DHCPC_LINK_OBJECT)pCxtLink->hParentTable;
    PCOSA_DML_DHCPC_FULL            pDhcpClient       = (PCOSA_DML_DHCPC_FULL)pCxtDhcpcLink->hContext;
    PCOSA_DATAMODEL_DHCPV4          pDhcpv4           = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;

    if ( pCxtLink->bNew )
    {
        returnStatus = CosaDmlDhcpcAddSentOption(NULL, pDhcpClient->Cfg.InstanceNumber, pDhcpSendOption );

        if ( returnStatus == ANSC_STATUS_SUCCESS )
        {
            pCxtLink->bNew = FALSE;

            CosaDhcpv4RegSetDhcpv4Info(pDhcpv4);
        }
        else
        {
            DHCPV4_SENDOPTION_SET_DEFAULTVALUE(pDhcpSendOption);

            if ( pCxtDhcpcLink->AliasOfSend[0] )
                AnscCopyString( pDhcpSendOption->Alias, pCxtDhcpcLink->AliasOfSend );
        }
    }
    else
    {
        returnStatus = CosaDmlDhcpcSetSentOption(NULL, pDhcpClient->Cfg.InstanceNumber, pDhcpSendOption);

        if ( returnStatus != ANSC_STATUS_SUCCESS)
        {
            CosaDmlDhcpcGetSentOptionbyInsNum(NULL, pDhcpClient->Cfg.InstanceNumber, pDhcpSendOption);
        }
    }
    
    AnscZeroMemory( pCxtDhcpcLink->AliasOfSend, sizeof(pCxtDhcpcLink->AliasOfSend) );
    
    return returnStatus;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        SentOption_Rollback
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
SentOption_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCP_OPT              pDhcpSendOption   = (PCOSA_DML_DHCP_OPT)pCxtLink->hContext;
    PCOSA_CONTEXT_DHCPC_LINK_OBJECT pCxtDhcpcLink     = (PCOSA_CONTEXT_DHCPC_LINK_OBJECT)pCxtLink->hParentTable;
    PCOSA_DML_DHCPC_FULL            pDhcpc            = (PCOSA_DML_DHCPC_FULL)pCxtDhcpcLink->hContext;

    if ( pCxtDhcpcLink->AliasOfSend[0] )
        AnscCopyString( pDhcpSendOption->Alias, pCxtDhcpcLink->AliasOfSend );

    if ( !pCxtLink->bNew )
    {
        CosaDmlDhcpcGetSentOptionbyInsNum(NULL, pDhcpc->Cfg.InstanceNumber, pDhcpSendOption);
    }
    else
    {
        DHCPV4_SENDOPTION_SET_DEFAULTVALUE(pDhcpSendOption);
    }
    
    AnscZeroMemory( pCxtDhcpcLink->AliasOfSend, sizeof(pCxtDhcpcLink->AliasOfSend) );
    
    return returnStatus;
}

/***********************************************************************

 APIs for Object:

    DHCPv4.Client.{i}.ReqOption.{i}.

    *  ReqOption_GetEntryCount
    *  ReqOption_GetEntry
    *  ReqOption_AddEntry
    *  ReqOption_DelEntry
    *  ReqOption_GetParamBoolValue
    *  ReqOption_GetParamIntValue
    *  ReqOption_GetParamUlongValue
    *  ReqOption_GetParamStringValue
    *  ReqOption_SetParamBoolValue
    *  ReqOption_SetParamIntValue
    *  ReqOption_SetParamUlongValue
    *  ReqOption_SetParamStringValue
    *  ReqOption_Validate
    *  ReqOption_Commit
    *  ReqOption_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        ReqOption_GetEntryCount
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
ReqOption_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_DHCPC_LINK_OBJECT pCxtLink          = (PCOSA_CONTEXT_DHCPC_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPC_FULL            pDhcpc            = (PCOSA_DML_DHCPC_FULL)pCxtLink->hContext;

    return AnscSListQueryDepth( &pCxtLink->ReqOptionList );
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        ReqOption_GetEntry
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
ReqOption_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_CONTEXT_DHCPC_LINK_OBJECT pCxtDhcpcLink        = (PCOSA_CONTEXT_DHCPC_LINK_OBJECT)hInsContext;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink             = NULL;
    PSINGLE_LINK_ENTRY              pSListEntry          = NULL;
    
    pSListEntry = AnscSListGetEntryByIndex(&pCxtDhcpcLink->ReqOptionList, nIndex);

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
        ReqOption_AddEntry
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
ReqOption_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_DHCPC_LINK_OBJECT pCxtDhcpcLink        = (PCOSA_CONTEXT_DHCPC_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPC_FULL            pDhcpc               = (PCOSA_DML_DHCPC_FULL)pCxtDhcpcLink->hContext;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink             = NULL;
    PCOSA_DML_DHCPC_REQ_OPT         pDhcpReqOption       = NULL;
    CHAR                            tmpBuff[64]          = {0};
    
    /* We need ask from backend */
    pDhcpReqOption  = (PCOSA_DML_DHCPC_REQ_OPT)AnscAllocateMemory( sizeof(COSA_DML_DHCPC_REQ_OPT) );
    if ( !pDhcpReqOption )
    {
        goto EXIT2;
    }

    DHCPV4_REQOPTION_SET_DEFAULTVALUE(pDhcpReqOption);
    
    pCxtLink = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory( sizeof(COSA_CONTEXT_LINK_OBJECT) );
    if ( !pCxtLink )
    {
        goto EXIT1;
    }

    pCxtLink->hContext       = (ANSC_HANDLE)pDhcpReqOption;
    pCxtLink->hParentTable   = (ANSC_HANDLE)pCxtDhcpcLink;
    pCxtLink->bNew           = TRUE;
    
    if ( !++pCxtDhcpcLink->maxInstanceOfReq )
    {
        pCxtDhcpcLink->maxInstanceOfReq = 1;
    }
    pDhcpReqOption->InstanceNumber = pCxtDhcpcLink->maxInstanceOfReq;
    pCxtLink->InstanceNumber       = pDhcpReqOption->InstanceNumber; 
    *pInsNumber                    = pDhcpReqOption->InstanceNumber;

    _ansc_sprintf( pDhcpReqOption->Alias, "ReqOption%d", pDhcpReqOption->InstanceNumber);

    /* Put into our list */
    CosaSListPushEntryByInsNum(&pCxtDhcpcLink->ReqOptionList, (PCOSA_CONTEXT_LINK_OBJECT)pCxtLink);

    /* we recreate the configuration */
    CosaDhcpv4RegSetDhcpv4Info((ANSC_HANDLE)g_pCosaBEManager->hDhcpv4);

    return (ANSC_HANDLE)pCxtLink;    
    
EXIT1:
        
    AnscFreeMemory(pDhcpReqOption);
    
EXIT2:   
        
    return NULL;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        ReqOption_DelEntry
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
ReqOption_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_DHCPC_LINK_OBJECT pCxtDhcpcLink        = (PCOSA_CONTEXT_DHCPC_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPC_FULL            pDhcpClient          = (PCOSA_DML_DHCPC_FULL)pCxtDhcpcLink->hContext;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink             = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_DML_DHCPC_REQ_OPT         pDhcpReqOption       = (PCOSA_DML_DHCPC_REQ_OPT)pCxtLink->hContext;
    PCOSA_DATAMODEL_DHCPV4          pDhcpv4              = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;

    if ( !pCxtLink->bNew )
    {
        returnStatus = CosaDmlDhcpcDelReqOption( NULL, pDhcpClient->Cfg.InstanceNumber, pDhcpReqOption->InstanceNumber );
        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            return returnStatus;
        }
    }

    if ( AnscSListPopEntryByLink(&pCxtDhcpcLink->ReqOptionList, &pCxtLink->Linkage) )
    {
        CosaDhcpv4RegSetDhcpv4Info(pDhcpv4);

        AnscFreeMemory(pCxtLink->hContext);
        AnscFreeMemory(pCxtLink);
    }

    return returnStatus;    
        
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        ReqOption_GetParamBoolValue
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
ReqOption_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink             = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPC_REQ_OPT         pDhcpReqOption       = (PCOSA_DML_DHCPC_REQ_OPT)pCxtLink->hContext;
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* collect value */
        *pBool    =  pDhcpReqOption->bEnabled;
        
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        ReqOption_GetParamIntValue
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
ReqOption_GetParamIntValue
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
        ReqOption_GetParamUlongValue
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
ReqOption_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink             = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPC_REQ_OPT         pDhcpReqOption       = (PCOSA_DML_DHCPC_REQ_OPT)pCxtLink->hContext;
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Order", TRUE))
    {
        /* collect value */
        *puLong  =  pDhcpReqOption->Order;
        
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Tag", TRUE))
    {
        /* collect value */
        *puLong  =  pDhcpReqOption->Tag;
        
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        ReqOption_GetParamStringValue
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
ReqOption_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink             = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPC_REQ_OPT         pDhcpReqOption       = (PCOSA_DML_DHCPC_REQ_OPT)pCxtLink->hContext;
    PCOSA_CONTEXT_DHCPC_LINK_OBJECT pCxtDhcpcLink     = (PCOSA_CONTEXT_DHCPC_LINK_OBJECT)pCxtLink->hParentTable;
    PCOSA_DML_DHCPC_FULL            pDhcpc            = (PCOSA_DML_DHCPC_FULL)pCxtDhcpcLink->hContext;
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE))
    {
        /* collect value */
        if ( AnscSizeOfString(pDhcpReqOption->Alias) < *pUlSize)
        {
            AnscCopyString(pValue, pDhcpReqOption->Alias);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDhcpReqOption->Alias)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "Value", TRUE))
    {
        /* collect value */
        CosaDmlDhcpcGetReqOptionbyInsNum(NULL, pDhcpc->Cfg.InstanceNumber, pDhcpReqOption);
        
        if ( AnscSizeOfString(pDhcpReqOption->Value) < *pUlSize)
        {
            AnscCopyString(pValue, pDhcpReqOption->Value);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDhcpReqOption->Value)+1;
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
        ReqOption_SetParamBoolValue
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
ReqOption_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink             = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPC_REQ_OPT         pDhcpReqOption       = (PCOSA_DML_DHCPC_REQ_OPT)pCxtLink->hContext;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* save update to backup */
        pDhcpReqOption->bEnabled   =  bValue;
        
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        ReqOption_SetParamIntValue
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
ReqOption_SetParamIntValue
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
        ReqOption_SetParamUlongValue
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
ReqOption_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink             = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPC_REQ_OPT         pDhcpReqOption       = (PCOSA_DML_DHCPC_REQ_OPT)pCxtLink->hContext;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Order", TRUE))
    {
        /* save update to backup */
        pDhcpReqOption->Order   = uValue;
        
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Tag", TRUE))
    {
        /* save update to backup */
        pDhcpReqOption->Tag   = (UCHAR)uValue;
        
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        ReqOption_SetParamStringValue
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
ReqOption_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPC_REQ_OPT         pDhcpReqOption    = (PCOSA_DML_DHCPC_REQ_OPT)pCxtLink->hContext;
    PCOSA_CONTEXT_DHCPC_LINK_OBJECT pCxtDhcpcLink     = (PCOSA_CONTEXT_DHCPC_LINK_OBJECT)pCxtLink->hParentTable;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink2         = NULL;
    PCOSA_DML_DHCPC_REQ_OPT         pDhcpReqOption2   = NULL;
    PSINGLE_LINK_ENTRY              pSListEntry       = NULL;
    BOOL                            bFound            = FALSE;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE))
    {
        /* Backup old alias firstly */
        AnscCopyString(pCxtDhcpcLink->AliasOfReq, pDhcpReqOption->Alias);

        AnscCopyString(pDhcpReqOption->Alias, pString);

        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        ReqOption_Validate
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
ReqOption_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPC_REQ_OPT         pDhcpReqOption    = (PCOSA_DML_DHCPC_REQ_OPT)pCxtLink->hContext;
    PCOSA_CONTEXT_DHCPC_LINK_OBJECT pCxtDhcpcLink     = (PCOSA_CONTEXT_DHCPC_LINK_OBJECT)pCxtLink->hParentTable;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink2         = NULL;
    PCOSA_DML_DHCPC_REQ_OPT         pDhcpReqOption2   = NULL;
    PSINGLE_LINK_ENTRY              pSListEntry       = NULL;
    BOOL                            bFound            = FALSE;

    /* Parent hasn't set, we don't permit child is set.*/
    if ( pCxtDhcpcLink->bNew )
    {
#if COSA_DHCPV4_ROLLBACK_TEST        
        ReqOption_Rollback(hInsContext);
#endif

        return FALSE;
    }

    /* For other check */
    if ( pCxtDhcpcLink->AliasOfReq[0] )
    {
        /* save update to backup */
        bFound                = FALSE;
        pSListEntry           = AnscSListGetFirstEntry(&pCxtDhcpcLink->ReqOptionList);
        while( pSListEntry != NULL)
        {
            pCxtLink2         = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSListEntry);
            pSListEntry       = AnscSListGetNextEntry(pSListEntry);

            pDhcpReqOption2  = (PCOSA_DML_DHCPC_REQ_OPT)pCxtLink2->hContext;

            if( DHCPV4_REQOPTION_ENTRY_MATCH2(pDhcpReqOption->Alias, pDhcpReqOption2->Alias ) )
            {
                if ( (ANSC_HANDLE)pCxtLink2 == hInsContext )
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
            ReqOption_Rollback(hInsContext);
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
        ReqOption_Commit
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
ReqOption_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPC_REQ_OPT         pDhcpReqOption    = (PCOSA_DML_DHCPC_REQ_OPT)pCxtLink->hContext;
    PCOSA_CONTEXT_DHCPC_LINK_OBJECT pCxtDhcpcLink     = (PCOSA_CONTEXT_DHCPC_LINK_OBJECT)pCxtLink->hParentTable;
    PCOSA_DML_DHCPC_FULL            pDhcpClient       = (PCOSA_DML_DHCPC_FULL)pCxtDhcpcLink->hContext;
    PCOSA_DATAMODEL_DHCPV4          pDhcpv4           = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;

    if ( pCxtLink->bNew )
    {
        returnStatus = CosaDmlDhcpcAddReqOption(NULL, pDhcpClient->Cfg.InstanceNumber, pDhcpReqOption );

        if ( returnStatus == ANSC_STATUS_SUCCESS )
        {
            pCxtLink->bNew = FALSE;

            CosaDhcpv4RegSetDhcpv4Info(pDhcpv4);
        }
        else
        {
            DHCPV4_REQOPTION_SET_DEFAULTVALUE(pDhcpReqOption);

            if ( pCxtDhcpcLink->AliasOfReq[0] )
                AnscCopyString( pDhcpReqOption->Alias, pCxtDhcpcLink->AliasOfReq );
        }
    }
    else
    {
        returnStatus = CosaDmlDhcpcSetReqOption(NULL, pDhcpClient->Cfg.InstanceNumber, pDhcpReqOption);

        if ( returnStatus != ANSC_STATUS_SUCCESS)
        {
            CosaDmlDhcpcGetReqOptionbyInsNum(NULL, pDhcpClient->Cfg.InstanceNumber, pDhcpReqOption);
        }
    }
    
    AnscZeroMemory( pCxtDhcpcLink->AliasOfReq, sizeof(pCxtDhcpcLink->AliasOfReq) );
    
    return returnStatus;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        ReqOption_Rollback
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
ReqOption_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPC_REQ_OPT         pDhcpReqOption    = (PCOSA_DML_DHCPC_REQ_OPT)pCxtLink->hContext;
    PCOSA_CONTEXT_DHCPC_LINK_OBJECT pCxtDhcpcLink     = (PCOSA_CONTEXT_DHCPC_LINK_OBJECT)pCxtLink->hParentTable;
    PCOSA_DML_DHCPC_FULL            pDhcpc            = (PCOSA_DML_DHCPC_FULL)pCxtDhcpcLink->hContext;

    if ( pCxtDhcpcLink->AliasOfReq[0] )
        AnscCopyString( pDhcpReqOption->Alias, pCxtDhcpcLink->AliasOfReq );

    if ( !pCxtLink->bNew )
    {
        CosaDmlDhcpcGetReqOptionbyInsNum(NULL, pDhcpc->Cfg.InstanceNumber, pDhcpReqOption);
    }
    else
    {
        DHCPV4_REQOPTION_SET_DEFAULTVALUE(pDhcpReqOption);
    }
    
    AnscZeroMemory( pCxtDhcpcLink->AliasOfReq, sizeof(pCxtDhcpcLink->AliasOfReq) );
    
    return returnStatus;
}

/***********************************************************************

 APIs for Object:

    DHCPv4.Server.

    *  Server_GetParamBoolValue
    *  Server_GetParamIntValue
    *  Server_GetParamUlongValue
    *  Server_GetParamStringValue
    *  Server_SetParamBoolValue
    *  Server_SetParamIntValue
    *  Server_SetParamUlongValue
    *  Server_SetParamStringValue
    *  Server_Validate
    *  Server_Commit
    *  Server_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Server_GetParamBoolValue
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
Server_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_DATAMODEL_DHCPV4          pDhcpv4           = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* collect value */
        *pBool  =  CosaDmlDhcpsGetState(NULL);
        
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Server_GetParamIntValue
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
Server_GetParamIntValue
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
        Server_GetParamUlongValue
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
Server_GetParamUlongValue
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
        Server_GetParamStringValue
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
Server_GetParamStringValue
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
        Server_SetParamBoolValue
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
Server_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
        
    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* save update to backup */
        returnStatus = CosaDmlDhcpsEnable((ANSC_HANDLE)NULL, bValue );

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
        Server_SetParamIntValue
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
Server_SetParamIntValue
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
        Server_SetParamUlongValue
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
Server_SetParamUlongValue
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
        Server_SetParamStringValue
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
Server_SetParamStringValue
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
        Server_Validate
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
Server_Validate
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
        Server_Commit
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
Server_Commit
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
        Server_Rollback
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
Server_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    return 0;
}

/***********************************************************************

 APIs for Object:

    DHCPv4.Server.X_CISCO_COM_StaticAddress.{i}.

    *  X_CISCO_COM_StaticAddress_GetEntryCount
    *  X_CISCO_COM_StaticAddress_GetEntry
    *  X_CISCO_COM_StaticAddress_AddEntry
    *  X_CISCO_COM_StaticAddress_DelEntry
    *  X_CISCO_COM_StaticAddress_GetParamBoolValue
    *  X_CISCO_COM_StaticAddress_GetParamIntValue
    *  X_CISCO_COM_StaticAddress_GetParamUlongValue
    *  X_CISCO_COM_StaticAddress_GetParamStringValue
    *  X_CISCO_COM_StaticAddress_SetParamBoolValue
    *  X_CISCO_COM_StaticAddress_SetParamIntValue
    *  X_CISCO_COM_StaticAddress_SetParamUlongValue
    *  X_CISCO_COM_StaticAddress_SetParamStringValue
    *  X_CISCO_COM_StaticAddress_Validate
    *  X_CISCO_COM_StaticAddress_Commit
    *  X_CISCO_COM_StaticAddress_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        X_CISCO_COM_StaticAddress_GetEntryCount
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
X_CISCO_COM_StaticAddress_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DHCPV4          pDhcpv4           = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;

    return AnscSListQueryDepth( &pDhcpv4->X_CISCO_COM_StaticAddressList );
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        X_CISCO_COM_StaticAddress_GetEntry
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
X_CISCO_COM_StaticAddress_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_DHCPV4          pDhcpv4              = (PCOSA_DATAMODEL_DHCPV4   )g_pCosaBEManager->hDhcpv4;
    PSLIST_HEADER                   pListHead            = (PSLIST_HEADER            )&pDhcpv4->X_CISCO_COM_StaticAddressList;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink             = NULL;
    PSINGLE_LINK_ENTRY              pSListEntry          = NULL;

    pSListEntry = AnscSListGetEntryByIndex(pListHead, nIndex);

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
        X_CISCO_COM_StaticAddress_AddEntry
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
X_CISCO_COM_StaticAddress_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink             = NULL;
    PCOSA_DML_DHCPS_X_CISCO_COM_SADDR pDhcpX_COM_CISCO_StaticAddress = NULL;
    PCOSA_DATAMODEL_DHCPV4          pDhcpv4              = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;
    
    /* We need ask from backend */
    pDhcpX_COM_CISCO_StaticAddress  = (PCOSA_DML_DHCPS_X_CISCO_COM_SADDR)AnscAllocateMemory( sizeof(COSA_DML_DHCPS_X_CISCO_COM_SADDR) );
    if ( !pDhcpX_COM_CISCO_StaticAddress )
    {
        goto EXIT2;
    }

    DHCPV4_X_COM_CISCO_SADDR_SET_DEFAULTVALUE(pDhcpX_COM_CISCO_StaticAddress);

    pCxtLink = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory( sizeof(COSA_CONTEXT_LINK_OBJECT) );
    if ( !pCxtLink )
    {
        goto EXIT1;
    }

    pCxtLink->hContext       = (ANSC_HANDLE)pDhcpX_COM_CISCO_StaticAddress;
    pCxtLink->hParentTable   = NULL;
    pCxtLink->bNew           = TRUE;
    
    if ( !++pDhcpv4->maxInstanceX_CISCO_COM_SAddr )
    {
        pDhcpv4->maxInstanceX_CISCO_COM_SAddr = 1;
    }
    pDhcpX_COM_CISCO_StaticAddress->InstanceNumber = pDhcpv4->maxInstanceX_CISCO_COM_SAddr;
    pCxtLink->InstanceNumber           = pDhcpX_COM_CISCO_StaticAddress->InstanceNumber; 
    *pInsNumber                        = pDhcpX_COM_CISCO_StaticAddress->InstanceNumber;

    _ansc_sprintf( pDhcpX_COM_CISCO_StaticAddress->Alias, "cpe-X_COM_CISCO_SAddr%d", pDhcpX_COM_CISCO_StaticAddress->InstanceNumber);

    /* Put into our list */
    CosaSListPushEntryByInsNum(&pDhcpv4->X_CISCO_COM_StaticAddressList, (PCOSA_CONTEXT_LINK_OBJECT)pCxtLink);

    /* we recreate the configuration */
    CosaDhcpv4RegSetDhcpv4Info(pDhcpv4);

    return (ANSC_HANDLE)pCxtLink;    

EXIT1:
        
    AnscFreeMemory(pDhcpX_COM_CISCO_StaticAddress);
    
EXIT2:   
        
    return NULL;
}



/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        X_CISCO_COM_StaticAddress_DelEntry
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
X_CISCO_COM_StaticAddress_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink             = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_DML_DHCPS_X_CISCO_COM_SADDR pDhcpX_COM_CISCO_StaticAddress = (PCOSA_DML_DHCPS_X_CISCO_COM_SADDR)pCxtLink->hContext;
    PCOSA_DATAMODEL_DHCPV4          pDhcpv4              = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;
    
    if ( !pCxtLink->bNew )
    {
        returnStatus = CosaDmlDhcpsDelX_COM_CISCO_Saddr( NULL, pDhcpX_COM_CISCO_StaticAddress->InstanceNumber );
        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            return returnStatus;
        }
    }

    /* Firstly we del this entry */
    if ( AnscSListPopEntryByLink(&pDhcpv4->X_CISCO_COM_StaticAddressList, &pCxtLink->Linkage) )
    {
        CosaDhcpv4RegSetDhcpv4Info(pDhcpv4);
        AnscFreeMemory(pCxtLink->hContext);
        AnscFreeMemory(pCxtLink);
    }
    
    return returnStatus;
}



/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_StaticAddress_GetParamBoolValue
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
X_CISCO_COM_StaticAddress_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_StaticAddress_GetParamIntValue
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
X_CISCO_COM_StaticAddress_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_StaticAddress_GetParamUlongValue
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
X_CISCO_COM_StaticAddress_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT               pContextLinkObject = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPS_X_CISCO_COM_SADDR       pEntry = (PCOSA_DML_DHCPS_X_CISCO_COM_SADDR)pContextLinkObject->hContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Yiaddr", TRUE))
    {
        /* collect value */
        *puLong = pEntry->Yiaddr.Value;
        
        return TRUE;
    }

   /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        X_CISCO_COM_StaticAddress_GetParamStringValue
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
X_CISCO_COM_StaticAddress_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_CONTEXT_LINK_OBJECT               pContextLinkObject = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPS_X_CISCO_COM_SADDR pEntry = (PCOSA_DML_DHCPS_X_CISCO_COM_SADDR)pContextLinkObject->hContext;
    
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE))
    {
        /* collect value */
        if ( AnscSizeOfString(pEntry->Alias) <= *pUlSize)
        {
            AnscCopyString(pValue, pEntry->Alias);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pEntry->Alias)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "Chaddr", TRUE))
    {
        /* collect value */
        if ( *pUlSize >= 18 )
        {
            _ansc_sprintf
                (
                    pValue,
                    "%02x:%02x:%02x:%02x:%02x:%02x",
                    pEntry->Chaddr[0],
                    pEntry->Chaddr[1],
                    pEntry->Chaddr[2],
                    pEntry->Chaddr[3],
                    pEntry->Chaddr[4],
                    pEntry->Chaddr[5]
                );
            pValue[17] = '\0'; 
            *pUlSize = AnscSizeOfString(pValue);
           
            return 0;
        }
        else
        {
            *pUlSize = sizeof(pEntry->Chaddr);
            return 1;
        }
    }
    
    if( AnscEqualString(ParamName, "DeviceName", TRUE))
    {
        /* collect value */
        if ( AnscSizeOfString(pEntry->DeviceName) <= *pUlSize)
        {
            AnscCopyString(pValue, pEntry->DeviceName);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pEntry->DeviceName)+1;
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
        X_CISCO_COM_StaticAddress_SetParamBoolValue
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
X_CISCO_COM_StaticAddress_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_StaticAddress_SetParamIntValue
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
X_CISCO_COM_StaticAddress_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_StaticAddress_SetParamUlongValue
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
X_CISCO_COM_StaticAddress_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT               pContextLinkObject = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPS_X_CISCO_COM_SADDR pEntry = (PCOSA_DML_DHCPS_X_CISCO_COM_SADDR)pContextLinkObject->hContext;
    
    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Yiaddr", TRUE))
    {

        /* save update to backup */
        pEntry->Yiaddr.Value  =  uValue;

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_StaticAddress_SetParamStringValue
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
X_CISCO_COM_StaticAddress_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_DATAMODEL_DHCPV4            pDhcpv4            = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;
    PCOSA_CONTEXT_LINK_OBJECT         pContextLinkObject = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPS_X_CISCO_COM_SADDR pEntry = (PCOSA_DML_DHCPS_X_CISCO_COM_SADDR)pContextLinkObject->hContext;
    int                               rc                 = -1;
    UCHAR                             chAddr[7]          = {'\0'};

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE))
    {
        AnscCopyString(pDhcpv4->AliasOfX_CISCO_COM_SAddr, pEntry->Alias);

        AnscCopyString(pEntry->Alias, pString);

        return TRUE;
    }

    if( AnscEqualString(ParamName, "Chaddr", TRUE))
    {
        /* save update to a temp array - This is required as sscanf puts a NULL character at the end which we dont have space for */
        rc = _ansc_sscanf
                (
                    pString,
                    "%x:%x:%x:%x:%x:%x",
                    chAddr,
                    chAddr+1,
                    chAddr+2,
                    chAddr+3,
                    chAddr+4,
                    chAddr+5
                );

        if(rc != 6)
        {
            CcspTraceWarning(("MAC Address not proerly formatted \n"));
            return FALSE;
        }
       
        /* Now copy the temp value to our back-up without NULL */ 
        pEntry->Chaddr[0] = chAddr[0];
        pEntry->Chaddr[1] = chAddr[1];
        pEntry->Chaddr[2] = chAddr[2];
        pEntry->Chaddr[3] = chAddr[3];
        pEntry->Chaddr[4] = chAddr[4];
        pEntry->Chaddr[5] = chAddr[5];

        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_DeviceName", TRUE))
    {
        /* save update to backup */
        AnscCopyString(pEntry->DeviceName,pString);
        return TRUE;

    }
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_StaticAddress_Validate
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
X_CISCO_COM_StaticAddress_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_DATAMODEL_DHCPV4          pDhcpv4           = (PCOSA_DATAMODEL_DHCPV4 )g_pCosaBEManager->hDhcpv4;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPS_X_CISCO_COM_SADDR pDhcpStaAddr    = (PCOSA_DML_DHCPS_X_CISCO_COM_SADDR)pCxtLink->hContext;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink2         = NULL;
    PCOSA_DML_DHCPS_X_CISCO_COM_SADDR pDhcpStaAddr2   = NULL;
    PSINGLE_LINK_ENTRY              pSListEntry       = NULL;
    BOOL                            bFound            = FALSE;
    UCHAR                           strIP[32]         = {'\0'};
    UINT                            ip[4]             = {0};
    int                             rc                = -1;
    int                             i                 = 0;

    if ( pDhcpv4->AliasOfX_CISCO_COM_SAddr[0] )
    {
        /* save update to backup */
        bFound                = FALSE;
        pSListEntry           = AnscSListGetFirstEntry(&pDhcpv4->X_CISCO_COM_StaticAddressList);
        while( pSListEntry != NULL)
        {
            pCxtLink2         = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSListEntry);
            pSListEntry       = AnscSListGetNextEntry(pSListEntry);

            pDhcpStaAddr2  = (PCOSA_DML_DHCPS_X_CISCO_COM_SADDR)pCxtLink2->hContext;

            if( DHCPV4_STATICADDRESS_ENTRY_MATCH2(pDhcpStaAddr->Alias, pDhcpStaAddr2->Alias ) )
            {
                if ( (ANSC_HANDLE)pCxtLink2 == hInsContext )
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
            X_COM_CISCO_StaticAddress_Rollback(hInsContext);
#endif
            return FALSE;
        }
    }

    /* Make sure Static IP Address is properly formatted and isnt a network or multicast address */
    _ansc_memset(strIP,0,32);
    _ansc_strcpy(strIP,_ansc_inet_ntoa(*((struct in_addr *)(&pDhcpStaAddr->Yiaddr))));
    rc = sscanf(strIP,"%d.%d.%d.%d",ip,ip+1,ip+2,ip+3);
    if(rc != 4)
    {
        CcspTraceWarning(("Static IP Address is not properly formatted \n"));
        _ansc_strcpy(pReturnParamName, "Yiaddr");
        return FALSE;
    }

    for(i = 0; i < 4; i++)
    {
        if((ip[i] == 0) || (ip[i] == 255))
        {
            CcspTraceWarning(("Static IP Address can not be a network address or multicast \n"));
            _ansc_strcpy(pReturnParamName, "Yiaddr");
            return FALSE;
        }
    }

    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        X_CISCO_COM_StaticAddress_Commit
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
X_CISCO_COM_StaticAddress_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink             = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPS_X_CISCO_COM_SADDR pDhcpStaticAddress  = (PCOSA_DML_DHCPS_X_CISCO_COM_SADDR)pCxtLink->hContext;
    PCOSA_DATAMODEL_DHCPV4          pDhcpv4              = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;

    if ( pCxtLink->bNew )
    {
        returnStatus = CosaDmlDhcpsAddX_COM_CISCO_Saddr(NULL, pDhcpStaticAddress );

        if ( returnStatus == ANSC_STATUS_SUCCESS )
        {
            pCxtLink->bNew = FALSE;

            CosaDhcpv4RegSetDhcpv4Info(pDhcpv4);
        }
        else
        {
            DHCPV4_X_COM_CISCO_SADDR_SET_DEFAULTVALUE(pDhcpStaticAddress);

            if ( pDhcpv4->AliasOfX_CISCO_COM_SAddr[0] )
                AnscCopyString( pDhcpStaticAddress->Alias, pDhcpv4->AliasOfX_CISCO_COM_SAddr );
        }
    }
    else
    {
        returnStatus = CosaDmlDhcpsSetX_COM_CISCO_Saddr(NULL, pDhcpStaticAddress);

        if ( returnStatus != ANSC_STATUS_SUCCESS)
        {
            CosaDmlDhcpsGetX_COM_CISCO_SaddrbyInsNum(NULL, pDhcpStaticAddress);
        }
    }
    
    AnscZeroMemory( pDhcpv4->AliasOfX_CISCO_COM_SAddr, sizeof(pDhcpv4->AliasOfX_CISCO_COM_SAddr) );
    
    return returnStatus;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        X_CISCO_COM_StaticAddress_Rollback
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
X_CISCO_COM_StaticAddress_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPS_X_CISCO_COM_SADDR pDhcpStaAddr    = (PCOSA_DML_DHCPS_X_CISCO_COM_SADDR)pCxtLink->hContext;
    PCOSA_DATAMODEL_DHCPV4          pDhcpv4              = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;

    if ( pDhcpv4->AliasOfX_CISCO_COM_SAddr[0] )
        AnscCopyString( pDhcpStaAddr->Alias, pDhcpv4->AliasOfX_CISCO_COM_SAddr );

    if ( !pCxtLink->bNew )
    {
        CosaDmlDhcpsGetX_COM_CISCO_SaddrbyInsNum(NULL, pDhcpStaAddr);
    }
    else
    {
        DHCPV4_X_COM_CISCO_SADDR_SET_DEFAULTVALUE(pDhcpStaAddr);
    }

    AnscZeroMemory( pDhcpv4->AliasOfX_CISCO_COM_SAddr, sizeof(pDhcpv4->AliasOfX_CISCO_COM_SAddr) );
    
    return returnStatus;
}

/***********************************************************************

 APIs for Object:

    DHCPv4.Server.Pool.{i}.

    *  Pool_GetEntryCount
    *  Pool_GetEntry
    *  Pool_AddEntry
    *  Pool_DelEntry
    *  Pool_GetParamBoolValue
    *  Pool_GetParamIntValue
    *  Pool_GetParamUlongValue
    *  Pool_GetParamStringValue
    *  Pool_SetParamBoolValue
    *  Pool_SetParamIntValue
    *  Pool_SetParamUlongValue
    *  Pool_SetParamStringValue
    *  Pool_Validate
    *  Pool_Commit
    *  Pool_Rollback

***********************************************************************/
#if 1//LNT_EMU
int PSMSetDHCPV4RecordValues(PCOSA_DML_DHCPS_POOL_FULL pNewCfg,ULONG instancenum)
{
        int retPsmSet = CCSP_SUCCESS;
        char param_name[256] = {0};
        char param_value[256] = {0};
        char start_buf[100];
        struct in_addr start_addr;
        start_addr.s_addr =  pNewCfg->Cfg.MinAddress.Value;
        inet_ntop(AF_INET, &start_addr, start_buf, sizeof start_buf);
        strcpy(param_value,start_buf);
        _PSM_WRITE_PARAM(PSM_DHCPV4_SERVER_POOL_MINADDRESS);
_ansc_sprintf(param_value, "%d.%d.%d.%d",
            pNewCfg->Cfg.MaxAddress.Dot[0], pNewCfg->Cfg.MaxAddress.Dot[1], pNewCfg->Cfg.MaxAddress.Dot[2], pNewCfg->Cfg.MaxAddress.Dot[3]);
        _PSM_WRITE_PARAM(PSM_DHCPV4_SERVER_POOL_MAXADDRESS);
        return 0;

}
int PSMSetLeaseTimeDHCPV4RecordValues(PCOSA_DML_DHCPS_POOL_FULL pNewCfg,ULONG instancenum)
{
        int retPsmSet = CCSP_SUCCESS;
        char param_name[256] = {0};
        char param_value[256] = {0};
        _ansc_sprintf(param_value, "%d", pNewCfg->Cfg.LeaseTime );
        _PSM_WRITE_PARAM(PSM_DHCPV4_SERVER_POOL_LEASETIME);
        return 0;
}
#endif


static is_invalid_unicast_ip_addr(unsigned int gw, unsigned int mask, unsigned int ipaddr)
{
    unsigned int subnet, bcast;

    /*Check whether ipaddr is zero, or a multicast address , or a loopback address*/
    if((ipaddr==0)||(ipaddr>=0xE0000000)||((ipaddr & 0xFF000000)==0x7F000000))
        return(1);
    subnet = gw & mask;
    bcast = subnet | (~mask);
    /*Check if ip addr is subnet or subnet broadcast address, or it is the same as gw*/
    if((ipaddr<=subnet)||(ipaddr>=bcast)||(ipaddr==gw))
        return(1);
    return(0);
}

static int is_pool_invalid(void *hInsContext)
{
    PCOSA_CONTEXT_POOL_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOL_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPS_POOL_FULL       pPool             = (PCOSA_DML_DHCPS_POOL_FULL)pCxtLink->hContext;
    PCOSA_DATAMODEL_DHCPV4          pDhcpv4           = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;
    ULONG                           minaddr, maxaddr, netmask, gateway;
    COSA_DML_DHCPS_POOL_CFG poolCfg;

    minaddr = ntohl(pPool->Cfg.MinAddress.Value);
    maxaddr = ntohl(pPool->Cfg.MaxAddress.Value);
    if(minaddr > maxaddr)
        return(1);
    poolCfg.InstanceNumber = pPool->Cfg.InstanceNumber;
    /*To get real GW & Netmask info*/
    CosaDmlDhcpsGetPoolCfg(NULL, &poolCfg);
    gateway = ntohl(poolCfg.IPRouters[0].Value);
    netmask = ntohl(poolCfg.SubnetMask.Value);
    if(is_invalid_unicast_ip_addr(gateway,netmask,minaddr)||is_invalid_unicast_ip_addr(gateway,netmask,maxaddr))
        return(1);
    return(0);
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Pool_GetEntryCount
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
Pool_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_DHCPV4          pDhcpv4           = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;

    return AnscSListQueryDepth( &pDhcpv4->PoolList );
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Pool_GetEntry
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
Pool_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_DHCPV4          pDhcpv4           = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;
    PCOSA_CONTEXT_POOL_LINK_OBJECT  pCxtLink          = NULL;
    PSINGLE_LINK_ENTRY              pSListEntry       = NULL;

    pSListEntry = AnscSListGetEntryByIndex(&pDhcpv4->PoolList, nIndex);

    if ( pSListEntry )
    {
        pCxtLink          = ACCESS_COSA_CONTEXT_POOL_LINK_OBJECT(pSListEntry);
        *pInsNumber       = pCxtLink->InstanceNumber;
    }

    return pSListEntry;
}



/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Pool_AddEntry
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
Pool_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PSINGLE_LINK_ENTRY              pSListEntry       = NULL;
    PCOSA_DATAMODEL_DHCPV4          pDhcpv4           = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;
    PCOSA_CONTEXT_POOL_LINK_OBJECT  pCxtLink          = NULL;
    PCOSA_DML_DHCPS_POOL_FULL       pPool             = NULL;
    CHAR                            tmpBuff[64]       = {0};
    ULONG                           i                 = 0;

    // return NULL; /* No dynamic addition/deletion of ServerPools at this point */
    
    pPool  = (PCOSA_DML_DHCPS_POOL_FULL)AnscAllocateMemory( sizeof(COSA_DML_DHCPS_POOL_FULL) );
    if ( !pPool )
    {
        goto EXIT2;
    }

    /* Set default value */
    DHCPV4_POOL_SET_DEFAULTVALUE(pPool);

    pCxtLink = (PCOSA_CONTEXT_POOL_LINK_OBJECT)AnscAllocateMemory( sizeof(COSA_CONTEXT_POOL_LINK_OBJECT) );
    if ( !pCxtLink )
    {
        goto EXIT1;
    }
    
    DHCPV4_POOL_INITIATION_CONTEXT(pCxtLink)

    pCxtLink->hContext       = (ANSC_HANDLE)pPool;
    pCxtLink->bNew           = TRUE;

    if ( !++pDhcpv4->maxInstanceOfPool )
    {
        pDhcpv4->maxInstanceOfPool = 1;
    }

    pPool->Cfg.InstanceNumber = pDhcpv4->maxInstanceOfPool;
    pCxtLink->InstanceNumber  = pPool->Cfg.InstanceNumber;
    *pInsNumber               = pPool->Cfg.InstanceNumber;

    _ansc_sprintf( pPool->Cfg.Alias, "Pool%d", pPool->Cfg.InstanceNumber);

    /* Put into our list */
    CosaSListPushEntryByInsNum(&pDhcpv4->PoolList, (PCOSA_CONTEXT_LINK_OBJECT)pCxtLink);

    /* we recreate the configuration */
    // not saving pool entry, but saving maxInstanceOfPool
    pCxtLink->bNew           = FALSE;
    CosaDhcpv4RegSetDhcpv4Info(pDhcpv4);

    // Write to SBAPI directly
    pPool->Cfg.bEnabled      = FALSE;
    returnStatus = CosaDmlDhcpsAddPool(NULL, pPool );

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
        Pool_DelEntry
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
Pool_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DHCPV4          pDhcpv4           = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;
    PCOSA_CONTEXT_POOL_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOL_LINK_OBJECT)hInstance;
    PCOSA_DML_DHCPS_POOL_FULL       pPool             = (PCOSA_DML_DHCPS_POOL_FULL)pCxtLink->hContext;

    //return ANSC_STATUS_FAILURE; /* No dynamic addition/deletion of ServerPools at this moment */

    /* Normally, two sublinks are empty because our framework will firstly 
              call delEntry for them before coming here. We needn't care them.
           */
    if(pPool->Cfg.InstanceNumber == 1)
    {
        AnscTraceFlow(("%s: pool instance %d is not allowed to delete\n", __FUNCTION__, pPool->Cfg.InstanceNumber));
        return ANSC_STATUS_FAILURE;
    }

    if ( !pCxtLink->bNew )
    {
        if(!pPool->Cfg.bAllowDelete)
        {
            AnscTraceFlow(("%s: pool instance %d is not allowed to delete\n", __FUNCTION__, pPool->Cfg.InstanceNumber));
            return ANSC_STATUS_FAILURE;
        }

        returnStatus = CosaDmlDhcpsDelPool(NULL, pPool->Cfg.InstanceNumber);
        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            return returnStatus;
        }
    }

    if (AnscSListPopEntryByLink(&pDhcpv4->PoolList, &pCxtLink->Linkage) )
    {
        // it should be safe to comment this out, 
        // because we never let bNew to be true, the entry is always saved on SBAPI only.
        //CosaDhcpv4RegSetDhcpv4Info(pDhcpv4);

        AnscFreeMemory(pCxtLink->hContext);
        AnscFreeMemory(pCxtLink);
    }
    
    return returnStatus;
}



/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Pool_GetParamBoolValue
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
Pool_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_POOL_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOL_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPS_POOL_FULL       pDhcpc            = (PCOSA_DML_DHCPS_POOL_FULL)pCxtLink->hContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* collect value */
        *pBool   =  pDhcpc->Cfg.bEnabled;
        
        return TRUE;
    }

    if( AnscEqualString(ParamName, "VendorClassIDExclude", TRUE))
    {
        /* collect value */
        /* *pBool   =  pDhcpc->Cfg.VendorClassIDExclude; */

        *pBool   = FALSE; /* Dummy Value */

        return TRUE;
    }

    if( AnscEqualString(ParamName, "ClientIDExclude", TRUE))
    {
        /* collect value */
        /* *pBool   =  pDhcpc->Cfg.ClientIDExclude; */
        *pBool   = FALSE; /* Dummy Value */ 

        return TRUE;
    }

    if( AnscEqualString(ParamName, "UserClassIDExclude", TRUE))
    {
        /* collect value */
        /* *pBool   =  pDhcpc->Cfg.UserClassIDExclude; */
        *pBool   = FALSE; /* Dummy Value */ 

        return TRUE;
    }

    if( AnscEqualString(ParamName, "ChaddrExclude", TRUE))
    {
        /* collect value */
        /* *pBool   =  pDhcpc->Cfg.ChaddrExclude; */
        *pBool   = FALSE; /* Dummy Value */ 

        return TRUE;
    }


    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "DNSServersEnabled", TRUE))
    {
        /* collect value */
        *pBool   =  pDhcpc->Cfg.DNSServersEnabled;

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Pool_GetParamIntValue
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
Pool_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_POOL_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOL_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPS_POOL_FULL       pPool             = (PCOSA_DML_DHCPS_POOL_FULL)pCxtLink->hContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "LeaseTime", TRUE))
    {
        /* collect value */
        *pInt = pPool->Cfg.LeaseTime;
        
        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_TimeOffset", TRUE))
    {
        /* collect value */
        *pInt = pPool->Cfg.X_CISCO_COM_TimeOffset;
        
        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Pool_GetParamUlongValue
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
Pool_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_POOL_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOL_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPS_POOL_FULL       pPool             = (PCOSA_DML_DHCPS_POOL_FULL)pCxtLink->hContext;
    COSA_DML_DHCPS_POOL_FULL        poolTemp;

    AnscZeroMemory(&poolTemp, sizeof(COSA_DML_DHCPS_POOL_FULL));
    AnscCopyMemory(&poolTemp, pPool, sizeof(COSA_DML_DHCPS_POOL_FULL)); 

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Status", TRUE))
    {
        /* collect value */
        CosaDmlDhcpsGetPoolInfo( hInsContext, pPool->Cfg.InstanceNumber, &poolTemp.Info );

        *puLong  = poolTemp.Info.Status;
        
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Order", TRUE))
    {
        /* collect value */
        *puLong  = pPool->Cfg.Order;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_Connected_Device_Number", TRUE))
    {
        /* collect value */
        CosaDmlDhcpsGetPoolInfo( hInsContext, pPool->Cfg.InstanceNumber, &poolTemp.Info );

        *puLong  = poolTemp.Info.X_CISCO_COM_Connected_Device_Number;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "VendorClassIDMode", TRUE))
    {
        /* collect value */
        /* *puLong  = pPool->Cfg.VendorClassIDMode; */
        *puLong  = 0; /* Not supported now */
        
        return TRUE;
    }
     
    if( AnscEqualString(ParamName, "MinAddress", TRUE))
    {
        /* collect value */
	*puLong  = pPool->Cfg.MinAddress.Value;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "MaxAddress", TRUE))
    {
        /* collect value */
	*puLong  = pPool->Cfg.MaxAddress.Value;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "SubnetMask", TRUE))
    {
        /* collect value */
        CosaDmlDhcpsGetPoolCfg(NULL,&poolTemp.Cfg);
        *puLong  = poolTemp.Cfg.SubnetMask.Value;

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Pool_GetParamStringValue
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
Pool_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_POOL_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOL_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPS_POOL_FULL       pPool             = (PCOSA_DML_DHCPS_POOL_FULL)pCxtLink->hContext;
    CHAR                           *pTmpString        = NULL;
    ULONG                           i                 = 0;
    ULONG                           j                 = 0;
    ULONG                           n                 = 0;
    PUCHAR                          pString           = NULL;
    COSA_DML_DHCPS_POOL_CFG tmpCfg;
    
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE))
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

    if( AnscEqualString(ParamName, "Interface", TRUE))
    {
        /* collect value */
        // TO DO: when changing pool 1 from syscfg to PSM, 
        // please following the other logic
        if(pPool->Cfg.InstanceNumber == 1)
        {
            // pPool->Cfg.Interface is name for pool 1
            pString = CosaUtilGetFullPathNameByKeyword
                    (
                        "Device.IP.Interface.",
                        "Name",
                        pPool->Cfg.Interface
                    );
            AnscTraceFlow(("%s: interface %s\n", __FUNCTION__, pPool->Cfg.Interface));  
            AnscTraceFlow(("%s: pString %s\n", __FUNCTION__, pString));               
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
        }
        else
        {
            //pPool->Cfg.Interface is instance number for other pool
            //_ansc_sprintf(pValue,"Device.IP.Interface.%s", pPool->Cfg.Interface);
            // pPool->Cfg.Interface is full path name
            AnscCopyString(pValue, pPool->Cfg.Interface);
            return 0;
        }

    }

    if( AnscEqualString(ParamName, "VendorClassID", TRUE))
    {
        /* collect value */
#if 0
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
#endif
        AnscCopyString(pValue, ""); /* Not Supported */
    }

    if( AnscEqualString(ParamName, "ClientID", TRUE))
    {
        /* collect value */
#if 0
        if ( AnscSizeOfString(pPool->Cfg.ClientID) < *pUlSize)
        {
            AnscCopyString(pValue, pPool->Cfg.ClientID);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pPool->Cfg.ClientID)+1;
            return 1;
        }
#endif
        AnscCopyString(pValue, ""); /* Not Supported */
    }

    if( AnscEqualString(ParamName, "UserClassID", TRUE))
    {
        /* collect value */
#if 0
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
#endif
        AnscCopyString(pValue, ""); /* Not Supported */
    }

    if( AnscEqualString(ParamName, "Chaddr", TRUE))
    {
        /* collect value */
#if 0
        if ( AnscSizeOfString(pPool->Cfg.Chaddr) < *pUlSize)
        {
            _ansc_sprintf
                (
                    pValue, 
                    "%2x:%2x:%2x:%2x:%2x:%2x", 
                    pPool->Cfg.Chaddr[0],
                    pPool->Cfg.Chaddr[1],
                    pPool->Cfg.Chaddr[2],
                    pPool->Cfg.Chaddr[3],
                    pPool->Cfg.Chaddr[4],
                    pPool->Cfg.Chaddr[5]
                );
            
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pPool->Cfg.Chaddr)+1;
            return 1;
        }
#endif
        AnscCopyString(pValue, ""); /* Not Supported */
    }

    if( AnscEqualString(ParamName, "ChaddrMask", TRUE))
    {
        /* collect value */
#if 0
        if ( AnscSizeOfString(pPool->Cfg.ChaddrMask) < *pUlSize)
        {
            _ansc_sprintf
                (
                    pValue, 
                    "%2x:%2x:%2x:%2x:%2x:%2x", 
                    pPool->Cfg.ChaddrMask[0],
                    pPool->Cfg.ChaddrMask[1],
                    pPool->Cfg.ChaddrMask[2],
                    pPool->Cfg.ChaddrMask[3],
                    pPool->Cfg.ChaddrMask[4],
                    pPool->Cfg.ChaddrMask[5]
                );
            
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pPool->Cfg.ChaddrMask)+1;
            return 1;
        }
#endif
        AnscCopyString(pValue, ""); /* Not Supported */
    }

    if( AnscEqualString(ParamName, "ReservedAddresses", TRUE))
    {
        /* collect value */        
#if 0
        if ( CosaDmlGetIpaddrString(pValue, pUlSize, (PULONG)&pPool->Cfg.ReservedAddresses[0], COSA_DML_DHCP_MAX_RESERVED_ADDRESSES ) )
        {
            return 0;
        }
        else
        {
            return 1;
        }
#endif
        AnscCopyString(pValue, ""); /* Not Supported */
    }

    if( AnscEqualString(ParamName, "DNSServers", TRUE))
    {
        /* collect value */
        if ( CosaDmlGetIpaddrString(pValue, pUlSize, (PULONG)&pPool->Cfg.DNSServers[0].Value, COSA_DML_DHCP_MAX_ENTRIES ) )
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "DomainName", TRUE))
    {
        if (pPool->Cfg.InstanceNumber == 1){
            tmpCfg.InstanceNumber = pPool->Cfg.InstanceNumber;
            CosaDmlDhcpsGetPoolCfg(NULL,&tmpCfg);
            snprintf(pValue,sizeof(tmpCfg.DomainName),"%s", tmpCfg.DomainName);
        }else
	        snprintf(pValue,sizeof(pPool->Cfg.DomainName),"%s", pPool->Cfg.DomainName);
        return 0;
    }

    if( AnscEqualString(ParamName, "IPRouters", TRUE))
    {
        PULONG pTmpAddr;
        if(pPool->Cfg.InstanceNumber == 1)
        {
            memset(&tmpCfg, 0, sizeof(tmpCfg));
            tmpCfg.InstanceNumber = pPool->Cfg.InstanceNumber;
            CosaDmlDhcpsGetPoolCfg(NULL,&tmpCfg);
            pTmpAddr = &tmpCfg.IPRouters[0].Value;
        }
        else
            pTmpAddr = &pPool->Cfg.IPRouters[0].Value;

        /* collect value */
        if ( CosaDmlGetIpaddrString(pValue, pUlSize, pTmpAddr, COSA_DML_DHCP_MAX_ENTRIES ) )
        {
            return 0;
        }
        else
        {
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
        Pool_SetParamBoolValue
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
Pool_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_POOL_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOL_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPS_POOL_FULL       pPool            = (PCOSA_DML_DHCPS_POOL_FULL)pCxtLink->hContext;
    BOOL bridgeInd = FALSE;

    AnscTraceFlow(("%s: ParamName = %s\n", __FUNCTION__, ParamName));
    is_usg_in_bridge_mode(&bridgeInd);
	if(bridgeInd)
		return(FALSE);

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* save update to backup */
        pPool->Cfg.bEnabled   = bValue;
        
        return TRUE;
    }

    if( AnscEqualString(ParamName, "VendorClassIDExclude", TRUE))
    {
        /* save update to backup */
        return FALSE; /* Not supported */

        pPool->Cfg.VendorClassIDExclude   = bValue;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "ClientIDExclude", TRUE))
    {
        /* save update to backup */
        return FALSE; /* Not supported */

        pPool->Cfg.ClientIDExclude   = bValue;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "UserClassIDExclude", TRUE))
    {
        /* save update to backup */
        return FALSE; /* Not supported */

        pPool->Cfg.UserClassIDExclude   = bValue;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "ChaddrExclude", TRUE))
    {
        /* save update to backup */
        return FALSE; /* Not supported */

        pPool->Cfg.ChaddrExclude   = bValue;

        return TRUE;
    }

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "DNSServersEnabled", TRUE))
    {
        /* save update to backup */
        pPool->Cfg.DNSServersEnabled   = bValue;

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Pool_SetParamIntValue
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
Pool_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
	ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
	PCOSA_CONTEXT_POOL_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOL_LINK_OBJECT)hInsContext;
	PCOSA_DML_DHCPS_POOL_FULL       pPool             = (PCOSA_DML_DHCPS_POOL_FULL)pCxtLink->hContext;
	BOOL bridgeInd = FALSE;	
	char str[1024];
	PCOSA_DML_DHCPS_POOL_CFG pSPoolCfg = NULL;
        pSPoolCfg = (PCOSA_DML_DHCPS_POOL_CFG)AnscAllocateMemory( sizeof(COSA_DML_DHCPS_POOL_CFG));//RDKB-EMUALTOR
	is_usg_in_bridge_mode(&bridgeInd);
	if(bridgeInd)
		return(FALSE);

	/* check the parameter name and set the corresponding value */
	if( AnscEqualString(ParamName, "LeaseTime", TRUE))
	{
#ifndef CONFIG_CISCO_DHCPS_REMOVE_LEASE_LIMIT
		/*DHCP Server doesn't accept lease time that is shorter than 120 seconds*/
		if((iValue>0)&&(iValue<120))
			return(FALSE);
#endif
		/*-1 means infinite lease, we don't allow other invalid time*/
		if((iValue!=-1)&&(iValue<=0))
			return(FALSE);

		/* save update to backup */
		pPool->Cfg.LeaseTime  = iValue;//RDKB-EMULATOR
		pSPoolCfg->LeaseTime = pPool->Cfg.LeaseTime;
		if(pPool->Cfg.InstanceNumber == 1)
	                PSMSetLeaseTimeDHCPV4RecordValues(pSPoolCfg,pPool->Cfg.InstanceNumber);
		sprintf(str,"%d",pPool->Cfg.LeaseTime );
	/*	ConfigValues config_values;
		config_values.lease_time = str;
		if(CcspHalSetDHCPConfigValues(DHCP_LEASE_TIME, &config_values)==-1)
			printf("SetDHCPConfigValues failed\n");*/
		if(pPool->Cfg.InstanceNumber == 1)
			CcspHalSetDHCPConfigValues(DHCP_LEASE_TIME,str);
		return TRUE;
	}

	if( AnscEqualString(ParamName, "X_CISCO_COM_TimeOffset", TRUE))
	{
		/* save update to backup */
		pPool->Cfg.X_CISCO_COM_TimeOffset  = iValue;

		return TRUE;
	}


	/* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
	return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Pool_SetParamUlongValue
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
Pool_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_POOL_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOL_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPS_POOL_FULL       pPool             = (PCOSA_DML_DHCPS_POOL_FULL)pCxtLink->hContext;
    COSA_DML_DHCPS_POOL_CFG poolCfg;    
    BOOL bridgeInd = FALSE;	

        PCOSA_DML_DHCPS_POOL_FULL pSPoolCfg = NULL;//RDKB-EMULATOR
        pSPoolCfg = (PCOSA_DML_DHCPS_POOL_FULL)AnscAllocateMemory( sizeof(COSA_DML_DHCPS_POOL_FULL));
	is_usg_in_bridge_mode(&bridgeInd);
	if(bridgeInd)
		return(FALSE);

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Order", TRUE))
    {
        /* save update to backup */
        pPool->Cfg.Order  = uValue;
        
        return TRUE;
    }

    if( AnscEqualString(ParamName, "VendorClassIDMode", TRUE))
    {
        /* save update to backup */
        return FALSE; /* Not supported */

        pPool->Cfg.VendorClassIDMode  = uValue;
        
        return TRUE;
    }

    if( AnscEqualString(ParamName, "MinAddress", TRUE))
    {
        /* save update to backup */
        poolCfg.InstanceNumber = pPool->Cfg.InstanceNumber;
        /* CosaDmlDhcpsGetPoolCfg(NULL, &poolCfg);*/
        char pFullName[256] = {0};
        char strval[64] = {0};
#if 1
        char str1[INET_ADDRSTRLEN];
#endif
        ULONG size = sizeof(strval);
        ULONG gw = 0, mask = 0;
        /* Suppose LanManagementEntry to Pool 1:1 mapping for now */
        _ansc_sprintf(pFullName, "Device.X_CISCO_COM_DeviceControl.LanManagementEntry.%d.LanIPAddress", pPool->Cfg.InstanceNumber);
        g_GetParamValueString(g_pDslhDmlAgent, pFullName, strval, &size);
        gw = _ansc_inet_addr(strval);

        size = sizeof(strval);
        AnscZeroMemory(strval, size);
        _ansc_sprintf(pFullName, "Device.X_CISCO_COM_DeviceControl.LanManagementEntry.%d.LanSubnetMask", pPool->Cfg.InstanceNumber);
        g_GetParamValueString(g_pDslhDmlAgent, pFullName, strval, &size);
        mask = _ansc_inet_addr(strval);

        if( pPool->Cfg.InstanceNumber == 1 && 
            is_invalid_unicast_ip_addr(ntohl(gw),ntohl(mask), ntohl(uValue)))
            return(FALSE);
        pPool->Cfg.MinAddress.Value  = uValue;
	if(pPool->Cfg.InstanceNumber == 1)
		PSMSetDHCPV4RecordValues(pPool,pPool->Cfg.InstanceNumber);//RDKB-EMULATOR
#if 1
       inet_ntop(AF_INET, &(pPool->Cfg.MinAddress.Value) , str1, INET_ADDRSTRLEN);
      /*  ConfigValues config_values;
                config_values.start = str1;
        if(CcspHalSetDHCPConfigValues(DHCP_STARTING_RANGE, &config_values)==-1)//LNT_EMU
                printf("SetDHCPConfigValues failed\n");*/
	if(pPool->Cfg.InstanceNumber == 1)
		CcspHalSetDHCPConfigValues(DHCP_STARTING_RANGE,str1);

#endif
        CcspTraceWarning(("RDKB_LAN_CONFIG_CHANGED: MinAddress of DHCP Range Changed ...\n"));
        
        return TRUE;
    }

    if( AnscEqualString(ParamName, "MaxAddress", TRUE))
    {
        /* save update to backup */
        poolCfg.InstanceNumber = pPool->Cfg.InstanceNumber;
        /* CosaDmlDhcpsGetPoolCfg(NULL, &poolCfg); */
        char pFullName[256] = {0};
        char strval[64] = {0};
#if 1
        char str1[INET_ADDRSTRLEN];
#endif
        ULONG size = sizeof(strval);
        ULONG gw = 0, mask = 0;
        /* Suppose LanManagementEntry to Pool 1:1 mapping for now */
        _ansc_sprintf(pFullName, "Device.X_CISCO_COM_DeviceControl.LanManagementEntry.%d.LanIPAddress", pPool->Cfg.InstanceNumber);
        g_GetParamValueString(g_pDslhDmlAgent, pFullName, strval, &size);
        gw = _ansc_inet_addr(strval);

        size = sizeof(strval);
        AnscZeroMemory(strval, size);
        _ansc_sprintf(pFullName, "Device.X_CISCO_COM_DeviceControl.LanManagementEntry.%d.LanSubnetMask", pPool->Cfg.InstanceNumber);
        g_GetParamValueString(g_pDslhDmlAgent, pFullName, strval, &size);
        mask = _ansc_inet_addr(strval);

        if( pPool->Cfg.InstanceNumber == 1 && 
            is_invalid_unicast_ip_addr(ntohl(gw),ntohl(mask), ntohl(uValue)) || 
            uValue < pPool->Cfg.MinAddress.Value)
            return(FALSE);
        pPool->Cfg.MaxAddress.Value  = uValue;
	if(pPool->Cfg.InstanceNumber == 1)
		PSMSetDHCPV4RecordValues(pPool,pPool->Cfg.InstanceNumber);//RDKB-EMULATOR
#if 1
        inet_ntop(AF_INET, &(pPool->Cfg.MaxAddress.Value) , str1, INET_ADDRSTRLEN);
       /* ConfigValues config_values;
                config_values.end = str1;
        if(CcspHalSetDHCPConfigValues(DHCP_ENDING_RANGE, &config_values)==-1)
                printf("SetDHCPConfigValues failed\n");*/
	if(pPool->Cfg.InstanceNumber == 1)
		CcspHalSetDHCPConfigValues(DHCP_ENDING_RANGE,str1);//RDKB_EMULATOR
#endif
        CcspTraceWarning(("RDKB_LAN_CONFIG_CHANGED: MaxAddress of DHCP Range Changed ...\n"));
        
        return TRUE;
    }

    if( AnscEqualString(ParamName, "SubnetMask", TRUE))
    {
        /* save update to backup */
        pPool->Cfg.SubnetMask.Value  = uValue;
        
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Pool_SetParamStringValue
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
Pool_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DHCPV4          pDhcpv4           = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;
    PCOSA_CONTEXT_POOL_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOL_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPS_POOL_FULL       pPool             = (PCOSA_DML_DHCPS_POOL_FULL)pCxtLink->hContext;
    PSINGLE_LINK_ENTRY              pSListEntry       = NULL;
    PCOSA_DML_DHCPS_POOL_FULL       pPool2            = NULL;
    BOOL                            bFound            = FALSE;
    BOOL bridgeInd = FALSE;
		
    AnscTraceFlow(("%s: ParamName %s, \npString %s\n", __FUNCTION__, ParamName, pString));    
	is_usg_in_bridge_mode(&bridgeInd);
	if(bridgeInd)
		return(FALSE);

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE))
    {        
        /* Backup old alias firstly */
        AnscCopyString(pDhcpv4->AliasOfPool, pPool->Cfg.Alias);

        AnscCopyString(pPool->Cfg.Alias, pString);
        
        return TRUE;
    }

    if( AnscEqualString(ParamName, "Interface", TRUE))
    {
        /* save update to backup */
        int     length  = strlen(pString);
        if( length > 0 &&
            *(pString+length-1) == '.')
        {
            *(pString+length-1) = '\0';
        }

        if(pPool->Cfg.InstanceNumber == 1)
        {
            // Need to convert to Device.IP.Interface.x.Name
            UCHAR   ucEntryParamName[256]       = {0};
            ULONG   ulEntryNameLen              = 256;
            UCHAR   ucEntryNameValue[256]       = {0};

            _ansc_sprintf(ucEntryParamName, "%s%s", pString, ".Name");
               
            ulEntryNameLen = sizeof(ucEntryNameValue);
            if ( 0 == CosaGetParamValueString(ucEntryParamName, ucEntryNameValue, &ulEntryNameLen))
            {

                AnscTraceFlow(("%s:paramName %s, paramValue %s\n", __FUNCTION__, ucEntryParamName, ucEntryNameValue));
                if(strlen(ucEntryNameValue) == 0)
                {
                    // if the Name string is empty, return failure
                    AnscTraceFlow(("%s: %s is empty, can't set \n", __FUNCTION__, ucEntryParamName));
                    return FALSE;
                }
                AnscCopyString(pPool->Cfg.Interface, ucEntryNameValue);
            }
            else
            {
                // this should happen, but just in case it happens, we return failure
                //AnscCopyString(pPool->Cfg.Interface, pString);
                return FALSE;
            }
        }
        else
        {
            // For other pools, keep full path in Interface.
            if(length < 256)
            {
                // extra error checking
                if(strstr(pPool->Cfg.Interface, "Device.IP.Interface."))
                {
                    AnscCopyString(pPool->Cfg.Interface, pString);
                }
                else
                {
                    AnscTraceFlow(("%s: interface path wrong %s\n", __FUNCTION__, pString));
                    return FALSE;
                }
            }
            else
            {
                AnscTraceFlow(("%s: interface too long %s\n", __FUNCTION__, pString));
                return FALSE;
            }
        }

        return TRUE;
    }

    if( AnscEqualString(ParamName, "VendorClassID", TRUE))
    {
            /* save update to backup */
        return FALSE; /* Not supported */

        AnscCopyString(pPool->Cfg.VendorClassID, pString);

        return TRUE;
    }

    if( AnscEqualString(ParamName, "ClientID", TRUE))
    {
        /* save update to backup */
        return FALSE; /* Not supported */

        AnscCopyString(pPool->Cfg.ClientID, pString);

        return TRUE;
    }

    if( AnscEqualString(ParamName, "UserClassID", TRUE))
    {
        /* save update to backup */
        return FALSE; /* Not supported */

        AnscCopyString(pPool->Cfg.UserClassID, pString);

        return TRUE;
    }

    if( AnscEqualString(ParamName, "Chaddr", TRUE))
    {
        /* save update to backup */
        return FALSE; /* Not supported */

        AnscCopyString(pPool->Cfg.Chaddr, pString);

        return TRUE;
    }

    if( AnscEqualString(ParamName, "ChaddrMask", TRUE))
    {
        /* save update to backup */
        return FALSE; /* Not supported */

        AnscCopyString(pPool->Cfg.ChaddrMask, pString);

        return TRUE;
    }

    if( AnscEqualString(ParamName, "ReservedAddresses", TRUE))
    {
        /* save update to backup */
        return FALSE; /* Not supported */

        return CosaDmlSetIpaddr((PULONG)&pPool->Cfg.ReservedAddresses[0].Value, pString, COSA_DML_DHCP_MAX_RESERVED_ADDRESSES);
    }

    if( AnscEqualString(ParamName, "DNSServers", TRUE))
    {
        /* save update to backup */
        return CosaDmlSetIpaddr((PULONG)&pPool->Cfg.DNSServers[0].Value, pString, COSA_DML_DHCP_MAX_ENTRIES);
    }

    if( AnscEqualString(ParamName, "DomainName", TRUE))
    {
        /* save update to backup */
        AnscCopyString(pPool->Cfg.DomainName, pString);

        return TRUE;
    }

    if( AnscEqualString(ParamName, "IPRouters", TRUE))
    {
        /* save update to backup */
        return CosaDmlSetIpaddr((PULONG)&pPool->Cfg.IPRouters[0].Value, pString, COSA_DML_DHCP_MAX_ENTRIES);
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Pool_Validate
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
Pool_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DHCPV4          pDhcpv4           = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;
    PCOSA_CONTEXT_POOL_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOL_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPS_POOL_FULL       pPool             = (PCOSA_DML_DHCPS_POOL_FULL)pCxtLink->hContext;
    PSINGLE_LINK_ENTRY              pSListEntry       = NULL;
    PCOSA_DML_DHCPS_POOL_FULL       pPool2            = NULL;
    BOOL                            bFound            = FALSE;
    UCHAR                           strIP[32]         = {'\0'};
    UINT                            ip[4]             = {0};
    int                             rc                = -1;
    int                             i                 = 0;
    ULONG                           minaddr, maxaddr, netmask, gateway;

    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Pool_Commit
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
Pool_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_POOL_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOL_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPS_POOL_FULL       pPool             = (PCOSA_DML_DHCPS_POOL_FULL)pCxtLink->hContext;
    PCOSA_DATAMODEL_DHCPV4          pDhcpv4           = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;

    // only validate it for first pool, should changed to better validation for second pool
    if( pPool->Cfg.InstanceNumber == 1 && 
        is_pool_invalid(hInsContext))
    {
        AnscTraceFlow(("%s: not valid pool\n", __FUNCTION__));
        return(ANSC_STATUS_FAILURE);
    }
    else
    {
        AnscTraceFlow(("%s: valid pool, pPool->Cfg.InstanceNumber = %d\n", __FUNCTION__, pPool->Cfg.InstanceNumber));
    }

    if ( pCxtLink->bNew )
    {
        AnscTraceFlow(("%s: new pool, add to SBAPI\n", __FUNCTION__));
        returnStatus = CosaDmlDhcpsAddPool(NULL, pPool );

        if ( returnStatus == ANSC_STATUS_SUCCESS )
        {
            pCxtLink->bNew = FALSE;

            CosaDhcpv4RegSetDhcpv4Info(pDhcpv4);
        }
        else
        {
            DHCPV4_POOL_SET_DEFAULTVALUE(pPool);
            
            if ( pDhcpv4->AliasOfClient[0] )
                AnscCopyString( pPool->Cfg.Alias, pDhcpv4->AliasOfPool );
        }
    }
    else
    {
        returnStatus = CosaDmlDhcpsSetPoolCfg(NULL, &pPool->Cfg);

        if ( returnStatus != ANSC_STATUS_SUCCESS)
        {
            CosaDmlDhcpsGetPoolCfg(NULL, &pPool->Cfg);
        }
    }
    
    AnscZeroMemory( pDhcpv4->AliasOfPool, sizeof(pDhcpv4->AliasOfPool) );

    return returnStatus;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Pool_Rollback
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
Pool_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_DHCPV4          pDhcpv4           = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;
    PCOSA_CONTEXT_POOL_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOL_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPS_POOL_FULL       pPool             = (PCOSA_DML_DHCPS_POOL_FULL)pCxtLink->hContext;

    if ( pDhcpv4->AliasOfPool[0] )
        AnscCopyString( pPool->Cfg.Alias, pDhcpv4->AliasOfPool );

    if ( !pCxtLink->bNew )
    {
#if 0 //LNT_EMU
        CosaDmlDhcpsGetPoolCfg( NULL, &pPool->Cfg );
#endif
    }
    else
    {
        DHCPV4_POOL_SET_DEFAULTVALUE(pPool);
    }
    
    AnscZeroMemory( pDhcpv4->AliasOfPool, sizeof(pDhcpv4->AliasOfPool) );
    
    return returnStatus;
}

/***********************************************************************

 APIs for Object:

    DHCPv4.Server.Pool.{i}.StaticAddress.{i}.

    *  StaticAddress_GetEntryCount
    *  StaticAddress_GetEntry
    *  StaticAddress_AddEntry
    *  StaticAddress_DelEntry
    *  StaticAddress_GetParamBoolValue
    *  StaticAddress_GetParamIntValue
    *  StaticAddress_GetParamUlongValue
    *  StaticAddress_GetParamStringValue
    *  StaticAddress_SetParamBoolValue
    *  StaticAddress_SetParamIntValue
    *  StaticAddress_SetParamUlongValue
    *  StaticAddress_SetParamStringValue
    *  StaticAddress_Validate
    *  StaticAddress_Commit
    *  StaticAddress_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        StaticAddress_GetEntryCount
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
StaticAddress_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_POOL_LINK_OBJECT  pCxtLink          = (PCOSA_CONTEXT_POOL_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPS_POOL_FULL       pPool             = (PCOSA_DML_DHCPS_POOL_FULL)pCxtLink->hContext;

    return AnscSListQueryDepth( &pCxtLink->StaticAddressList );
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        StaticAddress_GetEntry
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
StaticAddress_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_CONTEXT_POOL_LINK_OBJECT  pCxtPoolLink         = (PCOSA_CONTEXT_POOL_LINK_OBJECT)hInsContext;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink             = NULL;
    PSINGLE_LINK_ENTRY              pSListEntry          = NULL;

    pSListEntry = AnscSListGetEntryByIndex(&pCxtPoolLink->StaticAddressList, nIndex);

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
        StaticAddress_AddEntry
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
StaticAddress_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_POOL_LINK_OBJECT  pCxtPoolLink         = (PCOSA_CONTEXT_POOL_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPS_POOL_FULL       pPool                = (PCOSA_DML_DHCPS_POOL_FULL)pCxtPoolLink->hContext;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink             = NULL;
    PCOSA_DML_DHCPS_SADDR           pDhcpStaticAddress   = NULL;
    CHAR                            tmpBuff[64]          = {0};
    PCOSA_DATAMODEL_DHCPV4          pDhcpv4              = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;
    
    /* We need ask from backend */
    pDhcpStaticAddress  = (PCOSA_DML_DHCPS_SADDR)AnscAllocateMemory( sizeof(COSA_DML_DHCPS_SADDR) );
    if ( !pDhcpStaticAddress )
    {
        goto EXIT2;
    }

    DHCPV4_STATICADDRESS_SET_DEFAULTVALUE(pDhcpStaticAddress);

    pCxtLink = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory( sizeof(COSA_CONTEXT_LINK_OBJECT) );
    if ( !pCxtLink )
    {
        goto EXIT1;
    }

    pCxtLink->hContext       = (ANSC_HANDLE)pDhcpStaticAddress;
    pCxtLink->hParentTable   = (ANSC_HANDLE)pCxtPoolLink;
    pCxtLink->bNew           = TRUE;
    
    if ( !++pCxtPoolLink->maxInstanceOfStaticAddress )
    {
        pCxtPoolLink->maxInstanceOfStaticAddress = 1;
    }
    pDhcpStaticAddress->InstanceNumber = pCxtPoolLink->maxInstanceOfStaticAddress;
    pCxtLink->InstanceNumber           = pDhcpStaticAddress->InstanceNumber; 
    *pInsNumber                        = pDhcpStaticAddress->InstanceNumber;

    _ansc_sprintf( pDhcpStaticAddress->Alias, "StaticAddress%d", pDhcpStaticAddress->InstanceNumber);
#if 1 //RDKB-EMULATOR
     char param_value[256] = {0};
     char param_name[256]  = {0};
     sprintf(param_value,"%ld",pDhcpStaticAddress->InstanceNumber);
     sprintf(param_name,PSM_DHCPV4_SERVER_POOL_STATICADDRESS_INSTANCENUMBER);
     PSM_Set_Record_Value2(bus_handle,g_Subsystem, param_name, ccsp_string, param_value);
#endif

    /* Put into our list */
    CosaSListPushEntryByInsNum(&pCxtPoolLink->StaticAddressList, (PCOSA_CONTEXT_LINK_OBJECT)pCxtLink);

    /* we recreate the configuration */
    CosaDhcpv4RegSetDhcpv4Info(pDhcpv4);

    return (ANSC_HANDLE)pCxtLink;    

EXIT1:
        
    AnscFreeMemory(pDhcpStaticAddress);
    
EXIT2:   
        
    return NULL;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        StaticAddress_DelEntry
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
StaticAddress_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_POOL_LINK_OBJECT  pCxtPoolLink         = (PCOSA_CONTEXT_POOL_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPS_POOL_FULL       pDhcsPool            = (PCOSA_DML_DHCPS_POOL_FULL)pCxtPoolLink->hContext;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink             = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_DML_DHCPS_SADDR           pDhcpStaticAddress   = (PCOSA_DML_DHCPS_SADDR)pCxtLink->hContext;
    PCOSA_DATAMODEL_DHCPV4          pDhcpv4              = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;

    CcspHalDHCPv4DeleteReservedClients(pDhcpStaticAddress);//RDKB-EMULATOR
    PSM_Del_ReversedIP_RecordValues(pDhcpStaticAddress->InstanceNumber);

    AnscTraceFlow(("%s: pool instance %d, addr instance %d\n", __FUNCTION__, pDhcsPool->Cfg.InstanceNumber, pDhcpStaticAddress->InstanceNumber));    
    if ( !pCxtLink->bNew )
    {
        returnStatus = CosaDmlDhcpsDelSaddr( NULL, pDhcsPool->Cfg.InstanceNumber, pDhcpStaticAddress->InstanceNumber );
        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            return returnStatus;
        }
    }

    /* Firstly we del this entry */
    if ( AnscSListPopEntryByLink(&pCxtPoolLink->StaticAddressList, &pCxtLink->Linkage) )
    {
        CosaDhcpv4RegSetDhcpv4Info(pDhcpv4);
        AnscFreeMemory(pCxtLink->hContext);
        AnscFreeMemory(pCxtLink);
    }
    
    return returnStatus;
        
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        StaticAddress_GetParamBoolValue
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
StaticAddress_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink             = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPS_SADDR           pDhcpStaticAddress   = (PCOSA_DML_DHCPS_SADDR)pCxtLink->hContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* collect value */
        *pBool  =  pDhcpStaticAddress->bEnabled;
        
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        StaticAddress_GetParamIntValue
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
StaticAddress_GetParamIntValue
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
        StaticAddress_GetParamUlongValue
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
StaticAddress_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink             = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPS_SADDR           pDhcpStaticAddress   = (PCOSA_DML_DHCPS_SADDR)pCxtLink->hContext;
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;
    COSA_DML_DHCPS_POOL_CFG         Cfg                  = {0};
    ANSC_IPV4_ADDRESS               IPRouter;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Yiaddr", TRUE))
    {
        *puLong = pDhcpStaticAddress->Yiaddr.Value;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        StaticAddress_GetParamStringValue
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
StaticAddress_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink             = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPS_SADDR           pDhcpStaticAddress   = (PCOSA_DML_DHCPS_SADDR)pCxtLink->hContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE))
    {
        /* collect value */
        if ( AnscSizeOfString(pDhcpStaticAddress->Alias) < *pUlSize)
        {
            AnscCopyString(pValue, pDhcpStaticAddress->Alias);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDhcpStaticAddress->Alias)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "Chaddr", TRUE))
    {
        /* collect value */
        if ( sizeof(pDhcpStaticAddress->Chaddr) <= *pUlSize)
        {
            _ansc_sprintf
                (
                    pValue,
                    "%02x:%02x:%02x:%02x:%02x:%02x",
                    pDhcpStaticAddress->Chaddr[0],
                    pDhcpStaticAddress->Chaddr[1],
                    pDhcpStaticAddress->Chaddr[2],
                    pDhcpStaticAddress->Chaddr[3],
                    pDhcpStaticAddress->Chaddr[4],
                    pDhcpStaticAddress->Chaddr[5]
                );
            pValue[17] = '\0'; 
            *pUlSize = AnscSizeOfString(pValue);
           
            return 0;
        }
        else
        {
            *pUlSize = sizeof(pDhcpStaticAddress->Chaddr);
            return 1;
        }
    }
    
    if( AnscEqualString(ParamName, "X_CISCO_COM_DeviceName", TRUE))
    {
        /* collect value */
        if ( AnscSizeOfString(pDhcpStaticAddress->DeviceName) < *pUlSize)
        {
            AnscCopyString(pValue, pDhcpStaticAddress->DeviceName);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDhcpStaticAddress->DeviceName)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_Comment", TRUE))
    {
        /* collect value */
        if ( AnscSizeOfString(pDhcpStaticAddress->Comment) < *pUlSize)
        {
            AnscCopyString(pValue, pDhcpStaticAddress->Comment);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDhcpStaticAddress->Comment)+1;
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
        StaticAddress_SetParamBoolValue
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
StaticAddress_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink             = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPS_SADDR           pDhcpStaticAddress   = (PCOSA_DML_DHCPS_SADDR)pCxtLink->hContext;
    PCOSA_CONTEXT_POOL_LINK_OBJECT  pPoolLink            = (PCOSA_CONTEXT_POOL_LINK_OBJECT)pCxtLink->hParentTable;
    PCOSA_DML_DHCPS_POOL_FULL       pPool                = (PCOSA_DML_DHCPS_POOL_FULL)pPoolLink->hContext;
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;
    BOOL bridgeInd = FALSE;
		
	is_usg_in_bridge_mode(&bridgeInd);
	if(bridgeInd)
		return(FALSE);

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        /* save update to backup */
        pDhcpStaticAddress->bEnabled  = bValue;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        StaticAddress_SetParamIntValue
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
StaticAddress_SetParamIntValue
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
        StaticAddress_SetParamUlongValue
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
StaticAddress_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink             = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPS_SADDR           pDhcpStaticAddress   = (PCOSA_DML_DHCPS_SADDR)pCxtLink->hContext;
    PCOSA_CONTEXT_POOL_LINK_OBJECT  pPoolLink            = (PCOSA_CONTEXT_POOL_LINK_OBJECT)pCxtLink->hParentTable;
    PCOSA_DML_DHCPS_POOL_FULL       pPool                = (PCOSA_DML_DHCPS_POOL_FULL)pPoolLink->hContext;
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;
    BOOL bridgeInd = FALSE;
		
	is_usg_in_bridge_mode(&bridgeInd);
	if(bridgeInd)
		return(FALSE);

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Yiaddr", TRUE))
    {
#if 1/*added by song to check whether the IP is in the range*/
        COSA_DML_DHCPS_POOL_CFG poolCfg;
        poolCfg.InstanceNumber = 1;
        CosaDmlDhcpsGetPoolCfg(NULL, &poolCfg);
        if(( ntohl(uValue)< ntohl(poolCfg.MinAddress.Value))||( ntohl(uValue)> ntohl(poolCfg.MaxAddress.Value) )){
            return(FALSE);
        }
#endif
        /* save update to backup */
        pDhcpStaticAddress->Yiaddr.Value  =  uValue;
#if 1 //RDKB-EMULATOR
        //PSM-ACCESS - To store static table entries in PSM Database.
        PSM_Set_ReversedIP_RecordValues(pDhcpStaticAddress);
        CcspHalDHCPv4ReservedClients(pDhcpStaticAddress);
#endif

        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        StaticAddress_SetParamStringValue
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
StaticAddress_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink             = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPS_SADDR           pDhcpStaticAddress   = (PCOSA_DML_DHCPS_SADDR)pCxtLink->hContext;
    PCOSA_CONTEXT_POOL_LINK_OBJECT  pPoolLink            = (PCOSA_CONTEXT_POOL_LINK_OBJECT)pCxtLink->hParentTable;
    int                             rc                   = -1;
    UINT                            chAddr[7]            = {'\0'}, zeroMac[6];
    BOOL bridgeInd = FALSE;
		
	is_usg_in_bridge_mode(&bridgeInd);
	if(bridgeInd)
		return(FALSE);

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE))
    {
        AnscCopyString(pPoolLink->AliasOfStaAddr, pDhcpStaticAddress->Alias);

        AnscCopyString(pDhcpStaticAddress->Alias, pString);

        return TRUE;
    }

    if( AnscEqualString(ParamName, "Chaddr", TRUE))
    {
        /* save update to a temp array - This is required as sscanf puts a NULL character at the end which we dont have space for */
        rc = _ansc_sscanf
                (
                    pString,
                    "%x:%x:%x:%x:%x:%x",
                    chAddr,
                    chAddr+1,
                    chAddr+2,
                    chAddr+3,
                    chAddr+4,
                    chAddr+5
                );

        if(rc != 6)
        {
            CcspTraceWarning(("MAC Address not proerly formatted \n"));
            return FALSE;
        }
        memset(zeroMac, 0, sizeof(zeroMac));
        if((chAddr[0] & 1)||(memcmp(zeroMac,chAddr,sizeof(zeroMac))==0))
            return(FALSE);
        /* Now copy the temp value to our back-up without NULL */ 
        pDhcpStaticAddress->Chaddr[0] = chAddr[0];
        pDhcpStaticAddress->Chaddr[1] = chAddr[1];
        pDhcpStaticAddress->Chaddr[2] = chAddr[2];
        pDhcpStaticAddress->Chaddr[3] = chAddr[3];
        pDhcpStaticAddress->Chaddr[4] = chAddr[4];
        pDhcpStaticAddress->Chaddr[5] = chAddr[5];

        return TRUE;
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_DeviceName", TRUE))
    {
        /* save update to backup */
	    if ( ( sizeof(pDhcpStaticAddress->DeviceName) - 1 )  > AnscSizeOfString(pString))
    	{
			AnscCopyString(pDhcpStaticAddress->DeviceName,pString);
			return TRUE;
    	}
		else
		{
			CcspTraceWarning(("'%s' value should be less than (%d) charecters\n", ParamName, ( sizeof(pDhcpStaticAddress->DeviceName) - 1 )));
		}
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_Comment", TRUE))
    {
        /* save update to backup */
        if ( sizeof(pDhcpStaticAddress->Comment) > AnscSizeOfString(pString))
        {
            AnscCopyString(pDhcpStaticAddress->Comment,pString);
            PSM_Set_ReversedIP_RecordValues(pDhcpStaticAddress);//RDKB-EMULATOR
            return TRUE;
        }
        else
        {
            CcspTraceWarning(("'%s' value should be less than (%d) charecters\n", ParamName, ( sizeof(pDhcpStaticAddress->Comment) - 1 )));
        }
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        StaticAddress_Validate
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
StaticAddress_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPS_SADDR           pDhcpStaAddr      = (PCOSA_DML_DHCPS_SADDR)pCxtLink->hContext;
    PCOSA_CONTEXT_POOL_LINK_OBJECT  pCxtPoolLink      = (PCOSA_CONTEXT_POOL_LINK_OBJECT)pCxtLink->hParentTable;
    PCOSA_DML_DHCPS_POOL_FULL       pPool             = (PCOSA_DML_DHCPS_POOL_FULL)pCxtPoolLink->hContext;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink2         = NULL;
    PCOSA_DML_DHCPS_SADDR           pDhcpStaAddr2     = NULL;
    PSINGLE_LINK_ENTRY              pSListEntry       = NULL;
    BOOL                            bFound            = FALSE;
    UCHAR                           strIP[32]         = {'\0'};
    UINT                            ip[4]             = {0};
    int                             rc                = -1;
    int                             i                 = 0;
    ULONG                           ipaddr, netmask, gateway;

#if 0 /*removed by song*/
    /* Parent hasn't set, we don't permit child is set.*/
    if ( pCxtPoolLink->bNew )
    {
#if COSA_DHCPV4_ROLLBACK_TEST        
        StaticAddress_Rollback(hInsContext);
#endif
        return FALSE;
    }
    if ( pCxtPoolLink->AliasOfStaAddr[0] )
    {
        /* save update to backup */
        bFound                = FALSE;
        pSListEntry           = AnscSListGetFirstEntry(&pCxtPoolLink->StaticAddressList);
        while( pSListEntry != NULL)
        {
            pCxtLink2         = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSListEntry);
            pSListEntry       = AnscSListGetNextEntry(pSListEntry);

            pDhcpStaAddr2  = (PCOSA_DML_DHCPS_SADDR)pCxtLink2->hContext;

            if( DHCPV4_STATICADDRESS_ENTRY_MATCH2(pDhcpStaAddr->Alias, pDhcpStaAddr2->Alias ) )
            {
                if ( (ANSC_HANDLE)pCxtLink2 == hInsContext )
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
            StaticAddress_Rollback(hInsContext);
#endif
            return FALSE;
        }
    }
    /* Make sure Static IP Address is properly formatted and isnt a network or multicast address */
    ipaddr = pDhcpStaAddr->Yiaddr.Value;
    netmask = pPool->Cfg.SubnetMask.Value;
    gateway = pPool->Cfg.IPRouters[0].Value;
    if (IPv4Addr_IsLoopback(ipaddr)
            || IPv4Addr_IsMulticast(ipaddr)
            || IPv4Addr_IsBroadcast(ipaddr, gateway, netmask)
            || IPv4Addr_IsNetworkAddr(ipaddr, gateway, netmask)
            || !IPv4Addr_IsSameNetwork(ipaddr, gateway, netmask))
    {
        CcspTraceWarning(("%s: Invalid Static IP Address \n", __FUNCTION__));
        _ansc_strcpy(pReturnParamName, "Yiaddr");
        return FALSE;
    }

    /* Make sure Static IP Address is not same as our GW address */
    if((pDhcpStaAddr->Yiaddr.Value == pPool->Cfg.IPRouters[0].Value))
    {
        CcspTraceWarning(("Static IP Address same as the GW \n"));
        _ansc_strcpy(pReturnParamName, "Yiaddr");
        return FALSE;
    }

    if((pDhcpStaAddr->Yiaddr.Value < pPool->Cfg.MinAddress.Value) || (pDhcpStaAddr->Yiaddr.Value > pPool->Cfg.MaxAddress.Value))
    {
        CcspTraceWarning(("Static IP Address not in Range \n"));
        _ansc_strcpy(pReturnParamName, "Yiaddr");
        return FALSE;
    }
#endif

    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        StaticAddress_Commit
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
StaticAddress_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink             = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPS_SADDR           pDhcpStaticAddress   = (PCOSA_DML_DHCPS_SADDR)pCxtLink->hContext;
    PCOSA_CONTEXT_POOL_LINK_OBJECT  pCxtPoolLink         = (PCOSA_CONTEXT_POOL_LINK_OBJECT)pCxtLink->hParentTable;
    PCOSA_DML_DHCPS_POOL_FULL       pPool                = (PCOSA_DML_DHCPS_POOL_FULL)pCxtPoolLink->hContext;
    PCOSA_DATAMODEL_DHCPV4          pDhcpv4              = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;

    if ( pCxtLink->bNew ){
	if(pDhcpStaticAddress->bEnabled==FALSE){
		pDhcpStaticAddress->ActiveFlag = FALSE;
		return(ANSC_STATUS_SUCCESS);
	}
        returnStatus = CosaDmlDhcpsAddSaddr(NULL, pPool->Cfg.InstanceNumber, pDhcpStaticAddress );

        if ( returnStatus == ANSC_STATUS_SUCCESS ){
            pCxtLink->bNew = FALSE;
            pDhcpStaticAddress->ActiveFlag = TRUE;
            CosaDhcpv4RegSetDhcpv4Info(pDhcpv4);
        }else{
            if ( pCxtPoolLink->AliasOfStaAddr[0] )
                AnscCopyString( pDhcpStaticAddress->Alias, pCxtPoolLink->AliasOfStaAddr );
        }
    }else{
	if(pDhcpStaticAddress->ActiveFlag==TRUE){
		if(pDhcpStaticAddress->bEnabled==FALSE){/*to del this entry from backend*/
			returnStatus = CosaDmlDhcpsDelSaddr( NULL, pPool->Cfg.InstanceNumber, pDhcpStaticAddress->InstanceNumber );
			if(returnStatus == ANSC_STATUS_SUCCESS)
				pDhcpStaticAddress->ActiveFlag = FALSE;
		}else{/*to update this entry*/
			returnStatus = CosaDmlDhcpsSetSaddr(NULL, pPool->Cfg.InstanceNumber, pDhcpStaticAddress);
		}
	}else{
		if(pDhcpStaticAddress->bEnabled==FALSE){
			return(ANSC_STATUS_SUCCESS);
		}else{/*Add this entry to backend*/
			returnStatus = CosaDmlDhcpsAddSaddr(NULL, pPool->Cfg.InstanceNumber, pDhcpStaticAddress );
			if(returnStatus == ANSC_STATUS_SUCCESS)
				pDhcpStaticAddress->ActiveFlag==TRUE;
		}
	}
    }
    
    AnscZeroMemory( pCxtPoolLink->AliasOfStaAddr, sizeof(pCxtPoolLink->AliasOfStaAddr) );
    return returnStatus;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        StaticAddress_Rollback
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
StaticAddress_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPS_SADDR           pDhcpStaAddr      = (PCOSA_DML_DHCPS_SADDR)pCxtLink->hContext;
    PCOSA_CONTEXT_POOL_LINK_OBJECT  pCxtPoolLink      = (PCOSA_CONTEXT_POOL_LINK_OBJECT)pCxtLink->hParentTable;
    PCOSA_DML_DHCPS_POOL_FULL       pPool             = (PCOSA_DML_DHCPS_POOL_FULL)pCxtPoolLink->hContext;

    if ( pCxtPoolLink->AliasOfStaAddr[0] )
        AnscCopyString( pDhcpStaAddr->Alias, pCxtPoolLink->AliasOfStaAddr );
#if 0/*Removed by song*/
    if ( !pCxtLink->bNew )
    {
        CosaDmlDhcpsGetSaddrbyInsNum(NULL, pPool->Cfg.InstanceNumber, pDhcpStaAddr);
    }
    else
    {
        DHCPV4_STATICADDRESS_SET_DEFAULTVALUE(pDhcpStaAddr);
    }

    AnscZeroMemory( pCxtPoolLink->AliasOfStaAddr, sizeof(pCxtPoolLink->AliasOfStaAddr) );
#endif    
    return returnStatus;
}


/***********************************************************************

 APIs for Object:

    DHCPv4.Server.Pool.{i}.Option.{i}.

    *  Option1_GetEntryCount
    *  Option1_GetEntry
    *  Option1_AddEntry
    *  Option1_DelEntry
    *  Option1_GetParamBoolValue
    *  Option1_GetParamIntValue
    *  Option1_GetParamUlongValue
    *  Option1_GetParamStringValue
    *  Option1_SetParamBoolValue
    *  Option1_SetParamIntValue
    *  Option1_SetParamUlongValue
    *  Option1_SetParamStringValue
    *  Option1_Validate
    *  Option1_Commit
    *  Option1_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Option1_GetEntryCount
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
Option1_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_POOL_LINK_OBJECT    pCxtLink          = (PCOSA_CONTEXT_POOL_LINK_OBJECT)hInsContext;

    return AnscSListQueryDepth( &pCxtLink->OptionList );
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Option1_GetEntry
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
Option1_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_CONTEXT_POOL_LINK_OBJECT    pCxtPoolLink         = (PCOSA_CONTEXT_POOL_LINK_OBJECT)hInsContext;
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
        Option1_AddEntry
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
Option1_AddEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG*                      pInsNumber
    )
{
    ANSC_STATUS                       returnStatus         = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_POOL_LINK_OBJECT    pCxtPoolLink         = (PCOSA_CONTEXT_POOL_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPS_POOL_FULL         pPool                = (PCOSA_DML_DHCPS_POOL_FULL)pCxtPoolLink->hContext;
    PCOSA_CONTEXT_LINK_OBJECT         pCxtLink             = NULL;
    PCOSA_DML_DHCPSV4_OPTION          pDhcpOption          = NULL;
    CHAR                              tmpBuff[64]          = {0};
    PCOSA_DATAMODEL_DHCPV4            pDhcpv4              = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;
    
    /* We need ask from backend */
    //printf("%s\n", __FUNCTION__);
    if(pPool->Cfg.InstanceNumber == 1)
    {
        AnscTraceFlow(("%s: not supporting addinging option for pool 1.\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    pDhcpOption  = (PCOSA_DML_DHCPSV4_OPTION)AnscAllocateMemory( sizeof(COSA_DML_DHCPSV4_OPTION) );
    if ( !pDhcpOption )
    {
        goto EXIT2;
    }

    DHCPV4_POOLOPTION_SET_DEFAULTVALUE(pDhcpOption);

    pCxtLink = (PCOSA_CONTEXT_LINK_OBJECT)AnscAllocateMemory( sizeof(COSA_CONTEXT_LINK_OBJECT) );
    if ( !pCxtLink )
    {
        goto EXIT1;
    }

    pCxtLink->hContext       = (ANSC_HANDLE)pDhcpOption;
    pCxtLink->hParentTable   = (ANSC_HANDLE)pCxtPoolLink;
    pCxtLink->bNew           = TRUE;
    
    //printf("%s: pCxtPoolLink->maxInstanceOfOption = %d\n", __FUNCTION__, pCxtPoolLink->maxInstanceOfOption);
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
    pCxtLink->bNew = FALSE;
    CosaDhcpv4RegSetDhcpv4Info(pDhcpv4);
    
    //printf("%s: option tag %d, option value %s\n", __FUNCTION__, pDhcpOption->Tag, pDhcpOption->Value);
    CosaDmlDhcpsAddOption(NULL, pPool->Cfg.InstanceNumber, pDhcpOption );

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
        Option1_DelEntry
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
Option1_DelEntry
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    )
{
    ANSC_STATUS                       returnStatus         = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_POOL_LINK_OBJECT    pCxtPoolLink         = (PCOSA_CONTEXT_POOL_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPS_POOL_FULL         pDhcpPool            = (PCOSA_DML_DHCPS_POOL_FULL)pCxtPoolLink->hContext;
    PCOSA_CONTEXT_LINK_OBJECT         pCxtLink             = (PCOSA_CONTEXT_LINK_OBJECT)hInstance;
    PCOSA_DML_DHCPSV4_OPTION          pDhcpOption          = (PCOSA_DML_DHCPSV4_OPTION)pCxtLink->hContext;
    PCOSA_DATAMODEL_DHCPV4            pDhcpv4              = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;

    AnscTraceFlow(("%s: pool instance %d, option instance %d\n", __FUNCTION__, pDhcpPool->Cfg.InstanceNumber, pDhcpOption->InstanceNumber));    
    //printf("%s: pool instance %d, option instance %d\n", __FUNCTION__, pDhcpPool->Cfg.InstanceNumber, pDhcpOption->InstanceNumber);
    if(pDhcpPool->Cfg.InstanceNumber == 1)
    {
        AnscTraceFlow(("%s: not supporting deleting option for pool 1.\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
    if ( !pCxtLink->bNew )
    {
        returnStatus = CosaDmlDhcpsDelOption( NULL, pDhcpPool->Cfg.InstanceNumber, pDhcpOption->InstanceNumber );
        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            return returnStatus;
        }
    }

    /* Firstly we del this entry */
    //printf("%s: remove entry.\n", __FUNCTION__);
    if ( AnscSListPopEntryByLink(&pCxtPoolLink->OptionList, &pCxtLink->Linkage) )
    {
//        if ( pCxtLink->bNew )
//            CosaDhcpv4RegSetDhcpv4Info(pDhcpv4);
        
        AnscFreeMemory(pCxtLink->hContext);
        AnscFreeMemory(pCxtLink);
    }
    
    return returnStatus;
        
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Option1_GetParamBoolValue
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
Option1_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV4_OPTION        pDhcpOption   = (PCOSA_DML_DHCPSV4_OPTION)pCxtLink->hContext;

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
        Option1_GetParamIntValue
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
Option1_GetParamIntValue
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
        Option1_GetParamUlongValue
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
Option1_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV4_OPTION        pDhcpOption   = (PCOSA_DML_DHCPSV4_OPTION)pCxtLink->hContext;

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
        Option1_GetParamStringValue
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
Option1_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV4_OPTION        pDhcpOption   = (PCOSA_DML_DHCPSV4_OPTION)pCxtLink->hContext;

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
            //printf("%s: size, %d, %d\n", __FUNCTION__, AnscSizeOfString(pDhcpOption->Value), *pUlSize);
            AnscCopyString(pValue, pDhcpOption->Value);
            return 0;
        }
        else
        {
            //printf("%s: size two, %d, %d\n", __FUNCTION__, AnscSizeOfString(pDhcpOption->Value), *pUlSize);
            *pUlSize = AnscSizeOfString(pDhcpOption->Value)+1;
            
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
        Option1_SetParamBoolValue
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
Option1_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT         pCxtLink        = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV4_OPTION          pDhcpOption     = (PCOSA_DML_DHCPSV4_OPTION)pCxtLink->hContext;

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
        Option1_SetParamIntValue
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
Option1_SetParamIntValue
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
        Option1_SetParamUlongValue
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
Option1_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCxtLink      = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV4_OPTION        pDhcpOption   = (PCOSA_DML_DHCPSV4_OPTION)pCxtLink->hContext;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Tag", TRUE) )
    {
        /* save update to backup */        
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
        Option1_SetParamStringValue
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
Option1_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PCOSA_CONTEXT_LINK_OBJECT         pCxtLink             = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV4_OPTION          pDhcpOption          = (PCOSA_DML_DHCPSV4_OPTION)pCxtLink->hContext;
    PCOSA_CONTEXT_POOL_LINK_OBJECT    pPoolLink            = (PCOSA_CONTEXT_POOL_LINK_OBJECT)pCxtLink->hParentTable;

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


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Option1_Validate
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
Option1_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PCOSA_CONTEXT_LINK_OBJECT         pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV4_OPTION          pDhcpPoolOption   = (PCOSA_DML_DHCPSV4_OPTION)pCxtLink->hContext;
    PCOSA_CONTEXT_POOL_LINK_OBJECT    pCxtPoolLink      = (PCOSA_CONTEXT_POOL_LINK_OBJECT)pCxtLink->hParentTable;
    PCOSA_CONTEXT_LINK_OBJECT         pCxtLink2         = NULL;
    PCOSA_DML_DHCPSV4_OPTION          pDhcpPoolOption2  = NULL;
    PSINGLE_LINK_ENTRY                pSListEntry       = NULL;
    BOOL                              bFound            = FALSE;

    /* Parent hasn't set, we don't permit child is set.*/
    if ( pCxtPoolLink->AliasOfOption[0] )
    {
        /* save update to backup */
        bFound                = FALSE;
        pSListEntry           = AnscSListGetFirstEntry(&pCxtPoolLink->OptionList);
        while( pSListEntry != NULL)
        {
            pCxtLink2         = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSListEntry);
            pSListEntry       = AnscSListGetNextEntry(pSListEntry);

            pDhcpPoolOption2  = (PCOSA_DML_DHCPSV4_OPTION)pCxtLink2->hContext;

            if( DHCPV4_POOLOPTION_ENTRY_MATCH(pDhcpPoolOption->Alias, pDhcpPoolOption2->Alias ) )
            {
                if ( (ANSC_HANDLE)pCxtLink2 == hInsContext )
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
            return FALSE;
        }
    }

    
    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Option1_Commit
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
Option1_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                       returnStatus         = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_LINK_OBJECT         pCxtLink             = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV4_OPTION          pDhcpOption          = (PCOSA_DML_DHCPSV4_OPTION)pCxtLink->hContext;
    PCOSA_CONTEXT_POOL_LINK_OBJECT    pCxtPoolLink         = (PCOSA_CONTEXT_POOL_LINK_OBJECT)pCxtLink->hParentTable;
    PCOSA_DML_DHCPS_POOL_FULL         pPool                = (PCOSA_DML_DHCPS_POOL_FULL)pCxtPoolLink->hContext;
    PCOSA_DATAMODEL_DHCPV4            pDhcpv4              = (PCOSA_DATAMODEL_DHCPV4)g_pCosaBEManager->hDhcpv4;

    //printf("%s: %d \n", __FUNCTION__, pCxtLink->bNew);
    if ( pCxtLink->bNew )
    {
        returnStatus = CosaDmlDhcpsAddOption(NULL, pPool->Cfg.InstanceNumber, pDhcpOption );

        if ( returnStatus == ANSC_STATUS_SUCCESS )
        {
            pCxtLink->bNew = FALSE;

            CosaDhcpv4RegSetDhcpv4Info(pDhcpv4);
        }
        else
        {
            DHCPV4_POOLOPTION_SET_DEFAULTVALUE(pDhcpOption);

            if ( pCxtPoolLink->AliasOfOption[0] )
                AnscCopyString( pDhcpOption->Alias, pCxtPoolLink->AliasOfOption );
        }
    }
    else
    {
        returnStatus = CosaDmlDhcpsSetOption(NULL, pPool->Cfg.InstanceNumber, pDhcpOption);

        if ( returnStatus != ANSC_STATUS_SUCCESS)
        {
            CosaDmlDhcpsGetOptionbyInsNum(NULL, pPool->Cfg.InstanceNumber, pDhcpOption);
        }
    }
    
    AnscZeroMemory( pCxtPoolLink->AliasOfOption, sizeof(pCxtPoolLink->AliasOfOption) );
    
    return returnStatus;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Option1_Rollback
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
Option1_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_CONTEXT_LINK_OBJECT         pCxtLink          = (PCOSA_CONTEXT_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV4_OPTION          pDhcpPoolOption   = (PCOSA_DML_DHCPSV4_OPTION)pCxtLink->hContext;
    PCOSA_CONTEXT_POOL_LINK_OBJECT    pCxtPoolLink      = (PCOSA_CONTEXT_POOL_LINK_OBJECT)pCxtLink->hParentTable;
    PCOSA_DML_DHCPS_POOL_FULL         pPool             = (PCOSA_DML_DHCPS_POOL_FULL)pCxtPoolLink->hContext;

    //printf("%s:\n", __FUNCTION__);
    if ( pCxtPoolLink->AliasOfOption[0] )
        AnscCopyString( pDhcpPoolOption->Alias, pCxtPoolLink->AliasOfOption );

    if ( !pCxtLink->bNew )
    {
        CosaDmlDhcpsGetOptionbyInsNum(NULL, pPool->Cfg.InstanceNumber, pDhcpPoolOption);
    }
    else
    {
        DHCPV4_POOLOPTION_SET_DEFAULTVALUE(pDhcpPoolOption);
    }

    AnscZeroMemory( pCxtPoolLink->AliasOfOption, sizeof(pCxtPoolLink->AliasOfOption) );
    
    return returnStatus;
}


/***********************************************************************

 APIs for Object:

    DHCPv4.Server.Pool.{i}.Client.{i}.

    *  Client2_GetEntryCount
    *  Client2_GetEntry
    *  Client2_IsUpdated
    *  Client2_Synchronize
    *  Client2_GetParamBoolValue
    *  Client2_GetParamIntValue
    *  Client2_GetParamUlongValue
    *  Client2_GetParamStringValue
    *  Client2_SetParamBoolValue
    *  Client2_SetParamIntValue
    *  Client2_SetParamUlongValue
    *  Client2_SetParamStringValue
    *  Client2_Validate
    *  Client2_Commit
    *  Client2_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Client2_GetEntryCount
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
Client2_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_POOL_LINK_OBJECT    pCxtLink          = (PCOSA_CONTEXT_POOL_LINK_OBJECT)hInsContext;

    return pCxtLink->NumberOfClient;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Client2_GetEntry
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
Client2_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_CONTEXT_POOL_LINK_OBJECT    pCxtLink          = (PCOSA_CONTEXT_POOL_LINK_OBJECT)hInsContext;

    if ( nIndex >= pCxtLink->NumberOfClient )
        return NULL;
    
    *pInsNumber  = nIndex + 1; 
    
    return (ANSC_HANDLE)&pCxtLink->pClientList[nIndex];
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Client2_IsUpdated
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
Client2_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_CONTEXT_POOL_LINK_OBJECT    pCxtLink        = (PCOSA_CONTEXT_POOL_LINK_OBJECT)hInsContext;
    BOOL                              bIsUpdated   = TRUE;

    /* 
        We can use one rough granularity interval to get whole table in case 
        that the updating is too frequent.
        */
    if ( ( AnscGetTickInSeconds() - pCxtLink->PreviousVisitTimeOfClient ) < COSA_DML_DHCPV4_ACCESS_INTERVAL_POOLCLIENT )
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
        Client2_Synchronize
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
Client2_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                       returnStatus    = ANSC_STATUS_SUCCESS;    
    PCOSA_CONTEXT_POOL_LINK_OBJECT    pCxtLink        = (PCOSA_CONTEXT_POOL_LINK_OBJECT)hInsContext;
    PCOSA_DML_DHCPSV4_CLIENT          pDhcpsClient    = NULL;
    PCOSA_DML_DHCPSV4_CLIENTCONTENT   pDhcpsClientContent = NULL;
    ULONG                             uIndex           = 0;
    ULONG                             count           = 0;
    BOOL bridgeInd = FALSE;

    /* Free all */
    if ( pCxtLink->pClientList )
    {
        AnscFreeMemory(pCxtLink->pClientList);
        pCxtLink->pClientList    = NULL;

        pDhcpsClientContent = pCxtLink->pClientContentList;
        for( uIndex =0; uIndex < pCxtLink->NumberOfClient; uIndex++ )
        {
            if ( pDhcpsClientContent[uIndex].pIPAddress )
            {
                AnscFreeMemory(pDhcpsClientContent[uIndex].pIPAddress);
                pDhcpsClientContent[uIndex].pIPAddress = NULL;
            }
            
            if ( pDhcpsClientContent[uIndex].pOption )
            {
                AnscFreeMemory(pDhcpsClientContent[uIndex].pOption);            
                pDhcpsClientContent[uIndex].pOption = NULL;
            }
        }

        AnscFreeMemory(pCxtLink->pClientContentList);
        pCxtLink->pClientContentList    = NULL;

        pCxtLink->NumberOfClient = 0;
    }

    is_usg_in_bridge_mode(&bridgeInd);
	if(bridgeInd){
        pCxtLink->NumberOfClient= 0;
        returnStatus = ANSC_STATUS_SUCCESS;
    }else{
        /* Get all again */
        returnStatus = CosaDmlDhcpsGetClient
                    (
                        NULL,
                        pCxtLink->InstanceNumber,
                        &pDhcpsClient,
                        &pDhcpsClientContent,
                        &count
                    );

        if ( returnStatus == ANSC_STATUS_SUCCESS ){
            pCxtLink->pClientList        = pDhcpsClient;
            pCxtLink->pClientContentList = pDhcpsClientContent;
            pCxtLink->NumberOfClient  = count;
        }
    }

    return returnStatus;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Client2_GetParamBoolValue
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

/*
 *  TBC --  Define the function prototype in the header file!!!
 */
extern void CosaDmlDhcpsGetPrevClientNumber(ULONG ulPoolInstanceNumber, ULONG *pNumber);

BOOL
Client2_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;    
    PCOSA_DML_DHCPSV4_CLIENT        pDhcpsClient    = (PCOSA_DML_DHCPSV4_CLIENT)hInsContext;
    PCOSA_DML_DHCPSV4_CLIENTCONTENT PClientContent    = NULL;
    ULONG n;
    
    PClientContent = (PCOSA_DML_DHCPSV4_CLIENTCONTENT)CosaDhcpv4GetClientContentbyClient(hInsContext);

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Active", TRUE) ){
        n = 0;
        CosaDmlDhcpsGetPrevClientNumber(1, &n);
        if(n<=6){
         /* collect value */
            if(!strncmp("172.16.12.", _ansc_inet_ntoa(*((struct in_addr*)&(PClientContent->pIPAddress[0].IPAddress))),10)) 
             	returnStatus = CosaDmlDhcpsPing(&(PClientContent->pIPAddress[0]));
            else
            	returnStatus = CosaDmlDhcpsARPing(&(PClientContent->pIPAddress[0]));
            if ( returnStatus == ANSC_STATUS_SUCCESS ){
                *pBool = TRUE;
            }else{
                *pBool = FALSE;
            }
        }else /*there are many clients, don't ping the clients*/
            *pBool = pDhcpsClient->Active;
        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Client2_GetParamIntValue
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
Client2_GetParamIntValue
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
        Client2_GetParamUlongValue
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
Client2_GetParamUlongValue
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
        Client2_GetParamStringValue
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
Client2_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;    
    PCOSA_DML_DHCPSV4_CLIENT        pDhcpsClient    = (PCOSA_DML_DHCPSV4_CLIENT)hInsContext;

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

    if( AnscEqualString(ParamName, "X_CISCO_COM_Comment", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pDhcpsClient->X_CISCO_COM_Comment) < *pUlSize)
        {
            AnscCopyString(pValue, pDhcpsClient->X_CISCO_COM_Comment);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDhcpsClient->X_CISCO_COM_Comment)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_Interface", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pDhcpsClient->X_CISCO_COM_Interface) < *pUlSize)
        {
            AnscCopyString(pValue, pDhcpsClient->X_CISCO_COM_Interface);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDhcpsClient->X_CISCO_COM_Interface)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_HostName", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pDhcpsClient->X_CISCO_COM_HostName) < *pUlSize)
        {
            AnscCopyString(pValue, pDhcpsClient->X_CISCO_COM_HostName);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDhcpsClient->X_CISCO_COM_HostName)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "ClassId", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pDhcpsClient->ClassId) < *pUlSize)
        {
            AnscCopyString(pValue, pDhcpsClient->ClassId);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDhcpsClient->ClassId)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "Chaddr", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pDhcpsClient->Chaddr) < *pUlSize)
        {
            AnscCopyString(pValue, pDhcpsClient->Chaddr);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pDhcpsClient->Chaddr)+1;
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
        Client2_SetParamBoolValue
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
Client2_SetParamBoolValue
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
        Client2_SetParamIntValue
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
Client2_SetParamIntValue
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
        Client2_SetParamUlongValue
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
Client2_SetParamUlongValue
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
        Client2_SetParamStringValue
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
Client2_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        /* save update to backup */
        return FALSE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        Client2_Validate
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
Client2_Validate
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
        Client2_Commit
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
Client2_Commit
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
        Client2_Rollback
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
Client2_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    return 0;
}

/***********************************************************************

 APIs for Object:

    DHCPv4.Server.Pool.{i}.Client.{i}.IPv4Address.{i}.

    *  IPv4Address2_GetEntryCount
    *  IPv4Address2_GetEntry
    *  IPv4Address2_IsUpdated
    *  IPv4Address2_Synchronize
    *  IPv4Address2_GetParamBoolValue
    *  IPv4Address2_GetParamIntValue
    *  IPv4Address2_GetParamUlongValue
    *  IPv4Address2_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv4Address2_GetEntryCount
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
IPv4Address2_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DML_DHCPSV4_CLIENTCONTENT   PClientContent    = NULL;
    
    PClientContent = (PCOSA_DML_DHCPSV4_CLIENTCONTENT)CosaDhcpv4GetClientContentbyClient(hInsContext);

    if ( PClientContent )
    {
        return PClientContent->NumberofIPAddress;
    }
    else
        return 0;    
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        IPv4Address2_GetEntry
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
IPv4Address2_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    ANSC_STATUS                       returnStatus      = ANSC_STATUS_SUCCESS;
    PCOSA_DML_DHCPSV4_CLIENTCONTENT   PClientContent    = NULL;
    
    PClientContent = (PCOSA_DML_DHCPSV4_CLIENTCONTENT)CosaDhcpv4GetClientContentbyClient(hInsContext);

    if ( PClientContent )
    {
        *pInsNumber  = nIndex + 1; 
        return &PClientContent->pIPAddress[nIndex];
    }
    else
        return NULL;    

}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        IPv4Address2_GetParamBoolValue
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
IPv4Address2_GetParamBoolValue
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
        IPv4Address2_GetParamIntValue
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
IPv4Address2_GetParamIntValue
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
        IPv4Address2_GetParamUlongValue
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
IPv4Address2_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    ANSC_STATUS                            returnStatus    = ANSC_STATUS_SUCCESS;
    PCOSA_DML_DHCPSV4_CLIENT_IPADDRESS     pIPAddress      = (PCOSA_DML_DHCPSV4_CLIENT_IPADDRESS)hInsContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "IPAddress", TRUE) )
    {
        /* collect value */
        *puLong = pIPAddress->IPAddress;

        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        IPv4Address2_GetParamStringValue
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
IPv4Address2_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_DML_DHCPSV4_CLIENT_IPADDRESS     pIPAddress      = (PCOSA_DML_DHCPSV4_CLIENT_IPADDRESS)hInsContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "LeaseTimeRemaining", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pIPAddress->LeaseTimeRemaining) < *pUlSize)
        {
            AnscCopyString(pValue, pIPAddress->LeaseTimeRemaining);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pIPAddress->LeaseTimeRemaining)+1;
            return 1;
        }
    }

    if( AnscEqualString(ParamName, "X_CISCO_COM_LeaseTimeCreation", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pIPAddress->X_CISCO_COM_LeaseTimeCreation) < *pUlSize)
        {
            AnscCopyString(pValue, pIPAddress->X_CISCO_COM_LeaseTimeCreation);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pIPAddress->X_CISCO_COM_LeaseTimeCreation)+1;
            return 1;
        }
    }
    
    if( AnscEqualString(ParamName, "X_CISCO_COM_LeaseTimeDuration", TRUE) )
    {
#if 0//LNT_EMU
        CosaDmlDhcpsGetLeaseTimeDuration(pIPAddress);
#endif
        /* collect value */
        if ( AnscSizeOfString(pIPAddress->X_CISCO_COM_LeaseTimeDuration) < *pUlSize)
        {
            AnscCopyString(pValue, pIPAddress->X_CISCO_COM_LeaseTimeDuration);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pIPAddress->X_CISCO_COM_LeaseTimeDuration)+1;
            return 1;
        }
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/***********************************************************************

 APIs for Object:

    DHCPv4.Server.Pool.{i}.Client.{i}.Option.{i}.

    *  Option2_GetEntryCount
    *  Option2_GetEntry
    *  Option2_IsUpdated
    *  Option2_Synchronize
    *  Option2_GetParamBoolValue
    *  Option2_GetParamIntValue
    *  Option2_GetParamUlongValue
    *  Option2_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Option2_GetEntryCount
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
Option2_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DML_DHCPSV4_CLIENTCONTENT   PClientContent    = NULL;
    
    PClientContent = (PCOSA_DML_DHCPSV4_CLIENTCONTENT)CosaDhcpv4GetClientContentbyClient(hInsContext);

    if ( PClientContent )
    {
        return PClientContent->NumberofOption;
    }
    else
        return 0;    
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        Option2_GetEntry
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
Option2_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DML_DHCPSV4_CLIENTCONTENT   PClientContent    = NULL;
    
    PClientContent = (PCOSA_DML_DHCPSV4_CLIENTCONTENT)CosaDhcpv4GetClientContentbyClient(hInsContext);

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
        Option2_GetParamBoolValue
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
Option2_GetParamBoolValue
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
        Option2_GetParamIntValue
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
Option2_GetParamIntValue
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
        Option2_GetParamUlongValue
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
Option2_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DML_DHCPSV4_CLIENT_OPTION        pIPv4Option     = (PCOSA_DML_DHCPSV4_CLIENT_OPTION)hInsContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Tag", TRUE) )
    {
        /* collect value */
        *puLong  = pIPv4Option->Tag;

        return TRUE;
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        Option2_GetParamStringValue
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
Option2_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_DML_DHCPSV4_CLIENT_OPTION        pIPv4Option     = (PCOSA_DML_DHCPSV4_CLIENT_OPTION)hInsContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Value", TRUE) )
    {
        /* collect value */
        if ( AnscSizeOfString(pIPv4Option->Value) < *pUlSize)
        {
            AnscCopyString(pValue, pIPv4Option->Value);
            return 0;
        }
        else
        {
            *pUlSize = AnscSizeOfString(pIPv4Option->Value)+1;
            return 1;
        }
    }


    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

